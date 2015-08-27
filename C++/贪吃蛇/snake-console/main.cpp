#include<iostream>
#include"snake.h"
using std::cout;
int main(int argc,char *argv)
{
		system("color 4A");
		Snake sna;
		Apple fd;
		fd=product_food(fd);;

		//”Œœ∑—≠ª∑
		while(1)
		{
			
			if(sna.is_dead())
			{
				system("cls");
				GameOver();
				break;
			}
			if(sna.is_eat(fd))
			{
				fd=product_food(fd);
			}
			if(sna.is_win())
			{
				break;
			}
			sna.Move();
			
		}
	return 0;
}