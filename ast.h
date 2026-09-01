#ifndef AST_H
#define AST_H

#include "./token.h"
#include <map>
#include <memory>
#include <string>
#include <vector>

enum Type { Number, Boolean, Error, Finish, Assigned };

struct EvalObject {
  Type type;
  int value;
};

class Node {
public:
  virtual ~Node() {};
  virtual EvalObject eval(std::vector<Context> &context) = 0;
};

class Master : public Node {
private:
  std::vector<std::unique_ptr<Node>> trees;

public:
  Master();
  void addTree(std::vector<std::unique_ptr<Node>> add);
  virtual EvalObject eval(std::vector<Context> &context);
};

class NumberNode : public Node {
public:
  NumberNode(int val);
  int value;

  EvalObject eval(std::vector<Context> &context);
};

class OpNode : public Node {
public:
  OpNode(TokenKind op, std::unique_ptr<Node> l, std::unique_ptr<Node> r);

  TokenKind type;
  std::unique_ptr<Node> leftChild;
  std::unique_ptr<Node> rightChild;

  EvalObject eval(std::vector<Context> &context);
};

class AssignmentNode : public Node {
public:
  AssignmentNode(std::string name, std::unique_ptr<Node> val);

  std::string varName;
  std::unique_ptr<Node> value;

  EvalObject eval(std::vector<Context> &context);
};

class EqualityNode : public Node {
public:
  EqualityNode(std::unique_ptr<Node> l, std::unique_ptr<Node> r);

  std::unique_ptr<Node> left;
  std::unique_ptr<Node> right;

  EvalObject eval(std::vector<Context> &context);
};

class VariableNode : public Node {
public:
  VariableNode(std::string);

  std::string varName;

  EvalObject eval(std::vector<Context> &context);
};

class IfNode : public Node {
public:
  IfNode(std::vector<std::pair<std::unique_ptr<Node>, Master>>);
  EvalObject eval(std::vector<Context> &context);

  std::vector<std::pair<std::unique_ptr<Node>, Master>> conditionals;
};

#endif // !AST_H
