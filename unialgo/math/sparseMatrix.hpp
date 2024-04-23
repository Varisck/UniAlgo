#ifndef UNIALGO_MATH_SPARSEMATRIX_
#define UNIALGO_MATH_SPARSEMATRIX_

#include <iostream>       // std::cout DEBUG
#include <unordered_map>  // std::unordered_map
#include <utility>        // std::pair
#include <vector>         // std::vector

namespace unialgo {
namespace math {

// aliases for layout of matrix
#if __cplusplus >= 202300L
#include <mdspan>  // std::layout_right, std::layout_left
using std::layout_left;
using std::layout_right;
#else
// rows are contiguous in memory
struct column_major;
// columns are contiguous in memory
struct row_major;
using layout_left = column_major;
using layout_right = row_major;
#endif

/**
 * @brief Class rapresenting a sparse matrix
 *
 * @tparam LayoutType layout of matrix
 */
template <class LayoutType = layout_right>
class SparseMatrix {
 public:
  SparseMatrix(std::size_t r, std::size_t c) : rows_(r), cols_(c), matrix_(){};

  /**
   * @brief Construct a SparseMatrix from a 2d vector
   * @attention the vector has to have the same layout as the
   * matrix being constructed
   *
   * @
   * @param m 2d vector
   */
  SparseMatrix(std::vector<std::vector<double>> m);

  /**
   * @brief Construct a SparseMatrix from a 1d vector
   * @attention the vector has to have the same layout as the
   * matrix being constructed
   *
   * @param m 1d vector containing matrix
   * @param r number of rows in matrix
   * @param c number of columns in matrix
   */
  SparseMatrix(std::vector<double> m, std::size_t r, std::size_t c);

  ~SparseMatrix() = default;

  /**
   * @brief Return size of matrix
   *
   * @return std::pair<std::size_t, std::size_t> (rows, cols)
   */
  std::pair<std::size_t, std::size_t> size() {
    return std::make_pair(rows_, cols_);
  }

  /**
   * @brief Get rows inside matrix
   *
   * @return std::size_t number of rows
   */
  std::size_t rows() { return rows_; }

  /**
   * @brief Get columns inside matrix
   *
   * @return std::size_t number of columns
   */
  std::size_t cols() { return cols_; }

#if __cplusplus >= 202300L
  /**
   * @brief operator[] accessing a matrix element
   *
   * @param i row
   * @param j column
   * @return double mat_[i][j]
   */
  double operator[](std::size_t i, std::size_t j) const {
    return operator()(i, j);
    // if (matrix_.find(i) != matrix_.end()) {
    //   for (std::size_t k = 0; k < matrix_.at(i).first.size(); ++k) {
    //     if (matrix_.at(i).first[k] == j) {
    //       return matrix_.at(i).second[k];
    //     }
    //   }
    // }
    // return 0.0;
  }
#endif

  /**
   * @brief operator() accessing a matrix element
   *
   * @param i row
   * @param j column
   * @return double mat_[i][j]
   */
  double operator()(std::size_t i, std::size_t j) const {
    if (matrix_.find(i) != matrix_.end()) {
      for (std::size_t k = 0; k < matrix_.at(i).first.size(); ++k) {
        if (matrix_.at(i).first[k] == j) {
          return matrix_.at(i).second[k];
        }
      }
    }
    return 0.0;
  }

  /**
   * @brief Insert value in matrix
   *
   * if the matrix keeps the rows sorted insertion is O(cols)
   *
   * @param i row number
   * @param j col number
   * @param val value to insert
   */
  void operator()(std::size_t i, std::size_t j, double val) {
    if (matrix_.find(i) != matrix_.end()) {
      for (std::size_t k = 0; k < matrix_.at(i).first.size(); ++k) {
        if (matrix_.at(i).first[k] == j) {
          matrix_.at(i).second[k] = val;
        }
        // this keeps the matrix rows indexes sorted (not sure if needed)
        else if (matrix_.at(i).first[k] > j) {
          matrix_.at(i).first.insert(matrix_.at(i).first.begin() + k, j);
          matrix_.at(i).second.insert(matrix_.at(i).second.begin() + k, val);
          break;
        }
      }
    }
  }

  /**
   * @brief Returns a transposed version of the matrix
   * @attention the matrix is not sorted
   * @return SparseMatrix
   */
  SparseMatrix transpose() {
    SparseMatrix transposed(cols_, rows_);
    for (auto it = matrix_.begin(); it != matrix_.end(); ++it) {
      for (std::size_t c = 0; c < it->second.first.size(); ++c) {
        if (transposed.matrix_.find(it->second.first[c]) !=
            transposed.matrix_.end()) {
          transposed.matrix_.at(it->second.first[c])
              .first.emplace_back(it->first);
          transposed.matrix_.at(it->second.first[c])
              .second.emplace_back(it->second.second[c]);
        } else {
          transposed.matrix_[it->second.first[c]] =
              std::make_pair(std::vector<std::size_t>{it->first},
                             std::vector<double>{it->second.second[c]});
        }
      }
    }
    return transposed;
  }

  friend SparseMatrix operator*(const SparseMatrix& mat,
                                std::vector<double> vec) {}
  friend SparseMatrix operator*(std::vector<double> vec,
                                const SparseMatrix& mat) {}

  friend SparseMatrix operator*(double alpha, const SparseMatrix& mat) {}
  friend SparseMatrix operator*(const SparseMatrix& mat, double alpha) {}

  /**
   * @brief Mat + vec
   * @attention the vector has to have size == mat.rows_
   *
   * @param mat Sparse matrix
   * @param vec vector
   * @return SparseMatrix transposed
   */
  friend SparseMatrix operator+(const SparseMatrix& mat,
                                std::vector<double> vec) {}

  void debug() {
    auto printVec = [](std::vector<double> vec) {
      for (int i = 0; i < vec.size(); ++i) {
        std::cout << vec[i] << " ";
      }
      std::cout << std::endl;
    };

    auto printPos = [](std::vector<std::size_t> vec) {
      for (int i = 0; i < vec.size(); ++i) {
        std::cout << vec[i] << " ";
      }
      std::cout << std::endl;
    };

    for (int i = 0; i < rows_; ++i) {
      std::cout << "row: " << i << ":" << std::endl;
      printPos(matrix_.at(i).first);
      printVec(matrix_.at(i).second);
    }
  }

 private:
  std::unordered_map<std::size_t,
                     std::pair<std::vector<std::size_t>, std::vector<double>>>
      matrix_;
  std::size_t rows_;
  std::size_t cols_;
};

}  // namespace math
}  // namespace unialgo

#endif  // UNIALGO_MATH_SPARSEMATRIX_