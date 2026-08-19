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
  virtual std::string getName() = 0;
};

class OpNode : public Node {
public:
  OpNode(TokenKind op, std::map<std::string, int> &c, std::unique_ptr<Node> l,
         std::unique_ptr<Node> r);
  std::string getName() override;
  std::map<std::string, int> *context;
  TokenKind operation;
  std::unique_ptr<Node> leftChild;
  std::unique_ptr<Node> rightChild;

  int eval() override;
};

class NumberNode : public Node {
public:
  NumberNode(int val);
  std::string getName() override;
  int value;

  int eval() override;
};

class VariableNode : public Node {
public:
  VariableNode(std::string str, std::map<std::string, int> &c);

  std::string name;
  std::map<std::string, int> *context;

  int eval() override;
  std::string getName() override;
};

#endif // !AST_H
