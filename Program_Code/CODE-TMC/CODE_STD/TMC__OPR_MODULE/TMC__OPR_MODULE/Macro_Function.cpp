#include "StdAfx.h"
#include "CCommon_System.h"


// ...
int  Macro__CHECK_LPx_INDEX(const CString& stn_name)
{
	CString lp_name;
	
	for(int i=0; i<5; i++)
	{
		lp_name.Format("LP%1d",i+1);
		if(stn_name.CompareNoCase(lp_name) != 0)		continue;

		return i;
	}
	return -1;
}
int  Macro__CHECK_STx_INDEX(const CString& stn_name)
{
	if(stn_name.CompareNoCase("BUFF1") == 0)		return 0;
	if(stn_name.CompareNoCase("BUFF2") == 0)		return 1;

	return -1;
}

CString Macro__GET_LLx_NAME(const int ll_index)
{
	if(ll_index == 0)			return "LBA";
	if(ll_index == 1)			return "LBB";
	if(ll_index == 2)			return "LBC";
	if(ll_index == 3)			return "LBD";

	return "??";
}
int  Macro__CHECK_LLx_INDEX(const CString& stn_name)
{
	CString ll_name;

	for(int i=0; i<4;i++)
	{
		ll_name.Format("LL%1d",i+1);
		if(ll_name.CompareNoCase(stn_name) == 0)		return i;

		ll_name = Macro__GET_LLx_NAME(i);
		if(ll_name.CompareNoCase(stn_name) == 0)		return i;
	}

	return -1;
}
