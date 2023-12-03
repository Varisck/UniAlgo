#include "UniAlgo/pattern/stringMatching.hpp"

#include <string>
#include <unordered_map>
#include <vector>

namespace unialgo {

pattern::TransitionFunction pattern::MakeTransitionFunction(std::string p) {
  pattern::TransitionFunction transition;

  size_t unique_char = 0;
  // creates entrys unique_char -> col in tranision function
  for (size_t i = 0; i < p.size(); ++i) {
    if (!transition.lookup.contains(p[i])) {
      transition.lookup.emplace(std::make_pair(p[i], unique_char));
      unique_char++;
    }
  }
  // creates table of transitino function of size p.size() * unique_char
  transition.tf = std::vector<std::vector<std::size_t>>(
      p.size(), std::vector<std::size_t>(unique_char, 0));

  // TODO: check this!
  for (size_t i = 1; i <= p.size(); ++i) {
    std::size_t k = transition.tf[i - 1][transition.lookup.at(p[i - 1])];
    transition.tf[i - 1][transition.lookup.at(p[i - 1])] = i;
    // copy line k to line i
  }
}

std::vector<std::size_t> pattern::Fsa(std::string t, std::string p) {
  TransitionFunction tf = MakeTransitionFunction(p);
}

}  // namespace unialgo
