#include <gtest/gtest.h>

#include <string>
#include <vector>

#include "unialgo/pattern/stringMatching.hpp"
#include "unialgo/utils/bitvector/bitVectors.hpp"

namespace {

TEST(StringWVecConversion, Conversion) {
  // Test case 1: text with 3 unique chars
  std::string text = "abababcabab";
  unialgo::utils::WordVector wv = unialgo::pattern::StringToBitVector(text);
  EXPECT_EQ(wv.getWordSize(), 2);
  EXPECT_EQ(wv.size(), text.size());

  for (size_t i = 0; i < text.size(); ++i) {
    if (text[i] == 'a')
      EXPECT_EQ(wv[i], 0);
    else if (text[i] == 'b')
      EXPECT_EQ(wv[i], 1);
    else
      EXPECT_EQ(wv[i], 2);
  }
  // Test case 2: Pattern with 5 unique chars
  text = "abababcababiiipppp";
  wv = unialgo::pattern::StringToBitVector(text);
  EXPECT_EQ(wv.getWordSize(), 3);
  EXPECT_EQ(wv.size(), text.size());

  for (size_t i = 0; i < text.size(); ++i) {
    if (text[i] == 'a')
      EXPECT_EQ(wv[i], 0);
    else if (text[i] == 'b')
      EXPECT_EQ(wv[i], 1);
    else if (text[i] == 'c')
      EXPECT_EQ(wv[i], 2);
    else if (text[i] == 'i')
      EXPECT_EQ(wv[i], 3);
    else
      EXPECT_EQ(wv[i], 4);
  }
}

TEST(StringWVecConversion, ConvertingEmptyString) {
  // Test case 1: text with 3 unique chars
  std::string text1 = "";
  unialgo::utils::WordVector wv = unialgo::pattern::StringToBitVector(text1);
  EXPECT_EQ(wv.getWordSize(), 0);
  EXPECT_EQ(wv.size(), text1.size());
}

TEST(FinateStateAutomataTest, BasicTests) {
  // Test case 1: Pattern occurs multiple times in the text
  std::string text1 = "abababcabab";
  std::string pattern1 = "ab";
  std::vector<size_t> result1 = unialgo::pattern::Fsa(text1, pattern1);
  std::vector<size_t> expected1 = {0, 2, 4, 7, 9};
  EXPECT_EQ(result1, expected1);

  // Test case 2: Pattern occurs once in the text
  std::string text2 = "Hello, world!";
  std::string pattern2 = "world";
  std::vector<size_t> result2 = unialgo::pattern::Fsa(text2, pattern2);
  std::vector<size_t> expected2 = {7};
  EXPECT_EQ(result2, expected2);

  // Test case 3: Pattern does not occur in the text
  std::string text3 = "abcdef";
  std::string pattern3 = "xyz";
  std::vector<size_t> result3 = unialgo::pattern::Fsa(text3, pattern3);
  std::vector<size_t> expected3 = {};
  EXPECT_EQ(result3, expected3);
}

TEST(FinateStateAutomataTest, PatternLongerThenText) {
  // Test case 1: Pattern longer then text
  std::string text = "a";
  std::string pattern = "xyz";
  std::vector<size_t> result = unialgo::pattern::Fsa(text, pattern);
  std::vector<size_t> expected = {};
  EXPECT_EQ(result, expected);
}

TEST(KMPTest, BasicTests) {
  // Test case 1: Pattern occurs multiple times in the text
  std::string text1 = "abababcabab";
  std::string pattern1 = "ab";
  std::vector<size_t> result1 = unialgo::pattern::Fsa(text1, pattern1);
  std::vector<size_t> expected1 = {0, 2, 4, 7, 9};
  EXPECT_EQ(result1, expected1);

  // Test case 2: Pattern occurs once in the text
  std::string text2 = "Hello, world!";
  std::string pattern2 = "world";
  std::vector<size_t> result2 = unialgo::pattern::Fsa(text2, pattern2);
  std::vector<size_t> expected2 = {7};
  EXPECT_EQ(result2, expected2);

  // Test case 3: Pattern does not occur in the text
  std::string text3 = "abcdef";
  std::string pattern3 = "xyz";
  std::vector<size_t> result3 = unialgo::pattern::Fsa(text3, pattern3);
  std::vector<size_t> expected3 = {};
  EXPECT_EQ(result3, expected3);
}

TEST(KMPTest, PatternLongerThenText) {
  // Test case 1: Pattern longer then text
  std::string text = "a";
  std::string pattern = "xyz";
  std::vector<size_t> result = unialgo::pattern::Fsa(text, pattern);
  std::vector<size_t> expected = {};
  EXPECT_EQ(result, expected);
}

}  // namespace
