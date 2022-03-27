
// memorizerDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "memorizer.h"
#include "memorizerDlg.h"
#include "afxdialogex.h"
#include<Windows.h>
#include<thread>
#include<vector>

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


// CmemorizerDlg 对话框



CmemorizerDlg::CmemorizerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MEMORIZER_DIALOG, pParent)
	, m_missrateA(0)
	, m_replacerateA(0)
	, m_missrateB(0)
	, m_replacerateB(0)
	, m_missrateC(0)
	, m_replacerateC(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CmemorizerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Text(pDX, ID_WIZNEXT, m_requestA);
	DDX_Text(pDX, IDC_EDIT2, m_missrateA);
	DDX_Text(pDX, IDC_EDIT3, m_replacerateA);
	DDX_Text(pDX, ID_WIZNEXT2, m_requestB);
	DDX_Text(pDX, IDC_EDIT4, m_missrateB);
	DDX_Text(pDX, IDC_EDIT5, m_replacerateB);
	DDX_Text(pDX, ID_WIZNEXT3, m_requestC);
	DDX_Text(pDX, IDC_EDIT6, m_missrateC);
	DDX_Text(pDX, IDC_EDIT7, m_replacerateC);
	DDX_Control(pDX, IDC_COMBO1, m_combobox);
}

