#ifndef JIT_COMPILER_X64_H_
#define JIT_COMPILER_X64_H_

#include <unordered_map>
#include <vector>

enum Reg { RAX, RBX, RCX, RDX, RDI, RBP, RSP };

enum Jump {
  JMP,
  JZ,
};

enum Panic {
  NotSupported = 0xcc,
  MemoryError = 0xee,
};

typedef void (*CallFunc)(const char *);

class JitCode {

public:
  JitCode();

  void emit_code(const std::vector<int> &codes);

  void emit_push_r(Reg reg);

  void emit_mov_rr(Reg src, Reg dst);

  void emit_mov_ir(uint64_t src, Reg dst);

  void emit_inc_r(Reg reg);

  void emit_dec_r(Reg reg);

  void emit_call_r(Reg reg);

  void emit_pop_r(Reg reg);

  void emit_ret();

  void fill_offset_jmp_address(uint64_t offset, int64_t displacement);

  uint64_t emit_jmp_with_patchback(Jump type);

  inline CallFunc function() const;
  inline unsigned long code_size() const;

  void print() const;

private:
  bool imm_is_i32(int64_t imm) const;
  bool imm_is_u32(uint64_t imm) const;

  void emit_uint32(uint32_t imm);
  void emit_uint64(uint64_t imm);

  void extend_buffer();
  void check_buffer(const std::vector<int> &new_codes);

private:
  char *machine_code;
  unsigned long size;
  unsigned long page_size;
  unsigned long buf_size;
};

class JitCompiler {
public:
  JitCompiler();
  ~JitCompiler();

  void emit_move_right();

  void emit_move_left();

  void emit_data_inc();

  void emit_data_dec();

  void emit_putchar();

  void emit_getchar();

  void compile_and_run(const char *code);

private:
  void emit_push_regs();

  void emit_pop_regs();

private:
  JitCode *jit_code;
};

#endif