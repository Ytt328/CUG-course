#pragma once
#include"chessboard.h"
//窗口类
class game:public Chessboard
{
public:
	//构造函数
	game();

	//析构函数
	~game();

	//创建主窗口
	void creatmainwin();

	//创建人人对战窗口
	bool creatwin1();

	//创建人机对战窗口
	bool creatwin2();

	//创建结束窗口
	void createndwin();

	//开始游戏（人人对战）
	bool playgame1();

	//开始游戏（人机对战）
	bool playgame2();

	//判断是否有五子连成一线
	bool iswin(int, int, int);

	//是否继续游戏(人人对战)
	bool isagain1(int);

	//是否继续游戏（人机对战）
	bool isagain2(int);

	//输出五子棋盘的结果到文本中作为记录（人人对战）
	void print1(int);

	//输出五子棋盘的结果到文本中作为记录（人机对战）
	void print2(int);
};
