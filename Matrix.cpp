//////////////////////////////////////////////////////////////////////////////////
//Version: 20130614                                                            ///
//(Implementation of the CMatrix class)                                        ///    
//                                                                             ///
//                                                by  Xin-Chun Zhang(UESTC)    ///
//                                                   irving_zhang@163.com      ///
//                                                  Update data: 2013-6-14     ///
//////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Matrix.h"
#include <cmath>


CMatrix::CMatrix()
{
}


CMatrix::CMatrix(int row)//重载构造函数
{
	m_row = row;
	m_col = row;
	m_mat=new double *[row];

	for (int i = 0; i < row; ++ i)
	{
		m_mat[i] = new double [row];
	}
	
	for (int i = 0; i < row; ++ i)
	{
		for (int j = 0; j < row; ++ j)
		{
			m_mat[i][j] = 0;
		}
	}
}


CMatrix::CMatrix(int row, int col)//重载构造函数
{
	m_row = row;
	m_col = col;
	m_mat = new double *[row];

	for (int i = 0; i < row; ++ i)
	{
		m_mat[i] = new double [col];
	}

	for (int i = 0; i < row; ++ i)
	{
		for (int j = 0; j < col; ++ j)
		{
			m_mat[i][j] = 0;
		}
	}
}

CMatrix::CMatrix(const CMatrix &temp)//显式复制构造函数
{
	m_row = temp.m_row;
	m_col = temp.m_col;
	m_mat = new double *[m_row];

	for (int i = 0; i < m_row; ++i)
	{
		m_mat[i] = new double [m_col];
	}

	for (int i = 0; i < m_row; ++ i)
	{
		for (int j = 0; j < m_col; ++ j)
		{
			m_mat[i][j] = temp.m_mat[i][j];
		}
	}
}

void CMatrix::SetSize(int row, int col)//设置矩阵大小
{
	m_row = row;
	m_col = col;
	m_mat = new double *[row];

	for (int i=0; i < row; ++ i)
	{
		m_mat[i] = new double [col];
	}

	for (int i = 0; i < row; ++ i)
	{
		for (int j = 0; j < col; ++ j)
		{
			m_mat[i][j] = 0;
		}
	}
}
void CMatrix::SetSize(int row)//设置矩阵大小
{
	m_row = row;
	m_col = row;
	m_mat = new double *[row];

	for (int i = 0; i < row; ++ i)
	{
		 m_mat[i] = new double [row];
	}

	for (int i = 0; i < row; ++ i)
	{
		for (int j = 0; j < row; ++ j)
		{
			m_mat[i][j] = 0;
		}
	}
}

CMatrix::~CMatrix()//析构函数
{
	for (int i = 0; i < m_row; ++ i)
	{
		delete [] m_mat[i];
	}
	delete [] m_mat;
}

//Dolittle分解求矩阵逆
void CMatrix::MatInverse(double **mat, int dim)
{
	double d;
 
	double **tempmat = new double*[dim];

	for (int i = 0; i < dim; ++ i)
	{
		tempmat[i] = new double[dim];
	}
		
	int *indx = new int[dim * sizeof(int)];
	double *tempvector = new double[dim];
	double *col = tempvector;

	MatLudcmp(mat, dim, indx, &d);

	for (int j = 0; j < dim; ++ j)
    {
		for (int i = 0; i < dim; ++ i) 
		{
			col[i] = 0.0;
		}
		col[j] = 1.0;
		MatLubksb(mat, dim, indx, col);
     
		for (int i = 0; i < dim; ++i) 
		{
			tempmat[i][j] = col[i];
		}
    }
 
	for (int i = 0; i < dim; ++ i)
	{
		for (int j = 0; j < dim; ++ j)
		{
			mat[i][j] = tempmat[i][j];
		}
	}

 
	delete [] tempvector;

 	for (int i = 0; i < dim; ++ i)
	{
		delete [] tempmat[i];
	}
	delete []tempmat;
	delete indx;
}

