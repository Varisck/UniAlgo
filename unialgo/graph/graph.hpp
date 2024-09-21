#ifndef UNIALGO_GRAPH_GRAPH_
#define UNIALGO_GRAPH_GRAPH_

#include <memory>         // std::shared_ptr, std::weak_ptr
#include <unordered_map>  // std::unordered_map

#include "unialgo/graph/node.hpp"  // node class

namespace unialgo {
namespace graph {

enum GraphType { directed, undirected };

/**
 * @brief Graph class
 *
 * @details this creates a sparse representation of a graph obj
 *
 * @tparam Key value indexing nodes in graph
 * @tparam Data value inside nodes
 * @tparam Cost value for cost of graph's edges (default std::size_t) this type
 * must specialize std::numeric_limits
 * @tparam GT Graph type (default undirected)
 */
template <class Key, class Data, class Cost = std::size_t,
          unialgo::graph::GraphType GT = undirected>
class Graph {
 public:
  using NodeType = Node<Key, Data, Cost>::Node;
  using NodePtr = std::shared_ptr<NodeType>;
  using NodeIterator = typename NodeType::iterator;
  using NodeConstIterator = typename NodeType::const_iterator;
  using iterator = typename std::unordered_map<Key, NodePtr>::iterator;
  using const_iterator =
      typename std::unordered_map<Key, NodePtr>::const_iterator;
  using size_type = typename std::unordered_map<Key, NodePtr>::size_type;

  explicit Graph() {
    nodes_.clear();
    // set max_cost_ max value of Type Cost
    if (std::numeric_limits<Cost>::is_specialized) {
      max_cost_ = std::numeric_limits<Cost>::max();
    }
    // else {
    // raise error ?
    // }
  }

  Graph(const Graph& other)
      : nodes_(other.nodes_), max_cost_(other.max_cost_) {}

  /*
   *    ==== Lookup ====
   */

  /**
   * @brief Access data inside node, if key not in graph insert a new node with
   * empty data
   *
   * @param k key of node to access
   * @return Data& data to access
   */
  Data& operator[](const Key& k) { return insert(k).first->second->data(); }

  /**
   * @brief Access data inside node, if key not in graph insert a new node with
   * empty data
   *
   * @param k key of node to access
   * @return Data& data to access
   */
  Data& operator[](Key&& k) {
    return emplace(std::move(k)).first->second->data();
  }

  /**
   * @brief Returns a reference to data inside node with
   * specified key. If no such element exists, an exception of type
   * std::out_of_range is thrown.
   *
   * @param k key of the node to find
   * @return Data& data inside node with key = k
   */
  Data& at(const Key& k) { return nodes_.at(k)->data(); }
  const Data& at(const Key& k) const { return nodes_.at(k)->data(); }

  /**
   * @brief Finds an element with key equivalent to key
   *
   * @param k key value of the element to search for
   * @return iterator to the requested element. If no such element
   * is found, past-the-end (see end()) iterator is returned.
   */
  iterator find(const Key& k) { return nodes_.find(k); }
  const_iterator find(const Key& k) const { return nodes_.find(k); }

#if __cplusplus >= 202002L

  /**
   * @brief 1) Checks if there is an element with key equivalent to key in the
   * container
   *
   * @param k key value of the element to search for
   * @return true if there is such an element,
   * @return false otherwise
   */
  bool contains(const Key& k) const { return nodes_.contains(k); }

#endif

  // count (? == contains)
  // find based on data (? maybe with pred?)

  /*
   *    ==== Modifiers ====
   */

  /**
   * @brief Inserts node into the container, if the container doesn't
   * already contain a node with an equivalent key.
   *
   * @param k key
   * @return std::pair<iterator, bool> A pair consisting of an iterator to the
   * inserted node (or to the node that prevented the insertion) and a
   * bool value set to true if and only if the insertion took place.
   */
  std::pair<iterator, bool> insert(const Key& k) {
    return insert(k, NodeType());
  }

