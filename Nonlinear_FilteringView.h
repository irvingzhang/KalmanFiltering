
// Nonlinear_FilteringView.h : CNonlinear_FilteringView 类的接口
//
#ifndef _AFX_NONLINEAR_FILTERING_HEADER_
#define _AFX_NONLINEAR_FILTERING_HEADER_

#pragma once
#include "Graph.h"
#include "Globals.h"
#include "RealTrack.h"
#include "EKF.h"
#include "UKF.h"
#include "ThirdCKF.h"
#include "ICKF.h"
#include "FifthCKF.h"
#include "StoreData.h"
#include "GraphSeries.h"

class CNonlinear_FilteringView : public CView
{
private:
	UINT m_length;
	UINT m_times;
	int m_filters;
	int m_figure;
//	UINT m_select;
	//double Temp[4][1];
	//CMatrix nCM;
	//CPtrArray cnv;

private:
	RealTrack rt;
	EKF ekf;
	UKF ukf;
	ThirdCKF thirdckf;
	ICKF ickf;
	FifthCKF fifthckf;
	CProgressCtrl m_progress;

public:
	CGraph* filterGraph;
	BOOL graphComplete;

public:
	double RootMSE(CMatrix, int);
	double VelPosMSE(CMatrix);
	virtual void OnInitialUpdate();
	void GetExcuteProgPath(CString &pszPath);
	//void DeleteObj(CGraphSeries &);

protected: // 仅从序列化创建
	CNonlinear_FilteringView();
	//CNonlinear_FilteringView(CMatrix);
	DECLARE_DYNCREATE(CNonlinear_FilteringView)

// 特性
public:
	CNonlinear_FilteringDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CNonlinear_FilteringView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSetting();
	//afx_msg void OnBnClickedOk();
	afx_msg void OnPlot();
	afx_msg void OnHelper();
};

#ifndef _DEBUG  // Nonlinear_FilteringView.cpp 中的调试版本
inline CNonlinear_FilteringDoc* CNonlinear_FilteringView::GetDocument() const
   { return reinterpret_cast<CNonlinear_FilteringDoc*>(m_pDocument); }
#endif

#endif