// CLinParameterDlg.cpp: 实现文件
//

#include "pch.h"
#include "MapEditor.h"
#include "CLinParameterDlg.h"
#include "afxdialogex.h"


// CLinParameterDlg 对话框

IMPLEMENT_DYNAMIC(CLinParameterDlg, CDialogEx)

CLinParameterDlg::CLinParameterDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LIN_PARAMETER, pParent)
{
	m_Pattern = 0;//设置线型参数初始值
}

CLinParameterDlg::~CLinParameterDlg()
{
}

void CLinParameterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIN_PATTERN, m_ComboBox);
	DDX_Control(pDX, IDC_LIN_COLOR, m_ColorButton);
}


BEGIN_MESSAGE_MAP(CLinParameterDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_LIN_PATTERN, &CLinParameterDlg::OnSelchangeLinPattern)
END_MESSAGE_MAP()


// CLinParameterDlg 消息处理程序


BOOL CLinParameterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_ComboBox.SetCurSel(m_Pattern); //设置m_ComboBox 的初始值
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CLinParameterDlg::OnSelchangeLinPattern()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Pattern = m_ComboBox.GetCurSel();//设置当前选择的线型
}
