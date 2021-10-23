#if defined(_MSC_VER)
#define NOMINMAX
#pragma once
#endif

#ifndef PBRT_SRC_CORE_MEMORY_H_
#define PBRT_SRC_CORE_MEMORY_H_

#include "pbrt.h"

namespace pbrt {

void *AllocAligned(size_t size);
template <typename T>
T *AllocAligned(size_t count) {
    return (T *)AllocAligned(count * sizeof(T));
}

} // namespace pbrt

#endif // PBRT_SRC_CORE_MEMORY_H_