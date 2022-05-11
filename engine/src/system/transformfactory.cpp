#include "system/transformfactory.h"
#include "common.h"
#include "geometry/vector.h"
#include "ops/transform.h"
#include <memory>
namespace rengen::system {
TransformFactory::TransformFactory() {}

TransformFactory::~TransformFactory() { m_arena.FreeAll(); }

void TransformFactory::FreeAll() {
  for (auto it : m_transforms) {
    if (it) {
      delete (it);
      it = nullptr;
    }
  }
  m_transforms.clear();
}

ops::Transform *
TransformFactory::CreateTranslationTransform(const geometry::Vec3f &delta) {
  ops::Transform *t = m_arena.Alloc();
  t->SetTransform(ops::Translate(delta));
  return t;
}

void TransformFactory::CreateTranslationTransform(const geometry::Vec3f &delta,
                                                  ops::Transform **forward,
                                                  ops::Transform **inverse) {
  *forward = m_arena.Alloc();
  *inverse = m_arena.Alloc();

  auto t = ops::Translate(delta);
  (*forward)->SetTransform(t);
  (*inverse)->SetTransform(t.GetInverse());
}

void TransformFactory::CreateTranslateScaleTransform(
    const geometry::Vec3f &delta, const geometry::Vec3f &scale,
    ops::Transform **forward, ops::Transform **inverse) {

  *forward = m_arena.Alloc();
  *inverse = m_arena.Alloc();

  auto t = ops::Translate(delta) * ops::Scale(scale);
  (*forward)->SetTransform(t);
  (*inverse)->SetTransform(t.GetInverse());
}

} // namespace rengen::system