#include "pch.h"
#include "CCompressImage.h"
#include<fstream>
#include<sstream>
#include<stack>


CCompressImage::CCompressImage() {
	//bfOffBits = 0;
	//biWidth = 0;
	//biHeight = 0;
	/*s.push_back(0);
	b.push_back(0);
	l.push_back(0);*/
}

CCompressImage::~CCompressImage() {

}


void CCompressImage::reset() {
	if (Inf.biHeight != 0) {
		for (int i = 0; i < Inf.biHeight; i++) {
			delete[] data[i];
		}
		delete[]data;
		delete[] dataline;
	   /* delete[] s;
	    delete[] b;
	    delete[] l;*/
	}
	if (tagnum != 0) {
		delete[] tag;
		tagnum = 0;
	}
	//delete[] data;
	
}

void CCompressImage::setpath(CString p)
{
	path = p;
}

//读位图
bool CCompressImage::ReadBitmap() {
	if (path.IsEmpty()) {
		//路径为空
		return false;
	}
	ifstream fin(path, ios::binary);
	if (fin.is_open()) {
		reset();
		//读文件头
		fin.read((char*)&head.bfType, sizeof(head.bfType));
		fin.read((char*)&head.bfSize, sizeof(head.bfSize));
		fin.read((char*)&head.bfReserved1, sizeof(head.bfReserved1));
		fin.read((char*)&head.bfReserved2, sizeof(head.bfReserved2));
		fin.read((char*)&head.bfOffBits, sizeof(head.bfOffBits));
		//读信息头
		fin.read((char*)&Inf.bisize, sizeof(Inf.bisize));
		fin.read((char*)&Inf.biWidth, sizeof(Inf.biWidth));
		fin.read((char*)&Inf.biHeight, sizeof(Inf.biHeight));
		fin.read((char*)&Inf.biPlanes, sizeof(Inf.biPlanes));
		fin.read((char*)&Inf.biBitCount, sizeof(Inf.biBitCount));
		fin.read((char*)&Inf.biCompression, sizeof(Inf.biCompression));
		fin.read((char*)&Inf.biSizeImage, sizeof(Inf.biSizeImage));
		fin.read((char*)&Inf.biXPelsPerMeter, sizeof(Inf.biXPelsPerMeter));
		fin.read((char*)&Inf.biYPelsPerMeter, sizeof(Inf.biYPelsPerMeter));
		fin.read((char*)&Inf.biClrUsed, sizeof(Inf.biClrUsed));
		fin.read((char*)&Inf.biClrImportant, sizeof(Inf.biClrImportant));
		tagnum = (head.bfOffBits - 14 - 40) / sizeof(tagRGB);
		if (tagnum > 0) {
			tag = new tagRGB[tagnum];
			//读调色板
			for (int i = 0; i < tagnum; i++) {
				fin.read(&tag[i].rgbBlue, sizeof(tag[i].rgbBlue));
				fin.read(&tag[i].rgbGreen, sizeof(tag[i].rgbGreen));
				fin.read(&tag[i].rgbRed, sizeof(tag[i].rgbRed));
				fin.read(&tag[i].rgbReserved, sizeof(tag[i].rgbReserved));
			}
		}
		//读位图数据
		int h = Inf.biHeight;
		int w = Inf.biWidth;
		data = new unsigned char* [h];
		for (int i = 0; i < h; i++) {
			data[i] = new unsigned char[w];
		}
		unsigned char tmpd = 0;
		for (int i = 0; i < Inf.biHeight; i++) {
			//vector<unsigned char> tmpv;
			for (int j = 0; j < Inf.biWidth; j++) {
				fin.read((char*)&tmpd, sizeof(unsigned char));
				data[i][j] = tmpd;
				//tmpv.push_back(tmpd);
			}
			//data.push_back(tmpv);
		}
		//关闭文件
		fin.close();
		
		//蛇形读取
		int t = h * w;
		dataline = new unsigned char[t];
		int index = 0;
		for (int j = 0; j < Inf.biHeight; j++) {
			if (!(j % 2)) {
				for (int k = 0; k < Inf.biWidth; k++) {
					dataline[index++] = data[j][k];
					//dataline.push_back(data[j][k]);
				}
			}
			else {
				for (int k = Inf.biWidth - 1; k >= 0; k--) {
					dataline[index++] = data[j][k];
					//dataline.push_back(data[j][k]);
				}
			}
		}
		return true;
	}
	
	else {
		//文件打开失败
		return false;
	}
}

