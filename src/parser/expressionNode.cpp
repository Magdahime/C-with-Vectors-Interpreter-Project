#include "parser/expressionNode.hpp"

LogicalOperatorNode::LogicalOperatorNode(Token token)
    : ExpressionValueNode(token) {
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

MatrixOperatorNode::MatrixOperatorNode(Token token)
    : ExpressionValueNode(token) {
  if (token == Token::TokenType::DetToken) {
    this->type = NodeType::Det;
  } else if (token == Token::TokenType::TransToken) {
    this->type = NodeType::Transpose;
  } else {
    this->type = NodeType::Inverse;
  }
}

void ExpressionValueNode::buildTreeStringStream(int64_t depth,
                                                std::stringstream& tree) const {
  std::string indent(depth, ' ');
  tree << indent << LexicalTable::token2StringTable.at(token.getType()) << '\n';
  for (const auto& expresssion : expressions) {
    expresssion->buildTreeStringStream(depth + 1, tree);
  }
}

void ExpressionValueNode::add(ExpressionNodeUptr newNode) {
  expressions.push_back(std::move(newNode));
}
void ExpressionValueNode::add(std::vector<ExpressionNodeUptr>& newNodes) {
  std::ranges::move(newNodes, std::back_inserter(expressions));
}
void ExpressionValueNode::remove(const ExpressionNode* node) {
  expressions.erase(
      std::ranges::find(expressions, node, &ExpressionNodeUptr::get));
}

void ArgumentNode::buildTreeStringStream(std::stringstream& tree) const {
  tree << " " << LexicalTable::token2StringTable.at(token.getType()) << " "
       << identifier;
}