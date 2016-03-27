
#ifndef AFX_GRAPH_H__9DB68B4D_3C7C_47E2_9F72_EEDA5D2CDBB0__INCLUDED_
#define AFX_GRAPH_H__9DB68B4D_3C7C_47E2_9F72_EEDA5D2CDBB0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Graph.h : header file
//

//color definitions
#define RED				RGB(255,0,0)
#define GREEN			RGB(0,255,0) 
#define BLUE			RGB(0,0,255)
#define YELLOW			RGB(255,255,0)
#define ORANGE			RGB(255,153,51)
#define HOT_PINK		RGB(255,51,153)
#define PURPLE			RGB(153,0,204)
#define CYAN			RGB(0,255,255)
#define BLACK			RGB(0,0,0)
#define WHITE			RGB(255,255,255)
#define LAVENDER		RGB(199,177,255)
#define PEACH			RGB(255,226,177)
#define SKY_BLUE		RGB(142,255,255)
#define FOREST_GREEN	RGB(0,192,0)
#define BROWN			RGB(80,50,0)
#define TURQUOISE		RGB(0,192,192)
#define ROYAL_BLUE		RGB(0,0,192)
#define GREY			RGB(192,192,192)
#define DARK_GREY		RGB(128,128,128)
#define TAN				RGB(255,198,107)
#define DARK_BLUE		RGB(0,0,128)
#define MAROON			RGB(128,0,0)
#define DUSK			RGB(255,143,107)
#define LIGHT_GREY		RGB(225,225,225)	//only for 3D graph lines

//for graph type definitions :
	//single number = 2D type graph
	//number in 20s = 2D type graph related to graph of single number
	//number in 30s = 3D type graph related to graph of single number
//graph type definitions
#define BAR_GRAPH				0
#define LINE_GRAPH				1	//lines between plotted points

#define VERTICAL_ALIGN		1
#define HORIZONTAL_ALIGN	0

/////////////////////////////////////////////////////////////////////////////
// CGraph window

#include "GraphSeries.h"
#include "GraphLegend.h"

class CGraph : public CStatic
{
// Construction
public:
	CGraph();
	CGraph(int type);

// Attributes
public:
	//int maxHeight;

private:
	double tickSpace;	//number of pixels between "y" axis ticks
	CObList* graphSeries;
	int graphType;	//0 - bar graph, 1 - line graph, 2 - pie
	int graphAlignment;	//0 - horizontal, 1 - vertical
	int maxHeight;
	int maxWidth;
	CObList* colorList;
	CString axisYLabel;
	CString axisXLabel;
	int yApexPoint;	//yApex is the number of pixels from top to draw the x axis(bottom of y axis line)
	int xApexPoint;	//xApex is number of pixels from left to draw y axis (leftmost point of x axis line)
	int yAxisHeight;
	int xAxisWidth;
	int yTickFontSize;
	int xTickFontSize;
	int legendFontSize;
	double numTicks;//若为int，则坐标原点无法对齐
	double minTick, maxTick;
	int seriesSize;	//number of data elements to map per series
	CGraphLegend graphLegend;
	BOOL graphHasLegend;
	int legendWidth;
	int xAxisAlign;	//# - degree rotation (0 = horizontal, 90 = vertical, etc)
	int xAxisLabelLength;
	int yAxisLabelLength;	//in case I ever need it
	CString graphTitle;
	BOOL graphHasGridLines;
	int legendMaxText;
	int topYTick;
	int rightXTick;
	int graphQuadType;
	BOOL quadSetManually;
	BOOL inRedraw;

	//print settings
	int topMargin;
	int bottomMargin;
	int leftMargin;
	int rightMargin;
	int pGraphL, pGraphT, pGraphB, pGraphR;

// Operations
public:
	void SetXTickFontSize(int size);
	void SetYTickFontSize(int size);
	void SetLegendFontSize(int size);
	void DrawGraph(CDC* pDC);
	void AddSeries(CGraphSeries* dataSet);
	void SetXAxisLabel(CString label);
	void SetYAxisLabel(CString label);
	void SetLegend(int datagroup, CString label);
	void SetXAxisAlignment(int alignValue);
	void SetGraphType(int gType);
	void SetGraphTitle(CString title);
	int PrintGraph(CDC *pDC, CPrintInfo* pInfo);
	void SetMargins(int top, int bottom, int left, int right, int graphTop);
	void SetGridLines(BOOL hasGridLines);
	void SetGraphAlignment(int alignment);  //0 - vertical, 1 horizontal
	void SetTickLimits(double minTick, double maxTick, double tickStep);
	void SetColor(int dataGroup, COLORREF groupColor);
	void RemoveSeries(CString label);
	void RemoveColor(int dataGroup);
	void RemoveAllSeries();
	void RemoveAllColors();
	void RemoveLegend(int dataGroup);
	void RemoveAllLegends();
	void RemoveAllData();	//removes all series, legends and colors
	void SetGraphQuadType(int quads);

private:
	void DrawAxis(CDC* pDC);
	void DrawSeries(CDC* pDC);
	int DrawLegend(CDC* pDC);
	COLORREF GetColor(int dataGroup);
	int GetXAxisAlignment();

	void DrawBarSeries(CDC* pDC);
	void DrawLineSeries(CDC* pDC);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGraph)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGraph();

	// Generated message map functions
protected:
	//{{AFX_MSG(CGraph)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRAPH_H__9DB68B4D_3C7C_47E2_9F72_EEDA5D2CDBB0__INCLUDED_)
