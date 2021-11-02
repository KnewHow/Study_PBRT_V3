#if defined(_MSC_VER)
#define NOMINMAX
#pragma once
#endif

#ifndef PBRT_SRC_CORE_MATERIAL_H_
#define PBRT_SRC_CORE_MATERIAL_H_

#include "pbrt.h"
#include "spectrum.h"

namespace pbrt {

class Material{
public:
    Material(){};
    Material(const RGBAf &kd, const RGBAf &ks): kd(kd), ks(ks){}
    RGBAf kd;
    RGBAf ks;
};

} // namespace pbrt

#endif // PBRT_SRC_CORE_MATERIAL_H_