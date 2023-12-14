#include <iostream>

#include "unialgo/utils/bitvector/bitvector.hpp"

int main() {
  unialgo::utils::Bitvector bv(100);

  bv[0] = 1;
  bv[1] = 1;

  std::cout << "should be 3." << std::endl;

  bv.test();

  bv[0] = 0;
  bv[1] = 0;
  bv[12] = 1;

  std::cout << "should be 4096." << std::endl;

  bv.test();

  bv[12] = 0;

  std::cout << "should be 0." << std::endl;

  bv.test();

  bv[63] = 1;

  std::cout << "should be 2^63." << std::endl;

  bv.test();

  bv[63] = 0;
  bv[64] = 1;

  std::cout << "should be 0 1." << std::endl;

  bv.test();

  return 0;
}