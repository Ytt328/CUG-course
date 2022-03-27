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
//���캯��
game::game():Chessboard() {}

//��������
game::~game() {}

//����������
void game::creatmainwin()
{
	initgraph(600, 600);//��ʼ��ͼ�λ�����600*600����
	IMAGE img;//����һ��ͼ�������
	setaspectratio(1.1f, 1);// ���õ�ǰ��������
	loadimage(&img, _T("����1.png"));//��ͼƬ�ļ��л�ȡͼ����Ϊ��ʼ����ı���
	putimage(0, 0, &img, SRCPAINT);// ����ͼ����Ļ
	settextstyle(50, 20, (LPCTSTR)"����");//�����ַ�ƽ���߶�50���ء�ƽ�����20���أ��Լ������ʽΪ����
	settextcolor(BLUE);//���õ�ǰ�������ɫΪ��ɫ
	setbkmode(WHITE);//���ñ������ģʽ
	outtextxy(180, 100, (LPCTSTR)"����������");//��ָ��λ����������������塱
	settextstyle(40, 20, (LPCTSTR)"����");//�����ַ�ƽ���߶�Ϊ40���ء�ƽ�����Ϊ20���أ��Լ������ʽΪ����
	settextcolor(GREEN);//���õ�ǰ�������ɫΪ��ɫ
	setbkmode(WHITE);//���ñ������ģʽ
	outtextxy(200, 350, (LPCTSTR)"�˻���ս");//��ָ��λ��������˻���ս��
	outtextxy(200, 400, (LPCTSTR)"���˶�ս");//��ָ��λ���������ʼ��Ϸ��
	outtextxy(200, 451, (LPCTSTR)"�˳���Ϸ");//��ָ��λ��������˳���Ϸ��
	outtextxy(200, 500, (LPCTSTR)"��Ϸ����");//��ָ��λ���������Ϸ����
	MOUSEMSG m;//�����Ϣ
	while (true) {
		m = GetMouseMsg();//��ȡһ�������Ϣ
		if (m.uMsg == WM_MOUSEMOVE && m.x >= 200 && m.x <= 360 && m.y >= 350 && m.y <= 400) //�ж�����Ƿ��Ƶ��ˡ��˻���ս����
		{
			setlinecolor(YELLOW);
			rectangle(200, 350, 360, 400);//������ڡ��˻���ս�������û�ɫ������
		}
		else {
			setlinecolor(getbkcolor());
			rectangle(200, 350, 360, 400);//������ڡ��˻���ս�������ñ������ɫ������
		}
		if (m.uMsg == WM_MOUSEMOVE && m.x >= 200 && m.x <= 360 && m.y >= 401 && m.y <= 450) //�ж�����Ƿ��Ƶ��ˡ����˶�ս����
		{
			setlinecolor(YELLOW);
			rectangle(200, 400, 360, 450);//������ڡ����˶�ս�������û�ɫ������
		}
		else {
			setlinecolor(getbkcolor());
			rectangle(200, 400, 360, 450);//������ڡ����˶�ս�������ñ������ɫ������
		}
		if (m.uMsg == WM_MOUSEMOVE && m.x >= 200 && m.x <= 360 && m.y >= 451 && m.y <= 500) //�ж�����Ƿ��Ƶ��ˡ�������Ϸ����
		{
			setlinecolor(YELLOW);
			rectangle(200, 451, 360, 500);//������ڡ�������Ϸ�������û�ɫ������
		}
		else {
			setlinecolor(getbkcolor());
			rectangle(200, 451, 360, 500);//������ڡ�������Ϸ�������ñ������ɫ������
		}
		if (m.uMsg == WM_MOUSEMOVE && m.x >= 200 && m.x <= 360 && m.y >= 500 && m.y <= 550) //�ж�����Ƿ��Ƶ��ˡ���Ϸ���򡱴�
		{
			setlinecolor(YELLOW);
			rectangle(200, 500, 360, 550);//������ڡ���Ϸ���򡱴����û�ɫ������
		}
		else {
			setlinecolor(getbkcolor());
			rectangle(200, 500, 360, 550);//������ڡ���Ϸ���򡱴����ñ������ɫ������
		}
		if (m.uMsg == WM_LBUTTONDOWN && m.x >= 200 && m.x <= 360 && m.y >= 350 && m.y <= 400)//�ж�����Ƿ��ڡ��˻���ս�������������������
		{
			//��������˻���ս��
			//HWND hq = FindWindow(NULL, "����������");//��ȡ�����ھ��
			//int quit = MessageBox(hq, (LPCTSTR)"�˹�����δ��ȫ��������������ʹ��״̬���Ƿ������Ϸ", (LPCTSTR)"������ʾ", MB_YESNO);//�������ڣ���ȡ�û���ѡ��
			cleardevice();//����
			Sleep(200);//��ͣ0.2��
			bool Continue_Game = true;
			while (Continue_Game)
			{
				Continue_Game=creatwin2();//�����˻���ս�Ĵ���,�����ݷ���ֵ�ж��Ƿ������Ϸ
			}
			break;
		}
		if (m.uMsg == WM_LBUTTONDOWN && m.x >= 200 && m.x <= 360 && m.y >= 400 && m.y <= 450)//�ж�����Ƿ��ڡ����˶�ս�������������������
		{
			//����������˶�ս��
			setlinecolor(RED);
			rectangle(200, 400, 360, 450);//������ڡ���ʼ��Ϸ�������ú�ɫ������
			cleardevice();//����
			Sleep(200);//��ͣ0.2��
			bool Continue_Game = true;
			while (Continue_Game)
			{
				Continue_Game=creatwin1();//�������˶�ս�Ľ��棬�����ݷ���ֵ�ж��Ƿ������Ϸ
			}
			break;//����������Ϸ�����˳�
		}
		if (m.uMsg == WM_LBUTTONDOWN && m.x >= 200 && m.x <= 360 && m.y >= 451 && m.y <= 500)//�ж�����Ƿ��ڡ�������Ϸ�������������������
		{
			break;//������������Ϸ�����˳�
		}
		if (m.uMsg == WM_LBUTTONDOWN && m.x >= 200 && m.x <= 360 && m.y >= 500 && m.y <= 550)//�ж�����Ƿ��ڡ���Ϸ���򡱴����������������
		{
			HWND hq = FindWindow(NULL, "����������");//��ȡ�����ھ��
			int quit = MessageBox(hq, (LPCTSTR)"�������У������������ӵ�һ����ʤ", (LPCTSTR)"����", MB_YESNO);//�������ڣ���ȡ�û���ѡ��
			//break;
		}
	}
}

