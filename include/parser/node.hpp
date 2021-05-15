#pragma once
#include <algorithm>
#include <memory>
#include <ranges>
#include <vector>

#include "lexicalAnalyzer/token.hpp"

class Node;
using NodeUptr = std::unique_ptr<Node>;

class Node {
 public:
  Node() : token(Token(Token::TokenType::StartOfFileToken)){};
  Node(Token token) : token(token){};
  Node& operator=(Node&& other) = default;
  Node(const Node& other) = default;
  Node(Node&& other) = default;
  void add(NodeUptr& newNode) {
    children.push_back(std::move(newNode));
  }
  void remove(NodeUptr& node) {
    auto itr = std::ranges::find_if(children, [&node](NodeUptr& p){
      return p.get() == node.get();
    });
    children.erase(itr);
  }
  virtual void evaluate() = 0;
  virtual ~Node() = default;

 private:
  Token token;
  std::vector<NodeUptr> children;
};

class AdditiveOperatorNode : public Node {
  void evaluate() override {}
};

class MultiplicativeOperatorNode : public Node {
  void evaluate() override {}
};

class LogicalOperatorNode : public Node {
  void evaluate() override {}
};

class MatrixOperatorNode : public Node{
  void evaluate() override {}
};

class StatementNode : public Node {
  void evaluate() override {}
};
