#include "StdAfx.h"


//.....
int  Macro__Get_Date_Time(CString &date_time)
{
	CTime t = CTime::GetCurrentTime();

	date_time.Format("%00004d.%002d.%002d %002d:%002d:%002d",
		t.GetYear(),
		t.GetMonth(),
		t.GetDay(),
		t.GetHour(),
		t.GetMinute(),
		t.GetSecond());
	return 1;
}

CString Macro__Get_SystemTimeFile()
{
	SYSTEMTIME sz_system_time;
	GetLocalTime(&sz_system_time);

	CString str_time;
	str_time.Format("%04d_%02d_%02d-%02d_%02d_%02d", 		
		sz_system_time.wYear,
		sz_system_time.wMonth,
		sz_system_time.wDay,
		sz_system_time.wHour,
		sz_system_time.wMinute,
		sz_system_time.wSecond);

	return str_time;
}
CString Macro__Get_SystemTimeDir()
{
	SYSTEMTIME sz_system_time;
	GetLocalTime(&sz_system_time);

	CString str_time;
	str_time.Format("%04d_%02d", 		
		sz_system_time.wYear,
		sz_system_time.wMonth);

	return str_time;
}
