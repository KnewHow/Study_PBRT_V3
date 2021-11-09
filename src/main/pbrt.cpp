#include <iostream>
#include <filesystem>

#include "pbrt.h"
#include "geometry.h"
#include "scene.h"
#include "cameras/pinhole.h"
#include "lights/point.h"
#include "film.h"
#include "parallel.h"
#include "material.h"

using namespace pbrt;

int main(int argc, char* argv[]) {
    google::InitGoogleLogging(argv[0]);
    std::string log_dir = "/tmp/pbr_run_log";
    std::string stat_path = "/tmp/pbr_run_log/stat.txt";
    FILE *fp;
    fp = fopen (stat_path.c_str(), "w+");
    bool log_dir_is_exsit = std::filesystem::exists(log_dir.c_str());
    if(!log_dir_is_exsit) {
        std::filesystem::create_directory(log_dir.c_str());
    }
    FLAGS_log_dir = log_dir.c_str();
    
    std::vector<std::shared_ptr<Light>> lights;
    lights.push_back(std::make_shared<PointLight>(Point3f(0, 2, 0), RGBAf(1, 1, 1, 1)));
    std::shared_ptr<Scene> scene = std::make_shared<Scene>("../resource/connelbox/connelbox.obj", lights);
    Point2i fullResolution = Point2i(1024, 1024);
    std::shared_ptr<Film> film = std::make_shared<Film>(fullResolution, "result.ppm");
    Transform cameramTransform = LookAt(Point3f(278, 273, -800), Point3f(0, 0, 1), Vector3f(0, 1, 0)) * RotateZ(90) * RotateY(-90) * RotateX(120);
    std::shared_ptr<Camera> camera = std::make_shared<PinholeCamera>(Inverse(cameramTransform), film);
    ParallelForLoopExecutor::Init(std::nullopt);
    ParallelForLoopExecutor::ParallelFor2D([&](Point2i p){
        Ray ray;
        Float weight = camera->generateRay(p, ray);
        SurfaceInteraction isect;
        bool r = scene->accel->Intersect(ray, isect);
        if(r) {
           RGBAf specturm = isect.primitive->GetMaterial()->kd;
            film->AddSplat(p, specturm);
        } else {
            film->AddSplat(p, RGBAf(0,0,0,1));
        }
    }, fullResolution);
    ParallelForLoopExecutor::MergeWorkerThreadStats(); 
    ParallelForLoopExecutor::PrintStats(fp);
    ParallelForLoopExecutor::Clean();
    film->WriteImage();
    return 0;
}
