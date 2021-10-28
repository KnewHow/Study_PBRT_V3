#if defined(_MSC_VER)
#define NOMINMAX
#pragma once
#endif

#ifndef PBRT_SRC_CORE_STATS_H_
#define PBRT_SRC_CORE_STATS_H_

#include <map>
#include <functional>

#include "pbrt.h"

namespace pbrt {

class StatsAccumulator;

/**
 * A stat registerer, pull all stat variable into a registerer
*/
class StatRegisterer {
public:
    StatRegisterer(std::function<void(StatsAccumulator&)> func) {
        if(!funcs)
            funcs = new std::vector<std::function<void(StatsAccumulator&)>>;
        funcs->push_back(func);
    }
    
    /**
     * A function put all stat function results into the StatsAccumulator.
     * @param accum A stat accumulator put all stat result together.
    */
    static void Callback(StatsAccumulator &accum) {
        for(const auto &func: *funcs)
            func(accum);
    }
private:
    /**
    * A static vector, recording all stats function in whole project.
    * Notice the parameter of this function is a reference variable of StatsAccumulator.
    * You can see it as a global variabl of StatsAccumulator. All stat function will put together in it.
    */
    static std::vector<std::function<void(StatsAccumulator&)>> *funcs; 
};


/**
 * A stat accumulator pull all stats result together.
*/
class StatsAccumulator {
public:
    void ReportCounter(const std::string &name, int64_t val) {
        counter[name] += val;
    }
    void ReportMemoryCounter(const std::string &name, int64_t val) {
        memoryCounter[name] += val;
    }

private:
    std::map<std::string, int64_t> counter; // A counter to stat amout with name. The key represent name, the value represent amount
    std::map<std::string, int64_t> memoryCounter; // A memory counter to stat memory size by name
};

} // namespace pbrt

#endif // PBRT_SRC_CORE_STATS_H_