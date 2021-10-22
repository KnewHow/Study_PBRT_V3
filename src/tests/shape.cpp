
#include "tests/gtest/gtest.h"
#include "pbrt.h"
#include "shape.h"
#include "shape/triangle.h"

using namespace pbrt;

TEST(Triangle, BaseTest) {
    objl_loader.LoadFile("../../resource/cube/cube.obj");
    //loader.LoadFile("../../resource/connelbox/connelbox.obj");
    std::vector<objl::Mesh> meshs = objl_loader.LoadedMeshes;
    std::vector<Triangle> ts;
    for(const auto &mesh: meshs) {
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
            Triangle t(m, i/3);
            ts.push_back(t);
        }
    }
    EXPECT_EQ(ts.size(), 12);
}