// ��Win32Ӧ�ó����з������̨������׼���롢����������ض��򵽿���̨
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