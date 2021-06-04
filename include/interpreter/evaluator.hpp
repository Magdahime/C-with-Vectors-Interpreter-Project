#include "interpreter/interpreterHelpers.hpp"

class Evaluator {
 public:
  Evaluator(VariableMap& varMap, const int& currDepth) : variableMap(varMap), currentDepth(currDepth) {}
  Value evaluate(AdditiveOperatorNode* node);
  Value evaluate(MultiplicativeOperatorNode* node);
  Value evaluate(ExponentiationOperatorNode* node);
  Value evaluate(MatrixOperatorNode* node);
  Value evaluate(ValueNode* node);
  Value evaluate(IdentifierNode* node);

  void evaluate(LogicalOperatorNode* node);
  void evaluate(AssignmentNode* node);
  void evaluate(VariableNode* node);
  void evaluate(MatrixVariable* node);
  void evaluate(ArgumentNode* node);
  void evaluate(IfStatementNode* node);
  void evaluate(OtherwiseStatementNode* node);
  void evaluate(LoopStatementNode* node);
  void evaluate(AslasStatementNode* node);
  void evaluate(FunctionStatementNode* node);
  void evaluate(FunctionCallNode* node);
  void evaluate(ConditionStatementNode* node);
  void evaluate(CaseStatementNode* node);
  void evaluate(DefaultStatementNode* node);

 private:
  std::optional<VariableInfo> searchVariable(std::string identifier) const;
  VariableMap& variableMap;
  const int& currentDepth;
};
