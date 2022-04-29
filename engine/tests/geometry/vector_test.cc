#include "geometry/vector.h"
#include <gtest/gtest.h>

using namespace rengen::geometry;

TEST(HelloTest, BasicAssertions) {
  EXPECT_STRNE("hello", "world");

  EXPECT_EQ(7 * 6, 42);
}

TEST(VectorAdditionTest, BasicAssertions) {
  Vec3f v(1, 1, 1);
  Vec3f v2(2, 2, 2);

  EXPECT_EQ(v + v2, Vec3f(3, 3, 3));
}

TEST(VectorSubtractionTest, BasicAssertions) {
  Vec3f v(1, 1, 1);
  Vec3f v2(2, 2, 2);

  EXPECT_EQ(v - v2, Vec3f(-1, -1, -1));
}

TEST(VectorMultiplyByScalarTest, BasicAssertions) {
  Vec3f v(1.1, 2.2, 3.3);
  Vec3i v2(1, 2, 3);

  EXPECT_EQ(2 * v, Vec3f(2.2, 4.4, 6.6));
  EXPECT_EQ(2 * v2, Vec3i(2, 4, 6));
  EXPECT_EQ(2.1 * v2, Vec3i(2, 4, 6));

  EXPECT_EQ(2 * v2, v2 * 2);
  EXPECT_EQ(2 * v, v * 2);

  EXPECT_EQ(v / 2.0f, Vec3f(1.1 / 2, 2.2 / 2, 3.3 / 2));
}

TEST(VectorTypeConversionTest, BasicAssertions) {
  Vec3f v(1.1, 2.2, 3.3);
  EXPECT_EQ(v.ToVec3f(), v);
  EXPECT_EQ(v.ToVec3i(), Vec3i(1, 2, 3));
}

TEST(VectorIndexTest, BasicAssertions) {
  Vec3f v(1, 2, 3);
  EXPECT_EQ(v.x, v[0]);
  EXPECT_EQ(v.y, v[1]);
  EXPECT_EQ(v.z, v[2]);
  EXPECT_DEATH(v[3], "Assertion failed");
}

TEST(VectorNormTest, BasicAssertions) {
  Vec3f v(1.1, 2.2, 6234.2);
  Vec3f vn = v / v.norm();

  EXPECT_FLOAT_EQ(v.norm(), 6234.200485226634095);
  EXPECT_FLOAT_EQ(Vec3f::Normalize(v).norm(), 1.0);
}

TEST(VectorCrossProductTest, BasicAssertions) {
  //   Vec3f a(432.792023, 421.233856, 193.626312);
  //   Vec3f b(466.344543, 399.482147, 185.834976);
  //   Vec3f c(442.255188, 446.765808, 183.575760);

  Vec3f a(-0.142316, -0.166791, 0.411840);
  Vec3f b(-0.073039, -0.218144, 0.372349);
  Vec3f c(-0.126228, -0.116368, 0.360528);

  Vec3f ac = c - a;
  Vec3f ab = b - a;
  Vec3f zz = ac ^ ab;
  // EXPECT_EQ(zz, Vec3f(-0.663736,-0.418848,-0.619695));
  EXPECT_TRUE(zz.epsilon_eq(Vec3f(-0.663736, -0.418848, -0.619695)));
}