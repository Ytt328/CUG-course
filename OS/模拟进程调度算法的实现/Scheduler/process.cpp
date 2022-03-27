#include "pch.h"
#include "process.h"

/*构造函数*/
process::process(){}

/*析构函数*/
process::~process(){}

/*加入进程*/
void process::add_process(work w)
{
	allschedule.push_back(w);
}

/*加入进程时判断进程名是否重复*/
bool process::is_repeat(string s)
{
	for (int i = 0; i < allschedule.size(); i++)
	{
		if (allschedule[i].name == s)
		{
			return true;
		}
	}
	return false;
}


/*计算平均周转时间*/
double process::get_avg_turnaround()
{
	int sum = 0;
	for (int i = 0; i < allschedule.size(); i++)
	{
		sum += allschedule[i].turnaround_time;
	}
	double avg = static_cast<double>(sum) / allschedule.size();
	return avg;
}


/*计算平均带权周转时间*/
double process::get_avg_weightturnaround()
{
	double sum = 0;
	for (int i = 0; i < allschedule.size(); i++)
	{
		sum += allschedule[i].weighted_turnaround_time;
	}
	double avg = sum / allschedule.size();
	return avg;
}

/*设置进程的开始时间、运行时间等信息,用于FCFS*/
void process::set(int i, int& time)
{
	if (i == 0)
	{
		allschedule[i].start_time = allschedule[i].arrive_time > time ? allschedule[i].arrive_time : time;
	}
	else {
		allschedule[i].start_time = allschedule[i].arrive_time > allschedule[i - 1].end_time ? allschedule[i].arrive_time : allschedule[i - 1].end_time;
	}

	allschedule[i].end_time = allschedule[i].start_time + allschedule[i].run_time;
	time = allschedule[i].end_time;
	allschedule[i].turnaround_time = allschedule[i].end_time - allschedule[i].arrive_time;
	allschedule[i].weighted_turnaround_time = static_cast<double>(allschedule[i].turnaround_time) / allschedule[i].run_time;
	allschedule[i].is_worked = true;//标记该进程为已执行
}

/*查找就绪队列（按运行时间排序）中第一个到达的作业，用于SJF1*/
int process::find(int a,int time)
{

	for (int i = a; i < allschedule.size(); i++)
	{
		if (allschedule[i].arrive_time <= time && (!allschedule[i].is_worked))
		{
			return i;
		}
	}
	return -1;
}

/*获取响应比最高的进程，用于HRpF*/
int process::get_Rp(int time)
{
	int size = allschedule.size();
	double* Rp = new double[size]();
	//计算响应比
	for (int i = 0; i < size; i++)
	{
		if ((!allschedule[i].is_worked) && time >= allschedule[i].arrive_time)
		{
			Rp[i] = (static_cast<double>(time - allschedule[i].arrive_time + allschedule[i].run_time)) / allschedule[i].run_time;
		}
	}
	//查找响应比最高的进程序号
	int highest = -1;
	for (int i = 0; i < size; i++)
	{
		if (Rp[i] != 0)
		{
			if (highest == -1)
			{
				highest = i;
			}
			else
			{
				if (Rp[i] > Rp[highest])
				{
					highest = i;
				}
			}
		}
	}
	return highest;
}

/*设置进程信息，用于HRpF*/
void process::set1(int i, int& time)
{
	allschedule[i].start_time = allschedule[i].arrive_time < time ? time : allschedule[i].arrive_time;
	allschedule[i].end_time = allschedule[i].start_time + allschedule[i].run_time;
	allschedule[i].turnaround_time = allschedule[i].end_time - allschedule[i].arrive_time;
	allschedule[i].weighted_turnaround_time = static_cast<double>(allschedule[i].turnaround_time) / allschedule[i].run_time;
	allschedule[i].is_worked = true;//标记该进程为已执行
	time = allschedule[i].end_time;
}


/*先来先服务算法*/
void process::FCFS()
{
	//设置各进程初始值
	reset();

	//先按到达时间从小到大排序
	sort1();
	//排序后，按排序顺序调度
	int time = 0;//表示目前的时间
	for (int i = 0; i < allschedule.size(); i++)
	{
		set(i, time);//执行进程并设置进程信息
	}
}

/*短作业优先算法（非抢占式）*/
void process::SJF1()
{
	//设置各进程初始值
	reset();
	
	//先按运行时间从小到大排序
	sort2();

	//开始调度
	int time = 0;
	for (int i = 0; i < allschedule.size();)
	{
		if (!allschedule[i].is_worked)
		{
			if (allschedule[i].arrive_time <= time)
			{
				//计算信息
				allschedule[i].start_time = time;
				allschedule[i].end_time = allschedule[i].start_time + allschedule[i].run_time;
				allschedule[i].turnaround_time = allschedule[i].end_time - allschedule[i].arrive_time;
				allschedule[i].weighted_turnaround_time = static_cast<double>(allschedule[i].turnaround_time) / allschedule[i].run_time;
				allschedule[i].is_worked = true;//标记该进程为已执行
				time = allschedule[i].end_time;
				i++;
			}
			else {
				int next = find(i,time);//查找就绪队列中第一个已到达的进程
				if (next == -1) {
					next = i;//如果就绪队列中进程均未到达，则只能执行当前的i进程
				}
				//计算信息
				allschedule[next].start_time = allschedule[next].arrive_time < time ? time : allschedule[next].arrive_time;
				allschedule[next].end_time = allschedule[next].start_time + allschedule[next].run_time;
				allschedule[next].turnaround_time = allschedule[next].end_time - allschedule[next].arrive_time;
				allschedule[next].weighted_turnaround_time = static_cast<double>(allschedule[next].turnaround_time) / allschedule[next].run_time;
				allschedule[next].is_worked = true;//标记该进程为已执行
				time = allschedule[next].end_time;
			}
		}
		else {
			i++;
		}
	}

	//最后按开始时间从小到大排序，表示调度顺序
	sort3();
}

