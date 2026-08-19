#include "ast.h"
#include "token.h"
#include <functional>
#include <map>
#include <string>

std::map<TokenKind, std::function<int(int, int)>> funcMap = {
    {TokenKind::Plus, [](int a, int b) { return a + b; }},
    {TokenKind::Minus, [](int a, int b) { return a - b; }},
    {TokenKind::Star, [](int a, int b) { return a * b; }},
    {TokenKind::Slash, [](int a, int b) { return a / b; }},
};

NumberNode::NumberNode(int val) { value = val; }
int NumberNode::eval() { return value; }
std::string NumberNode::getName() { return ""; }

OpNode::OpNode(TokenKind op, std::map<std::string, int> &c,
               std::unique_ptr<Node> l, std::unique_ptr<Node> r) {
  context = &c;
  operation = op;
  leftChild = std::move(l);
  rightChild = std::move(r);
}
int OpNode::eval() {
  if (operation == TokenKind::Equals) {
    (*context)[leftChild->getName()] = rightChild->eval();
    return rightChild->eval();
  }
  return funcMap.at(operation)(leftChild->eval(), rightChild->eval());
}
std::string OpNode::getName() { return ""; }

VariableNode::VariableNode(std::string str, std::map<std::string, int> &c) {
  name = str;
  context = &c;
}

int VariableNode::eval() { return (*context).at(name); }
std::string VariableNode::getName() { return name; }
