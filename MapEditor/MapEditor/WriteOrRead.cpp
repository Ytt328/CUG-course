#include"pch.h"
#include"WriteOrRead.h"


/*��������д����ʱ�ļ�*/
void WritePntToFile(CFile* PntTmpF, int i, PNT_STRU point)
{
	PntTmpF->Seek(i*sizeof(PNT_STRU), CFile::begin);
	PntTmpF->Write(&point, sizeof(PNT_STRU));
}


/*����ʱ���ļ���ȡ������*/
void ReadTempFileToPnt(CFile* PntTmpF, int i, PNT_STRU& point)
{
	PntTmpF->Seek(i*sizeof(PNT_STRU), CFile::begin);//���¶�λָ��
	PntTmpF->Read(&point, sizeof(PNT_STRU)); //��ȡ����
}


/*�������ļ�������ʱ�ļ����㣩*/
void ReadPntPermanentFileToTemp(CFile* pntF, CFile* pntTmpF, int &nPnt, int &nLPnt)
{
	PNT_STRU point;
	pntF -> Seek(sizeof(VERSION), CFile::begin);//���ļ�ָ��ŵ��ļ�ͷ����
	pntF->Read(&nPnt, sizeof(int));//������������
	pntF -> Read(&nLPnt, sizeof(int));//������߼�����
	for (int i = 0; i < nLPnt;++i)
	{
		pntF -> Read(&point, sizeof(PNT_STRU)); //����������ļ�����������
		pntTmpF->Write(&point, sizeof(PNT_STRU)); //���д������ݵ���ʱ�ļ� 
	}
}


/*�޸���ʱ�ļ��е�����*/
void UpdatePnt(CFile* pntTmpF, int i, PNT_STRU pnt)
{
	WritePntToFile(pntTmpF, i, pnt);//��������д����ʱ�ļ�
}


/*����i���ߵ�����д����ʱ�����ļ�*/
void WriteLinNdxToFile(CFile* LinTmpNdxF, int i, LIN_NDX_STRU line)
{
	LinTmpNdxF -> Seek(i * sizeof(LIN_NDX_STRU), CFile::begin);
	LinTmpNdxF -> Write(&line, sizeof(LIN_NDX_STRU));
}


/*���ߵĵ�����д���ļ�*/
void WriteLinDatToFile(CFile* LinTmpDatF, long datOff, int i, D_DOT point)
{
	LinTmpDatF -> Seek(datOff + i * sizeof(D_DOT), CFile::begin);
	LinTmpDatF -> Write(&point, sizeof(D_DOT));
}


/*����ʱ�������ļ��ж�ȡ�ߵĵ�����*/
void ReadTempFileToLinDat(CFile* LinTmpDatF, long datOff, int i, D_DOT& pnt)
{
	LinTmpDatF -> Seek(datOff + i * sizeof(D_DOT), CFile::begin);
	LinTmpDatF -> Read(&pnt, sizeof(D_DOT));
}


/*����ʱ�������ļ��ж�ȡ�������ĺ���*/
void ReadTempFileToLinNdx(CFile* LinTmpNdxF, int i, LIN_NDX_STRU& LinNdx)
{
	LinTmpNdxF -> Seek(i * sizeof(LIN_NDX_STRU), CFile::begin);
	LinTmpNdxF -> Read(&LinNdx, sizeof(LIN_NDX_STRU));
}


