#ifndef SPHERE_H_
#define SPHERE_H_

#include "geometry/interaction.h"
#include "geometry/point.h"
#include "geometry/shapes/shape.h"
#include "ops/transform.h"

namespace rengen::geometry::shapes {
class Sphere : public Shape {
public:
  Sphere(Float radius, rengen::ops::Transform *objectToWorld,
         rengen::ops::Transform *worldToObject)
      : Shape(objectToWorld, worldToObject, false) {
    m_radius = radius;
    // The center is defined by the object to world transform acting on the
    // origin.
    m_center = (*objectToWorld)(Point3f(0, 0, 0));
  }
  
  bool Intersect(const Ray &ray, Float *tHit, Interaction *isect,
                 bool testAlphaTexuture = true) const override;

  Float m_radius;
  Point3f m_center;
};
} // namespace rengen::geometry::shapes

#endif