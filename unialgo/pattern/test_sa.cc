#include <gtest/gtest.h>

#include <algorithm>  // std::sort
#include <string>
#include <vector>

#include "unialgo/pattern/bwt.hpp"
#include "unialgo/pattern/matchingAlgo.hpp"
#include "unialgo/pattern/suffixArray.hpp"

namespace {

// ========== Suffix Array ==========

// testing sort on wordvector
TEST(StringWVecConversion, SortingOnWordvector) {
  unialgo::utils::WordVector wv(10, 5);
  // asign values at wv[i] = length - i
  wv[0] = 10;
  wv[1] = 9;
  wv[2] = 8;
  wv[3] = 7;
  wv[4] = 6;
  wv[5] = 5;
  wv[6] = 4;
  wv[7] = 3;
  wv[8] = 2;
  wv[9] = 1;

  // check values are correct:
  EXPECT_EQ(wv[0], 10);
  EXPECT_EQ(wv[1], 9);
  EXPECT_EQ(wv[9], 1);

  std::sort(wv.begin(), wv.end());

  EXPECT_EQ(wv[0], 1);
  EXPECT_EQ(wv[1], 2);
  EXPECT_EQ(wv[9], 10);
}

TEST(StringWVecConversion, SortingOnWordvectorLong) {
  unialgo::utils::WordVector wv(200, 10);
  // asign values at wv[i] = length - i
  for (int i = 0; i < wv.size(); ++i) {
    wv[i] = wv.size() - i;
  }

  // check values are correct:
  for (int i = 0; i < wv.size(); ++i) {
    EXPECT_EQ(wv[i], wv.size() - i);
  }

  std::sort(wv.begin(), wv.end());

  for (int i = 0; i < wv.size(); ++i) {
    EXPECT_EQ(wv[i], i + 1);
  }
}

TEST(SuffixArray, Creation) {
  std::string text = "ggtcagtc$";
  unialgo::utils::WordVector wv = unialgo::pattern::StringToBitVector(text);
  EXPECT_EQ(wv.getWordSize(), 3);
  EXPECT_EQ(wv.size(), text.size());

  // unialgo::pattern::SuffixArray sa(wv);
  unialgo::utils::WordVector sa = unialgo::pattern::makeSuffixArray(wv);
  EXPECT_EQ(sa[0], 8);
  EXPECT_EQ(sa[1], 4);
  EXPECT_EQ(sa[2], 7);
  EXPECT_EQ(sa[3], 3);
  EXPECT_EQ(sa[4], 0);
  EXPECT_EQ(sa[5], 5);
  EXPECT_EQ(sa[6], 1);
  EXPECT_EQ(sa[7], 6);
  EXPECT_EQ(sa[8], 2);
}

TEST(SuffixArray, linear_time) {
  std::string text = "mississippi$$$$";
  unialgo::utils::WordVector wv = unialgo::pattern::StringToBitVector(text);
  EXPECT_EQ(wv.getWordSize(), 3);
  EXPECT_EQ(wv.size(), text.size());

  // unialgo::pattern::SuffixArray sa(wv);
  unialgo::utils::WordVector sa(10, unialgo::utils::get_log_2(wv.size()));
  unialgo::pattern::make_suffix_array(wv, sa, 11,
                                      (1 << (wv.getWordSize() + 1)) - 1);
  EXPECT_EQ(sa[0], 10);
  EXPECT_EQ(sa[1], 7);
  EXPECT_EQ(sa[2], 4);
  EXPECT_EQ(sa[3], 1);
  EXPECT_EQ(sa[4], 0);
  EXPECT_EQ(sa[5], 9);
  EXPECT_EQ(sa[6], 8);
  EXPECT_EQ(sa[7], 6);
  EXPECT_EQ(sa[8], 3);
  EXPECT_EQ(sa[9], 5);
  EXPECT_EQ(sa[10], 2);
}

TEST(SuffixArray, sa_from_string) {
  std::string text = "mississippi";
  unialgo::utils::WordVector sa =
      unialgo::pattern::suffix_array_from_string(text);

  EXPECT_EQ(sa[0], 10);
  EXPECT_EQ(sa[1], 7);
  EXPECT_EQ(sa[2], 4);
  EXPECT_EQ(sa[3], 1);
  EXPECT_EQ(sa[4], 0);
  EXPECT_EQ(sa[5], 9);
  EXPECT_EQ(sa[6], 8);
  EXPECT_EQ(sa[7], 6);
  EXPECT_EQ(sa[8], 3);
  EXPECT_EQ(sa[9], 5);
  EXPECT_EQ(sa[10], 2);
}

// ========== BWT ==========

TEST(BWT, Creation) {
  // Test case 1: text with 3 unique chars
  std::string text = "ggtcagtc$";
  unialgo::utils::WordVector wv = unialgo::pattern::StringToBitVector(text);
  EXPECT_EQ(wv.getWordSize(), 3);
  EXPECT_EQ(wv.size(), text.size());

  unialgo::pattern::Bwt bwt(wv);
  EXPECT_EQ(wv.getWordSize(), bwt.getWordSize());
  EXPECT_EQ(wv.size(), bwt.size());
}

TEST(BWT, accessOperator) {
  // Test case 1: text with 3 unique chars
  std::string text = "ggtcagtc$";
  unialgo::utils::WordVector wv = unialgo::pattern::StringToBitVector(text);
  EXPECT_EQ(wv.getWordSize(), 3);
  EXPECT_EQ(wv.size(), text.size());

  unialgo::pattern::Bwt bwt(wv);
  EXPECT_EQ(wv.getWordSize(), bwt.getWordSize());

  EXPECT_EQ(bwt[0], 2);
  EXPECT_EQ(bwt[1], 2);
  EXPECT_EQ(bwt[2], 4);
  EXPECT_EQ(bwt[3], 4);
  EXPECT_EQ(bwt[4], 0);
  EXPECT_EQ(bwt[5], 1);
  EXPECT_EQ(bwt[6], 3);
  EXPECT_EQ(bwt[7], 3);
  EXPECT_EQ(bwt[8], 3);
}

TEST(BWT, operatorAt) {
  // Test case 1: text with 3 unique chars
  std::string text = "ggtcagtc$";
  unialgo::utils::WordVector wv = unialgo::pattern::StringToBitVector(text);
  EXPECT_EQ(wv.getWordSize(), 3);
  EXPECT_EQ(wv.size(), text.size());

  unialgo::pattern::Bwt bwt(wv);
  EXPECT_EQ(wv.getWordSize(), bwt.getWordSize());

  EXPECT_EQ(bwt.at(0), 2);
  EXPECT_EQ(bwt.at(1), 2);
  EXPECT_EQ(bwt.at(2), 4);
  EXPECT_EQ(bwt.at(3), 4);
  EXPECT_EQ(bwt.at(4), 0);
  EXPECT_EQ(bwt.at(5), 1);
  EXPECT_EQ(bwt.at(6), 3);
  EXPECT_EQ(bwt.at(7), 3);
  EXPECT_EQ(bwt.at(8), 3);
}

TEST(BWT, searchPattern) {
  std::string text = "ggtcagtc$";
  auto alph = unialgo::pattern::GetAlphabet(text);
  unialgo::utils::WordVector wv =
      unialgo::pattern::StringToBitVector(text, alph);

  EXPECT_EQ(wv.getWordSize(), 3);
  EXPECT_EQ(wv.size(), text.size());

  unialgo::pattern::Bwt bwt(wv);

  std::string pattern = "gtc";
  unialgo::utils::WordVector p =
      unialgo::pattern::StringToBitVector(pattern, alph);

  std::vector<std::size_t> res = bwt.searchPattern(p);

  // position in suffix array where pattern start
  EXPECT_EQ(res.size(), 2);
  EXPECT_EQ(res[0], 5);
  EXPECT_EQ(res[1], 6);

  auto sa = unialgo::pattern::makeSuffixArray(wv);

  EXPECT_EQ(sa[res[1]], 1);
  EXPECT_EQ(sa[res[0]], 5);
}

TEST(BWT, searchPatternWithSa) {
  std::string text = "ggtcagtc$";
  auto alph = unialgo::pattern::GetAlphabet(text);
  unialgo::utils::WordVector wv =
      unialgo::pattern::StringToBitVector(text, alph);

  EXPECT_EQ(wv.getWordSize(), 3);
  EXPECT_EQ(wv.size(), text.size());

  unialgo::pattern::Bwt bwt(wv);

  std::string pattern = "gtc";
  auto sa = unialgo::pattern::makeSuffixArray(wv);

  unialgo::utils::WordVector p =
      unialgo::pattern::StringToBitVector(pattern, alph);

  std::vector<std::size_t> res = bwt.searchPattern(p, sa);

  // position in text where pattern starts
  EXPECT_EQ(res.size(), 2);
  EXPECT_EQ(res[0], 5);
  EXPECT_EQ(res[1], 1);
}

TEST(BWT, searchPatternNoExisting) {
  std::string text = "ggtcagtc$";
  auto alph = unialgo::pattern::GetAlphabet(text);
  unialgo::utils::WordVector wv =
      unialgo::pattern::StringToBitVector(text, alph);

  EXPECT_EQ(wv.getWordSize(), 3);
  EXPECT_EQ(wv.size(), text.size());

  unialgo::pattern::Bwt bwt(wv);

  std::string pattern = "gtg";
  auto sa = unialgo::pattern::makeSuffixArray(wv);

  unialgo::utils::WordVector p =
      unialgo::pattern::StringToBitVector(pattern, alph);

  std::vector<std::size_t> res = bwt.searchPattern(p, sa);

  // position in text where pattern starts
  EXPECT_EQ(res.size(), 0);
}

}  // namespace
