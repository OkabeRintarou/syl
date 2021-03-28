#include"Table.h"
#include<ctime>
#include<cstdlib>
CTable::CTable(int w,int h)
{
	m_width=w;
	m_height=h;
	m_fruit.x=0;
	m_fruit.y=0;
}
CTable::~CTable(){}

CSnake * CTable::GetSnake()
{
	return & snake;
}
bool CTable::SetFood(int x,int y)	//在(x,y)处随机产生水果
{
	int i;
	for(i=0;i<snake.GetLength();i++)
		if(x == snake.GetPos()[0].x && y == snake.GetPos()[0].y)
			return false;
	if(x<0 || y<0 || x>m_width||y>m_height)
		return false;
	return true;
}

void CTable::SetFoodPos()
{
	//水果的横坐标取值为WALL_WIDTH--g_iClientWidth-WALL_WIDTH之间的+20,40,60...
	srand(unsigned(time(0)));
	int tableBlockWidth=(CXCLIENT-2*WALL_WIDTH)/20;
	int tableBlockHeight=(CYCLIENT-2*WALL_HEIGHT)/20;
	m_fruit.x=(rand()%tableBlockWidth)*20+WALL_WIDTH;
	m_fruit.y=(rand()%tableBlockHeight)*20+WALL_HEIGHT;
}
SPoint * CTable::GetFoodPos()
{
	return & m_fruit;
}
int CTable::GetTableWidth()
{
	return m_width;
}

int CTable::GetTableHeight()
{
	return  m_height;
}

void CTable::SnakeMove()
{
	snake.Move();
}
void CTable::ChangeSnakeDirect(MoveState d)
{
	snake.ChangeMoveState(d);
}