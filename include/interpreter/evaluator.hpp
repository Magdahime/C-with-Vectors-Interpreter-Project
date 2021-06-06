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

  Value evaluate(const AdditiveOperatorNode* node);
  Value evaluate(const MultiplicativeOperatorNode* node);
  Value evaluate(const ExponentiationOperatorNode* node);
  Value evaluate(const MatrixOperatorNode* node);
  Value evaluate(const ValueNode* node);
  Value evaluate(const IdentifierNode* node);
  Value evaluate(const ExpressionNode* node);
  Value evaluate(const LogicalOperatorNode* node);
  Value evaluate(const AssignmentNode* node);
  Value evaluate(const AssignNewValueNode* node);
  Value evaluate(const VariableNode* node);
  Value evaluate(const MatrixVariable* node);
  Value evaluate(const MatrixValueNode* node);
  Value evaluate(const MatrixSizeNode* node);
  Value evaluate(const ArgumentNode* node);
  Value evaluate(const IfStatementNode* node);
  Value evaluate(const OtherwiseStatementNode* node);
  Value evaluate(const LoopStatementNode* node);
  Value evaluate(const AslasStatementNode* node);
  Value evaluate(const FunctionStatementNode* node);
  Value evaluate(const FunctionCallNode* node);
  Value evaluate(const ConditionStatementNode* node);
  Value evaluate(const CaseStatementNode* node);
  Value evaluate(const DefaultStatementNode* node);

  LoopComp checkLoopComponents(const LoopStatementNode* node);
  Value checkAslasExpression(const ExpressionNode* node);
  bool checkCaseExpression(const ExpressionNode* node);
  bool checkZeroDivision(const Value value) const;
  std::optional<VariableInfo> searchVariable(std::string identifier,
                                             int currentDepth) const;
  void updateVariable(std::string identifier, int currentDepth, Value newValue);
  Matrix combineSizeValues(const AssignmentNode* node, const Matrix& size,
                           const Matrix& values) const;

  std::optional<std::pair<const FunctionStatementNode*, FunctionInfo>>
  searchFunction(std::string identifier) const;

  const VariableMap& getVariableMap() { return variableMap; }

 private:
  uint64_t currentDepth = 0;

  VariableMap variableMap;
  FunctionMap functionsMap;
  ScopeStack scopeStack;

  void enterFunction(std::string identifier, const FunctionStatementNode* node,
                     std::vector<ArgumentInfo> args);
};
