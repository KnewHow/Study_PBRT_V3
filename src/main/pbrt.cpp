#include <iostream>

#include "pbrt.h"
#include "geometric.h"

using namespace pbrt;

int main(int argc, char* argv[]) {
    google::InitGoogleLogging(argv[0]);
    #ifndef NDEBUG
        FLAGS_logtostderr = true;
    #endif
    Vector2i v1(1,1);
    Vector2i v2(2,2);
    Vector2f v3(1.2, 3.4);
    LOG(INFO) << "size of Float:" << v3;
}
