#ifndef CHIP8_CHIP8_H_
#define CHIP8_CHIP8_H_

#include <cstdint>

class Chip8VM {
public:
  uint8_t gfx[64 * 32];
  uint8_t keys[16];

  bool load(const char *filename);
  void execute();

private:
  void init();

private:
  uint8_t memory[4096];
  uint8_t V[16];

  uint16_t opcode;
  uint16_t I;
  uint16_t pc;

  uint8_t stack[48];
  uint8_t sp;

  bool waitKey;

  uint8_t delay_timer;
  uint8_t sound_timer;
};

#endif