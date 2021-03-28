#include<windows.h>
#include"Table.h"

//全局变量的声明
HINSTANCE hInst;
HWND hwnd;
HDC windowDC,bufDC,fruitDC,snakeDC,brickDC,headDC,scoreboardDC;
HBITMAP brickbmp,snakebmp,fruitbmp,headbmp,scoreboard;
DWORD tNow,tPre;
int g_iClientWidth,g_iClientHeight;
CTable table;
int iScores=0;

//函数声明
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE hInstance,int nShowCmd);
void ShellDraw(HDC hdc);
void GameAreaDraw(HDC hdc);
void MyPaint(HDC hdc);
LRESULT CALLBACK WndProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd )
{
	MSG msg;
	MyRegisterClass(hInstance);
	if(!InitInstance(hInstance,nShowCmd))
	{
		MessageBeep(0);
		return FALSE;
	}

	GetMessage(&msg,NULL,0,0);
	while(msg.message!=WM_QUIT)
	{
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			tNow=GetTickCount();
			if(tNow-tPre>=100)
			{
				MyPaint(bufDC);
			}
			if(table.GetSnake()->IsHeadTouchBlock(g_iClientWidth,g_iClientHeight)||table.GetSnake()->IsHeadTouchBody())	
			{
				MessageBox(hwnd,L"你挂了",L"游戏结束",MB_OK);
				PostQuitMessage(0);
			}
		}
	}
	return msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASS wc;
	wc.style=CS_HREDRAW|CS_VREDRAW;
	wc.lpfnWndProc=WndProc;
	wc.cbClsExtra=0;
	wc.cbWndExtra=0;
	wc.hIcon=LoadIcon(NULL,IDI_APPLICATION);
	wc.hCursor=LoadCursor(NULL,IDC_ARROW);
	wc.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.hInstance=hInstance;
	wc.lpszMenuName=NULL;
	wc.lpszClassName=TEXT("snake");

	return RegisterClass(&wc);
}


BOOL InitInstance(HINSTANCE hInstance,int nShowCmd)
{
	g_iClientWidth=GetSystemMetrics(SM_CXSCREEN );
	g_iClientHeight=GetSystemMetrics(SM_CYSCREEN);
	hwnd=CreateWindow(TEXT("snake"),TEXT("贪吃蛇"),WS_POPUP,
					  0,0,g_iClientWidth,g_iClientHeight,
					  NULL,NULL,hInstance,NULL);

	if(!hwnd)
	{
		MessageBeep(0);
		return FALSE;
	}

	ShowWindow(hwnd,nShowCmd);
	UpdateWindow(hwnd);

	windowDC=GetDC(hwnd);
	bufDC=CreateCompatibleDC(windowDC);
	fruitDC=CreateCompatibleDC(windowDC);
	snakeDC=CreateCompatibleDC(windowDC);
	brickDC=CreateCompatibleDC(windowDC);
	headDC=CreateCompatibleDC(windowDC);
	scoreboardDC=CreateCompatibleDC(windowDC);

	HBITMAP bmtemp=CreateCompatibleBitmap(windowDC,g_iClientWidth,g_iClientHeight);
	SelectObject(bufDC,bmtemp);
	DeleteObject(bmtemp);

	//加载位图
	brickbmp=(HBITMAP)LoadImage(NULL,L"brick.bmp",IMAGE_BITMAP,16,16,LR_LOADFROMFILE);
	snakebmp=(HBITMAP)LoadImage(NULL,L"snake.bmp",IMAGE_BITMAP,48,48,LR_LOADFROMFILE);
	fruitbmp=(HBITMAP)LoadImage(NULL,L"fruit.bmp",IMAGE_BITMAP,160,80,LR_LOADFROMFILE);
	headbmp=(HBITMAP)LoadImage(NULL,L"headbmp.bmp",IMAGE_BITMAP,48,48,LR_LOADFROMFILE);
	scoreboard=(HBITMAP)LoadImage(NULL,	L"scoreboard.bmp",IMAGE_BITMAP,256,55,LR_LOADFROMFILE);

	SelectObject(fruitDC,fruitbmp);
	SelectObject(brickDC,brickbmp);
	SelectObject(snakeDC,snakebmp);
	SelectObject(headDC,headbmp);
	SelectObject(scoreboardDC,scoreboard);
	
	//设置蛇头的坐标
	table.GetSnake()->SetHeadPos(WALL_WIDTH,WALL_HEIGHT);

	//设置水果
	table.SetFoodPos();
	
	return TRUE;

}

