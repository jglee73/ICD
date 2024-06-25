#pragma once

#include "Interface_Code.h"


class CMacro_FA
{
private:
	// ...
	CRITICAL_SECTION mCS_LOCK;

	CX_FA_300mm_LINK xFA_300mm_Link;

	int iInit_Flag;
	int iCheck_Count;

	// ...
	int Fnc_Init();
	int Check__Address(const void* p_addr,const CString& msg);

public:
	CMacro_FA();
	~CMacro_FA();

	// ...
	CI_FA_300mm__E30_CTRL* Get__FA_E30_CTRL();
	CI_FA_300mm__E40_CTRL* Get__FA_E40_CTRL();
	CI_FA_300mm__E94_CTRL* Get__FA_E94_CTRL();
};
