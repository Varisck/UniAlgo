#ifndef UNIALGO_GRAPH_ALGORITHMS_
#define UNIALGO_GRAPH_ALGORITHMS_

/**
 * @file algorithms.hpp
 * @brief This file contains the definition of the algorithms built in for the
 * unialgo::graph::Graph
 *
 */

#include <memory>         // std::shared_ptr, std::weak_ptr
#include <queue>          // std::queue, std::priority_queue
#include <unordered_map>  // std::unordered_map

#include "unialgo/graph/graph.hpp"

namespace unialgo {
namespace graph {

/**
 * @brief Structure to hold data about visited nodes
 *
 * @tparam K key of the graph
 * @tparam T node value on the graph
 * @tparam Cost default std::size_t
 * @tparam GT Graph type
 */
template <class K, class T, class Cost = std::size_t,
          unialgo::graph::GraphType GT = unialgo::graph::undirected>
struct VisitData {
  Cost distance = 0;  // distance from source
  std::size_t f = 0;  // time or end visit
  std::weak_ptr<typename unialgo::graph::Graph<K, T, Cost, GT>::NodeType>
      p;        // pred
  enum color {  // state of visit
    white,      // not visited
    gray,       // visiting the adj list
    black       // visited all the adj list
  } color = white;
};

/**
 * @brief Map containing pair <key, VisitData> where key is the key inisde the
 * graph of a given node and VisitData is the struct holding the data of the
 * node
 *
 */
template <class Key, class Data, class Cost = std::size_t,
          unialgo::graph::GraphType GT = unialgo::graph::undirected>
using VisitMap = std::unordered_map<Key, VisitData<Key, Data, Cost, GT>>;

/**
 * @brief Bfs visit of a graph
 *
 * @tparam Key type of the key for the node
 * @tparam Data type of the data inside node
 * @tparam Cost cost of the edge
 * @tparam GT type of the graph
 * @param g const Graph
 * @param start reference to the key of the starting node
 * @return VisitMap<Key, Data, Cost, GT> map containing data of visited nodes
 */
template <class Key, class Data, class Cost = std::size_t,
          unialgo::graph::GraphType GT>
VisitMap<Key, Data, Cost, GT> bfs(const Graph<Key, Data, Cost, GT>& g,
                                  const Key& start) {
  auto it = g.find(start);
  return bfs(g, it);
}

/**
 * @brief Bfs visit of a graph
 *
 * @tparam Key type of the key for the node
 * @tparam Data type of the data inside node
 * @tparam Cost cost of the edge
 * @tparam GT type of the graph
 * @param g const Graph
 * @param start const_iterator to the starting node
 * @return VisitMap<Key, Data, Cost, GT> map containing data of visited nodes
 */
template <class Key, class Data, class Cost = std::size_t,
          unialgo::graph::GraphType GT>
VisitMap<Key, Data, Cost, GT> bfs(
    const Graph<Key, Data, Cost, GT>& g,
    typename Graph<Key, Data, Cost, GT>::const_iterator start) {
  // data holding visited nodes
  VisitMap<Key, Data, Cost, GT> visited_nodes;

  // starting to visit first node
  visited_nodes[start->first].p.reset();
  visited_nodes[start->first].color =
      VisitData<Key, Data, Cost, GT>::color::gray;

  std::queue<typename Graph<Key, Data, Cost, GT>::NodePtr> queue;
  queue.push(start->second);

  while (!queue.empty()) {
    auto node = queue.front();
    queue.pop();
    // process adj list of node
    for (auto edge = node->begin(); edge != node->end(); ++edge) {
      if (auto adj_node = edge->get_node_ptr().lock()) {
        // process node only if it is a new node
        if (visited_nodes.find(adj_node->key()) == visited_nodes.end()) {
          visited_nodes[adj_node->key()].color =
              VisitData<Key, Data, Cost, GT>::color::gray;
          visited_nodes[adj_node->key()].p = node;
          visited_nodes[adj_node->key()].distance =
              visited_nodes[node->key()].distance + edge->cost();
        }
      }
    }
    visited_nodes[node->key()].color =
        VisitData<Key, Data, Cost, GT>::color::black;
  }
  return visited_nodes;
}

template <class Key, class Data, class Cost = std::size_t, GraphType GT>
void dfs_visit(VisitMap<Key, Data, Cost, GT>& visit_data,
               const Graph<Key, Data, Cost, GT>& g,
               typename Graph<Key, Data, Cost, GT>::const_iterator current,
               std::size_t& time) {
  ++time;
  // set the time of discover
  visit_data[current->first].distance = time;
  visit_data[current->first].color =
      VisitData<Key, Data, Cost, GT>::color::gray;

  for (auto edge = current->second->begin(); edge != current->second->end();
       ++edge) {
    if (auto adj_node = edge->get_node_ptr().lock()) {
      if (visit_data.find(adj_node->key()) == visit_data.end() ||
          visit_data[adj_node->key()].color ==
              VisitData<Key, Data, Cost, GT>::color::white) {
        visit_data[adj_node->key()].p = current->second;
        dfs_visit(visit_data, g, g.find(adj_node->key()), time);
      }
    }
  }
  // set the time of end
  ++time;
  visit_data[current->first].f = time;
  visit_data[current->first].color =
      VisitData<Key, Data, Cost, GT>::color::black;
}

/**
 * @brief Dfs visit of a graph
 *
 * @tparam Key type of the key for the node
 * @tparam Data type of the data inside node
 * @tparam Cost cost of the edge
 * @tparam GT type of the graph
 * @param g const Graph
 * @param start reference to the key of the starting node
 * @return VisitMap<Key, Data, Cost, GT> map containing data of visited nodes
 */
template <class Key, class Data, class Cost = std::size_t,
          unialgo::graph::GraphType GT>
VisitMap<Key, Data, Cost, GT> dfs(const Graph<Key, Data, Cost, GT>& g,
                                  const Key& start) {
  auto it = g.find(start);
  return dfs(g, it);
}

/**
 * @brief Dfs visit of a graph
 *
 * @details the distance and f fields are not the cost but are the time of
 * discover of every node during the recursion
 *
 * @tparam Key type of the key for the node
 * @tparam Data type of the data inside node
 * @tparam Cost cost of the edge
 * @tparam GT type of the graph
 * @param g const Graph
 * @param start const_iterator to the starting node
 * @return VisitMap<Key, Data, Cost, GT> map containing data of visited nodes
 */
template <class Key, class Data, class Cost = std::size_t,
          unialgo::graph::GraphType GT>
VisitMap<Key, Data, Cost, GT> dfs(
    const Graph<Key, Data, Cost, GT>& g,
    typename Graph<Key, Data, Cost, GT>::const_iterator start) {
  // data holding visited nodes
  VisitMap<Key, Data, Cost, GT> visited_nodes;

  // starting to visit first node
  visited_nodes[start->first].p.reset();
  std::size_t time = 0;

  dfs_visit(visited_nodes, g, start, time);
  return visited_nodes;
}

/**
 * @brief Dijkstra algorithm
 *
 * @tparam Key type of the key for the node
 * @tparam Data type of the data inside node
 * @tparam Cost cost of the edge
 * @tparam GT type of the graph
 * @param g const Graph
 * @param start reference to the key of the starting node
 * @return VisitMap<Key, Data, Cost, GT> map containing data of visited nodes
 */
template <class Key, class Data, class Cost = std::size_t,
          unialgo::graph::GraphType GT>
VisitMap<Key, Data, Cost, GT> dijkstra(const Graph<Key, Data, Cost, GT>& g,
                                       const Key& start) {
  auto it = g.find(start);
  return dijkstra(g, it);
}

/**
 * @brief Dijkstra algorithm
 *
 * @tparam Key type of the key for the node
 * @tparam Data type of the data inside node
 * @tparam Cost cost of the edge
 * @tparam GT type of the graph
 * @param g const Graph
 * @param start const_iterator to the starting node
 * @return VisitMap<Key, Data, Cost, GT> map containing data of visited nodes
 */
template <class Key, class Data, class Cost = std::size_t,
          unialgo::graph::GraphType GT>
VisitMap<Key, Data, Cost, GT> dijkstra(
    const Graph<Key, Data, Cost, GT>& g,
    typename Graph<Key, Data, Cost, GT>::const_iterator
        start) {  // data holding visited nodes
  VisitMap<Key, Data, Cost, GT> visited_nodes;

  // starting to visit first node
  visited_nodes[start->first].p.reset();
  visited_nodes[start->first].distance = 0;

  // key_cost pair for the heap
  using key_pair = std::pair<Key, Cost>;

  auto custom_compare = [](const key_pair& v1, const key_pair& v2) {
    return std::greater<Cost>()(v1.second, v2.second);
  };

  std::priority_queue<key_pair, std::vector<key_pair>, decltype(custom_compare)>
      queue(custom_compare);
  queue.push(std::make_pair(start->first, 0));

  while (!queue.empty()) {
    key_pair key_cost = queue.top();
    queue.pop();
    // only process if cost is updated (corresponds to the one in visited_nodes)
    if (visited_nodes[key_cost.first].distance == key_cost.second) {
      auto node_it = g.find(key_cost.first);  // find iterator inside graph
      // loop adj_list of node
      for (auto edge = node_it->second->begin(); edge != node_it->second->end();
           ++edge) {
        if (auto adj_node = edge->get_node_ptr().lock()) {
          Key adj_key = adj_node->key();
          // cost to node_to is cost of traveling from current + edge cost
          Cost new_cost_to_adj =
              visited_nodes[key_cost.first].distance + edge->cost();
          // process node if it is new or if the cost of traveling to the node
          // from the current one is less then the current best
          if (visited_nodes.find(adj_key) == visited_nodes.end() ||
              visited_nodes[adj_key].distance > new_cost_to_adj) {
            visited_nodes[adj_key].distance = new_cost_to_adj;
            visited_nodes[adj_key].p = node_it->second;
            queue.push(
                std::make_pair(adj_key, visited_nodes[adj_key].distance));
          }
        }
      }
    }
  }
  return visited_nodes;
}

/**
 * @brief Computes the layers of the matrix using FloydWarshall algorithm
 *
 * @details The indexes correspond to the order using iterators
 *
 * @tparam Key type of the key for the node
 * @tparam Data type of the data inside node
 * @tparam Cost cost of the edge
 * @tparam GT type of the graph
 * @param g const Graph
 * @param keys order for the keys
 * @return std::vector<Cost> vector representing FloydWarshall matrixes
 */
template <class Key, class Data, class Cost = std::size_t,
          unialgo::graph::GraphType GT>
std::vector<Cost> compute_floyd_warshall(Graph<Key, Data, Cost, GT>& g,
                                         std::vector<Key> keys) {
  std::size_t graph_size = g.size();
  // distance matrix initialized with max_cost
  std::vector<Cost> d(graph_size * graph_size * graph_size, g.get_max_cost());

  // initialize matrix[0] as adj matrix of graph
  for (std::size_t i = 0; i < graph_size; ++i)
    for (std::size_t j = 0; j < graph_size; ++j)
      if (g.is_edge(keys[i], keys[j]))
        d[j + i * graph_size] = g(keys[i], keys[j]);

  for (std::size_t k = 1; k < graph_size; ++k) {
    for (std::size_t i = 0; i < graph_size; ++i) {
      for (std::size_t j = 0; j < graph_size; ++j) {
        // d^(k-1)_(i,j) cost of path i,j of prev iteration
        Cost d_i_j_k_prev =
            d[j + i * graph_size + (k - 1) * graph_size * graph_size];
        // d^(k-1)_(i,k) cost of path i,k of prev iteration
        Cost d_i_k_prev =
            d[k + i * graph_size + (k - 1) * graph_size * graph_size];
        // d^(k-1)_(k,j) cost of path k,j of prev iteration
        Cost d_k_j_prev =
            d[j + k * graph_size + (k - 1) * graph_size * graph_size];
        // d^(k)_(i,j) cost of path i,j of current iteration
        Cost d_i_k_k_j = d_i_k_prev + d_k_j_prev;
        // set current path to fastest path (using k node or prev iteration)
        if (d_i_j_k_prev > d_i_k_k_j && d_i_k_prev != g.get_max_cost() &&
            d_k_j_prev != g.get_max_cost())
          d[j + i * graph_size + k * graph_size * graph_size] = d_i_k_k_j;
        else
          d[j + i * graph_size + k * graph_size * graph_size] = d_i_j_k_prev;
      }
    }
  }
  return d;
}

/**
 * @brief Return last matrix of FloydWarshall's algorithm
 *
 * @details The indexes correspond to the order using iterators
 *
 * @tparam Key type of the key for the node
 * @tparam Data type of the data inside node
 * @tparam Cost cost of the edge
 * @tparam GT type of the graph
 * @param g const Graph
 * @param keys order for the keys
 * @return std::vector<Cost> vector representing FloydWarshall matrixes
 */
template <class Key, class Data, class Cost = std::size_t,
          unialgo::graph::GraphType GT>
std::vector<Cost> floyd_warshall(Graph<Key, Data, Cost, GT>& g,
                                 std::vector<Key> keys) {
  std::size_t graph_size = g.size();
  std::vector<Cost> d = compute_floyd_warshall(g, keys);
  std::vector<Cost> res(
      d.begin() + (graph_size * graph_size * (graph_size - 1)),
      d.begin() + (graph_size * graph_size * graph_size));
  return res;
}

/**
 * @brief Creates MST of g starting from first node
 *
 * @tparam Key type of the key for the node
 * @tparam Data type of the data inside node
 * @tparam Cost cost of the edge
 * @tparam GT type of the graph
 * @param g const Graph
 * @return unialgo::graph::UndirectedGraph<Key, Data, Cost> undirected graph
 * representing MST
 */
template <class Key, class Data, class Cost = std::size_t,
          unialgo::graph::GraphType GT>
unialgo::graph::UndirectedGraph<Key, Data, Cost> prim(
    const Graph<Key, Data, Cost, GT>& g) {
  return prim(g, g.begin());
}

/**
 * @brief Creates MST of g starting from root
 *
 * @tparam Key type of the key for the node
 * @tparam Data type of the data inside node
 * @tparam Cost cost of the edge
 * @tparam GT type of the graph
 * @param root const key reference to key starting the algorithm from
 * @param g const Graph
 * @return unialgo::graph::UndirectedGraph<Key, Data, Cost> undirected graph
 * representing MST
 */
template <class Key, class Data, class Cost = std::size_t,
          unialgo::graph::GraphType GT>
unialgo::graph::UndirectedGraph<Key, Data, Cost> prim(
    const Graph<Key, Data, Cost, GT>& g, const Key& root) {
  auto it = g.find(root);
  return prim(g, it);
}

/**
 * @brief Creates MST of g starting from root
 *
 * @tparam Key type of the key for the node
 * @tparam Data type of the data inside node
 * @tparam Cost cost of the edge
 * @tparam GT type of the graph
 * @param root const iterator to node starting the algorithm from
 * @param g const Graph
 * @return unialgo::graph::UndirectedGraph<Key, Data, Cost> undirected graph
 * representing MST
 */
template <class Key, class Data, class Cost = std::size_t,
          unialgo::graph::GraphType GT>
unialgo::graph::UndirectedGraph<Key, Data, Cost> prim(
    const Graph<Key, Data, Cost, GT>& g,
    typename Graph<Key, Data, Cost, GT>::const_iterator root) {
  // map of nodes for current visit data
  VisitMap<Key, Data, Cost, GT> visiting_nodes_map;
  // starting to visit first node
  visiting_nodes_map[root->first].p.reset();
  visiting_nodes_map[root->first].distance = 0;

  // map for reconstructing mst from
  VisitMap<Key, Data, Cost, GT> mst_map;

  // key_cost pair for the heap
  using key_pair = std::pair<Key, Cost>;
  using const_iterator = typename Graph<Key, Data, Cost, GT>::const_iterator;

  auto custom_compare = [](const key_pair& v1, const key_pair& v2) {
    return std::greater<Cost>()(v1.second, v2.second);
  };

  std::priority_queue<key_pair, std::vector<key_pair>, decltype(custom_compare)>
      queue(custom_compare);
  queue.push(std::make_pair(root->first, 0));

  while (!queue.empty()) {
    auto key_cost_pair = queue.top();
    queue.pop();
    // check if node is in map of nodes to be visited and if the distance is
    // still valid
    if (visiting_nodes_map.find(key_cost_pair.first) !=
            visiting_nodes_map.end() &&
        visiting_nodes_map[key_cost_pair.first].distance ==
            key_cost_pair.second) {
      const_iterator currentIt = g.find(key_cost_pair.first);
      // loop on adj list of current node
      for (auto edge = currentIt->second->begin();
           edge != currentIt->second->end(); ++edge) {
        if (auto adj_node = edge->get_node_ptr().lock()) {
          Key adj_key = adj_node->key();  // key in graph of adj node
          // process node only if is not yet in the MST, and current visiting
          // node and if distance from the frontier is now less then current one
          if (mst_map.find(adj_key) == mst_map.end() &&
              (visiting_nodes_map.find(adj_key) == visiting_nodes_map.end() ||
               edge->cost() < visiting_nodes_map[adj_key].distance)) {
            visiting_nodes_map[adj_key].distance = edge->cost();
            visiting_nodes_map[adj_key].p = currentIt->second;
            queue.push(std::make_pair(adj_key, edge->cost()));
          }
        }
      }
      // put current node in mst
      mst_map[currentIt->first].p = visiting_nodes_map[currentIt->first].p;
      mst_map[currentIt->first].distance =
          visiting_nodes_map[currentIt->first].distance;
      visiting_nodes_map.erase(currentIt->first);
    }
  }

  // construct MST
  unialgo::graph::UndirectedGraph<Key, Data, Cost> mst;
  for (auto it = mst_map.begin(); it != mst_map.end(); ++it) {
    mst[it->first] = g.find(it->first)->second->data();
    if (auto node_pred = it->second.p.lock()) {
      mst(node_pred->key(), it->first) = g(node_pred->key(), it->first);
    }
  }
  return mst;
}

}  // namespace graph
}  // namespace unialgo

#endif  // UNIALGO_GRAPH_ALGORITHMS_