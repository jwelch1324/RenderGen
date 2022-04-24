#ifndef BBOX_H_
#define BBOX_H_

#include "common.h"
#include "geometry/vector.h"
#include "geometry/point.h"

namespace rengen::geometry
{
    class COREDLL BBox
    {
    public:
        P3f pMin, pMax;

        BBox()
        {
            pMin = P3f(INFINITY, INFINITY, INFINITY);
            pMax = P3f(-INFINITY, -INFINITY, -INFINITY);
        }
        BBox(const P3f &p) : pMin(p), pMax(p) {}
        BBox(const P3f &p1, const P3f &p2)
        {
            pMin = P3f(fmin(p1.x, p2.x), fmin(p1.y, p2.y), fmin(p1.z, p2.z));
            pMax = P3f(fmax(p1.x, p2.x), fmax(p1.y, p2.y), fmax(p1.z, p2.z));
        }

        BBox Union(const P3f &p) const
        {
            BBox ret = (*this);
            ret.pMin.x = fmin(ret.pMin.x, p.x);
            ret.pMin.y = fmin(ret.pMin.y, p.y);
            ret.pMin.z = fmin(ret.pMin.z, p.z);
            ret.pMax.x = fmax(ret.pMax.x, p.x);
            ret.pMax.y = fmax(ret.pMax.y, p.y);
            ret.pMax.z = fmax(ret.pMax.z, p.z);
            return ret;
        }

        BBox Union(const BBox &b2) const
        {
            BBox ret = (*this);
            ret.pMin.x = fmin(ret.pMin.x, b2.pMin.x);
            ret.pMin.y = fmin(ret.pMin.y, b2.pMin.y);
            ret.pMin.z = fmin(ret.pMin.z, b2.pMin.z);
            ret.pMax.x = fmax(ret.pMax.x, b2.pMax.x);
            ret.pMax.y = fmax(ret.pMax.y, b2.pMax.y);
            ret.pMax.z = fmax(ret.pMax.z, b2.pMax.z);
            return ret;
        }

        bool Overlaps(const BBox &b) const
        {
            bool x = (pMax.x >= b.pMin.x) && (pMin.x <= b.pMax.x);
            bool y = (pMax.y >= b.pMin.y) && (pMin.y <= b.pMax.y);
            bool z = (pMax.z >= b.pMin.z) && (pMin.z <= b.pMax.z);
            return (x && y && z);
        }

        bool Inside(const P3f &p) const
        {
            return (p.x >= pMin.x) && (p.x <= pMax.x) && (p.y >= pMin.y) && (p.y <= pMax.y) && (p.z >= pMin.z) && (p.z <= pMax.z);
        }

        float Volume() const
        {
            Vec3f v = pMax - pMin;
            return v.x * v.y * v.z;
        }

        void Expand(float delta)
        {
            pMin -= Vec3f(delta, delta, delta);
            pMax += Vec3f(delta, delta, delta);
        }

        int MaximumExtent() const
        {
            Vec3f diag = pMax - pMin;
            if (diag.x > diag.y && diag.x > diag.z)
            {
                return 0;
            }
            else if (diag.y > diag.z)
            {
                return 1;
            }
            else
            {
                return 2;
            }
        }

        void BoundingSphere(P3f *c, float *rad) const
        {
            *c = 0.5f * pMin + 0.5f * pMax;
            *rad = L2Distance(*c, pMax);
        }
    };
}

#endif