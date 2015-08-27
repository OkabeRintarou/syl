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
	CSnake * GetSnake();		//��ȡ��
	bool SetFood(int x,int y);		//��(x,y)���������ˮ��
	void SetFoodPos();
	SPoint * GetFoodPos();
	int GetTableWidth();
	int GetTableHeight();

	//�ߵĲ���
	void SnakeMove();
	void ChangeSnakeDirect(MoveState d);

};
#endif