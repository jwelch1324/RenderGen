#include <gtest/gtest.h>
#include <math.h>
#include "geometry/vector.h"
#include "geometry/point.h"

using namespace rengen::geometry;
TEST(PointAddVector, BasicAssertions)
{
    Vec3f v(1.1, 1.1, 1.1);
    P3f p(0, 0, 0);
    EXPECT_EQ(p + v, P3f(1.1, 1.1, 1.1));

    p += v;
    EXPECT_EQ(p, P3f(1.1, 1.1, 1.1));
}

TEST(PointSubVector, BasicAssertions)
{
    Vec3f v(1.1, 1.1, 1.1);
    P3f p(0, 0, 0);
    EXPECT_EQ(p - v, P3f(-1.1, -1.1, -1.1));

    p -= v;
    EXPECT_EQ(p, P3f(-1.1, -1.1, -1.1));
}

TEST(PointL2Distance, BasicAssertions)
{
    P3f p1(1, 1, 1);
    P3f p2(3, 3, 3);

    EXPECT_EQ(p1 - p2, Vec3f(-2, -2, -2));
    EXPECT_EQ(L2Distance(p1, p2), sqrtf(12.0f));
}