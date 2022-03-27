#include"pch.h"
#include"paint.h"
#include<math.h>
#include "Calculate.h"
#include "resource.h"

/*����*/
void DrawPnt(CClientDC* dc, PNT_STRU point)
{
	CBrush brush(point.color);
	CPen pen(PS_SOLID, 1, point.color);//penstyle��width��color
	CObject* oldObject=nullptr;
	switch (point.pattern)//�����״ͼ����
	{
		//�����״��ʮ���Σ�Բ�Σ��������
	case 0:
		//ʮ����
		oldObject = dc->SelectObject(&pen);//�����ʶ���ѡ��dc���滻oldproject
		dc->MoveTo((long)point.x - 4, (long)point.y);
		dc->LineTo((long)point.x + 4, (long)point.y);
		dc->MoveTo((long)point.x, (long)point.y - 4);
		dc->LineTo((long)point.x, (long)point.y + 4);
		break;
	case 1:
		//Բ��
		oldObject = dc->SelectObject(&brush);//����ˢ����ѡ��dc���滻oldproject
		dc->Ellipse((long)point.x - 2,(long)point.y - 2, (long)point.x + 2, (long)point.y + 2);
		break;
	case 2:
		//�������
		oldObject = dc->SelectObject(&pen);//�����ʶ���ѡ��dc���滻oldproject
		POINT external_pt[5], interior_pt[5];//�ⲿ���㣬�ڲ�����
		double external_r = 10, interior_r = external_r / 2;//�ⲿԲ�뾶���ڲ�Բ�뾶
		//��˳ʱ�����ⲿ���㣬����Ϊ���Ϸ������Ϸ������·������·������Ϸ�
		external_pt[0].x = (long)point.x;
		external_pt[0].y = long(point.y - external_r);
		external_pt[1].x = long(point.x + (external_r * cos(18.0 * 3.14 / 180)));
		external_pt[1].y = long(point.y - (external_r * sin(18.0 * 3.14 / 180)));
		external_pt[2].x = long(point.x + (external_r * sin(36.0 * 3.14 / 180)));
		external_pt[2].y = long(point.y + (external_r * cos(36.0 * 3.14 / 180)));
		external_pt[3].x = long(point.x - (external_r * sin(36.0 * 3.14 / 180)));
		external_pt[3].y = long(external_pt[2].y);
		external_pt[4].x = long(point.x - (external_r * cos(18.0 * 3.14 / 180)));
		external_pt[4].y = long(external_pt[1].y);
		//����ʱ�����ڲ����㣬����Ϊ���Ϸ������·������·������·������Ϸ�
		interior_pt[0].x = long(point.x + (interior_r * cos(54.0 * 3.14 / 180)));
		interior_pt[0].y = long(point.y - (interior_r * sin(54.0 * 3.14 / 180)));
		interior_pt[1].x = long(point.x + (interior_r * sin(72.0 * 3.14 / 180)));
		interior_pt[1].y = long(point.y + (interior_r * cos(72.0 * 3.14 / 180)));
		interior_pt[2].x = long(point.x);
		interior_pt[2].y = long(point.y + interior_r);
		interior_pt[3].x = long(point.x - (interior_r * sin(72.0 * 3.14 / 180)));
		interior_pt[3].y = long(interior_pt[1].y);
		interior_pt[4].x = long(point.x - (interior_r * cos(54.0 * 3.14 / 180)));
		interior_pt[4].y = long(interior_pt[0].y);
		dc -> MoveTo((long)external_pt[0].x, (long)external_pt[0].y);
		dc -> LineTo((long)interior_pt[0].x, (long)interior_pt[0].y);
		for (int i = 1; i < 5; i++)
		{
			dc->LineTo((long)external_pt[i].x, (long)external_pt[i].y);
			dc->LineTo((long)interior_pt[i].x, (long)interior_pt[i].y);
		}
		dc->LineTo((long)external_pt[0].x, (long)external_pt[0].y);
		break;
	}
	dc->SelectObject(oldObject);//ѡ��oldobject
}


/*��ʾ���е�*/
void ShowAllPnt(CClientDC* dc, CFile* pntTmpF, int pntNum)
{
	PNT_STRU point;
	for (int i = 0; i < pntNum; ++i) // ��ʾ��
	{
		ReadTempFileToPnt(pntTmpF, i, point); //����ʱ�ļ��ж�ȡ������
		if (point.isDel == 0)
			DrawPnt(dc, point); //���Ƶ�
	}
}


/*����*/
void DrawSeg(CClientDC* dc, LIN_NDX_STRU line, POINT point1, POINT point2)
{
	CPen pen;
	switch (line.pattern)
	{
	case 0: //ʵ��
		pen.CreatePen(PS_SOLID, 1, line.color); //����һ��ʵ�ߵĻ���
		break;
	case 1: //����
		pen.CreatePen(PS_DASH, 1, line.color); //���������ߵĻ���
		break;
	case 2://����
		pen.CreatePen(PS_DOT, 1, line.color); //����һ�����ߵĻ���
		break;
	default:
		break;
	}
	CPen* oldPen = dc->SelectObject(&pen);
	dc->MoveTo(point1.x, point1.y); //��ʼ���ߣ�������Ƶ�һ����ʼλ��
	dc->LineTo(point2.x, point2.y); //������:�ӳ�ʼ�㵽�ƶ���
	dc->SelectObject(oldPen);
}


