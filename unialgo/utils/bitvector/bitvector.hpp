#ifndef UNIALGO_UTILS_BITVECTOR_
#define UNIALGO_UTILS_BITVECTOR_

#include <stdint.h>  // uint64_t

#include <iostream>
#include <ostream>      // ostream
#include <type_traits>  // std::enable_if std::is_arithmetic
#include <vector>

#include "unialgo/utils/bitvector/bitMaps.hpp"
#include "unialgo/utils/bitvector/bitvector.hpp"

// TODO: put assert() on accessing bit_positions >= num_bits_

namespace unialgo {

namespace utils {

// forward decleration
class BitvectorReference;

/**
 * @class Bitvector
 *
 */
class Bitvector {
 public:
  using Type = uint64_t;                                  // type used in vector
  using Reference = BitvectorReference;                   // reference to bit
  static const std::size_t type_size = sizeof(Type) * 8;  // size of Type in bit

  /**
   * @brief Construct a new Bitvector object
   *
   * @param num_bits number of bits to store
   */
  Bitvector(std::size_t num_bits);

  /**
   * @brief Destroy the Bitvector object
   */
  ~Bitvector();

  /**
   * @brief Accessing bit in Bitvector
   *
   * @param bit_pos position of bit to access
   * @return Reference to bit at position bit_pos
   */
  Reference operator[](std::size_t bit_pos);

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
  bool GetBit(std::size_t bit_pos);

  /**
   * @brief Gettter for num_bits_
   *
   * @return std::size_t num_bits_ inside Bitvector
   */
  std::size_t getNumBits();

  Bitvector operator&(const Bitvector& bv);

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
  BitvectorReference(Bitvector::Type& value, std::size_t position)
      : value_(value), position_(position) {}

  ~BitvectorReference() { position_ = 0; }

  /**
   * @brief Get the bit value of referenced bit in Bitvector
   */
  bool getValue() { return static_cast<bool>((value_ & bit_set[position_])); }

  /**
   * @brief Comparison operator between BitvectorReference and arithmetic type
   * @return true if bit value referenced by bvReference == to
   * static_cast<bool>(value)
   * @return false otherwise
   */
  template <typename T, typename = typename std::enable_if<
                            std::is_arithmetic<T>::value, T>::type>
  friend inline bool operator==(const BitvectorReference& lhs, const T& value) {
    return static_cast<bool>((lhs.value_ & bit_set[lhs.position_])) ==
           static_cast<bool>(value);
  }

  /**
   * @brief Comparison operator between two BitvectorReference
   * @return true if bit value referenced by lhs == bit value referenced by rhs
   * @return false otherwise
   */
  friend inline bool operator==(const BitvectorReference& lhs,
                                const BitvectorReference& rhs) {
    return static_cast<bool>((lhs.value_ & bit_set[lhs.position_])) ==
           static_cast<bool>((rhs.value_ & bit_set[rhs.position_]));
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
      value_ |= bit_set[position_];
    } else {
      // value_ &= !bit_set[position_];
      value_ &= ((all_set << (position_ + 1)) |
                 lower_bits_set[position_]);  // mask 1..10..01..1
    }
    return *this;
  }

  /**
   * @brief Output to the steam given the value of the bit static_casted to bool
   */
  friend std::ostream& operator<<(std::ostream& os,
                                  const BitvectorReference& bv_ref) {
    os << static_cast<bool>((bv_ref.value_ & bit_set[bv_ref.position_]));
    return os;
  }

 private:
  Bitvector::Type& value_;  // word referenced in Bitvector
  std::size_t position_;    // position of bit in word
};

}  // namespace utils

}  // namespace unialgo

#endif  // UNIALGO_UTILS_BITVECTOR_