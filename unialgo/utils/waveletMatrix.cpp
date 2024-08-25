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

  utils::WordVector::Type bit_to_check = 1 << matrix_depth_ - 1;
  // to keep track of where the words move from layer to layer
  // first layer order = string
  std::vector<utils::WordVector::Type> layer_order;
  for (std::size_t i = 0; i < string.size(); ++i)
    layer_order.push_back(string[i]);

  for (std::size_t layer = 0; layer < matrix_depth_; ++layer) {
    // to update the layer_order
    std::vector<utils::WordVector::Type> zeros, ones;

    // debugging layer order
    // for (auto a : layer_order) std::cout << a;
    // std::cout << std::endl;

    for (std::size_t i = 0; i < string.size(); ++i) {
      // bit_to_check is set in word
      if (layer_order[i] & bit_to_check) {
        (*matrix_)[i + string.size() * layer] = 1;
        ones.push_back(layer_order[i]);
      } else
        zeros.push_back(layer_order[i]);
    }
    // remake the layer_order based on 0s and 1s
    layer_order = zeros;
    layer_order.insert(layer_order.end(), ones.begin(), ones.end());
    // next bit to check
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
        (helper_.rank(layer * string_size_,
                      layer * string_size_ + string_size_ - 1, 0)) *
            value -
        1;
    bit_to_set = bit_to_set >> 1;
  }
  return res;
}

void WaveletMatrix::print() const {
  for (std::size_t i = 0; i < matrix_depth_; ++i) {
    for (std::size_t j = 0; j < string_size_; ++j) {
      std::cout << (*matrix_)[j + i * string_size_];
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
  for (int i = 24; i < 48; ++i) std::cout << (*matrix_)[i];
  std::cout << std::endl;
}

}  // namespace utils
}  // namespace unialgo
