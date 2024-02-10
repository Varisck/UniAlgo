#ifndef UNIALGO_PATTERN_WORDVECMATCHING_
#define UNIALGO_PATTERN_WORDVECMATCHING_

#include <unordered_map>

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

}  // namespace pattern
}  // namespace unialgo

#endif  // UNIALGO_PATTERN_WORDVECMATCHING_