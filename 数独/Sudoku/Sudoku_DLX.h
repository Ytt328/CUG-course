#pragma once

struct Node
{
	int row;//��
	int column;//��
	Node* left;//��ڵ�
	Node* right;//�ҽڵ�
	Node* up;//�Ͻڵ�
	Node* down;//�½ڵ�
};
class Sudoku_DLX
{
private:
	int data[9][9];
	int matrix[729][324];
public:
	Sudoku_DLX(int** Data);
	~Sudoku_DLX();
	void setdata(int** Data);
};

