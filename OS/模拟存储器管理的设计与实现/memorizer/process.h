#pragma once

#include<string>
using namespace std;

#define PAGESIZE 512 //ҳ���СΪ512K
#define PAGENUM 50 //���50��ҳ��




//����ҳ
struct Ppage {
	int num;//ҳ��
	Ppage* nextpage;//��һҳָ��
	int vnum;//��Ӧ������ҳ��
	int R;//����λ
	int V;//�޸�λ
	//Vpage vpage;//��Ӧ������ҳ
	Ppage(){
		num = -1;
		vnum = -1;
		nextpage = nullptr;
		R = 0;
		V = 0;
	}
	Ppage(int n)
	{
		num = n;
		vnum = -1;
		nextpage = nullptr;
		R = 0;
		V = 0;
	}
};

//����ҳ
struct Vpage {
	int num;//ҳ��
	Vpage* nextpage;//��һҳ��ָ��
	//Ppage ppage;//��Ӧ������ҳ
	bool is_missing;//�Ƿ�ȱҳ
	Vpage()
	{
		num = -1;
		nextpage = nullptr;
		is_missing = true;
	}

};

/*������*/
class process
{
private:
	string name;//������
	double size;//���̴�С����KΪ��λ
	Ppage* physical_page;//ҳ��
	int p_num;//ҳ����
	Vpage* page_request;//ҳ�����˳��
	int request_size;//Ҫ���ȵ�ҳ����
	int** table;//���ҳ������ʱ��ҳ�������
	int* miss;//ȱҳ��
	int* replace;//�û���
	double* missrate;//ȱҳ��
	double* replacerate;//�û���
public:
	process();//���캯��
	void setname(string n);//���ý�����
	void setsize(double s);//���ý��̴�С
	bool setrequest(string request);//����ҳ�����˳��
	~process();//��������

	int** get_table() { return table; }
	int get_requestsize() { return request_size; }
	Vpage* get_pagerequest() { return page_request; }
	int* get_miss() { return miss; }
	int* get_replace() { return replace; }

	void set_missrate() {
		missrate[0] = (miss[0] * 1.0) / request_size;
	};
	void set_replacerate() {
		replacerate[0] = (replace[0] * 1.0) / request_size;
	};

	//�����û��㷨
	void FIFO(HANDLE &mutex);//�Ƚ��ȳ�ҳ���û��㷨
	void LRU(HANDLE &mutex);//������δʹ��ҳ���û��㷨
	void NUR(HANDLE &mutex);//���δʹ��ҳ���û��㷨
	void OPT(HANDLE &mutex);//����ҳ���û��㷨
};

