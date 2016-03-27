
#pragma once
#include "stdafx.h"
#include "afxtempl.h"
#include "GraphSeries.h"
#include "GraphLegend.h"
#include "math.h"
#include "Graph.h"
#include "GraphDataColor.h"
#include "afxwin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGraph

CGraph::CGraph()
{
	graphSeries = new CObList();
	seriesSize = 0;
	minTick = 0;
	maxTick = 100;
	numTicks = 10;
	tickSpace = 10;
	maxHeight = 0;

	graphHasLegend = FALSE;
	legendWidth = 0;
	legendMaxText= 0;
	graphAlignment = VERTICAL_ALIGN;  

	graphType = 0;

	xAxisAlign = 0;  //horizontal
	xAxisLabelLength = 0;

	xTickFontSize = 16;
	yTickFontSize = 16;
	legendFontSize = 12;

	SetGridLines(FALSE);

	graphQuadType = 1;
	quadSetManually = FALSE;
}

CGraph::CGraph(int type)
{
	graphSeries = new CObList();
	colorList = new CObList();
	seriesSize = 0;
	minTick = 0;
	maxTick = 100;
	numTicks = 10;
	tickSpace = 10;
	maxHeight = 0;

	graphHasLegend = FALSE;
	legendWidth = 0;
	legendMaxText= 0;
	graphAlignment = VERTICAL_ALIGN;  

	graphType = type;

	xAxisAlign = 0;  
	xAxisLabelLength = 0;

	xTickFontSize = 16;
	yTickFontSize = 16;
	legendFontSize = 12;

	SetGridLines(FALSE);

	graphQuadType = 1;
	if(type != BAR_GRAPH)
	{
		quadSetManually = TRUE;
	}
	else
	{
		quadSetManually = FALSE;
	}
}

CGraph::~CGraph()
{
	POSITION pos;
	CGraphSeries* pSeries;

	for (pos = graphSeries->GetHeadPosition(); pos != NULL; )
	{
		pSeries = (CGraphSeries*) graphSeries->GetNext( pos );
		graphSeries->RemoveAt(pos);
		delete pSeries;
	}

	graphSeries->RemoveAll();
	delete graphSeries;

	CGraphDataColor* dataColor;
	for ( pos = colorList->GetHeadPosition(); pos != NULL; )
	{
		dataColor = (CGraphDataColor*) colorList->GetNext( pos );
		colorList->RemoveAt(pos);
		delete dataColor;
	}

	colorList->RemoveAll();
	delete colorList;
	
}


BEGIN_MESSAGE_MAP(CGraph, CStatic)
	//{{AFX_MSG_MAP(CGraph)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGraph message handlers

void CGraph::SetGraphType(int gType)
{
	graphType = gType;
}

void CGraph::SetXAxisAlignment(int alignValue)
{
	xAxisAlign = alignValue;
}

int CGraph::GetXAxisAlignment()
{
	return xAxisAlign;
}

void CGraph::SetColor(int dataGroup, COLORREF groupColor)
{
	CGraphDataColor *dataColor = new CGraphDataColor(dataGroup, groupColor);
	colorList->AddTail(dataColor);
}

COLORREF CGraph::GetColor(int dataGroup)
{
	POSITION pos;
	if((colorList->GetCount() <= dataGroup) || (colorList->GetCount() == 0))
		return BLACK;
	
	CGraphDataColor* dataColor;
	pos = colorList->GetHeadPosition();

	//  
	for (int i = 0; i < dataGroup; ++ i)
	{
		colorList->GetNext(pos);
	}

	dataColor = (CGraphDataColor*) colorList->GetAt( pos );
	return dataColor->GetColor();	
}

