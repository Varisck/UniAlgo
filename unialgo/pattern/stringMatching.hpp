#ifndef UNIALGO_STRINGMATCHING_
#define UNIALGO_STRINGMATCHING_

#include <string>
#include <unordered_map>
#include <vector>

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
 * @brief Find occurrences of pattern p in text t
 *
 * @details this function implements a finite state automata algorithm
 *
 * @param t text
 * @param p pattern
 * @return std::vector<std::size_t> vector containing occurrences
 */
std::vector<std::size_t> Fsa(std::string t, std::string p);

/**
 * @brief This functino creates the transition function for the automata in FSA
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