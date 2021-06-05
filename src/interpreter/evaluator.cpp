#include "interpreter/evaluator.hpp"

#include "parser/statementNode.hpp"

Value Evaluator::evaluate(const ValueNode* node) const {
  return node->getValue();
}

Value Evaluator::evaluate(const IdentifierNode* node) const {
  std::string identifier = std::get<std::string>(node->getValue());
  std::optional<VariableInfo> var =
      searchVariable(variableMap, identifier, *currentDepth);
  if (var) return var->value;
  throw SemanticError("Unknown variable: " + identifier +
                      " at:" + node->getToken().getLinePositionString());
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
    Matrix returnMatrix;
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
          returnMatrix =
              std::get<Matrix>(leftValue) + std::get<Matrix>(rightValue);
        else
          returnMatrix =
              std::get<Matrix>(leftValue) - std::get<Matrix>(rightValue);
        if (!returnMatrix.empty()) return returnMatrix;
        throw SemanticError("Invalid types in AdditiveOperator at " +
                            node->getToken().getLinePositionString() +
                            ". Matrix's dimensions are not compatible.");
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
  auto left = node->getLeft();
  auto right = node->getRight();
  bool isMulti =
      (node->getType() == MultiplicativeOperatorNode::NodeType::Multiplication);
  Value leftValue = left->accept(*this);
  Value rightValue = right->accept(*this);
  if (!isMulti && checkZeroDivision(rightValue))
    throw SemanticError("Division per zero!!! At: " +
                        node->getToken().getLinePositionString());
  int signature =
      std::variant_size_v<Value> * leftValue.index() + rightValue.index();
  Matrix returnMatrix;
  switch (static_cast<OperatorSignatures>(signature)) {
    case OperatorSignatures::INTEGER_INTEGER:
      if (isMulti)
        return std::get<int64_t>(leftValue) * std::get<int64_t>(rightValue);
      else
        return std::get<int64_t>(leftValue) / std::get<int64_t>(rightValue);
    case OperatorSignatures::INTEGER_DOUBLE:
      if (isMulti)
        return std::get<int64_t>(leftValue) * std::get<double>(rightValue);
      else
        return std::get<int64_t>(leftValue) / std::get<double>(rightValue);
    case OperatorSignatures::DOUBLE_INTEGER:
      if (isMulti)
        return std::get<double>(leftValue) * std::get<int64_t>(rightValue);
      else
        return std::get<double>(leftValue) / std::get<int64_t>(rightValue);
    case OperatorSignatures::DOUBLE_DOUBLE:
      if (isMulti)
        return std::get<double>(leftValue) * std::get<double>(rightValue);
      else
        return std::get<double>(leftValue) / std::get<double>(rightValue);
    case OperatorSignatures::MATRIX_INTEGER:
      if (isMulti)
        return std::get<Matrix>(leftValue) * std::get<int64_t>(rightValue);
      else
        return std::get<Matrix>(leftValue) / std::get<int64_t>(rightValue);
    case OperatorSignatures::INTEGER_MATRIX:
      if (isMulti)
        return std::get<int64_t>(leftValue) * std::get<Matrix>(rightValue);
      else
        throw SemanticError("Invalid operation in MultiplicativeOperator at " +
                            node->getToken().getLinePositionString() +
                            ". Integer Matrix division is illegal.");
    case OperatorSignatures::MATRIX_DOUBLE:
      if (isMulti)
        return std::get<Matrix>(leftValue) * std::get<double>(rightValue);
      else
        return std::get<Matrix>(leftValue) / std::get<double>(rightValue);
    case OperatorSignatures::DOUBLE_MATRIX:
      if (isMulti)
        return std::get<double>(leftValue) * std::get<Matrix>(rightValue);
      else
        throw SemanticError("Invalid operation in MultiplicativeOperator at " +
                            node->getToken().getLinePositionString() +
                            ". Double Matrix division is illegal.");
    case OperatorSignatures::MATRIX_MATRIX:
      if (isMulti)
        return std::get<Matrix>(leftValue) * std::get<Matrix>(rightValue);
      else
        return std::get<Matrix>(leftValue) / std::get<Matrix>(rightValue);
    default:
      throw SemanticError("Wrong types in MultiplicativeOperator at " +
                          node->getToken().getLinePositionString() +
                          ". Correct types are matrix:matrix, " +
                          "matrix:double, " + "matrix:integer," +
                          "integer:integer, integer:double, double:integer, " +
                          "double:double.");
  }
}
Value Evaluator::evaluate(const ExponentiationOperatorNode* node) const {
  throw SemanticError("ExponentiationOperatorNode Not implemented! At: " +
                      node->getToken().getLinePositionString());
}
Value Evaluator::evaluate(const MatrixOperatorNode* node) const {
  throw SemanticError("MatrixOperatorNode Not implemented! At: " +
                      node->getToken().getLinePositionString());
}

