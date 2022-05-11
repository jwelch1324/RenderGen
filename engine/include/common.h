#ifndef COMMON_H_
#define COMMON_H_

#include <limits>
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

#include <assert.h>
#include <cmath>
#include <math.h>
#include <sstream>
#include <string>
#include <vector>

#ifdef PBRT_FLOAT_AS_DOUBLE
typedef double Float;
#else
typedef float Float;
#endif // PBRT_FLOAT_AS_DOUBLE

static constexpr Float MaxFloat = std::numeric_limits<Float>::max();
static constexpr Float Infinity = std::numeric_limits<Float>::infinity();

#ifdef __linux__
#include <memory.h>
#endif

#include "system/rengen_memory.h"

template <typename T, typename U, typename V>
inline T Clamp(T val, U low, V high) {
  if (val < low)
    return low;
  else if (val > high)
    return high;
  else
    return val;
}

inline std::vector<std::string> strsplit(const std::string &p_pcstStr,
                                         char delim) {
  std::vector<std::string> tokens;
  std::stringstream mySstream(p_pcstStr);
  std::string temp;

  while (getline(mySstream, temp, delim)) {
    tokens.push_back(temp);
  }

  return tokens;
}

template <typename T> inline bool EpislonEqual(T f1, T f2, Float eps = 1e-6) {
  return (std::abs(f1 - f2) <= eps);
}

#endif