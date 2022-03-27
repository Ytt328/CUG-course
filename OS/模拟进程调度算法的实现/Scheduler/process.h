#pragma once
#include <string>
#include<vector>
using namespace std;

struct work {
	string name;//������
	int arrive_time;//����ʱ��
	int run_time;//����ʱ��
	int start_time;//��ҵ��ʼ����ʱ��
	int end_time;//��ҵ���н���ʱ���
	int turnaround_time;//��ҵ��תʱ��
	double weighted_turnaround_time;//��ҵ��Ȩ��תʱ��
	bool is_worked;//��ǽ����Ƿ��Ѿ���ִ��
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
	process();//����
	~process();//����
	void add_process(work w);//�������
	bool is_repeat(string s);//�������ʱ���������Ƿ��ظ�

	void reset(); //���ø����̳�ʼֵ
	void sort1();//������ʱ���С��������
	void sort2();//������ʱ���С��������
	void sort3();//����ʼʱ���С��������

	void set(int i,int& time);//���ý��̵Ŀ�ʼʱ�䡢����ʱ�����Ϣ������FCFS
	void FCFS();//�����ȷ���

	int find(int a,int time);//���Ҿ��������е�һ���������ҵ������SJF1
	void SJF1();//����ҵ���ȣ�����ռʽ��
	void SJF2();//����ҵ���ȣ���ռʽ��

	int get_Rp(int time);//��ȡ��Ӧ����ߵĽ��̣�����HRpF
	void set1(int i,int& time);//���ý�����Ϣ������HRpF
	void HRpF();//����Ӧ������

	double get_avg_turnaround();//����ƽ����תʱ��
	double get_avg_weightturnaround();//����ƽ����Ȩ��תʱ��
	vector<work> get_allschedule();//�������н���
private:
	vector<work> allschedule;
};

