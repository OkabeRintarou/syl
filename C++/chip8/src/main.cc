#include <chip8.h>

#include <SDL2/SDL.h>

Chip8VM myChip8;

#define MODIFIER 10
#define SCREEN_WIDTH_PIXEL 64
#define SCREEN_HEIGHT_PIXEL 32

SDL_Window *gWindow = nullptr;
SDL_Renderer *gRender = nullptr;

bool Init() {
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    return false;
  }

  atexit(SDL_Quit);

  gWindow =
      SDL_CreateWindow("Chip8 Emulator", SDL_WINDOWPOS_UNDEFINED,
                       SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH_PIXEL * MODIFIER,
                       SCREEN_HEIGHT_PIXEL * MODIFIER, SDL_WINDOW_SHOWN);
  if (gWindow == nullptr) {
    return false;
  }

  gRender = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
  if (gRender == nullptr) {
    SDL_DestroyWindow(gWindow);
    gWindow = nullptr;
  }
  return true;
}

void FrameBegin() {
  SDL_SetRenderDrawColor(gRender, 0xff, 0xff, 0xff, 0xff);
  SDL_RenderClear(gRender);
}

void FrameEnd() { SDL_RenderPresent(gRender); }

void DrawAll(const Chip8VM &c8) {
  SDL_SetRenderDrawColor(gRender, 0x00, 0x00, 0x00, 0xff);

  SDL_Rect r{0};
  int c = 0;
  for (int y = 0; y < 32; y++) {
    for (int x = 0; x < 64; x++) {
      if (c8.gfx[64 * y + x]) {
        c++;
        r.x = MODIFIER * x;
        r.y = MODIFIER * y;
        r.w = r.h = MODIFIER;
        SDL_RenderFillRect(gRender, &r);
      }
    }
  }
}

void handleKeyDown(SDL_Keycode key) {
  switch (key) {
  case SDLK_ESCAPE:
    exit(0);
  case SDLK_1:
    myChip8.keys[0x1] = 1;
    break;
  case SDLK_2:
    myChip8.keys[0x2] = 1;
    break;
  case SDLK_3:
    myChip8.keys[0x3] = 1;
    break;
  case SDLK_4:
    myChip8.keys[0xc] = 1;
    break;
  case SDLK_q:
    myChip8.keys[0x4] = 1;
    break;
  case SDLK_w:
    myChip8.keys[0x5] = 1;
    break;
  case SDLK_e:
    myChip8.keys[0x6] = 1;
    break;
  case SDLK_r:
    myChip8.keys[0xd] = 1;
    break;
  case SDLK_a:
    myChip8.keys[0x7] = 1;
    break;
  case SDLK_s:
    myChip8.keys[0x8] = 1;
    break;
  case SDLK_d:
    myChip8.keys[0x9] = 1;
    break;
  case SDLK_f:
    myChip8.keys[0xe] = 1;
    break;
  case SDLK_z:
    myChip8.keys[0xa] = 1;
    break;
  case SDLK_x:
    myChip8.keys[0x0] = 1;
    break;
  case SDLK_c:
    myChip8.keys[0xb] = 1;
    break;
  case SDLK_v:
    myChip8.keys[0xf] = 1;
    break;
  }
}

void handleKeyUp(SDL_Keycode key) {
  switch (key) {
  case SDLK_1:
    myChip8.keys[0x1] = 0;
    break;
  case SDLK_2:
    myChip8.keys[0x2] = 0;
    break;
  case SDLK_3:
    myChip8.keys[0x3] = 0;
    break;
  case SDLK_4:
    myChip8.keys[0xc] = 0;
    break;
  case SDLK_q:
    myChip8.keys[0x4] = 0;
    break;
  case SDLK_w:
    myChip8.keys[0x5] = 0;
    break;
  case SDLK_e:
    myChip8.keys[0x6] = 0;
    break;
  case SDLK_r:
    myChip8.keys[0xd] = 0;
    break;
  case SDLK_a:
    myChip8.keys[0x7] = 0;
    break;
  case SDLK_s:
    myChip8.keys[0x8] = 0;
    break;
  case SDLK_d:
    myChip8.keys[0x9] = 0;
    break;
  case SDLK_f:
    myChip8.keys[0xe] = 0;
    break;
  case SDLK_z:
    myChip8.keys[0xa] = 0;
    break;
  case SDLK_x:
    myChip8.keys[0x0] = 0;
    break;
  case SDLK_c:
    myChip8.keys[0xb] = 0;
    break;
  case SDLK_v:
    myChip8.keys[0xf] = 0;
    break;
  }
}

int main(int argc, char *argv[]) {

  if (argc != 2) {
    fprintf(stderr, "Usage : %s <chip8-application>\n", argv[0]);
    exit(-1);
  }

  if (!myChip8.load(argv[1])) {
    return -1;
  }

  if (!Init()) {
    fprintf(stderr, "Init error: %s\n", SDL_GetError());
    exit(-1);
  }

  SDL_Event event;
  unsigned int uiCurrentTime = 0;
  unsigned int uiLastExecTime = 0;
  unsigned int uiNextFrameTime = SDL_GetTicks();
  bool quit = false;

  myChip8.execute();
  uiLastExecTime = SDL_GetTicks();

  for (;;) {
    do {
      uiCurrentTime = SDL_GetTicks();
      SDL_PollEvent(&event);
      switch (event.type) {
      case SDL_QUIT:
        quit = true;
        break;
      case SDL_KEYDOWN:
        handleKeyDown(event.key.keysym.sym);
        break;
      case SDL_KEYUP:
        handleKeyUp(event.key.keysym.sym);
        break;
      }

      if (quit) {
        break;
      }
      while (uiCurrentTime > uiLastExecTime) {
        myChip8.execute();
        uiLastExecTime = SDL_GetTicks();
      }

      SDL_Delay(1);
    } while (uiCurrentTime < uiNextFrameTime);

    if (quit)
      break;
    if (int(uiCurrentTime - uiNextFrameTime) > 1000) {
      uiNextFrameTime = uiCurrentTime + 1000 / 60;
    } else {
      uiNextFrameTime += 1000 / 60;
    }

    FrameBegin();
    DrawAll(myChip8);
    FrameEnd();
  }

  return 0;
}