#pragma once
#include "parser/node.hpp"

class ValueNode;
class MatrixValueNode;
class ExpressionValueNode;
class ArgumentNode;
class MatrixSizeNode;
class VariableNode;
class MatrixVariable;

using ExpressionNodeUptr = std::unique_ptr<ExpressionNode>;
using ValueNodeUptr = std::unique_ptr<ValueNode>;
using MatrixValueNodeUptr = std::unique_ptr<MatrixValueNode>;
using ExpressionValueNodeUptr = std::unique_ptr<ExpressionValueNode>;
using ArgumentNodeUptr = std::unique_ptr<ArgumentNode>;
using MatrixSizeNodeUptr = std::unique_ptr<MatrixSizeNode>;
using VariableNodeUptr = std::unique_ptr<VariableNode>;
using MatrixVariableUptr = std::unique_ptr<MatrixVariable>;

class ExpressionValueNode : public ExpressionNode {
 public:
  ExpressionValueNode(Token token) : ExpressionNode(token) {}

  void add(ExpressionNodeUptr newNode);
  void add(std::vector<ExpressionNodeUptr>& newNodes);
  void remove(const ExpressionNode* node);
  void buildTreeStringStream(int64_t depth,
                             std::stringstream& tree) const override;
  const std::vector<ExpressionNodeUptr>& getChildren() const { return expressions; }

 private:
  std::vector<ExpressionNodeUptr> expressions;
};

class ExpressionLeafNode : public ExpressionNode {
 public:
  ExpressionLeafNode(Token token) : ExpressionNode(token) {}
  void buildTreeStringStream(int64_t depth,
                             std::stringstream& tree) const override {
    std::string indent(depth, ' ');
    tree << indent << LexicalTable::token2StringTable.at(token.getType())
         << '\n';
  }

 private:
};

class AdditiveOperatorNode : public ExpressionValueNode {
 public:
  enum class NodeType { Plus, Minus };
  AdditiveOperatorNode(Token token) : ExpressionValueNode(token) {
    if (token.getSubtype() == Token::TokenSubtype::PlusToken)
      this->type = NodeType::Plus;
    else
      this->type = NodeType::Minus;
  }

 private:
  NodeType type;
};

class MultiplicativeOperatorNode : public ExpressionValueNode {
 public:
  MultiplicativeOperatorNode(Token token) : ExpressionValueNode(token) {
    if (token.getSubtype() == Token::TokenSubtype::MultiplicationToken)
      this->type = NodeType::Multiplication;
    else
      this->type = NodeType::Division;
  }

 private:
  enum class NodeType { Multiplication, Division };
  NodeType type;
};

class LogicalOperatorNode : public ExpressionValueNode {
 public:
  LogicalOperatorNode(Token token);

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

class MatrixOperatorNode : public ExpressionValueNode {
 public:
  MatrixOperatorNode(Token token);

 private:
  enum class NodeType { Det, Transpose, Inverse };
  NodeType type;
};

class AssigmentNode : public ExpressionValueNode {
 public:
  AssigmentNode(Token token) : ExpressionValueNode(token){};

 private:
};

class ExponentiationOperatorNode : public ExpressionValueNode {
 public:
  ExponentiationOperatorNode(Token token) : ExpressionValueNode(token) {}
};

class ValueNode : public ExpressionLeafNode {
 public:
  using ExpressionLeafNode::ExpressionLeafNode;
  ValueNode(Token token) : ExpressionLeafNode(token){};
  bool isLeaf() const override { return true; }

 private:
};

class MatrixValueNode : public ValueNode {
 public:
  MatrixValueNode(std::vector<ExpressionNodeUptr>& newValues)
      : ValueNode(Token(Token::TokenType::MatrixLiteralToken)) {
    std::ranges::move(newValues, std::back_inserter(values));
  };

  void buildTreeStringStream(int64_t depth,
                             std::stringstream& tree) const override;

 private:
  std::vector<ExpressionNodeUptr> values;
};

class MatrixSizeNode : public ValueNode {
 public:
  MatrixSizeNode(std::vector<ExpressionNodeUptr>& sizes)
      : ValueNode(Token(Token::TokenType::MatrixLiteralToken)) {
    std::ranges::move(sizes, std::back_inserter(values));
  };

 private:
  std::vector<ExpressionNodeUptr> values;
};

class VariableNode : public ExpressionLeafNode {
 public:
  VariableNode(Token token) : ExpressionLeafNode(token) {}
  void setIdentifier(std::string identifier) { this->identifier = identifier; }
  void setValue(ValueNodeUptr value) { this->variableValue = std::move(value); }

 protected:
  std::string identifier;
  ValueNodeUptr variableValue;
};

class MatrixVariable : public VariableNode {
 public:
  MatrixVariable(Token token) : VariableNode(token) {}
  void setMatrixSize(MatrixSizeNodeUptr size) {
    this->matrixSize = std::move(size);
  }
  void setValue(MatrixValueNodeUptr value) {
    this->variableValue = std::move(value);
  }

  void buildTreeStringStream(int64_t depth,
                             std::stringstream& tree) const override;

 private:
  MatrixSizeNodeUptr matrixSize;
};

class ArgumentNode : public ExpressionLeafNode {
 public:
  ArgumentNode(Token token) : ExpressionLeafNode(token){};
  void setIdentifier(std::string identifier) { this->identifier = identifier; }
  void setDefaultValue(ExpressionNodeUptr value) {
    this->defaultValue = std::move(value);
  }
  void buildTreeStringStream(std::stringstream& tree) const;

 private:
  std::string identifier;
  ExpressionNodeUptr defaultValue;
};