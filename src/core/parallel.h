#if defined(_MSC_VER)
#define NOMINMAX
#pragma once
#endif

#ifndef PBRT_SRC_CORE_PARALLEL_H_
#define PBRT_SRC_CORE_PARALLEL_H_

#include "pbrt.h"

#include <atomic>
#include <thread>

namespace pbrt {

class AtomicFloat {
public:
    explicit AtomicFloat(Float v) { bits = FloatToBits(v); }
    operator Float() const { return BitsToFloat(bits); }
    float operator=(Float v) {
        bits = FloatToBits(v);
        return v;
    }
    void Add(Float v) {
        #ifdef PBRT_FLOAT_AS_DOUBLE
            uint64_t oldBits = bits, newBits;
        #else 
            uint32_t oldBits = bits, newBits;
        #endif
        do {
           newBits = FloatToBits(BitsToFloat(oldBits) + v);
        } while (!bits.compare_exchange_weak(oldBits, newBits));
    }
private:
#ifdef PBRT_FLOAT_AS_DOUBLE
    std::atomic<uint64_t> bits;
#else
    std::atomic<uint32_t> bits;
#endif
};

} // namespace pbrt

#endif // PBRT_SRC_CORE_PARALLEL_H_