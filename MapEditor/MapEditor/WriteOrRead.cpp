#include"pch.h"
#include"WriteOrRead.h"


/*将点数据写入临时文件*/
void WritePntToFile(CFile* PntTmpF, int i, PNT_STRU point)
{
	PntTmpF->Seek(i*sizeof(PNT_STRU), CFile::begin);
	PntTmpF->Write(&point, sizeof(PNT_STRU));
}


/*从临时点文件读取点数据*/
void ReadTempFileToPnt(CFile* PntTmpF, int i, PNT_STRU& point)
{
	PntTmpF->Seek(i*sizeof(PNT_STRU), CFile::begin);//重新定位指针
	PntTmpF->Read(&point, sizeof(PNT_STRU)); //读取数据
}


/*从永久文件读到临时文件（点）*/
void ReadPntPermanentFileToTemp(CFile* pntF, CFile* pntTmpF, int &nPnt, int &nLPnt)
{
	PNT_STRU point;
	pntF -> Seek(sizeof(VERSION), CFile::begin);//将文件指针放到文件头后面
	pntF->Read(&nPnt, sizeof(int));//读点的物理个数
	pntF -> Read(&nLPnt, sizeof(int));//读点的逻辑个数
	for (int i = 0; i < nLPnt;++i)
	{
		pntF -> Read(&point, sizeof(PNT_STRU)); //逐个从永久文件读出点数据
		pntTmpF->Write(&point, sizeof(PNT_STRU)); //逐个写入点数据到临时文件 
	}
}


/*修改临时文件中点数据*/
void UpdatePnt(CFile* pntTmpF, int i, PNT_STRU pnt)
{
	WritePntToFile(pntTmpF, i, pnt);//将点数据写入临时文件
}


/*将第i条线的索引写入临时索引文件*/
void WriteLinNdxToFile(CFile* LinTmpNdxF, int i, LIN_NDX_STRU line)
{
	LinTmpNdxF -> Seek(i * sizeof(LIN_NDX_STRU), CFile::begin);
	LinTmpNdxF -> Write(&line, sizeof(LIN_NDX_STRU));
}


/*将线的点数据写入文件*/
void WriteLinDatToFile(CFile* LinTmpDatF, long datOff, int i, D_DOT point)
{
	LinTmpDatF -> Seek(datOff + i * sizeof(D_DOT), CFile::begin);
	LinTmpDatF -> Write(&point, sizeof(D_DOT));
}


/*从临时线数据文件中读取线的点数据*/
void ReadTempFileToLinDat(CFile* LinTmpDatF, long datOff, int i, D_DOT& pnt)
{
	LinTmpDatF -> Seek(datOff + i * sizeof(D_DOT), CFile::begin);
	LinTmpDatF -> Read(&pnt, sizeof(D_DOT));
}


/*从临时线索引文件中读取线索引的函数*/
void ReadTempFileToLinNdx(CFile* LinTmpNdxF, int i, LIN_NDX_STRU& LinNdx)
{
	LinTmpNdxF -> Seek(i * sizeof(LIN_NDX_STRU), CFile::begin);
	LinTmpNdxF -> Read(&LinNdx, sizeof(LIN_NDX_STRU));
}


