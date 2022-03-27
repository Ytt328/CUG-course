#include"filesystem.h"
#include<iostream>
using namespace std;



/*���캯��*/
filesystem::filesystem()
{
	root = new directory("root");
	dir = root;
	user_num = 0;
}

/*���캯��*/
//filesystem::filesystem(user U)
//{
//	//dir = new directory(U.name);
//	root = new directory(U.name);
//	dir = root;
//	//User = U;
//	//alluser = new user[MAX_USER];
//}

/*��������*/
filesystem::~filesystem() 
{

}

/*�ı�Ŀ¼*/
bool filesystem::CD()
{
	directory* cd = new directory();
	cin >> cd->name;
	string name = cd->name;
	if (name == "..")
	{
		if (dir == root)
		{
			cout << "��Ŀ¼Ϊ��Ŀ¼�����ɷ�����һ����" << endl;
			return false;
		}
		dir = dir->predir;
		cout << "����Ŀ¼�ɹ�" << endl;
		return true;
	}
	directory* tmpdir = dir->dirptr;
	while (tmpdir)
	{
		if (tmpdir->name == name)
		{
			dir = tmpdir;
			cout << "����Ŀ¼�ɹ���" << endl;
			return true;
		}
		tmpdir = tmpdir->nextdir;
	}
	cout << "�ı�ʧ�ܣ������ڸ�Ŀ¼" << endl;
	return false;
}

/*�����û�Ŀ¼*/
bool filesystem::MD()
{
	directory* md = new directory();
	cin >> md->name;
	string name = md->name;
	//���Ŀ¼���Ƴ����Ƿ񳬹����������
	if (name.length() > MAX_LENGTH)
	{
		cout << "��������Ŀ¼���ƹ���" << endl;
		return false;
	}
	//����Ƿ���ڿո�
	for (int i = 0; i < name.length(); i++)
	{
		if (name[i] == ' ')
		{
			cout << "��������Ŀ¼�����ܰ����ո�" << endl;
			return false;
		}
	}
	//����û�Ŀ¼���Ƿ�ﵽ����
	if (user_num == 10) {
		cout << "���������û�Ŀ¼�Ѵ�����" << endl;
		return false;
	}
	//ֻ���ڸ�Ŀ¼�²��ܴ����û�Ŀ¼
	if (dir != root) {
		cout << "��������ֻ���ڸ�Ŀ¼�²��ܴ����û�Ŀ¼��" << endl;
		return false;
	}
	//��鵱ǰĿ¼���Ƿ����������ͬ��Ŀ¼
	if (dir->dirptr == NULL)//��ǰĿ¼��û������Ŀ¼�ļ�
	{
		md->predir = dir;
		dir->dirptr = md;
		//dir->predir->predir = dir;
		cout << "�����ɹ���" << endl;
		user_num++;
		return true;
	}
	else {//��ǰĿ¼�´�������Ŀ¼�ļ�
		directory* tmpdir = dir->dirptr;
		while (tmpdir)
		{
			if(tmpdir->name == name)
			{
				cout << "����ʧ�ܣ��Ѵ���ͬ��Ŀ¼" << endl;
				return false;
			}
			tmpdir = tmpdir->nextdir;
		}
		md->nextdir = dir->dirptr;
		md->predir = dir;
		dir->dirptr = md;
		cout << "�����ɹ���" << endl;
		user_num++;
		return true;
	}
}

/*��ʾĿ¼*/
void filesystem::DIR()
{
	//��ʾ��ǰĿ¼�µ�����
	cout << dir->name <<":"<< endl;
	//��ʾ��ǰĿ¼�������ļ�
	cout << "\t�ļ���" << endl;
	file* tmpfile = dir->fileptr;
	while (tmpfile) {
		cout <<"\t"<< tmpfile->name << endl;
		tmpfile = tmpfile->next;
	}
	//��ʾ��ǰĿ¼������Ŀ¼
	cout << "\n\tĿ¼��" << endl;
	directory* tmpdir = dir->dirptr;
	while (tmpdir)
	{
		cout <<"\t"<< tmpdir->name << endl;
		tmpdir = tmpdir->nextdir;
	}
}

