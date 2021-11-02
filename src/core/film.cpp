#include "film.h"

namespace pbrt {

void Film::WriteImage() const {
    FILE* fp = fopen(filepath.c_str(), "wb");
    (void)fprintf(fp, "P6\n%d %d\n255\n", fullResolution.x, fullResolution.y);
    for(int i = 0; i < pixel.size(); ++i) {
        static unsigned char color[3];
        color[0] = (unsigned char)(255 * std::pow(Clamp(pixel[i].R, 0, 1), 0.6f));
        color[1] = (unsigned char)(255 * std::pow(Clamp(pixel[i].G, 0, 1), 0.6f));
        color[2] = (unsigned char)(255 * std::pow(Clamp(pixel[i].B, 0, 1), 0.6f));
        fwrite(color, 1, 3, fp);
    }
    fclose(fp);
}

void Film::AddSplat(const Point2i &p, const RGBAf &spectrum) {
    int idx = p.x * fullResolution.x + p.y;
    pixel[idx] =  spectrum;
}

} // namespace pbrt