
// SudokuDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "Sudoku.h"
#include "SudokuDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define IDC_D_BTN 10000


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


// CSudokuDlg 对话框



CSudokuDlg::CSudokuDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SUDOKU_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSudokuDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO, m_combobox);
	DDX_Control(pDX, IDC_BUTTON_BACK, m_Button_back);
	DDX_Control(pDX, IDC_BUTTON_TIPS, m_Button_tips);
}

BEGIN_MESSAGE_MAP(CSudokuDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND_RANGE(IDC_D_BTN, IDC_D_BTN+80, OnButtonClick)
	ON_CBN_SELCHANGE(IDC_COMBO, &CSudokuDlg::OnCbnSelchangeCombo)
	ON_BN_CLICKED(IDC_BUTTON_SUBMIT, &CSudokuDlg::OnBnClickedButtonSubmit)
	ON_BN_CLICKED(IDC_BUTTON_BACK, &CSudokuDlg::OnBnClickedButtonBack)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_PAUSE, &CSudokuDlg::OnBnClickedButtonPause)
	ON_BN_CLICKED(IDC_BUTTON_TIPS, &CSudokuDlg::OnBnClickedButtonTips)
	ON_BN_CLICKED(IDC_BUTTON_RESTART, &CSudokuDlg::OnBnClickedButtonRestart)
END_MESSAGE_MAP()


// CSudokuDlg 消息处理程序

BOOL CSudokuDlg::OnInitDialog()
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

	ShowWindow(SW_SHOW);

	// TODO: 在此添加额外的初始化代码
	m_combobox.SetCurSel(0);//默认选择“简单”难度
	prechoice = 0;//上一选择为“简单”
	//动态创建按钮并设置位置和大小
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++)
		{
			mapBtn[i][j].Create(_T(""), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(
				40 + j * 60,
				20 + i * 60,
				100 + j * 60,
				80 + i * 60), this, IDC_D_BTN + i * 9 + j);//创建按钮
			mapBtn[i][j].m_bDontUseWinXPTheme = TRUE;
			mapBtn[i][j].SetFaceColor(RGB(255, 255, 255));
			if (easy[i][j] != 0) {
				mapBtn[i][j].EnableWindow(false);
				CString Str;
				Str.Format(_T("%d"), easy[i][j]);//int转CString
				mapBtn[i][j].SetWindowTextW(Str);
				mapBtn[i][j].SetFaceColor(RGB(255, 255, 0));
			}
			ans[i][j] = easy[i][j];//初始化用户输入的答案
		}
	}
	//刚开始撤销和提示按钮都不可点击
	m_Button_back.EnableWindow(false);
	m_Button_tips.EnableWindow(false);
	//SetTimer(1, 100, NULL);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CSudokuDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSudokuDlg::OnPaint()
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
HCURSOR CSudokuDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//点击数独区域部分的按钮
void CSudokuDlg::OnButtonClick(UINT uID) {
	//上一次点击按钮恢复成原来的颜色
	if (pre_id != 0) {
		CMFCButton* pre = (CMFCButton*)GetDlgItem(pre_id);
		CString str;
		pre->GetWindowTextW(str);
		if (str.IsEmpty()) {
			pre->SetFaceColor(RGB(255, 255, 255));
		}
		else {
			pre->SetFaceColor(RGB(204, 255, 153));
		}
	}
	//设置当前点击的按钮的颜色
	CMFCButton* b = (CMFCButton*)GetDlgItem(uID);
	b->SetFaceColor(RGB(193, 210, 240));
	pre_id = uID;//更新pre_id
	m_Button_tips.EnableWindow(true);
}


