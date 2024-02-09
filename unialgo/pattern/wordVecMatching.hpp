#ifndef UNIALGO_PATTERN_WORDVECMATCHING_
#define UNIALGO_PATTERN_WORDVECMATCHING_

#include <unordered_map>

#include "unialgo/pattern/stringMatching.hpp"
#include "unialgo/utils/bitvector/bitVectors.hpp"

/**
 * @brief Implementation of pattern matching with std::string
 * \file wordVecMatching.hpp
 *
 * This file contains definition of exact string matching functions using
 * bitVector to rapresent strings, including convertion functions
 * from std::string to unialgo::utils::wordVector
 *
 *
 * all the string matching algo are in the unialgo::pattern namespace
 *
 *
 */

namespace unialgo {
namespace pattern {

// associate to chars in alphabet value in lexicological order
using Alphabet = std::unordered_map<char, uint8_t>;

/**
 * @brief Get the map of alphabet
 *
 * @param s string
 * @return std::unordered_map<char, uint8_t> map with unique char -> position in
 * alphabet order
 */
Alphabet GetAlphabet(std::string s);

/**
 * @brief Get bitVector rapresentation of string s
 *
 * @param s string to convert
 * @return utils::WordVector string s rapresented as bitVector
 */
utils::WordVector StringToBitVector(std::string s);

/**
 * @brief Get bitVector rapresentation of string s based on given alphabet
 *
 * @param s string to convert
 * @param std::unordered_map<char, uint8_t> alphabet to use on convertion
 * @return utils::WordVector string s rapresented as bitVector
 */
utils::WordVector StringToBitVector(std::string s, Alphabet alphabet);

template <typename T>
pattern::TransitionFunction<T> MakeTransitionFunctionT(T& p) {
  TransitionFunction<T> transition;
  size_t unique_char = 0;  // unique characters in pattern

  // creates entrys unique_char -> col in tranision function
  for (size_t i = 0; i < p.size(); ++i) {
    if (!transition.lookup.contains(p[i])) {
      transition.lookup.emplace(std::make_pair(p[i], unique_char));
      ++unique_char;
    }
  }

  // creates table of transitino function of size p.size() + 1 * unique_char
  transition.tf = std::vector<std::vector<std::size_t>>(p.size() + 1);
  transition.tf[0] = std::vector<std::size_t>(unique_char, 0);

  for (size_t i = 1; i <= p.size(); ++i) {
    std::size_t k = transition.tf[i - 1][transition.lookup.at(p[i - 1])];
    transition.tf[i - 1][transition.lookup.at(p[i - 1])] = i;
    // copy line k to line i
    transition.tf[i] = transition.tf[k];
  }
  return transition;
}

template <typename T>
std::vector<std::size_t> FsaT(T& t, T& p) {
  TransitionFunction<T> tf = MakeTransitionFunctionT(p);
  std::size_t state = 0;
  std::vector<std::size_t> occurrences;

  for (std::size_t i = 0; i < t.size(); ++i) {
    if (tf.lookup.contains(t[i])) {
      state = tf.tf[state][tf.lookup.at(t[i])];
    } else {
      state = 0;
    }
    if (state == p.size()) {
      occurrences.emplace_back(i - p.size() + 1);
    }
  }
  return occurrences;
}

}  // namespace pattern
}  // namespace unialgo

#endif  // UNIALGO_PATTERN_WORDVECMATCHING_