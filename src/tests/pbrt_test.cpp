#include "pbrt_test.h"
#include "OBJ_Loader.h"
#include "clock.h"
#include "geometry.h"
#include "pbrt.h"
#include "shape.h"
#include "shape/triangle.h"
#include "primitive.h"
#include "material.h"

namespace pbrt {
void generateTestRays(std::vector<Ray> &rays, int size) {
    rays.reserve(size);
    for(int i = 0; i < size; i++) {
        Ray r(Point3f(0, 0, 0), Normalize(Vector3f(get_random_Float(), get_random_Float(), get_random_Float())));
        rays.push_back(r) ;
    }
}

void printTime(const std::string &prefix, const std::chrono::milliseconds &begin, const std::chrono::milliseconds &end) {
    LOG(INFO) << prefix << (end - begin).count() << " ms.";
}

} // namespace pbrt