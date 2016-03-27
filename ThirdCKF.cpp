#include "StdAfx.h"
#include "ThirdCKF.h"
#include <cmath>


ThirdCKF::ThirdCKF(void)
{
}


ThirdCKF::~ThirdCKF(void)
{
}

void ThirdCKF::ThirdCKFDelete(CPtrArray &X)
{
	#pragma omp parallel for
	for (int i = 0; i < X.GetSize(); ++i)
	{
		delete (StoreData *)X.GetAt(i);
	}
	X.RemoveAll();
}

void ThirdCKF::ThirdCKFfiltering(double *z, UINT m_length)
{
    CMatrix Pplus(4, 4);
	Pplus = glb.Pplus;

    CMatrix xhat(4, 1);
	xhat = glb.xhat;
	
	StoreData *thirdckf = new StoreData(xhat(1, 1), xhat(2, 1), xhat(3, 1), xhat(4, 1));
	X.Add(thirdckf);

	CMatrix Shat(dim, dim);
	CMatrix rjpoint(dim, cpoints);
	CMatrix Xminus(dim, cpoints);
	CMatrix jtemp(dim, 1);
	CMatrix Zhat(1, cpoints);
	CMatrix W(dim, 1);
	CMatrix Pz(1, 1);
	CMatrix Xtemp(dim, 1);
	//CMatrix CX(dim, 1);
	//CMatrix Z(1, 1);
	CMatrix Pxz(4, 1);
	//CMatrix Znum(1, 1);

	int num = 1;//第0个未知存放的是占位符0，并不是真正的测量值，因此应当从下标1开始取测量值
	for (UINT count = TimeInterval; count <= m_length; count += TimeInterval)
	{
		//Time Update
		//Evaluate the Cholesky factor
		Shat = Pplus.Cholesky();

		//Evaluate the cubature points and the propagated cubature points//Estimate the predicted state
		CMatrix CX(4, 1);
		 
		for (int i = 1; i <= cpoints; ++i)
		{
			 
			for (int j = 1; j <= dim; ++j)
			{
				jtemp(j, 1) = glb.kesi_ThirdCKF(j, i);
			}
			jtemp = Shat * jtemp + xhat;
			Xtemp = glb.fai * jtemp;
			CX = CX + c_w * Xtemp;

			 
			for (int j = 1; j <= dim; ++j)
			{
				//rjpoint(j, i) = jtemp(j, 1);
				Xminus(j, i) = Xtemp(j, 1);
			}
		}
		//xhat = CX;

		//Estimate the predicted error covariance
		Pplus = c_w * (Xminus * (~Xminus)) - CX * (~CX) + glb.gama * glb.Im * (~glb.gama);


		//Measurement Update
		//Evaluate the Cholesky factor
		Shat = Pplus.Cholesky();

		//Evaluate the cubature points and the propagated cubature points//Estimate the predicted measurement
		CMatrix Z(1, 1);
		 
		for (int i = 1; i <= cpoints; ++i)
		{
			 
			for (int j = 1; j <= dim; ++j)
			{
				jtemp(j, 1) = glb.kesi_ThirdCKF(j, i);
			}
			jtemp = Shat * jtemp + CX;
			Zhat(1, i) = atan(jtemp(3, 1) / jtemp(1, 1));
			Z(1, 1) = Z(1, 1) + c_w * Zhat(1, i);

			 
			for (int j = 1; j <= dim; ++j)
			{
				rjpoint(j, i) = jtemp(j, 1);
			}
		}

		//Estimate the innovation covariance matrix
		Pz(1, 1) = Rn;//For saving memory
		Pz = c_w * (Zhat * (~Zhat)) - Z * Z + Pz;

		//Estimate the cross-covariance matrix 
		Pxz = c_w * (rjpoint * (~Zhat)) - CX * Z;

		//Estimate the Kalman gain
		W = (1 / (double)Pz(1, 1)) * Pxz;
		
		xhat = CX + W * (z[num] - Z(1, 1));
		++num;

		Pplus = Pplus - Pz(1, 1) * W * (~W);
		StoreData *thirdckf = new StoreData(xhat(1, 1), xhat(2, 1), xhat(3, 1), xhat(4, 1));
		X.Add(thirdckf);
	}
}