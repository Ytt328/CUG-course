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

//��λͼ
bool CCompressImage::ReadBitmap() {
	if (path.IsEmpty()) {
		//·��Ϊ��
		return false;
	}
	ifstream fin(path, ios::binary);
	if (fin.is_open()) {
		reset();
		//���ļ�ͷ
		fin.read((char*)&head.bfType, sizeof(head.bfType));
		fin.read((char*)&head.bfSize, sizeof(head.bfSize));
		fin.read((char*)&head.bfReserved1, sizeof(head.bfReserved1));
		fin.read((char*)&head.bfReserved2, sizeof(head.bfReserved2));
		fin.read((char*)&head.bfOffBits, sizeof(head.bfOffBits));
		//����Ϣͷ
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
			//����ɫ��
			for (int i = 0; i < tagnum; i++) {
				fin.read(&tag[i].rgbBlue, sizeof(tag[i].rgbBlue));
				fin.read(&tag[i].rgbGreen, sizeof(tag[i].rgbGreen));
				fin.read(&tag[i].rgbRed, sizeof(tag[i].rgbRed));
				fin.read(&tag[i].rgbReserved, sizeof(tag[i].rgbReserved));
			}
		}
		//��λͼ����
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
		//�ر��ļ�
		fin.close();
		
		//���ζ�ȡ
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
		//�ļ���ʧ��
		return false;
	}
}

