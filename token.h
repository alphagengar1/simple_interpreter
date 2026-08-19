#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>

enum class TokenKind {
  Number,
  Variable,
  Plus,
  Minus,
  Star,
  Slash,
  LParen,
  RParen,
  Equals,
  Eol,
  Eof
};

struct Token {
  TokenKind kind;
  int value;
};

std::ostream &operator<<(std::ostream &os, TokenKind kind);

#endif // !TOKEN_H
