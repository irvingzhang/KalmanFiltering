#ifndef _AFX_THIRD_DEGREE_CUBATURE_KALMAN_FILTERING_HEADER_
#define _AFX_THIRD_DEGREE_CUBATURE_KALMAN_FILTERING_HEADER_

#pragma once
#include "Matrix.h"
#include "Globals.h"
#include "StoreData.h"


class ThirdCKF
{
public:
	//double X_ThirdCKF[4][1];
	CPtrArray X;

private:
	Globals glb;

public:
	void ThirdCKFfiltering(double *, UINT);
	void ThirdCKFDelete(CPtrArray &);

public:
	ThirdCKF(void);
	//ThirdCKF(double, double, double, double);
	~ThirdCKF(void);
};

#endif

