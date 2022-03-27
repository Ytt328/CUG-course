#pragma once
//������/д�ļ�


#ifndef WRITEORREAD_H
#define WRITEORREAD_H
#include"MyDataType.h"

//��������д����ʱ�ļ��ĺ�������
void WritePntToFile(CFile* PntTmpF,int i, PNT_STRU point);

//����ʱ���ļ���ȡ�����ݵĺ�������
void ReadTempFileToPnt(CFile* PntTmpF, int i, PNT_STRU& point);

//�������ļ�������ʱ�ļ��ͺ���˵�����㣩
void ReadPntPermanentFileToTemp(CFile* pntF, CFile* pntTmpF, int& nPnt, int& nLPnt);

//���µ���ʱ�ļ��е����ݵĺ���˵��
void UpdatePnt(CFile* pntTmpF, int i, PNT_STRU pnt);

//������������д������ʱ�����ļ��ĺ���˵��
void WriteLinNdxToFile(CFile* LinTmpNdxF, int i, LIN_NDX_STRU line);

//���߽ڵ�����д������ʱ�����ļ��ĺ���˵��
void WriteLinDatToFile(CFile* LinTmpDatF, long datOff, int i, D_DOT point);

//����ʱ�������ļ��ж�ȡ�ߵĵ����ݵĺ�������
void ReadTempFileToLinDat(CFile* LinTmpDatF, long datOff, int i, D_DOT& pnt);

//����ʱ�������ļ��ж�ȡ�������ĺ�������
void ReadTempFileToLinNdx(CFile* LinTmpNdxF, int i, LIN_NDX_STRU& LinNdx);

//���ߵ������͵�����д�������ļ��ĺ�������
void WriteTempToLinPermanentFile(CFile* LinF, CFile* LinTmpDatF, CFile* LinTmpNdxF, VERSION LinVer, int nLin, int nLLin);

//�������ļ���ȡ�����ݵ���ʱ�ļ��ĺ�������
void ReadLinPermanentFileToTemp(CFile* LinF, CFile* LinTmpDatF,CFile* LinTmpNdxF, VERSION& LinVer, int& nLin, int & nLLin, long& TmpFLinDatOffset);

//��������ʱ�����ļ��������ݵĺ�������
void UpdateLin(CFile * LinTmpNdxF, int nLin , LIN_NDX_STRU line);

//�����ߵĵ����ݵ���ʱ�ļ��ĺ�������
void UpdateLin(CFile* LinTmpNdxF, CFile* LinTmpDatF, int LinNdx, double offset_x, double offset_y);

//�޸ĵ�һ���������ĺ�������
void AlterStartLin(CFile* LinTmpNdxF,long subdatoff,int nLine,int subNum);

//�޸ĵڶ����������ĺ�������
void AlterEndLin(CFile* LinTmpNdxF, int nLine);

//������������д������ʱ�����ļ��ĺ�������
void WriteRegNdxToFile(CFile* RegTmpNdxF, int i, REG_NDX_STRU Region);

//�����ڵ�����д������ʱ�����ļ��ĺ�������
void WriteRegDatToFile(CFile* RegTmpDatF, long datOff,int i, D_DOT point);

//������ʱ�ļ��ж�ȡ�������ĺ�������
void ReadTempFileToRegNdx(CFile* RegTmpNdxF, int i, REG_NDX_STRU& RegNdx);

//������ʱ�ļ��ж�ȡ���ڵ����ݵĺ�������
void ReadTempFileToRegDat(CFile* RegTmpDatF, long datOff, int i, D_DOT& Pnt);

//�����������͵�����д�������ļ��ĺ�������
void WriteTempToRegPermanentFile(CFile* RegF, CFile* RegTmpDatF, CFile* RegTmpNdxF, VERSION RegVer, int nReg, int nLReg);

//�������ļ���ȡ�����ݵ���ʱ�ļ��ĺ�������
void ReadRegPermanentFileToTemp(CFile* RegF, CFile* RegTmpDatF, CFile* RegTmpNdxF, VERSION& RegVer, int& nReg, int& nLReg, long& TmpFRegDatOffset);

//�������ĺ�������
void UpdateReg(CFile* RegTmpNdxF, CFile* RegTmpDatF, int RegNdx, double offset_x, double offset_y);
#endif
