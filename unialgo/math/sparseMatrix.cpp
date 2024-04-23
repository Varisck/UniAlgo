#include "unialgo/math/sparseMatrix.hpp"

#include "unialgo/math/helpers.hpp"

namespace unialgo {
namespace math {

SparseMatrix<layout_right>::SparseMatrix(std::vector<std::vector<double>> m) {
  rows_ = m.size();
  cols_ = m[0].size();
  for (std::size_t i = 0; i < rows_; ++i) {
    for (std::size_t j = 0; j < cols_; ++j) {
      if (!dequal(m[i][j], 0.0)) {
        matrix_[i].emplace_back(std::make_pair(j, m[i][j]));
      }
    }
  }
}

SparseMatrix<layout_left>::SparseMatrix(std::vector<std::vector<double>> m) {
  cols_ = m.size();
  rows_ = m[0].size();
  for (std::size_t i = 0; i < cols_; ++i) {
    for (std::size_t j = 0; j < rows_; ++j) {
      if (!dequal(m[i][j], 0.0)) {
        matrix_[i].emplace_back(std::make_pair(j, m[i][j]));
      }
    }
  }
}

SparseMatrix<layout_right>::SparseMatrix(std::vector<double> m, std::size_t r,
                                         std::size_t c) {
  rows_ = r;
  cols_ = c;
  for (std::size_t i = 0; i < rows_; ++i) {
    for (std::size_t j = 0; j < cols_; ++j) {
      if (!dequal(m[i * cols_ + j], 0.0)) {
        matrix_[i].emplace_back(std::make_pair(j, m[i * cols_ + j]));
      }
    }
  }
}

SparseMatrix<layout_left>::SparseMatrix(std::vector<double> m, std::size_t r,
                                        std::size_t c) {
  rows_ = r;
  cols_ = c;
  for (std::size_t i = 0; i < cols_; ++i) {
    for (std::size_t j = 0; j < rows_; ++j) {
      if (!dequal(m[i * rows_ + j], 0.0)) {
        matrix_[i].emplace_back(std::make_pair(j, m[i * rows_ + j]));
      }
    }
  }
}

template <class LayoutType>
std::pair<std::size_t, std::size_t> SparseMatrix<LayoutType>::size() {
  return std::make_pair(rows_, cols_);
}

template <class LayoutType>
std::size_t SparseMatrix<LayoutType>::rows() {
  return rows_;
}

template <class LayoutType>
std::size_t SparseMatrix<LayoutType>::cols() {
  return cols_;
}

}  // namespace math
}  // namespace unialgo