/*将线的索引和点数据写入永久文件的函数声明*/
void WriteTempToLinPermanentFile(CFile* LinF, CFile* LinTmpDatF, CFile* LinTmpNdxF, VERSION LinVer, int nLin, int nLLin)
{
	LIN_NDX_STRU TempLinNdx;
	D_DOT Pnt;
	long LinNdxOffset = sizeof(VERSION) + sizeof(int) * 2;
	long LinDatOffset = LinNdxOffset + sizeof(LIN_NDX_STRU) * nLin;
	LinF->Write(&LinVer, sizeof(VERSION)); //写入版本信息
	LinF->Write(&nLin, sizeof(int)); //写入物理数
	LinF->Write(&nLLin, sizeof(int)); //写入逻辑数
	for (int i = 0; i < nLin; i++)
	{
		//从临时线索引文件中读取线索引
		ReadTempFileToLinNdx(LinTmpNdxF, i, TempLinNdx);
		LinF -> Seek(LinDatOffset, CFile::begin);
		for (int j = 0; j < TempLinNdx.dotNum; j++)
		{
			//从临时线数据文件中读取线的点数据
			ReadTempFileToLinDat(LinTmpDatF , TempLinNdx. datOff, j , Pnt);
			//将线的点数据写入永久文件
			LinF->Write(&Pnt, sizeof(D_DOT));
		}
		LinF -> Seek(LinNdxOffset, CFile::begin);
		TempLinNdx.datOff = LinDatOffset;
		LinF->Write(&TempLinNdx, sizeof(LIN_NDX_STRU));//将线的索引写入永久文件
		LinNdxOffset += sizeof(LIN_NDX_STRU);//线索引存放位置改变
		LinDatOffset += (sizeof(D_DOT) * TempLinNdx.dotNum);//线数据存放位置改变
	}
}


/*从永久文件读取线数据到临时文件的函数声明*/
void ReadLinPermanentFileToTemp(CFile* LinF, CFile* LinTmpDatF, CFile* LinTmpNdxF, VERSION& LinVer, int& nLin, int& nLLin, long& TmpFLinDatOffset)
{
	LinF->Seek(0, CFile::begin);
	LinF->Read(&LinVer, sizeof(VERSION));//读取版本信息
	LinF->Read(&nLin, sizeof(int)); //读物理个数
	LinF->Read(&nLLin, sizeof(int)); //读逻辑个数
	long LinNdxOffset = sizeof(VERSION) + sizeof(int) * 2;
	long LinDatOffset = LinNdxOffset + sizeof(LIN_NDX_STRU) * nLin;
	TmpFLinDatOffset = 0;
	LIN_NDX_STRU TempLinNdx;
	D_DOT Pnt;
	for (int i = 0; i < nLin; i++)
	{
		LinF -> Seek(LinNdxOffset, CFile::begin);
		LinF -> Read(&TempLinNdx, sizeof(LIN_NDX_STRU));//读取线索引文件到临时线索引文件中
		LinF -> Seek(TempLinNdx.datOff, CFile::begin);
		for (int j = 0; j < TempLinNdx.dotNum; j++)
		{
			LinF->Read(&Pnt, sizeof(D_DOT));
			LinTmpDatF -> Write(&Pnt, sizeof(D_DOT));//读取线数据文件到临时数据文件中
		}
		TempLinNdx.datOff = TmpFLinDatOffset;
		LinTmpNdxF -> Write(&TempLinNdx, sizeof(LIN_NDX_STRU));//读取线索引文件到临时线索引文件中
		TmpFLinDatOffset += (sizeof(D_DOT) * TempLinNdx.dotNum);
		LinDatOffset += (sizeof(D_DOT) * TempLinNdx.dotNum);
		LinNdxOffset += sizeof(LIN_NDX_STRU);
	}
}


/*更新线临时文件中线数据*/
void UpdateLin(CFile* LinTmpNdxF, int nLin, LIN_NDX_STRU line)
{
	WriteLinNdxToFile(LinTmpNdxF , nLin,line);//重新写入第nLine条线索引
}


/*更新线的点数据到临时文件的函数*/
void UpdateLin(CFile* LinTmpNdxF, CFile* LinTmpDatF, int LinNdx, double offset_x, double offset_y)
{
	LIN_NDX_STRU tLin;
	D_DOT pt;
	ReadTempFileToLinNdx(LinTmpNdxF, LinNdx, tLin);//从点索引文件读取相应的线索引到tLin
	for (int i = 0; i < tLin.dotNum; i++)
	{
		//读取节点数据到pt中
		LinTmpDatF -> Seek(tLin.datOff+i*sizeof(D_DOT), CFile::begin);
		LinTmpDatF -> Read(&pt, sizeof(D_DOT));
		//设置pt的偏移，即节点的偏移
		pt.x = pt.x + offset_x;
		pt.y = pt.y + offset_y;
		//重新写入节点数据
		LinTmpDatF->Seek(tLin.datOff+i*sizeof(D_DOT), CFile:: begin);
		LinTmpDatF->Write(&pt, sizeof(D_DOT));
	}
}


