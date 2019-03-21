#include <cstdio>
#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <unordered_map>


using namespace std;

struct Value {
  Value(int n) : num(n), is_op(false) {}

  Value(char o) : op(o), is_op(true) {}

  union {
    int num;
    char op;
  };
  bool is_op;
};

bool inf2post(const string &str, vector<Value> &vec) {
  const char *p = str.c_str();
  char ch;
  int num = 0;
  bool init = false;
  stack<char> stk;

  unordered_map<char, int> pre = {
    {'+', 0},
    {'-', 0},
    {'*', 1},
    {'/', 1},
  };

  auto is_op = [](char ch) { return ch == '+' || ch == '-' || ch == '*' || ch == '/'; };


  while ((ch = *p++)) {
    if (std::isspace(ch)) {
      continue;
    }

    if (std::isdigit(ch)) {
      init = true;
      num = num * 10 + (ch - '0');
      continue;
    }

    if (init) {
      vec.emplace_back(Value{num});
      init = false;
      num = 0;
    }

    if ('(' == ch) {
      stk.push(ch);
    } else if (')' == ch) {
      char c;
      while ((c = stk.top()) != '(') {
        vec.emplace_back(Value{c});
        stk.pop();
      }
      if (stk.empty()) {
        return false;
      } else {
        stk.pop();
      }
    } else if (!is_op(ch)) {
      return false;
    } else {
      while (!stk.empty() && is_op(stk.top()) && pre[ch] <= pre[stk.top()]) {
        vec.emplace_back(Value{stk.top()});
        stk.pop();
      }
      stk.push(ch);
    }
  }

  if (init) {
    vec.emplace_back(Value{num});
  }

  while (!stk.empty()) {
    if ('(' == stk.top()) {
      return false;
    }
    vec.emplace_back(Value{stk.top()});
    stk.pop();
  }
  return true;
}

bool eval(const vector<Value> &vec, int &result) {
  result = 0;
  stack<int> stk;

  auto calc = [](char op, int lhs, int rhs) {
    switch (op) {
      case '+':
        return lhs + rhs;
      case '-':
        return lhs - rhs;
      case '*':
        return lhs * rhs;
      case '/':
        return lhs / rhs;
      default:
        assert(0);
    }
  };

  for (auto i = 0; i < vec.size(); i++) {
    if (vec[i].is_op) {
      if (stk.size() < 2) {
        return false;
      }
      int rhs = stk.top(); stk.pop();
      int lhs = stk.top(); stk.pop();
      result = calc(vec[i].op, lhs, rhs);
      stk.push(result);
    } else {
      stk.push(vec[i].num);
    }
  }

  if (stk.size() != 1) {
    return false;
  }
  result = stk.top();
  return true;
}

int eval(const string &input) {
  vector<Value> vec;
  if (!inf2post(input, vec)) {
    return -1;
  } else {
    int result;
    if (eval(vec, result)) {
      return result;
    } else {
      return -1;
    }
  }
}

int main() {

  assert(eval("1+2*3/2") == 4);
  assert(eval("(1+2-9)-(1*3-2)") == -7);
  assert(eval("1+2*(3+4)") == 15);
  return 0;
}