/*���ߵ������͵�����д�������ļ��ĺ�������*/
void WriteTempToLinPermanentFile(CFile* LinF, CFile* LinTmpDatF, CFile* LinTmpNdxF, VERSION LinVer, int nLin, int nLLin)
{
	LIN_NDX_STRU TempLinNdx;
	D_DOT Pnt;
	long LinNdxOffset = sizeof(VERSION) + sizeof(int) * 2;
	long LinDatOffset = LinNdxOffset + sizeof(LIN_NDX_STRU) * nLin;
	LinF->Write(&LinVer, sizeof(VERSION)); //д��汾��Ϣ
	LinF->Write(&nLin, sizeof(int)); //д��������
	LinF->Write(&nLLin, sizeof(int)); //д���߼���
	for (int i = 0; i < nLin; i++)
	{
		//����ʱ�������ļ��ж�ȡ������
		ReadTempFileToLinNdx(LinTmpNdxF, i, TempLinNdx);
		LinF -> Seek(LinDatOffset, CFile::begin);
		for (int j = 0; j < TempLinNdx.dotNum; j++)
		{
			//����ʱ�������ļ��ж�ȡ�ߵĵ�����
			ReadTempFileToLinDat(LinTmpDatF , TempLinNdx. datOff, j , Pnt);
			//���ߵĵ�����д�������ļ�
			LinF->Write(&Pnt, sizeof(D_DOT));
		}
		LinF -> Seek(LinNdxOffset, CFile::begin);
		TempLinNdx.datOff = LinDatOffset;
		LinF->Write(&TempLinNdx, sizeof(LIN_NDX_STRU));//���ߵ�����д�������ļ�
		LinNdxOffset += sizeof(LIN_NDX_STRU);//���������λ�øı�
		LinDatOffset += (sizeof(D_DOT) * TempLinNdx.dotNum);//�����ݴ��λ�øı�
	}
}


/*�������ļ���ȡ�����ݵ���ʱ�ļ��ĺ�������*/
void ReadLinPermanentFileToTemp(CFile* LinF, CFile* LinTmpDatF, CFile* LinTmpNdxF, VERSION& LinVer, int& nLin, int& nLLin, long& TmpFLinDatOffset)
{
	LinF->Seek(0, CFile::begin);
	LinF->Read(&LinVer, sizeof(VERSION));//��ȡ�汾��Ϣ
	LinF->Read(&nLin, sizeof(int)); //���������
	LinF->Read(&nLLin, sizeof(int)); //���߼�����
	long LinNdxOffset = sizeof(VERSION) + sizeof(int) * 2;
	long LinDatOffset = LinNdxOffset + sizeof(LIN_NDX_STRU) * nLin;
	TmpFLinDatOffset = 0;
	LIN_NDX_STRU TempLinNdx;
	D_DOT Pnt;
	for (int i = 0; i < nLin; i++)
	{
		LinF -> Seek(LinNdxOffset, CFile::begin);
		LinF -> Read(&TempLinNdx, sizeof(LIN_NDX_STRU));//��ȡ�������ļ�����ʱ�������ļ���
		LinF -> Seek(TempLinNdx.datOff, CFile::begin);
		for (int j = 0; j < TempLinNdx.dotNum; j++)
		{
			LinF->Read(&Pnt, sizeof(D_DOT));
			LinTmpDatF -> Write(&Pnt, sizeof(D_DOT));//��ȡ�������ļ�����ʱ�����ļ���
		}
		TempLinNdx.datOff = TmpFLinDatOffset;
		LinTmpNdxF -> Write(&TempLinNdx, sizeof(LIN_NDX_STRU));//��ȡ�������ļ�����ʱ�������ļ���
		TmpFLinDatOffset += (sizeof(D_DOT) * TempLinNdx.dotNum);
		LinDatOffset += (sizeof(D_DOT) * TempLinNdx.dotNum);
		LinNdxOffset += sizeof(LIN_NDX_STRU);
	}
}


/*��������ʱ�ļ���������*/
void UpdateLin(CFile* LinTmpNdxF, int nLin, LIN_NDX_STRU line)
{
	WriteLinNdxToFile(LinTmpNdxF , nLin,line);//����д���nLine��������
}


