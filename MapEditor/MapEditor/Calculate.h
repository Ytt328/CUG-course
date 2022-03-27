#pragma once
#ifndef CALCULATE_H
#define CALCULATE_H
#include"MyDataType.h"

//计算两点间距离的函数说明
double Distance(double x1, double y1, double x2, double y2);

//查找最近点的函数声明
PNT_STRU Findpoint(CPoint mousePoint,int pntNum,CFile* pntTmpF,int &nPnt);

//计算鼠标到线的最短距离的函数声明
double DisPntToSeg(D_DOT pt1,D_DOT pt2,D_DOT pt);

//查找最近线的函数声明
LIN_NDX_STRU Findline(CFile* LinTmpNdxF, CFile* LinTmpDatF, CPoint mousePoint,int LinNum, int& nLinNdx);

//POINT转D_DOT的函数说明
void PntToDot(D_DOT* dot, POINT* pnt, int num);
void PntToDot(D_DOT& dot, POINT pnt);

//D_DOT转POINT的函数说明
void DotToPnt(POINT* pnt, D_DOT* dot, int num);
void DotToPnt(POINT& pnt, D_DOT dot);

//数据坐标系转换到窗口坐标系的函数声明
void PntDPtoVP(D_DOT& pt, double zoom, double offset_x, double offset_y);

//窗口坐标系转换到数据坐标系和函数声明
void PntVPtoDP(D_DOT& pt, double zoom, double offset_x, double offset_y);

//计算矩形中心的函数声明
D_DOT GetCenter(RECT rect);

//计算拉框放大时放大倍数的函数声明
void modulusZoom(RECT client, RECT rect, double& zoom);

//修改线的点数据的函数声明
void AlterLindot(CFile* LinTmpDatF, LIN_NDX_STRU startLine, LIN_NDX_STRU endLine, int start, int end, long allDateOff);

//判断单击的点是否在区内的函数声明
bool PtinPolygon(CPoint p, D_DOT* ptPolygon, int nCount);

//查找离鼠标点最近区的函数声明
REG_NDX_STRU FindReg(CFile* RegTmpNdxF, CFile* RegTmpDatE, CPoint mousePoint, int RegNum, int& nRegNdx);

//临时索引文件更新区数据的函数声明
void UpdateReg(CFile* RegTmpNdxF, int nReg, REG_NDX_STRU Region);

//查找最近删除点的函数说明
PNT_STRU FindDeletePnt(CPoint mousePoint, int PntNum, CFile* PntTmpF, int& nPnt);

//查找最近删除线的函数说明
LIN_NDX_STRU FindDeleteline(CFile* LinTmpNdxF, CFile* LinTmpDatF, CPoint mousePoint, int LinNum, int& nLinNdx);

//查找最近删除区的函数说明
REG_NDX_STRU FindDeleteReg(CFile* RegTmpNdxF, CFile* RegTmpDatE, CPoint mousePoint, int RegNum, int& nRegNdx);

//查找某条线上距鼠标最近的点
D_DOT FindPntOnLin(CPoint mousePoint,CFile* LinTmpDatF,LIN_NDX_STRU tempLine,int& tempPntNum);

//删除线上离鼠标最近的点
void DelPntOnLin(CFile* LinTmpDatF, LIN_NDX_STRU& tempLine, int& tempPntNum);
#endif 

