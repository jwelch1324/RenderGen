
#include "geometry/shapes/plane.h"
#include "common.h"
#include "geometry/interaction.h"
#include "geometry/normal.h"
#include "geometry/point.h"
#include "geometry/vector.h"
#include <cstddef>
namespace rengen::geometry::shapes {

Plane::Plane()
    : Shape(rengen::ops::IdentityTransformPtr(),
            rengen::ops::IdentityTransformPtr(), false) {}

Plane::~Plane() {}

bool Plane::Intersect(const Ray &ray, Float *tHit, Interaction *isect,
                      bool testAlphaTexuture) const {
  // Move the ray into the local frame
  Ray localRay = (*m_WorldToObject)(ray);

  Point3f &a = localRay.o;                // a start point
  Vec3f k = Vec3f::Normalize(localRay.d); // k vector

  // Check if there is an intersection -- i.e. make sure the castray is not
  // parallel to the plane
  // Check that the z component is not close to zero
  if (EpislonEqual(k.z, Float(0.0), 1e-21)) {
    return false;
  }

  // otherwise there is an intersection
  Float t = -a.z / (k.z);

  // If t < 0 then the intersection is behind the camera
  if (t <= 0)
    return false;

  // compute the values for u and v
  Float u = a.x + k.x * t;
  Float v = a.y + k.y * t;

  // If the magnitude of both u or v is less than or equal to one then we must
  // be in the plane
  if (std::abs(u) > 1.0 || std::abs(v) > 1.0) {
    return false;
  }

  if (tHit != nullptr)
    *tHit = t;
  if (isect != nullptr) {
    isect->m_intersectionPoint = (*m_ObjectToWorld)(localRay.o + t * k);
    isect->m_time = t;
    isect->m_n = Normal3f::Normalize((*m_ObjectToWorld)(Normal3f(0, 0, -1)));
    isect->m_color = GetBaseColor();
  }

  return true;
}

Plane::Plane(ops::Transform *objectToWorld, ops::Transform *worldToObject)
    : Shape(objectToWorld, worldToObject, objectToWorld->SwapsHandedness()) {}

} // namespace rengen::geometry::shapes