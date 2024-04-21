#include "unialgo/pattern/suffixArray.hpp"

#include <algorithm>  // std::sort
#include <cmath>      // std::log, std::ceil
#include <iostream>

namespace unialgo {
namespace pattern {
// ==== SuffixArray implementation ====

SuffixArray::SuffixArray(utils::WordVector& vw) {
  // check that the last element in wordVector is $ terminated
  auto last_el = vw[vw.size() - 1];
  for (std::size_t i = 0; i < vw.size() - 1; ++i) {
    if (vw[i] < last_el || vw[i] == last_el) {
      // error
    }
  }
  // initialize sa_ with min wordSize required to store indexes
  const std::size_t wordSize = std::ceil(std::log(vw.size()) / std::log(2));
  sa_ = unialgo::utils::WordVector(vw.size(), wordSize);
  // fill sa_ with indexes first element is always the $-suffix
  sa_[0] = vw.size() - 1;
  for (std::size_t i = 1; i < sa_.size(); ++i) {
    sa_[i] = i - 1;
  }

  // create lambda to compare suffix a and b
  auto cmp = [&vw](unialgo::utils::WordVectorReference::value_type a,
                   unialgo::utils::WordVectorReference::value_type b) {
    std::size_t a_pos = a;
    std::size_t b_pos = b;
    while (a_pos < vw.size() && b_pos < vw.size() && vw[a_pos] == vw[b_pos]) {
      ++a_pos;
      ++b_pos;
    }
    // this are not needed for the proprety of the $ terminated vw
    // if (a_pos >= vw.size()) return true;
    // if (b_pos >= vw.size()) return false;
    return vw[a_pos] < vw[b_pos];
  };
  // order sa_ based on alph. order on suffixes
  std::sort(sa_.begin(), sa_.end(), cmp);
}

std::size_t SuffixArray::operator[](std::size_t pos) const { return sa_[pos]; }

std::size_t SuffixArray::size() const { return sa_.size(); }

}  // namespace pattern
}  // namespace unialgo