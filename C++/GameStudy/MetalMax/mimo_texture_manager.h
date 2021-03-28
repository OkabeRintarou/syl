#ifndef _MIMO_TEXTURE_MANAGER_
#define _MIMO_TEXTURE_MANAGER_

#include <string>
#include <map>
#include "SDL.h"

namespace mimo{
	class texture_manager
	{
	public:
		static texture_manager * instance();
	public:
		~texture_manager(){ _instance = 0; }
		bool load(std::string filename, std::string id, SDL_Renderer * prenderer);
		void draw(std::string id, int x, int y, int width, int height,
			SDL_Renderer * prenderer, SDL_RendererFlip flip = SDL_FLIP_NONE);
		void drawframe(std::string id, int x, int y, int width, int height,
			int current_row, int current_frame, SDL_Renderer * prenderer, SDL_RendererFlip flip = SDL_FLIP_NONE);
	private:
		texture_manager() :
			_current_frame(0){}
	private:
	/*public:*/
		int _current_frame;
		std::map<std::string, SDL_Texture*> _texture_map;

		static texture_manager * _instance;

	};
	

}
#endif //_MIMO_TEXTURE_MANAGER_