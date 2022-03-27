
// memorizerDlg.h: 头文件
//

#pragma once
#include"process.h"
#include"multiprocess.h"




// CmemorizerDlg 对话框
class CmemorizerDlg : public CDialogEx
{
// 构造
public:
	CmemorizerDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MEMORIZER_DIALOG };
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
	CListCtrl m_list;
	CString m_requestA;//A进程页面调度顺序
	double m_missrateA;//A进程缺页率
	double m_replacerateA;//A进程置换率
	CString m_requestB;//B进程页面调度顺序
	double m_missrateB;//B进程缺页率
	double m_replacerateB;//B进程置换率
	CString m_requestC;//C进程页面调度顺序
	double m_missrateC;//C进程缺页率
	double m_replacerateC;//C进程置换率
	afx_msg void OnBnClickedButton1();//并发执行

	//置换算法选择
	CComboBox m_combobox;
	int choice;

	//multiprocess Memory;
	HANDLE threada;
	HANDLE threadb;
	HANDLE threadc;
	m_struct a_r;
	m_struct b_r;
	m_struct c_r;
	/*process A;
	process B;
	process C;*/

};
