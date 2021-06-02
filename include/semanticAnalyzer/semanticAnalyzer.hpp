#pragma once
#include <cstdint>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "lexicalAnalyzer/token.hpp"

class StatementNode;
class ChildrenStatementNode;
class FunctionStatementNode;
class AslasStatementNode;
class IfStatementNode;
class OtherwiseStatementNode;
class FunctionCallNode;
class ConditionStatementNode;
class CaseStatementNode;
class DefaultStatementNode;
class ExpressionNode;
class ExponentiationOperatorNode;
class MatrixValueNode;
class ExpressionValueNode;
class ArgumentNode;
class LoopStatementNode;
class VariableNode;
class MatrixVariable;
class AssignmentNode;
class MatrixOperatorNode;
class LogicalOperatorNode;
class MultiplicativeOperatorNode;
class AdditiveOperatorNode;

class SemanticAnalyzer {
 public:
  enum class Type { Integer, String, Double, Matrix };

  void enterBlock();
  void closeBlock();
  void enterVariable(std::string identifier, VariableNode* node);
  void enterFunction(std::string identifier, FunctionStatementNode* node);
  std::optional<const VariableNode*> searchVariable(
      std::string identifier) const;
  std::optional<const FunctionStatementNode*> searchFunction(
      std::string identifier) const;

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

 private:
  uint64_t currentDepth = 0;
  std::unordered_map<std::pair<std::string, uint64_t>, const VariableNode*>
      variablesMap;
  std::unordered_map<std::string, const FunctionStatementNode*> functionsMap;
  std::stack<std::pair<uint64_t, std::unordered_set<std::string>>> scopeStack;

};  // namespace SemanticAnalyzer