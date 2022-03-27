#pragma once


// CLinParameterDlg 对话框

class CLinParameterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLinParameterDlg)

public:
	CLinParameterDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CLinParameterDlg();
	int m_Pattern;//线型参数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LIN_PARAMETER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CComboBox m_ComboBox;
	CMFCColorButton m_ColorButton;
	afx_msg void OnSelchangeLinPattern();
};
