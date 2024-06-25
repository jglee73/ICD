#include "StdAfx.h"
#include "CCommon_SYSTEM.h"


// ...
int  Macro__CHECK_LPx_INDEX(const CString& stn_name)
{
	CString lp_name;

	for(int i=0; i<CFG_LPx__SIZE; i++)
	{
		lp_name.Format("LP%1d",i+1);
		if(stn_name.CompareNoCase(lp_name) != 0)		continue;

		return i;
	}
	return -1;
}

int  Macro__CHECK_LLx_INDEX(const CString& stn_name)
{
	CString ll_name;

	for(int i=0; i<CFG_LLx__SIZE; i++)
	{
		ll_name.Format("LL%1d",i+1);
		if(stn_name.CompareNoCase(ll_name) != 0)		continue;

		return i;
	}
	for(int i=0; i<CFG_LLx__SIZE; i++)
	{
			 if(i == 0)		ll_name = "LBA";
		else if(i == 1)		ll_name = "LBB";
		else				continue;

		if(stn_name.CompareNoCase(ll_name) != 0)		continue;

		return i;
	}

	return -1;
}
int  Macro__CHECK_PMx_INDEX(const CString& stn_name)
{
	CString pm_name;

	for(int i=0; i<CFG_PMx__SIZE; i++)
	{
		pm_name.Format("PM%1d",i+1);
		if(stn_name.CompareNoCase(pm_name) != 0)		continue;

		return i;
	}
	return -1;
}

CString Macro__Get_Parity(const int parity_mode)
{
	if(parity_mode == 0)		return "None";
	if(parity_mode == 1)		return "Odd";
	if(parity_mode == 2)		return "Even";
	if(parity_mode == 3)		return "Mark";
	if(parity_mode == 4)		return "Space";

	return "Unknown";
}
