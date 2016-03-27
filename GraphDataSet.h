
#ifndef _AFX_GRAPHDATASET_HEADER_
#define _AFX_GRAPHDATASET_HEADER_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GraphDataSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGraphDataSet window

class CGraphDataSet : public CStatic
{
// Construction
public:
	CGraphDataSet();

// Attributes
public:

private :
	double xPosition;
	double yValue;

// Operations
public:
	void SetXPosition(double x);
	void SetYValue(double y);
	double GetXData();
	double GetYData();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGraphDataSet)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGraphDataSet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CGraphDataSet)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRAPHDATASET_H__3CBDC4C1_494F_11D5_AD40_006008198224__INCLUDED_)
