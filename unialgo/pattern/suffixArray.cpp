#include "unialgo/pattern/suffixArray.hpp"

#include <algorithm>  //std::sort

namespace unialgo {
namespace pattern {
// ==== SuffixArray implementation ====

SuffixArray::SuffixArray(utils::WordVector& vw) : sa_(vw.size()) {
  // check that the last element in wordVector is $ terminated
  auto last_el = vw[vw.size() - 1];
  for (std::size_t i = 0; i < vw.size() - 1; ++i) {
    if (vw[i] < last_el || vw[i] == last_el) {
      // error
    }
  }
  // initialize sa_ with indexes first element is always the $-suffix
  sa_[0] = vw.size() - 1;
  for (std::size_t i = 1; i < sa_.size(); ++i) {
    sa_[i] = i - 1;
  }
  // create lambda to compare suffix a and b
  auto cmp = [&vw](std::size_t a, std::size_t b) {
    while (vw[a] == vw[b] && a < vw.size() && b < vw.size()) {
      ++a;
      ++b;
    }
    // this are not needed for the proprety of the $ terminated vw
    // if (a == vw.size()) return true;
    // if (b == vw.size()) return false;
    return vw[a] < vw[b];
  };
  // order sa_ based on alph. order on suffixes
  std::sort(sa_.begin(), sa_.end(), cmp);
}

std::size_t SuffixArray::operator[](std::size_t pos) const { return sa_[pos]; }

std::size_t SuffixArray::size() const { return sa_.size(); }

}  // namespace pattern
}  // namespace unialgo