#ifndef UNIALGO_PATTERN_SUFFIXARRAY_
#define UNIALGO_PATTERN_SUFFIXARRAY_

#include <string>  // std::string

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

using wv_ref = utils::WordVectorReference;

/**
 * @brief Construct a new Suffix Array object
 * return sa (sa[i] index where the i-th suffix start)
 *
 * @details This also checks that the wv is $ terminated
 * (it is required that the last word is the smalles in the
 * alphabet order and unique)
 *
 * @param wv reference to construct sa from
 */
utils::WordVector makeSuffixArray(const utils::WordVector& vw);

/**
 * @brief Returns a WordVector from a string
 *
 * @attention The string is should not be "$"-terminated the function takes care
 * of this and laters calls unialgo::pattern::make_suffix_array
 *
 * @param s const reference to string
 * @return utils::WordVector Suffix Array
 */
utils::WordVector suffix_array_from_string(const std::string& s);

/**
 * @brief Check lexicographic order for pairs
 *
 * @param a1 first value of pair 1
 * @param a2 second value of pair 1
 * @param b1 first value of pair 2
 * @param b2 second value of pair 2
 * @return true a <= b
 * @return false otherwise
 */
inline bool lex_eq(wv_ref a1, wv_ref a2, wv_ref b1, wv_ref b2) {
  return (a1 < b1 || a1 == b1 && a2 <= b2);
}

/**
 * @brief Check lexicographic order for triples
 *
 * @return true a <= b
 * @return false otherwise
 */
inline bool lex_eq(wv_ref a1, wv_ref a2, wv_ref a3, wv_ref b1, wv_ref b2,
                   wv_ref b3) {
  return (a1 < b1 || a1 == b1 && lex_eq(a2, a3, b2, b3));
}

/**
 * @brief Stable sort of a[0...n-1] into b[0...n-1] with values in range 0...k
 * found inside r
 *
 * @param a positions to sort
 * @param b result of sorting
 * @param r keys inside here
 * @param n size of a and b arrays
 * @param offset is the offset for accessing r (i.e. if offset = 1 r[0] inside
 * the function will be r[0 + 1] from the caller)
 */
inline void radix_pass(utils::WordVector& a, utils::WordVector& b,
                       utils::WordVector& r, std::size_t n, std::size_t k,
                       int offset = 0) {
  utils::WordVector occ(k + 1, utils::get_log_2(n));
  for (std::size_t i = 0; i < n; ++i)
    occ[r[a[i] + offset]]++;                       // count occurrences
  for (std::size_t i = 0, sum = 0; i <= k; ++i) {  // exclusive prefix sums
    std::size_t t = occ[i];
    occ[i] = sum;
    sum += t;
  }
  for (std::size_t i = 0; i < n; ++i)
    b[occ[r[a[i] + offset]]++] = a[i];  // sort
}

/**
 * @brief Suffix array construction in linear time
 *
 * @attention This requires that string[n] = string[n + 1] = string[n + 2] = 0
 *
 * @details time complexity: O(n). This is from the paper: "Simple Linear Work
 * Suffix Array Construction" by Juha Karkkainen and Peter Sanders
 *
 * @param string string to create suffix array from
 * @param suffix_array reference to suffix array
 * @param n size of original string (not including s[n], s[n + 1] ...)
 * @param K range of values in string ((1 << (string.getWordSize() + 1)) - 1)
 */
inline void make_suffix_array(utils::WordVector& string,
                              utils::WordVector& suffix_array, std::size_t n,
                              std::size_t K) {
  std::size_t n0 = (n + 2) / 3;
  std::size_t n1 = (n + 1) / 3;
  std::size_t n2 = (n) / 3;
  std::size_t n02 = n0 + n2;  // size for array prefix of mod 1, 2

  // vector for suffix mod 1, 2
  utils::WordVector suffix12(n02 + 3, utils::get_log_2(n));
  suffix12[n02] = 0;
  suffix12[n02 + 1] = 0;
  suffix12[n02 + 2] = 0;
  // suffix array of suffixes mod 1, 2
  utils::WordVector sa12(n02 + 3, utils::get_log_2(n));
  sa12[n02] = 0;
  sa12[n02 + 1] = 0;
  sa12[n02 + 2] = 0;
  // suffix mod 0
  utils::WordVector suffix0(n0, utils::get_log_2(n));
  // suffix array for suffixes mod 0
  utils::WordVector sa0(n0, utils::get_log_2(n));
  // generates the suffix mod 1, 2 positions
  for (std::size_t i = 0, j = 0; i < n + (n0 - n1); i++)
    if (i % 3 != 0) suffix12[j++] = i;

  // lsb radix sort the mod 1 and mod 2 triples
  radix_pass(suffix12, sa12, string, n02, K, 2);
  radix_pass(sa12, suffix12, string, n02, K, 1);
  radix_pass(suffix12, sa12, string, n02, K, 0);

  // find lexicographic names of triples
  int name = 0, c0 = -1, c1 = -1, c2 = -1;
  for (int i = 0; i < n02; i++) {
    if (string[sa12[i]] != c0 || string[sa12[i] + 1] != c1 ||
        string[sa12[i] + 2] != c2) {
      name++;
      c0 = string[sa12[i]];
      c1 = string[sa12[i] + 1];
      c2 = string[sa12[i] + 2];
    }
    if (sa12[i] % 3 == 1) {
      suffix12[sa12[i] / 3] = name;
    }  // left half
    else {
      suffix12[sa12[i] / 3 + n0] = name;
    }  // right half
  }

  // recurse if names are not yet unique
  if (name < n02) {
    make_suffix_array(suffix12, sa12, n02, name);
    // store unique names in s12 using the suffix array
    for (int i = 0; i < n02; i++) suffix12[sa12[i]] = i + 1;
  } else  // generate the suffix array of s12 directly
    for (int i = 0; i < n02; i++) sa12[suffix12[i] - 1] = i;

  // stably sort the mod 0 suffixes from SA12 by their first character
  for (int i = 0, j = 0; i < n02; i++)
    if (sa12[i] < n0) suffix0[j++] = 3 * sa12[i];

  radix_pass(suffix0, sa0, string, n0, K, 0);

  // merge sorted SA0 suffixes and sorted SA12 suffixes
  for (int p = 0, t = n0 - n1, k = 0; k < n; k++) {
#define GetI() (sa12[t] < n0 ? sa12[t] * 3 + 1 : (sa12[t] - n0) * 3 + 2)
    int i = GetI();  // pos of current offset 12 suffix
    int j = sa0[p];  // pos of current offset 0 suffix
    if (sa12[t] < n0
            ?  // different compares for mod 1 and mod 2 suffixes
            lex_eq(string[i], suffix12[sa12[t] + n0], string[j],
                   suffix12[j / 3])
            : lex_eq(string[i], string[i + 1], suffix12[sa12[t] - n0 + 1],
                     string[j], string[j + 1],
                     suffix12[j / 3 + n0])) {  // suffix from SA12 is smaller
      suffix_array[k] = i;
      t++;
      if (t == n02)  // done --- only SA0 suffixes left
        for (k++; p < n0; p++, k++) suffix_array[k] = sa0[p];
    } else {  // suffix from SA0 is smaller
      suffix_array[k] = j;
      p++;
      if (p == n0)  // done --- only SA12 suffixes left
        for (k++; t < n02; t++, k++) suffix_array[k] = GetI();
    }
  }
}

}  // namespace pattern
}  // namespace unialgo

#endif  // UNIALGO_PATTERN_SUFFIXARRAY_