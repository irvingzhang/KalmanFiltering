#ifndef _AFX_IMBEDDED_CUBATURE_KALMAN_FILTERING_HEADER_
#define _AFX_IMBEDDED_CUBATURE_KALMAN_FILTERING_HEADER_


#pragma once
#include "Matrix.h"
#include "Globals.h"
#include "StoreData.h"

class ICKF
{
public:
	//double X_ICKF[4][1];
	CPtrArray X;

private:
	Globals glb;

public:
	void ICKFfiltering(double *, UINT);
	void ICKFDelete(CPtrArray &);

public:
	ICKF(void);
	//ICKF(double, double, double, double);
	~ICKF(void);
};

#endif