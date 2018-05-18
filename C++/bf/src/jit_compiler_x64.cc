#include <climits>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <sys/mman.h>
#include <unistd.h>

#include <stack>
#include <vector>

#include "jit_compiler_x64.h"
#include "timer.h"

using namespace std;

static void sys_err(const char *msg) {
  perror(msg);
  exit(errno);
}

static void sys_not_supported() {
  fprintf(stderr, "not supported instruction");
  exit(NotSupported);
}

static void sys_memory_error() {
  perror("memory error");
  exit(MemoryError);
}

JitCode::JitCode() {
  machine_code = nullptr;
  size = 0;
  buf_size = 0;
  page_size = (unsigned long)getpagesize();

  extend_buffer();
}

void JitCode::extend_buffer() {
  unsigned long new_size = buf_size;
  if (!new_size) {
    new_size = page_size;
  } else {
    new_size *= 2;
  }

  void *ptr;
  if (posix_memalign(&ptr, page_size, new_size)) {
    sys_memory_error();
  }

  if (mprotect(ptr, new_size, PROT_READ | PROT_WRITE)) {
    sys_memory_error();
  }

  if (machine_code) {
    memcpy(ptr, machine_code, size);
    free(machine_code);
  }
  buf_size = new_size;

  machine_code = (char *)ptr;
}

void JitCode::check_buffer(const std::vector<int> &new_codes) {
  if (size + new_codes.size() > buf_size) {
    extend_buffer();
  }
}

void JitCode::emit_code(const vector<int> &codes) {
  check_buffer(codes);

  for (int c : codes) {
    machine_code[size++] = (char)c;
  }
}

void JitCode::emit_push_r(Reg reg) {
  switch (reg) {
  case RBP:
    emit_code({0x55});
    break;
  case RDX:
    emit_code({0x52});
    break;
  case RBX:
    emit_code({0x53});
    break;
  default:
    sys_not_supported();
    break;
  }
}

void JitCode::emit_pop_r(Reg reg) {
  switch (reg) {
  case RBP:
    emit_code({0x5d});
    break;
  case RDX:
    emit_code({0x5a});
    break;
  case RBX:
    emit_code({0x5b});
    break;
  default:
    sys_not_supported();
    break;
  }
}

void JitCode::emit_inc_r(Reg reg) {
  switch (reg) {
  case RBX:
    emit_code({0x48, 0xff, 0xc3});
    break;
  case RCX:
    emit_code({0x48, 0xff, 0xc1});
    break;
  default:
    sys_not_supported();
  }
}

void JitCode::emit_dec_r(Reg reg) {
  switch (reg) {
  case RBX:
    emit_code({0x48, 0xff, 0xcb});
    break;
  case RCX:
    emit_code({0x48, 0xff, 0xc9});
    break;
  default:
    sys_not_supported();
  }
}

void JitCode::emit_call_r(Reg reg) {
  switch (reg) {
  case RBX:
    emit_code({0xff, 0xd3});
    break;
  case RCX:
    emit_code({0xff, 0xd1});
    break;
  default:
    sys_not_supported();
  }
}

void JitCode::emit_ret() { emit_code({0xc3}); }

uint64_t JitCode::emit_jmp_with_patchback(Jump type) {
  if (type == JMP) {
    emit_code({0xe9});
  } else if (type == JZ) {
    emit_code({0x0f, 0x84});
  } else {
    sys_not_supported();
  }

  uint64_t patch_addr = size;
  emit_code({0xaa, 0xbb, 0xcc, 0xdd});
  return patch_addr;
}