//�������˶�ս�Ĵ���
bool game::creatwin1()
{
	cleardevice();//����
	graphdefaults();// �������л�ͼ����ΪĬ��ֵ
	display();//��ʾ��������
	setlinecolor(WHITE);//���õ�ǰ������ɫΪ��ɫ
	settextstyle(20, 10, (LPCTSTR)"����");//�����ַ�ƽ���߶�Ϊ20���أ�ƽ�����Ϊ10���أ������ʽΪ����
	settextcolor(BLACK);//����������ɫΪ��ɫ
	setbkmode(WHITE);//���ñ������ģʽ
	outtextxy(5, 5, (LPCTSTR)"���1������");//��ָ��λ����������1�����ӡ�
	outtextxy(5, 28, (LPCTSTR)"������");//��ָ��λ���������������
	outtextxy(475, 5, (LPCTSTR)"���2������");//��ָ��λ����������2�����ӡ�
	outtextxy(475, 28, (LPCTSTR)"������");//��ָ��λ���������������
	settextstyle(25, 15, (LPCTSTR)"����");//�����ַ�ƽ���߶�Ϊ25���أ�ƽ�����Ϊ15���أ������ʽΪ����
	outtextxy(475, 560, (LPCTSTR)"�˳���Ϸ");//��ָ��λ��������˳���Ϸ��
	outtextxy(10, 560, (LPCTSTR)"���¿�ʼ");//��ָ��λ����������¿�ʼ��
	outtextxy(270, 560, (LPCTSTR)"����");//��ָ��λ����������塱
	//settextstyle(25, 15, (LPCTSTR)"����", 2700, 0, 0, 0, 0, 0);//�����ַ���λ25���أ���Ϊ15���أ������ʽΪ����
	//outtextxy(25, 200, (LPCTSTR)"�ص�������");//��ָ��λ��������ڷ����ӡ�
	return playgame1();
}

//�����˻���ս�Ĵ���
bool game::creatwin2()
{
	cleardevice();//����
	graphdefaults();// �������л�ͼ����ΪĬ��ֵ
	display();//��ʾ����������
	setlinecolor(WHITE);//���õ�ǰ������ɫΪ��ɫ
	settextstyle(20, 10, (LPCTSTR)"����");//�����ַ�ƽ���߶�Ϊ20���أ�ƽ�����Ϊ10���أ������ʽΪ����
	settextcolor(BLACK);//����������ɫΪ��ɫ
	setbkmode(WHITE);//���ñ������ģʽ
	outtextxy(5, 5, (LPCTSTR)"��ң�����");//��ָ��λ���������ң����ӡ�
	outtextxy(5, 28, (LPCTSTR)"������");//��ָ��λ���������������
	outtextxy(475, 5, (LPCTSTR)"���ԣ�����");//��ָ��λ����������ԣ����ӡ�
	outtextxy(475, 28, (LPCTSTR)"������");//��ָ��λ���������������
	settextstyle(25, 15, (LPCTSTR)"����");//�����ַ�ƽ���߶�Ϊ25���أ�ƽ�����Ϊ15���أ������ʽΪ����
	outtextxy(475, 560, (LPCTSTR)"�˳���Ϸ");//��ָ��λ��������˳���Ϸ��
	outtextxy(10, 560, (LPCTSTR)"���¿�ʼ");//��ָ��λ����������¿�ʼ��
	outtextxy(270, 560, (LPCTSTR)"����");//��ָ��λ����������塱
	//outtextxy(25,200,(LPCTSTR)"��\n��\n��\n��\n��");
	return playgame2();
}

//������������
void game::createndwin()
{
	cleardevice();//����
	IMAGE img;//����һ��ͼ����Ķ���
	loadimage(&img, _T("����2.png"));//��ͼƬ�ļ���ȡͼ��
	putimage(0, 0, &img, SRCPAINT);//�ڴ�������ʾͼ��
	settextcolor(BLACK);//ѡ���ı���ɫΪ��ɫ
	settextstyle(50, 20, (LPCTSTR)"����");//��50���أ���20���أ�����
	outtextxy(160, 80, (LPCTSTR)"��ӭ�´�������");//�ڴ����������ӭ�´�������
	Sleep(1500);//��ͣ1.5��
}

