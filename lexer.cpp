#include "lexer.h"
#include "token.h"

std::vector<Token> lexer(std::string line) {

  std::vector<Token> tokens;

  for (int i = 0; i < (int)line.size(); i++) {

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
    if (line[i] == '=') {
      tokens.push_back({TokenKind::Equals, 7});
    }
    if (line[i] == 'x') {
      tokens.push_back({TokenKind::Variable, 8});
    }

    if (isdigit(line[i])) {
      int currentNum = 0;
      while (i < (int)line.size() && isdigit(line[i])) {
        currentNum = currentNum * 10 + (line[i] - '0');
        i++;
      }
      tokens.push_back({TokenKind::Number, currentNum});
      i--;
    }
  }
  return tokens;
}
