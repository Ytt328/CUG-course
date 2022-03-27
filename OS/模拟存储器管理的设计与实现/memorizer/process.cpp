#include "pch.h"
#include "process.h"
#include<sstream>
#include<vector>
#include<thread>

/*构造函数*/
process::process()
{
	physical_page = new Ppage();
	page_request = new Vpage();
	request_size = 0;
	miss = new int[1];
	replace = new int[1];
	missrate = new double[1];
	replacerate = new double[1];
	//创建四个页框
	p_num = 4;
	int i = p_num;
	physical_page->num = i;
	i--;
	while (i) {
		Ppage* newpage = new Ppage(i);
		newpage->nextpage = physical_page;
		physical_page = newpage;
		i--;
	}
}


/*设置进程名*/
void process::setname(string n)
{
	name = n;
}


/*设置进程大小*/
void process::setsize(double s)
{
	size = s;
}


/*设置页面调度顺序*/
bool process::setrequest(string request)
{
	//根据输入的页面调度顺序初始化page_request
	//先将输入的数字存入一个数组中
	vector<int> requestarr;
	stringstream strstm(request);
	int tmp;
	while (strstm >> tmp)
	{
		if (tmp < 0 || tmp>PAGENUM) {
			return false;
		}
		requestarr.push_back(tmp);
	}
	//再初始化page_request
	Vpage* tmppage = page_request;
	page_request = tmppage;
	for (int i = 0; i < requestarr.size(); i++)
	{
		if (tmppage->num==-1) {
			tmppage->num = requestarr[i];
		}
		else {
			while (tmppage->nextpage) {
				tmppage = tmppage->nextpage;
			}
			Vpage* newVpage = new Vpage();
			newVpage->num = requestarr[i];
			tmppage->nextpage = newVpage;
		}
	}
	request_size = requestarr.size();

	table = new int* [p_num];
	for (int i = 0; i < p_num; i++)
	{
		table[i] = new int[request_size];
	}
	return true;
}

/*析构函数*/
process::~process()
{

}

/*先进先出页面置换算法*/
void process::FIFO(HANDLE &mutex)
{
	//1.首先检查页框中是否存在当前要调度的页面
	//2.若1不存在则检查页框中是否存在空页面
	//3.若2不存在则要进行页面置换，置换出最老的的页面
	//页框置空
	Ppage* tmp = physical_page;
	physical_page = tmp;
	while (tmp) {
		tmp->vnum = -1;
		tmp = tmp->nextpage;
	}

	//表置空
	for (int i = 0; i < p_num; i++) {
		for (int j = 0; j < request_size; j++)
		{
			table[i][j] = -1;
		}
	}
	miss[0] = 0;
	replace[0] = 0;
	missrate[0] = 0;
	replacerate[0] = 0;
	
	Vpage* request = page_request;
	page_request = request;
	int first = 1;//存有最老的页面的页框号
	int index = 0;
	while (request)
	{
		//Sleep(100);
		WaitForSingleObject(mutex, 100);
		Ppage* physical = physical_page;
		physical_page = physical;
		int flag = 0;//表示该页面请求是否完成
		//先检查现在页框中是否存在当前要调度的页面
		while (physical) {
			if (physical->vnum == request->num)
			{
				//当前页框中存在，则这次不缺页
				request->is_missing = false;
				flag = 1;//标记该页面请求完成
				break;
			}
			physical = physical->nextpage;
		}
		//现在页框中不存在当前要调度的页面
		if (flag == 0) {
			//先检查是否存在空页框
			physical = physical_page;
			while (physical) {
				if (physical->vnum == -1) {
					//存在空页框,缺页
					physical->vnum = request->num;
					request->is_missing = true;
					miss[0]++;//缺页数加一
					flag = 1;//标记该页面请求完成
					break;
				}
				physical = physical->nextpage;
			}
			//不存在空页框，只能进行页面置换
			if (flag == 0) {
				physical = physical_page;
				while (physical) {
					if (physical->num == first) {
						//置换出最老的页面
						physical->vnum = request->num;
						request->is_missing = true;
						miss[0]++;//缺页数加一
						replace[0]++;//置换数加一
						//改变最老的界面
						if (first <= 3) {
							first++;
						}
						else if (first == 4) {
							first = 1;
						}
						flag = 1;//标记该页面请求完成
						break;
					}
					physical = physical->nextpage;
				}
			}
		}
		Ppage* tmp = physical_page;
		for (int i = 0; i < p_num; i++)
		{
			table[i][index] = tmp->vnum;
			tmp = tmp->nextpage;
		}
		index++;
		request = request->nextpage;
		ReleaseMutex(mutex);
	}
	//计算缺页率和置换率
	missrate[0] = (miss[0] * 1.0) / request_size;
	replacerate[0] = (replace[0] * 1.0) / request_size;
}

