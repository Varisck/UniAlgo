#ifndef UNIALGO_MATH_SPARSEMATRIX_
#define UNIALGO_MATH_SPARSEMATRIX_

#include <algorithm>    // std::sort
#include <iostream>     // std::cout DEBUG
#include <type_traits>  // std::is_same_v
#include <utility>      // std::pair
#include <vector>       // std::vector

#include "unialgo/math/helpers.hpp"
#include "unialgo/math/sparseVector.hpp"

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
 * @brief Class representing a sparse matrix in compressed storage
 *
 * Uses Compressed Row Storage (CRS) for layout_right (row_major)
 * and Compressed Column Storage (CCS) for layout_left (column_major),
 * following the same convention as Eigen.
 *
 * @tparam LayoutType layout of matrix
 */
template <class LayoutType = layout_right>
class SparseMatrix {
  static constexpr bool IsRowMajor = std::is_same_v<LayoutType, layout_right>;

 public:
  SparseMatrix(std::size_t r, std::size_t c)
      : rows_(r),
        cols_(c),
        values_(),
        innerIndices_(),
        outerStarts_(outerSize_() + 1, 0) {}

  /**
   * @brief Construct a SparseMatrix from pre-built compressed storage arrays
   *
   * @param r number of rows
   * @param c number of columns
   * @param vals non-zero values
   * @param innerIdx inner indices (column indices for CRS, row indices for CCS)
   * @param outerPtr outer start offsets (size = outerSize + 1)
   */
  SparseMatrix(std::size_t r, std::size_t c, std::vector<double> vals,
               std::vector<std::size_t> innerIdx,
               std::vector<std::size_t> outerPtr)
      : rows_(r),
        cols_(c),
        values_(std::move(vals)),
        innerIndices_(std::move(innerIdx)),
        outerStarts_(std::move(outerPtr)) {}

  /**
   * @brief Construct a SparseMatrix from a 2d vector
   * @attention the vector has to have the same layout as the
   * matrix being constructed
   *
   * @param m 2d vector
   */
  SparseMatrix(std::vector<std::vector<double>> m) {
    if constexpr (IsRowMajor) {
      rows_ = m.size();
      cols_ = m[0].size();
    } else {
      cols_ = m.size();
      rows_ = m[0].size();
    }
    buildFromDense_(m.size(), m[0].size(),
                    [&](std::size_t o, std::size_t in) { return m[o][in]; });
  }

  /**
   * @brief Construct a SparseMatrix from a 1d vector
   * @attention the vector has to have the same layout as the
   * matrix being constructed
   *
   * @param m 1d vector containing matrix
   * @param r number of rows in matrix
   * @param c number of columns in matrix
   */
  SparseMatrix(std::vector<double> m, std::size_t r, std::size_t c)
      : rows_(r), cols_(c) {
    std::size_t inner = innerSize_();
    buildFromDense_(outerSize_(), inner, [&](std::size_t o, std::size_t in) {
      return m[o * inner + in];
    });
  }

  ~SparseMatrix() = default;

  /**
   * @brief Return size of matrix
   *
   * @return std::pair<std::size_t, std::size_t> (rows, cols)
   */
  std::pair<std::size_t, std::size_t> size() const {
    return std::make_pair(rows_, cols_);
  }

  /**
   * @brief Get rows inside matrix
   *
   * @return std::size_t number of rows
   */
  std::size_t rows() const { return rows_; }

  /**
   * @brief Get columns inside matrix
   *
   * @return std::size_t number of columns
   */
  std::size_t cols() const { return cols_; }

  /**
   * @brief operator() accessing an outer slice as a SparseVector
   *
   * Returns a row for row_major (CRS) or a column for column_major (CCS).
   * Always the fast O(nnz_in_slice) path.
   *
   * @param i outer index (row for CRS, column for CCS)
   * @return SparseVector
   */
  SparseVector operator()(std::size_t i) const {
    if constexpr (IsRowMajor) {
      return row(i);
    } else {
      return col(i);
    }
  }

