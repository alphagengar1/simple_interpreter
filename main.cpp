#include <iostream>
#include <list>
#include <math.h>
#include <string>
#include <vector>
using namespace std;

#include "./ast.h"
#include "./lexer.h"
#include "./parser.h"
#include "./token.h"

int main() {

  vector<string> lines;
  string line = "";

  while (getline(cin, line)) {
    lines.push_back(line);
  }

  for (const string &line : lines) {
    vector<Token> tokens = lexer(line);
    auto root = doOperation(tokens);
    cout << line << '=';
    cout << root->eval();
    cout << '\n';
  }
}