//写位图
bool CCompressImage::WriteBitmap() {
	
	CString FileName;
	upath = path.Left(path.ReverseFind('\\'));
	FileName = path.Right(path.GetLength() - path.ReverseFind('\\') - 1);
	FileName = FileName.Left(FileName.Find('.'));
	FileName = FileName + CString("u.bmp");
	upath = upath + '\\' + FileName;
	CFile* uncompressedfile = new CFile();
	if (uncompressedfile->Open(upath, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary)) {
		//写文件头
		//uncompressedfile->Write(&head, sizeof(BitHead));
		uncompressedfile->Write((char*)&head.bfType, sizeof(head.bfType));
		uncompressedfile->Write((char*)&head.bfSize, sizeof(head.bfSize));
		uncompressedfile->Write((char*)&head.bfReserved1, sizeof(head.bfReserved1));
		uncompressedfile->Write((char*)&head.bfReserved2, sizeof(head.bfReserved2));
		uncompressedfile->Write((char*)&head.bfOffBits, sizeof(head.bfOffBits));
		//写信息头
		uncompressedfile->Write(&Inf, sizeof(BitInf));
		//写调色板
		if (tagnum > 0) {
			for (int i = 0; i < tagnum; i++) {
				uncompressedfile->Write(&tag[i], sizeof(tagRGB));
			}
		}
		//写数据信息
		for (int i = 0; i < Inf.biHeight; i++) {
			for (int j = 0; j < Inf.biWidth; j++) {
				uncompressedfile->Write(&data[i][j], sizeof(unsigned char));
			}
		}
		uncompressedsize = uncompressedfile->GetLength();
		uncompressedfile->Close();
		return true;
	}

	return false;
}