/*�����ߵĵ����ݵ���ʱ�ļ��ĺ���*/
void UpdateLin(CFile* LinTmpNdxF, CFile* LinTmpDatF, int LinNdx, double offset_x, double offset_y)
{
	LIN_NDX_STRU tLin;
	D_DOT pt;
	ReadTempFileToLinNdx(LinTmpNdxF, LinNdx, tLin);//�ӵ������ļ���ȡ��Ӧ����������tLin
	for (int i = 0; i < tLin.dotNum; i++)
	{
		//��ȡ�ڵ����ݵ�pt��
		LinTmpDatF -> Seek(tLin.datOff+i*sizeof(D_DOT), CFile::begin);
		LinTmpDatF -> Read(&pt, sizeof(D_DOT));
		//����pt��ƫ�ƣ����ڵ��ƫ��
		pt.x = pt.x + offset_x;
		pt.y = pt.y + offset_y;
		//����д��ڵ�����
		LinTmpDatF->Seek(tLin.datOff+i*sizeof(D_DOT), CFile:: begin);
		LinTmpDatF->Write(&pt, sizeof(D_DOT));
	}
}


/*�޸ĵ�һ���������ĺ���*/
void AlterStartLin(CFile* LinTmpNdxF, long subdatoff, int nLine, int subNum)
{
	LIN_NDX_STRU LinNdx;
	//��ȡ��
	LinTmpNdxF->Seek(nLine*sizeof(LIN_NDX_STRU),CFile::begin);
	LinTmpNdxF->Read(&LinNdx, sizeof(LIN_NDX_STRU));
	//������
	LinNdx.datOff = subdatoff;//�����ߵĵ�����
	LinNdx.dotNum = subNum;//�����ߵĽڵ���
	//����д����
	LinTmpNdxF->Seek(nLine * sizeof(LIN_NDX_STRU), CFile::begin);
	LinTmpNdxF->Write(&LinNdx, sizeof(LIN_NDX_STRU));
}


/*�޸ĵڶ����������ĺ���*/
void AlterEndLin(CFile* LinTmpNdxF, int nLine)
{
	LIN_NDX_STRU  linNdx;
	//��ȡ��
	LinTmpNdxF->Seek(nLine * sizeof(LIN_NDX_STRU), CFile::begin);
	LinTmpNdxF->Read(&linNdx, sizeof(LIN_NDX_STRU));
	//������
	linNdx.dotNum = 0;
	linNdx.isDel = 1;
	//����д����
	LinTmpNdxF->Seek(nLine * sizeof(LIN_NDX_STRU), CFile::begin);
	LinTmpNdxF->Write(&linNdx, sizeof(LIN_NDX_STRU));
}


/*������������д������ʱ�����ļ��ĺ���*/
void WriteRegNdxToFile(CFile* RegTmpNdxF, int i, REG_NDX_STRU Region)
{
	RegTmpNdxF->Seek(i * sizeof(REG_NDX_STRU), CFile::begin);
	RegTmpNdxF->Write(&Region, sizeof(REG_NDX_STRU));
}


/*�����ڵ�����д������ʱ�����ļ��ĺ�������*/
void WriteRegDatToFile(CFile* RegTmpDatF, long datOff, int i, D_DOT point)
{
	RegTmpDatF->Seek(datOff + i*sizeof(D_DOT), CFile::begin);
	RegTmpDatF->Write(&point, sizeof(D_DOT));
}


/*������ʱ�ļ��ж�ȡ�������ĺ�������*/
void ReadTempFileToRegNdx(CFile* RegTmpNdxF, int i, REG_NDX_STRU& RegNdx)
{
	RegTmpNdxF->Seek(i * sizeof(REG_NDX_STRU), CFile::begin);
	RegTmpNdxF->Read(&RegNdx, sizeof(REG_NDX_STRU));
}


/*������ʱ�ļ��ж�ȡ���ڵ����ݵĺ�������*/
void ReadTempFileToRegDat(CFile* RegTmpDatF, long datOff, int i, D_DOT& Pnt)
{
	RegTmpDatF->Seek(datOff + i * sizeof(D_DOT), CFile::begin);
	RegTmpDatF->Read(&Pnt, sizeof(D_DOT));
}


