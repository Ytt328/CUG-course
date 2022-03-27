
// SchedulerDlg.h: 头文件
//

#pragma once
#include"process.h"


// CSchedulerDlg 对话框
class CSchedulerDlg : public CDialogEx
{
// 构造
public:
	CSchedulerDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SCHEDULER_DIALOG };
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
	CString m_process_name;//进程名
	int m_arrive_time;//进程到达时间
	int m_run_time;//进程运行时间

	double m_ave_turnaround_time;//平均周转时间
	double m_ave_wturnaround_time;//平均带权周转时间

    CListCtrl m_list;//list control控件

	process P;//进程类对象

	afx_msg void OnBnClickedButtonCreate();//创建进程确定按钮
	afx_msg void OnBnClickedButtonFcfs();//先来先服务算法按钮
	afx_msg void OnBnClickedButtonSjf1();//短作业优先(非抢占)算法按钮
	//afx_msg void OnBnClickedButtonSjf2();//短作业优先(抢占式)算法按钮
	afx_msg void OnBnClickedButtonHrpf();//高响应比优先算法按钮
	
};
