#pragma once
#include "parser/node.hpp"

class ExpressionNode;
class ValueNode;
class MatrixValueNode;
class ExpressionValueNode;
class ArgumentNode;
class MatrixSizeNode;
class VariableNode;
class MatrixVariable;
class AssignmentNode;
class AdditiveOperatorNode;
class MultiplicativeOperatorNode;
class LogicalOperatorNode;
class MatrixOperatorNode;
class ExponentiationOperatorNode;
class AssignNewValueNode;
class FunctionCallNode;


using FunctionCallNodeUptr = std::unique_ptr<FunctionCallNode>;
using AssignNewValueNodeUptr = std::unique_ptr<AssignNewValueNode>;
using ExpressionNodeUptr = std::unique_ptr<ExpressionNode>;
using ValueNodeUptr = std::unique_ptr<ValueNode>;
using MatrixValueNodeUptr = std::unique_ptr<MatrixValueNode>;
using ExpressionValueNodeUptr = std::unique_ptr<ExpressionValueNode>;
using ArgumentNodeUptr = std::unique_ptr<ArgumentNode>;
using MatrixSizeNodeUptr = std::unique_ptr<MatrixSizeNode>;
using VariableNodeUptr = std::unique_ptr<VariableNode>;
using MatrixVariableUptr = std::unique_ptr<MatrixVariable>;
using AssignmentNodeUptr = std::unique_ptr<AssignmentNode>;
using AdditiveOperatorNodeUptr = std::unique_ptr<AdditiveOperatorNode>;
using MultiplicativeOperatorNodeUptr =
    std::unique_ptr<MultiplicativeOperatorNode>;
using LogicalOperatorNodeUptr = std::unique_ptr<LogicalOperatorNode>;
using MatrixOperatorNodeUptr = std::unique_ptr<MatrixOperatorNode>;
using ExponentiationOperatorNodeUptr =
    std::unique_ptr<ExponentiationOperatorNode>;

class ExpressionValueNode : public ExpressionNode {
 public:
  ExpressionValueNode(Token token) : ExpressionNode(token) {}
  void add(ExpressionNodeUptr newNode);
  void add(std::vector<ExpressionNodeUptr>& newNodes);
  void remove(const ExpressionNode* node);
  void buildTreeStringStream(int64_t depth,
                             std::stringstream& tree) const override;
  const std::vector<ExpressionNodeUptr>& getChildren() const {
    return expressions;
  }

  const ExpressionNode* getLeft() const { return expressions[0].get(); }
  const ExpressionNode* getRight() const {
    if (expressions.size() == 2) {
      return expressions[1].get();
    }
    return nullptr;
  }

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

  Value accept(Evaluator& evaluator) const override {
    return evaluator.evaluate(this);
  }
  NodeType getType() const { return type; }

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

  Value accept(Evaluator& evaluator) const override {
    return evaluator.evaluate(this);
  }
  enum class NodeType { Multiplication, Division };
  NodeType getType() const { return type; }

 private:
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
    Equal,
    And,
    Or,
    Not
  };

  Value accept(Evaluator& evaluator) const override {
    return evaluator.evaluate(this);
  }
  NodeType getType() const { return type; }

 private:
  NodeType type;
};

class MatrixOperatorNode : public ExpressionValueNode {
 public:
  Value accept(Evaluator& evaluator) const override {
    return evaluator.evaluate(this);
  }
  MatrixOperatorNode(Token token);
  enum class NodeType { Det, Transpose, Inverse };
  NodeType getType() const { return type; }

 private:
  NodeType type;
};

class AssignmentNode : public ExpressionValueNode {
 public:
  AssignmentNode(Token token) : ExpressionValueNode(token){};
  Value accept(Evaluator& evaluator) const override {
    return evaluator.evaluate(this);
  }

 private:
};

class AssignNewValueNode : public AssignmentNode {
 public:
  AssignNewValueNode(Token token) : AssignmentNode(token){};
  Value accept(Evaluator& evaluator) const override {
    return evaluator.evaluate(this);
  }

 private:
};

class ExponentiationOperatorNode : public ExpressionValueNode {
 public:
  ExponentiationOperatorNode(Token token) : ExpressionValueNode(token) {}

  Value accept(Evaluator& evaluator) const override {
    return evaluator.evaluate(this);
  }
};

