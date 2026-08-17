#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <math.h>
#include <memory>
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

list<list<TokenKind>> Order = {{TokenKind::Plus, TokenKind::Minus},
                               {TokenKind::Star, TokenKind::Slash}};

map<list<TokenKind>, int> Directionality = {
    {{TokenKind::Plus, TokenKind::Minus}, -1},
    {{TokenKind::Star, TokenKind::Slash}, -1},
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

unique_ptr<Node> doOperation(vector<Token> tokens,
                             list<list<TokenKind>>::iterator curr);
unique_ptr<Node> parenValue(vector<Token> tokens);

int find_location(vector<Token> tokens, list<list<TokenKind>>::iterator curr) {
  int location = -1;
  int parenCount = 0;
  bool found = false;

  int step = Directionality[*curr];
  int start = Directionality[*curr] == 1 ? 0 : tokens.size() - 1;
  int end = Directionality[*curr] == 1 ? tokens.size() : -1;

  TokenKind opening =
      Directionality[*curr] == 1 ? TokenKind::LParen : TokenKind::RParen;

  TokenKind closing =
      Directionality[*curr] == 1 ? TokenKind::RParen : TokenKind::LParen;

  for (int i = start; i != end; i += step) {
    for (auto it : *curr) {
      if (tokens[i].kind == it) {
        location = i;
        found = true;
        break;
      }
    }
    if (found)
      break;

    if (tokens[i].kind == opening) {
      parenCount++;
      while (i < tokens.size() && parenCount != 0) {
        i += step;
        if (tokens[i].kind == opening)
          parenCount++;

        if (tokens[i].kind == closing)
          parenCount--;
      }
    }
  }
  return location;
}

unique_ptr<Node> doOperation(vector<Token> tokens,
                             list<list<TokenKind>>::iterator curr) {
  int location = -1;

  location = find_location(tokens, curr);

  if (location == -1) {
    if (++curr == Order.end()) {
      return parenValue(tokens);
    }
    --curr;
    return doOperation(tokens, ++curr);
  }

  vector<Token> tokensLeft;
  vector<Token> tokensRight;

  for (int i = 0; i < location; i++) {
    tokensLeft.push_back(tokens[i]);
  }
  for (int i = location + 1; i < tokens.size(); i++) {
    tokensRight.push_back(tokens[i]);
  }

  unique_ptr<Node> left(doOperation(tokensLeft, curr));
  unique_ptr<Node> right(doOperation(tokensRight, curr));

  unique_ptr<Node> root = make_unique<OpNode>(
      tokens[location].kind, std::move(left), std::move(right));
  return root;
}

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
    return doOperation(newTokens, Order.begin());
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
    auto root = doOperation(tokens, Order.begin());
    cout << line << '=';
    cout << root->eval();
    cout << '\n';
  }
}
