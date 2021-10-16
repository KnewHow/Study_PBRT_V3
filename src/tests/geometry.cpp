
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

inline bool compare_float(const Float &p1, const Float &v2) {
    return std::abs(p1 - v2) < Epsilon;
}

inline bool compare_vector_float(const Vector2f &p1, const Vector2f &v2) {
    Vector2f r = p1 - v2;
    return compare_float(r.x, 0.0f) && compare_float(r.y, 0.0f); 
}

inline bool compare_point_float(const Point2f &p1, const Point2f& p2) {
    Vector2f r = p1 - p2;
    return compare_float(r.x, 0.0f) && compare_float(r.y, 0.0f); 
}

inline bool compare_point_float(const Point3f &p1, const Point3f& p2) {
    Vector3f r = p1 - p2;
    return compare_float(r.x, 0.0f) && compare_float(r.y, 0.0f) && compare_float(r.z, 0.0f); 
}

inline bool compare_vector_float(const Vector3f &p1, const Vector3f &v2) {
    Vector3f r = p1 - v2;
    return compare_float(r.x, 0.0f) && compare_float(r.y, 0.0f) && compare_float(r.z, 0); 
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
    Vector2i p1(x1, y1);
    EXPECT_NE(v0, p1);
    EXPECT_EQ(v0 + p1, Vector2i(v0.x + p1.x, v0.y + p1.y));
    EXPECT_EQ(v0 += p1, Vector2i(v0.x + p1.x, v0.y + p1.y));

    EXPECT_EQ(v0 - p1, Vector2i(v0.x - p1.x, v0.y - p1.y));
    EXPECT_EQ(p1 -= v0, Vector2i(p1.x - v0.x, p1.y - v0.y));
    EXPECT_EQ(-v0, Vector2i(-v0.x, -v0.y));
}

TEST(Vector3i, OperateTest) {
    int begin = -1000;
    int end = 1000;
    int x0 = get_random_int(begin, end);
    int y0 = get_random_int(begin, end);
    int z0 = get_random_int(begin, end);
    int x1 = get_random_int(begin, end);
    int y1 = get_random_int(begin, end);
    int z1 = get_random_int(begin, end);
    int u = get_random_int(begin, end);

    Vector3i v0(x0, y0, z0);
    Vector3i p1(x1, y1, z1);
    EXPECT_NE(v0, p1);
    EXPECT_EQ(v0 + p1, Vector3i(v0.x + p1.x, v0.y + p1.y, v0.z + p1.z));
    EXPECT_EQ(v0 += p1, Vector3i(v0.x + p1.x, v0.y + p1.y, v0.z + p1.z));

    EXPECT_EQ(v0 - p1, Vector3i(v0.x - p1.x, v0.y - p1.y, v0.z - p1.z));
    EXPECT_EQ(p1 -= v0, Vector3i(p1.x - v0.x, p1.y - v0.y, p1.z - v0.z));
    EXPECT_EQ(-v0, Vector3i(-v0.x, -v0.y, -v0.z));
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
    Vector2f p1(x1, y1);

    EXPECT_NE(v0, p1);
    EXPECT_TRUE(compare_vector_float(v0 + p1, Vector2f(v0.x + p1.x, v0.y + p1.y)));
    EXPECT_TRUE(compare_vector_float(v0 += p1, Vector2f(v0.x + p1.x, v0.y + p1.y)));

    EXPECT_TRUE(compare_vector_float(v0 - p1, Vector2f(v0.x - p1.x, v0.y - p1.y)));
    EXPECT_TRUE(compare_vector_float(p1 -= v0, Vector2f(p1.x - v0.x, p1.y - v0.y)));

    EXPECT_TRUE(compare_vector_float(v0 * u, Vector2f(v0.x * u, v0.y * u)));
    EXPECT_TRUE(compare_vector_float(p1 *= u, Vector2f(p1.x * u, p1.y * u)));

      
    EXPECT_TRUE(compare_vector_float(v0 / u, Vector2f(v0.x / u, v0.y / u)));
    EXPECT_TRUE(compare_vector_float(p1 /= u, Vector2f(p1.x / u, p1.y / u)));

    EXPECT_TRUE(compare_vector_float(-v0, Vector2f(-v0.x, -v0.y)));
    EXPECT_TRUE(compare_float(v0[0], v0.x));
    EXPECT_TRUE(compare_float(v0[1], v0.y));

    EXPECT_TRUE(compare_float(v0.LengthSquared(), Float(v0.x * v0.x + v0.y * v0.y)));
    EXPECT_TRUE(compare_float(p1.Length(), std::sqrt(Float(p1.x * p1.x + p1.y * p1.y))));
}

