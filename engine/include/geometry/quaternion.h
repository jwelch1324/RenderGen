#ifndef QUATERNION_H_
#define QUATERNION_H_

#include "geometry/vector.h"
#include "ops/transform.h"
#include <algorithm>
#include <cmath>

namespace rengen::geometry {
class Quaternion {
public:
  Vec3f m_v;
  Float m_w;

  Quaternion() : m_v(0, 0, 0), m_w(1) {}
  Quaternion(const Vec3f &v, const Float w) : m_v(v), m_w(w) {}
  Quaternion(const ops::Transform &t);

  Quaternion &operator+=(const Quaternion &q) {
    m_v += q.m_v;
    m_w += q.m_w;
    return *this;
  }
  Quaternion operator+(const Quaternion &q) const {
    return Quaternion(m_v + q.m_v, m_w + q.m_w);
  }
  Quaternion operator-(const Quaternion &q) const {
    return Quaternion(m_v - q.m_v, m_w - q.m_w);
  }
  Quaternion operator*(Float f) const { return Quaternion(f * m_v, f * m_w); }
  Quaternion &operator*=(Float f) {
    m_v *= f;
    m_w *= f;
    return (*this);
  }

  Quaternion operator*(const Quaternion &q) const {
    Quaternion new_q;
    new_q.m_v = m_v ^ q.m_v + m_w * q.m_v + q.m_w * m_v;
    new_q.m_w = m_w * q.m_w - (m_v * q.m_v);
    return new_q;
  }

  Float Dot(const Quaternion &q) const { return m_v * q.m_v + m_w * q.m_w; }

  static inline Quaternion Normalize(const Quaternion &q) {
    return q / std::sqrt(q.Dot(q));
  }

  Quaternion operator/(Float f) const {
    Float inv = 1.0f / f;
    return Quaternion(inv * m_v, inv * m_w);
  }

  Quaternion &operator/=(Float f) {
    Float inv = 1.0f / f;
    m_v *= inv;
    m_w *= inv;
    return (*this);
  }

  inline Float qx() const { return m_v.x; }
  inline Float qy() const { return m_v.y; }
  inline Float qz() const { return m_v.z; }
  inline Float qw() const { return m_w; }

  ops::Transform ToTransform() const;
};

Quaternion Slerp(Float t, const Quaternion &q1, const Quaternion &q2);

} // namespace rengen::geometry

#endif