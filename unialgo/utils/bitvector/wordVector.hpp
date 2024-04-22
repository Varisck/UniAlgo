#ifndef UNIALGO_UTILS_WORDVECTOR_
#define UNIALGO_UTILS_WORDVECTOR_

#include <stdint.h>  // uint64_t

#include <cmath>       // std::ceil, std::floor
#include <functional>  // std::hash
#include <iostream>
#include <ostream>      // ostream
#include <type_traits>  // std::enable_if, std::is_arithmetic
#include <utility>      // std::swap
#include <vector>

#include "unialgo/utils/bitvector/bitMaps.hpp"

// TODO: review all members of WordVectorRef (possibly templated members are not
// needed)
// TODO: add cassert for incorrect access?, word_size > 64

namespace unialgo {
namespace utils {

// forward decleration
template <typename Reference_type>
class WordVectorRef;

// defined at end of file:
// using WordVectorReference = WordVector::Reference;
// using WordVectorConstReference = WordVector::ConstReference;

/**
 * @brief Class WordVector
 * implements a bitvector class with custom word bit size
 */
class WordVector {
 public:
  using Type = uint64_t;                    // Type used in vector
  using reference_type = Type;              // type of class WordVectorRef
  using const_reference_type = const Type;  // type of const class WordVectorRef
  using Reference =
      WordVectorRef<reference_type>;  // reference to bit in vector
  using ConstReference =
      WordVectorRef<const_reference_type>;  // const reference to bit in
                                            // vector
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
  class TempIterator;

  using Iterator = TempIterator<Reference, WordVector*>;
  using ConstIterator = TempIterator<ConstReference, const WordVector*>;

  Iterator begin();
  Iterator end();

  ConstIterator cbegin() const;
  ConstIterator cend() const;

 private:
  std::vector<Type> bits_;  // vector of bits
  uint8_t word_size_;       // size of a single word to store
  std::size_t num_words_;   // number of words in bits_
};

template <typename Reference_type>
class WordVectorRef {
 public:
  using value_type = Reference_type;

  /**
   * @brief Construct a reference from non const pointer (non const WordVector)
   *
   * @param value poiter to word where value to reference is stored
   * @param position position of bits in word
   * @param word_size size of referenced word in WordVector
   */
  WordVectorRef(Reference_type* value, std::size_t position, uint8_t word_size)
      : word_size_(word_size), value_(value), position_(position) {}

  WordVectorRef() : word_size_(0), value_(nullptr), position_(0) {}

  WordVectorRef(const WordVectorRef& ref)
      : word_size_(ref.word_size_),
        value_(ref.value_),
        position_(ref.position_) {}

  template <typename U,
            typename = std::enable_if_t<std::is_const_v<Reference_type>, U>>
  WordVectorRef(const WordVectorRef<U>& ref)
      : word_size_(ref.getWordSize()),
        value_(ref.getValuePointer()),
        position_(ref.getPosition()) {}

  WordVectorRef(WordVectorRef&& ref)
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
  template <typename T, typename = std::enable_if<
                            std::is_arithmetic<T>::value &&
                                !std::is_const<typename std::remove_reference<
                                    Reference_type>::type>::value,
                            T>::type>
  WordVectorRef& operator=(const T& value) {
    utils::write_bits(value_, value, position_, word_size_);
    return *this;
  }

  /**
   * @brief Used to change word value at position_ in the referenced value
   * inside wordVector
   *
   * @param value value to set the word to the value
   * @return WordVectorRef& with word changed
   */
  template <typename T, typename = std::enable_if<
                            std::is_arithmetic<T>::value &&
                                !std::is_const<typename std::remove_reference<
                                    Reference_type>::type>::value,
                            T>::type>
  WordVectorRef& operator=(T&& value) {
    utils::write_bits(value_, value, position_, word_size_);
    return *this;
  }

  // this needs reviewing
  WordVectorRef& operator=(const WordVectorRef& other) {
    if constexpr (!std::is_const_v<Reference_type>) {
      utils::write_bits(value_, other.getValue(), position_, word_size_);
    }
    return *this;
  }

