#pragma once
#include "StdAfx.h"
#include "RealTrack.h"

#include "Globals.h"
#include "SettingDlg.h"
#include <cmath>

RealTrack::RealTrack(void)
{
	memset(Z_Real, 0, sizeof(Z_Real));
}

RealTrack::~RealTrack(void)
{
}

void RealTrack::RealTrackDelete(CPtrArray &X)
{
	#pragma omp parallel for
	for (int i = 0; i < X.GetSize(); ++i)
	{
		delete (StoreData *)X.GetAt(i);
	}
	X.RemoveAll();
}

double RealTrack::rand_normal(const double Mean, const double SquareMargin) 
{
	double model = 4294967296.0;
	double multiplicator = 663608941.0;
	double temp_a, temp_b;
	static double seed = (double)time(NULL);

	seed *= multiplicator;
	seed -= (int)(seed/model) * model;
	temp_a = seed/model;

	seed *= multiplicator;
	seed -= (int)(seed/model) * model;
	temp_b = seed/model;

 
	/*double x, y, r;
	do {
	    x = 2.0*rand()/RAND_MAX - 1;
	    y = 2.0*rand()/RAND_MAX - 1;
	    r = x*x + y*y;
	} while (r == 0.0 || r > 1.0);*/

	double gauss_rand = Mean + temp_a * sqrt(-2.0 * log(temp_b)) * SquareMargin;
	return gauss_rand;

}

void RealTrack::GenerateRealTrack(UINT m_length)
{
	CMatrix Pplus(4, 4);
	Pplus = glb.Pplus;

    CMatrix xhat(4, 1);
	xhat = glb.xhat;

	StoreData *rt = new StoreData(xhat(1, 1), xhat(2, 1), xhat(3, 1), xhat(4,1));
	X.Add(rt);

	
	double z = 0;
	Z_Real[0] = z;

	CMatrix Procnoise(2, 1);

	int num = 1;
	for (UINT i = TimeInterval; i <= m_length; i += TimeInterval)
	{		
		//srand((time_t)time((time_t *)NULL));
		Procnoise(1, 1) = rand_normal(0, sqrt(Qn)); 
		Procnoise(2, 1) = rand_normal(0, sqrt(Qn));

		xhat = glb.fai * xhat + glb.gama * Procnoise; 
		z = atan(xhat(3, 1) / xhat(1, 1)) + sqrt(Rn);

		Z_Real[num] = z;
		++num;

		StoreData *rt = new StoreData(xhat(1, 1), xhat(2, 1), xhat(3, 1), xhat(4,1));
		X.Add(rt);

		//Z_Real.push_back(z);
		//RealTrack rtra(z);
		//Z.Add(&rtra);
	}
}