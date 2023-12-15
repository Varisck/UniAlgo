#include "unialgo/utils/bitvector/bitvector.hpp"

#include <stddef.h>  // size_t

#include <cmath>
#include <iostream>

#include "unialgo/utils/bitvector/bitMaps.hpp"

namespace unialgo {

utils::Bitvector::Bitvector(std::size_t num_bits)
    : num_bits_(num_bits),
      bits_(std::ceil(num_bits / static_cast<double>(type_size))) {}

utils::Bitvector::~Bitvector() { num_bits_ = 0; }

void utils::Bitvector::SetBit(std::size_t bit_pos) {
  bits_[std::floor(bit_pos / type_size)] |= bit_set[bit_pos % type_size];
}

bool utils::Bitvector::GetBit(std::size_t bit_pos) {
  return bits_[std::floor(bit_pos / type_size)] & bit_set[bit_pos % type_size];
}

utils::Bitvector::Reference utils::Bitvector::operator[](std::size_t bit_pos) {
  return BitvectorReference(bits_[std::floor(bit_pos / type_size)],
                            (bit_pos % type_size));
}

std::size_t utils::Bitvector::getNumBits() { return num_bits_; }

}  // namespace unialgo
