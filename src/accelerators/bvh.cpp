
#include "bvh.h"
#include "memory.h"

namespace pbrt {

/**
 * A linear BVH node, use it, we can avoid recursive traversal BVH tree, it will prompt performance
*/
struct LinearBVHNode {
    Bounds3f bound; // The bound of the node
    union
    {
        int primitiveOffset; // Only for leaf node, it represents the staring index in the `primitives`
        int secondChildOffset; // Only for interior node, it represents the second node index in the `primitives`. becase we will use DFS(deep first search) travelsal for the BVH tree, the first child will be recorded with current interior node index + 1, it is knew for us. So we only need to record the second child index in interior node
    };
    uint16_t nPrimitives; // Only for leaf node, it represents how many primitives in the leaf node. If we have primitiveOffset and nPrimitives, we can travelsal all primitives in the node.
    uint8_t axis; // Only for interior, it represents which axies the node had been splited.
    uint8_t pad[1]; // make sure 32 bytes for whole struct, it will more effective for CPU
};

/**
 * A intermediate node for recursive build bvh node, then they will be transformed as LinearBVHNodes
*/
struct BVHBuildNode {
    void InitLeaf(int first, int n, const Bounds3f &b){
        primitiveOffset = first;
        nPrimitives = n;
        bound = b;
        children[0] = children[1] = nullptr;
    }
    void InitInterior(int axis, std::shared_ptr<BVHBuildNode> c0, std::shared_ptr<BVHBuildNode> c1) {
        splitAxis = axis;
        children[0] = c0;
        children[1] = c1;
        bound = Union(c0->bound, c1->bound);
    }
    Bounds3f bound;
    std::shared_ptr<BVHBuildNode> children[2]; // Only for interior node
    int splitAxis; // Only for interior node
    int primitiveOffset; // Only for leaf node
    int nPrimitives; // Only for leaf node
};

/**
 * A simplied version of primitive, recording the index and bounding info.
 * It will be sorted to divide into suitable two subtree. 
 * So a important propery in it is to record the original index in the primitives.
*/
struct BVHPrimitiveInfo {
    BVHPrimitiveInfo(){}
    BVHPrimitiveInfo(int i, const Bounds3f& b) {
        index = i;
        bound = b;
        centroid = (b.pMin + b.pMax) / 2;
    }
    int index;
    Bounds3f bound;
    Point3f centroid; // The centroid in the bound box
};

/**
 * A bucket store for SHA 
*/
struct BucketInfo {
    int count = 0; // How many primitives the bucket contains
    Bounds3f bound; // The bound of these primitives
};

BVHAccel::BVHAccel(std::vector<std::shared_ptr<Primitive>> ps, SplitMethod sm, int maxPrimsInNode): primitives(std::move(ps)), method(sm), maxPrimitivesInNode(maxPrimsInNode) {
    if(primitives.size() == 0) return;
    std::vector<BVHPrimitiveInfo> infos(primitives.size());
    for(int i = 0; i < primitives.size(); ++i) 
        infos[i] = BVHPrimitiveInfo(i, primitives[i]->WorldBound());
    std::vector<std::shared_ptr<Primitive>> orderedPrimitives;
    orderedPrimitives.reserve(primitives.size());
    int totalNodes = 0;
    std::shared_ptr<BVHBuildNode> root = RecursiveBuild(infos, 0, infos.size(), totalNodes, orderedPrimitives);
    primitives.swap(orderedPrimitives);
    nodes = AllocAligned<LinearBVHNode>(totalNodes);
    int offset = 0;
    FlattenBVHTree(root, offset);
    DCHECK_EQ(totalNodes, offset);
}

BVHAccel::~BVHAccel() {
    free(nodes);
}

std::shared_ptr<BVHBuildNode> BVHAccel::RecursiveBuild(std::vector<BVHPrimitiveInfo> &primitiveInfos, int begin, int end, int &totalNodes, std::vector<std::shared_ptr<Primitive>> &orderedPrimitives) {
    DCHECK_NE(begin, end);
    std::shared_ptr<BVHBuildNode> node = std::make_shared<BVHBuildNode>();
    ++totalNodes;
    int nPrimitives = end - begin;
    
    Bounds3f bounds;
    for(int i = begin; i < end; ++i)
        bounds = Union(bounds, primitiveInfos[i].bound);
    
    if(nPrimitives <= 1) { // build leaf node if left primitive less than 1
        int firstOffset = orderedPrimitives.size();
        for(int i = begin; i < end; ++i) {
            int index = primitiveInfos[i].index;
            orderedPrimitives.push_back(primitives[index]);
        }
        node->InitLeaf(firstOffset, nPrimitives, bounds);
        return node;
    }

    Bounds3f centroidBounds;
    for(int i = begin; i < end; ++i) 
        centroidBounds = Union(centroidBounds, primitiveInfos[i].centroid);
    int dim = centroidBounds.MaximumExtent();
    int mid = (begin + end) / 2;

    if(centroidBounds.pMax[dim] == centroidBounds.pMin[dim]) { // if all primitive with same centroid bounds, put them together as leaf node
        int firstOffset = orderedPrimitives.size();
        for(int i = begin; i < end; ++i) {
            int index = primitiveInfos[i].index;
            orderedPrimitives.push_back(primitives[index]);
        }
        node->InitLeaf(firstOffset, nPrimitives, bounds);
        return node;
    }

    switch (method) {
        case SplitMethod::Middle: {
            Float pmid = (centroidBounds.pMin[dim] + centroidBounds.pMax[dim]) / 2;
            BVHPrimitiveInfo *ptrMid = std::partition(&primitiveInfos[begin], &primitiveInfos[end - 1] + 1, [dim, pmid](const BVHPrimitiveInfo &info){
                return info.centroid[dim] < pmid;
            });
            mid = ptrMid - &primitiveInfos[0];
            if(mid != begin && mid != end) break;
            // if use above way don't split primivite, try follow method
        }
        case SplitMethod::EqualCounts: {
            mid = (begin + end) / 2;
            std::nth_element(&primitiveInfos[begin], &primitiveInfos[mid], &primitiveInfos[end - 1] + 1, [dim](const BVHPrimitiveInfo &a, const BVHPrimitiveInfo &b){
                return a.centroid[dim] < b.centroid[dim];
            });
            break;
        }
        case SplitMethod::SAH: // for SAH and defualt, both use SAH
        default: {
            if(nPrimitives <= SHA_THRESHOLD) { // if primitive amount less than SHA_THRESHOLD, don't use SAH， just use EqualCounts
                mid = (begin + end) / 2;
                std::nth_element(&primitiveInfos[begin], &primitiveInfos[mid], &primitiveInfos[end - 1] + 1, [dim](const BVHPrimitiveInfo &a, const BVHPrimitiveInfo &b){
                    return a.centroid[dim] < b.centroid[dim];
                });
                break;
            }

            BucketInfo buckets[SHA_N_BUCKETS];
            for(int i = begin; i < end; ++i) {
                int b = centroidBounds.Offset(primitiveInfos[i].centroid)[dim] * SHA_N_BUCKETS;
                if(b == SHA_N_BUCKETS) --b;
                DCHECK_GE(b, 0);
                DCHECK_LT(b, SHA_N_BUCKETS);
                ++buckets[b].count;
                buckets[b].bound = Union(buckets[b].bound, primitiveInfos[i].bound);
            }

            Float cost[SHA_N_BUCKETS-1]; 
            for(int i = 0; i < SHA_N_BUCKETS - 1; ++i) {
                int c0 = 0, c1 = 0;
                Bounds3f b0, b1;
                for(int j = 0; j <= i; ++j) {
                    c0 += buckets[j].count;
                    b0 = Union(b0, buckets[j].bound);
                }
                for(int j = i + 1; j < SHA_N_BUCKETS; ++j) {
                    c1 += buckets[j].count;
                    b1 = Union(b1, buckets[j].bound);
                }
                cost[i] = 1 + (c0 * b0.SurfaceArea() + c1 * b1.SurfaceArea()) / bounds.SurfaceArea();
            }

            int minimumIndex = 0;
            Float minimumCost = cost[0];
            for(int i = 1; i < SHA_N_BUCKETS; ++i) {
                if(cost[i] < minimumCost) {
                    minimumCost = cost[i];
                    minimumIndex = i;
                }
            }

            Float leafCost = nPrimitives;
            if(nPrimitives > maxPrimitivesInNode || minimumCost < leafCost) { // if primitive amouts greater than maxPrimitivesInNode or minimumCost less than leafCost(traversal all primitives spend time)
                BVHPrimitiveInfo *ptrMid = std::partition(&primitiveInfos[begin], &primitiveInfos[end -1] + 1, [&](const BVHPrimitiveInfo &info){
                    int b = centroidBounds.Offset(info.centroid)[dim] * SHA_N_BUCKETS;
                    if(b == SHA_N_BUCKETS) --b;
                    DCHECK_GE(b, 0);
                    DCHECK_LT(b, SHA_N_BUCKETS);
                    return b <= minimumIndex;
                });
                mid = ptrMid - &primitiveInfos[0];
            } else { // other wise put them together as a leaf node
                int firstOffset = orderedPrimitives.size();
                for(int i = begin; i < end; ++i) {
                    int index = primitiveInfos[i].index;
                    orderedPrimitives.push_back(primitives[index]);
                }
                node->InitLeaf(firstOffset, nPrimitives, bounds);
                return node;
            }
            break;
        }
    }
    node->InitInterior(dim, 
                       RecursiveBuild(primitiveInfos, begin, mid, totalNodes, orderedPrimitives),
                       RecursiveBuild(primitiveInfos, mid, end, totalNodes, orderedPrimitives));
    return node;
}

} // namespace pbrt