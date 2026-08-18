#include "./parser.h"

using std::vector;

std::map<std::list<TokenKind>, int> Directionality = {
    {{TokenKind::Plus, TokenKind::Minus}, -1},
    {{TokenKind::Star, TokenKind::Slash}, -1},
};

std::map<TokenKind, std::function<int(int, int)>> funcMap = {
    {TokenKind::Plus, [](int a, int b) { return a + b; }},
    {TokenKind::Minus, [](int a, int b) { return a - b; }},
    {TokenKind::Star, [](int a, int b) { return a * b; }},
    {TokenKind::Slash, [](int a, int b) { return a / b; }},
};

int OpNode::eval() {
  return funcMap.at(operation)(leftChild->eval(), rightChild->eval());
}

inline std::list<std::list<TokenKind>> Order = {
    {TokenKind::Plus, TokenKind::Minus}, {TokenKind::Star, TokenKind::Slash}};

std::unique_ptr<Node> doOperation(vector<Token> tokens) {
  return doOperation(tokens, Order.begin(), 0, tokens.size());
}

std::unique_ptr<Node>
doOperation(vector<Token> tokens,
            std::list<std::list<TokenKind>>::iterator curr, int begin,
            int end) {
  int location = -1;

  location = find_location(tokens, curr, begin, end);

  if (location == -1) {
    if (++curr == Order.end()) {
      return parenValue(tokens, begin, end);
    }
    return doOperation(tokens, curr, begin, end);
  }

  std::unique_ptr<Node> left(doOperation(tokens, curr, begin, location));
  std::unique_ptr<Node> right(doOperation(tokens, curr, location + 1, end));

  std::unique_ptr<Node> root = std::make_unique<OpNode>(
      tokens[location].kind, std::move(left), std::move(right));
  return root;
}

std::unique_ptr<Node> parenValue(vector<Token> tokens, int begin, int end) {
  if (end - begin == 1 && tokens[begin].kind == TokenKind::Number) {
    std::unique_ptr<Node> num =
        std::make_unique<NumberNode>(tokens[begin].value);
    return num;
  }
  if (tokens[begin].kind == TokenKind::LParen &&
      tokens[end - 1].kind == TokenKind::RParen) {
    return doOperation(tokens, Order.begin(), begin + 1, end - 1);
  }
  std::cout << "PAREN VALUE ERROR\n";
  for (int i = 0; i < (int)tokens.size(); i++) {
    std::cout << tokens[i].kind << ' ';
  }
  return nullptr;
};

int find_location(vector<Token> tokens,
                  std::list<std::list<TokenKind>>::iterator curr, int begin,
                  int end) {
  int location = -1;
  int parenCount = 0;
  bool found = false;

  int step = Directionality[*curr];
  int start = Directionality[*curr] == 1 ? begin : end - 1;
  int stop = Directionality[*curr] == 1 ? end : begin - 1;

  TokenKind opening =
      Directionality[*curr] == 1 ? TokenKind::LParen : TokenKind::RParen;

  TokenKind closing =
      Directionality[*curr] == 1 ? TokenKind::RParen : TokenKind::LParen;

  for (int i = start; i != stop; i += step) {
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
      while (i < (int)tokens.size() && parenCount != 0) {
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