TEST(Vector3f, OperateTest) {
    Float begin = -1000.0;
    Float end = 1000.0;
    Float x0 = get_random_float(begin, end);
    Float y0 = get_random_float(begin, end);
    Float z0 = get_random_float(begin, end);
    Float x1 = get_random_float(begin, end);
    Float y1 = get_random_float(begin, end);
    Float z1 = get_random_float(begin, end);
    Float u = get_random_float(begin, end);


    Vector3f v0(x0, y0, z0);
    Vector3f p1(x1, y1, z1);

    EXPECT_NE(v0, p1);
    EXPECT_TRUE(compare_vector_float(v0 + p1, Vector3f(v0.x + p1.x, v0.y + p1.y, v0.z + p1.z)));
    EXPECT_TRUE(compare_vector_float(v0 += p1, Vector3f(v0.x + p1.x, v0.y + p1.y, v0.z + p1.z)));

    EXPECT_TRUE(compare_vector_float(v0 - p1, Vector3f(v0.x - p1.x, v0.y - p1.y, v0.z - p1.z)));
    EXPECT_TRUE(compare_vector_float(p1 -= v0, Vector3f(p1.x - v0.x, p1.y - v0.y, p1.z - v0.z)));

    EXPECT_TRUE(compare_vector_float(v0 * u, Vector3f(v0.x * u, v0.y * u, v0.z * u)));
    EXPECT_TRUE(compare_vector_float(p1 *= u, Vector3f(p1.x * u, p1.y * u, p1.z * u)));

      
    EXPECT_TRUE(compare_vector_float(v0 / u, Vector3f(v0.x / u, v0.y / u, v0.z / u)));
    EXPECT_TRUE(compare_vector_float(p1 /= u, Vector3f(p1.x / u, p1.y / u, p1.z / u)));

    EXPECT_TRUE(compare_vector_float(-v0, Vector3f(-v0.x, -v0.y, -v0.z)));
    EXPECT_TRUE(compare_float(v0[0], v0.x));
    EXPECT_TRUE(compare_float(v0[1], v0.y));
    EXPECT_TRUE(compare_float(v0[2], v0.z));

    EXPECT_TRUE(compare_float(v0.LengthSquared(), Float(v0.x * v0.x + v0.y * v0.y + v0.z * v0.z)));
    EXPECT_TRUE(compare_float(p1.Length(), std::sqrt(Float(p1.x * p1.x + p1.y * p1.y + p1.z * p1.z))));
}

TEST(Point2f, OperateTest) {
    Float begin = -1000.0;
    Float end = 1000.0;
    Float x0 = get_random_float(begin, end);
    Float y0 = get_random_float(begin, end);
    Float x1 = get_random_float(begin, end);
    Float y1 = get_random_float(begin, end);
    Float u = get_random_float(begin, end);


    Point2f p0(x0, y0);
    Point2f p1(x1, y1);
    Vector2f v(get_random_float(begin, end), get_random_float(begin, end));

    Vector2i v2 = (Vector2i)p0;
    EXPECT_EQ(v2, Vector2i((int)p0.x, (int)p0.y));

    Point2i p = (Point2i)v;
    EXPECT_EQ(p, Point2i((int)v.x, (int)v.y));

    p = (Point2i)p1;
    EXPECT_EQ(p, Point2i((int)p1.x, (int)p1.y));

    EXPECT_NE(p0, p1);
    EXPECT_TRUE(compare_point_float(p0 + p1, Point2f(p0.x + p1.x, p0.y + p1.y)));
    EXPECT_TRUE(compare_point_float(p0 += p1, Point2f(p0.x + p1.x, p0.y + p1.y)));
    EXPECT_TRUE(compare_point_float(p0 + v, Point2f(p0.x + v.x, p0.y + v.y)));
    EXPECT_TRUE(compare_point_float(p1 += v, Point2f(p1.x + v.x, p1.y + v.y)));

    EXPECT_TRUE(compare_vector_float(p0 - p1, Vector2f(p0.x - p1.x, p0.y - p1.y)));
    EXPECT_TRUE(compare_point_float(p0 - v, Point2f(p0.x - v.x, p0.y - v.y)));
    EXPECT_TRUE(compare_point_float(p1 -= v, Point2f(p1.x - v.x, p1.y - v.y)));

    EXPECT_TRUE(compare_point_float(p0 * u, Point2f(p0.x * u, p0.y * u)));
    EXPECT_TRUE(compare_point_float(p1 *= u, Point2f(p1.x * u, p1.y * u)));

      
    EXPECT_TRUE(compare_point_float(p0 / u, Point2f(p0.x / u, p0.y / u)));
    EXPECT_TRUE(compare_point_float(p1 /= u, Point2f(p1.x / u, p1.y / u)));

    EXPECT_TRUE(compare_point_float(-p0, Point2f(-p0.x, -p0.y)));
    EXPECT_TRUE(compare_float(p0[0], p0.x));
    EXPECT_TRUE(compare_float(p0[1], p0.y));
}


