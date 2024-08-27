#ifndef UNIALGO_UTILS_WAVELET_MATRIX_
#define UNIALGO_UTILS_WAVELET_MATRIX_

#include <memory>  // std::shared_ptr
#include <vector>  // std::vector

#include "unialgo/utils/bitvector/bitvector.hpp"
#include "unialgo/utils/bitvector/rankHelper.hpp"
#include "unialgo/utils/bitvector/wordVector.hpp"

namespace unialgo {
namespace utils {

class WaveletMatrix {
 public:
  WaveletMatrix() = default;
  WaveletMatrix(const unialgo::utils::WordVector& string);

  ~WaveletMatrix() = default;

  WaveletMatrix(const WaveletMatrix&) = default;
  WaveletMatrix(WaveletMatrix&&) = default;

  WaveletMatrix& operator=(const WaveletMatrix&) = default;
  WaveletMatrix& operator=(WaveletMatrix&&) = default;

  /**
   * @brief Access value in string
   *
   * This is O(log(|alphabet|)) = O(matrix_depth_)
   * Note: basicly constant since |alphabet| is samll
   *
   * @param indx value to access
   * @return uint64_t bit rapresentation of string[i]
   */
  uint64_t acces(std::size_t indx) const;

  /**
   * @brief Rank of charachter up to position
   *
   * Complexity is O(log(|alphabet|)) = O(matrix_depth)
   * Note: basicly constant since |alphabet| is samll
   *
   * @param character character to count occurrences
   * @param pos position to end count (included)
   * @return std::size_t # of occ in [0, character]
   */
  std::size_t rank(const uint64_t character, std::size_t pos) const;
  std::size_t rank(const unialgo::utils::WordVectorConstReference character,
                   std::size_t pos) const;
  std::size_t rank(const unialgo::utils::WordVectorReference character,
                   std::size_t pos) const;

  void print() const;

  std::size_t getStringSize() const;
  std::size_t getMatrixDepth() const;

 private:
  std::size_t string_size_;                            // size of the string
  std::size_t matrix_depth_;                           // depth of the matrix
  std::shared_ptr<unialgo::utils::Bitvector> matrix_;  // bitvector for matrix_
  unialgo::utils::RankHelper helper_;  // helper for constant rank on bv
  unialgo::utils::WordVector Zs_;      // #0s in layerss
};  // class WaveletMatrix

}  // namespace utils

}  // namespace unialgo

#endif  // UNIALGO_UTILS_WAVELET_TREE_