void JitCode::emit_mov_ir(uint64_t src, Reg dst) {
  if (imm_is_u32(src)) {
    switch (dst) {
    case RAX:
      emit_code({0x48, 0xc7, 0xc0});
      break;
    case RBX:
      emit_code({0x48, 0xc7, 0xc3});
      break;
    case RCX:
      emit_code({0x48, 0xc7, 0xc1});
      break;
    case RDI:
      emit_code({0x48, 0xc7, 0xc7});
      break;
    default:
      sys_not_supported();
      break;
    }

    emit_uint32((uint32_t)src);
  } else {
    switch (dst) {
    case RAX:
      emit_code({0x48, 0xb8});
      break;
    case RBX:
      emit_code({0x48, 0xbb});
      break;
    case RCX:
      emit_code({0x48, 0xb9});
      break;
    case RDI:
      emit_code({0x48, 0xbf});
      break;
    default:
      sys_not_supported();
      break;
    }
    emit_uint64(src);
  }
}

void JitCode::emit_mov_rr(Reg src, Reg dst) {
  if (src == RBP && dst == RSP) {
    emit_code({0x48, 0x89, 0xec});
  } else if (src == RSP && dst == RBP) {
    emit_code({0x48, 0x89, 0xe5});
  } else if (src == RDI && dst == RDX) {
    emit_code({0x48, 0x89, 0xfa});
  } else if (src == RAX && dst == RDI) {
    emit_code({0x48, 0x89, 0xc7});
  } else if (src == RBX && dst == RDI) {
    emit_code({0x48, 0x89, 0xdf});
  } else if (src == RCX && dst == RDI) {
    emit_code({0x48, 0x89, 0xcf});
  } else if (src == RDX && dst == RDI) {
    emit_code({0x48, 0x89, 0xd7});
  } else {
    sys_not_supported();
  }
}

bool JitCode::imm_is_u32(uint64_t imm) const {
  return imm <= (uint64_t)UINT32_MAX;
}

bool JitCode::imm_is_i32(int64_t imm) const {
  return imm >= INT32_MIN && imm <= INT32_MAX;
}

void JitCode::emit_uint32(uint32_t imm) {
  vector<int> vec(4);
  for (int i = 0; i < 4; i++) {
    vec[i] = (uint8_t)(imm);
    imm >>= 8;
  }
  emit_code(vec);
}

void JitCode::emit_uint64(uint64_t imm) {
  vector<int> vec(8);
  for (int i = 0; i < 8; i++) {
    vec[i] = (uint8_t)(imm);
    imm >>= 8;
  }
  emit_code(vec);
}

CallFunc JitCode::function() const {
  if (mprotect(machine_code, buf_size, PROT_READ | PROT_EXEC)) {
    sys_memory_error();
  }
  return (CallFunc)machine_code;
}

unsigned long JitCode::code_size() const { return size; }

void JitCode::fill_offset_jmp_address(uint64_t offset, int64_t displacement) {
  if (!imm_is_i32(displacement)) {
    fprintf(stderr, "jmp too far");
    exit(1);
  }
  int32_t dis = (int32_t)displacement;
  for (int i = 0; i < 4; i++) {
    machine_code[offset + i] = (int8_t)dis;

    dis >>= 8;
  }
}

void JitCode::print() const {
  uint8_t v;
  for (auto i = 0; i < size; i++) {
    v = (uint8_t)machine_code[i];
    if (v < 10) {
      printf("0%x ", v);
    } else {
      printf("%x ", v);
    }
  }
  printf("\n");
  fflush(stdout);
}

JitCompiler::JitCompiler() { jit_code = new JitCode(); }

JitCompiler::~JitCompiler() { delete jit_code; }

void JitCompiler::emit_push_regs() {
  jit_code->emit_push_r(RBX);
  jit_code->emit_push_r(RDX);
}

void JitCompiler::emit_pop_regs() {
  jit_code->emit_pop_r(RDX);
  jit_code->emit_pop_r(RBX);
}

void JitCompiler::emit_move_right() { jit_code->emit_inc_r(RBX); }

void JitCompiler::emit_move_left() { jit_code->emit_dec_r(RBX); }

void JitCompiler::emit_data_inc() {
  jit_code->emit_code({0x8a, 0x0c, 0x1a}); // mov (%rdx,%rbx),%cl
  jit_code->emit_inc_r(RCX);
  jit_code->emit_code({0x88, 0x0c, 0x1a}); // mov %cl,(%rdx,%rbx)
}

