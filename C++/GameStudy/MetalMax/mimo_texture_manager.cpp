#include "mimo_include.h"
#include "mimo_texture_manager.h"
#include "SDL_image.h"
#include "mimo_debug.h"

namespace mimo{

	texture_manager * texture_manager::_instance = 0;

	texture_manager * texture_manager::instance()
	{
		if (_instance == 0){
			_instance = new texture_manager();
		}
		return _instance;
	}

	bool texture_manager::load(std::string filename, std::string id, SDL_Renderer * prenderer)
	{
		assert(prenderer != 0);

		SDL_Surface * tmp_surface = IMG_Load(filename.c_str());
		if (tmp_surface == 0){
			std::string err = std::string("load picture ") + filename + " failed.\n";
			MIMO_DEBUG(err.c_str());
			return false;
		}

		SDL_Texture * texture = SDL_CreateTextureFromSurface(prenderer, tmp_surface);
		SDL_FreeSurface(tmp_surface);

		if (texture != 0){

			_texture_map[id] = texture;
			return true;
		}

		std::string err = std::string("load texture ") + filename + " failed.\n";
		MIMO_DEBUG(err.c_str());

		return false;
	}

	void texture_manager::draw(std::string id, int x, int y, int width, int height,
		SDL_Renderer * prenderer, SDL_RendererFlip flip)
	{
		SDL_Texture * texture = _texture_map[id];
		assert(texture != 0);
		assert(prenderer  != 0);

		SDL_Rect src_rect;
		SDL_Rect dst_rect;

		src_rect.x = 0;
		src_rect.y = 0;
		src_rect.w = dst_rect.w = width;
		src_rect.h = dst_rect.h = height;
		dst_rect.x = x;
		dst_rect.y = y;

		SDL_RenderCopyEx(prenderer, texture, &src_rect,
			&dst_rect, 0, 0, flip);
	}

	void texture_manager::drawframe(std::string id, int x, int y, int width, int height, int current_row, int current_frame, SDL_Renderer * prenderer, SDL_RendererFlip flip /* = SDL_FLIP_NONE */)
	{
		SDL_Rect src_rect;
		SDL_Rect dst_rect;

		src_rect.x = width * current_frame;
		src_rect.y = height * (current_row - 1);
		src_rect.w = width;
		src_rect.h = height;

		dst_rect.x = x;
		dst_rect.y = y;
		dst_rect.w = width;
		dst_rect.h = height;

		SDL_RenderCopyEx(prenderer, _texture_map[id], &src_rect, &dst_rect, 0, 0, flip);
	}


}
