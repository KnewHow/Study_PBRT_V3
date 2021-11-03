#if defined(_MSC_VER)
#define NOMINMAX
#pragma once
#endif

#ifndef PBRT_SRC_CORE_FILM_H_
#define PBRT_SRC_CORE_FILM_H_

#include "pbrt.h"
#include "geometry.h"
#include "spectrum.h"

namespace pbrt {

/**
 * A film store all pixel RGBA color in final image, and provide
 * some others operations, such as write RGBA into a image file.
*/
class Film {
public:
    Film(const Point2i &fullResolution, const std::string &filepath)
        :fullResolution(fullResolution), filepath(filepath) {
            pixel.clear();
            std::fill_n(std::back_inserter(pixel), fullResolution.x * fullResolution.y, RGBAf(0, 0, 0, 1));
        }
    void WriteImage() const;
    void AddSplat(const Point2i &p, const RGBAf &spectrum);
    inline Float GetRatio() const { return (Float)fullResolution.x / fullResolution.y; }  
    const Point2i fullResolution;
    const std::string filepath; // the final image path

private:
    std::vector<RGBAf> pixel;

};

} // namespace pbrt

#endif // PBRT_SRC_CORE_FILM_H_