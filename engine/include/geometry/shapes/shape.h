#ifndef SHAPE_H_
#define SHAPE_H_

#include "geometry/interaction.h"
#include "geometry/point.h"
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

  void SetBaseColor(const Point3f &rgbColor) { m_baseColor = rgbColor; }
  Point3f GetBaseColor() const { return m_baseColor; }

private:
  Point3f m_baseColor; // The default color to render for the shape when it
                       // doesn't have a texture
};
} // namespace rengen::geometry

#endif