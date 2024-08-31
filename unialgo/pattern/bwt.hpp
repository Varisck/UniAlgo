#ifndef UNIALGO_PATTERN_BWT_
#define UNIALGO_PATTERN_BWT_

#include <unordered_map>  // std::unordered_map

#include "unialgo/utils/bitvector/wordVector.hpp"
#include "unialgo/utils/waveletMatrix.hpp"

namespace unialgo {
namespace pattern {

/**
 * @class Implementation of Burrows-Wheeler-Transorm
 * @brief The implementation is done using FM-index
 *
 * @paragraph FM-index saves memory, the occ_ matrix is a waveletMatrix saves
 * space and query is consant (log_2 on size of alphabet)
 *
 */
class Bwt {
 public:
  /**
   * @brief Construct a new Bwt object
   *
   * @param text WordVector to construct btw from
   */
  Bwt(const unialgo::utils::WordVector& text);

  /**
   * @brief Construct a new Bwt object
   *
   * @param text WordVector to construct btw from
   * @param sa suffix array of text
   */
  Bwt(const unialgo::utils::WordVector& text, unialgo::utils::WordVector sa);

  /**
   * @brief Returns Bwt[pos]
   *
   * @attention Time complexity: O(log(|alphabet|))
   *
   * @param pos position to access value from
   * @return uint64_t value at Bwt[pos]
   */
  uint64_t operator[](std::size_t pos) const;

  /**
   * @brief Return Bwt[pos]
   *
   * @attention Time complexity: O(log(|alphabet|))
   * this checks if pos >= text.size()
   *
   * @param pos position to access value from
   * @return uint64_t value at Bwt[i]
   */
  uint64_t at(std::size_t pos) const;

  /**
   * @brief Search for pattern in BWT
   *
   * @param pattern pattern to search
   * @return std::vector<std::size_t> position in Suffix Array of text where
   * pattern is found
   */
  std::vector<std::size_t> searchPattern(
      const unialgo::utils::WordVector& pattern) const;

  /**
   * @brief Search for pattern in BWT
   *
   * @param pattern pattern to search
   * @param sa Suffix Array of original text
   * @return std::vector<std::size_t> position in original text where pattern
   * start
   */
  std::vector<std::size_t> searchPattern(
      const unialgo::utils::WordVector& pattern,
      const unialgo::utils::WordVector& sa) const;

  /**
   * @brief Get the Word Size of WordVector text
   *
   * @return std::size_t size of the single WordVector reference
   */
  std::size_t getWordSize() const;

  /**
   * @brief Returns Bwt size
   *
   * @return std::size_t size of bwt
   */
  std::size_t size() const;

 private:
  /**
   * @brief Backward extension of interval
   * input Q-interval [b, e) -> output sigmaQ-interval [b', e')
   * @attention Time complexity: O(1)
   *
   * @param b start of interval included
   * @param e end of interval excluded
   * @param sigma value to extend interval with
   * @return std::pair<std::size_t, std::size_t> new interval [b', e')
   */
  std::pair<std::size_t, std::size_t> backward_extend(uint64_t b, uint64_t e,
                                                      uint64_t sigma) const;

  std::unordered_map<uint64_t, std::size_t> c_;  // stores #values < key
  unialgo::utils::WaveletMatrix
      occ_;  // data structure for rank(value, position)
};

}  // namespace pattern
}  // namespace unialgo

#endif  // UNIALGO_PATTERN_BWT_