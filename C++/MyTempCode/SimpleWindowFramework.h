/***********************************************************************
SimpleWindowFramework.h
***********************************************************************/
#ifndef __SYL_SIMPLEWINDOWFRAMEWORK_H__
#define __SYL_SIMPLEWINDOWFRAMEWORK_H__
#include <windows.h>

namespace syl
{
	namespace simpleui
	{
		//HRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


		class SimpleWindowBase
		{
		protected:
			HWND _hwnd;
			HINSTANCE _hinstance;
			int _width, _height;
			WNDPROC _wndproc;
		public:
			virtual ~SimpleWindowBase() {}
		public:
			SimpleWindowBase();
			SimpleWindowBase(int width, int height);
			SimpleWindowBase(HINSTANCE hInstance, int width = 640, int height = 480, WNDPROC wndproc = syl::simpleui::SimpleWindowBase::WndProc);
			virtual bool Init();
			virtual bool Show(int nShowCmd = SW_NORMAL);
		private:
			static HRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
		};

		
		
	}// namespace simpleui
}



#endif//__SYL_SIMPLEWINDOWFRAMEWORK_H__