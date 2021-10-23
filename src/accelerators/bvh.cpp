
#include "bvh.h"

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

BVHAccel::BVHAccel(std::vector<std::shared_ptr<Primitive>> ps, SplitMethod sm, int maxPrimsInNode): primitives(std::move(ps)), method(sm), maxPrimitivesInNode(maxPrimsInNode) {
    if(primitives.size() == 0) return;
    std::vector<BVHPrimitiveInfo> infos(primitives.size());
    for(int i = 0; i < primitives.size(); ++i) 
        infos[i] = BVHPrimitiveInfo(i, primitives[i]->WorldBound());
    std::vector<std::shared_ptr<Primitive>> orderedPrimitives;
    orderedPrimitives.reserve(primitives.size());
    int totalNodes = 0;
    std::shared_ptr<BVHBuildNode> root = RecursiveBuild(infos, 0, infos.size() - 1, totalNodes, orderedPrimitives);
    primitives.swap(orderedPrimitives);
}


} // namespace pbrt