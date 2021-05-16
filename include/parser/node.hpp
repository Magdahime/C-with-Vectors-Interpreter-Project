#pragma once
#include <algorithm>
#include <memory>
#include <ranges>
#include <vector>

#include "lexicalAnalyzer/lexicalTable.hpp"
#include "lexicalAnalyzer/token.hpp"

class Node;
using NodeUptr = std::unique_ptr<Node>;

class Node {
 public:
  Node() : token(Token(Token::TokenType::RootToken)){};
  Node(Token token) : token(token){};
  Node& operator=(Node&& other) = default;
  Node(const Node& other) = default;
  Node(Node&& other) = default;
  void printChildren() {
    std::cout << "Nodes:"
              << "\n";
    for (auto const& node : children) {
      std::cout << LexicalTable::token2StringTable.at(
                       node->getToken().getType())
                << "\n";
    }
  }
  std::vector<NodeUptr>& getChildren() { return children; }
  Token getToken() const { return token; }
  void add(NodeUptr newNode) { children.push_back(std::move(newNode)); }
  void add(std::vector<NodeUptr>& newNodes) {
    while (!newNodes.empty()) {
      auto newNode = std::move(newNodes.front());
      newNodes.erase(newNodes.begin());
      children.push_back(std::move(newNode));
    }
  }
  void remove(NodeUptr& node) {
    auto itr = std::ranges::find_if(
        children, [&node](NodeUptr& p) { return p.get() == node.get(); });
    children.erase(itr);
  }
  bool empty() { return children.empty(); }
  virtual void evaluate() = 0;
  virtual ~Node() = default;

 protected:
  Token token;
  std::vector<NodeUptr> children;
};

class CommentNode : public Node {
 public:
  CommentNode(Token token) : Node(token) {}
  void evaluate() override {}
  ~CommentNode() override {}
};

class AdditiveOperatorNode : public Node {
 public:
  AdditiveOperatorNode(Token token) : Node(token) {}
  void evaluate() override {}
  ~AdditiveOperatorNode() override {}
};

class MultiplicativeOperatorNode : public Node {
 public:
  MultiplicativeOperatorNode(Token token) : Node(token) {}
  void evaluate() override {}
  ~MultiplicativeOperatorNode() override {}
};

class LogicalOperatorNode : public Node {
 public:
  LogicalOperatorNode(Token token) : Node(token) {}
  void evaluate() override {}
  ~LogicalOperatorNode() override {}
};

class MatrixOperatorNode : public Node {
 public:
  MatrixOperatorNode(Token token) : Node(token) {}
  void evaluate() override {}
  ~MatrixOperatorNode() override {}
};

class StatementNode : public Node {
 public:
  StatementNode(Token token) : Node(token) {}
  void evaluate() override {}
  ~StatementNode() override {}
};

class LiteralNode : public Node {
 public:
  LiteralNode(Token token) : Node(token) {}
  void evaluate() override {}
  ~LiteralNode() override {}
};

class LeafNode : public Node {
 public:
  LeafNode(Token token) : Node(token) {}
  void evaluate() override {}
  ~LeafNode() override {}
};

class RootNode : public Node {
 public:
  RootNode() : Node(Token(Token::TokenType::RootToken)) {}
  void evaluate() override {}
  ~RootNode() override {}
};

class AssigmentOperatorNode : public Node {
 public:
  AssigmentOperatorNode(Token token) : Node(token) {}
  void evaluate() override {}
  ~AssigmentOperatorNode() override {}
};