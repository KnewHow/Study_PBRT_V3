#include <iostream>

#include "pbrt.h"

int main(int argc, char* argv[]) {
    google::InitGoogleLogging(argv[0]);
    #ifndef NDEBUG
        FLAGS_logtostderr = true;
    #endif
    LOG(INFO) << "This is test log!" << std::endl;
}
