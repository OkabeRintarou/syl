
//  d3dUtility.h


#ifndef _D3DUTILITY_H_H_
#define _D3DUTILITY_H_H_

#include <d3d9.h>
#include <d3dx9.h>
#include <Windows.h>


void Msg_Error(wchar_t * err);


namespace d3d
{
	bool InitD3D(
		HINSTANCE hInstance,
		int width, int height,
		bool windowed,
		D3DDEVTYPE devicetype,
		IDirect3DDevice9 ** device);

	int EnterMsgLoop(
		bool(*ptr_display)(float timeDelta));

	LRESULT CALLBACK WndProc(
		HWND hWnd,
		UINT msg,
		WPARAM wParam,
		LPARAM lParam);

	template<typename T>void Release(T t)
	{
		if (t)
		{
			t->Release();
			t = 0;
		}
	}

	template<typename T> void Delete(T t)
	{
		if (t)
		{
			t->Delete();
			t = 0;
		}
	}

}//end namespace d3d
#endif