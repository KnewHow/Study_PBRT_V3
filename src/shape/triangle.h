#if defined(_MSC_VER)
#define NOMINMAX
#pragma once
#endif

#ifndef PBRT_SRC_SHAPE_TRIANGLE_H_
#define PBRT_SRC_SHAPE_TRIANGLE_H_

#include "pbrt.h"
#include "geometry.h"
#include "shape.h"

namespace pbrt {

/**
 * A collections to record all geometric information of a triangle mesh.
 * Then all trinagle will shared the memory. By this way, we will avoid redundant store point and save memory 
*/
struct TriangleMesh {
    TriangleMesh(int nTriangles, int nVertices, const std::vector<int> &vIndices, const std::vector<Point3f> &ps, const std::vector<Normal3f> &ns);
    int nTriangles, nVertices;
    std::vector<int> vertexIndices; // the index of vetex in points and normal, it size is nTrinagle * 3, use three indices as a group to represent a trinagle.
    std::unique_ptr<Point3f[]> p; // points 
    std::unique_ptr<Normal3f[]> n;
};

class Triangle: public Shape {
public:
    Triangle(const std::shared_ptr<TriangleMesh> &mesh, int triNumber): mesh(mesh) {
        v = &mesh->vertexIndices[3 * triNumber];
    }
    virtual bool Intersection(const Ray &ray, Float &tHit, SurfaceInteraction &isect) const override; 
    virtual bool IntersectionP(const Ray &ray) const override;
    virtual Float Area() const override;
    virtual Interaction Sample(Float &pdf) const;
    virtual Bounds3f WorldBound() const override;
private:
    const std::shared_ptr<TriangleMesh> mesh;
    const int *v; // the pointer point the vertice index, you can use v[0] v[1] v[2] to access the index in mesh->p[]
};

} // namespace pbrt

#endif // PBRT_SRC_SHAPE_TRIANGLE_H_