#include <algorithm>
#include <SDL2/SDL.h>
#include "tetris.h"

using namespace std;

const int kBlockSize = 35;
const int kBlockHNum = 12;
const int kBlockVNum = 21;


Tetris::Tetris(int width,int height)
{
    quit = false;
    coll = false;
    this->width = width;
    this->height = height;
    hNum = height / kBlockSize;
    vNum = width / kBlockSize;
    matrix.resize(hNum);
    for(int i = 0;i < (int)matrix.size();i++) {
        matrix[i].resize(vNum);
        fill(matrix[i].begin(),matrix[i].end(),0);
    }
    downSpeed = 2;
    genBuildingBlock();

    window = NULL;
    render = NULL;

    window = SDL_CreateWindow("tetris",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,
                              width,height,SDL_WINDOW_SHOWN);
    if(window == NULL) {
        return;
    }

    render = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
    if(render == NULL) {
        SDL_DestroyWindow(window);
        window = NULL;
    }
};

Tetris::~Tetris()
{
    if(render) {
        SDL_DestroyRenderer(render);
        render = NULL;
    }
    if(window) {
        SDL_DestroyWindow(window);
        window = NULL;
    }
}

bool Tetris::valid() const
{
    return window && render;
}

void Tetris::run()
{
    unsigned int uiNextFrameTime = SDL_GetTicks();
    unsigned int uiCurrentTime = 0;

    for(;;) {
        do {
            uiCurrentTime = SDL_GetTicks();
            updateEvent();
            if(quit) {
                goto exit_;
            }
            SDL_Delay(1);
        }while(uiCurrentTime < uiNextFrameTime);

        if(int(uiCurrentTime  - uiNextFrameTime) > 1000) {
            uiNextFrameTime = uiCurrentTime + 1000 / 60;
        } else {
            uiNextFrameTime += 1000 / 60;
        }


        SDL_SetRenderDrawColor(render,0xff,0xff,0xff,0xff);
        SDL_RenderClear(render);
        drawAll();
        SDL_RenderPresent(render);
    }

    exit_:
    return;
}


void Tetris::drawAll()
{
    static unsigned int fadingStartTime = 0;
    if(fadingStartTime == 0) {
        fadingStartTime = SDL_GetTicks();
    }

    // draw line
    SDL_SetRenderDrawColor(render,0x00,0x00,0x00,0xff);

    for(int i = 1;i < hNum;i++) {
        SDL_RenderDrawLine(render,0,i * kBlockSize,width,i * kBlockSize);
    }
    for(int i = 1;i < vNum;i++) {
        SDL_RenderDrawLine(render,i * kBlockSize,0,i * kBlockSize,height);
    }

    if(coll || collision()) {
         for(int i = 0;i < 4;i++) {
            matrix[int(yPos + deltaY[i])][int(xPos + deltaX[i])] = 1;
        }
        genBuildingBlock();
        coll = false;
    } else {
        float originX = xPos,originY = yPos;

        yPos += downSpeed * (SDL_GetTicks() - fadingStartTime) / 1000.0;

        for(int i = 0;i < 4;i++) {
            if(matrix[yPos + deltaY[i]][xPos + deltaX[i]] == 1) {
                xPos = originX;
                yPos = originY;
                for(int j = 0;j < 4;j++) {
                    matrix[int(yPos + deltaY[j])][int(xPos + deltaX[j])] = 1;
                }
                genBuildingBlock();
                break;
            }
        }
    }

    for(int i = 0;i < hNum;i++) {
        bool allOne = true;
        for(int j = 0;j < vNum;j++) {
            if(matrix[i][j] == 0) {
                allOne = false;
            }
        }
        if(allOne) {
             decline(i);
        }
    }



    // draw blocks
    SDL_SetRenderDrawColor(render,0x00,0x00,0xff,0xff);
    for(int i = 0;i < hNum;i++) {
        for(int j = 0;j < vNum;j++) {
            if(matrix[i][j]) {
                SDL_Rect rect{j * kBlockSize,i * kBlockSize,kBlockSize,kBlockSize};
                 SDL_RenderFillRect(render,&rect);
            }
        }
    }

    for(int i = 0;i < 4;i++) {
        SDL_Rect rect {((int)xPos + deltaX[i]) * kBlockSize,
                       ((int)yPos + deltaY[i]) * kBlockSize,
                       kBlockSize,
                       kBlockSize
        };

        SDL_RenderFillRect(render,&rect);
    }


    fadingStartTime = SDL_GetTicks();
}

