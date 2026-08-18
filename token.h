#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>

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

std::ostream &operator<<(std::ostream &os, TokenKind kind);

#endif // !TOKEN_H
