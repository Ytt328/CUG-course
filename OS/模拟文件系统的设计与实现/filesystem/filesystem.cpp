#include"filesystem.h"
#include<iostream>
using namespace std;



/*构造函数*/
filesystem::filesystem()
{
	root = new directory("root");
	dir = root;
	user_num = 0;
}

/*构造函数*/
//filesystem::filesystem(user U)
//{
//	//dir = new directory(U.name);
//	root = new directory(U.name);
//	dir = root;
//	//User = U;
//	//alluser = new user[MAX_USER];
//}

/*析构函数*/
filesystem::~filesystem() 
{

}

/*改变目录*/
bool filesystem::CD()
{
	directory* cd = new directory();
	cin >> cd->name;
	string name = cd->name;
	if (name == "..")
	{
		if (dir == root)
		{
			cout << "该目录为根目录，不可返回上一级！" << endl;
			return false;
		}
		dir = dir->predir;
		cout << "更改目录成功" << endl;
		return true;
	}
	directory* tmpdir = dir->dirptr;
	while (tmpdir)
	{
		if (tmpdir->name == name)
		{
			dir = tmpdir;
			cout << "更改目录成功！" << endl;
			return true;
		}
		tmpdir = tmpdir->nextdir;
	}
	cout << "改变失败！不存在该目录" << endl;
	return false;
}

/*创建用户目录*/
bool filesystem::MD()
{
	directory* md = new directory();
	cin >> md->name;
	string name = md->name;
	//检查目录名称长度是否超过最大允许长度
	if (name.length() > MAX_LENGTH)
	{
		cout << "创建错误！目录名称过长" << endl;
		return false;
	}
	//检查是否存在空格
	for (int i = 0; i < name.length(); i++)
	{
		if (name[i] == ' ')
		{
			cout << "创建错误！目录名不能包含空格" << endl;
			return false;
		}
	}
	//检查用户目录数是否达到上限
	if (user_num == 10) {
		cout << "创建错误！用户目录已达上限" << endl;
		return false;
	}
	//只有在根目录下才能创建用户目录
	if (dir != root) {
		cout << "创建错误！只有在根目录下才能创建用户目录！" << endl;
		return false;
	}
	//检查当前目录下是否存在名字相同的目录
	if (dir->dirptr == NULL)//当前目录下没有其他目录文件
	{
		md->predir = dir;
		dir->dirptr = md;
		//dir->predir->predir = dir;
		cout << "创建成功！" << endl;
		user_num++;
		return true;
	}
	else {//当前目录下存在其他目录文件
		directory* tmpdir = dir->dirptr;
		while (tmpdir)
		{
			if(tmpdir->name == name)
			{
				cout << "创建失败！已存在同名目录" << endl;
				return false;
			}
			tmpdir = tmpdir->nextdir;
		}
		md->nextdir = dir->dirptr;
		md->predir = dir;
		dir->dirptr = md;
		cout << "创建成功！" << endl;
		user_num++;
		return true;
	}
}

/*显示目录*/
void filesystem::DIR()
{
	//显示当前目录下的内容
	cout << dir->name <<":"<< endl;
	//显示当前目录下所有文件
	cout << "\t文件：" << endl;
	file* tmpfile = dir->fileptr;
	while (tmpfile) {
		cout <<"\t"<< tmpfile->name << endl;
		tmpfile = tmpfile->next;
	}
	//显示当前目录下所有目录
	cout << "\n\t目录：" << endl;
	directory* tmpdir = dir->dirptr;
	while (tmpdir)
	{
		cout <<"\t"<< tmpdir->name << endl;
		tmpdir = tmpdir->nextdir;
	}
}

