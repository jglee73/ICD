#include "stdafx.h"
#include "CCommon_Utility.h"


// ...
CCommon_Utility::CCommon_Utility()
{

}
CCommon_Utility::~CCommon_Utility()
{

}

// ...
bool CCommon_Utility::Check__Link(const CString& str_link)
{
	if(str_link.CompareNoCase("NO")   == 0)		return false;
	if(str_link.CompareNoCase("NONE") == 0)		return false;
	if(str_link.CompareNoCase("NULL") == 0)		return false;

	return true;
}

// ...
int CCommon_Utility
::Get__StringArrray_From_String(const CString& str_org_data,const CString& str_sep,CStringArray& l_data, const bool active__empty_count)
{
	CString str_data = str_org_data;

	l_data.RemoveAll();

	while(1)
	{
		int db_index = str_data.Find(str_sep);
		if(db_index < 0)
		{
			if(str_data.GetLength() > 0)
			{
				l_data.Add(str_data);
			}
			break;
		}

		CString str_token = str_data;			
		str_token.Delete(db_index, str_token.GetLength()-db_index);

		str_data.Delete(0, 1+db_index);

		if(db_index > 0)
		{
			l_data.Add(str_token);
		}
		else if(db_index == 0)
		{
			if(active__empty_count)				l_data.Add("");
		}
	}

	return 1;
}
int CCommon_Utility
::Get__Hexa_From_String(const CString& str_data)
{
	int i_hexa = 0;

	int i_limit = str_data.GetLength();
	int i;

	for(i=0; i<i_limit; i++)
	{
		char ch = str_data[i];

		i_hexa *= 0x10;

		if((ch >= '0') && (ch <= '9'))
		{
			i_hexa += (ch - '0');
			continue;
		}
		if((ch >= 'A') && (ch <= 'F'))
		{
			i_hexa += ((ch - 'A') + 10);
			continue;
		}
		if((ch >= 'a') && (ch <= 'f'))
		{
			i_hexa += ((ch - 'a') + 10);
			continue;
		}		
	}

	return i_hexa;
}

int CCommon_Utility
::Check__Same_String(const CString& str_data, const CStringArray& l_data)
{
	int i_limit = l_data.GetSize();
	int i;

	for(i=0; i<i_limit; i++)
	{
		if(str_data.CompareNoCase(l_data[i]) == 0)
			return i;
	}
	return -1;
}