class ValueNode : public ExpressionLeafNode {
 public:
  using ExpressionLeafNode::ExpressionLeafNode;
  Value accept(Evaluator& evaluator) const override {
    return evaluator.evaluate(this);
  }

  ValueNode(Token token) : ExpressionLeafNode(token){};
  bool isLeaf() const override { return true; }
  virtual const Value getValue() const {
    switch (token.getValue().index()) {
      case 1:
        return token.getInt();
      case 2:
        return token.getDouble();
      case 3:
        return token.getString();
      default:
        return {};
    }
  }

 private:
};

class IdentifierNode : public ValueNode {
 public:
  Value accept(Evaluator& evaluator) const override {
    return evaluator.evaluate(this);
  }
  IdentifierNode(Token token) : ValueNode(token){};
  const Value getValue() const override { return token.getString(); }
};

class MatrixValueNode : public ValueNode {
 public:
  MatrixValueNode(std::vector<ExpressionNodeUptr>& newValues)
      : ValueNode(Token(Token::TokenType::MatrixLiteralToken)) {
    std::ranges::move(newValues, std::back_inserter(values));
  };
  void buildTreeStringStream(int64_t depth,
                             std::stringstream& tree) const override;

  const std::vector<ExpressionNodeUptr>& getValues() const { return values; }

  Value accept(Evaluator& evaluator) const override {
    return evaluator.evaluate(this);
  }

 private:
  std::vector<ExpressionNodeUptr> values;
};

class MatrixSizeNode : public ValueNode {
 public:
  MatrixSizeNode(std::vector<ExpressionNodeUptr>& sizes)
      : ValueNode(Token(Token::TokenType::MatrixLiteralToken)) {
    std::ranges::move(sizes, std::back_inserter(values));
  };
  void buildTreeStringStream(int64_t depth,
                             std::stringstream& tree) const override;
  const std::vector<ExpressionNodeUptr>& getValues() const { return values; }
  Value accept(Evaluator& evaluator) const override {
    return evaluator.evaluate(this);
  }

 private:
  std::vector<ExpressionNodeUptr> values;
};

class VariableNode : public ExpressionLeafNode {
 public:
  VariableNode(Token token) : ExpressionLeafNode(token) {}
  void setIdentifier(std::string identifier) { this->identifier = identifier; }
  std::string getIdentifier() const { return identifier; }
  void setValue(ValueNodeUptr value) { this->variableValue = std::move(value); }
  const ValueNode* getValue() const { return variableValue.get(); }

  Value accept(Evaluator& evaluator) const override {
    return evaluator.evaluate(this);
  }

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

  const MatrixSizeNode* getMatrixSizeNode() const { return matrixSize.get(); }

  void buildTreeStringStream(int64_t depth,
                             std::stringstream& tree) const override;

  Value accept(Evaluator& evaluator) const override {
    return evaluator.evaluate(this);
  }

 private:
  MatrixSizeNodeUptr matrixSize;
};

class ArgumentNode : public ExpressionLeafNode {
 public:
  ArgumentNode(Token token) : ExpressionLeafNode(token){};
  void setIdentifier(std::string identifier) { this->identifier = identifier; }
  std::string getIdentifier() const { return identifier; }
  void buildTreeStringStream(std::stringstream& tree) const;

  Value accept(Evaluator& evaluator) const override {
    return evaluator.evaluate(this);
  }

 private:
  std::string identifier;
};

class FunctionCallNode : public ExpressionValueNode {
 public:
  FunctionCallNode(Token token) : ExpressionValueNode(token){};
  void setIdentifier(std::string identifier) { this->identifier = identifier; }
  void setArgumentsNodes(std::vector<ExpressionNodeUptr>& arguments) {
    std::ranges::move(arguments, std::back_inserter(this->arguments));
  }

  std::string getIdentifier() const { return identifier; }
  const std::vector<ExpressionNodeUptr>& getArguments() const {
    return arguments;
  }
  void buildTreeStringStream(int64_t depth,
                             std::stringstream& tree) const override;
  Value accept(Evaluator& evaluator) const override {
    return evaluator.evaluate(this);
  }

 private:
  std::string identifier;
  std::vector<ExpressionNodeUptr> arguments;
};