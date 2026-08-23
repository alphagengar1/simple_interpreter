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

  map<std::string, int> context;

  for (const string &line : lines) {
    vector<Token> tokens = lexer(line);
    auto root = doOperation(tokens);
    int answer = root->eval(context);
    if (answer != -67) {
      cout << line << '=';
      cout << answer;
    } else {
      cout << "assignment done!";
    }
    cout << '\n';
  }
}
