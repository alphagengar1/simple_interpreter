#include "token.h"

std::ostream &operator<<(std::ostream &os, Token token) {
  switch (token.kind) {
  case TokenKind::Semicolon:
    return os << ";";
  case TokenKind::LBrace:
    return os << "{";
  case TokenKind::RBrace:
    return os << "}";
  case TokenKind::If:
    return os << "if";
  case TokenKind::Else:
    return os << "else";
  case TokenKind::Equality:
    return os << "==";
  case TokenKind::Number:
    return os << token.value;
  case TokenKind::Variable:
    return os << token.name;
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
  case TokenKind::Equals:
    return os << "=";
  case TokenKind::Eol:
    return os << "\\n";
  case TokenKind::Eof:
    return os << "<eof>";
  }
  return os << "??";
}