/*修改第一条线索引的函数*/
void AlterStartLin(CFile* LinTmpNdxF, long subdatoff, int nLine, int subNum)
{
	LIN_NDX_STRU LinNdx;
	//读取线
	LinTmpNdxF->Seek(nLine*sizeof(LIN_NDX_STRU),CFile::begin);
	LinTmpNdxF->Read(&LinNdx, sizeof(LIN_NDX_STRU));
	//更新线
	LinNdx.datOff = subdatoff;//更新线的点索引
	LinNdx.dotNum = subNum;//更新线的节点数
	//重新写入线
	LinTmpNdxF->Seek(nLine * sizeof(LIN_NDX_STRU), CFile::begin);
	LinTmpNdxF->Write(&LinNdx, sizeof(LIN_NDX_STRU));
}


/*修改第二条线索引的函数*/
void AlterEndLin(CFile* LinTmpNdxF, int nLine)
{
	LIN_NDX_STRU  linNdx;
	//读取线
	LinTmpNdxF->Seek(nLine * sizeof(LIN_NDX_STRU), CFile::begin);
	LinTmpNdxF->Read(&linNdx, sizeof(LIN_NDX_STRU));
	//更新线
	linNdx.dotNum = 0;
	linNdx.isDel = 1;
	//重新写入线
	LinTmpNdxF->Seek(nLine * sizeof(LIN_NDX_STRU), CFile::begin);
	LinTmpNdxF->Write(&linNdx, sizeof(LIN_NDX_STRU));
}


/*将区索引数据写入区临时索引文件的函数*/
void WriteRegNdxToFile(CFile* RegTmpNdxF, int i, REG_NDX_STRU Region)
{
	RegTmpNdxF->Seek(i * sizeof(REG_NDX_STRU), CFile::begin);
	RegTmpNdxF->Write(&Region, sizeof(REG_NDX_STRU));
}


/*将区节点数据写入区临时数据文件的函数声明*/
void WriteRegDatToFile(CFile* RegTmpDatF, long datOff, int i, D_DOT point)
{
	RegTmpDatF->Seek(datOff + i*sizeof(D_DOT), CFile::begin);
	RegTmpDatF->Write(&point, sizeof(D_DOT));
}


/*从区临时文件中读取区索引的函数声明*/
void ReadTempFileToRegNdx(CFile* RegTmpNdxF, int i, REG_NDX_STRU& RegNdx)
{
	RegTmpNdxF->Seek(i * sizeof(REG_NDX_STRU), CFile::begin);
	RegTmpNdxF->Read(&RegNdx, sizeof(REG_NDX_STRU));
}


/*从区临时文件中读取区节点数据的函数声明*/
void ReadTempFileToRegDat(CFile* RegTmpDatF, long datOff, int i, D_DOT& Pnt)
{
	RegTmpDatF->Seek(datOff + i * sizeof(D_DOT), CFile::begin);
	RegTmpDatF->Read(&Pnt, sizeof(D_DOT));
}


