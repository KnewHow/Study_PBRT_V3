#include "parallel.h"

namespace pbrt {

int ParallelForLoopExecutor::nThreads = 0;
std::vector<std::thread> ParallelForLoopExecutor::threads;
ParallelForLoopTask* ParallelForLoopExecutor::tasks = nullptr;
std::mutex ParallelForLoopExecutor::taskMutex;
std::condition_variable ParallelForLoopExecutor::taskCV;
bool ParallelForLoopExecutor::isShutdownThreads = false;

std::atomic<bool> ParallelForLoopExecutor::isReportWorkerStats = false;
std::atomic<int> ParallelForLoopExecutor::reporterCount = 0;
std::condition_variable ParallelForLoopExecutor::reportDoneCV;
std::mutex ParallelForLoopExecutor::reportMutex;
StatsAccumulator ParallelForLoopExecutor::GLOBAL_STATS_ACCUMULATOR;

void ParallelForLoopExecutor::Init(std::optional<int> maxAvailableThreads) {
    DCHECK_EQ(threads.size(), 0);
    tasks = nullptr;
    isShutdownThreads = false;
    isReportWorkerStats = false;
    nThreads = maxAvailableThreads.has_value() ? maxAvailableThreads.value() : NumSystemCores();
    std::shared_ptr<Barrier> barrier = std::make_shared<Barrier>(nThreads);
    for(int i = 0; i < nThreads - 1; i++) { // we reserve main thread, don't put it into the thread pool
        threads.push_back(std::thread(WorkerThreadFunc, i + 1, barrier));
    }
    // main thread wait
    barrier->Wait();
}


void ParallelForLoopExecutor::WorkerThreadFunc(int index, std::shared_ptr<Barrier> barrier) {
    LOG(INFO) << "Start execute worker thread: " << index;
    // do something pre-process here
    barrier->Wait();
    barrier.reset(); // release barrier

    std::unique_lock<std::mutex> lock(taskMutex); // Below code will be execute serial
    while(!isShutdownThreads) {
        if(isReportWorkerStats) {
            ReportThreadStats();
            if(--reporterCount == 0)
                reportDoneCV.notify_one(); // wake up main thread
            taskCV.wait(lock); // then sleep again
        } else if(!tasks) {
            taskCV.wait(lock);
        } else {
            ParallelForLoopTask &task = *tasks;
            int64_t startIndex = task.nextIndex;
            int64_t endIndex = std::min(startIndex + task.chunkSize, task.maxIndex);
            task.nextIndex = endIndex;
            ++task.activeWorkers;
            if(task.nextIndex == task.maxIndex) 
                tasks = task.next;
            
            lock.unlock(); // Below code will be execute parallel
            for(int i = startIndex; i < endIndex; ++i) {
                if(task.func1) {
                    task.func1(i);
                } else {
                    DCHECK(task.func2);
                    task.func2(Point2i(i % task.nX, i / task.nX));
                }
            }

            lock.lock(); // Below code will be executed serial
            --task.activeWorkers;
            if(task.isFinish()) taskCV.notify_all();
        }
    }
    LOG(INFO) << "Exit worker thread: " << index;
}

void ParallelForLoopExecutor::ParallelFor2D(std::function<void(Point2i)> func, const Point2i &count, int chunkSize) {
    DCHECK(threads.size() > 0 || nThreads == 1);
    if(threads.empty() || count.x * count.y <= 1) { // if no extra threads or amout is too samll, do task use main thread direct.   
        for(int y = 0; y < count.y; ++y)
            for(int x = 0; x < count.x; ++x)
                func(Point2i(x, y));
        return;
    }
    
    ParallelForLoopTask task(func, count, chunkSize);
    
    {
        std::lock_guard<std::mutex> lock(taskMutex);  // insert new task to linklist head
        task.next = tasks;
        tasks = &task;
    }

    std::unique_lock<std::mutex> lock(taskMutex);
    taskCV.notify_all();
    while(!task.isFinish()) { // main thread join executing task
        int64_t startIndex = task.nextIndex;
        int64_t endIndex = std::min(startIndex + task.chunkSize, task.maxIndex);
        task.nextIndex = endIndex;
        ++task.activeWorkers;
        if(task.nextIndex == task.maxIndex) 
            tasks = task.next;
        
        lock.unlock();
        for(int i = startIndex; i < endIndex; ++i) {
            if(task.func1) {
                task.func1(i);
            } else {
                DCHECK(task.func2);
                task.func2(Point2i(i % task.nX, i / task.nX));
            }
        }
        
        lock.lock();
        --task.activeWorkers;
    }

}

void ParallelForLoopExecutor::ParallelFor1D(std::function<void(int64_t)> func, int64_t count, int chunkSize) {
    DCHECK(threads.size() > 0 || nThreads == 1);
    if(threads.empty() || count <= 1) { // if no extra threads or amout is too samll, do task use main thread direct.   
        for(int64_t i = 0; i < count; ++i)
            func(i);
        return;
    }
    
    ParallelForLoopTask task(func, count,chunkSize);
    
    {
        std::lock_guard<std::mutex> lock(taskMutex);  // insert new task to linklist head
        task.next = tasks;
        tasks = &task;
    }

    std::unique_lock<std::mutex> lock(taskMutex);
    taskCV.notify_all();
    while(!task.isFinish()) { // main thread join executing task
        int64_t startIndex = task.nextIndex;
        int64_t endIndex = std::min(startIndex + task.chunkSize, task.maxIndex);
        task.nextIndex = endIndex;
        ++task.activeWorkers;
        if(task.nextIndex == task.maxIndex) 
            tasks = task.next;
        
        lock.unlock();
        for(int i = startIndex; i < endIndex; ++i) {
            if(task.func1) {
                task.func1(i);
            } else {
                DCHECK(task.func2);
                task.func2(Point2i(i % task.nX, i / task.nX));
            }
        }
        
        lock.lock();
        --task.activeWorkers;
    }
}

void ParallelForLoopExecutor::MergeWorkerThreadStats() {
    std::unique_lock lock(taskMutex);
    std::unique_lock doneLock(reportMutex);
    isReportWorkerStats = true;
    reporterCount = threads.size();
    taskCV.notify_all();
    reportDoneCV.wait(lock, [&](){ return reporterCount == 0; });
    ReportThreadStats(); // because main thread join the calculation, so main thread need report statistic
    isReportWorkerStats = false;
}


void ParallelForLoopExecutor::PrintStats(FILE *dest) {
    GLOBAL_STATS_ACCUMULATOR.Print(dest);
}

void ParallelForLoopExecutor::ClearStats() {
    GLOBAL_STATS_ACCUMULATOR.Clear();
}

void ParallelForLoopExecutor::Clean() {
    if(threads.empty()) return;
    
    {
        std::lock_guard<std::mutex> lock(taskMutex);
        isShutdownThreads = true;
        taskCV.notify_all();
    }

    for(std::thread &thread : threads)
        thread.join();
    
    threads.erase(threads.begin(), threads.end());
    isShutdownThreads = false;
}

void ParallelForLoopExecutor::ReportThreadStats() {
    std::mutex mutex;
    std::lock_guard<std::mutex> lock(mutex);
    StatRegisterer::Callback(GLOBAL_STATS_ACCUMULATOR);
}

} // namespace pbrt