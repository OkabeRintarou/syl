#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <unordered_map>

#include "interpreter.h"
#include "jit_compiler_x64.h"

static void usage(char *argv[]) {
  fprintf(stderr, "usage: %s [--jit] <input-filename>\n", argv[0]);
  exit(1);
}

int main(int argc, char *argv[]) {
  const char *filename = nullptr;
  bool jit_opt = false;

  if (argc == 2) {
    filename = argv[1];
  } else if (argc == 3) {
    if (strcmp(argv[1], "--jit") != 0) {
      usage(argv);
    }
    jit_opt = true;
    filename = argv[2];
  } else {
    usage(argv);
  }

  FILE *fp;
  if ((fp = fopen(filename, "r")) == NULL) {
    fprintf(stderr, "Cannot open file %s\n", argv[1]);
    exit(1);
  }

  long fsz = 0;
  fseek(fp, 0, SEEK_END);
  fsz = ftell(fp);
  rewind(fp);
  char *code = new char[fsz + 1];
  if (fread(code, sizeof(char), fsz, fp) != fsz) {
    fprintf(stderr, "fread error\n");
    exit(1);
  }
  code[fsz] = 0;

  if (jit_opt) {
    JitCompiler().compile_and_run(code);
  } else {
    Interpreter::run(code);
  }

  delete[] code;
  return 0;
}