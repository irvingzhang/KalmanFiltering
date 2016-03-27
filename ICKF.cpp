#include "StdAfx.h"
#include "ICKF.h"


ICKF::ICKF(void)
{
	//memset(X_ICKF, 0, sizeof(X_ICKF));
}


ICKF::~ICKF(void)
{
}

void ICKF::ICKFDelete(CPtrArray &X)
{
	#pragma omp parallel for
	for (int i = 0; i < X.GetSize(); ++i)
	{
		delete (StoreData *)X.GetAt(i);
	}
	X.RemoveAll();
}

void ICKF::ICKFfiltering(double *z, UINT m_length)
{
	CMatrix Pplus(4, 4);
	Pplus = glb.Pplus;

    CMatrix xhat(4, 1);
	xhat = glb.xhat;
	
	StoreData *ickf = new StoreData(xhat(1, 1), xhat(2, 1), xhat(3, 1), xhat(4, 1));
	X.Add(ickf);

	CMatrix Shat(dim, dim);
	CMatrix rjpoint1(dim, icpoints - 1);
	CMatrix rjpoint2(dim, 1);
	CMatrix Xminus1(dim, icpoints - 1);
	CMatrix Repmat(dim, icpoints - 1);
	CMatrix Xminus2(dim, 1);
	CMatrix jtemp(dim, 1);
	CMatrix Zhat1(1, icpoints - 1);
	CMatrix RepmatZ(1, icpoints - 1);
	CMatrix Zhat2(1, 1);
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

		 
		for (int i = 1; i <= icpoints; ++i)
		{
			 
			for (int j = 1; j <= dim; ++j)
			{
				jtemp(j, 1) = glb.kesi_ICKF(j, i);
			}
			jtemp = Shat * jtemp + xhat;
			Xtemp = glb.fai * jtemp;

			if (i < icpoints)//此编程风格会提高代码的冗余度，但提高了程序的运行效率，下同
			{
				 
				for (int j = 1; j <= dim; ++j)
				{
					Xminus1(j, i) = Xtemp(j, 1);
				}
				CX = CX + ic_w1 * Xtemp;
			}
			else
			{
				 
				for (int j = 1; j <= dim; ++j)
				{
					Xminus2(j, 1) = Xtemp(j, 1);
				}
				CX = CX + ic_w2 * Xtemp;
			}
		}

		//xhat = CX;

		//Estimate the predicted error covariance
		 
		for (int i = 1; i < icpoints; ++i)
		{
			 
			for (int j = 1; j <= dim; ++j)
			{
				Repmat(j, i) = CX(j, 1);
			}
		}
		Pplus = ic_w1 * ((Xminus1 - Repmat) * (~(Xminus1 - Repmat))) + 
			ic_w2* (Xminus2 - CX) * (~(Xminus2 - CX)) + glb.gama * glb.Im * (~glb.gama);


		//Measurement Update
		//Evaluate the Cholesky factor
		Shat = Pplus.Cholesky();

		//Evaluate the cubature points and the propagated cubature points//Estimate the predicted measurement
		CMatrix Z(1, 1);
		 
		for (int i = 1; i <= icpoints; ++i)
		{
			 
			for (int j = 1; j <= dim; ++j)
			{
				jtemp(j, 1) = glb.kesi_ICKF(j, i);
			}
			jtemp = Shat * jtemp + CX;

			if (i < icpoints)
			{
				Zhat1(1, i) = atan(jtemp(3, 1) / jtemp(1, 1));
			    Z(1, 1) = Z(1, 1) + ic_w1 * Zhat1(1, i);

				 
				for (int j = 1; j <= dim; ++j)
				{
					rjpoint1(j, i) = jtemp(j, 1);
				}
			}
			else
			{
				Zhat2(1, 1) = atan(jtemp(3, 1) / jtemp(1, 1));
				Z(1, 1) = Z(1, 1) + ic_w2 * Zhat2(1, 1);

				 
				for (int j = 1; j <= dim; ++j)
				{
					rjpoint2(j, 1) = jtemp(j, 1);
				}
			}
		}


		//Estimate the innovation covariance matrix
		Pz(1, 1) = Rn;//For saving memory
		#pragma omp parallel for
		for (int i = 1; i < icpoints; ++i)
		{
			RepmatZ(1, i) = Z(1, 1);
		}
		Pz = ic_w1 * ((Zhat1 - RepmatZ)* (~(Zhat1 - RepmatZ))) + ic_w2 * (Zhat2 - Z) * (Zhat2 - Z) + Pz;

		//Estimate the cross-covariance matrix 
		Pxz = ic_w1 * ((rjpoint1 - Repmat) * (~(Zhat1 - RepmatZ))) + ic_w2 *((rjpoint2 - CX) * (Zhat2 - Z));

		//Estimate the Kalman gain
		W = (1 / (double)Pz(1, 1)) * Pxz;

		//Estimate the updated state
		//Znum(1, 1) = z[num];
		
		xhat = CX + W * (z[num] - Z(1, 1));
		++num;

		Pplus = Pplus - Pz(1, 1) * W * (~W);

		StoreData *ickf = new StoreData(xhat(1, 1), xhat(2, 1), xhat(3, 1), xhat(4, 1));
		X.Add(ickf);
	}
}