//压缩
bool CCompressImage::Compress() {
	//压缩过程
	int t = Inf.biHeight * Inf.biWidth;
	s = new int[t+1];//前n各像素点最优分段存储所需要的总位数
	b = new int[t+1];//每个像素用二进制表示时最少需要多少位
	l = new int[t+1];//像素点所在段每个像素需要的最大位数
	s[0] = 0;
	b[0] = 0;
	l[0] = 0;
	for (int i = 1; i <= t; i++) {
		b[i] = length(dataline[i - 1]);//计算像素本身需要的最大位数
		int bmax = b[i];//记录下当前像素点位数
		s[i] = s[i - 1] + bmax;//加上b的位数
		l[i] = 1;//段长为1(初始化)
		for (int j = 2; j <= i && j <= Lmax; j++) {//向前查找是否存在更优的分段，长度不超过Lmax(256)
			if (bmax < b[i - j + 1]) {//当前像素点位数小于前面分段的像素点位数，更新bmax
				bmax = b[i - j + 1];
			}
			if (s[i] > s[i - j] + j * bmax) {
				//当前i个像素点分段的大小大于前i-j个像素点分段大小加上j*bmax
				//说明存在更优的分段
				s[i] = s[i - j] + j * bmax;//更新大小
				l[i] = j;//更新段长数据
			}
		}
		s[i] += header;//加上“头”的大小(11位)
	}
	//构造最优解，需要由后往前算，所以使用栈
	stack<int> L;//像素段含有的像素数（0-255）
	int index = Inf.biWidth*Inf.biHeight;
	while (index > 0) {
		L.push(l[index]-1);//从后向前存入段长信息
		index = index - l[index];
	}

	//文件名
	cpath= path.Left(path.ReverseFind('\\'));
	CString FileName;
	FileName = path.Right(path.GetLength() - path.ReverseFind('\\') - 1);
	FileName = FileName.Left(FileName.Find('.'));
	FileName = FileName + CString(".cbmp");
	cpath = cpath + '\\' + FileName;
	CFile* compressedfile = new CFile();
	//写入文件
	if (compressedfile->Open(cpath, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary)) {
		//写文件头
		compressedfile->Write(&head, sizeof(BitHead));
		//写信息头
		compressedfile->Write(&Inf, sizeof(BitInf));
		//写调色板
		for (int i = 0; i < tagnum; i++) {
			compressedfile->Write(&tag[i], sizeof(tagRGB));
		}
		//写入压缩后数据总位数
		compressedfile->Write(&s[Inf.biWidth*Inf.biHeight], sizeof(int));
		compressedsize = compressedfile->GetLength();
		//写像素点数据
		int index = 0;//dataline索引
		unsigned int buffer = 0;//要存入的数据缓冲区
		int buffersize = 0;//当前要存入数据的位数
		int size = L.size();
		for (int i = 0; i < size; i++) {
			unsigned char num1 = L.top();//段长,占8位
			L.pop();
			buffer = (buffer << 8)|num1;//加入buffer
			int num2 = 0;//段内每个元素的个数,占3位，需要与其他数据一起存入
			//获取段内元素的最大位数
			for (int t = index; t < index + num1+1; t++) {
				num2 = (b[t+1] > num2) ? b[t+1] : num2;
			}
			num2 = num2 - 1;//-1后范围在0-7，可以用3位来表示
			buffer = (buffer << 3) | num2;//buffer左移三位，后三位加入num2
			buffersize += 11;
			while (buffersize >= 8) {//要存入的数据大于8位，可以开始存入
				int tmpbuffer = buffer;
				unsigned char temp = (tmpbuffer >> (buffersize - 8));
				compressedfile->Write(&temp, sizeof(unsigned char));//存入缓冲区的前8位
				buffer = buffer & (int)(pow(2,buffersize-8)-1);//前8位置为0
				buffersize -= 8;//缓冲区大小更新
			}
			for (int j = 0; j <= num1; j++) {
				unsigned char tmp=dataline[index + j];
				buffer = (buffer << (num2+1)) | tmp;
				buffersize += (num2+1);
				while (buffersize >= 8) {//要存入的数据大于8位，可以开始存入
					int tmpbuffer = buffer;
					unsigned char temp = (tmpbuffer >> (buffersize - 8));
					compressedfile->Write(&temp, sizeof(unsigned char));//存入缓冲区的前8位
					buffer = buffer & (int)(pow(2, buffersize - 8) - 1);//前8位置为0
					buffersize -= 8;//缓冲区大小更新
				}
			}
			index += num1+1;
		}
		if (buffersize >= 0) {//最后剩下小于8位的数，补0后存入
			unsigned char tmp1 = buffer << (8 - buffersize);
			compressedfile->Write(&tmp1, sizeof(unsigned char));
			buffersize = 0;
		}
	}
	compressedsize = compressedfile->GetLength();//获取文件大小
	compressedfile->Close();//关闭文件
	//释放动态内存
	delete[] s;
	delete[] b;
	delete[] l;
	return true;
}

