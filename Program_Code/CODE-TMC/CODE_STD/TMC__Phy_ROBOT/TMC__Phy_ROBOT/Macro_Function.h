#pragma once


extern int  Macro__CHECK_LPx_INDEX(const CString& stn_name);
extern int  Macro__CHECK_STx_INDEX(const CString& stn_name);

extern int  Macro__CHECK_PMx_INDEX(const CString& stn_name);

extern CString Macro__GET_LLx_NAME(const int ll_index);
extern int  Macro__CHECK_LLx_INDEX(const CString& stn_name);

extern int  Macro__Get_PortID_SlotID_Of_Wafer_Title(const CString& wfr_title,
													CString& port_id,
													CString& slot_id);

