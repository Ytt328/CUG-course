#include "pch.h"
#include "process.h"
#include<sstream>
#include<vector>
#include<thread>

/*���캯��*/
process::process()
{
	physical_page = new Ppage();
	page_request = new Vpage();
	request_size = 0;
	miss = new int[1];
	replace = new int[1];
	missrate = new double[1];
	replacerate = new double[1];
	//�����ĸ�ҳ��
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


/*���ý�����*/
void process::setname(string n)
{
	name = n;
}


/*���ý��̴�С*/
void process::setsize(double s)
{
	size = s;
}


/*����ҳ�����˳��*/
bool process::setrequest(string request)
{
	//���������ҳ�����˳���ʼ��page_request
	//�Ƚ���������ִ���һ��������
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
	//�ٳ�ʼ��page_request
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

/*��������*/
process::~process()
{

}

/*�Ƚ��ȳ�ҳ���û��㷨*/
void process::FIFO(HANDLE &mutex)
{
	//1.���ȼ��ҳ�����Ƿ���ڵ�ǰҪ���ȵ�ҳ��
	//2.��1����������ҳ�����Ƿ���ڿ�ҳ��
	//3.��2��������Ҫ����ҳ���û����û������ϵĵ�ҳ��
	//ҳ���ÿ�
	Ppage* tmp = physical_page;
	physical_page = tmp;
	while (tmp) {
		tmp->vnum = -1;
		tmp = tmp->nextpage;
	}

	//���ÿ�
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
	int first = 1;//�������ϵ�ҳ���ҳ���
	int index = 0;
	while (request)
	{
		//Sleep(100);
		WaitForSingleObject(mutex, 100);
		Ppage* physical = physical_page;
		physical_page = physical;
		int flag = 0;//��ʾ��ҳ�������Ƿ����
		//�ȼ������ҳ�����Ƿ���ڵ�ǰҪ���ȵ�ҳ��
		while (physical) {
			if (physical->vnum == request->num)
			{
				//��ǰҳ���д��ڣ�����β�ȱҳ
				request->is_missing = false;
				flag = 1;//��Ǹ�ҳ���������
				break;
			}
			physical = physical->nextpage;
		}
		//����ҳ���в����ڵ�ǰҪ���ȵ�ҳ��
		if (flag == 0) {
			//�ȼ���Ƿ���ڿ�ҳ��
			physical = physical_page;
			while (physical) {
				if (physical->vnum == -1) {
					//���ڿ�ҳ��,ȱҳ
					physical->vnum = request->num;
					request->is_missing = true;
					miss[0]++;//ȱҳ����һ
					flag = 1;//��Ǹ�ҳ���������
					break;
				}
				physical = physical->nextpage;
			}
			//�����ڿ�ҳ��ֻ�ܽ���ҳ���û�
			if (flag == 0) {
				physical = physical_page;
				while (physical) {
					if (physical->num == first) {
						//�û������ϵ�ҳ��
						physical->vnum = request->num;
						request->is_missing = true;
						miss[0]++;//ȱҳ����һ
						replace[0]++;//�û�����һ
						//�ı����ϵĽ���
						if (first <= 3) {
							first++;
						}
						else if (first == 4) {
							first = 1;
						}
						flag = 1;//��Ǹ�ҳ���������
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
	//����ȱҳ�ʺ��û���
	missrate[0] = (miss[0] * 1.0) / request_size;
	replacerate[0] = (replace[0] * 1.0) / request_size;
}

/*������δʹ��ҳ���û��㷨*/
void process::LRU(HANDLE &mutex)
{
	//1.���ȼ��ҳ�����Ƿ���ڵ�ǰҪ���ȵ�ҳ��
	//2.��1����������ҳ�����Ƿ���ڿ�ҳ��
	//3.��2��������Ҫ����ҳ���û����û������δʹ�õ�ҳ��

	//ҳ���ÿ�
	Ppage* tmp = physical_page;
	physical_page = tmp;
	while (tmp) {
		tmp->vnum = -1;
		tmp = tmp->nextpage;
	}

	//���ÿ�
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


	//int miss = 0;//ȱҳ����
	//int replace = 0;//�û�����
	
	Vpage* request = page_request;
	page_request = request;
	int time = 0;
	int lasttimevisit[4] = { 0 };//ҳ���Ӧ������ҳ�ϴα����ʵ�ʱ��
	int index = 0;
	while (request) {
		WaitForSingleObject(mutex, 100);
		Ppage* physical = physical_page;
		physical_page = physical;
		int flag = 0;//��ʾ��ҳ�������Ƿ����
		//�ȼ������ҳ�����Ƿ���ڵ�ǰҪ���ȵ�ҳ��
		while (physical) {
			if (physical->vnum == request->num)
			{
				
				//��ǰҳ���д��ڣ�����β�ȱҳ
				request->is_missing = false;
				flag = 1;//��Ǹ�ҳ���������
				//���ø�ҳ����ϴη���ʱ��
				time++;
				lasttimevisit[physical->num-1] = time;
				break;
			}
			physical = physical->nextpage;
		}
		//����ҳ���в����ڵ�ǰҪ���ȵ�ҳ��
		if (flag == 0) {
			//�ȼ���Ƿ���ڿ�ҳ��
			physical = physical_page;
			while (physical) {
				if (physical->vnum == -1) {
					//���ڿ�ҳ��,ȱҳ
					physical->vnum = request->num;
					request->is_missing = true;
					miss[0]++;//ȱҳ����һ
					flag = 1;//��Ǹ�ҳ���������
					//���ø�ҳ����ϴη���ʱ��
					time++;
					lasttimevisit[physical->num-1] = time;
					break;
				}
				physical = physical->nextpage;
			}
			//�����ڿ�ҳ��ֻ�ܽ���ҳ���û�
			if (flag == 0) {
				physical = physical_page;
				//���ҵ����δʹ�õ�ҳ��
				int min = 0;
				for (int i = 0; i < 4; i++) {
					if (lasttimevisit[i] < lasttimevisit[min]) {
						min = i;
					}
				}
				while (physical) {
					if (physical->num == min + 1) {
						physical->vnum = request->num;
						//���ø�ҳ����ϴη���ʱ��
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
	//����ȱҳ�ʺ��û���
	missrate[0] = (miss[0] * 1.0) / request_size;
	replacerate[0] = (replace[0] * 1.0) / request_size;
}

/*���δʹ��ҳ���û��㷨*/
void process::NUR(HANDLE &mutex)
{
	//1.���ȼ��ҳ�����Ƿ���ڵ�ǰҪ���ȵ�ҳ��
	//2.��1����������ҳ�����Ƿ���ڿ�ҳ��
	//3.��2��������Ҫ����ҳ���û����û������δʹ�õ�ҳ��

	//ҳ���ÿ�
	Ppage* tmp = physical_page;
	physical_page = tmp;
	while (tmp) {
		tmp->vnum = -1;
		tmp = tmp->nextpage;
	}

	//���ÿ�
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
	//int miss = 0;//ȱҳ����
	//int replace = 0;//�û�����
	//WaitForSingleObject(mutex, INFINITE);
	Vpage* request = page_request;
	page_request = request;
	int index = 0;
	int time[4] = { -1,-1,-1,-1 };//��ҳ������ϴη��ʵ�ʱ��
	while (request) {
		WaitForSingleObject(mutex, 100);
		Ppage* physical = physical_page;
		physical_page = physical;
		int flag = 0;//��ʾ��ҳ�������Ƿ����
		//�ȼ������ҳ�����Ƿ���ڵ�ǰҪ���ȵ�ҳ��
		while (physical) {
			if (physical->vnum == request->num)
			{
				//��ǰҳ���д��ڣ�����β�ȱҳ
				request->is_missing = false;
				physical->R = 1;
				time[physical->num - 1] = 0;
				flag = 1;//��Ǹ�ҳ���������
				break;
			}
			physical = physical->nextpage;
		}
		//����ҳ���в����ڵ�ǰҪ���ȵ�ҳ��
		if (flag == 0) {
			//�ȼ���Ƿ���ڿ�ҳ��
			physical = physical_page;
			while (physical) {
				if (physical->vnum == -1) {
					//���ڿ�ҳ��,ȱҳ
					physical->vnum = request->num;
					request->is_missing = true;
					time[physical->num - 1] = 0;
					miss[0]++;//ȱҳ����һ
					physical->R = 1;
					//physical->V = 1;
					flag = 1;//��Ǹ�ҳ���������
					break;
				}
				physical = physical->nextpage;
			}
			//�����ڿ�ҳ��ֻ�ܽ���ҳ���û�
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
	
	//����ȱҳ�ʺ��û���
	missrate[0] = (miss[0] * 1.0) / request_size;
	replacerate[0] = (replace[0] * 1.0) / request_size;
}

/*����ҳ���û��㷨*/
void process::OPT(HANDLE &mutex)
{
	//1.���ȼ��ҳ�����Ƿ���ڵ�ǰҪ���ȵ�ҳ��
	//2.��1����������ҳ�����Ƿ���ڿ�ҳ��
	//3.��2��������Ҫ����ҳ���û����û�����������ʱ����Ҫ�õ���ҳ��

	//ҳ���ÿ�
	Ppage* tmp = physical_page;
	physical_page = tmp;
	while (tmp) {
		tmp->vnum = -1;
		tmp = tmp->nextpage;
	}

	//���ÿ�
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
	//int miss = 0;//ȱҳ����
	//int replace = 0;//�û�����
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
		int flag = 0;//��ʾ��ҳ�������Ƿ����
		//�ȼ������ҳ�����Ƿ���ڵ�ǰҪ���ȵ�ҳ��
		while (physical) {
			if (physical->vnum == request->num)
			{
				//��ǰҳ���д��ڣ�����β�ȱҳ
				request->is_missing = false;
				flag = 1;//��Ǹ�ҳ���������
				break;
			}
			physical = physical->nextpage;
		}
		//����ҳ���в����ڵ�ǰҪ���ȵ�ҳ��
		if (flag == 0) {
			//�ȼ���Ƿ���ڿ�ҳ��
			physical = physical_page;
			while (physical) {
				if (physical->vnum == -1) {
					//���ڿ�ҳ��,ȱҳ
					physical->vnum = request->num;
					request->is_missing = true;
					miss[0]++;//ȱҳ����һ
					flag = 1;//��Ǹ�ҳ���������
					break;
				}
				physical = physical->nextpage;
			}
			//�����ڿ�ҳ��ֻ�ܽ���ҳ���û�
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
				//���Ҿ����´η���ʱ�����ҳ�����ڵ�ҳ���ҳ���
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
	//����ȱҳ�ʺ��û���
	missrate[0] = (miss[0] * 1.0) / request_size;
	replacerate[0] = (replace[0] * 1.0) / request_size;
}