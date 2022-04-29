#include "geometry/bbox.h"
#include "geometry/point.h"
#include "ops/transform.h"
#include <benchmark/benchmark.h>

using namespace rengen::geometry;
using namespace rengen::ops;

static void BM_BBoxDefaultTransform(benchmark::State &state) {
  BBox b0(Point3f(0, 0, 0), Point3f(1, 1, 1));
  auto tr = Translate(Vec3f(1, 1, 1));
  for (auto _ : state)
    auto newBB = tr(b0);
}

static void BM_BBoxSlowTransform(benchmark::State &state) {
  BBox b0(Point3f(0, 0, 0), Point3f(1, 1, 1));
  auto tr = Translate(Vec3f(1, 1, 1));
  for (auto _ : state)
    auto newBB = tr.SlowBBoxTransform(b0);
}

static void BM_BBoxFastTransform(benchmark::State &state) {
  BBox b0(Point3f(0, 0, 0), Point3f(1, 1, 1));
  auto tr = Translate(Vec3f(1, 1, 1));
  for (auto _ : state)
    auto newBB = tr.FastBBoxTransform(b0);
}

BENCHMARK(BM_BBoxDefaultTransform);
BENCHMARK(BM_BBoxSlowTransform);
BENCHMARK(BM_BBoxFastTransform);

BENCHMARK_MAIN();