#include "tests/gtest/gtest.h"
#include "pbrt.h"
#include "shape.h"
#include "shape/triangle.h"
#include "primitive.h"
#include "OBJ_Loader.h"
#include "material.h"
#include "accelerators/bvh.h"

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