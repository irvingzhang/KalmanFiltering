#ifndef _AFX_FIFTH_DEGREE_CUBATURE_KALMAN_FILTERING_HEADER_
#define _AFX_FIFTH_DEGREE_CUBATURE_KALMAN_FILTERING_HEADER_

#pragma once
#include "Matrix.h"
#include "Globals.h"
#include "StoreData.h"

class FifthCKF
{
public:
	//double X_FifthCKF[4][1];
	CPtrArray X;

private:
	Globals glb;

public:
	void FifthCKFfiltering(double *, UINT);
	void FifthCKFDelete(CPtrArray &);

public:
	FifthCKF(void);
	//FifthCKF(double, double, double, double);
	~FifthCKF(void);
};

#endif