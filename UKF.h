#ifndef _AFX_UNSCENTED_KALMAN_FILTERING_HEADER_
#define _AFX_UNSCENTED_KALMAN_FILTERING_HEADER_

#pragma once
#include "Matrix.h"
#include "Globals.h"
#include "StoreData.h"

class UKF
{
public:
	CPtrArray X;

private:
	Globals glb;

public:
	void UKFfiltering(double *, UINT);
	void UKFDelete(CPtrArray &);

public:
	UKF(void);
	~UKF(void);
};

#endif