#include "ops/transform.h"
#include "common.h"
#include "geometry/bbox.h"
#include "geometry/matrix.h"
#include "geometry/point.h"
#include "geometry/vector.h"

namespace rengen::ops {
using namespace geometry;

Transform &IdentityTransform() {
  static Transform IDTRANSFORM = Transform(Matrix4f());
  return IDTRANSFORM;
}

Transform *IdentityTransformPtr() { return &(IdentityTransform()); }

Point3f Transform::operator()(const Point3f &pt) const {
  Float x = pt.x;
  Float y = pt.y;
  Float z = pt.z;
  Float xp =
      m_mat.m[0][0] * x + m_mat.m[0][1] * y + m_mat.m[0][2] * z + m_mat.m[0][3];
  Float yp =
      m_mat.m[1][0] * x + m_mat.m[1][1] * y + m_mat.m[1][2] * z + m_mat.m[1][3];
  Float zp =
      m_mat.m[2][0] * x + m_mat.m[2][1] * y + m_mat.m[2][2] * z + m_mat.m[2][3];
  Float wp =
      m_mat.m[3][0] * x + m_mat.m[3][1] * y + m_mat.m[3][2] * z + m_mat.m[3][3];

  assert(wp != 0);
  if (wp == 1.)
    return Point3f(xp, yp, zp);
  else
    return Point3f(xp, yp, zp) / wp;
}

void Transform::operator()(const Point3f &pt, Point3f *ptrans) const {
  Float x = pt.x;
  Float y = pt.y;
  Float z = pt.z;
  ptrans->x =
      m_mat.m[0][0] * x + m_mat.m[0][1] * y + m_mat.m[0][2] * z + m_mat.m[0][3];
  ptrans->y =
      m_mat.m[1][0] * x + m_mat.m[1][1] * y + m_mat.m[1][2] * z + m_mat.m[1][3];
  ptrans->z =
      m_mat.m[2][0] * x + m_mat.m[2][1] * y + m_mat.m[2][2] * z + m_mat.m[2][3];
  Float wp =
      m_mat.m[3][0] * x + m_mat.m[3][1] * y + m_mat.m[3][2] * z + m_mat.m[3][3];

  assert(wp != 0);
  if (wp != 1.)
    (*ptrans) /= wp;
}

Vec3f Transform::operator()(const Vec3f &vec) const {
  Float x = vec.x;
  Float y = vec.y;
  Float z = vec.z;
  Float xp = m_mat.m[0][0] * x + m_mat.m[0][1] * y + m_mat.m[0][2] * z;
  Float yp = m_mat.m[1][0] * x + m_mat.m[1][1] * y + m_mat.m[1][2] * z;
  Float zp = m_mat.m[2][0] * x + m_mat.m[2][1] * y + m_mat.m[2][2] * z;

  return Vec3f(xp, yp, zp);
}

void Transform::operator()(const Vec3f &vec, Vec3f *vtrans) const {
  Float x = vec.x;
  Float y = vec.y;
  Float z = vec.z;
  vtrans->x = m_mat.m[0][0] * x + m_mat.m[0][1] * y + m_mat.m[0][2] * z;
  vtrans->y = m_mat.m[1][0] * x + m_mat.m[1][1] * y + m_mat.m[1][2] * z;
  vtrans->z = m_mat.m[2][0] * x + m_mat.m[2][1] * y + m_mat.m[2][2] * z;
}

Normal3f Transform::operator()(const Normal3f &n) const {
  Float x = n.x;
  Float y = n.y;
  Float z = n.z;

  Float xp = m_matInv.m[0][0] * x + m_matInv.m[1][0] * y + m_matInv.m[2][0] * z;
  Float yp = m_matInv.m[0][1] * x + m_matInv.m[1][1] * y + m_matInv.m[2][1] * z;
  Float zp = m_matInv.m[0][2] * x + m_matInv.m[1][2] * y + m_matInv.m[2][2] * z;

  return Normal3f(xp, yp, zp);
}

void Transform::operator()(const Normal3f &n, Normal3f *ntrans) const {
  Float x = n.x;
  Float y = n.y;
  Float z = n.z;

  ntrans->x =
      m_matInv.m[0][0] * x + m_matInv.m[1][0] * y + m_matInv.m[2][0] * z;
  ntrans->y =
      m_matInv.m[0][1] * x + m_matInv.m[1][1] * y + m_matInv.m[2][1] * z;
  ntrans->z =
      m_matInv.m[0][2] * x + m_matInv.m[1][2] * y + m_matInv.m[2][2] * z;
}

Ray Transform::operator()(const Ray &r) const {
  Ray ret;
  (*this)(r.o, &ret.o);
  (*this)(r.d, &ret.d);
  ret.mint = r.mint;
  ret.maxt = r.maxt;
  ret.time = r.time;
  return ret;
}

void Transform::operator()(const Ray &r, Ray *rtrans) const {
  Point3f o = r.o;
  Vec3f d = r.d;
  (*this)(o, &rtrans->o);
  (*this)(d, &rtrans->d);
  rtrans->mint = r.mint;
  rtrans->maxt = r.maxt;
  rtrans->time = r.time;
}

BBox Transform::SlowBBoxTransform(const BBox &b) const {
  const Transform &M = (*this);
  BBox ret(M(Point3f(b.pMin.x, b.pMin.y, b.pMin.z)));
  ret = ret.Union(M(Point3f(b.pMin.x, b.pMin.y, b.pMax.z)));
  ret = ret.Union(M(Point3f(b.pMax.x, b.pMin.y, b.pMin.z)));
  ret = ret.Union(M(Point3f(b.pMin.x, b.pMax.y, b.pMin.z)));

  ret = ret.Union(M(Point3f(b.pMax.x, b.pMax.y, b.pMin.z)));
  ret = ret.Union(M(Point3f(b.pMax.x, b.pMin.y, b.pMax.z)));
  ret = ret.Union(M(Point3f(b.pMin.x, b.pMax.y, b.pMax.z)));

  ret = ret.Union(M(Point3f(b.pMax.x, b.pMax.y, b.pMax.z)));

  return ret;
}

BBox Transform::FastBBoxTransform(const geometry::BBox &b) const {
  BBox ret;

  Vec3f ex(b.pMax.x - b.pMin.x, 0, 0);
  Vec3f ey(0, b.pMax.y - b.pMin.y, 0);
  Vec3f ez(0, 0, b.pMax.z - b.pMin.z);

  // new displacements from the pmin
  auto pez = (*this)(ez);
  auto pey = (*this)(ey);
  auto pex = (*this)(ex);

  // new placement of pmin
  auto newPmin = (*this)(b.pMin);

  // reform pmax
  auto newPmax = newPmin + pez + pey + pex;

  return BBox(newPmin, newPmax);
}

Transform Transform::operator*(const Transform &t2) const {
  return Transform(Matrix4f::MatMul(this->m_mat, t2.m_mat),
                   Matrix4f::MatMul(t2.m_matInv, this->m_matInv));
}

} // namespace rengen::ops