#ifndef SNAKE_0xFFFFFFFFF_H
#define SNAKE_0xFFFFFFFFF_H


//ºê¶¨Òå

#define MAX 50
#define APPLE "¡î"
#define NODE  "¡ö"
#define SOUND  "\a"
#define Null   " "
#define cxClient 78
#define cyClient 24


struct Point
{
	int x;
	int y;
};

typedef  Point Apple;
void gotoxy(int x,int y);
void hidden();
void GameOver();
void GameWin();
Apple & product_food(Apple & app);

class Snake
{
private:
	Point Body[MAX];
	Point head;
	int len;
	int speed;
	char mode;
public:
	Snake();
	~Snake();
	void Move();
	bool is_dead();
	bool is_eat(Apple food);
	bool is_win();
};


#endif