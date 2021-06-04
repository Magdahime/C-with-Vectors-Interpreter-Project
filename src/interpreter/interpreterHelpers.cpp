#include "interpreter/interpreterHelpers.hpp"

std::optional<VariableInfo> searchVariable(const VariableMap& variableMap,
                                           std::string identifier,
                                           int currentDepth) {
  auto iter = variableMap.find(std::make_pair(identifier, currentDepth));
  if (iter != variableMap.end())return iter->second;
  return {};
}

std::optional<const FunctionStatementNode*> searchFunction(
    const FunctionMap& functionMap, std::string identifier) {
  auto iter = functionMap.find(identifier);
  if (iter != functionMap.end()) return iter->second;
  return {};
}