  WordVectorRef& operator=(WordVectorRef&& other) {
    utils::write_bits(value_, other.getValue(), position_, word_size_);
    return *this;
  }

  void changeReference(WordVectorRef&& other) {
    word_size_ = std::move(other.getWordSize());
    value_ = std::move(other.getValuePointer());
    position_ = std::move(other.getPosition());
  }

  void swap(WordVectorRef& other) {
    WordVector::Type tmp = getValue();
    utils::write_bits(value_, other.getValue(), position_, word_size_);
    utils::write_bits(other.value_, tmp, other.position_, other.word_size_);
  }

  /**
   * @brief Get the Value of referenced value
   *
   * @return const WordVector::Type
   */
  const WordVector::Type getValue() const {
    return utils::read_bits(value_, position_, word_size_);
  }

  uint8_t getWordSize() const { return word_size_; }

  std::size_t getPosition() const { return position_; }

  Reference_type* getValuePointer() const { return value_; }

  /**
   * @brief Prefix increment operator (++a)
   */
  template <typename = typename std::enable_if<!std::is_const<
                typename std::remove_reference<Reference_type>::type>::value>>
  WordVectorRef<Reference_type>& operator++() {
    auto value = (*this).getValue();
    utils::write_bits(value_, value + 1, position_, word_size_);
    return *this;
  }

  /**
   * @brief Postfix increment operator (a++)
   *
   * @return const WordVector::Type value before increment
   */
  template <typename = typename std::enable_if<!std::is_const<
                typename std::remove_reference<Reference_type>::type>::value>>
  const WordVector::Type operator++(int) {
    auto value = (*this).getValue();
    ++(*this);
    return value;
  }

  /**
   * @brief Prefix decrement operator
   */
  template <typename = typename std::enable_if<!std::is_const<
                typename std::remove_reference<Reference_type>::type>::value>>
  WordVectorRef<Reference_type>& operator--() {
    auto value = (*this).getValue();
    utils::write_bits(value_, value - 1, position_, word_size_);
    return *this;
  }

  /**
   * @brief Postfix decrement operator
   *
   * @return const WordVector::Type value before decrement
   */
  template <typename = typename std::enable_if<!std::is_const<
                typename std::remove_reference<Reference_type>::type>::value>>
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

  // operator int() const { return getValue(); }
  operator Reference_type() const { return getValue(); }
  // operator WordVector::Type() const { return getValue(); }

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
  uint8_t word_size_;      // size of a single word to reference
  std::size_t position_;   // position where the referenced word starts
  Reference_type* value_;  // full word containing ref value
};

// reconsider the iterator class (?)
template <typename Ref_iterator, typename Wordvector_pointer>
class WordVector::TempIterator {
 public:
  using iterator_category = std::random_access_iterator_tag;
  using difference_type = std::ptrdiff_t;
  // using difference_type = long;
  using value_type = Ref_iterator::value_type;
  using pointer = Ref_iterator*;
  using reference = Ref_iterator&;

  TempIterator(std::size_t bit_pos, Wordvector_pointer bv)
      : bit_pos_(bit_pos), wv_(bv), ref_to_((*bv)[bit_pos]) {}

  TempIterator& operator=(const TempIterator& other) {
    bit_pos_ = other.bit_pos_;
    wv_ = other.wv_;
    ref_to_.changeReference((*other.wv_)[other.bit_pos_]);
    return *this;
  }

  // check this may be optimized
  void resetReference() { ref_to_.changeReference((*wv_)[bit_pos_]); }

  reference operator*() {
    resetReference();
    return ref_to_;
  }
  pointer operator->() {
    resetReference();
    return &ref_to_;
  }

  TempIterator& operator++() {
    bit_pos_++;
    return *this;
  }

  TempIterator operator++(int) {
    TempIterator tmp = *this;
    ++(*this);
    return tmp;
  }

  TempIterator& operator--() {
    bit_pos_--;
    return *this;
  }

  TempIterator operator--(int) {
    TempIterator tmp = *this;
    --(*this);
    return tmp;
  }

  TempIterator& operator+=(difference_type n) {
    bit_pos_ += n;
    return *this;
  }

