#include "unialgo/pattern/bwt.hpp"

#include "unialgo/pattern/suffixArray.hpp"

namespace unialgo {
namespace pattern {

Bwt::Bwt(const unialgo::utils::WordVector& text,
         unialgo::utils::WordVector sa) {
  // turn sa into bwt vector in place
  // first element is always the one preceding $ (or smallest in lex order -> 0)
  sa[0] = text[text.size() - 2].getValue();
  // bwt[i] = text[sa[i] - 1]
  for (std::size_t i = 1; i < text.size(); ++i) {
    if (sa[i] > 0) sa[i] = text[sa[i] - 1].getValue();
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

Bwt::Bwt(const unialgo::utils::WordVector& text) {
  // return sa (sa[i] index where the i-th suffix start)
  unialgo::utils::WordVector sa = unialgo::pattern::makeSuffixArray(text);
  *this = Bwt(text, sa);
}

std::size_t Bwt::getWordSize() const { return occ_.getMatrixDepth() - 1; }

uint64_t Bwt::operator[](std::size_t pos) const { return occ_.acces(pos); }

uint64_t Bwt::at(std::size_t pos) const {
  assert(pos < occ_.getStringSize() && "Bwt access out of bound .at()");
  return occ_.acces(pos);
}

std::size_t Bwt::size() const { return occ_.getStringSize(); }

std::pair<std::size_t, std::size_t> Bwt::backward_extend(uint64_t b, uint64_t e,
                                                         uint64_t sigma) const {
  std::size_t b1 = c_.at(sigma);
  if (b > 0) b1 += occ_.rank(sigma, b - 1);
  std::size_t e1 = c_.at(sigma);
  if (e > 0) e1 += occ_.rank(sigma, e - 1);
  return std::make_pair(b1, e1);
}

std::vector<std::size_t> Bwt::searchPattern(
    const unialgo::utils::WordVector& pattern) const {
  std::size_t end = pattern.size();
  if (end == 0) return std::vector<std::size_t>{};
  std::size_t start = 0;
  std::size_t j = 0;
  while (j < pattern.size()) {
    uint64_t sigma = pattern[pattern.size() - 1 - j];
    auto interval = backward_extend(start, end, sigma);
    start = interval.first;
    end = interval.second;
    ++j;
    // interval not valid
    if (start == end) return std::vector<std::size_t>{};
  }
  std::vector<std::size_t> res;
  for (std::size_t i = start; i < end; ++i) res.push_back(i);
  return res;
}

std::vector<std::size_t> Bwt::searchPattern(
    const unialgo::utils::WordVector& pattern,
    const unialgo::utils::WordVector& sa) const {
  std::vector<std::size_t> positions = (*this).searchPattern(pattern);
  std::vector<std::size_t> res({});
  for (std::size_t pos : positions) {
    res.push_back(sa[pos]);
  }
  return res;
}

}  // namespace pattern
}  // namespace unialgo