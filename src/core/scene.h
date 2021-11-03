#if defined(_MSC_VER)
#define NOMINMAX
#pragma once
#endif

#ifndef PBRT_SRC_CORE_SCENE_H_
#define PBRT_SRC_CORE_SCENE_H_

#include <map>

#include "pbrt.h"
#include "geometry.h"
#include "light.h"
#include "primitive.h"
#include "accelerators/bvh.h"

namespace pbrt {

class Scene {
public:
    Scene(const std::string &modelPath, const std::vector<std::shared_ptr<Light>> &lights)
        :modelPath(modelPath), lights(lights) {
            std::vector<std::shared_ptr<Primitive>> ps;
            bool r = loadModel(ps, modelPath);
            if(!r) {
                LOG(FATAL) << "load model failure from path: " << modelPath;
            }
            accel = std::make_shared<BVHAccel>(ps);
    }
    static bool loadModel(std::vector<std::shared_ptr<Primitive>> &ps, const std::string& path);
    static std::map<std::string, std::vector<std::shared_ptr<Primitive>>> PSCache;

    const std::string modelPath;
    std::vector<std::shared_ptr<Light>> lights;
    std::shared_ptr<BVHAccel> accel;
};

} // namespace pbrt

#endif // PBRT_SRC_CORE_SCENE_H_