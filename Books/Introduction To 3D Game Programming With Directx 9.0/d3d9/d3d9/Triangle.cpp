
#include"d3dUtility.h"

extern IDirect3DDevice9 * Device;

const int Width = 640;
const int Height = 480;

IDirect3DVertexBuffer9 * triangle = 0;  //vertex buffer to store triangle data

//
// 顶点结构
//
struct Vertex
{
	Vertex(){}
	Vertex(float x, float y, float z)
		:_x(x), _y(y), _z(z){}

	float _x, _y, _z;
	static const DWORD FVF;
};

const DWORD Vertex::FVF = D3DFVF_XYZ;

bool Setup(void)
{
	// 创建容纳3个Vertex类型顶点的静态顶点缓存
	Device->CreateVertexBuffer(
		3 * sizeof(Vertex),
		D3DUSAGE_WRITEONLY,
		Vertex::FVF,
		D3DPOOL_MANAGED,
		&triangle,
		0);

	//访问缓存内容
	Vertex * vertices = 0;
	triangle->Lock(0, 0, (void**)&vertices, 0);

	vertices[0] = Vertex(-1.0f, 0.0f, 2.0f);
	vertices[1] = Vertex(0.0f, 1.0f, 2.0f);
	vertices[2] = Vertex(1.0f, 0.0f, 2.0f);

	triangle->Unlock();

	//
	// Set the projection matrix
	//

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
		&proj,
		D3DX_PI * 0.5f,
		(float)Width / (float)Height,
		1.0f,
		1000.0f);
	Device->SetTransform(D3DTS_PROJECTION, &proj);

	//
	// Set wireframe mode render state
	//
	Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);


	return true;
}

void Cleanup(void)
{
	d3d::Release<IDirect3DVertexBuffer9*>(triangle);
}

bool Display(float timeDelta)
{
	if (Device)
	{
		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);

		Device->BeginScene();

		Device->SetStreamSource(0, triangle, 0, sizeof(Vertex));
		Device->SetFVF(Vertex::FVF);
		// Draw a triangle
		Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

		Device->EndScene();

		Device->Present(0, 0, 0, 0);
	}

	return true;
}
