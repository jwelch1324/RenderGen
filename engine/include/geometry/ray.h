#ifndef RAY_H_
#define RAY_H_

#include "common.h"
#include "geometry/vector.h"
#include "geometry/point.h"

namespace rengen::geometry
{
    class COREDLL Ray
    {
    public:
        Point<float> o;
        Vector<float> d;
        mutable float mint, maxt;
        float time;
        Ray() : mint(RAY_EPSILON), maxt(INFINITY), time(0.f) {}
        Ray(const Point<float> &origin, const Vector<float> &direction, float start = RAY_EPSILON, float end = INFINITY, float t = 0.f) : o(origin), d(direction), mint(start), maxt(end), time(t) {}

        Point<float> operator()(float t) const { return o + t * d; }
    };

    class COREDLL RayDifferential : public Ray
    {
    public:
        bool hasDifferentials;
        Ray rx, ry;

        RayDifferential() { hasDifferentials = false; }
        RayDifferential(const Point3f &origin, const Vec3f &direction) : Ray(origin, direction) { hasDifferentials = false; }
        explicit RayDifferential(const Ray &r) : Ray(r) { hasDifferentials = false; }
    };
}

#endif