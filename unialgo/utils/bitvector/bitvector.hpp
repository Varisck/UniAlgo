#ifndef UNIALGO_UTILS_BITVECTOR_
#define UNIALGO_UTILS_BITVECTOR_

#include <stdint.h>  // uint64_t

#include <iostream>     // std::cout
#include <iterator>     // std::forward_iterator_tag;
#include <ostream>      // std::ostream
#include <type_traits>  // std::enable_if, std::is_arithmetic
#include <vector>

#include "unialgo/utils/bitvector/bitMaps.hpp"

namespace unialgo {

namespace utils {

// forward decleration
class BitvectorReference;

/**
 * @brief Class Bitvector
 * a class implementing a vector of bits
 */
class Bitvector {
 public:
  using Type = uint64_t;                                  // type used in vector
  using Reference = BitvectorReference;                   // reference to bit
  using ConstReference = const BitvectorReference;        // const ref to bit
  static const std::size_t type_size = sizeof(Type) * 8;  // size of Type in bit

  /**
   * @brief Construct a new Bitvector object
   *
   * @param num_bits number of bits to store
   */
  Bitvector(std::size_t num_bits);

  /**
   * @brief Construct a new Bitvector object
   */
  Bitvector() : num_bits_(0), bits_(0) {}

  ~Bitvector() = default;

  Bitvector(const Bitvector&) = default;
  Bitvector(Bitvector&&) = default;
  Bitvector& operator=(const Bitvector&) = default;
  Bitvector& operator=(Bitvector&&) = default;

  /**
   * @brief Accessing bit in Bitvector
   *
   * @param bit_pos position of bit to access
   * @return Reference to bit at position bit_pos
   */
  Reference operator[](std::size_t bit_pos);

  /**
   * @brief Accessing bit in Bitvector
   *
   * @param bit_pos position of bit to access
   * @return Reference to bit at position bit_pos
   */
  ConstReference operator[](std::size_t bit_pos) const;

  /**
   * @brief Accessing bit in Bitvector
   *
   * This checks bounds for access
   *
   * @param pos position of bit to access
   * @return Reference to bit at position bit_pos
   */
  Reference at(std::size_t pos);

  /**
   * @brief Accessing bit in Bitvector
   *
   * This checks bounds for access
   *
   * @param pos position of bit to access
   * @return Reference to bit at position bit_pos
   */
  ConstReference at(std::size_t pos) const;

  /**
   * @brief Access range of bits
   *
   * Important: must satisfy: start < end
   *
   * @param start starting position to access
   * @param end end position to access (included)
   * @return Bitvector containing bits from [start, end]
   */
  Bitvector operator()(std::size_t start, std::size_t end) const;

#if __cplusplus >= 202300L

  Bitvector operator[](std::size_t start, std::size_t end) const {
    return this->operator()(start, end);
  }

#endif

  /**
   * @brief Set bit in Bitvector
   *
   * @param bit_pos position of bit to set
   */
  void SetBit(std::size_t bit_pos);

  /**
   * @brief Access single bit in Bitvector
   *
   * @param bit_pos position of bit to access
   * @return bool value of bit at bit_pos
   */
  bool GetBit(std::size_t bit_pos) const;

  /**
   * @brief Gettter for num_bits_
   *
   * @return std::size_t num_bits_ inside Bitvector
   */
  std::size_t getNumBits() const;

  /**
   * @brief Get size of Bitvector = num_bits
   *
   * @return std::size_t number of bits in bitvector
   */
  std::size_t size() const;

  /**
   * @brief Get the Vector inside Bitvector
   *
   * @return std::vector<Type> vector of bits
   */
  std::vector<Type> getBitVec() const;

  /**
   * @brief Bitwise and on bitvector
   *
   * @param bv Bitvector
   * @return Bitvector bitvector with result of bitwise and between this and bv
   */
  Bitvector operator&(const Bitvector& bv);

  /**
   * @brief Bitwise and on this bitvector with argument
   *
   * @param bv Bitvector to and with
   * @return this bitvector with bitwise and between this and bv
   */
  Bitvector& operator&=(const Bitvector& bv);

  /**
   * @brief rightl-shift of bitvectors bits
   *
   * @param value amount to shift
   * @return this bitvector right-shifted by value
   */
  template <typename T, typename = typename std::enable_if<
                            std::is_arithmetic<T>::value, T>::type>
  Bitvector& operator>=(const T value);

  /**
   * @brief Compare bitvectors
   *
   * @param other bitvector to compare this with
   * @return true bitvector are the same (size and bit values)
   * @return false bitvectors are not the same
   */
  bool operator==(const Bitvector& other) const;

  /**
   * @brief Compare bitvectors
   *
   * @param other bitvector to compare this with
   * @return true bitvector are the same (size and bit values)
   * @return false bitvectors are not the same
   */
  bool operator!=(const Bitvector& other) const;

  /**
   * @brief Output to the steam bits inside bv from left to right (101....)
   * bv[size]bv[size-1]...bv[0]
   *
   */
  friend std::ostream& operator<<(std::ostream& os, const Bitvector& bv);

  /**
   * @brief Generic Iterator implementation
   *
   * @tparam Ref_iterator type iterating on
   * @tparam Bitvector_pointer pointer to bitvector referenced
   */
  template <typename Ref_iterator, typename Bitvector_pointer>
  class TempIterator {
   public:
    using iterator_category = std::forward_iterator_tag;
    // using difference_type = std::ptrdiff_t;
    using difference_type = long;
    using value_type = Ref_iterator;
    using pointer = Ref_iterator*;
    using reference = Ref_iterator;

