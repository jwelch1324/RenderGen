#ifndef PPMIMAGE_H_
#define PPMIMAGE_H_

#include "io/tgaimage.h"
#include <iostream>
#include <sstream>
#include <string>

namespace rengen::io {

// PPMImage is a RAW RGB Pixel format image --
class PPMImage {
public:
PPMImage(const PPMImage&& o) {
    this->m_pixels = std::move(o.m_pixels);
    m_nx = o.m_nx;
    m_ny = o.m_ny;
    m_maxPixelValue = o.m_maxPixelValue;
}

  PPMImage(int nx, int ny, int maxPixelValue = 255)
      : m_nx(nx), m_ny(ny), m_maxPixelValue(maxPixelValue) {
    // Preallocate the pixel grid
    m_pixels.reserve(nx);
    for (int i = 0; i < nx; i++) {
      m_pixels.push_back(std::vector<RGBAColor>());
    }

    for (auto &it : m_pixels) {
      it.reserve(ny);
      for (int j = 0; j < ny; j++){
         it.push_back(RGBAColor());
      }
    }
  }

  bool operator==(const PPMImage &o) const;

  void Write(std::string outPath) const;
  void Set(int x, int y, const RGBAColor &color);
  static PPMImage&& Read(std::string inPath);

private:
  int m_nx, m_ny, m_maxPixelValue;
  std::vector<std::vector<RGBAColor>> m_pixels; // Row x Column Ordering
};
} // namespace rengen::io

#endif