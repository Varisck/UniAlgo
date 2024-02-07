#ifndef UNIALGO_UTILS_WORDVECTOR_
#define UNIALGO_UTILS_WORDVECTOR_

#include <stdint.h>  // uint64_t

#include <cmath>  // ceil, floor
#include <iostream>
#include <ostream>      // ostream
#include <type_traits>  // std::enable_if std::is_arithmetic
#include <vector>

#include "unialgo/utils/bitvector/bitMaps.hpp"

// TODO: add cassert for incorrect access?, word_size > 64

namespace unialgo {
namespace utils {

// forward decleration
template <uint8_t word_size>
class WordVectorRef;

/**
 * @class WordVector
 * implements a bitvector class with custom word bit size
 *
 * @tparam word_size size of word in vector (max val: 64)
 */
template <uint8_t word_size>
class WordVector {
 public:
  using Type = uint64_t;                       // Type used in vector
  using Reference = WordVectorRef<word_size>;  // reference to bit in vector
  static const std::size_t type_size = sizeof(Type) * 8;  // size of Type

  /**
   * @brief Construct a new Word Vector object
   *
   * @param num_words num words to store in vector
   * @param wordSize size of a word (default = word_size)
   */
  WordVector(std::size_t num_words, Type wordSize = word_size);

  /**
   * @brief Construct a new Word Vector object
   * default wordSize = word_size
   * default num_word = 0
   */
  WordVector();

  /**
   * @brief Accessing word in WordVector
   *
   * @param pos position of word to access
   * @return Reference to word at position pos in WordVector
   */
  Reference operator[](std::size_t pos);

  /**
   * @brief Get the number of bits in vector (num_words_ * word_size_)
   *
   * @return std::size_t number of bits in vector
   */
  std::size_t getNumBits();

 private:
  std::vector<Type> bits_;  // vector of bits
  uint8_t word_size_;       // size of a single word to store
  std::size_t num_words_;   // number of words in bits_
};

template <uint8_t word_size>
class WordVectorRef {
 public:
  WordVectorRef(WordVector<word_size>::Type& value, std::size_t position)
      : word_size_(word_size), value_(value), position_(position) {}

  /**
   * @brief Used to change word value at position_ in the referenced value
   * inside wordVector
   *
   * @param value value to set the word to the value
   * @return WordVectorRef& with word changed
   */
  template <typename T, typename = typename std::enable_if<
                            std::is_arithmetic<T>::value, T>::type>
  WordVectorRef& operator=(const T& value) {
    utils::write_bits(&value_, value, position_, word_size_);
    return *this;
  }

  const WordVector<word_size>::Type getValue() const {
    return utils::read_bits(&value_, position_, word_size_);
  }

  /**
   * @brief Comparison operator between WordVectorRefef and arithmetic type
   * @return true if bit value referenced by wvRef == to value
   * @return false otherwise
   */
  template <typename T, typename = typename std::enable_if<
                            std::is_arithmetic<T>::value, T>::type>
  friend inline bool operator==(const WordVectorRef& lhs, const T& value) {
    return lhs.getValue() == value;
  }

  /**
   * @brief Comparison operator between two WordVectorRef
   * @return true if bit values referenced by lhs == bit values referenced by
   * rhs
   * @return false otherwise
   */
  friend inline bool operator==(const WordVectorRef& lhs,
                                const WordVectorRef& rhs) {
    return lhs.getValue() == rhs.getValue();
  }

 private:
  uint8_t word_size_;     // size of a single word to reference
  std::size_t position_;  // position where the referenced word starts
  WordVector<word_size>::Type& value_;  // full word containing ref value
};

// ============ Implementation WordVector ============

template <uint8_t word_size>
WordVector<word_size>::WordVector()
    : num_words_(0), word_size_(word_size), bits_(0) {}

template <uint8_t word_size>
WordVector<word_size>::WordVector(std::size_t num_words,
                                  WordVector::Type wordSize)
    : num_words_(num_words),
      word_size_(word_size),
      bits_(
          std::ceil((num_words * word_size) / static_cast<double>(type_size))) {
}

template <uint8_t word_size>
std::size_t WordVector<word_size>::getNumBits() {
  return num_words_ * word_size_;
}

template <uint8_t word_size>
WordVector<word_size>::Reference WordVector<word_size>::operator[](
    std::size_t pos) {
  return WordVectorRef<word_size>(
      bits_[std::floor((pos * word_size_) / type_size)],
      ((pos * word_size_) % type_size));
}

}  // namespace utils
}  // namespace unialgo

#endif  // UNIALGO_UTILS_WORDVECTOR_