  TempIterator& operator-=(difference_type n) {
    bit_pos_ -= n;
    return *this;
  }

  friend bool operator==(const TempIterator& a, const TempIterator& b) {
    return a.wv_ == b.wv_ && a.bit_pos_ == b.bit_pos_;
  }

  friend bool operator!=(const TempIterator& a, const TempIterator& b) {
    return !(a == b);
  }

  friend bool operator>(const TempIterator& a, const TempIterator& b) {
    return a.bit_pos_ > b.bit_pos_;
  }

  friend bool operator<(const TempIterator& a, const TempIterator& b) {
    return a.bit_pos_ < b.bit_pos_;
  }

  friend bool operator<=(const TempIterator& a, const TempIterator& b) {
    return !(a.bit_pos_ > b.bit_pos_);
  }

  friend bool operator>=(const TempIterator& a, const TempIterator& b) {
    return !(a.bit_pos_ < b.bit_pos_);
  }

  friend difference_type operator-(const TempIterator& a,
                                   const TempIterator& b) {
    std::cout << "operator- between " << a.bit_pos_ << " and " << b.bit_pos_
              << " res is " << a.bit_pos_ - b.bit_pos_ << std::endl;
    return a.bit_pos_ - b.bit_pos_;
  }

  friend TempIterator operator+(const TempIterator& it, difference_type n) {
    return TempIterator(it.bit_pos_ + n, it.wv_);
  }

  friend TempIterator operator+(difference_type n, const TempIterator& it) {
    return it + n;
  }

  friend TempIterator operator-(const TempIterator& it, difference_type n) {
    return TempIterator(it.bit_pos_ - n, it.wv_);
  }

  friend TempIterator operator-(difference_type n, const TempIterator& it) {
    return it - n;
  }

 private:
  std::size_t bit_pos_;    // position of bit in bv
  Wordvector_pointer wv_;  // pointer to wv for iterator
                           // WordVector::Reference
  Ref_iterator ref_to_;    // reference object of value pointer by iterator
};

inline WordVector::Iterator WordVector::begin() {
  return WordVector::Iterator(0, this);
}

inline WordVector::Iterator WordVector::end() {
  return WordVector::Iterator(num_words_, this);
}

inline WordVector::ConstIterator WordVector::cbegin() const {
  return WordVector::ConstIterator(0, this);
}

inline WordVector::ConstIterator WordVector::cend() const {
  return WordVector::ConstIterator(num_words_, this);
}

// swap two references of wordVector
template <typename Reference_type>
inline void swap(unialgo::utils::WordVectorRef<Reference_type>& a,
                 unialgo::utils::WordVectorRef<Reference_type>& b) {
  a.swap(b);
}
// swap two iterators of wordVector
inline void iter_swap(unialgo::utils::WordVector::Iterator a,
                      unialgo::utils::WordVector::Iterator b) {
  std::swap(*a, *b);
}

using WordVectorReference = WordVector::Reference;
using WordVectorConstReference = WordVector::ConstReference;

}  // namespace utils
}  // namespace unialgo

namespace std {

/**
 * @brief Hashing function for reference to wordVector
 */
template <typename Reference_type>
struct hash<unialgo::utils::WordVectorRef<Reference_type>> {
  // for Reference
  std::size_t operator()(
      const unialgo::utils::WordVectorRef<Reference_type>& ref) const {
    return std::hash<unialgo::utils::WordVector::reference_type>()(
        ref.getValue());
  }
};

// swap two references of wordVector
// this should not be overloaded inside std (remove this sooner or later)
template <typename Reference_type>
inline void swap(unialgo::utils::WordVectorRef<Reference_type>& a,
                 unialgo::utils::WordVectorRef<Reference_type>& b) {
  a.swap(b);
}

// swap two iterators of wordVector
// this should not be overloaded inside std (remove this sooner or later)
inline void iter_swap(unialgo::utils::WordVector::Iterator a,
                      unialgo::utils::WordVector::Iterator b) {
  std::swap(*a, *b);
}

}  // namespace std

#endif  // UNIALGO_UTILS_WORDVECTOR_