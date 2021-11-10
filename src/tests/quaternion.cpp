#include "pbrt_test.h"
#include "quaternion.h"
#include "transform.h"

using namespace pbrt;

TEST(Quaternion, BasicTest) {
    Transform t1 = RotateX(30) * RotateY(60);
    Transform t2 = RotateX(56) * RotateZ(120);
    Quaternion q1(t1);
    Quaternion q2(t2);
    Quaternion q3 = q1 + q2;
    LOG(INFO) << "[test Quaternion] sum is: " << q3;
    Quaternion q4 =  Slerp(0.5, q1, q2);
    LOG(INFO) << "[test Quaternion] lerp is: " << q4;
}