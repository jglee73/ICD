#include "StdAfx.h"


// ...
int Macro__Get_Date_Time(CString& date_time)
{
	CTime t = CTime::GetCurrentTime();

	date_time.Format("%00004d%002d%002d%002d%002d%002d",
					 t.GetYear(),
					 t.GetMonth(),
					 t.GetDay(),
					 t.GetHour(),
					 t.GetMinute(),
					 t.GetSecond());
	return 1;
}

int Macro__Get_Date_Time(CString& str_date, CString& str_time)
{
	CTime t = CTime::GetCurrentTime();

	str_date.Format("%00004d_%002d_%002d", t.GetYear(),t.GetMonth(),t.GetDay());
	str_time.Format("%002d%002d%002d", t.GetHour(),t.GetMinute(),t.GetSecond());

	return 1;
}

int Macro__Get_Wafer_Info(const CString& wfr_title, int& lp_id,int& lp_slot)
{
	CString wfr_bff = wfr_title;	

	// ...
	{
		int s_index = wfr_bff.Find("P");
		if(s_index < 0)		return -1;

		wfr_bff.Delete(0, s_index+1);
	}
	lp_id = atoi(wfr_bff);

	// ...
	{
		int s_index = wfr_bff.Find(":");
		if(s_index < 0)		return -2;

		wfr_bff.Delete(0, s_index+1);
	}
	lp_slot = atoi(wfr_bff);

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

CString Macro__Get_Current_Time()
{
	SYSTEMTIME file__sys_time;
	GetLocalTime(&file__sys_time);

	CString str_time;
	str_time.Format("%02d:%02d:%02d.%03d",		
					file__sys_time.wHour,
					file__sys_time.wMinute,
					file__sys_time.wSecond,
					file__sys_time.wMilliseconds);

	return str_time;
}
