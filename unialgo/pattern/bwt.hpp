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
  Bwt(unialgo::utils::WordVector& text);

 private:
  /**
   * @brief Backward extension of interval
   *
   * input Q-interval [b, e) -> output sigmaQ-interval [b', e')
   * @attention Time complexity: O(1)
   *
   * @param b start of interval included
   * @param e end of interval excluded
   * @param sigma value to extend interval with
   * @return std::pair<std::size_t, std::size_t> new interval [b', e')
   */
  std::pair<std::size_t, std::size_t> backward_extend(uint64_t b, uint64_t e,
                                                      uint64_t sigma);

  std::unordered_map<uint64_t, std::size_t> c_;  // stores #values < key
  unialgo::utils::WaveletMatrix
      occ_;  // data structure for rank(value, position)
};

}  // namespace pattern
}  // namespace unialgo

#endif  // UNIALGO_PATTERN_BWT_