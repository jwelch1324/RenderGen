#ifndef DIFFERENTIAL_H_
#define DIFFERENTIAL_H_

#include "geometry/normal.h"
#include "geometry/point.h"
#include "geometry/shape.h"
#include "geometry/vector.h"

namespace rengen::geometry {

class DifferentialGeometry {
public:
  DifferentialGeometry() {
    m_u = m_v = 0.;
    m_shape = nullptr;
  }

  DifferentialGeometry(const Point3f &P, const Vec3f &DPDU, const Vec3f &DPDV,
                       const Vec3f &DNDU, const Vec3f &DNDV, Float uu, Float vv,
                       const Shape *SHAPE)
      : p(P), dpdu(DPDU), dpdv(DPDV), dndu(DNDU), dndv(DNDV) {
        m_nn = Normal3f(Normal3f::Normalize(Normal3f(dpdu^dpdv)));
        m_u = uu;
        m_v = vv;
        m_shape = SHAPE;

        //TODO: Check handedness of the shape transform along with its specification iof reverse orientation
        // if(shape->reverseOrientation ^ shape->transformSwapsHandedness)
        // m_nn *= -1.f;
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