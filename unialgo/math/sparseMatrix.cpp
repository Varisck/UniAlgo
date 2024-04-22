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

template <class LayoutType>
SparseMatrix<LayoutType>::SparseMatrix(std::vector<double> m, std::size_t r,
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

}  // namespace math
}  // namespace unialgo
