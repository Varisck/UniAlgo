#include "unialgo/pattern/bwt.hpp"

#include "unialgo/pattern/suffixArray.hpp"

namespace unialgo {
namespace pattern {

Bwt::Bwt(unialgo::utils::WordVector& text) {
  // return sa (sa[i] index where the i-th suffix start)
  unialgo::utils::WordVector sa = unialgo::pattern::makeSuffixArray(text);

  // turn sa into bwt vector in place
  // first element is always the one preceding $ (or smallest in lex order -> 0)
  sa[0] = text[text.size() - 2];
  // bwt[i] = text[sa[i] - 1]
  for (std::size_t i = 1; i < text.size(); ++i) {
    if (sa[i] > 0) sa[i] = text[sa[i] - 1];
  }
  // wavelet matrix of bwt
  occ_ = unialgo::utils::WaveletMatrix(sa);

  // store # values < c forall c (expressable over wordvec.wordsize)
  uint64_t value = 0;
  c_[0] = 0;

  while (value <= ((1 << text.getWordSize()) - 1)) {
    c_[value + 1] = c_[value] + occ_.rank(value, text.size() - 1);
    ++value;
  }
}

}  // namespace pattern
}  // namespace unialgo