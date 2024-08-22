#include <gtest/gtest.h>

#include <algorithm>  // std::sort
#include <cmath>
#include <memory>   // std::shared_ptr
#include <utility>  // std::swap

#include "unialgo/utils/bitvector/bitVectors.hpp"

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

// Testing operator &, &=
TEST(TestBitvector, BitVectorAndOperator) {
  unialgo::utils::Bitvector bv(100);

  bv[64] = 1;
  EXPECT_EQ(bv[64], 1);

  bv[30] = 1;
  bv[31] = 1;
  bv[32] = 0;
  EXPECT_EQ(bv[30], 1);
  EXPECT_EQ(bv[31], 1);
  EXPECT_EQ(bv[32], 0);

  unialgo::utils::Bitvector bv2(100);
  bv2[30] = 1;
  bv2[31] = 0;
  bv2[32] = 0;
  EXPECT_EQ(bv2[30], 1);
  EXPECT_EQ(bv2[31], 0);
  EXPECT_EQ(bv2[32], 0);

  unialgo::utils::Bitvector res = bv & bv2;
  EXPECT_EQ(bv.getNumBits(), res.getNumBits());
  EXPECT_EQ(res[30], 1);
  EXPECT_EQ(res[31], 0);

  bv &= bv2;
  EXPECT_EQ(bv[30], 1);
  EXPECT_EQ(bv[31], 0);
}

// Testing operator >=
TEST(TestBitvector, BitVectorRightShiftOperator) {
  unialgo::utils::Bitvector bv(100);

  bv[32] = 1;
  bv[31] = 1;

  EXPECT_EQ(bv[32], 1);
  EXPECT_EQ(bv[31], 1);
  for (int i = 0; i < 31; ++i) EXPECT_EQ(bv[i], 0);
  for (int i = 33; i < 128; ++i) EXPECT_EQ(bv[i], 0);

  bv >= 1;

  EXPECT_EQ(bv[31], 1);
  EXPECT_EQ(bv[30], 1);
  for (int i = 0; i < 30; ++i) EXPECT_EQ(bv[i], 0);
  for (int i = 32; i < 128; ++i) EXPECT_EQ(bv[i], 0);
}

// Testing operator >=
TEST(TestBitvector, rightShiftBetweenWords) {
  unialgo::utils::Bitvector bv(100);

  bv[32] = 1;
  bv[31] = 1;
  bv[64] = 1;
  bv[65] = 1;

  EXPECT_EQ(bv[31], 1);
  EXPECT_EQ(bv[32], 1);
  EXPECT_EQ(bv[64], 1);
  EXPECT_EQ(bv[65], 1);
  for (int i = 0; i < 31; ++i) EXPECT_EQ(bv[i], 0);
  for (int i = 33; i < 64; ++i) EXPECT_EQ(bv[i], 0);
  for (int i = 66; i < 128; ++i) EXPECT_EQ(bv[i], 0);

  bv >= 1;

  EXPECT_EQ(bv[30], 1);
  EXPECT_EQ(bv[31], 1);
  EXPECT_EQ(bv[63], 1);
  EXPECT_EQ(bv[64], 1);
  for (int i = 0; i < 30; ++i) EXPECT_EQ(bv[i], 0);
  for (int i = 32; i < 63; ++i) EXPECT_EQ(bv[i], 0);
  for (int i = 65; i < 128; ++i) EXPECT_EQ(bv[i], 0);
}

