#pragma once
#include<iostream>
#include<string>
using namespace std;

#define MAX_USER 10  //���ɴ������û�Ŀ¼��
#define MAX_FILE 10  //ÿ���û����ɱ�����ļ���
#define MAX_LENGTH 20 //�ļ�����Ŀ¼����������ĳ���

/*�ļ��ṹ��*/
struct file {
	string name;//�ļ���
	string detail;//�ļ���������
	file* next;//��һ�ļ�ָ��
	bool isopen;//�Ƿ��
	file() {
		isopen = false;
		next = nullptr;
	}
};

/*Ŀ¼�ṹ��*/
struct directory {
	string name;//Ŀ¼��
	directory* nextdir;//��һĿ¼ָ��
	directory* predir;//��һ��Ŀ¼
	file* fileptr;//Ŀ¼�µ��ļ�ָ��
	directory* dirptr;//Ŀ¼�µ�Ŀ¼ָ��
	int file_num;
	directory() {
		file_num = 0;
		nextdir = nullptr;
		predir = nullptr;
		fileptr = nullptr;
		dirptr = nullptr;
	}
	directory(string n) {
		file_num = 0;
		name = n;
		nextdir = nullptr;
		predir = nullptr;
		fileptr = nullptr;
		dirptr = nullptr;
	}
};


class filesystem 
{
private:
	directory* dir;//��ǰĿ¼
	directory* root;//��Ŀ¼
	//int file_num;//�ļ���
	int user_num;//�û���
	//user User;//��ǰ�û�
	//user* alluser=new user[MAX_USER];//�����û�
	//int user_num;//�û���
public:
	filesystem();//���캯��
	//filesystem(user U);//���캯��
	~filesystem();//��������

	bool CD();//�ı�Ŀ¼
	bool MD();//����Ŀ¼
	void DIR();//��ʾĿ¼
	bool RD();//ɾ��Ŀ¼

	bool openall();//�������ļ�
	bool open();//�򿪵����ļ�
	bool createfile();//����һ���ļ�
	bool deletefile();//ɾ��һ���ļ�

	bool write();//д�ļ�
	bool read();//���ļ�
	bool change();//�޸��ļ�������

	bool run();//�����ļ�ϵͳ
	//void exit();//�˳�
};