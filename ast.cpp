#include "ast.h"
#include "token.h"
#include <functional>
#include <map>

std::map<TokenKind, std::function<int(int, int)>> funcMap = {
    {TokenKind::Plus, [](int a, int b) { return a + b; }},
    {TokenKind::Minus, [](int a, int b) { return a - b; }},
    {TokenKind::Star, [](int a, int b) { return a * b; }},
    {TokenKind::Slash, [](int a, int b) { return a / b; }},
};

// number node
NumberNode::NumberNode(int val) { value = val; }
int NumberNode::eval(std::map<std::string, int> &context) { return value; }

// op node
OpNode::OpNode(TokenKind op, std::unique_ptr<Node> l, std::unique_ptr<Node> r) {
  type = op;
  leftChild = std::move(l);
  rightChild = std::move(r);
}
int OpNode::eval(std::map<std::string, int> &context) {
  return funcMap.at(type)(leftChild->eval(context), rightChild->eval(context));
}

// var node
VariableNode::VariableNode(std::string t) { varName = t; }
int VariableNode::eval(std::map<std::string, int> &context) {
  return context.at(varName);
}

// assignment node
AssignmentNode::AssignmentNode(std::string name, std::unique_ptr<Node> val) {
  varName = name;
  value = std::move(val);
}
int AssignmentNode::eval(std::map<std::string, int> &context) {
  context[varName] = value->eval(context);
  return -67; // returns arbitrary value for now
}
