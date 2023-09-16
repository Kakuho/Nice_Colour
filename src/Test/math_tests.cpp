#include <vector>
#include <iostream>
#include "math.hpp"

int main(){
  std::vector<double> v1{1.0, 2.0, 3.0};
  std::vector<double> v2{2.0, 4.0, 8.0};
  std::cout << EuclideanNorm(v1, v2) << '\n';
}
