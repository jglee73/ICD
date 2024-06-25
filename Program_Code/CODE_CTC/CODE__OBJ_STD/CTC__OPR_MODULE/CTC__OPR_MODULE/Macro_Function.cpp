#include "StdAfx.h"
#include "CCommon_System.h"


// ...
int  Macro__Get_LLx_INDEX_OF_LLx_NAME(const CString& ll_name)
{
	if(ll_name.CompareNoCase("LL1") == 0)			return 0;
	if(ll_name.CompareNoCase("LL2") == 0)			return 1;
	if(ll_name.CompareNoCase("LL3") == 0)			return 2;
	if(ll_name.CompareNoCase("LL4") == 0)			return 3;

	return -1;
}

int  Macro__Get_LL_INDEX(const CString& ll_name)
{
	if(ll_name.CompareNoCase("LBA") == 0)			return 0;
	if(ll_name.CompareNoCase("LBB") == 0)			return 1;
	if(ll_name.CompareNoCase("LBC") == 0)			return 2;
	if(ll_name.CompareNoCase("LBD") == 0)			return 3;

	return Macro__Get_LLx_INDEX_OF_LLx_NAME(ll_name);
}

int  Macro__Get_PM_INDEX(const CString& pm_name)
{
	CString str_name;
	int i;

	for(i=0;i<CFG_PM_LIMIT;i++)
	{
		str_name.Format("PM%1d",i+1);

		if(pm_name.Find(str_name) == 0)
		{
			return (i);
		}
	}
	return -1;
}

CString Macro__Get_STx_NAME(const CString& st_name)
{
	if(st_name.CompareNoCase(STR__ST1) == 0)		return STR__BUFF1;
	if(st_name.CompareNoCase(STR__ST2) == 0)		return STR__BUFF2;

	return st_name;
}

int  Macro__Get_LP_INDEX(const CString& lp_name)
{
	CString str_name;
	int i;

	for(i=0;i<CFG_LP_LIMIT;i++)
	{
		str_name.Format("LP%1d",i+1);

		if(lp_name.Find(str_name) == 0)
		{
			return (i);
		}
	}
	return -1;
}
int  Macro__Check_SEQ_ID_From_PMx(const CString& pm_name)
{
	CString str_name;
	int i;

	for(i=0;i<CFG_PM_LIMIT;i++)
	{
		str_name.Format("PM%1d",i+1);

		if(pm_name.Find(str_name) == 0)
		{
			return (i+2);
		}
	}

	if((pm_name.CompareNoCase("LL1") == 0)
	|| (pm_name.CompareNoCase("LL2") == 0)
	|| (pm_name.CompareNoCase("LL3") == 0)
	|| (pm_name.CompareNoCase("LL4") == 0))
	{
		return 1;
	}
	return -1;
}
int  Macro__Check_Item(const CString& str_item,const CStringArray& l_item)
{
	int limit = l_item.GetSize();
	int i;

	for(i=0;i<limit;i++)
	{
		if(str_item.CompareNoCase(l_item[i]) == 0)
		{
			return 1;
		}
	}
	return -1;
}

CString MACRO__Get_Quotation_String(const CString& string)
{
	CString word;

	bool active_comment = false;
	int i_limit = string.GetLength();

	for(int i=0; i<i_limit; i++)
	{
		char ch = string.GetAt(i);

		if(ch == '\"')
		{
			if(active_comment)			active_comment = false;
			else						active_comment = true;

			continue;
		}
		else if(active_comment)
		{
			word += ch;
		}
	}

	if(active_comment)		return "";
	return word;
}

int  Macro__Get_Para_List_From_String(const CString& str_data,
									  CStringArray& l_para)
{
	// ...
	{
		l_para.RemoveAll();
	}

	// ...
	int w_count = 0;
	CString word;
	int ch_flag = -1;
	char ch;

	int limit = str_data.GetLength();
	int i;

	for(i=0;i<limit;i++)
	{
		ch = str_data.GetAt(i);

		if(ch == '[')
		{
			ch_flag = 1;

			w_count++;
		}
		else if(ch == ']')
		{
			ch_flag = -1;

			l_para.Add(word);
			word = "";
		}
		else
		{
			if(ch_flag > 0)
			{
				word += ch;
			}
		}
	}

	return 1;
}

void Macro__Load_StringArray(CStringArray& l_db_array,const CStringArray& l_src_array)
{
	CString str_db;

	int limit = l_src_array.GetSize();
	int i;

	for(i=0;i<limit;i++)
	{
		str_db = l_src_array[i];

		// ...
		int flag = -1;

		int k_limit = l_db_array.GetSize();
		int k;

		for(k=0;k<k_limit;k++)
		{
			if(str_db.CompareNoCase(l_db_array[k]) == 0)
			{
				flag = 1;
				break;
			}
		}

		if(flag < 0)
		{
			l_db_array.Add(str_db);
		}
	}
}

// ...
int  Macro__Analyze_Title_Info(const CString& title,
							   int& wfr_ptn,
							   int& wfr_slot)
{
	CString bff = title;
	CString str_ptn  = title;
	CString str_slot = title;

	wfr_ptn  = -1;
	wfr_slot = -1;

	// ...
	int db_index = bff.Find(":",0);

	if(db_index < 0)		return -1;

	str_ptn.Delete(db_index,bff.GetLength()-db_index);
	str_slot.Delete(0,db_index+1);

	if(str_ptn.GetLength() < 2)		return -1;
	if(str_ptn.GetAt(0) != 'P')		return -1;
	str_ptn.Delete(0,1);

	wfr_ptn  = atoi(str_ptn);
	wfr_slot = atoi(str_slot);

	return 1;
}

CString Macro__Get_Current_Date()
{
	SYSTEMTIME file__sys_time;
	GetLocalTime(&file__sys_time);

	CString str_date;
	str_date.Format("%04d_%02d_%02d",		
					file__sys_time.wYear,
					file__sys_time.wMonth,
					file__sys_time.wDay);

	return str_date;
}
CString Macro__Get_Current_Date_Time()
{
	SYSTEMTIME file__sys_time;
	GetLocalTime(&file__sys_time);

	CString str_date;
	str_date.Format("%04d-%02d-%02d %02d:%02d:%02d",		
					file__sys_time.wYear,
					file__sys_time.wMonth,
					file__sys_time.wDay,
					file__sys_time.wHour,
					file__sys_time.wMinute,
					file__sys_time.wSecond);

	return str_date;
}
