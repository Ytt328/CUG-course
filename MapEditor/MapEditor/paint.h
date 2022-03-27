#pragma once
//绘制


#ifndef PAINT_H
#define PAINT_H
#include"MyDataType.h"
#include"WriteOrRead.h"

//绘制点函数声明
void DrawPnt(CClientDC* dc,PNT_STRU point);

//显示所有点函数声明
void ShowAllPnt(CClientDC* dc, CFile* pntTmpF , int pntNum) ; 

//构造线段的函数说明
void DrawSeg(CClientDC* dc, LIN_NDX_STRU line, POINT point1, POINT point2);

//显示所有线的函数声明
void ShowAllLin(CClientDC* de, CFile* LinTmpNdxF, CFile* LinTmpDatF, int LinNum);

//显示所有点(新)的函数声明
void ShowAllPnt(CClientDC* dc,CFile* PntTmpF, int PntNum, double zoomOffset_x, double zoomOffset_y, double zoom, char isDel);

//显示所有线(新)的函数声明
void ShowAllLin(CClientDC* dc, CFile* LinTmpNdxF, CFile* LinTmpDatF, int LinNum, double zoomOffset_x, double zoom0ffset_y, double zoom, char isDel);

//显示区的函数声明
void ShowAllReg(CClientDC* dc, CFile* RegTmpNdxF, CFile* RegTmpDatF, int RegNum, double zoomOffset_x, double zoomOffset_y, double zoom, char isDel);

//绘制区的函数声明
void DrawReg(CClientDC* dc, REG_NDX_STRU Region, POINT* pt, long nPnt);
#endif
