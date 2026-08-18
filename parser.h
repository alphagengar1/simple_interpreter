#ifndef PARSER_H
#define PARSER_H

#include "./ast.h"
#include "token.h"
#include <functional>
#include <list>
#include <map>

std::unique_ptr<Node>
doOperation(std::vector<Token> tokens,
            std::list<std::list<TokenKind>>::iterator curr, int begin, int end);

std::unique_ptr<Node> doOperation(std::vector<Token> tokens);

std::unique_ptr<Node> parenValue(std::vector<Token> tokens, int begin, int end);

int find_location(std::vector<Token> tokens,
                  std::list<std::list<TokenKind>>::iterator curr, int begin,
                  int end);

#endif // !PARSER_H
