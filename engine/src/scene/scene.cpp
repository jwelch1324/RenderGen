#include "scene/scene.h"
#include "geometry/interaction.h"
#include "geometry/normal.h"
#include "geometry/point.h"
#include "geometry/ray.h"
#include "geometry/shapes/plane.h"
#include "geometry/shapes/sphere.h"
#include "geometry/vector.h"
#include "ops/transform.h"
#include "scene/camera.h"
#include "scene/light.h"
#include "scene/lights/pointlight.h"
#include "system/transformfactory.h"
#include <cstdlib>
#include <iostream>
#include <memory>
namespace rengen::scene {
Scene::Scene() {
  ops::Transform *fwd, *rev;
  auto &tfactory = system::TransformFactory::get_instance();
  tfactory.ParseFromString("T 0 -10 -1", &fwd, &rev);

  m_camera.SetCameraToWorld(fwd);
  m_camera.SetLookAt(geometry::Point3f(0, 0, 0));
  m_camera.SetUp(geometry::Vec3f(0, 0, 1));
  m_camera.SetHorzSize(0.25);
  m_camera.SetAspect(16.0 / 9.0);
  m_camera.UpdateCameraGeometry();

  tfactory.ParseFromString("U -1.5 0.0 0.0 0.5 0.5 0.75", &fwd, &rev);

  // Construct a test sphere
  m_shapes.push_back(std::make_shared<geometry::shapes::Sphere>(
      geometry::shapes::Sphere(1.0, fwd, rev)));

  // fwd = rev = nullptr;

  tfactory.ParseFromString("S 0.75 .5 .5", &fwd, &rev);
  m_shapes.push_back(std::make_shared<geometry::shapes::Sphere>(
      geometry::shapes::Sphere(1.0, fwd, rev)));

  // fwd = rev = nullptr;

  tfactory.ParseFromString("U 1.5 0.0 0.0 0.75 .75 0.75", &fwd, &rev);
  m_shapes.push_back(std::make_shared<geometry::shapes::Sphere>(
      geometry::shapes::Sphere(1.0, fwd, rev)));

  // m_shapes.at(0)->SetBaseColor(geometry::Point3f(0.25, .5, .8));
  // m_shapes.at(1)->SetBaseColor(geometry::Point3f(1, .5, 0.0));
  // m_shapes.at(2)->SetBaseColor(geometry::Point3f(1, .8, 0));
  m_shapes.at(0)->SetBaseColor(geometry::Point3f(.5, .5, .5));
  m_shapes.at(1)->SetBaseColor(geometry::Point3f(.5, .5, .5));
  m_shapes.at(2)->SetBaseColor(geometry::Point3f(.5, .5, .5));

  tfactory.ParseFromString("U 0 0 .75 4 4 1", &fwd, &rev);
  m_shapes.push_back(std::make_shared<geometry::shapes::Plane>(fwd, rev));
  m_shapes.at(3)->SetBaseColor(geometry::Point3f(.5, .5, .5));

  // Construct a test light
  m_lights.push_back(
      std::make_shared<lights::PointLight>(lights::PointLight()));
  m_lights.at(0)->m_position = geometry::Point3f(5.0, -10.0, -5.0);
  m_lights.at(0)->m_color = geometry::Point3f(0, 0, 1);
  m_lights.at(0)->m_intensity = 1;

  m_lights.push_back(
      std::make_shared<lights::PointLight>(lights::PointLight()));
  m_lights.at(1)->m_position = geometry::Point3f(-5, -10, -5);
  m_lights.at(1)->m_color = geometry::Point3f(1.0, 0, 0);

  m_lights.push_back(
      std::make_shared<lights::PointLight>(lights::PointLight()));
  m_lights.at(2)->m_position = geometry::Point3f(0, -10, -5);
  m_lights.at(2)->m_color = geometry::Point3f(0, 1, 0);
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
  int ns = 2;

  for (int x = 0; x < xSize; x++)
    for (int y = 0; y < ySize; y++) {
      Float rtotal = 0.0;
      Float gtotal = 0.0;
      Float btotal = 0.0;

      // Perform anti-aliasing with ns rays.
      for (int nn = 0; nn < ns; nn++) {

        // Normalize the x and y coordinates
        Float normX = (Float(x + drand48()) * xFact) - 1.0;
        Float normY = (Float(y + drand48()) * yFact) - 1.0;

        // Generate a ray for this pixel
        m_camera.GenerateRay(normX, normY, &cameraRay);

        std::shared_ptr<geometry::Shape> closestObject;
        geometry::Point3f closestIntPoint;
        geometry::Normal3f closestLocalNormal;
        geometry::Point3f closestLocalColor;
        Float minDist = 1e6;
        bool intersectionFound = false;

        // Test for Intersections with all objects in the scene.
        bool validInt = false;
        for (auto currentObject : m_shapes) {
          validInt = currentObject->Intersect(cameraRay, &tHit, &interStruct);
          if (validInt) {
            // Set the flag to indicate that we found an intersection
            intersectionFound = true;

            // Compute the distance between the camera and point of intersection
            Float dist = (interStruct.m_intersectionPoint - cameraRay.o).norm();

            // If this object is closer to the camera than any other that we
            // have seen before then store a ref to it
            if (dist < minDist) {
              minDist = dist;
              closestObject = currentObject;
              closestIntPoint = interStruct.m_intersectionPoint;
              closestLocalNormal = interStruct.m_n;
              closestLocalColor = interStruct.m_color;
            }
          }
        }

        // Compute illumination for closest object, assuming that there was one
        // found
        if (intersectionFound) {
          // Compute intensity
          Float intensity = 0.0;
          geometry::Point3f color;
          Float red = 0.0;
          Float green = 0.0;
          Float blue = 0.0;
          bool validIllum = false;
          bool illumnFound = false;
          for (auto currentLight : m_lights) {
            validIllum = currentLight->ComputeIllumination(
                closestIntPoint, closestLocalNormal, m_shapes, closestObject,
                color, intensity);

            if (validIllum) {
              illumnFound = true;
              red += color.x * intensity;
              green += color.y * intensity;
              blue += color.z * intensity;
            }
          }

          if (illumnFound) {
            red *= closestLocalColor.x;
            green *= closestLocalColor.y;
            blue *= closestLocalColor.z;

            rtotal += red;
            gtotal += green;
            btotal += blue;
          }
        }
      }
      outputImage.SetPixel(x, y, rtotal / ns, gtotal / ns, btotal / ns);
    }
  return true;
}
} // namespace rengen::scene