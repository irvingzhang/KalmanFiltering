#pragma once
#include "StdAfx.h"
#include "EKF.h"
#include <cmath>


EKF::EKF(void)
{
	//memset(X_EKF, 0, sizeof(X_EKF));
}


EKF::~EKF(void)
{
}

void EKF::EKFDelete(CPtrArray &X)
{
	#pragma omp parallel for
	for (int i = 0; i < X.GetSize(); ++i)
	{
		delete (StoreData *)X.GetAt(i);
	}
	X.RemoveAll();
}

/*EKF::EKF(double x1, double x2, double x3, double x4)
{
	memset(X_EKF, 0, sizeof(X_EKF));
	this->X_EKF[0][0] = x1;
	this->X_EKF[1][0] = x2;
	this->X_EKF[2][0] = x3;
	this->X_EKF[3][0] = x4;
}*/

void EKF::EKFfiltering(double *z, UINT m_length)
{
	//CMatrix fai(4, 4);
    //fai(1, 1) = fai(1, 2) = fai(2, 2) = fai(3, 3) = fai(3, 4) = fai(4, 4) = 1;

    //CMatrix gama(4, 2);
    //gama(1, 1) = gama(3, 2) = 0.5;
    //gama(2, 1) = gama(4, 2) = 1;

    CMatrix Pplus(4, 4);
	Pplus = glb.Pplus;

    CMatrix xhat(4, 1);
	xhat = glb.xhat;
	
	StoreData *ekf = new StoreData(xhat(1, 1), xhat(2, 1), xhat(3, 1), xhat(4, 1));
	X.Add(ekf);

	//CMatrix I(4, 4);
	//I(1, 1) = I(2, 2) = I(3, 3) = I(4, 4) = 1;

	//CMatrix Im(2, 2);
	//Im(1, 1) = Im(2, 2) = Qn;

	CMatrix H(1, dim);
	CMatrix K(dim, 1);
	//CMatrix tem(1, 1);
	//CMatrix CMte(dim, 1);
	CMatrix Znum(1, 1);

	int num = 1;//第0个未知存放的是占位符0，并不是真正的测量值，因此应当从下标1开始取测量值
	for (UINT count = TimeInterval; count <= m_length; count += TimeInterval)
	{
		Pplus = glb.fai * Pplus * (~glb.fai) + glb.gama * glb.Im * (~glb.gama);

		H(1, 1) = -xhat(3, 1) / ((1 + pow(xhat(3, 1) / xhat(1, 1), 2)) * pow(xhat(1, 1), 2));
		H(1, 2) = H(1, 4) = 0;
		H(1, 3) = 1 / ((1 + pow(xhat(3, 1) / xhat(1, 1), 2)) * xhat(1, 1));

		//CMatrix tem(1, 1);
		Znum(1, 1) = Rn;//For saving memory
		Znum = H * Pplus * (~H) + Znum;
 		K = ((double)1 / Znum(1, 1)) *Pplus * (~H);

		xhat = glb.fai * xhat;
		
		xhat = xhat + (z[num] - atan(xhat(3, 1) / xhat(1, 1))) * K;

		Pplus = (glb.I - K * H) * Pplus;
	
		++num;

		StoreData *ekf = new StoreData(xhat(1, 1), xhat(2, 1), xhat(3, 1), xhat(4, 1));
		X.Add(ekf);
	}
}
