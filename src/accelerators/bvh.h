#if defined(_MSC_VER)
#define NOMINMAX
#pragma once
#endif

#ifndef PBRT_SRC_ACCELERATORS_BVH_H_
#define PBRT_SRC_ACCELERATORS_BVH_H_

#include "pbrt.h"
#include "primitive.h"
#include "geometry.h"

namespace pbrt {

struct LinearBVHNode;
struct BVHBuildNode;
struct BVHPrimitiveInfo;


/**
 * an accelerator base the BVH(Bounding Volume Hierarchies)
*/
class BVHAccel : public Aggregate {
public:
    /**
     * BVH will split primitive with three way: SHA(Surface Area Heuristic), Middle(Split with median) and EqualCounts(Split into same count)
    */
    enum class SplitMethod {
        SAH, Middle, EqualCounts
    };
    BVHAccel(std::vector<std::shared_ptr<Primitive>> ps, SplitMethod sm = SplitMethod::SAH, int maxPrimsInNode = 1);
    ~BVHAccel();
    virtual bool Intersect(const Ray &ray, SurfaceInteraction &isect) const override;
    virtual bool IntersectP(const Ray &ray) const override;
    virtual Bounds3f WorldBound() const override;

private:
    std::shared_ptr<BVHBuildNode> RecursiveBuild(std::vector<BVHPrimitiveInfo> &primitiveInfos, int begin, int end, int &totalNodes, std::vector<std::shared_ptr<Primitive>> &orderedPrimitives);
    int FlattenBVHTree(const std::shared_ptr<BVHBuildNode> node, int &offset);
    
    std::vector<std::shared_ptr<Primitive>> primitives; // It store all actual primitve, they are the leaf nodes in the BVH tree, and its index in the vector will be recorded to search
    LinearBVHNode *nodes; // a head point for a LinearBVHNode array, we transform a tree node into a linear array, it will get good performance in traversal tree
    const int maxPrimitivesInNode; // the maximax capacity of primitives in each leaf node
    SplitMethod method; // Which split method, it will be used in split algorithms
};

} // namespace pbrt



#endif // PBRT_SRC_ACCELERATORS_BVH_H_