#include <gtest/gtest.h>

#include <algorithm>  // std::sort
#include <cmath>
#include <memory>   // std::shared_ptr
#include <utility>  // std::swap

#include "unialgo/pattern/wordVecMatching.hpp"
#include "unialgo/utils/bitvector/bitVectors.hpp"
#include "unialgo/utils/waveletMatrix.hpp"

namespace {

TEST(TestingWavelet, Construction) {
  std::string s = "476532101417476532101417";

  auto alph = unialgo::pattern::GetAlphabet(s);
  auto wv = unialgo::pattern::StringToBitVector(s, alph);

  unialgo::utils::WaveletMatrix mat(wv);
  EXPECT_EQ(mat.getStringSize(), wv.size());
  EXPECT_EQ(mat.getMatrixDepth(), wv.getWordSize());
}

TEST(TestingWavelet, Access) {
  std::string s = "476532101417476532101417";

  auto alph = unialgo::pattern::GetAlphabet(s);
  auto wv = unialgo::pattern::StringToBitVector(s, alph);

  unialgo::utils::WaveletMatrix mat(wv);
  EXPECT_EQ(mat.getStringSize(), wv.size());
  EXPECT_EQ(mat.getMatrixDepth(), wv.getWordSize());

  EXPECT_EQ(mat.acces(0), wv[0].getValue());
  EXPECT_EQ(mat.acces(1), wv[1].getValue());
  EXPECT_EQ(mat.acces(3), wv[3].getValue());
  EXPECT_EQ(mat.acces(7), wv[7].getValue());
  EXPECT_EQ(mat.acces(10), wv[10].getValue());
}

TEST(TestingWavelet, rank) {
  std::string s = "476532101417476532101417";

  auto alph = unialgo::pattern::GetAlphabet(s);
  auto wv = unialgo::pattern::StringToBitVector(s, alph);

  unialgo::utils::WaveletMatrix mat(wv);
  EXPECT_EQ(mat.getStringSize(), wv.size());
  EXPECT_EQ(mat.getMatrixDepth(), wv.getWordSize());

  EXPECT_EQ(mat.rank(alph['4'], 2), 1);
  EXPECT_EQ(mat.rank(alph['3'], 2), 0);
  EXPECT_EQ(mat.rank(alph['4'], 9), 2);
  EXPECT_EQ(mat.rank(alph['4'], s.length() - 1), 4);
}

TEST(TestingWavelet, rank2) {
  std::string s =
      "abcdegfaedcfbgeafdcebgafdecgabfcdegabfcdegabcdegfabcdegfabcfedgabcfdegbc"
      "degaedcfba";

  auto alph = unialgo::pattern::GetAlphabet(s);
  auto wv = unialgo::pattern::StringToBitVector(s, alph);

  unialgo::utils::WaveletMatrix mat(wv);
  EXPECT_EQ(mat.getStringSize(), wv.size());
  EXPECT_EQ(mat.getMatrixDepth(), wv.getWordSize());

  auto naiveRank = [s](int pos, char c) {
    int count = 0;
    for (int i = 0; i <= pos; ++i)
      if (s[i] == c) ++count;
    return count;
  };

  int count = naiveRank(12, 'a');
  EXPECT_EQ(mat.rank(alph['a'], 12), count);
  for (auto c : alph) {
    for (int i = 0; i < s.size(); ++i) {
      count = naiveRank(i, c.first);
      EXPECT_EQ(mat.rank(c.second, i), count);
    }
  }
}

}  // namespace