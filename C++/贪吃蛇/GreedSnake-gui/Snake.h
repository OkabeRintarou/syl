#ifndef _SNAKE_H_
#define _SNAKE_H_

#include"Init.h"

enum MoveState{MS_UP,MS_DOWN,MS_LEFT,MS_RIGHT};
enum BitmapState{BS_HEAD,BS_BODY};
struct SPoint
{
	int x;
	int y;
};
class CSnake
{
private:
	int m_length;			//蛇的长度
	SPoint * m_pPos;		//指向蛇的每一节身体的坐标的指针
	MoveState m_dir;				//蛇移动的方向
	
public:
	CSnake();
	~CSnake();
	int GetLength();
	void ChangeMoveState(MoveState d);
	MoveState GetMoveState();
	bool IsHeadTouchBody();
	bool IsHeadTouchBlock(int width,int heiht);
	bool IsEatFruit(int x,int y);		//是否吃到(x,y)处的水果
	bool AddBody();	
	void SetHeadPos(int x,int y);
	void Move();
	SPoint * GetPos();
};
#endif