#include <iostream>
#include <math.h>
#include <string>
#include <vector>

#include "./lexer.h"
#include "./parser.h"
#include "./token.h"

using namespace std;

int main() {

  vector<string> lines;
  string line = "";

  while (getline(cin, line)) {
    lines.push_back(line);
  }

  map<string, int> context;

  for (const string &line : lines) {
    vector<Token> tokens = lexer(line);
    auto root = doOperation(tokens, context);
    cout << line << '=';
    cout << root->eval();
    cout << '\n';
  }
}
