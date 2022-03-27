#pragma once
#include"process.h"



struct m_struct {
	process p;
	int choice;
};

//多进程
class multiprocess
{
private:
	Vpage* vp;//虚拟页
	int size;//外存大小
	int vp_num;//虚拟页数
	//三个进程
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

	//信号量
	//static HANDLE mutex;
public:
	multiprocess();//构造函数
	bool create(string requesta,string requestb,string requestc);//初始化

	~multiprocess();//析构函数
	process get_processA() { return A; }
	process get_processB() { return B; }
	process get_processC() { return C; }
	int** get_table(process p) { return p.get_table(); }
	int get_requestsize(process p) { return p.get_requestsize(); }
	Vpage* get_pagerequest(process p) { return p.get_pagerequest(); }
	void run(int choice);//运行
};