//从键盘输入值
BOOL CSudokuDlg::PreTranslateMessage(MSG* pMsg)
{
	UINT nID = GetWindowLong(pMsg->hwnd, GWL_ID);//获取点击按钮的ID
	if (nID>= IDC_D_BTN&&nID<=IDC_D_BTN+80) {//点击的是九宫格区域的按钮
		if (pMsg->message == WM_CHAR) {
			int c = (int)pMsg->wParam - 48;
			if (c > 0 && c <= 9) {//输入1-9间的数字才响应
				CMFCButton* but = (CMFCButton*)GetDlgItem(nID);
				CString before;
				but->GetWindowTextW(before);
				if (_ttoi(before) != c) {//将这一步存到allaction中，若前后内容相同则不加入
					action Step(nID, _ttoi(before));
					if (allaction.size() < 50) {//未到50步，直接加入
						allaction.push_back(Step);
						m_Button_back.EnableWindow(true);
					}
					else {//已经存储了50步，将最先加入的那一步删除再加入
						allaction.erase(allaction.begin());
						allaction.push_back(Step);
						m_Button_back.EnableWindow(true);
					}
				}
				CString Str;
				Str.Format(_T("%d"), c);
				but->SetWindowTextW(Str);//设置按钮内容
				int row = (nID - IDC_D_BTN) / 9;
				int col = (nID - IDC_D_BTN) % 9;
				ans[row][col] = c;//响应到ans
				but->SetFaceColor(RGB(204, 255, 153));//设置颜色
			}
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


//改变难度
void CSudokuDlg::OnCbnSelchangeCombo()
{
	// 确定改变难度
	if (MessageBox(_T("确定要改变难度吗？\n确定后将立即生效"), _T("提示"), MB_YESNO|MB_ICONINFORMATION)==IDYES) {
		int choice = m_combobox.GetCurSel();
		prechoice = choice;
		allaction.clear();
		//简单
		if (choice == 0) {
			for (int i = 0; i < 9; i++) {
				for (int j = 0; j < 9; j++) {
					mapBtn[i][j].EnableWindow(true);
					mapBtn[i][j].SetWindowTextW(L"");
					mapBtn[i][j].SetFaceColor(RGB(255, 255, 255));
					if (easy[i][j] != 0) {
						mapBtn[i][j].EnableWindow(false);
						CString Str;
						Str.Format(_T("%d"), easy[i][j]);
						mapBtn[i][j].SetWindowTextW(Str);
						mapBtn[i][j].SetFaceColor(RGB(255, 255, 0));
					}
					ans[i][j] = easy[i][j];
				}
			}
			m_Button_back.EnableWindow(false);
			m_Button_tips.EnableWindow(false);
		}
		//中等
		else if (choice == 1) {
			for (int i = 0; i < 9; i++) {
				for (int j = 0; j < 9; j++) {
					mapBtn[i][j].EnableWindow(true);
					mapBtn[i][j].SetWindowTextW(L"");
					mapBtn[i][j].SetFaceColor(RGB(255, 255, 255));
					if (middle[i][j] != 0) {
						mapBtn[i][j].EnableWindow(false);
						CString Str;
						Str.Format(_T("%d"), middle[i][j]);
						mapBtn[i][j].SetWindowTextW(Str);
						mapBtn[i][j].SetFaceColor(RGB(255, 255, 0));
					}
					ans[i][j] = middle[i][j];
				}
			}
			m_Button_back.EnableWindow(false);
			m_Button_tips.EnableWindow(false);
		}
		//困难
		else if(choice==2){
			for (int i = 0; i < 9; i++) {
				for (int j = 0; j < 9; j++) {
					mapBtn[i][j].EnableWindow(true);
					mapBtn[i][j].SetWindowTextW(L"");
					mapBtn[i][j].SetFaceColor(RGB(255, 255, 255));
					if (hard[i][j] != 0) {
						mapBtn[i][j].EnableWindow(false);
						CString Str;
						Str.Format(_T("%d"), hard[i][j]);
						mapBtn[i][j].SetWindowTextW(Str);
						mapBtn[i][j].SetFaceColor(RGB(255, 255, 0));
					}
					ans[i][j] = hard[i][j];
				}
			}
			m_Button_back.EnableWindow(false);
			m_Button_tips.EnableWindow(false);
		}
		//终极
		else {
			for (int i = 0; i < 9; i++) {
				for (int j = 0; j < 9; j++) {
					mapBtn[i][j].EnableWindow(true);
					mapBtn[i][j].SetWindowTextW(L"");
					mapBtn[i][j].SetFaceColor(RGB(255, 255, 255));
					if (hardest[i][j] != 0) {
						mapBtn[i][j].EnableWindow(false);
						CString Str;
						Str.Format(_T("%d"), hardest[i][j]);
						mapBtn[i][j].SetWindowTextW(Str);
						mapBtn[i][j].SetFaceColor(RGB(255, 255, 0));
					}
					ans[i][j] = hardest[i][j];
				}
			}
			m_Button_back.EnableWindow(false);
			m_Button_tips.EnableWindow(false);
		}
	}
	//取消改变难度
	else {
		m_combobox.SetCurSel(prechoice);//难度不变
	}
}


//提交答案
void CSudokuDlg::OnBnClickedButtonSubmit()
{
	int issuccess[9]={ 0 };//代表每一行，每一列，每一宫是否含有1-9
	//检查行
	for (int i = 0; i < 9; i++) {
		memset(issuccess, 0, sizeof(issuccess));
		for (int j = 0; j < 9; j++) {
			if (ans[i][j] == 0) {
				MessageBox(_T("很遗憾，你的答案错了！"), _T("提示"), MB_OK | MB_ICONEXCLAMATION);
				return;
			}
			if (!issuccess[ans[i][j]-1]) {
				issuccess[ans[i][j] - 1] = 1;
			}
			else {
				MessageBox(_T("很遗憾，你的答案错了！"), _T("提示"), MB_OK | MB_ICONEXCLAMATION);
				return;
			}
		}
	}
	//检查列
	for (int i = 0; i < 9; i++) {
		memset(issuccess, 0, sizeof(issuccess));
		for (int j = 0; j < 9; j++) {
			if (ans[j][i] == 0) {
				MessageBox(_T("很遗憾，你的答案错了！"), _T("提示"), MB_OK | MB_ICONEXCLAMATION);
				return;
			}
			if (!issuccess[ans[j][i] - 1]) {
				issuccess[ans[j][i] - 1] = 1;
			}
			else {
				MessageBox(_T("很遗憾，你的答案错了！"), _T("提示"), MB_OK | MB_ICONEXCLAMATION);
				return;
			}
		}
	}
	//检查宫
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			memset(issuccess, 0, sizeof(issuccess));
			for (int tmpi = i * 3; tmpi < i * 3 + 3; tmpi++) {
				for (int tmpj = j * 3; tmpj < j * 3 + 3; tmpj++) {
					if (ans[tmpi][tmpj] == 0) {
						MessageBox(_T("很遗憾，你的答案错了！"), _T("提示"), MB_OK | MB_ICONEXCLAMATION);
						return;
					}
					if (!issuccess[ans[tmpi][tmpj] - 1]) {
						issuccess[ans[tmpi][tmpj] - 1] = 1;
					}
					else {
						MessageBox(_T("很遗憾，你的答案错了！"), _T("提示"), MB_OK | MB_ICONEXCLAMATION);
						return;
					}
				}
			}
		}
	}
	MessageBox(_T("恭喜你！答对了！"), _T("提示"), MB_OK|MB_ICONINFORMATION);
}


//撤销
void CSudokuDlg::OnBnClickedButtonBack()
{
	//已记录的步骤不为空才能进行撤销
	if (!allaction.empty()) {
		action back = allaction.back();
		allaction.pop_back();
		if (allaction.size() == 0) {
			m_Button_back.EnableWindow(false);//记录的步骤为空时设置按钮为不可点击
		}
		CString Str;
		Str.Format(_T("%d"), back.pre_value);
		ans[(back.ID - IDC_D_BTN) / 9][(back.ID - IDC_D_BTN) % 9] = _ttoi(Str);//ans的内容回到上一步的状态
		//按钮颜色以及显示内容变化
		if (_ttoi(Str) == 0) {
			GetDlgItem(back.ID)->SetWindowTextW(L"");
			CMFCButton* but = (CMFCButton*)GetDlgItem(back.ID);
			but->SetFaceColor(RGB(255, 255, 255));
		}
		else {
			GetDlgItem(back.ID)->SetWindowTextW(Str);
		}
	}
	if (pre_id != 0) {//更改pre_id,即上一次点击的格子的id
		CMFCButton* pre = (CMFCButton*)GetDlgItem(pre_id);
		CString str;
		pre->GetWindowTextW(str);
		if (str.IsEmpty()) {
			pre->SetFaceColor(RGB(255, 255, 255));
		}
		else {
			pre->SetFaceColor(RGB(204, 255, 153));
		}
		pre_id = 0;
		m_Button_tips.EnableWindow(false);
	}
}


//void CSudokuDlg::OnTimer(UINT_PTR nIDEvent)
//{
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//	
//	CDialogEx::OnTimer(nIDEvent);
//}

//暂停/继续
void CSudokuDlg::OnBnClickedButtonPause()
{
	CString Str;
	GetDlgItem(IDC_BUTTON_PAUSE)->GetWindowTextW(Str);
	//点击“暂停”后显示“继续”，再次点击显示“暂停”
	//停止计时暂未实现
	if (Str == "暂停") {
		GetDlgItem(IDC_BUTTON_PAUSE)->SetWindowTextW(L"继续");
	}
	else {
		GetDlgItem(IDC_BUTTON_PAUSE)->SetWindowTextW(L"暂停");
	}
	//设置pre_id
	if (pre_id != 0) {
		CMFCButton* pre = (CMFCButton*)GetDlgItem(pre_id);
		CString str;
		pre->GetWindowTextW(str);
		if (str.IsEmpty()) {
			pre->SetFaceColor(RGB(255, 255, 255));
		}
		else {
			pre->SetFaceColor(RGB(204, 255, 153));
		}
		pre_id = 0;
		m_Button_tips.EnableWindow(false);
	}
}


//提示
void CSudokuDlg::OnBnClickedButtonTips()
{
	memset(Ans, 0, sizeof(Ans));//设置Ans内容全为0
	backtrace(0);//回溯进行求解
	CMFCButton* but = (CMFCButton*)GetDlgItem(pre_id);
	int row = (pre_id - IDC_D_BTN) / 9;
	int col = (pre_id - IDC_D_BTN) % 9;
	if (Ans[row][col] != 0) {//有解，当前点击的格子获得一个提示答案
		CString Str;
		Str.Format(_T("%d"), Ans[row][col]);
		but->SetWindowTextW(Str);
		but->SetFaceColor(RGB(204, 255, 153));
		ans[row][col] = Ans[row][col];
	}
	else {//无解，弹出错误提示
		MessageBox(_T("当前已无解！请尝试修改输入"), _T("提示"),MB_OK|MB_ICONWARNING);

	}
	pre_id = 0;
	m_Button_tips.EnableWindow(false);
}

//重新开始
void CSudokuDlg::OnBnClickedButtonRestart()
{
	if (MessageBox(_T("确定要重新开始吗？"), _T("提示"), MB_YESNO | MB_ICONINFORMATION) == IDYES) {
		int choice = m_combobox.GetCurSel();
		prechoice = choice;
		pre_id = 0;
		allaction.clear();
		//简单
		if (choice == 0) {
			for (int i = 0; i < 9; i++) {
				for (int j = 0; j < 9; j++) {
					mapBtn[i][j].EnableWindow(true);
					mapBtn[i][j].SetWindowTextW(L"");
					mapBtn[i][j].SetFaceColor(RGB(255, 255, 255));
					if (easy[i][j] != 0) {
						mapBtn[i][j].EnableWindow(false);
						CString Str;
						Str.Format(_T("%d"), easy[i][j]);
						mapBtn[i][j].SetWindowTextW(Str);
						mapBtn[i][j].SetFaceColor(RGB(255, 255, 0));
					}
					ans[i][j] = easy[i][j];
				}
			}
			m_Button_back.EnableWindow(false);
			m_Button_tips.EnableWindow(false);
		}
		//中等
		else if (choice == 1) {
			for (int i = 0; i < 9; i++) {
				for (int j = 0; j < 9; j++) {
					mapBtn[i][j].EnableWindow(true);
					mapBtn[i][j].SetWindowTextW(L"");
					mapBtn[i][j].SetFaceColor(RGB(255, 255, 255));
					if (middle[i][j] != 0) {
						mapBtn[i][j].EnableWindow(false);
						CString Str;
						Str.Format(_T("%d"), middle[i][j]);
						mapBtn[i][j].SetWindowTextW(Str);
						mapBtn[i][j].SetFaceColor(RGB(255, 255, 0));
					}
					ans[i][j] = middle[i][j];
				}
			}
			m_Button_back.EnableWindow(false);
			m_Button_tips.EnableWindow(false);
		}
		//困难
		else if (choice == 2) {
			for (int i = 0; i < 9; i++) {
				for (int j = 0; j < 9; j++) {
					mapBtn[i][j].EnableWindow(true);
					mapBtn[i][j].SetWindowTextW(L"");
					mapBtn[i][j].SetFaceColor(RGB(255, 255, 255));
					if (hard[i][j] != 0) {
						mapBtn[i][j].EnableWindow(false);
						CString Str;
						Str.Format(_T("%d"), hard[i][j]);
						mapBtn[i][j].SetWindowTextW(Str);
						mapBtn[i][j].SetFaceColor(RGB(255, 255, 0));
					}
					ans[i][j] = hard[i][j];
				}
			}
			m_Button_back.EnableWindow(false);
			m_Button_tips.EnableWindow(false);
		}
		//终极
		else {
			for (int i = 0; i < 9; i++) {
				for (int j = 0; j < 9; j++) {
					mapBtn[i][j].EnableWindow(true);
					mapBtn[i][j].SetWindowTextW(L"");
					mapBtn[i][j].SetFaceColor(RGB(255, 255, 255));
					if (hardest[i][j] != 0) {
						mapBtn[i][j].EnableWindow(false);
						CString Str;
						Str.Format(_T("%d"), hardest[i][j]);
						mapBtn[i][j].SetWindowTextW(Str);
						mapBtn[i][j].SetFaceColor(RGB(255, 255, 0));
					}
					ans[i][j] = hardest[i][j];
				}
			}
			m_Button_back.EnableWindow(false);
			m_Button_tips.EnableWindow(false);
		}
	}
}

//回溯
void CSudokuDlg::backtrace(int count) {
	if (count == 81)
	{
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++) {
				Ans[i][j] = ans[i][j];
			}
		}
		return;
	}
	int row = count / 9;
	int col = count % 9;
	if (ans[row][col] == 0)//如果该位置为0，
	{
		for (int i = 1; i <= 9; ++i)
		{
			ans[row][col] = i;//将1-9填入该位置
			if (isplace(count)) //判断填入的数时候能够放入
			{
				backtrace(count + 1);//如果可以放入，则对下一个位置进行操作
			}
		}
		ans[row][col] = 0;
	}
	else//如果该位置不为0，则直接对下个位置的进行操作
	{
		if (isplace(count)) {
			backtrace(count + 1);
		}
		else {	return;}
	}
}

//判断填入的数字是否合理
bool CSudokuDlg::isplace(int count) {
	int row = count / 9;
	int col = count % 9;
	int j;
	//判断填入的数是否在该行
	for (j = 0; j < 9; ++j)
	{
		if (ans[row][j] == ans[row][col] && j != col)
		{
			return false;
		}
	}
	//判断填入的数是否在该列
	for (j = 0; j < 9; ++j)
	{
		if (ans[j][col] == ans[row][col] && j != row)
		{
			return false;
		}
	}
	//判断填入的数是否在3*3的小框中
	int tempRow = row / 3 * 3;
	int tempCol = col / 3 * 3;
	for (j = tempRow; j < tempRow + 3; ++j)
	{
		for (int k = tempCol; k < tempCol + 3; ++k)

		{
			if (ans[j][k] == ans[row][col] && j != row && k != col)
			{
				return false;
			}
		}
	}
	return true;
}