#include "StdAfx.h"
#include "CMacro_FA.h"


CMacro_FA mFA_Link;


// ...
CMacro_FA::CMacro_FA()
{
	InitializeCriticalSection(&mCS_LOCK);

	iInit_Flag = -1;
	iCheck_Count = 0;
}
CMacro_FA::~CMacro_FA()
{

	DeleteCriticalSection(&mCS_LOCK);
}

// ...
int  CMacro_FA::Fnc_Init()
{
	if(iInit_Flag > 0)			return 1;

	EnterCriticalSection(&mCS_LOCK);

	while(1)
	{
		iInit_Flag = xFA_300mm_Link->Open("");

		if(iInit_Flag < 0)
		{
			Sleep(100);
			
			if(iCheck_Count == 10)		printf("CTC__PHY_PMC.CMacro_FA::Fnc_Init() - Error ! \n");
			if(iCheck_Count >  10)		break;

			iCheck_Count++;
			continue;
		}

		break;
	}

	LeaveCriticalSection(&mCS_LOCK);
	return iInit_Flag;
}
int  CMacro_FA::Check__Address(const void* p_addr,const CString& msg)
{
	if(p_addr != NULL)
	{
		return 1;
	}
	return -1;
}

// ...
CI_FA_300mm__E30_CTRL* CMacro_FA::Get__FA_E30_CTRL()
{
	if(Fnc_Init() < 0)		return NULL;

	return xFA_300mm_Link->Get_FA_300mm_CTRL()->Get_E30();
}
CI_FA_300mm__E40_CTRL* CMacro_FA::Get__FA_E40_CTRL()
{
	if(Fnc_Init() < 0)		return NULL;

	return xFA_300mm_Link->Get_FA_300mm_CTRL()->Get_E40();
}
CI_FA_300mm__E94_CTRL* CMacro_FA::Get__FA_E94_CTRL()
{
	if(Fnc_Init() < 0)		return NULL;

	return xFA_300mm_Link->Get_FA_300mm_CTRL()->Get_E94();
}
