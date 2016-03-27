#ifndef _AFX_STORE_DATA_HEADER_
#define _AFX_STORE_DATA_HEADER_
#pragma once
class StoreData
{
public:
	double state[4][1];

public:
	StoreData(void);
	StoreData(double, double, double, double);
	~StoreData(void);
};

#endif