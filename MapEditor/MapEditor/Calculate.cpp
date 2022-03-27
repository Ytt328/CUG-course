#include"pch.h"
#include"Calculate.h"
#include"WriteOrRead.h"
#include<math.h>

/*计算两点间距离*/
double Distance(double x1, double y1, double x2, double y2)
{
	return(sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)));
}

/*判断浮点数的大小*/
bool issmall(double x1, double x2)
{
	if (x1 <= x2)
	{
		return false;
	}
	else
	{
		return true;
	}
}

/*查找最近点*/
PNT_STRU Findpoint(CPoint mousePoint, int pntNum, CFile* pntTmpF, int& nPnt)
{
	PNT_STRU point, tPnt = { 0,0,RGB(0,0,0),0,0 };
	double min = 10.0;//在10个像素范围内查找
	double dst;
	for (int i = 0; i < pntNum; i++)
	{
		ReadTempFileToPnt(pntTmpF, i, point);
		if (point.isDel)
		{
			continue;
		}
		dst = Distance(mousePoint.x,mousePoint.y,point.x,point.y);
		if (dst<=min)
		{
			min = dst;
			tPnt = point;
			nPnt = i;
		}
	}
	return(tPnt);
}


/*计算鼠标到线的最短距离的函数*/
double DisPntToSeg(D_DOT pt1, D_DOT pt2, D_DOT pt)
{
	double a = (pt2.x - pt1.x )*( pt.x - pt1.x) + (pt2.y - pt1.y) * (pt.y - pt1.y);	//定义向量积a,指示夹角∠pt2pt1pt
	double b = (pt1.x - pt2.x) * (pt.x - pt2.x) + (pt1.y - pt2.y) * (pt.y - pt2.y);	//定义向量积b,指示夹角∠pt1py2pt
	if (a * b>1e-10)//a*b大于0可得出两个夹角都为锐角
	{
		double area;//这三点围成三角形的面积
		double hight;//点到线段的垂直距离
		//根据海伦公式计算三角形的面积
		double s = ((Distance(pt1.x, pt1.y, pt2.x, pt2.y)+ Distance(pt.x, pt.y, pt1.x, pt1.y)+ Distance(pt.x, pt.y, pt2.x, pt2.y))/2);
		area = sqrt(s * (s - Distance(pt.x, pt.y, pt1.x, pt1.y))* (s - Distance(pt.x, pt.y, pt2.x, pt2.y))* (s - Distance(pt1.x, pt1.y, pt2.x, pt2.y)));
		hight = 2 * area / Distance(pt1.x, pt1.y, pt2.x, pt2.y);//再根据面积的底边长求出高
		return hight;
	}
	else
	{
		//两个夹角中有一个是钝角或有一个是直角，鼠标到线的最短距离在鼠标与线段两端点间的距离产生
		return (Distance(pt.x, pt.y, pt1.x, pt1.y) > Distance(pt.x, pt.y, pt2.x,pt2.y))? Distance(pt.x, pt.y, pt2.x, pt2.y) : Distance(pt.x, pt.y, pt1.x, pt1.y);
	}
}


/*查找最近线的函数声明*/
LIN_NDX_STRU Findline(CFile* LinTmpNdxF, CFile* LinTmpDatF, CPoint mousePoint, int LinNum, int& nLinNdx)
{
	double min = 10;//在10像素内寻找
	LIN_NDX_STRU tLine = {//最近的线
            tLine.isDel = 0,
            tLine.color = RGB(0,0,0),
            tLine.pattern = 0 ,
            tLine.dotNum = 0 ,
            tLine.datOff = 0 };
	LIN_NDX_STRU line;
	D_DOT pt1, pt2, mpt;
	CFile tempLinDatF;
	for (int i = 0; i < LinNum; ++i)
	{
		ReadTempFileToLinNdx(LinTmpNdxF, i, line);//从临时线索引文件读取线索引
		if (line.isDel == 0)
		{
			for (int j = 0; j < line.dotNum - 1; j++)
			{
				ReadTempFileToLinDat(LinTmpDatF, line.datOff, j, pt1);//从临时线数据文件读取节点
				ReadTempFileToLinDat(LinTmpDatF, line.datOff, j + 1, pt2);//从临时线数据文件读取节点
				mpt.x = mousePoint.x;
				mpt.y = mousePoint.y;
				if (issmall(min, DisPntToSeg(pt1, pt2, mpt)))//鼠标到该线段的距离小于min
				{
					nLinNdx = i;
					min = DisPntToSeg(pt1, pt2, mpt);
					tLine = line;
				}
			}
		}
	}
	return tLine;
}


