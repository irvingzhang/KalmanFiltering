// SettingDlg.cpp : 实现文件
//

#pragma once
#include "stdafx.h"
#include "Nonlinear_Filtering.h"
#include "SettingDlg.h"
#include "afxdialogex.h"


// CSettingDlg 对话框

IMPLEMENT_DYNAMIC(CSettingDlg, CDialogEx)

CSettingDlg::CSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSettingDlg::IDD, pParent)
{

	m_length = 25;
	m_times = 1;
	m_filters = 0;
	m_figure = 0;
}

CSettingDlg::~CSettingDlg()
{
}

void CSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_LENGTH, m_length);
	DDX_Text(pDX, IDC_TIMES, m_times);
	DDX_Radio(pDX, IDC_EKF, m_filters);
	DDX_Radio(pDX, IDC_RMSE, m_figure);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}


BEGIN_MESSAGE_MAP(CSettingDlg, CDialogEx)
//	ON_BN_CLICKED(IDOK, &CSettingDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CSettingDlg 消息处理程序


//void CSettingDlg::OnBnClickedOk()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	CDialogEx::OnOK();
//}


BOOL CSettingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化，将对话框初始化为Windows Aero风格
	OSVERSIONINFOEX osver = {0};
	osver.dwOSVersionInfoSize = sizeof(osver);
	osver.dwMajorVersion = 5;

	DWORDLONG dwlConditionMask = 0;
	VER_SET_CONDITION(dwlConditionMask, VER_MAJORVERSION, VER_GREATER);

	if (VerifyVersionInfo(&osver, VER_MAJORVERSION, dwlConditionMask))//判断操作系统版本，若达到Vista以上版本，则显示Aero效果
	{
		SetWindowLong(m_hWnd,GWL_EXSTYLE,WS_EX_LAYERED);
		SetLayeredWindowAttributes(TRANSPARENT_COLOR, 0, LWA_COLORKEY);
		BOOL bDwm ;
		DwmIsCompositionEnabled (&bDwm);
		if (bDwm)
		{  
			MARGINS mrg = {-1};//{-1};  
			DwmExtendFrameIntoClientArea(m_hWnd , &mrg);
			SetBackgroundColor(TRANSPARENT_COLOR);
			CMFCVisualManager ::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
			CMFCButton ::EnableWindowsTheming ();
		} 
	}

	    // Load accelerators 
    //m_hAccel = ::LoadAccelerators(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME)); //快捷键表id
    //ASSERT(m_hAccel); 

	return TRUE;

	// 异常: OCX 属性页应返回 FALSE
}

/*BOOL CSettingDlg::PreTranslateMessage(MSG* pMsg) 
{ 
    if (WM_KEYFIRST <= pMsg->message && 
        pMsg->message <= WM_KEYLAST) 
    { 
        HACCEL hAccel = m_hAccel; 
        if (hAccel && 
            ::TranslateAccelerator(m_hWnd, hAccel, pMsg)) 
            return TRUE; 
    } 
    return CDialog::PreTranslateMessage(pMsg); 
}*/