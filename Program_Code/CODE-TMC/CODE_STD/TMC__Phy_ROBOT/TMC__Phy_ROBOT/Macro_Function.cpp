#include "StdAfx.h"
#include "CCommon_System.h"


// ...
int  Macro__CHECK_LPx_INDEX(const CString& stn_name)
{
	CString lp_name;
	int i;

	for(i=0;i<CFG_LPx__SIZE;i++)
	{
		lp_name.Format("LP%1d",i+1);
		if(stn_name.CompareNoCase(lp_name) != 0)		continue;

		return i;
	}
	return -1;
}
int  Macro__CHECK_STx_INDEX(const CString& stn_name)
{
	if((stn_name.CompareNoCase(STR__ST1)   == 0)
	|| (stn_name.CompareNoCase(STR__BUFF1) == 0))
	{
		return 0;
	}
	if((stn_name.CompareNoCase(STR__ST2)   == 0)
	|| (stn_name.CompareNoCase(STR__BUFF2) == 0))
	{
		return 1;
	}

	return -1;
}

int  Macro__CHECK_PMx_INDEX(const CString& stn_name)
{
	CString pm_name;
	int i;

	for(i=0;i<CFG_PMx__SIZE;i++)
	{
		pm_name.Format("PM%1d",i+1);

		int p_index = stn_name.Find(pm_name);
		if(p_index < 0)			continue;

		return i;
	}
	return -1;
}

CString Macro__GET_LLx_NAME(const int ll_index)
{
	if(ll_index == 0)			return _MODULE__LBA;
	if(ll_index == 1)			return _MODULE__LBB;
	if(ll_index == 2)			return _MODULE__LBC;
	if(ll_index == 3)			return _MODULE__LBD;

	return _MODULE__LLx;
}
int  Macro__CHECK_LLx_INDEX(const CString& stn_name)
{
	CString ll_name;
	int i;

	for(i=0; i<CFG_LLx__SIZE;i++)
	{
		ll_name.Format("LL%1d",i+1);
		if(ll_name.CompareNoCase(stn_name) == 0)		return i;

		ll_name = Macro__GET_LLx_NAME(i);
		if(ll_name.CompareNoCase(stn_name) == 0)		return i;
	}

	return -1;
}

int  Macro__Get_PortID_SlotID_Of_Wafer_Title(const CString& wfr_title,
											 CString& port_id,
											 CString& slot_id)
{
	// ...
	{
		port_id = "";
		slot_id = "";
	}

	int active__portid = -1;
	int active__slotid = -1;

	int i_len = wfr_title.GetLength();
	int i;

	for(i=0; i<i_len;i++)
	{
		char ch = (char) wfr_title.GetAt(i);

		if(ch == 'P')
		{
			active__portid = 1;
			continue;
		}
		else if(ch == ':')
		{
			if(active__portid > 0)
			{
				active__portid = -1;
				active__slotid = 1;
			}
			continue;
		}

		if(active__portid > 0)
		{
			port_id += ch;
		}
		else if(active__slotid > 0)
		{
			slot_id += ch;
		}
	}

	if((active__portid > 0)
	&& (active__slotid > 0))
	{
		return 1;
	}
	return -1;
}