/*最近最久未使用页面置换算法*/
void process::LRU(HANDLE &mutex)
{
	//1.首先检查页框中是否存在当前要调度的页面
	//2.若1不存在则检查页框中是否存在空页面
	//3.若2不存在则要进行页面置换，置换出最久未使用的页面

	//页框置空
	Ppage* tmp = physical_page;
	physical_page = tmp;
	while (tmp) {
		tmp->vnum = -1;
		tmp = tmp->nextpage;
	}

	//表置空
	for (int i = 0; i < p_num; i++) {
		for (int j = 0; j < request_size; j++)
		{
			table[i][j] = -1;
		}
	}

	miss[0] = 0;
	replace[0] = 0;
	missrate[0] = 0;
	replacerate[0] = 0;


	//int miss = 0;//缺页次数
	//int replace = 0;//置换次数
	
	Vpage* request = page_request;
	page_request = request;
	int time = 0;
	int lasttimevisit[4] = { 0 };//页框对应的虚拟页上次被访问的时间
	int index = 0;
	while (request) {
		WaitForSingleObject(mutex, 100);
		Ppage* physical = physical_page;
		physical_page = physical;
		int flag = 0;//表示该页面请求是否完成
		//先检查现在页框中是否存在当前要调度的页面
		while (physical) {
			if (physical->vnum == request->num)
			{
				
				//当前页框中存在，则这次不缺页
				request->is_missing = false;
				flag = 1;//标记该页面请求完成
				//设置该页框的上次访问时间
				time++;
				lasttimevisit[physical->num-1] = time;
				break;
			}
			physical = physical->nextpage;
		}
		//现在页框中不存在当前要调度的页面
		if (flag == 0) {
			//先检查是否存在空页框
			physical = physical_page;
			while (physical) {
				if (physical->vnum == -1) {
					//存在空页框,缺页
					physical->vnum = request->num;
					request->is_missing = true;
					miss[0]++;//缺页数加一
					flag = 1;//标记该页面请求完成
					//设置该页框的上次访问时间
					time++;
					lasttimevisit[physical->num-1] = time;
					break;
				}
				physical = physical->nextpage;
			}
			//不存在空页框，只能进行页面置换
			if (flag == 0) {
				physical = physical_page;
				//先找到最近未使用的页框
				int min = 0;
				for (int i = 0; i < 4; i++) {
					if (lasttimevisit[i] < lasttimevisit[min]) {
						min = i;
					}
				}
				while (physical) {
					if (physical->num == min + 1) {
						physical->vnum = request->num;
						//设置该页框的上次访问时间
						time++;
						lasttimevisit[physical->num - 1] = time;
						request->is_missing = true;
						flag = 1;
						miss[0]++;
						replace[0]++;
						break;

					}
					physical = physical->nextpage;
				}
				
			}
		}
		Ppage* tmp = physical_page;
		for (int i = 0; i < p_num; i++) 
		{
			table[i][index] = tmp->vnum;
			tmp = tmp->nextpage;
		}
		index++;
		request = request->nextpage;
        ReleaseMutex(mutex);
	}	
	//计算缺页率和置换率
	missrate[0] = (miss[0] * 1.0) / request_size;
	replacerate[0] = (replace[0] * 1.0) / request_size;
}

