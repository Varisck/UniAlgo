#include "unialgo/pattern/stringMatching.hpp"

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

namespace unialgo {

pattern::TransitionFunction pattern::MakeTransitionFunction(std::string p) {
  pattern::TransitionFunction transition;
  size_t unique_char = 0;  // unique characters in pattern

  // creates entrys unique_char -> col in tranision function
  for (size_t i = 0; i < p.size(); ++i) {
    if (!transition.lookup.contains(p[i])) {
      transition.lookup.emplace(std::make_pair(p[i], unique_char));
      unique_char++;
    }
  }

  // creates table of transitino function of size p.size() + 1 * unique_char
  transition.tf = std::vector<std::vector<std::size_t>>(p.size() + 1);
  transition.tf[0] = std::vector<std::size_t>(unique_char, 0);

  for (size_t i = 1; i <= p.size(); ++i) {
    std::size_t k = transition.tf[i - 1][transition.lookup.at(p[i - 1])];
    transition.tf[i - 1][transition.lookup.at(p[i - 1])] = i;
    // copy line k to line i
    transition.tf[i] = transition.tf[k];
  }
  return transition;
}

std::vector<std::size_t> pattern::Fsa(std::string t, std::string p) {
  TransitionFunction tf = MakeTransitionFunction(p);
  std::size_t state = 0;
  std::vector<std::size_t> occurrences;

  for (std::size_t i = 0; i < t.size(); ++i) {
    if (tf.lookup.contains(t[i])) {
      state = tf.tf[state][tf.lookup.at(t[i])];
    } else {
      state = 0;
    }
    if (state == p.size()) {
      occurrences.emplace_back(i - p.size() + 1);
    }
  }
  return occurrences;
}

}  // namespace unialgo
