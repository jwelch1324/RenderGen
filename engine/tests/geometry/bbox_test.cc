#include <gtest/gtest.h>
#include "geometry/geometry.h"

using namespace rengen::geometry;

TEST(BBoxFromTwoPointsTest, BasicAssertions)
{
    P3f p1(1.1, 2.2, 11.22);
    P3f p2(4.2, 3.3, 4.123);
    BBox b(p1, p2);
    EXPECT_EQ(b.pMin, P3f(1.1, 2.2, 4.123));
    EXPECT_EQ(b.pMax, P3f(4.2, 3.3, 11.22));
}