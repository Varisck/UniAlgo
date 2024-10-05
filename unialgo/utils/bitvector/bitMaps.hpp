#ifndef UNIALGO_BITMAPS_
#define UNIALGO_BITMAPS_

#include <stdint.h>  // uint64_t

#include <cmath>  // std::ceil

namespace unialgo {

namespace utils {

// word with all bits set
static const uint64_t all_set = -1ULL;

// list of single bit sets
// position i in bit_set = word with i-th bit set
static const uint64_t bit_set[] = {
    1,
    2,
    4,
    8,
    16,
    32,
    64,
    128,
    256,
    512,
    1024,
    2048,
    4096,
    8192,
    16384,
    32768,
    65536,
    131072,
    262144,
    524288,
    1048576,
    2097152,
    4194304,
    8388608,
    16777216,
    33554432,
    67108864,
    134217728,
    268435456,
    536870912,
    1073741824,
    2147483648,
    4294967296,
    8589934592,
    17179869184,
    34359738368,
    68719476736,
    137438953472,
    274877906944,
    549755813888,
    1099511627776,
    2199023255552,
    4398046511104,
    8796093022208,
    17592186044416,
    35184372088832,
    70368744177664,
    140737488355328,
    281474976710656,
    562949953421312,
    1125899906842624,
    2251799813685248,
    4503599627370496,
    9007199254740992,
    18014398509481984,
    36028797018963968,
    72057594037927936,
    144115188075855872,
    288230376151711744,
    576460752303423488,
    1152921504606846976,
    2305843009213693952,
    4611686018427387904,
    9223372036854775808U,
};

// list of bits with the i least significant bits set and the high bits not set.
// lo_set[0] = 0ULL, lo_set[1]=1ULL, lo_set[2]=3ULL...
static const uint64_t lower_bits_set[] = {0ULL,
                                          1ULL,
                                          3ULL,
                                          7ULL,
                                          15ULL,
                                          31ULL,
                                          63ULL,
                                          127ULL,
                                          255ULL,
                                          511ULL,
                                          1023ULL,
                                          2047ULL,
                                          4095ULL,
                                          8191ULL,
                                          16383ULL,
                                          32767ULL,
                                          65535ULL,
                                          131071ULL,
                                          262143ULL,
                                          524287ULL,
                                          1048575ULL,
                                          2097151ULL,
                                          4194303ULL,
                                          8388607ULL,
                                          16777215ULL,
                                          33554431ULL,
                                          67108863ULL,
                                          134217727ULL,
                                          268435455ULL,
                                          536870911ULL,
                                          1073741823ULL,
                                          2147483647ULL,
                                          4294967295ULL,
                                          8589934591ULL,
                                          17179869183ULL,
                                          34359738367ULL,
                                          68719476735ULL,
                                          137438953471ULL,
                                          274877906943ULL,
                                          549755813887ULL,
                                          1099511627775ULL,
                                          2199023255551ULL,
                                          4398046511103ULL,
                                          8796093022207ULL,
                                          17592186044415ULL,
                                          35184372088831ULL,
                                          70368744177663ULL,
                                          140737488355327ULL,
                                          281474976710655ULL,
                                          562949953421311ULL,
                                          1125899906842623ULL,
                                          2251799813685247ULL,
                                          4503599627370495ULL,
                                          9007199254740991ULL,
                                          18014398509481983ULL,
                                          36028797018963967ULL,
                                          72057594037927935ULL,
                                          144115188075855871ULL,
                                          288230376151711743ULL,
                                          576460752303423487ULL,
                                          1152921504606846975ULL,
                                          2305843009213693951ULL,
                                          4611686018427387903ULL,
                                          9223372036854775807ULL,
                                          18446744073709551615ULL};

// utility functions for bits taken from: sdsl - succinct data structures
// library

/**
 * @brief Set number of bits in word
 *
 * @param word word to set bit in
 * @param value value of bits
 * @param offset offset of bits in word
 * @param len len of value
 */
inline void write_bits(uint64_t* word, uint64_t value, uint8_t offset,
                       const uint8_t len) {
  value &= lower_bits_set[len];  // clear upper portion of value
  if (offset + len < 64) {
    // clear target bits in word
    *word &= ((all_set << (offset + len)) |
              lower_bits_set[offset]);  // mask 1..10..01..1
    // set target bits in word
    *word |= (value << offset);
  } else {
    *word &= ((lower_bits_set[offset]));  // mask 0....01..1
    *word |= (value << offset);
    if ((offset = (offset + len) & 0x3F)) {      // offset+len > 64
      *(word + 1) &= (~lower_bits_set[offset]);  // mask 1...10..0
      *(word + 1) |= (value >> (len - offset));
    }
  }
}

/**
 * @brief Read value of bits in word
 *
 * @param word word to read bits from
 * @param offset offset of bits in word
 * @param len lenght of bits to read
 * @return uint64_t word with bits read starting from lsb
 */
inline uint64_t read_bits(const uint64_t* word, uint8_t offset,
                          const uint8_t len) {
  uint64_t w1 = (*word) >> offset;
  if ((offset + len) > 64) {  // if offset+len > 64
    return w1 |               // w1 or w2 adepted:
           ((*(word + 1) &
             lower_bits_set[(offset + len) & 0x3F])  // set higher bits zero
            << (64 - offset));                       // move bits to the left
  } else {
    return w1 & lower_bits_set[len];
  }
}

// turn this into a bit hack
// inline uint64_t getLog2(const uint64_t value) {
//   return std::ceil(std::log(value) / std::log(2));
// }

inline std::size_t get_log_2(const double value) {
  return std::ceil(std::log(value) / std::log(2));
}

}  // namespace utils
}  // namespace unialgo

#endif  // UNIALGO_BITMAPS_