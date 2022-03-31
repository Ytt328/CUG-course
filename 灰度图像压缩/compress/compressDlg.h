
// compressDlg.h: 头文件
//

#pragma once

#include"CCompressImage.h"

#define WM_UPDATE_STATIC (WM_USER + 100)


// CcompressDlg 对话框
class CcompressDlg : public CDialogEx
{
// 构造
public:
	CcompressDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COMPRESS_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CCompressImage compressor;
	CString Path;
	afx_msg void OnBnClickedButtonView();
	
	afx_msg void OnBnClickedButtonClear();
	
	afx_msg void OnBnClickedButtonCompress();
	afx_msg void OnBnClickedButtonUncompress();
	CEdit m_Edit;
	CString m_detail;
	static CString detail;
	CProgressCtrl m_processctrl;

	static UINT COMPRESS(LPVOID lpParam);
	static UINT UNCOMPRESS(LPVOID lpParam);
protected:
	afx_msg LRESULT OnUpdateStatic(WPARAM wParam, LPARAM lParam);
};


struct mystruct1 {
	CCompressImage compressor;
	CString Path;
	CString m_detail;
	CEdit m_Edit;
};