/*POINT转为D_DOT的函数（线，区）*/
void PntToDot(D_DOT* dot, POINT* pnt, int num)
{
	for (int i = 0; i < num; ++i)
	{
		dot[i].x = pnt[i].x;
		dot[i].y = pnt[i].y;
	}
}


/*POINT转为D_DOT的函数（点）*/
void PntToDot(D_DOT& dot, POINT pnt)
{
	dot.x = pnt.x;
	dot.y = pnt.y;
}


/*D_DOT转化为POINT的函数（线，区）*/
void DotToPnt(POINT* pnt, D_DOT* dot, int num)
{
	for (int i = 0; i < num; i++)
	{
		pnt[i].x = (long)dot[i].x;
		pnt[i].y = (long)dot[i].y;
	}
}


/*D_DOT转化为POINT的函数（点）*/
void DotToPnt(POINT& pnt, D_DOT dot)
{
	pnt.x = (long)dot.x;
	pnt.y = (long)dot.y;
}


/*数据坐标系转换到窗口坐标系的函数*/
void PntDPtoVP(D_DOT& pt, double zoom, double offset_x, double offset_y)
{
	pt.x = pt.x - offset_x;
	pt.y = pt.y - offset_y;
	pt.x = zoom * pt.x;
	pt.y = zoom * pt.y;
}


/*窗口坐标系转换到数据坐标系的函数*/
void PntVPtoDP(D_DOT& pt, double zoom, double offset_x, double offset_y)
{
	pt.x = pt.x / zoom;
	pt.y = pt.y / zoom;
	pt.x = pt.x + offset_x;
	pt.y = pt.y + offset_y;
}


/*计算矩形中心的函数*/
D_DOT GetCenter(RECT rect)
{
	D_DOT pt;
	pt.y = 0.5 * (rect.bottom + rect.top);
	pt.x = 0.5 * (rect.right + rect.left);
	return pt;
}


/*计算拉框放大时放大倍数的函数*/
void modulusZoom(RECT client, RECT rect, double& zoom)
{
	zoom = min(client.right / (double)(rect.right - rect.left),client.bottom / (double)(rect.bottom - rect.top));//用客户区的长比上矩形框的长，用客户区的宽比上矩形框的宽,取这两个比值中较小的一个作为本次放大的倍数
}


