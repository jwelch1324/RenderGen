#ifndef POINTLIGHT_H_
#define POINTLIGHT_H_

#include "scene/light.h"
namespace rengen::scene::lights {
class PointLight : public Light {
public:
  PointLight();
  virtual ~PointLight() override;

  virtual bool ComputeIllumination(
      const geometry::Point3f &intersectionPoint,
      const geometry::Normal3f &localNormal,
      const std::vector<std::shared_ptr<rengen::geometry::Shape>> &objList,
      const std::shared_ptr<rengen::geometry::Shape> &currentObject,
      geometry::Point3f &color, Float &intensity) override;
};
} // namespace rengen::scene::lights

#endif