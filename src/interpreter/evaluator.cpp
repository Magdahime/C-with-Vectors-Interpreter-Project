#include "interpreter/evaluator.hpp"

#include "parser/statementNode.hpp"

Value Evaluator::evaluate(ValueNode* node) { return node->getValue(); };

Value Evaluator::evaluate(IdentifierNode* node) {
  std::string identifier = node->getValue();
  return searchVariable(identifier, currentDepth)->value;
}

Value evaluate(AdditiveOperatorNode* node) {
  auto left = node->getLeft();
  auto right = node->getRight();
  if (right) {
    Value leftValue = evaluate(left);
    Value rightValue = evaluate(right);
    int signature =
        std::variant_size_v<Value>() * leftValue.index() + rightValue.index();
    switch(signature){
        case OperatorSignatures::INTEGER_INTEGER:
        case OperatorSignatures::INTEGER_DOUBLE:
        case OperatorSignatures::DOUBLE_INTEGER:
        case OperatorSignatures::DOUBLE_DOUBLE:
        case OperatorSignatures::STRING_STRING:
        case OperatorSignatures::MATRIX_INTEGER:
        case OperatorSignatures::INTEGER_MATRIX:
        case OperatorSignatures::MATRIX_DOUBLE:
        case OperatorSignatures::DOUBLE_MATRIX:
        case OperatorSignatures::MATRIX_STRING:
        default:
        
    }
  } else {
  }
}