/*删除目录*/
bool filesystem::RD()
{
	directory* rd = new directory();
	cin >> rd->name;
	string name = rd->name;
	directory* tmpdir = dir->dirptr;
	dir->dirptr = tmpdir;
	bool is_delete = true;
	if (tmpdir->name == name)//要删除的目录在该目录链表中排第一位
	{
		if (tmpdir->fileptr != NULL || tmpdir->dirptr != NULL)
		{
			char choice;
			cout << "确定删除该目录下的所有文件和目录？y/n" << endl;
			cin >> choice;
			if (choice == 'n') {
				is_delete = false;
			}
			else if (choice == 'y') {
				is_delete = true;
			}
			else {
				cout << "输入错误！" << endl;
				is_delete = false;
			}
		}
		if (is_delete) {
			dir->dirptr = tmpdir->nextdir;
			cout << "删除成功！" << endl;
			user_num--;
			return true;
		}
	}
	while (tmpdir->nextdir) {
		if (tmpdir->nextdir->name == name) {
			if (tmpdir->nextdir->fileptr != NULL || tmpdir->nextdir->dirptr != NULL)
			{
				char choice;
				cout << "确定删除该目录下的所有文件和目录？y/n" << endl;
				cin >> choice;
				if (choice == 'n') {
					is_delete = false;
				}
				else if (choice == 'y') {
					is_delete = true;
				}
				else {
					cout << "输入错误！" << endl;
					is_delete = false;
				}
			}
			if (is_delete) {
				tmpdir->nextdir = tmpdir->nextdir->nextdir;
				cout << "删除成功！" << endl;
				user_num--;
				return true;
			}
		}
		tmpdir = tmpdir->nextdir;
	}
	cout << "删除失败！找不到该目录" << endl;
	return false;
}

/*打开所有文件*/
bool filesystem::openall()
{
	file* tmp = dir->fileptr;
	if (tmp == NULL)
	{
		cout << "打开失败！当前目录下没有文件" << endl;
		return false;
	}
	cout << "打开成功！" << endl;
	while (tmp)
	{
		cout << tmp->name << ": " << tmp->detail << endl;
		tmp = tmp->next;
	}
	return true;
}

/*打开单个文件*/
bool filesystem::open()
{
	file* openfile = new file();
	cin >> openfile->name;//输入要打开的文件名
	string name = openfile->name;
	file* tmp = dir->fileptr;
	dir->fileptr = tmp;
	while (tmp)
	{
		if (tmp->name == name)
		{
			tmp->isopen = true;
			cout << "打开成功！" << endl;
			cout << name << ":" << tmp->detail << endl;
			return true;
		}
		tmp = tmp->next;
	}
	cout << "打开失败！当前目录下找不到该文件" << endl;
	return false;
}

/*创建文件*/
bool filesystem::createfile()
{
	
	file* newfile = new file;
	//getline(cin, newfile->name, '\n');
	cin >> newfile->name;//输入要创建的文件名
	string name = newfile->name;
	if (dir == root) {
		cout << "创建失败！请转到用户目录下创建文件！" << endl;
		return false;
	}
	if (dir->file_num == MAX_FILE) {
		cout << "创建失败！文件数达上限！" << endl;
		return false;
	}
	//检查文件名是否超出长度
	if (name.length() > MAX_LENGTH)
	{
		cout << "文件名过长！" << endl;
		return false;
	}
	//检查文件名是否含有空格
	for (int i = 0; i < name.length(); i++) {
		if (name[i] == ' ')
		{
			cout << "文件命名不能含有空格！" << endl;
			return false;
		}
	}
	//检查当前文件数是否超过每个用户可保存的最大文件数
	/*if (file_num >= MAX_FILE)
	{
		cout << "创建失败！该用户可保存的文件数已达上限！" << endl;
		return false;
	}*/
	//检查当前目录下是否存在同名文件
	if (dir->fileptr == NULL)//当前目录下为空
	{
		dir->fileptr = newfile;
		dir->file_num++; 
		cout << "创建成功!" << endl;
		return true;
	}
	else {//当前目录下有文件
		file* tmp = dir->fileptr;
		while (tmp)
		{
			if (tmp->name == name)
			{
				cout << "创建失败！已存在同名文件" << endl;
				return false;
			}
			tmp = tmp->next;
		}
		//文件无误，添加到当前目录下
	    newfile->next = dir->fileptr;
	    dir->fileptr = newfile;
		dir->file_num++;
	    cout << "创建成功！" << endl;
	    return true;
	}
	
}

