#include <cstdio>
#include <iostream>
#include <math.h>
#include <string>
#include <vector>

#include "./lexer.h"
#include "./parser.h"
#include "./token.h"

using namespace std;

int main() {

  std::string lines((std::istreambuf_iterator<char>(std::cin)),
                    std::istreambuf_iterator<char>());

  vector<Context> context;

  Master masterNode;

  vector<Token> tokens = lexer(lines);

  masterNode.addTree(parseBlock(tokens));
  masterNode.eval(context);
}
