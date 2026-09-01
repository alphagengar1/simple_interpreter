#include "./parser.h"
#include "ast.h"
#include "token.h"

std::unique_ptr<Node>
createExpression(std::vector<Token> &tokens,
                 std::list<std::list<TokenKind>>::iterator curr, int begin,
                 int end);

std::unique_ptr<Node> getValue(std::vector<Token> &tokens, int begin, int end);

int findLocation(std::vector<Token> &tokens,
                 std::list<std::list<TokenKind>>::iterator curr, int begin,
                 int end);

std::map<std::list<TokenKind>, int> Directionality = {
    {{TokenKind::Equals}, -1},
    {{TokenKind::Equality}, -1},
    {{TokenKind::Plus, TokenKind::Minus}, -1},
    {{TokenKind::Star, TokenKind::Slash}, -1},
};

std::list<std::list<TokenKind>> Order = {{TokenKind::Equals},
                                         {TokenKind::Equality},
                                         {TokenKind::Plus, TokenKind::Minus},
                                         {TokenKind::Star, TokenKind::Slash}};

enum StatementType { Expression, If, Function };

int findLocation(std::vector<Token> &tokens,
                 std::list<std::list<TokenKind>>::iterator curr, int begin,
                 int end) {
  int location = -1;
  int parenCount = 0;
  bool found = false;

  int step = Directionality.at(*curr);
  int start = Directionality.at(*curr) == 1 ? begin : end - 1;
  int stop = Directionality.at(*curr) == 1 ? end : begin - 1;

  TokenKind opening =
      Directionality.at(*curr) == 1 ? TokenKind::LParen : TokenKind::RParen;

  TokenKind closing =
      Directionality.at(*curr) == 1 ? TokenKind::RParen : TokenKind::LParen;

  for (int i = start; i != stop; i += step) {
    for (auto it : *curr) {
      if (tokens[i].kind == it) {
        location = i;
        found = true;
        break;
      }
    }
    if (found)
      break;

    if (tokens[i].kind == opening) {
      parenCount++;
      while (i != stop && parenCount != 0) {
        i += step;
        if (tokens[i].kind == opening)
          parenCount++;

        if (tokens[i].kind == closing)
          parenCount--;
      }
    }
  }
  return location;
}

std::unique_ptr<Node> getValue(std ::vector<Token> &tokens, int begin,
                               int end) {
  if (begin == end) {
    std::cout << "paren error begin == end lolz\n";
    return nullptr;
  }

  if (end - begin == 1 && tokens[begin].kind == TokenKind::Number) {
    std::unique_ptr<Node> num =
        std::make_unique<NumberNode>(tokens[begin].value);
    return num;
  }

  if (end - begin == 1 && tokens[begin].kind == TokenKind::Variable) {
    std::unique_ptr<Node> var =
        std::make_unique<VariableNode>(tokens[begin].name);
    return var;
  }

  if (tokens[begin].kind == TokenKind::LParen &&
      tokens[end - 1].kind == TokenKind::RParen) {
    return createExpression(tokens, Order.begin(), begin + 1, end - 1);
  }

  std::cout << "PAREN VALUE ERROR\n";
  for (int i = 0; i < (int)tokens.size(); i++) {
    std::cout << tokens[i] << ' ';
  }

  return nullptr;
};

std::unique_ptr<Node>
createExpression(std::vector<Token> &tokens,
                 std::list<std::list<TokenKind>>::iterator curr, int begin,
                 int end) {
  int location = -1;

  location = findLocation(tokens, curr, begin, end);

  if (location == -1) {
    if (++curr == Order.end()) {
      return getValue(tokens, begin, end);
    }
    return createExpression(tokens, curr, begin, end);
  }

  if ((*curr).front() == TokenKind::Equals) {
    std::unique_ptr<Node> value(
        createExpression(tokens, curr, location + 1, end));

    std::unique_ptr<Node> assignment = std::make_unique<AssignmentNode>(
        tokens[location - 1].name, std::move(value));

    return assignment;
  }
  if ((*curr).front() == TokenKind::Equality) {
    std::unique_ptr<Node> left(createExpression(tokens, curr, begin, location));
    std::unique_ptr<Node> right(
        createExpression(tokens, curr, location + 1, end));

    std::unique_ptr<Node> equality =
        std::make_unique<EqualityNode>(std::move(left), std::move(right));
    return equality;
  }

  std::unique_ptr<Node> left(createExpression(tokens, curr, begin, location));
  std::unique_ptr<Node> right(
      createExpression(tokens, curr, location + 1, end));

  std::unique_ptr<Node> root = std::make_unique<OpNode>(
      tokens[location].kind, std::move(left), std::move(right));
  return root;
}

