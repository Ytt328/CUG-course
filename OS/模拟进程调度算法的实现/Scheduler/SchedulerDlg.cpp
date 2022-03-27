
// SchedulerDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "Scheduler.h"
#include "SchedulerDlg.h"
#include "afxdialogex.h"
#include"process.h"
#include"vector"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSchedulerDlg 对话框



CSchedulerDlg::CSchedulerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SCHEDULER_DIALOG, pParent)
	, m_process_name(_T(""))
	, m_arrive_time(0)
	, m_run_time(0)
	, m_ave_turnaround_time(0)
	, m_ave_wturnaround_time(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSchedulerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_process_name);
	DDX_Text(pDX, IDC_EDIT2, m_arrive_time);
	DDX_Text(pDX, IDC_EDIT3, m_run_time);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Text(pDX, IDC_EDIT, m_ave_turnaround_time);
	DDX_Text(pDX, IDC_EDIT5, m_ave_wturnaround_time);
}

BEGIN_MESSAGE_MAP(CSchedulerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CREATE, &CSchedulerDlg::OnBnClickedButtonCreate)
	ON_BN_CLICKED(IDC_BUTTON_FCFS, &CSchedulerDlg::OnBnClickedButtonFcfs)
	ON_BN_CLICKED(IDC_BUTTON_SJF1, &CSchedulerDlg::OnBnClickedButtonSjf1)
	//ON_BN_CLICKED(IDC_BUTTON_SJF2, &CSchedulerDlg::OnBnClickedButtonSjf2)
	ON_BN_CLICKED(IDC_BUTTON_HRPF, &CSchedulerDlg::OnBnClickedButtonHrpf)
END_MESSAGE_MAP()


// CSchedulerDlg 消息处理程序

BOOL CSchedulerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	

	CRect rect;
	// 获取编程语言列表视图控件的位置和大小   
	m_list.GetClientRect(&rect);

	// 为列表视图控件添加全行选中和栅格风格   
	m_list.SetExtendedStyle(m_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	// 为列表视图控件添加七列   
	m_list.InsertColumn(0, _T("进程名"), LVCFMT_CENTER, rect.Width() / 7, 0);
	m_list.InsertColumn(1, _T("到达时间"), LVCFMT_CENTER, rect.Width() / 7, 1);
	m_list.InsertColumn(2, _T("运行时间"), LVCFMT_CENTER, rect.Width() / 7, 2);
	m_list.InsertColumn(3, _T("开始时间"), LVCFMT_CENTER, rect.Width() / 7, 3);
	m_list.InsertColumn(4, _T("结束时间"), LVCFMT_CENTER, rect.Width() / 7, 4);
	m_list.InsertColumn(5, _T("周转时间"), LVCFMT_CENTER, rect.Width() / 7, 5);
	m_list.InsertColumn(6, _T("带权周转时间"), LVCFMT_CENTER, rect.Width() / 7, 6);


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CSchedulerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSchedulerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSchedulerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


/*创建进程*/
void CSchedulerDlg::OnBnClickedButtonCreate()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	string process_name(CW2A(m_process_name.GetString()));//CString转string

	//错误输入提示
	if (process_name.empty())
	{
		MessageBox(_T("进程名不能为空！"), _T("错误"), MB_OK | MB_ICONWARNING);
		return;
	}
	if (m_arrive_time < 0)
	{
		MessageBox(_T("进程到达时间必须不能为负数！"), _T("错误"), MB_OK | MB_ICONWARNING);
		return;
	}
	if (m_run_time == 0)
	{
		MessageBox(_T("进程运行时间不能为0！"), _T("错误"), MB_OK | MB_ICONWARNING);
		return;
	}
	if (m_run_time < 0)
	{
		MessageBox(_T("进程运行时间不能为负数！"), _T("错误"), MB_OK | MB_ICONWARNING);
		return;
	}
	if (P.is_repeat(process_name))
	{
		MessageBox(_T("该进程已存在！"), _T("错误"), MB_OK | MB_ICONWARNING);
		return;
	}

	//无错误输入，加入进程
	work w(process_name, m_arrive_time, m_run_time);
	P.add_process(w);

	//在list control空间中显示各进程信息
	int ncount = m_list.GetItemCount(); //获取控件中的列表数量
	for (int i = ncount - 1; i >= 0; --i)
	{
		//清空原有数据
		m_list.DeleteItem(i);
	}
	//显示新的数据
	vector<work> tmp = P.get_allschedule();
	for (int i = 0; i < tmp.size(); i++)
	{
		CString str1(tmp[i].name.c_str());
		CString str2, str3;
		str2.Format(L"%d", tmp[i].arrive_time);
		str3.Format(L"%d", tmp[i].run_time);
		m_list.InsertItem(i, str1);
		m_list.SetItemText(i, 1, str2);
		m_list.SetItemText(i, 2, str3);
	}
	m_list.EnsureVisible(m_list.GetItemCount() - 1, FALSE);
	UpdateData(FALSE);
}

