
#ifndef _AFX_GRAPHDATACOLOR_HEADER_
#define _AFX_GRAPHDATACOLOR_HEADER_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GraphDataColor.h : header file
//

#include "Graph.h"

/////////////////////////////////////////////////////////////////////////////
// CGraphDataColor window

class CGraphDataColor : public CStatic
{
// Construction
public:
	CGraphDataColor(int group, COLORREF color);

// Attributes
public:
	int dataGroup;	//just an id indicator to help searching
	COLORREF groupColor;

// Operations
public:
	COLORREF GetColor();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGraphDataColor)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGraphDataColor();

	// Generated message map functions
protected:
	//{{AFX_MSG(CGraphDataColor)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRAPHDATACOLOR_H__3CBDC4C5_494F_11D5_AD40_006008198224__INCLUDED_)
