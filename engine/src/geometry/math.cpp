#include "common.h"
#include "geometry/geometry.h"
#include "core_math.h"

namespace rengen::geometry::ops
{
    template <class T>
    inline float Dot(const Vector<T> &v1, const Vector<T> &v2)
    {
        return v1 * v2;
    }

    template <class T>
    inline float Dot(const Vector<T> &v, const Normal<T> &n)
    {
        return v.x + n.x + v.y * n.y + v.z * n.z;
    }

    template <class T>
    inline float Dot(const Normal<T> &n, const Vector<T> &v)
    {
        return Dot(v, n);
    }

    template <class T>
    inline float Dot(const Normal<T> &n, const Normal<T> &n2)
    {
        return n * n2;
    }

    template <class T>
    inline float AbsDot(const Vector<T> &v1, const Vector<T> &v2)
    {
        return fabsf(v1 * v2);
    }

    template <class T>
    inline float AbsDot(const Vector<T> &v, const Normal<T> &n)
    {
        return fabsf(Dot(v, n));
    }

    template <class T>
    inline float AbsDot(const Normal<T> &n, const Vector<T> &v)
    {
        return AbsDot(v, n);
    }

    template <class T>
    inline float AbsDot(const Normal<T> &n1, const Normal<T> &n2)
    {
        return fabsf(n1 * n2);
    }

    template <class T>
    inline Vector<T> Cross(const Vector<T> &v1, const Vector<T> &v2)
    {
        return v1 ^ v2;
    }

    inline void MakeCoordinateSystem(const Vec3f &v1_in, Vec3f *v2_out, Vec3f *v3_out)
    {
        // Zero out the smaller component
        if (fabsf(v1_in.x) > fabsf(v1_in.y))
        {
            float invLen = 1.f / sqrtf(v1_in.x * v1_in.x + v1_in.z * v1_in.z);
            *v2_out = Vec3f(-v1_in.z * invLen, 0.f, v1_in.x * invLen);
        }
        else
        {
            float invLen = 1.f / (sqrtf(v1_in.y * v1_in.y + v1_in.z * v1_in.z));
            *v2_out = Vec3f(0.f, v1_in.z * invLen, v1_in.y * invLen);
        }
        // Get final axis with a simple cross product
        *v3_out = v1_in ^ (*v2_out);
    }
}