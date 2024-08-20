#ifndef UNIALGO_UTILS_BITVECTOR_RANKHELPER_
#define UNIALGO_UTILS_BITVECTOR_RANKHELPER_

#include <stdint.h>  // uint64_t

#include <cmath>   // std::pow, std::log, std::ceil
#include <memory>  // shared_ptr

#include "unialgo/utils/bitvector/bitvector.hpp"
#include "unialgo/utils/bitvector/wordVector.hpp"

/**
 * @file This file contains rank helper for utils::bitvector
 *
 */

namespace unialgo {
namespace utils {

class RankHelper {
 public:
  RankHelper(std::shared_ptr<unialgo::utils::Bitvector> bv) : bv_ptr_(bv) {
    size_second_ = std::ceil(std::log(bv->size()) / 2);
    size_first_ = std::pow(size_second_, 2);
    first_ = unialgo::utils::WordVector(
        bv->size() / size_first_ + 1,
        std::ceil(std::log(size_first_) / std::log(2)));
    second_ = unialgo::utils::WordVector(
        bv->size() / size_second_ + 1,
        std::ceil(std::log(size_second_) / std::log(2)));
    // debug();
    init();
    // debug();
  };

  RankHelper(const RankHelper&) = default;
  RankHelper(RankHelper&&) = default;
  RankHelper& operator=(const RankHelper&) = default;
  RankHelper& operator=(RankHelper&&) = default;

  /**
   * @brief Rank helper for bitvector
   *
   *  this should be O(1) theory (implementing hashmap makes it O(1))
   * (!!) implement hashmap with logic and on word to make it constant
   * implementation is O(size_second_) ~ O(log(n) / 2)
   *
   * @param indx index (must be valid)
   * @return std::size_t # of bits set to 1 [0, indx]
   */
  std::size_t rank(std::size_t indx) const {
    if (indx % size_first_ == 0) return first_[indx / size_first_];
    if (indx % size_second_ == 0)
      return first_[indx / size_first_] + second_[indx / size_second_];
    // this can become constant |
    std::size_t count = 0;
    for (std::size_t i = size_second_ * indx / size_second_ + 1; i <= indx; ++i)
      if (bv_ptr_->GetBit(i)) ++count;
    return first_[indx / size_first_] + second_[indx / size_second_] + count;
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
      if (i % size_second_ == 0 && i % size_first_ == 0)
        second_[i / size_second_] = 0;
      if (i % size_second_ == 0 && (i / size_second_ + 1) < second_.size())
        second_[i / size_second_ + 1] = second_[i / size_second_];
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
      bv_ptr_;                        // shared_ptr to bitvector
  unialgo::utils::WordVector first_;  // layers of count
  unialgo::utils::WordVector second_;
  std::size_t size_first_;   // first layer's block size
  std::size_t size_second_;  // second layer's block size

};  // RankHelper

}  // namespace utils
}  // namespace unialgo

#endif  // UNIALGO_UTILS_BITVECTOR_RANKHELPER_
