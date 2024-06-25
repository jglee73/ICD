#include "StdAfx.h"
#include "CCommon_System.h"


// ...
int  Macro__Get_Job_List_From_DB_List(const CUIntArray& l_db_list,CUIntArray& l_job_list)
{
	l_job_list.RemoveAll();

	// ...
	int i_limit = l_db_list.GetSize();
	int i;

	for(i=0;i<i_limit;i++)
	{
		int db_id = l_db_list[i];

		// ...
		{
			int check_flag = -1;

			int k_limit = l_job_list.GetSize();
			int k;

			for(k=0;k<k_limit;k++)
			{
				int job_id = l_job_list[k];
				if(job_id != db_id)			continue;

				check_flag = 1;
				break;
			}

			if(check_flag < 0)
			{
				l_job_list.Add(db_id);	
			}
		}
	}

	return 1;
}

int  Macro__Check_PMC_ID(const int pmc_id,const CStringArray& l_pmc_id)
{
	CString str_pmc;
	CString cmp_pmc;

	// ...
	{
		str_pmc.Format("PM%1d", pmc_id);
	}

	// ...
	int limit = l_pmc_id.GetSize();
	int i;

	for(i=0;i<limit;i++)
	{
		if(str_pmc.CompareNoCase(l_pmc_id[i]) == 0)
		{
			return 1;
		}
	}
	return -1;
}
int  Macro__Get_PMC_INDEX(const CString& str_pmc)
{
	CString cmp_pmc;

	int limit = CFG_PM_LIMIT;
	int i;

	for(i=0;i<limit;i++)
	{
		cmp_pmc.Format("PM%1d", i+1);

		if(str_pmc.Find(cmp_pmc) == 0)
		{
			return i;
		}
	}
	return -1;
}
int  Macro__Get_PMC_ID(const CString& str_pmc)
{
	int p_index = Macro__Get_PMC_INDEX(str_pmc);
	
	if(p_index < 0)		return -1;
	return (p_index + 1);
}

// ...
int  Macro__Analyze_Title(const CString& title,CString& str_ptn,CString& str_slot)
{
	CString bff = title;
	
	str_ptn  = title;
	str_slot = title;

	int db_index = bff.Find(":",0);

	//
	if(db_index < 0)		return -1;

	str_ptn.Delete(db_index,bff.GetLength()-db_index);
	str_slot.Delete(0,db_index+1);

	//
	if(str_ptn.GetLength() < 2)		return -1;
	if(str_ptn.GetAt(0) != 'P')		return -1;

	str_ptn.Insert(0,"L");

	//
	int i_slot = atoi(str_slot);
	str_slot.Format("%1d",i_slot);
	
	return 1;
}
int  Macro__Analyze_Title_Info(const CString& title,
							   int& wfr_ptn,
							   int& wfr_slot)
{
	//.....
	CString bff = title;
	CString str_ptn  = title;
	CString str_slot = title;

	wfr_ptn  = -1;
	wfr_slot = -1;

	//.....
	int db_index = bff.Find(":",0);

	//
	if(db_index < 0)		return -1;

	str_ptn.Delete(db_index,bff.GetLength()-db_index);
	str_slot.Delete(0,db_index+1);

	//
	if(str_ptn.GetLength() < 2)		return -1;
	if(str_ptn.GetAt(0) != 'P')		return -1;
	str_ptn.Delete(0,1);

	//
	wfr_ptn  = atoi(str_ptn);
	wfr_slot = atoi(str_slot);
	
	return 1;
}

int  Macro__Check_Equal__Any_String(const CString& cmp_data,
									const CStringArray& l_data)
{
	// ...
	int i_limit;
	int i;
	
	i_limit = l_data.GetSize();
	
	for(i=0;i<i_limit;i++)
	{
		if(cmp_data.CompareNoCase(l_data[i]) == 0)
		{
			return 1;
		}
	}
	
	return -1;
}

int  Macro__Check_Equal__Any_String(const CStringArray& l_data1,
									const CStringArray& l_data2)
{
	// ...
	CString str_data1;
	CString str_data2;

	int i_limit;
	int i;

	int k_limit;
	int k;

	// ...
	i_limit = l_data1.GetSize();
	
	for(i=0;i<i_limit;i++)
	{
		str_data1 = l_data1[i];

		// ...
		k_limit = l_data2.GetSize();

		for(k=0;k<k_limit;k++)
		{
			str_data2 = l_data2[k];

			if(str_data1.CompareNoCase(str_data2) == 0)
			{
				return 1;
			}
		}
	}
	
	return -1;
}

int  Macro__Check_Equal__All_String(const CStringArray& l_data1,
									const CStringArray& l_data2)
{
	// ...
	CString str_data1;
	CString str_data2;
	
	int i_limit;
	int i;
	
	int k_limit;
	int k;
	
	// ...
	i_limit = l_data1.GetSize();
	k_limit = l_data2.GetSize();

	if(i_limit != k_limit)
	{
		return -1;
	}
	
	for(i=0;i<i_limit;i++)
	{
		str_data1 = l_data1[i];
		
		for(k=0;k<k_limit;k++)
		{
			str_data2 = l_data2[k];
			
			if(str_data1.CompareNoCase(str_data2) != 0)
			{
				return -1;
			}
		}
	}
	
	return 1;
}

