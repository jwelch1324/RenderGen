#include "scene/camera.h"
#include "geometry/point.h"
#include "geometry/ray.h"
#include "geometry/vector.h"
#include "ops/transform.h"
#include <cmath>
#include <math.h>

namespace rengen::scene {
Camera::Camera() {
  m_cameraPosition = geometry::Point3f(0, 0, 0);
  m_cameraLookAt = geometry::Vec3f(0.0, 0.0, 0.0);
  m_cameraUp = geometry::Vec3f(0, 0, 1);
  m_cameraLength = 1.0;
  m_cameraHorizontalSize = 1.0;
  m_cameraAspectRatio = 1.0;
}

void Camera::SetPosition(const geometry::Point3f &newPosition) {
  m_cameraPosition = newPosition;
}

void Camera::SetLookAt(const geometry::Vec3f &newLookAt) {
  m_cameraLookAt = newLookAt;
}

void Camera::SetUp(const geometry::Vec3f &upVector) { m_cameraUp = upVector; }

void Camera::SetRenderPlaneDistance(const Float newLength) {
  m_cameraLength = newLength;
}

void Camera::SetHorzSize(const Float newSize) {
  m_cameraHorizontalSize = newSize;
}

void Camera::SetAspect(const Float newAspect) {
  m_cameraAspectRatio = newAspect;
}

void Camera::SetCameraToWorld(ops::Transform *cameraToWorld) {
  m_cameraToWorld = cameraToWorld;
}

void Camera::UpdateCameraGeometry() {

  m_cameraPosition = (*m_cameraToWorld)(geometry::Point3f(0, 0, 0));

  m_alignmentVector = geometry::Vec3f::Normalize(
      (m_cameraLookAt - geometry::Vec3f(m_cameraPosition)));

  // Compute the U and V vectors
  m_projectionScreenU =
      geometry::Vec3f::Normalize(m_alignmentVector ^ m_cameraUp);

  m_projectionScreenV =
      geometry::Vec3f::Normalize(m_projectionScreenU ^ m_alignmentVector);

  // Compute the position of the center point of the screen
  m_projectionScreenCenter =
      m_cameraPosition + (m_cameraLength * m_alignmentVector);

  // Modify the U and V vectors to match the size and aspect ratio
  m_projectionScreenU = m_projectionScreenU * m_cameraHorizontalSize;
  m_projectionScreenV =
      m_projectionScreenV * (m_cameraHorizontalSize / m_cameraAspectRatio);
}

bool Camera::GenerateRay(Float proScreenU, Float proScreenV,
                         geometry::Ray *outRay) {
  if (std::abs(proScreenU) > 1)
    proScreenU = proScreenU / std::abs(proScreenU);

  if (std::abs(proScreenV) > 1)
    proScreenV = proScreenV / std::abs(proScreenV);

  // Compute the location of the screen point in world coordinates
  geometry::Point3f screenWorldPart1 =
      m_projectionScreenCenter + (m_projectionScreenU * proScreenU);

  geometry::Point3f screenWorldCoordinate =
      screenWorldPart1 + (m_projectionScreenV * proScreenV);

  outRay->o = m_cameraPosition;
  outRay->d = screenWorldCoordinate - m_cameraPosition;

  return true;
}

} // namespace rengen::scene