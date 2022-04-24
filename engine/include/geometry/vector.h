#ifndef VECTOR_H_
#define VECTOR_H_

#include "common.h"

namespace rengen::geometry
{
    template <class T>
    class COREDLL Normal;

    template <class T>
    class COREDLL Vector
    {
    public:
        T x, y, z;
        Vector<T>(T _x = 0, T _y = 0, T _z = 0) : x(_x), y(_y), z(_z) {}

        explicit Vector<T>(const Normal<T> &n) : x(n.x), y(n.y), z(n.z) {}

        // Equality of Two Vectors
        bool operator==(const Vector<T> &rhs) const
        {
            return (x == rhs.x) && (y == rhs.y) && (z == rhs.z);
        }

        bool operator!=(const Vector<T> &rhs) const
        {
            return (x != rhs.s) || (y != rhs.y) || (z != rhs.z);
        }

        bool epsilon_eq(const Vector<T> &rhs, float eps = 1e-6) const
        {
            return (abs(x - rhs.x) <= eps) && (abs(y - rhs.y) <= eps) && (abs(z - rhs.z) <= eps);
        }

        // Addition/Subtraction of Two Vectors
        Vector<T> operator+(const Vector<T> &v) const
        {
            return Vector<T>(x + v.x, y + v.y, z + v.z);
        }

        Vector<T> &operator+=(const Vector<T> &v)
        {
            x += v.x;
            y += v.y;
            z += v.z;
            return *this;
        }

        Vector<T> operator-(const Vector<T> &v) const
        {
            return Vector<T>(x - v.x, y - v.y, z - v.y);
        }

        Vector<T> &operator-=(const Vector<T> &v)
        {
            x -= v.x;
            y -= v.y;
            z -= v.z;
            return *this;
        }

        // Scaling of a Vector
        Vector<T> operator*(float f) const
        {
            return Vector<T>((T)(x * f), (T)(y * f), (T)(z * f));
        }

        Vector<T> &operator*=(float f)
        {
            x = T(x * f);
            y = T(y * f);
            z = T(z * f);
            return *this;
        }

        Vector<T> operator/(float f) const
        {
            assert(f != 0);
            float inv = 1.0f / f;
            return Vector<T>((T)(x * inv), (T)(y * inv), (T)(z * inv));
        }

        Vector<T> operator/=(float f)
        {
            assert(f != 0);
            float inv = 1.0f / f;
            x *= f;
            y *= f;
            z *= f;
            return *this;
        }

        // Vector Product Operators
        // Dot Product between two vectors
        inline float operator*(const Vector<T> &rhs) const
        {
            return x * rhs.x + y * rhs.y + z * rhs.z;
        }

        inline float cosine(const Vector<T> &o) const
        {
            return (this * o) / (this->norm() * o.norm());
        }

        // Cross Product between two vectors
        inline Vector<T> operator^(const Vector<T> &rhs) const
        {
            return Vector<T>(this->y * rhs.z - this->z * rhs.y, -(this->x * rhs.z - this->z * rhs.x), this->x * rhs.y - this->y * rhs.x);
        }

        inline float sine(const Vector<T> &o) const
        {
            // Sine of the angle is the ratio of the norm of the cross product and the product of norms of the individual vectors
            return ((*this) ^ o).norm() / (this->norm() * o.norm());
        }

        // Negation Operator
        Vector<T> operator-() const
        {
            return Vector<T>(-x, -y, -z);
        }

        // Indexing Operator
        T operator[](int i) const
        {
            assert(i >= 0 && i <= 2);
            return (&x)[i];
        }

        T &operator[](int i)
        {
            assert(i >= 0 && i <= 2);
            return (&x)[i];
        }

        // Normalization Functions
        inline float norm() const
        {
            // Perform dot product with self and take sqrt
            return sqrtf(*this * *this);
        }

        inline float Length() const { return norm(); }

        inline float LengthSquared() const { return *this * *this; }

        static inline Vector<T> Normalize(const Vector<T> &v)
        {
            return v / v.norm();
        }

        // Type Conversion Functions

        // Convert the current vector to a Vec3f
        Vector<float> ToVec3f() const
        {
            return Vector<float>(float(x), float(y), float(z));
        }

        // Convert the current vector to a Vec3i
        Vector<int> ToVec3i() const
        {
            return Vector<int>(int(x), int(y), int(z));
        }
    };

    typedef Vector<float> Vec3f;
    typedef Vector<int> Vec3i;

    template <class T>
    inline Vector<T> operator*(float f, const Vector<T> &v)
    {
        return v * f;
    }

}
#endif