/*修改线的点数据*/
void AlterLindot(CFile* LinTmpDatF, LIN_NDX_STRU startLine, LIN_NDX_STRU endLine, int start, int end, long allDateOff)
{
	D_DOT pt1, pt2, pt3, pt4, point;
	int ndot = 0;
	//从临时文件中读取两条线的端点数据
	ReadTempFileToLinDat(LinTmpDatF, startLine.datOff, 0, pt1);
	ReadTempFileToLinDat(LinTmpDatF, startLine.datOff, startLine.dotNum-1, pt2);
	ReadTempFileToLinDat(LinTmpDatF, endLine.datOff, 0, pt3);
	ReadTempFileToLinDat(LinTmpDatF, endLine.datOff, endLine.dotNum - 1, pt4);
	double d1 = min(Distance(pt1.x, pt1.y, pt3.x, pt3.y), Distance(pt1.x, pt1.y, pt4.x, pt4.y));//第一条线起点到第二条线端点的最小距离
	double d2 = min(Distance(pt2.x, pt2.y, pt3.x, pt3.y), Distance(pt2.x, pt2.y, pt4.x, pt4.y));//第一条线终点到第二条线端点的最小距离
	if (d1 < d2)
	{
		//d1<d2,则第一条线起点与第二条线相连
		if (Distance(pt1.x, pt1.y, pt3.x, pt3.y) < Distance(pt1.x, pt1.y, pt4.x, pt4.y))
		{
			//第一条线起点与第二条线起点相连
			for (int i = endLine.dotNum - 1; i >= 0; --i)
			{
				//反向读取第二条线的节点并依次写入文件中
				ReadTempFileToLinDat(LinTmpDatF, endLine.datOff, i, point);
				WriteLinDatToFile(LinTmpDatF, allDateOff, ndot, point);
				ndot++;
			}
			for (int i = 0; i < startLine.dotNum; ++i)
			{
				//正向读取第一条线的节点并依次写入文件中
				ReadTempFileToLinDat(LinTmpDatF, startLine.datOff, i, point);
				WriteLinDatToFile(LinTmpDatF, allDateOff, ndot, point);
				ndot++;
			}
		}
		else
		{
			//第一条线起点与第二条线终点相连
			for (int i =0; i<endLine.dotNum; ++i)
			{
				//正向读取第二条线的节点并依次写入文件中
				ReadTempFileToLinDat(LinTmpDatF, endLine.datOff, i, point);
				WriteLinDatToFile(LinTmpDatF, allDateOff, ndot, point);
				ndot++;
			}
			for (int i = 0; i < startLine.dotNum; ++i)
			{
				//正向读取第一条线的节点并依次写入文件中
				ReadTempFileToLinDat(LinTmpDatF, startLine.datOff, i, point);
				WriteLinDatToFile(LinTmpDatF, allDateOff, ndot, point);
				ndot++;
			}
		}
	}
	else
	{
		//第一条线终点与第二条线端点相连
		if (Distance(pt2.x, pt2.y, pt3.x, pt3.y) < Distance(pt2.x, pt2.y, pt4.x, pt4.y))
		{
			//第一条线终点与第二条线起点相连
			for (int i = 0; i < startLine.dotNum; ++i)
			{
				//正向读取第一条线的节点并依次写入文件中
				ReadTempFileToLinDat(LinTmpDatF, startLine.datOff, i, point);
				WriteLinDatToFile(LinTmpDatF, allDateOff, ndot, point);
				ndot++;
			}
			for (int i = 0; i < endLine.dotNum; ++i)
			{
				//正向读取第二条线的节点并依次写入文件中
				ReadTempFileToLinDat(LinTmpDatF, endLine.datOff, i, point);
				WriteLinDatToFile(LinTmpDatF, allDateOff, ndot, point);
				ndot++;
			}
		}
		else
		{
			//第一条线终点与第二条线终点相连
			for (int i = 0; i < startLine.dotNum; ++i)
			{
				//正向读取第一条线的节点并依次写入文件中
				ReadTempFileToLinDat(LinTmpDatF, startLine.datOff, i, point);
				WriteLinDatToFile(LinTmpDatF, allDateOff, ndot, point);
				ndot++;
			}
			for (int i = endLine.dotNum-1; i>=0; --i)
			{
				//反向读取第二条线的节点并依次写入文件中
				ReadTempFileToLinDat(LinTmpDatF, endLine.datOff, i, point);
				WriteLinDatToFile(LinTmpDatF, allDateOff, ndot, point);
				ndot++;
			}
		}
	}
	ndot = 0;
}


/*判断单击的点是否在区内的函数声明*/
bool PtinPolygon(CPoint p, D_DOT* ptPolygon, int nCount)
{
	int nCross = 0;
	for (int i = 0; i < nCount; i++)
	{
		D_DOT p1 = ptPolygon[i];
		D_DOT p2 = ptPolygon[(i + 1)%nCount];
		//求解y=p.y与p1p2的交点
		if (p1.y == p2.y)//p1p2与y=p.y平行
		{
			continue;
		}
		if (p.y < min(p1.y, p2.y))//交点在p1p2延长线上
		{
			continue;
		}
		if (p.y >= max(p1.y, p2.y))//交点在p1p2延长线上
		{
			continue;
		}
		//求交点的x坐标
		double x = (double)(p.y - p1.y) * (double)(p2.x - p1.x) / (double)(p2.y - p1.y) + p1.x;
		if (x > p.x)
		{
			nCross++; //只统计单边交点
		}
	}
	//单边交点为偶数，点在多边形之外
		return (nCross%2 == 1);
}


