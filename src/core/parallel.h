#if defined(_MSC_VER)
#define NOMINMAX
#pragma once
#endif

#ifndef PBRT_SRC_CORE_PARALLEL_H_
#define PBRT_SRC_CORE_PARALLEL_H_

#include <atomic>
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <stack>

#include "pbrt.h"
#include "geometry.h"
#include "stats.h"

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

/**
 * A Paralle for loop task struct, it store the function which need to parallel execute.
 * Actually we use a link list to store these task, when a new task come in, we always insert it
 * in the head of the list. Why do that? Because the parallel for loop maybe be executed nested,
 * so use this way, we can guarantee the inner foor loop can be executed first. 
 * Actuall we don't support nested for loop parallel execute, so don't try it. But we also design it.
*/
class ParallelForLoopTask {
public:
    ParallelForLoopTask(std::function<void(int64_t)> func, int64_t maxIndex, int chunk)
        :func1(std::move(func)), maxIndex(maxIndex), chunkSize(chunk), nX(-1) {
        nextIndex = 0;
        activeWorkers = 0;
        next = nullptr;
    }
    ParallelForLoopTask(std::function<void(Point2i)> func, const Point2i count, int chunk = 1)
        :func2(std::move(func)), maxIndex(count.x * count.y), chunkSize(chunk), nX(count.x) {
        nextIndex = 0;
        activeWorkers = 0;
        next = nullptr;
    }
    bool isFinish() const {
        return nextIndex >= maxIndex && activeWorkers == 0;
    }

    const std::function<void(int64_t)> func1; // A 1D-function will be parallel executed, it has a integer as the parameter.
    const std::function<void(Point2i)> func2; // A 2D-function will be parallel executed, it has a point2i as the parameter.
    const int64_t maxIndex; // The maximum index we can arrive.
    const int chunkSize; // Each thread can conatins how many chunk.
    int64_t nextIndex; // Next starting execute index.
    int activeWorkers; // The amount of active worker threads.
    ParallelForLoopTask *next; // A pointer point next task. You can see it as a linklist;
    const int nX; // the row number of X. This variable will be used in 2D-function, represent the rows of x. If we knwo current index, we can get x coordinats with: index % nX, and get y coordinates with: index / nX; 
};

/**
 * Before multiply threads parallel execute tasks, each thread needs to do some pre-process work separate.
 * So a condition is required that we should let all thread wait until all thread complete pre-process work.
 * The barrie we design is for this. Each thread should hold a shared_ptr for this, and when these threads complete the pre-process work,
 * they can reset it to make the barrier ineffective.
*/
class Barrier {
public:
    Barrier(int activeThreads): activeThreads(activeThreads){ DCHECK_GT(activeThreads, 0); }
    ~Barrier() { DCHECK_EQ(activeThreads, 0); }
    void Wait() {
        std::unique_lock<std::mutex> lock(mutex);
        DCHECK_GT(activeThreads, 0);
        if(--activeThreads == 0) // if all thead complete pre-process, notify all, otherwise wait for others
            cv.notify_all();
        else
            cv.wait(lock, [&](){return activeThreads == 0;});
    }

private:
    std::mutex mutex; // A mutex let thread execute serial when do some sync operations
    std::condition_variable cv; // A condition variable let thread block until some condition satisty
    int activeThreads; // The amount of active threads
};


/**
 * A parallel exctutor
*/
class ParallelForLoopExecutor {
private:
    ParallelForLoopExecutor(){};
    ~ParallelForLoopExecutor();
public:
    /**
     * Init ParallelForLoopExecutor, before call other mehtods, you must call this method first.
     * We recommand you call this method once, then call other methods. If you has some special requirements, you can call it by youself.
     * @param maxAvailableThreads The Amount of available thread, if it is assigned, use it.
     * Otherwise use the max available threads the OS can support.
    */
    static void Init(std::optional<int> maxAvailableThreads);

    /**
     * Parallel execute 2D-function, we only guarentee parallel to execute, not do any sycn operations.
     * If there some shared memory in the func, make sure threads safe by yourself.
     * @param func A 2D-function will be parallel executed.
     * @param count Use a point2i use represent how many points will be executed. The amout is equal: count.x * count.y.
     *              We will execute the point from (0, 0) to (count.x count.y)
    */
    static void ParallelFor2D(std::function<void(Point2i)> func, const Point2i &count, int chunkSize = 1);
    
    /**
     * Parallel execute 1D-function, we only guarentee parallel to execute, not do any sycn operations. 
     * If there some shared memory in the func, make sure threads safe by yourself.
     * @param func A 1D-function will be parallel executed.
     * @param count The amount of how many interators will be executed. we will execute the for loop from 0 to the count;
     * @param chunkSize How many chunks each thread will be executed for one time.
    */
    static void ParallelFor1D(std::function<void(int64_t)> func, int64_t count, int chunkSize);

    
    static void MergeWorkerThreadStats();

    static void PrintStats(FILE *dest);
    static void ClearStats();
    /**
     * Clean thread, when the program terminate, you can call this method to do some clean operations 
    */
    static void Clean();

private: 
    static void WorkerThreadFunc(int index, std::shared_ptr<Barrier> barrier);
    static int NumSystemCores() { return std::max(1u, std::thread::hardware_concurrency()); }
    static void ReportThreadStats();

    static int nThreads;
    static std::vector<std::thread> threads; // Store all thread with a thread pool
    static ParallelForLoopTask *tasks; // A task list, you can see it as a head of link list;
    static std::mutex taskMutex; // A mutex to guarentee all task sync operation
    static std::condition_variable taskCV; // Let all worker threads wait for a condition;
    static bool isShutdownThreads; // If we shutdown the thread pool

    // Below variable is relative of statistics
    static StatsAccumulator GLOBAL_STATS_ACCUMULATOR; // a global stats accumulator
    static std::atomic<bool> isReportWorkerStats; // Whether execute statistic  
    static std::atomic<int> reporterCount; // how many thread is stating, if the variable is zero, we will finish statistic work
    static std::condition_variable reportDoneCV; // let main thread wait until all threads complete statistic work
    static std::mutex reportMutex; // a mutex do some sync when do some statistic work
};


} // namespace pbrt

#endif // PBRT_SRC_CORE_PARALLEL_H_