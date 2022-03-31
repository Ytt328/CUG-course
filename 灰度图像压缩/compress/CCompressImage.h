#pragma once
#include<vector>
using namespace std;

#define  Lmax    256 //每个像素段可含最多的像素点
#define  header  11 //一个段标题占11位


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
	CString path;//路径名

	unsigned char** data;//位图数据,按矩阵存储
	unsigned char* dataline;//位图数据,一维蛇形存储
	int* b;//dataline中每个元素的位数
	int* l;//每个像素段内的像素数
	int* s;//像素序列{p1,p2...pi}的最优分段所需的存储位数

	BitHead head;//文件头
	BitInf Inf;//信息头
	tagRGB* tag;//调色板
	int tagnum;//调色板数
	int beforecompressedsize;//压缩前大小
	int compressedsize;//压缩后大小
	int beforeuncompressedsize;//解压前大小
	int uncompressedsize;//解压后大小

	CString cpath;//压缩后的路径
	CString upath;//解压后的路径
public:
	CCompressImage();//构造
	~CCompressImage();//析构

	
	void reset();//重置
	void setpath(CString p);//设置路径
	bool ReadBitmap();//读
	bool WriteBitmap();//写
	bool Compress();//压缩
	bool UnCompress();//解压

	int length(int i);//计算一个数转化为二进制时的位数
	
	int getsize() { return head.bfSize; }//获取文件压缩前大小
	int getcompresssize() { return compressedsize; }//获取压缩后的大小
	int getbefore() { return beforeuncompressedsize; }//获取解压前大小
	int getafter() { return uncompressedsize; } //获取解压后大小

	CString getcpath() { return cpath; }//获取压缩后路径
	CString getupath() { return upath; }//获取解压后路径
};