/*查找离鼠标点最近区的函数声明*/
REG_NDX_STRU FindReg(CFile* RegTmpNdxF, CFile* RegTmpDatF, CPoint mousePoint, int RegNum, int& nRegNdx)
{
	REG_NDX_STRU RegNdx;
	REG_NDX_STRU tRegNdx = { tRegNdx.isDel = 0,
	                         tRegNdx.color=RGB(0,0,0),
		                     tRegNdx.pattern=0,
		                     tRegNdx.dotNum=0,
		                     tRegNdx.datOff=0};
	for (int i = 0; i < RegNum; i++)
	{
		ReadTempFileToRegNdx(RegTmpNdxF, i, RegNdx); //从临时文件读区索引
		if (RegNdx.isDel == 0)//当前区未被删除
		{
			D_DOT* pt = new D_DOT[RegNdx.dotNum];
			for (int j = 0; j < RegNdx.dotNum; j++)
			{
				//从临时文件中读取区的点数据
				ReadTempFileToRegDat(RegTmpDatF, RegNdx.datOff, j, pt[j]);
			}
			if (PtinPolygon(mousePoint, pt, RegNdx.dotNum))//点在区内
			{
				tRegNdx = RegNdx;
				nRegNdx = i;
				delete[] pt;
				break;
			}
			else
			{
				delete[]pt;
			}
		}
	}
	return tRegNdx;
}


/*临时索引文件更新区数据的函数声明*/
void UpdateReg(CFile* RegTmpNdxF, int nReg, REG_NDX_STRU Region)
{
	WriteRegNdxToFile(RegTmpNdxF, nReg, Region);
}

/*查找最近删除点的函数说明*/
PNT_STRU FindDeletePnt(CPoint mousePoint, int PntNum, CFile* PntTmpF, int& nPnt)
{
	PNT_STRU point;
	PNT_STRU tPnt = { tPnt.x = 0,
				   tPnt.y = 0,
				   tPnt.color = RGB(0,0,0),
				   tPnt.pattern = 0,
				   tPnt.isDel = 0 };
	double min = 10;//在10个像素范围内寻找
	for (int i = 0; i < PntNum; i++)
	{
		ReadTempFileToPnt(PntTmpF, i, point);//从临时点文件读取点到point
		if (!point.isDel)
		{
			continue;
		}
		double dist = Distance(mousePoint.x, mousePoint.y, point.x, point.y);//鼠标与找到的点之间的距离
		if (issmall(min, dist))
		{
			min = dist;
			tPnt = point;
			nPnt = i;
		}
	}
	return tPnt;
}


/*查找最近删除线的函数说明*/
LIN_NDX_STRU FindDeleteline(CFile* LinTmpNdxF, CFile* LinTmpDatF, CPoint mousePoint, int LinNum, int& nLinNdx)
{
	double min = 10;//在10像素内寻找
	LIN_NDX_STRU tLine = {//最近的线
			tLine.isDel = 0,
			tLine.color = RGB(0,0,0),
			tLine.pattern = 0 ,
			tLine.dotNum = 0 ,
			tLine.datOff = 0 };
	LIN_NDX_STRU line;
	D_DOT pt1, pt2, mpt;
	CFile tempLinDatF;
	for (int i = 0; i < LinNum; ++i)
	{
		ReadTempFileToLinNdx(LinTmpNdxF, i, line);//从临时线索引文件读取线索引
		if (!line.isDel == 0)
		{
			for (int j = 0; j < line.dotNum - 1; j++)
			{
				ReadTempFileToLinDat(LinTmpDatF, line.datOff, j, pt1);//从临时线数据文件读取节点
				ReadTempFileToLinDat(LinTmpDatF, line.datOff, j + 1, pt2);//从临时线数据文件读取节点
				mpt.x = mousePoint.x;
				mpt.y = mousePoint.y;
				if (issmall(min, DisPntToSeg(pt1, pt2, mpt)))//鼠标到该线段的距离小于min
				{
					nLinNdx = i;
					min = DisPntToSeg(pt1, pt2, mpt);
					tLine = line;
				}
			}
		}
	}
	return tLine;
}