/*ɾ��Ŀ¼*/
bool filesystem::RD()
{
	directory* rd = new directory();
	cin >> rd->name;
	string name = rd->name;
	directory* tmpdir = dir->dirptr;
	dir->dirptr = tmpdir;
	bool is_delete = true;
	if (tmpdir->name == name)//Ҫɾ����Ŀ¼�ڸ�Ŀ¼�������ŵ�һλ
	{
		if (tmpdir->fileptr != NULL || tmpdir->dirptr != NULL)
		{
			char choice;
			cout << "ȷ��ɾ����Ŀ¼�µ������ļ���Ŀ¼��y/n" << endl;
			cin >> choice;
			if (choice == 'n') {
				is_delete = false;
			}
			else if (choice == 'y') {
				is_delete = true;
			}
			else {
				cout << "�������" << endl;
				is_delete = false;
			}
		}
		if (is_delete) {
			dir->dirptr = tmpdir->nextdir;
			cout << "ɾ���ɹ���" << endl;
			user_num--;
			return true;
		}
	}
	while (tmpdir->nextdir) {
		if (tmpdir->nextdir->name == name) {
			if (tmpdir->nextdir->fileptr != NULL || tmpdir->nextdir->dirptr != NULL)
			{
				char choice;
				cout << "ȷ��ɾ����Ŀ¼�µ������ļ���Ŀ¼��y/n" << endl;
				cin >> choice;
				if (choice == 'n') {
					is_delete = false;
				}
				else if (choice == 'y') {
					is_delete = true;
				}
				else {
					cout << "�������" << endl;
					is_delete = false;
				}
			}
			if (is_delete) {
				tmpdir->nextdir = tmpdir->nextdir->nextdir;
				cout << "ɾ���ɹ���" << endl;
				user_num--;
				return true;
			}
		}
		tmpdir = tmpdir->nextdir;
	}
	cout << "ɾ��ʧ�ܣ��Ҳ�����Ŀ¼" << endl;
	return false;
}

/*�������ļ�*/
bool filesystem::openall()
{
	file* tmp = dir->fileptr;
	if (tmp == NULL)
	{
		cout << "��ʧ�ܣ���ǰĿ¼��û���ļ�" << endl;
		return false;
	}
	cout << "�򿪳ɹ���" << endl;
	while (tmp)
	{
		cout << tmp->name << ": " << tmp->detail << endl;
		tmp = tmp->next;
	}
	return true;
}

/*�򿪵����ļ�*/
bool filesystem::open()
{
	file* openfile = new file();
	cin >> openfile->name;//����Ҫ�򿪵��ļ���
	string name = openfile->name;
	file* tmp = dir->fileptr;
	dir->fileptr = tmp;
	while (tmp)
	{
		if (tmp->name == name)
		{
			tmp->isopen = true;
			cout << "�򿪳ɹ���" << endl;
			cout << name << ":" << tmp->detail << endl;
			return true;
		}
		tmp = tmp->next;
	}
	cout << "��ʧ�ܣ���ǰĿ¼���Ҳ������ļ�" << endl;
	return false;
}

/*�����ļ�*/
bool filesystem::createfile()
{
	
	file* newfile = new file;
	//getline(cin, newfile->name, '\n');
	cin >> newfile->name;//����Ҫ�������ļ���
	string name = newfile->name;
	if (dir == root) {
		cout << "����ʧ�ܣ���ת���û�Ŀ¼�´����ļ���" << endl;
		return false;
	}
	if (dir->file_num == MAX_FILE) {
		cout << "����ʧ�ܣ��ļ��������ޣ�" << endl;
		return false;
	}
	//����ļ����Ƿ񳬳�����
	if (name.length() > MAX_LENGTH)
	{
		cout << "�ļ���������" << endl;
		return false;
	}
	//����ļ����Ƿ��пո�
	for (int i = 0; i < name.length(); i++) {
		if (name[i] == ' ')
		{
			cout << "�ļ��������ܺ��пո�" << endl;
			return false;
		}
	}
	//��鵱ǰ�ļ����Ƿ񳬹�ÿ���û��ɱ��������ļ���
	/*if (file_num >= MAX_FILE)
	{
		cout << "����ʧ�ܣ����û��ɱ�����ļ����Ѵ����ޣ�" << endl;
		return false;
	}*/
	//��鵱ǰĿ¼���Ƿ����ͬ���ļ�
	if (dir->fileptr == NULL)//��ǰĿ¼��Ϊ��
	{
		dir->fileptr = newfile;
		dir->file_num++; 
		cout << "�����ɹ�!" << endl;
		return true;
	}
	else {//��ǰĿ¼�����ļ�
		file* tmp = dir->fileptr;
		while (tmp)
		{
			if (tmp->name == name)
			{
				cout << "����ʧ�ܣ��Ѵ���ͬ���ļ�" << endl;
				return false;
			}
			tmp = tmp->next;
		}
		//�ļ�������ӵ���ǰĿ¼��
	    newfile->next = dir->fileptr;
	    dir->fileptr = newfile;
		dir->file_num++;
	    cout << "�����ɹ���" << endl;
	    return true;
	}
	
}

