#include "pch.h"
#include "process.h"

/*���캯��*/
process::process(){}

/*��������*/
process::~process(){}

/*�������*/
void process::add_process(work w)
{
	allschedule.push_back(w);
}

/*�������ʱ�жϽ������Ƿ��ظ�*/
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


/*����ƽ����תʱ��*/
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


/*����ƽ����Ȩ��תʱ��*/
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

/*���ý��̵Ŀ�ʼʱ�䡢����ʱ�����Ϣ,����FCFS*/
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
	allschedule[i].is_worked = true;//��Ǹý���Ϊ��ִ��
}

/*���Ҿ������У�������ʱ�������е�һ���������ҵ������SJF1*/
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

/*��ȡ��Ӧ����ߵĽ��̣�����HRpF*/
int process::get_Rp(int time)
{
	int size = allschedule.size();
	double* Rp = new double[size]();
	//������Ӧ��
	for (int i = 0; i < size; i++)
	{
		if ((!allschedule[i].is_worked) && time >= allschedule[i].arrive_time)
		{
			Rp[i] = (static_cast<double>(time - allschedule[i].arrive_time + allschedule[i].run_time)) / allschedule[i].run_time;
		}
	}
	//������Ӧ����ߵĽ������
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

/*���ý�����Ϣ������HRpF*/
void process::set1(int i, int& time)
{
	allschedule[i].start_time = allschedule[i].arrive_time < time ? time : allschedule[i].arrive_time;
	allschedule[i].end_time = allschedule[i].start_time + allschedule[i].run_time;
	allschedule[i].turnaround_time = allschedule[i].end_time - allschedule[i].arrive_time;
	allschedule[i].weighted_turnaround_time = static_cast<double>(allschedule[i].turnaround_time) / allschedule[i].run_time;
	allschedule[i].is_worked = true;//��Ǹý���Ϊ��ִ��
	time = allschedule[i].end_time;
}


/*�����ȷ����㷨*/
void process::FCFS()
{
	//���ø����̳�ʼֵ
	reset();

	//�Ȱ�����ʱ���С��������
	sort1();
	//����󣬰�����˳�����
	int time = 0;//��ʾĿǰ��ʱ��
	for (int i = 0; i < allschedule.size(); i++)
	{
		set(i, time);//ִ�н��̲����ý�����Ϣ
	}
}

/*����ҵ�����㷨������ռʽ��*/
void process::SJF1()
{
	//���ø����̳�ʼֵ
	reset();
	
	//�Ȱ�����ʱ���С��������
	sort2();

	//��ʼ����
	int time = 0;
	for (int i = 0; i < allschedule.size();)
	{
		if (!allschedule[i].is_worked)
		{
			if (allschedule[i].arrive_time <= time)
			{
				//������Ϣ
				allschedule[i].start_time = time;
				allschedule[i].end_time = allschedule[i].start_time + allschedule[i].run_time;
				allschedule[i].turnaround_time = allschedule[i].end_time - allschedule[i].arrive_time;
				allschedule[i].weighted_turnaround_time = static_cast<double>(allschedule[i].turnaround_time) / allschedule[i].run_time;
				allschedule[i].is_worked = true;//��Ǹý���Ϊ��ִ��
				time = allschedule[i].end_time;
				i++;
			}
			else {
				int next = find(i,time);//���Ҿ��������е�һ���ѵ���Ľ���
				if (next == -1) {
					next = i;//������������н��̾�δ�����ֻ��ִ�е�ǰ��i����
				}
				//������Ϣ
				allschedule[next].start_time = allschedule[next].arrive_time < time ? time : allschedule[next].arrive_time;
				allschedule[next].end_time = allschedule[next].start_time + allschedule[next].run_time;
				allschedule[next].turnaround_time = allschedule[next].end_time - allschedule[next].arrive_time;
				allschedule[next].weighted_turnaround_time = static_cast<double>(allschedule[next].turnaround_time) / allschedule[next].run_time;
				allschedule[next].is_worked = true;//��Ǹý���Ϊ��ִ��
				time = allschedule[next].end_time;
			}
		}
		else {
			i++;
		}
	}

	//��󰴿�ʼʱ���С�������򣬱�ʾ����˳��
	sort3();
}

/*����ҵ�����㷨����ռʽ��*/
void process::SJF2()
{
	//���ø����̳�ʼֵ
	reset();

	//�Ȱ�����ʱ���С��������
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

	//��ʼ����
	int time = 0;
	for (int i = 0; i < allschedule.size(); i++)
	{
		if (!allschedule[i].is_worked)
		{
			if (allschedule[i].arrive_time <= time)
			{
				//������Ϣ
				allschedule[i].start_time = time;
				allschedule[i].end_time = allschedule[i].start_time + allschedule[i].run_time;
				allschedule[i].turnaround_time = allschedule[i].end_time - allschedule[i].arrive_time;
				allschedule[i].weighted_turnaround_time = static_cast<double>(allschedule[i].turnaround_time) / allschedule[i].run_time;
				allschedule[i].is_worked = true;//��Ǹý���Ϊ��ִ��
				time = allschedule[i].end_time;
				i++;
			}
			else
			{
				int next = find(i, time);
				if (next == -1)
				{
					//���������в����ڽ��̵���
					next = i;
					allschedule[next].start_time = allschedule[next].arrive_time < time ? time : allschedule[next].arrive_time;
					allschedule[next].end_time = allschedule[next].start_time + allschedule[next].run_time;
					allschedule[next].turnaround_time = allschedule[next].end_time - allschedule[next].arrive_time;
					allschedule[next].weighted_turnaround_time = static_cast<double>(allschedule[next].turnaround_time) / allschedule[next].run_time;
					allschedule[next].is_worked = true;//��Ǹý���Ϊ��ִ��
					time = allschedule[next].end_time;
				}
				else {
					//���������д��ڽ��̵���
					//��ʱӦ��ִ�иý��̣����ý��̲�������ʱ����̵�
					int tmpendtime = allschedule[next].run_time + time;//����ҵԤ�ƽ���ʱ��
					if (allschedule[i].arrive_time < tmpendtime)
					{
						//��Ԥ�ƽ���ʱ����i���̵���
						if ((tmpendtime-allschedule[i].arrive_time)>allschedule[i].run_time)
						{
							//i���̵���ʱ
							//�����ǰ���н��̻���Ҫ���е�ʱ�����i������Ҫ������ʱ��
							//�����i��ռִ�У���ǰ���н��̷ֳ���������
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

/*����Ӧ�������㷨*/
void process::HRpF()
{
	//���ø����̳�ʼֵ
	reset();

	//�Ȱ�����ʱ���С��������
	sort1();
	//��ʼ����
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
	//��󰴿�ʼʱ���С�������򣬱�ʾ����˳��
	sort3();
}

/*�������н���*/
vector<work> process::get_allschedule()
{
	return allschedule;
}


/*���ø����̳�ʼֵ*/
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

/*������ʱ���С��������*/
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

/*������ʱ���С��������*/
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

/*����ʼʱ���С��������*/
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
