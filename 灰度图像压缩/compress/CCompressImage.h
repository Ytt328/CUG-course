#pragma once
#include<vector>
using namespace std;

#define  Lmax    256 //ÿ�����ضοɺ��������ص�
#define  header  11 //һ���α���ռ11λ


struct BitHead {
	short bfType;
	int bfSize;
	short bfReserved1;
	short bfReserved2;
	int bfOffBits;
};

struct BitInf {
	int bisize;
	int biWidth;
	int biHeight;
	short biPlanes;
	short biBitCount;
	int biCompression;
	int biSizeImage;
	int biXPelsPerMeter;
	int biYPelsPerMeter;
	int biClrUsed;
	int biClrImportant;
};


struct tagRGB {
	char rgbBlue;
	char rgbGreen;
	char rgbRed;
	char rgbReserved;
};

class CCompressImage
{
private:
	CString path;//·����

	unsigned char** data;//λͼ����,������洢
	unsigned char* dataline;//λͼ����,һά���δ洢
	int* b;//dataline��ÿ��Ԫ�ص�λ��
	int* l;//ÿ�����ض��ڵ�������
	int* s;//��������{p1,p2...pi}�����ŷֶ�����Ĵ洢λ��

	BitHead head;//�ļ�ͷ
	BitInf Inf;//��Ϣͷ
	tagRGB* tag;//��ɫ��
	int tagnum;//��ɫ����
	int beforecompressedsize;//ѹ��ǰ��С
	int compressedsize;//ѹ�����С
	int beforeuncompressedsize;//��ѹǰ��С
	int uncompressedsize;//��ѹ���С

	CString cpath;//ѹ�����·��
	CString upath;//��ѹ���·��
public:
	CCompressImage();//����
	~CCompressImage();//����

	
	void reset();//����
	void setpath(CString p);//����·��
	bool ReadBitmap();//��
	bool WriteBitmap();//д
	bool Compress();//ѹ��
	bool UnCompress();//��ѹ

	int length(int i);//����һ����ת��Ϊ������ʱ��λ��
	
	int getsize() { return head.bfSize; }//��ȡ�ļ�ѹ��ǰ��С
	int getcompresssize() { return compressedsize; }//��ȡѹ����Ĵ�С
	int getbefore() { return beforeuncompressedsize; }//��ȡ��ѹǰ��С
	int getafter() { return uncompressedsize; } //��ȡ��ѹ���С

	CString getcpath() { return cpath; }//��ȡѹ����·��
	CString getupath() { return upath; }//��ȡ��ѹ��·��
};

