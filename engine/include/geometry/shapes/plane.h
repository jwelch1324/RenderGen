#ifndef __PLANE_H__
#define __PLANE_H__
#include "geometry/shapes/shape.h"
#include "ops/transform.h"

namespace rengen::geometry::shapes {

class Plane : public Shape {
public:
  Plane();
  Plane(ops::Transform *objectToWorld, ops::Transform *worldToObject);
  virtual ~Plane() override;

  virtual bool Intersect(const Ray &ray, Float *tHit, Interaction *isect,
                         bool testAlphaTexuture = true) const override;
};
} // namespace rengen::geometry::shapes

#endif // __PLANE_H__