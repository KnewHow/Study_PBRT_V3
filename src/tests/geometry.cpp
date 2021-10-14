
#include <random>

#include "tests/gtest/gtest.h"
#include "pbrt.h"
#include "geometry.h"


using namespace pbrt;

Float Epsilon = 0.0001f;

inline int get_random_int() {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_real_distribution<float> dist(0.0, 1.0);
    return dist(rng);
}


inline Float get_random_float(Float begin, Float end) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_real_distribution<Float> dist(begin, end);
    return dist(rng);
}

inline int get_random_int(int begin, int end) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<int> dist(begin, end);
    return dist(rng);
}

inline bool compare_float(const Float &v1, const Float &v2) {
    return std::abs(v1 - v2) < Epsilon;
}

inline bool compare_vector2_float(const Vector2f &v1, const Vector2f &v2) {
    Vector2f r = v1 - v2;
    return compare_float(r.x, 0.0f) && compare_float(r.y, 0.0f); 
}

TEST(Vector2i, OperateTest) {
    int begin = -1000;
    int end = 1000;
    int x0 = get_random_int(begin, end);
    int y0 = get_random_int(begin, end);
    int x1 = get_random_int(begin, end);
    int y1 = get_random_int(begin, end);
    int u = get_random_int(begin, end);

    Vector2i v0(x0, y0);
    Vector2i v1(x1, y1);
    EXPECT_NE(v0, v1);
    EXPECT_EQ(v0 + v1, Vector2i(v0.x + v1.x, v0.y + v1.y));
    EXPECT_EQ(v0 += v1, Vector2i(v0.x + v1.x, v0.y + v1.y));

    EXPECT_EQ(v0 - v1, Vector2i(v0.x - v1.x, v0.y - v1.y));
    EXPECT_EQ(v1 -= v0, Vector2i(v1.x - v0.x, v1.y - v0.y));
    EXPECT_EQ(-v0, Vector2i(-v0.x, -v0.y));
}


TEST(Vector2f, OperateTest) {
    Float begin = -1000.0;
    Float end = 1000.0;
    Float x0 = get_random_float(begin, end);
    Float y0 = get_random_float(begin, end);
    Float x1 = get_random_float(begin, end);
    Float y1 = get_random_float(begin, end);
    Float u = get_random_float(begin, end);


    Vector2f v0(x0, y0);
    Vector2f v1(x1, y1);
    Vector2f r0;
    Vector2f r1;
    bool rb;
    EXPECT_NE(v0, v1);
    EXPECT_TRUE(compare_vector2_float(v0 + v1, Vector2f(v0.x + v1.x, v0.y + v1.y)));
    EXPECT_TRUE(compare_vector2_float(v0 += v1, Vector2f(v0.x + v1.x, v0.y + v1.y)));

    EXPECT_TRUE(compare_vector2_float(v0 - v1, Vector2f(v0.x - v1.x, v0.y - v1.y)));
    EXPECT_TRUE(compare_vector2_float(v1 -= v0, Vector2f(v1.x - v0.x, v1.y - v0.y)));

    EXPECT_TRUE(compare_vector2_float(v0 * u, Vector2f(v0.x * u, v0.y * u)));
    EXPECT_TRUE(compare_vector2_float(v1 *= u, Vector2f(v1.x * u, v1.y * u)));

      
    EXPECT_TRUE(compare_vector2_float(v0 / u, Vector2f(v0.x / u, v0.y / u)));
    EXPECT_TRUE(compare_vector2_float(v1 /= u, Vector2f(v1.x / u, v1.y / u)));

    EXPECT_TRUE(compare_vector2_float(-v0, Vector2f(-v0.x, -v0.y)));
    EXPECT_TRUE(compare_float(v0[0], v0.x));
    EXPECT_TRUE(compare_float(v0[1], v0.y));

    EXPECT_TRUE(compare_float(v0.LengthSquared(), Float(v0.x * v0.x + v0.y * v0.y)));
    EXPECT_TRUE(compare_float(v1.Length(), std::sqrt(Float(v1.x * v1.x + v1.y * v1.y))));
}