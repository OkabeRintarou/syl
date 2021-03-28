#include<iostream>
#include<cstdlib>
#include<ctime>
#include<Windows.h>
#include<conio.h>
#include"snake.h"

using std::cout;
using std::cin;

Snake::Snake()
{
	len=3;

	mode='d';

	speed=300;

	head.x=cxClient/2-1;
	head.y=cyClient/2;
	Body[0].x=head.x;
	Body[0].y=head.y;


	for(int i=1;i<MAX;i++)
	{
		Body[i].x=Body[i-1].x-2;
		Body[i].y=Body[i-1].y;
	}
}

Snake::~Snake()
{
}

void Snake::Move()
{
	hidden();
	if(kbhit())
	{
		char ch;
		ch=_getch();
		
		//如果蛇往回走就发出响声
		if((mode=='a'&&ch=='d')||(mode=='d'&&ch=='a')||(mode=='s'&&ch=='w')||mode=='w'&&ch=='s')
		{
			cout<<SOUND;
		}
		
		//更改蛇移动的方向
		else
		{
			switch(ch)
			{
			case 'a':mode='a';break;
			case 'd':mode='d';break;
			case 's':mode='s';break;
			case 'w':mode='w';break;
			default:cout<<SOUND;break;
			}
		}
	}
	
	

	//根据蛇移动的方向改变蛇头的坐标
	switch(mode)
	{
	case'a':head.x-=2;break;
	case'd':head.x+=2;break;
	case's':head.y+=1;break;
	case'w':head.y-=1;break;
	default:cout<<SOUND;break;
	}

	//应该在更新蛇身体坐标之前擦除蛇尾
	gotoxy(Body[len-1].x,Body[len-1].y);
	cout<<Null;
	//更新蛇的每一节身体坐标
	for(int i=len-1;i>0;i--)
	{
		Body[i].x=Body[i-1].x;
		Body[i].y=Body[i-1].y;
	}

	Body[0].x=head.x;
	Body[0].y=head.y;

	//蛇的身体坐标更新之后重新画出蛇的身体

	
	gotoxy(Body[len-1].x,Body[len-1].y);
	cout<<Null;
	gotoxy(Body[0].x,Body[0].y);
	cout<<NODE;
	/*for(int i=0;i<len;i++)
	{
		gotoxy(Body[i].x,Body[i].y);
		cout<<NODE;
	}*/
	//================使用上述注释的代码会引起闪烁===============================
	Sleep(speed);

}



//判断蛇是否死亡
bool Snake::is_dead()
{
	//撞到墙死亡
	if(head.x>cxClient||head.x<0||head.y>cyClient||head.y<0)
		return TRUE;
	//撞到自己的身体死亡

	for(int i=1;i<len;i++)
		if(head.x == Body[i].x && head.y == Body[i].y)
			return TRUE;
	return FALSE;
}

//判断蛇是否吃到苹果
bool Snake::is_eat(Apple food)
{
	if(head.x == food.x && head.y ==food.y)
	{
		speed-=2;
		++len;
		return TRUE;
	}
	else 
		return FALSE;
}

bool Snake::is_win()
{
	if(len==MAX)
	{
		GameWin();
		return TRUE;
	}
	else 
		return FALSE;
}
void gotoxy(int x,int y)
{
	COORD pos={x,y};    //定义坐标
	HANDLE hOut=GetStdHandle(STD_OUTPUT_HANDLE);  //设置一个句柄
	SetConsoleCursorPosition(hOut,pos);
}


void hidden()                            //隐藏光标函数

{

HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

CONSOLE_CURSOR_INFO cci;

GetConsoleCursorInfo(hOut,&cci);

cci.bVisible=0;                   //0隐藏，1显示

SetConsoleCursorInfo(hOut,&cci);

}


//随机产生食物 
Apple & product_food(Apple & app)
{
	
	srand(unsigned(time(0)));
	app.x=rand()%cxClient;
	app.y=rand()%cyClient;
	if(app.x%2!=0)
		app.x-=1;
	gotoxy(app.x,app.y);
	cout<<APPLE;
	return app;
}
void GameOver()
{
	gotoxy(cxClient/2-5,cyClient/2);
	cout<<"GameOver!";
	char c=_getch();
}

void GameWin()
{
	gotoxy(cxClient/2-5,cyClient/2);
	cout<<"GameWin";
	char c=_getch();
}


