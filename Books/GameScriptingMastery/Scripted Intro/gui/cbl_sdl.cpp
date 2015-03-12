#include "cbl_sdl.h"
#include "cbl_exception.h"
#include <map>

namespace cbl{
	namespace sdl{
		
		/*  XWindow class implements */
		XWindow::XWindow(const std::string & title, int x, int y, int w, int h, WindowFlag flags)
		{
			window_ = SDL_CreateWindow(title.c_str(), x, y, w, h, flags);
			if (window_ == nullptr){
				throw window_create_failed_exception{ "Create Window failed." };
			}
		}

		XWindow::~XWindow()
		{
			if (window_ != nullptr){
				SDL_DestroyWindow(window_);
			}
			
		}

		/* XRenderer implements */
		XRenderer::XRenderer(const XWindow & win,RendererFlag flag,int index)
		{
			renderer_ = SDL_CreateRenderer(win.window_, index, flag);
			if (renderer_ == nullptr){
				throw renderer_create_failed_exception{ "create renderer failed." };
			}
		}

		XRenderer::~XRenderer()
		{
			if (renderer_ != nullptr){
				SDL_DestroyRenderer(renderer_);
			}
		}

	}//end namespace sdl
}