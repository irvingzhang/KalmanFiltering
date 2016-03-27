#ifndef _AFX_REAL_TRACK_HEADER_
#define _AFX_REAL_TRACK_HEADER_

#pragma once
#include "Matrix.h"
#include "Globals.h"
#include "StoreData.h"

const double PI = 3.1415926536;

class RealTrack
{
public:
	double Z_Real[MAX];
	CPtrArray X;

private:
	Globals glb;
	
	
public:
	void GenerateRealTrack(UINT);
	void RealTrackDelete(CPtrArray &);
	double rand_normal(double mean, double stddev);

public:
	RealTrack(void);
	~RealTrack(void);
};
#endif
