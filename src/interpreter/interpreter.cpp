#include "interpreter/interpreter.hpp"

#include "parser/statementNode.hpp"

Interpreter::Interpreter()
    : semAnalyzer(variablesMap, functionsMap, currentDepth),
      evaluator(variablesMap, currentDepth) {
  scopeStack.emplace(currentDepth, std::set<std::string>());
}

void Interpreter::enterBlock() {
  currentDepth++;
  scopeStack.emplace(currentDepth, std::set<std::string>());
}
void Interpreter::closeBlock() {
  std::set<std::string> identifiers = scopeStack.top().second;
  scopeStack.pop();
  for (const auto& identifier : identifiers) {
    variablesMap.erase(std::make_pair(identifier, currentDepth));
  }
  currentDepth--;
}

void Interpreter::enterVariable(std::string identifier, std::string value) {
  VariableInfo info = {Type::String, value};
  variablesMap.emplace(
      std::make_pair(std::make_pair(identifier, currentDepth), info));
  scopeStack.top().second.insert(identifier);
}
void Interpreter::enterVariable(std::string identifier, double value) {
  VariableInfo info = {Type::Double, value};
  variablesMap.emplace(
      std::make_pair(std::make_pair(identifier, currentDepth), info));
  scopeStack.top().second.insert(identifier);
}
void Interpreter::enterVariable(std::string identifier, Matrix value) {
  VariableInfo info = {Type::Matrix, value};
  variablesMap.emplace(
      std::make_pair(std::make_pair(identifier, currentDepth), info));
  scopeStack.top().second.insert(identifier);
}
void Interpreter::enterVariable(std::string identifier, int64_t value) {
  VariableInfo info = {Type::Integer, value};
  variablesMap.emplace(
      std::make_pair(std::make_pair(identifier, currentDepth), info));
  scopeStack.top().second.insert(identifier);
}

void Interpreter::enterFunction(std::string identifier,
                                FunctionStatementNode* node) {
  functionsMap.emplace(std::make_pair(identifier, node));
}

void Interpreter::enterVariable(std::string identifier, TokenVariant value) {
  if (std::holds_alternative<int64_t>(value))
    enterVariable(identifier, std::get<int64_t>(value));
  else if (std::holds_alternative<Matrix>(value))
    enterVariable(identifier, std::get<Matrix>(value));
  else if (std::holds_alternative<double>(value))
    enterVariable(identifier, std::get<double>(value));
  else if (std::holds_alternative<std::string>(value))
    enterVariable(identifier, std::get<std::string>(value));
}
void Interpreter::visit(AdditiveOperatorNode* node) { semAnalyzer.check(node); }
void Interpreter::visit(MultiplicativeOperatorNode* node) {
  semAnalyzer.check(node);
}
void Interpreter::visit(LogicalOperatorNode* node) { semAnalyzer.check(node); }
void Interpreter::visit(MatrixOperatorNode* node) { semAnalyzer.check(node); }
void Interpreter::visit(AssignmentNode* node) { semAnalyzer.check(node); }
void Interpreter::visit(ExponentiationOperatorNode* node) {
  semAnalyzer.check(node);
}
void Interpreter::visit(VariableNode* node) { semAnalyzer.check(node); }
void Interpreter::visit(MatrixVariable* node) { semAnalyzer.check(node); }
void Interpreter::visit(ArgumentNode* node) { semAnalyzer.check(node); }
void Interpreter::visit(IfStatementNode* node) { semAnalyzer.check(node); }
void Interpreter::visit(OtherwiseStatementNode* node) {
  semAnalyzer.check(node);
}
void Interpreter::visit(LoopStatementNode* node) { semAnalyzer.check(node); }
void Interpreter::visit(AslasStatementNode* node) { semAnalyzer.check(node); }
void Interpreter::visit(FunctionStatementNode* node) {
  semAnalyzer.check(node);
}
void Interpreter::visit(FunctionCallNode* node) { semAnalyzer.check(node); }
void Interpreter::visit(ConditionStatementNode* node) {
  semAnalyzer.check(node);
}
void Interpreter::visit(CaseStatementNode* node) { semAnalyzer.check(node); }
void Interpreter::visit(DefaultStatementNode* node) { semAnalyzer.check(node); }
void Interpreter::visit(ValueNode* node) { semAnalyzer.check(node); }
void Interpreter::visit(ExpressionValueNode* node){semAnalyzer.check(node);}