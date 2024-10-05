#ifndef UNIALGO_GRAPH_NODE_
#define UNIALGO_GRAPH_NODE_

#include <cassert>  // assert
#include <memory>   // std::shared_ptr, std::weak_ptr
#include <vector>   // std::vector

namespace unialgo {
namespace graph {

template <class Key, class Data, class Cost = std::size_t>
class Node {
 private:
  class Edge {
   public:
    explicit Edge(const std::weak_ptr<Node> node_to, Cost& cost)
        : node_to_(node_to), cost_(std::make_shared<Cost>(cost)) {}

    Edge(const std::weak_ptr<Node> node_to, std::shared_ptr<Cost> cost)
        : node_to_(node_to), cost_(cost) {}

    Edge(const Edge& other) : node_to_(other.node_to_), cost_(other.cost_) {}

    /**
     * @brief Access cost of edge
     *
     * @return Cost& reference to cost traversing edge
     */
    Cost& cost() { return *cost_; }

    /**
     * @brief Get the cost ptr of edge
     *
     * @return std::shared_ptr<Cost> shared pointer to edge cost
     */
    std::shared_ptr<Cost> get_cost_ptr() { return cost_; }

    /**
     * @brief Get the node ptr object
     *
     * @return std::weak_ptr<Node> weak pointer to node
     */
    std::weak_ptr<Node> get_node_ptr() { return node_to_; }

    friend class Node<Key, Data, Cost>;

   private:
    std::shared_ptr<Cost> cost_;   // pointer to edge's cost
    std::weak_ptr<Node> node_to_;  // pointer to node connected by edge
  };

 public:
  using NodePtr = std::shared_ptr<Node>;
  using iterator = typename std::vector<Edge>::iterator;
  using const_iterator = typename std::vector<Edge>::const_iterator;

  explicit Node() {}
  Node(const Data& data) : data_(data) {}
  Node(Data&& data) : data_(std::move(data)) {}
  Node(const Node& node) : data_(node.data_) {}
  Node(Node&& other) : data_(std::move(other.data_)) {}

  /**
   * @brief Find node inside edge lis
   *
   * @param toPtr pointer of node to find
   * @return const_iterator of edge containing node (.end() if node connected)
   */
  const_iterator find(NodePtr toPtr) const {
    assert(toPtr != nullptr);
    for (const_iterator it = edge_list_.begin(); it != edge_list_.end(); ++it) {
      if (it->node_to_.lock() == toPtr) return it;
    }
    return edge_list_.end();
  }

  /**
   * @brief Find node inside edge list
   *
   * @param toPtr pointer of node to find
   * @return iterator of edge containing node (.end() if node connected)
   */
  iterator find(NodePtr toPtr) {
    assert(toPtr != nullptr);
    for (iterator it = edge_list_.begin(); it != edge_list_.end(); ++it) {
      if (it->node_to_.lock() == toPtr) return it;
    }
    return edge_list_.end();
  }

  /**
   * @brief Check if node is inside edge list
   *
   * @param toPtr node to check
   * @return true toPtr node is connected to this node
   * @return false otherwise
   */
  bool is_edge(NodePtr toPtr) const { return find(toPtr) != edge_list_.end(); }

  /**
   * @brief Insert an edge in node adj list, if edge alrady in adj list returns
   * position
   *
   * @param pos iterator before which the content will be inserted
   * @param toPtr node to construct edge to
   * @param cost cost of edge from this to toPtr
   * @return std::pair<iterator, bool> A pair consisting of an iterator to the
   * inserted edge (or the edge that prevented the insertion) and a
   * bool value set to true if and only if the insertion took place.
   */
  std::pair<iterator, bool> insert(const_iterator pos, const NodePtr toPtr,
                                   Cost& cost) {
    assert(toPtr != nullptr);

    iterator it = find(toPtr);  // check if toPtr is alrady an edge
    if (it != edge_list_.end()) {
      return std::make_pair({it, false});  // return current edge position
    }
    // insert edge in pos
    return std::make_pair({edge_list_.insert(pos, toPtr, cost), true});
  }

  /**
   * @brief Insert an edge at the end of the adj list, if edge alrady in adj
   * list returns position
   *
   * @param toPtr node to construct edge to
   * @param cost cost of edge from this to toPtr
   * @return std::pair<iterator, bool> A pair consisting of an iterator to the
   * inserted edge (or the edge that prevented the insertion) and a
   * bool value set to true if and only if the insertion took place.
   */
  std::pair<iterator, bool> insert_back(const NodePtr toPtr, Cost& cost) {
    return insert(edge_list_.end(), toPtr, cost);
  }

  /**
   * @brief Insert an edge at end of adj list
   *
   * @attention Does not check if edge is alrady inside the adj list, this
   * should be verified by the caller
   *
   * @param toPtr node to construct edge to
   * @param cost cost of edge from this to toPtr
   * @return iterator pointing to inserted value
   */
  iterator push_back(const NodePtr toPtr, Cost& cost) {
    assert(toPtr != nullptr);
    // return edge_list_.push_back(toPtr, cost);
    return push_back(toPtr, std::make_shared<Cost>(cost));
  }

  /**
   * @brief Insert an edge at end of adj list
   *
   * @attention Does not check if edge is alrady inside the adj list, this
   * should be verified by the caller
   *
   * @param toPtr node to construct edge to
   * @param costPtr cost of edge from this to toPtr
   * @return iterator pointing to inserted value
   */
  iterator push_back(const NodePtr toPtr, std::shared_ptr<Cost> costPtr) {
    assert(toPtr != nullptr);
    edge_list_.push_back(Edge(toPtr, costPtr));
    return --edge_list_.end();
  }

  /**
   * @brief Remove edge from adj list of node
   *
   * @param toPtr pointer to the node that should be removed
   */
  void remove_edge(NodePtr toPtr) {
    assert(toPtr != nullptr);

    auto pred = [&toPtr](Edge& e) {
      // check if pointer is still valid
      if (auto adj_node_ptr = e.get_node_ptr().lock()) {
        if (adj_node_ptr == toPtr) return true;  // points to same node
        return false;                            // different edge don't erase
      }
      return true;  // node ptr is not valid remove it
    };

    std::erase_if(edge_list_, pred);
  }

  /**
   * @brief Get key used for this node inside Graph
   *
   * @return Key of this inside graph
   */
  const Key& key() const { return key_; }

  /**
   * @brief Set the key object
   *
   * @param k new Key
   */
  void set_key(const Key& k) { key_ = k; }

  /**
   * @brief Get the Data object
   *
   * @return Data& reference to data inside node
   */
  Data& data() { return data_; }

  /**
   * @brief Get the Data object
   *
   * @return Data copy of data inside node
   */
  const Data& data() const { return data_; }

  /*
   *    ==== Iterators ====
   */

  iterator begin() noexcept { return edge_list_.begin(); }
  const_iterator begin() const noexcept { return edge_list_.cbegin(); }
  const_iterator cbegin() const noexcept { return edge_list_.cbegin(); }

  iterator end() noexcept { return edge_list_.end(); }
  const_iterator end() const noexcept { return edge_list_.cend(); }
  const_iterator cend() const noexcept { return edge_list_.cend(); }

 private:
  std::vector<Edge> edge_list_;  // list of edges in node
  Data data_;                    // data inside node
  Key key_;                      // key inside graph to reference this node
  // ^ make this pointer (?) might be less memory for complex keys
};  // class Node

}  // namespace graph
}  // namespace unialgo

#endif