std::unique_ptr<Node> createExpression(std::vector<Token> &tokens) {
  return createExpression(tokens, Order.begin(), 0, tokens.size());
}

std::unique_ptr<Node> createIf(std::vector<Token> &tokens) {

  // for (int i = 0; i < (int)tokens.size(); i++) {
  //   std::cout << tokens[i] << ' ';
  // }
  // std::cout << '\n';
  std::vector<std::pair<std::unique_ptr<Node>, Master>> conditionals;
  int idx = 0;

  while (idx < (int)tokens.size()) {

    std::vector<Token> condition;

    if (tokens[idx].kind == TokenKind::If) {
      idx++;
      if (tokens[idx].kind == TokenKind::Else) {
        idx++;
      }
    }

    if (tokens[idx].kind == TokenKind::Else) {
      idx++;
      condition = {{TokenKind::Number, 1, ""}};
    }

    while (tokens[idx].kind != TokenKind::LBrace) {
      condition.push_back(tokens[idx]);
      idx++;
    }

    std::vector<Token> statements;
    int openBrace = 1;
    idx++;
    while (openBrace != 0) {
      if (tokens[idx].kind == TokenKind::LBrace)
        openBrace++;

      if (tokens[idx].kind == TokenKind::RBrace) {
        openBrace--;
        if (openBrace == 0) {
          idx++;
          break;
        }
      }
      statements.push_back(tokens[idx]);
      idx++;
    }
    // std::cout << "bomb";
    // for (auto &i : statements) {
    //   std::cout << i << ' ';
    // }
    // std::cout << "\n" << idx << ' ' << tokens.size() << '\n';
    // return std::make_unique<NumberNode>(12);

    Master master;
    master.addTree(parseBlock(statements));

    std::pair<std::unique_ptr<Node>, Master> currentConditional = {
        createExpression(condition), std::move(master)};
    conditionals.push_back(std::move(currentConditional));
  }
  return std::make_unique<IfNode>(std::move(conditionals));
}

std::vector<std::unique_ptr<Node>> parseBlock(std::vector<Token> &tokens) {

  int idx = 0;

  std::vector<std::unique_ptr<Node>> nodes;

  while (idx < (int)tokens.size()) {

    std::vector<Token> currentTokens = {};
    StatementType statementType = StatementType::Expression;

    if (tokens[idx].kind == TokenKind::If) {
      statementType = StatementType::If;
      std::vector<Token> ifTokens;
      int openBraces = 0;
      bool openBrace = true;

      while ((openBraces || openBrace) && idx < (int)tokens.size()) {
        if (tokens[idx].kind == TokenKind::LBrace) {
          openBraces++;
        }
        if (tokens[idx].kind == TokenKind::RBrace) {
          openBraces--;
          openBrace = false;
        }
        ifTokens.push_back(tokens[idx]);
        idx++;
        if (tokens[idx].kind == TokenKind::Else ||
            tokens[idx].kind == TokenKind::If)
          openBrace = true;
      }
      nodes.push_back(createIf(ifTokens));
      continue;
    }

    for (; idx < (int)tokens.size(); idx++) {
      if (tokens[idx].kind == TokenKind::Semicolon) {
        idx++;
        break;
      }
      currentTokens.push_back(tokens[idx]);
    }
    if (statementType == StatementType::Expression) {
      nodes.push_back(createExpression(currentTokens));
    }
  }
  return nodes;
}

// std::vector<std::unique_ptr<Node>> parseBlock(std::vector<Token> &tokens) {
//   return parseBlock(tokens, 0, tokens.size());
// }
