#include "core_math.h"
#include "geometry/geometry.h"
#include "geometry/point.h"
#include "geometry/ray.h"
#include "geometry/vector.h"
#include "io/ppmimage.h"
#include "io/tgaimage.h"
#include <cstdlib>
#include <gtest/gtest.h>

using namespace rengen;

io::RGBAColor TestColor(const geometry::Ray &r) {
  geometry::Vec3f rn = geometry::Vec3f::Normalize(r.d);
  // Interpolate along the y axis
  float t = 0.5 * (rn.y + 1.0);
  geometry::Point3f pc = geometry::ops::PLerp(
      t, geometry::Point3f(1.0, 1.0, 1.0), geometry::Point3f(0.5, 0.7, 1.0));
  io::RGBAColor c(pc.x * 255.99, pc.y * 255.99, pc.z * 255.99);
  return c;
}

Float hitSphere(const geometry::Point3f center, const Float radius,
                const geometry::Ray &r) {
  // This function works by determining if there is any value t for which ray
  // intersects the surface of the sphere of radius r and centered on point c
  geometry::Vec3f oc = r.o - center;
  Float a = r.d * r.d;
  Float b = 2.0f * oc * r.d;
  Float c = oc * oc - radius * radius;
  Float discriminant = b * b - 4 * a * c;
  if (discriminant < 0)
    return -1.0f;
  else {
    return (-b - std::sqrt(discriminant)) / (2.0 * a);
  }
}
const geometry::Vec3f lightVec(0,0,-1);

io::RGBAColor SphereColor(const geometry::Ray &r) {
  geometry::Point3f sphereCenter(0, 0, -1);
  Float t = hitSphere(sphereCenter, 0.5, r);
  if (t > 0) {
    geometry::Vec3f nv = geometry::Vec3f::Normalize(r(t) - sphereCenter);
    return io::RGBAColor(0.5 * (nv.x + 1) * 255.99, 0.5 * (nv.y + 1) * 255.99,
                         0.5 * (nv.z + 1) * 255.99);
  }

  return TestColor(r);
}

TEST(SimpleRayTraceTest, BasicAssertions) {
  // Define the viewing volume
  // Bottom left corner of the image plane
  geometry::Point3f lower_left_corner(-2.0, -1.0, -1.0);
  // Displacement from bottom left to bottom right of the image plane
  geometry::Vec3f horizontal(4, 0, 0);
  // Displacement from bottom left to top left of the image plane
  geometry::Vec3f vertical(0, 2.0, 0);
  geometry::Point3f origin(0, 0, 0); // This is where the camera sits...

  int nx = 200;
  int ny = 100;
  auto canvas = io::PPMImage(nx, ny);

  // Now we scan each pixel and draw a ray from the center of the camera (eye)
  // to that pixel
  for (int i = 0; i < nx; i++)
    for (int j = 0; j < ny; j++) {
      // Calculate UV coords on image plane
      float u = float(i) / float(nx);
      float v = float(j) / float(ny);
      // Cast the ray
      geometry::Vec3f f;
      auto ray =
          geometry::Ray(origin, geometry::Vec3f(lower_left_corner +
                                                u * horizontal + v * vertical));
      canvas.Set(i, j, TestColor(ray));
    }

  canvas.Write("raytest.ppm");
}

TEST(SimpleSphereRayTest, BasicAssertions) {
  // Define the viewing volume
  // Bottom left corner of the image plane
  geometry::Point3f lower_left_corner(-2, -1, -1);
  // Displacement from bottom left to bottom right of the image plane
  geometry::Vec3f horizontal(4, 0, 0);
  // Displacement from bottom left to top left of the image plane
  geometry::Vec3f vertical(0, 2.0, 0);
  geometry::Point3f origin(0, 0, 0); // This is where the camera sits...

  int nx = 2000;
  int ny = 1000;
  int ns = 100;
  auto canvas = io::PPMImage(nx, ny);

  // Now we scan each pixel and draw a ray from the center of the camera (eye)
  // to that pixel
  for (int i = 0; i < nx; i++)
    for (int j = 0; j < ny; j++) {
      // Calculate UV coords on image plane
      geometry::Point3f fcolor(0, 0, 0);
      for (int k = 0; k < ns; k++) {
        float u = float(i + drand48()) / float(nx);
        float v = float(j + drand48()) / float(ny);
        // Cast the ray
        geometry::Vec3f f;
        auto ray = geometry::Ray(
            origin,
            geometry::Vec3f(lower_left_corner + u * horizontal + v * vertical));
        io::RGBAColor color = SphereColor(ray);
        fcolor += geometry::Vec3f(color.r(), color.g(), color.b());
      }
      fcolor /= ns;
      canvas.Set(i, j, io::RGBAColor(fcolor.x, fcolor.y, fcolor.z));
    }

  canvas.Write("sphereRayTest.ppm");
}