BEGIN_MESSAGE_MAP(CmemorizerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CmemorizerDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CmemorizerDlg 消息处理程序

BOOL CmemorizerDlg::OnInitDialog()
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
	m_combobox.SetCurSel(choice);

	CRect rect;
	// 获取编程语言列表视图控件的位置和大小   
	m_list.GetClientRect(&rect);
	// 为列表视图控件添加全行选中和栅格风格   
	m_list.SetExtendedStyle(m_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	//CRect rect;

	//// 获取编程语言列表视图控件的位置和大小   
	//m_list.GetClientRect(&rect);

	//// 为列表视图控件添加全行选中和栅格风格   
	//m_list.SetExtendedStyle(m_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CmemorizerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CmemorizerDlg::OnPaint()
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
HCURSOR CmemorizerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




void CmemorizerDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	string requestA(CW2A(m_requestA.GetString()));
	string requestB(CW2A(m_requestB.GetString()));
	string requestC(CW2A(m_requestC.GetString()));
	if (requestA.size() == 0 || requestB.size() == 0 || requestC.size() == 0)
	{
		MessageBox(_T("页面调度顺序不能为空！"), _T("错误"), MB_OK | MB_ICONWARNING);
		return;
	}

	for (int i = 0; i < requestA.size(); i++)
	{
		if (requestA[i] != '0' && requestA[i] != '1' && requestA[i] != '2' && requestA[i] != '3' && requestA[i] != '4'
			&&requestA[i] != '5' && requestA[i] != '6'&& requestA[i] != '7' && requestA[i] != '8' && requestA[i] != '9'
			&&requestA[i]!=' ')
		{
			MessageBox(_T("页面调度顺序输入错误！\n只能输入正整数和空格！"), _T("错误"), MB_OK | MB_ICONWARNING);
			return;
		}
	}
	for (int i = 0; i < requestB.size(); i++)
	{
		if (requestB[i] != '0' && requestB[i] != '1' && requestB[i] != '2' && requestB[i] != '3' && requestB[i] != '4'
			&& requestB[i] != '5' && requestB[i] != '6' && requestB[i] != '7' && requestB[i] != '8' && requestB[i] != '9'
			&& requestB[i] != ' ')
		{
			MessageBox(_T("页面调度顺序输入错误！\n只能输入正整数和空格！"), _T("错误"), MB_OK | MB_ICONWARNING);
			return;
		}
	}
	for (int i = 0; i < requestC.size(); i++)
	{
		if (requestC[i] != '0' && requestC[i] != '1' && requestC[i] != '2' && requestC[i] != '3' && requestC[i] != '4'
			&& requestC[i] != '5' && requestC[i] != '6' && requestC[i] != '7' && requestC[i] != '8' && requestC[i] != '9'
			&& requestC[i] != ' ')
		{
			MessageBox(_T("页面调度顺序输入错误！\n只能输入正整数和空格！"), _T("错误"), MB_OK | MB_ICONWARNING);
			return;
		}
	}

	multiprocess M;
	if (!M.create(requestA, requestB, requestC))
	{
		MessageBox(_T("页面号只能是1-50的正整数！"), _T("警告"), MB_OK | MB_ICONWARNING);
		return;
	}
	choice = m_combobox.GetCurSel();
	M.run(choice);

	int* a1 = M.get_processA().get_miss();
	int* a2 = M.get_processA().get_replace();
	int* b1 = M.get_processB().get_miss();
	int* b2 = M.get_processB().get_replace();
	int* c1 = M.get_processC().get_miss();
	int* c2 = M.get_processC().get_replace();

	int** table1 = M.get_table(M.get_processA());
	int size1 = M.get_requestsize(M.get_processA());
	int** table2 = M.get_table(M.get_processB());
	int size2 = M.get_requestsize(M.get_processB());
	int** table3 = M.get_table(M.get_processC());
	int size3 = M.get_requestsize(M.get_processC());

	
	//清空表格内容
	m_list.DeleteAllItems();
	while (m_list.DeleteColumn(0));   //删除第一项，删了之后后面的会前移

	m_missrateA = (a1[0] * 1.0) / size1;
	m_replacerateA = (a2[0] * 1.0) / size1;
	m_missrateB = (b1[0] * 1.0) / size2;
	m_replacerateB = (b2[0] * 1.0) / size2;
	m_missrateC = (c1[0] * 1.0) / size3;
	m_replacerateC = (c2[0] * 1.0) / size3;
	
	int max = (size1 > size2 ? size1 : size2) > size3 ? (size1 > size2 ? size1 : size2) : size3;

	Vpage* V1 = M.get_pagerequest(M.get_processA());
	Vpage* V2 = M.get_pagerequest(M.get_processB());
	Vpage* V3 = M.get_pagerequest(M.get_processC());

	CString Str("页面走向");
	CString Str1("缺");
	CString Str2("缺页");
	CString Str3(" ");
	//CString a("A进程"), b("B进程"), c("C进程");


	//A进程
	m_list.InsertColumn(0, Str, LVCFMT_CENTER, 80, 0);
	for (int i = 0; i < max; i++)
	{
		CString str;
		if (V1 != NULL)
		{
			str.Format(L"%d", V1->num);
			m_list.InsertColumn(i + 1, str, LVCFMT_CENTER, 80, i + 1);
			V1 = V1->nextpage;
		}
		else {
			m_list.InsertColumn(i + 1, Str3, LVCFMT_CENTER, 80, i + 1);
		}
	}
	for (int i = 0; i < 4; i++)
	{
		CString str("物理块");
		CString str1;
		str1.Format(L"%d", i+1);
		str = str + str1;
		m_list.InsertItem(i, str);
		for (int j = 0; j < size1; j++)
		{
			CString str2;
			if (table1[i][j] != -1) {
				str2.Format(L"%d", table1[i][j]);
			}
			m_list.SetItemText(i, j + 1, str2);
		}
	}
	V1 = M.get_pagerequest(M.get_processA());
	m_list.InsertItem(4, Str2);
	for (int i = 0; i < size1; i++)
	{
		if (V1->is_missing)
		{
			m_list.SetItemText(4, i + 1, Str1);
		}
		V1 = V1->nextpage;
	}
	m_list.InsertItem(5, Str3);
	m_list.InsertItem(6, Str3);
	//B进程
	m_list.InsertItem(7, Str);
	for (int i = 0; i < size2; i++)
	{
		CString str;
		str.Format(L"%d", V2->num);
		m_list.SetItemText(7, i + 1, str);
		V2 = V2->nextpage;
	}
	for (int i = 0; i < 4; i++)
	{
		CString str("物理块");
		CString str1;
		str1.Format(L"%d", i + 1);
		str = str + str1;
		m_list.InsertItem(i+8, str);
		for (int j = 0; j < size2; j++)
		{
			CString str2;
			if (table2[i][j] != -1) {
				str2.Format(L"%d", table2[i][j]);
			}
			m_list.SetItemText(i+8, j + 1, str2);
		}
	}
	V2 = M.get_pagerequest(M.get_processB());
	m_list.InsertItem(12, Str2);
	for (int i = 0; i < size2; i++)
	{
		if (V2->is_missing)
		{
			m_list.SetItemText(12, i + 1, Str1);
		}
		V2 = V2->nextpage;
	}
	m_list.InsertItem(13, Str3);
	m_list.InsertItem(14, Str3);

	//C进程
	m_list.InsertItem(15, Str);
	for (int i = 0; i < size3; i++)
	{
		CString str;
		str.Format(L"%d", V3->num);
		m_list.SetItemText(15, i + 1, str);
		V3 = V3->nextpage;
	}
	for (int i = 0; i < 4; i++)
	{
		CString str("物理块");
		CString str1;
		str1.Format(L"%d", i + 1);
		str = str + str1;
		m_list.InsertItem(i + 16, str);
		for (int j = 0; j < size3; j++)
		{
			CString str2;
			if (table3[i][j] != -1) {
				str2.Format(L"%d", table3[i][j]);
			}
			m_list.SetItemText(i + 16, j + 1, str2);
		}
	}
	V3 = M.get_pagerequest(M.get_processC());
	m_list.InsertItem(20, Str2);
	for (int i = 0; i < size3; i++)
	{
		if (V3->is_missing)
		{
			m_list.SetItemText(20, i + 1, Str1);
		}
		V3 = V3->nextpage;
	}

	UpdateData(FALSE);
}
