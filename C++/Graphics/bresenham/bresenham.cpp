#include <SDL2/SDL.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

SDL_Window* gWindow = NULL;
SDL_Renderer* gRender = NULL;

bool Init()
{
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        return false;
    }

    atexit(SDL_Quit);

    gWindow = SDL_CreateWindow("bresenham",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,
                              SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
    if(gWindow == NULL) {
        return false;
    }

    gRender = SDL_CreateRenderer(gWindow,-1,SDL_RENDERER_ACCELERATED);
    if(gRender == NULL) {
        SDL_DestroyWindow(gWindow);
        gWindow = NULL;
    }
    return true;
}

void FrameBegin()
{
    SDL_SetRenderDrawColor(gRender,0xff,0xff,0xff,0xff);
    SDL_RenderClear(gRender);
}

void FrameEnd()
{
    SDL_RenderPresent(gRender);
}

void DrawLine(SDL_Renderer* render,int x1,int y1,int x2,int y2)
{
    int dx = x2 - x1,dy = y2 - y1,x = x1,y = y1;
    int ux = dx > 0 ? 1 : -1;
    int uy = dy > 0 ? 1 : -1;
    int eps = 0;

    dx = abs(dx);dy = abs(dy);

    if(dy < dx) {
        for(x = x1;x != x2;x += ux) {
            SDL_RenderDrawPoint(render,x,y);
            eps += dy;
            if((eps << 1) >= dx) {
                y += uy;
                eps -= dx;
            }
        }
    } else {
        for(y = y1;y != y2;y += uy) {
            SDL_RenderDrawPoint(render,x,y);
            eps += dx;
            if((eps << 1) >= dy) {
                x += ux;
                eps -= dy;
            }
        }
    }
    SDL_RenderDrawPoint(render,x2,y2);
}

void DrawAll()
{
    SDL_SetRenderDrawColor(gRender,0xff,0x00,0x00,0xff);

    DrawLine(gRender,320,240,640,240);
    DrawLine(gRender,320,240,640,360);
    DrawLine(gRender,320,240,640,480);
    DrawLine(gRender,320,240,480,480);
    DrawLine(gRender,320,240,320,480);
    DrawLine(gRender,320,480,320,240);
    DrawLine(gRender,160,480,320,240);
    DrawLine(gRender,0,480,320,240);
    DrawLine(gRender,0,360,320,240);
    DrawLine(gRender,0,240,320,240);
    DrawLine(gRender,0,120,320,240);
    DrawLine(gRender,0,0,320,240);
    DrawLine(gRender,160,0,320,240);
    DrawLine(gRender,320,0,320,240);
    DrawLine(gRender,480,0,320,240);
    DrawLine(gRender,640,0,320,240);
    DrawLine(gRender,640,120,320,240);
    DrawLine(gRender,640,240,320,240);
}

int main()
{
    if(!Init()) {
        fprintf(stderr,"SDL init error:%s\n",SDL_GetError());
        return -1;
    }
    SDL_Event event;
    unsigned int uiCurrentTime = 0;
    unsigned int uiNextFrameTime = SDL_GetTicks();
    bool quit = false;

    for(;;) {
        do {
            uiCurrentTime = SDL_GetTicks();
            SDL_PollEvent(&event);
            if(event.type == SDL_QUIT) {
                quit = true;
                break;
            }
            SDL_Delay(1);
        }while(uiCurrentTime < uiNextFrameTime);

        if(quit)break;
        if(int(uiCurrentTime - uiNextFrameTime) > 1000) {
            uiNextFrameTime = uiCurrentTime + 1000 / 60;
        } else {
            uiNextFrameTime += 1000 / 60;
        }

        FrameBegin();
        DrawAll();
        FrameEnd();
    }

    return 0;
}
