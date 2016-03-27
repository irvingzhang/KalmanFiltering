#include "StdAfx.h"
#include "UKF.h"
#include <cmath>
#include "Matrix.h"

UKF::UKF(void)
{
}


UKF::~UKF(void)
{
}

void UKF::UKFDelete(CPtrArray &X)
{
	#pragma omp parallel for
	for (int i = 0; i < X.GetSize(); ++i)
	{
		delete (StoreData *)X.GetAt(i);
	}
	X.RemoveAll();
}

void UKF::UKFfiltering(double *z, UINT m_length)
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
	
	StoreData *ukf = new StoreData(xhat(1, 1), xhat(2, 1), xhat(3, 1), xhat(4, 1));
	X.Add(ukf);

	//CMatrix Im(2, 2);
	//Im(1, 1) = Im(2, 2) = Qn;
	
	CMatrix u_x_point(dim,u_total_number);
	CMatrix Ptemp(dim, dim);
	CMatrix u_x_hat(dim, 1);
	CMatrix Xtemp(dim, 1);
	CMatrix u_y_hat(1, u_total_number);
	double u_yhat = 0;
	double u_Pzz = 0;
	CMatrix  u_Pxz(dim, 1);
	CMatrix u_W(dim, 1);

	int num = 1;
	for (UINT count = TimeInterval; count <= m_length; count += TimeInterval)
	{
		 
		for (int j = 1; j <= dim; ++j)
		{
			u_x_point(j, 1) = xhat(j, 1);
		}
		Ptemp = ((u_symmetry_number + u_k) * Pplus).Cholesky();

		 
		for (int j = 2; j <= u_total_number; ++j)
		{
			if (j <= u_symmetry_number + 1)
			{
				 
				for (int k = 1; k <= dim; ++k)
				{
					u_x_point(k, j) = xhat(k, 1) + Ptemp(k, j - 1);
				}
			}
			else
			{
				 
				for (int k = 1; k <= dim; ++k)
				{
					u_x_point(k, j) = xhat(k, 1) - Ptemp(k, j - u_symmetry_number - 1);
				}
			}
		}

		u_x_point = glb.fai * u_x_point;

		 
		for (int i = 1; i <= dim; ++i)
		{
			Xtemp(i, 1) = u_x_point(i, 1);
		}

		 
		for (int j = 1; j <= dim; ++j)
		{
			u_x_hat(j, 1) = u_weight_1 * u_x_point(j, 1);
		}

		 
		for (int j = 2; j <= u_total_number; ++j)
		{
			 
			for (int i = 1; i <= dim; ++i)
			{
				u_x_hat(i, 1) = u_x_hat(i, 1) + u_weight_N1 * u_x_point(i, j);
			}
		}

		Pplus = u_weight_1 * ((Xtemp - u_x_hat) * (~(Xtemp - u_x_hat))) + glb.gama * glb.Im * (~glb.gama);
		 
		for (int j = 2; j <= u_total_number; ++j)
		{
			 
			for (int i =1; i <= dim; ++i)
			{
				Xtemp(i, 1) = u_x_point(i, j);
			}
			Pplus = Pplus + u_weight_N1 * ((Xtemp - u_x_hat) * (~(Xtemp - u_x_hat)));
		}

		 
		for (int i = 1; i<= u_total_number; ++i)
		{
			u_y_hat(1, i) = atan(u_x_point(3, i) / u_x_point(1, i));
		}

		u_yhat = u_weight_1 * u_y_hat(1, 1);

		 
		for (int j = 2; j <= u_total_number; ++j)
		{
			u_yhat = u_yhat + u_weight_N1 * u_y_hat(1, j);
		}



		u_Pzz = Rn + u_weight_1 * (u_y_hat(1, 1) - u_yhat) * (u_y_hat(1, 1) - u_yhat);

		 
		for (int j = 2; j <= u_total_number; ++j)
		{
			u_Pzz = u_Pzz + u_weight_N1 * (u_y_hat(1, j) - u_yhat) * (u_y_hat(1, j) - u_yhat);
		}

		 
		for (int i = 1; i <= dim; ++i)
		{
			u_Pxz(i, 1) = u_weight_1 * (u_x_point(i,1) - u_x_hat(i, 1)) * (u_y_hat(1, 1) - u_yhat);
		}

		 
		for (int j = 2; j <= u_total_number; ++j)
		{
			 
			for (int i = 1; i <= dim; ++i)
			{
				u_Pxz(i, 1) = u_Pxz(i, 1) + u_weight_N1 * (u_x_point(i,j) - u_x_hat(i, 1)) * (u_y_hat(1, j) - u_yhat);
			}
		}


		u_W = (1 / u_Pzz) * u_Pxz;

		xhat = u_x_hat + (z[num] - u_yhat) * u_W;
		++num;

		Pplus = Pplus - u_Pzz * (u_W * (~u_W));

		StoreData *ukf = new StoreData(xhat(1, 1), xhat(2, 1), xhat(3, 1), xhat(4, 1));
		X.Add(ukf);
	}
}