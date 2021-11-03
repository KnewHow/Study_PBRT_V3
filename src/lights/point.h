#if defined(_MSC_VER)
#define NOMINMAX
#pragma once
#endif

#ifndef PBRT_SRC_LIGHTS_POINT_H_
#define PBRT_SRC_LIGHTS_POINT_H_

#include "light.h"
#include "geometry.h"

namespace pbrt {
class PointLight: public Light {
public:
    PointLight(const Point3f &p, const RGBAf &I): position(p),I(I){}
    const Point3f position;
    const RGBAf I;
};

} // namespace pbrt

#endif // PBRT_SRC_LIGHTS_POINT_H_