#include "unialgo/utils/bitvector/wordVector.hpp"

namespace unialgo {
namespace utils {
// ============ Implementation WordVector ============

WordVector::WordVector() : num_words_(0), word_size_(2), bits_(0) {}

WordVector::WordVector(std::size_t num_words, WordVector::Type wordSize)
    : num_words_(num_words),
      word_size_(wordSize),
      bits_(
          std::ceil((num_words * wordSize) / static_cast<double>(type_size))) {}

std::size_t WordVector::getNumBits() const { return num_words_ * word_size_; }

std::size_t WordVector::size() const { return num_words_; }

uint8_t WordVector::getWordSize() const { return word_size_; }

WordVector::Reference WordVector::operator[](std::size_t pos) {
  return WordVectorRef(&bits_[std::floor((pos * word_size_) / type_size)],
                       ((pos * word_size_) % type_size), word_size_);
}

WordVector::ConstReference WordVector::operator[](std::size_t pos) const {
  return WordVectorRef(&bits_[std::floor((pos * word_size_) / type_size)],
                       ((pos * word_size_) % type_size), word_size_);
}

}  // namespace utils
}  // namespace unialgo