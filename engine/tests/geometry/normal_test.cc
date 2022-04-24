#include <gtest/gtest.h>
#include "geometry/vector.h"
#include "geometry/normal.h"

using namespace rengen::geometry;

TEST(NormalFromVector, BasicAssertions)
{
    Vec3f v(1.1, 1.1, 1.1);
    Normal<float> n;
    n = Normal<float>(v);
    EXPECT_EQ(n, Normal<float>(1.1, 1.1, 1.1));
}

TEST(VectorFromNormal, BasicAssertions)
{
    N3f n(1.1, 2.2, 3.3);
    Vec3f v;
    v = Vec3f(n);
    EXPECT_EQ(v, Vec3f(1.1, 2.2, 3.3));
}