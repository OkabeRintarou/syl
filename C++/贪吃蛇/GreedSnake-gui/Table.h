#ifndef GREED_SNAKE_TABLE_H
#define GREED_SNAKE_TABLE_H
#include"Snake.h"
class CTable
{
private:
	int m_width;		
	int m_height;		
	CSnake snake;
	SPoint m_fruit;
public:
	CTable(int w=0,int h=0);
	~CTable();
	CSnake * GetSnake();		//获取蛇
	bool SetFood(int x,int y);		//在(x,y)处随机产生水果
	void SetFoodPos();
	SPoint * GetFoodPos();
	int GetTableWidth();
	int GetTableHeight();

	//蛇的操作
	void SnakeMove();
	void ChangeSnakeDirect(MoveState d);

};
#endif