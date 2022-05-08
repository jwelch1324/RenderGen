#ifndef RAY_H_
#define RAY_H_

#include "common.h"
#include "geometry/point.h"
#include "geometry/vector.h"

namespace rengen::geometry {
class Ray {
public:
  Point<Float> o;
  Vector<Float> d;
  mutable Float mint, maxt;
  Float time;
  Ray() : mint(RAY_EPSILON), maxt(INFINITY), time(0.f) {}
  Ray(const Point<Float> &origin, const Vector<Float> &direction,
      Float start = RAY_EPSILON, Float end = INFINITY, Float t = 0.f)
      : o(origin), d(direction), mint(start), maxt(end), time(t) {}

  Point<Float> operator()(Float t) const { return o + t * d; }
};

class RayDifferential : public Ray {
public:
  bool hasDifferentials;
  Ray rx, ry;

  RayDifferential() { hasDifferentials = false; }
  RayDifferential(const Point3f &origin, const Vec3f &direction)
      : Ray(origin, direction) {
    hasDifferentials = false;
  }
  explicit RayDifferential(const Ray &r) : Ray(r) { hasDifferentials = false; }
};
} // namespace rengen::geometry

#endif