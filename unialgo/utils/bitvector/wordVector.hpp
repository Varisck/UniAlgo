#ifndef UNIALGO_UTILS_WORDVECTOR_
#define UNIALGO_UTILS_WORDVECTOR_

#include <stdint.h>  // uint64_t

#include <cmath>       // ceil, floor
#include <functional>  //std::hash
#include <iostream>
#include <ostream>      // ostream
#include <type_traits>  // std::enable_if std::is_arithmetic
#include <vector>

#include "unialgo/utils/bitvector/bitMaps.hpp"

// TODO: add cassert for incorrect access?, word_size > 64

namespace unialgo {
namespace utils {

// forward decleration
template <typename T>
class WordVectorRef;

/**
 * @class WordVector
 * implements a bitvector class with custom word bit size
 */
class WordVector {
 public:
  using Type = uint64_t;                  // Type used in vector
  using Reference = WordVectorRef<Type>;  // reference to bit in vector
  using ConstReference =
      const WordVectorRef<const Type>;  // const reference to bit in vector
  using value_type = std::size_t;
  static const std::size_t type_size = sizeof(Type) * 8;  // size of Type

  /**
   * @brief Construct a new Word Vector object
   *
   * @param num_words num words to store in vector
   * @param wordSize size of a word (default = 2)
   */
  WordVector(std::size_t num_words, Type wordSize = 2);

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
   * @brief Accessing word in WordVector
   *
   * @param pos position of word to access
   * @return Reference to word at position pos in WordVector
   */
  ConstReference operator[](std::size_t pos) const;

  /**
   * @brief Get the number of bits in vector (num_words_ * word_size_)
   *
   * @return std::size_t number of bits in vector
   */
  std::size_t getNumBits() const;

  /**
   * @brief Size of vector (num_words_ inside vector)
   *
   * @return std::size_t size of vector
   */
  std::size_t size() const;

  /**
   * @brief Get the size of words in vector
   *
   * @return uint8_t size of words
   */
  uint8_t getWordSize() const;

 private:
  std::vector<Type> bits_;  // vector of bits
  uint8_t word_size_;       // size of a single word to store
  std::size_t num_words_;   // number of words in bits_
};

// template on type may solve the issue (????)
template <typename RT>
class WordVectorRef {
 public:
  WordVectorRef(RT* value, std::size_t position, uint8_t word_size)
      : word_size_(word_size), value_(value), position_(position) {}

  WordVectorRef() : word_size_(0), value_(nullptr), position_(0) {}

  WordVectorRef(const WordVectorRef& ref)
      : word_size_(ref.word_size_),
        value_(ref.value_),
        position_(ref.position_) {}

  WordVectorRef(const WordVectorRef&& ref)
      : word_size_(std::move(ref.word_size_)),
        value_(std::move(ref.value_)),
        position_(std::move(ref.position_)) {}

  /**
   * @brief Used to change word value at position_ in the referenced value
   * inside wordVector
   *
   * @param value value to set the word to the value
   * @return WordVectorRef& with word changed
   */
  template <typename T, typename = typename std::enable_if<
                            std::is_arithmetic<T>::value, T>::type>
  WordVectorRef<RT>& operator=(const T& value) {
    utils::write_bits(value_, value, position_, word_size_);
    return *this;
  }

  /**
   * @brief Get the Value of referenced value
   *
   * @return const WordVector<word_size>::Type
   */
  const RT getValue() const {
    return utils::read_bits(value_, position_, word_size_);
  }

  /**
   * @brief Prefix increment operator (++a)
   */
  WordVectorRef& operator++() {
    auto value = (*this).getValue();
    utils::write_bits(value_, value + 1, position_, word_size_);
    return *this;
  }

  /**
   * @brief Postfix increment operator (a++)
   *
   * @return const WordVector<word_size>::Type value before increment
   */
  const RT operator++(int) {
    auto value = (*this).getValue();
    ++(*this);
    return value;
  }

  /**
   * @brief Prefix decrement operator
   */
  WordVectorRef& operator--() {
    auto value = (*this).getValue();
    utils::write_bits(value_, value - 1, position_, word_size_);
    return *this;
  }

  /**
   * @brief Postfix decrement operator
   *
   * @return const WordVector<word_size>::Type value before decrement
   */
  const RT operator--(int) {
    auto value = (*this).getValue();
    --(*this);
    return value;
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

  /**
   * @brief Comparison operator between two WordVectorRef
   */
  friend inline bool operator!=(const WordVectorRef& lhs,
                                const WordVectorRef& rhs) {
    return !(lhs == rhs);
  }

  /**
   * @brief Comparison operator between two WordVectorRef
   * @return true if bit values referenced by lhs > bit values referenced by
   * rhs
   * @return false otherwise
   */
  friend inline bool operator>(const WordVectorRef& lhs,
                               const WordVectorRef& rhs) {
    return lhs.getValue() > rhs.getValue();
  }

  /**
   * @brief Comparison operator between two WordVectorRef
   * @return true if bit values referenced by lhs < bit values referenced by
   * rhs
   * @return false otherwise
   */
  friend inline bool operator<(const WordVectorRef& lhs,
                               const WordVectorRef& rhs) {
    return lhs.getValue() < rhs.getValue();
  }

  /**
   * @brief Comparison operator between two WordVectorRef
   * @return true if bit values referenced by lhs >= bit values referenced by
   * rhs
   * @return false otherwise
   */
  friend inline bool operator>=(const WordVectorRef& lhs,
                                const WordVectorRef& rhs) {
    return lhs.getValue() >= rhs.getValue();
  }

  /**
   * @brief Comparison operator between two WordVectorRef
   * @return true if bit values referenced by lhs < bit values referenced by
   * rhs
   * @return false otherwise
   */
  friend inline bool operator<=(const WordVectorRef& lhs,
                                const WordVectorRef& rhs) {
    return lhs.getValue() <= rhs.getValue();
  }

  /**
   * @brief Output to the stream the bit rapresentation of referenced value
   */
  friend std::ostream& operator<<(std::ostream& os, const WordVectorRef& ref) {
    auto bitValue = ref.getValue();
    for (int16_t i = ref.word_size_ - 1; i >= 0; --i)
      os << static_cast<bool>((bitValue & bit_set[i]));
    return os;
  }

 private:
  const uint8_t word_size_;     // size of a single word to reference
  const std::size_t position_;  // position where the referenced word starts
  RT* value_;                   // full word containing ref value
};

using WordVectorReference = WordVector::Reference;

}  // namespace utils
}  // namespace unialgo

/**
 * @brief Hashing function for reference to wordVector
 */
template <typename T>
struct std::hash<typename unialgo::utils::WordVectorRef<T>> {
  std::size_t operator()(const unialgo::utils::WordVectorRef<T>& ref) const {
    return std::hash<T>()(ref.getValue());
  }
};

#endif  // UNIALGO_UTILS_WORDVECTOR_