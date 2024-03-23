#include <gtest/gtest.h>

#include <string>
#include <vector>

#include "unialgo/pattern/matchingAlgo.hpp"
#include "unialgo/pattern/suffixArray.hpp"

namespace {

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
