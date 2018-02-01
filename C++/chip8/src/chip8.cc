#include <cstdlib>

#include <algorithm>
#include <functional>
#include <vector>

#include <chip8.h>

static unsigned char chip8_fontset[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

inline uint16_t right3(uint16_t op) { return op & (uint16_t)0x0fff; }
inline uint8_t right2(uint16_t op) { return (uint8_t)(op & (uint8_t)0x00ff); }
inline uint8_t right1(uint16_t op) { return (uint8_t)(op & (uint8_t)0x000f); }
inline uint8_t byte0(uint16_t op) { return (uint8_t)((op >> 12) & 0x0f); }
inline uint8_t byte1(uint16_t op) { return (uint8_t)((op >> 8) & 0x0f); }
inline uint8_t byte2(uint16_t op) { return (uint8_t)((op >> 4) & 0x0f); }

void Chip8VM::init() {
  pc = 0x200;
  opcode = 0;
  I = 0;
  sp = 0;

  // Clear display
  std::fill(std::begin(gfx), std::end(gfx), 0);

  // Clear stack
  std::fill(std::begin(stack), std::begin(stack), 0);

  // Clear memory
  std::fill(std::begin(memory), std::end(memory), 0);

  // Clear keys
  std::fill(std::begin(keys), std::end(keys), 0);

  std::copy(std::begin(chip8_fontset), std::end(chip8_fontset), memory + 0x50);

  waitKey = false;
  delay_timer = 0;
  sound_timer = 0;

  srand(unsigned(time(nullptr)));
}

bool Chip8VM::load(const char *filename) {
  init();

  printf("Loading: %s\n", filename);

  FILE *fp = fopen(filename, "rb");
  if (fp == nullptr) {
    fputs("File error", stderr);
    return false;
  }

  fseek(fp, 0, SEEK_END);
  long lSize = ftell(fp);
  rewind(fp);
  printf("File size: %d\n", (int)lSize);

  std::vector<char> buffer(lSize);
  size_t result = fread(&buffer[0], 1, lSize, fp);
  if (result != lSize) {
    fputs("Read error", stderr);
    return false;
  }

  if ((4096 - 512) > lSize) {
    for (int i = 0; i < lSize; i++) {
      memory[i + 0x200] = (uint8_t)buffer[i];
    }
  } else {
    printf("Error: ROM to big for memory");
  }

  fclose(fp);

  return true;
}

void Chip8VM::execute() {
  static std::function<void(uint16_t)> handlers[0x10] = {
      // 0
      [this](uint16_t op) {
        if (op == 0x00e0) {
          // Clear the screen
          std::fill(std::begin(gfx), std::end(gfx), 0);

        } else if (op == 0x00ee) {
          // Return from a subroutine
          sp -= 2;
          pc = (uint16_t)((stack[sp]) << 8) + stack[sp + 1];
        } else {
          // 0NNN: call program at address NNN
          stack[sp] = (uint8_t)((pc >> 8) & 0xff);
          stack[sp + 1] = (uint8_t)(pc & 0xff);
          sp += 2;
          pc = right3(op);
        }
      },

      // 1
      [this](uint16_t op) {
        // 1NNN: goto NNN
        pc = right3(op);
      },

      // 2
      [this](uint16_t op) {
        // 2NNN: call subroutine at NNN
        stack[sp] = (uint8_t)((pc >> 8) & 0xff);
        stack[sp + 1] = (uint8_t)(pc & 0xff);
        sp += 2;
        pc = right3(op);
      },

      // 3
      [this](uint16_t op) {
        // 3XNN: skip the next instruction if Vx == NN
        if (V[byte1(op)] == right2(op)) {
          pc += 2;
        }
      },

      // 4
      [this](uint16_t op) {
        // 4XNN: skip the next instruction if Vx != NN
        if (V[byte1(op)] != right2(op)) {
          pc += 2;
        }
      },

      // 5
      [this](uint16_t op) {
        // 5XY0: skip the next instruction if Vx == Vy
        if (right1(op) != 0) {
          fprintf(stderr, "Illegal instruction： %x\n", op);
          exit(-1);
        }
        if (V[byte1(op)] == V[byte2(op)]) {
          pc += 2;
        }
      },

      // 6
      [this](uint16_t op) {
        // 6XNN: Vx = NN
        V[byte1(op)] = right2(op);
      },

      // 7
      [this](uint16_t op) {
        // 7XNN: Vx += NN
        V[byte1(op)] += right2(op);
      },

      // 8
      [this](uint16_t op) {
        switch (right1(op)) {
        case 0:
          // 8XY0: Vx = Vy
          V[byte1(op)] = V[byte2(op)];
          break;
        case 1:
          // 8XY1: Vx = Vx | Vy
          V[byte1(op)] |= V[byte2(op)];
          break;
        case 2:
          // 8XY2: Vx = Vx & Vy
          V[byte1(op)] &= V[byte2(op)];

          break;
        case 3:
          // 8XY3: Vx = Vx ^ Vy
          V[byte1(op)] ^= V[byte2(op)];
          break;
        case 4:
          // 8XY4: Vx += Vy, VF is set to 1 if there's a carry otherwise is set
          // to 0
          if (V[byte2(op)] > 0xff - V[byte1(op)]) {
            V[0xf] = 1;
          } else {
            V[0xf] = 0;
          }
          V[byte1(op)] += V[byte2(op)];
          break;
        case 5:
          // 8XY5: Vx -= Vy, VF is set to 1 if there's a borrow otherwise is set
          // to 0
          if (V[byte1(op)] < V[byte2(op)]) {
            V[0xf] = 1;
          } else {
            V[0xf] = 0;
          }
          V[byte1(op)] -= V[byte2(op)];
          break;
        case 6:
          // 8XY6: Vx = Vy = Vy >> 1, VF is set to the least significant bit of
          // Vy before the shift
          // WARNING: on some mordern interpreters, Vx is shifted instead, while
          // Vy is ignored
          V[0xf] = V[byte1(op)] & (uint8_t)0x01;
          V[byte1(op)] >>= 1;
          break;
        case 7:
          // 8XY7: Vx = Vy - Vx, VF is set to 1 if there is a borrow otherwise
          // is set to 0

          if (V[byte2(op)] < V[byte1(op)]) {
            V[0xf] = 1;
          } else {
            V[0xf] = 0;
          }
          V[byte1(op)] = V[byte2(op)] - V[byte1(op)];
          break;
        case 0xe:
          // 8XYE: Vx = Vy = Vy << 1, VF is set to the most significant bit of
          // VF before the shift
          // WARNING: on some mordern interpreters, Vx is shifted instead, while
          // Vy is ignored
          V[0xf] = (V[byte1(op)] >> 7) & (uint8_t)0x01;
          V[byte1(op)] <<= 1;
          break;
        default:
          fprintf(stderr, "Illegal instruction: %x\n", op);
          exit(-1);
        }
      },

      // 9
      [this](uint16_t op) {
        // 9XY0: if (Vx != Vy) skip the next instruction
        if (right1(op) != 0) {
          fprintf(stderr, "Illegal instruction： %x\n", op);
          exit(-1);
        }
        if (V[byte1(op)] != V[byte2(op)]) {
          pc += 2;
        }
      },

      // A
      [this](uint16_t op) {
        // ANNN: I = NNN
        I = right3(op);
      },

      // B
      [this](uint16_t op) {
        // BNNN: PC = V0 + NNN
        pc = V[0] + right3(op);
      },

      // C
      [this](uint16_t op) {
        // CXNN: Vx = rand() & NN
        V[byte1(op)] = (uint8_t)((rand() % 256) & right2(op));
      },

      // D
      [this](uint16_t op) {
        // DXYN:
        uint8_t x = V[byte1(op)];
        uint8_t y = V[byte2(op)];
        uint8_t height = right1(op);
        uint8_t pixel;
        V[0xf] = 0;
        for (int dy = 0; dy < height; dy++) {
          pixel = memory[I + dy];
          for (int dx = 0; dx < 8; dx++) {
            if ((pixel & (0x80 >> dx)) != 0) {
              if (gfx[(y + dy) * 64 + (x + dx)] == 1) {
                V[0xf] = 1;
              }

              gfx[(y + dy) * 64 + (x + dx)] ^= 1;
            }
          }
        }
      },

      // E
      [this](uint16_t op) {
        uint8_t i = right2(op);
        if (i == 0x9E) {
          // EX9E: key[Vx] == 1
          if (keys[byte1(op)]) {
            pc += 2;
          }
        } else if (i == 0xA1) {
          // EXA1: keys[Vx] == 0
          if (!keys[byte1(op)]) {
            pc += 2;
          }
        } else {
          fprintf(stderr, "Illegal instruction： %x\n", op);
          exit(-1);
        }
      },

      // F
      [this](uint16_t op) {
        uint8_t i = byte2(op);
        switch (i) {
        case 0:
          if (right1(op) == 7) {
            // FX07: Vx = delay timer
            V[byte1(op)] = delay_timer;
          } else if (right1(op) == 0x0A) {
            // FX0A: Vx = get_key()
            bool keyPressed = false;
            for (int j = 0; j < 16; j++) {
              if (keys[j]) {
                V[byte1(op)] = (uint8_t)j;
                keyPressed = true;
                break;
              }
            }
            if (!keyPressed) {
              waitKey = true;
            }
          } else {
            fprintf(stderr, "Illegal instruction： %x\n", op);
            exit(-1);
          }
          break;
        case 1:
          if (right1(op) == 0x05) {
            // FX15: delay timer = Vx
            delay_timer = V[byte1(op)];
          } else if (right1(op) == 0x08) {
            // FX18: sound timer = Vx
            sound_timer = V[byte1(op)];
          } else if (right1(op) == 0x0E) {
            // FX1E: I += Vx
            I += V[byte1(op)];
          } else {
            fprintf(stderr, "Illegal instruction： %x\n", op);
            exit(-1);
          }
          break;
        case 2:
          if (right1(op) == 0x9) {
            // FX29: I = spride_addr[Vx]
            I = (uint8_t)(0x50 + V[byte1(op)] * 0x05);
          } else {
            fprintf(stderr, "Illegal instruction： %x\n", op);
            exit(-1);
          }
          break;
        case 3:
          if (right1(op) == 0x3) {
            // FX33: set_BCD(Vx)
            uint8_t Vx = V[byte1(op)];
            memory[I] = (uint8_t)(Vx / 100);
            memory[I + 1] = (uint8_t)((Vx / 10) % 10);
            memory[I + 2] = (uint8_t)(Vx % 10);
          } else {
            fprintf(stderr, "Illegal instruction： %x\n", op);
            exit(-1);
          }
          break;
        case 5:
          if (right1(op) == 0x5) {
            // FX55: Stores V0 to Vx(included) in memory starting at address I
            uint8_t x = byte1(op);
            for (int j = 0; j <= x; j++) {
              memory[I + j] = V[j];
            }
          } else {
            fprintf(stderr, "Illegal instruction： %x\n", op);
            exit(-1);
          }
          break;
        case 6:
          if (right1(op) == 0x5) {
            // FX65: Fills V0 to Vx(included) in memory starting at address I
            uint8_t x = byte1(op);
            for (int j = 0; j <= x; j++) {
              V[j] = memory[I + j];
            }
          } else {
            fprintf(stderr, "Illegal instruction： %x\n", op);
            exit(-1);
          }
          break;
        default:
          fprintf(stderr, "Illegal instruction： %x\n", op);
          exit(-1);
        }
      },
  };

  opcode = memory[pc] << 8 | memory[pc + 1];
  pc += 2;
  waitKey = false;
  handlers[byte0(opcode)](opcode);
  if (waitKey) {
    return;
  }
  if (delay_timer > 0) {
    delay_timer--;
  }
  if (sound_timer > 0) {
    if (sound_timer == 1) {
      printf("Beeping\n");
    }
    sound_timer--;
  }
}