  /**
   * @brief Inserts node into the container, if the container doesn't
   * already contain a node with an equivalent key.
   *
   * @param k key to assign the node
   * @param n NodeType& reference to node to create
   * @return std::pair<iterator, bool> A pair consisting of an iterator to the
   * inserted node (or to the node that prevented the insertion) and a
   * bool value set to true if and only if the insertion took place.
   */
  std::pair<iterator, bool> insert(const Key& k, const NodeType& n) {
    std::pair<iterator, bool> p{
        nodes_.insert({k, std::make_shared<NodeType>(n)})};
    p.first->second->set_key(k);
    return p;
  }

  /**
   * @brief Inserts a new node into the container constructed in-place with
   * the given args, if there is no element with the key in the container.
   *
   * @param k key of the node
   * @return std::pair<iterator, bool> A pair consisting <iterator to the
   * inserted element (or to the element that prevented the insertion), bool
   * value set to true if and only if the insertion took place>.
   */
  std::pair<iterator, bool> emplace(Key&& k) {
    return emplace(std::move(k), std::move(NodeType()));
  }

  /**
   * @brief Inserts a new node into the container constructed in-place with
   * the given args, if there is no element with the key in the container.
   *
   * @param k key of the node
   * @param n node to insert with given key
   * @return std::pair<iterator, bool> A pair consisting <iterator to the
   * inserted element (or to the element that prevented the insertion), bool
   * value set to true if and only if the insertion took place>.
   */
  std::pair<iterator, bool> emplace(Key&& k, NodeType&& n) {
    std::pair<iterator, bool> p{nodes_.emplace(
        std::move(k), std::move(std::make_shared<NodeType>(std::move(n))))};
    p.first->second->set_key(k);
    return p;
  }

  /**
   * @brief Removes specified node from the container. The order of the
   * remaining elements is preserved. (This makes it possible to erase
   * individual elements while iterating through the container.)
   *
   * @param pos iterator of node to remove
   * @return iterator following the last removed element.
   */
  iterator erase(iterator pos) { return nodes_.erase(pos); }
  iterator erase(const Key& k) { return nodes_.erase(k); }
  iterator erase(Key&& k) { return nodes_.erase(std::move(k)); }

  // clear
  // merge
  // swap

  /**
   * @brief Check if the given 2 nodes are connected in the directional way
   * n1 -> n2
   *
   * @param n1 start node
   * @param n2 end node
   * @return true if there is an edge n1 -> n2
   * @return false otherwise
   */
  bool is_edge(const_iterator n1, const_iterator n2) const {
    return n1 != cend() && n2 != cend() && n1->second->is_edge(n2->second);
  }

  bool is_edge(const Key& k1, const Key& k2) const {
    const_iterator n1 = nodes_.find(k1);
    const_iterator n2 = nodes_.find(k2);
    return is_edge(n1, n2);
  }

  /**
   * @brief Access Cost between nodes, if the edge is not in the graph insert
   * a new edge with max_cost_
   *
   * @details if the graph is undirected also create the backward edge
   * (n2 -> n1)
   *
   * @param k1 reference of node1's Key
   * @param k2 reference of node2's Key
   * @return Cost& reference to cost of edge n1 -> n2
   */
  Cost& operator()(const Key& k1, const Key& k2) {
    const_iterator n1 = insert(k1).first;
    const_iterator n2 = insert(k2).first;
    return operator()(n1, n2);
  }

  /**
   * @brief Access Cost between nodes, if the edge is not in the graph insert
   * a new edge with max_cost_
   *
   * @details if the graph is undirected also create the backward edge
   * (n2 -> n1)
   *
   * @param k1 Key to node1
   * @param k2 Key to node2
   * @return Cost& reference to cost of edge n1 -> n2
   */
  Cost& operator()(Key&& k1, Key&& k2) {
    const_iterator n1 = emplace(std::move(k1)).first;
    const_iterator n2 = emplace(std::move(k2)).first;
    return operator()(n1, n2);
  }

