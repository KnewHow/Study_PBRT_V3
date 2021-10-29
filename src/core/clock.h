#if defined(_MSC_VER)
#define NOMINMAX
#pragma once
#endif

#ifndef PBRT_SRC_CORE_CLOCK_H_
#define PBRT_SRC_CORE_CLOCK_H_

#include <chrono>
#include <ctime>

namespace pbrt {

inline std::chrono::milliseconds getCurrentMilliseconds() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
}



} // namespace pbrt

#endif // PBRT_SRC_CORE_CLOCK_H_