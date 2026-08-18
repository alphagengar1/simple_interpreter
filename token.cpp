#include "token.h"

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
