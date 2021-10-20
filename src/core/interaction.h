#if defined(_MSC_VER)
#define NOMINMAX
#pragma once
#endif

#ifndef PBRT_SRC_CORE_INTERACTION_H_
#define PBRT_SRC_CORE_INTERACTION_H_

#include "pbrt.h"
#include "geometry.h"

namespace pbrt {

/**
 * The interaction information, it just provide some basic information
*/
struct Interaction {
    Interaction(){}
    Interaction(const Point3f &p, const Normal3f &n): p(p), n(n){}
    Point3f p;
    Normal3f n;
};

/**
 * The inseraction in surface, expect some basic information, 
 * it will provide some other information about the surface, 
 * like as the shape the interection in and the material in the surface.
*/
struct SurfaceInteraction: public Interaction {
    SurfaceInteraction(){}
    SurfaceInteraction(const Shape *shape, const Point3f &p, const Normal3f &n):Interaction(p, n), shape(shape){}
    const Shape *shape;
    const Primitive *primitive; // it will record the material the suferface is
};


} // namespace pbrt

#endif // PBRT_SRC_CORE_INTERACTION_H_
