#include"pch.h"
#include"Calculate.h"
#include"WriteOrRead.h"
#include<math.h>

/*������������*/
double Distance(double x1, double y1, double x2, double y2)
{
	return(sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)));
}

/*�жϸ������Ĵ�С*/
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

/*���������*/
PNT_STRU Findpoint(CPoint mousePoint, int pntNum, CFile* pntTmpF, int& nPnt)
{
	PNT_STRU point, tPnt = { 0,0,RGB(0,0,0),0,0 };
	double min = 10.0;//��10�����ط�Χ�ڲ���
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


/*������굽�ߵ���̾���ĺ���*/
double DisPntToSeg(D_DOT pt1, D_DOT pt2, D_DOT pt)
{
	double a = (pt2.x - pt1.x )*( pt.x - pt1.x) + (pt2.y - pt1.y) * (pt.y - pt1.y);	//����������a,ָʾ�нǡ�pt2pt1pt
	double b = (pt1.x - pt2.x) * (pt.x - pt2.x) + (pt1.y - pt2.y) * (pt.y - pt2.y);	//����������b,ָʾ�нǡ�pt1py2pt
	if (a * b>1e-10)//a*b����0�ɵó������нǶ�Ϊ���
	{
		double area;//������Χ�������ε����
		double hight;//�㵽�߶εĴ�ֱ����
		//���ݺ��׹�ʽ���������ε����
		double s = ((Distance(pt1.x, pt1.y, pt2.x, pt2.y)+ Distance(pt.x, pt.y, pt1.x, pt1.y)+ Distance(pt.x, pt.y, pt2.x, pt2.y))/2);
		area = sqrt(s * (s - Distance(pt.x, pt.y, pt1.x, pt1.y))* (s - Distance(pt.x, pt.y, pt2.x, pt2.y))* (s - Distance(pt1.x, pt1.y, pt2.x, pt2.y)));
		hight = 2 * area / Distance(pt1.x, pt1.y, pt2.x, pt2.y);//�ٸ�������ĵױ߳������
		return hight;
	}
	else
	{
		//�����н�����һ���Ƕ۽ǻ���һ����ֱ�ǣ���굽�ߵ���̾�����������߶����˵��ľ������
		return (Distance(pt.x, pt.y, pt1.x, pt1.y) > Distance(pt.x, pt.y, pt2.x,pt2.y))? Distance(pt.x, pt.y, pt2.x, pt2.y) : Distance(pt.x, pt.y, pt1.x, pt1.y);
	}
}


/*��������ߵĺ�������*/
LIN_NDX_STRU Findline(CFile* LinTmpNdxF, CFile* LinTmpDatF, CPoint mousePoint, int LinNum, int& nLinNdx)
{
	double min = 10;//��10������Ѱ��
	LIN_NDX_STRU tLine = {//�������
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
		ReadTempFileToLinNdx(LinTmpNdxF, i, line);//����ʱ�������ļ���ȡ������
		if (line.isDel == 0)
		{
			for (int j = 0; j < line.dotNum - 1; j++)
			{
				ReadTempFileToLinDat(LinTmpDatF, line.datOff, j, pt1);//����ʱ�������ļ���ȡ�ڵ�
				ReadTempFileToLinDat(LinTmpDatF, line.datOff, j + 1, pt2);//����ʱ�������ļ���ȡ�ڵ�
				mpt.x = mousePoint.x;
				mpt.y = mousePoint.y;
				if (issmall(min, DisPntToSeg(pt1, pt2, mpt)))//��굽���߶εľ���С��min
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


/*POINTתΪD_DOT�ĺ������ߣ�����*/
void PntToDot(D_DOT* dot, POINT* pnt, int num)
{
	for (int i = 0; i < num; ++i)
	{
		dot[i].x = pnt[i].x;
		dot[i].y = pnt[i].y;
	}
}


/*POINTתΪD_DOT�ĺ������㣩*/
void PntToDot(D_DOT& dot, POINT pnt)
{
	dot.x = pnt.x;
	dot.y = pnt.y;
}


/*D_DOTת��ΪPOINT�ĺ������ߣ�����*/
void DotToPnt(POINT* pnt, D_DOT* dot, int num)
{
	for (int i = 0; i < num; i++)
	{
		pnt[i].x = (long)dot[i].x;
		pnt[i].y = (long)dot[i].y;
	}
}


/*D_DOTת��ΪPOINT�ĺ������㣩*/
void DotToPnt(POINT& pnt, D_DOT dot)
{
	pnt.x = (long)dot.x;
	pnt.y = (long)dot.y;
}


/*��������ϵת������������ϵ�ĺ���*/
void PntDPtoVP(D_DOT& pt, double zoom, double offset_x, double offset_y)
{
	pt.x = pt.x - offset_x;
	pt.y = pt.y - offset_y;
	pt.x = zoom * pt.x;
	pt.y = zoom * pt.y;
}


/*��������ϵת������������ϵ�ĺ���*/
void PntVPtoDP(D_DOT& pt, double zoom, double offset_x, double offset_y)
{
	pt.x = pt.x / zoom;
	pt.y = pt.y / zoom;
	pt.x = pt.x + offset_x;
	pt.y = pt.y + offset_y;
}


/*����������ĵĺ���*/
D_DOT GetCenter(RECT rect)
{
	D_DOT pt;
	pt.y = 0.5 * (rect.bottom + rect.top);
	pt.x = 0.5 * (rect.right + rect.left);
	return pt;
}


/*��������Ŵ�ʱ�Ŵ����ĺ���*/
void modulusZoom(RECT client, RECT rect, double& zoom)
{
	zoom = min(client.right / (double)(rect.right - rect.left),client.bottom / (double)(rect.bottom - rect.top));//�ÿͻ����ĳ����Ͼ��ο�ĳ����ÿͻ����Ŀ���Ͼ��ο�Ŀ�,ȡ��������ֵ�н�С��һ����Ϊ���ηŴ�ı���
}


/*�޸��ߵĵ�����*/
void AlterLindot(CFile* LinTmpDatF, LIN_NDX_STRU startLine, LIN_NDX_STRU endLine, int start, int end, long allDateOff)
{
	D_DOT pt1, pt2, pt3, pt4, point;
	int ndot = 0;
	//����ʱ�ļ��ж�ȡ�����ߵĶ˵�����
	ReadTempFileToLinDat(LinTmpDatF, startLine.datOff, 0, pt1);
	ReadTempFileToLinDat(LinTmpDatF, startLine.datOff, startLine.dotNum-1, pt2);
	ReadTempFileToLinDat(LinTmpDatF, endLine.datOff, 0, pt3);
	ReadTempFileToLinDat(LinTmpDatF, endLine.datOff, endLine.dotNum - 1, pt4);
	double d1 = min(Distance(pt1.x, pt1.y, pt3.x, pt3.y), Distance(pt1.x, pt1.y, pt4.x, pt4.y));//��һ������㵽�ڶ����߶˵����С����
	double d2 = min(Distance(pt2.x, pt2.y, pt3.x, pt3.y), Distance(pt2.x, pt2.y, pt4.x, pt4.y));//��һ�����յ㵽�ڶ����߶˵����С����
	if (d1 < d2)
	{
		//d1<d2,���һ���������ڶ���������
		if (Distance(pt1.x, pt1.y, pt3.x, pt3.y) < Distance(pt1.x, pt1.y, pt4.x, pt4.y))
		{
			//��һ���������ڶ������������
			for (int i = endLine.dotNum - 1; i >= 0; --i)
			{
				//�����ȡ�ڶ����ߵĽڵ㲢����д���ļ���
				ReadTempFileToLinDat(LinTmpDatF, endLine.datOff, i, point);
				WriteLinDatToFile(LinTmpDatF, allDateOff, ndot, point);
				ndot++;
			}
			for (int i = 0; i < startLine.dotNum; ++i)
			{
				//�����ȡ��һ���ߵĽڵ㲢����д���ļ���
				ReadTempFileToLinDat(LinTmpDatF, startLine.datOff, i, point);
				WriteLinDatToFile(LinTmpDatF, allDateOff, ndot, point);
				ndot++;
			}
		}
		else
		{
			//��һ���������ڶ������յ�����
			for (int i =0; i<endLine.dotNum; ++i)
			{
				//�����ȡ�ڶ����ߵĽڵ㲢����д���ļ���
				ReadTempFileToLinDat(LinTmpDatF, endLine.datOff, i, point);
				WriteLinDatToFile(LinTmpDatF, allDateOff, ndot, point);
				ndot++;
			}
			for (int i = 0; i < startLine.dotNum; ++i)
			{
				//�����ȡ��һ���ߵĽڵ㲢����д���ļ���
				ReadTempFileToLinDat(LinTmpDatF, startLine.datOff, i, point);
				WriteLinDatToFile(LinTmpDatF, allDateOff, ndot, point);
				ndot++;
			}
		}
	}
	else
	{
		//��һ�����յ���ڶ����߶˵�����
		if (Distance(pt2.x, pt2.y, pt3.x, pt3.y) < Distance(pt2.x, pt2.y, pt4.x, pt4.y))
		{
			//��һ�����յ���ڶ������������
			for (int i = 0; i < startLine.dotNum; ++i)
			{
				//�����ȡ��һ���ߵĽڵ㲢����д���ļ���
				ReadTempFileToLinDat(LinTmpDatF, startLine.datOff, i, point);
				WriteLinDatToFile(LinTmpDatF, allDateOff, ndot, point);
				ndot++;
			}
			for (int i = 0; i < endLine.dotNum; ++i)
			{
				//�����ȡ�ڶ����ߵĽڵ㲢����д���ļ���
				ReadTempFileToLinDat(LinTmpDatF, endLine.datOff, i, point);
				WriteLinDatToFile(LinTmpDatF, allDateOff, ndot, point);
				ndot++;
			}
		}
		else
		{
			//��һ�����յ���ڶ������յ�����
			for (int i = 0; i < startLine.dotNum; ++i)
			{
				//�����ȡ��һ���ߵĽڵ㲢����д���ļ���
				ReadTempFileToLinDat(LinTmpDatF, startLine.datOff, i, point);
				WriteLinDatToFile(LinTmpDatF, allDateOff, ndot, point);
				ndot++;
			}
			for (int i = endLine.dotNum-1; i>=0; --i)
			{
				//�����ȡ�ڶ����ߵĽڵ㲢����д���ļ���
				ReadTempFileToLinDat(LinTmpDatF, endLine.datOff, i, point);
				WriteLinDatToFile(LinTmpDatF, allDateOff, ndot, point);
				ndot++;
			}
		}
	}
	ndot = 0;
}


/*�жϵ����ĵ��Ƿ������ڵĺ�������*/
bool PtinPolygon(CPoint p, D_DOT* ptPolygon, int nCount)
{
	int nCross = 0;
	for (int i = 0; i < nCount; i++)
	{
		D_DOT p1 = ptPolygon[i];
		D_DOT p2 = ptPolygon[(i + 1)%nCount];
		//���y=p.y��p1p2�Ľ���
		if (p1.y == p2.y)//p1p2��y=p.yƽ��
		{
			continue;
		}
		if (p.y < min(p1.y, p2.y))//������p1p2�ӳ�����
		{
			continue;
		}
		if (p.y >= max(p1.y, p2.y))//������p1p2�ӳ�����
		{
			continue;
		}
		//�󽻵��x����
		double x = (double)(p.y - p1.y) * (double)(p2.x - p1.x) / (double)(p2.y - p1.y) + p1.x;
		if (x > p.x)
		{
			nCross++; //ֻͳ�Ƶ��߽���
		}
	}
	//���߽���Ϊż�������ڶ����֮��
		return (nCross%2 == 1);
}


/*����������������ĺ�������*/
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
		ReadTempFileToRegNdx(RegTmpNdxF, i, RegNdx); //����ʱ�ļ���������
		if (RegNdx.isDel == 0)//��ǰ��δ��ɾ��
		{
			D_DOT* pt = new D_DOT[RegNdx.dotNum];
			for (int j = 0; j < RegNdx.dotNum; j++)
			{
				//����ʱ�ļ��ж�ȡ���ĵ�����
				ReadTempFileToRegDat(RegTmpDatF, RegNdx.datOff, j, pt[j]);
			}
			if (PtinPolygon(mousePoint, pt, RegNdx.dotNum))//��������
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


/*��ʱ�����ļ����������ݵĺ�������*/
void UpdateReg(CFile* RegTmpNdxF, int nReg, REG_NDX_STRU Region)
{
	WriteRegNdxToFile(RegTmpNdxF, nReg, Region);
}

/*�������ɾ����ĺ���˵��*/
PNT_STRU FindDeletePnt(CPoint mousePoint, int PntNum, CFile* PntTmpF, int& nPnt)
{
	PNT_STRU point;
	PNT_STRU tPnt = { tPnt.x = 0,
				   tPnt.y = 0,
				   tPnt.color = RGB(0,0,0),
				   tPnt.pattern = 0,
				   tPnt.isDel = 0 };
	double min = 10;//��10�����ط�Χ��Ѱ��
	for (int i = 0; i < PntNum; i++)
	{
		ReadTempFileToPnt(PntTmpF, i, point);//����ʱ���ļ���ȡ�㵽point
		if (!point.isDel)
		{
			continue;
		}
		double dist = Distance(mousePoint.x, mousePoint.y, point.x, point.y);//������ҵ��ĵ�֮��ľ���
		if (issmall(min, dist))
		{
			min = dist;
			tPnt = point;
			nPnt = i;
		}
	}
	return tPnt;
}


/*�������ɾ���ߵĺ���˵��*/
LIN_NDX_STRU FindDeleteline(CFile* LinTmpNdxF, CFile* LinTmpDatF, CPoint mousePoint, int LinNum, int& nLinNdx)
{
	double min = 10;//��10������Ѱ��
	LIN_NDX_STRU tLine = {//�������
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
		ReadTempFileToLinNdx(LinTmpNdxF, i, line);//����ʱ�������ļ���ȡ������
		if (!line.isDel == 0)
		{
			for (int j = 0; j < line.dotNum - 1; j++)
			{
				ReadTempFileToLinDat(LinTmpDatF, line.datOff, j, pt1);//����ʱ�������ļ���ȡ�ڵ�
				ReadTempFileToLinDat(LinTmpDatF, line.datOff, j + 1, pt2);//����ʱ�������ļ���ȡ�ڵ�
				mpt.x = mousePoint.x;
				mpt.y = mousePoint.y;
				if (issmall(min, DisPntToSeg(pt1, pt2, mpt)))//��굽���߶εľ���С��min
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


/*�������ɾ�����ĺ���˵��*/
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
		ReadTempFileToRegNdx(RegTmpNdxF, i, RegNdx); //����ʱ�ļ���������
		if (!RegNdx.isDel == 0)//��ǰ��δ��ɾ��
		{
			D_DOT* pt = new D_DOT[RegNdx.dotNum];
			for (int j = 0; j < RegNdx.dotNum; j++)
			{
				//����ʱ�ļ��ж�ȡ���ĵ�����
				ReadTempFileToRegDat(RegTmpDatF, RegNdx.datOff, j, pt[j]);
			}
			if (PtinPolygon(mousePoint, pt, RegNdx.dotNum))//��������
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


/*����ĳ�����Ͼ����������ĵ�*/
D_DOT FindPntOnLin(CPoint mousePoint, CFile* LinTmpDatF,LIN_NDX_STRU tempLine,int& tempPntNum)
{
	       D_DOT tempDot;
		   D_DOT tempDot1;
	       ReadTempFileToLinDat(LinTmpDatF, tempLine.datOff, 0, tempDot);//��ȡ���ߵ�һ���ڵ����ݵ�tempDot��
	       int dis = Distance(mousePoint.x, mousePoint.y, tempDot.x, tempDot.y);//�����һ���ڵ㵽���ľ���
		   tempDot1 = tempDot;
	       tempPntNum = 0;
	       for (int i = 1; i < tempLine.dotNum; i++)
	       {
		        ReadTempFileToLinDat(LinTmpDatF, tempLine.datOff, i, tempDot);//��ȡ�ڵ����ݵ�tempDot��
		        if (Distance(mousePoint.x, mousePoint.y, tempDot.x, tempDot.y)<dis)
		        {
			        dis = Distance(mousePoint.x, mousePoint.y, tempDot.x, tempDot.y);
			        tempPntNum = i;
					tempDot1 = tempDot;
		        }
	       }
	       return tempDot1;
}


/*ɾ���������������ĵ�*/
void DelPntOnLin(CFile* LinTmpDatF, LIN_NDX_STRU& tempLine, int& tempPntNum)
{
	//����˺����ڵ���һ�����ڵ���3
	D_DOT dot;
	if (tempPntNum == 0)//��ɾ�����ǵ�һ���ڵ�
	{
		for (int i = 1; i < tempLine.dotNum; i++)
		{
			ReadTempFileToLinDat(LinTmpDatF, tempLine.datOff, i, dot);
			WriteLinDatToFile(LinTmpDatF, tempLine.datOff, i - 1, dot);
		}
	}
	else if (tempPntNum == tempLine.dotNum - 1)//��ɾ���������һ���ڵ�
	{
		ReadTempFileToLinDat(LinTmpDatF, tempLine.datOff, tempLine.dotNum-2, dot);
		WriteLinDatToFile(LinTmpDatF, tempLine.datOff, tempLine.dotNum - 1, dot);
	}
	else//ɾ���ĵ�Ȳ��ǵ�һ���ڵ�Ҳ�������һ���ڵ�
	{
		for (int i = tempPntNum; i < tempLine.dotNum-1; i++)
		{
			ReadTempFileToLinDat(LinTmpDatF, tempLine.datOff,i+1, dot);
			WriteLinDatToFile(LinTmpDatF, tempLine.datOff, i, dot);
		}
	}
	tempLine.dotNum--;//�߽ڵ�����һ

}