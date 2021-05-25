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
  explicit Node(Token token) : token(token){};
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
  std::vector<NodeUptr>& getChildren()  { return children; }
  Token getToken() const { return token; }
  void add(NodeUptr newNode) { children.push_back(std::move(newNode)); }
  void add(std::vector<NodeUptr>& newNodes) {
    std::ranges::move(newNodes, std::back_inserter(children));
  }

  void remove(const Node* node) {
    children.erase(std::ranges::find(children, node, &NodeUptr::get));
  }

  bool empty() const { return children.empty(); }
  virtual void evaluate() = 0;
  virtual ~Node() = default;

 protected:
  Token token;
  std::vector<NodeUptr> children;
};

class CommentNode : public Node {
 public:
  using Node::Node;
  void evaluate() override {}
};

class AdditiveOperatorNode : public Node {
 public:
  using Node::Node;
  void evaluate() override {}
};

class MultiplicativeOperatorNode : public Node {
 public:
  using Node::Node;
  void evaluate() override {}
};

class LogicalOperatorNode : public Node {
 public:
  using Node::Node;
  void evaluate() override {}
};

class MatrixOperatorNode : public Node {
 public:
  using Node::Node;
  void evaluate() override {}
};

class StatementNode : public Node {
 public:
  using Node::Node;
  void evaluate() override {}
};

class LiteralNode : public Node {
 public:
  using Node::Node;
  void evaluate() override {}
};

class LeafNode : public Node {
 public:
  using Node::Node;
  void evaluate() override {}
};

class RootNode : public Node {
 public:
  RootNode() : Node(Token(Token::TokenType::RootToken)) {}
  void evaluate() override {}
};

class AssigmentOperatorNode : public Node {
 public:
  using Node::Node;
  void evaluate() override {}
};

class ExponentiationOperatorNode : public Node {
 public:
  using Node::Node;
  void evaluate() override {}
};