//дλͼ
bool CCompressImage::WriteBitmap() {
	
	CString FileName;
	upath = path.Left(path.ReverseFind('\\'));
	FileName = path.Right(path.GetLength() - path.ReverseFind('\\') - 1);
	FileName = FileName.Left(FileName.Find('.'));
	FileName = FileName + CString("u.bmp");
	upath = upath + '\\' + FileName;
	CFile* uncompressedfile = new CFile();
	if (uncompressedfile->Open(upath, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary)) {
		//д�ļ�ͷ
		//uncompressedfile->Write(&head, sizeof(BitHead));
		uncompressedfile->Write((char*)&head.bfType, sizeof(head.bfType));
		uncompressedfile->Write((char*)&head.bfSize, sizeof(head.bfSize));
		uncompressedfile->Write((char*)&head.bfReserved1, sizeof(head.bfReserved1));
		uncompressedfile->Write((char*)&head.bfReserved2, sizeof(head.bfReserved2));
		uncompressedfile->Write((char*)&head.bfOffBits, sizeof(head.bfOffBits));
		//д��Ϣͷ
		uncompressedfile->Write(&Inf, sizeof(BitInf));
		//д��ɫ��
		if (tagnum > 0) {
			for (int i = 0; i < tagnum; i++) {
				uncompressedfile->Write(&tag[i], sizeof(tagRGB));
			}
		}
		//д������Ϣ
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

//ѹ��
bool CCompressImage::Compress() {
	//ѹ������
	int t = Inf.biHeight * Inf.biWidth;
	s = new int[t+1];//ǰn�����ص����ŷֶδ洢����Ҫ����λ��
	b = new int[t+1];//ÿ�������ö����Ʊ�ʾʱ������Ҫ����λ
	l = new int[t+1];//���ص����ڶ�ÿ��������Ҫ�����λ��
	s[0] = 0;
	b[0] = 0;
	l[0] = 0;
	for (int i = 1; i <= t; i++) {
		b[i] = length(dataline[i - 1]);//�������ر�����Ҫ�����λ��
		int bmax = b[i];//��¼�µ�ǰ���ص�λ��
		s[i] = s[i - 1] + bmax;//����b��λ��
		l[i] = 1;//�γ�Ϊ1(��ʼ��)
		for (int j = 2; j <= i && j <= Lmax; j++) {//��ǰ�����Ƿ���ڸ��ŵķֶΣ����Ȳ�����Lmax(256)
			if (bmax < b[i - j + 1]) {//��ǰ���ص�λ��С��ǰ��ֶε����ص�λ��������bmax
				bmax = b[i - j + 1];
			}
			if (s[i] > s[i - j] + j * bmax) {
				//��ǰi�����ص�ֶεĴ�С����ǰi-j�����ص�ֶδ�С����j*bmax
				//˵�����ڸ��ŵķֶ�
				s[i] = s[i - j] + j * bmax;//���´�С
				l[i] = j;//���¶γ�����
			}
		}
		s[i] += header;//���ϡ�ͷ���Ĵ�С(11λ)
	}
	//�������Ž⣬��Ҫ�ɺ���ǰ�㣬����ʹ��ջ
	stack<int> L;//���ضκ��е���������0-255��
	int index = Inf.biWidth*Inf.biHeight;
	while (index > 0) {
		L.push(l[index]-1);//�Ӻ���ǰ����γ���Ϣ
		index = index - l[index];
	}

	//�ļ���
	cpath= path.Left(path.ReverseFind('\\'));
	CString FileName;
	FileName = path.Right(path.GetLength() - path.ReverseFind('\\') - 1);
	FileName = FileName.Left(FileName.Find('.'));
	FileName = FileName + CString(".cbmp");
	cpath = cpath + '\\' + FileName;
	CFile* compressedfile = new CFile();
	//д���ļ�
	if (compressedfile->Open(cpath, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary)) {
		//д�ļ�ͷ
		compressedfile->Write(&head, sizeof(BitHead));
		//д��Ϣͷ
		compressedfile->Write(&Inf, sizeof(BitInf));
		//д��ɫ��
		for (int i = 0; i < tagnum; i++) {
			compressedfile->Write(&tag[i], sizeof(tagRGB));
		}
		//д��ѹ����������λ��
		compressedfile->Write(&s[Inf.biWidth*Inf.biHeight], sizeof(int));
		compressedsize = compressedfile->GetLength();
		//д���ص�����
		int index = 0;//dataline����
		unsigned int buffer = 0;//Ҫ��������ݻ�����
		int buffersize = 0;//��ǰҪ�������ݵ�λ��
		int size = L.size();
		for (int i = 0; i < size; i++) {
			unsigned char num1 = L.top();//�γ�,ռ8λ
			L.pop();
			buffer = (buffer << 8)|num1;//����buffer
			int num2 = 0;//����ÿ��Ԫ�صĸ���,ռ3λ����Ҫ����������һ�����
			//��ȡ����Ԫ�ص����λ��
			for (int t = index; t < index + num1+1; t++) {
				num2 = (b[t+1] > num2) ? b[t+1] : num2;
			}
			num2 = num2 - 1;//-1��Χ��0-7��������3λ����ʾ
			buffer = (buffer << 3) | num2;//buffer������λ������λ����num2
			buffersize += 11;
			while (buffersize >= 8) {//Ҫ��������ݴ���8λ�����Կ�ʼ����
				int tmpbuffer = buffer;
				unsigned char temp = (tmpbuffer >> (buffersize - 8));
				compressedfile->Write(&temp, sizeof(unsigned char));//���뻺������ǰ8λ
				buffer = buffer & (int)(pow(2,buffersize-8)-1);//ǰ8λ��Ϊ0
				buffersize -= 8;//��������С����
			}
			for (int j = 0; j <= num1; j++) {
				unsigned char tmp=dataline[index + j];
				buffer = (buffer << (num2+1)) | tmp;
				buffersize += (num2+1);
				while (buffersize >= 8) {//Ҫ��������ݴ���8λ�����Կ�ʼ����
					int tmpbuffer = buffer;
					unsigned char temp = (tmpbuffer >> (buffersize - 8));
					compressedfile->Write(&temp, sizeof(unsigned char));//���뻺������ǰ8λ
					buffer = buffer & (int)(pow(2, buffersize - 8) - 1);//ǰ8λ��Ϊ0
					buffersize -= 8;//��������С����
				}
			}
			index += num1+1;
		}
		if (buffersize >= 0) {//���ʣ��С��8λ��������0�����
			unsigned char tmp1 = buffer << (8 - buffersize);
			compressedfile->Write(&tmp1, sizeof(unsigned char));
			buffersize = 0;
		}
	}
	compressedsize = compressedfile->GetLength();//��ȡ�ļ���С
	compressedfile->Close();//�ر��ļ�
	//�ͷŶ�̬�ڴ�
	delete[] s;
	delete[] b;
	delete[] l;
	return true;
}

//��ѹ
bool CCompressImage::UnCompress() {
	if (path.IsEmpty()) {
		//·��Ϊ��
		return false;
	}
	CFile* Compressed=new CFile();
	if (Compressed->Open(path, CFile::modeRead | CFile::typeBinary)) {
		reset();
		beforeuncompressedsize = Compressed->GetLength();
		//���ļ�ͷ
		Compressed->Read(&head, sizeof(BitHead));
		//����Ϣͷ
		Compressed->Read(&Inf, sizeof(BitInf));
		tagnum = (head.bfOffBits - 14 - 40) / sizeof(tagRGB);
		if (tagnum > 0) {
			tag = new tagRGB[tagnum];
			//����ɫ��
			for (int i = 0; i < tagnum; i++) {
				Compressed->Read(&tag[i], sizeof(tagRGB));
			}
		}
		//�����ص�����
		int t = Inf.biHeight * Inf.biWidth;
		dataline = new unsigned char[t+1];
		int index = 0;
		int total_bitnum = 0;//ѹ�������еõ������ŷֶ���Ҫ��λ��
		Compressed->Read(&total_bitnum, sizeof(int));
		int buffer = 0;//Ҫ���������ݻ�����
		int buffersize = 0;//��ǰҪ�������ݵ�λ��
		int readsize = 0;//��ǰ�Ѷ�ȡ����λ��
		while (readsize < total_bitnum) {
			unsigned char tmp1,tmp2;
			Compressed->Read(&tmp1, sizeof(unsigned char));
			Compressed->Read(&tmp2, sizeof(unsigned char));
			readsize += 16;//�Ѷ����ݳ��ȸ���
			buffer = (buffer << 8) | tmp1;
			buffer = (buffer << 8) | tmp2;
			buffersize += 16;//���������ȸ���
			int tmpb = buffer;
			int Length=(tmpb>>(buffersize-8));//�γ�
			buffer = buffer & (int)(pow(2, buffersize - 8)-1);
			buffersize -= 8;
			tmpb = buffer;
			int Size=(tmpb>>(buffersize-3));//����ÿ�����ص��λ��
			buffer = buffer & (int)(pow(2, buffersize - 3) - 1);
			buffersize -= 3;
			int count = 0;//��ǰ���Ѷ�������ص����
			while (buffersize >= Size + 1 && count < Length + 1) {//���������ȴ������ص��С(ѹ����)�����Դ���
				int tmp1 = buffer;
				unsigned char tmp2 = tmp1 >> (buffersize - Size - 1);//��ȡ���ص�����
				dataline[index++] = tmp2;//����dataline
				count++;//�������ص������һ
				buffer = buffer & (int)(pow(2, buffersize - Size - 1) - 1);//ǰnλ��0
				buffersize = buffersize - Size - 1;//��������С����
			}
			while(count<Length+1) {
				unsigned char tmp;
				Compressed->Read(&tmp, sizeof(unsigned char));//��8λ
				readsize += 8;//�Ѷ����ȸ���
				buffer = (buffer <<8)|tmp;//����������
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
		Compressed->Close();//�ر��ļ�
		//������д�����(����ת�������)
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


//�ֶ�
//void CCompressImage::Traceback(int n, int& i, vector<int> s1, vector<int> l1) {
//	if (n == 0) {
//		return;
//	}
//	Traceback(n - l1[n], i, s1, l1);
//	s1[i++] = n - l1[n];
//}

//����һ����ת��Ϊ������ʱ��λ��
int CCompressImage::length(int i) {
	int k = 1;
	i = i / 2;
	while (i > 0) {
		k++;
		i = i / 2;
	}
	return k;
}

