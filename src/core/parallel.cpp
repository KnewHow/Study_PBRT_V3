#include "parallel.h"

namespace pbrt {

ParallelForLoopExecutor::ParallelForLoopExecutor(std::optional<int> maxAvailableThreads) {
    DCHECK_EQ(threads.size(), 0);
    tasks = nullptr;
    int nThreads = maxAvailableThreads.has_value() ? maxAvailableThreads.value() : NumSystemCores();
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
    while(!shutdownThreads) {
        if(tasks == nullptr) {
            taskCV.wait(lock);
        } else {
            ParallelForLoopTask &task = *tasks;
            int64_t startIndex = task.nextIndex;
            int64_t endIndex = std::min(startIndex + task.chunkSize, task.maxIndex);
            task.nextIndex = endIndex;
            ++task.activeWorkers;
            if(task.nextIndex == task.maxIndex) 
                tasks = tasks->next;
            
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

void ParallelForLoopExecutor::ParallelFor2D(std::function<void(Point2i)> func, const Point2i &count) {
    DCHECK(threads.size() > 0);
    if(threads.empty() || count.x * count.y <= 1) { // if no extra threads or amout is too samll, do task use main thread direct.   
        for(int y = 0; y < count.y; ++y)
            for(int x = 0; x < count.x; ++x)
                func(Point2i(x, y));
        
        return;
    }
    
    ParallelForLoopTask task(func, count);
    
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
            tasks = tasks->next;
        
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

} // namespace pbrt