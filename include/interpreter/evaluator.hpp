#pragma once
#include "interpreter/interpreterHelpers.hpp"

class Evaluator {
 public:
  Evaluator(const VariableMap& varMap, const int& currDepth)
      : variableMap(varMap), currentDepth(currDepth) {}
  Value evaluate(const AdditiveOperatorNode* node) const;

  Value evaluate(const MultiplicativeOperatorNode* node) const;
  Value evaluate(const ExponentiationOperatorNode* node) const;
  Value evaluate(const MatrixOperatorNode* node) const;
  Value evaluate(const ValueNode* node) const;
  Value evaluate(const IdentifierNode* node) const;
  Value evaluate(const ExpressionNode* node) const;
  Value evaluate(const LogicalOperatorNode* node) const;
  Value evaluate(const AssignmentNode* node) const;
  Value evaluate(const VariableNode* node) const;
  Value evaluate(const MatrixVariable* node) const;
  Value evaluate(const ArgumentNode* node) const;
  Value evaluate(const IfStatementNode* node) const;
  Value evaluate(const OtherwiseStatementNode* node) const;
  Value evaluate(const LoopStatementNode* node) const;
  Value evaluate(const AslasStatementNode* node) const;
  Value evaluate(const FunctionStatementNode* node) const;
  Value evaluate(const FunctionCallNode* node) const;
  Value evaluate(const ConditionStatementNode* node) const;
  Value evaluate(const CaseStatementNode* node) const;
  Value evaluate(const DefaultStatementNode* node) const;

 private:
  const VariableMap& variableMap;
  const int& currentDepth;
};
