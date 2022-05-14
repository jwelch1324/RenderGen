#include "system/transformfactory.h"
#include "common.h"
#include "geometry/vector.h"
#include "ops/transform.h"
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <sstream>
#include <string>
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

void TransformFactory::CreateScalingTransform(const geometry::Vec3f &scale,
                                              ops::Transform **forward,
                                              ops::Transform **reverse) {

  *forward = m_arena.Alloc();
  *reverse = m_arena.Alloc();

  auto t = ops::Scale(scale);
  (*forward)->SetTransform(t);
  (*reverse)->SetTransform(t.GetInverse());
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

bool TransformFactory::ParseFromString(std::string transformDef,
                                       ops::Transform **forward,
                                       ops::Transform **reverse) {
  using namespace std;

  auto it = m_storedTransforms.find(transformDef);
  if (it != m_storedTransforms.end()) {
    *forward = it->second.fwd;
    *reverse = it->second.rev;
    return true;
  }

  vector<string> tokens;

  stringstream instream(transformDef);

  string tmp;

  while (getline(instream, tmp, ' ')) {
    tokens.push_back(tmp);
  }

  char typeKey = toupper(tokens.at(0)[0]);
  float p1, p2, p3 = 0;
  TransformPair newPair;

  if (tokens.size() == 4) {
    // could be scaling translate or XYZ rotate
    p1 = std::atof(tokens[1].c_str());
    p2 = std::atof(tokens[2].c_str());
    p3 = std::atof(tokens[3].c_str());

    switch (typeKey) {
    case 'T':
      CreateTranslationTransform(geometry::Vec3f(p1, p2, p3), forward, reverse);
      newPair.fwd = *forward;
      newPair.rev = *reverse;
      m_storedTransforms[transformDef] = newPair;
      return true;
    case 'S':
      CreateScalingTransform(geometry::Vec3f(p1, p2, p3), forward, reverse);
      newPair.fwd = *forward;
      newPair.rev = *reverse;
      return true;
    default:
      printf("unknown type %c\n", typeKey);
      return false;
    }
  } else if (tokens.size() == 7) {
    // Likely a translate + scale transform
    float p4, p5, p6 = 0.0;
    p1 = std::atof(tokens[1].c_str());
    p2 = std::atof(tokens[2].c_str());
    p3 = std::atof(tokens[3].c_str());
    p4 = std::atof(tokens[4].c_str());
    p5 = std::atof(tokens[5].c_str());
    p6 = std::atof(tokens[6].c_str());

    switch (typeKey) {
    case 'U':
      CreateTranslateScaleTransform(geometry::Vec3f(p1, p2, p3),
                                    geometry::Vec3f(p4, p5, p6), forward,
                                    reverse);
      newPair.fwd = *forward;
      newPair.rev = *reverse;
      m_storedTransforms[transformDef] = newPair;
      return true;
    default:
      printf("unknown transform type %c\n", typeKey);
      return false;
    }
  }
  return false;
}

} // namespace rengen::system