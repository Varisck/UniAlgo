#include <iostream>

#include "unialgo/utils/bitvector/bitvector.hpp"

int main() {
  unialgo::utils::Bitvector bv(100);

  bv[0] = 1;
  bv[1] = 1;

  std::cout << "should be 3." << std::endl;

  return 0;
}