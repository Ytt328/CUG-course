#pragma once

#include<string>
using namespace std;

#define PAGESIZE 512 //页面大小为512K
#define PAGENUM 50 //最多50个页面




//物理页
struct Ppage {
	int num;//页号
	Ppage* nextpage;//下一页指针
	int vnum;//对应的虚拟页号
	int R;//访问位
	int V;//修改位
	//Vpage vpage;//对应的虚拟页
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

//虚拟页
struct Vpage {
	int num;//页号
	Vpage* nextpage;//下一页面指针
	//Ppage ppage;//对应的物理页
	bool is_missing;//是否缺页
	Vpage()
	{
		num = -1;
		nextpage = nullptr;
		is_missing = true;
	}

};

/*进程类*/
class process
{
private:
	string name;//进程名
	double size;//进程大小，以K为单位
	Ppage* physical_page;//页框
	int p_num;//页框数
	Vpage* page_request;//页面调度顺序
	int request_size;//要调度的页面数
	int** table;//存放页面走向时各页框的内容
	int* miss;//缺页数
	int* replace;//置换数
	double* missrate;//缺页率
	double* replacerate;//置换率
public:
	process();//构造函数
	void setname(string n);//设置进程名
	void setsize(double s);//设置进程大小
	bool setrequest(string request);//设置页面调度顺序
	~process();//析构函数

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

	//四种置换算法
	void FIFO(HANDLE &mutex);//先进先出页面置换算法
	void LRU(HANDLE &mutex);//最近最久未使用页面置换算法
	void NUR(HANDLE &mutex);//最近未使用页面置换算法
	void OPT(HANDLE &mutex);//最优页面置换算法
};

