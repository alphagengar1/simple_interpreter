#ifndef AST_H
#define AST_H

#include "./token.h"
#include <map>
#include <memory>
#include <string>

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

  TokenKind type;
  std::unique_ptr<Node> leftChild;
  std::unique_ptr<Node> rightChild;

  int eval() override;
};

class AssignmentNode : public Node {
public:
  AssignmentNode(std::string name, std::map<std::string, int> &c,
                 std::unique_ptr<Node> val);

  std::map<std::string, int> *context;
  std::string varName;
  std::unique_ptr<Node> value;

  int eval() override;
};

class VariableNode : public Node {
public:
  VariableNode(std::string, std::map<std::string, int> &c);

  std::map<std::string, int> *context;

  std::string varName;

  int eval() override;
};

#endif // !AST_H
