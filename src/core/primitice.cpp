#include "primitive.h"

namespace pbrt {

bool GeometicPrimitive::Intersect(const Ray &ray, SurfaceInteraction &isect) const {
    Float tHit;
    if(!shape->Intersection(ray, tHit, isect)) return false;
    ray.tMax = tHit;
    isect.primitive = this;
    return true;
}

bool GeometicPrimitive::IntersectP(const Ray &ray) const {
    return shape->IntersectionP(ray);
}

std::shared_ptr<Material> GeometicPrimitive::GetMaterial() const {
    return material;
}

Bounds3f GeometicPrimitive::WorldBound() const {
    return shape->WorldBound();
}


} // namespace pbrt