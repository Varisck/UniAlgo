#ifndef UNIALGO_UTILS_WORDVECTOR_
#define UNIALGO_UTILS_WORDVECTOR_

#include <stdint.h>  // uint64_t

#include <cmath>       // ceil, floor
#include <functional>  //std::hash
#include <iostream>
#include <ostream>      // ostream
#include <type_traits>  // std::enable_if, std::is_arithmetic
#include <vector>

#include "unialgo/utils/bitvector/bitMaps.hpp"

// TODO: add cassert for incorrect access?, word_size > 64

namespace unialgo {
namespace utils {

// forward decleration
class WordVectorRef;

// defined at end of file:
// using WordVectorReference = WordVector::Reference;
// using WordVectorConstReference = WordVector::ConstReference;

/**
 * @class WordVector
 * implements a bitvector class with custom word bit size
 */
class WordVector {
 public:
  using Type = uint64_t;                    // Type used in vector
  using reference_type = Type;              // type of class WordVectorRef
  using const_reference_type = const Type;  // type of const class WordVectorRef
  using Reference = WordVectorRef;          // reference to bit in vector
  using ConstReference =
      const WordVectorRef;  // const reference to bit in vector
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
   * @brief copy
   *
   * @param other
   */
  WordVector(const WordVector& other)
      : num_words_(other.num_words_),
        word_size_(other.word_size_),
        bits_(other.bits_) {}

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

  /**
   * @brief Output to the steam bits inside wordvector from left to right
   * (101....) wv[size]wv[size-1]...wv[0]
   *
   */
  friend std::ostream& operator<<(std::ostream& os, const WordVector& wv);

  template <typename Ref_iterator, typename Wordvector_pointer>
  class TempIterator {
   public:
    using iterator_category = std::forward_iterator_tag;
    // using difference_type = std::ptrdiff_t;
    using difference_type = long;
    using value_type = Ref_iterator;
    using pointer = Ref_iterator*;
    using reference = Ref_iterator;

    TempIterator(std::size_t bit_pos, Wordvector_pointer bv)
        : bit_pos_(bit_pos), wv_(bv) {}

    reference operator*() const { return (*wv_)[bit_pos_]; }
    pointer operator->() { return &((*wv_)[bit_pos_]); }

    TempIterator& operator++() {
      bit_pos_++;
      return *this;
    }

    TempIterator operator++(int) {
      TempIterator tmp = *this;
      ++(*this);
      return tmp;
    }

    friend bool operator==(const TempIterator& a, const TempIterator& b) {
      return a.wv_ == b.wv_ && a.bit_pos_ == b.bit_pos_;
    }

    friend bool operator!=(const TempIterator& a, const TempIterator& b) {
      return !(a == b);
    }

   private:
    std::size_t bit_pos_;    // position of bit in bv
    Wordvector_pointer wv_;  // pointer to wv for iterator
  };

  using Iterator = TempIterator<Reference, WordVector*>;
  using ConstIterator = TempIterator<ConstReference, const WordVector*>;

  Iterator begin() { return Iterator(0, this); }
  Iterator end() { return Iterator(num_words_ - 1, this); }

  ConstIterator cbegin() const { return ConstIterator(0, this); }
  ConstIterator cend() const { return ConstIterator(num_words_ - 1, this); }

 private:
  std::vector<Type> bits_;  // vector of bits
  uint8_t word_size_;       // size of a single word to store
  std::size_t num_words_;   // number of words in bits_
};

class WordVectorRef {
 public:
  /**
   * @brief Construct a reference from non const pointer (non const WordVector)
   *
   * @param value poiter to word where value to reference is stored
   * @param position position of bits in word
   * @param word_size size of referenced word in WordVector
   */
  WordVectorRef(WordVector::reference_type* value, std::size_t position,
                uint8_t word_size)
      : word_size_(word_size),
        value_(value),
        position_(position),
        kValue_(*value) {}

  /**
   * @brief Construct a reference from const pointer (const WordVector)
   *
   * @param value poiter to word where value to reference is stored
   * @param position position of bits in word
   * @param word_size size of referenced word in WordVector
   */
  WordVectorRef(WordVector::const_reference_type* value, std::size_t position,
                uint8_t word_size)
      : position_(position),
        word_size_(word_size),
        kValue_(*value),
        value_(nullptr) {}

  WordVectorRef() : word_size_(0), value_(nullptr), position_(0), kValue_(0) {}

  WordVectorRef(const WordVectorRef& ref)
      : word_size_(ref.word_size_),
        value_(ref.value_),
        position_(ref.position_),
        kValue_(ref.kValue_) {}

  WordVectorRef(const WordVectorRef&& ref)
      : word_size_(std::move(ref.word_size_)),
        value_(std::move(ref.value_)),
        position_(std::move(ref.position_)),
        kValue_(std::move(ref.kValue_)) {}

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
    utils::write_bits(value_, value, position_, word_size_);
    return *this;
  }

  /**
   * @brief Get the Value of referenced value
   *
   * @return const WordVector::Type
   */
  const WordVector::Type getValue() const {
    if (value_ != nullptr)
      return utils::read_bits(value_, position_, word_size_);
    return utils::read_bits(&kValue_, position_, word_size_);
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
   * @return const WordVector::Type value before increment
   */
  const WordVector::Type operator++(int) {
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
   * @return const WordVector::Type value before decrement
   */
  const WordVector::Type operator--(int) {
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
  WordVector::const_reference_type kValue_;  // value for constant reference
  const uint8_t word_size_;     // size of a single word to reference
  const std::size_t position_;  // position where the referenced word starts
  WordVector::reference_type* value_;  // full word containing ref value
};

using WordVectorReference = WordVector::Reference;
using WordVectorConstReference = WordVector::ConstReference;

}  // namespace utils
}  // namespace unialgo

/**
 * @brief Hashing function for reference to wordVector
 */
template <>
struct std::hash<unialgo::utils::WordVectorRef> {
  // for Reference
  std::size_t operator()(const unialgo::utils::WordVectorRef& ref) const {
    return std::hash<unialgo::utils::WordVector::reference_type>()(
        ref.getValue());
  }
};

#endif  // UNIALGO_UTILS_WORDVECTOR_