#ifndef UNIALGO_STRINGMATCHING_
#define UNIALGO_STRINGMATCHING_

#include <string>
#include <unordered_map>
#include <vector>

#include "unialgo/utils/bitvector/bitVectors.hpp"

/**
 * @brief Implementation of pattern matching with std::string
 * \file stringMatching.hpp
 *
 * this file contains implementatino of exact string matching on std::string
 * all the string matching algo are in the unialgo::pattern namespace
 */

namespace unialgo {

namespace pattern {

// Used to containt transition function for finate state automata
struct TransitionFunction {
  std::vector<std::vector<std::size_t>> tf;      // transition function
  std::unordered_map<char, std::size_t> lookup;  // loockup table char -> col
};

/**
 * @brief Get map unique char -> occurrence in string s
 *
 * @param s
 * @return std::unordered_map<char, std::size_t> unique_char -> occurrence
 */
std::unordered_map<char, std::size_t> AlphabetSize(std::string s);

/**
 * @brief Get bitVector rapresentation of string s
 *
 * @param s string to convert
 * @return utils::WordVector string s rapresented as bitVector
 */
utils::WordVector StringToBitVector(std::string s);

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
 * @return std::vector<std::size_t> vector containing occurrences
 */
std::vector<std::size_t> Fsa(std::string t, std::string p);

// TODO: implement Fsa with transitionFunction as argument (reuse of transition
// function for same pattern)

/**
 * @brief This function creates the transition function for the automata in FSA
 *
 * Time Complexity: O(m * |Sigma|) where:
 *    Sigma = unique_chars in pattern p
 *    m size of pattern p
 *    the |Sigma| comes from std::vector operator= Linear in size
 *
 * @param p pattern
 * @return TransitionFuction transition
 * function
 */

TransitionFunction MakeTransitionFunction(std::string p);

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
 * @brief Builds the prefix functino for kmp (aka failure function)
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

}  // namespace pattern

}  // namespace unialgo

#endif  // UNIALGO_STRINGMATCHING_