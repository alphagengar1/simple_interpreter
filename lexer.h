#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include <string>
#include <vector>

std::vector<Token> lexer(std::string line);

#endif // !LEXER_H
