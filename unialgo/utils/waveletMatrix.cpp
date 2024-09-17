#include "unialgo/utils/waveletMatrix.hpp"

#include <cmath>   // std::ceil
#include <vector>  // std::vector

namespace unialgo {
namespace utils {

WaveletMatrix::WaveletMatrix(const utils::WordVector& string)
    : string_size_(string.size()) {
  // matrix_depth_ = len_alphabet, assume len alphabet is wordSize
  matrix_depth_ = string.getWordSize();

  matrix_ = std::make_shared<unialgo::utils::Bitvector>(
      unialgo::utils::Bitvector(string.size() * matrix_depth_));
  Zs_ = WordVector(matrix_depth_,
                   std::ceil(std::log(string.size()) / std::log(2)));

  utils::WordVector::Type bit_to_check = 1 << matrix_depth_ - 1;
  utils::WordVector layer_order = string;

  for (std::size_t layer = 0; layer < matrix_depth_; ++layer) {
    // for (auto a : layer_order) std::cout << a.getValue();
    // std::cout << std::endl;

    utils::WordVector backet(string.size(), string.getWordSize());
    std::size_t zero_count = 0;

    for (std::size_t i = 0; i < string.size(); ++i) {
      if (layer_order[i].getValue() & bit_to_check) {
        (*matrix_)[i + string.size() * layer] = 1;
        backet[string.size() - 1 - (i - zero_count)] = layer_order[i];
      } else {
        backet[zero_count] = layer_order[i];
        ++zero_count;
      }
    }
    // store the zero_count of layer
    Zs_[layer] = zero_count;
    // reverse the 1s in backet
    for (std::size_t i = zero_count;
         i < (string.size() - zero_count) / 2 + zero_count; ++i) {
      auto a = backet[i].getValue();
      backet[i] = backet[string.size() - 1 - (i - zero_count)];
      backet[string.size() - 1 - (i - zero_count)] = a;
    }
    layer_order = backet;

    bit_to_check = bit_to_check >> 1;
  }

  helper_ = utils::RankHelper(matrix_);
}

std::size_t WaveletMatrix::getMatrixDepth() const { return matrix_depth_; }

std::size_t WaveletMatrix::getStringSize() const { return string_size_; }

uint64_t WaveletMatrix::acces(std::size_t indx) const {
  uint64_t res = 0;
  uint64_t bit_to_set = 1 << matrix_depth_ - 1;
  std::size_t pos = indx;  // this is relative to layers and not bv

  for (std::size_t layer = 0; layer < matrix_depth_; ++layer) {
    bool value = (*matrix_)[pos + layer * string_size_].getValue();

    // conditionaly set or clear bit (bit hacks)
    res ^= (-value ^ res) & bit_to_set;
    // if value = 1 need to add to position count of 0s in layer
    // to get position of 1 in next layer need to skip all the 0s
    pos =
        helper_.rank(layer * string_size_, pos + layer * string_size_, value) +
        Zs_[layer] * value - 1;
    bit_to_set = bit_to_set >> 1;
  }
  return res;
}

std::size_t WaveletMatrix::rank(const uint64_t character, std::size_t i) const {
  uint64_t bit_to_set = 1 << matrix_depth_ - 1;
  std::size_t p = 0;
  std::size_t layer_start;
  for (std::size_t layer = 0; layer < matrix_depth_; ++layer) {
    bool bit_value = character & bit_to_set;  // bit value of char at level
    layer_start = layer * string_size_;  // position in bv of first bit in layer
    // if p == 0 can't do p - 1
    if (p > 0) p = helper_.rank(layer_start, p - 1 + layer_start, bit_value);
    p += (Zs_[layer] * bit_value);
    i = helper_.rank(layer_start, i + layer_start, bit_value) +
        (Zs_[layer] * bit_value) - 1;
    if (i + 1 - p == 0) return 0;  // check for overflow of i
    bit_to_set = bit_to_set >> 1;
  }
  return i - p + 1;
}

std::size_t WaveletMatrix::rank(
    const unialgo::utils::WordVectorConstReference character,
    std::size_t pos) const {
  return rank(character.getValue(), pos);
}

std::size_t WaveletMatrix::rank(
    const unialgo::utils::WordVectorReference character,
    std::size_t pos) const {
  return rank(character.getValue(), pos);
}

}  // namespace utils
}  // namespace unialgo
