
//////////////////////////////////////////////////////////////////////////
// File : d3dUtility.cpp
//////////////////////////////////////////////////////////////////////////

#include "d3dUtility.h"


void Msg_Error(wchar_t * err)
{
	MessageBoxW(NULL, err, L"error", MB_ICONERROR);
}

bool d3d::InitD3D(HINSTANCE hInstance,
	int width, int height,
	bool windowed,
	D3DDEVTYPE devicetype,
	IDirect3DDevice9 ** device)
{
	// Create the main application window
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)d3d::WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.hIcon = ::LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wc.lpszMenuName = 0;
	wc.lpszClassName = TEXT("d3dApp");

	if (!RegisterClass(&wc))
	{
		Msg_Error(TEXT("RegisterClass -- FAILED"));
		return false;
	}

	HWND hwnd = 0;
	hwnd = ::CreateWindow(TEXT("d3dApp"), TEXT("d3dApp"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL);

	::MoveWindow(hwnd, 100, 100, 640, 480, TRUE);

	::ShowWindow(hwnd, SW_SHOWNORMAL);
	::UpdateWindow(hwnd);

	// Init D3D
	HRESULT hr = 0;
	// Step 1: Create the IDirect3D9 object
	IDirect3D9 * d3d9 = 0;
	d3d9 = Direct3DCreate9(D3D_SDK_VERSION);
	if (!d3d9)
	{
		Msg_Error(TEXT("Direct3DCreate9() -- FAILED"));
		return false;
	}

	// Step 2: Check up for hardware vp
	D3DCAPS9 caps;
	d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, devicetype, &caps);

	int vp = 0;
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	// Step 3:fill out the D3DPRESENT_PARAMETER structure
	D3DPRESENT_PARAMETERS d3dpp;
	d3dpp.BackBufferWidth = width;
	d3dpp.BackBufferHeight = height;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hwnd;
	d3dpp.Windowed = windowed;
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.Flags = 0;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	// Step 4 :Create the device
	hr = d3d9->CreateDevice(
		D3DADAPTER_DEFAULT, devicetype, hwnd, vp, &d3dpp, device);

	if (FAILED(hr))
	{
		// try again using a 16-bit depth buffer
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

		hr = d3d9->CreateDevice(
			D3DADAPTER_DEFAULT,
			devicetype,
			hwnd,
			vp,
			&d3dpp,
			device);

		if (FAILED(hr))
		{
			d3d9->Release(); // done with d3d9 object
			Msg_Error(TEXT("CreateDevice -- FAILED"));
			return false;
		}
	}

	d3d9->Release();

	return true;
}


int d3d::EnterMsgLoop(bool(*ptr_display)(float timeDelta))
{
	MSG msg;
	memset(&msg, 0, sizeof(msg));

	static float last_time = (float)timeGetTime();

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		else
		{
			float current_time = (float)timeGetTime();

			float time_delta = (current_time - last_time)*0.001f;

			ptr_display(time_delta);
			last_time = current_time;
		}
	}

	return msg.wParam;
}
