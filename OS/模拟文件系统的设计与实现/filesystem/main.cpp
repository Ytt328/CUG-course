#include"filesystem.h"
#include<iostream>
#include<iomanip>
using namespace std;


int main()
{
	filesystem FS;
	if (!FS.run())
	{
		system("PAUSE");
		return 0;
	}
}