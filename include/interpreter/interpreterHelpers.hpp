#pragma once
#include <map>
#include <set>
#include <stack>
#include <string>

#include "matrix.hpp"

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
class ValueNode;
class MatrixOperatorNode;
class LogicalOperatorNode;
class MultiplicativeOperatorNode;
class AdditiveOperatorNode;
class IdentifierNode;

using Value = std::variant<int64_t, double, std::string, Matrix>;

enum class Type { Integer, Double, String, Matrix };

enum class OperatorSignatures {
  INTEGER_INTEGER,
  INTEGER_DOUBLE,
  INTEGER_STRING,
  INTEGER_MATRIX,
  DOUBLE_INTEGER,
  DOUBLE_DOUBLE,
  DOUBLE_STRING,
  DOUBLE_MATRIX,
  STRING_INTEGER,
  STRING_DOUBLE,
  STRING_STRING,
  STRING_MATRIX,
  MATRIX_INTEGER,
  MATRIX_DOUBLE,
  MATRIX_STRING,
  MATRIX_MATRIX,
};

struct VariableInfo {
  Type type;
  Value value;
};

using VariableMap = std::map<std::pair<std::string, uint64_t>, VariableInfo>;
using FunctionMap = std::map<std::string, const FunctionStatementNode*>;
using ScopeStack = std::stack<std::pair<uint64_t, std::set<std::string>>>;

std::optional<VariableInfo> searchVariable(const VariableMap& variableMap,
                                           std::string identifier,
                                           int currentDepth);

std::optional<const FunctionStatementNode*> searchFunction(
    const FunctionMap& functionMap, std::string identifier);
