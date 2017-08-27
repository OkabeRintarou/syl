#include <iostream>
#include "tetris.h"

int main()
{
    Tetris tetris(kBlockSize * kBlockHNum,kBlockSize * kBlockVNum);
    if(!tetris.valid()) {
        fprintf(stderr,"Init game world failed:%s\n",SDL_GetError());
        return -1;
    }
    tetris.run();
    return 0;
}