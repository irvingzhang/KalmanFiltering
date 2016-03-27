//保存的全局变量
#ifndef _GLOBAL_VARIABLES
#define _GLOBAL_VARIABLES

#pragma once
#include <cmath>
#include "Matrix.h"

const int MAX = 1001;
const int TimeInterval = 1;//时间间隔
const double Qn = pow(0.0005, 2);//过程噪声
const double Rn = pow(0.001, 2);//测量噪声
const int dim = 4;//模型维数
const int cpoints = 2 * dim;//三维CKF的容积点数
const int hcpoints =  2 * (int)pow((double)dim, 2) + 1;//五维CKF的容积点数
const int u_k = 0;//3 - dim;
const int u_symmetry_number = dim;
const int u_total_number = 2 * dim + 1;//UKF的采样点数
const double u_weight_1 = u_k / ((double)u_symmetry_number + u_k);
const double u_weight_N1 = 1 / (2 * ((double)u_symmetry_number + u_k));
const int icpoints = (int)pow((double)2, dim) + 1;//ICKF的容积点数
const double delta = 1.0;
const double c_w = 1 / (double)cpoints;//三阶CKF的权值
const double ic_w1 = 1 / ((double)delta * (int)pow((double)2, dim + 1));//ICKF的权值
const double ic_w2 = 1 - 1 / (2 * (double)delta);
const double c5_w1 = (4 - (double)dim) / 18;//五阶CKF的权值
const double c5_w2 = (double) 1 - (7 - (double)dim) * (double)dim / 18;
const double c5_w4 = (double) 1 / 36;

class Globals
{
public:
	CMatrix fai;
	CMatrix gama;
	CMatrix Pplus;
	CMatrix xhat;
	CMatrix Im;
	CMatrix I;
	CMatrix kesi_ICKF;
	CMatrix kesi_ThirdCKF;
	CMatrix kesi_FifthCKF;

public:
	Globals(void);
	~Globals(void);
};

#endif