//////////////////////////////////////////////////////////////////////////////////
//Version: 20130614                                                            ///
//(Matrix class for the basic matrix operations)                               ///    
//                                                                             ///
//                                                by  Xin-Chun Zhang(UESTC)    ///
//                                                   irving_zhang@163.com      ///
//                                                  Update data: 2013-6-14     ///
//////////////////////////////////////////////////////////////////////////////////

#ifndef _AFX_MATRIX_HEADER_
#define _AFX_MATRIX_HEADER_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define   TINY 1.0e-20

//矩阵类，用于矩阵的运算
class CMatrix  
{
public:
	double **m_mat;//矩阵指针
	int m_row;//矩阵的行列
	int m_col;

public:
	CMatrix();
	CMatrix(int);//重载构造函数，用于构造方阵
	CMatrix(int,int);//重载构造函数，用于构造指定行和列的矩阵
	CMatrix (const CMatrix &);//显式复制构造函数
	virtual ~CMatrix();//析构函数

public:
	void SetSize(int);//设定方阵的维数
	void SetSize(int,int);//设定矩阵的行和列
	double MaxMod();//返回模值（绝对值）最大的元素
	CMatrix Cholesky();//Cholesky分解，并取下三角矩阵

public:
	//以下为操作符重载
	CMatrix& operator =(CMatrix &);//矩阵赋值
	CMatrix operator +(CMatrix &);//矩阵加法
	CMatrix operator -(CMatrix &);//矩阵减法
	CMatrix operator *(CMatrix &);//矩阵乘法
	CMatrix operator /(double);//矩阵除以一个数
	CMatrix operator !();//矩阵求逆
	CMatrix operator ~();//矩阵转置
	double & operator ()(int,int);//取矩阵某行某列的元素
	CMatrix operator *(double);//矩阵与数相乘
	friend CMatrix operator *(double d , CMatrix& m) {return m * d;}//数与矩阵相乘
	

//Dolittle分解求矩阵逆
protected:
	void MatInverse(double** mat, int dim);
	void MatLudcmp(double** mat, int, int*, double*);
	void MatLubksb(double** mat, int, int*, double*);
};

#endif // _AFX_MATRIX_HEADER_
