#ifndef TRANSFORMFACTORY_H_
#define TRANSFORMFACTORY_H_

#include "geometry/vector.h"
#include "ops/transform.h"
#include "system/rengen_memory.h"
#include "system/singleton.h"

namespace rengen::system {

class TransformFactory : public Singleton<TransformFactory> {
public:
  TransformFactory();
  ~TransformFactory();

  void FreeAll();

  ops::Transform *CreateTranslationTransform(const geometry::Vec3f &delta);
  void CreateTranslationTransform(const geometry::Vec3f &delta,
                                  ops::Transform **forward,
                                  ops::Transform **inverse);

  ops::Transform *CreateScalingTransform(const geometry::Vec3f &scale);
  ops::Transform *CreateRotationTransform(const geometry::Vec3f &angles);

  void CreateTranslateScaleTransform(const geometry::Vec3f &delta,
                                     const geometry::Vec3f &scale,
                                     ops::Transform **forward,
                                     ops::Transform **inverse);

private:
  std::vector<ops::Transform *> m_transforms;
  ObjectArena<ops::Transform> m_arena;
};

} // namespace rengen::system

#endif