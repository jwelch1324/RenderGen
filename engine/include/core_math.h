#ifndef CORE_MATH_H_
#define CORE_MATH_H_

#include "common.h"
#include "geometry/geometry.h"
#include "geometry/quaternion.h"
#include <cmath>

namespace rengen::geometry::ops {
template <class T> inline Float Dot(const Vector<T> &v1, const Vector<T> &v2);

template <class T> inline Float Dot(const Vector<T> &v, const Normal<T> &n);

template <class T> inline Float Dot(const Normal<T> &n, const Vector<T> &v);

template <class T> inline Float Dot(const Normal<T> &n, const Normal<T> &n2);

inline Float Dot(const Quaternion &q1, const Quaternion &q2);

template <class T>
inline Float AbsDot(const Vector<T> &v1, const Vector<T> &v2);

template <class T> inline Float AbsDot(const Vector<T> &v, const Normal<T> &n);

template <class T> inline Float AbsDot(const Normal<T> &n, const Vector<T> &v);

template <class T>
inline Float AbsDot(const Normal<T> &n1, const Normal<T> &n2);

template <class T>
inline Vector<T> Cross(const Vector<T> &v1, const Vector<T> &v2);

inline void MakeCoordinateSystem(const Vec3f &v1_in, Vec3f *v2_out,
                                 Vec3f *v3_out);

inline Quaternion Normalize(const Quaternion &q) {
  return q / Float(std::sqrt(Dot(q, q)));
}

inline BBox Union(const BBox &b, const P3f &p) { return b.Union(p); }

inline BBox Union(const BBox &b1, const BBox &b2) { return b1.Union(b2); }

// Linear Interpolation
inline Float Lerp(Float t, Float v1, Float v2) {
  return (1.f - t) * v1 + t * v2;
}
} // namespace rengen::geometry::ops

#endif