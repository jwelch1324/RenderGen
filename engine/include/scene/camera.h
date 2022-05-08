#ifndef CAMERA_H_
#define CAMERA_H_

#include "common.h"
#include "geometry/point.h"
#include "geometry/ray.h"
#include "geometry/vector.h"
#include "ops/transform.h"
namespace rengen::scene {
class Camera {
public:
  Camera();

  // Functions to set camera parameters
  void SetPosition(const geometry::Point3f &newPosition);
  void SetLookAt(const geometry::Vec3f &newLookAt);
  void SetUp(const geometry::Vec3f &upVector);
  void SetRenderPlaneDistance(const Float newLength);
  void SetHorzSize(const Float newSize);
  void SetAspect(const Float newAspect);
  void SetCameraToWorld(const ops::Transform &cameraToWorld);

  // Function to Return Camera Parameters
  geometry::Point3f GetPosition() const { return m_cameraPosition; }
  geometry::Vec3f GetLookAt() const { return m_cameraLookAt; }
  geometry::Vec3f GetUp() const { return m_cameraUp; }
  geometry::Vec3f GetU() const { return m_projectionScreenU; }
  geometry::Vec3f GetV() const { return m_projectionScreenV; }
  geometry::Point3f GetScreenCenter() const { return m_projectionScreenCenter; }
  ops::Transform &GetCameraToWorld() { return m_cameraToWorld; }
  ops::Transform &GetWorldToCamera() { return m_worldToCamera; }

  Float GetLength() const { return m_cameraLength; }
  Float GetHorzSize() const { return m_cameraHorizontalSize; }
  Float GetAspect() const { return m_cameraAspectRatio; }

  // Function to generate a ray
  geometry::Ray GenerateRay(Float proScreenX, Float proScreenY);

  // Function to update the camera geometry
  void UpdateCameraGeometry();

private:
  geometry::Point3f m_cameraPosition;
  geometry::Vec3f m_cameraLookAt;
  geometry::Vec3f m_cameraUp;
  Float m_cameraLength;
  Float m_cameraHorizontalSize;
  Float m_cameraAspectRatio;

  geometry::Vec3f m_alignmentVector;
  geometry::Vec3f m_projectionScreenU;
  geometry::Vec3f m_projectionScreenV;
  geometry::Point3f m_projectionScreenCenter;

  ops::Transform m_cameraToWorld;
  ops::Transform m_worldToCamera;
};
} // namespace rengen::scene

#endif