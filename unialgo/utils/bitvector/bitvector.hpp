#ifndef UNIALGO_UTILS_BITVECTOR_
#define UNIALGO_UTILS_BITVECTOR_

#include <stdint.h>  // uint64_t

#include <ostream>      //ostream
#include <type_traits>  // std::enable_if std::is_arithmetic
#include <vector>

#include "unialgo/utils/bitvector/bitMaps.hpp"
#include "unialgo/utils/bitvector/bitvector.hpp"

namespace unialgo {

namespace utils {

class BitvectorReference;

class Bitvector {
 public:
  using Type = uint64_t;
  // change this to class reference
  // using Reference = Type&;
  using Reference = BitvectorReference;
  static const std::size_t type_size = sizeof(Type) * 8;

  /**
   * @brief Construct a new Bitvector object
   *
   * @param num_bits number of bits to store
   */
  Bitvector(std::size_t num_bits);

  /**
   * @brief Destroy the Bitvector object
   *
   */
  ~Bitvector();

  /**
   * @brief accessing bit in Bitvector
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
   * @brief access sinble bit in Bitvector
   *
   * @param bit_pos position of bit to access
   * @return Reference vallue of bit at bit_pos
   */
  bool GetBit(std::size_t bit_pos);

 private:
  std::vector<Type> bits_;  // vector containing bits
  std::size_t num_bits_;    // number of bits in bits_
};

class BitvectorReference {
 public:
  /**
   * @brief Construct a new Bitvector Reference object
   *
   * @param position position of the bit referenced
   */
  BitvectorReference(typename Bitvector::Type& value, std::size_t position)
      : value_(value), position_(position) {}

  ~BitvectorReference() { position_ = 0; }

  template <typename T, typename = typename std::enable_if<
                            std::is_arithmetic<T>::value, T>::type>
  BitvectorReference& operator=(const T& value) {
    if (value) {
      value_ |= bit_set[position_];
    } else {
      value_ &= !bit_set[position_];
    }
    return *this;
  }

  friend std::ostream& operator<<(std::ostream& os,
                                  const BitvectorReference& obj) {
    os << (obj.value_ & bit_set[obj.position_]);
    return os;
  }

 private:
  std::size_t position_;
  typename Bitvector::Type& value_;
};

}  // namespace utils

}  // namespace unialgo

#endif  // UNIALGO_UTILS_BITVECTOR_