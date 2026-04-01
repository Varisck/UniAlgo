#include <gtest/gtest.h>

#include <algorithm>  // std::sort
#include <cmath>
#include <memory>   // std::shared_ptr
#include <utility>  // std::swap

#include "unialgo/pattern/wordVecMatching.hpp"
#include "unialgo/utils/bitvector/bitVectors.hpp"
#include "unialgo/utils/waveletMatrix.hpp"

namespace {

TEST(TestingWavelet, ConstructionSmall) {
  std::string s = "476532101417";

  auto alph = unialgo::pattern::GetAlphabet(s);
  auto wv = unialgo::pattern::StringToBitVector(s, alph);

  unialgo::utils::WaveletMatrix mat(wv);
  EXPECT_EQ(mat.getStringSize(), wv.size());
  EXPECT_EQ(mat.getMatrixDepth(), wv.getWordSize());

  EXPECT_EQ(mat.acces(0), wv[0].getValue());
  EXPECT_EQ(mat.acces(1), wv[1].getValue());
  EXPECT_EQ(mat.acces(2), wv[2].getValue());
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

TEST(TestingWavelet, rankSmall) {
  std::string s = "476532101417";

  auto alph = unialgo::pattern::GetAlphabet(s);
  auto wv = unialgo::pattern::StringToBitVector(s, alph);

  unialgo::utils::WaveletMatrix mat(wv);
  EXPECT_EQ(mat.getStringSize(), wv.size());
  EXPECT_EQ(mat.getMatrixDepth(), wv.getWordSize());

  EXPECT_EQ(mat.acces(0), wv[0].getValue());
  EXPECT_EQ(mat.acces(1), wv[1].getValue());
  EXPECT_EQ(mat.acces(2), wv[2].getValue());

  EXPECT_EQ(mat.rank(alph['4'], 0), 1);
  EXPECT_EQ(mat.rank(alph['4'], 1), 1);
  EXPECT_EQ(mat.rank(alph['4'], 2), 1);
  EXPECT_EQ(mat.rank(alph['4'], 9), 2);
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

TEST(TestingWavelet, CopyConstructor) {
  std::string s = "476532101417476532101417";
  auto alph = unialgo::pattern::GetAlphabet(s);
  auto wv = unialgo::pattern::StringToBitVector(s, alph);

  unialgo::utils::WaveletMatrix original(wv);
  unialgo::utils::WaveletMatrix copy(original);

  // copy must produce identical results
  for (std::size_t i = 0; i < wv.size(); ++i) {
    EXPECT_EQ(copy.acces(i), original.acces(i));
  }
  for (auto c : alph) {
    for (std::size_t i = 0; i < wv.size(); ++i) {
      EXPECT_EQ(copy.rank(c.second, i), original.rank(c.second, i));
    }
  }
}

TEST(TestingWavelet, CopyConstructorIndependence) {
  std::string s = "abcdegfaedcfbge";
  auto alph = unialgo::pattern::GetAlphabet(s);
  auto wv = unialgo::pattern::StringToBitVector(s, alph);

  unialgo::utils::WaveletMatrix original(wv);
  unialgo::utils::WaveletMatrix copy(original);

  // destroy original, copy must still work
  {
    unialgo::utils::WaveletMatrix temp(std::move(original));
  }

  for (std::size_t i = 0; i < wv.size(); ++i) {
    EXPECT_EQ(copy.acces(i), wv[i].getValue());
  }
}

TEST(TestingWavelet, CopyAssignment) {
  std::string s1 = "476532101417";
  auto alph1 = unialgo::pattern::GetAlphabet(s1);
  auto wv1 = unialgo::pattern::StringToBitVector(s1, alph1);

  std::string s2 = "abcdegf";
  auto alph2 = unialgo::pattern::GetAlphabet(s2);
  auto wv2 = unialgo::pattern::StringToBitVector(s2, alph2);

  unialgo::utils::WaveletMatrix mat1(wv1);
  unialgo::utils::WaveletMatrix mat2(wv2);

  mat2 = mat1;

  EXPECT_EQ(mat2.getStringSize(), mat1.getStringSize());
  for (std::size_t i = 0; i < wv1.size(); ++i) {
    EXPECT_EQ(mat2.acces(i), mat1.acces(i));
  }
}

TEST(TestingWavelet, SelfAssignment) {
  std::string s = "476532101417";
  auto alph = unialgo::pattern::GetAlphabet(s);
  auto wv = unialgo::pattern::StringToBitVector(s, alph);

  unialgo::utils::WaveletMatrix mat(wv);

  mat = mat;

  for (std::size_t i = 0; i < wv.size(); ++i) {
    EXPECT_EQ(mat.acces(i), wv[i].getValue());
  }
}

TEST(TestingWavelet, MoveConstructor) {
  std::string s = "476532101417476532101417";
  auto alph = unialgo::pattern::GetAlphabet(s);
  auto wv = unialgo::pattern::StringToBitVector(s, alph);

  unialgo::utils::WaveletMatrix original(wv);

  std::vector<uint64_t> expected_access(wv.size());
  for (std::size_t i = 0; i < wv.size(); ++i)
    expected_access[i] = original.acces(i);

  unialgo::utils::WaveletMatrix moved(std::move(original));

  for (std::size_t i = 0; i < wv.size(); ++i) {
    EXPECT_EQ(moved.acces(i), expected_access[i]);
  }
}

TEST(TestingWavelet, MoveAssignment) {
  std::string s1 = "476532101417";
  auto alph1 = unialgo::pattern::GetAlphabet(s1);
  auto wv1 = unialgo::pattern::StringToBitVector(s1, alph1);

  std::string s2 = "abcdegf";
  auto alph2 = unialgo::pattern::GetAlphabet(s2);
  auto wv2 = unialgo::pattern::StringToBitVector(s2, alph2);

  unialgo::utils::WaveletMatrix mat1(wv1);
  unialgo::utils::WaveletMatrix mat2(wv2);

  std::vector<uint64_t> expected(wv1.size());
  for (std::size_t i = 0; i < wv1.size(); ++i)
    expected[i] = mat1.acces(i);

  mat2 = std::move(mat1);

  EXPECT_EQ(mat2.getStringSize(), wv1.size());
  for (std::size_t i = 0; i < wv1.size(); ++i) {
    EXPECT_EQ(mat2.acces(i), expected[i]);
  }
}

TEST(TestingWavelet, CopyIntoVector) {
  std::string s = "476532101417";
  auto alph = unialgo::pattern::GetAlphabet(s);
  auto wv = unialgo::pattern::StringToBitVector(s, alph);

  // no reserve — deliberately allow reallocation to test move safety
  std::vector<unialgo::utils::WaveletMatrix> vec;
  vec.push_back(unialgo::utils::WaveletMatrix(wv));
  vec.push_back(vec[0]);
  vec.push_back(std::move(vec[0]));

  for (std::size_t v = 1; v < vec.size(); ++v) {
    EXPECT_EQ(vec[v].getStringSize(), wv.size());
    for (std::size_t i = 0; i < wv.size(); ++i) {
      EXPECT_EQ(vec[v].acces(i), wv[i].getValue());
    }
  }
}

}  // namespace