//绘制外壳
void ShellDraw(HDC hdc)
{
	int i,j;
	for(i=0;i<g_iClientWidth;i+=BLOCK_WIDTH)
		for(j=0;j<g_iClientHeight;j+=BLOCK_HEIGHT)
		{
			BitBlt(hdc,i,j,BLOCK_WIDTH,BLOCK_HEIGHT,brickDC,0,0,SRCCOPY);
		}
}

//游戏区域的绘制
void GameAreaDraw(HDC hdc)
{
	
	RECT rect;
	rect.left=WALL_WIDTH;
	rect.top=WALL_HEIGHT;
	rect.right=g_iClientWidth-WALL_WIDTH;
	rect.bottom=g_iClientHeight-WALL_HEIGHT;
	FillRect(hdc,&rect,WHITE_BRUSH);

	//绘制得分板
	SetBkMode(hdc,TRANSPARENT);
	BitBlt(hdc,400,20,256,55,scoreboardDC,0,0,SRCCOPY);
	TCHAR str[15];
	wsprintf(str,L"Scores:%d ",iScores*100);
	TextOut(hdc,440,35,str,wcslen(str));
	//绘制水果
	BitBlt(hdc,table.GetFoodPos()->x,table.GetFoodPos()->y,20,20,fruitDC,100,0,SRCPAINT);
	BitBlt(hdc,table.GetFoodPos()->x,table.GetFoodPos()->y,20,20,fruitDC,20,0,SRCAND);
	if(table.GetSnake()->IsEatFruit(table.GetFoodPos()->x,table.GetFoodPos()->y))
	{
		table.SetFoodPos();
		BitBlt(hdc,table.GetFoodPos()->x,table.GetFoodPos()->y,20,20,fruitDC,100,0,SRCPAINT);
		BitBlt(hdc,table.GetFoodPos()->x,table.GetFoodPos()->y,20,20,fruitDC,20,0,SRCAND);
	}

	//绘制蛇
	int i;
	COLORREF color=GetPixel(headDC,0,0);
	::GdiTransparentBlt(hdc,table.GetSnake()->GetPos()[0].x,table.GetSnake()->GetPos()[0].y,20,20,
						    headDC,0,0,48,48,color);
	color=GetPixel(snakeDC,0,0);
	for(i=1;i<table.GetSnake()->GetLength();i++)
		::GdiTransparentBlt(hdc,table.GetSnake()->GetPos()[i].x,table.GetSnake()->GetPos()[i].y,20,20,
						    snakeDC,0,0,48,48,color);
	table.SnakeMove();
	
}
void MyPaint(HDC hdc)
{
	//游戏外壳绘制到缓冲区
	ShellDraw(hdc);
	//游戏区域绘制到缓冲区
	GameAreaDraw(hdc);
	//将缓冲区的位图贴到环境设备DC
	BitBlt(windowDC,0,0,g_iClientWidth,g_iClientHeight,hdc,0,0,SRCCOPY);
	//或取这次画图的时间
	if(table.GetSnake()->IsEatFruit(table.GetFoodPos()->x,table.GetFoodPos()->y))
	{
		iScores++;
		table.GetSnake()->AddBody();
	}
	tPre=GetTickCount();
}
LRESULT CALLBACK WndProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	
	switch(msg)
	{
	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
		case VK_UP:
			if(table.GetSnake()->GetMoveState()!=MS_DOWN)
				table.ChangeSnakeDirect(MS_UP);
			break;
		case VK_DOWN:
			if(table.GetSnake()->GetMoveState()!=MS_UP)
				table.ChangeSnakeDirect(MS_DOWN);
			break;
		case VK_LEFT:
			if(table.GetSnake()->GetMoveState()!=MS_RIGHT)
				table.ChangeSnakeDirect(MS_LEFT);
			break;
		case VK_RIGHT:
			if(table.GetSnake()->GetMoveState()!=MS_LEFT)
				table.ChangeSnakeDirect(MS_RIGHT);
			break;
		}
		return 0;
	case WM_DESTROY:
		DeleteObject(fruitbmp);
		DeleteObject(snakebmp);
		DeleteObject(brickbmp);
		DeleteObject(scoreboard);
		DeleteObject(headbmp);
		DeleteDC(windowDC);
		DeleteDC(snakeDC);
		DeleteDC(fruitDC);
		DeleteDC(brickDC);
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hwnd,msg,wParam,lParam);
	}

	return 0;
}