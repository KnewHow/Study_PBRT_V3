#include "memory.h"

namespace pbrt {

// Memory Allocation Functions
void *AllocAligned(size_t size) {
#if defined(PBRT_HAVE__ALIGNED_MALLOC)
    return _aligned_malloc(size, PBRT_L1_CACHE_LINE_SIZE);
#elif defined(PBRT_HAVE_POSIX_MEMALIGN)
    void *ptr;
    if (posix_memalign(&ptr, PBRT_L1_CACHE_LINE_SIZE, size) != 0) ptr = nullptr;
    return ptr;
#else
    return memalign(PBRT_L1_CACHE_LINE_SIZE, size);
#endif
}

} // namespace pbrt