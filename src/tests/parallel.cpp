#include "tests/gtest/gtest.h"
#include "pbrt.h"
#include "parallel.h"
#include "stringprint.h"
#include <chrono>


using namespace pbrt;

void increment(AtomicFloat &v, int times) {
    for(int i = 0; i < times; i++) {
        v.Add(1);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

TEST(AtomicFloat, ParallelTest) {
    int times = 100;
    AtomicFloat v = (AtomicFloat)0.0;
    std::thread t1(increment, std::ref(v), times);
    std::thread t2(increment, std::ref(v), times);
    std::thread t3(increment, std::ref(v), times);
    t1.join();
    t2.join();
    t3.join();
    LOG(INFO) << "v is: " << StringPrintf("%3.2f", (Float)v);
    EXPECT_TRUE(compare_float((Float)v, (Float)times * 3));
    //AtomicFloat f(1.0);
}