void CMatrix::MatLudcmp(double **a, int n, int *indx, double *d)
{
	int imax;
	double big,dum,sum,temp;
  
	double *vv = new double[n * sizeof(double)];
	if (NULL == vv) //内存分配失败
	{
		exit(1);
	}
	*d = 1.0;

	for (int i = 0; i < n; ++ i)
    {
		double big = 0.0;

		for (int j = 0; j < n; ++ j)
		{
			if ((temp = fabs(a[i][j])) > big) 
			{
				big = temp;
			}
		}
     
		if (0.0 == big)
		{
			exit(1);
		}
     
		vv[i] = 1.0 / big;
	}


	for (int j = 0; j < n; ++ j)
    {
		for (int i = 0; i < j; ++ i)
		{
			sum = a[i][j];
			
			for (int k = 0; k < i; ++ k) 
			{
				sum -= a[i][k] * a[k][j];
			}
      
			a[i][j] = sum;
		}
     
		big = 0.0;
     
		for (int i = j; i < n; ++ i)
        {
			sum = a[i][j];
      
			for (int k = 0; k < j; ++ k) 
			{
				sum -= a[i][k] * a[k][j];
			}
      
			a[i][j] = sum;
      
			if ((dum = vv[i] * fabs(sum)) >= big)
			{
				big = dum;
				imax = i;
			}
		}
     
		if (j != imax)
		{
			for (int k = 0; k < n; ++ k)
			{
				dum = a[imax][k];
				a[imax][k] = a[j][k];
				a[j][k] = dum;
			}
        
			*d = -(*d);
			vv[imax] = vv[j];
		}
    
		indx[j] = imax;
     
		if (a[j][j] == 0.0) 
		{
			a[j][j] = TINY;
		}
     
		if (j != n-1)
		{
			dum = 1.0 / a[j][j];
     
			for (int i = j + 1; i < n; ++ i) 
			{
				a[i][j] *= dum;
			}
		}
	}
 
	delete vv;
}

void CMatrix::MatLubksb(double **a, int n, int *indx, double *b)
{
	int ii = -1;
	double sum;

	for (int i = 0; i < n; ++ i)
    {
		int ip = indx[i];
		sum = b[ip];
		b[ip] = b[i];
     
		if (ii >= 0)
		{
			for (int j = ii; j < i; ++ j) 
			{
				sum -= a[i][j] * b[j];
			}
		}
     
		else if (sum) 
		{
			ii = i;
		}
		b[i] = sum;
	}

	for (int i = n - 1; i >= 0; -- i)
    {
		sum = b[i];
     
		for (int j = i + 1; j < n; ++ j) 
		{
			sum -= a[i][j] * b[j];
		}
     
		b[i] = sum / a[i][i];
    }
}

//操作符重载
CMatrix CMatrix::operator +(CMatrix &a)//矩阵加法
{
	if (a.m_col != m_col)
	{
		exit(1);
	}

 	if (a.m_row != m_row)
	{
 		exit(1);
	}
		
	CMatrix temp(m_row, m_col);

	for(int i = 0; i < m_row; ++ i)
	{
		for(int j = 0; j < m_col; ++ j)
		{
			temp.m_mat[i][j] = a.m_mat[i][j] + m_mat[i][j];
		}
	}
	return temp;
}

CMatrix CMatrix::operator -(CMatrix &a)//矩阵减法
{
	if (a.m_col != m_col)
	{
		exit(1);
	}
 	
	if (a.m_row != m_row)
	{
 		exit(1);
	}

	CMatrix temp(m_row, m_col);

	for(int i = 0; i < m_row; ++ i)
	{
		for(int j = 0; j < m_col; ++ j)
		{
			temp.m_mat[i][j] = m_mat[i][j] - a.m_mat[i][j];
		}
	}
	return temp;


}

