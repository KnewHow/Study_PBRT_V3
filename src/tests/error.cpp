#include <iomanip>
#include <limits>

#include "pbrt_test.h"
#include "stringprint.h"
#include "efloat.h"

using namespace pbrt;

TEST(NextFloatUpAndDown, BasicTest) {
    float v = 1.2;
    float r1 = NextFloatUp(v);
    float r2 = NextFloatDown(v);
    LOG(INFO) << "[test float type]" << std::setprecision( std::numeric_limits<float>::max_digits10 ) << "original v is: " << v << ", next float up v is: " << r1 << ", next float down v is: " << r2;
    EXPECT_NE(r1, r2);
    EXPECT_NE(v, r1);
    EXPECT_NE(v, r2);

    double d = 1.2;
    double r3 = NextFloatUp(d);
    double r4 = NextFloatDown(d);
    LOG(INFO) << "[test double type]" << std::setprecision( std::numeric_limits<double>::max_digits10  ) << "original v is: " << d << ", next float up v is: " << r3 << ", next float down v is: " << r4;
    EXPECT_NE(r3, r4);
    EXPECT_NE(d, r3);
    EXPECT_NE(d, r4);
}

TEST(GammaError, BasicTest) {
    Float a = 1.0, b = 2.0, c = 3.0, d = 4.0;
    Float sum = a + b + c + d;
    Float error = gamma(3) * std::abs(a + b) + gamma(2) * std::abs(c) + gamma(1) * std::abs(d);
    Float sum_error = sum + error;
    LOG(INFO) << "[test gamma error] " << std::fixed <<std::setprecision( std::numeric_limits<Float>::max_digits10 ) << "sum is: " << sum << ", error is: " << error << ", sum error is: " << sum_error;
    EXPECT_NE(sum, sum_error);
}

TEST(EFloat, BasicTest) {
    EFloat sum(0);
    for(int i = 1; i <= 100; ++i) {
        EFloat v(i);
        sum = sum + v;
    }
    LOG(INFO) << "[EFloat test] sum is: " << sum;
    EFloat product(1);
    for(int i = 0; i < 10; ++i) {
        EFloat v(get_random_Float(1, 5));
        product = product * v;
    }
    LOG(INFO) << "[EFloat test] product is: " << product;
}