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
	int m_length;			//�ߵĳ���
	SPoint * m_pPos;		//ָ���ߵ�ÿһ������������ָ��
	MoveState m_dir;				//���ƶ��ķ���
	
public:
	CSnake();
	~CSnake();
	int GetLength();
	void ChangeMoveState(MoveState d);
	MoveState GetMoveState();
	bool IsHeadTouchBody();
	bool IsHeadTouchBlock(int width,int heiht);
	bool IsEatFruit(int x,int y);		//�Ƿ�Ե�(x,y)����ˮ��
	bool AddBody();	
	void SetHeadPos(int x,int y);
	void Move();
	SPoint * GetPos();
};
#endif