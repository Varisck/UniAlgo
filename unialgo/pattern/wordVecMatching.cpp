#include "unialgo/pattern/wordVecMatching.hpp"

#include <cmath>  //std::log, std::ceil
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace unialgo {
namespace pattern {

std::unordered_set<char> GetUniqueChars(std::string s) {
  std::unordered_set<char> uniques;
  for (char c : s) uniques.insert(c);
  return uniques;
}

// not the most efficient implementation but generaly small alphabet
Alphabet GetAlphabet(std::string s) {
  Alphabet alphabet;
  // find unique chars
  for (std::size_t i = 0; i < s.size(); ++i) {
    if (!alphabet.contains(s[i])) {
      alphabet.emplace(std::make_pair(s[i], 0));
    }
  }
  // order unique chars in alphabetical order
  std::vector<char> alph;
  for (auto pair : alphabet) {
    alph.emplace_back(pair.first);
  }
  std::sort(alph.begin(), alph.end());
  // give chars in alphabet corresponding value in alphabetical order
  for (uint8_t i = 0; i < alph.size(); ++i) {
    alphabet[alph[i]] = i;
  }
  return alphabet;
}

utils::WordVector StringToBitVector(std::string s, Alphabet alphabet) {
  // compute minimum size bit to represent alphabet
  const std::size_t wordSize =
      std::ceil(std::log(alphabet.size()) / std::log(2));
  // creates wordVector with wordSize = log_2(|Sigma|), size = |s|
  utils::WordVector wv(s.size(), wordSize);

  for (std::size_t i = 0; i < s.size(); ++i) {
    wv[i] = alphabet.at(s[i]);
  }

  return wv;
}

utils::WordVector StringToBitVector(std::string s) {
  Alphabet alphabet = GetAlphabet(s);
  return StringToBitVector(s, alphabet);
}

}  // namespace pattern
}  // namespace unialgo