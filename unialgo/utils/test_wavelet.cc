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

  // std::cout << std::endl;
  // mat.print();

  EXPECT_EQ(mat.acces(0), wv[0].getValue());
  EXPECT_EQ(mat.acces(1), wv[1].getValue());
  EXPECT_EQ(mat.acces(3), wv[3].getValue());
  EXPECT_EQ(mat.acces(7), wv[7].getValue());
  EXPECT_EQ(mat.acces(10), wv[10].getValue());
}

}  // namespace