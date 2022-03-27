#pragma once
#include <string>
#include<vector>
using namespace std;

struct work {
	string name;//进程名
	int arrive_time;//到达时间
	int run_time;//运行时间
	int start_time;//作业开始运行时间
	int end_time;//作业运行结束时间点
	int turnaround_time;//作业周转时间
	double weighted_turnaround_time;//作业带权周转时间
	bool is_worked;//标记进程是否已经被执行
	work(string n, int a, int r)
	{
		name = n;
		arrive_time = a;
		run_time = r;
		is_worked = FALSE;
	}
	work()
	{
		arrive_time = -1;
		run_time = -1;
		start_time = -1;
		end_time = -1;
		turnaround_time = -1;
		weighted_turnaround_time = -1;
		is_worked = FALSE;
	}
};


class process{
public:
	process();//构造
	~process();//析构
	void add_process(work w);//加入进程
	bool is_repeat(string s);//加入进程时检查进程名是否重复

	void reset(); //设置各进程初始值
	void sort1();//按到达时间从小到大排序
	void sort2();//按运行时间从小到大排序
	void sort3();//按开始时间从小到大排序

	void set(int i,int& time);//设置进程的开始时间、运行时间等信息，用于FCFS
	void FCFS();//先来先服务

	int find(int a,int time);//查找就绪队列中第一个到达的作业，用于SJF1
	void SJF1();//短作业优先（非抢占式）
	void SJF2();//短作业优先（抢占式）

	int get_Rp(int time);//获取响应比最高的进程，用于HRpF
	void set1(int i,int& time);//设置进程信息，用于HRpF
	void HRpF();//高响应比优先

	double get_avg_turnaround();//计算平均周转时间
	double get_avg_weightturnaround();//计算平均带权周转时间
	vector<work> get_allschedule();//返回所有进程
private:
	vector<work> allschedule;
};

