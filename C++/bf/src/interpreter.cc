#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

#include "interpreter.h"
#include "timer.h"

using namespace std;

const int kDefaultSize = 40960;

static unordered_map<unsigned long, unsigned long>
brackets_cache(const char *code) {
  unordered_map<unsigned long, unsigned long> cache;
  stack<unsigned long> stk;
  const char *pc = code;
  char ch;
  while ((ch = *pc)) {
    if (ch == '[') {
      stk.push(pc - code);
    } else if (ch == ']') {
      cache[stk.top()] = pc - code;
      cache[pc - code] = stk.top();
      stk.pop();
    }
    ++pc;
  }
  return cache;
}
static void run(const char *code,
                unordered_map<unsigned long, unsigned long> &cache) {
  vector<char> data(kDefaultSize);
  const char *pc = code;
  char *dc = &data[0];
  char ch;

  fill(data.begin(), data.end(), 0);

  while ((ch = *pc)) {
    switch (ch) {
    case '>':
      ++dc;
      break;
    case '<':
      --dc;
      break;
    case '+':
      ++*dc;
      break;
    case '-':
      --*dc;
      break;
    case '.':
      putchar(*dc);
      break;
    case ',':
      *dc = (char)getchar();
      break;
    case '[':
      if (!*dc) {
        pc = code + cache[pc - code];
      }
      break;
    case ']':
      if (*dc) {
        pc = code + cache[pc - code];
      }
      break;
    default:
      break;
    }
    ++pc;
  }
}

namespace Interpreter {

void run(const char *code) {
  unordered_map<unsigned long, unsigned long> cache = brackets_cache(code);
  Timer timer;
  timer.reset();
  ::run(code, cache);
  printf("interpreter cost %.2lfs\n", timer.elapsed());
}
}
