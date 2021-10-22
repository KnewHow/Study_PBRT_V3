#include "tests/gtest/gtest.h"
#include "pbrt.h"
#include "shape.h"
#include "shape/triangle.h"
#include "primitive.h"
#include "OBJ_Loader.h"

using namespace pbrt;

TEST(GeometricPrimitive, InitPrimitive) {
    objl::Loader loader;
    loader.LoadFile("../../resource/cube/cube.obj");
    // loader.LoadFile("../../resource/connelbox/connelbox.obj");
    // std::vector<objl::Mesh> meshs = loader.LoadedMeshes;
    // std::vector<Primitive> ps
    // EXPECT_TRUE(true);
}