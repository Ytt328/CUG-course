#pragma once
#ifndef CALCULATE_H
#define CALCULATE_H
#include"MyDataType.h"

//������������ĺ���˵��
double Distance(double x1, double y1, double x2, double y2);

//���������ĺ�������
PNT_STRU Findpoint(CPoint mousePoint,int pntNum,CFile* pntTmpF,int &nPnt);

//������굽�ߵ���̾���ĺ�������
double DisPntToSeg(D_DOT pt1,D_DOT pt2,D_DOT pt);

//��������ߵĺ�������
LIN_NDX_STRU Findline(CFile* LinTmpNdxF, CFile* LinTmpDatF, CPoint mousePoint,int LinNum, int& nLinNdx);

//POINTתD_DOT�ĺ���˵��
void PntToDot(D_DOT* dot, POINT* pnt, int num);
void PntToDot(D_DOT& dot, POINT pnt);

//D_DOTתPOINT�ĺ���˵��
void DotToPnt(POINT* pnt, D_DOT* dot, int num);
void DotToPnt(POINT& pnt, D_DOT dot);

//��������ϵת������������ϵ�ĺ�������
void PntDPtoVP(D_DOT& pt, double zoom, double offset_x, double offset_y);

//��������ϵת������������ϵ�ͺ�������
void PntVPtoDP(D_DOT& pt, double zoom, double offset_x, double offset_y);

//����������ĵĺ�������
D_DOT GetCenter(RECT rect);

//��������Ŵ�ʱ�Ŵ����ĺ�������
void modulusZoom(RECT client, RECT rect, double& zoom);

//�޸��ߵĵ����ݵĺ�������
void AlterLindot(CFile* LinTmpDatF, LIN_NDX_STRU startLine, LIN_NDX_STRU endLine, int start, int end, long allDateOff);

//�жϵ����ĵ��Ƿ������ڵĺ�������
bool PtinPolygon(CPoint p, D_DOT* ptPolygon, int nCount);

//����������������ĺ�������
REG_NDX_STRU FindReg(CFile* RegTmpNdxF, CFile* RegTmpDatE, CPoint mousePoint, int RegNum, int& nRegNdx);

//��ʱ�����ļ����������ݵĺ�������
void UpdateReg(CFile* RegTmpNdxF, int nReg, REG_NDX_STRU Region);

//�������ɾ����ĺ���˵��
PNT_STRU FindDeletePnt(CPoint mousePoint, int PntNum, CFile* PntTmpF, int& nPnt);

//�������ɾ���ߵĺ���˵��
LIN_NDX_STRU FindDeleteline(CFile* LinTmpNdxF, CFile* LinTmpDatF, CPoint mousePoint, int LinNum, int& nLinNdx);

//�������ɾ�����ĺ���˵��
REG_NDX_STRU FindDeleteReg(CFile* RegTmpNdxF, CFile* RegTmpDatE, CPoint mousePoint, int RegNum, int& nRegNdx);

//����ĳ�����Ͼ��������ĵ�
D_DOT FindPntOnLin(CPoint mousePoint,CFile* LinTmpDatF,LIN_NDX_STRU tempLine,int& tempPntNum);

//ɾ���������������ĵ�
void DelPntOnLin(CFile* LinTmpDatF, LIN_NDX_STRU& tempLine, int& tempPntNum);
#endif 

