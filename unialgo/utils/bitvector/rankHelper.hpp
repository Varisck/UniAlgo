#ifndef UNIALGO_UTILS_BITVECTOR_RANKHELPER_
#define UNIALGO_UTILS_BITVECTOR_RANKHELPER_

#include <stdint.h>  // uint64_t

#include <cmath>          // std::pow, std::log, std::ceil
#include <memory>         // shared_ptr
#include <unordered_map>  // std::unordered_map

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
    debug();
    init();
    debug();
  };

  RankHelper(const RankHelper&) = default;
  RankHelper(RankHelper&&) = default;
  RankHelper& operator=(const RankHelper&) = default;
  RankHelper& operator=(RankHelper&&) = default;

  /**
   * @brief Rank helper for bitvector
   *
   * Rank is constant in time O(1)
   *
   * Note: when indx is over the last element of the second layer implementation
   * is O(log(n) / 2) (very rare case)
   *
   * @param indx index (must be valid)
   * @return std::size_t # of bits set to 1 [0, indx]
   */
  std::size_t rank(std::size_t indx) const {
    if (indx % size_first_ == 0) return first_[indx / size_first_];
    if (indx % size_second_ == 0)
      return first_[indx / size_first_] + second_[indx / size_second_];
    // constant except when indx is over the last element of the second layer
    if (size_second_ * indx / size_second_ + size_second_ - 1 <=
        bv_ptr_->getNumBits()) {
      auto bv = bv_ptr_->operator()(
          size_second_ * indx / size_second_ + 1,
          size_second_ * indx / size_second_ + size_second_ - 1);
      return first_[indx / size_first_] + second_[indx / size_second_] +
             t.at(bv)[indx % size_second_ - 1];
    } else {
      std::size_t count = 0;
      for (std::size_t i = size_second_ * indx / size_second_ + 1; i <= indx;
           ++i)
        if (bv_ptr_->GetBit(i)) ++count;
      return first_[indx / size_first_] + second_[indx / size_second_] + count;
    }
  }

  /**
   * @brief Create and computes the layers for rank
   *
   */
  void init() {
    std::size_t size_wv_word = std::ceil(std::log(size_second_) / std::log(2));
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
        if ((i / size_second_ + 1) < second_.size()) {
          // brings count forward
          second_[i / size_second_ + 1] = second_[i / size_second_];
          // make hashmap
          auto bv = bv_ptr_->operator()(i + 1, i + size_second_ - 1);
          if (t.find(bv) == t.end()) {
            utils::WordVector wv(size_second_ - 1, size_wv_word);
            wv[0] = bv[0].getValue();
            for (std::size_t j = 1; j < bv.getNumBits(); ++j)
              if (bv.at(j) == 1) wv[j] = wv[j - 1] + 1;
            t.emplace(bv, wv);
          }
        }
      }
    }

    utils::Bitvector bv(size_second_ - 1);
    // for (std::size_t i = 0; i < size_second_; ++i) {
    // }
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

    std::cout << "Hashmap: " << std::endl;

    for (auto it = t.begin(); it != t.end(); ++it) {
      std::cout << "Key: " << it->first << ": " << it->second << std::endl;
    }
    std::cout << std::endl;
  }

 private:
  std::shared_ptr<unialgo::utils::Bitvector>
      bv_ptr_;  // shared_ptr to bitvector

  // layers of count
  unialgo::utils::WordVector first_;
  unialgo::utils::WordVector second_;
  std::unordered_map<unialgo::utils::Bitvector, unialgo::utils::WordVector>
      t;  // hashmap for 3 layer instant lookup
          // <bitvector between 2 second layer values, array with count at
          // position relative to key>

  std::size_t size_first_;   // first layer's block size
  std::size_t size_second_;  // second layer's block size

};  // RankHelper

}  // namespace utils
}  // namespace unialgo

#endif  // UNIALGO_UTILS_BITVECTOR_RANKHELPER_