/*短作业优先算法（抢占式）*/
void process::SJF2()
{
	//设置各进程初始值
	reset();

	//先按运行时间从小到大排序
	sort2();
	/*for (int i = 0; i < allschedule.size() - 1; i++)
	{
		for (int j = i + 1; j < allschedule.size(); j++)
		{
			if ((allschedule[i].run_time > allschedule[j].run_time) || ((allschedule[i].run_time == allschedule[j].run_time) && (allschedule[i].arrive_time > allschedule[j].arrive_time)))
			{
				work tmp = allschedule[i];
				allschedule[i] = allschedule[j];
				allschedule[j] = tmp;
			}
		}
	}*/

	//开始调度
	int time = 0;
	for (int i = 0; i < allschedule.size(); i++)
	{
		if (!allschedule[i].is_worked)
		{
			if (allschedule[i].arrive_time <= time)
			{
				//计算信息
				allschedule[i].start_time = time;
				allschedule[i].end_time = allschedule[i].start_time + allschedule[i].run_time;
				allschedule[i].turnaround_time = allschedule[i].end_time - allschedule[i].arrive_time;
				allschedule[i].weighted_turnaround_time = static_cast<double>(allschedule[i].turnaround_time) / allschedule[i].run_time;
				allschedule[i].is_worked = true;//标记该进程为已执行
				time = allschedule[i].end_time;
				i++;
			}
			else
			{
				int next = find(i, time);
				if (next == -1)
				{
					//就绪队列中不存在进程到达
					next = i;
					allschedule[next].start_time = allschedule[next].arrive_time < time ? time : allschedule[next].arrive_time;
					allschedule[next].end_time = allschedule[next].start_time + allschedule[next].run_time;
					allschedule[next].turnaround_time = allschedule[next].end_time - allschedule[next].arrive_time;
					allschedule[next].weighted_turnaround_time = static_cast<double>(allschedule[next].turnaround_time) / allschedule[next].run_time;
					allschedule[next].is_worked = true;//标记该进程为已执行
					time = allschedule[next].end_time;
				}
				else {
					//就绪队列中存在进程到达
					//此时应该执行该进程，但该进程不是运行时间最短的
					int tmpendtime = allschedule[next].run_time + time;//该作业预计结束时间
					if (allschedule[i].arrive_time < tmpendtime)
					{
						//在预计结束时间内i进程到达
						if ((tmpendtime-allschedule[i].arrive_time)>allschedule[i].run_time)
						{
							//i进程到达时
							//如果当前运行进程还需要运行的时间大于i进程需要的运行时间
							//则进程i抢占执行，当前运行进程分成两个进程
							allschedule[next].start_time = time;
							allschedule[next].end_time = allschedule[next].start_time + allschedule[i].arrive_time - time;
						}
					}
				}
			}
		}
		else {
			i++;
		}
	}

}

/*高响应比优先算法*/
void process::HRpF()
{
	//设置各进程初始值
	reset();

	//先按到达时间从小到大排序
	sort1();
	//开始调度
	int time = 0;
	for (int i = 0; i < allschedule.size();)
	{
		if (!allschedule[i].is_worked)
		{
			int next = get_Rp(time);
			if (next == -1)
			{
				next = i;
				i++;
			}
			set1(next, time);
		}
		else {
			i++;
		}
	}
	//最后按开始时间从小到大排序，表示调度顺序
	sort3();
}

/*返回所有进程*/
vector<work> process::get_allschedule()
{
	return allschedule;
}


/*设置各进程初始值*/
void process::reset() {
	for (int i = 0; i < allschedule.size(); i++)
	{
		allschedule[i].is_worked = false;
		allschedule[i].start_time = -1;
		allschedule[i].end_time = -1;
		allschedule[i].turnaround_time = -1;
		allschedule[i].weighted_turnaround_time = -1;
	}
}

/*按到达时间从小到大排序*/
void process::sort1() {
	for (int i = 0; i < allschedule.size() - 1; i++)
	{
		for (int j = i + 1; j < allschedule.size(); j++)
		{
			if (allschedule[i].arrive_time > allschedule[j].arrive_time)
			{
				work tmp = allschedule[i];
				allschedule[i] = allschedule[j];
				allschedule[j] = tmp;
			}
		}
	}
}

/*按运行时间从小到大排序*/
void process::sort2() {
	for (int i = 0; i < allschedule.size() - 1; i++)
	{
		for (int j = i + 1; j < allschedule.size(); j++)
		{
			if ((allschedule[i].run_time > allschedule[j].run_time) || ((allschedule[i].run_time == allschedule[j].run_time) && (allschedule[i].arrive_time > allschedule[j].arrive_time)))
			{
				work tmp = allschedule[i];
				allschedule[i] = allschedule[j];
				allschedule[j] = tmp;
			}
		}
	}
}

/*按开始时间从小到大排序*/
void process::sort3() {
	for (int i = 0; i < allschedule.size() - 1; i++)
	{
		for (int j = i + 1; j < allschedule.size(); j++)
		{
			if ((allschedule[i].start_time > allschedule[j].start_time))
			{
				work tmp = allschedule[i];
				allschedule[i] = allschedule[j];
				allschedule[j] = tmp;
			}
		}
	}
}
