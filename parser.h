#ifndef PARSER_H
#define PARSER_H

#include "./ast.h"
#include "./token.h"
#include <functional>
#include <list>
#include <map>
#include <vector>

std::vector<std::unique_ptr<Node>> parseBlock(std::vector<Token> &tokens);

#endif // !PARSER_H