//解压
bool CCompressImage::UnCompress() {
	if (path.IsEmpty()) {
		//路径为空
		return false;
	}
	CFile* Compressed=new CFile();
	if (Compressed->Open(path, CFile::modeRead | CFile::typeBinary)) {
		reset();
		beforeuncompressedsize = Compressed->GetLength();
		//读文件头
		Compressed->Read(&head, sizeof(BitHead));
		//读信息头
		Compressed->Read(&Inf, sizeof(BitInf));
		tagnum = (head.bfOffBits - 14 - 40) / sizeof(tagRGB);
		if (tagnum > 0) {
			tag = new tagRGB[tagnum];
			//读调色板
			for (int i = 0; i < tagnum; i++) {
				Compressed->Read(&tag[i], sizeof(tagRGB));
			}
		}
		//读像素点数据
		int t = Inf.biHeight * Inf.biWidth;
		dataline = new unsigned char[t+1];
		int index = 0;
		int total_bitnum = 0;//压缩过程中得到的最优分段需要的位数
		Compressed->Read(&total_bitnum, sizeof(int));
		int buffer = 0;//要读出的数据缓冲区
		int buffersize = 0;//当前要读出数据的位数
		int readsize = 0;//当前已读取的总位数
		while (readsize < total_bitnum) {
			unsigned char tmp1,tmp2;
			Compressed->Read(&tmp1, sizeof(unsigned char));
			Compressed->Read(&tmp2, sizeof(unsigned char));
			readsize += 16;//已读数据长度更新
			buffer = (buffer << 8) | tmp1;
			buffer = (buffer << 8) | tmp2;
			buffersize += 16;//缓冲区长度更新
			int tmpb = buffer;
			int Length=(tmpb>>(buffersize-8));//段长
			buffer = buffer & (int)(pow(2, buffersize - 8)-1);
			buffersize -= 8;
			tmpb = buffer;
			int Size=(tmpb>>(buffersize-3));//段中每个像素点的位数
			buffer = buffer & (int)(pow(2, buffersize - 3) - 1);
			buffersize -= 3;
			int count = 0;//当前段已读入的像素点个数
			while (buffersize >= Size + 1 && count < Length + 1) {//缓冲区长度大于像素点大小(压缩后)，可以存入
				int tmp1 = buffer;
				unsigned char tmp2 = tmp1 >> (buffersize - Size - 1);//获取像素点数据
				dataline[index++] = tmp2;//存入dataline
				count++;//读入像素点个数加一
				buffer = buffer & (int)(pow(2, buffersize - Size - 1) - 1);//前n位置0
				buffersize = buffersize - Size - 1;//缓冲区大小更新
			}
			while(count<Length+1) {
				unsigned char tmp;
				Compressed->Read(&tmp, sizeof(unsigned char));//读8位
				readsize += 8;//已读长度更新
				buffer = (buffer <<8)|tmp;//缓冲区更新
				buffersize += 8;
				while (buffersize >= Size+1&& count < Length + 1) {
					int tmp1 = buffer;
					unsigned char tmp2 = tmp1 >> (buffersize - Size - 1);
					dataline[index++] = tmp2;
					count++;
					buffer = buffer & (int)(pow(2, buffersize - Size - 1) - 1);
					buffersize = buffersize - Size - 1;
				}
			}
		}
		Compressed->Close();//关闭文件
		//将数据写入矩阵(蛇形转储逆操作)
		int h = Inf.biHeight;
		int w = Inf.biWidth;
		data = new unsigned char* [h];
		for (int i = 0; i < h; i++) {
			data[i] = new unsigned char[w];
		}
		int index1 = 0;
		for (int i = 0; i < h; i++) {
			if (!(i % 2)) {
				for (int j = 0; j < w; j++) {
					data[i][j] = dataline[index1++];
				}
			}
			else {
				for (int j = w - 1; j >= 0; j--) {
					data[i][j] = dataline[index1++];
				}
			}
		}
	}
	return true;
}


//分段
//void CCompressImage::Traceback(int n, int& i, vector<int> s1, vector<int> l1) {
//	if (n == 0) {
//		return;
//	}
//	Traceback(n - l1[n], i, s1, l1);
//	s1[i++] = n - l1[n];
//}

//计算一个数转化为二进制时的位数
int CCompressImage::length(int i) {
	int k = 1;
	i = i / 2;
	while (i > 0) {
		k++;
		i = i / 2;
	}
	return k;
}

