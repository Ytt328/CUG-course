#include"chess.h"
#include<graphics.h>
#include<windows.h>
#include<cstdio>

//���캯��
chess::chess(int a,int b)
{
	color = a;
	r = b;
}

//��������
chess::~chess(){}

//�ڽ�������ʾ����
void chess::printchess(int i,int j)
{
	if (color == 1)
	{
		setfillcolor(BLACK);//�����ɫΪ��ɫ
		solidcircle(90 + (30 * i), 90 + (30 * j), r);//�ڸ�λ�û�һ���ޱ߿��Ұ뾶Ϊr���ص����Բ
	}
	else if (color == 2)
	{
		setfillcolor(WHITE);//�����ɫΪ��ɫ
		solidcircle(90 + (30 * i), 90 + (30 * j), r);//�ڸ�λ�û�һ���ޱ߿��Ұ뾶Ϊr���ص����Բ
	}
}
