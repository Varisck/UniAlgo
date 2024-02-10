#include "unialgo/pattern/stringMatching.hpp"

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

namespace unialgo {

std::vector<std::size_t> pattern::MakePrefixFunction(std::string p) {
  std::vector<std::size_t> pf(p.size());

  pf[0] = 0;  // longest border of size 1 is 0
  std::size_t current_border = 0;

  for (std::size_t i = 1; i < p.size(); ++i) {
    while (current_border > 0 && p[current_border] != p[i]) {
      current_border = pf[current_border - 1];
    }
    if (p[current_border] == p[i]) ++current_border;
    pf[i] = current_border;
  }
  return pf;
}

std::vector<std::size_t> pattern::Kmp(std::string t, std::string p) {
  std::vector<std::size_t> pf = pattern::MakePrefixFunction(p);
  std::vector<std::size_t> occurrences;
  std::size_t j = 0;

  for (std::size_t i = 0; i < p.size(); ++i) {
    while (j > 0 && p[j + 1] != t[i]) {
      j = pf[j];
    }
    if (p[j + 1] == t[i]) ++j;
    if (j == p.size() - 1) {
      occurrences.emplace_back(i - p.size() + 1);
    }
  }
  return occurrences;
}

}  // namespace unialgo
