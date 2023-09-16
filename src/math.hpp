#ifndef MATH_HPP
#define MATH_HPP
// Math header which implements helper mathematical function used throughout the project
#include <vector>
#include <cmath>
#include <cassert>

template <typename T>
double EuclideanNorm(std::vector<T>& v1, std::vector<T>& v2){
  assert(v1.size() == 3);
  assert(v2.size() == 3);
  return std::sqrt(
      std::pow(v1[0] - v2[0], 2) +
      std::pow(v1[1] - v2[1], 2) +
      std::pow(v1[2] - v2[2], 2)
  );
}


#endif
