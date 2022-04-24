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
#include <assert.h>

#endif