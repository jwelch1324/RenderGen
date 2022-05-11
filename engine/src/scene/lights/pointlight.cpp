#include "scene/lights/pointlight.h"
#include "geometry/normal.h"
#include "geometry/point.h"
#include "geometry/vector.h"
#include <cmath>

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

  // Compute the angle between the local normal and the light ray
  // Note that we assume that localNormal is a unit vector
  Float angle = std::acos(
      geometry::Vec3f(geometry::Normal3f::Normalize(localNormal)) * lightDir);

  if (angle > 1.5708) {
    color = m_color;
    intensity = 0.0;
    return false;
  } else {
    color = m_color;
    intensity = m_intensity * (1.0 - (angle / 1.5708));
    return true;
  }
}
} // namespace rengen::scene::lights