/*��ʾ�����ߵĺ�������*/
void ShowAllLin(CClientDC* dc, CFile* LinTmpNdxF, CFile* LinTmpDatF, int LinNum)
{
	LIN_NDX_STRU line;
	for (int i = 0; i < LinNum; i++)
	{
		ReadTempFileToLinNdx(LinTmpNdxF, i, line);//����ʱ�������ļ���ȡ������
		if(line.isDel)
			continue;
		D_DOT dotl, dot2;
		POINT pnt1, pnt2;
		for (int j = 0; j < line.dotNum - 1; j++)
		{
			ReadTempFileToLinDat(LinTmpDatF, line.datOff, j, dotl);//����ʱ�������ļ���ȡ�ڵ�
			ReadTempFileToLinDat(LinTmpDatF, line.datOff, j + 1, dot2);//����ʱ�������ļ���ȡ�ڵ�
			DotToPnt(pnt1, dotl);//DotתPnt
			DotToPnt(pnt2, dot2);//DotתPnt
			DrawSeg(dc, line, pnt1, pnt2);//������
		}
	}
}


/*��ʾ���е�(��)�ĺ���*/
void ShowAllPnt(CClientDC* dc, CFile* PntTmpF, int PntNum, double zoomOffset_x, double zoomOffset_y, double zoom, char isDel)
{
	PNT_STRU point;
	D_DOT xy;
	for (int i = 0; i < PntNum; ++i) //��ʾ��
	{
		ReadTempFileToPnt(PntTmpF, i, point); //����ʱ�ļ���ȡ��
		if (point.isDel == isDel)
		{
			xy.x = point.x;
			xy.y = point.y;
			//����ϵת��������ת���ڣ�
			PntDPtoVP(xy, zoom, zoomOffset_x, zoomOffset_y);
			point.x = xy.x;
			point.y = xy.y;
			DrawPnt(dc, point); //���Ƶ�
		}
	}
}


/*��ʾ������(��)�ĺ�������*/
void ShowAllLin(CClientDC* dc, CFile* LinTmpNdxF, CFile* LinTmpDatF, int LinNum, double zoomOffset_x, double zoomOffset_y, double zoom, char isDel)
{
	LIN_NDX_STRU line;
	for (int i = 0; i < LinNum; i++)
	{
		ReadTempFileToLinNdx(LinTmpNdxF, i, line); //����ʱ�ļ���ȡ������
		if (line.isDel == isDel)
		{
			D_DOT dot1, dot2;
			POINT pnt1, pnt2;
			for (int j = 0; j < line.dotNum - 1; j++)
			{
				//����ʱ�ļ���ȡ�ߵĵ�����
				ReadTempFileToLinDat(LinTmpDatF, line.datOff, j, dot1);
				ReadTempFileToLinDat(LinTmpDatF, line.datOff, j + 1, dot2);
				//����ϵת��������ת���ڣ�
				PntDPtoVP(dot1, zoom, zoomOffset_x, zoomOffset_y);
				PntDPtoVP(dot2, zoom, zoomOffset_x, zoomOffset_y);
				//DotתPnt
				DotToPnt(pnt1, dot1);
				DotToPnt(pnt2, dot2);
				DrawSeg(dc, line, pnt1, pnt2); //������
			}
		}
	}
}


/*��ʾ���ĺ�������*/
void ShowAllReg(CClientDC* dc, CFile* RegTmpNdxF, CFile* RegTmpDatF, int RegNum, double zoomOffset_x, double zoomOffset_y, double zoom, char isDel)
{
	REG_NDX_STRU Region;
	D_DOT pt;
	for (int i = 0; i < RegNum; i++)
	{
		ReadTempFileToRegNdx(RegTmpNdxF, i, Region);//����ʱ�ļ���ȡ������
		D_DOT* dot;
		dot = (D_DOT*) malloc(Region.dotNum * sizeof(D_DOT));//���ٶ�̬�ڴ�
		ZeroMemory(dot, Region.dotNum * sizeof(D_DOT));//0��䣬��ʼ��
		for (int j = 0; j < Region.dotNum; ++j)
		{
			//���δ�����ʱ�ļ���ȡ���Ľڵ����ݲ�����ת��Ϊ��������
			ReadTempFileToRegDat(RegTmpDatF, Region.datOff, j, pt);//��ȡ����
			PntDPtoVP(pt, zoom, zoomOffset_x, zoomOffset_y);//����ת��
			dot[j] = pt;//�浽����
		}
		if (Region.isDel == isDel)
		{
			POINT* point = new POINT[Region.dotNum];
			DotToPnt(point, dot, Region.dotNum);
			DrawReg(dc, Region, point, Region.dotNum); //������
			delete[] point;
		}
		free(dot);//�ͷŶ�̬�ڴ�
	}
}


/*�������ĺ�������*/
void DrawReg(CClientDC* dc, REG_NDX_STRU Region, POINT* pt, long nPnt)
{
	CBrush brush(Region.color);
	CPen pen(PS_SOLID, 1, Region.color);
	CObject* oldObject;
	oldObject = dc -> SelectObject(&pen);
	CBitmap bitmap;
	switch (Region.pattern)
	{
	case 0://ʵ��
		oldObject = dc->SelectObject(&brush);
		break;
	case 1://����
		dc->SelectStockObject(NULL_BRUSH);
		break;
	//case 2://λͼ
	//	bitmap.LoadBitmap(IDB_BITMAP3);
	//	dc->SelectObject(bitmap);
	//	break;
	default:
		break;
	}
	dc->Polygon(pt, nPnt);//�õ�ǰpen���������õ�ǰ��ˢ�������
	dc-> SelectObject(&oldObject);
}