
// SudokuDlg.h: 头文件
//

#pragma once

#include <vector>

struct action
{
	UINT ID;//点击按钮的ID
	int pre_value;//上次的值
	action(UINT id,int p) {
		ID = id;
		pre_value = p;
	}
};


// CSudokuDlg 对话框
class CSudokuDlg : public CDialogEx
{
// 构造
public:
	CSudokuDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SUDOKU_DIALOG };
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
	afx_msg void OnButtonClick(UINT uID);
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	CMFCButton mapBtn[9][9];//用动态创建按钮模拟九宫格

	CComboBox m_combobox;//难度选择下拉框
	int prechoice = 0;//上一选择
	int pre_id = 0;//上一选择的格子
	int ans[9][9];//用户输入的答案
	int Ans[9][9];//提示的答案
	CButton m_Button_back;//撤销按钮
	CButton m_Button_tips;//提示按钮
	std::vector<action> allaction;//存储步骤，用于撤销
	//简单难度初始数据
	int easy[9][9] = { 9,8,5,1,0,0,2,0,0,
					  0,2,0,5,0,6,3,8,4,
					  4,3,0,2,0,7,0,0,9,
					  8,0,2,0,0,0,0,4,0,
					  6,9,4,3,1,0,0,0,7,
					  0,7,0,0,0,4,9,0,0,
					  0,0,8,4,0,0,7,9,0,
					  3,0,0,0,0,0,8,0,5,
					  0,0,0,8,0,0,4,3,0 };
	//中等难度初始数据
	int middle[9][9] = { 0,5,0,0,6,4,7,3,2,
						0,7,0,5,0,0,4,0,8,
						4,0,0,0,8,0,0,6,0,
						0,0,0,0,2,9,0,5,0,
						2,0,4,0,0,7,1,9,0,
						0,0,5,3,0,0,0,0,7,
						0,0,0,0,0,0,0,7,0,
						7,0,0,0,0,0,9,4,6,
						6,0,0,0,0,0,8,0,5 };
	//困难难度初始数据
	int hard[9][9] = { 0,8,6,9,0,0,1,7,0,
					  0,0,0,0,0,0,0,0,0,
					  0,4,0,8,0,3,0,0,2,
					  0,5,0,0,0,8,0,0,9,
					  0,0,2,0,6,0,3,0,0,
					  9,0,0,1,0,0,0,4,0,
					  2,0,0,4,0,6,0,9,0,
					  0,0,0,0,0,0,0,0,0,
					  0,6,1,0,0,7,8,2,0 };
	//终极难度初始数据
	int hardest[9][9] = { 8,0,0,0,0,0,0,0,0,
					 0,0,3,6,0,0,0,0,0,
					 0,7,0,0,9,0,2,0,0,
					 0,5,0,0,0,7,0,0,0,
					 0,0,0,0,4,5,7,0,0,
					 0,0,0,1,0,0,0,3,0,
					 0,0,1,0,0,0,0,6,8,
					 0,0,8,5,0,0,0,1,0,
					 0,9,0,0,0,0,4,0,0 };
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);//从键盘输入值
	afx_msg void OnCbnSelchangeCombo();//改变难度
	afx_msg void OnBnClickedButtonSubmit();//提交
	
	afx_msg void OnBnClickedButtonBack();//撤销
	//afx_msg void OnTimer(UINT_PTR nIDEvent);//计时
	afx_msg void OnBnClickedButtonPause();//暂停/继续
	afx_msg void OnBnClickedButtonTips();//提示
	afx_msg void OnBnClickedButtonRestart();//重新开始

    void backtrace(int count);//回溯
	bool isplace(int count);//判断填入是否合理
};
