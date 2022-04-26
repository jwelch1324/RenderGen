#ifndef NORMAL_H_
#define NORMAL_H_

#include "common.h"
#include "geometry/vector.h"
#include <assert.h>

namespace rengen::geometry
{
    template <class T>
    class COREDLL Normal
    {
    public:
        T x, y, z;
        Normal(T _x = 0, T _y = 0, T _z = 0) : x(_x), y(_y), z(_z) {}
        explicit Normal(const Vector<T> &v) : x(v.x), y(v.y), z(v.z) {}

        bool operator==(const Normal<T> &o) const { return (x == o.x) && (y == o.y) && (z == o.z); }
        bool operator!=(const Normal<T> &o) const { return (x != o.x) || (y != o.y) || (z != o.z); }

        Normal<T> operator-() const { return Normal<T>(-x, -y, -z); }
        Normal<T> operator+(const Normal<T> &n) { return Normal<T>((T)(x + n.x), (T)(y + n.y), (T)(z + n.z)); }
        Normal<T> &operator+=(const Normal<T> &n)
        {
            x += n.x;
            y += n.y;
            z += n.z;
            return (*this);
        }

        Normal<T> operator-(const Normal<T> &n) const { return (*this + (-n)); }
        Normal<T> operator-=(const Normal<T> &n)
        {
            (*this) += (-n);
            return (*this);
        }

        Normal<T> operator*(const Float f) const { return Normal<T>((T)(x * f), (T)(y * f), (T)(z * f)); }
        Normal<T> operator*=(const Float f)
        {
            x = T(x * f);
            y = T(y * f);
            z = T(z * f);
            return (*this);
        }

        Normal<T> operator/(const Float f) const
        {
            assert(f != 0);
            Float inv = 1.0f / f;
            return Normal<T>(x * inv, y * inv, z * inv);
        }
        Normal<T> operator/=(const Float f)
        {
            assert(f != 0);
            Float inv = 1.0f / f;
            x *= inv;
            y *= inv;
            z *= inv;
            return (*this);
        }

        inline Float operator*(const Normal<T> &o) const
        {
            return x * o.x + y * o.y + z * o.z;
        }

        inline Float LengthSquared() const
        {
            return (x * x + y * y + z * z);
        }

        inline Float norm() const
        {
            return sqrtf(LengthSquared());
        }

        static Normal<T> Normalize(const Normal<T> &n)
        {
            return n / n.norm();
        }
    };

    typedef Normal<Float> N3f;
    typedef Normal<int> N3i;
    typedef Normal<Float> Normal3f;
    typedef Normal<int> Normal3i;

}

#endif