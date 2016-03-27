
#pragma once
#include "stdafx.h"
#include "GraphLegend.h"
#include "GraphLegendSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGraphLegend

CGraphLegend::CGraphLegend()
{
	legendLabels = new CObList();
	legendLength = 0;

}

CGraphLegend::~CGraphLegend()
{
	POSITION pos;
	CGraphLegendSet *legendSet;
	pos = legendLabels->GetHeadPosition();
	while(pos != NULL)
	{
		legendSet = (CGraphLegendSet*)legendLabels->GetAt(pos);
		legendLabels->RemoveAt(pos);
		delete legendSet;
	}
	delete legendLabels;
}


BEGIN_MESSAGE_MAP(CGraphLegend, CStatic)
	//{{AFX_MSG_MAP(CGraphLegend)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGraphLegend message handlers

void CGraphLegend::SetLegendText(int datagroup, CString label)
{
	CGraphLegendSet *legendData = new CGraphLegendSet();
	legendData->SetLegendLabel(datagroup, label);
	legendLabels->AddTail(legendData);
	if(label.GetLength() > legendLength)
		legendLength = label.GetLength();
}

CString CGraphLegend::GetLegendText(int datagroup)
{
	POSITION pos;
	CGraphLegendSet *legendData;
	if((legendLabels->GetCount() < datagroup) || (legendLabels->GetCount() == 0))
	{
		return "";
	}

	pos = legendLabels->GetHeadPosition();
	#pragma omp parallel for
	for (int i = 0; i < datagroup; i++)
	{
		legendLabels->GetNext(pos);
	}
	legendData = (CGraphLegendSet*)legendLabels->GetAt(pos);
	return legendData->GetLegendLabel();
}

int CGraphLegend::GetLegendLength()
{
	return legendLength;
}

void CGraphLegend::Remove(int dataGroup)
{
	POSITION pos;
	CGraphLegendSet *legendSet;
	pos = legendLabels->GetHeadPosition();
	#pragma omp parallel for
	for (int i = 0; i < dataGroup; i++)
	{
		legendSet = (CGraphLegendSet*)legendLabels->GetAt(pos);
		legendLabels->GetNext(pos);
	}
	legendLabels->RemoveAt(pos);
	delete legendSet;
}

void CGraphLegend::RemoveAll()
{
	POSITION pos;
	CGraphLegendSet *legendSet;
	pos = legendLabels->GetHeadPosition();
	while(pos != NULL)
	{
		legendSet = (CGraphLegendSet*)legendLabels->GetAt(pos);
		legendLabels->RemoveAt(pos);
		delete legendSet;
	}
}
