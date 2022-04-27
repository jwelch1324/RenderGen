#include "ops/transform.h"
#include "common.h"
#include "geometry/bbox.h"
#include "geometry/point.h"
#include "geometry/vector.h"

namespace rengen::ops {
using namespace geometry;

Point3f Transform::operator()(const Point3f &pt) const {
  float x = pt.x;
  float y = pt.y;
  float z = pt.z;
  float xp =
      m_mat.m[0][0] * x + m_mat.m[0][1] * y + m_mat.m[0][2] * z + m_mat.m[0][3];
  float yp =
      m_mat.m[1][0] * x + m_mat.m[1][1] * y + m_mat.m[1][2] * z + m_mat.m[1][3];
  float zp =
      m_mat.m[2][0] * x + m_mat.m[2][1] * y + m_mat.m[2][2] * z + m_mat.m[2][3];
  float wp =
      m_mat.m[3][0] * x + m_mat.m[3][1] * y + m_mat.m[3][2] * z + m_mat.m[3][3];

  assert(wp != 0);
  if (wp == 1.)
    return Point3f(xp, yp, zp);
  else
    return Point3f(xp, yp, zp) / wp;
}

void Transform::operator()(const Point3f &pt, Point3f *ptrans) const {
  float x = pt.x;
  float y = pt.y;
  float z = pt.z;
  ptrans->x =
      m_mat.m[0][0] * x + m_mat.m[0][1] * y + m_mat.m[0][2] * z + m_mat.m[0][3];
  ptrans->y =
      m_mat.m[1][0] * x + m_mat.m[1][1] * y + m_mat.m[1][2] * z + m_mat.m[1][3];
  ptrans->z =
      m_mat.m[2][0] * x + m_mat.m[2][1] * y + m_mat.m[2][2] * z + m_mat.m[2][3];
  float wp =
      m_mat.m[3][0] * x + m_mat.m[3][1] * y + m_mat.m[3][2] * z + m_mat.m[3][3];

  assert(wp != 0);
  if (wp != 1.)
    (*ptrans) /= wp;
}

Vec3f Transform::operator()(const Vec3f &vec) const {
  float x = vec.x;
  float y = vec.y;
  float z = vec.z;
  float xp = m_mat.m[0][0] * x + m_mat.m[0][1] * y + m_mat.m[0][2] * z;
  float yp = m_mat.m[1][0] * x + m_mat.m[1][1] * y + m_mat.m[1][2] * z;
  float zp = m_mat.m[2][0] * x + m_mat.m[2][1] * y + m_mat.m[2][2] * z;

  return Vec3f(xp, yp, zp);
}

void Transform::operator()(const Vec3f &vec, Vec3f *vtrans) const {
  float x = vec.x;
  float y = vec.y;
  float z = vec.z;
  vtrans->x = m_mat.m[0][0] * x + m_mat.m[0][1] * y + m_mat.m[0][2] * z;
  vtrans->y = m_mat.m[1][0] * x + m_mat.m[1][1] * y + m_mat.m[1][2] * z;
  vtrans->z = m_mat.m[2][0] * x + m_mat.m[2][1] * y + m_mat.m[2][2] * z;
}

Normal3f Transform::operator()(const Normal3f &n) const {
  float x = n.x;
  float y = n.y;
  float z = n.z;

  float xp = m_matInv.m[0][0] * x + m_matInv.m[1][0] * y + m_matInv.m[2][0] * z;
  float yp = m_matInv.m[0][1] * x + m_matInv.m[1][1] * y + m_matInv.m[2][1] * z;
  float zp = m_matInv.m[0][2] * x + m_matInv.m[1][2] * y + m_matInv.m[2][2] * z;

  return Normal3f(xp, yp, zp);
}

void Transform::operator()(const Normal3f &n, Normal3f *ntrans) const {
  float x = n.x;
  float y = n.y;
  float z = n.z;

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

BBox Transform::SlowTransform(const BBox &b) const {
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

BBox Transform::FastTransform(const geometry::BBox &b) const {
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

} // namespace rengen::ops