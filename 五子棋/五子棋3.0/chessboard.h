#pragma once
//������
class Chessboard
{
private:
	int** chessboard;//����(15*15)
public:
	//���캯��
	Chessboard();

	//��������
	~Chessboard();

	//����ָ�����̶�ά�����ָ��
	int** getchessboard();

	//�ڽ�������ʾ���̼�����
	void display();
};
