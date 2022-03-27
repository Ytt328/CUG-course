#include"game.h"
#include"chess.h"
#include<easyx.h>
#include<graphics.h>
#include<conio.h>
#include<windows.h>
#include<stdio.h>
#include<string>
#include<time.h>
#include<iostream>
#include<fstream>
#include<sstream>
#include<iomanip>
using namespace std;
//构造函数
game::game():Chessboard() {}

//析构函数
game::~game() {}

//创建主窗口
void game::creatmainwin()
{
	initgraph(600, 600);//初始化图形环境，600*600像素
	IMAGE img;//定义一个图像类对象
	setaspectratio(1.1f, 1);// 设置当前缩放因子
	loadimage(&img, _T("背景1.png"));//从图片文件中获取图像作为起始界面的背景
	putimage(0, 0, &img, SRCPAINT);// 绘制图像到屏幕
	settextstyle(50, 20, (LPCTSTR)"宋体");//设置字符平均高度50像素、平均宽度20像素，以及字体格式为宋体
	settextcolor(BLUE);//设置当前字体的颜色为蓝色
	setbkmode(WHITE);//设置背景混合模式
	outtextxy(180, 100, (LPCTSTR)"欢乐五子棋");//在指定位置输出“欢乐五子棋”
	settextstyle(40, 20, (LPCTSTR)"楷体");//设置字符平均高度为40像素、平均宽度为20像素，以及字体格式为楷体
	settextcolor(GREEN);//设置当前字体的颜色为绿色
	setbkmode(WHITE);//设置背景混合模式
	outtextxy(200, 350, (LPCTSTR)"人机对战");//在指定位置输出“人机对战”
	outtextxy(200, 400, (LPCTSTR)"人人对战");//在指定位置输出“开始游戏”
	outtextxy(200, 451, (LPCTSTR)"退出游戏");//在指定位置输出“退出游戏”
	outtextxy(200, 500, (LPCTSTR)"游戏规则");//在指定位置输出“游戏规则”
	MOUSEMSG m;//鼠标信息
	while (true) {
		m = GetMouseMsg();//获取一个鼠标消息
		if (m.uMsg == WM_MOUSEMOVE && m.x >= 200 && m.x <= 360 && m.y >= 350 && m.y <= 400) //判断鼠标是否移到了“人机对战”处
		{
			setlinecolor(YELLOW);
			rectangle(200, 350, 360, 400);//若鼠标在“人机对战”处，用黄色方框标记
		}
		else {
			setlinecolor(getbkcolor());
			rectangle(200, 350, 360, 400);//若鼠标在“人机对战”处，用背景填充色方框标记
		}
		if (m.uMsg == WM_MOUSEMOVE && m.x >= 200 && m.x <= 360 && m.y >= 401 && m.y <= 450) //判断鼠标是否移到了“人人对战”处
		{
			setlinecolor(YELLOW);
			rectangle(200, 400, 360, 450);//若鼠标在“人人对战”处，用黄色方框标记
		}
		else {
			setlinecolor(getbkcolor());
			rectangle(200, 400, 360, 450);//若鼠标在“人人对战”处，用背景填充色方框标记
		}
		if (m.uMsg == WM_MOUSEMOVE && m.x >= 200 && m.x <= 360 && m.y >= 451 && m.y <= 500) //判断鼠标是否移到了“结束游戏”处
		{
			setlinecolor(YELLOW);
			rectangle(200, 451, 360, 500);//若鼠标在“结束游戏”处，用黄色方框标记
		}
		else {
			setlinecolor(getbkcolor());
			rectangle(200, 451, 360, 500);//若鼠标在“结束游戏”处，用背景填充色方框标记
		}
		if (m.uMsg == WM_MOUSEMOVE && m.x >= 200 && m.x <= 360 && m.y >= 500 && m.y <= 550) //判断鼠标是否移到了“游戏规则”处
		{
			setlinecolor(YELLOW);
			rectangle(200, 500, 360, 550);//若鼠标在“游戏规则”处，用黄色方框标记
		}
		else {
			setlinecolor(getbkcolor());
			rectangle(200, 500, 360, 550);//若鼠标在“游戏规则”处，用背景填充色方框标记
		}
		if (m.uMsg == WM_LBUTTONDOWN && m.x >= 200 && m.x <= 360 && m.y >= 350 && m.y <= 400)//判断鼠标是否在“人机对战”处按下左键（单击）
		{
			//若点击“人机对战”
			//HWND hq = FindWindow(NULL, "欢乐五子棋");//获取父窗口句柄
			//int quit = MessageBox(hq, (LPCTSTR)"此功能尚未完全开发，仅处于能使用状态，是否继续游戏", (LPCTSTR)"友情提示", MB_YESNO);//弹出窗口，获取用户的选择
			cleardevice();//清屏
			Sleep(200);//暂停0.2秒
			bool Continue_Game = true;
			while (Continue_Game)
			{
				Continue_Game=creatwin2();//绘制人机对战的窗口,并根据返回值判断是否继续游戏
			}
			break;
		}
		if (m.uMsg == WM_LBUTTONDOWN && m.x >= 200 && m.x <= 360 && m.y >= 400 && m.y <= 450)//判断鼠标是否在“人人对战”处按下左键（单击）
		{
			//若点击“人人对战”
			setlinecolor(RED);
			rectangle(200, 400, 360, 450);//若鼠标在“开始游戏”处，用红色方框标记
			cleardevice();//清屏
			Sleep(200);//暂停0.2秒
			bool Continue_Game = true;
			while (Continue_Game)
			{
				Continue_Game=creatwin1();//绘制人人对战的界面，并根据返回值判断是否继续游戏
			}
			break;//若不继续游戏，则退出
		}
		if (m.uMsg == WM_LBUTTONDOWN && m.x >= 200 && m.x <= 360 && m.y >= 451 && m.y <= 500)//判断鼠标是否在“结束游戏”处按下左键（单击）
		{
			break;//单击“结束游戏”，退出
		}
		if (m.uMsg == WM_LBUTTONDOWN && m.x >= 200 && m.x <= 360 && m.y >= 500 && m.y <= 550)//判断鼠标是否在“游戏规则”处按下左键（单击）
		{
			HWND hq = FindWindow(NULL, "欢乐五子棋");//获取父窗口句柄
			int quit = MessageBox(hq, (LPCTSTR)"黑棋先行，率先连成五子的一方获胜", (LPCTSTR)"规则", MB_YESNO);//弹出窗口，获取用户的选择
			//break;
		}
	}
}

