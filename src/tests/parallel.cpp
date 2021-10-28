#include <chrono>

#include "tests/gtest/gtest.h"
#include "pbrt.h"
#include "parallel.h"
#include "stringprint.h"
#include "clock.h"

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
    LOG(INFO) << "[AtomicFloat] v is: " << StringPrintf("%3.2f", (Float)v);
    EXPECT_TRUE(compare_float((Float)v, (Float)times * 3));
}



TEST(ParallelForLoopExecutor, ParallelFor1D) {
    AtomicFloat v = (AtomicFloat)0.0;
    std::chrono::milliseconds begin, end;
    int times = 20000;
    ParallelForLoopExecutor::Init(std::nullopt);
    begin = getCurrentMilliseconds();
    ParallelForLoopExecutor::ParallelFor1D([&](int64_t n){
        v.Add(1.0);
        for(int i = 0; i < 10000; ++i);
    }, times, 1000);
    end = getCurrentMilliseconds();
    LOG(INFO) << "[ParallelFor1D] parallel calclulate cost: " << (end - begin).count() << " millseconds.";
    EXPECT_TRUE(compare_float((Float)v, (Float)times));
    ParallelForLoopExecutor::Clean();
    
    v = 0.0;
    begin = getCurrentMilliseconds();
    for(int i = 0; i < times; ++i) {
        v.Add(1.0);
        for(int i = 0; i < 10000; ++i);
    }
    end = getCurrentMilliseconds();
    LOG(INFO) << "[ParallelFor1D] serial calclulate cost: " << (end - begin).count() << " millseconds.";
    EXPECT_TRUE(compare_float((Float)v, (Float)times));
}

TEST(ParallelForLoopExecutor, ParallelFor2D) {
    AtomicFloat v = (AtomicFloat)0.0;
    ParallelForLoopExecutor::Init(std::nullopt);
    std::chrono::milliseconds begin, end;
    Point2i count(1001, 1001);
    begin = getCurrentMilliseconds();
    ParallelForLoopExecutor::ParallelFor2D([&](Point2i p){
        v.Add(1.0);
        for(int i = 0; i < 100; ++i);
    }, count, 2000);
    end = getCurrentMilliseconds();
    ParallelForLoopExecutor::Clean();
    LOG(INFO) << "[ParallelFor2D] parallel calclulate cost: " << (end - begin).count() << " millseconds.";
    EXPECT_TRUE(compare_float((Float)v, (Float)count.x * count.y));

    v = 0.0;
    begin = getCurrentMilliseconds();
    for(int i = 0; i < count.x; ++i) {
        for(int j = 0; j < count.y; ++j) {
            v.Add(1.0);
            for(int i = 0; i < 100; ++i);
        }
       
    }
    end = getCurrentMilliseconds();
    LOG(INFO) << "[ParallelFor2D] serial calclulate cost: " << (end - begin).count() << " millseconds.";
    EXPECT_TRUE(compare_float((Float)v, (Float)count.x * count.y));
}