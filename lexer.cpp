#include "lexer.h"
#include "token.h"

std::vector<Token> lexer(std::string line) {

  std::vector<Token> tokens;

  for (int i = 0; i < (int)line.size(); i++) {

    if (line[i] == ' ')
      continue;

    if (line[i] == '+') {
      tokens.push_back({TokenKind::Plus, 0, ""});
      continue;
    }
    if (line[i] == '-') {
      tokens.push_back({TokenKind::Minus, 0, ""});
      continue;
    }
    if (line[i] == '*') {
      tokens.push_back({TokenKind::Star, 0, ""});
      continue;
    }
    if (line[i] == '/') {
      tokens.push_back({TokenKind::Slash, 0, ""});
      continue;
    }
    if (line[i] == '(') {
      tokens.push_back({TokenKind::LParen, 0, ""});
      continue;
    }
    if (line[i] == ')') {
      tokens.push_back({TokenKind::RParen, 0, ""});
      continue;
    }
    if (line[i] == '=') {
      tokens.push_back({TokenKind::Equals, 0, ""});
    }

    if (isalpha(line[i])) {
      std::string currentVar = "";
      while (i < (int)line.size() && isalpha(line[i])) {
        currentVar += line[i];
        i++;
      }
      tokens.push_back({TokenKind::Variable, 0, currentVar});
      i--;
    }

    if (isdigit(line[i])) {
      int currentNum = 0;
      while (i < (int)line.size() && isdigit(line[i])) {
        currentNum = currentNum * 10 + (line[i] - '0');
        i++;
      }
      tokens.push_back({TokenKind::Number, currentNum, ""});
      i--;
    }
  }
  // for (int i = 0; i < tokens.size(); i++) {
  //   std::cout << i << ": ";
  //   std::cout << tokens[i].kind << " ";
  //   std::cout << tokens[i].name << " ";
  //   std::cout << tokens[i].value << " ";
  //   std::cout << "\n";
  // }
  return tokens;
}
