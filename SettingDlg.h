#pragma once
#include "resource.h"
#include <dwmapi.h> 
#pragma comment(lib, "dwmapi.lib") 

#define TRANSPARENT_COLOR RGB (200, 201, 202)

// CSettingDlg 对话框

class CSettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSettingDlg)

private:
	CMFCButton m_btnOK;
	CMFCButton m_btnCancel;
	//HACCEL m_hAccel;

public:
	CSettingDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSettingDlg();

// 对话框数据
	enum { IDD = IDD_DLG_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	//BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()

public:
	UINT m_length;//仿真时长
	UINT m_times;//仿真次数
	int m_filters;//仿真算法
	int m_figure;//所画图形
//	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
