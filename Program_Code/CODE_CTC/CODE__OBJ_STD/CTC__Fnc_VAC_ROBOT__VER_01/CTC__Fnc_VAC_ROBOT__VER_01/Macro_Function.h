#pragma once


extern int  Macro__Get_Job_List_From_DB_List(const CUIntArray& l_db_list,CUIntArray& l_job_list);
extern int  Macro__Check_PMC_ID(const int pmc_id,const CStringArray& l_pmc_id);
extern int  Macro__Get_PMC_ID(const CString& str_pmc);
extern int  Macro__Get_PMC_INDEX(const CString& str_pmc);

extern int  Macro__Analyze_Title(const CString& title,CString& str_ptn,CString& str_slot);
extern int  Macro__Analyze_Title_Info(const CString& title,int& wfr_ptn,int& wfr_slot);

extern int  Macro__Check_Equal__Any_String(const CString& cmp_data,const CStringArray& l_data);
extern int  Macro__Check_Equal__Any_String(const CStringArray& l_data1,const CStringArray& l_data2);
extern int  Macro__Check_Equal__All_String(const CStringArray& l_data1,const CStringArray& l_data2);

extern int  Macro__Check__Src_Item_Included(const CStringArray& l_src_data,const CStringArray& l_trg_data);
extern int  Macro__Check_PM_Index_With_StringArray(const int pm_i,const CStringArray& l_pmc);

extern int  Macro__Delete_Trg_With_Src(const CStringArray& l_src_name,
										const CStringArray& l_src_rcp,
										const CStringArray& l_trg_name,
										const CStringArray& l_trg_rcp,
										CStringArray& l_pmc_name,
										CStringArray& l_pmc_rcp);
extern int  Macro__Delete_Trg_With_Src(const CStringArray& l_src_name,
										CStringArray& l_trg_name,
										CStringArray& l_trg_rcp);

extern int  Macro__Change_Order__Trg_With_Src(const CStringArray& l_src_name,
									   const CStringArray& l_trg_name,
									   const CStringArray& l_trg_rcp,
									   CStringArray& l_pmc_name,
									   CStringArray& l_pmc_rcp);
