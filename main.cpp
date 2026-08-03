#include <functional>
#include <iostream>
#include <map>
#include <math.h>
#include <stack>
#include <string>
#include <vector>
using namespace std;
using MathFunction = std::function<int(int, int)>;

map<string, int> ascii;
vector<string> lines;
map<string, string> type;

vector<pair<string, string>>
evaluate_expression(vector<pair<string, string>> &expression,
                    vector<string> &operands,
                    map<string, MathFunction> &funcMap);

vector<pair<string, string>>
tokenizer(string line, map<string, string> &type,
          map<string, function<int(int, int)>> &funcMap);

void ascii_mappings(map<string, int> &ascii) {
  for (int i = 0; i < 10; i++) {
    ascii.insert({string(1, i + '0'), i + 48});
  }
  ascii.insert({"+", 43});
  ascii.insert({"-", 45});
  ascii.insert({"*", 42});
  ascii.insert({"/", 47});

  ascii.insert({"(", 47});
  ascii.insert({")", 47});

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

  type.insert({"(", "parenthesis_open"});
  type.insert({")", "parenthesis_close"});
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

vector<pair<string, string>>
evaluate_expression(vector<pair<string, string>> &expression,
                    vector<string> &operands,
                    map<string, MathFunction> &funcMap) {

  if (expression.size() == 1) {
    return expression;
  }

  vector<pair<string, string>> new_expression;
  for (int i = 0; i < expression.size() - 1; i++) {

    bool wanted_operation = false;
    for (int j = 0; j < operands.size(); j++) {
      if (expression[i].first == operands[j]) {
        wanted_operation = true;
        break;
      }
    }
    if (!wanted_operation) {
      new_expression.push_back(expression[i]);
      if (i == expression.size() - 2) {
        new_expression.push_back(expression.back());
      }
      continue;
    }

    pair<string, string> left_input = new_expression[new_expression.size() - 1];
    pair<string, string> right_input = expression[i + 1];

    vector<pair<string, string>> evaluated_left_input = {left_input};
    vector<pair<string, string>> evaluated_right_input = {right_input};

    if (left_input.second == "parenthetical") {
      string unevaluated_left_input = "";
      for (int i = 1; i < left_input.first.size() - 1; i++) {
        unevaluated_left_input += left_input.first[i];
      }
      vector<pair<string, string>> tokenized_left_input =
          tokenizer(unevaluated_left_input, type, funcMap);

      vector<string> current_operation = {"*", "/"};
      evaluated_left_input =
          evaluate_expression(tokenized_left_input, current_operation, funcMap);

      current_operation = {"+", "-"};
      evaluated_left_input =
          evaluate_expression(tokenized_left_input, current_operation, funcMap);
    }

    if (right_input.second == "parenthetical") {
      string unevaluated_right_input = "";
      for (int i = 1; i < right_input.first.size() - 1; i++) {
        unevaluated_right_input += right_input.first[i];
      }
      vector<pair<string, string>> tokenized_right_input =
          tokenizer(unevaluated_right_input, type, funcMap);

      vector<string> current_operation = {"*", "/"};
      evaluated_right_input = evaluate_expression(tokenized_right_input,
                                                  current_operation, funcMap);

      current_operation = {"+", "-"};
      evaluated_right_input = evaluate_expression(tokenized_right_input,
                                                  current_operation, funcMap);
    }

    int new_value =
        funcMap[expression[i].first](stoi(evaluated_left_input[0].first),
                                     stoi(evaluated_right_input[0].first));

    new_expression[new_expression.size() - 1] = {int_to_string(new_value),
                                                 "number"};
    i++;
  }
  return new_expression;
}

vector<pair<string, string>>
tokenizer(string line, map<string, string> &type,
          map<string, function<int(int, int)>> &funcMap) {

  vector<pair<string, string>> expression;
  string current_expression = "";
  string current_type = "";
  stack<char> open_parenthesis;

  for (int i = 0; i < line.size(); i++) {
    current_expression += string(1, line[i]);

    if (type.count(string(1, current_expression[0])) == 0) {
      cout << "-- PARSING ERROR" << '\n';
      cout << current_expression << '\n';
      cout << type[current_expression] << '\n';
      cout << "PARSING ERROR --" << '\n';
    }

    current_type = type[current_expression];

    if (current_type == "parenthesis_open" && open_parenthesis.size() == 0) {
      open_parenthesis.push('(');
      while (open_parenthesis.size() > 0) {
        i++;
        if (i == line.size()) {
          cout << "-- PARENTHESIS PARSING ERROR" << '\n';
          cout << current_expression << '\n';
          cout << type[current_expression] << '\n';
          cout << "PARENTHESIS PARSING ERROR --" << '\n';
          return expression;
        }
        if (type[string(1, line[i])] == "parenthesis_close") {
          open_parenthesis.pop();
        }
        if (type[string(1, line[i])] == "parenthesis_open") {
          open_parenthesis.push('(');
        }
        current_expression += string(1, line[i]);
      }
      current_type = "parenthetical";
    } else if (current_type == "number" && i < line.size() - 1) {
      if (type[string(1, line[i + 1])] == "number") {
        continue;
      }
    }
    expression.push_back({current_expression, current_type});
    current_expression = "";
  }
  return expression;
}

int main() {

  string line = "";

  while (getline(cin, line)) {
    lines.push_back(line);
  }

  ascii_mappings(ascii);

  create_types(type);

  for (auto &line : lines) {
    for (auto &chr : line) {
      if (ascii.count(string(1, chr)) == 0) {
        cout << "bad characters";
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

  for (string &line : lines) {

    vector<pair<string, string>> tokens = tokenizer(line, type, funcMap);

    vector<string> current_operation = {"*", "/"};
    tokens = evaluate_expression(tokens, current_operation, funcMap);

    current_operation = {"+", "-"};
    tokens = evaluate_expression(tokens, current_operation, funcMap);

    cout << line << '=';
    cout << tokens[0].first << '\n';
  }
}
