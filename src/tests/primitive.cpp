#include "tests/gtest/gtest.h"
#include "pbrt.h"
#include "shape.h"
#include "shape/triangle.h"
#include "primitive.h"
#include "OBJ_Loader.h"
#include "material.h"

using namespace pbrt;

TEST(GeometricPrimitive, InitPrimitive) {
    objl::Loader loader;
    loader.LoadFile("../../resource/cube/cube.obj");
    // loader.LoadFile("../../resource/connelbox/connelbox.obj");
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
    EXPECT_EQ(ps.size(), 12);
}