TEST(Point3f, OperateTest) {
    Float begin = -1000.0;
    Float end = 1000.0;
    Float x0 = get_random_float(begin, end);
    Float y0 = get_random_float(begin, end);
    Float z0 = get_random_float(begin, end);
    Float x1 = get_random_float(begin, end);
    Float y1 = get_random_float(begin, end);
    Float z1 = get_random_float(begin, end);
    Float u = get_random_float(begin, end);


    Point3f p0(x0, y0, z0);
    Point3f p1(x1, y1, z1);
    Vector3f v(get_random_float(begin, end), get_random_float(begin, end), get_random_float(begin, end));

    Vector2f v1 = (Vector2f)p0;
    EXPECT_TRUE(compare_vector_float(v1, Vector2f(p0.x, p0.y)));

    Vector3i v2 = (Vector3i)p0;
    EXPECT_EQ(v2, Vector3i((int)p0.x, (int)p0.y, (int)p0.z));

    Point3i p = (Point3i)v;
    EXPECT_EQ(p, Point3i((int)v.x, (int)v.y, (int)v.z));

    p = (Point3i)p1;
    EXPECT_EQ(p, Point3i((int)p1.x, (int)p1.y, (int)p1.z));

    EXPECT_NE(p0, p1);
    EXPECT_TRUE(compare_point_float(p0 + p1, Point3f(p0.x + p1.x, p0.y + p1.y, p0.z + p1.z)));
    EXPECT_TRUE(compare_point_float(p0 += p1, Point3f(p0.x + p1.x, p0.y + p1.y, p0.z + p1.z)));
    EXPECT_TRUE(compare_point_float(p0 + v, Point3f(p0.x + v.x, p0.y + v.y, p0.z + v.z)));
    EXPECT_TRUE(compare_point_float(p1 += v, Point3f(p1.x + v.x, p1.y + v.y, p1.z + v.z)));

    EXPECT_TRUE(compare_vector_float(p0 - p1, Vector3f(p0.x - p1.x, p0.y - p1.y, p0.z - p1.z)));
    EXPECT_TRUE(compare_point_float(p0 - v, Point3f(p0.x - v.x, p0.y - v.y, p0.z - v.z)));
    EXPECT_TRUE(compare_point_float(p1 -= v, Point3f(p1.x - v.x, p1.y - v.y, p1.z - v.z)));

    EXPECT_TRUE(compare_point_float(p0 * u, Point3f(p0.x * u, p0.y * u, p0.z * u)));
    EXPECT_TRUE(compare_point_float(p1 *= u, Point3f(p1.x * u, p1.y * u, p1.z * u)));

      
    EXPECT_TRUE(compare_point_float(p0 / u, Point3f(p0.x / u, p0.y / u, p0.z / u)));
    EXPECT_TRUE(compare_point_float(p1 /= u, Point3f(p1.x / u, p1.y / u, p1.z / u)));

    EXPECT_TRUE(compare_point_float(-p0, Point3f(-p0.x, -p0.y, -p0.z)));
    EXPECT_TRUE(compare_float(p0[0], p0.x));
    EXPECT_TRUE(compare_float(p0[1], p0.y));
    EXPECT_TRUE(compare_float(p0[2], p0.z));
}

