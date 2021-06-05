#include "interpreter/semanticAnalyzer.hpp"

#include "parser/statementNode.hpp"

// int64_t SemanticAnalyzer::checkStart() const {
//   int64_t startInt;
//   if (std::holds_alternative<int64_t>(start)) {
//     startInt = std::get<int64_t>(start);
//   } else {
//     startString = std::get<std::string>(start);
//     VariableNode* variableNode = searchVariable(startString);
//     if (!variableNode)
//       throw SemanticError("Unknown start variable at: " +
//                           node->getToken().getLinePositionString());
//     if (variableNode->getToken().getType() !=
//         Token::TokenType::IntegerLiteralToken) {
//       throw SemanticError("Wrong start variable type: should be integer! At:
//       " +
//                           node->getToken().getLinePositionString());
//     }
//     // startInt = variableNode->getToken().getInt();
//   }
//   if (startInt < 0)
//     throw SemanticError("Incorrect start value at: " +
//                         node->getToken().getLinePositionString());
//   return startInt;
// }

// int64_t checkStop(const std::variant<std::string, int64_t> stop,
//                   const int64_t startInt) const {
//   int64_t stopInt;
//   if (std::holds_alternative<int64_t>(stop)) {
//     stopInt = std::get<int64_t>(stop);
//   } else {
//     stopString = std::get<std::string>(stop);
//     VariableNode* variableNode = searchVariable(stopString);
//     if (!variableNode)
//       throw SemanticError("Unknown stop variable at: " +
//                           node->getToken().getLinePositionString());
//     if (variableNode->getToken().getType() !=
//         Token::TokenType::IntegerLiteralToken) {
//       throw SemanticError("Wrong stop variable type: should be integer! At: "
//       +
//                           node->getToken().getLinePositionString());
//     }
//     // stopInt = variableNode->getToken().getInt();
//   }
//   if (stopInt < startInt)
//     throw SemanticError(
//         "Stop should be bigger than start in LoopStatement! At: " +
//         node->getToken().getLinePositionString());
// }

void SemanticAnalyzer::check(const AdditiveOperatorNode* node) const {
  throw SemanticError("Not implemented! At: " +
                      node->getToken().getLinePositionString());
}
void SemanticAnalyzer::check(const MultiplicativeOperatorNode* node) const {
  throw SemanticError("Not implemented! At: " +
                      node->getToken().getLinePositionString());
}
void SemanticAnalyzer::check(const LogicalOperatorNode* node) const {
  throw SemanticError("Not implemented! At: " +
                      node->getToken().getLinePositionString());
}
void SemanticAnalyzer::check(const MatrixOperatorNode* node) const {
  throw SemanticError("Not implemented! At: " +
                      node->getToken().getLinePositionString());
}
void SemanticAnalyzer::check(const AssignmentNode* node) const {
  throw SemanticError("Not implemented! At: " +
                      node->getToken().getLinePositionString());
}
void SemanticAnalyzer::check(const ExponentiationOperatorNode* node) const {
  throw SemanticError("Not implemented! At: " +
                      node->getToken().getLinePositionString());
}
void SemanticAnalyzer::check(const VariableNode* node) const {
  throw SemanticError("Not implemented! At: " +
                      node->getToken().getLinePositionString());
}
void SemanticAnalyzer::check(const MatrixVariable* node) const {
  throw SemanticError("Not implemented! At: " +
                      node->getToken().getLinePositionString());
}
void SemanticAnalyzer::check(const ArgumentNode* node) const {
  throw SemanticError("Not implemented! At: " +
                      node->getToken().getLinePositionString());
}
void SemanticAnalyzer::check(const IfStatementNode* node) const {
  throw SemanticError("Not implemented! At: " +
                      node->getToken().getLinePositionString());
}
void SemanticAnalyzer::check(const OtherwiseStatementNode* node) const {
  throw SemanticError("Not implemented! At: " +
                      node->getToken().getLinePositionString());
}
void SemanticAnalyzer::check(const LoopStatementNode* node) const {
  throw SemanticError("Not implemented! At: " +
                      node->getToken().getLinePositionString());
}
void SemanticAnalyzer::check(const AslasStatementNode* node) const {
  throw SemanticError("Not implemented! At: " +
                      node->getToken().getLinePositionString());
}
void SemanticAnalyzer::check(const FunctionStatementNode* node) const {
  throw SemanticError("Not implemented! At: " +
                      node->getToken().getLinePositionString());
}
void SemanticAnalyzer::check(const FunctionCallNode* node) const {
  throw SemanticError("Not implemented! At: " +
                      node->getToken().getLinePositionString());
}
void SemanticAnalyzer::check(const ConditionStatementNode* node) const {
  throw SemanticError("Not implemented! At: " +
                      node->getToken().getLinePositionString());
}
void SemanticAnalyzer::check(const CaseStatementNode* node) const {
  throw SemanticError("Not implemented! At: " +
                      node->getToken().getLinePositionString());
}
void SemanticAnalyzer::check(const DefaultStatementNode* node) const {
  throw SemanticError("Not implemented! At: " +
                      node->getToken().getLinePositionString());
}
void SemanticAnalyzer::check(const ValueNode* node) const {
  throw SemanticError("Not implemented! At: " +
                      node->getToken().getLinePositionString());
}

void SemanticAnalyzer::check(const ExpressionValueNode* node) const {
  throw SemanticError("Not implemented! At: " +
                      node->getToken().getLinePositionString());
}