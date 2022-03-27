// CreateFileDig.cpp: 实现文件
//

#include "pch.h"
#include "MapEditor.h"
#include "CreateFileDig.h"
#include "afxdialogex.h"


// CCreateFileDig 对话框

IMPLEMENT_DYNAMIC(CCreateFileDig, CDialogEx)

CCreateFileDig::CCreateFileDig(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CREATE_FILE, pParent)
	, m_add(_T(""))
{
	HMODULE module = GetModuleHandle(0);
	char* pFileName = new char[MAX_PATH];
	GetModuleFileName(module, LPWSTR(pFileName), MAX_PATH);
	m_add.Format(_T( "%s"), pFileName);
	int nPos = m_add.ReverseFind(_T('\\'));
	if (nPos < 0)
	{
		m_add = "";
	}
	else
	{
		m_add = m_add.Left(nPos);// 得到.EXE 的文件地址
	}
	delete[] pFileName;
}

CCreateFileDig::~CCreateFileDig()
{
}

void CCreateFileDig::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CREATE_FILE_ADDRESS, m_add);
}


BEGIN_MESSAGE_MAP(CCreateFileDig, CDialogEx)
	ON_BN_CLICKED(IDC_CREATE_FILE_CHANGE_ADDRESS_BTN, &CCreateFileDig::OnBnClickedCreateFileChangeAddressBtn)
END_MESSAGE_MAP()


// CCreateFileDig 消息处理程序


/*路径更改*/
void CCreateFileDig::OnBnClickedCreateFileChangeAddressBtn()
{
	// TODO: 在此添加控件通知处理程序代码
	BROWSEINFO bInfo;
	ZeroMemory(&bInfo, sizeof(bInfo));
	bInfo.hwndOwner = GetSafeHwnd();
	bInfo.lpszTitle = _T("请选择临时文件存放的路径:");
	bInfo.ulFlags = BIF_RETURNONLYFSDIRS;
	LPITEMIDLIST lpDlist; //用来保存返回信息的IDList
	lpDlist = SHBrowseForFolder(&bInfo); //显示选择对话框
	if (lpDlist != NULL)//用户按了确定按钮
	{
		TCHAR chPath[MAX_PATH]; //用来存储路径的字符串
		SHGetPathFromIDList(lpDlist, chPath); //把项目标识列表转化成字符串
		m_add = chPath; //将TC HAR 类型字符串转换为CString 类型字符串
		UpdateData(FALSE); //将变量值更新到控件
	}
}
