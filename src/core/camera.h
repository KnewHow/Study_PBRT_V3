#if defined(_MSC_VER)
#define NOMINMAX
#pragma once
#endif

#ifndef PBRT_SRC_CORE_CAMERA_CPP_
#define PBRT_SRC_CORE_CAMERA_CPP_

#include "pbrt.h"
#include "geometry.h"
#include "transform.h"
#include "film.h"

namespace pbrt {

/**
 * a basic class of camera, we use coordinates transform let 
 * camera locate in (0, 0, 0), look at positive z direct. So when we
 * generate a ray from camera, we need to transform it from camera coordinates to world coordinates.
 * Use this way, we can move camera in the world
*/
class Camera {
public:
    Camera(const Transform &cameraToWorld, std::shared_ptr<Film> film)
        :cameraToWorld(cameraToWorld), film(film){}
    /**
     * Generate ray.
     * @param p the point in the film
     * @param ray the ray will be return as the generated ray.
     * @return the value represent the weight of how many radiance contribute to this point. 
    */
    virtual Float generateRay(const Point2i &p, Ray &ray) const = 0;
protected:
    std::shared_ptr<Film> film; // a film store some image informations
    Transform cameraToWorld; // 
};


} // namespace pbrt

#endif // PBRT_SRC_CORE_CAMERA_CPP_