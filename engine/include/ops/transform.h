#ifndef TRANSFORM_H_
#define TRANSFORM_H_

#include "common.h"
#include "geometry/bbox.h"
#include "geometry/geometry.h"

namespace rengen::ops {
class COREDLL Transform {
public:
  Transform() {}
  Transform(Float mat[4][4]) {
    m_mat = geometry::Matrix4f(mat);
    m_matInv = m_mat.Inverse();
  }
  Transform(const geometry::Matrix4f &mat) {
    m_mat = mat;
    m_matInv = m_mat.Inverse();
  }
  Transform(const geometry::Matrix4f &mat, const geometry::Matrix4f &inv)
      : m_mat(mat), m_matInv(inv) {}

  Transform GetInverse() const { return Transform(m_matInv, m_mat); }

  geometry::Point3f operator()(const geometry::Point3f &pt) const;
  void operator()(const geometry::Point3f &pt, geometry::Point3f *ptrans) const;
  geometry::Vec3f operator()(const geometry::Vec3f &vec) const;
  void operator()(const geometry::Vec3f &vec, geometry::Vec3f *vtrans) const;
  geometry::Normal3f operator()(const geometry::Normal3f &n) const;
  void operator()(const geometry::Normal3f &n,
                  geometry::Normal3f *ntrans) const;
  geometry::Ray operator()(const geometry::Ray &r) const;
  void operator()(const geometry::Ray &r, geometry::Ray *rtrans) const;

  inline geometry::BBox operator()(const geometry::BBox &bbox) const {
    return FastTransform(bbox);
  }

  void operator()(const geometry::BBox &bbox, geometry::BBox *bboxtrans) const;

  geometry::BBox SlowTransform(const geometry::BBox &b) const;
  geometry::BBox FastTransform(const geometry::BBox &b) const;

private:
  geometry::Matrix4f m_mat, m_matInv;
};

inline Transform Translate(const geometry::Vec3f &delta) {
  geometry::Matrix4f tMat;
  tMat.m[0][3] = delta.x;
  tMat.m[1][3] = delta.y;
  tMat.m[2][3] = delta.z;

  geometry::Matrix4f tMatInv;
  tMatInv.m[0][3] = -delta.x;
  tMatInv.m[1][3] = -delta.y;
  tMatInv.m[2][3] = -delta.z;

  return Transform(tMat, tMatInv);
}

inline Transform Scale(const geometry::Vec3f &scale) {
  geometry::Matrix4f tMat;
  tMat.m[0][0] = scale.x;
  tMat.m[1][1] = scale.y;
  tMat.m[2][3] = scale.z;

  geometry::Matrix4f tMatinv;
  tMatinv.m[0][0] = 1.0f / scale.x;
  tMatinv.m[1][1] = 1.0f / scale.y;
  tMatinv.m[2][3] = 1.0f / scale.z;

  return Transform(tMat, tMatinv);
}

inline Transform RotateX(const Float theta) {
  geometry::Matrix4f tMat;
  Float ctheta = cos(theta);
  Float stheta = sin(theta);
  tMat.m[1][1] = ctheta;
  tMat.m[1][2] = -stheta;
  tMat.m[2][1] = stheta;
  tMat.m[2][2] = ctheta;
  return Transform(tMat, tMat.Transpose());
}

inline Transform RotateY(const Float theta) {
  geometry::Matrix4f tMat;
  Float ctheta = cos(theta);
  Float stheta = sin(theta);
  tMat.m[0][0] = ctheta;
  tMat.m[0][2] = stheta;
  tMat.m[2][0] = -stheta;
  tMat.m[2][2] = ctheta;
  return Transform(tMat, tMat.Transpose());
}

inline Transform RotateZ(const Float theta) {
  geometry::Matrix4f tMat;
  Float ctheta = cos(theta);
  Float stheta = sin(theta);
  tMat.m[0][0] = ctheta;
  tMat.m[0][1] = -stheta;
  tMat.m[1][0] = stheta;
  tMat.m[1][1] = ctheta;

  return Transform(tMat, tMat.Transpose());
}

inline Transform LookAt(const geometry::Vec3f &camera,
                        const geometry::Vec3f &object,
                        const geometry::Vec3f &up) {
  using namespace geometry;
  Vec3f normUp = Vec3f::Normalize(up);
  Vec3f z = Vec3f::Normalize((camera - object));
  Vec3f x = z ^ normUp;
  Vec3f y = x ^ z;

  Matrix4f camToWorld(x.x, y.x, z.x, camera.x, x.y, y.y, z.y, camera.y, x.z,
                      y.z, z.z, camera.z, 0, 0, 0, 1);

  return Transform(camToWorld.Inverse(), camToWorld);
}

inline Transform Rotate(Float angle, const geometry::Vec3f &axis) {
  using namespace geometry;
  Vec3f a = Vec3f::Normalize(axis);
  Float s = Float(sin(angle));
  Float c = Float(cos(angle));
  Float m[4][4];

  m[0][0] = a.x * a.x + (1.f - a.x * a.x) * c;
  m[0][1] = a.x * a.y * (1.f - c) - a.z * s;
  m[0][2] = a.x * a.z * (1.f - c) + a.y * s;
  m[0][3] = 0;

  m[1][0] = a.x * a.y * (1.f - c) + a.z * s;
  m[1][1] = a.y * a.y + (1.f - a.y * a.y) * c;
  m[1][2] = a.y * a.z * (1.f - c) - a.x * s;
  m[1][3] = 0;

  m[2][0] = a.x * a.z * (1.f - c) - a.y * s;
  m[2][1] = a.y * a.z * (1.f - c) + a.x * s;
  m[2][2] = a.z * a.z + (1.f - a.z * a.z) * c;
  m[2][3] = 0;

  m[3][0] = 0;
  m[3][1] = 0;
  m[3][2] = 0;
  m[3][3] = 1;

  Matrix4f tMat(m);
  return Transform(tMat, tMat.Transpose());
}

} // namespace rengen::ops

#endif