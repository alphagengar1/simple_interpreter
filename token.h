#ifndef TOKEN_H
#define TOKEN_H

#include <functional>
#include <iostream>
#include <map>

struct Context {
  std::map<std::string, int> var_context;
};

enum class SymbolKind { Number, Variable, Operations, Other };

enum class TokenKind {
  // number
  Number,
  // variable
  Variable,
  // if
  If,
  Else,
  // operations
  Plus,
  Minus,
  Star,
  Slash,
  LBrace,
  RBrace,
  LParen,
  RParen,
  Equals,
  Equality,
  // other
  Eol,
  Eof,
  Semicolon
};

struct Token {
  TokenKind kind;
  int value;
  std::string name;
};

std::ostream &operator<<(std::ostream &os, Token kind);

#endif // !TOKEN_H
