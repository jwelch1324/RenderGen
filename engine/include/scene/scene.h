#ifndef SCENE_H_
#define SCENE_H_

#include "draw/sdlimagecanvas.h"
#include "geometry/shapes/shape.h"
#include "scene/camera.h"
#include "scene/light.h"
#include <SDL.h>
#include <memory>
#include <vector>

// Forward Declare Renderers So we can make friends of the class
namespace rengen::system::render {
class RenderSubSystem;
class SerialRenderer;
} // namespace rengen::system::render

namespace rengen::scene {
class Scene {
public:
  // Default constructor
  Scene();

  // Function to perform the actual rendering.
  bool Render(draw::SDLImageCanvas &outputImage);
  void AddShape(std::shared_ptr<geometry::Shape> shape) {
    m_shapes.push_back(shape);
  }
  friend class system::render::RenderSubSystem;
  friend class system::render::SerialRenderer;
  // Private functions
private:
  // Private Members
private:
  Camera m_camera;
  std::vector<std::shared_ptr<geometry::Shape>> m_shapes;
  std::vector<std::shared_ptr<Light>> m_lights;
};

} // namespace rengen::scene

#endif