/*先来先服务算法*/
void CSchedulerDlg::OnBnClickedButtonFcfs()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	P.FCFS();
	//在list control空间中显示各进程信息
	int ncount = m_list.GetItemCount(); //获取控件中的列表数量
	for (int i = ncount - 1; i >= 0; --i)
	{
		//清空原有数据
		m_list.DeleteItem(i);
	}
	//显示新的数据
	vector<work> tmp = P.get_allschedule();
	for (int i = 0; i < tmp.size(); i++)
	{
		CString str1(tmp[i].name.c_str());
		CString str2, str3, str4, str5, str6, str7;
		str2.Format(L"%d", tmp[i].arrive_time);
		str3.Format(L"%d", tmp[i].run_time);
		str4.Format(L"%d", tmp[i].start_time);
		str5.Format(L"%d", tmp[i].end_time);
		str6.Format(L"%d", tmp[i].turnaround_time);
		str7.Format(L"%.3lf", tmp[i].weighted_turnaround_time);//最多显示三位小数
		m_list.InsertItem(i, str1);
		m_list.SetItemText(i, 1, str2);
		m_list.SetItemText(i, 2, str3);
		m_list.SetItemText(i, 3, str4);
		m_list.SetItemText(i, 4, str5);
		m_list.SetItemText(i, 5, str6);
		m_list.SetItemText(i, 6, str7);
	}
	m_list.EnsureVisible(m_list.GetItemCount() - 1, FALSE);

	m_ave_turnaround_time = P.get_avg_turnaround();
	m_ave_wturnaround_time = P.get_avg_weightturnaround();
	//设置保留三位小数
	m_ave_turnaround_time = floor(m_ave_turnaround_time * 1000.000f + 0.5) / 1000.000f;
	m_ave_wturnaround_time = floor(m_ave_wturnaround_time * 1000.000f + 0.5) / 1000.000f;


	UpdateData(FALSE);
}

/*非抢占式短作业优先算法*/
void CSchedulerDlg::OnBnClickedButtonSjf1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	P.SJF1();
	//在list control空间中显示各进程信息
	int ncount = m_list.GetItemCount(); //获取控件中的列表数量
	for (int i = ncount - 1; i >= 0; --i)
	{
		//清空原有数据
		m_list.DeleteItem(i);
	}
	//显示新的数据
	vector<work> tmp = P.get_allschedule();
	for (int i = 0; i < tmp.size(); i++)
	{
		CString str1(tmp[i].name.c_str());
		CString str2, str3, str4, str5, str6, str7;
		str2.Format(L"%d", tmp[i].arrive_time);
		str3.Format(L"%d", tmp[i].run_time);
		str4.Format(L"%d", tmp[i].start_time);
		str5.Format(L"%d", tmp[i].end_time);
		str6.Format(L"%d", tmp[i].turnaround_time);
		str7.Format(L"%.3lf", tmp[i].weighted_turnaround_time);//最多显示三位小数
		m_list.InsertItem(i, str1);
		m_list.SetItemText(i, 1, str2);
		m_list.SetItemText(i, 2, str3);
		m_list.SetItemText(i, 3, str4);
		m_list.SetItemText(i, 4, str5);
		m_list.SetItemText(i, 5, str6);
		m_list.SetItemText(i, 6, str7);
	}
	m_list.EnsureVisible(m_list.GetItemCount() - 1, FALSE);

	m_ave_turnaround_time = P.get_avg_turnaround();
	m_ave_wturnaround_time = P.get_avg_weightturnaround();
	//设置保留三位小数
	m_ave_turnaround_time = floor(m_ave_turnaround_time * 1000.000f + 0.5) / 1000.000f;
	m_ave_wturnaround_time = floor(m_ave_wturnaround_time * 1000.000f + 0.5) / 1000.000f;


	UpdateData(FALSE);

}

