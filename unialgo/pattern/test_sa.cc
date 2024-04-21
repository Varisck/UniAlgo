#include <gtest/gtest.h>

#include <algorithm>  // std::sort
#include <string>
#include <vector>

#include "unialgo/pattern/matchingAlgo.hpp"
#include "unialgo/pattern/suffixArray.hpp"

namespace {

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

TEST(StringWVecConversion, Conversion) {
  // Test case 1: text with 3 unique chars
  std::string text = "ggtcagtc$";
  unialgo::utils::WordVector wv = unialgo::pattern::StringToBitVector(text);
  EXPECT_EQ(wv.getWordSize(), 3);
  EXPECT_EQ(wv.size(), text.size());

  unialgo::pattern::SuffixArray sa(wv);
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

}  // namespace