/*删除文件*/
bool filesystem::deletefile()
{
	file* Deletefile = new file();
	cin >> Deletefile->name;//输入要删除的文件名
	string name = Deletefile->name;
	file* tmp = dir->fileptr;
	dir->fileptr = tmp;
	if (tmp->name == name) {
		//要删除的文件是目录下第一个文件
		dir->fileptr = tmp->next;
		dir->file_num--;
		cout << "删除成功" << endl;
		return true;
	}
	else {
		//要删除的文件不是目录下第一个文件
		if (tmp->next == NULL)
		{
			cout << "删除失败！当前目录下不存在该文件" << endl;
			return false;
		}
		while (tmp->next) {
			if (tmp->next->name == name)
			{
				tmp->next = tmp->next->next;//从链表中删除该文件
				dir->file_num--;
				cout << "删除成功" << endl;
				return true;
			}
			tmp = tmp->next;
		}
		cout << "删除失败！当前目录下不存在该文件" << endl;
		return false;
	}

}

/*写文件*/
bool filesystem::write()
{
	file* writefile = new file();
	cin >> writefile->name;//输入要写的文件名
	string name = writefile->name;
	file* tmp = dir->fileptr;
	dir->fileptr = tmp;
	while (tmp)
	{
		if (tmp->name == name)
		{
			if (!tmp->isopen) {
				cout << "文件未打开！请先打开该文件再进行写操作！" << endl;
				return false;
			}
			bool end = false;
			while (!end) {
				string tmpdetail;
				cin >> tmpdetail;
				tmp->detail +="\n"+ tmpdetail;
				cout << "是否要结束输入?  y/n ";
				while (true) {
					char choice;
					cin >> choice;
					if (choice == 'y') {
						end = true;
						break;
					}
					else if (choice == 'n') {
						end = false;
						break;
					}
					else {
						cout << "输入错误！请重新输入你的选择：";
					}
				}
				//cin >> tmp->detail;//输入要写入文件的内容
			}
			
			cout << "写入成功！" << endl;
			return true;
		}
		tmp = tmp->next;
	}
	//当前目录下匹配不到要写的文件，报错
	cout << "错误！当前目录找不到该文件！" << endl;
	return false;
}

/*读文件*/
bool filesystem::read()
{
	file* readfile = new file();
	cin >> readfile->name;//输入要读的文件的名字
	string name = readfile->name;
	file* tmp = dir->fileptr;
	while (tmp)
	{
		if (tmp->name == name)
		{
			cout << "读取成功！" << endl;
			cout << "文件内容为：" << tmp->detail << endl;
			return true;
		}
		tmp = tmp->next;
	}
	//当前目录下匹配不到要读的文件，报错
	cout << "错误！当前目录找不到该文件！" << endl;
	return false;
}

/*改变文件保护码*/
bool filesystem::change()
{
	return true;
}

/*运行文件系统*/
bool filesystem::run()
{
	cout << "1.create 文件名----创建文件" << endl;
	cout << "2.write 文件名-----写文件" << endl;
	cout << "3.read 文件名------读文件" << endl;
	cout << "4.delete 文件名----删除文件" << endl;
	cout << "5.open 文件名------打开文件" << endl;
	cout << "6.openall----------打开所有文件" << endl;
	cout << "7.md 目录名--------创建目录(用户目录)" << endl;
	cout << "8.rd 目录名--------删除目录(用户目录)" << endl;
	cout << "9.dir--------------显示目录" << endl;
	cout << "10.cd 目录名-------更改目录" << endl;
	cout << "11.cd ..-----------返回上一级目录" << endl;
	cout << "12.exit------------退出" << endl;
	string choice;
	while (true) {
		directory* tmpdir = dir;
		string address(tmpdir->name + "/");
		while (tmpdir->predir)
		{
			address = tmpdir->predir->name + "/" + address;
			tmpdir = tmpdir->predir;
		}
		cout << address << ">";
		cin >> choice;
		if (choice == "create")
		{
			createfile();
			//break;
		}
		else if (choice == "write")
		{
			write();
			//break;
		}
		else if (choice == "read")
		{
			read();
			//break;
		}
		else if (choice == "delete")
		{
			deletefile();
			//break;
		}
		else if (choice == "open")
		{
			open();
			//break;
		}
		else if (choice == "openall")
		{
			openall();
			//break;
		}
		else if (choice == "md")
		{
			MD();
			//break;
		}
		else if (choice == "rd")
		{
			RD();
			//break;
		}
		else if (choice == "dir")
		{
			DIR();
			//break;
		}
		else if (choice == "cd")
		{
			CD();
			//break;
		}
		else if (choice == "exit")
		{
			return false;
		}
		else
		{
			cout << "无法识别的语句" << endl;
			//break;
		}
	}
}