//创建人人对战的窗口
bool game::creatwin1()
{
	cleardevice();//清屏
	graphdefaults();// 重置所有绘图设置为默认值
	display();//显示背景棋盘
	setlinecolor(WHITE);//设置当前线条颜色为白色
	settextstyle(20, 10, (LPCTSTR)"黑体");//设置字符平均高度为20像素，平均宽度为10像素，字体格式为楷体
	settextcolor(BLACK);//设置文字颜色为黑色
	setbkmode(WHITE);//设置背景混合模式
	outtextxy(5, 5, (LPCTSTR)"玩家1：黑子");//在指定位置输出“玩家1：黑子”
	outtextxy(5, 28, (LPCTSTR)"步数：");//在指定位置输出“步数：”
	outtextxy(475, 5, (LPCTSTR)"玩家2：白子");//在指定位置输出“玩家2：白子”
	outtextxy(475, 28, (LPCTSTR)"步数：");//在指定位置输出“步数：”
	settextstyle(25, 15, (LPCTSTR)"黑体");//设置字符平均高度为25像素，平均宽度为15像素，字体格式为楷体
	outtextxy(475, 560, (LPCTSTR)"退出游戏");//在指定位置输出“退出游戏”
	outtextxy(10, 560, (LPCTSTR)"重新开始");//在指定位置输出“重新开始”
	outtextxy(270, 560, (LPCTSTR)"悔棋");//在指定位置输出“悔棋”
	//settextstyle(25, 15, (LPCTSTR)"楷体", 2700, 0, 0, 0, 0, 0);//设置字符高位25像素，宽为15像素，字体格式为楷体
	//outtextxy(25, 200, (LPCTSTR)"回到主界面");//在指定位置输出“黑方落子”
	return playgame1();
}

//创建人机对战的窗口
bool game::creatwin2()
{
	cleardevice();//清屏
	graphdefaults();// 重置所有绘图设置为默认值
	display();//显示背景与棋盘
	setlinecolor(WHITE);//设置当前线条颜色为白色
	settextstyle(20, 10, (LPCTSTR)"楷体");//设置字符平均高度为20像素，平均宽度为10像素，字体格式为楷体
	settextcolor(BLACK);//设置文字颜色为黑色
	setbkmode(WHITE);//设置背景混合模式
	outtextxy(5, 5, (LPCTSTR)"玩家：黑子");//在指定位置输出“玩家：黑子”
	outtextxy(5, 28, (LPCTSTR)"步数：");//在指定位置输出“步数：”
	outtextxy(475, 5, (LPCTSTR)"电脑：白子");//在指定位置输出“电脑：白子”
	outtextxy(475, 28, (LPCTSTR)"步数：");//在指定位置输出“步数：”
	settextstyle(25, 15, (LPCTSTR)"楷体");//设置字符平均高度为25像素，平均宽度为15像素，字体格式为楷体
	outtextxy(475, 560, (LPCTSTR)"退出游戏");//在指定位置输出“退出游戏”
	outtextxy(10, 560, (LPCTSTR)"重新开始");//在指定位置输出“重新开始”
	outtextxy(270, 560, (LPCTSTR)"悔棋");//在指定位置输出“悔棋”
	//outtextxy(25,200,(LPCTSTR)"回\n到\n主\n界\n面");
	return playgame2();
}

//创建结束窗口
void game::createndwin()
{
	cleardevice();//清屏
	IMAGE img;//定义一个图像类的对象
	loadimage(&img, _T("背景2.png"));//从图片文件获取图像
	putimage(0, 0, &img, SRCPAINT);//在窗口上显示图像
	settextcolor(BLACK);//选择文本颜色为黑色
	settextstyle(50, 20, (LPCTSTR)"宋体");//高50像素，宽20像素，宋体
	outtextxy(160, 80, (LPCTSTR)"欢迎下次再来！");//在窗口输出“欢迎下次再来”
	Sleep(1500);//暂停1.5秒
}

