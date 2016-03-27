#include "StdAfx.h"
#include "StoreData.h"


StoreData::StoreData(void)
{
}

StoreData::StoreData(double x1, double x2, double x3, double x4)
{
	this->state[0][0] = x1;
	this->state[1][0] = x2;
	this->state[2][0] = x3;
	this->state[3][0] = x4;
}

StoreData::~StoreData(void)
{
}
