#pragma once
#include <cstdint>
#include <map>
#include <set>
#include <stack>
#include <vector>

#include "interpreter/interpreterHelpers.hpp"
#include "lexicalAnalyzer/token.hpp"

class SemanticAnalyzer {
 public:
  SemanticAnalyzer(VariableMap& varMap, FunctionMap& funMap, const int& currDepth)
      : variableMap(varMap), functionMap(funMap), currentDepth(currDepth) {}

  void check(const AdditiveOperatorNode* node) const;
  void check(const MultiplicativeOperatorNode* node) const;
  void check(const LogicalOperatorNode* node) const;
  void check(const MatrixOperatorNode* node) const;
  void check(const AssignmentNode* node) const;
  void check(const ExponentiationOperatorNode* node) const;
  void check(const VariableNode* node) const;
  void check(const MatrixVariable* node) const;
  void check(const ArgumentNode* node) const;
  void check(const IfStatementNode* node) const;
  void check(const OtherwiseStatementNode* node) const;
  void check(const LoopStatementNode* node) const;
  void check(const AslasStatementNode* node) const;
  void check(const FunctionStatementNode* node) const;
  void check(const FunctionCallNode* node) const;
  void check(const ConditionStatementNode* node) const;
  void check(const CaseStatementNode* node) const;
  void check(const DefaultStatementNode* node) const;
  void check(const ValueNode* node) const;
  void check(const ExpressionValueNode* node) const;
 private:

  VariableMap& variableMap;
  FunctionMap& functionMap;
  const int& currentDepth;
};