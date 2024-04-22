#ifndef UNIALGO_MATH_SPARSEMATRIX_
#define UNIALGO_MATH_SPARSEMATRIX_

#include <unordered_map>  //std::unordered_map
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

 private:
  std::unordered_map<int, std::vector<std::pair<int, double>>> matrix_;
  std::size_t rows_;
  std::size_t cols_;
};

}  // namespace math
}  // namespace unialgo

#endif  // UNIALGO_MATH_SPARSEMATRIX_