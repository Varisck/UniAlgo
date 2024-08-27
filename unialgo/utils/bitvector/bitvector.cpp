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

Bitvector::Reference Bitvector::at(std::size_t pos) {
  assert(pos < num_bits_ && "Bitvector access out of bounds");
  return this->operator[](pos);
}

Bitvector::ConstReference Bitvector::at(std::size_t pos) const {
  assert(pos < num_bits_ && "Bitvector access out of bounds");
  return this->operator[](pos);
}

typename Bitvector::Bitvector Bitvector::operator()(std::size_t start,
                                                    std::size_t end) const {
  assert(start <= end && "Bitvector operator() start > end");
  std::size_t bits_to_read = end - start + 1;
  Bitvector bv(bits_to_read);
  std::size_t current_word = 0;            // current word in bv to save
  std::size_t offset = start % type_size;  // offset relative to word
  Bitvector::Type const* word =
      &bits_[start / type_size];         // pointer to word with bits
  Bitvector::Type w1 = *word >> offset;  // word containing bits in bv

  // read full words as long as possible
  while (bits_to_read > 64) {
    w1 = w1 | ((*(word + 1) & utils::lower_bits_set[offset]) << (64 - offset));
    bv.bits_[current_word] = w1;
    ++current_word;
    bits_to_read -= 64;
    word += 1;
    w1 = *word >> offset;
  }
  // if reamining bites_to_read overflow in next word by offset
  if ((offset + bits_to_read) > 64) {
    w1 = w1 |
         ((*(word + 1) &
           lower_bits_set[(offset + bits_to_read) & 0x3F])  // like doing % 64
          << (64 - offset));
    bv.bits_[current_word] = w1;
    return bv;
  }
  // case where reamining bits_to_read do not overflow the word
  w1 = w1 & lower_bits_set[bits_to_read];
  bv.bits_[current_word] = w1;
  return bv;
}

std::size_t Bitvector::getNumBits() const { return num_bits_; }

std::size_t Bitvector::size() const { return num_bits_; }

std::vector<typename Bitvector::Type> Bitvector::getBitVec() const {
  return bits_;
}

bool Bitvector::operator==(const Bitvector& other) const {
  if (this->getNumBits() != other.getNumBits()) return false;
  for (std::size_t i = 0; i < bits_.size(); ++i)
    if (bits_[i] != other.bits_[i]) return false;
  return true;
}

bool Bitvector::operator!=(const Bitvector& other) const {
  return !(*this == other);
}

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
  for (std::size_t i = bv.size() - 1; i > 0; --i) os << bv[i];
  os << bv[0];
  return os;
}

}  // namespace utils
}  // namespace unialgo
