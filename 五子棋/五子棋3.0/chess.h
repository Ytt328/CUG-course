#pragma once
//������
class chess
{
	int color;//1��ʾ��ɫ��2��ʾ��ɫ
	int r;//���Ӱ뾶
public:
	//���캯��
	chess(int,int);

	//��������
	~chess();

	//�ڽ�������ʾ����
	void printchess(int i, int j);
};
