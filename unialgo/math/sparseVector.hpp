#ifndef UNIALGO_MATH_SPARSEVECTOR_
#define UNIALGO_MATH_SPARSEVECTOR_

#include <iostream>  // std::cout DEBUG
#include <vector>    // std::vector

#include "unialgo/math/helpers.hpp"

namespace unialgo {
namespace math {

/**
 * @brief Class representing a sparse vector in compressed storage
 *
 * Stores only non-zero entries as two parallel arrays:
 * indices_ (sorted positions) and values_.
 */
class SparseVector {
 public:
  SparseVector(std::size_t size) : size_(size), indices_(), values_() {}

  /**
   * @brief Construct a SparseVector from a dense vector
   *
   * @param v dense vector
   */
  SparseVector(std::vector<double> v) : size_(v.size()) {
    for (std::size_t i = 0; i < v.size(); ++i) {
      if (!dequal(v[i], 0.0)) {
        indices_.push_back(i);
        values_.push_back(v[i]);
      }
    }
  }

  /**
   * @brief Construct a SparseVector from indices and values
   *
   * @param indices sorted positions of non-zeros
   * @param values corresponding values
   * @param size logical length of the vector
   */
  SparseVector(std::vector<std::size_t> indices, std::vector<double> values,
               std::size_t size)
      : size_(size),
        indices_(std::move(indices)),
        values_(std::move(values)) {}

  ~SparseVector() = default;

  /**
   * @brief Get the logical size of the vector
   *
   * @return std::size_t size
   */
  std::size_t size() const { return size_; }

  /**
   * @brief Get the number of non-zero entries
   *
   * @return std::size_t number of stored entries
   */
  std::size_t nnz() const { return values_.size(); }

  /**
   * @brief Access element at index i
   *
   * @param i index
   * @return double value at i, 0.0 if not stored
   */
  double operator()(std::size_t i) const {
    for (std::size_t k = 0; k < indices_.size(); ++k) {
      if (indices_[k] == i) return values_[k];
      if (indices_[k] > i) break;  // sorted, won't find it
    }
    return 0.0;
  }

  /**
   * @brief Insert or update value at index i
   *
   * @param i index
   * @param val value to insert
   */
  void operator()(std::size_t i, double val) {
    for (std::size_t k = 0; k < indices_.size(); ++k) {
      if (indices_[k] == i) {
        values_[k] = val;
        return;
      }
      if (indices_[k] > i) {
        // insert here to keep sorted
        indices_.insert(indices_.begin() + k, i);
        values_.insert(values_.begin() + k, val);
        return;
      }
    }
    // append at end
    indices_.push_back(i);
    values_.push_back(val);
  }

  /**
   * @brief Dot product between two sparse vectors
   *
   * Merges the two sorted index arrays, only multiplying
   * where both vectors have a stored entry.
   *
   * @param a first sparse vector
   * @param b second sparse vector
   * @return double dot product
   */
  friend double dot(const SparseVector& a, const SparseVector& b) {
    double result = 0.0;
    std::size_t i = 0, j = 0;
    while (i < a.nnz() && j < b.nnz()) {
      if (a.indices_[i] == b.indices_[j]) {
        result += a.values_[i] * b.values_[j];
        ++i;
        ++j;
      } else if (a.indices_[i] < b.indices_[j]) {
        ++i;
      } else {
        ++j;
      }
    }
    return result;
  }

  /**
   * @brief Dot product with a dense vector
   *
   * @param sparse sparse vector
   * @param dense dense vector
   * @return double dot product
   */
  friend double dot(const SparseVector& sparse,
                    const std::vector<double>& dense) {
    double result = 0.0;
    for (std::size_t k = 0; k < sparse.nnz(); ++k) {
      result += sparse.values_[k] * dense[sparse.indices_[k]];
    }
    return result;
  }

  friend double dot(const std::vector<double>& dense,
                    const SparseVector& sparse) {
    return dot(sparse, dense);
  }

  friend SparseVector operator*(double alpha, const SparseVector& vec) {
    SparseVector result(vec);
    for (std::size_t k = 0; k < result.values_.size(); ++k) {
      result.values_[k] *= alpha;
    }
    return result;
  }
  friend SparseVector operator*(const SparseVector& vec, double alpha) {
    return alpha * vec;
  }

  /**
   * @brief Convert to a dense vector
   *
   * @return std::vector<double> dense representation
   */
  std::vector<double> toDense() const {
    std::vector<double> result(size_, 0.0);
    for (std::size_t k = 0; k < nnz(); ++k) {
      result[indices_[k]] = values_[k];
    }
    return result;
  }

  void debug() const {
    std::cout << "indices: ";
    for (auto v : indices_) std::cout << v << " ";
    std::cout << std::endl;
    std::cout << "values: ";
    for (auto v : values_) std::cout << v << " ";
    std::cout << std::endl;
  }

  // SparseMatrix needs access to indices_/values_ for row()/col()/operator*
  template <class LayoutType>
  friend class SparseMatrix;

 private:
  std::size_t size_;
  std::vector<std::size_t> indices_;
  std::vector<double> values_;
};

}  // namespace math
}  // namespace unialgo

#endif  // UNIALGO_MATH_SPARSEVECTOR_