//��ʼ��Ϸ�����˶�ս��
bool game::playgame1()
{
	int** board = getchessboard();
	int player1win = 0;//���1Ӯ�Ĵ���
	int player2win = 0;//���2Ӯ�Ĵ���
	IMAGE img1;
	IMAGE img2;
	loadimage(&img1, _T("����1.png"));
	loadimage(&img2, _T("����.jpg"));
	settextstyle(25, 15, (LPCTSTR)"����");//�����ַ���Ϊ25���أ���Ϊ15���أ������ʽΪ����
	string str = to_string(player1win) + ":" + to_string(player2win);//to_string:convert int to string,������ת��Ϊ�ַ���
	outtextxy(265, 15, (LPCTSTR)str.data());//��str��������������ָ��λ��      str.date():���ַ��������ʽ�����ַ���������
	int StepNum1 = 0, StepNum2 = 0;//���1��ǰ���߲��������2��ǰ���߲���
	settextstyle(20, 10, (LPCTSTR)"����");//�����ַ���λ20���أ���Ϊ10���أ������ʽΪ����
	outtextxy(60, 27, (LPCTSTR)to_string(StepNum1).data());//��ָ��λ��������1�Ĳ���
	outtextxy(530, 27, (LPCTSTR)to_string(StepNum2).data());//��ָ��λ��������2�Ĳ���
	settextstyle(25, 15, (LPCTSTR)"����", 2700, 0, 0, 0, 0, 0);//�����ַ���λ25���أ���Ϊ15���أ������ʽΪ����
	outtextxy(25, 50, (LPCTSTR)"�ڷ�����");//��ָ��λ��������ڷ����ӡ�
	int Pre_i = -1, Pre_j = -1, Pre_C = -1, Pre_R = -1;
	int player = 1;
	bool UndoAble = true;
	MOUSEMSG msg;//�����Ϣ
	while (true)
	{
		msg = GetMouseMsg();//ȡ�������Ϣ
		//�˳���Ϸ
		if (msg.uMsg == WM_MOUSEMOVE && msg.x >= 475 && msg.x <= 595 && msg.y >= 560 && msg.y <= 585)//�ж�����Ƿ��Ƶ��ˡ��˳���Ϸ��
		{
			setlinecolor(RED);
			rectangle(475, 560, 595, 585);//���ƶ����ˡ��˳���Ϸ�����ú�ɫ����
		}
		else
		{
			setlinecolor(BLACK);
			rectangle(475, 560, 595, 585);//δ�Ƶ����˳���Ϸ�����ú�ɫ����
		}
		if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 460 && msg.x <= 580 && msg.y >= 550 && msg.y <= 575)//�ж�����Ƿ����ˡ��˳���Ϸ��
		{
			return false;//������˳���Ϸ������false
		}
		//���¿�ʼ
		if (msg.uMsg == WM_MOUSEMOVE && msg.x >= 10 && msg.x <= 130 && msg.y >= 560 && msg.y <= 585)//�ж�����Ƿ��Ƶ��ˡ����¿�ʼ��
		{
			setlinecolor(RED);
			rectangle(10, 560, 130, 585);//���Ƶ��ˡ����¿�ʼ�����ú�ɫ����
		}
		else
		{
			setlinecolor(BLACK);
			rectangle(10, 560, 130, 585); //δ�Ƶ������¿�ʼ�����ú�ɫ����
		}
		if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 10 && msg.x <= 130 && msg.y >= 560 && msg.y <= 585)//�ж��Ƿ����ˡ����¿�ʼ��
		{
			player1win = 0;
			player2win = 0;
			return true;
			//������ˡ����¿�ʼ��������true
		}
		//�ص�������
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
		//����
		if (msg.uMsg == WM_MOUSEMOVE && msg.x >= 270 && msg.x <= 330 && msg.y >= 560 && msg.y <= 585)//�ж�����Ƿ��Ƶ��ˡ����塱
		{
			setlinecolor(RED);
			rectangle(270, 560, 330, 585);
			//������Ƶ��ˡ����塱���ú�ɫ����
		}
		else
		{
			setlinecolor(BLACK);
			rectangle(270, 560, 330, 585);
			//�����δ�Ƶ������塱���ú�ɫ����
		}
		if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 270 && msg.x <= 330 && msg.y >= 560 && msg.y <= 585)//�ж�����Ƿ����ˡ����塱
		{
			HWND hq = FindWindow(NULL, "����������");//��ȡ�����ھ��
			if (Pre_i == -1)
			{
				MessageBox(hq, (LPCTSTR)"Ŀǰ�����ܻ���", "��ʾ", MB_OK);
			}
			else if (UndoAble)
			{
				UndoAble = false;
				board[Pre_i][Pre_j] = 0;
				clearcircle(Pre_C, Pre_R, 12);//���Բ��������һ������ʧ
				putimage(Pre_C - 12, Pre_R - 12, 25, 25, &img2, Pre_C - 12 - 50, Pre_R - 12 - 50, SRCCOPY);//�����̻�ԭ����һ��������
				if (player == 1)//��ʱ�ֵ����1���嵫���1��û�£����2ѡ�����
				{
					settextstyle(20, 10, (LPCTSTR)"����");//�����ַ���Ϊ20���أ���Ϊ10���أ�������ʽΪ����
					putimage(530, 27, 20, 20, &img1, 530, 27, SRCCOPY);
					outtextxy(530, 27, (LPCTSTR)to_string(--StepNum2).data());//���2������һ
					settextstyle(25, 15, (LPCTSTR)"����", 2700, 0, 0, 0, 0, 0);//�����ַ���25���أ���15���أ�������ʽΪ����
					outtextxy(575, 50, (LPCTSTR)"�׷�����");//��ָ��λ��������׷����ӡ�
					settextstyle(25, 15, (LPCTSTR)"����", 0, 0, 0, 0, 0, 0);//�����ַ���25���أ���15���أ�������ʽΪ����
					clearrectangle(10, 70, 40, 150);//��վ�������
					putimage(10, 70, 30 + 1, 100 + 1, &img1, 10, 70, SRCCOPY);
					player = 2;//ת�����2
				}
				else//��ʱ�ֵ����2���嵫���2��û�£����1ѡ�����
				{
					settextstyle(20, 10, (LPCTSTR)"����");//�����ַ���Ϊ20���أ���Ϊ10���أ�������ʽΪ����
					putimage(60, 27, 20, 20, &img1, 60, 27, SRCCOPY);
					outtextxy(60, 27, (LPCTSTR)to_string(--StepNum1).data());//���1������һ
					settextstyle(25, 15, (LPCTSTR)"����", 2700, 0, 0, 0, 0, 0);//�����ַ���25���أ���15���أ�������ʽΪ����
					outtextxy(25, 50, (LPCTSTR)"�ڷ�����");//��ָ��λ��������ڷ����ӡ�
					settextstyle(25, 15, (LPCTSTR)"����", 0, 0, 0, 0, 0, 0);//�����ַ���25���أ���15���أ�������ʽΪ����
					clearrectangle(560, 70, 590, 150);//��վ�������
					putimage(560, 70, 30 + 1, 100 + 1, &img1, 560, 70, SRCCOPY);
					player = 1;//ת�����1
				}
			}
			else
			{
				MessageBox(hq, (LPCTSTR)"�����������һ��", "�Ѿ��ڹ�����", MB_OK);
			}
		}
		//��������
		if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 80 && msg.x <= 520 && msg.y >= 80 && msg.y <= 520)//�ж��Ƿ����������������
		{
			int i = 0, j = 0;//����������ڵĵ������������е�λӦ
			for (; i < 15; i++)
			{
				if (msg.y <= 90 + i * 30 + 10 && msg.y >= 90 + i * 30 - 10)//������������
				{
					for (; j < 15; j++)
					{
						if (msg.x <= 90 + j * 30 + 10 && msg.x >= 90 + j * 30 - 10)//������������
						{
							UndoAble = true;
							if (player == 1 && board[i][j] == 0) 
							{//��ʱ�����1�������������ָ��û������
								chess chess1(1, 12);//����һ������ ��ɫ���뾶Ϊ12����
								chess1.printchess(j, i);//�ڸõ㻭һ������
								board[i][j] = 1;//��Ǹõ����к���
								settextstyle(20, 10, (LPCTSTR)"����", 0, 0, 0, 0, 0, 0);//�����ַ���20����10������
								putimage(60, 27, 20, 20, &img1, 60, 27, SRCCOPY);
								outtextxy(60, 27, (LPCTSTR)to_string(++StepNum1).data());//���1������һ
								settextstyle(25, 15, (LPCTSTR)"����", 2700, 0, 0, 0, 0, 0);//��25�����أ���15������
								outtextxy(575, 50, (LPCTSTR)"�׷�����");//�ڽ�������ʾ���׷����ӡ�
								settextstyle(25, 15, (LPCTSTR)"����", 0, 0, 0, 0, 0, 0);//��25�����أ���15������
								clearrectangle(10, 70, 40, 150);//��վ�������
								putimage(10, 70, 30 + 1, 100 + 1, &img1, 10, 70, SRCCOPY);
								if (iswin(i, j, 1))//�ж����1�Ƿ�����������
								{
									//�����1����������һ��
									settextstyle(25, 15, (LPCTSTR)"����");//��25�����أ���15������
									putimage(265, 15, 75, 25, &img1, 265, 15, SRCCOPY);
									str = to_string(++player1win) + " : " + to_string(player2win);//���1Ӯ�Ĵ�����һ
									outtextxy(265, 15, (LPCTSTR)str.data());
									return isagain1(1);//�ж��Ƿ�Ӯ���Ƿ������
								}
								player = 2;//ת�����2
								break;
							}
							else if (player == 2 && board[i][j] == 0)
							{
								//��ʱ�����2�����������ָ��û������
								chess chess1(2, 12);//����һ������ ��ɫ���뾶Ϊ12����
								chess1.printchess(j, i);//�ڸõ㻭һ������
								board[i][j] = 2;//��Ǹõ����а���
								settextstyle(20, 10, (LPCTSTR)"����", 0, 0, 0, 0, 0, 0); //�����ַ���20����10������
								putimage(530, 27, 20, 20, &img1, 530, 27, SRCCOPY);
								outtextxy(530, 27, (LPCTSTR)to_string(++StepNum2).data());//���2������1
								settextstyle(25, 15, (LPCTSTR)"����", 2700, 0, 0, 0, 0, 0);//��25�����أ���15������
								outtextxy(25, 50, (LPCTSTR)"�ڷ�����");//�ڽ�������ʾ���ڷ����ӡ�
								settextstyle(25, 15, (LPCTSTR)"����", 0, 0, 0, 0, 0, 0);//��25�����أ���15������
								clearrectangle(560, 70, 590, 150);//��վ�������
								putimage(560, 70, 30 + 1, 100 + 1, &img1, 560, 70, SRCCOPY);
								if (iswin(i, j, 2))//�ж����2�Ƿ�����������
								{
									//�����1����������һ��
									settextstyle(25, 15, (LPCTSTR)"����");//��25�����أ���15������
									putimage(265, 15, 75, 25, &img1, 265, 15, SRCCOPY);
									str = to_string(player1win) + " : " + to_string(++player2win);//���2Ӯ�Ĵ�����һ
									outtextxy(265, 15, (LPCTSTR)str.data());
									return isagain1(2);//�ж��Ƿ�Ӯ���Ƿ������
								}
								player = 1;//ת�����1
								break;

							}
						}
					}
				}
			}
		}
	}
}

