#ifndef LIGHT_H_
#define LIGHT_H_

#include "geometry/normal.h"
#include "geometry/point.h"
#include "geometry/shapes/shape.h"
#include "geometry/vector.h"
#include <memory>

namespace rengen::scene {
class Light {
public:
  Light();
  virtual ~Light();

  // Function to compute illumination contribution.
  virtual bool ComputeIllumination(
      const geometry::Point3f &intersectionPoint,
      const geometry::Normal3f &localNormal,
      const std::vector<std::shared_ptr<rengen::geometry::Shape>> &objList,
      const std::shared_ptr<rengen::geometry::Shape> &currentObject,
      geometry::Point3f &color, Float &intensity);

  geometry::Point3f m_color;
  geometry::Point3f m_position;
  Float m_intensity;
};
} // namespace rengen::scene

#endif