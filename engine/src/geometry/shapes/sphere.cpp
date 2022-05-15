#include "geometry/shapes/sphere.h"
#include "geometry/normal.h"
#include "geometry/point.h"
#include "geometry/ray.h"
#include "geometry/vector.h"
#include <cmath>

namespace rengen::geometry::shapes {

bool Sphere::Intersect(const Ray &castRay, Float *tHit, Interaction *isect,
                       bool testAlphaTexture) const {

  Ray ray = (*m_WorldToObject)(castRay);

  Vec3f vhat = Vec3f::Normalize(ray.d);
  Vec3f oc = Vec3f(ray.o);

  Float a = ray.d * ray.d;
  // Calculate b
  Float b = 2.0 * (oc * ray.d);

  // Calculate c
  Float c = oc * oc - 1.0;

  Float disc = b * b - 4 * a * c;

  // If the discriminant is zero then there are no intersections
  if (disc <= 0)
    return false;

  Float sqrtD = std::sqrt(disc);
  Float r1 = (-b + sqrtD) / (2.0 * a);
  Float r2 = (-b - sqrtD) / (2.0 * a);

  if ((r1 < 0.0) || (r2 < 0.0)) {
    // Some part of the object is behind the camera so we ignore it
    return false;
  }

  // Determine which point of intersection was closest to the camera
  if (tHit != nullptr) {
    if (r1 < r2)
      *tHit = r1;
    else
      *tHit = r2;

    if (isect != nullptr) {
      isect->m_intersectionPoint = (*m_ObjectToWorld)(ray(*tHit));
      isect->m_time = *tHit;
      isect->m_n = Normal3f(Vec3f(isect->m_intersectionPoint -
                                  (*m_ObjectToWorld)(Point3f(0, 0, 0))));

      if (isect->m_n == Normal3f(0, 0, 0))
        printf("Hmm...\n");

      isect->m_color = GetBaseColor();
    }
  }
  return true;
}

// bool Sphere::Intersect(const Ray &ray, Float *tHit, Interaction *isect,
//                        bool testAlphaTexuture) const {
//   Vec3f oc = ray.o - m_center;
//   Float a = ray.d * ray.d;
//   Float b = 2.0f * oc * ray.d;
//   Float c = oc * oc - m_radius * m_radius;
//   Float discriminant = b * b - 4 * a * c;
//   if (discriminant < 0)
//     return false;
//   else {
//     Float t1 = (-b - std::sqrt(discriminant)) / (2.0 * a);
//     Float t2 = (-b + std::sqrt(discriminant)) / (2.0 * a);
//     if ((t1 < 0.0) || (t2 < 0.0)) {
//       return false;
//     } else {

//       // Find which solution is closet to the camera
//       if (t1 < t2) {
//         *tHit = t1;
//       } else {
//         *tHit = t2;
//       }
//       isect->m_intersectionPoint = ray(*tHit);
//       isect->m_time = *tHit;
//       isect->m_n = Normal(isect->m_intersectionPoint - m_center);
//       return true;
//     }
//   }
//}

} // namespace rengen::geometry::shapes