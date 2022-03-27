#pragma once
//棋子类
class chess
{
	int color;//1表示黑色，2表示白色
	int r;//棋子半径
public:
	//构造函数
	chess(int,int);

	//析构函数
	~chess();

	//在界面上显示棋子
	void printchess(int i, int j);
};
