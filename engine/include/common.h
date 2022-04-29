#ifndef COMMON_H_
#define COMMON_H_

#ifdef WIN32
#ifdef CORE_SOURCE
#define COREDLL __declspec(dllexport)
#else
#define COREDLL __declspec(dllimport)
#endif
#define DLLEXPORT __declspec(dllexport)
#else
#define COREDLL
#define DLLEXPORT
#endif

#define RAY_EPSILON 1e-3f

#include <math.h>
#include <cmath>
#include <assert.h>

#ifdef PBRT_FLOAT_AS_DOUBLE
typedef double Float;
#else
typedef float Float;
#endif // PBRT_FLOAT_AS_DOUBLE

#include "system/rengen_memory.h"

template <typename T, typename U, typename V>
inline T Clamp(T val, U low, V high) {
    if (val < low) return low;
    else if (val > high) return high;
    else return val;
}

#endif