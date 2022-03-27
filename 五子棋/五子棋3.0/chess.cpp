#include"chess.h"
#include<graphics.h>
#include<windows.h>
#include<cstdio>

//构造函数
chess::chess(int a,int b)
{
	color = a;
	r = b;
}

//析构函数
chess::~chess(){}

//在界面上显示棋子
void chess::printchess(int i,int j)
{
	if (color == 1)
	{
		setfillcolor(BLACK);//填充颜色为黑色
		solidcircle(90 + (30 * i), 90 + (30 * j), r);//在该位置画一个无边框且半径为r像素的填充圆
	}
	else if (color == 2)
	{
		setfillcolor(WHITE);//填充颜色为白色
		solidcircle(90 + (30 * i), 90 + (30 * j), r);//在该位置画一个无边框且半径为r像素的填充圆
	}
}