void JitCompiler::emit_data_dec() {
  jit_code->emit_code({0x8a, 0x0c, 0x1a}); // mov (%rdx,%rbx),%cl
  jit_code->emit_dec_r(RCX);
  jit_code->emit_code({0x88, 0x0c, 0x1a}); // mov %cl,(%rdx,%rbx)
}

void JitCompiler::emit_putchar() {

  emit_push_regs();

  jit_code->emit_code({0x31, 0xc9});             // xor %rcx,%rcx
  jit_code->emit_code({0x8a, 0x0c, 0x1a});       // mov (%rdx,%rbx),%cl
  jit_code->emit_mov_rr(RCX, RDI);               // mov %rcx,%rdi
  jit_code->emit_mov_ir((uint64_t)putchar, RCX); // mov $putchar,%rcx
  jit_code->emit_call_r(RCX);                    // call %rcx

  emit_pop_regs();
}

void JitCompiler::emit_getchar() {
  emit_push_regs();

  jit_code->emit_mov_ir((uint64_t)getchar, RCX);
  jit_code->emit_call_r(RCX);

  emit_pop_regs();

  jit_code->emit_code({0x88, 0x04, 0x1a}); // mov %al,(%rdx,%rbx)
}

void JitCompiler::compile_and_run(const char *code) {
  Timer timer;
  timer.reset();

  jit_code->emit_push_r(RBP);      // push %rbp
  jit_code->emit_mov_rr(RSP, RBP); // mov %rsp,%rbp

  emit_push_regs();

  jit_code->emit_mov_ir(0, RBX);   // RBX = data pointer
  jit_code->emit_mov_rr(RDI, RDX); // RDX = memory base

  const char *pc = code;
  int ch;
  uint64_t start_addr, patch_addr, patch_addr2, cur_addr;
  stack<pair<uint64_t, uint64_t>> stk;
  pair<uint64_t, uint64_t> p;

  while ((ch = *pc)) {
    switch (ch) {
    case '>':
      emit_move_right();
      break;
    case '<':
      emit_move_left();
      break;
    case '+':
      emit_data_inc();
      break;
    case '-':
      emit_data_dec();
      break;
    case '.':
      emit_putchar();
      break;
    case ',':
      emit_getchar();
      break;
    case '[':

      start_addr = jit_code->code_size();
      if (start_addr == 0) {
        break; // skip comment
      }

      jit_code->emit_code({0x48, 0x31, 0xc9});            // xor %rcx,%rcx
      jit_code->emit_code({0x8a, 0x0c, 0x1a});            // mov (%rdx,%rbx),%cl
      jit_code->emit_code({0x84, 0xc9});                  // test %cl,%cl
      patch_addr = jit_code->emit_jmp_with_patchback(JZ); // jz
      stk.push(make_pair(start_addr, patch_addr));
      break;
    case ']':
      p = stk.top();
      stk.pop();
      patch_addr2 = jit_code->emit_jmp_with_patchback(JMP);
      cur_addr = jit_code->code_size();
      jit_code->fill_offset_jmp_address(p.second, cur_addr - (p.second + 4));
      jit_code->fill_offset_jmp_address(patch_addr2,
                                        (int64_t)p.first - (int64_t)cur_addr);
      break;
    default:
      break;
    }
    ++pc;
  }

  emit_pop_regs();
  jit_code->emit_mov_rr(RBP, RSP);
  jit_code->emit_pop_r(RBP);
  jit_code->emit_ret();

  printf("compile code cost %.2lfs\n", timer.elapsed());
  printf("compiled code bytes: %ld\n", jit_code->code_size());

  char *data = (char *)malloc(sizeof(char) * 40960);
  memset(data, 0, 40960);
  timer.reset();
  jit_code->function()(data);
  printf("run compiled code cost %.2lf\n", timer.elapsed());
  free(data);
}
