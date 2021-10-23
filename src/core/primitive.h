#if defined(_MSC_VER)
#define NOMINMAX
#pragma once
#endif

#ifndef PBRT_SRC_CORE_PRIMITIVE_H_
#define PBRT_SRC_CORE_PRIMITIVE_H_

#include "pbrt.h"
#include "geometry.h"
#include "interaction.h"
#include "shape.h"

namespace pbrt {

/**
 * A primitve basic class build a bridge between the geometric shape and shading subsystem.
 * As we know, geometic shape only contains geometric information, they don't have shading information
 * like material that renderder need in render time.
 * By this way, we can let `Primitive` combine them together
*/
class Primitive {
public:
    /**
     * Ray intersect with the primitve, in this method, in order to find the minimum hit point,
     * we should update the tMax in the ray to filter the bigger hit point.
     * You'd better use the shape that the primitive contains to implemnt this method, not recommand implment directly.
    */
    virtual bool Intersect(const Ray &ray, SurfaceInteraction &isect) const = 0;

    /**
     * The method is same with above, just we don't care the hit point information.
     * We just need to whether they are intersect and don't need to update the tMax value
    */
    virtual bool IntersectP(const Ray &ray) const = 0;

    /**
     * Get the material of the primitive
    */
    virtual std::shared_ptr<Material> GetMaterial() const = 0;

    /**
     * Get the world bound of the primitive
    */
    virtual Bounds3f WorldBound() const = 0;
};

class GeometicPrimitive: public Primitive {
public:
    GeometicPrimitive(const std::shared_ptr<Shape> &shape, const std::shared_ptr<Material> &material): shape(shape), material(material){}
    virtual bool Intersect(const Ray &ray, SurfaceInteraction &isect) const override;
    virtual bool IntersectP(const Ray &ray) const override;
    virtual std::shared_ptr<Material> GetMaterial() const override;
    virtual Bounds3f WorldBound() const override;
private:
    std::shared_ptr<Shape> shape;
    std::shared_ptr<Material> material;
};

} // namespace pbrt

#endif // PBRT_SRC_CORE_PRIMITIVE_H_