int  Macro__Check__Src_Item_Included(const CStringArray& l_src_data,
								     const CStringArray& l_trg_data)
{
	// ...
	CString src_data;
	CString trg_data;
	
	int i_limit;
	int i;
	
	int k_limit;
	int k;
	
	// ...
	i_limit = l_src_data.GetSize();
	
	for(i=0;i<i_limit;i++)
	{
		src_data = l_src_data[i];
		
		// ...
		int check_flag = -1;

		k_limit = l_trg_data.GetSize();
		
		for(k=0;k<k_limit;k++)
		{
			trg_data = l_trg_data[k];
			
			if(src_data.CompareNoCase(trg_data) != 0)
			{
				continue;
			}

			check_flag = 1;
			break;
		}

		if(check_flag < 0)
		{
			return -1;
		}
	}
	
	return 1;
}

int  Macro__Check_PM_Index_With_StringArray(const int pm_i,
											const CStringArray& l_pmc)
{
	// ...
	CString pmc_name;
	pmc_name.Format("PM%1d", pm_i+1);	

	// ...
	int limit = l_pmc.GetSize();
	int i;

	for(i=0;i<limit;i++)
	{
		if(l_pmc[i].Find(pmc_name) == 0)
		{
			return 1;
		}
	}
	return -1;
}

int  Macro__Delete_Trg_With_Src(const CStringArray& l_src_name,
								const CStringArray& l_src_rcp,
								const CStringArray& l_trg_name,
								const CStringArray& l_trg_rcp,
								CStringArray& l_pmc_name,
								CStringArray& l_pmc_rcp)
{
	// ...
	{
		l_pmc_name.RemoveAll();
		l_pmc_rcp.RemoveAll();
	}

	// ...
	int i_limit;
	int i;

	int k_limit;
	int k;
	
	i_limit = l_trg_name.GetSize();

	for(i=0;i<i_limit;i++)
	{
		CString trg_name = l_trg_name[i];
		CString trg_rcp  = l_trg_rcp[i];
			
		// ...
		int check_flag = -1;

		k_limit = l_src_name.GetSize();

		for(k=0;k<k_limit;k++)
		{
			CString src_name = l_src_name[k];

			if(src_name.CompareNoCase(trg_name) != 0)
			{
				continue;
			}

			check_flag = 1;
			break;
		}

		if(check_flag < 0)
		{
			l_pmc_name.Add(trg_name);
			l_pmc_rcp.Add(trg_rcp);
		}
	}

	return 1;
}

int  Macro__Change_Order__Trg_With_Src(const CStringArray& l_src_name,
									   const CStringArray& l_trg_name,
									   const CStringArray& l_trg_rcp,
									   CStringArray& l_pmc_name,
									   CStringArray& l_pmc_rcp)
{
	// ...
	{
		l_pmc_name.RemoveAll();
		l_pmc_rcp.RemoveAll();
	}
	
	// ...
	int i_limit;
	int i;
	
	int k_limit;
	int k;

	// ...
	i_limit = l_trg_name.GetSize();
	
	for(i=0;i<i_limit;i++)
	{
		CString trg_name = l_trg_name[i];
		CString trg_rcp  = l_trg_rcp[i];
		
		// ...
		int check_flag = -1;
		
		k_limit = l_src_name.GetSize();
		
		for(k=0;k<k_limit;k++)
		{
			CString src_name = l_src_name[k];
			
			if(src_name.CompareNoCase(trg_name) != 0)
			{
				continue;
			}
			
			check_flag = 1;
			break;
		}
		
		if(check_flag < 0)
		{
			l_pmc_name.Add(trg_name);
			l_pmc_rcp.Add(trg_rcp);
		}
	}

	/*
	// ...
	i_limit = l_trg_name.GetSize();
	
	for(i=0;i<i_limit;i++)
	{
		CString trg_name = l_trg_name[i];
		CString trg_rcp  = l_trg_rcp[i];
		
		// ...
		if(Macro__Check_Equal__Any_String(trg_name,l_pmc_name) > 0)
		{
			continue;
		}	

		// ...
		{
			l_pmc_name.Add(trg_name);
			l_pmc_rcp.Add(trg_rcp);
		}
	}
	*/
	
	return 1;
}

int  Macro__Delete_Trg_With_Src(const CStringArray& l_src_name,
								CStringArray& l_trg_name,
								CStringArray& l_trg_rcp)
{
	CStringArray l_pmc_name;
	CStringArray l_pmc_rcp;

	// ...
	int i_limit = l_trg_name.GetSize();

	for(int i=0; i < i_limit; i++)
	{
		CString trg_name = l_trg_name[i];
		CString trg_rcp  = l_trg_rcp[i];
		
		int check_flag = -1;

		// ...
		int k_limit = l_src_name.GetSize();
		
		for(int k=0; k < k_limit; k++)
		{
			CString src_name = l_src_name[k];
			
			if(src_name.CompareNoCase(trg_name) != 0)
			{
				continue;
			}
			
			check_flag = 1;
			break;
		}
		
		if(check_flag < 0)
		{
			l_pmc_name.Add(trg_name);
			l_pmc_rcp.Add(trg_rcp);
		}
	}

	// ...
	{
		l_trg_name.RemoveAll();
		l_trg_rcp.RemoveAll();

		l_trg_name.Copy(l_pmc_name);
		l_trg_rcp.Copy(l_pmc_rcp);
	}
	return 1;
}
