
// compressDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "compress.h"
#include "compressDlg.h"
#include "afxdialogex.h"
#include"CCompressImage.h"

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
public:
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


// CcompressDlg 对话框


CString CcompressDlg::detail= (CString)"";

CcompressDlg::CcompressDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_COMPRESS_DIALOG, pParent)
	, Path(_T(""))
	, m_detail(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CcompressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PATH, Path);
	DDX_Control(pDX, IDC_EDIT_DETAIL, m_Edit);
	DDX_Text(pDX, IDC_EDIT_DETAIL, m_detail);
	DDX_Control(pDX, IDC_PROGRESS1, m_processctrl);
}

BEGIN_MESSAGE_MAP(CcompressDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_VIEW, &CcompressDlg::OnBnClickedButtonView)
	ON_BN_CLICKED(IDC_BUTTON_Clear, &CcompressDlg::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_Compress, &CcompressDlg::OnBnClickedButtonCompress)
	ON_BN_CLICKED(IDC_BUTTON_Uncompress, &CcompressDlg::OnBnClickedButtonUncompress)
	ON_MESSAGE(WM_UPDATE_STATIC, &CcompressDlg::OnUpdateStatic)
END_MESSAGE_MAP()


// CcompressDlg 消息处理程序

BOOL CcompressDlg::OnInitDialog()
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
	m_processctrl.SetRange(0, 100);
	m_processctrl.SetStep(1);
	m_processctrl.SetPos(0);


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CcompressDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CcompressDlg::OnPaint()
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
HCURSOR CcompressDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//浏览选择文件
void CcompressDlg::OnBnClickedButtonView()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CFileDialog dlg(true);
	dlg.m_ofn.lpstrFilter = L"bmp\0 * .bmp\0""cbmp\0 * .cbmp\0";
	if (dlg.DoModal() == IDOK) {
		Path = dlg.m_ofn.lpstrFile;
	}
	UpdateData(FALSE);
}

//清空内容
void CcompressDlg::OnBnClickedButtonClear()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Edit.SetWindowTextW(L"");
}

UINT CcompressDlg::COMPRESS(LPVOID lpParameter)
{
	CcompressDlg* dig = (CcompressDlg*)lpParameter;
	dig->compressor.setpath(dig->Path);
	if (!dig->compressor.ReadBitmap()) {
		//MessageBox(_T("读取位图数据失败！"), _T("错误"), MB_OK | MB_ICONWARNING);
		return 0;
	}

	int size = dig->compressor.getsize();
	CString str("位图数据读取成功!\r\n压缩前位图大小：");

	CString s;
	s.Format(_T("%d字节\r\n"), size);
	str = str + s;
	CString str1("压缩中......\r\n");
	str = str + str1;
	CString str2("压缩成功！\r\n压缩后位图大小：");
	detail = detail.IsEmpty() ? str : detail + "\r\n" + str;
	::SendMessage(dig->m_hWnd, WM_UPDATE_STATIC, 0, 0);//发信息给窗口，实时显示进程状态
	dig->m_processctrl.SetStep(0.000000001);
	dig->m_processctrl.SetPos(80);
	if (dig->compressor.Compress()) {

		size = dig->compressor.getcompresssize();
		dig->m_processctrl.SetStep(10);
		dig->m_processctrl.SetPos(100);
		s.Format(_T("%d字节\r\n"), size);
		str2 = str2 + s;
		detail = detail + str2;
		::SendMessage(dig->m_hWnd, WM_UPDATE_STATIC, 0, 0);//发信息给窗口，实时显示进程状态

	}
	CString str3("压缩文件路径：");
	detail = detail + str3 + dig->compressor.getcpath() + CString("\r\n");
	::SendMessage(dig->m_hWnd,WM_UPDATE_STATIC, 0, 0);//发信息给窗口，实时显示进程状态
	return 0;
}

void CcompressDlg::OnBnClickedButtonCompress()
{
	// TODO: 在此添加控件通知处理程序代码
	//UpdateData(TRUE);
	m_processctrl.SetPos(0);
	if (Path.IsEmpty()) {
		MessageBox(_T("路径名不能为空！"), _T("错误"), MB_OK | MB_ICONWARNING);
		return;
	}
	CString firstFileSuffix = Path.Right(Path.GetLength() - Path.ReverseFind('.') - 1);
	if (firstFileSuffix != "bmp") {
		MessageBox(_T("压缩操作仅支持bmp文件！"), _T("警告"), MB_OK | MB_ICONWARNING);
		return;
	}
	//创建一个线程
	CWinThread* mythread = AfxBeginThread( COMPRESS,this,THREAD_PRIORITY_NORMAL,0,0,NULL);
	m_Edit.LineScroll(m_Edit.GetLineCount());//自动滚到最下面一行
}


UINT CcompressDlg::UNCOMPRESS(LPVOID lpParameter)
{
	CcompressDlg* dig = (CcompressDlg*)lpParameter;
	dig->compressor.setpath(dig->Path);
	if (dig->compressor.UnCompress()) {
        int size = dig->compressor.getbefore();
		CString str("解压前大小：");
		CString s;
		s.Format(_T("%d字节\r\n"), size);
		str = str + s;
		CString str1("解压中......\r\n");
		str = str + str1;
		detail = detail.IsEmpty() ? str : detail + "\r\n" + str;
		::SendMessage(dig->m_hWnd, WM_UPDATE_STATIC, 0, 0);//发信息给窗口，实时显示进程状态
		dig->m_processctrl.SetStep(0.001);
		dig->m_processctrl.SetPos(80);
		CString str2("解压成功！\r\n解压后位图大小：");
		dig->compressor.WriteBitmap();
		dig->m_processctrl.SetStep(10);
		dig->m_processctrl.SetPos(100);
		size = dig->compressor.getafter();
		s.Format(_T("%d字节\r\n"), size);
		str2 = str2 + s;
		detail = detail + str2;
		CString str3("解压文件路径：");
		detail = detail + str3 + dig->compressor.getupath() + CString("\r\n");
		::SendMessage(dig->m_hWnd, WM_UPDATE_STATIC, 0, 0);//发信息给窗口，实时显示进程状态
	}
	else {
		//MessageBox(_T("解压失败！"), _T("错误"), MB_OK | MB_ICONWARNING);
		return 0;
	}
	return 0;
}

void CcompressDlg::OnBnClickedButtonUncompress()
{
	// TODO: 在此添加控件通知处理程序代码
	//UpdateData(TRUE);
	m_processctrl.SetPos(0);
	if (Path.IsEmpty()) {
		MessageBox(_T("路径名不能为空！"), _T("错误"), MB_OK | MB_ICONWARNING);
		return;
	}
	CString firstFileSuffix = Path.Right(Path.GetLength() - Path.ReverseFind('.') - 1);
	if (firstFileSuffix != "cbmp") {
		MessageBox(_T("解压操作仅支持cbmp文件！"), _T("警告"), MB_OK | MB_ICONWARNING);
		return;
	}
	//创建线程
	CWinThread* mythread = AfxBeginThread(UNCOMPRESS, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	m_Edit.LineScroll(m_Edit.GetLineCount());//自动滚到最下面一行
	
}


afx_msg LRESULT CcompressDlg::OnUpdateStatic(WPARAM wParam, LPARAM lParam)
{
	GetDlgItem(IDC_EDIT_DETAIL)->SetWindowTextW(detail);//更新窗口内容
	m_Edit.LineScroll(m_Edit.GetLineCount());//自动滚到最下面一行
	return 0;
}
