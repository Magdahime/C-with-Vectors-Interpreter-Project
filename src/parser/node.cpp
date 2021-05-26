#include "parser/node.hpp"

LogicalOperatorNode::LogicalOperatorNode(Token token)
    : BinaryOperatorNode(token) {
  switch (token.getSubtype()) {
    case Token::TokenSubtype::GreaterToken:
      this->type = NodeType::Greater;
      break;
    case Token::TokenSubtype::GreaterOrEqualToken:
      this->type = NodeType::GreaterEqual;
      break;
    case Token::TokenSubtype::LessToken:
      this->type = NodeType::Less;
      break;
    case Token::TokenSubtype::LessOrEqualToken:
      this->type = NodeType::LessEqual;
      break;
    case Token::TokenSubtype::NotEqualToken:
      this->type = NodeType::NotEqual;
      break;
    case Token::TokenSubtype::EqualToken:
      this->type = NodeType::Equal;
      break;
    default:
      std::string message = "Token at " + token.getLinePositionString() +
                            " is unexpected. Expecting: LogicalOperatorToken";
      throw UnexpectedToken(message);
  }
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

void StatementNode::buildTreeStringStream(int64_t depth,
                                          std::stringstream& tree) const {
  std::string indent(depth, ' ');
  tree << indent << LexicalTable::token2StringTable.at(token.getType()) << '\n';
  for (const auto& child : children) {
    child->buildTreeStringStream(depth + 1, tree);
  }
}

void StatementNode::remove(const Node* node) {
  children.erase(std::ranges::find(children, node, &NodeUptr::get));
}

void StatementNode::add(NodeUptr newNode) {
  children.push_back(std::move(newNode));
}
void StatementNode::add(std::vector<NodeUptr>& newNodes) {
  std::ranges::move(newNodes, std::back_inserter(children));
}

void BinaryOperatorNode::buildTreeStringStream(int64_t depth,
                                               std::stringstream& tree) const {
  std::string indent(depth, ' ');
  tree << indent << LexicalTable::token2StringTable.at(token.getType()) << '\n';
  if (left->isLeaf()) {
    std::string indent(depth + 1, ' ');
    tree << indent
         << LexicalTable::token2StringTable.at(left->getToken().getType())
         << '\n';
  } else {
    left->buildTreeStringStream(depth + 1, tree);
  }
  if (right->isLeaf()) {
    std::string indent(depth + 1, ' ');
    tree << indent
         << LexicalTable::token2StringTable.at(right->getToken().getType())
         << '\n';
  } else {
    right->buildTreeStringStream(depth + 1, tree);
  }
}