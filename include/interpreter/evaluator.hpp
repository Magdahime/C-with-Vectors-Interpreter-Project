#pragma once
#include <cmath>

#include "interpreter/interpreterHelpers.hpp"
#include "lexicalAnalyzer/token.hpp"

class Evaluator {
 public:
  Evaluator() { scopeStack.emplace(currentDepth, std::set<std::string>()); }
  void enterBlock();
  void closeBlock();
  void enterVariable(std::string identifier, TokenVariant value);
  void enterVariable(std::string identifier, std::string value);
  void enterVariable(std::string identifier, double value);
  void enterVariable(std::string identifier, Matrix value);
  void enterVariable(std::string identifier, int64_t value);

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

  bool checkZeroDivision(const Value value) const;
  std::optional<VariableInfo> searchVariable(std::string identifier,
                                             int currentDepth) const;

  std::optional<const FunctionStatementNode*> searchFunction(
      std::string identifier) const;

  const VariableMap& getVariableMap() { return variableMap; }

 private:
  uint64_t currentDepth = 0;

  VariableMap variableMap;
  FunctionMap functionsMap;
  ScopeStack scopeStack;

  void enterFunction(std::string identifier, FunctionStatementNode* node);
};
