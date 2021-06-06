#pragma once
#include "parser/expressionNode.hpp"
#include "parser/node.hpp"

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

using StatementNodeUptr = std::unique_ptr<StatementNode>;
using ChildrenStatementNodeUptr = std::unique_ptr<ChildrenStatementNode>;
using FunctionStatementNodeUptr = std::unique_ptr<FunctionStatementNode>;
using AslasStatementNodeUptr = std::unique_ptr<AslasStatementNode>;
using IfStatementNodeUptr = std::unique_ptr<IfStatementNode>;
using OtherwiseStatementNodeUptr = std::unique_ptr<OtherwiseStatementNode>;
using FunctionCallNodeUptr = std::unique_ptr<FunctionCallNode>;
using ConditionStatementNodeUptr = std::unique_ptr<ConditionStatementNode>;
using CaseStatementNodeUptr = std::unique_ptr<CaseStatementNode>;
using DefaultStatementNodeUptr = std::unique_ptr<DefaultStatementNode>;

class ChildrenStatementNode : public StatementNode {
 public:
  ChildrenStatementNode(Token token) : StatementNode(token){};
  ChildrenStatementNode() : StatementNode(Token(Token::TokenType::RootToken)){};

  void add(StatementNodeUptr newNode);
  void add(std::vector<StatementNodeUptr>& newNodes);
  const std::vector<StatementNodeUptr>& getChildren() const { return children; }
  void buildTreeStringStream(int64_t depth,
                             std::stringstream& tree) const override;
  void remove(const StatementNode* node);
  Value accept(Evaluator& evaluator) const override {
    for (const auto& child : children) {
      child->accept(evaluator);
    }
    return 0;
  }

 protected:
  std::vector<StatementNodeUptr> children;
};

class IfStatementNode : public ChildrenStatementNode {
 public:
  IfStatementNode(Token token) : ChildrenStatementNode(token){};
  void setIfExpression(ExpressionNodeUptr expression) {
    this->ifExpression = std::move(expression);
  }
  void setOtherwiseExpression(StatementNodeUptr expression) {
    this->otherwiseExpression = std::move(expression);
  }
  const StatementNode* getOtherwiseExpression() const {
    if (otherwiseExpression) return otherwiseExpression.get();
    return nullptr;
  }
  const ExpressionNode* getIfExpression() const { return ifExpression.get(); }
  void buildTreeStringStream(int64_t depth,
                             std::stringstream& tree) const override;

  Value accept(Evaluator& evaluator) const override {
    return evaluator.evaluate(this);
  }

 private:
  ExpressionNodeUptr ifExpression;
  StatementNodeUptr otherwiseExpression;
};

class OtherwiseStatementNode : public ChildrenStatementNode {
 public:
  OtherwiseStatementNode(Token token) : ChildrenStatementNode(token){};
  Value accept(Evaluator& evaluator) const override {
    return evaluator.evaluate(this);
  }

  void buildTreeStringStream(int64_t depth,
                             std::stringstream& tree) const override;

 private:
};

class LoopStatementNode : public ChildrenStatementNode {
 public:
  LoopStatementNode(Token token) : ChildrenStatementNode(token) {}

  void setStep(ExpressionNodeUptr newStep) { this->step = std::move(newStep); }
  void setEnd(ExpressionNodeUptr newEnd) { this->end = std::move(newEnd); }
  void setStart(ExpressionNodeUptr newStart) {
    this->start = std::move(newStart);
  }
  const ExpressionNode* getStart() const { return start.get(); }
  const ExpressionNode* getEnd() const { return end.get(); }
  const ExpressionNode* getStep() const { return step.get(); }

  Value accept(Evaluator& evaluator) const override {
    return evaluator.evaluate(this);
  }

 private:
  ExpressionNodeUptr start;
  ExpressionNodeUptr end;
  ExpressionNodeUptr step;
};

class AslasStatementNode : public ChildrenStatementNode {
 public:
  AslasStatementNode(Token token) : ChildrenStatementNode(token){};
  void setAsLAsExpression(ExpressionNodeUptr expression) {
    this->aslasExpression = std::move(expression);
  }
  const ExpressionNode* getAsLAsExpression() const {
    return aslasExpression.get();
  }
  void buildTreeStringStream(int64_t depth,
                             std::stringstream& tree) const override;
  Value accept(Evaluator& evaluator) const override {
    return evaluator.evaluate(this);
  }

 private:
  ExpressionNodeUptr aslasExpression;
};

class FunctionStatementNode : public ChildrenStatementNode {
 public:
  FunctionStatementNode(Token functionToken, Token returnTypeToken)
      : ChildrenStatementNode(functionToken), returnType(returnTypeToken){};

  void setReturnType(Token& returnType) { this->returnType = returnType; }
  void setIdentifier(std::string identifier) { this->identifier = identifier; }
  void setArgumentsNodes(std::vector<ArgumentNodeUptr>& arguments) {
    std::ranges::move(arguments, std::back_inserter(this->arguments));
  }
  void buildTreeStringStream(int64_t depth,
                             std::stringstream& tree) const override;
  Value accept(Evaluator& evaluator) const override {
    return evaluator.evaluate(this);
  }

 protected:
  Token returnType;
  std::string identifier;
  std::vector<ArgumentNodeUptr> arguments;
};

class FunctionCallNode : public ChildrenStatementNode {
 public:
  FunctionCallNode(Token token) : ChildrenStatementNode(token){};
  void setIdentifier(std::string identifier) { this->identifier = identifier; }
  void setArgumentsNodes(std::vector<ExpressionNodeUptr>& arguments) {
    std::ranges::move(arguments, std::back_inserter(this->arguments));
  }
  void buildTreeStringStream(int64_t depth,
                             std::stringstream& tree) const override;
  Value accept(Evaluator& evaluator) const override {
    return evaluator.evaluate(this);
  }

 private:
  std::string identifier;
  std::vector<ExpressionNodeUptr> arguments;
};

class ConditionStatementNode : public ChildrenStatementNode {
 public:
  ConditionStatementNode(Token token) : ChildrenStatementNode(token){};
  void buildTreeStringStream(int64_t depth,
                             std::stringstream& tree) const override;
  Value accept(Evaluator& evaluator) const override {
    return evaluator.evaluate(this);
  }
  void setDefault(StatementNodeUptr defaultNode) {
    this->defaultStatement = std::move(defaultNode);
  }
  const StatementNode* getDefault() const {
    if (defaultStatement) return defaultStatement.get();
    return nullptr;
  }

 private:
  StatementNodeUptr defaultStatement;
};

class CaseStatementNode : public ChildrenStatementNode {
 public:
  CaseStatementNode(Token token) : ChildrenStatementNode(token){};
  void setCaseExpression(ExpressionNodeUptr expression) {
    this->caseExpression = std::move(expression);
  }
  const ExpressionNode* getCaseExpression() const{
    return caseExpression.get();
  }

  Value accept(Evaluator& evaluator) const override {
    return evaluator.evaluate(this);
  }

 private:
  ExpressionNodeUptr caseExpression;
};

class DefaultStatementNode : public ChildrenStatementNode {
 public:
  DefaultStatementNode(Token token) : ChildrenStatementNode(token){};
  Value accept(Evaluator& evaluator) const override {
    return evaluator.evaluate(this);
  }

 private:
};