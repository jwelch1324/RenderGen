#include "geometry/quaternion.h"
#include "common.h"
#include "geometry/matrix.h"
#include "ops/transform.h"

namespace rengen::geometry {

ops::Transform Quaternion::ToTransform() const {
  Matrix4f m(
      1 - 2 * (qy() * qy() + qz() * qz()), 2 * (qx() * qy() + qz() * qw()),
      2 * (qx() * qz() - qy() * qw()), 0, 2 * (qx() * qy() - qz() * qw()),
      1 - 2 * (qx() * qx() + qz() * qz()), 2 * (qy() * qz() + qx() * qw()), 0,
      2 * (qx() * qz() + qy() * qw()), 2 * (qy() * qz() - qx() * qw()),
      1 - 2 * (qx() * qx() + qy() * qy()), 0, 0, 0, 0, 1);

  return ops::Transform(m);
}

Quaternion Slerp(Float t, const Quaternion &q1, const Quaternion &q2) {
  Float cosTheta = q1.Dot(q2);
  if (cosTheta > 0.9995f) {
    return Quaternion::Normalize(q1 * (1 - t) + q2 * t);
  } else {
    Float theta = std::acos(Clamp(cosTheta, -1, 1));
    Float thetap = theta * t;
    Quaternion qperp = Quaternion::Normalize(q2 - q1 * cosTheta);
    return q1 * std::cos(thetap) + qperp * std::sin(thetap);
  }
}
} // namespace rengen::geometry