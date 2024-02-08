#include <gtest/gtest.h>

#include "unialgo/utils/bitvector/bitvector.hpp"
#include "unialgo/utils/bitvector/wordVector.hpp"

namespace {

// ============ Testing BitVector ============

// Testing operator[] on Bitvector
TEST(TestBitvector, TestingAccessOperator) {
  unialgo::utils::Bitvector bv(62);

  bv[12] = 1;
  for (int i = 0; i < 12; ++i) EXPECT_EQ(bv[i], 0);
  EXPECT_EQ(bv[12], 1);
  for (int i = 13; i < 64; ++i) EXPECT_EQ(bv[i], 0);
}

// Testing Bitvector with lenght > Bitvector::type_size
TEST(TestBitvector, TestingMoreWords) {
  unialgo::utils::Bitvector bv(100);

  bv[64] = 1;

  for (int i = 0; i < 64; ++i) EXPECT_EQ(bv[i], 0);
  for (int i = 65; i < 128; ++i) EXPECT_EQ(bv[i], 0);

  EXPECT_EQ(bv[64], 1);
}

// More testing for operator[] on bigger bv
TEST(TestBitvector, TestSetBitAndClearBit) {
  unialgo::utils::Bitvector bv(100);

  bv[64] = 1;

  for (int i = 0; i < 64; ++i) EXPECT_EQ(bv[i], 0);
  for (int i = 65; i < 128; ++i) EXPECT_EQ(bv[i], 0);

  EXPECT_EQ(bv[64], 1);

  bv[64] = 0;

  for (int i = 0; i < 128; ++i) EXPECT_EQ(bv[i], 0);
}

// Testing function getBit() clearBit()
TEST(TestBitvector, TestSetBitAndGetBitFunctions) {
  unialgo::utils::Bitvector bv(100);

  bv.SetBit(32);

  EXPECT_EQ(bv.GetBit(32), 1);
  EXPECT_EQ(bv[32], bv.GetBit(32));
  for (int i = 0; i < 32; ++i) EXPECT_EQ(bv[i], 0);
  for (int i = 33; i < 128; ++i) EXPECT_EQ(bv[i], 0);

  bv[32] = 0;

  for (int i = 0; i < 128; ++i) EXPECT_EQ(bv[i], 0);
}

// Testing BitvectorReference getValue() function
TEST(TestBitvectorReference, TestGetValue) {
  unialgo::utils::Bitvector bv(10);
  bv[9] = 1;

  auto bvR = bv[9];

  EXPECT_EQ(bvR.getValue(), 1);
}

// Testing BitvectorReference operator== between bv, bv and bv, value
TEST(TestBitvectorReference, TestCompOperator) {
  unialgo::utils::Bitvector bv(10);
  bv[9] = 1;
  bv[8] = 1;

  auto bvR8 = bv[8];
  auto bvR9 = bv[9];

  EXPECT_EQ(bvR8 == bvR9, 1);
  EXPECT_EQ(bvR8, bvR9);
  EXPECT_EQ(bv[0], bv[1]);
  EXPECT_EQ(bv[0] == bv[1], 1);
}

// ============ Testing WordVector ============

// Testing getNumBits
TEST(TestingWordVector, getNumBits) {
  unialgo::utils::WordVector<2> wv(10);

  EXPECT_EQ(wv.getNumBits(), 20);
}

TEST(TestingWordVector, accessOperator) {
  unialgo::utils::WordVector<2> wv(10);

  wv[0] = 1;

  EXPECT_EQ(wv[0].getValue(), 1);
  for (int i = 1; i < 10; ++i) EXPECT_EQ(wv[i].getValue(), 0);
}

// testing that only the referenced 2 bits could change
TEST(TestingWordVector, accessingOverBitSize) {
  unialgo::utils::WordVector<2> wv(10);

  wv[0] = 8;

  for (int i = 0; i < 10; ++i) EXPECT_EQ(wv[i].getValue(), 0);
}

// testing operator == in reference
TEST(TestingWordVector, referenceComparingWithArithmeticType) {
  unialgo::utils::WordVector<2> wv(10);

  wv[0] = 8;
  wv[2] = 1;
  wv[3] = 3;

  EXPECT_EQ(wv[0], 0);
  EXPECT_EQ(wv[1], 0);
  EXPECT_EQ(wv[2], 1);
  EXPECT_EQ(wv[3], 3);
  for (int i = 4; i < 10; ++i) EXPECT_EQ(wv[i], 0);
}

// testing operator == in reference
TEST(TestingWordVector, compareRefRef) {
  unialgo::utils::WordVector<2> wv(10);

  wv[0] = 2;
  wv[1] = 2;
  wv[2] = 1;
  wv[3] = 1;

  EXPECT_EQ(wv[0], 2);
  EXPECT_EQ(wv[1], 2);
  EXPECT_EQ(wv[2], 1);
  EXPECT_EQ(wv[3], 1);
  for (int i = 4; i < 10; ++i) EXPECT_EQ(wv[i], 0);

  EXPECT_NE(wv[0], wv[2]);
  EXPECT_NE(wv[2], wv[0]);
  EXPECT_EQ(wv[0], wv[1]);
  EXPECT_EQ(wv[2], wv[3]);
}

// testing compare <, >, <=, >=, in reference
TEST(TestingWordVector, compareRefValues) {
  unialgo::utils::WordVector<2> wv(10);

  wv[0] = 0;
  wv[1] = 1;
  wv[2] = 2;
  wv[3] = 3;

  EXPECT_EQ(wv[0], 0);
  EXPECT_EQ(wv[1], 1);
  EXPECT_EQ(wv[2], 2);
  EXPECT_EQ(wv[3], 3);
  for (int i = 4; i < 10; ++i) EXPECT_EQ(wv[i], 0);

  EXPECT_TRUE(wv[0] <= wv[0]);
  EXPECT_TRUE(wv[0] < wv[3]);
  EXPECT_TRUE(wv[0] <= wv[3]);
  EXPECT_TRUE(wv[3] > wv[2]);
  EXPECT_TRUE(wv[3] >= wv[2]);
}

}  // namespace