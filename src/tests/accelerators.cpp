#include "tests/gtest/gtest.h"
#include "pbrt.h"
#include "shape.h"
#include "shape/triangle.h"
#include "primitive.h"
#include "OBJ_Loader.h"
#include "material.h"
#include "accelerators/bvh.h"
#include "clock.h"

#include <filesystem>

using namespace pbrt;


TEST(BVHAccel, BaseTest) {
    objl::Loader loader;
    std::string resourcePath = "../../resource/cube/cube.obj";
    loader.LoadFile(resourcePath);
    if(loader.LoadedMeshes.empty()) {
        LOG(WARNING) << "not load mesh from this path: " << resourcePath;
        return;
    }
    std::vector<objl::Mesh> meshs = loader.LoadedMeshes;
    std::vector<std::shared_ptr<Primitive>> ps;
    for(const auto &mesh: meshs) {
        std::shared_ptr<Material> material = std::make_shared<Material>(RGBAf(mesh.MeshMaterial.Kd.X, mesh.MeshMaterial.Kd.Y, mesh.MeshMaterial.Kd.Z, 1.0),
                                                                        RGBAf(mesh.MeshMaterial.Ks.X, mesh.MeshMaterial.Ks.Y, mesh.MeshMaterial.Ks.Z, 1.0));
        std::vector<int> idxs;
        std::vector<Point3f> p;
        std::vector<Normal3f> n;
        p.reserve(mesh.Vertices.size());
        n.reserve(mesh.Vertices.size());
        idxs.reserve(mesh.Indices.size());
        for(const auto &vertex: mesh.Vertices) {
            p.push_back(Point3f(vertex.Position.X, vertex.Position.Y, vertex.Position.Z));
            n.push_back(Normal3f(vertex.Normal.X, vertex.Normal.Y, vertex.Normal.Z));
        }
        for(const auto idx: mesh.Indices) idxs.push_back(idx);
        std::shared_ptr<TriangleMesh> m = std::make_shared<TriangleMesh>(idxs.size()/3, p.size(), idxs, p, n);
        for(int i = 0; i < idxs.size(); i += 3) {
            std::shared_ptr<Triangle> triangle = std::make_shared<Triangle>(m, i/3);
            std::shared_ptr<GeometicPrimitive> gp = std::make_shared<GeometicPrimitive>(triangle, material);
            ps.push_back(gp);
        }
    }
    BVHAccel bvh(ps);
    int intersectTimes = 1000;
    for(int i = 0; i < intersectTimes; i++) {
        Ray ray(Point3f(0, 0, 0), Normalize(Vector3f(get_random_Float(), get_random_Float(), get_random_Float())));
        SurfaceInteraction isect;
        bool isHit = bvh.Intersect(ray, isect);
        // LOG(INFO) << "is hit: " << std::boolalpha << isHit << ", ray is: " << ray << ", hit point is: " << isect.p << ". hit point normal: " << isect.n;
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

bool loadHugeModel(std::vector<std::shared_ptr<Primitive>> &ps, std::chrono::milliseconds &begin, std::chrono::milliseconds &end);
void test_bvh_insersect(const std::shared_ptr<BVHAccel> bvh, const std::vector<Ray> rays, std::chrono::milliseconds &begin, std::chrono::milliseconds &end);
void test_bvh_insersectP(const std::shared_ptr<BVHAccel> bvh, const std::vector<Ray> rays, std::chrono::milliseconds &begin, std::chrono::milliseconds &end);
std::shared_ptr<BVHAccel> buildBVH(const std::vector<std::shared_ptr<Primitive>> &ps, BVHAccel::SplitMethod method, std::chrono::milliseconds &begin, std::chrono::milliseconds &end);
void printTime(const std::string &prefix, const std::chrono::milliseconds &begin, const std::chrono::milliseconds &end);
std::vector<Ray> generateTestRays();
void generateTestRays2(std::vector<Ray> &rays);

TEST(BVHAccel, ComparePerformance) {
    std::vector<std::shared_ptr<Primitive>> ps;
    LOG(INFO) << "current exec path: " << std::filesystem::current_path();
    std::chrono::milliseconds begin, end;
    bool r = loadHugeModel(ps, begin, end);
    if(!r) {
        return;
    }
    printTime("Load Huge Model took: ", begin, end);


    auto bvh_middle = buildBVH(ps, BVHAccel::SplitMethod::Middle, begin, end);
    printTime("Build BVH with Millde took: ", begin, end);

    auto bvh_equalCounts = buildBVH(ps, BVHAccel::SplitMethod::EqualCounts, begin, end);
    printTime("Build BVH with EqualCounts took: ", begin, end);

    auto bvh_sah = buildBVH(ps, BVHAccel::SplitMethod::SAH, begin, end);
    printTime("Build BVH with SAH took: ", begin, end);
    
    LOG(INFO) << "---------------------test insersect----------------------------";
    
    //std::vector<Ray> rays = generateTestRays();
    std::vector<Ray> rays;
    generateTestRays2(rays);

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

bool loadHugeModel(std::vector<std::shared_ptr<Primitive>> &ps, std::chrono::milliseconds &begin, std::chrono::milliseconds &end) {
    begin = getCurrentMilliseconds();
    std::string HUGE_MODEL_PATH = "../../resource/hutao/hutao.obj";
    objl::Loader loader;
    loader.LoadFile(HUGE_MODEL_PATH);
    if(loader.LoadedMeshes.empty()) {
        LOG(WARNING) << "not load mesh from this path: " << HUGE_MODEL_PATH;
        return false;
    }
    std::vector<objl::Mesh> meshs = loader.LoadedMeshes;
    for(const auto &mesh: meshs) {
        std::shared_ptr<Material> material = std::make_shared<Material>(RGBAf(mesh.MeshMaterial.Kd.X, mesh.MeshMaterial.Kd.Y, mesh.MeshMaterial.Kd.Z, 1.0),
                                                                        RGBAf(mesh.MeshMaterial.Ks.X, mesh.MeshMaterial.Ks.Y, mesh.MeshMaterial.Ks.Z, 1.0));
        std::vector<int> idxs;
        std::vector<Point3f> p;
        std::vector<Normal3f> n;
        p.reserve(mesh.Vertices.size());
        n.reserve(mesh.Vertices.size());
        idxs.reserve(mesh.Indices.size());
        for(const auto &vertex: mesh.Vertices) {
            p.push_back(Point3f(vertex.Position.X, vertex.Position.Y, vertex.Position.Z));
            n.push_back(Normal3f(vertex.Normal.X, vertex.Normal.Y, vertex.Normal.Z));
        }
        for(const auto idx: mesh.Indices) idxs.push_back(idx);
        std::shared_ptr<TriangleMesh> m = std::make_shared<TriangleMesh>(idxs.size()/3, p.size(), idxs, p, n);
        for(int i = 0; i < idxs.size(); i += 3) {
            std::shared_ptr<Triangle> triangle = std::make_shared<Triangle>(m, i/3);
            std::shared_ptr<GeometicPrimitive> gp = std::make_shared<GeometicPrimitive>(triangle, material);
            ps.push_back(gp);
        }
    }
    end = getCurrentMilliseconds();
    return true;
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

void printTime(const std::string &prefix, const std::chrono::milliseconds &begin, const std::chrono::milliseconds &end) {
    LOG(INFO) << prefix << (end - begin).count();
}

std::vector<Ray> generateTestRays() {
    int amount = 10000;
    std::vector<Ray> rays(amount);
    for(int i = 0; i < amount; i++) {
        Ray r(Point3f(0, 0, 0), Normalize(Vector3f(get_random_Float(), get_random_Float(), get_random_Float())));
        rays[i] = r;
    }
    return rays;
}

void generateTestRays2(std::vector<Ray> &rays) {
    int amount = 10000;
    rays.reserve(amount);
    for(int i = 0; i < amount; i++) {
        Ray r(Point3f(0, 0, 0), Normalize(Vector3f(get_random_Float(), get_random_Float(), get_random_Float())));
        rays.push_back(r) ;
    }
}