// Testing operator >=
TEST(TestBitvector, rightShiftValue) {
  unialgo::utils::Bitvector bv(100);

  bv[32] = 1;
  bv[31] = 1;
  bv[64] = 1;
  bv[65] = 1;
  bv[66] = 1;

  EXPECT_EQ(bv[31], 1);
  EXPECT_EQ(bv[32], 1);
  EXPECT_EQ(bv[64], 1);
  EXPECT_EQ(bv[65], 1);
  EXPECT_EQ(bv[66], 1);
  for (int i = 0; i < 31; ++i) EXPECT_EQ(bv[i], 0);
  for (int i = 33; i < 64; ++i) EXPECT_EQ(bv[i], 0);
  for (int i = 67; i < 128; ++i) EXPECT_EQ(bv[i], 0);

  bv >= 2;

  EXPECT_EQ(bv[29], 1);
  EXPECT_EQ(bv[30], 1);
  EXPECT_EQ(bv[62], 1);
  EXPECT_EQ(bv[63], 1);
  EXPECT_EQ(bv[64], 1);
  for (int i = 0; i < 29; ++i) EXPECT_EQ(bv[i], 0);
  for (int i = 31; i < 62; ++i) EXPECT_EQ(bv[i], 0);
  for (int i = 65; i < 128; ++i) EXPECT_EQ(bv[i], 0);
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

// testing ostream on reference
TEST(TestBitvector, ostream) {
  unialgo::utils::Bitvector bv(10);

  bv[9] = 1;
  bv[8] = 1;

  EXPECT_EQ(bv[9], 1);
  EXPECT_EQ(bv[8], 1);
  for (int i = 0; i < 8; ++i) EXPECT_EQ(bv[i], 0);

  std::ostringstream oss;
  oss << bv;
  EXPECT_EQ(oss.str(), "1100000000");
}

// Testing Iterator
TEST(TestBitvector, TestIterator) {
  unialgo::utils::Bitvector bv(100);

  bv.SetBit(32);

  EXPECT_EQ(bv.GetBit(32), 1);
  EXPECT_EQ(bv[32], bv.GetBit(32));
  unialgo::utils::Bitvector::Iterator it = bv.begin();
  for (int i = 0; i < 32; ++i) {
    EXPECT_EQ(*it, 0);
    ++it;
  }
  EXPECT_EQ(*it, 1);
  (*it) = 0;
  ++it;
  EXPECT_EQ(bv[32], 0);
  for (int i = 33; i < 128; ++i) {
    EXPECT_EQ(*it, 0);
    ++it;
  }
}

// Testing ConstIterator
TEST(TestBitvector, TestConstIterator) {
  unialgo::utils::Bitvector bv(100);

  bv.SetBit(32);

  EXPECT_EQ(bv.GetBit(32), 1);
  EXPECT_EQ(bv[32], bv.GetBit(32));
  unialgo::utils::Bitvector::ConstIterator it = bv.cbegin();
  for (int i = 0; i < 32; ++i) {
    EXPECT_EQ(*it, 0);
    ++it;
  }
  EXPECT_EQ(*it, 1);
  ++it;
  for (int i = 33; i < 128; ++i) {
    EXPECT_EQ(*it, 0);
    ++it;
  }
}

TEST(TestBitvector, TestingIteratorEnd) {
  unialgo::utils::Bitvector bv(100);

  for (auto it = bv.begin(); it != bv.end(); ++it) *it = 1;

  for (int i = 0; i < 100; ++i) EXPECT_EQ(bv[i], 1);

  EXPECT_EQ(bv[100], 0);
}

// operator()(start, end)
TEST(TestBitvector, TestingOperatorBits) {
  unialgo::utils::Bitvector bv(100);
  bv[0] = 1;
  bv[1] = 1;
  bv[2] = 1;
  bv[3] = 1;
  EXPECT_EQ(bv[0], 1);
  EXPECT_EQ(bv[1], 1);
  EXPECT_EQ(bv[2], 1);
  EXPECT_EQ(bv[3], 1);

  unialgo::utils::Bitvector v2 = bv(0, 3);

  EXPECT_EQ(v2.getNumBits(), 4);
  EXPECT_EQ(v2[0], 1);
  EXPECT_EQ(v2[1], 1);
  EXPECT_EQ(v2[2], 1);
  EXPECT_EQ(v2[3], 1);
  // test getting a full word
  v2 = bv(0, 64);
  EXPECT_EQ(v2.getNumBits(), 65);
  EXPECT_EQ(v2[0], 1);
  EXPECT_EQ(v2[1], 1);
  EXPECT_EQ(v2[2], 1);
  EXPECT_EQ(v2[3], 1);
  for (int i = 4; i < 64; ++i) EXPECT_EQ(v2[i], 0);

  // making sure bv is not changed (operator() is const)
  EXPECT_EQ(bv[0], 1);
  EXPECT_EQ(bv[1], 1);
  EXPECT_EQ(bv[2], 1);
  EXPECT_EQ(bv[3], 1);
}

// operator()(start, end)
TEST(TestBitvector, TestingOperatorBitsOverflowing) {
  unialgo::utils::Bitvector bv(100);
  bv[62] = 1;
  bv[63] = 1;
  bv[64] = 1;
  bv[65] = 1;
  EXPECT_EQ(bv[62], 1);
  EXPECT_EQ(bv[63], 1);
  EXPECT_EQ(bv[64], 1);
  EXPECT_EQ(bv[65], 1);

  unialgo::utils::Bitvector v2 = bv(62, 65);

  EXPECT_EQ(v2.getNumBits(), 4);
  EXPECT_EQ(v2[0], 1);
  EXPECT_EQ(v2[1], 1);
  EXPECT_EQ(v2[2], 1);
  EXPECT_EQ(v2[3], 1);
}

// operator()(start, end)
TEST(TestBitvector, TestingOperatorBitsLong) {
  unialgo::utils::Bitvector bv(100);

  for (auto it = bv.begin(); it != bv.end(); ++it) *it = 1;

  bv[12] = 0;
  bv[98] = 0;

  unialgo::utils::Bitvector v2 = bv(12, 98);

  EXPECT_EQ(v2.getNumBits(), 87);
  EXPECT_EQ(v2[0], 0);
  for (int i = 1; i < 86; ++i) EXPECT_EQ(v2[i], 1);
  EXPECT_EQ(v2[86], 0);
}

// ============ Testing WordVector ============

// Testing getters
TEST(TestingWordVector, getters) {
  unialgo::utils::WordVector wv(10);

  EXPECT_EQ(wv.getNumBits(), 20);
  EXPECT_EQ(wv.getWordSize(), 2);
  EXPECT_EQ(wv.size(), 10);
}

TEST(TestingWordVector, accessOperator) {
  unialgo::utils::WordVector wv(10);

  wv[0] = 1;

  EXPECT_EQ(wv[0].getValue(), 1);
  for (int i = 1; i < 10; ++i) EXPECT_EQ(wv[i].getValue(), 0);
}

// testing that only the referenced 2 bits could change
TEST(TestingWordVector, accessingOverBitSize) {
  unialgo::utils::WordVector wv(10);

  wv[0] = 8;

  for (int i = 0; i < 10; ++i) EXPECT_EQ(wv[i].getValue(), 0);
}

// testing operator == in reference
TEST(TestingWordVector, referenceComparingWithArithmeticType) {
  unialgo::utils::WordVector wv(10);

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
  unialgo::utils::WordVector wv(10);

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

  unialgo::utils::WordVectorConstReference a = wv[0];
  // a = wv[0];
  EXPECT_EQ(a, wv[0]);
}

// testing compare <, >, <=, >=, in reference
TEST(TestingWordVector, compareRefValues) {
  unialgo::utils::WordVector wv(10);

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

// TEST(TestingWordVector, refAssignmentOp) {
//   unialgo::utils::WordVector wv(10);

//   wv[0] = 0;
//   wv[1] = 1;
//   wv[2] = 2;
//   wv[3] = 3;

//   EXPECT_EQ(wv[0], 0);
//   EXPECT_EQ(wv[1], 1);
//   EXPECT_EQ(wv[2], 2);
//   EXPECT_EQ(wv[3], 3);
//   for (int i = 4; i < 10; ++i) EXPECT_EQ(wv[i], 0);

//   unialgo::utils::WordVectorConstReference a(wv[1]);
//   // a = wv[1];
//   wv[0] = a;
//   // wv[0] = wv[1];
//   EXPECT_EQ(wv[0], 1);
//   EXPECT_EQ(wv[1], 1);
//   // wv[2] = *(++wv.begin());
//   wv[2] = wv[1];
//   EXPECT_EQ(wv[2], 1);
//   EXPECT_EQ(wv[3], 3);
// }

// // testing all combination of operator= on wordvectorreference
// TEST(TestingWordVector, refAssignmentOpAll) {
//   unialgo::utils::WordVector wv(10);
//   wv[0] = 0;
//   wv[1] = 1;
//   wv[2] = 2;
//   wv[3] = 3;

//   EXPECT_EQ(wv[0], 0);
//   EXPECT_EQ(wv[1], 1);
//   EXPECT_EQ(wv[2], 2);
//   EXPECT_EQ(wv[3], 3);
//   for (int i = 4; i < 10; ++i) EXPECT_EQ(wv[i], 0);

//   // changing value and checking if reference is updated:
//   unialgo::utils::WordVectorConstReference a(wv[1]);
//   EXPECT_EQ(a, 1);
//   wv[1] = 0;
//   EXPECT_EQ(a, 0);
//   wv[1] = 1;

//   wv[7] = wv[1];
//   EXPECT_EQ(wv[7], 1);

//   // assigning rvalue to lvalue reference const and non const:
//   unialgo::utils::WordVectorReference b(wv[1]);
//   EXPECT_EQ(b, 1);
//   wv[0] = b;
//   EXPECT_EQ(wv[0], 1);
//   wv[0] = 0;
//   EXPECT_EQ(wv[0], 0);
//   wv[0] = a;
//   EXPECT_EQ(wv[0], 1);

//   // creating reference assiging value and checking if value updates in wv:
//   unialgo::utils::WordVectorReference c = wv[2];
//   EXPECT_EQ(c, 2);
//   c = a;
//   EXPECT_EQ(c, 1);
//   EXPECT_EQ(wv[2], 1);

//   // creating const reference and assigning reference
//   // checking referenced value is changed in obj but value
//   // is not updated in wv:
//   wv[5] = 2;
//   unialgo::utils::WordVectorConstReference d = wv[5];
//   EXPECT_EQ(d, 2);
//   d = b;
//   EXPECT_EQ(d, 1);
//   EXPECT_EQ(wv[5], 2);
// }

// testing iterator assignment
TEST(TestingWordVector, iteratorAssignment) {
  unialgo::utils::WordVector wv(10);
  wv[0] = 0;
  wv[1] = 1;
  wv[2] = 2;
  wv[3] = 3;

  EXPECT_EQ(wv[0], 0);
  EXPECT_EQ(wv[1], 1);
  EXPECT_EQ(wv[2], 2);
  EXPECT_EQ(wv[3], 3);
  for (int i = 4; i < 10; ++i) EXPECT_EQ(wv[i], 0);

  wv[1] = *(wv.begin());
  EXPECT_EQ(wv[0], 0);
  EXPECT_EQ(wv[1], 0);
  EXPECT_EQ(wv[2], 2);
  EXPECT_EQ(wv[3], 3);
  wv[1] = 1;
  EXPECT_EQ(wv[1], 1);

  unialgo::utils::WordVector::Iterator it = wv.begin();
  ++it;
  EXPECT_EQ(*it, 1);
  unialgo::utils::WordVector::Iterator it2 = wv.begin();
  ++it2;
  ++it2;
  EXPECT_EQ(*it2, 2);

  it = it2;
  EXPECT_EQ(*it, 2);

  unialgo::utils::WordVector::ConstIterator cit = wv.cbegin();
  ++cit;
  EXPECT_EQ(*cit, 1);
  unialgo::utils::WordVector::ConstIterator cit2 = wv.cbegin();
  ++cit2;
  ++cit2;
  EXPECT_EQ(*cit2, 2);

  cit = cit2;
  EXPECT_EQ(*cit, 2);
}

// testing swap on reference

// testing swap between references
TEST(TestingWordVector, swapRefRef) {
  unialgo::utils::WordVector wv(10);

  wv[0] = 1;
  wv[1] = 2;

  EXPECT_EQ(wv[0], 1);
  EXPECT_EQ(wv[1], 2);

  unialgo::utils::WordVectorReference a = wv[0];
  unialgo::utils::WordVectorReference b = wv[1];

  EXPECT_EQ(a, 1);
  EXPECT_EQ(b, 2);

  std::swap(a, b);

  EXPECT_EQ(a, 2);
  EXPECT_EQ(b, 1);

  EXPECT_EQ(wv[0], 2);
  EXPECT_EQ(wv[1], 1);
}

// testing iterator
TEST(TestingWordVector, testingIterator) {
  unialgo::utils::WordVector wv(10);

  wv[0] = 0;
  wv[1] = 1;
  wv[2] = 2;
  wv[3] = 3;
  wv[4] = 0;
  wv[5] = 1;
  wv[6] = 2;
  wv[7] = 3;
  wv[8] = 0;
  wv[9] = 1;
  auto i = 0;
  for (auto it = wv.begin(); it != wv.end(); ++it) {
    EXPECT_EQ(*it, (i % 4));
    ++i;
  }
}

// testing iter_swap
TEST(TestingWordVector, iter_swap) {
  unialgo::utils::WordVector wv(10);
  wv[0] = 0;
  wv[1] = 1;
  wv[2] = 2;
  EXPECT_EQ(wv[0], 0);
  EXPECT_EQ(wv[1], 1);
  EXPECT_EQ(wv[2], 2);

  std::iter_swap(wv.begin(), wv.begin() + 1);
  EXPECT_EQ(wv[0], 1);
  EXPECT_EQ(wv[1], 0);
  EXPECT_EQ(wv[2], 2);
}

// testing iterator
TEST(TestingWordVector, testingIteratorIncrement) {
  unialgo::utils::WordVector wv(10);

  wv[0] = 0;
  wv[1] = 1;
  wv[2] = 2;
  wv[3] = 3;
  wv[4] = 0;
  wv[5] = 1;
  wv[6] = 2;
  wv[7] = 3;
  wv[8] = 0;
  wv[9] = 1;
  auto it = wv.begin();
  EXPECT_EQ(*(it++), 0);
  EXPECT_EQ(*(it), 1);
  EXPECT_EQ(*++it, 2);
}

// testing iterator end
TEST(TestingWordVector, testingIteratorEnd) {
  unialgo::utils::WordVector wv(10);

  wv[9] = 1;
  auto it = wv.end();
  --it;
  EXPECT_EQ(*it, 1);
  EXPECT_EQ(std::next(wv.begin(), 10), wv.end());
}

// testing iterator end
TEST(TestingWordVector, testingIteratorValueType) {
  unialgo::utils::WordVector wv(10);
  wv[0] = 2;
  wv[1] = 1;
  EXPECT_EQ(wv[0], 2);
  EXPECT_EQ(wv[1], 1);

  auto it = wv.begin();
  auto it2 = wv.begin();
  ++it2;
  EXPECT_EQ(*it, 2);
  EXPECT_EQ(*it2, 1);

  unialgo::utils::WordVector::Iterator::value_type val = std::move(*it);
  ::testing::StaticAssertTypeEq<decltype(val), unsigned long long int>();
  EXPECT_EQ(val, 2);

  *it = std::move(*it2);
  EXPECT_EQ(*it, 1);
  EXPECT_EQ(*it2, 1);
  EXPECT_EQ(val, 2);

  *it2 = std::move(val);
  EXPECT_EQ(*it, 1);
  EXPECT_EQ(*it2, 2);
  EXPECT_EQ(val, 2);

  EXPECT_EQ(wv[0], 1);
  EXPECT_EQ(wv[1], 2);
}

// testing move_backwords
TEST(TestingWordVector, testingMoveBackwards) {
  unialgo::utils::WordVector wv(10);

  wv[0] = 0;
  wv[1] = 1;
  wv[2] = 2;
  wv[3] = 3;
  wv[4] = 0;
  EXPECT_EQ(wv[0], 0);
  EXPECT_EQ(wv[1], 1);
  EXPECT_EQ(wv[2], 2);
  EXPECT_EQ(wv[3], 3);
  EXPECT_EQ(wv[4], 0);

  auto first = wv.begin();
  auto last = std::next(wv.begin(), 3);
  auto result = wv.end();

  std::move_backward(first, last, result);

  EXPECT_EQ(wv[9], 2);
  EXPECT_EQ(wv[8], 1);
  EXPECT_EQ(wv[7], 0);
}

TEST(TestingWordVector, testingSwap) {
  unialgo::utils::WordVector wv(10);

  wv[0] = 0;
  wv[1] = 1;
  wv[2] = 2;
  wv[3] = 3;

  EXPECT_EQ(wv[0], 0);
  EXPECT_EQ(wv[1], 1);
  EXPECT_EQ(wv[2], 2);
  EXPECT_EQ(wv[3], 3);
  auto it = wv.begin();
  EXPECT_EQ(*it, 0);
  ++it;
  EXPECT_EQ(*it, 1);
  ++it;
  EXPECT_EQ(*it, 2);
  ++it;
  EXPECT_EQ(*it, 3);
  ++it;
  auto second = wv.begin();
  ++second;
  swap(*wv.begin(), *second);
  EXPECT_EQ(wv[0], 1);
  EXPECT_EQ(wv[1], 0);
  std::iter_swap(wv.begin(), second);
  EXPECT_EQ(wv[0], 0);
  EXPECT_EQ(wv[1], 1);
}

// testing operator++, operator--
TEST(TestingWordVector, testIncrementDecrement) {
  unialgo::utils::WordVector wv(10);

  wv[0] = 0;
  wv[1] = 1;
  wv[2] = 2;
  wv[3] = 3;

  EXPECT_EQ(wv[0], 0);
  EXPECT_EQ(wv[1], 1);
  EXPECT_EQ(wv[2], 2);
  EXPECT_EQ(wv[3], 3);
  for (int i = 4; i < 10; ++i) EXPECT_EQ(wv[i], 0);

  wv[0]++;
  EXPECT_NE(wv[0], 0);
  EXPECT_EQ(wv[0], 1);

  EXPECT_NE(wv[0]++, 2);
  EXPECT_EQ(wv[0], 2);

  EXPECT_NE(++wv[0], 2);
  EXPECT_EQ(wv[0], 3);

  wv[0]--;
  EXPECT_NE(wv[0], 3);
  EXPECT_EQ(wv[0], 2);

  EXPECT_NE(wv[0]--, 1);
  EXPECT_EQ(wv[0], 1);

  EXPECT_NE(--wv[0], 1);
  EXPECT_EQ(wv[0], 0);

  EXPECT_EQ(wv[0], 0);
  wv[0]--;
  EXPECT_EQ(wv[0], 3);
}

// testing reference cast to int
TEST(TestingWordVector, testingReferenceCast) {
  unialgo::utils::WordVector wv(10);

  wv[0] = 0;
  wv[1] = 1;
  wv[2] = 2;

  EXPECT_EQ(wv[0], 0);
  EXPECT_EQ(wv[1], 1);
  EXPECT_EQ(wv[2], 2);

  int first = wv[0];
  int second = wv[1];
  int third = wv[2];

  EXPECT_EQ(first, 0);
  EXPECT_EQ(second, 1);
  EXPECT_EQ(third, 2);

  std::size_t sizet = wv[0];
  EXPECT_EQ(sizet, 0);
}

// testing ostream on reference
TEST(TestingWordVector, ostreamWvRef) {
  unialgo::utils::WordVector wv(10);

  wv[0] = 0;
  wv[1] = 1;
  wv[2] = 2;
  wv[3] = 3;

  EXPECT_EQ(wv[0], 0);
  EXPECT_EQ(wv[1], 1);
  EXPECT_EQ(wv[2], 2);
  EXPECT_EQ(wv[3], 3);
  for (int i = 4; i < 10; ++i) EXPECT_EQ(wv[i], 0);

  std::ostringstream oss;
  oss << wv[0];
  EXPECT_EQ(oss.str(), "00");
  oss << wv[1];
  EXPECT_EQ(oss.str(), "0001");
  oss << wv[2];
  EXPECT_EQ(oss.str(), "000110");
  oss << wv[3];
  EXPECT_EQ(oss.str(), "00011011");
}

// testing ostream on wordvector
TEST(TestingWordVector, ostreamWv) {
  unialgo::utils::WordVector wv(10);

  wv[0] = 0;
  wv[1] = 1;
  wv[2] = 2;
  wv[3] = 3;

  EXPECT_EQ(wv[0], 0);
  EXPECT_EQ(wv[1], 1);
  EXPECT_EQ(wv[2], 2);
  EXPECT_EQ(wv[3], 3);
  for (int i = 4; i < 10; ++i) EXPECT_EQ(wv[i], 0);

  std::ostringstream oss;
  oss << wv;
  EXPECT_EQ(oss.str(), "00000000000011100100");
}

// testing hasing funciton on reference
TEST(TestingWordVector, hashFunction) {
  unialgo::utils::WordVector wv(10);

  wv[0] = 0;
  wv[1] = 0;
  wv[2] = 2;
  wv[3] = 3;

  EXPECT_EQ(wv[0], 0);
  EXPECT_EQ(wv[1], 0);
  EXPECT_EQ(wv[2], 2);
  EXPECT_EQ(wv[3], 3);
  for (int i = 4; i < 10; ++i) EXPECT_EQ(wv[i], 0);

  EXPECT_EQ(std::hash<unialgo::utils::WordVectorReference>{}(wv[0]),
            std::hash<unialgo::utils::WordVectorReference>{}(wv[0]));
  EXPECT_EQ(std::hash<unialgo::utils::WordVectorReference>{}(wv[0]),
            std::hash<unialgo::utils::WordVectorReference>{}(wv[1]));
  EXPECT_NE(std::hash<unialgo::utils::WordVectorReference>{}(wv[0]),
            std::hash<unialgo::utils::WordVectorReference>{}(wv[2]));
  EXPECT_NE(std::hash<unialgo::utils::WordVectorReference>{}(wv[0]),
            std::hash<unialgo::utils::WordVectorReference>{}(wv[3]));
  EXPECT_NE(std::hash<unialgo::utils::WordVectorReference>{}(wv[3]),
            std::hash<unialgo::utils::WordVectorReference>{}(wv[2]));
}

// testing const reference
TEST(TestingWordVector, constReference) {
  unialgo::utils::WordVector wv(10);

  wv[0] = 0;
  wv[1] = 0;
  wv[2] = 2;
  wv[3] = 3;

  EXPECT_EQ(wv[0], 0);
  EXPECT_EQ(wv[1], 0);
  EXPECT_EQ(wv[2], 2);
  EXPECT_EQ(wv[3], 3);
  for (int i = 4; i < 10; ++i) EXPECT_EQ(wv[i], 0);

  const unialgo::utils::WordVector constWv(wv);
  EXPECT_EQ(constWv[0], 0);
  EXPECT_EQ(constWv[1], 0);
  EXPECT_EQ(constWv[2], 2);
  EXPECT_EQ(constWv[3], 3);
  for (int i = 4; i < 10; ++i) EXPECT_EQ(constWv[i], 0);
}

// testing hasing funciton on ConstReference
TEST(TestingWordVector, hashFunctionConstReference) {
  unialgo::utils::WordVector wv(10);

  wv[0] = 0;
  wv[1] = 0;
  wv[2] = 2;
  wv[3] = 3;

  EXPECT_EQ(wv[0], 0);
  EXPECT_EQ(wv[1], 0);
  EXPECT_EQ(wv[2], 2);
  EXPECT_EQ(wv[3], 3);
  for (int i = 4; i < 10; ++i) EXPECT_EQ(wv[i], 0);

  const unialgo::utils::WordVector constWv(wv);
  EXPECT_EQ(constWv[0], 0);
  EXPECT_EQ(constWv[1], 0);
  EXPECT_EQ(constWv[2], 2);
  EXPECT_EQ(constWv[3], 3);
  for (int i = 4; i < 10; ++i) EXPECT_EQ(constWv[i], 0);

  EXPECT_EQ(std::hash<unialgo::utils::WordVectorConstReference>{}(constWv[0]),
            std::hash<unialgo::utils::WordVectorConstReference>{}(constWv[0]));
  EXPECT_EQ(std::hash<unialgo::utils::WordVectorConstReference>{}(constWv[0]),
            std::hash<unialgo::utils::WordVectorConstReference>{}(constWv[1]));
  EXPECT_NE(std::hash<unialgo::utils::WordVectorConstReference>{}(constWv[0]),
            std::hash<unialgo::utils::WordVectorConstReference>{}(constWv[2]));
  EXPECT_NE(std::hash<unialgo::utils::WordVectorConstReference>{}(constWv[0]),
            std::hash<unialgo::utils::WordVectorConstReference>{}(constWv[3]));
  EXPECT_NE(std::hash<unialgo::utils::WordVectorConstReference>{}(constWv[3]),
            std::hash<unialgo::utils::WordVectorConstReference>{}(constWv[2]));
}

// ============ Testing BitVector Helpers ============

TEST(TestingHelpers, TestingRankHelper) {
  auto bv = std::make_shared<unialgo::utils::Bitvector>(100);
  bv->operator[](1) = 1;
  bv->operator[](9) = 1;
  bv->operator[](10) = 1;
  bv->operator[](99) = 1;
  unialgo::utils::RankHelper helper(bv);
  EXPECT_EQ(helper.rank(0), 0);
  EXPECT_EQ(helper.rank(6), 1);
  EXPECT_EQ(helper.rank(16), 3);
  EXPECT_EQ(helper.rank(99), 4);
}

TEST(TestingHelpers, TestingSize) {
  auto bv = std::make_shared<unialgo::utils::Bitvector>(101);
  bv->operator[](1) = 1;
  bv->operator[](9) = 1;
  bv->operator[](10) = 1;
  bv->operator[](99) = 1;
  unialgo::utils::RankHelper helper(bv);
  EXPECT_EQ(helper.rank(0), 0);
  EXPECT_EQ(helper.rank(6), 1);
  EXPECT_EQ(helper.rank(16), 3);
  EXPECT_EQ(helper.rank(99), 4);
  EXPECT_EQ(helper.rank(100), 4);
  bv->operator[](100) = 1;
  unialgo::utils::RankHelper helper2(bv);
  EXPECT_EQ(helper.rank(0), 0);
  EXPECT_EQ(helper.rank(6), 1);
  EXPECT_EQ(helper.rank(16), 3);
  EXPECT_EQ(helper.rank(99), 4);
  EXPECT_EQ(helper.rank(100), 4);
}

}  // namespace