#pragma once
#include"chessboard.h"
//������
class game:public Chessboard
{
public:
	//���캯��
	game();

	//��������
	~game();

	//����������
	void creatmainwin();

	//�������˶�ս����
	bool creatwin1();

	//�����˻���ս����
	bool creatwin2();

	//������������
	void createndwin();

	//��ʼ��Ϸ�����˶�ս��
	bool playgame1();

	//��ʼ��Ϸ���˻���ս��
	bool playgame2();

	//�ж��Ƿ�����������һ��
	bool iswin(int, int, int);

	//�Ƿ������Ϸ(���˶�ս)
	bool isagain1(int);

	//�Ƿ������Ϸ���˻���ս��
	bool isagain2(int);

	//����������̵Ľ�����ı�����Ϊ��¼�����˶�ս��
	void print1(int);

	//����������̵Ľ�����ı�����Ϊ��¼���˻���ս��
	void print2(int);
};
