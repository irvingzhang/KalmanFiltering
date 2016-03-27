#ifndef _AFX_EXTENDED_KALMAN_FILTERING_HEADER_
#define _AFX_EXTENDED_KALMAN_FILTERING_HEADER_

#pragma once
#include "Matrix.h"
#include "Globals.h"
#include "StoreData.h"

class EKF
{
public:
	//double X_EKF[4][1];
	CPtrArray X;

private:
	Globals glb;

public:
	void EKFfiltering(double *, UINT);
	void EKFDelete(CPtrArray &);

public:
	EKF(void);
	//EKF(double, double, double, double);
	~EKF(void);
};
#endif
