#include <filesystem>

#include "tests/gtest/gtest.h"
#include "pbrt.h"
#include "shape.h"
#include "shape/triangle.h"
#include "primitive.h"
#include "OBJ_Loader.h"
#include "material.h"
#include "accelerators/bvh.h"
#include "clock.h"
#include "parallel.h"
#include "stats.h"

using namespace pbrt;

bool loadHugeModel(std::vector<std::shared_ptr<Primitive>> &ps, std::optional<std::string> path, std::chrono::milliseconds &begin, std::chrono::milliseconds &end);
void test_bvh_insersect(const std::shared_ptr<BVHAccel> bvh, const std::vector<Ray> rays, std::chrono::milliseconds &begin, std::chrono::milliseconds &end);
void test_bvh_insersectP(const std::shared_ptr<BVHAccel> bvh, const std::vector<Ray> rays, std::chrono::milliseconds &begin, std::chrono::milliseconds &end);
std::shared_ptr<BVHAccel> buildBVH(const std::vector<std::shared_ptr<Primitive>> &ps, BVHAccel::SplitMethod method, std::chrono::milliseconds &begin, std::chrono::milliseconds &end);
void printTime(const std::string &prefix, const std::chrono::milliseconds &begin, const std::chrono::milliseconds &end);
std::vector<Ray> generateTestRays();
void generateTestRays(std::vector<Ray> &rays, int size = 10000);


TEST(Stats, ParallelStatistic) {
    FILE *fp;
    fp  = fopen ("/tmp/pbr_test_log/pbrt_stat.txt", "w+"); 
    std::vector<std::shared_ptr<Primitive>> ps;
    std::chrono::milliseconds begin, end;
    bool r = loadHugeModel(ps, "../../resource/hutao/hutao.obj", begin, end);
    if(!r) {
        return;
    }
    auto bvh_sah = buildBVH(ps, BVHAccel::SplitMethod::SAH, begin, end);
    std::vector<Ray> rays;
    generateTestRays(rays, 50000);
    
    ParallelForLoopExecutor::Init(std::nullopt);
    ParallelForLoopExecutor::MergeWorkerThreadStats(); 
    ParallelForLoopExecutor::ClearStats();
    begin = getCurrentMilliseconds();
    for(const Ray &ray: rays) {
        SurfaceInteraction isect;
        bvh_sah->Intersect(ray, isect);
    }
    end = getCurrentMilliseconds();
    printTime("[Stats] test bvh intersect serial took: ", begin, end);
    
    ParallelForLoopExecutor::MergeWorkerThreadStats(); 
    ParallelForLoopExecutor::PrintStats(fp);
    
    ParallelForLoopExecutor::ClearStats();
    begin = getCurrentMilliseconds();
    ParallelForLoopExecutor::ParallelFor1D([&](int64_t i){
        SurfaceInteraction isect;
        bvh_sah->Intersect(rays[i], isect);
    }, rays.size(), 1000);
    end = getCurrentMilliseconds();
    LOG(INFO) << "[Stats] test bvh intersect parallel took: " << (end - begin).count();
    
    ParallelForLoopExecutor::MergeWorkerThreadStats(); 
    ParallelForLoopExecutor::PrintStats(fp);
    ParallelForLoopExecutor::Clean();
}
