#ifndef _TETRIS_H
#define _TETRIS_H

#include <SDL2/SDL.h>
#include <vector>

#define BLOCK_SQUARE        0
#define BLOCK_LINE          1
#define BLOCK_LEFTZ         2
#define BLOCK_RIGHTZ        3
#define BLOCK_LEFTL         4
#define BLOCK_RIGHTL        5
#define BLOCK_T             6

extern const int kBlockSize;
extern const int kBlockHNum;
extern const int kBlockVNum;

class Tetris
{
public:
    Tetris(int width,int height);
    ~Tetris();

    bool valid()const;
    void run();

private:
    void fixPos();
    void updateEvent();
    void genBuildingBlock();
    void drawAll();
    void clockwiseRotate();
    bool collision()const;
    void decline(int line);
private:
    SDL_Window* window;
    SDL_Renderer* render;

    std::vector<std::vector<int>> matrix;

    int hNum,vNum;
    int width,height;

    float xPos,yPos;
    int deltaX[4],deltaY[4];
    int blockType;
    int downSpeed;

    bool quit;
    bool coll;
};

#endif // _TETRIS_H
