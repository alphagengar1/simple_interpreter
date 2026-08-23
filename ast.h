#ifndef AST_H
#define AST_H

#include "./token.h"
#include <map>
#include <memory>
#include <string>

class Node {
public:
  virtual ~Node() {};
  virtual int eval(std::map<std::string, int> &context) = 0;
};

class NumberNode : public Node {
public:
  NumberNode(int val);
  int value;

  int eval(std::map<std::string, int> &context);
};

class OpNode : public Node {
public:
  OpNode(TokenKind op, std::unique_ptr<Node> l, std::unique_ptr<Node> r);

  TokenKind type;
  std::unique_ptr<Node> leftChild;
  std::unique_ptr<Node> rightChild;

  int eval(std::map<std::string, int> &context);
};

class AssignmentNode : public Node {
public:
  AssignmentNode(std::string name, std::unique_ptr<Node> val);

  std::string varName;
  std::unique_ptr<Node> value;

  int eval(std::map<std::string, int> &context);
};

class VariableNode : public Node {
public:
  VariableNode(std::string);

  std::string varName;

  int eval(std::map<std::string, int> &context);
};

#endif // !AST_H
