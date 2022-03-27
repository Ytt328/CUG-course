#include "pch.h"
#include "multiprocess.h"

static HANDLE mutex = CreateMutexA(NULL, FALSE, NULL);
DWORD WINAPI RUN(LPVOID lpPaggam)
{
	m_struct* tmp;
	tmp = (m_struct*)lpPaggam;
    if (tmp->choice == 0)
	{
		tmp->p.FIFO(mutex);
	}
	else if (tmp->choice == 1)
	{
		tmp->p.LRU(mutex);
	}
	else if (tmp->choice == 2)
	{
		tmp->p.NUR(mutex);
	}
	else if(tmp->choice==3)
	{
		tmp->p.OPT(mutex);
	}
	//CloseHandle(mutex);
	return 0;
}



/*构造函数*/
multiprocess::multiprocess()
{
	//mutex = CreateMutexA(NULL, FALSE, "mutex");
	size = 25600;
	vp_num = size / PAGESIZE;
	A.setsize(4300.8);
	A.setname("A");
	B.setsize(8192);
	B.setname("B");
	C.setsize(10035.2);
	C.setname("C");
}

/*初始化页面请求*/
bool multiprocess::create(string requesta,string requestb,string requestc)
{
	size = 25600;
	vp_num = size / PAGESIZE;
	if (!A.setrequest(requesta))
	{
		return false;
	}
	if (!B.setrequest(requestb))
	{
		return false;
	}
	if (!C.setrequest(requestc)) {
		return false;
	}
	return true;
}

/*析构函数*/
multiprocess::~multiprocess()
{

}

/*运行*/
void multiprocess::run(int choice)
{
	a.p = A;
	a.choice = choice;
	b.p = B;
	b.choice = choice;
	c.p = C;
	c.choice = choice;
	threada=CreateThread(NULL, 0, RUN, &a, 0, NULL);
	threadb=CreateThread(NULL, 0, RUN, &b, 0, NULL);
	threadc=CreateThread(NULL, 0, RUN, &c, 0, NULL);
	//CloseHandle(mutex);

	//switch (choice) {
	//case 0://FIFO置换算法
	//	CreateThread(NULL, 0, RUN, &a, 0, NULL);
	//	A.FIFO(mutex);
	//	B.FIFO(mutex);
	//	C.FIFO(mutex);
	//	break;
	//case 1://LRU置换算法
	//	A.LRU(mutex);
	//	B.LRU(mutex);
	//	C.LRU(mutex);
	//	break;
	//case 2://NRU置换算法
	//	A.NUR(mutex);
	//	B.NUR(mutex);
	//	C.NUR(mutex);
	//	break;
	//case 3://OPT置换算法
	//	A.OPT(mutex);
	//	B.OPT(mutex);
	//	C.OPT(mutex);
	//	break;
	//}
}
