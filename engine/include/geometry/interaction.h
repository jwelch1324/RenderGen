#ifndef DIFFERENTIAL_H_
#define DIFFERENTIAL_H_

#include "common.h"
#include "geometry/normal.h"
#include "geometry/point.h"
#include "geometry/vector.h"
#include "media/medium.h"

namespace rengen::geometry {
class Shape;

class Interaction {
public:
  Interaction() : m_time(0) {}
  Interaction(const Point3f &p, const Normal3f &n, const Vec3f &pError,
              const Vec3f &wo, Float time,
              const medium::MediumInterface &mediumInterface)
      : m_intersectionPoint(p), m_time(time), m_pError(pError), m_wo(wo),
        m_n(n), m_mediumInterface(mediumInterface) {}

  bool IsSurfaceInteraction() const { return m_n != Normal3f(); }

  Interaction(const Point3f &p, const Vec3f &wo, Float time,
              const medium::MediumInterface &mediumInterface)
      : m_intersectionPoint(p), m_time(time), m_wo(wo),
        m_mediumInterface(mediumInterface) {}

  Interaction(const Point3f &p, Float time,
              const medium::MediumInterface &mediumInterface)
      : m_intersectionPoint(p), m_time(time),
        m_mediumInterface(mediumInterface) {}

  bool IsMediumInteraction() const { return !IsSurfaceInteraction(); }

  Point3f m_intersectionPoint;
  Float m_time;
  Vec3f m_pError;
  Vec3f m_wo;
  Normal3f m_n;
  Point3f m_color;
  rengen::medium::MediumInterface m_mediumInterface;
};

// class SurfaceInteraction : public Interaction {

// };

class DiffGeo {
public:
  DiffGeo() {
    m_u = m_v = 0.;
    m_shape = nullptr;
  }

  DiffGeo(const Point3f &P, const Vec3f &DPDU, const Vec3f &DPDV,
          const Vec3f &DNDU, const Vec3f &DNDV, Float uu, Float vv,
          const Shape *SHAPE)
      : p(P), dpdu(DPDU), dpdv(DPDV), dndu(DNDU), dndv(DNDV) {
    m_nn = Normal3f(Normal3f::Normalize(Normal3f(dpdu ^ dpdv)));
    m_u = uu;
    m_v = vv;
    m_shape = SHAPE;

    // TODO: Check handedness of the shape transform along with its
    // specification iof reverse orientation
    //  if(shape->reverseOrientation ^ shape->transformSwapsHandedness)
    //  m_nn *= -1.f;
  }

  Point3f p;
  Normal3f m_nn;
  Vec3f dpdu, dpdv;
  Vec3f dndu, dndv;
  Float m_u, m_v;

  const Shape *m_shape;
};

} // namespace rengen::geometry

#endif