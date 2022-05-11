#include "scene/light.h"

namespace rengen::scene {

Light::Light() {}
Light::~Light() {}

bool Light::ComputeIllumination(
    const geometry::Point3f &intersectionPoint,
    const geometry::Normal3f &localNormal,
    const std::vector<std::shared_ptr<rengen::geometry::Shape>> &objList,
    const std::shared_ptr<rengen::geometry::Shape> &currentObject,
    geometry::Point3f &color, Float &intensity) {
  return false;
}
} // namespace rengen::scene