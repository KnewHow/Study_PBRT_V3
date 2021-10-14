#ifndef PBRT_CORE_PBRT_H
#define PBRT_CORE_PBRT_H



#include <iostream>
#include <cmath>
#include <glog/logging.h>

#ifdef PBRT_FLOAT_AS_DOUBLE
    typedef double Float;
#else
    typedef float Float;
#endif

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

} // namespace pbrt



#endif