#include "pinhole.h"

namespace pbrt {

Float PinholeCamera::generateRay(const Point2i &p, Ray &ray) const {
    float scale = std::tan(DegTorad(45.0 * 0.5));
    Float x = (2 * (p.x + 0.5) / (Float)film->fullResolution.x - 1) *
                      film->GetRatio() * scale;
    Float y = (1 - 2 * (p.y + 0.5) / (Float)film->fullResolution.y) * scale;
    ray.o = position;
    ray.d = Normalize(Vector3f(x, y, 1));
    return 1.0;
}

} // namespace pbrt