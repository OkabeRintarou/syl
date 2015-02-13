#include "cbl_sdl.h"
#include "cbl_exception.h"
#include <map>

namespace cbl{
	namespace sdl{
		
		static std::map<WindowFlag,Uint32> window_flag =
		{
			{ WindowFlag::WINDOW_FULLSCREEN,             SDL_WINDOW_FULLSCREEN},
			{ WindowFlag::WINDOW_FULLSCREEN_DESKTOP,     SDL_WINDOW_FULLSCREEN_DESKTOP},
			{ WindowFlag::WINDOW_ALLOW_HIGHDPI,          SDL_WINDOW_ALLOW_HIGHDPI},
			{ WindowFlag::WINDOW_BORDERLESS,             SDL_WINDOW_BORDERLESS},
			{ WindowFlag::WINDOW_FOREIGN,                SDL_WINDOW_FOREIGN},
			{ WindowFlag::WINDOW_HIDDEN,                 SDL_WINDOW_HIDDEN},
			{ WindowFlag::WINDOW_INPUT_FOCUS,            SDL_WINDOW_INPUT_FOCUS},
			{ WindowFlag::WINDOW_INPUT_GRABBED,          SDL_WINDOW_INPUT_FOCUS},
			{ WindowFlag::WINDOW_MAXIMIZED,              SDL_WINDOW_MAXIMIZED},
			{ WindowFlag::WINDOW_MINIMIZED,              SDL_WINDOW_MINIMIZED},
			{ WindowFlag::WINDOW_MOUSE_FOCUS,            SDL_WINDOW_MOUSE_FOCUS },
			{ WindowFlag::WINDOW_OPENGL,                 SDL_WINDOW_OPENGL},
			{ WindowFlag::WINDOW_RESIZABLE,              SDL_WINDOW_RESIZABLE},
			{ WindowFlag::WINDOW_SHOWN,                  SDL_WINDOW_SHOWN}
		};

		Uint32 GetWindowFlag(WindowFlag flag)
		{
			return window_flag[flag];
		}

		/*  XWindow class implements */
		XWindow::XWindow(const std::string & title, int x, int y, int w, int h, WindowFlag flags)
		{
			window_ = SDL_CreateWindow(title.c_str(), x, y, w, h, GetWindowFlag(flags));
			if (window_ == 0){
				throw window_create_failed_exception{ "Create Window failed." };
			}
		}

		XWindow::~XWindow()
		{
			SDL_DestroyWindow(window_);
		}


	}//end namespace sdl
}