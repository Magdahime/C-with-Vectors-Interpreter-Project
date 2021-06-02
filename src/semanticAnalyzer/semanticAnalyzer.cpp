#include "semanticAnalyzer/semanticAnalyzer.hpp"

void SemanticAnalyzer::enterBlock() {
  currentDepth++;
  scopeStack.emplace(currentDepth, std::unordered_set<std::string>());
}
void SemanticAnalyzer::closeBlock() {
  std::unordered_set<std::string> identifiers = scopeStack.top().second;
  scopeStack.pop();
  for (const auto& identifier : identifiers) {
    variablesMap.erase(std::make_pair(identifier, currentDepth));
  }
  currentDepth--;
}
void SemanticAnalyzer::enterVariable(std::string identifier,
                                     VariableNode* node) {
  variablesMap.emplace(
      std::make_pair(std::make_pair(identifier, currentDepth), node));
    scopeStack.top().second.insert(identifier);
}
void SemanticAnalyzer::enterFunction(std::string identifier,
                                     FunctionStatementNode* node) {
  functionsMap.emplace(std::make_pair(identifier, node));
}
std::optional<const VariableNode*> SemanticAnalyzer::searchVariable(
    std::string identifier) const {
  auto iter = variablesMap.find(std::make_pair(identifier, currentDepth));
  if (iter != variablesMap.end()) return iter->second;
  return {};
}

std::optional<const FunctionStatementNode*> SemanticAnalyzer::searchFunction(
    std::string identifier) const {
  auto iter = functionsMap.find(identifier);
  if (iter != functionsMap.end()) return iter->second;
  return {};
}

void SemanticAnalyzer::visit(AdditiveOperatorNode* node) {}
void SemanticAnalyzer::visit(MultiplicativeOperatorNode* node) {}
void SemanticAnalyzer::visit(LogicalOperatorNode* node) {}
void SemanticAnalyzer::visit(MatrixOperatorNode* node) {}
void SemanticAnalyzer::visit(AssignmentNode* node) {}
void SemanticAnalyzer::visit(ExponentiationOperatorNode* node) {}
void SemanticAnalyzer::visit(VariableNode* node) {}
void SemanticAnalyzer::visit(MatrixVariable* node) {}
void SemanticAnalyzer::visit(ArgumentNode* node) {}
void SemanticAnalyzer::visit(IfStatementNode* node) {}
void SemanticAnalyzer::visit(OtherwiseStatementNode* node) {}
void SemanticAnalyzer::visit(LoopStatementNode* node) {}
void SemanticAnalyzer::visit(AslasStatementNode* node) {}
void SemanticAnalyzer::visit(FunctionStatementNode* node) {}
void SemanticAnalyzer::visit(FunctionCallNode* node) {}
void SemanticAnalyzer::visit(ConditionStatementNode* node) {}
void SemanticAnalyzer::visit(CaseStatementNode* node) {}
void SemanticAnalyzer::visit(DefaultStatementNode* node) {}