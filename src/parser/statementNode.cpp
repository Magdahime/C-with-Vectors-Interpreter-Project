#include "parser/statementNode.hpp"

void ChildrenStatementNode::buildTreeStringStream(
    int64_t depth, std::stringstream& tree) const {
  std::string indent(depth, ' ');
  tree << indent << LexicalTable::token2StringTable.at(token.getType()) << '\n';
  for (const auto& child : children) {
    child->buildTreeStringStream(depth + 1, tree);
  }
}

void ChildrenStatementNode::remove(const StatementNode* node) {
  children.erase(std::ranges::find(children, node, &StatementNodeUptr::get));
}

void ChildrenStatementNode::add(StatementNodeUptr newNode) {
  children.push_back(std::move(newNode));
}
void ChildrenStatementNode::add(std::vector<StatementNodeUptr>& newNodes) {
  std::ranges::move(newNodes, std::back_inserter(children));
}

void LoopStatementNode::setStart(TokenVariant newStart) {
  if (std::holds_alternative<int64_t>(newStart))
    start = std::get<int64_t>(newStart);
  else
    start = std::get<std::string>(newStart);
}
void LoopStatementNode::setEnd(TokenVariant newEnd) {
  if (std::holds_alternative<int64_t>(newEnd))
    end = std::get<int64_t>(newEnd);
  else
    end = std::get<std::string>(newEnd);
}
void LoopStatementNode::setStep(TokenVariant newStep) {
  if (std::holds_alternative<int64_t>(newStep))
    step = std::get<int64_t>(newStep);
  else
    step = std::get<std::string>(newStep);
}

void FunctionStatementNode::buildTreeStringStream(
    int64_t depth, std::stringstream& tree) const {
  std::string indent(depth, ' ');
  tree << indent << LexicalTable::token2StringTable.at(returnType.getType())
       << " function " << identifier << "(";
  for (const auto& argument : arguments) {
    argument->buildTreeStringStream(tree);
    tree << ", ";
  }
  tree << ")" << '\n';

  for (const auto& child : children) {
    child->buildTreeStringStream(depth + 1, tree);
  }
}