#include "d3dUtility.h"
#include "Main.h"

IDirect3DDevice9 * Device = 0;

LRESULT CALLBACK d3d::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		::PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			::DestroyWindow(hWnd);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(__in HINSTANCE hInstance,
	__in_opt HINSTANCE hPrevInstance,
	__in LPSTR lpCmdLine,
	__in int nShowCmd)
{
	if (!d3d::InitD3D(hInstance, 640, 480, true, D3DDEVTYPE_HAL, &Device))
	{
		Msg_Error(TEXT("InitD3D -- FAILED"));
		return 0;
	}

	if (!Setup())
	{
		Msg_Error(TEXT("Setup -- FAILED"));
		return 0;
	}

	d3d::EnterMsgLoop(Display);

	Cleanup();

	Device->Release();

	return 0;
}