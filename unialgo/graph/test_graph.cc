#include <gtest/gtest.h>

#include <algorithm>  // std::sort
#include <cmath>
#include <memory>   // std::shared_ptr
#include <utility>  // std::swap

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

}  // namespace