Value Evaluator::evaluate(const LogicalOperatorNode* node) const {
  throw SemanticError("LogicalOperatorNode Not implemented! At: " +
                      node->getToken().getLinePositionString());
}
Value Evaluator::evaluate(const AssignmentNode* node) const {
  throw SemanticError("AssignmentNode Not implemented! At: " +
                      node->getToken().getLinePositionString());
}
Value Evaluator::evaluate(const VariableNode* node) const {
  throw SemanticError("VariableNode Not implemented! At: " +
                      node->getToken().getLinePositionString());
}
Value Evaluator::evaluate(const MatrixVariable* node) const {
  throw SemanticError("MatrixVariable Not implemented! At: " +
                      node->getToken().getLinePositionString());
}
Value Evaluator::evaluate(const ArgumentNode* node) const {
  throw SemanticError("ArgumentNode Not implemented! At: " +
                      node->getToken().getLinePositionString());
}
Value Evaluator::evaluate(const IfStatementNode* node) const {
  throw SemanticError("IfStatementNode Not implemented! At: " +
                      node->getToken().getLinePositionString());
}
Value Evaluator::evaluate(const OtherwiseStatementNode* node) const {
  throw SemanticError("OtherwiseStatementNode Not implemented! At: " +
                      node->getToken().getLinePositionString());
}
Value Evaluator::evaluate(const LoopStatementNode* node) const {
  throw SemanticError("LoopStatementNode Not implemented! At: " +
                      node->getToken().getLinePositionString());
}
Value Evaluator::evaluate(const AslasStatementNode* node) const {
  throw SemanticError("AslasStatementNode Not implemented! At: " +
                      node->getToken().getLinePositionString());
}
Value Evaluator::evaluate(const FunctionStatementNode* node) const {
  throw SemanticError("FunctionStatementNode Not implemented! At: " +
                      node->getToken().getLinePositionString());
}
Value Evaluator::evaluate(const FunctionCallNode* node) const {
  throw SemanticError("FunctionCallNode Not implemented! At: " +
                      node->getToken().getLinePositionString());
}
Value Evaluator::evaluate(const ConditionStatementNode* node) const {
  throw SemanticError("ConditionStatementNode Not implemented! At: " +
                      node->getToken().getLinePositionString());
}
Value Evaluator::evaluate(const CaseStatementNode* node) const {
  throw SemanticError("CaseStatementNode Not implemented! At: " +
                      node->getToken().getLinePositionString());
}
Value Evaluator::evaluate(const DefaultStatementNode* node) const {
  throw SemanticError("DefaultStatementNode Not implemented! At: " +
                      node->getToken().getLinePositionString());
}

Value Evaluator::evaluate(const ExpressionNode* node) const {
  throw SemanticError("ExpressionNode Not implemented! At: " +
                      node->getToken().getLinePositionString());
}

bool Evaluator::checkZeroDivision(const Value value) const {
  switch (value.index()) {
    case 0:
      if (std::get<int64_t>(value) == 0) return true;
      return false;
    case 1:
      if (std::get<double>(value) == 0) return true;
      return false;
    default:
      return false;
  }
}