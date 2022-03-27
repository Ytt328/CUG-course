#include "pch.h"
#include "Sudoku_DLX.h"

Sudoku_DLX::Sudoku_DLX(int** Data) {
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			data[i][j] = Data[i][j];
		}
	}
}

Sudoku_DLX::~Sudoku_DLX() {

}

void Sudoku_DLX::setdata(int** Data) {
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			data[i][j] = Data[i][j];
		}
	}
}
