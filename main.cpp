#include <functional>
#include <iostream>
#include <map>
#include <math.h>
#include <memory>
#include <stack>
#include <string>
#include <vector>
using namespace std;
using MathFunction = std::function<int(int, int)>;

map<string, int> ascii;
vector<string> lines;
map<string, string> type;

map<string, function<int(int, int)>> funcMap = {
    {"+", [](int a, int b) { return a + b; }},
    {"-", [](int a, int b) { return a - b; }},
    {"*", [](int a, int b) { return a * b; }},
    {"/", [](int a, int b) { return a / b; }},
};

class Node {
public:
  virtual ~Node() {};
  virtual int eval() = 0;
};

class NumberNode : public Node {
public:
  NumberNode(int val) { value = val; }
  int value;

  int eval() override { return value; }
};

class OpNode : public Node {
public:
  OpNode(string operation, unique_ptr<Node> l, unique_ptr<Node> r) {
    op = operation;
    leftChild = std::move(l);
    rightChild = std::move(r);
  }
  string op;
  unique_ptr<Node> leftChild;
  unique_ptr<Node> rightChild;

  int eval() override {
    return funcMap.at(op)(leftChild->eval(), rightChild->eval());
  }
};

void ascii_mappings(map<string, int> &ascii) {
  for (int i = 0; i < 10; i++) {
    ascii.insert({string(1, i + '0'), i + 48});
  }
  ascii.insert({"+", 43});
  ascii.insert({"-", 45});
  ascii.insert({"*", 42});
  ascii.insert({"/", 47});

  ascii.insert({"(", 47});
  ascii.insert({")", 47});

  return;
}

void create_types(map<string, string> &type) {
  for (int i = 0; i < 10; i++) {
    type.insert({string(1, i + '0'), "number"});
  }

  type.insert({"+", "operator"});
  type.insert({"-", "operator"});
  type.insert({"*", "operator"});
  type.insert({"/", "operator"});

  type.insert({"(", "parenthesis_open"});
  type.insert({")", "parenthesis_close"});
}

int main() {

  ascii_mappings(ascii);
  create_types(type);

  string line = "";

  while (getline(cin, line)) {
    lines.push_back(line);
  }

  ascii_mappings(ascii);

  create_types(type);

  for (auto &line : lines) {
    for (auto &chr : line) {
      if (ascii.count(string(1, chr)) == 0) {
        cout << "bad characters";
        return 0;
      }
    }
  }

  unique_ptr<Node> two = make_unique<NumberNode>(2);
  unique_ptr<Node> three = make_unique<NumberNode>(3);
  unique_ptr<Node> four = make_unique<NumberNode>(4);

  unique_ptr<Node> addition =
      make_unique<OpNode>("+", std::move(two), std::move(three));

  unique_ptr<Node> root =
      make_unique<OpNode>("*", std::move(addition), std::move(four));

  cout << root->eval();
}
