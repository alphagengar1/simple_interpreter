#include "lexer.h"
#include "token.h"

std::vector<Token> lexer(std::string line) {

  std::vector<Token> tokens;

  for (int i = 0; i < (int)line.size(); i++) {

    if (i < (int)line.size() - 3 && line[i] == 'e' && line[i + 1] == 'l' &&
        line[i + 2] == 's' && line[i + 3] == 'e') {
      tokens.push_back({TokenKind::Else, 0, ""});
      i += 3;
      continue;
    }
    if (i < (int)line.size() - 1 && line[i] == 'i' && line[i + 1] == 'f') {
      tokens.push_back({TokenKind::If, 0, ""});
      i++;
      continue;
    }
    if (i < (int)line.size() - 1 && line[i] == '=' && line[i + 1] == '=') {
      tokens.push_back({TokenKind::Equality, 0, ""});
      i++;
      continue;
    }
    if (line[i] == ';') {
      tokens.push_back({TokenKind::Semicolon, 0, ""});
      continue;
    }

    if (line[i] == '{') {
      tokens.push_back({TokenKind::LBrace, 0, ""});
      continue;
    }
    if (line[i] == '}') {
      tokens.push_back({TokenKind::RBrace, 0, ""});
      continue;
    }

    if (line[i] == ' ' || line[i] == '\n')
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
