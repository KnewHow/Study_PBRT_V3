#if defined(_MSC_VER)
#define NOMINMAX
#pragma once
#endif

#ifndef PBRT_CORE_PBRT_H
#define PBRT_CORE_PBRT_H

#include <iostream>
#include <vector>
#include <memory>
#include <cmath>
#include <limits>
#include <algorithm>
#include <random>
#include <optional>
#include <glog/logging.h>

#include <string.h>

#ifdef PBRT_HAVE_MALLOC_H
#include <malloc.h>  // for _alloca, memalign
#endif
#ifdef PBRT_HAVE_ALLOCA_H
#include <alloca.h>
#endif

#ifdef PBRT_FLOAT_AS_DOUBLE
    typedef double Float;
#else
    typedef float Float;
#endif

#ifdef _MSC_VER
#define MaxFloat std::numeric_limits<Float>::max()
#define Infinity std::numeric_limits<Float>::infinity()
#else
static PBRT_CONSTEXPR Float MaxFloat = std::numeric_limits<Float>::max();
static PBRT_CONSTEXPR Float Infinity = std::numeric_limits<Float>::infinity();
#endif

#ifdef _MSC_VER
#define MachineEpsilon (std::numeric_limits<Float>::epsilon() * 0.5)
#else
static PBRT_CONSTEXPR Float MachineEpsilon =
    std::numeric_limits<Float>::epsilon() * 0.5;
#endif

#ifndef PBRT_L1_CACHE_LINE_SIZE
  #define PBRT_L1_CACHE_LINE_SIZE 64
#endif

static PBRT_CONSTEXPR Float ShadowEpsilon = 0.0001f;
static PBRT_CONSTEXPR Float Pi = 3.14159265358979323846;
static PBRT_CONSTEXPR Float InvPi = 0.31830988618379067154;
static PBRT_CONSTEXPR Float Inv2Pi = 0.15915494309189533577;
static PBRT_CONSTEXPR Float Inv4Pi = 0.07957747154594766788;
static PBRT_CONSTEXPR Float PiOver2 = 1.57079632679489661923;
static PBRT_CONSTEXPR Float PiOver4 = 0.78539816339744830961;
static PBRT_CONSTEXPR Float Sqrt2 = 1.41421356237309504880;

namespace pbrt {

// Global Forward Declarations
template<typename T>
class Vector2;
template<typename T>
class Vector3; 
template <typename T>
class Point2;
template <typename T>
class Point3;
template <typename T>
class Normal3;
template <typename T>
class Bounds2;
template <typename T>
class Bounds3;
class Ray;
class Interaction;
class SurfaceInteraction;
class Primitive;
class Shape;
class Transform;
class Film;
class Material;

inline bool compare_float(const Float &p1, const Float &v2) {
    return std::abs(p1 - v2) < ShadowEpsilon;
}

inline float get_random_Float() {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_real_distribution<Float> dist(0.0, 1.0);
    return dist(rng);
}

inline Float Lerp(Float t, Float v0, Float v1) { return (1 - t) * v0 + t * v1; }

inline Float gamma(int n) { // generate a infinitely
    return (n * MachineEpsilon) / (1 - n * MachineEpsilon);
}

template <typename T, typename U, typename V>
inline T Clamp(T val, U low, V high) {
    if (val < low)
        return low;
    else if (val > high)
        return high;
    else
        return val;
}

inline uint32_t FloatToBits(float f) {
    uint32_t ui;
    memcpy(&ui, &f, sizeof(float));
    return ui;
}

inline float BitsToFloat(uint32_t ui) {
    float f;
    memcpy(&f, &ui, sizeof(uint32_t));
    return f;
}

inline uint64_t FloatToBits(double f) {
    uint64_t ui;
    memcpy(&ui, &f, sizeof(f));
    return ui;
}

inline double BitsToFloat(uint64_t ui) {
    double f;
    memcpy(&f, &ui, sizeof(uint64_t));
    return f;
}

inline Float DegTorad(Float degree) {
    return degree * M_PI / 180.0;
}

} // namespace pbrt



#endif