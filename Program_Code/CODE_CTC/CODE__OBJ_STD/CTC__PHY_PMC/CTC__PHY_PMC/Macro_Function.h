#pragma once


extern int Macro__Get_Date_Time(CString &date_time);
extern int Macro__Get_Date_Time(CString& str_date, CString& str_time);
extern int Macro__Get_Wafer_Info(const CString& wfr_title, int& lp_id,int& lp_slot);

extern CString Macro__Get_Current_Date();
extern CString Macro__Get_Current_Time();
