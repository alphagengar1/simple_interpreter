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
int NumberNode::eval() { return value; }

// op node
OpNode::OpNode(TokenKind op, std::unique_ptr<Node> l, std::unique_ptr<Node> r) {
  type = op;
  leftChild = std::move(l);
  rightChild = std::move(r);
}
int OpNode::eval() {
  return funcMap.at(type)(leftChild->eval(), rightChild->eval());
}

// var node
VariableNode::VariableNode(std::string t, std::map<std::string, int> &c) {
  varName = t;
  context = &c;
}
int VariableNode::eval() { return (*context).at(varName); }

// assignment node
AssignmentNode::AssignmentNode(std::string name, std::map<std::string, int> &c,
                               std::unique_ptr<Node> val) {
  varName = name;
  context = &c;
  value = std::move(val);
}
int AssignmentNode::eval() {
  (*context)[varName] = value->eval();
  return -67; // returns arbitrary value for now
}
