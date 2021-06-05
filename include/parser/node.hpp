#pragma once
#include <algorithm>
#include <memory>
#include <ranges>
#include <vector>

#include "interpreter/evaluator.hpp"
#include "lexicalAnalyzer/lexicalTable.hpp"
#include "lexicalAnalyzer/token.hpp"

class Node;
using NodeUptr = std::unique_ptr<Node>;
using Value = std::variant<int64_t, double, std::string, Matrix>;

class Node {
 public:
  Node() : token(Token(Token::TokenType::RootToken)){};
  explicit Node(Token token) : token(token){};
  Node& operator=(Node&& other) = default;
  Node(const Node& other) = default;
  Node(Node&& other) = default;
  Token getToken() const { return token; }

  virtual bool isLeaf() const {
    if (token == Token::TokenType::StringLiteralToken ||
        token == Token::TokenType::IntegerLiteralToken ||
        token == Token::TokenType::DoubleLiteralToken)
      return true;
    return false;
  }

  virtual void buildTreeStringStream(int64_t depth,
                                     std::stringstream& tree) const {
    std::string indent(depth, ' ');
    tree << indent << LexicalTable::token2StringTable.at(token.getType())
         << '\n';
  }
  virtual ~Node() = default;

  const std::string getPrintTree() const {
    std::stringstream ss;
    buildTreeStringStream(0, ss);
    return ss.str();
  }

 protected:
  Token token;
};

class StatementNode : public Node {
 public:
  using Node::Node;
  virtual Value accept(const Evaluator& evaluator) const = 0;
};

class ExpressionNode : public StatementNode {
 public:
  ExpressionNode(Token token) : StatementNode(token) {}
  Value accept(const Evaluator& evaluator) const =0;
};
