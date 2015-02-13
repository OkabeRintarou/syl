#ifndef CBL_SDL_H_
#define CBL_SDL_H_

#include <SDL.h>
#include <string>

namespace cbl{
	namespace sdl{
		

		enum class WindowFlag
		{
			WINDOW_FULLSCREEN,
			WINDOW_FULLSCREEN_DESKTOP,
			WINDOW_OPENGL,
			WINDOW_SHOWN,
			WINDOW_HIDDEN,
			WINDOW_BORDERLESS,
			WINDOW_RESIZABLE,
			WINDOW_MINIMIZED,
			WINDOW_MAXIMIZED,
			WINDOW_INPUT_GRABBED,
			WINDOW_INPUT_FOCUS,
			WINDOW_MOUSE_FOCUS,
			WINDOW_FOREIGN,
			WINDOW_ALLOW_HIGHDPI
		};

		
		class XSdl
		{

		};
		class XWindow
		{
		public:
			XWindow(const std::string & title, int x, int y, int w, int h, WindowFlag flags);
			~XWindow();
		private:
			SDL_Window * window_;
		};


		extern inline Uint32 GetWindowFlag(WindowFlag flag);
	}
}

#endif