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

}  // namespace