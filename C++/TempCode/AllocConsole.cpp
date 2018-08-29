// 在Win32应用程序中分配控制台并将标准输入、输出、错误重定向到控制台
#include <windows.h>
#include <stdio.h>

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nShowCmd)
{
#define USE_WIN32_CONSOLE

#ifdef USE_WIN32_CONSOLE
	AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
#endif

#ifdef USE_WIN32_CONSOLE
	FreeConsole();
#endif

	return 0;
}