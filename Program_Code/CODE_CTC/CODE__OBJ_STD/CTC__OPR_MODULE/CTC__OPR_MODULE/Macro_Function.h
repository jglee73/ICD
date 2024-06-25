#pragma once


extern int  Macro__Get_LLx_INDEX_OF_LLx_NAME(const CString& ll_name);

extern int  Macro__Get_LL_INDEX(const CString& ll_name);
extern int  Macro__Get_PM_INDEX(const CString& pm_name);
extern int  Macro__Get_LP_INDEX(const CString& lp_name);

extern CString Macro__Get_STx_NAME(const CString& st_name);

extern int  Macro__Check_SEQ_ID_From_PMx(const CString& pm_name);
extern int  Macro__Check_Item(const CString& str_item,const CStringArray& l_item);

extern CString MACRO__Get_Quotation_String(const CString& string);
extern int  Macro__Get_Para_List_From_String(const CString& str_data,CStringArray& l_para);

extern void Macro__Load_StringArray(CStringArray& l_db_array,const CStringArray& l_src_array);
extern int  Macro__Analyze_Title_Info(const CString& title, int& wfr_ptn,int& wfr_slot);

extern CString Macro__Get_Current_Date();
extern CString Macro__Get_Current_Date_Time();

