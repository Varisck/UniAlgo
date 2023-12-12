#include <iostream>

#include "unialgo/utils/bitvector/bitvector.hpp"

int main() {
  unialgo::utils::Bitvector bv(62);

  bv.SetBit(32);
  std::cout << bv.GetBit(32) << std::endl;
  std::cout << bv[32] << std::endl;

  return 0;
}