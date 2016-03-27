#include "StdAfx.h"
#include "Globals.h"


Globals::Globals(void)
{
	fai.SetSize(4, 4);
	fai(1, 1) = fai(1, 2) = fai(2, 2) = fai(3, 3) = fai(3, 4) = fai(4, 4) = 1;

	gama.SetSize(4, 2);
	gama(1, 1) = gama(3, 2) = 0.5;
    gama(2, 1) = gama(4, 2) = 1;

	Pplus.SetSize(4, 4);
	Pplus(1, 1) = Pplus(3, 3) = 0.01;
    Pplus(2, 2) = pow(0.005, 2);
    Pplus(4, 4) = pow(0.01, 2);

	xhat.SetSize(4, 1);
    xhat(1, 1) = -0.05;
    xhat(2, 1) = 0.0001;
    xhat(3, 1) = 0.7;
    xhat(4, 1) = -0.055;

	kesi_ICKF.SetSize(dim, icpoints);
	kesi_ICKF(1, 1) = kesi_ICKF(2, 1) = kesi_ICKF(3, 1) = kesi_ICKF(4, 1) = 
	kesi_ICKF(1, 2) = kesi_ICKF(2, 2) = kesi_ICKF(3, 2) = kesi_ICKF(1, 3) =
	kesi_ICKF(2, 3) = kesi_ICKF(1, 4) = kesi_ICKF(1, 5) = kesi_ICKF(3, 5) =
	kesi_ICKF(4, 5) = kesi_ICKF(1, 6) = kesi_ICKF(4, 6) = kesi_ICKF(1, 7) =
	kesi_ICKF(3, 7) = kesi_ICKF(1, 8) = kesi_ICKF(2, 8) = kesi_ICKF(4, 8) = sqrt((double)2 * delta);

 
	for(int i = 1; i <= 8; ++i)
	{
		 
		for(int j = 1; j <= dim; ++j)
		{
			if(kesi_ICKF(j, i) == 0.0)
			{
				kesi_ICKF(j, i) = -sqrt((double)2 * delta);
			}
		}
	}

 
	for (int i = 9; i <= icpoints - 1; ++i)
	{
		 
		for(int j = 1; j <= dim; ++j)
		{
			kesi_ICKF(j, i) = -kesi_ICKF(j, i - 8);
		}
	}
	kesi_ICKF(1, icpoints) = kesi_ICKF(2, icpoints) = kesi_ICKF(3, icpoints) = kesi_ICKF(4, icpoints) = 0;

	Im.SetSize(2, 2);
	Im(1, 1) = Im(2, 2) = Qn;

	I.SetSize(4, 4);
	I(1, 1) = I(2, 2) = I(3, 3) = I(4, 4) = 1;

	kesi_ThirdCKF.SetSize(dim, cpoints);
	kesi_ThirdCKF(1, 1) = kesi_ThirdCKF(2, 2) = kesi_ThirdCKF(3, 3) = kesi_ThirdCKF(4, 4) = 2;//sqrt((double)cpoints / 2);
	kesi_ThirdCKF(1, 5) = kesi_ThirdCKF(2, 6) = kesi_ThirdCKF(3, 7) = kesi_ThirdCKF(4, 8) = -2;//-sqrt((double)cpoints / 2);

	kesi_FifthCKF.SetSize(dim, hcpoints);
	kesi_FifthCKF(1, 1) = kesi_FifthCKF(2, 1) = kesi_FifthCKF(3, 1) = kesi_FifthCKF(4, 1) = 0;
	kesi_FifthCKF(1, 2) = kesi_FifthCKF(2, 3) = kesi_FifthCKF(3, 4) = kesi_FifthCKF(4, 5) = sqrt((double)3);
	kesi_FifthCKF(1, 6) = kesi_FifthCKF(2, 7) = kesi_FifthCKF(3, 8) = kesi_FifthCKF(4, 9) = -sqrt((double)3);
	kesi_FifthCKF(1, 10) = kesi_FifthCKF(2, 10) = kesi_FifthCKF(1, 11) = kesi_FifthCKF(1, 12) = 
	kesi_FifthCKF(3, 12) = kesi_FifthCKF(1, 13) = kesi_FifthCKF(1, 14) = kesi_FifthCKF(4, 14) = 
	kesi_FifthCKF(1, 15) = kesi_FifthCKF(2, 16) = kesi_FifthCKF(3, 16) = kesi_FifthCKF(2, 17) =
	kesi_FifthCKF(2, 18) = kesi_FifthCKF(4, 18) = kesi_FifthCKF(2, 19) = kesi_FifthCKF(3, 20) = 
	kesi_FifthCKF(4, 20) = kesi_FifthCKF(3, 21) = sqrt((double)3);
	kesi_FifthCKF(2, 11) = kesi_FifthCKF(3, 13) = kesi_FifthCKF(4, 15) = kesi_FifthCKF(3, 17) = 
	kesi_FifthCKF(4, 19) = kesi_FifthCKF(4, 21) = -sqrt((double)3);

 
	for (int i = 22; i <= hcpoints; ++i)
	{
		 
		for (int j = 1; j <= dim; ++j)
		{
			kesi_FifthCKF(j, i) = -kesi_FifthCKF(j, i - 12);
		}
	}
}


Globals::~Globals(void)
{
}
