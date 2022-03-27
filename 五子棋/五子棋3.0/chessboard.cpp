#include"chessboard.h"
#include<easyx.h>

//���캯��
Chessboard::Chessboard()
{
	//��int** ���͵�chessboardָ��һ����ά����
	chessboard = new int* [15];//����15��int* ���Ͷ��������
	for (int i(0); i < 15; i++) 
	{
		chessboard[i] = new int[15];//����15��int���Ͷ��������
	}
	for (int i(0); i < 15; i++)
	{
		for (int j(0); j < 15; j++)
		{
			chessboard[i][j] = 0;
		}
	}
}

//��������
Chessboard::~Chessboard()
{
	//�ͷŶ�̬�ڴ�
	for (int i(0); i < 15; i++)
	{
		delete[] chessboard[i];
	}
	delete[]chessboard;
}

//����ָ�����̶�ά�����ָ��
int** Chessboard::getchessboard()
{
	return chessboard;
}

//�ڽ�������ʾ���̼�����
void Chessboard::display()
{
	IMAGE img1;
	IMAGE img2;
	loadimage(&img1, _T("����1.png"));// ��ͼƬ�л�ȡͼ����Ϊ����1
	putimage(0, 0, &img1, SRCPAINT);//���Ʊ���1����Ļ
	clearrectangle(50, 50, 550, 550);// ��վ���������յ�����������ʾ����
	loadimage(&img2, _T("����.png"));// ��ͼƬ�л�ȡͼ����Ϊ����
	putimage(50, 50, &img2, SRCPAINT);//�������̵���Ļ
}