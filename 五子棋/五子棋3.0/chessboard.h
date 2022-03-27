#pragma once
//棋盘类
class Chessboard
{
private:
	int** chessboard;//棋盘(15*15)
public:
	//构造函数
	Chessboard();

	//析构函数
	~Chessboard();

	//返回指向棋盘二维数组的指针
	int** getchessboard();

	//在界面上显示棋盘及背景
	void display();
};
