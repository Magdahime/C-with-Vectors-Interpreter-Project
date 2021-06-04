#pragma once
#include "interpreter/evaluator.hpp"
#include "interpreter/interpreterHelpers.hpp"
#include "interpreter/semanticAnalyzer.hpp"

class Interpreter {
 public:
  Interpreter();
  void enterBlock();
  void closeBlock();
  void enterVariable(std::string identifier, TokenVariant value);
  void enterVariable(std::string identifier, std::string value);
  void enterVariable(std::string identifier, double value);
  void enterVariable(std::string identifier, Matrix value);
  void enterVariable(std::string identifier, int64_t value);
  void visit(AdditiveOperatorNode* node);
  void visit(MultiplicativeOperatorNode* node);
  void visit(LogicalOperatorNode* node);
  void visit(MatrixOperatorNode* node);
  void visit(AssignmentNode* node);
  void visit(ExponentiationOperatorNode* node);
  void visit(VariableNode* node);
  void visit(MatrixVariable* node);
  void visit(ArgumentNode* node);
  void visit(IfStatementNode* node);
  void visit(OtherwiseStatementNode* node);
  void visit(LoopStatementNode* node);
  void visit(AslasStatementNode* node);
  void visit(FunctionStatementNode* node);
  void visit(FunctionCallNode* node);
  void visit(ConditionStatementNode* node);
  void visit(CaseStatementNode* node);
  void visit(DefaultStatementNode* node);
  void visit(ValueNode* node);
  void visit(ExpressionValueNode* node);
  const Evaluator& getEvaluator() { return evaluator; }
  const VariableMap& getVariableMap() { return variablesMap; }

 private:
  uint64_t currentDepth = 0;

  VariableMap variablesMap;
  FunctionMap functionsMap;
  ScopeStack scopeStack;

  SemanticAnalyzer semAnalyzer;
  Evaluator evaluator;
  void enterFunction(std::string identifier, FunctionStatementNode* node);
};