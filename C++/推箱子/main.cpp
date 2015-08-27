#include "Map.h"
#include "Graphics.h"
#include "Game.h"

#ifdef main
#undef main
#endif

int main()
{
	Graphics myGraphics(20 * OBJ_WIDTH, 20 * OBJ_HEIGHT, 0, SDL_HWSURFACE, 100);
	Map myMap(&myGraphics);
	Game myGame(&myGraphics, &myMap);
	
	int ret = myGame.StartGame("level1.map");

	bool bRunning = true;
	while (bRunning){
		// begin next level
		if (ret == 1){
			string next_filename = myGame.NextLevel();
			myGame.StartGame(next_filename);
		}
		else if (ret == 0){
			// re flush
			return 0;
		}
	}
	return 0;
}