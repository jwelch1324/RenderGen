#include "scene/lights/pointlight.h"
#include "geometry/interaction.h"
#include "geometry/normal.h"
#include "geometry/point.h"
#include "geometry/ray.h"
#include "geometry/vector.h"
#include <cmath>
#include <cstddef>

namespace rengen::scene::lights {
PointLight::PointLight() {
  m_color = geometry::Point3f(1.0, 1.0, 1.0);
  m_intensity = 1.0;
  m_position = geometry::Point3f(0, 0, 0);
}

PointLight::~PointLight() {}

// Compute illumination
bool PointLight::ComputeIllumination(
    const geometry::Point3f &intersectionPoint,
    const geometry::Normal3f &localNormal,
    const std::vector<std::shared_ptr<rengen::geometry::Shape>> &objList,
    const std::shared_ptr<rengen::geometry::Shape> &currentObject,
    geometry::Point3f &color, Float &intensity) {

  // construct a vector from the intersection point to the light
  geometry::Vec3f lightDir =
      geometry::Vec3f::Normalize(m_position - intersectionPoint);

  // compute a staring point
  geometry::Point3f startPoint = intersectionPoint;

  // Construct a ray from the point of intersection to the light.
  geometry::Ray lightRay(startPoint, lightDir);

  // check for intersections with all of the objects in the scene, except for
  // the current one...
  bool validInt = false;

  for (auto sceneObject : objList) {
    if (sceneObject != currentObject) {
      validInt = sceneObject->Intersect(lightRay, nullptr, nullptr);

      // If we have an intersection, then there is no point checking further so
      // we can break -- i.e. this object is blocking light from this source...
      if (validInt)
        break;
    }
  }

  // Only continue to compute illumination if the light ray didn't strike
  // something else
  if (validInt) {
    color = m_color;
    intensity = 0.0;
    return false;
  }

  // Compute the angle between the local normal and the light ray
  Float angle = std::acos(
      geometry::Vec3f(geometry::Normal3f::Normalize(localNormal)) * lightDir);

  if (angle > 1.5708) {
    // No illumination
    color = m_color;
    intensity = 0.0;
    return false;
  } else {
    color = m_color;
    intensity = m_intensity * (1.0 - angle / 1.5708);
    return true;
  }
}
} // namespace rengen::scene::lights