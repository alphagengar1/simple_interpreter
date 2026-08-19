#include "./parser.h"
#include "ast.h"
#include "token.h"

using std::vector;

std::unique_ptr<Node>
doOperation(std::vector<Token> &tokens,
            std::list<std::list<TokenKind>>::iterator curr, int begin, int end,
            std::map<std::string, int> &context);

std::unique_ptr<Node> parenValue(std::vector<Token> &tokens, int begin, int end,
                                 std::map<std::string, int> &context);

int find_location(std::vector<Token> &tokens,
                  std::list<std::list<TokenKind>>::iterator curr, int begin,
                  int end, std::map<TokenKind, int> &context);

std::map<std::list<TokenKind>, int> Directionality = {
    {{TokenKind::Equals}, 1},
    {{TokenKind::Plus, TokenKind::Minus}, -1},
    {{TokenKind::Star, TokenKind::Slash}, -1},
};

std::list<std::list<TokenKind>> Order = {{TokenKind::Equals},
                                         {TokenKind::Plus, TokenKind::Minus},
                                         {TokenKind::Star, TokenKind::Slash}};

int find_location(vector<Token> &tokens,
                  std::list<std::list<TokenKind>>::iterator curr, int begin,
                  int end) {
  int location = -1;
  int parenCount = 0;
  bool found = false;

  int step = Directionality.at(*curr);
  int start = Directionality.at(*curr) == 1 ? begin : end - 1;
  int stop = Directionality.at(*curr) == 1 ? end : begin - 1;

  TokenKind opening =
      Directionality.at(*curr) == 1 ? TokenKind::LParen : TokenKind::RParen;

  TokenKind closing =
      Directionality.at(*curr) == 1 ? TokenKind::RParen : TokenKind::LParen;

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
      while (i != end && parenCount != 0) {
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

std::unique_ptr<Node> parenValue(vector<Token> &tokens, int begin, int end,
                                 std::map<std::string, int> &context) {
  if (begin == end) {
    std::cout << "paren error begin == end lolz\n";
    return nullptr;
  }

  if (end - begin == 1 && tokens[begin].kind == TokenKind::Number) {
    std::unique_ptr<Node> num =
        std::make_unique<NumberNode>(tokens[begin].value);
    return num;
  }

  if (end - begin == 1 && tokens[begin].kind == TokenKind::Variable) {
    std::unique_ptr<Node> var =
        std::make_unique<VariableNode>(tokens[begin].name, context);
    return var;
  }

  if (tokens[begin].kind == TokenKind::LParen &&
      tokens[end - 1].kind == TokenKind::RParen) {
    return doOperation(tokens, Order.begin(), begin + 1, end - 1, context);
  }

  std::cout << "PAREN VALUE ERROR\n";
  for (int i = 0; i < (int)tokens.size(); i++) {
    std::cout << tokens[i].kind << ' ';
  }

  return nullptr;
};

std::unique_ptr<Node>
doOperation(vector<Token> &tokens,
            std::list<std::list<TokenKind>>::iterator curr, int begin, int end,
            std::map<std::string, int> &context) {
  int location = -1;

  location = find_location(tokens, curr, begin, end);

  if (location == -1) {
    if (++curr == Order.end()) {
      return parenValue(tokens, begin, end, context);
    }
    return doOperation(tokens, curr, begin, end, context);
  }

  if ((*curr).front() == TokenKind::Equals) {
    std::unique_ptr<Node> value(
        doOperation(tokens, curr, location + 1, end, context));

    std::unique_ptr<Node> assignment = std::make_unique<AssignmentNode>(
        tokens[location - 1].name, context, std::move(value));

    return assignment;
  }

  std::unique_ptr<Node> left(
      doOperation(tokens, curr, begin, location, context));
  std::unique_ptr<Node> right(
      doOperation(tokens, curr, location + 1, end, context));

  std::unique_ptr<Node> root = std::make_unique<OpNode>(
      tokens[location].kind, std::move(left), std::move(right));
  return root;
}

std::unique_ptr<Node> doOperation(vector<Token> &tokens,
                                  std::map<std::string, int> &context) {
  return doOperation(tokens, Order.begin(), 0, tokens.size(), context);
}
