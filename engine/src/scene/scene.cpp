#include "scene/scene.h"

namespace rengen::scene {
Scene::Scene() {}

// Function to do actual rendering
bool Scene::Render(draw::SDLImageCanvas &outputImage) {
  // Get dimensions of output image
  int xSize = outputImage.GetXSize();
  int ySize = outputImage.GetYSize();

  // Create some color variations
  for (int x = 0; x < xSize; x++)
    for (int y = 0; y < ySize; y++) {
      double red = (double(x) / xSize) * 255.0;
      double blue = double(y) / ySize * 255.0;
      outputImage.SetPixel(x, y, red, 0.0, blue);
    }

  return true;
}
} // namespace rengen::scene