// GraphLegendSet.cpp : implementation file
//

#pragma once
#include "stdafx.h"
#include "GraphLegendSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGraphLegendSet

CGraphLegendSet::CGraphLegendSet()
{
	legendId = 0;
	legendLabel = "";
}

CGraphLegendSet::~CGraphLegendSet()
{
}


BEGIN_MESSAGE_MAP(CGraphLegendSet, CStatic)
	//{{AFX_MSG_MAP(CGraphLegendSet)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGraphLegendSet message handlers

void CGraphLegendSet::SetLegendLabel(int id, CString legendText)
{
	legendId = id;
	legendLabel = legendText;
}

CString CGraphLegendSet::GetLegendLabel()
{
	return legendLabel;
}
