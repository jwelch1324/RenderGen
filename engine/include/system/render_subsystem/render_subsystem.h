#ifndef __RENDER_SUBSYSTEM_H__
#define __RENDER_SUBSYSTEM_H__
// Abstract base class defining a render subsystem -- the subsystem should be
// able to take in a Scene Definition consisting of objects and lights (definied
// with their appropriate transforms) and consume a camera ray then return a
// pixel color to render at the coordinate originating the camera ray.

// The idea here is that the render subsystem is seperate from the actual scene
// being rendered and can be local, serial, distributed, etc...

#include "geometry/point.h"
#include "geometry/ray.h"
#include "scene/scene.h"
namespace rengen::system::render {

class RenderSubSystem {
public:
  RenderSubSystem() {}

  virtual geometry::RGBColor RenderRay(geometry::Ray &ray) const = 0;
  void SetScene(scene::Scene &sceneIn) { m_sceneReference = sceneIn; }

protected:
  scene::Scene m_sceneReference;
};
} // namespace rengen::system::render
#endif // __RENDER_SUBSYSTEM_H__