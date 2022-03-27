#include"chessboard.h"
#include<easyx.h>

//构造函数
Chessboard::Chessboard()
{
	//令int** 类型的chessboard指向一个二维数组
	chessboard = new int* [15];//含有15个int* 类型对象的数组
	for (int i(0); i < 15; i++) 
	{
		chessboard[i] = new int[15];//含有15个int类型对象的数组
	}
	for (int i(0); i < 15; i++)
	{
		for (int j(0); j < 15; j++)
		{
			chessboard[i][j] = 0;
		}
	}
}

//析构函数
Chessboard::~Chessboard()
{
	//释放动态内存
	for (int i(0); i < 15; i++)
	{
		delete[] chessboard[i];
	}
	delete[]chessboard;
}

//返回指向棋盘二维数组的指针
int** Chessboard::getchessboard()
{
	return chessboard;
}

//在界面上显示棋盘及背景
void Chessboard::display()
{
	IMAGE img1;
	IMAGE img2;
	loadimage(&img1, _T("背景1.png"));// 从图片中获取图像作为背景1
	putimage(0, 0, &img1, SRCPAINT);//绘制背景1到屏幕
	clearrectangle(50, 50, 550, 550);// 清空矩形区域，清空的区域用来显示棋盘
	loadimage(&img2, _T("棋盘.png"));// 从图片中获取图像作为棋盘
	putimage(50, 50, &img2, SRCPAINT);//绘制棋盘到屏幕
}