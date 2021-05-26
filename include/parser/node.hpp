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
  Token getToken() const { return token; }

  virtual bool isLeaf() const {
    if (token == Token::TokenType::StringLiteralToken ||
        token == Token::TokenType::IntegerLiteralToken ||
        token == Token::TokenType::DoubleLiteralToken)
      return true;
    return false;
  }

  virtual void evaluate() = 0;
  virtual void buildTreeStringStream(int64_t depth,
                                     std::stringstream& tree) const {
    std::string indent(depth,  ' ');
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
  void evaluate() override {}
  void add(NodeUptr newNode);
  void add(std::vector<NodeUptr>& newNodes);
  const std::vector<NodeUptr>& getChildren() const { return children; }
  void buildTreeStringStream(int64_t depth, std::stringstream& tree) const override;
  void remove(const Node* node);

 protected:
  std::vector<NodeUptr> children;
};

class BinaryOperatorNode : public Node {
 public:
  BinaryOperatorNode(Token token, NodeUptr left, NodeUptr right)
      : Node(token), left(std::move(left)), right(std::move(right)) {}
  BinaryOperatorNode(Token token) : Node(token){};
  void evaluate() override {}
  void buildTreeStringStream(int64_t depth, std::stringstream& tree) const override;
  void setLeft(NodeUptr left) { this->left = std::move(left); }
  void setRight(NodeUptr right) { this->right = std::move(right); }

 protected:
  NodeUptr left;
  NodeUptr right;
};

class AdditiveOperatorNode : public BinaryOperatorNode {
 public:
  enum class NodeType { Plus, Minus };
  AdditiveOperatorNode(Token token) : BinaryOperatorNode(token) {
    if (token.getSubtype() == Token::TokenSubtype::PlusToken)
      this->type = NodeType::Plus;
    else
      this->type = NodeType::Minus;
  }
  void evaluate() override {}

 private:
  NodeType type;
};

class MultiplicativeOperatorNode : public BinaryOperatorNode {
 public:
  MultiplicativeOperatorNode(Token token) : BinaryOperatorNode(token) {
    if (token.getSubtype() == Token::TokenSubtype::MultiplicationToken)
      this->type = NodeType::Multiplication;
    else
      this->type = NodeType::Division;
  }
  void evaluate() override {}

 private:
  enum class NodeType { Multiplication, Division };
  NodeType type;
};

class LogicalOperatorNode : public BinaryOperatorNode {
 public:
  LogicalOperatorNode(Token token);
  void evaluate() override {}

  enum class NodeType {
    Greater,
    GreaterEqual,
    Less,
    LessEqual,
    NotEqual,
    Equal
  };

 private:
  NodeType type;
};

class MatrixOperatorNode : public Node {
 public:
  using Node::Node;
  void evaluate() override {}

 private:
  enum class NodeType { Det, Transpose, Inverse };
  NodeType type;
  Token operatorToken;
  std::optional<std::string> identifier;
  std::optional<Matrix> matrix;
};

class VariableNode : public Node {
 public:
 private:
  Token typeToken;
  std::string identifier;
};

class LiteralNode : public Node {
 public:
  using Node::Node;
  void evaluate() override {}
  bool isLeaf() const override { return true; }

 private:
  std::optional<Matrix> matrix;
};

class RootNode : public Node {
 public:
  RootNode() : Node(Token(Token::TokenType::RootToken)) {}
  void evaluate() override {}
};

class AssigmentOperatorNode : public BinaryOperatorNode {
 public:
  void evaluate() override {}

 private:
};

class ExponentiationOperatorNode : public BinaryOperatorNode {
 public:
  void evaluate() override {}
};

class IfStatementNode : public StatementNode {
 public:
  void evaluate() override {}
};

class LoopStatementNode : public StatementNode {
 public:
  LoopStatementNode(Token token) : StatementNode(token) {}
  void evaluate() override {}

  void setStep(TokenVariant newStep);
  void setEnd(TokenVariant newEnd);
  void setStart(TokenVariant newStart);

 private:
  std::variant<std::string, int64_t> start = 0;
  std::variant<std::string, int64_t> end = 0;
  std::variant<std::string, int64_t> step = 1;
};

class AslasStatementNode : public StatementNode {
 public:
  void evaluate() override {}

 private:
};

class ArgumentNode : public Node {
 public:
 private:
  Token typeToken;
  std::string identifier;
  std::optional<TokenVariant> defaultValue;
};

class FunctionStatementNode : public StatementNode {
 public:
  void evaluate() override {}

 private:
  Token returnType;
  std::string identifier;
  std::vector<ArgumentNode> arguments;
};

class FunctionCallNode : public StatementNode {
 public:
  void evaluate() override {}

 private:
  std::string identifier;
  std::vector<ArgumentNode> arguments;
};
