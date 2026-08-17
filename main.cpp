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

enum class TokenKind {
  Number,
  Plus,
  Minus,
  Star,
  Slash,
  LParen,
  RParen,
  Eol,
  Eof
};

struct Token {
  TokenKind kind;
  int value;
};

map<TokenKind, function<int(int, int)>> funcMap = {
    {TokenKind::Plus, [](int a, int b) { return a + b; }},
    {TokenKind::Minus, [](int a, int b) { return a - b; }},
    {TokenKind::Star, [](int a, int b) { return a * b; }},
    {TokenKind::Slash, [](int a, int b) { return a / b; }},
};

std::ostream &operator<<(std::ostream &os, TokenKind kind) {
  switch (kind) {
  case TokenKind::Number:
    return os << "<num>";
  case TokenKind::Plus:
    return os << "+";
  case TokenKind::Minus:
    return os << "-";
  case TokenKind::Star:
    return os << "*";
  case TokenKind::Slash:
    return os << "/";
  case TokenKind::LParen:
    return os << "(";
  case TokenKind::RParen:
    return os << ")";
  case TokenKind::Eol:
    return os << "\\n";
  case TokenKind::Eof:
    return os << "<eof>";
  }
  return os << "??";
}

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
  OpNode(TokenKind op, unique_ptr<Node> l, unique_ptr<Node> r) {
    operation = op;
    leftChild = std::move(l);
    rightChild = std::move(r);
  }
  TokenKind operation;
  unique_ptr<Node> leftChild;
  unique_ptr<Node> rightChild;

  int eval() override {
    return funcMap.at(operation)(leftChild->eval(), rightChild->eval());
  }
};

vector<Token> tokenizer(string line) {

  vector<Token> tokens;

  for (int i = 0; i < line.size(); i++) {

    if (line[i] == ' ')
      continue;

    if (line[i] == '+') {
      tokens.push_back({TokenKind::Plus, 1});
      continue;
    }
    if (line[i] == '-') {
      tokens.push_back({TokenKind::Minus, 2});
      continue;
    }
    if (line[i] == '*') {
      tokens.push_back({TokenKind::Star, 3});
      continue;
    }
    if (line[i] == '/') {
      tokens.push_back({TokenKind::Slash, 4});
      continue;
    }
    if (line[i] == '(') {
      tokens.push_back({TokenKind::LParen, 5});
      continue;
    }
    if (line[i] == ')') {
      tokens.push_back({TokenKind::RParen, 6});
      continue;
    }

    if (isdigit(line[i])) {
      int currentNum = 0;
      while (i < line.size() && isdigit(line[i])) {
        currentNum = currentNum * 10 + stoi(string(1, line[i]));
        i++;
      }
      tokens.push_back({TokenKind::Number, currentNum});
      i--;
    }
  }
  return tokens;
}

unique_ptr<Node> plusMinus(vector<Token> tokens);
unique_ptr<Node> multDivide(vector<Token> tokens);
unique_ptr<Node> parenValue(vector<Token> tokens);

unique_ptr<Node> plusMinus(vector<Token> tokens) {
  int location = -1;
  int parenCount = 0;

  for (int i = tokens.size() - 1; i >= 0; i--) {
    if (tokens[i].kind == TokenKind::Plus ||
        tokens[i].kind == TokenKind::Minus) {
      location = i;
      break;
    }
    if (tokens[i].kind == TokenKind::RParen) {
      parenCount++;
      while (i >= 0 && parenCount != 0) {
        i--;
        if (tokens[i].kind == TokenKind::RParen)
          parenCount++;

        if (tokens[i].kind == TokenKind::LParen)
          parenCount--;
      }
    }
  }
  if (location == -1) {
    return multDivide(tokens);
  }

  vector<Token> tokensLeft;
  vector<Token> tokensRight;

  for (int i = 0; i < location; i++) {
    tokensLeft.push_back(tokens[i]);
  }
  for (int i = location + 1; i < tokens.size(); i++) {
    tokensRight.push_back(tokens[i]);
  }

  unique_ptr<Node> left(plusMinus(tokensLeft));
  unique_ptr<Node> right(plusMinus(tokensRight));

  unique_ptr<Node> root = make_unique<OpNode>(
      tokens[location].kind, std::move(left), std::move(right));
  return root;
};

unique_ptr<Node> multDivide(vector<Token> tokens) {
  int location = -1;
  int parenCount = 0;

  for (int i = tokens.size() - 1; i >= 0; i--) {
    if (tokens[i].kind == TokenKind::Star ||
        tokens[i].kind == TokenKind::Slash) {
      location = i;
      break;
    }
    if (tokens[i].kind == TokenKind::RParen) {
      parenCount++;
      while (i >= 0 && parenCount != 0) {
        i--;
        if (tokens[i].kind == TokenKind::RParen)
          parenCount++;

        if (tokens[i].kind == TokenKind::LParen)
          parenCount--;
      }
    }
  }
  if (location == -1) {
    return parenValue(tokens);
  }

  vector<Token> tokensLeft;
  vector<Token> tokensRight;

  for (int i = 0; i < location; i++) {
    tokensLeft.push_back(tokens[i]);
  }
  for (int i = location + 1; i < tokens.size(); i++) {
    tokensRight.push_back(tokens[i]);
  }

  unique_ptr<Node> left(multDivide(tokensLeft));
  unique_ptr<Node> right(multDivide(tokensRight));

  unique_ptr<Node> root = make_unique<OpNode>(
      tokens[location].kind, std::move(left), std::move(right));
  return root;
};

unique_ptr<Node> parenValue(vector<Token> tokens) {
  if (tokens.size() == 1 && tokens[0].kind == TokenKind::Number) {
    unique_ptr<Node> num = make_unique<NumberNode>(tokens[0].value);
    return num;
  }
  if (tokens[0].kind == TokenKind::LParen &&
      tokens[tokens.size() - 1].kind == TokenKind::RParen) {
    vector<Token> newTokens;
    for (int i = 1; i < tokens.size() - 1; i++) {
      newTokens.push_back(tokens[i]);
    }
    return plusMinus(newTokens);
  }
  cout << "PAREN VALUE ERROR\n";
  for (int i = 0; i < tokens.size(); i++) {
    cout << tokens[i].kind << ' ';
  }
  return nullptr;
};

int main() {

  vector<string> lines;
  string line = "";

  while (getline(cin, line)) {
    lines.push_back(line);
  }

  for (const string &line : lines) {
    vector<Token> tokens = tokenizer(line);
    auto root = plusMinus(tokens);
    cout << line << '=';
    cout << root->eval();
    cout << '\n';
  }
}