//��ʼ��Ϸ���˻���ս��
bool game::playgame2()
{
	int** board = getchessboard();
	int playerwin = 0;
	int computerwin = 0;
	IMAGE img1;
	IMAGE img2;
	loadimage(&img1, _T("����1.png"));
	loadimage(&img2, _T("����.jpg"));
	settextstyle(25, 15, (LPCTSTR)"����");//�����ַ���Ϊ25���أ���Ϊ15���أ������ʽΪ����
	string str = to_string(playerwin) + ":" + to_string(computerwin);//to_string:convert int to string,������ת��Ϊ�ַ���
	outtextxy(265, 15, (LPCTSTR)str.data());//��str��������������ָ��λ��      str.date():���ַ��������ʽ�����ַ���������
	int PlayerNum = 0, ComputerNum = 0;//��ҵ�ǰ���߲��������Ե�ǰ���߲���
	settextstyle(20, 10, (LPCTSTR)"����");//�����ַ���λ20���أ���Ϊ10���أ������ʽΪ����
	outtextxy(60, 27, (LPCTSTR)to_string(PlayerNum).data());//��ָ��λ�������ҵĲ���
	outtextxy(530, 27, (LPCTSTR)to_string(ComputerNum).data());//��ָ��λ��������2�Ĳ���
	settextstyle(25, 15, (LPCTSTR)"����", 2700, 0, 0, 0, 0, 0);//�����ַ���λ25���أ���Ϊ15���أ������ʽΪ����
	outtextxy(25, 50, (LPCTSTR)"�ڷ�����");//��ָ��λ��������ڷ����ӡ�
	int Pre_i = -1, Pre_j = -1, Pre_C = -1, Pre_R = -1;
	int player = 1;
	bool UndoAble = true;
	MOUSEMSG msg;//�����Ϣ
	while (true)
	{
		msg = GetMouseMsg();//ȡ�������Ϣ
		//�˳���Ϸ
		if (msg.uMsg == WM_MOUSEMOVE && msg.x >= 475 && msg.x <= 595 && msg.y >= 560 && msg.y <= 585)//�ж�����Ƿ��Ƶ��ˡ��˳���Ϸ��
		{
			setlinecolor(RED);
			rectangle(475, 560, 595, 585);//���ƶ����ˡ��˳���Ϸ�����ú�ɫ����
		}
		else
		{
			setlinecolor(BLACK);
			rectangle(475, 560, 595, 585);//δ�Ƶ����˳���Ϸ�����ú�ɫ����
		}
		if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 460 && msg.x <= 580 && msg.y >= 550 && msg.y <= 575)//�ж�����Ƿ����ˡ��˳���Ϸ��
		{
			return false;//������˳���Ϸ������false
		}
		//���¿�ʼ
		if (msg.uMsg == WM_MOUSEMOVE && msg.x >= 10 && msg.x <= 130 && msg.y >= 560 && msg.y <= 585)//�ж�����Ƿ��Ƶ��ˡ����¿�ʼ��
		{
			setlinecolor(RED);
			rectangle(10, 560, 130, 585);//���Ƶ��ˡ����¿�ʼ�����ú�ɫ����
		}
		else
		{
			setlinecolor(BLACK);
			rectangle(10, 560, 130, 585); //δ�Ƶ������¿�ʼ�����ú�ɫ����
		}
		if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 10 && msg.x <= 130 && msg.y >= 560 && msg.y <= 585)//�ж��Ƿ����ˡ����¿�ʼ��
		{
			playerwin = 0;
			computerwin = 0;
			return true;
			//������ˡ����¿�ʼ��������true
		}
		//����
		if (msg.uMsg == WM_MOUSEMOVE && msg.x >= 270 && msg.x <= 330 && msg.y >= 560 && msg.y <= 585)//�ж�����Ƿ��Ƶ��ˡ����塱
		{
			setlinecolor(RED);
			rectangle(270, 560, 330, 585);
			//������Ƶ��ˡ����塱���ú�ɫ����
		}
		else
		{
			setlinecolor(BLACK);
			rectangle(270, 560, 330, 585);
			//�����δ�Ƶ������塱���ú�ɫ����
		}
		if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 270 && msg.x <= 330 && msg.y >= 560 && msg.y <= 585)//�ж�����Ƿ����ˡ����塱
		{
			HWND hq = FindWindow(NULL, "����������");//��ȡ�����ھ��
			if (Pre_i == -1)
			{
				MessageBox(hq, (LPCTSTR)"Ŀǰ�����ܻ���", "��ʾ", MB_OK);//������Ϣ��
			}
			else if (UndoAble)
			{
				UndoAble = false;
				board[Pre_i][Pre_j] = 0;
				clearcircle(Pre_C, Pre_R, 12);//���Բ��������һ������ʧ
				putimage(Pre_C - 12, Pre_R - 12, 25, 25, &img2, Pre_C - 12 - 50, Pre_R - 12 - 50, SRCCOPY);//�����̻�ԭ����һ��������
				if (player == 1)//��ʱ�ֵ�������嵫��һ�û�£����Բ��ɻ���
				{
					MessageBox(hq, (LPCTSTR)"���Բ�֧�ֻ���", "��ʾ", MB_OK);
					player = 2;//ת������
				}
				else//��ʱ�ֵ��������嵫���Ի�û���壬���ѡ�����
				{
					settextstyle(20, 10, (LPCTSTR)"����");//�����ַ���Ϊ20���أ���Ϊ10���أ�������ʽΪ����
					putimage(60, 27, 20, 20, &img1, 60, 27, SRCCOPY);//
					outtextxy(60, 27, (LPCTSTR)to_string(--PlayerNum).data());//��Ҳ�����һ
					settextstyle(25, 15, (LPCTSTR)"����", 2700, 0, 0, 0, 0, 0);//�����ַ���25���أ���15���أ�������ʽΪ����
					outtextxy(25, 50, (LPCTSTR)"�ڷ�����");//��ָ��λ��������ڷ����ӡ�
					settextstyle(25, 15, (LPCTSTR)"����", 0, 0, 0, 0, 0, 0);//�����ַ���25���أ���15���أ�������ʽΪ����
					clearrectangle(560, 70, 590, 150);//��վ�������
					putimage(560, 70, 30 + 1, 100 + 1, &img1, 560, 70, SRCCOPY);//
					player = 1;//ת�����2
				}
			}
			else
			{
				MessageBox(hq, (LPCTSTR)"�����������һ��", "�Ѿ��ڹ�����", MB_OK);
			}
		}
		//��������
		if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 50 && msg.x <= 550 && msg.y >= 50 && msg.y <= 550)//����Ұ������
		{
			int i = 0, j = 0;
			for (; i < 15; i++)
			{
				if (msg.y <= 90 + i * 30 + 10 && msg.y >= 90 + i * 30 - 10)//������������
				{
					for (; j < 15; j++)
					{
						if (msg.x <= 90 + j * 30 + 10 && msg.x >= 90 + j * 30 - 10)//������������
						{
							UndoAble = true;//���Ի���
							if (player == 1 && board[i][j] == 0)
							{
								chess chess1(1, 12);//����һ�����ӣ���ɫ���뾶Ϊ12����
								chess1.printchess(j, i);//�ڸõ㻭һ������
								board[i][j] = 1;//��Ǹõ����к���
								settextstyle(20, 10, (LPCTSTR)"����", 0, 0, 0, 0, 0, 0);//�����ַ���20����10������
								putimage(60, 27, 20, 20, &img1, 60, 27, SRCCOPY);
								outtextxy(60, 27, (LPCTSTR)to_string(++PlayerNum).data());//��Ҳ�����һ
								settextstyle(25, 15, (LPCTSTR)"����", 2700, 0, 0, 0, 0, 0);//��25�����أ���15������
								outtextxy(575, 50, (LPCTSTR)"�׷�����");//�ڽ�������ʾ���׷����ӡ�
								settextstyle(25, 15, (LPCTSTR)"����", 0, 0, 0, 0, 0, 0);//��25�����أ���15������
								clearrectangle(10, 70, 40, 150);//��վ�������
								putimage(10, 70, 30 + 1, 100 + 1, &img1, 10, 70, SRCCOPY);
								if (iswin(i, j, 1))//�ж�����Ƿ�����������
								{
									//�����1����������һ��
									settextstyle(25, 15, (LPCTSTR)"����");//��25�����أ���15������
									putimage(265, 15, 75, 25, &img1, 265, 15, SRCCOPY);//���ȷִ����
									str = to_string(++playerwin) + " : " + to_string(computerwin);//���1Ӯ�Ĵ�����һ
									outtextxy(265, 15, (LPCTSTR)str.data());//��������ȷ�
									return isagain2(1);//�ж��Ƿ�Ӯ���Ƿ������
								}
								else {
									//ʵ�ֵ�������
									Sleep(1000);//��ͣһ��
									player = 2;//ת������
									int a = -1, b = -1;//���������
									//int a1 = 0, b1 = 0, a2 = 0, b2 = 0;
									//��鵱ǰ������ӵ����
									if (a == -1) {
										for (int i(0); i < 15; i++)
										{
											for (int j(0); j < 15; j++)
											{
												//����Ѱ�Ұ���
												if (board[i][j] == 2)
												{
													int a1 = 0, b1 = 0, a2 = 0, b2 = 0;
													int count = 1;//��Ǵ�ʱ����һ��������Ϊ1
													//��������Ƿ�������
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
													if (count == 4)//������������һ��
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
												//����Ѱ�Ұ���
												if (board[i][j] == 2)
												{
													int a1 = 0, b1 = 0, a2 = 0, b2 = 0;
													int count = 1;//��Ǵ�ʱ����һ��������Ϊ1
													//��������Ƿ�������
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
													if (count == 4)//������������һ��
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
												//����Ѱ�Ұ���
												if (board[i][j] == 2)
												{
													//int a1 = 0, b1 = 0, a2 = 0, b2 = 0;
													int count = 1;//��Ǵ�ʱ����һ��������Ϊ1
													//��Խ��߼��										    
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
													if (count == 4)//������������һ��
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
												//����Ѱ�Ұ���
												if (board[i][j] == 2)
												{
													int a1 = 0, b1 = 0, a2 = 0, b2 = 0;
													int count = 1;//��Ǵ�ʱ����һ��������Ϊ1
													//�ҶԽ��߼��
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
													if (count == 4)//������������һ��
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
									//������ı�����δ���ְ������������ӻ��������ӵ����

									if (a < 0 || b < 0)
									{
										int a1 = 0, b1 = 0, a2 = 0, b2 = 0;
										int black = 1;//����һ�����ϵĺ������
										//���Է����ӵ����
										//�������Ƿ������������Լ���������
										//����

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
									//���������߹��̼������δȷ������λ��
									if (a < 0 || b < 0)
									{
										int a1 = 0, b1 = 0, a2 = 0, b2 = 0;
										int black = 1;//����һ�����ϵĺ������
										//���Է����ӵ����
										//�������Ƿ������������Լ���������
										//����
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
									//���������߹��̼������δȷ������λ��
									if (a < 0 || b < 0)
									{
										int a1 = 0, b1 = 0, a2 = 0, b2 = 0;
										int black = 1;//����һ�����ϵĺ������
										//���Է����ӵ����
										//�������Ƿ������������Լ���������
										//��Խ��߷���
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
									//���������߹��̼������δȷ������λ��
									if (a < 0 || b < 0)
									{
										int a1 = 0, b1 = 0, a2 = 0, b2 = 0;
										int black = 1;//����һ�����ϵĺ������
										//���Է����ӵ����
										//�������Ƿ������������Լ���������
										//�ҶԽ��߷���
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
									settextstyle(20, 10, (LPCTSTR)"����", 0, 0, 0, 0, 0, 0); //�����ַ���20����10������
									putimage(530, 27, 20, 20, &img1, 530, 27, SRCCOPY);
									outtextxy(530, 27, (LPCTSTR)to_string(++ComputerNum).data());
									settextstyle(25, 15, (LPCTSTR)"����", 2700, 0, 0, 0, 0, 0);//��25�����أ���15������
									outtextxy(25, 50, (LPCTSTR)"�ڷ�����");
									settextstyle(25, 15, (LPCTSTR)"����", 0, 0, 0, 0, 0, 0);//��25�����أ���15������
									clearrectangle(560, 70, 590, 150);
									putimage(560, 70, 30 + 1, 100 + 1, &img1, 560, 70, SRCCOPY);
		    						if (iswin(a, b, 2))
									{
										settextstyle(25, 15, (LPCTSTR)"����");//��25�����أ���15������
										putimage(265, 15, 75, 25, &img1, 265, 15, SRCCOPY);
										str = to_string(playerwin) + " : " + to_string(++computerwin);
										outtextxy(265, 15, (LPCTSTR)str.data());
										return isagain2(2);//�ж��Ƿ�Ӯ���Ƿ������
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

//�ж��Ƿ���������һ��
bool game::iswin(int a,int b,int c)
{
	int** board = getchessboard();
	int count = 1;//��������������
	//������
	//���ϲ����Ƿ���ͬɫ������
	for (int a1 = a - 1, b1 = b; a1 >= 0 && board[a1][b1] == c; a1--)
	{
		count++;
	}
	//���²����Ƿ���ͬɫ������
	for (int a1 = a + 1, b1 = b; a1 < 15 && board[a1][b1] == c; a1++)
	{
		count++;
	}
	//��������ͬɫ���������ڵ���5������true������һ�ʤ
	if (count >= 5)
	{
		return true;
	}
	else {
		count = 1;//��������ͬɫ������С��5��������������������count���¹�Ϊ1
	}

	//������
	//��������Ƿ���ͬɫ������
	for (int a1 = a, b1 = b - 1; b1 >= 0 && board[a1][b1] == c; b1--)
	{
		count++;
	}
	//���Ҳ����Ƿ���ͬɫ������
	for (int a1 = a, b1 = b + 1; b1 < 15 && board[a1][b1] == c; b1++)
	{
		count++;
	}
	//��������ͬɫ���������ڵ���5������true������һ�ʤ
	if (count >= 5)
	{
		return true;
	}
	else {
		count = 1;//��������ͬɫ������С��5��������������������count���¹�Ϊ1
	}

	//�Խ��߷����飨���ϣ����£�
	//����
	for (int a1 = a - 1, b1 = b - 1; a1 >= 0 && b1 >= 0 && board[a1][b1] == c; a1--, b1--)
	{
		count++;
	}
	//����
	for (int a1 = a + 1, b1 = b + 1; a1 < 15 && b1 < 15 && board[a1][b1] == c; a1++, b1++)
	{
		count++;
	}
	//���Խ��ߣ����ϣ����£���ͬɫ���������ڵ���5������true������һ�ʤ
	if (count >= 5)
	{
		return true;
	}
	else {
		count = 1;//���Խ��ߣ����ϣ����£���ͬɫ������С��5��������������������count���¹�Ϊ1
	}

	//�Խ��߼�飨���£����ϣ�
	//����
	for (int a1 = a + 1, b1 = b - 1; a1 < 15 && b1 >= 0 && board[a1][b1] == c; a1++, b1--)
	{
		count++;
	}
	//����
	for (int a1 = a - 1, b1 = b + 1; a >= 0 && b < 15 && board[a1][b1] == c; a1--, b1++)
	{
		count++;
	}
	//���Խ��ߣ����£����ϣ���ͬɫ���������ڵ���5������true������һ�ʤ
	if (count >= 5)
	{
		return true;
	}
	else {
		count = 1;//���Խ��ߣ����£����ϣ���ͬɫ������С��5��������������������count���¹�Ϊ1
		return false;//���ĸ�����û�����������ߣ��򷵻�false
	}
}

//�Ƿ������Ϸ(���˶�ս)
bool game::isagain1(int a)
{
	int** board = getchessboard();
	settextstyle(20, 10, (LPCTSTR)"����");//�ַ���20���أ���10���أ�����
	settextcolor(YELLOW);//�����ı���ɫΪ��ɫ
	if (a == 1) {
		outtextxy(250, 50, (LPCTSTR)"player1 win!");//�����player1 win!��
		print1(a);
	}
	else {
		outtextxy(250, 50, (LPCTSTR)"player2 win!");//�����player2 win����
		print1(a);
	}
	for (int i(0); i < 15; i++)
	{
		for (int j(0); j < 15; j++)
		{
			board[i][j] = 0;//�������е�Ԫ��������Ϊ0
		}
	}
	HWND hq = FindWindow(NULL, "����������");//��ȡ�����ھ��
	string str = "Player" + to_string(a) + "  Win";
	int quit = MessageBox(hq, (LPCTSTR)"�Ƿ������Ϸ", (LPCTSTR)str.data(), MB_YESNO);//�������ڣ���ȡ�û���ѡ��
	if (IDYES == quit)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//�Ƿ������Ϸ(���˶�ս)
bool game::isagain2(int a)
{
	int** board = getchessboard();
	settextstyle(20, 10, (LPCTSTR)"����");//�ַ���20���أ���10���أ�����
	settextcolor(YELLOW);//�����ı���ɫΪ��ɫ
	if (a == 1) {
		outtextxy(250, 50, (LPCTSTR)"player win!");//�����player win!��
		print2(a);
	}
	else {
		outtextxy(250, 50, (LPCTSTR)"computer win!");//�����computer win����
		print2(a);
	}
	for (int i(0); i < 15; i++)
	{
		for (int j(0); j < 15; j++)
		{
			board[i][j] = 0;//�������е�Ԫ��������Ϊ0
		}
	}
	HWND hq = FindWindow(NULL, "����������");//��ȡ�����ھ��
	string str;
	if (a == 1) {
		str = "Player  Win";
	}
	else if (a == 2)
	{
		str = "Computer Win";
	}
	int quit = MessageBox(hq, (LPCTSTR)"�Ƿ������Ϸ", (LPCTSTR)str.data(), MB_YESNO);//�������ڣ���ȡ�û���ѡ��
	if (IDYES == quit)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//����������̵Ľ�����ı�����Ϊ��¼�����˶�ս��
void game::print1(int b)
{
	const char* path = "C:/Users/86187/source/repos/������3.0/��Ϸ��¼.txt";
	ofstream mfile(path, ios_base::out | ios_base::app);//�������������
	int** a = getchessboard();
	mfile << "player" << b << "win!" << endl;
	mfile << "���̼�¼����" << endl;
	mfile << "1��ʾ���壬2��ʾ���壬0��ʾ�ô�����" << endl;
	for (int i(0); i < 15; i++)
	{
		for (int j(0); j < 15; j++)
		{
			mfile << a[i][j]<<" ";
		}
		mfile << '\n';
	}

}

//����������̵Ľ�����ı�����Ϊ��¼���˻���ս��
void game::print2(int b)
{
	const char* path = "C:/Users/86187/source/repos/������3.0/��Ϸ��¼.txt";
	ofstream mfile(path, ios_base::out | ios_base::app);//�������������
	int** a = getchessboard();
	if (b == 1) {
		mfile << "player win!" << endl;
	}
	else if (b == 2)
	{
		mfile << "computer win!" << endl;
	}
	mfile << "���̼�¼����" << endl;
	mfile << "1��ʾ���壬2��ʾ���壬0��ʾ�ô�����" << endl;
	for (int i(0); i < 15; i++)
	{
		for (int j(0); j < 15; j++)
		{
			mfile << a[i][j] << " ";
		}
		mfile << '\n';
	}
}