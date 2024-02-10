#ifndef UNIALGO_STRINGMATCHING_
#define UNIALGO_STRINGMATCHING_

#include <cassert>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility>  //declval
#include <vector>

#include "unialgo/utils/bitvector/bitVectors.hpp"

/**
 * @brief Implementation of pattern matching with std::string
 * \file stringMatching.hpp
 *
 * this file contains definition of exact string matching functions on
 * std::string
 *
 * all the string matching algo are in the unialgo::pattern namespace
 *
 * Fsa, Kmp, Byg
 */

namespace unialgo {

namespace pattern {

/**
 * @brief Used to containt transition function for finate state automata
 * @tparam T Type of the pattern like object (ex: std::string, std::vector<>,
 * unialgo::utils::wordVector)
 *
 */
template <typename T>
struct TransitionFunction {
  std::vector<std::vector<std::size_t>> tf;  // transition function for automata
  // loockup table maps single element in pattern to column of transition
  // function
  std::unordered_map<
      typename std::remove_reference<decltype(std::declval<T>()[std::declval<
          typename T::value_type>()])>::type,
      std::size_t>
      lookup;  // loockup table key is typeof T[] value: corresponding col in tf
};

/**
 * @brief This function creates the transition function for the automata in FSA
 *
 * Time Complexity: O(m * |Sigma|) where:
 *    Sigma = unique_chars in pattern p
 *    m size of pattern p
 *    the |Sigma| comes from std::vector operator= Linear in size
 *
 * @param p pattern
 * @tparam T type of string and pattern (default std::string)
 * @return TransitionFuction transition function to run fsa with pattern
 */
template <typename T = std::string>
TransitionFunction<T> MakeTransitionFunction(T& p);

/**
 * @brief Find occurrences of pattern p in text t using precalculated transition
 * function on pattern
 *
 * @details this function implements a finite state automata algorithm
 * this function calls pattern::MakeTransitionFunction and then scans the
 * input text.
 *
 * Time Complexity of search: O(n) + O(m * |Sigma|) where:
 *    n is size of text t linear scan
 *    m * |Sigma| is makeTransitionFunction call
 *
 * @param t text
 * @param p pattern
 * @param tf TransitionFunctino relative to pattern p
 * @tparam T type of string and pattern (default std::string)
 * @return std::vector<std::size_t> vector containing occurrences
 */
template <typename T = std::string>
std::vector<std::size_t> Fsa(T& t, T& p, TransitionFunction<T>& tf);

/**
 * @brief Find occurrences of pattern p in text t
 *
 * @details this function implements a finite state automata algorithm
 * this function calls pattern::MakeTransitionFunction and then scans the
 * input text.
 *
 * Time Complexity of search: O(n) + O(m * |Sigma|) where:
 *    n is size of text t linear scan
 *    m * |Sigma| is makeTransitionFunction call
 *
 * @param t text
 * @param p pattern
 * @tparam T type of string and pattern (default std::string)
 * @return std::vector<std::size_t> vector containing occurrences
 */
template <typename T = std::string>
std::vector<std::size_t> Fsa(T& t, T& p);

/**
 * @brief Find occurrences of pattern p in text t
 *
 * @details this function implements the KMP algorithm
 *
 * @param t text
 * @param p pattern
 * @return std::vector<std::size_t> vector containing occurrences
 */
std::vector<std::size_t> Kmp(std::string t, std::string p);

/**
 * @brief Builds the prefix function for kmp (aka failure function)
 *
 * Time Complexity:
 *
 * @param p
 * @return std::vector<std::size_t>
 */
std::vector<std::size_t> MakePrefixFunction(std::string p);

/**
 * @brief Find occurrences of pattern p in text t
 *
 * @details this function implements the BYG algorithm
 *
 * @param t text
 * @param p pattern
 * @return std::vector<std::size_t> vector containing occurrences
 */
std::vector<std::size_t> Byg(std::string t, std::string p);

// =============== Implementation ===============

template <typename T = std::string>
TransitionFunction<T> MakeTransitionFunction(T& p) {
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

template <typename T = std::string>
std::vector<std::size_t> Fsa(T& t, T& p, TransitionFunction<T>& tf) {
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

template <typename T = std::string>
std::vector<std::size_t> Fsa(T& t, T& p) {
  TransitionFunction<T> tf = MakeTransitionFunction(p);
  return Fsa<T>(t, p, tf);
}

// if type is wordvector make sure that pattern and text wordSize is matching
template <>
inline std::vector<std::size_t> Fsa<unialgo::utils::WordVector>(
    unialgo::utils::WordVector& t, unialgo::utils::WordVector& p,
    TransitionFunction<unialgo::utils::WordVector>& tf) {
  assert(t.getWordSize() == p.getWordSize() &&
         "word_size not matching for text and pattern");
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

#endif  // UNIALGO_STRINGMATCHING_