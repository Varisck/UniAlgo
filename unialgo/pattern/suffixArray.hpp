#ifndef UNIALGO_PATTERN_SUFFIXARRAY_
#define UNIALGO_PATTERN_SUFFIXARRAY_

#include <vector>

#include "unialgo/utils/bitvector/wordVector.hpp"

/**
 * @brief This file contains an implementation of a suffix array
 * \file suffixArray.hpp
 *
 *  suffix array creation is not linear yet (not the best implementation beside
 * the scope of the project)
 *
 */

namespace unialgo {
namespace pattern {

/**
 * @class SuffixArray
 *
 */
class SuffixArray {
 public:
  /**
   * @brief Construct a new Suffix Array object
   *
   * this also checks that the wv is $ terminated
   * (it is required that the last word is the smalles in the
   * alphabet order and unique)
   *
   * @param wv reference to construct sa from
   */
  SuffixArray(utils::WordVector& wv);

  // this now returns the index position
  // maybe return the value of word at pos using shard_ptr?
  std::size_t operator[](std::size_t pos) const;

  /**
   * @brief Size of Suffix Array
   *
   * @return std::size_t size
   */
  std::size_t size() const;

 private:
  unialgo::utils::WordVector sa_;
  // std::vector<std::size_t> sa_;  // suffix array
  // using shared_pointer to original wv for access?
};

}  // namespace pattern
}  // namespace unialgo

#endif  // UNIALGO_PATTERN_SUFFIXARRAY_