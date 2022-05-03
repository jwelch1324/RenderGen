#ifndef SHAPE_H_
#define SHAPE_H_

#include "geometry/interaction.h"
#include "geometry/ray.h"
#include "ops/transform.h"

namespace rengen::geometry {
class Shape {
public:
  Shape(const rengen::ops::Transform *objectToWorld,
        const rengen::ops::Transform *worldToObject, bool reverseOrientation)
      : m_ObjectToWorld(objectToWorld), m_WorldToObject(worldToObject),
        m_ReverseOrientation(reverseOrientation),
        transformSwapsHandedness(objectToWorld->SwapsHandedness()) {}

  const rengen::ops::Transform *m_ObjectToWorld, *m_WorldToObject;
  const bool m_ReverseOrientation;
  const bool transformSwapsHandedness;

  virtual ~Shape() {}

  virtual bool Intersect(const Ray &ray, Float *tHit, Interaction *isect,
                         bool testAlphaTexuture = true) const = 0;
};
} // namespace rengen::geometry

#endif