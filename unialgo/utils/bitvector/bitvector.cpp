#include "unialgo/utils/bitvector/bitvector.hpp"

#include <stddef.h>  // size_t

#include <cassert>
#include <cmath>
#include <iostream>

#include "unialgo/utils/bitvector/bitMaps.hpp"

namespace unialgo {
namespace utils {

Bitvector::Bitvector(std::size_t num_bits)
    : num_bits_(num_bits),
      bits_(std::ceil(num_bits / static_cast<double>(type_size))) {}

Bitvector::~Bitvector() { num_bits_ = 0; }

void Bitvector::SetBit(std::size_t bit_pos) {
  bits_[std::floor(bit_pos / type_size)] |= bit_set[bit_pos % type_size];
}

bool Bitvector::GetBit(std::size_t bit_pos) const {
  return bits_[std::floor(bit_pos / type_size)] & bit_set[bit_pos % type_size];
}

Bitvector::Reference Bitvector::operator[](std::size_t bit_pos) {
  return BitvectorReference(&bits_[std::floor(bit_pos / type_size)],
                            (bit_pos % type_size));
}

Bitvector::ConstReference Bitvector::operator[](std::size_t bit_pos) const {
  return BitvectorReference(&bits_[std::floor(bit_pos / type_size)],
                            (bit_pos % type_size));
}

std::size_t Bitvector::getNumBits() const { return num_bits_; }

std::size_t Bitvector::size() const { return num_bits_; }

Bitvector Bitvector::operator&(const Bitvector& bv) {
  // assert(bv.bits_.size() == bits_.size() && "bitvector size not matching");
  assert(bv.getNumBits() == num_bits_ && "bitvector size not matching");
  Bitvector res(bv.num_bits_);
  for (std::size_t i = 0; i < res.bits_.size(); ++i) {
    res.bits_[i] = bits_[i] & bv.bits_[i];
  }
  return res;
}

Bitvector& Bitvector::operator&=(const Bitvector& bv) {
  // assert(bv.bits_.size() == bits_.size() && "bitvector size not matching");
  assert(bv.getNumBits() == num_bits_ && "bitvector size not matching");
  for (std::size_t i = 0; i < bits_.size(); ++i) {
    bits_[i] = bits_[i] & bv.bits_[i];
  }
  return *this;
}

std::ostream& operator<<(std::ostream& os, const Bitvector& bv) {
  for (int64_t i = bv.size() - 1; i >= 0; --i) os << bv[i];
  return os;
}

}  // namespace utils
}  // namespace unialgo