  /**
   * @brief Access Cost between nodes, if the edge is not in the graph insert
   * a new edge with max_cost_
   *
   * @details if the graph is undirected also create the backward edge
   * (n2 -> n1)
   *
   * @param n1 const_iterator to first node
   * @param n2 const_iterator to second node
   * @return Cost& reference to cost of edge n1 -> n2
   */
  Cost& operator()(const_iterator n1, const_iterator n2) {
    NodeIterator edge = n1->second->find(n2->second);  // check if edge exist
    if (edge != n1->second->end()) {
      return edge->cost();  // return existing edge cost
    }
    // create edge directed way (n1 -> n2)
    std::shared_ptr<Cost> cp =
        n1->second->push_back(n2->second, max_cost_)->get_cost_ptr();
    // if graph is undirected create other way around with same pointer
    if (get_graph_type() == undirected) {
      n2->second->push_back(n1->second, cp);
    }
    return *cp;  // return newly inserted cost value
  }

  /**
   * @brief Remove the edge between n1 and n2
   *
   * @details if the graph is undirected also removes the backward edge
   * (n2 -> n1)
   *
   * @param n1 const_iterator to node1
   * @param n2 const_iterator to node2
   */
  void remove_edge(const_iterator n1, const_iterator n2) {
    n1->second->remove_edge(n2->second);
    if (get_graph_type() == undirected) n2->second->remove_edge(n1->second);
  }

  /**
   * @brief Remove the edge between n1 and n2
   *
   * @details if the graph is undirected also removes the backward edge
   * (n2 -> n1)
   *
   * @param k1 reference to node1's key
   * @param n2 reference to node2's key
   */
  void remove_edge(const Key& k1, const Key& k2) {
    const_iterator n1 = insert(k1).first;
    const_iterator n2 = insert(k2).first;
    remove_edge(n1, n2);
  }

  /**
   * @brief Remove the edge between n1 and n2
   *
   * @details if the graph is undirected also removes the backward edge
   * (n2 -> n1)
   *
   * @param k1 key to node1
   * @param k2 key to node2
   */
  void remove_edge(Key&& k1, Key&& k2) {
    const_iterator n1 = emplace(std::move(k1)).first;
    const_iterator n2 = emplace(std::move(k2)).first;
    remove_edge(n1, n2);
  }

  /*
   *    ==== Capacity ====
   */

  /**
   * @brief Checks if the container has no elements, i.e. whether begin() ==
   * end()
   *
   * @return true if the container is empty
   * @return false otherwise
   */
  bool empty() const noexcept { return nodes_.empty(); }

  /**
   * @brief Returns the number of elements in the container, i.e.
   * std::distance(begin(), end())
   *
   * @return size_type The number of elements in the container.
   */
  size_type size() const noexcept { return nodes_.size(); }

  /**
   * @brief Get the graph type
   *
   * @return GraphType type of graph (directed / undirected)
   */
  GraphType get_graph_type() const noexcept { return GT; }

  /*
   *    ==== Iterators ====
   */

  iterator begin() noexcept { return nodes_.begin(); }
  const_iterator begin() const noexcept { return nodes_.cbegin(); }
  const_iterator cbegin() const noexcept { return nodes_.cbegin(); }

  iterator end() noexcept { return nodes_.end(); }
  const_iterator end() const noexcept { return nodes_.cend(); }
  const_iterator cend() const noexcept { return nodes_.cend(); }

 private:
  std::unordered_map<Key, NodePtr> nodes_;  // nodes inside graph
  Cost max_cost_;                           // this is cost type numeric limit
};  // class Graph

// defining graphs aliases
template <class Key, class Data, class Cost = std::size_t>
using DirectedGraph = Graph<Key, Data, Cost, directed>;

template <class Key, class Data, class Cost = std::size_t>
using UndirectedGraph = Graph<Key, Data, Cost, undirected>;

}  // namespace graph
}  // namespace unialgo

#endif