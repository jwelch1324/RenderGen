#ifndef MATRIX_H_
#define MATRIX_H_

#include "common.h"
#include "system/rengen_memory.h"

namespace rengen::geometry
{
    template <class T>
    class COREDLL Matrix4x4 : public ReferenceCounted
    {
    public:
        T m[4][4];

        Matrix4x4()
        {
            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    if (i == j)
                        m[i][j] = (T)(1);
                    else
                        m[i][j] = (T)(0);
                }
            }
        }

        Matrix4x4(T mat[4][4])
        {
            for (int i = 0; i < 4; i++)
                for (int j = 0; j < 4; j++)
                    m[i][j] = mat[i][j];
        }

        Matrix4x4(
            T t00, T t01, T t02, T t03,
            T t10, T t11, T t12, T t13,
            T t20, T t21, T t22, T t23,
            T t30, T t31, T t32, T t33)
        {
            m[0][0] = t00;
            m[0][1] = t01;
            m[0][2] = t02;
            m[0][3] = t03;

            m[1][0] = t10;
            m[1][1] = t11;
            m[1][2] = t12;
            m[1][3] = t13;

            m[2][0] = t20;
            m[2][1] = t21;
            m[2][2] = t22;
            m[2][3] = t23;

            m[3][0] = t30;
            m[3][1] = t31;
            m[3][2] = t32;
            m[3][3] = t33;
        }

        Reference<Matrix4x4> Transpose() const
        {
            return new Matrix4x4(
                m[0][0], m[1][0], m[2][0], m[3][0],
                m[0][1], m[1][1], m[2][1], m[3][1],
                m[0][2], m[1][2], m[2][2], m[3][2],
                m[0][3], m[1][3], m[2][3], m[3][3]);
        }

        static Reference<Matrix4x4> MatMul(const Reference<Matrix4x4> &m1, const Reference<Matrix4x4> &m2)
        {
            float r[4][4];
            for (int i = 0; i < 4; i++)
                for (int j = 0; j < 4; j++)
                    r[i][j] = m1->m[i][0] * m2->m[0][j] +
                              m1->m[i][1] * m2->m[1][j] +
                              m1->m[i][2] * m2->m[2][j] +
                              m1->m[i][3] * m2->m[3][j];
            return new Matrix4x4(r);
        }
    };
}

#endif