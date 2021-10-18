#if defined(_MSC_VER)
#define NOMINMAX
#pragma once
#endif

#ifndef PBRT_SRC_SHAPE_TRIANGLE_H_
#define PBRT_SRC_SHAPE_TRIANGLE_H_

#include "pbrt.h"
#include "geometry.h"

namespace pbrt {

class Triangle: public Shape {

};

} // namespace pbrt

#endif // PBRT_SRC_SHAPE_TRIANGLE_H_