#ifndef UNIALGO_UTILS_BITVECTOR_RANKHELPER_
#define UNIALGO_UTILS_BITVECTOR_RANKHELPER_

#include <stdint.h>  // uint64_t

#include <cmath>    // std::pow, std::log, std::ceil
#include <memory>   // shared_ptr

#include "unialgo/utils/bitvector/bitvector.hpp"
#include "unialgo/utils/bitvector/wordVector.hpp"

/**
 * @file This file contains rank helper for utils::bitvector
 *
 */

// TODO: maybe put assert on rank/select(x) with x >= n

namespace unialgo {
namespace utils {

class RankHelper {
 public:
  RankHelper() : bv_ptr_() {};

  RankHelper(std::shared_ptr<unialgo::utils::Bitvector> bv) : bv_ptr_(bv) {
    size_second_ = std::ceil(std::log(bv->size()) / 2);
    size_first_ = std::pow(size_second_, 2);
    first_ = unialgo::utils::WordVector(
        bv->size() / size_first_ + 1,
        std::ceil(std::log(bv.get()->size()) / std::log(2)));
    second_ = unialgo::utils::WordVector(
        bv->size() / size_second_ + 1,
        std::ceil(std::log(size_first_) / std::log(2)));
    init();
  };

  ~RankHelper() = default;
  RankHelper(const RankHelper&) = default;
  RankHelper(RankHelper&&) = default;
  RankHelper& operator=(const RankHelper&) = default;
  RankHelper& operator=(RankHelper&&) = default;

  /**
   * @brief Rank helper for bitvector
   *
   * Rank is O(1): two table lookups + popcount on at most
   * size_second_ bits for the intra-block remainder.
   *
   * @param indx index (must be valid)
   * @return std::size_t # of bits set to 1 in [0, indx] (returns 0 if pointer
   * = nullptr)
   */
  std::size_t rank(std::size_t indx) const {
    if (bv_ptr_.get() == nullptr) return 0;

    std::size_t count = first_[indx / size_first_].getValue() +
                        second_[indx / size_second_].getValue();

    // Count remaining bits from second-layer boundary to indx using popcount
    std::size_t block_start = (indx / size_second_) * size_second_;
    if (block_start == indx) return count;

    const auto* data = bv_ptr_->data();
    for (std::size_t i = block_start + 1; i <= indx;) {
      std::size_t word_idx = i / Bitvector::type_size;
      std::size_t bit_offset = i % Bitvector::type_size;

      uint64_t word = data[word_idx];
      // How many bits remain in this word that we care about
      std::size_t bits_left = indx - i + 1;
      std::size_t bits_in_word = Bitvector::type_size - bit_offset;

      if (bits_left < bits_in_word) {
        // Mask: keep only bits from bit_offset to bit_offset + bits_left - 1
        word = (word >> bit_offset) & ((1ULL << bits_left) - 1);
        count += __builtin_popcountll(word);
        break;
      } else {
        // Count all bits from bit_offset to end of word
        word = word >> bit_offset;
        count += __builtin_popcountll(word);
        i += bits_in_word;
      }
    }
    return count;
  }

  /**
   * @brief Rank helper for bitvector
   *
   * @param indx index (must be valid)
   * @param value what to count 0, 1
   * @return std::size_t # of bits set to value [0, indx] (returns 0 if pointer
   * = nullptr)
   */
  std::size_t rank(std::size_t indx, bool value) const {
    if (bv_ptr_.get() == nullptr) return 0;
    if (value) return rank(indx);  // count 1
    return indx - rank(indx) + 1;  // count 0
  }

  /**
   * @brief Counter of value from start to end
   *
   * @param start range
   * @param end range
   * @param value what to count 0, 1
   * @return std::size_t # ob bits set to value in [start, end]
   */
  std::size_t rank(std::size_t start, std::size_t end, bool value) const {
    if (start == 0) return rank(end, value);
    if (rank(end, value) <= rank(start - 1, value)) return 0;
    return rank(end, value) - rank(start - 1, value);
  }

  /**
   * @brief Select helper for bitvector
   *
   * Note this implementation is O(log(n))
   *
   * @param count i-th element searched
   * @param value what to search 0, 1
   * @return std::size_t position in bv of the i-th value, -1 if not present
   * (returns 0 if pointer = nullptr)
   */
  std::size_t select(std::size_t count, bool value) const {
    if (bv_ptr_.get() == nullptr) return 0;

    std::size_t start = 0;
    std::size_t end = bv_ptr_->getNumBits() - 1;

    while (start <= end) {
      std::size_t mid = start + (end - start) / 2;
      // found first occ of i-th element
      if ((mid == 0 || count > rank(mid - 1, value)) &&
          rank(mid, value) == count)
        return mid;
      // classic search
      else if (rank(mid, value) < count)
        start = mid + 1;
      else
        end = mid - 1;
    }
    return -1;
  }

  /**
   * @brief Create and computes the layers for rank
   *
   */
  void init() {
    for (std::size_t i = 0; i < bv_ptr_->size(); ++i) {
      if (bv_ptr_->GetBit(i)) {
        ++first_[std::ceil(static_cast<double>(i) / size_first_)];
        ++second_[std::ceil(static_cast<double>(i) / size_second_)];
      }
      if (i % size_first_ == 0 && (i / size_first_ + 1) < first_.size())
        first_[i / size_first_ + 1] = first_[i / size_first_];

      if (i % size_second_ == 0) {
        // reset counter if aligned with first level
        if (i % size_first_ == 0) second_[i / size_second_] = 0;
        if ((i / size_second_ + 1) < second_.size())
          second_[i / size_second_ + 1] = second_[i / size_second_];
      }
    }
  }

  std::size_t GetFirstBlockSize() const { return size_first_; }
  std::size_t GetSecondBlockSize() const { return size_second_; }

  void debug() {
    std::cout << "FirstLayerSize: " << size_first_
              << " SecondLayerSize: " << size_second_ << std::endl
              << " length array first: " << first_.size()
              << " length second: " << second_.size() << std::endl;
    for (int i = 0; i < bv_ptr_->getNumBits(); ++i) {
      std::cout << bv_ptr_->GetBit(i) << " ";
    }
    std::cout << std::endl;
    for (int i = 0; i < bv_ptr_->getNumBits(); ++i) {
      if (i % size_first_ == 0 && i / size_first_ < first_.size())
        std::cout << static_cast<unsigned long long>(
                         first_[i / size_first_].getValue())
                  << " ";
      else
        std::cout << "  ";
    }
    std::cout << std::endl;
    for (int i = 0; i < bv_ptr_->getNumBits(); ++i) {
      if (i % size_second_ == 0 && i / size_second_ < second_.size())
        std::cout << static_cast<unsigned long long>(
                         second_[i / size_second_].getValue())
                  << " ";
      else
        std::cout << "  ";
    }
    std::cout << std::endl;
  }

 private:
  std::shared_ptr<unialgo::utils::Bitvector>
      bv_ptr_;  // shared_ptr to bitvector

  // layers of count
  unialgo::utils::WordVector first_;
  unialgo::utils::WordVector second_;

  std::size_t size_first_;   // first layer's block size
  std::size_t size_second_;  // second layer's block size

};  // RankHelper

}  // namespace utils
}  // namespace unialgo

#endif  // UNIALGO_UTILS_BITVECTOR_RANKHELPER_
