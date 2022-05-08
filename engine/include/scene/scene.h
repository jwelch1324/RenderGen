#ifndef SCENE_H_
#define SCENE_H_

#include "draw/sdlimagecanvas.h"
#include <SDL.h>
#include <vector>

namespace rengen::scene {

class Scene {
public:
  // Default constructor
  Scene();

  // Function to perform the actual rendering.
  bool Render(draw::SDLImageCanvas &outputImage);

  // Private functions
private:
  // Private Members
private:
};

} // namespace rengen::scene

#endif