//开始游戏（人人对战）
bool game::playgame1()
{
	int** board = getchessboard();
	int player1win = 0;//玩家1赢的次数
	int player2win = 0;//玩家2赢的次数
	IMAGE img1;
	IMAGE img2;
	loadimage(&img1, _T("背景1.png"));
	loadimage(&img2, _T("棋盘.jpg"));
	settextstyle(25, 15, (LPCTSTR)"楷体");//设置字符高为25像素，宽为15像素，字体格式为楷体
	string str = to_string(player1win) + ":" + to_string(player2win);//to_string:convert int to string,将整数转换为字符串
	outtextxy(265, 15, (LPCTSTR)str.data());//将str内容输出到界面的指定位置      str.date():以字符数组的形式返回字符串的内容
	int StepNum1 = 0, StepNum2 = 0;//玩家1当前所走步数，玩家2当前所走步数
	settextstyle(20, 10, (LPCTSTR)"楷体");//设置字符高位20像素，宽为10像素，字体格式为楷体
	outtextxy(60, 27, (LPCTSTR)to_string(StepNum1).data());//在指定位置输出玩家1的步数
	outtextxy(530, 27, (LPCTSTR)to_string(StepNum2).data());//在指定位置输出玩家2的步数
	settextstyle(25, 15, (LPCTSTR)"楷体", 2700, 0, 0, 0, 0, 0);//设置字符高位25像素，宽为15像素，字体格式为楷体
	outtextxy(25, 50, (LPCTSTR)"黑方落子");//在指定位置输出“黑方落子”
	int Pre_i = -1, Pre_j = -1, Pre_C = -1, Pre_R = -1;
	int player = 1;
	bool UndoAble = true;
	MOUSEMSG msg;//鼠标信息
	while (true)
	{
		msg = GetMouseMsg();//取得鼠标信息
		//退出游戏
		if (msg.uMsg == WM_MOUSEMOVE && msg.x >= 475 && msg.x <= 595 && msg.y >= 560 && msg.y <= 585)//判断鼠标是否移到了“退出游戏”
		{
			setlinecolor(RED);
			rectangle(475, 560, 595, 585);//若移动到了“退出游戏”，用红色框标记
		}
		else
		{
			setlinecolor(BLACK);
			rectangle(475, 560, 595, 585);//未移到“退出游戏”，用黑色框标记
		}
		if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 460 && msg.x <= 580 && msg.y >= 550 && msg.y <= 575)//判断鼠标是否点击了“退出游戏”
		{
			return false;//点击了退出游戏，返回false
		}
		//重新开始
		if (msg.uMsg == WM_MOUSEMOVE && msg.x >= 10 && msg.x <= 130 && msg.y >= 560 && msg.y <= 585)//判断鼠标是否移到了“重新开始”
		{
			setlinecolor(RED);
			rectangle(10, 560, 130, 585);//若移到了“重新开始”，用红色框标记
		}
		else
		{
			setlinecolor(BLACK);
			rectangle(10, 560, 130, 585); //未移到“重新开始”，用黑色框标记
		}
		if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 10 && msg.x <= 130 && msg.y >= 560 && msg.y <= 585)//判断是否点击了“重新开始”
		{
			player1win = 0;
			player2win = 0;
			return true;
			//点击到了“重新开始”，返回true
		}
		//回到主界面
		/*if (msg.uMsg == WM_MOUSEMOVE && msg.x >= 25 && msg.x <= 40 && msg.y >= 200 && msg.y <= 325)
		{
			setlinecolor(RED);
			rectangle(25, 200, 40, 325);
		}
		else 
		{
			setlinecolor(BLACK);
			rectangle(25, 200, 40, 325);
		}*/
		//悔棋
		if (msg.uMsg == WM_MOUSEMOVE && msg.x >= 270 && msg.x <= 330 && msg.y >= 560 && msg.y <= 585)//判断鼠标是否移到了“悔棋”
		{
			setlinecolor(RED);
			rectangle(270, 560, 330, 585);
			//若鼠标移到了“悔棋”，用红色框标记
		}
		else
		{
			setlinecolor(BLACK);
			rectangle(270, 560, 330, 585);
			//若鼠标未移到“悔棋”，用黑色框标记
		}
		if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 270 && msg.x <= 330 && msg.y >= 560 && msg.y <= 585)//判断鼠标是否点击了“悔棋”
		{
			HWND hq = FindWindow(NULL, "欢乐五子棋");//获取父窗口句柄
			if (Pre_i == -1)
			{
				MessageBox(hq, (LPCTSTR)"目前还不能悔棋", "提示", MB_OK);
			}
			else if (UndoAble)
			{
				UndoAble = false;
				board[Pre_i][Pre_j] = 0;
				clearcircle(Pre_C, Pre_R, 12);//清空圆心区域，上一步棋消失
				putimage(Pre_C - 12, Pre_R - 12, 25, 25, &img2, Pre_C - 12 - 50, Pre_R - 12 - 50, SRCCOPY);//将棋盘还原成上一步的样子
				if (player == 1)//此时轮到玩家1下棋但玩家1还没下，玩家2选择悔棋
				{
					settextstyle(20, 10, (LPCTSTR)"楷体");//设置字符高为20像素，宽为10像素，字体样式为楷体
					putimage(530, 27, 20, 20, &img1, 530, 27, SRCCOPY);
					outtextxy(530, 27, (LPCTSTR)to_string(--StepNum2).data());//玩家2步数减一
					settextstyle(25, 15, (LPCTSTR)"楷体", 2700, 0, 0, 0, 0, 0);//设置字符高25像素，宽15像素，字体样式为楷体
					outtextxy(575, 50, (LPCTSTR)"白方落子");//在指定位置输出“白方落子”
					settextstyle(25, 15, (LPCTSTR)"楷体", 0, 0, 0, 0, 0, 0);//设置字符高25像素，宽15像素，字体样式为楷体
					clearrectangle(10, 70, 40, 150);//清空矩形区域
					putimage(10, 70, 30 + 1, 100 + 1, &img1, 10, 70, SRCCOPY);
					player = 2;//转到玩家2
				}
				else//此时轮到玩家2下棋但玩家2还没下，玩家1选择悔棋
				{
					settextstyle(20, 10, (LPCTSTR)"楷体");//设置字符高为20像素，宽为10像素，字体样式为楷体
					putimage(60, 27, 20, 20, &img1, 60, 27, SRCCOPY);
					outtextxy(60, 27, (LPCTSTR)to_string(--StepNum1).data());//玩家1步数减一
					settextstyle(25, 15, (LPCTSTR)"楷体", 2700, 0, 0, 0, 0, 0);//设置字符高25像素，宽15像素，字体样式为楷体
					outtextxy(25, 50, (LPCTSTR)"黑方落子");//在指定位置输出“黑方落子”
					settextstyle(25, 15, (LPCTSTR)"楷体", 0, 0, 0, 0, 0, 0);//设置字符高25像素，宽15像素，字体样式为楷体
					clearrectangle(560, 70, 590, 150);//清空矩形区域
					putimage(560, 70, 30 + 1, 100 + 1, &img1, 560, 70, SRCCOPY);
					player = 1;//转到玩家1
				}
			}
			else
			{
				MessageBox(hq, (LPCTSTR)"最多连续悔棋一次", "已经悔过棋了", MB_OK);
			}
		}
		//棋盘区域
		if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 80 && msg.x <= 520 && msg.y >= 80 && msg.y <= 520)//判断是否在棋盘区域按下左键
		{
			int i = 0, j = 0;//对于鼠标所在的点在棋盘数组中的位应
			for (; i < 15; i++)
			{
				if (msg.y <= 90 + i * 30 + 10 && msg.y >= 90 + i * 30 - 10)//检测鼠标所在行
				{
					for (; j < 15; j++)
					{
						if (msg.x <= 90 + j * 30 + 10 && msg.x >= 90 + j * 30 - 10)//检测鼠标所在列
						{
							UndoAble = true;
							if (player == 1 && board[i][j] == 0) 
							{//此时是玩家1在下棋且鼠标所指出没有棋子
								chess chess1(1, 12);//定义一个棋子 黑色，半径为12像素
								chess1.printchess(j, i);//在该点画一个黑棋
								board[i][j] = 1;//标记该点已有黑棋
								settextstyle(20, 10, (LPCTSTR)"楷体", 0, 0, 0, 0, 0, 0);//设置字符高20，宽10，楷体
								putimage(60, 27, 20, 20, &img1, 60, 27, SRCCOPY);
								outtextxy(60, 27, (LPCTSTR)to_string(++StepNum1).data());//玩家1步数加一
								settextstyle(25, 15, (LPCTSTR)"楷体", 2700, 0, 0, 0, 0, 0);//高25个像素，宽15个像素
								outtextxy(575, 50, (LPCTSTR)"白方落子");//在界面上显示“白方落子”
								settextstyle(25, 15, (LPCTSTR)"楷体", 0, 0, 0, 0, 0, 0);//高25个像素，宽15个像素
								clearrectangle(10, 70, 40, 150);//清空矩形区域
								putimage(10, 70, 30 + 1, 100 + 1, &img1, 10, 70, SRCCOPY);
								if (iswin(i, j, 1))//判断玩家1是否有五子连线
								{
									//若玩家1有五子连成一线
									settextstyle(25, 15, (LPCTSTR)"楷体");//高25个像素，宽15个像素
									putimage(265, 15, 75, 25, &img1, 265, 15, SRCCOPY);
									str = to_string(++player1win) + " : " + to_string(player2win);//玩家1赢的次数加一
									outtextxy(265, 15, (LPCTSTR)str.data());
									return isagain1(1);//判断是否赢及是否继续玩
								}
								player = 2;//转到玩家2
								break;
							}
							else if (player == 2 && board[i][j] == 0)
							{
								//此时是玩家2下棋且鼠标所指处没有棋子
								chess chess1(2, 12);//定义一个棋子 白色，半径为12像素
								chess1.printchess(j, i);//在该点画一个白棋
								board[i][j] = 2;//标记该点已有白棋
								settextstyle(20, 10, (LPCTSTR)"楷体", 0, 0, 0, 0, 0, 0); //设置字符高20，宽10，楷体
								putimage(530, 27, 20, 20, &img1, 530, 27, SRCCOPY);
								outtextxy(530, 27, (LPCTSTR)to_string(++StepNum2).data());//玩家2步数加1
								settextstyle(25, 15, (LPCTSTR)"楷体", 2700, 0, 0, 0, 0, 0);//高25个像素，宽15个像素
								outtextxy(25, 50, (LPCTSTR)"黑方落子");//在界面上显示“黑方落子”
								settextstyle(25, 15, (LPCTSTR)"楷体", 0, 0, 0, 0, 0, 0);//高25个像素，宽15个像素
								clearrectangle(560, 70, 590, 150);//清空矩形区域
								putimage(560, 70, 30 + 1, 100 + 1, &img1, 560, 70, SRCCOPY);
								if (iswin(i, j, 2))//判断玩家2是否有五子连线
								{
									//若玩家1有五子连成一线
									settextstyle(25, 15, (LPCTSTR)"楷体");//高25个像素，宽15个像素
									putimage(265, 15, 75, 25, &img1, 265, 15, SRCCOPY);
									str = to_string(player1win) + " : " + to_string(++player2win);//玩家2赢的次数加一
									outtextxy(265, 15, (LPCTSTR)str.data());
									return isagain1(2);//判断是否赢及是否继续玩
								}
								player = 1;//转到玩家1
								break;

							}
						}
					}
				}
			}
		}
	}
}

