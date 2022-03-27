// CRegParameterDlg.cpp: 实现文件
//

#include "pch.h"
#include "MapEditor.h"
#include "CRegParameterDlg.h"
#include "afxdialogex.h"


// CRegParameterDlg 对话框

IMPLEMENT_DYNAMIC(CRegParameterDlg, CDialogEx)

CRegParameterDlg::CRegParameterDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_REG_PARAMETER, pParent)
{
	m_Pattern = 0;//设置区型参数初始值
}

CRegParameterDlg::~CRegParameterDlg()
{
}

void CRegParameterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REG_PATTERN, m_ComboBox1);
	DDX_Control(pDX, IDC_REG_COLOR, m_ColorButton);
}


BEGIN_MESSAGE_MAP(CRegParameterDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_REG_PATTERN, &CRegParameterDlg::OnSelchangeRegPattern)
END_MESSAGE_MAP()


// CRegParameterDlg 消息处理程序


BOOL CRegParameterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_ComboBox1.SetCurSel(m_Pattern); //设置m_ComboBox1 的初始值
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CRegParameterDlg::OnSelchangeRegPattern()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Pattern = m_ComboBox1.GetCurSel();//设置当前选择的线型
}
