#include"Snake.h"
#include<cstdio>
#define SNAKE_MOVE   20
CSnake::CSnake()
{
	m_length=1;
	m_dir=MS_RIGHT;
	m_pPos=new SPoint[m_length];
	//初始化每每一节蛇身体的坐标
	int i;
	for(i=0;i<m_length;i++)
		m_pPos[i].x=m_pPos[i].y=0;
}
CSnake::~CSnake()
{
	delete [] m_pPos;
}
int	 CSnake::GetLength()
{
	return m_length;
}
void CSnake::ChangeMoveState(MoveState d)
{
	m_dir=d;
}
MoveState CSnake::GetMoveState()
{
	return m_dir;
}
bool CSnake::IsHeadTouchBody()
{
	if(m_length<4)
		return false;
	int i;
	for(i=1;i<m_length;i++)
		if(m_pPos[0].x == m_pPos[i].x && m_pPos[0].y == m_pPos[i].y )
			return true;
	return false;
}
bool CSnake::IsHeadTouchBlock(int width,int height)
{
	if(m_pPos[0].x<WALL_WIDTH || m_pPos[0].x>width-WALL_WIDTH || m_pPos[0].y<WALL_HEIGHT || m_pPos[0].y >height-WALL_HEIGHT)
		return true;
	return false;
}
bool CSnake::IsEatFruit(int x,int y)		//是否吃到(x,y)处的水果
{
	if(m_pPos[0].x == x && m_pPos[0].y == y)
		return true;
	return false;
}
bool CSnake::AddBody()
{
	int i;
	//保存蛇身体的每一节坐标
	SPoint * savepos=new SPoint[m_length];
	for(i=0;i<m_length;i++)
	{
		savepos[i].x=m_pPos[i].x;
		savepos[i].y=m_pPos[i].y;
	}
	++m_length;
	delete [] m_pPos;		//释放m_pPos指向的空间
	m_pPos=NULL;
	m_pPos=new SPoint[m_length];
	for(i=0;i<m_length-1;i++)
	{
		m_pPos[i].x=savepos[i].x;
		m_pPos[i].y=savepos[i].y;
	}
	m_pPos[m_length-1].x=m_pPos[m_length-2].x;
	m_pPos[m_length-1].y=m_pPos[m_length-2].y;
	delete [] savepos;
	return true;
}
void CSnake::SetHeadPos(int x,int y)
{
	m_pPos[0].x=x;
	m_pPos[0].y=y;
}
void CSnake::Move()
{
	int i;
	for(i=m_length-1;i>0;i--)
	{
		m_pPos[i].x=m_pPos[i-1].x;
		m_pPos[i].y=m_pPos[i-1].y;
	}

	//蛇头坐标根据移动方向进行改变
	switch(m_dir)
	{
	case MS_UP:
		m_pPos[0].y-=SNAKE_MOVE;
		break;
	case MS_DOWN:
		m_pPos[0].y+=SNAKE_MOVE;break;
	case MS_LEFT:
		m_pPos[0].x-=SNAKE_MOVE;break;
	case MS_RIGHT:
		m_pPos[0].x+=SNAKE_MOVE;break;
	}
}
SPoint * CSnake::GetPos()
{
	return m_pPos;
}