//开始游戏（人机对战）
bool game::playgame2()
{
	int** board = getchessboard();
	int playerwin = 0;
	int computerwin = 0;
	IMAGE img1;
	IMAGE img2;
	loadimage(&img1, _T("背景1.png"));
	loadimage(&img2, _T("棋盘.jpg"));
	settextstyle(25, 15, (LPCTSTR)"楷体");//设置字符高为25像素，宽为15像素，字体格式为楷体
	string str = to_string(playerwin) + ":" + to_string(computerwin);//to_string:convert int to string,将整数转换为字符串
	outtextxy(265, 15, (LPCTSTR)str.data());//将str内容输出到界面的指定位置      str.date():以字符数组的形式返回字符串的内容
	int PlayerNum = 0, ComputerNum = 0;//玩家当前所走步数，电脑当前所走步数
	settextstyle(20, 10, (LPCTSTR)"楷体");//设置字符高位20像素，宽为10像素，字体格式为楷体
	outtextxy(60, 27, (LPCTSTR)to_string(PlayerNum).data());//在指定位置输出玩家的步数
	outtextxy(530, 27, (LPCTSTR)to_string(ComputerNum).data());//在指定位置输出玩家2的步数
	settextstyle(25, 15, (LPCTSTR)"楷体", 2700, 0, 0, 0, 0, 0);//设置字符高位25像素，宽为15像素，字体格式为楷体
	outtextxy(25, 50, (LPCTSTR)"黑方落子");//在指定位置输出“黑方落子”
	int Pre_i = -1, Pre_j = -1, Pre_C = -1, Pre_R = -1;
	int player = 1;
	bool UndoAble = true;
	MOUSEMSG msg;//鼠标信息
	while (true)
	{
		msg = GetMouseMsg();//取得鼠标信息
		//退出游戏
		if (msg.uMsg == WM_MOUSEMOVE && msg.x >= 475 && msg.x <= 595 && msg.y >= 560 && msg.y <= 585)//判断鼠标是否移到了“退出游戏”
		{
			setlinecolor(RED);
			rectangle(475, 560, 595, 585);//若移动到了“退出游戏”，用红色框标记
		}
		else
		{
			setlinecolor(BLACK);
			rectangle(475, 560, 595, 585);//未移到“退出游戏”，用黑色框标记
		}
		if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 460 && msg.x <= 580 && msg.y >= 550 && msg.y <= 575)//判断鼠标是否点击了“退出游戏”
		{
			return false;//点击了退出游戏，返回false
		}
		//重新开始
		if (msg.uMsg == WM_MOUSEMOVE && msg.x >= 10 && msg.x <= 130 && msg.y >= 560 && msg.y <= 585)//判断鼠标是否移到了“重新开始”
		{
			setlinecolor(RED);
			rectangle(10, 560, 130, 585);//若移到了“重新开始”，用红色框标记
		}
		else
		{
			setlinecolor(BLACK);
			rectangle(10, 560, 130, 585); //未移到“重新开始”，用黑色框标记
		}
		if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 10 && msg.x <= 130 && msg.y >= 560 && msg.y <= 585)//判断是否点击了“重新开始”
		{
			playerwin = 0;
			computerwin = 0;
			return true;
			//点击到了“重新开始”，返回true
		}
		//悔棋
		if (msg.uMsg == WM_MOUSEMOVE && msg.x >= 270 && msg.x <= 330 && msg.y >= 560 && msg.y <= 585)//判断鼠标是否移到了“悔棋”
		{
			setlinecolor(RED);
			rectangle(270, 560, 330, 585);
			//若鼠标移到了“悔棋”，用红色框标记
		}
		else
		{
			setlinecolor(BLACK);
			rectangle(270, 560, 330, 585);
			//若鼠标未移到“悔棋”，用黑色框标记
		}
		if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 270 && msg.x <= 330 && msg.y >= 560 && msg.y <= 585)//判断鼠标是否点击了“悔棋”
		{
			HWND hq = FindWindow(NULL, "欢乐五子棋");//获取父窗口句柄
			if (Pre_i == -1)
			{
				MessageBox(hq, (LPCTSTR)"目前还不能悔棋", "提示", MB_OK);//弹出消息框
			}
			else if (UndoAble)
			{
				UndoAble = false;
				board[Pre_i][Pre_j] = 0;
				clearcircle(Pre_C, Pre_R, 12);//清空圆心区域，上一步棋消失
				putimage(Pre_C - 12, Pre_R - 12, 25, 25, &img2, Pre_C - 12 - 50, Pre_R - 12 - 50, SRCCOPY);//将棋盘还原成上一步的样子
				if (player == 1)//此时轮到玩家下棋但玩家还没下，电脑不可悔棋
				{
					MessageBox(hq, (LPCTSTR)"电脑不支持悔棋", "提示", MB_OK);
					player = 2;//转到电脑
				}
				else//此时轮到电脑下棋但电脑还没下棋，玩家选择悔棋
				{
					settextstyle(20, 10, (LPCTSTR)"楷体");//设置字符高为20像素，宽为10像素，字体样式为楷体
					putimage(60, 27, 20, 20, &img1, 60, 27, SRCCOPY);//
					outtextxy(60, 27, (LPCTSTR)to_string(--PlayerNum).data());//玩家步数减一
					settextstyle(25, 15, (LPCTSTR)"楷体", 2700, 0, 0, 0, 0, 0);//设置字符高25像素，宽15像素，字体样式为楷体
					outtextxy(25, 50, (LPCTSTR)"黑方落子");//在指定位置输出“黑方落子”
					settextstyle(25, 15, (LPCTSTR)"楷体", 0, 0, 0, 0, 0, 0);//设置字符高25像素，宽15像素，字体样式为楷体
					clearrectangle(560, 70, 590, 150);//清空矩形区域
					putimage(560, 70, 30 + 1, 100 + 1, &img1, 560, 70, SRCCOPY);//
					player = 1;//转到玩家2
				}
			}
			else
			{
				MessageBox(hq, (LPCTSTR)"最多连续悔棋一次", "已经悔过棋了", MB_OK);
			}
		}
		//棋盘区域
		if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 50 && msg.x <= 550 && msg.y >= 50 && msg.y <= 550)//若玩家按下左键
		{
			int i = 0, j = 0;
			for (; i < 15; i++)
			{
				if (msg.y <= 90 + i * 30 + 10 && msg.y >= 90 + i * 30 - 10)//检测鼠标所在行
				{
					for (; j < 15; j++)
					{
						if (msg.x <= 90 + j * 30 + 10 && msg.x >= 90 + j * 30 - 10)//检测鼠标所在列
						{
							UndoAble = true;//可以悔棋
							if (player == 1 && board[i][j] == 0)
							{
								chess chess1(1, 12);//定义一个棋子，黑色，半径为12像素
								chess1.printchess(j, i);//在该点画一个黑棋
								board[i][j] = 1;//标记该点已有黑棋
								settextstyle(20, 10, (LPCTSTR)"楷体", 0, 0, 0, 0, 0, 0);//设置字符高20，宽10，楷体
								putimage(60, 27, 20, 20, &img1, 60, 27, SRCCOPY);
								outtextxy(60, 27, (LPCTSTR)to_string(++PlayerNum).data());//玩家步数加一
								settextstyle(25, 15, (LPCTSTR)"楷体", 2700, 0, 0, 0, 0, 0);//高25个像素，宽15个像素
								outtextxy(575, 50, (LPCTSTR)"白方落子");//在界面上显示“白方落子”
								settextstyle(25, 15, (LPCTSTR)"楷体", 0, 0, 0, 0, 0, 0);//高25个像素，宽15个像素
								clearrectangle(10, 70, 40, 150);//清空矩形区域
								putimage(10, 70, 30 + 1, 100 + 1, &img1, 10, 70, SRCCOPY);
								if (iswin(i, j, 1))//判断玩家是否有五子连线
								{
									//若玩家1有五子连成一线
									settextstyle(25, 15, (LPCTSTR)"楷体");//高25个像素，宽15个像素
									putimage(265, 15, 75, 25, &img1, 265, 15, SRCCOPY);//将比分处清空
									str = to_string(++playerwin) + " : " + to_string(computerwin);//玩家1赢的次数加一
									outtextxy(265, 15, (LPCTSTR)str.data());//重新输出比分
									return isagain2(1);//判断是否赢及是否继续玩
								}
								else {
									//实现电脑下棋
									Sleep(1000);//暂停一秒
									player = 2;//转到电脑
									int a = -1, b = -1;//电脑下棋点
									//int a1 = 0, b1 = 0, a2 = 0, b2 = 0;
									//检查当前自身白子的情况
									if (a == -1) {
										for (int i(0); i < 15; i++)
										{
											for (int j(0); j < 15; j++)
											{
												//遍历寻找白棋
												if (board[i][j] == 2)
												{
													int a1 = 0, b1 = 0, a2 = 0, b2 = 0;
													int count = 1;//标记此时连在一起白棋个数为1
													//纵向查找是否有四子
													for (int x = i - 1; x >= 0 && board[x][j] == 2; x--)//
													{
														count++;
														a1 = x;
													}
													for (int x = i + 1; x < 15 && board[x][j] == 2; x++)
													{
														count++;
														a2 = x;
													}
													if (count == 4)//已有四子连成一线
													{
														if (a1 > 0 && board[a1-1][j] == 0)
														{
															a = a1-1;
															b = j;
															break;
														}
														if (a2 < 14 && board[a2+1 ][j] == 0)
														{
															a = a2+1;
															b = j;
															break;
														}
													}
													if (count == 3 && a1 > 0 && board[a1 - 1][j] == 0 && a2 < 14 && board[a2 + 1][j] != 0)
													{
														a = a1 - 1;
														b = j;
														break;
													}
													if (count == 3 && a2 < 14 && board[a2 + 1][j] == 0 && a1 > 0 && board[a1 - 1][j] != 0)
													{
														a = a2 + 1;
														b = j;
														break;
													}
													else if (count == 3 && a1 > 0 && board[a1-1][j] == 0 && a2  < 14 && board[a2+1][j] == 0)
													{
														a = a1-1;
														b = j;
														break;
													}
													else
													{
														a = -2;
														//b = -2;
														//break;
													}
												}
												//if (i == 14 && j == 14)
												//{
												//	a = -2;
												//	//b = -2;
												//}
											}
										}
									}
									if (a == -2 ) {
										for (int i(0); i < 15; i++)
										{
											for (int j(0); j < 15; j++)
											{
												//遍历寻找白棋
												if (board[i][j] == 2)
												{
													int a1 = 0, b1 = 0, a2 = 0, b2 = 0;
													int count = 1;//标记此时连在一起白棋个数为1
													//横向查找是否有四子
													for (int y = j - 1; y >= 0 && board[i][y] == 2; y--)
													{
														count++;
														b1 = y;
													}
													for (int y = j + 1; y < 15 && board[i][y] == 2; y++)
													{
														count++;
														b2 = y;
													}
													if (count == 4)//已有四子连成一线
													{
														if (b1 > 0 && board[i][b1-1] == 0)
														{
															a = i;
															b = b1-1 ;
															break;
														}
														if (b2 < 14 && board[i][b2+1 ] == 0)
														{
															a = i;
															b = b2+1 ;
															break;
														}
													}
													if (count == 3 && b1 > 0 && board[i][b1 - 1] == 0 && b2 < 14 && board[i][b2 + 1] != 0)
													{
														a = i;
														b = b1 - 1;
														break;
													}
													if (count == 3 && b1 > 0 && board[i][b1 - 1] != 0 && b2 < 14 && board[i][b2 + 1] == 0)
													{
														a = i;
														b = b2 + 1;
														break;
													}
													else if (count == 3 && b1  > 0 && board[i][b1-1] == 0 && b2  < 14 && board[i][b2+1 ] == 0)
													{
														a = i;
														b = b1-1 ;
														break;
													}
													else
													{
														a = -3;
														//b = -3;
														//break;
													}
												}
												//if (i == 14 && j == 14)
												//{
												//	a = -3;
												//	//b = -3;
												//}
											}
										}
									}
									if (a == -3 )
									{
										for (int i(0); i < 15; i++)
										{
											for (int j(0); j < 15; j++)
											{
												//遍历寻找白棋
												if (board[i][j] == 2)
												{
													//int a1 = 0, b1 = 0, a2 = 0, b2 = 0;
													int count = 1;//标记此时连在一起白棋个数为1
													//左对角线检查										    
													int a1 = 0, b1 = 0, a2 = 0, b2 = 0;
													
													for (int x = i - 1,  y = j - 1; x >= 0 && y >= 0 && board[x][y] == 2; x--, y--)
													{
														count++;
														a1 = x;
														b1 = y;
													}
													for (int x = i + 1, y = j + 1; x < 15 && y < 15 && board[x][y] == 2; x++, y++)
													{
														count++;
														a2 = x;
														b2 = y;
													}
													if (count == 4)//已有四子连成一线
													{
														if (a1 > 0 && b1 > 0 && board[a1-1][b1-1] == 0)
														{
															a = a1-1;
															b = b1-1;
															break;
														}
														if (a2 < 14 && b2 < 14 && board[a2+1][b2+1] == 0)
														{
															a = a2+1 ;
															b = b2+1;
															break;
														}
													}
													if (count == 3 && a1 > 0 && b1 > 0 && board[a1 - 1][b1 - 1] == 0 && a2 < 14 && b2 < 14 && board[a1 + 1][b2 + 1] != 0)
													{
														a = a1 - 1;
														b = b1 - 1;
														break;
													}
													if (count == 3 && a1 > 0 && b1 > 0 && board[a1 - 1][b1 - 1] != 0 && a2 < 14 && b2 < 14 && board[a1 + 1][b2 + 1] == 0)
													{
														a = a1 + 1;
														b = b1 + 1;
														break;
													}
													else if (count == 3 && a1  > 0 && b1 > 0 && board[a1-1][b1-1] == 0 && a2 < 14 && b2  < 14 && board[a1+1][b2+1] == 0)
													{
														a = a1 - 1;
														b = b1 - 1;
														break;
													}
													else
													{
														a = -4;
														//b = -4;
														//break;
													}
												}
												//if (i == 14 && j == 14)
												//{
												//	a = -4;
												//	//b = -4;
												//}
											}
										}
									}
									if (a == -4 ) {
										for (int i(0); i < 15; i++)
										{
											for (int j(0); j < 15; j++)
											{
												//遍历寻找白棋
												if (board[i][j] == 2)
												{
													int a1 = 0, b1 = 0, a2 = 0, b2 = 0;
													int count = 1;//标记此时连在一起白棋个数为1
													//右对角线检查
													for (int x = i - 1,  y = j + 1; x >= 0 && y < 15 && board[x][y] == 2; x--, y++)
													{
														count++;
														a1 = x;
														b1 = y;
													}
													for (int x = i + 1, y = j - 1; x < 15 && y >= 0 && board[x][y] == 2; x++, y--)
													{
														count++;
														a2 = x;
														b2 = y;
													}
													if (count == 4)//已有四子连成一线
													{
														if (a1 > 0 && b1 < 14 && board[a1-1][b1+1] == 0)
														{
															a = a1-1;
															b = b1+1;
															break;
														}
														if (a2 < 14 && b2 > 0 && board[a2+1][b2-1] == 0)
														{
															a = a2+1 ;
															b = b2-1;
															break;
														}
													}
													if (count == 3 && a1 > 0 && b1 < 14 && board[a1 - 1][b1 + 1] == 0 && a2 < 14 && b2  > 0 && board[a2 + 1][b2 - 1] != 0)
													{
														a = a1 - 1;
														b = b1 + 1;
														break;
													}
													if (count == 3 && a1 > 0 && b1 < 14 && board[a1 - 1][b1 + 1] != 0 && a2 < 14 && b2  > 0 && board[a2 + 1][b2 - 1] == 0)
													{
														a = a1 + 1;
														b = b1 - 1;
														break;
													}
													else if (count == 3 && a1  > 0 && b1  < 14 && board[a1-1 ][b1+1] == 0 && a2 <14 && b2  > 0 && board[a2+1 ][b2-1] == 0)
													{
														a = a1-1 ;
														b = b1+1 ;
														break;
													}
												}
											}
										}
					                }
									//若上面的遍历中未发现白子有连续四子或连续三子的清况

									if (a < 0 || b < 0)
									{
										int a1 = 0, b1 = 0, a2 = 0, b2 = 0;
										int black = 1;//连在一条线上的黑棋个数
										//检查对方黑子的清况
										//检查黑子是否有四子连线以及三子连线
										//纵向

										for (int x = i - 1; x >= 0 && board[x][j] == 1; x--)
										{
											black++;
											a1 = x;
										}
										for (int x = i + 1; x < 15 && board[x][j] == 1; x++)
										{
											black++;
											a2 = x;
										}
										if (black == 4)
										{
											if (a1 > 0 && board[a1-1][j] == 0)
											{
												a = a1-1 ;
												b = j;
											}
											else if (a2 < 14 && board[a2+1 ][j] == 0)
											{
												a = a2+1 ;
												b = j;
											}
										}
										if (black == 3 && a1 > 0 && board[a1 - 1][j] == 0 && a2 < 14 && board[a2 + 1][j] != 0)
										{
											a = a1 - 1;
											b = j;
										}
										if (black == 3 && a1 > 0 && board[a1 - 1][j] != 0 && a2 < 14 && board[a2 + 1][j] == 0)
										{
											a = a2 + 1;
											b = j;
										}
										else if (black == 3 && a1  > 0 && board[a1-1][j] == 0 && a2  < 14 && board[a2+1 ][j] == 0)
										{
											a = a1-1;
											b = j;
										}
									}
									//若经过上诉过程计算机还未确定下棋位置
									if (a < 0 || b < 0)
									{
										int a1 = 0, b1 = 0, a2 = 0, b2 = 0;
										int black = 1;//连在一条线上的黑棋个数
										//检查对方黑子的清况
										//检查黑子是否有四子连线以及三子连线
										//横向
										for (int y = j - 1; y >= 0 && board[i][y] == 1; y--)
										{
											black++;
											b1 = y;
										}
										for (int y = j + 1; y < 15 && board[i][y] == 1; y++)
										{
											black++;
											b2 = y;
										}
										if (black == 4)
										{
											if (b1 > 0 && board[i][b1-1] == 0)
											{
												a = i;
												b = b1-1;
											}
											else if (b2 < 14 && board[i][b2+1] == 0)
											{
												a = i;
												b = b2+1;
											}
										}
										if (black == 3 && b1 > 0 && board[i][b1 - 1] == 0 && b2 < 14 && board[i][b2 + 1] != 0)
										{
											a = i;
											b = b1 - 1;
										}
										if (black == 3 && b1 > 0 && board[i][b1 - 1] != 0 && b2 < 14 && board[i][b2 + 1] == 0)
										{
											a = i;
											b = b2 + 1;
										}
										else if (black == 3 && b1  > 0 && board[i][b1-1] == 0 && b2 < 14 && board[i][b2+1] == 0)
										{
											a = i;
											b = b1-1;
										}
									}
									//若经过上诉过程计算机还未确定下棋位置
									if (a < 0 || b < 0)
									{
										int a1 = 0, b1 = 0, a2 = 0, b2 = 0;
										int black = 1;//连在一条线上的黑棋个数
										//检查对方黑子的清况
										//检查黑子是否有四子连线以及三子连线
										//左对角线方向
										for (int x=i-1, y = j - 1; x >= 0&&y >= 0 && board[x][y] == 1; x--,y--)
										{
											black++;
											a1 = x;
											b1 = y;
										}
										for (int x=i+1, y = j + 1; x < 15&&y < 15 && board[x][y] == 1; x++,y++)
										{
											black++;
											a2 = x;
											b2 = y;
										}
										if (black == 4)
										{
											if (a1 > 0&&b1 > 0 && board[a1-1][b1-1] == 0)
											{
												a = a1-1 ;
												b = b1-1 ;
											}
											else if (a2<14&&b2 < 14 && board[a2+1][b2+1] == 0)
											{
												a = a2+1;
												b = b2+1;
											}
										}
										if (black == 3 && a1 > 0 && b1 > 0 && board[a1 - 1][b1 - 1] == 0 && a2 < 14 && b2 < 14 && board[a2 + 1][b2 + 1] != 0)
										{
											a = a1 - 1;
											b = b1 - 1;
										}
										if (black == 3 && a1 > 0 && b1 > 0 && board[a1 - 1][b1 - 1] != 0 && a2 < 14 && b2 < 14 && board[a2 + 1][b2 + 1] == 0)
										{
											a = a2 + 1;
											b = b2 + 1;
										}
										else if (black == 3 && a1>0&&b1 > 0 && board[a1-1][b1-1] == 0 && a2<14&&b2 < 14 && board[a2+1][b2+1] == 0)
										{
											a = a1-1;
											b = b1-1;
										}
									}
									//若经过上诉过程计算机还未确定下棋位置
									if (a < 0 || b < 0)
									{
										int a1 = 0, b1 = 0, a2 = 0, b2 = 0;
										int black = 1;//连在一条线上的黑棋个数
										//检查对方黑子的清况
										//检查黑子是否有四子连线以及三子连线
										//右对角线方向
										for (int x = i - 1,  y = j + 1; x >= 0 && y < 15 && board[x][y] == 1; x--, y++)
										{
											black++;
											a1 = x;
											b1 = y;
										}
										for (int x = i + 1,  y = j - 1; x < 15 && y >=0 && board[x][y] == 1; x++, y--)
										{
											black++;
											a2 = x;
											b2 = y;
										}
										if (black == 4)
										{
											if (a1 > 0 && b1 <14  && board[a1-1 ][b1+1 ] == 0)
											{
												a = a1-1 ;
												b = b1+1;
											}
											else if (a2 < 14 && b2 > 0 && board[a2+1][b2-1] == 0)
											{
												a = a2 +1;
												b = b2 -1;
											}
										}
										if (black == 3 && a1 > 0 && b1 < 14 && board[a1 - 1][b1 + 1] == 0 && a2 < 14 && b2  >0 && board[a2 + 1][b2 - 1] != 0)
										{
											a = a1 - 1;
											b = b1 + 1;
										}
										if (black == 3 && a1 > 0 && b1 < 14 && board[a1 - 1][b1 + 1] != 0 && a2 < 14 && b2  >0 && board[a2 + 1][b2 - 1] == 0)
										{
											a = a2 + 1;
											b = b2 - 1;
										}
										else if (black == 3 && a1  > 0 && b1  < 14 && board[a1-1][b1+1 ] == 0 && a2  < 14 && b2  >0 && board[a2+1 ][b2-1] == 0)
										{
											a = a1-1;
											b = b1+1;
										}
									}
									if(a<0||b<0)
									{
										//a = 0;
										//b = 0;
										srand((unsigned int) time (0));
										if (!board[i - 1][j] ||!board[i-1][j+1]||!board[i-1][j-1]|| !board[i + 1][j] ||!board[i+1][j+1]||!board[i+1][j-1]|| !board[i][j - 1] || !board[i][j + 1])
										{
											for (int x(i - 1); x <= i + 1; x++)
											{
												for (int y(j - 1); y <= j + 1; y++)
												{
													if (board[x][y] == 0)
													{
														a = x;
														b = y;
														break;
													}
												}
											}
										}
										else
										{
											a = rand() % 15;
											b = rand() % 15;
											while (board[a][b] != 0)
											{
												a = rand() % 15;
												b = rand() % 15;
											}
										}

									}									
									chess chess2(2, 12);
									chess2.printchess(b, a);
									board[a][b] = 2;
									settextstyle(20, 10, (LPCTSTR)"楷体", 0, 0, 0, 0, 0, 0); //设置字符高20，宽10，楷体
									putimage(530, 27, 20, 20, &img1, 530, 27, SRCCOPY);
									outtextxy(530, 27, (LPCTSTR)to_string(++ComputerNum).data());
									settextstyle(25, 15, (LPCTSTR)"楷体", 2700, 0, 0, 0, 0, 0);//高25个像素，宽15个像素
									outtextxy(25, 50, (LPCTSTR)"黑方落子");
									settextstyle(25, 15, (LPCTSTR)"楷体", 0, 0, 0, 0, 0, 0);//高25个像素，宽15个像素
									clearrectangle(560, 70, 590, 150);
									putimage(560, 70, 30 + 1, 100 + 1, &img1, 560, 70, SRCCOPY);
		    						if (iswin(a, b, 2))
									{
										settextstyle(25, 15, (LPCTSTR)"楷体");//高25个像素，宽15个像素
										putimage(265, 15, 75, 25, &img1, 265, 15, SRCCOPY);
										str = to_string(playerwin) + " : " + to_string(++computerwin);
										outtextxy(265, 15, (LPCTSTR)str.data());
										return isagain2(2);//判断是否赢及是否继续玩
									}
									player = 1;
									break;
								}
							}
						}
					}
				}
			}			
		}
	}
}

