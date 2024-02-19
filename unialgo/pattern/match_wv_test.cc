#include <gtest/gtest.h>

#include <cassert>
#include <string>
#include <vector>

#include "unialgo/pattern/matchingAlgo.hpp"

namespace {

// ========= Testing alphabet =========
TEST(AlphabetTesting, AlphabetTesting_empty) {
  unialgo::pattern::Alphabet alph = unialgo::pattern::GetAlphabet("");
  EXPECT_EQ(alph.size(), 0);
}

TEST(AlphabetTesting, AlphabetTesting_order) {
  unialgo::pattern::Alphabet alph = unialgo::pattern::GetAlphabet("acbdeg");
  EXPECT_EQ(alph.size(), 6);
  EXPECT_EQ(alph.at('a'), 0);
  EXPECT_EQ(alph.at('b'), 1);
  EXPECT_EQ(alph.at('c'), 2);
  EXPECT_EQ(alph.at('d'), 3);
  EXPECT_EQ(alph.at('e'), 4);
  EXPECT_EQ(alph.at('g'), 5);
}

TEST(AlphabetTesting, getUniqueChars) {
  auto uniques = unialgo::pattern::GetUniqueChars("abcdeaaeedd");
  EXPECT_TRUE(uniques.contains('a'));
  EXPECT_TRUE(uniques.contains('b'));
  EXPECT_TRUE(uniques.contains('c'));
  EXPECT_TRUE(uniques.contains('d'));
  EXPECT_TRUE(uniques.contains('e'));
  EXPECT_TRUE(!uniques.contains('f'));
}

TEST(FinateStateAutomataTestWV, fsaOnWordVector) {
  // Test case 1: Pattern occurs multiple times in the text
  std::string text1 = "abababcabab";
  std::string pattern1 = "ab";

  auto alphabet = unialgo::pattern::GetAlphabet(text1);

  unialgo::utils::WordVector wvText1 =
      unialgo::pattern::StringToBitVector(text1, alphabet);
  unialgo::utils::WordVector wvPattern1 =
      unialgo::pattern::StringToBitVector(pattern1, alphabet);
  EXPECT_EQ(wvText1.getWordSize(), 2);
  EXPECT_EQ(wvPattern1.getWordSize(), 2);
  std::vector<size_t> result1 = unialgo::pattern::Fsa<>(wvText1, wvPattern1);
  std::vector<size_t> expected1 = {0, 2, 4, 7, 9};
  EXPECT_EQ(result1, expected1);
}

// test case where word_size doesn't match
TEST(FinateStateAutomataTestWV, checkWordSize) {
  std::string text1 = "abababcabab";
  std::string pattern1 = "ab";

  unialgo::utils::WordVector wvText1 =
      unialgo::pattern::StringToBitVector(text1);
  unialgo::utils::WordVector wvPattern1 =
      unialgo::pattern::StringToBitVector(pattern1);
  EXPECT_EQ(wvText1.getWordSize(), 2);
  EXPECT_EQ(wvPattern1.getWordSize(), 1);
  ASSERT_DEATH(unialgo::pattern::Fsa<>(wvText1, wvPattern1),
               ".*word_size not matching for text and pattern.*");
}

}  // namespace
