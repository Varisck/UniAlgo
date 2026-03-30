#ifndef UNIALGO_GRAPH_ADJGRAPH_
#define UNIALGO_GRAPH_ADJGRAPH_

#include <unordered_map>
#include <vector>

#include "unialgo/graph/graph.hpp"

namespace unialgo {
namespace graph {
/**
 * @file This file implements an adjMatrix representation
 *    for the unialgo::Graph
 *
 */

template <class Key, class Data, class Cost = std::size_t,
          unialgo::graph::GraphType GT = unialgo::graph::GraphType::undirected>
class AdjMatrix {
 public:
  AdjMatrix(const unialgo::graph::Graph<Key, Data, Cost, GT>& g) {
    std::unordered_map<Key, std::size_t> key_to_mat_indx;
    std::size_t i = 0;
    // storing node data inside array and mapping keys to indexes
    for (auto it = g.begin(); it != g.end(); ++it) {
      node_data_.push_back(it->second->data());
      key_to_mat_indx[it->first] = i;
      ++i;
    }
    // constructing the adj matrix
    for (auto it = g.begin(); it != g.end(); ++it) {
      adj_.push_back(std::vector<Cost>(g.size(), g.get_max_cost()));
      for (auto edge = it->second->begin(); edge != it->second->end(); ++edge) {
        if (auto adj_node = edge->get_node_ptr().lock()) {
          adj_[key_to_mat_indx[it->first]][key_to_mat_indx[adj_node->key()]] =
              edge->cost();
        }
      }
    }
  }

 private:
  std::vector<std::vector<Cost>> adj_;  // adj_matrix
  std::vector<Data> node_data_;         // vector containing node data
};

}  // namespace graph
}  // namespace unialgo

#endif
