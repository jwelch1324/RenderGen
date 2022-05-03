#include "io/ppmimage.h"
#include "common.h"
#include "io/tgaimage.h"
#include <cstring>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <string>

namespace rengen::io {
void PPMImage::Write(std::string outPath) const {
  std::ofstream file(outPath, std::ios::out);
  std::stringstream ss;
  if (!file.is_open()) {
    printf("Unable to open the file %s for writing\n", outPath.c_str());
    return;
  }

  // Write the standard header
  file << "P3\n"
       << this->m_nx << " " << this->m_ny << "\n"
       << this->m_maxPixelValue << "\n";

  char buffer[1024] = {0};
  // Now for each RGB value we write out the raw uint value
  for(int j = m_ny-1; j>= 0; j--){
    for(int i = 0; i<m_nx; i++){
      auto pixel = m_pixels[i][j];
      sprintf(buffer, "%d %d %d\n", pixel.r(), pixel.g(), pixel.b());
      auto len = std::strlen(buffer);
      file << buffer;
      memset(buffer, 0, len);
    }
  }

  file.close();
}

void PPMImage::Set(int x, int y, const RGBAColor &color) {
  if ((x >= m_nx) || (y >= m_ny)) {
    printf("pixel value out of range... %d, %d", x, y);
    return;
  }

  m_pixels[x][y] = color;
}

PPMImage &&PPMImage::Read(std::string inPath) {
  return std::move(PPMImage(0,0));
  std::ifstream file(inPath, std::ios::in);
  if (!file.is_open()) {
    return std::move(PPMImage(0, 0));
  }
  std::vector<std::string> lines;
  std::string line;
  while (std::getline(file, line, '\n')) {
    if (line[0] == '#') {
      continue;
    }
    lines.push_back(line);
  }

  // The first three non commented lines are the header
  if (lines[0] != "P3") {
    return std::move(PPMImage(0, 0));
  }
  auto dims = strsplit(lines[1], ' ');
  int nx = atoi(dims[0].c_str());
  int ny = atoi(dims[1].c_str());

  int maxValue = atoi(lines[2].c_str());

  auto newImg = PPMImage(nx, ny, maxValue);

  RGBAColor c;
  int ii = 0;
  int jj = 0;
  for (int i = 3; i < lines.size(); i++) {
    // auto fields = strsplit(lines[i].c_str(), ' ');
    // c.r(atoi(fields[ff].c_str()));
    // c.g(atoi(fields[ff + 1].c_str()));
    // c.b(atoi(fields[ff + 2].c_str()));
    // newImg.m_pixels[i][jj] = c;

  }
  return std::move(newImg);
}

bool PPMImage::operator==(const PPMImage &o) const {
  if ((m_nx != o.m_nx) || (m_ny != o.m_ny))
    return false;

  for (int i = 0; i < m_nx; i++) {
    for (int j = 0; j < m_ny; j++) {
      auto &c0 = m_pixels[i][j];
      auto &c1 = o.m_pixels[i][j];
      if (c0 != c1)
        return false;
    }
  }
  return true;
}

} // namespace rengen::io