/*将区的索引和点数据写入永久文件的函数声明*/
void WriteTempToRegPermanentFile(CFile* RegF, CFile* RegTmpDatF, CFile* RegTmpNdxF, VERSION RegVer, int nReg, int nLReg)
{
	REG_NDX_STRU TempRegNdx;//用于储存读入的索引文件
	D_DOT Pnt;//用于储存读入的数据文件
	long RegNdxOffset = sizeof(VERSION) + sizeof(int) * 2;//永久文件开头一个VERSION结构，一个物理数，一个逻辑数
	long RegDatOffset = RegNdxOffset + sizeof(REG_NDX_STRU) * nReg;//永久文件开头+索引文件大小
	RegF->Write(&RegVer, sizeof(VERSION));//写入版本信息
	RegF->Write(&nReg, sizeof(int));//写入物理数
	RegF->Write(&nLReg, sizeof(int));//写入逻辑数
	for (int i = 0; i < nReg; i++) {
		//从临时区索引文件中读取线索引
		ReadTempFileToRegNdx(RegTmpNdxF, i, TempRegNdx);
		RegF->Seek(RegDatOffset, CFile::begin);
		for (int j = 0; j < TempRegNdx.dotNum; j++) {
			//从临时区数据文件中读取线的点数据
			ReadTempFileToRegDat(RegTmpDatF, TempRegNdx.datOff, j, Pnt);
			//将区的点数据写入永久文件
			RegF->Write(&Pnt, sizeof(D_DOT));
		}
		RegF->Seek(RegNdxOffset, CFile::begin);//指针定位到索引文件开头
		TempRegNdx.datOff = RegDatOffset;//设置结点数据储存位置
		RegF->Write(&TempRegNdx, sizeof(REG_NDX_STRU));//将线的索引写入永久文件
		RegNdxOffset += sizeof(REG_NDX_STRU);
		RegDatOffset += (sizeof(D_DOT) * TempRegNdx.dotNum);
	}
}

/*从永久文件读取区数据到临时文件的函数声明*/
void ReadRegPermanentFileToTemp(CFile* RegF, CFile* RegTmpDatF, CFile* RegTmpNdxF, VERSION& RegVer, int& nReg, int& nLReg, long& TmpFRegDatOffset)
{
	RegF->Seek(0, CFile::begin);
	RegF->Read(&RegVer, sizeof(VERSION));
	RegF->Read(&nReg, sizeof(int)); //读物理个数
	RegF->Read(&nLReg, sizeof(int));//读逻辑个数
	long RegNdxOffset = sizeof(VERSION) + sizeof(int) * 2;//永久文件开头一个VERSION结构，一个物理数，一个逻辑数
	long RegDatOffset = RegNdxOffset + sizeof(REG_NDX_STRU) * nReg;//永久文件开头+索引文件大小
	TmpFRegDatOffset = 0;//临时区数据文件偏移量
	REG_NDX_STRU  TempRegNdx;
	D_DOT Pnt;
	for (int i = 0; i < nReg; i++) {
		RegF->Seek(RegNdxOffset, CFile::begin);//索引文件定位
		RegF->Read(&TempRegNdx, sizeof(REG_NDX_STRU));
		RegF->Seek(TempRegNdx.datOff, CFile::begin);//数据文件定位
		for (int j = 0; j < TempRegNdx.dotNum; j++) {
			RegF->Read(&Pnt, sizeof(D_DOT));
			RegTmpDatF->Write(&Pnt, sizeof(D_DOT));

		}
		TempRegNdx.datOff = TmpFRegDatOffset;
		RegTmpNdxF->Write(&TempRegNdx, sizeof(REG_NDX_STRU));
		TmpFRegDatOffset += (sizeof(D_DOT) * TempRegNdx.dotNum);
		RegDatOffset += (sizeof(D_DOT) * TempRegNdx.dotNum);
		RegNdxOffset += sizeof(REG_NDX_STRU);
	}
}


/*更新区的函数声明*/
void UpdateReg(CFile* RegTmpNdxF, CFile* RegTmpDatF, int RegNdx, double offset_x, double offset_y)
{
	REG_NDX_STRU tReg;
	D_DOT pt;
	ReadTempFileToRegNdx(RegTmpNdxF, RegNdx, tReg);//从区临时文件读取区索引到tReg
	for (int i = 0; i < tReg.dotNum; i++)
	{
		//读取节点数据到pt
		RegTmpDatF->Seek(tReg.datOff + i * sizeof(D_DOT), CFile:: begin);
		RegTmpDatF->Read(&pt, sizeof(D_DOT));
		//实现节点的偏移
		pt.x = pt.x + offset_x;
		pt.y = pt.y + offset_y;
		//重新写入节点数据
		RegTmpDatF->Seek(tReg.datOff + i * sizeof(D_DOT), CFile:: begin);
		RegTmpDatF->Write(&pt, sizeof(D_DOT));
	}
}