void CGraph::DrawGraph(CDC* pDC)
{
	CString tickLabel;
	CWnd* graphWnd = pDC->GetWindow();

	CRect graphRect;
	graphWnd->GetClientRect(&graphRect);
	TEXTMETRIC	tm;

	graphRect.NormalizeRect();

	//reset graph to be clear background
	COLORREF backColor;
	backColor = RGB(255,255,255);  //replace with desired background color
	CBrush backBrush (backColor);
	CBrush* pOldBackBrush;
	pOldBackBrush = pDC->SelectObject(&backBrush);
	//DeleteObject(pOldBackBrush);
	pDC->Rectangle(0, 0, graphRect.right, graphRect.bottom);//.Width(), graphRect.Height());
	pDC->SelectObject(pOldBackBrush);

	maxHeight = graphRect.Height() - 20;  //for frame window and status bar
	maxWidth = graphRect.Width() - 5;  //for frame window

	//draw graph title
	CFont titleFont;
	int tFontSize = 24;
	while((graphTitle.GetLength() * (tFontSize / 2)) > maxWidth)
	{
		tFontSize -= 2;
	}
	titleFont.CreateFont(tFontSize, 0, 0, 0, 700, FALSE, FALSE, 0,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Times New Roman");
	CFont* pOldFont = (CFont*) pDC->SelectObject(&titleFont);

	pDC->GetTextMetrics(&tm);

	int charWidth = tm.tmAveCharWidth;
	//next line is centered....trust me !!
	pDC->TextOut((graphRect.Width() / 2) - ((graphTitle.GetLength() / 2) * charWidth),
				10, graphTitle);
	pDC->SelectObject(pOldFont);

	{
		//compute label size for axis alignment
		CFont sideFont, axisFont;
		int tFontSize = 16;
		if(maxWidth > maxHeight)
		{
			while((axisYLabel.GetLength() * (tFontSize / 2)) > maxHeight)
			{
				tFontSize -= 2;
			}
		}
		else
		{
			while((axisXLabel.GetLength() * (tFontSize / 2)) > maxWidth)
			{
				tFontSize -= 2;
			}
		}
		axisFont.CreateFont(tFontSize, 0, 0, 0, 700, FALSE, FALSE, 0,
			ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Times New Roman");
		sideFont.CreateFont(tFontSize, 0, 900, 0, 700, FALSE, FALSE, 0,
			ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Times New Roman");

		CFont* pOldFont = (CFont*) pDC->SelectObject(&sideFont);
		pDC->GetTextMetrics(&tm);
		int charWidth = tm.tmAveCharWidth;
		int labelHeight = tm.tmHeight + 10;
		if(graphAlignment)
		{
			if(axisYLabel.GetLength() == 0)
				labelHeight = 0;
		}
		else
		{
			if(axisXLabel.GetLength() == 0)
				labelHeight = 0;
		}
		pDC->SelectObject(pOldFont);

		tickLabel.Format("%.4f", maxTick);

		//determine axis specifications 
		if(graphAlignment)
		{
				xApexPoint = 5 + (tickLabel.GetLength() * (yTickFontSize / 2)) + 45; 
				if(!xAxisAlign)  //horizontal
					yApexPoint = (maxHeight - 5) - 45;		//apex points are the cross section of axis lines
				else
					yApexPoint = (maxHeight - 5) - (xAxisLabelLength * (xTickFontSize / 2)) - 10;
				yAxisHeight = yApexPoint - 40;
				xAxisWidth = (maxWidth - 5) - xApexPoint;
		}
		else
		{
			xApexPoint = 5 + (xAxisLabelLength * charWidth) + labelHeight; //labelHeight added for y-axis label height
			yApexPoint = (maxHeight - 15) - tm.tmHeight;
			yAxisHeight = yApexPoint - 40;
			xAxisWidth = (maxWidth - 5) - xApexPoint;
		}
	}

	//draw legend
	if(graphHasLegend)
	{
		int legendLeft = DrawLegend(pDC);
		xAxisWidth = legendLeft - 10 - xApexPoint;
	}

	if((!quadSetManually) && (graphType == BAR_GRAPH))
	{
		//computer number of quadrants needed based on data
		POSITION pos;
		pos = graphSeries->GetHeadPosition();
		CGraphSeries* tmpSeries;
		double minXValue = 0;
		double minYValue = 0;

		//  
		for (int x = 1; x <= graphSeries->GetCount(); ++ x)
		{
			tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);
			//  
			for (int s = 0; s < seriesSize; ++ s)
			{
				//to allow scalability (height may be less than tickRange)
				double curXValue = tmpSeries->GetXDataValue();
				double curYValue = tmpSeries->GetYDataValue();
				if(curXValue < minXValue)
					minXValue = curXValue;
				if(curYValue < minYValue)
					minYValue = curYValue;
			}
		}
		graphQuadType = 1;
		if((minXValue < 0) && (minYValue < 0))
			graphQuadType = 4;
		if((minXValue < 0) && (minYValue >= 0))
			graphQuadType = 2;
		if((minXValue >= 0) && (minYValue < 0))
			graphQuadType = 3;
	}

	if(graphType == BAR_GRAPH)
	{
		switch(graphQuadType)
		{
			case 2 :
				xApexPoint += xAxisWidth / 2;
				break;
			case 3 :
				yApexPoint -= yAxisHeight / 2;
				break;
			case 4 :
				xApexPoint += xAxisWidth / 2;
				yApexPoint -= yAxisHeight / 2;
				break;
		}
	}

		inRedraw = FALSE;
		//draw axis lines
		DrawAxis(pDC);

	//draw series data and labels
	DrawSeries(pDC);
}

void CGraph::DrawAxis(CDC* pDC)
{
	TEXTMETRIC	tm;
	pDC->SetTextColor(BLACK);
	CFont sideFont, axisFont, dummyFont;
	int charWidth, labelHeight;
	double tickScale;
	int tickXLocation, tickYLocation;
	int seriesSpace;
	CFont* pOldFont;
	int x, y;

	COLORREF axisLineColor;
	axisLineColor = BLACK;
	CPen axisPen (PS_SOLID, 2, axisLineColor);
	dummyFont.CreateFont(12, 0, 0, 0, 700, FALSE, FALSE, 0,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Times New Roman");
	pOldFont = (CFont*)pDC->SelectObject(&dummyFont);
	switch(graphType)
	{
		case BAR_GRAPH :
		case LINE_GRAPH :
				CPen* pOldAxisPen;
				pOldAxisPen = pDC->SelectObject(&axisPen);
				switch(graphQuadType)
				{
					case 1 :
						//draw y axis
						pDC->MoveTo(xApexPoint, yApexPoint);  
						pDC->LineTo(xApexPoint, yApexPoint - yAxisHeight);

						pDC->MoveTo(xApexPoint + xAxisWidth, yApexPoint);  
						pDC->LineTo(xApexPoint + xAxisWidth, yApexPoint - yAxisHeight);

						//draw x axis
						pDC->MoveTo(xApexPoint, yApexPoint);  
						pDC->LineTo(xApexPoint + xAxisWidth, yApexPoint);

						pDC->MoveTo(xApexPoint, yApexPoint - yAxisHeight);  
						pDC->LineTo(xApexPoint + xAxisWidth, yApexPoint - yAxisHeight);
						break;
					case 2 :
						//draw y axis
						pDC->MoveTo(xApexPoint, yApexPoint);  
						pDC->LineTo(xApexPoint, yApexPoint - yAxisHeight);

						//draw x axis
						pDC->MoveTo(xApexPoint - (xAxisWidth / 2), yApexPoint);  
						pDC->LineTo(xApexPoint + (xAxisWidth / 2), yApexPoint);
						break;
					case 3 :
						//draw y axis
						pDC->MoveTo(xApexPoint, yApexPoint + (yAxisHeight / 2));  
						pDC->LineTo(xApexPoint, yApexPoint - (yAxisHeight / 2));

						//draw x axis(画x坐标轴)
						pDC->MoveTo(xApexPoint, yApexPoint);//yApexPoint);  
						pDC->LineTo(xApexPoint + xAxisWidth, yApexPoint);//yApexPoint);
						break;
					case 4 :
						//draw y axis
						pDC->MoveTo(xApexPoint, yApexPoint + (yAxisHeight / 2));  
						pDC->LineTo(xApexPoint, yApexPoint - (yAxisHeight / 2));

						//draw x axis
						pDC->MoveTo(xApexPoint - (xAxisWidth / 2), yApexPoint);  
						pDC->LineTo(xApexPoint + (xAxisWidth / 2), yApexPoint);
						break;
				}
				pDC->SelectObject(pOldAxisPen);

				int tFontSize;
				//draw labels
				tFontSize = 16;
				if(maxWidth > maxHeight)
				{
					while((axisYLabel.GetLength() * (tFontSize / 2)) > maxHeight)
					{
						tFontSize -= 2;
					}
				}
				else
				{
					while((axisXLabel.GetLength() * (tFontSize / 2)) > maxWidth)
					{
						tFontSize -= 2;
					}
				}
				axisFont.CreateFont(tFontSize, 0, 0, 0, 700, FALSE, FALSE, 0,
					ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
					DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Times New Roman");
				sideFont.CreateFont(tFontSize, 0, 900, 0, 700, FALSE, FALSE, 0,
					ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
					DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Times New Roman");

				pDC->SelectObject(&sideFont);
				pDC->GetTextMetrics(&tm);
				charWidth = tm.tmAveCharWidth;

				if(graphAlignment)
				{
					switch(graphQuadType)
					{
						case 1 :
						case 2 :
							pDC->TextOut(10, (yApexPoint - (yAxisHeight / 2)) + ((axisYLabel.GetLength() * charWidth) / 2), axisYLabel);
							break;
						case 3 :
						case 4 :
							pDC->TextOut(10, yApexPoint + ((axisYLabel.GetLength() * charWidth) / 2), axisYLabel);
							break;
					}


					pDC->SelectObject(&axisFont);
					pDC->GetTextMetrics(&tm);
					charWidth = tm.tmAveCharWidth;
					switch(graphQuadType)
					{
						case 1 :
							pDC->TextOut(xApexPoint + (xAxisWidth / 2) - ((axisXLabel.GetLength() / 2) * charWidth), maxHeight - 5 - 6, axisXLabel);
							break;
						case 2 :
							pDC->TextOut(xApexPoint - ((axisXLabel.GetLength() / 2) * charWidth), maxHeight - 5 - 6, axisXLabel);
							break;
						case 3 :
							pDC->TextOut(xApexPoint + (xAxisWidth / 2) - ((axisXLabel.GetLength() / 2) * charWidth), maxHeight - 5 - 6, axisXLabel);
							break;
						case 4 :
							pDC->TextOut(xApexPoint - ((axisXLabel.GetLength() / 2) * charWidth), maxHeight - 5 - 6, axisXLabel);
							break;
					}

					
					pDC->SelectObject(pOldFont);

					tickScale = 0.00;
					tickScale = (yAxisHeight - 10) / numTicks;

					//draw y axis ticks
					if(!inRedraw)
					{
						switch(graphQuadType)
						{
							case 1 :
								//  
								for (y = 1; y <= numTicks; ++ y) 
								{
									tickYLocation = (int)(yApexPoint - (y * tickScale));

									//draw tick mark
									pDC->MoveTo(xApexPoint - 3, tickYLocation);
									pDC->LineTo(xApexPoint + 3, tickYLocation);

									if(graphHasGridLines)
									{
										//draw grid lines
										COLORREF gridLineColor;
										gridLineColor = DARK_GREY;
										CPen gridPen (PS_SOLID, 1, gridLineColor);
										CPen* pOldPen;
										pOldPen = pDC->SelectObject(&gridPen);
										pDC->MoveTo(xApexPoint, tickYLocation);
										pDC->LineTo(xApexPoint + xAxisWidth, tickYLocation);
										pDC->SelectObject(pOldPen);
									}

									//draw tick label
									CString tickLabel;
									tickLabel.Format("%.4f", minTick + (y * tickSpace));

									CFont yFont;
									yFont.CreateFont(yTickFontSize, 0, 0, 0, 700, FALSE, FALSE, 0,
										ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
										DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Times New Roman");
									pDC->SelectObject(&yFont);

									pDC->TextOut(xApexPoint - 10 - (tickLabel.GetLength() * (yTickFontSize / 2)), tickYLocation - 6, tickLabel);
									pDC->SelectObject(pOldFont);
									topYTick = tickYLocation;
								}
								break;
								pDC->SelectObject(pOldFont);
							case 2 :
								{
									//  
									for (y = 1; y <= numTicks; ++ y)  
									{
										tickYLocation = (int)(yApexPoint - (y * tickScale));

										//draw tick mark
										pDC->MoveTo(xApexPoint - 3, tickYLocation);
										pDC->LineTo(xApexPoint + 3, tickYLocation);

										if(graphHasGridLines)
										{
											//draw grid lines
											COLORREF gridLineColor;
											gridLineColor = DARK_GREY;
											CPen gridPen (PS_SOLID, 1, gridLineColor);
											CPen* pOldPen;
											pOldPen = pDC->SelectObject(&gridPen);
											pDC->MoveTo(xApexPoint - (xAxisWidth / 2), tickYLocation);
											pDC->LineTo(xApexPoint + (xAxisWidth / 2), tickYLocation);
											pDC->SelectObject(pOldPen);
										}

										//draw tick label
										CString tickLabel;
										tickLabel.Format("%.4f", minTick + (y * tickSpace));

										CFont yFont;
										yFont.CreateFont(yTickFontSize, 0, 0, 0, 700, FALSE, FALSE, 0,
											ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
											DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Times New Roman");
										pDC->SelectObject(&yFont);

										pDC->TextOut(xApexPoint - (xAxisWidth / 2) - 10 - (tickLabel.GetLength() * (yTickFontSize / 2)), tickYLocation - 6, tickLabel);
										topYTick = tickYLocation;

									}
									//draw the 0
									pDC->TextOut(xApexPoint - (xAxisWidth / 2) - 10 - (yTickFontSize / 2), yApexPoint - 6, "0");
								}
								pDC->SelectObject(pOldFont);
								break;
							case 3 :
								//  
								for (y = 0; y <= numTicks / 2; ++ y) 
								{
									tickYLocation = (int)(yApexPoint + 5 - (yAxisHeight / 2) + (y * tickScale));

									//draw tick mark
									pDC->MoveTo(xApexPoint - 3, tickYLocation);
									pDC->LineTo(xApexPoint + 3, tickYLocation);

									if(graphHasGridLines)
									{
										//draw grid lines
										COLORREF gridLineColor;
										gridLineColor = DARK_GREY;
										CPen gridPen (PS_SOLID, 1, gridLineColor);
										CPen* pOldPen;
										pOldPen = pDC->SelectObject(&gridPen);
										pDC->MoveTo(xApexPoint, tickYLocation);
										pDC->LineTo(xApexPoint, tickYLocation);
										pDC->SelectObject(pOldPen);
									}

									//draw tick label
									CString tickLabel;
									tickLabel.Format("%.4f", maxTick - (y * tickSpace));

									CFont yFont;
									yFont.CreateFont(yTickFontSize, 0, 0, 0, 700, FALSE, FALSE, 0,
										ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
										DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Times New Roman");
									pDC->SelectObject(&yFont);

									pDC->TextOut(xApexPoint - 10 - (tickLabel.GetLength() * (yTickFontSize / 2)), tickYLocation - 6, tickLabel);
									topYTick = tickYLocation;

								}
								//draw the 0
								pDC->TextOut(xApexPoint - (xAxisWidth / 2) - 10 - (yTickFontSize / 2), yApexPoint - 6, "0");
								
								//  
								for (; y <= numTicks; ++ y) 
								{
									tickYLocation = (int)(yApexPoint + 5 - (yAxisHeight / 2) + (y * tickScale));

									//draw tick mark
									pDC->MoveTo(xApexPoint - 3, tickYLocation);
									pDC->LineTo(xApexPoint + 3, tickYLocation);

									if(graphHasGridLines)
									{
										//draw grid lines
										COLORREF gridLineColor;
										gridLineColor = DARK_GREY;
										CPen gridPen (PS_SOLID, 1, gridLineColor);
										CPen* pOldPen;
										pOldPen = pDC->SelectObject(&gridPen);
										pDC->MoveTo(xApexPoint, tickYLocation);
										pDC->LineTo(xApexPoint, tickYLocation);
										pDC->SelectObject(pOldPen);
									}

									//draw tick label
									CString tickLabel;
									tickLabel.Format("%.4f", minTick + ((numTicks - y) * tickSpace));

									pDC->TextOut(xApexPoint - 10 - (tickLabel.GetLength() * (yTickFontSize / 2)), tickYLocation - 6, tickLabel);
									topYTick = tickYLocation;

								}
								pDC->SelectObject(pOldFont);
								break;
							pDC->SelectObject(pOldFont);
							case 4 :
								//  
								for (y = 1; y <= numTicks / 2; ++ y) 
								{
									tickYLocation = (int)(yApexPoint - (y * tickScale));

									//draw tick mark
									pDC->MoveTo(xApexPoint - 3, tickYLocation);
									pDC->LineTo(xApexPoint + 3, tickYLocation);

									if(graphHasGridLines)
									{
										//draw grid lines
										COLORREF gridLineColor;
										gridLineColor = DARK_GREY;
										CPen gridPen (PS_SOLID, 1, gridLineColor);
										CPen* pOldPen;
										pOldPen = pDC->SelectObject(&gridPen);
										pDC->MoveTo(xApexPoint - (xAxisWidth / 2), tickYLocation);
										pDC->LineTo(xApexPoint + (xAxisWidth / 2), tickYLocation);
										pDC->SelectObject(pOldPen);
									}

									//draw tick label
									CString tickLabel;
									tickLabel.Format("%.4f", minTick + (y * tickSpace));

									CFont yFont;
									yFont.CreateFont(yTickFontSize, 0, 0, 0, 700, FALSE, FALSE, 0,
										ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
										DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Times New Roman");
									pDC->SelectObject(&yFont);

									pDC->TextOut(xApexPoint - (xAxisWidth / 2) - 10 - (tickLabel.GetLength() * (yTickFontSize / 2)), tickYLocation - 6, tickLabel);
									topYTick = tickYLocation;

								}
								//draw the 0
								pDC->TextOut(xApexPoint - (xAxisWidth / 2) - 10 - (yTickFontSize / 2), yApexPoint - 6, "0");
								
								//  
								for (; y <= numTicks; ++ y)  
								{
									tickYLocation = (int)(yApexPoint - (yAxisHeight / 2) + (y * tickScale));

									//draw tick mark
									pDC->MoveTo(xApexPoint - 3, tickYLocation);
									pDC->LineTo(xApexPoint + 3, tickYLocation);

									if(graphHasGridLines)
									{
										//draw grid lines
										COLORREF gridLineColor;
										gridLineColor = DARK_GREY;
										CPen gridPen (PS_SOLID, 1, gridLineColor);
										CPen* pOldPen;
										pOldPen = pDC->SelectObject(&gridPen);
										pDC->MoveTo(xApexPoint - (xAxisWidth / 2), tickYLocation);
										pDC->LineTo(xApexPoint + (xAxisWidth / 2), tickYLocation);
										pDC->SelectObject(pOldPen);
									}

									//draw tick label
									CString tickLabel;
									tickLabel.Format("%.4f", minTick + ((numTicks - y) * tickSpace));

									pDC->TextOut(xApexPoint - (xAxisWidth / 2) - 10 - (tickLabel.GetLength() * (yTickFontSize / 2)), tickYLocation - 6, tickLabel);
									topYTick = tickYLocation;

								}
								pDC->SelectObject(pOldFont);
								break;
						}
					}

					//draw X axis tick marks
					switch(graphQuadType)
					{
						case 1 :
							{
								POSITION pos;
								pos = graphSeries->GetHeadPosition();
								CGraphSeries* tmpSeries;
								//  
								for (int x = 1; x <= graphSeries->GetCount(); ++ x)
								{
									tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);

									seriesSpace = xAxisWidth / graphSeries->GetCount();
									tickXLocation = xApexPoint + ((x * seriesSpace) - (seriesSpace / 2));

									pDC->MoveTo(tickXLocation,yApexPoint - 3);
									pDC->LineTo(tickXLocation,yApexPoint + 3);

									//draw tick label
									CString tickLabel;
									CFont xFont;
									xFont.CreateFont(xTickFontSize, 0, 0, 0, 700, FALSE, FALSE, 0,
										ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
										DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Times New Roman");
									pDC->SelectObject(&xFont);
									tickLabel = tmpSeries->GetLabel();
									if(!xAxisAlign)  //horizontal
										pDC->TextOut(tickXLocation - ((tickLabel.GetLength() * 8) / 2), yApexPoint + 8, tickLabel);
									else
									{
										CFont sideFont2;
										sideFont2.CreateFont(xTickFontSize, 0, (xAxisAlign * 10), 0, 700, FALSE, FALSE, 0,
											ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
											DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Times New Roman");
										pDC->SelectObject(&sideFont2);
										pDC->GetTextMetrics(&tm);
										charWidth = tm.tmAveCharWidth;
										if(xAxisAlign < 180)
											pDC->TextOut(tickXLocation - 8, yApexPoint + 8 + (xAxisLabelLength * charWidth), tickLabel);
										else
											pDC->TextOut(tickXLocation + 2, yApexPoint + 8, tickLabel);

										pDC->SelectObject(pOldFont);
									}
									rightXTick = tickXLocation;
								}
							}
							break;
						case 2 :
							{
								POSITION pos;
								pos = graphSeries->GetHeadPosition();
								CGraphSeries* tmpSeries;
								seriesSpace = xAxisWidth / graphSeries->GetCount();
								//  
								for (x = 1; x <= graphSeries->GetCount() / 2; ++ x)
								{
									tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);

									tickXLocation = xApexPoint - (xAxisWidth / 2) + ((x * seriesSpace) - (seriesSpace / 2));

									pDC->MoveTo(tickXLocation,yApexPoint - 3);
									pDC->LineTo(tickXLocation,yApexPoint + 3);

									if(!inRedraw)
									{
										//draw tick label
										CString tickLabel;
										tickLabel = tmpSeries->GetLabel();
										if(!xAxisAlign)  //horizontal
										{
											CFont xFont;
											xFont.CreateFont(xTickFontSize, 0, 0, 0, 700, FALSE, FALSE, 0,
												ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
												DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Times New Roman");
											pDC->SelectObject(&xFont);
											pDC->TextOut(tickXLocation - ((tickLabel.GetLength() * 8) / 2), yApexPoint + 8, tickLabel);
											pDC->SelectObject(pOldFont);
										}
										else
										{
											CFont sideFont2;
											sideFont2.CreateFont(xTickFontSize, 0, (xAxisAlign * 10), 0, 700, FALSE, FALSE, 0,
												ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
												DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Times New Roman");
											pDC->SelectObject(&sideFont2);
											pDC->GetTextMetrics(&tm);
											charWidth = tm.tmAveCharWidth;
											if(xAxisAlign < 180)
												pDC->TextOut(tickXLocation - 8, yApexPoint + 8 + (xAxisLabelLength * charWidth), tickLabel);
											else
												pDC->TextOut(tickXLocation + 2, yApexPoint + 8, tickLabel);

											pDC->SelectObject(pOldFont);
										}
									}
									rightXTick = tickXLocation;
								}
								if((graphSeries->GetCount() % 2) > 0)
								{
									tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);
									
									tickXLocation = xApexPoint;

									pDC->MoveTo(tickXLocation,yApexPoint - 3);
									pDC->LineTo(tickXLocation,yApexPoint + 3);

									if(!inRedraw)
									{
										//draw tick label
										CString tickLabel;
										tickLabel = tmpSeries->GetLabel();
										if(!xAxisAlign)  //horizontal
										{
											CFont xFont;
											xFont.CreateFont(xTickFontSize, 0, 0, 0, 700, FALSE, FALSE, 0,
												ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
												DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Times New Roman");
											pDC->SelectObject(&xFont);
											pDC->TextOut(tickXLocation - ((tickLabel.GetLength() * 8) / 2), yApexPoint + 8, tickLabel);
											pDC->SelectObject(pOldFont);
										}
										else
										{
											CFont sideFont2;
											sideFont2.CreateFont(xTickFontSize, 0, (xAxisAlign * 10), 0, 700, FALSE, FALSE, 0,
												ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
												DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Times New Roman");
											pDC->SelectObject(&sideFont2);
											pDC->GetTextMetrics(&tm);
											charWidth = tm.tmAveCharWidth;
											if(xAxisAlign < 180)
												pDC->TextOut(tickXLocation - 8, yApexPoint + 8 + (xAxisLabelLength * charWidth), tickLabel);
											else
												pDC->TextOut(tickXLocation + 2, yApexPoint + 8, tickLabel);

											pDC->SelectObject(pOldFont);
										}
									}
									rightXTick = tickXLocation;
									++ x;
								}

								//  
								for (; x <= graphSeries->GetCount(); ++ x)
								{
									tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);

									tickXLocation = xApexPoint - (xAxisWidth / 2) + ((x * seriesSpace) - (seriesSpace / 2));

									pDC->MoveTo(tickXLocation,yApexPoint - 3);
									pDC->LineTo(tickXLocation,yApexPoint + 3);

									if(!inRedraw)
									{
										//draw tick label
										CString tickLabel;
										tickLabel = tmpSeries->GetLabel();
										if(!xAxisAlign)  //horizontal
										{
											CFont xFont;
											xFont.CreateFont(xTickFontSize, 0, 0, 0, 700, FALSE, FALSE, 0,
												ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
												DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Times New Roman");
											pDC->SelectObject(&xFont);
											pDC->TextOut(tickXLocation - ((tickLabel.GetLength() * 8) / 2), yApexPoint + 8, tickLabel);
											pDC->SelectObject(pOldFont);
										}
										else
										{
											CFont sideFont2;
											sideFont2.CreateFont(xTickFontSize, 0, (xAxisAlign * 10), 0, 700, FALSE, FALSE, 0,
												ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
												DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Times New Roman");
											pDC->SelectObject(&sideFont2);
											pDC->GetTextMetrics(&tm);
											charWidth = tm.tmAveCharWidth;
											if(xAxisAlign < 180)
												pDC->TextOut(tickXLocation - 8, yApexPoint + 8 + (xAxisLabelLength * charWidth), tickLabel);
											else
												pDC->TextOut(tickXLocation + 2, yApexPoint + 8, tickLabel);

											pDC->SelectObject(pOldFont);
										}
									}
									rightXTick = tickXLocation;
								}
							}
							break;
						case 3 :
							{
								POSITION pos;
								pos = graphSeries->GetHeadPosition();
								CGraphSeries* tmpSeries;
								  
								for (int x = 1; x <= graphSeries->GetCount(); ++ x)
								{
									tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);

									seriesSpace = xAxisWidth / graphSeries->GetCount();
									tickXLocation = xApexPoint + ((x * seriesSpace) - (seriesSpace / 2));

									pDC->MoveTo(tickXLocation,yApexPoint - 3);
									pDC->LineTo(tickXLocation,yApexPoint + 3);

									if(!inRedraw)
									{
										//draw tick label
										CString tickLabel;
										tickLabel = tmpSeries->GetLabel();
										if(!xAxisAlign)  //horizontal
										{
											CFont xFont;
											xFont.CreateFont(xTickFontSize, 0, 0, 0, 700, FALSE, FALSE, 0,
												ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
												DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Times New Roman");
											pDC->SelectObject(&xFont);
											pDC->TextOut(tickXLocation - ((tickLabel.GetLength() * 8) / 2), yApexPoint + (yAxisHeight / 2) + 8, tickLabel);
											pDC->SelectObject(pOldFont);
										}
										else
										{
											CFont sideFont2;
											sideFont2.CreateFont(xTickFontSize, 0, (xAxisAlign * 10), 0, 700, FALSE, FALSE, 0,
												ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
												DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Times New Roman");
											pDC->SelectObject(&sideFont2);
											pDC->GetTextMetrics(&tm);
											charWidth = tm.tmAveCharWidth;
											if(xAxisAlign < 180)
												pDC->TextOut(tickXLocation - 8, yApexPoint + (yAxisHeight / 2) + 8 + (xAxisLabelLength * charWidth), tickLabel);
											else
												pDC->TextOut(tickXLocation + 2, yApexPoint + (yAxisHeight / 2) + 8, tickLabel);

											pDC->SelectObject(pOldFont);
										}
									}
									rightXTick = tickXLocation;
								}
							}
							break;
						case 4 :
							{
								POSITION pos;
								pos = graphSeries->GetHeadPosition();
								CGraphSeries* tmpSeries;
								seriesSpace = xAxisWidth / graphSeries->GetCount();
								  
								for (x = 1; x <= graphSeries->GetCount() / 2; ++ x)
								{
									tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);

									tickXLocation = xApexPoint - (xAxisWidth / 2) + ((x * seriesSpace) - (seriesSpace / 2));

									pDC->MoveTo(tickXLocation,yApexPoint - 3);
									pDC->LineTo(tickXLocation,yApexPoint + 3);

									if(!inRedraw)
									{
										//draw tick label
										CString tickLabel;
										tickLabel = tmpSeries->GetLabel();
										if(!xAxisAlign)  //horizontal
										{
											CFont xFont;
											xFont.CreateFont(xTickFontSize, 0, 0, 0, 700, FALSE, FALSE, 0,
												ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
												DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Times New Roman");
											pDC->SelectObject(&xFont);
											pDC->TextOut(tickXLocation - ((tickLabel.GetLength() * 8) / 2), yApexPoint + (yAxisHeight / 2) + 8, tickLabel);
											pDC->SelectObject(pOldFont);
										}
										else
										{
											CFont sideFont2;
											sideFont2.CreateFont(xTickFontSize, 0, (xAxisAlign * 10), 0, 700, FALSE, FALSE, 0,
												ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
												DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Times New Roman");
											pDC->SelectObject(&sideFont2);
											pDC->GetTextMetrics(&tm);
											charWidth = tm.tmAveCharWidth;
											if(xAxisAlign < 180)
												pDC->TextOut(tickXLocation - 8, yApexPoint + (yAxisHeight / 2) + 8 + (xAxisLabelLength * charWidth), tickLabel);
											else
												pDC->TextOut(tickXLocation + 2, yApexPoint + (yAxisHeight / 2) + 8, tickLabel);

											pDC->SelectObject(pOldFont);
										}
									}
									rightXTick = tickXLocation;
								}
								if((graphSeries->GetCount() % 2) > 0)
								{
									tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);
									
									tickXLocation = xApexPoint;

									pDC->MoveTo(tickXLocation,yApexPoint - 3);
									pDC->LineTo(tickXLocation,yApexPoint + 3);

									if(!inRedraw)
									{
										//draw tick label
										CString tickLabel;
										tickLabel = tmpSeries->GetLabel();
										if(!xAxisAlign)  //horizontal
										{
											CFont xFont;
											xFont.CreateFont(xTickFontSize, 0, 0, 0, 700, FALSE, FALSE, 0,
												ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
												DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Times New Roman");
											pDC->SelectObject(&xFont);
											pDC->TextOut(tickXLocation - ((tickLabel.GetLength() * 8) / 2), yApexPoint + (yAxisHeight / 2) + 8, tickLabel);
											pDC->SelectObject(pOldFont);
										}
										else
										{
											CFont sideFont2;
											sideFont2.CreateFont(xTickFontSize, 0, (xAxisAlign * 10), 0, 700, FALSE, FALSE, 0,
												ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
												DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Times New Roman");
											pDC->SelectObject(&sideFont2);
											pDC->GetTextMetrics(&tm);
											charWidth = tm.tmAveCharWidth;
											if(xAxisAlign < 180)
												pDC->TextOut(tickXLocation - 8, yApexPoint + (yAxisHeight / 2) + 8 + (xAxisLabelLength * charWidth), tickLabel);
											else
												pDC->TextOut(tickXLocation + 2, yApexPoint + (yAxisHeight / 2) + 8, tickLabel);

											pDC->SelectObject(pOldFont);
										}
									}
									rightXTick = tickXLocation;
									++ x;
								}

								//  
								for (; x <= graphSeries->GetCount(); ++ x)
								{
									tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);

									tickXLocation = xApexPoint - (xAxisWidth / 2) + ((x * seriesSpace) - (seriesSpace / 2));

									pDC->MoveTo(tickXLocation,yApexPoint - 3);
									pDC->LineTo(tickXLocation,yApexPoint + 3);

									if(!inRedraw)
									{
										//draw tick label
										CString tickLabel;
										tickLabel = tmpSeries->GetLabel();
										if(!xAxisAlign)  //horizontal
										{
											CFont xFont;
											xFont.CreateFont(xTickFontSize, 0, 0, 0, 700, FALSE, FALSE, 0,
												ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
												DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Times New Roman");
											pDC->SelectObject(&xFont);
											pDC->TextOut(tickXLocation - ((tickLabel.GetLength() * 8) / 2), yApexPoint + (yAxisHeight / 2) + 8, tickLabel);
											pDC->SelectObject(pOldFont);
										}
										else
										{
											CFont sideFont2;
											sideFont2.CreateFont(xTickFontSize, 0, (xAxisAlign * 10), 0, 700, FALSE, FALSE, 0,
												ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
												DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Times New Roman");
											pDC->SelectObject(&sideFont2);
											pDC->GetTextMetrics(&tm);
											charWidth = tm.tmAveCharWidth;
											if(xAxisAlign < 180)
												pDC->TextOut(tickXLocation - 8, yApexPoint + (yAxisHeight / 2) + 8 + (xAxisLabelLength * charWidth), tickLabel);
											else
												pDC->TextOut(tickXLocation + 2, yApexPoint + (yAxisHeight / 2) + 8, tickLabel);

											pDC->SelectObject(pOldFont);
										}
									}
									rightXTick = tickXLocation;
								}
							}
							break;
					}
				}
				else
				{
					//reverse above stuff and treat as horizontal graph
					switch(graphQuadType)
					{
						case 1 :
							pDC->TextOut(10, (yApexPoint - (yAxisHeight / 2)) + (axisYLabel.GetLength() / 2), axisXLabel);

							pDC->SelectObject(&axisFont);
							pDC->GetTextMetrics(&tm);
							charWidth = tm.tmAveCharWidth;
							labelHeight = tm.tmHeight + 10;
							if(axisYLabel.GetLength() == 0)
								labelHeight = 0;
							pDC->TextOut(xApexPoint + (xAxisWidth / 2) - ((axisXLabel.GetLength() / 2) * charWidth), maxHeight - 5 - 6, axisYLabel);
							
							pDC->SelectObject(pOldFont);

							//to allow scalability (height may be less than tickRange)
							tickScale = 0.00;

							//draw y axis ticks
							{
								tickScale = (yAxisHeight - 10) / seriesSize;
								POSITION pos;
								pos = graphSeries->GetHeadPosition();
								CGraphSeries* tmpSeries;
								  
								for (y = 1; y <= graphSeries->GetCount(); ++ y)  
								{
									tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);

									seriesSpace = yAxisHeight / graphSeries->GetCount();
									tickYLocation = yApexPoint - ((y * seriesSpace) - (seriesSpace / 2));

									//draw tick mark
									pDC->MoveTo(xApexPoint - 3, tickYLocation);
									pDC->LineTo(xApexPoint + 3, tickYLocation);

									if(!inRedraw)
									{
										//draw tick label
										CString tickLabel;
										tickLabel = tmpSeries->GetLabel();

										CFont yFont;
										yFont.CreateFont(yTickFontSize, 0, 0, 0, 700, FALSE, FALSE, 0,
											ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
											DEFAULT_QUALITY, FIXED_PITCH | FF_ROMAN,"Times New Roman");
										pDC->SelectObject(&yFont);
										charWidth = tm.tmAveCharWidth;

										pDC->TextOut(xApexPoint - (xAxisLabelLength * charWidth) - 5, tickYLocation - (tm.tmHeight / 2), tickLabel);
									}
									topYTick = tickYLocation;
									pDC->SelectObject(pOldFont);
								}
							}
							
							//draw X axis tick marks
							tickScale = (xAxisWidth - 10) / numTicks;
							//  
							for (x = 1; x <= numTicks; ++ x)
							{
								tickXLocation = xApexPoint + (int)(x * tickScale);

								pDC->MoveTo(tickXLocation,yApexPoint - 3);
								pDC->LineTo(tickXLocation,yApexPoint + 3);

								if(!inRedraw)
								{
									if(graphHasGridLines)
									{
										//draw grid lines
										COLORREF gridLineColor;
										gridLineColor = DARK_GREY;
										CPen gridPen (PS_SOLID, 1, gridLineColor);
										CPen* pOldPen;
										pOldPen = pDC->SelectObject(&gridPen);
										pDC->MoveTo(tickXLocation, yApexPoint);
										pDC->LineTo(tickXLocation, yApexPoint - yAxisHeight);
										pDC->SelectObject(pOldPen);
									}

									//draw tick label
									CString tickLabel;
									tickLabel.Format("%.4f", minTick + (x * tickSpace));
									if(!xAxisAlign)  //horizontal
									{
										CFont xFont;
										xFont.CreateFont(xTickFontSize, 0, 0, 0, 700, FALSE, FALSE, 0,
											ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
											DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Times New Roman");
										pDC->SelectObject(&xFont);
										charWidth = tm.tmAveCharWidth;
										pDC->TextOut(tickXLocation - ((tickLabel.GetLength() * charWidth) / 2), yApexPoint + 8, tickLabel);
										pDC->SelectObject(pOldFont);
									}
									else
									{
										CFont sideFont2;
										sideFont2.CreateFont(xTickFontSize, 0, (xAxisAlign * 10), 0, 700, FALSE, FALSE, 0,
											ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
											DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Times New Roman");
										pDC->SelectObject(&sideFont2);
										pDC->GetTextMetrics(&tm);
										charWidth = tm.tmAveCharWidth;
										if(xAxisAlign < 180)
											pDC->TextOut(tickXLocation - 8, yApexPoint + 8 + (xAxisLabelLength * charWidth), tickLabel);
										else
											pDC->TextOut(tickXLocation + 2, yApexPoint + 8, tickLabel);

										pDC->SelectObject(pOldFont);
									}
								}
								rightXTick = tickXLocation;
							}
							break;
						case 2 :
							pDC->TextOut(10, (yApexPoint - (yAxisHeight / 2)) + (axisYLabel.GetLength() / 2), axisXLabel);

							pDC->SelectObject(&axisFont);
							pDC->GetTextMetrics(&tm);
							charWidth = tm.tmAveCharWidth;
							labelHeight = tm.tmHeight + 10;
							if(axisYLabel.GetLength() == 0)
								labelHeight = 0;
							pDC->TextOut(xApexPoint - ((axisXLabel.GetLength() / 2) * charWidth), maxHeight - 5 - 6, axisYLabel);
							
							pDC->SelectObject(pOldFont);

							//to allow scalability (height may be less than tickRange)
							tickScale = 0.00;
							seriesSpace = yAxisHeight / graphSeries->GetCount();

							//draw y axis ticks
							{
								tickScale = (yAxisHeight - 10) / seriesSize;
								POSITION pos;
								pos = graphSeries->GetHeadPosition();
								CGraphSeries* tmpSeries;
								  
								for (y = 1; y <= graphSeries->GetCount(); ++ y)  
								{
									tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);

									tickYLocation = yApexPoint - ((y * seriesSpace) - (seriesSpace / 2));

									//draw tick mark
									pDC->MoveTo(xApexPoint - 3, tickYLocation);
									pDC->LineTo(xApexPoint + 3, tickYLocation);

									if(!inRedraw)
									{
										//draw tick label
										CString tickLabel;
										tickLabel = tmpSeries->GetLabel();

										CFont yFont;
										yFont.CreateFont(yTickFontSize, 0, 0, 0, 700, FALSE, FALSE, 0,
											ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
											DEFAULT_QUALITY, FIXED_PITCH | FF_ROMAN,"Times New Roman");
										pDC->SelectObject(&yFont);
										charWidth = tm.tmAveCharWidth;

										pDC->TextOut(xApexPoint - (xAxisWidth / 2) - (xAxisLabelLength * charWidth) - 5, tickYLocation - (tm.tmHeight / 2), tickLabel);
									}
									topYTick = tickYLocation;
									pDC->SelectObject(pOldFont);
								}
								topYTick = tickYLocation;
								pDC->SelectObject(pOldFont);
							}
							
							//draw X axis tick marks
							tickScale = (xAxisWidth - 10) / numTicks;
							//  
							for (x = 0; x <= numTicks / 2; ++ x)
							{
								tickXLocation = xApexPoint - (xAxisWidth / 2) + (int)(x * tickScale) + 5;

								if(!inRedraw)
								{
									pDC->MoveTo(tickXLocation,yApexPoint - 3);
									pDC->LineTo(tickXLocation,yApexPoint + 3);

									if(graphHasGridLines)
									{
										//draw grid lines
										COLORREF gridLineColor;
										gridLineColor = DARK_GREY;
										CPen gridPen (PS_SOLID, 1, gridLineColor);
										CPen* pOldPen;
										pOldPen = pDC->SelectObject(&gridPen);
										pDC->MoveTo(tickXLocation, yApexPoint - yAxisHeight);
										pDC->LineTo(tickXLocation, yApexPoint);
										pDC->SelectObject(pOldPen);
									}

									//draw tick label
									CString tickLabel;
									tickLabel.Format("%.4f", minTick + (x * tickSpace));
									if(!xAxisAlign)  //horizontal
									{
										CFont xFont;
										xFont.CreateFont(xTickFontSize, 0, 0, 0, 700, FALSE, FALSE, 0,
											ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
											DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Times New Roman");
										pDC->SelectObject(&xFont);
										charWidth = tm.tmAveCharWidth;
										pDC->TextOut(tickXLocation - ((tickLabel.GetLength() * charWidth) / 2), yApexPoint + 8, tickLabel);
										pDC->SelectObject(pOldFont);
									}
									else
									{
										CFont sideFont2;
										sideFont2.CreateFont(xTickFontSize, 0, (xAxisAlign * 10), 0, 700, FALSE, FALSE, 0,
											ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
											DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Times New Roman");
										pDC->SelectObject(&sideFont2);
										pDC->GetTextMetrics(&tm);
										charWidth = tm.tmAveCharWidth;
										if(xAxisAlign < 180)
											pDC->TextOut(tickXLocation - 8, yApexPoint + 8 + (xAxisLabelLength * charWidth), tickLabel);
										else
											pDC->TextOut(tickXLocation + 2, yApexPoint + 8, tickLabel);

										pDC->SelectObject(pOldFont);
									}
								}
								rightXTick = tickXLocation;
							}
							//draw the 0 location
							tickXLocation = xApexPoint - (xAxisWidth / 2) + (int)(x * tickScale);
							if(!inRedraw)
							{
								//draw tick label
								if(!xAxisAlign)  //horizontal
								{
									CFont xFont;
									xFont.CreateFont(xTickFontSize, 0, 0, 0, 700, FALSE, FALSE, 0,
										ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
										DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Times New Roman");
									pDC->SelectObject(&xFont);
									charWidth = tm.tmAveCharWidth;
									pDC->TextOut(tickXLocation - (charWidth / 2), yApexPoint + 8, "0");
									pDC->SelectObject(pOldFont);
								}
								else
								{
									CFont sideFont2;
									sideFont2.CreateFont(xTickFontSize, 0, (xAxisAlign * 10), 0, 700, FALSE, FALSE, 0,
										ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
										DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Times New Roman");
									pDC->SelectObject(&sideFont2);
									pDC->GetTextMetrics(&tm);
									charWidth = tm.tmAveCharWidth;
									if(xAxisAlign < 180)
										pDC->TextOut(tickXLocation - 8, yApexPoint + 8 + (xAxisLabelLength * charWidth), "0");
									else
										pDC->TextOut(tickXLocation + 2, yApexPoint + 8, "0");

									pDC->SelectObject(pOldFont);
								}
							}

							//  
							for (; x <= numTicks; ++ x)
							{
								tickXLocation = xApexPoint - (xAxisWidth / 2) + (int)(x * tickScale);

								pDC->MoveTo(tickXLocation,yApexPoint - 3);
								pDC->LineTo(tickXLocation,yApexPoint + 3);

								if(!inRedraw)
								{
									if(graphHasGridLines)
									{
										//draw grid lines
										COLORREF gridLineColor;
										gridLineColor = DARK_GREY;
										CPen gridPen (PS_SOLID, 1, gridLineColor);
										CPen* pOldPen;
										pOldPen = pDC->SelectObject(&gridPen);
										pDC->MoveTo(tickXLocation, yApexPoint - yAxisHeight);
										pDC->LineTo(tickXLocation, yApexPoint);
										pDC->SelectObject(pOldPen);
									}

									//draw tick label
									CString tickLabel;
									tickLabel.Format("%.4f", minTick + (x * tickSpace));
									if(!xAxisAlign)  //horizontal
									{
										CFont xFont;
										xFont.CreateFont(xTickFontSize, 0, 0, 0, 700, FALSE, FALSE, 0,
											ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
											DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Times New Roman");
										pDC->SelectObject(&xFont);
										charWidth = tm.tmAveCharWidth;
										pDC->TextOut(tickXLocation - ((tickLabel.GetLength() * charWidth) / 2), yApexPoint + 8, tickLabel);
										pDC->SelectObject(pOldFont);
									}
									else
									{
										CFont sideFont2;
										sideFont2.CreateFont(xTickFontSize, 0, (xAxisAlign * 10), 0, 700, FALSE, FALSE, 0,
											ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
											DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Times New Roman");
										pDC->SelectObject(&sideFont2);
										pDC->GetTextMetrics(&tm);
										charWidth = tm.tmAveCharWidth;
										if(xAxisAlign < 180)
											pDC->TextOut(tickXLocation - 8, yApexPoint + 8 + (xAxisLabelLength * charWidth), tickLabel);
										else
											pDC->TextOut(tickXLocation + 2, yApexPoint + 8, tickLabel);

										pDC->SelectObject(pOldFont);
									}
								}
								rightXTick = tickXLocation;
							}
							break;
						case 3 :
							pDC->TextOut(10, yApexPoint - (axisYLabel.GetLength() / 2), axisXLabel);

							pDC->SelectObject(&axisFont);
							pDC->GetTextMetrics(&tm);
							charWidth = tm.tmAveCharWidth;
							labelHeight = tm.tmHeight + 10;
							if(axisYLabel.GetLength() == 0)
								labelHeight = 0;
							pDC->TextOut(xApexPoint + (xAxisWidth / 2) - ((axisXLabel.GetLength() / 2) * charWidth), maxHeight - 5 - 6, axisYLabel);
							
							pDC->SelectObject(pOldFont);

							//to allow scalability (height may be less than tickRange)
							tickScale = 0.00;
							seriesSpace = yAxisHeight / graphSeries->GetCount();

							//draw y axis ticks
							{
								tickScale = (yAxisHeight - 10) / seriesSize;
								POSITION pos;
								pos = graphSeries->GetHeadPosition();
								CGraphSeries* tmpSeries;
								  
								for (y = 1; y <= graphSeries->GetCount() / 2; ++ y)  
								{
									tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);

									tickYLocation = yApexPoint - (yAxisHeight / 2) + ((y * seriesSpace) - (seriesSpace / 2));

									//draw tick mark
									pDC->MoveTo(xApexPoint - 3, tickYLocation);
									pDC->LineTo(xApexPoint + 3, tickYLocation);

									if(!inRedraw)
									{
										//draw tick label
										CString tickLabel;
										tickLabel = tmpSeries->GetLabel();

										CFont yFont;
										yFont.CreateFont(yTickFontSize, 0, 0, 0, 700, FALSE, FALSE, 0,
											ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
											DEFAULT_QUALITY, FIXED_PITCH | FF_ROMAN,"Times New Roman");
										pDC->SelectObject(&yFont);
										charWidth = tm.tmAveCharWidth;

										pDC->TextOut(xApexPoint - 5 - (tickLabel.GetLength() * (yTickFontSize / 2)), tickYLocation - 6, tickLabel);
									}
									topYTick = tickYLocation;
									pDC->SelectObject(pOldFont);
								}
								//draw the 0 location
								tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);
								tickYLocation = yApexPoint;
								//draw tick mark
								pDC->MoveTo(xApexPoint - 3, tickYLocation);
								pDC->LineTo(xApexPoint + 3, tickYLocation);

								if(!inRedraw)
								{
									//draw tick label
									CString tickLabel;
									tickLabel = tmpSeries->GetLabel();

									CFont yFont;
									yFont.CreateFont(yTickFontSize, 0, 0, 0, 700, FALSE, FALSE, 0,
										ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
										DEFAULT_QUALITY, FIXED_PITCH | FF_ROMAN,"Times New Roman");
									pDC->SelectObject(&yFont);
									charWidth = tm.tmAveCharWidth;

									pDC->TextOut(xApexPoint - 5 - (tickLabel.GetLength() * (yTickFontSize / 2)), tickYLocation - 6, tickLabel);
								}
								topYTick = tickYLocation;
								pDC->SelectObject(pOldFont);
								++ y;

								//  
								for (; y <= graphSeries->GetCount(); ++ y)  
								{
									tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);

									tickYLocation = yApexPoint - (yAxisHeight / 2) + ((y * seriesSpace) - (seriesSpace / 2));

									//draw tick mark
									pDC->MoveTo(xApexPoint - 3, tickYLocation);
									pDC->LineTo(xApexPoint + 3, tickYLocation);

									if(!inRedraw)
									{
										//draw tick label
										CString tickLabel;
										tickLabel = tmpSeries->GetLabel();

										CFont yFont;
										yFont.CreateFont(yTickFontSize, 0, 0, 0, 700, FALSE, FALSE, 0,
											ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
											DEFAULT_QUALITY, FIXED_PITCH | FF_ROMAN,"Times New Roman");
										pDC->SelectObject(&yFont);
										charWidth = tm.tmAveCharWidth;

										pDC->TextOut(xApexPoint - 5 - (tickLabel.GetLength() * (yTickFontSize / 2)), tickYLocation - 6, tickLabel);
									}
									topYTick = tickYLocation;
									pDC->SelectObject(pOldFont);
								}
							}
							
							//draw X axis tick marks
							double oldNumTicks;
							oldNumTicks = numTicks;
							numTicks /= 2;
							tickScale = (xAxisWidth - 10) / numTicks;

							//  
							for (x = 0; x <= numTicks; ++ x)
							{
								tickXLocation = xApexPoint + (int)(x * tickScale);

								if(!inRedraw)
								{
									pDC->MoveTo(tickXLocation,yApexPoint - 3);
									pDC->LineTo(tickXLocation,yApexPoint + 3);

									if(graphHasGridLines)
									{
										//draw grid lines
										COLORREF gridLineColor;
										gridLineColor = DARK_GREY;
										CPen gridPen (PS_SOLID, 1, gridLineColor);
										CPen* pOldPen;
										pOldPen = pDC->SelectObject(&gridPen);
										pDC->MoveTo(tickXLocation, yApexPoint - (yAxisHeight / 2));
										pDC->LineTo(tickXLocation, yApexPoint + (yAxisHeight / 2));
										pDC->SelectObject(pOldPen);
									}

									//draw tick label
									CString tickLabel;
									tickLabel.Format("%.4f", (x * tickSpace * 2));
									if(!xAxisAlign)  //horizontal
									{
										CFont xFont;
										xFont.CreateFont(xTickFontSize, 0, 0, 0, 700, FALSE, FALSE, 0,
											ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
											DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Times New Roman");
										pDC->SelectObject(&xFont);
										charWidth = tm.tmAveCharWidth;
										pDC->TextOut(tickXLocation - ((tickLabel.GetLength() * charWidth) / 2), yApexPoint + (yAxisHeight / 2) + 8, tickLabel);
										pDC->SelectObject(pOldFont);
									}
									else
									{
										CFont sideFont2;
										sideFont2.CreateFont(xTickFontSize, 0, (xAxisAlign * 10), 0, 700, FALSE, FALSE, 0,
											ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
											DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Times New Roman");
										pDC->SelectObject(&sideFont2);
										pDC->GetTextMetrics(&tm);
										charWidth = tm.tmAveCharWidth;
										if(xAxisAlign < 180)
											pDC->TextOut(tickXLocation - 8, yApexPoint + (yAxisHeight / 2) + 8 + (xAxisLabelLength * charWidth), tickLabel);
										else
											pDC->TextOut(tickXLocation + 2, yApexPoint + (yAxisHeight / 2) + 8, tickLabel);

										pDC->SelectObject(pOldFont);
									}
								}
							}
							numTicks = oldNumTicks;
							rightXTick = tickXLocation;
							break;
						case 4 :
							pDC->TextOut(10, yApexPoint - (axisYLabel.GetLength() / 2), axisXLabel);

							pDC->SelectObject(&axisFont);
							pDC->GetTextMetrics(&tm);
							charWidth = tm.tmAveCharWidth;
							labelHeight = tm.tmHeight + 10;
							if(axisYLabel.GetLength() == 0)
								labelHeight = 0;
							pDC->TextOut(xApexPoint - ((axisXLabel.GetLength() / 2) * charWidth), maxHeight - 5 - 6, axisYLabel);
							
							pDC->SelectObject(pOldFont);

							//to allow scalability (height may be less than tickRange)
							tickScale = 0.00;
							seriesSpace = yAxisHeight / graphSeries->GetCount();

							//draw y axis ticks
							{
								tickScale = (yAxisHeight - 10) / seriesSize;
								POSITION pos;
								pos = graphSeries->GetHeadPosition();
								CGraphSeries* tmpSeries;
								  
								for (y = 1; y <= graphSeries->GetCount() / 2; ++ y)  
								{
									tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);

									tickYLocation = yApexPoint - (yAxisHeight / 2) + ((y * seriesSpace) - (seriesSpace / 2));

									//draw tick mark
									pDC->MoveTo(xApexPoint - 3, tickYLocation);
									pDC->LineTo(xApexPoint + 3, tickYLocation);

									if(!inRedraw)
									{
										//draw tick label
										CString tickLabel;
										tickLabel = tmpSeries->GetLabel();

										CFont yFont;
										yFont.CreateFont(yTickFontSize, 0, 0, 0, 700, FALSE, FALSE, 0,
											ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
											DEFAULT_QUALITY, FIXED_PITCH | FF_ROMAN,"Times New Roman");
										pDC->SelectObject(&yFont);
										charWidth = tm.tmAveCharWidth;

										pDC->TextOut(xApexPoint - (xAxisWidth / 2) - (xAxisLabelLength * charWidth) - 5, tickYLocation - (tm.tmHeight / 2), tickLabel);
									}
									topYTick = tickYLocation;
									pDC->SelectObject(pOldFont);
								}
								//draw the 0 location
								tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);
								tickYLocation = yApexPoint;
								//draw tick mark
								pDC->MoveTo(xApexPoint - 3, tickYLocation);
								pDC->LineTo(xApexPoint + 3, tickYLocation);

								if(!inRedraw)
								{
									//draw tick label
									CString tickLabel;
									tickLabel = tmpSeries->GetLabel();

									CFont yFont;
									yFont.CreateFont(yTickFontSize, 0, 0, 0, 700, FALSE, FALSE, 0,
										ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
										DEFAULT_QUALITY, FIXED_PITCH | FF_ROMAN,"Times New Roman");
									pDC->SelectObject(&yFont);
									charWidth = tm.tmAveCharWidth;

									pDC->TextOut(xApexPoint - (xAxisWidth / 2) - (xAxisLabelLength * charWidth) - 5, tickYLocation - (tm.tmHeight / 2), tickLabel);
								}
								topYTick = tickYLocation;
								pDC->SelectObject(pOldFont);
								++ y;

								//  
								for (; y <= graphSeries->GetCount(); ++ y)  
								{
									tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);

									tickYLocation = yApexPoint - (yAxisHeight / 2) + ((y * seriesSpace) - (seriesSpace / 2));

									//draw tick mark
									pDC->MoveTo(xApexPoint - 3, tickYLocation);
									pDC->LineTo(xApexPoint + 3, tickYLocation);

									if(!inRedraw)
									{
										//draw tick label
										CString tickLabel;
										tickLabel = tmpSeries->GetLabel();

										CFont yFont;
										yFont.CreateFont(yTickFontSize, 0, 0, 0, 700, FALSE, FALSE, 0,
											ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
											DEFAULT_QUALITY, FIXED_PITCH | FF_ROMAN,"Times New Roman");
										pDC->SelectObject(&yFont);
										charWidth = tm.tmAveCharWidth;

										pDC->TextOut(xApexPoint - (xAxisWidth / 2) - (xAxisLabelLength * charWidth) - 5, tickYLocation - (tm.tmHeight / 2), tickLabel);
									}
									topYTick = tickYLocation;
									pDC->SelectObject(pOldFont);
								}
							}

							//draw X axis tick marks
							tickScale = (xAxisWidth - 10) / numTicks;
							//  
							for (x = 0; x <= numTicks / 2; ++ x)
							{
								tickXLocation = xApexPoint - (xAxisWidth / 2) + (int)(x * tickScale) + 5;

								if(!inRedraw)
								{
									pDC->MoveTo(tickXLocation,yApexPoint - 3);
									pDC->LineTo(tickXLocation,yApexPoint + 3);

									if(graphHasGridLines)
									{
										//draw grid lines
										COLORREF gridLineColor;
										gridLineColor = DARK_GREY;
										CPen gridPen (PS_SOLID, 1, gridLineColor);
										CPen* pOldPen;
										pOldPen = pDC->SelectObject(&gridPen);
										pDC->MoveTo(tickXLocation, yApexPoint - (yAxisHeight / 2));
										pDC->LineTo(tickXLocation, yApexPoint + (yAxisHeight / 2));
										pDC->SelectObject(pOldPen);
									}

									//draw tick label
									CString tickLabel;
									tickLabel.Format("%.4f", minTick + (x * tickSpace));
									if(!xAxisAlign)  //horizontal
									{
										CFont xFont;
										xFont.CreateFont(xTickFontSize, 0, 0, 0, 700, FALSE, FALSE, 0,
											ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
											DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Times New Roman");
										pDC->SelectObject(&xFont);
										charWidth = tm.tmAveCharWidth;
										pDC->TextOut(tickXLocation - ((tickLabel.GetLength() * charWidth) / 2), yApexPoint + (yAxisHeight / 2) + 8, tickLabel);
										pDC->SelectObject(pOldFont);
									}
									else
									{
										CFont sideFont2;
										sideFont2.CreateFont(xTickFontSize, 0, (xAxisAlign * 10), 0, 700, FALSE, FALSE, 0,
											ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
											DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Times New Roman");
										pDC->SelectObject(&sideFont2);
										pDC->GetTextMetrics(&tm);
										charWidth = tm.tmAveCharWidth;
										if(xAxisAlign < 180)
											pDC->TextOut(tickXLocation - 8, yApexPoint + (yAxisHeight / 2) + 8 + (xAxisLabelLength * charWidth), tickLabel);
										else
											pDC->TextOut(tickXLocation + 2, yApexPoint + (yAxisHeight / 2) + 8, tickLabel);

										pDC->SelectObject(pOldFont);
									}
								}
								rightXTick = tickXLocation;
							}
							//draw the 0 location
							tickXLocation = xApexPoint - (xAxisWidth / 2) + (int)(x * tickScale);
							if(!inRedraw)
							{
								if(graphHasGridLines)
								{
									//draw grid lines
									COLORREF gridLineColor;
									gridLineColor = DARK_GREY;
									CPen gridPen (PS_SOLID, 1, gridLineColor);
									CPen* pOldPen;
									pOldPen = pDC->SelectObject(&gridPen);
									pDC->MoveTo(tickXLocation, yApexPoint - (yAxisHeight / 2));
									pDC->LineTo(tickXLocation, yApexPoint + (yAxisHeight / 2));
									pDC->SelectObject(pOldPen);
								}

								//draw tick label
								if(!xAxisAlign)  //horizontal
								{
									CFont xFont;
									xFont.CreateFont(xTickFontSize, 0, 0, 0, 700, FALSE, FALSE, 0,
										ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
										DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Times New Roman");
									pDC->SelectObject(&xFont);
									charWidth = tm.tmAveCharWidth;
									pDC->TextOut(tickXLocation - (charWidth / 2), yApexPoint + (yAxisHeight / 2) + 8, "0");
									pDC->SelectObject(pOldFont);
								}
								else
								{
									CFont sideFont2;
									sideFont2.CreateFont(xTickFontSize, 0, (xAxisAlign * 10), 0, 700, FALSE, FALSE, 0,
										ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
										DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Times New Roman");
									pDC->SelectObject(&sideFont2);
									pDC->GetTextMetrics(&tm);
									charWidth = tm.tmAveCharWidth;
									if(xAxisAlign < 180)
										pDC->TextOut(tickXLocation - 8, yApexPoint + (yAxisHeight / 2) + 8 + (xAxisLabelLength * charWidth), "0");
									else
										pDC->TextOut(tickXLocation + 2, yApexPoint + (yAxisHeight / 2) + 8, "0");

									pDC->SelectObject(pOldFont);
								}
							}
							++ x;

							//  
							for (; x <= numTicks; ++ x)
							{
								tickXLocation = xApexPoint - (xAxisWidth / 2) + (int)(x * tickScale);

								pDC->MoveTo(tickXLocation,yApexPoint - 3);
								pDC->LineTo(tickXLocation,yApexPoint + 3);

								if(!inRedraw)
								{
									if(graphHasGridLines)
									{
										//draw grid lines
										COLORREF gridLineColor;
										gridLineColor = DARK_GREY;
										CPen gridPen (PS_SOLID, 1, gridLineColor);
										CPen* pOldPen;
										pOldPen = pDC->SelectObject(&gridPen);
										pDC->MoveTo(tickXLocation, yApexPoint - (yAxisHeight / 2));
										pDC->LineTo(tickXLocation, yApexPoint + (yAxisHeight / 2));
										pDC->SelectObject(pOldPen);
									}

									//draw tick label
									CString tickLabel;
									tickLabel.Format("%.4f", (x / 2) * tickSpace);
									if(!xAxisAlign)  //horizontal
									{
										CFont xFont;
										xFont.CreateFont(xTickFontSize, 0, 0, 0, 700, FALSE, FALSE, 0,
											ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
											DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Times New Roman");
										pDC->SelectObject(&xFont);
										charWidth = tm.tmAveCharWidth;
										pDC->TextOut(tickXLocation - ((tickLabel.GetLength() * charWidth) / 2), yApexPoint + (yAxisHeight / 2) + 8, tickLabel);
										pDC->SelectObject(pOldFont);
									}
									else
									{
										CFont sideFont2;
										sideFont2.CreateFont(xTickFontSize, 0, (xAxisAlign * 10), 0, 700, FALSE, FALSE, 0,
											ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
											DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Times New Roman");
										pDC->SelectObject(&sideFont2);
										pDC->GetTextMetrics(&tm);
										charWidth = tm.tmAveCharWidth;
										if(xAxisAlign < 180)
											pDC->TextOut(tickXLocation - 8, yApexPoint + (yAxisHeight / 2) + 8 + (xAxisLabelLength * charWidth), tickLabel);
										else
											pDC->TextOut(tickXLocation + 2, yApexPoint + (yAxisHeight / 2) + 8, tickLabel);

										pDC->SelectObject(pOldFont);
									}
								}
								rightXTick = tickXLocation;
							}
							break;
					}
				}
				break;
	}
}

void CGraph::AddSeries(CGraphSeries* dataSet)
{
	int numData = 0;

	numData = dataSet->GetDataCount();

	if(numData > seriesSize)
	{
		seriesSize = numData;
	}

	graphSeries->AddTail(dataSet);
}


void CGraph::SetXAxisLabel(CString label)
{
	axisXLabel = label;
	xAxisLabelLength = axisXLabel.GetLength();
}

void CGraph::SetYAxisLabel(CString label)
{
	axisYLabel = label;
	yAxisLabelLength = axisYLabel.GetLength();
}

void CGraph::DrawSeries(CDC* pDC)
{
	switch(graphType)
	{
		case BAR_GRAPH :
				 DrawBarSeries(pDC);
				 break;
		case LINE_GRAPH :
				 DrawLineSeries(pDC);
				 break;
		default :
				 AfxMessageBox("无需绘制的图形");
				 break;
	}

}

void CGraph::SetLegend(int datagroup, CString label)
{
		graphLegend.SetLegendText(datagroup, label);
		graphHasLegend = TRUE;
		if(legendMaxText < label.GetLength())
			legendMaxText = label.GetLength();
}

int CGraph::DrawLegend(CDC* pDC)
{
	//determine size of legend
	//12 chars per seriesSize + 6 for spacing (3 top and bottom) 
	//+ 1 set for label title(3+12+6) + rectangle (2 chars) + 3 for final bottom buffer
	int legendHeight = 23 + (seriesSize * 18) + 3;
	int legendL, legendT, legendR, legendB;
	int barL, barT, barR, barB;
	TEXTMETRIC tm;

	legendT = (maxHeight / 2) - (legendHeight / 2);
	legendB = legendT + legendHeight;
	legendR = maxWidth - 5;
	legendL = legendR - ((graphLegend.GetLegendLength() * (int)(legendFontSize * .66)) + 25);
		//allows 50 pixels for display of legend bar 45 + 5 space.
	legendWidth = legendR - legendL;

	pDC->Rectangle(legendL, legendT, legendR, legendB);
	CFont legendTitleFont;
	legendTitleFont.CreateFont(16, 0, 0, 0, 700, FALSE, FALSE, 0,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Times New Roman");
	CFont* pOldFont = (CFont*) pDC->SelectObject(&legendTitleFont);
	pDC->GetTextMetrics(&tm);
	int charWidth = tm.tmAveCharWidth;
	pDC->TextOut(legendL + (legendWidth / 2) - (3 * charWidth), 
					legendT + 3, "Legend");
	pDC->SelectObject(pOldFont);
	  
	for (int i = 0; i < seriesSize; ++ i)
	{
		//top "Legend" text will use 12 + 3 top + 6 bottom (21 total)
		//each legend label will need 3 chars on top, so the 24 in the offset
		//each label than uses 12 + 3 below + 3 above next label, so 18
		// in the i * offset.  
		CFont legendFont;
		legendFont.CreateFont(legendFontSize, 0, 0, 0, 700, FALSE, FALSE, 0,
			ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Times New Roman");
		pOldFont = (CFont*) pDC->SelectObject(&legendFont);

		pDC->TextOut(legendL + 5, legendT + 24 + (i * 18) + 1, graphLegend.GetLegendText(i));

		pDC->SelectObject(pOldFont);

		//draw bar
		COLORREF barColor;
		barColor = GetColor(i);
		CBrush brush (barColor);
		CBrush* pOldBrush;
		pOldBrush = pDC->SelectObject(&brush);

		barL = legendL + 5 + (legendMaxText * (legendFontSize / 2)) + 5;
		barT = legendT + 24 + (i * 18) + 1, graphLegend.GetLegendText(i);
		barR = legendR - 5;
		barB = barT + 12;
		pDC->Rectangle(barL, barT, barR, barB);

		pDC->SelectObject(pOldBrush);
	}

	return legendL;
}

void CGraph::SetGraphTitle(CString title)
{
	graphTitle = title;
}


void CGraph::SetMargins(int top, int bottom, int left, int right, int graphTop)
{
	//page margins for portrait or landscape distinction
	topMargin = top;
	bottomMargin = bottom;
	leftMargin = left;
	rightMargin = right;

	//set up rectangle area for showing the graph
	pGraphL = leftMargin;
	pGraphT = graphTop;
	pGraphB = pGraphT - 5760/*7200*/;
	pGraphR = rightMargin;

	if(pGraphB < bottomMargin)
		pGraphB = bottomMargin; 

}

void CGraph::SetGridLines(BOOL hasGridLines)
{
	graphHasGridLines = hasGridLines;
}

void CGraph::SetGraphAlignment(int alignment)
{
	graphAlignment = alignment;
	//because of how it ends up looking, I'm forcing 3D line graphs
	//to always display as if in vertical mode graph
}

void CGraph::SetTickLimits(double minTickValue, double maxTickValue, double tickStep)
{
	maxTick = maxTickValue;
	minTick = minTickValue;
	tickSpace = tickStep;
	numTicks = (maxTick - minTick) / tickStep;
	if((numTicks * tickStep) < (maxTick - minTick))
		++ numTicks;
}

void CGraph::DrawBarSeries(CDC* pDC)
{
	int barWidth;
	int dataPlotSize;   //used to plot rects of data
	int barL, barT, barR, barB;
	int tickXLocation, tickYLocation;
	int seriesSpace;
	int barHeight;
	POSITION pos;
	CGraphSeries* tmpSeries;
	int x, y, s;

	if(graphAlignment)
	{
		//determine width of barchart data blocks
		seriesSpace = xAxisWidth / graphSeries->GetCount();
		barWidth = (int)((seriesSpace * .6) / seriesSize);
		dataPlotSize = seriesSize * barWidth;

		pos = graphSeries->GetHeadPosition();

		switch(graphQuadType)
		{
			case 1 :
				  
				for (x = 1; x <= graphSeries->GetCount(); ++ x)
				{
					tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);

					tickXLocation = xApexPoint + ((x * seriesSpace) - (seriesSpace / 2));

					  
					for (s = 0; s < seriesSize; ++ s)
					{
						if(tmpSeries->GetData(s) > -1)
						{
							double dataScale = 0.00;
							dataScale = ((yAxisHeight - 10) * 1.00) / (maxTick - minTick);
							barHeight = (int)(((tmpSeries->GetData(s) - minTick) * 1.00) * dataScale);
							if(((tmpSeries->GetData(s) - minTick) > 0) && (barHeight < 1))
								barHeight = (int)dataScale;

							barL = tickXLocation - (dataPlotSize / 2) + (s * barWidth);
							if(barHeight > yAxisHeight)
								barT = yApexPoint - yAxisHeight;
							else
								barT = yApexPoint - barHeight;
							barR = barL + barWidth;
							barB = yApexPoint;
							
							COLORREF barColor;
							barColor = GetColor(s);
							CBrush brush (barColor);
							CBrush* pOldBrush;
							pOldBrush = pDC->SelectObject(&brush);
							pDC->Rectangle(barL, barT, barR, barB);
							pDC->SelectObject(pOldBrush);
						}
					}
				}
				break;
			case 2 :
				  
				for (x = 1; x <= graphSeries->GetCount() / 2; ++ x)
				{
					tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);

					tickXLocation = xApexPoint - (xAxisWidth / 2) + ((x * seriesSpace) - (seriesSpace / 2));

					  
					for (s = 0; s < seriesSize; ++ s)
					{
						double dataScale = 0.00;
						dataScale = ((yAxisHeight - 10) * 1.00) / (maxTick - minTick);
						barHeight = (int)(((tmpSeries->GetData(s) - minTick) * 1.00) * dataScale);
						if(barHeight < 1)
							barHeight = (int)dataScale;

						barL = tickXLocation - (dataPlotSize / 2) + (s * barWidth);
						if(barHeight > (yAxisHeight / 2))
							barT = yApexPoint - (yAxisHeight / 2);
						else
							barT = yApexPoint - barHeight;
						barR = barL + barWidth;
						barB = yApexPoint;
						
						COLORREF barColor;
						barColor = GetColor(s);
						CBrush brush (barColor);
						CBrush* pOldBrush;
						pOldBrush = pDC->SelectObject(&brush);
						pDC->Rectangle(barL, barT, barR, barB);
						pDC->SelectObject(pOldBrush);
					}
				}
				//draw at the 0 location
				tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);
				tickXLocation = xApexPoint;

				  
				for (s = 0; s < seriesSize; ++ s)
				{
					double dataScale = 0.00;
					dataScale = ((yAxisHeight - 10) * 1.00) / (maxTick - minTick);
					barHeight = (int)(((tmpSeries->GetData(s) - minTick) * 1.00) * dataScale);
					if(((tmpSeries->GetData(s) - minTick) > 0) && (barHeight < 1))
						barHeight = (int)dataScale;

					barL = tickXLocation - (dataPlotSize / 2) + (s * barWidth);
					if(barHeight > yAxisHeight)
						barT = yApexPoint - yAxisHeight;
					else
						barT = yApexPoint - barHeight;
					barR = barL + barWidth;
					barB = yApexPoint;
					
					COLORREF barColor;
					barColor = GetColor(s);
					CBrush brush (barColor);
					CBrush* pOldBrush;
					pOldBrush = pDC->SelectObject(&brush);
					pDC->Rectangle(barL, barT, barR, barB);
					pDC->SelectObject(pOldBrush);
				}
				++ x;

				//  
				for (; x <= graphSeries->GetCount(); ++ x)
				{
					tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);

					tickXLocation = xApexPoint - (xAxisWidth / 2) + ((x * seriesSpace) - (seriesSpace / 2));

					  
					for (s = 0; s < seriesSize; ++ s)
					{
						double dataScale = 0.00;
						dataScale = ((yAxisHeight - 10) * 1.00) / (maxTick - minTick);
						barHeight = (int)(((tmpSeries->GetData(s) - minTick) * 1.00) * dataScale);
						if(((tmpSeries->GetData(s) - minTick) > 0) && (barHeight < 1))
							barHeight = (int)dataScale;

						barL = tickXLocation - (dataPlotSize / 2) + (s * barWidth);
						if(barHeight > (yAxisHeight / 2))
							barT = yApexPoint - (yAxisHeight / 2);
						else
							barT = yApexPoint - barHeight;
						barR = barL + barWidth;
						barB = yApexPoint;
						
						COLORREF barColor;
						barColor = GetColor(s);
						CBrush brush (barColor);
						CBrush* pOldBrush;
						pOldBrush = pDC->SelectObject(&brush);
						pDC->Rectangle(barL, barT, barR, barB);
						pDC->SelectObject(pOldBrush);
					}
				}
				break;
			case 3 :
				  
				for (x = 1; x <= graphSeries->GetCount(); ++ x)
				{
					tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);

					tickXLocation = xApexPoint + ((x * seriesSpace) - (seriesSpace / 2));

					  
					for (s = 0; s < seriesSize; ++ s)
					{
						double dataScale = 0.00;
						dataScale = ((yAxisHeight - 10) * 1.00) / (maxTick - minTick);
						barHeight = (int)(((tmpSeries->GetData(s) - minTick) * 1.00) * dataScale);

						barL = tickXLocation - (dataPlotSize / 2) + (s * barWidth);
						if(barHeight < 0)
						{
							if((0 - barHeight) > (yAxisHeight / 2))
								barT = yApexPoint - (yAxisHeight / 2);
							else
								barT = yApexPoint - barHeight;
						}
						else
						{
							if(barHeight > (yAxisHeight / 2))
								barT = yApexPoint - (yAxisHeight / 2);
							else
								barT = yApexPoint - barHeight;
						}
						barR = barL + barWidth;
						barB = yApexPoint;
						
						COLORREF barColor;
						barColor = GetColor(s);
						CBrush brush (barColor);
						CBrush* pOldBrush;
						pOldBrush = pDC->SelectObject(&brush);
						pDC->Rectangle(barL, barT, barR, barB);
						pDC->SelectObject(pOldBrush);
					}
				}
				break;
			case 4 :
				  
				for (x = 1; x <= graphSeries->GetCount() / 2; ++ x)
				{
					tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);

					tickXLocation = xApexPoint - (xAxisWidth / 2) + ((x * seriesSpace) - (seriesSpace / 2));

					  
					for (s = 0; s < seriesSize; ++ s)
					{
						double dataScale = 0.00;
						dataScale = ((yAxisHeight - 10) * 1.00) / (maxTick - minTick);
						barHeight = (int)(((tmpSeries->GetData(s) - minTick) * 1.00) * dataScale);

						barL = tickXLocation - (dataPlotSize / 2) + (s * barWidth);
						if(barHeight < 0)
						{
							if((0 - barHeight) > (yAxisHeight / 2))
								barT = yApexPoint - (yAxisHeight / 2);
							else
								barT = yApexPoint - barHeight;
						}
						else
						{
							if(barHeight > (yAxisHeight / 2))
								barT = yApexPoint - (yAxisHeight / 2);
							else
								barT = yApexPoint - barHeight;
						}
						barR = barL + barWidth;
						barB = yApexPoint;
						
						COLORREF barColor;
						barColor = GetColor(s);
						CBrush brush (barColor);
						CBrush* pOldBrush;
						pOldBrush = pDC->SelectObject(&brush);
						pDC->Rectangle(barL, barT, barR, barB);
						pDC->SelectObject(pOldBrush);
					}
				}
				//draw at the 0 location
				tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);
				tickXLocation = xApexPoint;
				  
				for (s = 0; s < seriesSize; ++ s)
				{
					double dataScale = 0.00;
					dataScale = ((yAxisHeight - 10) * 1.00) / (maxTick - minTick);
					barHeight = (int)(((tmpSeries->GetData(s) - minTick) * 1.00) * dataScale);
					if(((tmpSeries->GetData(s) - minTick) > 0) && (barHeight < 1))
						barHeight = (int)dataScale;

					barL = tickXLocation - (dataPlotSize / 2) + (s * barWidth);
					if(barHeight < 0)
					{
						if((0 - barHeight) > (yAxisHeight / 2))
							barT = yApexPoint - (yAxisHeight / 2);
						else
							barT = yApexPoint - barHeight;
					}
					else
					{
						if(barHeight > (yAxisHeight / 2))
							barT = yApexPoint - (yAxisHeight / 2);
						else
							barT = yApexPoint - barHeight;
					}
					barR = barL + barWidth;
					barB = yApexPoint;
					
					COLORREF barColor;
					barColor = GetColor(s);
					CBrush brush (barColor);
					CBrush* pOldBrush;
					pOldBrush = pDC->SelectObject(&brush);
					pDC->Rectangle(barL, barT, barR, barB);
					pDC->SelectObject(pOldBrush);
				}
				++ x;

				//  
				for (; x <= graphSeries->GetCount(); ++ x)
				{
					tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);

					tickXLocation = xApexPoint - (xAxisWidth / 2) + ((x * seriesSpace) - (seriesSpace / 2));

					//  
					for (s = 0; s < seriesSize; ++ s)
					{
						double dataScale = 0.00;
						dataScale = ((yAxisHeight - 10) * 1.00) / (maxTick - minTick);
						barHeight = (int)(((tmpSeries->GetData(s) - minTick) * 1.00) * dataScale);
						if(((tmpSeries->GetData(s) - minTick) > 0) && (barHeight < 1))
							barHeight = (int)dataScale;

						barL = tickXLocation - (dataPlotSize / 2) + (s * barWidth);
						if(barHeight < 0)
						{
							if((0 - barHeight) > (yAxisHeight / 2))
								barT = yApexPoint - (yAxisHeight / 2);
							else
								barT = yApexPoint - barHeight;
						}
						else
						{
							if(barHeight > (yAxisHeight / 2))
								barT = yApexPoint - (yAxisHeight / 2);
							else
								barT = yApexPoint - barHeight;
						}
						barR = barL + barWidth;
						barB = yApexPoint;
						
						COLORREF barColor;
						barColor = GetColor(s);
						CBrush brush (barColor);
						CBrush* pOldBrush;
						pOldBrush = pDC->SelectObject(&brush);
						pDC->Rectangle(barL, barT, barR, barB);
						pDC->SelectObject(pOldBrush);
					}
				}
				break;
		}

	}
	else
	{
		//determine width of barchart data blocks
		seriesSpace = yAxisHeight / graphSeries->GetCount();
		barHeight = (int)((seriesSpace * .6) / seriesSize);
		dataPlotSize = seriesSize * barHeight;

		pos = graphSeries->GetHeadPosition();

		switch(graphQuadType)
		{
			case 1 :
				//  
				for (y = 1; y <= graphSeries->GetCount(); ++ y)
				{
					tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);

					tickYLocation = yApexPoint - ((y * seriesSpace) - (seriesSpace / 2));

					//  
					for (s = 0; s < seriesSize; ++ s)
					{
						double dataScale = 0.00;
						dataScale = ((xAxisWidth - 10) * 1.00) / (maxTick - minTick);

						barWidth = (int)(((tmpSeries->GetData(s) - minTick) * 1.00) * dataScale);
						if(((tmpSeries->GetData(s) - minTick) > 0) && (barWidth < 1))
							barWidth = (int)dataScale;

						barL = xApexPoint;
						barT = tickYLocation - (dataPlotSize / 2) + (s * barHeight);
						barB = barT + barHeight;
						if(barWidth > xAxisWidth)
							barR = xApexPoint + xAxisWidth;
						else
							barR = xApexPoint + barWidth;

						COLORREF barColor;
						barColor = GetColor(s);
						CBrush brush (barColor);
						CBrush* pOldBrush;
						pOldBrush = pDC->SelectObject(&brush);
						pDC->Rectangle(barL, barT, barR, barB);
						pDC->SelectObject(pOldBrush);
					}
				}
				break;
			case 2 :
				//  
				for (y = 1; y <= graphSeries->GetCount(); ++ y)
				{
					tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);

					tickYLocation = yApexPoint - ((y * seriesSpace) - (seriesSpace / 2));

					//  
					for (s = 0; s < seriesSize; ++ s)
					{
						double dataScale = 0.00;
						dataScale = ((xAxisWidth - 10) * 1.00) / (maxTick - minTick);

						barWidth = (int)(((tmpSeries->GetData(s) - minTick) * 1.00) * dataScale);
						if(((tmpSeries->GetData(s) - minTick) > 0) && (barWidth < 1))
							barWidth = (int)dataScale;

						barL = xApexPoint;
						barT = tickYLocation - (dataPlotSize / 2) + (s * barHeight);
						barB = barT + barHeight;
						if(barWidth < 0)
						{
							if((0 - barWidth) > (xAxisWidth / 2))
								barR = xApexPoint + (xAxisWidth / 2);
							else
								barR = xApexPoint + barWidth;
						}
						else
						{
							if(barWidth > (xAxisWidth / 2))
								barR = xApexPoint + (xAxisWidth / 2);
							else
								barR = xApexPoint + barWidth;
						}

						COLORREF barColor;
						barColor = GetColor(s);
						CBrush brush (barColor);
						CBrush* pOldBrush;
						pOldBrush = pDC->SelectObject(&brush);
						pDC->Rectangle(barL, barT, barR, barB);
						pDC->SelectObject(pOldBrush);
					}
				}
				break;
			case 3 :
				//  
				for (y = 1; y <= graphSeries->GetCount(); ++ y)
				{
					tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);

					tickYLocation = yApexPoint - (yAxisHeight / 2) + ((y * seriesSpace) - (seriesSpace / 2));

					//  
					for (s = 0; s < seriesSize; ++ s)
					{
						double dataScale = 0.00;
						dataScale = ((xAxisWidth - 10) * 1.00) / (maxTick - minTick);

						barWidth = (int)(((tmpSeries->GetData(s) - minTick) * 1.00) * dataScale);
						if(((tmpSeries->GetData(s) - minTick) > 0) && (barWidth < 1))
							barWidth = (int)dataScale;

						barL = xApexPoint;
						barT = tickYLocation - (dataPlotSize / 2) + (s * barHeight);
						barB = barT + barHeight;
						if(barWidth > xAxisWidth)
							barR = xApexPoint + xAxisWidth;
						else
							barR = xApexPoint + barWidth;

						COLORREF barColor;
						barColor = GetColor(s);
						CBrush brush (barColor);
						CBrush* pOldBrush;
						pOldBrush = pDC->SelectObject(&brush);
						pDC->Rectangle(barL, barT, barR, barB);
						pDC->SelectObject(pOldBrush);
					}
				}
				break;
			case 4 :
				//  
				for (y = 1; y <= graphSeries->GetCount() / 2; ++ y)
				{
					tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);

					tickYLocation = yApexPoint - (yAxisHeight / 2) + ((y * seriesSpace) - (seriesSpace / 2));

					//  
					for (s = 0; s < seriesSize; ++ s)
					{
						double dataScale = 0.00;
						dataScale = ((xAxisWidth - 10) * 1.00) / (maxTick - minTick);

						barWidth = (int)(((tmpSeries->GetData(s) - minTick) * 1.00) * dataScale);
						if(((tmpSeries->GetData(s) - minTick) > 0) && (barWidth < 1))
							barWidth = (int)dataScale;

						barL = xApexPoint;
						barT = tickYLocation - (dataPlotSize / 2) + (s * barHeight);
						barB = barT + barHeight;
						if(barWidth < 0)
						{
							if((0 - barWidth) > (xAxisWidth / 2))
								barR = xApexPoint + (xAxisWidth / 2);
							else
								barR = xApexPoint + barWidth;
						}
						else
						{
							if(barWidth > (xAxisWidth / 2))
								barR = xApexPoint + (xAxisWidth / 2);
							else
								barR = xApexPoint + barWidth;
						}

						COLORREF barColor;
						barColor = GetColor(s);
						CBrush brush (barColor);
						CBrush* pOldBrush;
						pOldBrush = pDC->SelectObject(&brush);
						pDC->Rectangle(barL, barT, barR, barB);
						pDC->SelectObject(pOldBrush);
					}
				}
				//draw the 0 location
				tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);
				tickYLocation = yApexPoint;
				//  
				for (s = 0; s < seriesSize; ++ s)
				{
					double dataScale = 0.00;
					dataScale = ((xAxisWidth - 10) * 1.00) / (maxTick - minTick);

					barWidth = (int)(((tmpSeries->GetData(s) - minTick) * 1.00) * dataScale);
					if(((tmpSeries->GetData(s) - minTick) > 0) && (barWidth < 1))
						barWidth = (int)dataScale;

					barL = xApexPoint;
					barT = tickYLocation - (dataPlotSize / 2) + (s * barHeight);
					barB = barT + barHeight;
					if(barWidth < 0)
					{
						if((0 - barWidth) > (xAxisWidth / 2))
							barR = xApexPoint + (xAxisWidth / 2);
						else
							barR = xApexPoint + barWidth;
					}
					else
					{
						if(barWidth > (xAxisWidth / 2))
							barR = xApexPoint + (xAxisWidth / 2);
						else
							barR = xApexPoint + barWidth;
					}

					COLORREF barColor;
					barColor = GetColor(s);
					CBrush brush (barColor);
					CBrush* pOldBrush;
					pOldBrush = pDC->SelectObject(&brush);
					pDC->Rectangle(barL, barT, barR, barB);
					pDC->SelectObject(pOldBrush);
				}
				++ y;
	
				//  
				for (; y <= graphSeries->GetCount(); ++ y)
				{
					tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);

					tickYLocation = yApexPoint - (yAxisHeight / 2) + ((y * seriesSpace) - (seriesSpace / 2));

					//  
					for (s = 0; s < seriesSize; ++ s)
					{
						double dataScale = 0.00;
						dataScale = ((xAxisWidth - 10) * 1.00) / (maxTick - minTick);

						barWidth = (int)(((tmpSeries->GetData(s) - minTick) * 1.00) * dataScale);
						if(((tmpSeries->GetData(s) - minTick) > 0) && (barWidth < 1))
							barWidth = (int)dataScale;

						barL = xApexPoint;
						barT = tickYLocation - (dataPlotSize / 2) + (s * barHeight);
						barB = barT + barHeight;
						if(barWidth < 0)
						{
							if((0 - barWidth) > (xAxisWidth / 2))
								barR = xApexPoint + (xAxisWidth / 2);
							else
								barR = xApexPoint + barWidth;
						}
						else
						{
							if(barWidth > (xAxisWidth / 2))
								barR = xApexPoint + (xAxisWidth / 2);
							else
								barR = xApexPoint + barWidth;
						}

						COLORREF barColor;
						barColor = GetColor(s);
						CBrush brush (barColor);
						CBrush* pOldBrush;
						pOldBrush = pDC->SelectObject(&brush);
						pDC->Rectangle(barL, barT, barR, barB);
						pDC->SelectObject(pOldBrush);
					}
				}
				break;
		}

	}
}

void CGraph::DrawLineSeries(CDC* pDC)
{
	int barWidth;
	int dataPlotSize;   //used to plot rects of data
	int tickXLocation, tickYLocation;
	int seriesSpace;
	int barHeight;
	POSITION pos;
	CGraphSeries* tmpSeries;

	int lastXLoc = 113, lastYLoc = 477;
	if(graphAlignment)
	{
		//  
		for (int s = 0; s < seriesSize; s++)
		{
			//determine width of barchart data blocks
			seriesSpace = xAxisWidth / graphSeries->GetCount();
			barWidth = (int)((seriesSpace * .6) / seriesSize);
			dataPlotSize = seriesSize * barWidth;

			pos = graphSeries->GetHeadPosition();

			//  
			for (int x = 1; x <= graphSeries->GetCount(); x++)
			{
				tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);

				if(tmpSeries->GetData(s) > -1)
				{
					tickXLocation = xApexPoint + ((x * seriesSpace) - (seriesSpace / 2));
					
					barHeight = 0;
					double dataScale = 0.00;
					
					dataScale = ((yAxisHeight - 10) * 1.00) / (maxTick - minTick);
					barHeight = (int)(((tmpSeries->GetData(s) - minTick) * 1.00) * dataScale);
					//if(((tmpSeries->GetData(s) - minTick) > 0) && (barHeight < 1))
						//barHeight = (int)dataScale;

					//if(barHeight > yApexPoint)
						//barHeight = (int)((maxTick - minTick) * dataScale);

					int divergence = yApexPoint - (int)((maxTick - minTick) * dataScale);

					int yDataLocation = yApexPoint - barHeight;
					//now have x and y location of center of ellipse
					COLORREF barColor;
					barColor = GetColor(s);
					CBrush brush (barColor);
					CBrush* pOldBrush;
					pOldBrush = pDC->SelectObject(&brush);
					//draw line back to last data member
					if(x > 1)
					{
						CPen* pOldPen;
						CPen linePen (PS_SOLID, 1, barColor);
						pOldPen = pDC->SelectObject(&linePen);
						//pDC->MoveTo(lastXLoc, lastYLoc);
						//pDC->LineTo(tickXLocation, yDataLocation);
						//pDC->MoveTo(lastXLoc + 2, lastYLoc - 1);
						//pDC->LineTo(tickXLocation - 3, yDataLocation - 1);
						
						if (yDataLocation > divergence && lastYLoc > divergence)//y轴最小刻度为477
						{
							pDC->MoveTo(lastXLoc + 2, lastYLoc - 1);
							pDC->LineTo(tickXLocation - 3, yDataLocation - 1);
							pDC->Ellipse(tickXLocation - 3, yDataLocation - 3,
						                 tickXLocation + 3, yDataLocation + 3);
						}
						else if (yDataLocation <= divergence && lastYLoc > divergence)
						{
							pDC->MoveTo(lastXLoc + 2, lastYLoc - 1);
							pDC->LineTo((int)(lastXLoc + seriesSpace / 2), divergence - 10);//x轴之间的间隔点为seriesSpace个像素点
						}
						else if (yDataLocation > divergence && lastYLoc <= divergence)
						{
							pDC->MoveTo((int)(lastXLoc + seriesSpace / 2), divergence - 10);
							pDC->LineTo(tickXLocation - 3, yDataLocation - 1);
							pDC->Ellipse(tickXLocation - 3, yDataLocation - 3,
						                 tickXLocation + 3, yDataLocation + 3);
						}
						/*else
						{
							lastXLoc = tickXLocation;
							lastYLoc = yDataLocation;
						}*/
						pDC->SelectObject(pOldPen);
					}
					//now draw ellipse...
					//pDC->Ellipse(tickXLocation - 3, yDataLocation - 3,
						//tickXLocation + 3, yDataLocation + 3);
					//pDC->Ellipse(tickXLocation - 3, yDataLocation - 3,
						//tickXLocation + 3, yDataLocation + 3);
					lastXLoc = tickXLocation;
					lastYLoc = yDataLocation;
					pDC->SelectObject(pOldBrush);
				}
			}
		}
	}
	else
	{
		//  
		for (int s = 0; s < seriesSize; ++ s)
		{
			//determine width of barchart data blocks
			seriesSpace = yAxisHeight / graphSeries->GetCount();
			barHeight = (int)((seriesSpace * .6) / seriesSize);
			dataPlotSize = seriesSize * barHeight;

			pos = graphSeries->GetHeadPosition();

			//  
			for (int x = 1; x <= graphSeries->GetCount(); ++ x)
			{
				tmpSeries = (CGraphSeries*)graphSeries->GetNext(pos);

				if(tmpSeries->GetData(s) > -1)
				{
					tickYLocation = yApexPoint - ((x * seriesSpace) - (seriesSpace / 2));
					
					barWidth = 0;
					double dataScale = 0.00;

					if((maxTick - minTick) != xAxisWidth)
						dataScale = (xAxisWidth * 1.00) / ((maxTick - minTick) * 1.00);
					else dataScale = 1;
					barWidth = (int)(((tmpSeries->GetData(s) - minTick) * 1.00) * dataScale);
					if(((tmpSeries->GetData(s) - minTick) > 0) && (barWidth < 1))
						barWidth = (int)dataScale;

					int xDataLocation = xApexPoint + barWidth;
					
					//now have x and y location of center of ellipse
					COLORREF barColor;
					barColor = GetColor(s);
					CBrush brush (barColor);
					CBrush* pOldBrush;
					pOldBrush = pDC->SelectObject(&brush);
					//draw line back to last data member
					if(x > 1)
					{
						CPen* pOldPen;
						CPen linePen (PS_SOLID, 1, barColor);
						pOldPen = pDC->SelectObject(&linePen);
						pDC->MoveTo(lastXLoc + 2, lastYLoc - 1);
						pDC->LineTo(xDataLocation - 3, tickYLocation - 1);
						pDC->SelectObject(pOldPen);
					}
					//now draw ellipse...
					pDC->Ellipse(xDataLocation - 3, tickYLocation - 3,
						xDataLocation + 3, tickYLocation + 3);
					lastYLoc = tickYLocation;
					lastXLoc = xDataLocation;
					pDC->SelectObject(pOldBrush);
				}
			}
		}
	}
}

void CGraph::RemoveSeries(CString label)
{
	POSITION pos;
	CGraphSeries* pSeries;
	pos = graphSeries->GetHeadPosition();
	while(pos != NULL)
	{
		pSeries = (CGraphSeries*)graphSeries->GetAt(pos);
		if(pSeries->GetLabel().Compare(label) == 0)
		{
			graphSeries->RemoveAt(pos);
			delete pSeries;
			break;
		}
		graphSeries->GetNext(pos);
	}
}

void CGraph::RemoveColor(int dataGroup)
{
	POSITION pos;
	CGraphDataColor* dataColor;
	pos = colorList->GetHeadPosition();
	//  
	for (int i = 0; i < dataGroup; ++ i)
	{
		dataColor = (CGraphDataColor*)colorList->GetNext(pos);
	}
	colorList->RemoveAt(pos);
	delete dataColor;
}

void CGraph::RemoveLegend(int dataGroup)
{
	graphLegend.Remove(dataGroup);
}

void CGraph::RemoveAllSeries()
{
	POSITION pos;
	CGraphSeries* pSeries;
	pos = graphSeries->GetHeadPosition();
	while(pos != NULL)
	{
		pSeries = (CGraphSeries*) graphSeries->GetAt( pos );
		graphSeries->RemoveAt(pos);
		delete pSeries;
	}

	graphSeries->RemoveAll();
}

void CGraph::RemoveAllColors()
{
	POSITION pos;
	CGraphDataColor* dataColor;
	pos = colorList->GetHeadPosition();
	while(pos != NULL)
	{
		dataColor = (CGraphDataColor*) colorList->GetAt( pos );
		colorList->RemoveAt(pos);
		delete dataColor;
	}

	colorList->RemoveAll();
}

void CGraph::RemoveAllLegends()
{
	graphLegend.RemoveAll();
}

void CGraph::RemoveAllData()
{
	RemoveAllSeries();
	RemoveAllLegends();
	RemoveAllColors();
}


void CGraph::SetGraphQuadType(int quads)
{
	if(graphType == BAR_GRAPH)
	{
		if((quads < 1) || (quads > 4))
			graphQuadType = 1;
		else
		{
			graphQuadType = quads;
			quadSetManually = TRUE;
		}
	}
}
