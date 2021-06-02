#pragma once
#include <algorithm>
#include <memory>
#include <ranges>
#include <vector>

#include "lexicalAnalyzer/lexicalTable.hpp"
#include "lexicalAnalyzer/token.hpp"
#include "semanticAnalyzer/semanticAnalyzer.hpp"


class Node;
using NodeUptr = std::unique_ptr<Node>;
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
  
  virtual void accept(SemanticAnalyzer& semAnalyzer){};

  const std::string getPrintTree() const {
    std::stringstream ss;
    buildTreeStringStream(0, ss);
    return ss.str();
  }

 protected:
  Token token;
};

class RootNode : public Node {
 public:
  RootNode() : Node(Token(Token::TokenType::RootToken)) {}
};

class StatementNode : public Node {
 public:

  using Node::Node;
};

class ExpressionNode : public StatementNode {
  public:
  ExpressionNode(Token token) : StatementNode(token) {}
};