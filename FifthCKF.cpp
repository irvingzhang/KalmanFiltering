#include "StdAfx.h"
#include "FifthCKF.h"


FifthCKF::FifthCKF(void)
{
}


FifthCKF::~FifthCKF(void)
{
}

void FifthCKF::FifthCKFDelete(CPtrArray &X)
{
	#pragma omp parallel for
	for (int i = 0; i < X.GetSize(); ++i)
	{
		delete (StoreData *)X.GetAt(i);
	}
	X.RemoveAll();
}

void FifthCKF::FifthCKFfiltering(double *z, UINT m_length)
{
	CMatrix Pplus(4, 4);
	Pplus = glb.Pplus;

    CMatrix xhat(4, 1);
	xhat = glb.xhat;
	
	StoreData *fifthckf = new StoreData(xhat(1, 1), xhat(2, 1), xhat(3, 1), xhat(4, 1));
	X.Add(fifthckf);

	CMatrix Shat(dim, dim);
	CMatrix rjpoint1(dim, 1);
	CMatrix rjpoint2(dim, cpoints);
	CMatrix rjpoint3(dim, hcpoints - cpoints - 1);
	CMatrix Xminus1(dim, 1);
	CMatrix Xminus2(dim, cpoints);
	CMatrix Repmat2(dim, cpoints);
	CMatrix Xminus3(dim, hcpoints - cpoints - 1);
	CMatrix Repmat3(dim, hcpoints - cpoints - 1);
	CMatrix jtemp(dim, 1);
	CMatrix Zhat1(1, 1);
	CMatrix Zhat2(1, cpoints);
	CMatrix RepmatZ2(1, cpoints);
	CMatrix Zhat3(1, hcpoints - cpoints - 1);
	CMatrix RepmatZ3(1, hcpoints - cpoints - 1);
	CMatrix W(dim, 1);
	CMatrix Pz(1, 1);
	CMatrix Xtemp(dim, 1);
	CMatrix Pxz(4, 1);

	int num = 1;//第0个未知存放的是占位符0，并不是真正的测量值，因此应当从下标1开始取测量值
	for (UINT count = TimeInterval; count <= m_length; count += TimeInterval)
	{
		//Time Update
		//Evaluate the Cholesky factor
		Shat = Pplus.Cholesky();

		//Evaluate the cubature points and the propagated cubature points//Estimate the predicted state
		CMatrix CX(4, 1);
		 
		for (int i = 1; i <= hcpoints; ++i)
		{
			 
			for (int j = 1; j <= dim; ++j)
			{
				jtemp(j, 1) = glb.kesi_FifthCKF(j, i);
			}
			jtemp = Shat * jtemp + xhat;
			Xtemp = glb.fai * jtemp;

			if ( 1 == i)
			{
				 
				for (int j = 1; j <= dim; ++j)
				{
					Xminus1(j, 1) = Xtemp(j, 1);
				}
				CX = CX + c5_w2 * Xtemp;
			}
			else if (i > 1 && i <= cpoints + 1)//此编程风格会提高代码的冗余度，但提高了程序的运行效率，下同
			{
				 
				for (int j = 1; j <= dim; ++j)
				{
					Xminus2(j, i - 1) = Xtemp(j, 1);
				}
				CX = CX + c5_w1 * Xtemp;
			}
			else
			{
				 
				for (int j = 1; j <= dim; ++j)
				{
					Xminus3(j, i - cpoints - 1) = Xtemp(j, 1);
				}
				CX = CX + c5_w4 * Xtemp;
			}
		}


		//xhat = CX;

		//Estimate the predicted error covariance
		 
		for (int i = 1; i <= cpoints; ++i)
		{
			#pragma omp parallel for
			for (int j = 1; j <= dim; ++j)
			{
				Repmat2(j, i) = CX(j, 1);
			}
		}

		 
		for (int i = 1; i <= hcpoints - cpoints - 1; ++i)
		{
			#pragma omp parallel for
			for(int j = 1; j <= dim; ++j)
			{
				Repmat3(j, i) = CX(j, 1);
			}
		}

		Pplus = c5_w2 * ((Xminus1 - CX) * (~(Xminus1 - CX))) + 
			c5_w1 * (Xminus2 - Repmat2) * (~(Xminus2 - Repmat2)) + 
			c5_w4 * (Xminus3 - Repmat3) * (~(Xminus3 - Repmat3)) + glb.gama * glb.Im * (~glb.gama);

		//Measurement Update
		//Evaluate the Cholesky factor
		Shat = Pplus.Cholesky();

		//Evaluate the cubature points and the propagated cubature points//Estimate the predicted measurement
		CMatrix Z(1, 1);
		 
		for (int i = 1; i <= hcpoints; ++i)
		{
			 
			for (int j = 1; j <= dim; ++j)
			{
				jtemp(j, 1) = glb.kesi_FifthCKF(j, i);
			}
			jtemp = Shat * jtemp + CX;

			if (1 == i)
			{
				Zhat1(1, 1) = atan(jtemp(3, 1) / jtemp(1, 1));
				Z(1, 1) = Z(1, 1) + c5_w2 * Zhat1(1, 1);

				#pragma omp parallel for
				for (int j = 1; j<= dim; ++j)
				{
					rjpoint1(j, 1) = jtemp(j, 1);
				}
			}
			else if (i > 1 && i <= cpoints + 1)
			{
				Zhat2(1, i - 1) = atan(jtemp(3, 1) / jtemp(1, 1));
			    Z(1, 1) = Z(1, 1) + c5_w1 * Zhat2(1, i - 1);

				#pragma omp parallel for
				for (int j = 1; j <= dim; ++j)
				{
					rjpoint2(j, i - 1) = jtemp(j, 1);
				}
			}
			else
			{
				Zhat3(1, i - cpoints - 1) = atan(jtemp(3, 1) / jtemp(1, 1));
				Z(1, 1) = Z(1, 1) + c5_w4 * Zhat3(1, i - cpoints - 1);

				#pragma omp parallel for
				for (int j = 1; j <= dim; ++j)
				{
					rjpoint3(j, i - cpoints - 1) = jtemp(j, 1);
				}
			}
		}

		//Estimate the innovation covariance matrix
		Pz(1, 1) = Rn;//For saving memory

		#pragma omp parallel for
		for (int i = 1; i <= cpoints; ++i)
		{
			RepmatZ2(1, i) = Z(1, 1);
		}

		#pragma omp parallel for
		for (int i = 1; i <= hcpoints - cpoints - 1; ++i)
		{
			RepmatZ3(1, i) = Z(1, 1);
		}

		Pz = c5_w2 * (Zhat1 - Z) * (~(Zhat1 - Z)) + 
			c5_w1 * (Zhat2 - RepmatZ2)* (~(Zhat2 - RepmatZ2)) + 
			c5_w4 * (Zhat3 - RepmatZ3) * (~(Zhat3 - RepmatZ3)) + Pz;

		//Estimate the cross-covariance matrix 
		Pxz = c5_w2 * (rjpoint1 - CX) * (~(Zhat1 - Z)) + 
			c5_w1 *(rjpoint2 - Repmat2) * (~(Zhat2 - RepmatZ2)) + 
			c5_w4 * (rjpoint3 - Repmat3) * (~(Zhat3 - RepmatZ3));

		//Estimate the Kalman gain
		W = ((double)1 / Pz(1, 1)) * Pxz;

		//Estimate the updated state
		//Znum(1, 1) = z[num];
		
		xhat = CX + W * (z[num] - Z(1, 1));
		++num;

		Pplus = Pplus - Pz(1, 1) * W * (~W);

		StoreData *fifthckf = new StoreData(xhat(1, 1), xhat(2, 1), xhat(3, 1), xhat(4, 1));
		X.Add(fifthckf);
	}
}