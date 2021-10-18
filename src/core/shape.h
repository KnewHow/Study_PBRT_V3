#if defined(_MSC_VER)
#define NOMINMAX
#pragma once
#endif

#ifndef PBRT_SRC_CORE_SHAPE_H_
#define PBRT_SRC_CORE_SHAPE_H_

#include "pbrt.h"
#include "geometry.h"

namespace pbrt {

/**
 * This is a base class for all shapes in render, 
 * all shapes should inherit from it, and implement virtual methods by themselves.
*/
class Shape {
public:
    /**
     * Try do a ray intersect with a shape.
     * @param ray the ray
     * @param isect if the ray intersect with the shape, return some surface information about the interaction point.
     * @return if they are intersection, return true, otherwise return false
    */
    virtual bool Intersection(const Ray &ray, SurfaceInteraction &isect) const = 0; 
    
    /**
     * Try do a ray intersect with a shape, just care if they can intersect, don't care others.
     * @return if they are, return ture, otherwise return false
    */
    virtual bool IntersectionP(const Ray &ray) const = 0;

    /**
     * get surface of the shape
     * @return the surface area of the shape
    */
    virtual Float Area() const = 0;

    /**
     * Sample the shape
     * @param pdf the param will be returned represent Probability density function
    */
    virtual void Sample(Float& pdf) const = 0;

    /**
     * Get the world Bound of the shape.
     * @return The world bould of the shape
    */
    virtual Bounds3f worldBound() const = 0;
};

} // namespace pbrt

#endif // PBRT_SRC_CORE_SHAPE_H_