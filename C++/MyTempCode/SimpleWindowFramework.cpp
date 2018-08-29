#include "SimpleWindowFramework.h"

namespace syl
{
	namespace simpleui
	{
		// SimpleWindowBase
		SimpleWindowBase::SimpleWindowBase()
		{
			_hwnd = 0;
			_hinstance = GetModuleHandle(NULL);
			_width = 640;
			_height = 480;
			_wndproc = syl::simpleui::SimpleWindowBase::WndProc;

		}

		SimpleWindowBase::SimpleWindowBase(int width, int height)
		{
			_hwnd = 0;
			_hinstance = GetModuleHandle(NULL);
			_width = width;
			_height = height;
			_wndproc = syl::simpleui::SimpleWindowBase::WndProc;
		}

		SimpleWindowBase::SimpleWindowBase(HINSTANCE hInstance, int width, int height, WNDPROC wndproc)
			:_hinstance(hInstance), _width(width), _height(height), _wndproc(wndproc)
		{
			_hwnd = 0;
		}

		bool SimpleWindowBase::Init()
		{
			WNDCLASSEX wcex;

			wcex.cbSize = sizeof(WNDCLASSEX);

			wcex.style = CS_HREDRAW | CS_VREDRAW;
			wcex.lpfnWndProc = _wndproc;
			wcex.cbClsExtra = 0;
			wcex.cbWndExtra = 0;
			wcex.hInstance = _hinstance;
			wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
			wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
			wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
			wcex.lpszMenuName = NULL;
			wcex.lpszClassName = TEXT("SimpleWindowFramework");
			wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

			if (FAILED(RegisterClassEx(&wcex)))
			{
				return false;
			}
			return true;
		}

		bool SimpleWindowBase::Show(int nShowCmd)
		{
			_hwnd = ::CreateWindow(TEXT("SimpleWindowFramework"), TEXT("SimpleWindowwork"),
				WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
				NULL, NULL, _hinstance, NULL);
			if (_hwnd == 0)
			{
				return false;
			}

			::MoveWindow(_hwnd, 200, 200, _width, _height, true);
			::ShowWindow(_hwnd,nShowCmd);
			::UpdateWindow(_hwnd);

			MSG msg = { 0 };
			while (GetMessage(&msg, 0, 0, 0))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}

			return true;
		}

		
		HRESULT CALLBACK SimpleWindowBase::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			switch (msg)
			{
			case WM_PRINT:
			{
				PAINTSTRUCT ps;
				HDC  hdc = BeginPaint(hwnd, &ps);
				EndPaint(hwnd, &ps);
			}
				break;
			case WM_DESTROY:
				PostQuitMessage(0);
				break;
			default:
				return DefWindowProc(hwnd, msg, wParam, lParam);
			}
			return 0;
		}

	}
}


