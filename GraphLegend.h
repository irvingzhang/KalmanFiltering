
#ifndef _AFX_GRAPHLEGEND_HEADER_
#define _AFX_GRAPHLEGEND_HEADER_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GraphLegend.h : header file
//

#include "GraphLegendSet.h"

/////////////////////////////////////////////////////////////////////////////
// CGraphLegend window

class CGraphLegend : public CStatic
{
// Construction
public:
	CGraphLegend();

// Attributes
public:


private:
	int legendLength;
	CObList *legendLabels;

// Operations
public:
	void SetLegendText(int datagroup, CString label);
	CString GetLegendText(int datagroup);
	int GetLegendLength();
	void Remove(int dataGroup);
	void RemoveAll();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGraphLegend)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGraphLegend();

	// Generated message map functions
protected:
	//{{AFX_MSG(CGraphLegend)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRAPHLEGEND_H__99A8A552_4029_40CC_839A_D8210C46CD8B__INCLUDED_)
