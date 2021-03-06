#ifndef POINT_H_
#define POINT_H_

#include "common.h"
#include "geometry/vector.h"

namespace rengen::geometry {
template <class T> class COREDLL Point {
public:
  T x, y, z;
  Point(T _x = 0, T _y = 0, T _z = 0) : x(_x), y(_y), z(_z) {}

  inline bool operator==(const Point<T> &o) const {
    return (x == o.x) && (y == o.y) && (z == o.z);
  }

  // Add a vector to the point
  Point<T> operator+(const Vector<T> &v) const {
    return Point(x + v.x, y + v.y, z + v.z);
  }
  Point<T> &operator+=(const Vector<T> &v) {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
  }

  Vector<T> operator-(const Point<T> &p) const {
    return Vector<T>(x - p.x, y - p.y, z - p.z);
  }

  Point<T> operator-(const Vector<T> &v) const { return (*this) + (-v); }

  Point<T> &operator-=(const Vector<T> &v) {
    (*this) += (-v);
    return *this;
  }

  Point<T> operator+(const Point<T> &p) const {
    return Point<T>((T)(x + p.x), (T)(y + p.y), (T)(z + p.z));
  }

  Point<T> &operator+(const Point<T> &p) {
    x += p.x;
    y += p.y;
    z += p.z;
    return (*this);
  }

  Point<T> operator*(Float f) const {
    return Point<T>((T)(x * f), (T)(y * f), (T)(z * f));
  }

  Point<T> &operator*=(Float f) {
    x = T(x * f);
    y = T(y * f);
    z = T(z * f);
    return *this;
  }

  Point<T> operator/=(Float f) {
    Float inv = 1.0f / f;
    x = T(x * inv);
    y = T(y * inv);
    z = T(z * inv);
    return *this;
  }

  Point<T> operator/(Float f) {
    Float inv = 1.0f / f;
    return Point<T>(T(x * inv), T(y * inv), T(z * inv));
  }
};

class RGBColor : public Point<Float> {
public:
  RGBColor(Float r = 0, Float g = 0, Float b = 0) : Point<Float>(r, g, b) {}
  Float &r() { return x; }
  Float &g() { return y; }
  Float &b() { return z; }
};

typedef Point<Float> Point3f;
typedef Point<Float> P3f;
typedef Point<int> P3i;
typedef Point<int> Point3i;

template <class T>
inline Float L2Distance(const Point<T> &p1, const Point<T> &p2) {
  return (p1 - p2).norm();
}

template <class T>
inline Float L2DistanceSquared(const Point<T> &p1, const Point<T> &p2) {
  return (p1 - p2).LengthSquared();
}

template <class T> inline Point<T> operator*(const Float f, const Point<T> &p) {
  return p * f;
}
} // namespace rengen::geometry

#endif