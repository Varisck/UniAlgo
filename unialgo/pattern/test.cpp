#include <iostream>

#include "unialgo/pattern/stringMatching.hpp"

int main() {
  std::string pattern = "acacbac";
  auto tf = unialgo::pattern::MakeTransitionFunction(pattern);

  std::cout << "Transition function: " << std::endl;
  for (int i = 0; i < tf.lookup.size(); ++i) {
    for (auto pair : tf.lookup) {
      if (pair.second == i) {
        std::cout << pair.first << " ";
      }
    }
  }
  std::cout << std::endl;
  for (int i = 0; i < tf.tf.size(); ++i) {
    for (int j = 0; j < tf.lookup.size(); ++j) {
      std::cout << tf.tf[i][j] << " ";
    }
    std::cout << std::endl;
  }

  std::string text = "adeacacbacdeac";
  auto occ = unialgo::pattern::Fsa(text, pattern);
  for (auto i : occ) {
    std::cout << "Found occurrence at: " << i << std::endl;
  }
}