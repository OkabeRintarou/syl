#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <stack>
#include <unordered_map>

using namespace std;

const int kDefaultSize = 40960;

unordered_map<unsigned long,unsigned long> brackets_cache(const char *code) {
  unordered_map<unsigned long,unsigned long > cache;
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

void run(const char *code,unordered_map<unsigned long,unsigned long> &cache) {
  vector<char> data(kDefaultSize);
  const char *pc = code;
  char *dc = &data[0];
  char ch;

  fill(data.begin(),data.end(),0);

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

int main(int argc, char *argv[]) {
  if (argc!=2) {
    fprintf(stderr, "Usage: %s <input>\n", argv[0]);
    exit(1);
  }
  FILE *fp;
  if ((fp = fopen(argv[1], "r"))==NULL) {
    fprintf(stderr, "Cannot open file %s\n", argv[1]);
    exit(1);
  }

  long fsz = 0;
  fseek(fp, 0, SEEK_END);
  fsz = ftell(fp);
  rewind(fp);
  char * code = new char[fsz + 1];
  if (fread(code,sizeof(char),fsz,fp) != fsz) {
    fprintf(stderr,"fread error\n");
    exit(1);
  }
  code[fsz] = 0;

  unordered_map<unsigned long,unsigned long> cache = brackets_cache(code);
  run(code,cache);

  delete[] code;
  return 0;
}
