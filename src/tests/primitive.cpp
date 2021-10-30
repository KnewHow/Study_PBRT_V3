
#include "pbrt_test.h"
#include "shape.h"
#include "shape/triangle.h"
#include "primitive.h"
#include "material.h"

using namespace pbrt;

TEST(GeometricPrimitive, InitPrimitive) {
    std::vector<std::shared_ptr<Primitive>> ps;
    bool r = loadModel(ps, "../resource/cube/cube.obj");
    if(!r) {
        return;
    }
    EXPECT_EQ(ps.size(), 12);
}