/*最近未使用页面置换算法*/
void process::NUR(HANDLE &mutex)
{
	//1.首先检查页框中是否存在当前要调度的页面
	//2.若1不存在则检查页框中是否存在空页面
	//3.若2不存在则要进行页面置换，置换出最近未使用的页面

	//页框置空
	Ppage* tmp = physical_page;
	physical_page = tmp;
	while (tmp) {
		tmp->vnum = -1;
		tmp = tmp->nextpage;
	}

	//表置空
	for (int i = 0; i < p_num; i++) {
		for (int j = 0; j < request_size; j++)
		{
			table[i][j] = 0;
		}
	}


	miss[0] = 0;
	replace[0] = 0;
	missrate[0] = 0;
	replacerate[0] = 0;
	//int miss = 0;//缺页次数
	//int replace = 0;//置换次数
	//WaitForSingleObject(mutex, INFINITE);
	Vpage* request = page_request;
	page_request = request;
	int index = 0;
	int time[4] = { -1,-1,-1,-1 };//各页面距离上次访问的时间
	while (request) {
		WaitForSingleObject(mutex, 100);
		Ppage* physical = physical_page;
		physical_page = physical;
		int flag = 0;//表示该页面请求是否完成
		//先检查现在页框中是否存在当前要调度的页面
		while (physical) {
			if (physical->vnum == request->num)
			{
				//当前页框中存在，则这次不缺页
				request->is_missing = false;
				physical->R = 1;
				time[physical->num - 1] = 0;
				flag = 1;//标记该页面请求完成
				break;
			}
			physical = physical->nextpage;
		}
		//现在页框中不存在当前要调度的页面
		if (flag == 0) {
			//先检查是否存在空页框
			physical = physical_page;
			while (physical) {
				if (physical->vnum == -1) {
					//存在空页框,缺页
					physical->vnum = request->num;
					request->is_missing = true;
					time[physical->num - 1] = 0;
					miss[0]++;//缺页数加一
					physical->R = 1;
					//physical->V = 1;
					flag = 1;//标记该页面请求完成
					break;
				}
				physical = physical->nextpage;
			}
			//不存在空页框，只能进行页面置换
			if (flag == 0) {
				physical = physical_page;
				while (physical) {
					if (physical->R == 0 && physical->V == 0)
					{
						physical->vnum = request->num;
						request->is_missing = true;
						time[physical->num - 1] = 0;
						miss[0]++;
						replace[0]++;
						physical->R = 1;
						physical->V = 1;
						flag = 1;
						break;
					}
					physical = physical->nextpage;
				}
				if (flag == 0) {
					physical = physical_page;
					while (physical)
					{
						if (physical->R == 0 && physical->V == 1)
						{
							physical->vnum = request->num;
							request->is_missing = true;
							time[physical->num - 1] = 0;
							miss[0]++;
							replace[0]++;
							physical->R = 1;
							physical->V = 1;
							flag = 1;
							break;
						}
						physical = physical->nextpage;
					}
				}
			}
		}
		physical = physical_page;
		while (physical)
		{
			if (time[physical->num - 1] != -1)
			{
				time[physical->num - 1]++;
			}
			if (time[physical->num - 1] == 5)
			{
				physical->R = 0;
				time[physical->num - 1] = -1;
			}
			physical = physical->nextpage;
		}


		if (flag == 1)
		{
			Ppage* tmp = physical_page;
			for (int i = 0; i < p_num; i++)
			{
				table[i][index] = tmp->vnum;
				tmp = tmp->nextpage;
			}
			index++;
			request = request->nextpage;
		}
		ReleaseMutex(mutex);
	}
	
	//计算缺页率和置换率
	missrate[0] = (miss[0] * 1.0) / request_size;
	replacerate[0] = (replace[0] * 1.0) / request_size;
}

