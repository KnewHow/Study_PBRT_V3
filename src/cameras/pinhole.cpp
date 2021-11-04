#include "pinhole.h"

namespace pbrt {

Float PinholeCamera::generateRay(const Point2i &p, Ray &ray) const {
    float scale = std::tan(Radians(40.0 * 0.5));
    Float x = (2 * (p.x + 0.5) / (Float)film->fullResolution.x - 1) *
                      film->GetRatio() * scale;
    Float y = (1 - 2 * (p.y + 0.5) / (Float)film->fullResolution.y) * scale;
    ray.o = Point3f(0, 0, 0);
    ray.d = Normalize(Vector3f(x, y, 1));
    ray = cameraToWorld(ray);
    return 1.0;
}

} // namespace pbrt