#include "geometry/shapes/sphere.h"
#include "geometry/point.h"
#include "geometry/vector.h"

namespace rengen::geometry::shapes {

bool Sphere::Intersect(const Ray &ray, Float *tHit, Interaction *isect,
                       bool testAlphaTexuture) const {
  Vec3f oc = ray.o - m_center;
  Float a = ray.d * ray.d;
  Float b = 2.0f * oc * ray.d;
  Float c = oc * oc - m_radius * m_radius;
  Float discriminant = b * b - 4 * a * c;
  if (discriminant < 0)
    return false;
  else {
    *tHit = (-b - std::sqrt(discriminant)) / (2.0 * a);
    isect->m_P = ray(*tHit);
    isect->m_time = *tHit;
    isect->m_n = Normal(isect->m_P - m_center);
    return true;
  }
}

} // namespace rengen::geometry::shapes