CMatrix CMatrix::operator *(CMatrix &a)//矩阵乘法
{
	if(m_col != a.m_row)
	{
 		exit(1);
	}

 	CMatrix temp(m_row, a.m_col);

  	for (int i = 0; i < m_row; ++ i)
	{
 		for (int j = 0; j < a.m_col; ++ j)
 		{
 			for (int n = 0; n < m_col; ++ n)
			{
 				temp.m_mat[i][j] += m_mat[i][n] * a.m_mat[n][j];
			}
 		}
	}
 
	return temp;
}

CMatrix CMatrix::Cholesky()//Cholesky
{
	if (m_col != m_row)
	{
		exit(1);
	}

	CMatrix temp(m_row, m_col);

	for (int i1 = 0; i1 < m_row; ++ i1)//注意：此处应当是<
	{
		for (int j1 = 0; j1 <= i1; ++ j1)//注意：此处应当是<=
		{
			double xigema = m_mat[i1][j1];

			for (int t1 = 0; t1 <= j1 -1; ++ t1)//注意：此处应当是<=
			{
				xigema = xigema - temp.m_mat[i1][t1] * temp.m_mat[j1][t1];
			}

			if (i1 == j1)
			{
				temp.m_mat[i1][i1] = sqrt(xigema);
			}
			else
			{
				temp.m_mat[i1][j1] = xigema / temp.m_mat[j1][j1];
			}
		}
	}
	
	return temp;
}

CMatrix& CMatrix::operator =(CMatrix &a)//矩阵赋值
{
	if (a.m_col != m_col)
	{
 		exit(1);
	}

 	if (a.m_row != m_row)
	{
 		exit(1);
	}

	for (int i = 0; i < m_row; ++ i)
	{
		for(int j = 0; j < m_col; ++ j)
		{
			m_mat[i][j] = a.m_mat[i][j];
		}
	}

	return *this;
}

double & CMatrix::operator ()(int row, int col)//取矩阵元素
{
	if (row < 1 || row > m_row || col < 1 || col > m_col)
	{
 		exit(1);
	}
	return m_mat[row-1][col-1];
}

CMatrix CMatrix::operator !()//矩阵求逆
{
	if (m_col != m_row)
	{
 		exit(1);
	}

	CMatrix temp(m_row);

	for (int i = 0; i < m_row; ++ i)
	{
		for (int j = 0; j < m_col; ++ j)
		{
			temp.m_mat[i][j] = m_mat[i][j];
		}
	}

	MatInverse(temp.m_mat, m_row);
	return temp;
}

CMatrix CMatrix::operator ~()//矩阵转置
{
	CMatrix temp(m_col, m_row);

	for (int i = 0; i < m_row; ++ i)
	{
		for (int j = 0; j < m_col; ++ j)
		{
			temp.m_mat[j][i] = m_mat[i][j];
		}
	}

	return temp;
}

CMatrix CMatrix::operator *(double d)//矩阵与数相乘
{
	CMatrix temp(m_row, m_col);

	for (int i = 0; i < m_row; ++ i)
	{
		for (int j = 0; j < m_col; ++ j)
		{
			temp.m_mat[i][j] = m_mat[i][j] * d;
		}
	}
	return temp;
}

CMatrix CMatrix::operator /(double d)//矩阵与数相除
{
	CMatrix temp(m_row, m_col);

	for (int i = 0; i < m_row; ++ i)
	{
		for (int j = 0; j < m_col; ++ j)
		{
			temp.m_mat[i][j] = m_mat[i][j] / d;
		}
	}
	return temp;
}

double CMatrix::MaxMod()
{
	double temp = 0.0;

	for (int i = 0; i < m_col; ++ i)
	{
		for (int j = 0; j < m_row; ++ j)
		{
			temp = max(abs(m_mat[j][i]), temp);
		}
	}
	return temp;
}