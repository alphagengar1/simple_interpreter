#include "ast.h"

NumberNode::NumberNode(int val) { value = val; }
int NumberNode::eval() { return value; }

OpNode::OpNode(TokenKind op, std::unique_ptr<Node> l, std::unique_ptr<Node> r) {
  operation = op;
  leftChild = std::move(l);
  rightChild = std::move(r);
}
