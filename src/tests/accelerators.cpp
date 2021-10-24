#include "tests/gtest/gtest.h"
#include "pbrt.h"
#include "shape.h"
#include "shape/triangle.h"
#include "primitive.h"
#include "OBJ_Loader.h"
#include "material.h"
#include "accelerators/bvh.h"

using namespace pbrt;

struct Person {
    int counter = 0;
};

TEST(BVHAccel, BaseTest) {
    Person p;
    ++p.counter;
    LOG(INFO) << "counter is： " << p.counter;
    //BVHAccel bvh;
    EXPECT_TRUE(true);
}