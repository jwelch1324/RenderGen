#include "geometry/bbox.h"
#include "geometry/point.h"
#include "ops/transform.h"
#include <gtest/gtest.h>

using namespace rengen::ops;
using namespace rengen::geometry;

TEST(BBoxTransformTest, BasicAssertions) {
  BBox b0(Point3f(0, 0, 0), Point3f(1, 1, 1));

  auto tr = Translate(Vec3f(1, 1, 1));

  auto newBB = tr(b0);
  auto newFastBB = tr.FastBBoxTransform(b0);
  EXPECT_TRUE(newBB.epsilonEqual(newFastBB));

  auto str = Scale(Vec3f(1, 2, 3));

  newBB = str(b0);
  newFastBB = str(b0);
  EXPECT_TRUE(newBB.epsilonEqual(newFastBB));
}