#ifndef UNIALGO_PATTERN_SUFFIXARRAY_
#define UNIALGO_PATTERN_SUFFIXARRAY_

#include "unialgo/utils/bitvector/wordVector.hpp"

/**
 * @brief Function to create suffix array from WordVector
 * \file suffixArray.hpp
 *
 *  suffix array creation is not linear yet (not the best implementation beside
 *  the scope of the project)
 *
 */

namespace unialgo {
namespace pattern {

/**
 * @brief Construct a new Suffix Array object
 * return sa (sa[i] index where the i-th suffix start)
 *
 * Important: this also checks that the wv is $ terminated
 * (it is required that the last word is the smalles in the
 * alphabet order and unique)
 *
 * @param wv reference to construct sa from
 */
utils::WordVector makeSuffixArray(const utils::WordVector& vw);

}  // namespace pattern
}  // namespace unialgo

#endif  // UNIALGO_PATTERN_SUFFIXARRAY_