#pragma once
#include "parser/node.hpp"
#include "parser/expressionNode.hpp"

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
  void accept( Interpreter& interpreter) override{
    for(const auto& child : children){
      child->accept( interpreter);
    }
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
  void buildTreeStringStream(int64_t depth,
                             std::stringstream& tree) const override;

  void accept( Interpreter& interpreter) override {
    interpreter.visit(this);
  }

 private:
  ExpressionNodeUptr ifExpression;
};

class OtherwiseStatementNode : public ChildrenStatementNode {
 public:
  OtherwiseStatementNode(Token token) : ChildrenStatementNode(token){};
  void accept( Interpreter& interpreter) override {
    interpreter.visit(this);
  }

 private:
};

class LoopStatementNode : public ChildrenStatementNode {
 public:
  LoopStatementNode(Token token) : ChildrenStatementNode(token) {}

  void setStep(TokenVariant newStep);
  void setEnd(TokenVariant newEnd);
  void setStart(TokenVariant newStart);
  std::variant<std::string, int64_t> getStart(){return start;}
  std::variant<std::string, int64_t> getEnd(){return end;}
  std::variant<std::string, int64_t> getStep(){return step;}

  void accept( Interpreter& interpreter) override {
    interpreter.visit(this);
  }

 private:
  std::variant<std::string, int64_t> start = 0;
  std::variant<std::string, int64_t> end = 0;
  std::variant<std::string, int64_t> step = 1;
};

class AslasStatementNode : public ChildrenStatementNode {
 public:
  AslasStatementNode(Token token) : ChildrenStatementNode(token){};
  void setAsLAsExpression(ExpressionNodeUptr expression) {
    this->aslasExpression = std::move(expression);
  }
  void buildTreeStringStream(int64_t depth,
                             std::stringstream& tree) const override;
  void accept( Interpreter& interpreter) override {
    interpreter.visit(this);
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
  void accept( Interpreter& interpreter) override {
    interpreter.visit(this);
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
  void accept( Interpreter& interpreter) override {
    interpreter.visit(this);
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
  void accept( Interpreter& interpreter) override {
    interpreter.visit(this);
  }

 private:
};

class CaseStatementNode : public ChildrenStatementNode {
 public:
  CaseStatementNode(Token token) : ChildrenStatementNode(token){};
  void setCaseExpression(ExpressionNodeUptr expression) {
    this->caseExpression = std::move(expression);
  }
  void accept( Interpreter& interpreter) override {
    interpreter.visit(this);
  }
 private:
  ExpressionNodeUptr caseExpression;
};

class DefaultStatementNode : public ChildrenStatementNode {
 public:
  DefaultStatementNode(Token token) : ChildrenStatementNode(token){};
  void accept( Interpreter& interpreter) override {
    interpreter.visit(this);
  }
 private:
};