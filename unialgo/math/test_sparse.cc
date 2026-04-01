#include <gtest/gtest.h>

#include <vector>

#include "unialgo/math/sparseMatrix.hpp"

namespace {

// ============ Testing SparseMatrix ============

// testing contructor from 1d vector
TEST(TestSparseMatrix, Constructor1d) {
  std::vector<double> v({1, 2, 3, 4, 5, 6});

  unialgo::math::SparseMatrix<unialgo::math::layout_right> matRight(v, 2, 3);
  // 1 2 3
  // 4 5 6
  EXPECT_EQ(matRight(0, 0), 1.0);
  EXPECT_EQ(matRight(0, 1), 2.0);
  EXPECT_EQ(matRight(0, 2), 3.0);
  EXPECT_EQ(matRight(1, 0), 4.0);
  EXPECT_EQ(matRight(1, 1), 5.0);
  EXPECT_EQ(matRight(1, 2), 6.0);
  EXPECT_EQ(matRight.rows(), 2);
  EXPECT_EQ(matRight.cols(), 3);
  EXPECT_EQ(matRight.size().first, 2);
  EXPECT_EQ(matRight.size().second, 3);

  unialgo::math::SparseMatrix<unialgo::math::layout_left> matLeft(v, 2, 3);
  // 1 3 5
  // 2 4 6
  EXPECT_EQ(matLeft(0, 0), 1.0);
  EXPECT_EQ(matLeft(0, 1), 3.0);
  EXPECT_EQ(matLeft(0, 2), 5.0);
  EXPECT_EQ(matLeft(1, 0), 2.0);
  EXPECT_EQ(matLeft(1, 1), 4.0);
  EXPECT_EQ(matLeft(1, 2), 6.0);
  EXPECT_EQ(matLeft.rows(), 2);
  EXPECT_EQ(matLeft.cols(), 3);
  EXPECT_EQ(matLeft.size().first, 2);
  EXPECT_EQ(matLeft.size().second, 3);

  unialgo::math::SparseMatrix<unialgo::math::layout_left> matLeft2(v, 3, 2);
  // 1 4
  // 2 5
  // 3 6
  EXPECT_EQ(matLeft2(0, 0), 1.0);
  EXPECT_EQ(matLeft2(0, 1), 4.0);
  EXPECT_EQ(matLeft2(1, 0), 2.0);
  EXPECT_EQ(matLeft2(1, 1), 5.0);
  EXPECT_EQ(matLeft2(2, 0), 3.0);
  EXPECT_EQ(matLeft2(2, 1), 6.0);
  EXPECT_EQ(matLeft2.rows(), 3);
  EXPECT_EQ(matLeft2.cols(), 2);
  EXPECT_EQ(matLeft2.size().first, 3);
  EXPECT_EQ(matLeft2.size().second, 2);
}

// testing constructo for 2d matrix
TEST(TestSparseMatrix, Constructor2d) {
  std::vector<std::vector<double>> v({{1, 2, 3}, {4, 5, 6}});

  unialgo::math::SparseMatrix<unialgo::math::layout_right> matRight(v);
  // 1 2 3
  // 4 5 6
  EXPECT_EQ(matRight(0, 0), 1.0);
  EXPECT_EQ(matRight(0, 1), 2.0);
  EXPECT_EQ(matRight(0, 2), 3.0);
  EXPECT_EQ(matRight(1, 0), 4.0);
  EXPECT_EQ(matRight(1, 1), 5.0);
  EXPECT_EQ(matRight(1, 2), 6.0);
  EXPECT_EQ(matRight.rows(), 2);
  EXPECT_EQ(matRight.cols(), 3);
  EXPECT_EQ(matRight.size().first, 2);
  EXPECT_EQ(matRight.size().second, 3);

  unialgo::math::SparseMatrix<unialgo::math::layout_left> matLeft(v);
  // 1 4
  // 2 5
  // 3 6
  EXPECT_EQ(matLeft(0, 0), 1.0);
  EXPECT_EQ(matLeft(0, 1), 4.0);
  EXPECT_EQ(matLeft(1, 0), 2.0);
  EXPECT_EQ(matLeft(1, 1), 5.0);
  EXPECT_EQ(matLeft(2, 0), 3.0);
  EXPECT_EQ(matLeft(2, 1), 6.0);
  EXPECT_EQ(matLeft.rows(), 3);
  EXPECT_EQ(matLeft.cols(), 2);
  EXPECT_EQ(matLeft.size().first, 3);
  EXPECT_EQ(matLeft.size().second, 2);
}

// testing 0.0 values
TEST(TestSparseMatrix, TestZeroValues) {
  std::vector<std::vector<double>> v(
      {{1, 0, 3}, {0, 5, 0}, {0, 0, 9}, {10, 0, 0}});

  unialgo::math::SparseMatrix<unialgo::math::layout_right> matRight(v);
  // 1 0 3
  // 0 5 0
  // 0 0 9
  // 10 0 0
  EXPECT_EQ(matRight(0, 0), 1.0);
  EXPECT_EQ(matRight(0, 1), 0.0);
  EXPECT_EQ(matRight(0, 2), 3.0);
  EXPECT_EQ(matRight(1, 0), 0.0);
  EXPECT_EQ(matRight(1, 1), 5.0);
  EXPECT_EQ(matRight(1, 2), 0.0);
  EXPECT_EQ(matRight(2, 0), 0.0);
  EXPECT_EQ(matRight(2, 1), 0.0);
  EXPECT_EQ(matRight(2, 2), 9.0);
  EXPECT_EQ(matRight(3, 0), 10.0);
  EXPECT_EQ(matRight(3, 1), 0.0);
  EXPECT_EQ(matRight(3, 2), 0.0);
  EXPECT_EQ(matRight.rows(), 4);
  EXPECT_EQ(matRight.cols(), 3);
  EXPECT_EQ(matRight.size().first, 4);
  EXPECT_EQ(matRight.size().second, 3);
}

// testing transpose
TEST(TestSparseMatrix, TestTranspose) {
  std::vector<std::vector<double>> v({{1, 2, 3}, {4, 5, 6}});
  unialgo::math::SparseMatrix<unialgo::math::layout_right> matRight(v);
  // 1 2 3
  // 4 5 6
  EXPECT_EQ(matRight(0, 0), 1.0);
  EXPECT_EQ(matRight(0, 1), 2.0);
  EXPECT_EQ(matRight(0, 2), 3.0);
  EXPECT_EQ(matRight(1, 0), 4.0);
  EXPECT_EQ(matRight(1, 1), 5.0);
  EXPECT_EQ(matRight(1, 2), 6.0);
  EXPECT_EQ(matRight.rows(), 2);
  EXPECT_EQ(matRight.cols(), 3);
  EXPECT_EQ(matRight.size().first, 2);
  EXPECT_EQ(matRight.size().second, 3);

  unialgo::math::SparseMatrix<unialgo::math::layout_right> matRight2 =
      matRight.transpose();
  // 1 4
  // 2 5
  // 3 6
  EXPECT_EQ(matRight2(0, 0), 1.0);
  EXPECT_EQ(matRight2(0, 1), 4.0);
  EXPECT_EQ(matRight2(1, 0), 2.0);
  EXPECT_EQ(matRight2(1, 1), 5.0);
  EXPECT_EQ(matRight2(2, 0), 3.0);
  EXPECT_EQ(matRight2(2, 1), 6.0);
  EXPECT_EQ(matRight2.rows(), 3);
  EXPECT_EQ(matRight2.cols(), 2);
  EXPECT_EQ(matRight2.size().first, 3);
  EXPECT_EQ(matRight2.size().second, 2);
  // matRight2.debug();
  // std::cout << "NOT SORTED" << std::endl;
}

// testing scalar multiplication
TEST(TestSparseMatrix, ScalarMultiplication) {
  std::vector<std::vector<double>> v(
      {{1, 0, 3}, {0, 5, 0}, {0, 0, 9}, {10, 0, 0}});

  unialgo::math::SparseMatrix<unialgo::math::layout_right> matRight(v);

  // alpha * mat
  auto scaled = 2.0 * matRight;
  EXPECT_EQ(scaled(0, 0), 2.0);
  EXPECT_EQ(scaled(0, 1), 0.0);
  EXPECT_EQ(scaled(0, 2), 6.0);
  EXPECT_EQ(scaled(1, 1), 10.0);
  EXPECT_EQ(scaled(2, 2), 18.0);
  EXPECT_EQ(scaled(3, 0), 20.0);
  EXPECT_EQ(scaled(3, 1), 0.0);
  EXPECT_EQ(scaled.rows(), 4);
  EXPECT_EQ(scaled.cols(), 3);

  // mat * alpha
  auto scaled2 = matRight * 0.5;
  EXPECT_EQ(scaled2(0, 0), 0.5);
  EXPECT_EQ(scaled2(0, 2), 1.5);
  EXPECT_EQ(scaled2(1, 1), 2.5);
  EXPECT_EQ(scaled2(2, 2), 4.5);
  EXPECT_EQ(scaled2(3, 0), 5.0);
  EXPECT_EQ(scaled2(3, 1), 0.0);

  // original unchanged
  EXPECT_EQ(matRight(0, 0), 1.0);
  EXPECT_EQ(matRight(1, 1), 5.0);
  EXPECT_EQ(matRight(2, 2), 9.0);
  EXPECT_EQ(matRight(3, 0), 10.0);
}

// testing scalar multiplication with column major
TEST(TestSparseMatrix, ScalarMultiplicationColMajor) {
  std::vector<std::vector<double>> v({{1, 2, 3}, {4, 5, 6}});

  unialgo::math::SparseMatrix<unialgo::math::layout_left> matLeft(v);

  auto scaled = 3.0 * matLeft;
  EXPECT_EQ(scaled(0, 0), 3.0);
  EXPECT_EQ(scaled(0, 1), 12.0);
  EXPECT_EQ(scaled(1, 0), 6.0);
  EXPECT_EQ(scaled(1, 1), 15.0);
  EXPECT_EQ(scaled(2, 0), 9.0);
  EXPECT_EQ(scaled(2, 1), 18.0);
  EXPECT_EQ(scaled.rows(), 3);
  EXPECT_EQ(scaled.cols(), 2);

  // original unchanged
  EXPECT_EQ(matLeft(0, 0), 1.0);
  EXPECT_EQ(matLeft(2, 1), 6.0);
}

// testing multiplication by zero
TEST(TestSparseMatrix, ScalarMultiplicationByZero) {
  std::vector<std::vector<double>> v({{1, 2}, {3, 4}});
  unialgo::math::SparseMatrix<unialgo::math::layout_right> mat(v);

  auto scaled = 0.0 * mat;
  EXPECT_EQ(scaled(0, 0), 0.0);
  EXPECT_EQ(scaled(0, 1), 0.0);
  EXPECT_EQ(scaled(1, 0), 0.0);
  EXPECT_EQ(scaled(1, 1), 0.0);
}

// testing mat * vec (row major)
TEST(TestSparseMatrix, MatVecMultiplication) {
  // 1 0 2
  // 0 3 0
  // 4 0 5
  std::vector<std::vector<double>> v({{1, 0, 2}, {0, 3, 0}, {4, 0, 5}});
  unialgo::math::SparseMatrix<unialgo::math::layout_right> mat(v);
  std::vector<double> vec({10, 20, 30});

  auto result = mat * vec;
  // row 0: 1*10 + 0*20 + 2*30 = 70
  // row 1: 0*10 + 3*20 + 0*30 = 60
  // row 2: 4*10 + 0*20 + 5*30 = 190
  EXPECT_EQ(result.size(), 3);
  EXPECT_EQ(result[0], 70.0);
  EXPECT_EQ(result[1], 60.0);
  EXPECT_EQ(result[2], 190.0);
}

// testing vec * mat (row major)
TEST(TestSparseMatrix, VecMatMultiplication) {
  // 1 0 2
  // 0 3 0
  // 4 0 5
  std::vector<std::vector<double>> v({{1, 0, 2}, {0, 3, 0}, {4, 0, 5}});
  unialgo::math::SparseMatrix<unialgo::math::layout_right> mat(v);
  std::vector<double> vec({10, 20, 30});

  auto result = vec * mat;
  // col 0: 10*1 + 20*0 + 30*4 = 130
  // col 1: 10*0 + 20*3 + 30*0 = 60
  // col 2: 10*2 + 20*0 + 30*5 = 170
  EXPECT_EQ(result.size(), 3);
  EXPECT_EQ(result[0], 130.0);
  EXPECT_EQ(result[1], 60.0);
  EXPECT_EQ(result[2], 170.0);
}

// testing mat * vec (column major)
TEST(TestSparseMatrix, MatVecMultiplicationColMajor) {
  // layout_left: v[col][row]
  // v = {{1, 0, 4}, {0, 3, 0}, {2, 0, 5}}
  // matrix:
  // 1 0 2
  // 0 3 0
  // 4 0 5
  std::vector<std::vector<double>> v({{1, 0, 4}, {0, 3, 0}, {2, 0, 5}});
  unialgo::math::SparseMatrix<unialgo::math::layout_left> mat(v);
  std::vector<double> vec({10, 20, 30});

  auto result = mat * vec;
  EXPECT_EQ(result.size(), 3);
  EXPECT_EQ(result[0], 70.0);
  EXPECT_EQ(result[1], 60.0);
  EXPECT_EQ(result[2], 190.0);
}

// testing mat * vec with non-square matrix
TEST(TestSparseMatrix, MatVecMultiplicationNonSquare) {
  // 1 2 3
  // 4 5 6
  std::vector<std::vector<double>> v({{1, 2, 3}, {4, 5, 6}});
  unialgo::math::SparseMatrix<unialgo::math::layout_right> mat(v);
  std::vector<double> vec({1, 2, 3});

  auto result = mat * vec;
  // row 0: 1*1 + 2*2 + 3*3 = 14
  // row 1: 4*1 + 5*2 + 6*3 = 32
  EXPECT_EQ(result.size(), 2);
  EXPECT_EQ(result[0], 14.0);
  EXPECT_EQ(result[1], 32.0);

  std::vector<double> vec2({1, 2});
  auto result2 = vec2 * mat;
  // col 0: 1*1 + 2*4 = 9
  // col 1: 1*2 + 2*5 = 12
  // col 2: 1*3 + 2*6 = 15
  EXPECT_EQ(result2.size(), 3);
  EXPECT_EQ(result2[0], 9.0);
  EXPECT_EQ(result2[1], 12.0);
  EXPECT_EQ(result2[2], 15.0);
}

// testing empty constructor and insertion
TEST(TestSparseMatrix, EmptyConstructorAndInsert) {
  unialgo::math::SparseMatrix<unialgo::math::layout_right> mat(3, 3);
  EXPECT_EQ(mat.rows(), 3);
  EXPECT_EQ(mat.cols(), 3);
  // everything should be zero
  EXPECT_EQ(mat(0, 0), 0.0);
  EXPECT_EQ(mat(1, 1), 0.0);
  EXPECT_EQ(mat(2, 2), 0.0);

  // insert some values
  mat(0, 0, 1.0);
  mat(0, 2, 3.0);
  mat(1, 1, 5.0);
  mat(2, 0, 7.0);
  EXPECT_EQ(mat(0, 0), 1.0);
  EXPECT_EQ(mat(0, 1), 0.0);
  EXPECT_EQ(mat(0, 2), 3.0);
  EXPECT_EQ(mat(1, 1), 5.0);
  EXPECT_EQ(mat(2, 0), 7.0);
  EXPECT_EQ(mat(2, 2), 0.0);

  // update existing value
  mat(0, 0, 99.0);
  EXPECT_EQ(mat(0, 0), 99.0);
  // other values unchanged
  EXPECT_EQ(mat(0, 2), 3.0);
  EXPECT_EQ(mat(1, 1), 5.0);
}

// testing insertion keeps sorted order
TEST(TestSparseMatrix, InsertSortedOrder) {
  unialgo::math::SparseMatrix<unialgo::math::layout_right> mat(2, 4);

  // insert out of order within a row
  mat(0, 3, 4.0);
  mat(0, 1, 2.0);
  mat(0, 0, 1.0);
  mat(0, 2, 3.0);
  EXPECT_EQ(mat(0, 0), 1.0);
  EXPECT_EQ(mat(0, 1), 2.0);
  EXPECT_EQ(mat(0, 2), 3.0);
  EXPECT_EQ(mat(0, 3), 4.0);

  // verify mat * vec still works after out-of-order insertion
  std::vector<double> vec({1, 2, 3, 4});
  auto result = mat * vec;
  // row 0: 1*1 + 2*2 + 3*3 + 4*4 = 30
  // row 1: all zeros = 0
  EXPECT_EQ(result[0], 30.0);
  EXPECT_EQ(result[1], 0.0);
}

// testing insertion with column major
TEST(TestSparseMatrix, InsertColMajor) {
  unialgo::math::SparseMatrix<unialgo::math::layout_left> mat(2, 3);

  mat(0, 0, 1.0);
  mat(1, 0, 2.0);
  mat(0, 2, 5.0);
  EXPECT_EQ(mat(0, 0), 1.0);
  EXPECT_EQ(mat(1, 0), 2.0);
  EXPECT_EQ(mat(0, 1), 0.0);
  EXPECT_EQ(mat(0, 2), 5.0);
}

}  // namespace