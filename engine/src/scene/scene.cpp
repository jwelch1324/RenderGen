#include "scene/scene.h"
#include "geometry/interaction.h"
#include "geometry/point.h"
#include "geometry/ray.h"
#include "geometry/shapes/sphere.h"
#include "geometry/vector.h"
#include "ops/transform.h"
#include "scene/light.h"
#include "scene/lights/pointlight.h"
#include "system/transformfactory.h"
#include <iostream>
#include <memory>
namespace rengen::scene {
Scene::Scene() {
  m_camera.SetCameraToWorld(
      system::TransformFactory::get_instance().CreateTranslationTransform(
          geometry::Vec3f(0, -10, 0)));
  m_camera.SetLookAt(geometry::Vec3f(0, 0, 0));
  m_camera.SetUp(geometry::Vec3f(0, 0, 1));
  m_camera.SetHorzSize(0.25);
  m_camera.SetAspect(16.0 / 9.0);
  m_camera.UpdateCameraGeometry();
  ops::Transform *fwd, *rev;

  auto &tfactory = system::TransformFactory::get_instance();
  tfactory.CreateTranslationTransform(geometry::Vec3f(0, 0, 0), &fwd, &rev);

  // Construct a test sphere
  m_shapes.push_back(std::make_shared<geometry::shapes::Sphere>(
      geometry::shapes::Sphere(1.0, fwd, rev)));

  fwd = rev = nullptr;

  tfactory.CreateTranslateScaleTransform(
      geometry::Vec3f(-1.0, 0, 0), geometry::Vec3f(0.5, 1, 1), &fwd, &rev);
  m_shapes.push_back(std::make_shared<geometry::shapes::Sphere>(
      geometry::shapes::Sphere(1.0, fwd, rev)));
  m_shapes.at(1)->SetBaseColor(geometry::Point3f(255, 255, 125));
  //   m_shapes.push_back(
  //       std::make_shared<geometry::shapes::Sphere>(geometry::shapes::Sphere()));

  // Construct a test light
  m_lights.push_back(
      std::make_shared<lights::PointLight>(lights::PointLight()));
  m_lights.at(0)->m_position = geometry::Point3f(5, -10.0, -5.0);
  m_lights.at(0)->m_color = geometry::Point3f(255, 255, 255);
  m_lights.at(0)->m_intensity = 1;
}

// Function to do actual rendering
bool Scene::Render(draw::SDLImageCanvas &outputImage) {
  // Get dimensions of output image
  int xSize = outputImage.GetXSize();
  int ySize = outputImage.GetYSize();

  // Loop over each pixel in our render plane
  geometry::Ray cameraRay;
  cameraRay.mint = 1e-6;
  cameraRay.maxt = 0.0;
  geometry::Interaction interStruct;

  Float xFact = 1.0 / (double(xSize) / 2.0);
  Float yFact = 1.0 / (double(ySize) / 2.0);
  Float tHit = 0.0;
  Float minDist = 1e6;
  Float maxDist = 0.0;

  for (int x = 0; x < xSize; x++)
    for (int y = 0; y < ySize; y++) {
      // Normalze the x and y coordinates
      Float normX = (Float(x) * xFact) - 1.0;
      Float normY = (Float(y) * yFact) - 1.0;

      // Generate a ray for this pixel
      m_camera.GenerateRay(normX, normY, &cameraRay);

      // Test for Intersections with all objects in the scene.
      bool validInt = false;
      for (auto &it : m_shapes) {
        validInt = it->Intersect(cameraRay, &tHit, &interStruct);
        if (validInt) {

          // Compute the intensity of the illumination
          Float intensity;
          geometry::Point3f color;
          bool validIllum = false;

          for (auto currentLight : m_lights) {
            validIllum = currentLight->ComputeIllumination(
                interStruct.m_intersectionPoint, interStruct.m_n, m_shapes, it,
                color, intensity);
          }

          Float dist = (interStruct.m_intersectionPoint - cameraRay.o).norm();
          if (dist > maxDist)
            maxDist = dist;
          if (dist < minDist)
            minDist = dist;

          if (validIllum) {
            // outputImage.SetPixel(x, y, 255.0 * intensity, 0, 0);
            auto baseColor = it->GetBaseColor();
            outputImage.SetPixel(x, y, intensity * baseColor.x,
                                 intensity * baseColor.y,
                                 intensity * baseColor.z);
          } else {
            // outputImage.SetPixel(x, y, 0, 0, 0);
          }
        }
      }

      if (!validInt) {
        // outputImage.SetPixel(x, y, 255, 255, 255);
      }
    }
  std::cout << "Min Dist: " << minDist << std::endl;
  std::cout << "Max Dist: " << maxDist << std::endl;

  return true;
}
} // namespace rengen::scene