/*�����������͵�����д�������ļ��ĺ�������*/
void WriteTempToRegPermanentFile(CFile* RegF, CFile* RegTmpDatF, CFile* RegTmpNdxF, VERSION RegVer, int nReg, int nLReg)
{
	REG_NDX_STRU TempRegNdx;//���ڴ������������ļ�
	D_DOT Pnt;//���ڴ������������ļ�
	long RegNdxOffset = sizeof(VERSION) + sizeof(int) * 2;//�����ļ���ͷһ��VERSION�ṹ��һ����������һ���߼���
	long RegDatOffset = RegNdxOffset + sizeof(REG_NDX_STRU) * nReg;//�����ļ���ͷ+�����ļ���С
	RegF->Write(&RegVer, sizeof(VERSION));//д��汾��Ϣ
	RegF->Write(&nReg, sizeof(int));//д��������
	RegF->Write(&nLReg, sizeof(int));//д���߼���
	for (int i = 0; i < nReg; i++) {
		//����ʱ�������ļ��ж�ȡ������
		ReadTempFileToRegNdx(RegTmpNdxF, i, TempRegNdx);
		RegF->Seek(RegDatOffset, CFile::begin);
		for (int j = 0; j < TempRegNdx.dotNum; j++) {
			//����ʱ�������ļ��ж�ȡ�ߵĵ�����
			ReadTempFileToRegDat(RegTmpDatF, TempRegNdx.datOff, j, Pnt);
			//�����ĵ�����д�������ļ�
			RegF->Write(&Pnt, sizeof(D_DOT));
		}
		RegF->Seek(RegNdxOffset, CFile::begin);//ָ�붨λ�������ļ���ͷ
		TempRegNdx.datOff = RegDatOffset;//���ý�����ݴ���λ��
		RegF->Write(&TempRegNdx, sizeof(REG_NDX_STRU));//���ߵ�����д�������ļ�
		RegNdxOffset += sizeof(REG_NDX_STRU);
		RegDatOffset += (sizeof(D_DOT) * TempRegNdx.dotNum);
	}
}

/*�������ļ���ȡ�����ݵ���ʱ�ļ��ĺ�������*/
void ReadRegPermanentFileToTemp(CFile* RegF, CFile* RegTmpDatF, CFile* RegTmpNdxF, VERSION& RegVer, int& nReg, int& nLReg, long& TmpFRegDatOffset)
{
	RegF->Seek(0, CFile::begin);
	RegF->Read(&RegVer, sizeof(VERSION));
	RegF->Read(&nReg, sizeof(int)); //���������
	RegF->Read(&nLReg, sizeof(int));//���߼�����
	long RegNdxOffset = sizeof(VERSION) + sizeof(int) * 2;//�����ļ���ͷһ��VERSION�ṹ��һ����������һ���߼���
	long RegDatOffset = RegNdxOffset + sizeof(REG_NDX_STRU) * nReg;//�����ļ���ͷ+�����ļ���С
	TmpFRegDatOffset = 0;//��ʱ�������ļ�ƫ����
	REG_NDX_STRU  TempRegNdx;
	D_DOT Pnt;
	for (int i = 0; i < nReg; i++) {
		RegF->Seek(RegNdxOffset, CFile::begin);//�����ļ���λ
		RegF->Read(&TempRegNdx, sizeof(REG_NDX_STRU));
		RegF->Seek(TempRegNdx.datOff, CFile::begin);//�����ļ���λ
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


/*�������ĺ�������*/
void UpdateReg(CFile* RegTmpNdxF, CFile* RegTmpDatF, int RegNdx, double offset_x, double offset_y)
{
	REG_NDX_STRU tReg;
	D_DOT pt;
	ReadTempFileToRegNdx(RegTmpNdxF, RegNdx, tReg);//������ʱ�ļ���ȡ��������tReg
	for (int i = 0; i < tReg.dotNum; i++)
	{
		//��ȡ�ڵ����ݵ�pt
		RegTmpDatF->Seek(tReg.datOff + i * sizeof(D_DOT), CFile:: begin);
		RegTmpDatF->Read(&pt, sizeof(D_DOT));
		//ʵ�ֽڵ��ƫ��
		pt.x = pt.x + offset_x;
		pt.y = pt.y + offset_y;
		//����д��ڵ�����
		RegTmpDatF->Seek(tReg.datOff + i * sizeof(D_DOT), CFile:: begin);
		RegTmpDatF->Write(&pt, sizeof(D_DOT));
	}
}