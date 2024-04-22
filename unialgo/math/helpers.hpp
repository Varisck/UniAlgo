#ifndef UNIALGO_MATH_HELPERS_
#define UNIALGO_MATH_HELPERS_

#include <float.h>  // DBL_EPSILON

#include <cmath>  // std::abs

namespace unialgo {
namespace math {

// check if two doubles are (almost) equal, distance less then epliston % of max
bool dequal(double a, double b, double epsilon = DBL_EPSILON) {
  double res = std::abs(a - b);
  // find max without abs (?)
  double max = std::max(std::abs(a), std::abs(b));
  return res <= epsilon * max;
}

}  // namespace math
}  // namespace unialgo

#endif  // UNIALGO_MATH_HELPERS_