/*最优页面置换算法*/
void process::OPT(HANDLE &mutex)
{
	//1.首先检查页框中是否存在当前要调度的页面
	//2.若1不存在则检查页框中是否存在空页面
	//3.若2不存在则要进行页面置换，置换出接下来暂时不需要用到的页面

	//页框置空
	Ppage* tmp = physical_page;
	physical_page = tmp;
	while (tmp) {
		tmp->vnum = -1;
		tmp = tmp->nextpage;
	}

	//表置空
	for (int i = 0; i < p_num; i++) {
		for (int j = 0; j < request_size; j++)
		{
			table[i][j] = 0;
		}
	}

	miss[0] = 0;
	replace[0] = 0;
	missrate[0] = 0;
	replacerate[0] = 0;
	//int miss = 0;//缺页次数
	//int replace = 0;//置换次数
	//WaitForSingleObject(mutex, INFINITE);
	Vpage* request = page_request;
	page_request = request;
	int index = 0;
	int next_visit[4] = { 0 };
	int time = 0;

	while (request) {
		WaitForSingleObject(mutex, 100);
		Ppage* physical = physical_page;
		physical_page = physical;
		int flag = 0;//表示该页面请求是否完成
		//先检查现在页框中是否存在当前要调度的页面
		while (physical) {
			if (physical->vnum == request->num)
			{
				//当前页框中存在，则这次不缺页
				request->is_missing = false;
				flag = 1;//标记该页面请求完成
				break;
			}
			physical = physical->nextpage;
		}
		//现在页框中不存在当前要调度的页面
		if (flag == 0) {
			//先检查是否存在空页框
			physical = physical_page;
			while (physical) {
				if (physical->vnum == -1) {
					//存在空页框,缺页
					physical->vnum = request->num;
					request->is_missing = true;
					miss[0]++;//缺页数加一
					flag = 1;//标记该页面请求完成
					break;
				}
				physical = physical->nextpage;
			}
			//不存在空页框，只能进行页面置换
			if (flag == 0) {
				//physical = physical_page;
				Vpage* tmpV = request;
				while (tmpV) {
					physical = physical_page;
					while (physical)
					{
						if (tmpV->num == physical->vnum&&next_visit[physical->num-1]==0)
						{
							time++;
							next_visit[physical->num - 1] = time;
						}
						physical = physical->nextpage;
					}
					tmpV = tmpV->nextpage;
				}
				//查找距离下次访问时间最长的页面所在的页框的页框号
				int max = -1;
				for (int i = 0; i < p_num; i++)
				{
					if (next_visit[i] == 0)
					{
						max = i;
						break;
					}
					else if (next_visit[i] > next_visit[max])
					{
						max = i;
					}
				}
				if (max != -1)
				{
					physical = physical_page;
					while (physical)
					{
						if (physical->num == max + 1)
						{
							physical->vnum = request->num;
							request->is_missing = true;
							miss[0]++;
							replace[0]++;
							flag = 1;
							time = 0;
							for (int i = 0; i < p_num; i++)
							{
								next_visit[i] = 0;
							}

						}
						physical = physical->nextpage;
					}
				}
			}
		}
		Ppage* tmp = physical_page;
		for (int i = 0; i < p_num; i++)
		{
			table[i][index] = tmp->vnum;
			tmp = tmp->nextpage;
		}
		index++;
		request = request->nextpage;
		ReleaseMutex(mutex);
	}
	//计算缺页率和置换率
	missrate[0] = (miss[0] * 1.0) / request_size;
	replacerate[0] = (replace[0] * 1.0) / request_size;
}