/*查找最近删除区的函数说明*/
REG_NDX_STRU FindDeleteReg(CFile* RegTmpNdxF, CFile* RegTmpDatF, CPoint mousePoint, int RegNum, int& nRegNdx)
{
	REG_NDX_STRU RegNdx;
	REG_NDX_STRU tRegNdx = { tRegNdx.isDel = 0,
							 tRegNdx.color = RGB(0,0,0),
							 tRegNdx.pattern = 0,
							 tRegNdx.dotNum = 0,
							 tRegNdx.datOff = 0 };
	for (int i = 0; i < RegNum; i++)
	{
		ReadTempFileToRegNdx(RegTmpNdxF, i, RegNdx); //从临时文件读区索引
		if (!RegNdx.isDel == 0)//当前区未被删除
		{
			D_DOT* pt = new D_DOT[RegNdx.dotNum];
			for (int j = 0; j < RegNdx.dotNum; j++)
			{
				//从临时文件中读取区的点数据
				ReadTempFileToRegDat(RegTmpDatF, RegNdx.datOff, j, pt[j]);
			}
			if (PtinPolygon(mousePoint, pt, RegNdx.dotNum))//点在区内
			{
				tRegNdx = RegNdx;
				nRegNdx = i;
				delete[] pt;
				break;
			}
			else
			{
				delete[]pt;
			}
		}
	}
	return tRegNdx;
}


/*查找某条线上距离鼠标最近的点*/
D_DOT FindPntOnLin(CPoint mousePoint, CFile* LinTmpDatF,LIN_NDX_STRU tempLine,int& tempPntNum)
{
	       D_DOT tempDot;
		   D_DOT tempDot1;
	       ReadTempFileToLinDat(LinTmpDatF, tempLine.datOff, 0, tempDot);//读取该线第一个节点数据到tempDot中
	       int dis = Distance(mousePoint.x, mousePoint.y, tempDot.x, tempDot.y);//计算第一个节点到鼠标的距离
		   tempDot1 = tempDot;
	       tempPntNum = 0;
	       for (int i = 1; i < tempLine.dotNum; i++)
	       {
		        ReadTempFileToLinDat(LinTmpDatF, tempLine.datOff, i, tempDot);//读取节点数据到tempDot中
		        if (Distance(mousePoint.x, mousePoint.y, tempDot.x, tempDot.y)<dis)
		        {
			        dis = Distance(mousePoint.x, mousePoint.y, tempDot.x, tempDot.y);
			        tempPntNum = i;
					tempDot1 = tempDot;
		        }
	       }
	       return tempDot1;
}


/*删除线上离鼠标最近的点*/
void DelPntOnLin(CFile* LinTmpDatF, LIN_NDX_STRU& tempLine, int& tempPntNum)
{
	//进入此函数节点数一定大于等于3
	D_DOT dot;
	if (tempPntNum == 0)//若删除的是第一个节点
	{
		for (int i = 1; i < tempLine.dotNum; i++)
		{
			ReadTempFileToLinDat(LinTmpDatF, tempLine.datOff, i, dot);
			WriteLinDatToFile(LinTmpDatF, tempLine.datOff, i - 1, dot);
		}
	}
	else if (tempPntNum == tempLine.dotNum - 1)//若删除的是最后一个节点
	{
		ReadTempFileToLinDat(LinTmpDatF, tempLine.datOff, tempLine.dotNum-2, dot);
		WriteLinDatToFile(LinTmpDatF, tempLine.datOff, tempLine.dotNum - 1, dot);
	}
	else//删除的点既不是第一个节点也不是最后一个节点
	{
		for (int i = tempPntNum; i < tempLine.dotNum-1; i++)
		{
			ReadTempFileToLinDat(LinTmpDatF, tempLine.datOff,i+1, dot);
			WriteLinDatToFile(LinTmpDatF, tempLine.datOff, i, dot);
		}
	}
	tempLine.dotNum--;//线节点数减一

}