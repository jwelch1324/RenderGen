#ifndef QBIMG_H_
#define QBIMG_H_

#include "common.h"
#include "io/tgaimage.h"
#include <SDL.h>
#include <string>
#include <vector>

namespace rengen::draw {

class SDLImageCanvas {
public:
  SDLImageCanvas();
  ~SDLImageCanvas();

  // Initialize
  void Initialize(const int xSize, const int ySize, SDL_Renderer *pRenderer);

  // Function to set the color of a pixel
  void SetPixel(const int x, const int y, const double red, const double green,
                const double blue);

  inline int GetXSize() const { return m_xSize; }
  inline int GetYSize() const { return m_ySize; }

  // Function to return the image for display
  void Display();

private:
  Uint32 ConvertColor(const double red, const double green, const double blue);
  void InitTexture();
  void ComputeMaxValues();

private:
  // Arrays to store image data
  std::vector<std::vector<double>> m_rChannel;
  std::vector<std::vector<double>> m_gChannel;
  std::vector<std::vector<double>> m_bChannel;

  // Image Dimensions
  int m_xSize, m_ySize;
  SDL_Renderer *m_pRenderer;
  SDL_Texture *m_pTexture;

  Float m_maxRed, m_maxGreen, m_maxBlue, m_overallMax;
};
} // namespace rengen::draw

#endif