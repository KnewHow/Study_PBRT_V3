#if defined(_MSC_VER)
#define NOMINMAX
#pragma once
#endif

#ifndef PBRT_SRC_CORE_MATERIAL_H_
#define PBRT_SRC_CORE_MATERIAL_H_

#include "pbrt.h"

namespace pbrt {

template <typename T>
class RGBA {
public:
    RGBA(): R(0), G(0), B(0), A(0){}
    RGBA(T R, T G, T B, T A): R(R), G(G), B(B), A(A){}
    RGBA &operator=(const RGBA &rgba) {
        R = rgba.R;
        G = rgba.G;
        B = rgba.B;
        A = rgba.A;
        return *this;
    }
    T R, G, B, A;
};

typedef RGBA<Float> RGBAf;
typedef RGBA<int> RGBAi;

class Material{
public:
    Material(){};
    Material(const RGBAf &kd, const RGBAf &ks): kd(kd), ks(ks){}
    RGBAf kd;
    RGBAf ks;
};

} // namespace pbrt

#endif // PBRT_SRC_CORE_MATERIAL_H_