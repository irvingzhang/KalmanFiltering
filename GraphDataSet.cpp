
// GraphDataSet.cpp : implementation file
//
#pragma once
#include "stdafx.h"
#include "GraphDataSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGraphDataSet

CGraphDataSet::CGraphDataSet()
{
	xPosition = 0;
	yValue = 0;
}

CGraphDataSet::~CGraphDataSet()
{
}


BEGIN_MESSAGE_MAP(CGraphDataSet, CStatic)
	//{{AFX_MSG_MAP(CGraphDataSet)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGraphDataSet message handlers

void CGraphDataSet::SetXPosition(double x)
{
	xPosition = x;
}

void CGraphDataSet::SetYValue(double y)
{
	yValue = y;
}

double CGraphDataSet::GetXData()
{
	return xPosition;
}

double CGraphDataSet::GetYData()
{
	return yValue;
}
