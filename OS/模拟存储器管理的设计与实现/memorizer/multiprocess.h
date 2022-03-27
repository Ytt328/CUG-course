#pragma once
#include"process.h"



struct m_struct {
	process p;
	int choice;
};

//�����
class multiprocess
{
private:
	Vpage* vp;//����ҳ
	int size;//����С
	int vp_num;//����ҳ��
	//��������
	process A;
	process B;
	process C;
	//
	HANDLE threada;
	HANDLE threadb;
	HANDLE threadc;

	m_struct a;
	m_struct b;
	m_struct c;

	//�ź���
	//static HANDLE mutex;
public:
	multiprocess();//���캯��
	bool create(string requesta,string requestb,string requestc);//��ʼ��

	~multiprocess();//��������
	process get_processA() { return A; }
	process get_processB() { return B; }
	process get_processC() { return C; }
	int** get_table(process p) { return p.get_table(); }
	int get_requestsize(process p) { return p.get_requestsize(); }
	Vpage* get_pagerequest(process p) { return p.get_pagerequest(); }
	void run(int choice);//����
};

