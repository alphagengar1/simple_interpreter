#ifndef AST_H
#define AST_H

#include "./token.h"
#include <memory>

class Node {
public:
  virtual ~Node() {};
  virtual int eval() = 0;
};

class NumberNode : public Node {
public:
  NumberNode(int val);
  int value;

  int eval() override;
};

class OpNode : public Node {
public:
  OpNode(TokenKind op, std::unique_ptr<Node> l, std::unique_ptr<Node> r);
  TokenKind operation;
  std::unique_ptr<Node> leftChild;
  std::unique_ptr<Node> rightChild;

  int eval() override;
};

#endif // !AST_H