void Tetris::updateEvent()
{
    SDL_Event event;
    SDL_PollEvent(&event);

    int originX = xPos,originY = yPos;
    bool yChanged = false;
    bool changeX = true;

    switch(event.type) {
        case SDL_QUIT:
            quit = true;
            break;
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym) {
                case SDLK_SPACE:
                    downSpeed = 8;
                    break;
                case SDLK_LEFT:
                    for(int i = 0;i < 4;i++) {
                        if(matrix[yPos + deltaY[i]][xPos - 1 + deltaX[i]] == 1) {
                            changeX = false;
                            break;
                        }
                    }
                    if(changeX) {
                        xPos -= 1;
                        fixPos();
                    }
                    break;
                case SDLK_RIGHT:
                    for(int i = 0;i < 4;i++) {
                        if(matrix[yPos + deltaY[i]][xPos + 1 + deltaX[i]] == 1) {
                            changeX = false;
                            break;
                        }
                    }
                    if(changeX) {
                        xPos += 1;
                        fixPos();
                    }

                    break;
                case SDLK_DOWN:
                    clockwiseRotate();
                    fixPos();
                    yChanged = true;
                    break;
                case SDLK_UP:
                    clockwiseRotate();
                    clockwiseRotate();
                    clockwiseRotate();
                    yChanged = true;
                    fixPos();
                    break;
            }
            break;
        case SDL_KEYUP:
            switch(event.key.keysym.sym) {
                case SDLK_SPACE:
                    downSpeed = 2;
                    break;
                default:
                    break;
            }
            break;
    }

    if(yChanged) {
        for(int i = 0;i < 4;i++) {
            if(matrix[yPos + deltaY[i]][xPos + deltaX[i]] == 1) {
                xPos = originX;
                yPos = originY;
                coll = true;
                break;
            }
        }
    }
}

void Tetris::genBuildingBlock()
{
    xPos = 5;
    yPos = 0;
    blockType = rand() % 7;
    switch(blockType) {
        case BLOCK_SQUARE:
            deltaX[0] = -1;deltaY[0] = 1;
            deltaX[1] = 0;deltaY[1] = 1;
            deltaX[2] = -1;deltaY[2] = 0;
            deltaX[3] = 0;deltaY[3] = 0;
            break;
        case BLOCK_LINE:
            deltaX[0] = -2;deltaY[0] = 0;
            deltaX[1] = -1;deltaY[1] = 0;
            deltaX[2] = 0;deltaY[2] = 0;
            deltaX[3] = 1;deltaY[3] = 0;
            break;
        case BLOCK_LEFTZ:
            deltaX[0] = -1;deltaY[0] = 1;
            deltaX[1] = 0;deltaY[1] = 1;
            deltaX[2] = 0;deltaY[2] = 0;
            deltaX[3] = 1;deltaY[3] = 0;
            break;
        case BLOCK_RIGHTZ:
            deltaX[0] = 0;deltaY[0] = 0;
            deltaX[1] = 0;deltaY[1] = 1;
            deltaX[2] = 1;deltaY[2] = 1;
            deltaX[3] = -1;deltaY[3] = 0;
        case BLOCK_LEFTL:
            deltaX[0] = 0;deltaY[0] = 1;
            deltaX[1] = 0;deltaY[1] = 0;
            deltaX[2] = 0;deltaY[2] = -1;
            deltaX[3] = -1;deltaY[3] = -1;
            break;
        case BLOCK_RIGHTL:
            deltaX[0] = 0;deltaY[0] = 1;
            deltaX[1] = 0;deltaY[1] = 0;
            deltaX[2] = 0;deltaY[2] = -1;
            deltaX[3] = 1;deltaY[3] = -1;
            break;
        case BLOCK_T:
            deltaX[0] = 0;deltaY[0] = 1;
            deltaX[1] = 0;deltaY[1] = 0;
            deltaX[2] = -1;deltaY[2] = 0;
            deltaX[3] = 1;deltaY[3] = 0;
            break;
    }
    fixPos();
}

void Tetris::fixPos()
{
    for(int i = 0;i < 4;i++) {
        if(xPos + deltaX[i] < 0) {
            xPos += abs(xPos + deltaX[i]);
        }
        if(yPos + deltaY[i] < 0) {
            yPos += abs(yPos + deltaY[i]);
        }
        if(xPos + deltaX[i] >= vNum) {
            xPos -= (xPos + deltaX[i] - vNum + 1);
        }
        if(yPos + deltaY[i] >= hNum) {
            yPos -= (yPos + deltaY[i] - hNum + 1);
        }
    }
}

void Tetris::clockwiseRotate()
{
    if(blockType == BLOCK_SQUARE) {
        return;
    }
    for (int i = 0; i < 4; i++) {
        int originX = deltaX[i];
        deltaX[i] = -deltaY[i];
        deltaY[i] = originX;
    }
}

bool Tetris::collision() const
{
    for(int i = 0;i < 4;i++) {
        if(yPos + deltaY[i] >= hNum - 1) {
            return true;
        }
    }
    return false;
}

void Tetris::decline(int line)
{
    for(int i = line;i >= 1;i--) {
        for(int j = 0;j < vNum;j++) {
            matrix[i][j] = matrix[i - 1][j];
        }
    }
    fill(matrix[0].begin(),matrix[0].end(),0);
}