  /**
   * @brief operator[] accessing an outer slice as a SparseVector
   *
   * Same as operator()(i).
   *
   * @param i outer index (row for CRS, column for CCS)
   * @return SparseVector
   */
  SparseVector operator[](std::size_t i) const {
    return operator()(i);
  }

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
    auto [outer, inner] = toOuterInner_(i, j);
    // scan the compressed slice for this outer index
    std::size_t start = outerStarts_[outer];
    std::size_t end = outerStarts_[outer + 1];
    for (std::size_t k = start; k < end; ++k) {
      if (innerIndices_[k] == inner) return values_[k];
      if (innerIndices_[k] > inner) break;  // sorted, won't find it
    }
    return 0.0;
  }

  /**
   * @brief Insert or update value in matrix
   *
   * @param i row number
   * @param j col number
   * @param val value to insert
   */
  void operator()(std::size_t i, std::size_t j, double val) {
    auto [outer, inner] = toOuterInner_(i, j);
    std::size_t start = outerStarts_[outer];
    std::size_t end = outerStarts_[outer + 1];
    for (std::size_t k = start; k < end; ++k) {
      if (innerIndices_[k] == inner) {
        // already exists, just update
        values_[k] = val;
        return;
      }
      if (innerIndices_[k] > inner) {
        // insert here to keep inner indices sorted
        innerIndices_.insert(innerIndices_.begin() + k, inner);
        values_.insert(values_.begin() + k, val);
        // shift all following outer pointers
        for (std::size_t o = outer + 1; o <= outerSize_(); ++o) {
          outerStarts_[o]++;
        }
        return;
      }
    }
    // append at the end of this outer slice
    innerIndices_.insert(innerIndices_.begin() + end, inner);
    values_.insert(values_.begin() + end, val);
    for (std::size_t o = outer + 1; o <= outerSize_(); ++o) {
      outerStarts_[o]++;
    }
  }

  /**
   * @brief Returns a transposed version of the matrix
   *
   * @return SparseMatrix transposed
   */
  SparseMatrix transpose() const {
    SparseMatrix transposed(cols_, rows_);
    std::size_t nnz = values_.size();
    if (nnz == 0) return transposed;

    // Count entries per new outer index
    // In the transposed matrix, original inner indices become new outer indices
    std::vector<std::size_t> count(transposed.outerSize_() + 1, 0);
    for (std::size_t k = 0; k < nnz; ++k) {
      count[innerIndices_[k] + 1]++;
    }
    for (std::size_t o = 0; o < transposed.outerSize_(); ++o) {
      count[o + 1] += count[o];
    }

    transposed.outerStarts_ = count;
    transposed.values_.resize(nnz);
    transposed.innerIndices_.resize(nnz);

    // Fill: iterate original outer slices so new inner indices are inserted
    // in ascending order
    std::vector<std::size_t> pos(count.begin(), count.end() - 1);
    for (std::size_t o = 0; o < outerSize_(); ++o) {
      for (std::size_t k = outerStarts_[o]; k < outerStarts_[o + 1]; ++k) {
        std::size_t newOuter = innerIndices_[k];
        std::size_t idx = pos[newOuter]++;
        transposed.innerIndices_[idx] = o;
        transposed.values_[idx] = values_[k];
      }
    }

    return transposed;
  }

  /**
   * @brief Mat * vec
   * @attention the vector has to have size == mat.cols_
   *
   * Iterates only over stored non-zeros. For each entry,
   * maps (outer, inner) back to (row, col) based on layout
   * and accumulates values_[k] * vec[col] into result[row].
   *
   * @param mat Sparse matrix (m x n)
   * @param vec vector of size n
   * @return std::vector<double> result of size m
   */
  friend std::vector<double> operator*(const SparseMatrix& mat,
                                       const std::vector<double>& vec) {
    std::vector<double> result(mat.rows_, 0.0);
    for (std::size_t o = 0; o < mat.outerSize_(); ++o) {
      for (std::size_t k = mat.outerStarts_[o]; k < mat.outerStarts_[o + 1];
           ++k) {
        std::size_t in = mat.innerIndices_[k];
        std::size_t row = IsRowMajor ? o : in;
        std::size_t col = IsRowMajor ? in : o;
        result[row] += mat.values_[k] * vec[col];
      }
    }
    return result;
  }
  /**
   * @brief vec * Mat, equivalent to mat^T * vec
   * @attention the vector has to have size == mat.rows_
   *
   * Same loop as mat * vec but accumulates
   * vec[row] * values_[k] into result[col].
   *
   * @param vec vector of size m
   * @param mat Sparse matrix (m x n)
   * @return std::vector<double> result of size n
   */
  friend std::vector<double> operator*(const std::vector<double>& vec,
                                       const SparseMatrix& mat) {
    std::vector<double> result(mat.cols_, 0.0);
    for (std::size_t o = 0; o < mat.outerSize_(); ++o) {
      for (std::size_t k = mat.outerStarts_[o]; k < mat.outerStarts_[o + 1];
           ++k) {
        std::size_t in = mat.innerIndices_[k];
        std::size_t row = IsRowMajor ? o : in;
        std::size_t col = IsRowMajor ? in : o;
        result[col] += vec[row] * mat.values_[k];
      }
    }
    return result;
  }

  friend SparseMatrix operator*(double alpha, const SparseMatrix& mat) {
    SparseMatrix result(mat);
    for (std::size_t k = 0; k < result.values_.size(); ++k) {
      result.values_[k] *= alpha;
    }
    return result;
  }
  friend SparseMatrix operator*(const SparseMatrix& mat, double alpha) {
    return alpha * mat;
  }

  /**
   * @brief Mat * Mat
   *
   * For CRS (row major): iterates rows of A, for each non-zero (k, val_a)
   * in that row, adds val_a * row k of B into a workspace. This builds
   * the result row-by-row in CRS order.
   *
   * For CCS (col major): iterates columns of B, for each non-zero (k, val_b)
   * in that column, adds val_b * column k of A into a workspace. This builds
   * the result column-by-column in CCS order.
   *
   * Both paths only use fast O(nnz_in_slice) outer access.
   *
   * @param A Sparse matrix (m x n)
   * @param B Sparse matrix (n x p)
   * @return SparseMatrix (m x p)
   */
  friend SparseMatrix operator*(const SparseMatrix& A, const SparseMatrix& B) {
    SparseMatrix C(A.rows_, B.cols_);

    // workspace size is the inner dimension of the result
    std::vector<double> work(C.innerSize_(), 0.0);
    std::vector<std::size_t> touched;

    for (std::size_t o = 0; o < C.outerSize_(); ++o) {
      if constexpr (IsRowMajor) {
        // row o of C = sum of A[o,k] * row_k(B)
        std::size_t aStart = A.outerStarts_[o];
        std::size_t aEnd = A.outerStarts_[o + 1];
        for (std::size_t ak = aStart; ak < aEnd; ++ak) {
          std::size_t k = A.innerIndices_[ak];
          double val_a = A.values_[ak];
          // add val_a * row k of B into workspace
          std::size_t bStart = B.outerStarts_[k];
          std::size_t bEnd = B.outerStarts_[k + 1];
          for (std::size_t bk = bStart; bk < bEnd; ++bk) {
            std::size_t j = B.innerIndices_[bk];
            if (dequal(work[j], 0.0)) touched.push_back(j);
            work[j] += val_a * B.values_[bk];
          }
        }
      } else {
        // col o of C = sum of B[k,o] * col_k(A)
        std::size_t bStart = B.outerStarts_[o];
        std::size_t bEnd = B.outerStarts_[o + 1];
        for (std::size_t bk = bStart; bk < bEnd; ++bk) {
          std::size_t k = B.innerIndices_[bk];
          double val_b = B.values_[bk];
          // add val_b * col k of A into workspace
          std::size_t aStart = A.outerStarts_[k];
          std::size_t aEnd = A.outerStarts_[k + 1];
          for (std::size_t ak = aStart; ak < aEnd; ++ak) {
            std::size_t i = A.innerIndices_[ak];
            if (dequal(work[i], 0.0)) touched.push_back(i);
            work[i] += val_b * A.values_[ak];
          }
        }
      }

      // compress workspace into outer slice o of C
      std::sort(touched.begin(), touched.end());
      for (std::size_t idx : touched) {
        if (!dequal(work[idx], 0.0)) {
          C.innerIndices_.push_back(idx);
          C.values_.push_back(work[idx]);
        }
        work[idx] = 0.0;
      }
      C.outerStarts_[o + 1] = C.values_.size();
      touched.clear();
    }

    return C;
  }

  /**
   * @brief Get a row of the matrix as a SparseVector
   *
   * @param i row index
   * @return SparseVector of size cols_
   */
  SparseVector row(std::size_t i) const { return slice_(i, IsRowMajor); }

  /**
   * @brief Get a column of the matrix as a SparseVector
   *
   * @param j column index
   * @return SparseVector of size rows_
   */
  SparseVector col(std::size_t j) const { return slice_(j, !IsRowMajor); }

  /**
   * @brief Mat * SparseVector
   * @attention the vector has to have size == mat.cols_
   *
   * Only iterates over non-zeros in both the matrix and the vector.
   *
   * @param mat Sparse matrix (m x n)
   * @param vec SparseVector of size n
   * @return std::vector<double> result of size m
   */
  friend std::vector<double> operator*(const SparseMatrix& mat,
                                       const SparseVector& vec) {
    std::vector<double> result(mat.rows_, 0.0);
    for (std::size_t o = 0; o < mat.outerSize_(); ++o) {
      for (std::size_t k = mat.outerStarts_[o]; k < mat.outerStarts_[o + 1];
           ++k) {
        std::size_t in = mat.innerIndices_[k];
        std::size_t row = IsRowMajor ? o : in;
        std::size_t col = IsRowMajor ? in : o;
        // only multiply if the sparse vector has this col
        double v = vec(col);
        if (!dequal(v, 0.0)) {
          result[row] += mat.values_[k] * v;
        }
      }
    }
    return result;
  }

  /**
   *
   * Problem this should return a dense matrix
   *
   * @brief Mat + vec
   * @attention the vector has to have size == mat.rows_
   *
   * @param mat Sparse matrix
   * @param vec vector
   * @return SparseMatrix
   */
  // friend SparseMatrix operator+(const SparseMatrix& mat,
  //                               std::vector<double> vec) {
  //   SparseMatrix result(mat);
  //   for (std::size_t o = 0; o < mat.outerSize_(); ++o) {
  //     for (std::size_t k = mat.outerStarts_[o]; k < mat.outerStarts_[o + 1];
  //          ++k) {
  //       std::size_t in = mat.innerIndices_[k];
  //       std::size_t row = IsRowMajor ? o : in;
  //       result.values_[k] += vec[row];
  //     }
  //   }
  //   return result;
  // }

  void debug() const {
    std::cout << "outerStarts: ";
    for (auto v : outerStarts_) std::cout << v << " ";
    std::cout << std::endl;
    std::cout << "innerIndices: ";
    for (auto v : innerIndices_) std::cout << v << " ";
    std::cout << std::endl;
    std::cout << "values: ";
    for (auto v : values_) std::cout << v << " ";
    std::cout << std::endl;
  }

 private:
  std::size_t rows_;
  std::size_t cols_;
  std::vector<double> values_;
  std::vector<std::size_t> innerIndices_;
  std::vector<std::size_t> outerStarts_;

  std::size_t outerSize_() const { return IsRowMajor ? rows_ : cols_; }
  std::size_t innerSize_() const { return IsRowMajor ? cols_ : rows_; }

  // extracts a slice from the compressed storage
  // if isOuter, idx is an outer index → O(nnz_in_slice), direct copy
  // if !isOuter, idx is an inner index → O(nnz), scans all outer slices
  SparseVector slice_(std::size_t idx, bool isOuter) const {
    if (isOuter) {
      std::size_t start = outerStarts_[idx];
      std::size_t end = outerStarts_[idx + 1];
      return SparseVector(
          std::vector<std::size_t>(innerIndices_.begin() + start,
                                   innerIndices_.begin() + end),
          std::vector<double>(values_.begin() + start,
                              values_.begin() + end),
          innerSize_());
    }
    // scan all outer slices for entries with inner index == idx
    std::vector<std::size_t> indices;
    std::vector<double> values;
    for (std::size_t o = 0; o < outerSize_(); ++o) {
      for (std::size_t k = outerStarts_[o]; k < outerStarts_[o + 1]; ++k) {
        if (innerIndices_[k] == idx) {
          indices.push_back(o);
          values.push_back(values_[k]);
        }
        if (innerIndices_[k] > idx) break;  // sorted
      }
    }
    return SparseVector(std::move(indices), std::move(values), outerSize_());
  }

  std::pair<std::size_t, std::size_t> toOuterInner_(std::size_t i,
                                                    std::size_t j) const {
    if constexpr (IsRowMajor) {
      return {i, j};
    } else {
      return {j, i};
    }
  }

  /**
   * @brief Build compressed storage from a dense source
   *
   * Populates values_, innerIndices_ and outerStarts_ in two passes:
   *  1. Counts non-zeros per outer slice and builds outerStarts_
   *     via prefix sum.
   *  2. Fills values_ and innerIndices_ using a per-slice write
   *     position that advances as entries are inserted.
   *
   * @tparam Accessor callable (std::size_t outer, std::size_t inner) -> double
   * @param outerDim number of outer slices (rows for CRS, cols for CCS)
   * @param innerDim number of inner indices per slice
   * @param access returns the dense value at (outer, inner)
   */
  template <typename Accessor>
  void buildFromDense_(std::size_t outerDim, std::size_t innerDim,
                       Accessor access) {
    // count non-zeros per outer slice, store shifted by one in outerStarts_
    outerStarts_.resize(outerDim + 1, 0);
    for (std::size_t o = 0; o < outerDim; ++o) {
      for (std::size_t in = 0; in < innerDim; ++in) {
        if (!dequal(access(o, in), 0.0)) {
          outerStarts_[o + 1]++;
        }
      }
    }
    // turn counts into cumulative offsets
    for (std::size_t o = 0; o < outerDim; ++o) {
      outerStarts_[o + 1] += outerStarts_[o];
    }

    std::size_t nnz = outerStarts_[outerDim];
    values_.resize(nnz);
    innerIndices_.resize(nnz);
    // pos[o] tracks the next write position for each outer slice
    std::vector<std::size_t> pos(outerStarts_.begin(), outerStarts_.end() - 1);
    for (std::size_t o = 0; o < outerDim; ++o) {
      for (std::size_t in = 0; in < innerDim; ++in) {
        double val = access(o, in);
        if (!dequal(val, 0.0)) {
          std::size_t idx = pos[o]++;
          innerIndices_[idx] = in;
          values_[idx] = val;
        }
      }
    }
  }
};

}  // namespace math
}  // namespace unialgo

#endif  // UNIALGO_MATH_SPARSEMATRIX_
