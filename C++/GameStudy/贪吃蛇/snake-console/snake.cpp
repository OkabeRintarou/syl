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
		
		//����������߾ͷ�������
		if((mode=='a'&&ch=='d')||(mode=='d'&&ch=='a')||(mode=='s'&&ch=='w')||mode=='w'&&ch=='s')
		{
			cout<<SOUND;
		}
		
		//�������ƶ��ķ���
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
	
	

	//�������ƶ��ķ���ı���ͷ������
	switch(mode)
	{
	case'a':head.x-=2;break;
	case'd':head.x+=2;break;
	case's':head.y+=1;break;
	case'w':head.y-=1;break;
	default:cout<<SOUND;break;
	}

	//Ӧ���ڸ�������������֮ǰ������β
	gotoxy(Body[len-1].x,Body[len-1].y);
	cout<<Null;
	//�����ߵ�ÿһ����������
	for(int i=len-1;i>0;i--)
	{
		Body[i].x=Body[i-1].x;
		Body[i].y=Body[i-1].y;
	}

	Body[0].x=head.x;
	Body[0].y=head.y;

	//�ߵ������������֮�����»����ߵ�����

	
	gotoxy(Body[len-1].x,Body[len-1].y);
	cout<<Null;
	gotoxy(Body[0].x,Body[0].y);
	cout<<NODE;
	/*for(int i=0;i<len;i++)
	{
		gotoxy(Body[i].x,Body[i].y);
		cout<<NODE;
	}*/
	//================ʹ������ע�͵Ĵ����������˸===============================
	Sleep(speed);

}



//�ж����Ƿ�����
bool Snake::is_dead()
{
	//ײ��ǽ����
	if(head.x>cxClient||head.x<0||head.y>cyClient||head.y<0)
		return TRUE;
	//ײ���Լ�����������

	for(int i=1;i<len;i++)
		if(head.x == Body[i].x && head.y == Body[i].y)
			return TRUE;
	return FALSE;
}

//�ж����Ƿ�Ե�ƻ��
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
	COORD pos={x,y};    //��������
	HANDLE hOut=GetStdHandle(STD_OUTPUT_HANDLE);  //����һ�����
	SetConsoleCursorPosition(hOut,pos);
}


void hidden()                            //���ع�꺯��

{

HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

CONSOLE_CURSOR_INFO cci;

GetConsoleCursorInfo(hOut,&cci);

cci.bVisible=0;                   //0���أ�1��ʾ

SetConsoleCursorInfo(hOut,&cci);

}


//�������ʳ�� 
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


