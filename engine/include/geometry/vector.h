#ifndef VECTOR_H_
#define VECTOR_H_

#include "common.h"

namespace rengen::geometry {
template <class T> class COREDLL Normal;

template <class t> struct Vec2 {
  t x, y;
  Vec2<t>() : x(t()), y(t()) {}
  Vec2<t>(t _x, t _y) : x(_x), y(_y) {}
  Vec2<t>(const Vec2<t> &v) : x(t()), y(t()) { *this = v; }
  Vec2<t> &operator=(const Vec2<t> &v) {
    if (this != &v) {
      x = v.x;
      y = v.y;
    }
    return *this;
  }
  Vec2<t> operator+(const Vec2<t> &V) const {
    return Vec2<t>(x + V.x, y + V.y);
  }
  Vec2<t> operator-(const Vec2<t> &V) const {
    return Vec2<t>(x - V.x, y - V.y);
  }
  Vec2<t> operator*(float f) const { return Vec2<t>(x * f, y * f); }
  t &operator[](const int i) {
    if (x <= 0)
      return x;
    else
      return y;
  }
  template <class> friend std::ostream &operator<<(std::ostream &s, Vec2<t> &v);
};

template <class T> class COREDLL Vector {
public:
  T x, y, z;
  Vector<T>(T _x = 0, T _y = 0, T _z = 0) : x(_x), y(_y), z(_z) {}

  explicit Vector<T>(const Normal<T> &n) : x(n.x), y(n.y), z(n.z) {}

  // Equality of Two Vectors
  bool operator==(const Vector<T> &rhs) const {
    return (x == rhs.x) && (y == rhs.y) && (z == rhs.z);
  }

  bool operator!=(const Vector<T> &rhs) const {
    return (x != rhs.s) || (y != rhs.y) || (z != rhs.z);
  }

  bool epsilon_eq(const Vector<T> &rhs, Float eps = 1e-6) const {
    return (abs(x - rhs.x) <= eps) && (abs(y - rhs.y) <= eps) &&
           (abs(z - rhs.z) <= eps);
  }

  // Addition/Subtraction of Two Vectors
  Vector<T> operator+(const Vector<T> &v) const {
    return Vector<T>(x + v.x, y + v.y, z + v.z);
  }

  Vector<T> &operator+=(const Vector<T> &v) {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
  }

  Vector<T> operator-(const Vector<T> &v) const {
    return Vector<T>(x - v.x, y - v.y, z - v.z);
  }

  Vector<T> &operator-=(const Vector<T> &v) {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
  }

  // Scaling of a Vector
  Vector<T> operator*(Float f) const {
    return Vector<T>((T)(x * f), (T)(y * f), (T)(z * f));
  }

  Vector<T> &operator*=(Float f) {
    x = T(x * f);
    y = T(y * f);
    z = T(z * f);
    return *this;
  }

  Vector<T> operator/(Float f) const {
    assert(f != 0);
    Float inv = 1.0f / f;
    return Vector<T>((T)(x * inv), (T)(y * inv), (T)(z * inv));
  }

  Vector<T> operator/=(Float f) {
    assert(f != 0);
    Float inv = 1.0f / f;
    x *= f;
    y *= f;
    z *= f;
    return *this;
  }

  // Vector Product Operators

  // Dot Product between two vectors
  inline Float operator*(const Vector<T> &rhs) const {
    return x * rhs.x + y * rhs.y + z * rhs.z;
  }

  inline Float cosine(const Vector<T> &o) const {
    return (this * o) / (this->norm() * o.norm());
  }

  // Cross Product between two vectors
  inline Vector<T> operator^(const Vector<T> &rhs) const {
    return Vector<T>(y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z,
                     x * rhs.y - y * rhs.x);
    // y*v.z-z*v.y, z*v.x-x*v.z, x*v.y-y*v.x
  }

  inline Float sine(const Vector<T> &o) const {
    // Sine of the angle is the ratio of the norm of the cross product and the
    // product of norms of the individual vectors
    return ((*this) ^ o).norm() / (this->norm() * o.norm());
  }

  // Negation Operator
  Vector<T> operator-() const { return Vector<T>(-x, -y, -z); }

  // Indexing Operator
  T operator[](int i) const {
    assert(i >= 0 && i <= 2);
    return (&x)[i];
  }

  T &operator[](int i) {
    assert(i >= 0 && i <= 2);
    return (&x)[i];
  }

  // Normalization Functions
  inline Float norm() const {
    // Perform dot product with self and take sqrt
    return std::sqrt(x * x + y * y + z * z);
  }

  inline Float Length() const { return norm(); }

  inline Float LengthSquared() const { return *this * *this; }

  static inline Vector<T> Normalize(const Vector<T> &v) {
    return v * 1 / v.norm();
  }

  // Type Conversion Functions

  // Convert the current vector to a Vec3f
  Vector<Float> ToVec3f() const {
    return Vector<Float>(Float(x), Float(y), Float(z));
  }

  // Convert the current vector to a Vec3i
  Vector<int> ToVec3i() const { return Vector<int>(int(x), int(y), int(z)); }
};

typedef Vector<Float> Vec3f;
typedef Vector<int> Vec3i;

typedef Vec2<int> Vec2i;
typedef Vec2<Float> Vec2f;

template <class T> inline Vector<T> operator*(Float f, const Vector<T> &v) {
  return v * f;
}

} // namespace rengen::geometry
#endif