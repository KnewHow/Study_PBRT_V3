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
    std::vector<int> a;
    a.reserve(3);
    std::vector<int> b(3);
    LOG(INFO) << "a size is: " << a.size() << ", b size is: " << b.size();
    //BVHAccel bvh;
    EXPECT_TRUE(true);
}