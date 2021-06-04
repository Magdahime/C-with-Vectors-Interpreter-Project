#include "interpreter/evaluator.hpp"

#include "parser/statementNode.hpp"

Value Evaluator::evaluate(const ValueNode* node) const {
  return node->getValue();
}

Value Evaluator::evaluate(const IdentifierNode* node) const {
  std::string identifier = std::get<std::string>(node->getValue());
  return searchVariable(variableMap, identifier, currentDepth)->value;
}

Value Evaluator::evaluate(const AdditiveOperatorNode* node) const {
  auto left = node->getLeft();
  auto right = node->getRight();
  bool isPlus = (node->getType() == AdditiveOperatorNode::NodeType::Plus);
  Value leftValue = left->accept(*this);
  if (right) {
    Value rightValue = right->accept(*this);
    int signature =
        std::variant_size_v<Value> * leftValue.index() + rightValue.index();
    switch (static_cast<OperatorSignatures>(signature)) {
      case OperatorSignatures::INTEGER_INTEGER:
        if (isPlus)
          return std::get<int64_t>(leftValue) + std::get<int64_t>(rightValue);
        else
          return std::get<int64_t>(leftValue) - std::get<int64_t>(rightValue);
      case OperatorSignatures::INTEGER_DOUBLE:
        if (isPlus)
          return std::get<int64_t>(leftValue) + std::get<double>(rightValue);
        else
          return std::get<int64_t>(leftValue) - std::get<double>(rightValue);
      case OperatorSignatures::DOUBLE_INTEGER:
        if (isPlus)
          return std::get<double>(leftValue) + std::get<int64_t>(rightValue);
        else
          return std::get<double>(leftValue) - std::get<int64_t>(rightValue);
      case OperatorSignatures::DOUBLE_DOUBLE:
        if (isPlus)
          return std::get<double>(leftValue) + std::get<double>(rightValue);
        else
          return std::get<double>(leftValue) - std::get<double>(rightValue);
      case OperatorSignatures::STRING_STRING:
        if (isPlus)
          return std::get<std::string>(leftValue) +
                 std::get<std::string>(rightValue);
        else
          throw SemanticError("Invalid operation in AdditiveOperator at " +
                              node->getToken().getLinePositionString() +
                              ". Operator-(const std::string&, "
                              "std::string& ) is undefinded.");
      case OperatorSignatures::MATRIX_INTEGER:
        if (isPlus)
          return std::get<Matrix>(leftValue) + std::get<int64_t>(rightValue);
        else
          return std::get<Matrix>(leftValue) - std::get<int64_t>(rightValue);
      case OperatorSignatures::INTEGER_MATRIX:
        if (isPlus)
          return std::get<int64_t>(leftValue) + std::get<Matrix>(rightValue);
        else
          return std::get<int64_t>(leftValue) - std::get<Matrix>(rightValue);
      case OperatorSignatures::MATRIX_DOUBLE:
        if (isPlus)
          return std::get<Matrix>(leftValue) + std::get<double>(rightValue);
        else
          return std::get<Matrix>(leftValue) - std::get<double>(rightValue);
      case OperatorSignatures::DOUBLE_MATRIX:
        if (isPlus)
          return std::get<double>(leftValue) + std::get<Matrix>(rightValue);
        else
          return std::get<double>(leftValue) - std::get<Matrix>(rightValue);
      case OperatorSignatures::MATRIX_MATRIX:
        if (isPlus)
          return std::get<Matrix>(leftValue) + std::get<Matrix>(rightValue);
        else
          return std::get<Matrix>(leftValue) - std::get<Matrix>(rightValue);
      default:
        throw SemanticError(
            "Wrong types in AdditiveOperator at " +
            node->getToken().getLinePositionString() +
            ". Correct types are matrix:matrix, matrix:double, double:matrix, "
            "matrix:integer, integer:matrix, "
            "string:string, integer:integer, integer:double, double:integer, "
            "double:double.");
    }
  } else {
    if (!isPlus) {
      switch (leftValue.index()) {
        case 0:
          return -(std::get<int64_t>(leftValue));
        case 1:
          return -(std::get<double>(leftValue));
        case 2:
          throw SemanticError("Invalid unary operator in AdditiveOperator at " +
                              node->getToken().getLinePositionString() +
                              ". Operator-(const std::string&) is undefinded.");
        case 3:
          return -(std::get<Matrix>(leftValue));
      }
    }
  }
  return leftValue;
}

Value Evaluator::evaluate(const MultiplicativeOperatorNode* node) const {
  throw SemanticError("Not implemented! At: " +
                      node->getToken().getLinePositionString());
}
Value Evaluator::evaluate(const ExponentiationOperatorNode* node) const {
  throw SemanticError("Not implemented! At: " +
                      node->getToken().getLinePositionString());
}
Value Evaluator::evaluate(const MatrixOperatorNode* node) const {
  throw SemanticError("Not implemented! At: " +
                      node->getToken().getLinePositionString());
}

Value Evaluator::evaluate(const LogicalOperatorNode* node) const {
  throw SemanticError("Not implemented! At: " +
                      node->getToken().getLinePositionString());
}
Value Evaluator::evaluate(const AssignmentNode* node) const {
  throw SemanticError("Not implemented! At: " +
                      node->getToken().getLinePositionString());
}
Value Evaluator::evaluate(const VariableNode* node) const {
  throw SemanticError("Not implemented! At: " +
                      node->getToken().getLinePositionString());
}
Value Evaluator::evaluate(const MatrixVariable* node) const {
  throw SemanticError("Not implemented! At: " +
                      node->getToken().getLinePositionString());
}
Value Evaluator::evaluate(const ArgumentNode* node) const {
  throw SemanticError("Not implemented! At: " +
                      node->getToken().getLinePositionString());
}
Value Evaluator::evaluate(const IfStatementNode* node) const {
  throw SemanticError("Not implemented! At: " +
                      node->getToken().getLinePositionString());
}
Value Evaluator::evaluate(const OtherwiseStatementNode* node) const {
  throw SemanticError("Not implemented! At: " +
                      node->getToken().getLinePositionString());
}
Value Evaluator::evaluate(const LoopStatementNode* node) const {
  throw SemanticError("Not implemented! At: " +
                      node->getToken().getLinePositionString());
}
Value Evaluator::evaluate(const AslasStatementNode* node) const {
  throw SemanticError("Not implemented! At: " +
                      node->getToken().getLinePositionString());
}
Value Evaluator::evaluate(const FunctionStatementNode* node) const {
  throw SemanticError("Not implemented! At: " +
                      node->getToken().getLinePositionString());
}
Value Evaluator::evaluate(const FunctionCallNode* node) const {
  throw SemanticError("Not implemented! At: " +
                      node->getToken().getLinePositionString());
}
Value Evaluator::evaluate(const ConditionStatementNode* node) const {
  throw SemanticError("Not implemented! At: " +
                      node->getToken().getLinePositionString());
}
Value Evaluator::evaluate(const CaseStatementNode* node) const {
  throw SemanticError("Not implemented! At: " +
                      node->getToken().getLinePositionString());
}
Value Evaluator::evaluate(const DefaultStatementNode* node) const {
  throw SemanticError("Not implemented! At: " +
                      node->getToken().getLinePositionString());
}

Value Evaluator::evaluate(const ExpressionNode* node) const {
  throw SemanticError("Not implemented! At: " +
                      node->getToken().getLinePositionString());
}