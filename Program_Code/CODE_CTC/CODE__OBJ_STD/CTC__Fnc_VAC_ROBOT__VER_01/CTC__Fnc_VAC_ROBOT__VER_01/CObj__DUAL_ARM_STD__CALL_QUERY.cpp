#include "StdAfx.h"
#include "CObj__DUAL_ARM_STD.h"


#define _QRY__GET_EMPTY_ARM                 "GET.EMPTY_ARM"


// ...
int CObj__DUAL_ARM_STD
::__Define__QUERY(CStringArray& l_query)
{
	l_query.Add(_QRY__GET_EMPTY_ARM);

	return 1;
}

int CObj__DUAL_ARM_STD
::__Call__QUERY_LIST(const CString& query_name, CStringArray& l_data)
{
	l_data.RemoveAll();

	if(query_name.CompareNoCase(_QRY__GET_EMPTY_ARM) == 0)
	{
		return pVAC_RB__OBJ_CTRL->Call__QUERY_LIST(query_name, l_data);
	}

	return -1;
}
int CObj__DUAL_ARM_STD
::__Call__QUERY_LIST(const CString& query_name,const CStringArray& l_sub_query, CStringArray& l_data)
{
	int log_enable = dCH__QUERY_LOG_MODE->Check__DATA(STR__ENABLE);

	if(log_enable > 0)
	{
		CString log_msg = "";
		CString log_bff;

		log_msg = " * CObj__DUAL_ARM_STD::__Call__QUERY_LIST() ... \n";
		
		int i_limit = l_sub_query.GetSize();
		int i;

		for(i=0; i<i_limit; i++)
		{
			log_bff.Format("  ** %1d) %s \n", i+1,l_sub_query[i]);
			log_msg += log_bff;
		}
		printf(log_msg);
	}

	// ...
	l_data.RemoveAll();

	if(query_name.CompareNoCase(_QRY__GET_EMPTY_ARM) == 0)
	{
		int r_flag = pVAC_RB__OBJ_CTRL->Call__QUERY_LIST(query_name, l_data);

		if(log_enable > 0)
		{
			CString log_msg = "";
			CString log_bff;

			int i_limit = l_data.GetSize();
			int i;

			for(i=0; i<i_limit; i++)
			{
				log_bff.Format("  ** Empty_Arm : %s \n", l_data[i]);
				log_msg += log_bff;
			}

			printf(log_msg);
		}
		return r_flag;
	}

	return -1;
}
