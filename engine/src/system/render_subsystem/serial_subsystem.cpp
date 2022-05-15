#include "system/render_subsystem/serial_subsystem.h"
#include "geometry/interaction.h"
#include "geometry/point.h"

namespace rengen::system::render {
geometry::RGBColor SerialRenderer::RenderRay(geometry::Ray &cameraRay) const {
  std::shared_ptr<geometry::Shape> closestObject;
  geometry::Point3f closestIntPoint;
  geometry::Normal3f closestLocalNormal;
  geometry::Point3f closestLocalColor;
  Float minDist = 1e6;
  bool intersectionFound = false;

  geometry::Interaction interStruct;
  Float tHit;

  Float rtotal = 0.0;
  Float gtotal = 0.0;
  Float btotal = 0.0;

  // Test for Intersections with all objects in the scene.
  bool validInt = false;
  for (auto currentObject : m_sceneReference.m_shapes) {
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
    for (auto currentLight : m_sceneReference.m_lights) {
      validIllum = currentLight->ComputeIllumination(
          closestIntPoint, closestLocalNormal, m_sceneReference.m_shapes,
          closestObject, color, intensity);

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
  return geometry::RGBColor(rtotal, gtotal, btotal);
}
} // namespace rengen::system::render