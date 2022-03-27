#pragma once


// CCreateFileDig 对话框

class CCreateFileDig : public CDialogEx
{
	DECLARE_DYNAMIC(CCreateFileDig)

public:
	CCreateFileDig(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CCreateFileDig();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CREATE_FILE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_add;
	afx_msg void OnBnClickedCreateFileChangeAddressBtn();
};
