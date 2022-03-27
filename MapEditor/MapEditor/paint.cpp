#include"pch.h"
#include"paint.h"
#include<math.h>
#include "Calculate.h"
#include "resource.h"

/*画点*/
void DrawPnt(CClientDC* dc, PNT_STRU point)
{
	CBrush brush(point.color);
	CPen pen(PS_SOLID, 1, point.color);//penstyle，width，color
	CObject* oldObject=nullptr;
	switch (point.pattern)//点的形状图案号
	{
		//点的形状：十字形，圆形，五角星形
	case 0:
		//十字行
		oldObject = dc->SelectObject(&pen);//将画笔对象选入dc，替换oldproject
		dc->MoveTo((long)point.x - 4, (long)point.y);
		dc->LineTo((long)point.x + 4, (long)point.y);
		dc->MoveTo((long)point.x, (long)point.y - 4);
		dc->LineTo((long)point.x, (long)point.y + 4);
		break;
	case 1:
		//圆形
		oldObject = dc->SelectObject(&brush);//将画刷对象选入dc，替换oldproject
		dc->Ellipse((long)point.x - 2,(long)point.y - 2, (long)point.x + 2, (long)point.y + 2);
		break;
	case 2:
		//五角星行
		oldObject = dc->SelectObject(&pen);//将画笔对象选入dc，替换oldproject
		POINT external_pt[5], interior_pt[5];//外部顶点，内部顶点
		double external_r = 10, interior_r = external_r / 2;//外部圆半径，内部圆半径
		//先顺时针求外部顶点，依次为正上方，右上方，右下方，左下方，左上方
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
		//再逆时针求内部顶点，依次为右上方、右下方、正下方、左下方、左上方
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
	dc->SelectObject(oldObject);//选回oldobject
}


/*显示所有点*/
void ShowAllPnt(CClientDC* dc, CFile* pntTmpF, int pntNum)
{
	PNT_STRU point;
	for (int i = 0; i < pntNum; ++i) // 显示点
	{
		ReadTempFileToPnt(pntTmpF, i, point); //从临时文件中读取点数据
		if (point.isDel == 0)
			DrawPnt(dc, point); //绘制点
	}
}


/*画线*/
void DrawSeg(CClientDC* dc, LIN_NDX_STRU line, POINT point1, POINT point2)
{
	CPen pen;
	switch (line.pattern)
	{
	case 0: //实线
		pen.CreatePen(PS_SOLID, 1, line.color); //创建一个实线的画笔
		break;
	case 1: //虚线
		pen.CreatePen(PS_DASH, 1, line.color); //创建个虚线的画笔
		break;
	case 2://点线
		pen.CreatePen(PS_DOT, 1, line.color); //创建一个点线的画笔
		break;
	default:
		break;
	}
	CPen* oldPen = dc->SelectObject(&pen);
	dc->MoveTo(point1.x, point1.y); //开始画线，将光标移到一个初始位置
	dc->LineTo(point2.x, point2.y); //绘制线:从初始点到移动点
	dc->SelectObject(oldPen);
}


/*显示所有线的函数声明*/
void ShowAllLin(CClientDC* dc, CFile* LinTmpNdxF, CFile* LinTmpDatF, int LinNum)
{
	LIN_NDX_STRU line;
	for (int i = 0; i < LinNum; i++)
	{
		ReadTempFileToLinNdx(LinTmpNdxF, i, line);//从临时线索引文件读取线索引
		if(line.isDel)
			continue;
		D_DOT dotl, dot2;
		POINT pnt1, pnt2;
		for (int j = 0; j < line.dotNum - 1; j++)
		{
			ReadTempFileToLinDat(LinTmpDatF, line.datOff, j, dotl);//从临时线索引文件读取节点
			ReadTempFileToLinDat(LinTmpDatF, line.datOff, j + 1, dot2);//从临时线索引文件读取节点
			DotToPnt(pnt1, dotl);//Dot转Pnt
			DotToPnt(pnt2, dot2);//Dot转Pnt
			DrawSeg(dc, line, pnt1, pnt2);//绘制线
		}
	}
}


/*显示所有点(新)的函数*/
void ShowAllPnt(CClientDC* dc, CFile* PntTmpF, int PntNum, double zoomOffset_x, double zoomOffset_y, double zoom, char isDel)
{
	PNT_STRU point;
	D_DOT xy;
	for (int i = 0; i < PntNum; ++i) //显示点
	{
		ReadTempFileToPnt(PntTmpF, i, point); //从临时文件读取点
		if (point.isDel == isDel)
		{
			xy.x = point.x;
			xy.y = point.y;
			//坐标系转换（数据转窗口）
			PntDPtoVP(xy, zoom, zoomOffset_x, zoomOffset_y);
			point.x = xy.x;
			point.y = xy.y;
			DrawPnt(dc, point); //绘制点
		}
	}
}


/*显示所有线(新)的函数声明*/
void ShowAllLin(CClientDC* dc, CFile* LinTmpNdxF, CFile* LinTmpDatF, int LinNum, double zoomOffset_x, double zoomOffset_y, double zoom, char isDel)
{
	LIN_NDX_STRU line;
	for (int i = 0; i < LinNum; i++)
	{
		ReadTempFileToLinNdx(LinTmpNdxF, i, line); //从临时文件读取线索引
		if (line.isDel == isDel)
		{
			D_DOT dot1, dot2;
			POINT pnt1, pnt2;
			for (int j = 0; j < line.dotNum - 1; j++)
			{
				//从临时文件读取线的点数据
				ReadTempFileToLinDat(LinTmpDatF, line.datOff, j, dot1);
				ReadTempFileToLinDat(LinTmpDatF, line.datOff, j + 1, dot2);
				//坐标系转换（数据转窗口）
				PntDPtoVP(dot1, zoom, zoomOffset_x, zoomOffset_y);
				PntDPtoVP(dot2, zoom, zoomOffset_x, zoomOffset_y);
				//Dot转Pnt
				DotToPnt(pnt1, dot1);
				DotToPnt(pnt2, dot2);
				DrawSeg(dc, line, pnt1, pnt2); //绘制线
			}
		}
	}
}


/*显示区的函数声明*/
void ShowAllReg(CClientDC* dc, CFile* RegTmpNdxF, CFile* RegTmpDatF, int RegNum, double zoomOffset_x, double zoomOffset_y, double zoom, char isDel)
{
	REG_NDX_STRU Region;
	D_DOT pt;
	for (int i = 0; i < RegNum; i++)
	{
		ReadTempFileToRegNdx(RegTmpNdxF, i, Region);//从临时文件读取区索引
		D_DOT* dot;
		dot = (D_DOT*) malloc(Region.dotNum * sizeof(D_DOT));//开辟动态内存
		ZeroMemory(dot, Region.dotNum * sizeof(D_DOT));//0填充，初始化
		for (int j = 0; j < Region.dotNum; ++j)
		{
			//依次从区临时文件读取区的节点数据并将其转换为窗口坐标
			ReadTempFileToRegDat(RegTmpDatF, Region.datOff, j, pt);//读取数据
			PntDPtoVP(pt, zoom, zoomOffset_x, zoomOffset_y);//坐标转换
			dot[j] = pt;//存到数组
		}
		if (Region.isDel == isDel)
		{
			POINT* point = new POINT[Region.dotNum];
			DotToPnt(point, dot, Region.dotNum);
			DrawReg(dc, Region, point, Region.dotNum); //绘制区
			delete[] point;
		}
		free(dot);//释放动态内存
	}
}


/*绘制区的函数声明*/
void DrawReg(CClientDC* dc, REG_NDX_STRU Region, POINT* pt, long nPnt)
{
	CBrush brush(Region.color);
	CPen pen(PS_SOLID, 1, Region.color);
	CObject* oldObject;
	oldObject = dc -> SelectObject(&pen);
	CBitmap bitmap;
	switch (Region.pattern)
	{
	case 0://实心
		oldObject = dc->SelectObject(&brush);
		break;
	case 1://空心
		dc->SelectStockObject(NULL_BRUSH);
		break;
	//case 2://位图
	//	bitmap.LoadBitmap(IDB_BITMAP3);
	//	dc->SelectObject(bitmap);
	//	break;
	default:
		break;
	}
	dc->Polygon(pt, nPnt);//用当前pen画轮廓，用当前画刷填充多边形
	dc-> SelectObject(&oldObject);
}