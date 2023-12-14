#include <gtest/gtest.h>

#include "unialgo/utils/bitvector/bitvector.hpp"

namespace {

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

}  // namespace