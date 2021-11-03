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
    PinholeCamera(const Transform &cameraToWorld, std::shared_ptr<Film> film, Point3f p)
        :Camera(cameraToWorld, film), position(p){}
    virtual Float generateRay(const Point2i &p, Ray &ray) const override;
private:
    Point3f position;
};

} // namespace pbrt

#endif // PBRT_SRC_CAMERA_PINHOLE_H_