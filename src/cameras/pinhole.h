#if defined(_MSC_VER)
#define NOMINMAX
#pragma once
#endif

#ifndef PBRT_SRC_CAMERA_PINHOLE_H_
#define PBRT_SRC_CAMERA_PINHOLE_H_

#include "camera.h"
#include "geometry.h"
namespace pbrt {

class PinholeCamera: public Camera {
public:
    PinholeCamera(const Transform &cameraToWorld, std::shared_ptr<Film> film)
        :Camera(cameraToWorld, film){}
    virtual Float generateRay(const Point2i &p, Ray &ray) const override;
};

} // namespace pbrt

#endif // PBRT_SRC_CAMERA_PINHOLE_H_