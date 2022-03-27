#pragma once
#include<iostream>
#include<string>
using namespace std;

#define MAX_USER 10  //最多可创建的用户目录数
#define MAX_FILE 10  //每个用户最多可保存的文件数
#define MAX_LENGTH 20 //文件名和目录名可允许最长的长度

/*文件结构体*/
struct file {
	string name;//文件名
	string detail;//文件具体内容
	file* next;//下一文件指针
	bool isopen;//是否打开
	file() {
		isopen = false;
		next = nullptr;
	}
};

/*目录结构体*/
struct directory {
	string name;//目录名
	directory* nextdir;//下一目录指针
	directory* predir;//上一级目录
	file* fileptr;//目录下的文件指针
	directory* dirptr;//目录下的目录指针
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
	directory* dir;//当前目录
	directory* root;//根目录
	//int file_num;//文件数
	int user_num;//用户数
	//user User;//当前用户
	//user* alluser=new user[MAX_USER];//所有用户
	//int user_num;//用户数
public:
	filesystem();//构造函数
	//filesystem(user U);//构造函数
	~filesystem();//析构函数

	bool CD();//改变目录
	bool MD();//创建目录
	void DIR();//显示目录
	bool RD();//删除目录

	bool openall();//打开所有文件
	bool open();//打开单个文件
	bool createfile();//建立一个文件
	bool deletefile();//删除一个文件

	bool write();//写文件
	bool read();//读文件
	bool change();//修改文件保护码

	bool run();//运行文件系统
	//void exit();//退出
};