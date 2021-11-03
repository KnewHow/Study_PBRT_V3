#if defined(_MSC_VER)
#define NOMINMAX
#pragma once
#endif

#ifndef PBRT_SRC_CORE_LIGHT_H_
#define PBRT_SRC_CORE_LIGHT_H_

#include "pbrt.h"
#include "geometry.h"
#include "spectrum.h"

namespace pbrt {

/**
 * A light basic class
*/
class Light {
};

} // namespace pbrt

#endif // PBRT_SRC_CORE_LIGHT_H_