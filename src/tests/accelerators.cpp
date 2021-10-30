#include <filesystem>

#include "pbrt_test.h"
#include "accelerators/bvh.h"
#include "clock.h"

using namespace pbrt;


TEST(BVHAccel, BaseTest) {
    std::vector<std::shared_ptr<Primitive>> ps;
    bool r = loadModel(ps, "../resource/cube/cube.obj");
    if(!r) return;
    BVHAccel bvh(ps);
    int intersectTimes = 1000;
    for(int i = 0; i < intersectTimes; i++) {
        Ray ray(Point3f(0, 0, 0), Normalize(Vector3f(get_random_Float(), get_random_Float(), get_random_Float())));
        SurfaceInteraction isect;
        bool isHit = bvh.Intersect(ray, isect);
        EXPECT_TRUE(isHit);
    }

    for(int i = 0; i < intersectTimes; i++) {
        Ray ray(Point3f(0, 0, 0), Normalize(Vector3f(get_random_Float(),get_random_Float(),get_random_Float())));
        SurfaceInteraction isect;
        bool isHit = bvh.IntersectP(ray);
        EXPECT_TRUE(isHit);
    }

    {
        Ray ray (Point3f(2, 0, 0), Vector3f(0, 1, 0));
        SurfaceInteraction isect;
        bool isHit = bvh.Intersect(ray, isect);
        EXPECT_FALSE(isHit);
        isHit = bvh.IntersectP(ray);
        EXPECT_FALSE(isHit);
    }
}

void test_bvh_insersect(const std::shared_ptr<BVHAccel> bvh, const std::vector<Ray> rays, std::chrono::milliseconds &begin, std::chrono::milliseconds &end);
void test_bvh_insersectP(const std::shared_ptr<BVHAccel> bvh, const std::vector<Ray> rays, std::chrono::milliseconds &begin, std::chrono::milliseconds &end);
std::shared_ptr<BVHAccel> buildBVH(const std::vector<std::shared_ptr<Primitive>> &ps, BVHAccel::SplitMethod method, std::chrono::milliseconds &begin, std::chrono::milliseconds &end);

TEST(BVHAccel, ComparePerformance) {
    std::vector<std::shared_ptr<Primitive>> ps;
    LOG(INFO) << "current exec path: " << std::filesystem::current_path();
    std::chrono::milliseconds begin, end;
    bool r = loadModel(ps, "../resource/hutao/hutao.obj");
    if(!r) {
        return;
    }
   
    auto bvh_middle = buildBVH(ps, BVHAccel::SplitMethod::Middle, begin, end);
    printTime("Build BVH with Millde took: ", begin, end);

    auto bvh_equalCounts = buildBVH(ps, BVHAccel::SplitMethod::EqualCounts, begin, end);
    printTime("Build BVH with EqualCounts took: ", begin, end);

    auto bvh_sah = buildBVH(ps, BVHAccel::SplitMethod::SAH, begin, end);
    printTime("Build BVH with SAH took: ", begin, end);
    
    LOG(INFO) << "---------------------test insersect----------------------------";
    
    //std::vector<Ray> rays = generateTestRays();
    std::vector<Ray> rays;
    generateTestRays(rays);

    test_bvh_insersect(bvh_middle, rays, begin, end);
    printTime("Test BVH with Millde took: ", begin, end);

    test_bvh_insersect(bvh_equalCounts, rays, begin, end);
    printTime("Test BVH with EqualCounts took: ", begin, end);

    test_bvh_insersect(bvh_sah, rays, begin, end);
    printTime("Test BVH with SAH took: ", begin, end);

    LOG(INFO) << "---------------------test insersectP----------------------------";

    test_bvh_insersectP(bvh_middle, rays, begin, end);
    printTime("Test BVH with Millde took: ", begin, end);

    test_bvh_insersectP(bvh_equalCounts, rays, begin, end);
    printTime("Test BVH with EqualCounts took: ", begin, end);

    test_bvh_insersectP(bvh_sah, rays, begin, end);
    printTime("Test BVH with SAH took: ", begin, end);
}

void test_bvh_insersect(const std::shared_ptr<BVHAccel> bvh, const std::vector<Ray> rays, std::chrono::milliseconds &begin, std::chrono::milliseconds &end) {
    begin = getCurrentMilliseconds();
    int hitTimes = 0;
    for(const Ray &ray: rays) {
        SurfaceInteraction isect;
        bool isHit = bvh->Intersect(ray, isect);
        if(isHit) ++hitTimes;
    }
    LOG(INFO) << "test_bvh_insersect result, hit times: " << hitTimes << ", no hit times: " << rays.size() - hitTimes;
    end = getCurrentMilliseconds();
}

void test_bvh_insersectP(const std::shared_ptr<BVHAccel> bvh, const std::vector<Ray> rays, std::chrono::milliseconds &begin, std::chrono::milliseconds &end) {
    begin = getCurrentMilliseconds();
    int hitTimes = 0;
    for(const Ray &ray: rays) {
        bool isHit = bvh->IntersectP(ray);
        if(isHit) ++hitTimes;
    }
    LOG(INFO) << "test_bvh_insersectP result, hit times: " << hitTimes << ", no hit times: " << rays.size() - hitTimes;
    end = getCurrentMilliseconds();
}

std::shared_ptr<BVHAccel> buildBVH(const std::vector<std::shared_ptr<Primitive>> &ps, BVHAccel::SplitMethod method, std::chrono::milliseconds &begin, std::chrono::milliseconds &end) {
    begin = getCurrentMilliseconds();
    std::shared_ptr<BVHAccel> bvh = std::make_shared<BVHAccel>(ps, method);
    end = getCurrentMilliseconds();
    return bvh;
}