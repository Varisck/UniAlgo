#include "unialgo/graph/algorithms.hpp"

#include <memory>         // std::shared_ptr, std::weak_ptr
#include <queue>          // std::queue, std::priority_queue
#include <unordered_map>  // std::unordered_map

namespace unialgo {
namespace graph {

template <class Key, class Data, class Cost = std::size_t, GraphType GT>
VisitMap<Key, Data, Cost, GT> bfs(const Graph<Key, Data, Cost, GT>& g,
                                  const Key& k) {
  auto it = g.find(k);
  return bfs(g, it);
}

template <class Key, class Data, class Cost = std::size_t, GraphType GT>
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
    for (auto edge = node.begin(); edge != node.end(); ++edge) {
      if (auto adj_node = edge->get_node_ptr().lock()) {
        // process node only if it is a new node
        if (visited_nodes.find(adj_node->key()) == visited_nodes.end()) {
          visited_nodes[adj_node->key()].color =
              VisitData<Key, Data, Cost, GT>::color::gray;
          visited_nodes[adj_node->key()].p = node;
          visited_nodes[adj_node->key()].distance =
              visited_nodes[node->key()].distance + edge.cost();
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

template <class Key, class Data, class Cost = std::size_t, GraphType GT>
VisitMap<Key, Data, Cost, GT> dfs(const Graph<Key, Data, Cost, GT>& g,
                                  const Key& k) {
  auto it = g.find(k);
  return dfs(g, it);
}

template <class Key, class Data, class Cost = std::size_t, GraphType GT>
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

template <class Key, class Data, class Cost = std::size_t, GraphType GT>
VisitMap<Key, Data, Cost, GT> dijkstra(const Graph<Key, Data, Cost, GT>& g,
                                       const Key& k) {
  auto it = g.find(k);
  return dijkstra(g, it);
}

template <class Key, class Data, class Cost = std::size_t, GraphType GT>
VisitMap<Key, Data, Cost, GT> dijkstra(
    const Graph<Key, Data, Cost, GT>& g,
    typename Graph<Key, Data, Cost, GT>::const_iterator start) {
  // data holding visited nodes
  VisitMap<Key, Data, Cost, GT> visited_nodes;

  // starting to visit first node
  visited_nodes[start->first].p.reset();
  visited_nodes[start->first].distance = 0;

  // key_cost pair for the heap
  using key_pair = std::pair<const Key&, Cost>;

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
            queue.push(std::make_pair(adj_key, visited_nodes[adj_key].d));
          }
        }
      }
    }
  }
  return visited_nodes;
}

}  // namespace graph
}  // namespace unialgo