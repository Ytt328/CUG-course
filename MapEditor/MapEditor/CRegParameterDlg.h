#pragma once


// CRegParameterDlg 对话框

class CRegParameterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRegParameterDlg)

public:
	CRegParameterDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CRegParameterDlg();
	int m_Pattern;//区型参数
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_REG_PARAMETER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_ComboBox1;
	CMFCColorButton m_ColorButton;
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeRegPattern();
};