    TempIterator(std::size_t bit_pos, Bitvector_pointer bv)
        : bit_pos_(bit_pos), bv_(bv) {}

    reference operator*() const { return (*bv_)[bit_pos_]; }
    pointer operator->() { return &((*bv_)[bit_pos_]); }

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
      return a.bv_ == b.bv_ && a.bit_pos_ == b.bit_pos_;
    }

    friend bool operator!=(const TempIterator& a, const TempIterator& b) {
      return !(a == b);
    }

   private:
    std::size_t bit_pos_;   // position of bit in bv
    Bitvector_pointer bv_;  // pointer to bv for iterator
  };

  using Iterator = TempIterator<Reference, Bitvector*>;
  using ConstIterator = TempIterator<ConstReference, const Bitvector*>;

  Iterator begin() { return Iterator(0, this); }
  Iterator end() { return Iterator(num_bits_, this); }

  ConstIterator cbegin() const { return ConstIterator(0, this); }
  ConstIterator cend() const { return ConstIterator(num_bits_, this); }

 private:
  std::vector<Type> bits_;  // vector containing bits
  std::size_t num_bits_;    // number of bits in bits_
};

/**
 * @class BitvectorReference
 * this class implements a value that references a single bit inside a Bitvector
 * obj
 */
class BitvectorReference {
 public:
  /**
   * @brief Construct a new Bitvector Reference object
   *
   * @param value reference to word in Bitvector
   * @param position position of the bit referenced
   */
  BitvectorReference(Bitvector::Type* value, std::size_t position)
      : value_(value), position_(position), kValue_(*value) {}

  /**
   * @brief Construct a const BitVectorReference object
   *
   * @param value const reference to word in Bitvector
   * @param position position of the bit referenced
   */
  BitvectorReference(const Bitvector::Type* value, std::size_t position)
      : value_(nullptr), position_(position), kValue_(*value) {}

  ~BitvectorReference() { position_ = 0; }

  /**
   * @brief Get the bit value of referenced bit in Bitvector
   */
  bool getValue() const {
    return static_cast<bool>((kValue_ & bit_set[position_]));
  }

  /**
   * @brief Comparison operator between BitvectorReference and arithmetic type
   * @return true if bit value referenced by bvReference == to
   * static_cast<bool>(value)
   * @return false otherwise
   */
  template <typename T, typename = typename std::enable_if<
                            std::is_arithmetic<T>::value, T>::type>
  friend inline bool operator==(const BitvectorReference& lhs, const T& value) {
    return lhs.getValue() == static_cast<bool>(value);
  }

  /**
   * @brief Comparison operator between two BitvectorReference
   * @return true if bit value referenced by lhs == bit value referenced by rhs
   * @return false otherwise
   */
  friend inline bool operator==(const BitvectorReference& lhs,
                                const BitvectorReference& rhs) {
    return lhs.getValue() == rhs.getValue();
  }

  /**
   * @brief Used to change bit at position_ in the referenced value inside
   * Bitvector
   *
   * @param value value to set the bit to the value becomes static_cast<bool>
   * @return BitvectorReference& with bit changed
   */
  template <typename T, typename = typename std::enable_if<
                            std::is_arithmetic<T>::value, T>::type>
  BitvectorReference& operator=(const T& value) {
    if (static_cast<bool>(value)) {
      *value_ |= bit_set[position_];
    } else {
      *value_ &= ((all_set << (position_ + 1)) |
                  lower_bits_set[position_]);  // mask 1..10..01..1
    }
    return *this;
  }

  /**
   * @brief Output to the steam given the value of the bit static_casted to bool
   */
  friend std::ostream& operator<<(std::ostream& os,
                                  const BitvectorReference& bv_ref) {
    os << static_cast<bool>(bv_ref.getValue());
    return os;
  }

 private:
  const Bitvector::Type kValue_;  // const value for constRef
  Bitvector::Type* value_;        // pointer to word referenced in Bitvector
  std::size_t position_;          // position of bit in word
};

// =============== Implementation  ===============
template <typename T, typename = typename std::enable_if<
                          std::is_arithmetic<T>::value, T>::type>
Bitvector& Bitvector::operator>=(const T value) {
  Bitvector::Type lsbs = 0;
  Bitvector::Type msbs = 0;

  for (std::size_t i = bits_.size() - 1; i > 0; --i) {
    lsbs = bits_[i] & lower_bits_set[value];
    bits_[i] = bits_[i] >> value;
    bits_[i] = bits_[i] | msbs;
    msbs = lsbs << (Bitvector::type_size - value);
  }

  bits_[0] = bits_[0] >> value;
  bits_[0] = bits_[0] | msbs;
  return *this;
}

}  // namespace utils
}  // namespace unialgo

// hashing
namespace std {
template <>
struct hash<unialgo::utils::Bitvector> {
  std::size_t operator()(const unialgo::utils::Bitvector& bv) const {
    size_t seed = 0;
    for (int value : bv.getBitVec()) {
      seed ^= std::hash<int>()(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
    seed ^= std::hash<size_t>()(bv.getNumBits()) + 0x9e3779b9 + (seed << 6) +
            (seed >> 2);
    return seed;
  }
};  // namespace std
}  // namespace std

#endif  // UNIALGO_UTILS_BITVECTOR_