//判断是否五子连成一线
bool game::iswin(int a,int b,int c)
{
	int** board = getchessboard();
	int count = 1;//连起来的棋子数
	//纵向检查
	//向上查找是否有同色的棋子
	for (int a1 = a - 1, b1 = b; a1 >= 0 && board[a1][b1] == c; a1--)
	{
		count++;
	}
	//向下查找是否有同色的棋子
	for (int a1 = a + 1, b1 = b; a1 < 15 && board[a1][b1] == c; a1++)
	{
		count++;
	}
	//若纵向上同色棋子数大于等于5，返回true，该玩家获胜
	if (count >= 5)
	{
		return true;
	}
	else {
		count = 1;//若纵向上同色的棋子小于5个，将连起来的棋子数count重新归为1
	}

	//横向检查
	//向左查找是否有同色的棋子
	for (int a1 = a, b1 = b - 1; b1 >= 0 && board[a1][b1] == c; b1--)
	{
		count++;
	}
	//向右查找是否有同色的棋子
	for (int a1 = a, b1 = b + 1; b1 < 15 && board[a1][b1] == c; b1++)
	{
		count++;
	}
	//若横向上同色棋子数大于等于5，返回true，该玩家获胜
	if (count >= 5)
	{
		return true;
	}
	else {
		count = 1;//若横向上同色的棋子小于5个，将连起来的棋子数count重新归为1
	}

	//对角线方向检查（左上，右下）
	//左上
	for (int a1 = a - 1, b1 = b - 1; a1 >= 0 && b1 >= 0 && board[a1][b1] == c; a1--, b1--)
	{
		count++;
	}
	//右下
	for (int a1 = a + 1, b1 = b + 1; a1 < 15 && b1 < 15 && board[a1][b1] == c; a1++, b1++)
	{
		count++;
	}
	//若对角线（左上，右下）上同色棋子数大于等于5，返回true，该玩家获胜
	if (count >= 5)
	{
		return true;
	}
	else {
		count = 1;//若对角线（左上，右下）上同色的棋子小于5个，将连起来的棋子数count重新归为1
	}

	//对角线检查（左下，右上）
	//左下
	for (int a1 = a + 1, b1 = b - 1; a1 < 15 && b1 >= 0 && board[a1][b1] == c; a1++, b1--)
	{
		count++;
	}
	//右上
	for (int a1 = a - 1, b1 = b + 1; a >= 0 && b < 15 && board[a1][b1] == c; a1--, b1++)
	{
		count++;
	}
	//若对角线（左下，右上）上同色棋子数大于等于5，返回true，该玩家获胜
	if (count >= 5)
	{
		return true;
	}
	else {
		count = 1;//若对角线（左下，右上）上同色的棋子小于5个，将连起来的棋子数count重新归为1
		return false;//若四个方向都没有五子连成线，则返回false
	}
}

