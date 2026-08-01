#include <functional>
#include <iostream>
#include <map>
#include <math.h>
#include <string>
#include <vector>
using namespace std;
using MathFunction = std::function<int(int, int)>;

void ascii_mappings(map<string, int> &ascii) {
  for (int i = 0; i < 10; i++) {
    ascii.insert({string(1, i + '0'), i + 48});
  }
  ascii.insert({"+", 43});
  ascii.insert({"-", 45});
  ascii.insert({"*", 42});
  ascii.insert({"/", 47});

  return;
}

void create_types(map<string, string> &type) {
  for (int i = 0; i < 10; i++) {
    type.insert({string(1, i + '0'), "number"});
  }
  type.insert({"+", "operator"});
  type.insert({"-", "operator"});
  type.insert({"*", "operator"});
  type.insert({"/", "operator"});
}

string int_to_string(int a) {
  string s = "";
  for (int i = 8; i >= 0; i--) {
    for (int j = 9; j >= 1; j--) {
      if (pow(10, i) * j <= a) {
        s += (j + '0');
        a -= pow(10, i) * j;
        break;
      }
      if (s.size() > 0 && j == 1) {
        s += '0';
      }
    }
  }
  return s;
}

void perform_operation(vector<pair<string, string>> &expression,
                       vector<string> &operands,
                       map<string, MathFunction> &funcMap) {
  vector<pair<string, string>> new_expression;
  for (int i = 0; i < expression.size() - 1; i++) {
    bool right_operation = false;
    for (int j = 0; j < operands.size(); j++) {
      if (expression[i].first == operands[j]) {
        right_operation = true;
        break;
      }
    }
    if (!right_operation) {
      new_expression.push_back(expression[i]);
      if (i == expression.size() - 2) {
        new_expression.push_back(expression.back());
      }
      continue;
    }

    int new_value = funcMap[expression[i].first](
        stoi(new_expression[new_expression.size() - 1].first),
        stoi(expression[i + 1].first));

    new_expression[new_expression.size() - 1] = {int_to_string(new_value),
                                                 "number"};
    i++;
  }
  expression.clear();
  expression = new_expression;
  return;
}

int main() {

  vector<string> lines;
  string line = "";

  while (getline(cin, line)) {
    lines.push_back(line);
  }

  map<string, int> ascii;
  ascii_mappings(ascii);

  map<string, string> type;
  create_types(type);

  for (auto &line : lines) {
    for (auto &chr : line) {
      if (ascii.count(string(1, chr)) == 0) {
        return 0;
      }
    }
  }

  map<string, function<int(int, int)>> funcMap = {
      {"+", [](int a, int b) { return a + b; }},
      {"-", [](int a, int b) { return a - b; }},
      {"*", [](int a, int b) { return a * b; }},
      {"/", [](int a, int b) { return a / b; }},
  };

  for (auto &line : lines) {
    vector<pair<string, string>> expression;
    string current_expression = "";
    string current_type = "";
    for (int i = 0; i < line.size(); i++) {
      current_expression += string(1, line[i]);

      if (type.count(current_expression) == 0) {
        cout << "-- PARSING ERROR" << '\n';
        cout << current_expression << '\n';
        cout << type[current_expression] << '\n';
        cout << "PARSING ERROR --" << '\n';
        return 0;
      }

      current_type = type[current_expression];

      if (current_type == "number" && i < line.size() - 1) {
        if (type[string(1, line[i + 1])] == "number") {
          continue;
        }
      }
      expression.push_back({current_expression, current_type});
      current_expression = "";
    }
    vector<string> current_operation = {"*", "/"};
    perform_operation(expression, current_operation, funcMap);
    current_operation = {"+", "-"};
    perform_operation(expression, current_operation, funcMap);
    cout << line << '=';
    cout << expression[0].first << '\n';
  }
}
