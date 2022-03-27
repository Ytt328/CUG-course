#pragma once

struct Node
{
	int row;//行
	int column;//列
	Node* left;//左节点
	Node* right;//右节点
	Node* up;//上节点
	Node* down;//下节点
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

