#include <gtest/gtest.h>

#include <algorithm>  // std::sort
#include <cmath>
#include <memory>   // std::shared_ptr
#include <utility>  // std::swap

#include "unialgo/graph/algorithms.hpp"
#include "unialgo/graph/graph.hpp"

namespace {

TEST(TestingGraph, test_empty_size) {
  unialgo::graph::DirectedGraph<int, int> g;
  EXPECT_EQ(g.empty(), true);
  EXPECT_EQ(g.size(), 0);
  g[12] = 1;
  EXPECT_EQ(g.empty(), false);
  EXPECT_EQ(g.size(), 1);
}

TEST(TestingGraph, accessing_elements_emplace) {
  unialgo::graph::DirectedGraph<int, int> g;
  g[12] = 1;
  g[10] = 10;

  EXPECT_EQ(g[12], 1);
  EXPECT_EQ(g[10], 10);
  EXPECT_EQ(g.emplace(10).second, false);
  EXPECT_EQ(g.emplace(10).first->second->data(), 10);
  EXPECT_EQ(g.size(), 2);
}

TEST(TestingGraph, accessing_elements_insert) {
  unialgo::graph::DirectedGraph<int, int> g;
  int a = 12;
  int b = 10;
  g[a] = 1;
  g[b] = 10;

  EXPECT_EQ(g[a], 1);
  EXPECT_EQ(g[b], 10);
  EXPECT_EQ(g.insert(a).second, false);
  EXPECT_EQ(g.insert(a).first->second->data(), 1);
}

TEST(TestingGraph, making_edges) {
  unialgo::graph::DirectedGraph<int, int> g;

  g[12] = 1;
  g[10] = 10;

  EXPECT_EQ(g[12], 1);
  EXPECT_EQ(g[10], 10);
  EXPECT_EQ(g.emplace(10).second, false);
  EXPECT_EQ(g.emplace(10).first->second->data(), 10);
  EXPECT_EQ(g.size(), 2);

  g(12, 10) = 100;
  EXPECT_EQ(g.is_edge(12, 10), true);
  EXPECT_EQ(g.is_edge(10, 12), false);
  EXPECT_EQ(g(12, 10), 100);

  g(12, 10) = 10;
  EXPECT_EQ(g(12, 10), 10);
}

TEST(TestingGraph, making_edges_undirected) {
  unialgo::graph::UndirectedGraph<int, int> g;

  g[12] = 1;
  g[10] = 10;

  EXPECT_EQ(g[12], 1);
  EXPECT_EQ(g[10], 10);
  EXPECT_EQ(g.emplace(10).second, false);
  EXPECT_EQ(g.emplace(10).first->second->data(), 10);
  EXPECT_EQ(g.size(), 2);

  g(12, 10) = 100;
  EXPECT_EQ(g.is_edge(12, 10), true);
  EXPECT_EQ(g.is_edge(10, 12), true);
  EXPECT_EQ(g(12, 10), 100);
  EXPECT_EQ(g(10, 12), 100);

  g(10, 12) = 10;
  EXPECT_EQ(g(10, 12), 10);
  EXPECT_EQ(g(12, 10), 10);
}

TEST(TestingGraph, removing_edges) {
  unialgo::graph::DirectedGraph<int, int> g;

  g[12] = 1;
  g[10] = 10;

  EXPECT_EQ(g[12], 1);
  EXPECT_EQ(g[10], 10);
  EXPECT_EQ(g.emplace(10).second, false);
  EXPECT_EQ(g.emplace(10).first->second->data(), 10);
  EXPECT_EQ(g.size(), 2);

  g(12, 10) = 100;
  EXPECT_EQ(g.is_edge(12, 10), true);
  EXPECT_EQ(g.is_edge(10, 12), false);
  EXPECT_EQ(g(12, 10), 100);

  g(12, 10) = 10;
  g(10, 12) = 10;
  EXPECT_EQ(g(12, 10), 10);
  EXPECT_EQ(g(10, 12), 10);

  g.remove_edge(12, 10);
  EXPECT_EQ(g.is_edge(12, 10), false);
  EXPECT_EQ(g.is_edge(10, 12), true);
}

TEST(TestingGraph, removing_edges_undirected) {
  unialgo::graph::UndirectedGraph<int, int> g;

  g[12] = 1;
  g[10] = 10;

  EXPECT_EQ(g[12], 1);
  EXPECT_EQ(g[10], 10);
  EXPECT_EQ(g.emplace(10).second, false);
  EXPECT_EQ(g.emplace(10).first->second->data(), 10);
  EXPECT_EQ(g.size(), 2);

  g(12, 10) = 100;
  EXPECT_EQ(g.is_edge(12, 10), true);
  EXPECT_EQ(g.is_edge(10, 12), true);
  EXPECT_EQ(g(12, 10), 100);
  EXPECT_EQ(g(10, 12), 100);

  g(10, 12) = 10;
  EXPECT_EQ(g(10, 12), 10);
  EXPECT_EQ(g(12, 10), 10);

  g.remove_edge(12, 10);
  EXPECT_EQ(g.is_edge(12, 10), false);
  EXPECT_EQ(g.is_edge(10, 12), false);
}

TEST(TestingGraph, erase_node) {
  unialgo::graph::UndirectedGraph<int, int> g;

  g[12] = 1;
  g[10] = 10;

  EXPECT_EQ(g[12], 1);
  EXPECT_EQ(g[10], 10);
  EXPECT_EQ(g.emplace(10).second, false);
  EXPECT_EQ(g.emplace(10).first->second->data(), 10);
  EXPECT_EQ(g.size(), 2);

  g(12, 10) = 100;
  EXPECT_EQ(g.is_edge(12, 10), true);
  EXPECT_EQ(g.is_edge(10, 12), true);
  EXPECT_EQ(g(12, 10), 100);
  EXPECT_EQ(g(10, 12), 100);

  g.erase(12);

  EXPECT_EQ(g.is_edge(12, 10), false);
  EXPECT_EQ(g.is_edge(10, 12), false);
}

TEST(TestingGraphAlgorithms, bfs) {
  unialgo::graph::UndirectedGraph<char, std::string> g;

  g['a'] = "Node a";
  g['b'] = "Node b";
  g['c'] = "Node c";
  g['d'] = "Node d";
  g['e'] = "Node e";
  g['f'] = "Node f";

  g('a', 'd') = 30;
  g('d', 'c') = 30;
  g('a', 'b') = 10;
  g('b', 'c') = 20;
  g('a', 'f') = 100;
  g('d', 'f') = 1;
  g('b', 'e') = 100;

  auto visit_data = unialgo::graph::dijkstra(g, 'a');
  EXPECT_EQ(visit_data['c'].distance, 30);
  EXPECT_EQ(visit_data['f'].distance, 31);
  EXPECT_EQ(visit_data['e'].distance, 110);
}

TEST(TestingGraphAlgorithms, floyd_warshall) {
  unialgo::graph::DirectedGraph<int, int> g;

  g[0] = 0;
  g[1] = 1;
  g[2] = 2;
  g[3] = 3;
  g[4] = 4;
  g[5] = 5;
  g[6] = 6;
  g[7] = 7;

  g(0, 3) = 1;
  g(1, 2) = 7;
  g(1, 6) = 8;
  g(2, 4) = 3;
  g(2, 5) = 8;
  g(3, 1) = 7;
  g(3, 5) = 2;
  g(3, 7) = 2;
  g(5, 7) = 1;
  g(6, 2) = 3;
  g(6, 7) = 1;
  g(7, 6) = 6;

  // list of keys in graph
  std::vector<int> keys;
  for (int i = 0; i < g.size(); ++i) keys.push_back(i);

  auto d = floyd_warshall(g, keys);
  EXPECT_EQ(d[0], g.get_max_cost());
  EXPECT_EQ(d[1], 8);
  EXPECT_EQ(d[2], 12);
  EXPECT_EQ(d[3], 1);
}

TEST(TestingGraphAlgorithms, prim) {
  unialgo::graph::UndirectedGraph<int, int> g;

  g[1] = 1;
  g[2] = 2;
  g[3] = 3;
  g[4] = 4;
  g[5] = 5;
  g[6] = 6;
  g[7] = 7;

  g(1, 2) = 5;
  g(1, 4) = 17;
  g(2, 4) = 12;
  g(2, 3) = 13;
  g(2, 6) = 8;
  g(3, 7) = 6;
  g(4, 5) = 10;
  g(5, 6) = 4;
  g(6, 4) = 8;
  g(6, 3) = 11;
  g(6, 7) = 14;

  auto mst = prim(g);
  EXPECT_EQ(mst.is_edge(1, 2), true);
  EXPECT_EQ(mst(1, 2), 5);
  EXPECT_EQ(mst.is_edge(2, 6), true);
  EXPECT_EQ(mst(2, 6), 8);
  EXPECT_EQ(mst.is_edge(6, 3), true);
  EXPECT_EQ(mst(6, 3), 11);
  EXPECT_EQ(mst.is_edge(6, 4), true);
  EXPECT_EQ(mst(6, 4), 8);
  EXPECT_EQ(mst.is_edge(6, 5), true);
  EXPECT_EQ(mst(6, 5), 4);
  EXPECT_EQ(mst.is_edge(3, 7), true);
  EXPECT_EQ(mst(3, 7), 6);
}

}  // namespace