/*抢占式短作业优先算法*/
//void CSchedulerDlg::OnBnClickedButtonSjf2()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	MessageBox(_T("该算法暂未实现！"), _T("提示"), MB_OK);
//	return;
//	//UpdateData(TRUE);
//	//P.SJF2();
//	////在list control空间中显示各进程信息
//	//int ncount = m_list.GetItemCount(); //获取控件中的列表数量
//	//for (int i = ncount - 1; i >= 0; --i)
//	//{
//	//	//清空原有数据
//	//	m_list.DeleteItem(i);
//	//}
//	////显示新的数据
//	//vector<work> tmp = P.get_allschedule();
//	//for (int i = 0; i < tmp.size(); i++)
//	//{
//	//	CString str1(tmp[i].name.c_str());
//	//	CString str2, str3, str4, str5, str6, str7;
//	//	str2.Format(L"%d", tmp[i].arrive_time);
//	//	str3.Format(L"%d", tmp[i].run_time);
//	//	str4.Format(L"%d", tmp[i].start_time);
//	//	str5.Format(L"%d", tmp[i].end_time);
//	//	str6.Format(L"%d", tmp[i].turnaround_time);
//	//	str7.Format(L"%.3lf", tmp[i].weighted_turnaround_time);//最多显示三位小数
//	//	m_list.InsertItem(i, str1);
//	//	m_list.SetItemText(i, 1, str2);
//	//	m_list.SetItemText(i, 2, str3);
//	//	m_list.SetItemText(i, 3, str4);
//	//	m_list.SetItemText(i, 4, str5);
//	//	m_list.SetItemText(i, 5, str6);
//	//	m_list.SetItemText(i, 6, str7);
//	//}
//	//m_list.EnsureVisible(m_list.GetItemCount() - 1, FALSE);
//
//	//m_ave_turnaround_time = P.get_avg_turnaround();
//	//m_ave_wturnaround_time = P.get_avg_weightturnaround();
//	////设置保留三位小数
//	//m_ave_turnaround_time = floor(m_ave_turnaround_time * 1000.000f + 0.5) / 1000.000f;
//	//m_ave_wturnaround_time = floor(m_ave_wturnaround_time * 1000.000f + 0.5) / 1000.000f;
//
//
//	//UpdateData(FALSE);
//
//}


/*高响应比优先算法*/
void CSchedulerDlg::OnBnClickedButtonHrpf()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	P.HRpF();
	//在list control空间中显示各进程信息
	int ncount = m_list.GetItemCount(); //获取控件中的列表数量
	for (int i = ncount - 1; i >= 0; --i)
	{
		//清空原有数据
		m_list.DeleteItem(i);
	}
	//显示新的数据
	vector<work> tmp = P.get_allschedule();
	for (int i = 0; i < tmp.size(); i++)
	{
		CString str1(tmp[i].name.c_str());
		CString str2, str3, str4, str5, str6, str7;
		str2.Format(L"%d", tmp[i].arrive_time);
		str3.Format(L"%d", tmp[i].run_time);
		str4.Format(L"%d", tmp[i].start_time);
		str5.Format(L"%d", tmp[i].end_time);
		str6.Format(L"%d", tmp[i].turnaround_time);
		str7.Format(L"%.3lf", tmp[i].weighted_turnaround_time);//最多显示三位小数
		m_list.InsertItem(i, str1);
		m_list.SetItemText(i, 1, str2);
		m_list.SetItemText(i, 2, str3);
		m_list.SetItemText(i, 3, str4);
		m_list.SetItemText(i, 4, str5);
		m_list.SetItemText(i, 5, str6);
		m_list.SetItemText(i, 6, str7);
	}
	m_list.EnsureVisible(m_list.GetItemCount() - 1, FALSE);

	m_ave_turnaround_time = P.get_avg_turnaround();
	m_ave_wturnaround_time = P.get_avg_weightturnaround();
	//设置保留三位小数
	m_ave_turnaround_time = floor(m_ave_turnaround_time * 1000.000f + 0.5) / 1000.000f;
	m_ave_wturnaround_time = floor(m_ave_wturnaround_time * 1000.000f + 0.5) / 1000.000f;


	UpdateData(FALSE);

}
