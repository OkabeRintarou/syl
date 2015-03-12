#ifndef CBL_SDL_H_
#define CBL_SDL_H_

#include <SDL.h>
#include <string>

namespace cbl{
	namespace sdl{
		
		using WindowFlag = SDL_WindowFlags;
		using RendererFlag = SDL_RendererFlags;

		class XRenderer;

		class XSdl
		{

		};

		class XWindow
		{
		public:
			XWindow(const std::string & title, int x, int y, int w, int h, WindowFlag flags);
			~XWindow();
		private:
			friend class XRenderer;
			SDL_Window * window_;
		};

		class XRenderer
		{
		public:
			XRenderer(const XWindow & win,RendererFlag flag = RendererFlag::SDL_RENDERER_ACCELERATED,int index = -1);
			~XRenderer();
		private:
			SDL_Renderer * renderer_;
		};
	}
}

#endif