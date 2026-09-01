#include "ast.h"
#include "token.h"
#include <functional>
#include <map>
#include <vector>

std::map<TokenKind, std::function<int(int, int)>> funcMap = {
    {TokenKind::Plus, [](int a, int b) { return a + b; }},
    {TokenKind::Minus, [](int a, int b) { return a - b; }},
    {TokenKind::Star, [](int a, int b) { return a * b; }},
    {TokenKind::Slash, [](int a, int b) { return a / b; }},
};

// master node
Master::Master() {}

void Master::addTree(std::vector<std::unique_ptr<Node>> add) {
  trees = std::move(add);
}

EvalObject Master::eval(std::vector<Context> &context) {
  context.emplace_back();
  for (int i = 0; i < (int)trees.size(); i++) {
    EvalObject answer = trees[i]->eval(context);

    if (answer.type == Type::Number) {
      std::cout << answer.value << '\n';
    } else if (answer.type == Type::Boolean) {
      std::cout << ((answer.value) ? "True\n" : "False\n");
    } else if (answer.type == Type::Assigned) {
      std::cout << "Variable Assigned\n";
    } else if (answer.type == Type::Error) {
      std::cout << "Error\n";
    }
  }
  context.pop_back();
  return EvalObject{Type::Finish, 1};
}

// number node
NumberNode::NumberNode(int val) { value = val; }
EvalObject NumberNode::eval(std::vector<Context> &context) {
  return EvalObject{Type::Number, value};
}

// op node
OpNode::OpNode(TokenKind op, std::unique_ptr<Node> l, std::unique_ptr<Node> r) {
  type = op;
  leftChild = std::move(l);
  rightChild = std::move(r);
}
EvalObject OpNode::eval(std::vector<Context> &context) {
  int answer = funcMap.at(type)(leftChild->eval(context).value,
                                rightChild->eval(context).value);
  return EvalObject{Type::Number, answer};
}

// var node
VariableNode::VariableNode(std::string t) { varName = t; }
EvalObject VariableNode::eval(std::vector<Context> &context) {
  for (int i = (int)context.size() - 1; i >= 0; i--) {
    if (context[i].var_context.count(varName) == 0)
      continue;
    return EvalObject{Type::Number, context[i].var_context[varName]};
  }
  return EvalObject{Type::Error, 0};
}

// assignment node
AssignmentNode::AssignmentNode(std::string name, std::unique_ptr<Node> val) {
  varName = name;
  value = std::move(val);
}

EvalObject AssignmentNode::eval(std::vector<Context> &context) {
  for (int i = (int)context.size() - 1; i >= 0; i--) {
    if (context[i].var_context.count(varName) == 0)
      continue;
    context[i].var_context[varName] = value->eval(context).value;
    return EvalObject{Type::Assigned, 1};
  }
  context[context.size() - 1].var_context[varName] = value->eval(context).value;
  return EvalObject{Type::Assigned, 1};
}

// equality node
EqualityNode::EqualityNode(std::unique_ptr<Node> l, std::unique_ptr<Node> r) {
  left = std::move(l);
  right = std::move(r);
}
EvalObject EqualityNode::eval(std::vector<Context> &context) {
  if (left->eval(context).type == Type::Error ||
      right->eval(context).type == Type::Error)
    return EvalObject{Error, 0};
  if (left->eval(context).value == right->eval(context).value)
    return EvalObject{Type::Boolean, 1};
  return EvalObject{Type::Boolean, 0};
}

// if node
IfNode::IfNode(std::vector<std::pair<std::unique_ptr<Node>, Master>> c) {
  conditionals = std::move(c);
}
EvalObject IfNode::eval(std::vector<Context> &context) {

  for (int i = 0; i < (int)conditionals.size(); i++) {
    if (conditionals[i].first->eval(context).value)
      return conditionals[i].second.eval(context);
  }

  return EvalObject{Type::Finish, 0};
}
