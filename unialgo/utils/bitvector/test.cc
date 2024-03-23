#include <gtest/gtest.h>

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

// testing iterator
TEST(TestingWordVector, testingIterator) {
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

  EXPECT_EQ(std::hash<unialgo::utils::WordVectorReference>{}(constWv[0]),
            std::hash<unialgo::utils::WordVectorReference>{}(constWv[0]));
  EXPECT_EQ(std::hash<unialgo::utils::WordVectorReference>{}(constWv[0]),
            std::hash<unialgo::utils::WordVectorReference>{}(constWv[1]));
  EXPECT_NE(std::hash<unialgo::utils::WordVectorReference>{}(constWv[0]),
            std::hash<unialgo::utils::WordVectorReference>{}(constWv[2]));
  EXPECT_NE(std::hash<unialgo::utils::WordVectorReference>{}(constWv[0]),
            std::hash<unialgo::utils::WordVectorReference>{}(constWv[3]));
  EXPECT_NE(std::hash<unialgo::utils::WordVectorReference>{}(constWv[3]),
            std::hash<unialgo::utils::WordVectorReference>{}(constWv[2]));
}

}  // namespace