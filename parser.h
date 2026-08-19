#ifndef PARSER_H
#define PARSER_H

#include "./ast.h"
#include "./token.h"
#include <functional>
#include <list>
#include <map>
#include <vector>

std::unique_ptr<Node> doOperation(std::vector<Token> &tokens,
                                  std::map<std::string, int> &context);

#endif // !PARSER_H