/*ɾ���ļ�*/
bool filesystem::deletefile()
{
	file* Deletefile = new file();
	cin >> Deletefile->name;//����Ҫɾ�����ļ���
	string name = Deletefile->name;
	file* tmp = dir->fileptr;
	dir->fileptr = tmp;
	if (tmp->name == name) {
		//Ҫɾ�����ļ���Ŀ¼�µ�һ���ļ�
		dir->fileptr = tmp->next;
		dir->file_num--;
		cout << "ɾ���ɹ�" << endl;
		return true;
	}
	else {
		//Ҫɾ�����ļ�����Ŀ¼�µ�һ���ļ�
		if (tmp->next == NULL)
		{
			cout << "ɾ��ʧ�ܣ���ǰĿ¼�²����ڸ��ļ�" << endl;
			return false;
		}
		while (tmp->next) {
			if (tmp->next->name == name)
			{
				tmp->next = tmp->next->next;//��������ɾ�����ļ�
				dir->file_num--;
				cout << "ɾ���ɹ�" << endl;
				return true;
			}
			tmp = tmp->next;
		}
		cout << "ɾ��ʧ�ܣ���ǰĿ¼�²����ڸ��ļ�" << endl;
		return false;
	}

}

/*д�ļ�*/
bool filesystem::write()
{
	file* writefile = new file();
	cin >> writefile->name;//����Ҫд���ļ���
	string name = writefile->name;
	file* tmp = dir->fileptr;
	dir->fileptr = tmp;
	while (tmp)
	{
		if (tmp->name == name)
		{
			if (!tmp->isopen) {
				cout << "�ļ�δ�򿪣����ȴ򿪸��ļ��ٽ���д������" << endl;
				return false;
			}
			bool end = false;
			while (!end) {
				string tmpdetail;
				cin >> tmpdetail;
				tmp->detail +="\n"+ tmpdetail;
				cout << "�Ƿ�Ҫ��������?  y/n ";
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
						cout << "��������������������ѡ��";
					}
				}
				//cin >> tmp->detail;//����Ҫд���ļ�������
			}
			
			cout << "д��ɹ���" << endl;
			return true;
		}
		tmp = tmp->next;
	}
	//��ǰĿ¼��ƥ�䲻��Ҫд���ļ�������
	cout << "���󣡵�ǰĿ¼�Ҳ������ļ���" << endl;
	return false;
}

/*���ļ�*/
bool filesystem::read()
{
	file* readfile = new file();
	cin >> readfile->name;//����Ҫ�����ļ�������
	string name = readfile->name;
	file* tmp = dir->fileptr;
	while (tmp)
	{
		if (tmp->name == name)
		{
			cout << "��ȡ�ɹ���" << endl;
			cout << "�ļ�����Ϊ��" << tmp->detail << endl;
			return true;
		}
		tmp = tmp->next;
	}
	//��ǰĿ¼��ƥ�䲻��Ҫ�����ļ�������
	cout << "���󣡵�ǰĿ¼�Ҳ������ļ���" << endl;
	return false;
}

/*�ı��ļ�������*/
bool filesystem::change()
{
	return true;
}

/*�����ļ�ϵͳ*/
bool filesystem::run()
{
	cout << "1.create �ļ���----�����ļ�" << endl;
	cout << "2.write �ļ���-----д�ļ�" << endl;
	cout << "3.read �ļ���------���ļ�" << endl;
	cout << "4.delete �ļ���----ɾ���ļ�" << endl;
	cout << "5.open �ļ���------���ļ�" << endl;
	cout << "6.openall----------�������ļ�" << endl;
	cout << "7.md Ŀ¼��--------����Ŀ¼(�û�Ŀ¼)" << endl;
	cout << "8.rd Ŀ¼��--------ɾ��Ŀ¼(�û�Ŀ¼)" << endl;
	cout << "9.dir--------------��ʾĿ¼" << endl;
	cout << "10.cd Ŀ¼��-------����Ŀ¼" << endl;
	cout << "11.cd ..-----------������һ��Ŀ¼" << endl;
	cout << "12.exit------------�˳�" << endl;
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
			cout << "�޷�ʶ������" << endl;
			//break;
		}
	}
}