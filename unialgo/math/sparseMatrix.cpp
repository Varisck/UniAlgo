#include "unialgo/math/sparseMatrix.hpp"

#include "unialgo/math/helpers.hpp"

namespace unialgo {
namespace math {

template <>
SparseMatrix<layout_right>::SparseMatrix(std::vector<std::vector<double>> m) {
  rows_ = m.size();
  cols_ = m[0].size();
  for (std::size_t i = 0; i < rows_; ++i) {
    for (std::size_t j = 0; j < cols_; ++j) {
      if (!dequal(m[i][j], 0.0)) {
        if (matrix_.find(i) != matrix_.end()) {
          matrix_[i].first.emplace_back(j);
          matrix_[i].second.emplace_back(m[i][j]);
        } else {
          matrix_[i] = std::make_pair(std::vector<std::size_t>{j},
                                      std::vector<double>{m[i][j]});
        }
      }
    }
  }
}

template <>
SparseMatrix<layout_left>::SparseMatrix(std::vector<std::vector<double>> m) {
  cols_ = m.size();
  rows_ = m[0].size();
  for (std::size_t i = 0; i < cols_; ++i) {
    for (std::size_t j = 0; j < rows_; ++j) {
      if (!dequal(m[i][j], 0.0)) {
        if (matrix_.find(j) != matrix_.end()) {
          matrix_[j].first.emplace_back(i);
          matrix_[j].second.emplace_back(m[i][j]);
        } else {
          matrix_[j] = std::make_pair(std::vector<std::size_t>{i},
                                      std::vector<double>{m[i][j]});
        }
      }
    }
  }
}

template <>
SparseMatrix<layout_right>::SparseMatrix(std::vector<double> m, std::size_t r,
                                         std::size_t c) {
  rows_ = r;
  cols_ = c;
  for (std::size_t i = 0; i < rows_; ++i) {
    for (std::size_t j = 0; j < cols_; ++j) {
      if (!dequal(m[i * cols_ + j], 0.0)) {
        if (matrix_.find(i) != matrix_.end()) {
          matrix_[i].first.emplace_back(j);
          matrix_[i].second.emplace_back(m[i * cols_ + j]);
        } else {
          matrix_[i] = std::make_pair(std::vector<std::size_t>{j},
                                      std::vector<double>{m[i * cols_ + j]});
        }
      }
    }
  }
}

template <>
SparseMatrix<layout_left>::SparseMatrix(std::vector<double> m, std::size_t r,
                                        std::size_t c) {
  rows_ = r;
  cols_ = c;
  for (std::size_t i = 0; i < cols_; ++i) {
    for (std::size_t j = 0; j < rows_; ++j) {
      if (!dequal(m[i * rows_ + j], 0.0)) {
        if (matrix_.find(j) != matrix_.end()) {
          matrix_[j].first.emplace_back(i);
          matrix_[j].second.emplace_back(m[i * rows_ + j]);
        } else {
          matrix_[j] = std::make_pair(std::vector<std::size_t>{i},
                                      std::vector<double>{m[i * rows_ + j]});
        }
      }
    }
  }
}

}  // namespace math
}  // namespace unialgo