//是否继续游戏(人人对战)
bool game::isagain1(int a)
{
	int** board = getchessboard();
	settextstyle(20, 10, (LPCTSTR)"黑体");//字符高20像素，宽10像素，黑体
	settextcolor(YELLOW);//设置文本颜色为黄色
	if (a == 1) {
		outtextxy(250, 50, (LPCTSTR)"player1 win!");//输出“player1 win!”
		print1(a);
	}
	else {
		outtextxy(250, 50, (LPCTSTR)"player2 win!");//输出“player2 win！”
		print1(a);
	}
	for (int i(0); i < 15; i++)
	{
		for (int j(0); j < 15; j++)
		{
			board[i][j] = 0;//将棋盘中的元素重新设为0
		}
	}
	HWND hq = FindWindow(NULL, "欢乐五子棋");//获取父窗口句柄
	string str = "Player" + to_string(a) + "  Win";
	int quit = MessageBox(hq, (LPCTSTR)"是否继续游戏", (LPCTSTR)str.data(), MB_YESNO);//弹出窗口，获取用户的选择
	if (IDYES == quit)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//是否继续游戏(人人对战)
bool game::isagain2(int a)
{
	int** board = getchessboard();
	settextstyle(20, 10, (LPCTSTR)"黑体");//字符高20像素，宽10像素，黑体
	settextcolor(YELLOW);//设置文本颜色为黄色
	if (a == 1) {
		outtextxy(250, 50, (LPCTSTR)"player win!");//输出“player win!”
		print2(a);
	}
	else {
		outtextxy(250, 50, (LPCTSTR)"computer win!");//输出“computer win！”
		print2(a);
	}
	for (int i(0); i < 15; i++)
	{
		for (int j(0); j < 15; j++)
		{
			board[i][j] = 0;//将棋盘中的元素重新设为0
		}
	}
	HWND hq = FindWindow(NULL, "欢乐五子棋");//获取父窗口句柄
	string str;
	if (a == 1) {
		str = "Player  Win";
	}
	else if (a == 2)
	{
		str = "Computer Win";
	}
	int quit = MessageBox(hq, (LPCTSTR)"是否继续游戏", (LPCTSTR)str.data(), MB_YESNO);//弹出窗口，获取用户的选择
	if (IDYES == quit)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//输出五子棋盘的结果到文本中作为记录（人人对战）
void game::print1(int b)
{
	const char* path = "C:/Users/86187/source/repos/五子棋3.0/游戏记录.txt";
	ofstream mfile(path, ios_base::out | ios_base::app);//创建输出流对象
	int** a = getchessboard();
	mfile << "player" << b << "win!" << endl;
	mfile << "棋盘记录如下" << endl;
	mfile << "1表示黑棋，2表示白棋，0表示该处无棋" << endl;
	for (int i(0); i < 15; i++)
	{
		for (int j(0); j < 15; j++)
		{
			mfile << a[i][j]<<" ";
		}
		mfile << '\n';
	}

}

//输出五子棋盘的结果到文本中作为记录（人机对战）
void game::print2(int b)
{
	const char* path = "C:/Users/86187/source/repos/五子棋3.0/游戏记录.txt";
	ofstream mfile(path, ios_base::out | ios_base::app);//创建输出流对象
	int** a = getchessboard();
	if (b == 1) {
		mfile << "player win!" << endl;
	}
	else if (b == 2)
	{
		mfile << "computer win!" << endl;
	}
	mfile << "棋盘记录如下" << endl;
	mfile << "1表示黑棋，2表示白棋，0表示该处无棋" << endl;
	for (int i(0); i < 15; i++)
	{
		for (int j(0); j < 15; j++)
		{
			mfile << a[i][j] << " ";
		}
		mfile << '\n';
	}
}