#include "StdAfx.h"
#include "CObj__DUAL_ARM_STD.h"


#define _QRY__GET_EMPTY_ARM                 "GET.EMPTY_ARM"

#define _QRY__GET_EMPTY_SLOT_OF_BUUFER      "GET_EMPTY_SLOT_OF_BUUFER"
#define _QRY__CHECK_BUSY_SLOT_OF_BUUFER     "CHECK_BUSY_SLOT_OF_BUUFER"


// ...
int CObj__DUAL_ARM_STD
::__Define__QUERY(CStringArray& l_query)
{
	l_query.Add(_QRY__GET_EMPTY_ARM);

	l_query.Add(_QRY__GET_EMPTY_SLOT_OF_BUUFER);
	l_query.Add(_QRY__CHECK_BUSY_SLOT_OF_BUUFER);

	return 1;
}

int CObj__DUAL_ARM_STD
::__Call__QUERY(const CString& query_name,CString& query_data)
{
	return -1;
}
int CObj__DUAL_ARM_STD
::__Call__QUERY_LIST(const CString& query_name,CStringArray& l_data)
{
	l_data.RemoveAll();

	if(query_name.CompareNoCase(_QRY__GET_EMPTY_ARM) == 0)
	{
		return pATM_RB__OBJ_CTRL->Call__QUERY_LIST(query_name, l_data);
	}
	return -1;
}
int CObj__DUAL_ARM_STD
::__Call__QUERY_LIST(const CString& query_name,const CStringArray& l_sub_query, CStringArray& l_data)
{
	CString log_msg;
	CString log_bff;

	// ...
	int log_enable = dCH__CFG_QUERY_LOG_MODE->Check__DATA(STR__ENABLE);

	if(log_enable > 0)
	{
		log_msg.Format("Query_Name <- %s \n", query_name);

		int i_limit = l_sub_query.GetSize();
		int i;

		for(i=0; i<i_limit; i++)
		{
			log_bff.Format("  %1d)  %s \n", i+1,l_sub_query[i]);
			log_msg += log_bff;
		}

		xAPP_LOG_CTRL->WRITE__LOG(log_msg);
	}

	// ...
	l_data.RemoveAll();

	if(query_name.CompareNoCase(_QRY__GET_EMPTY_ARM) == 0)
	{
		return pATM_RB__OBJ_CTRL->Call__QUERY_LIST(query_name, l_data);
	}

	if((query_name.CompareNoCase(_QRY__GET_EMPTY_SLOT_OF_BUUFER)  == 0)
	|| (query_name.CompareNoCase(_QRY__CHECK_BUSY_SLOT_OF_BUUFER) == 0))
	{
		int flag = 0;

		if(query_name.CompareNoCase(_QRY__GET_EMPTY_SLOT_OF_BUUFER) == 0)
		{
			flag = _QUERY__GET_EMPTY_SLOT_OF_BUUFER(log_enable, l_data);	
		}
		else if(query_name.CompareNoCase(_QRY__CHECK_BUSY_SLOT_OF_BUUFER) == 0)
		{
			flag = _QUERY__CHECK_BUSY_SLOT_OF_BUUFER(log_enable, l_sub_query);

			// ...
			{
				CString str_state = "UNKNOWN";

					 if(flag > 0)		str_state = "YES";
				else if(flag < 0)		str_state = "NO";

				l_data.RemoveAll();
				l_data.Add(str_state);
			}

			flag = 1;
		}

		if(log_enable > 0)
		{
			log_msg.Format("Query Result : Flag(%1d) \n", flag);

			int i_limit = l_data.GetSize();
			for(int i=0; i<i_limit; i++)
			{
				log_bff.Format("  %1d)  %s \n", i+1,l_data[i]);
				log_msg += log_bff;
			}

			xAPP_LOG_CTRL->WRITE__LOG(log_msg);
		}
		return flag;
	}

	if(log_enable > 0)
	{
		log_msg = "Query Error : Not Register !";

		xAPP_LOG_CTRL->WRITE__LOG(log_msg);
	}
	return -1;
}

int CObj__DUAL_ARM_STD
::_QUERY__GET_EMPTY_SLOT_OF_BUUFER(const int log_enable, CStringArray& l_info)
{
	l_info.RemoveAll();

	// ...
	CString str_data;

	// OBJ : SIDE BUFFER (1) ...
	if(sCH__SCH_DB_ST1_USE_FLAG->Check__DATA(STR__ENABLE) > 0)
	{
		int cfg_max = (int) xEXT_CH__BUFFER1__CFG_SLOT_MAX->Get__VALUE();
		int i;

		if(cfg_max > CFG_LP__SLOT_MAX)		
		{
			cfg_max = CFG_LP__SLOT_MAX;
		}

		for(i=0; i<cfg_max; i++)
		{
			if(xEXT_CH__BUFFER1__SLOT_STATUS[i]->Check__DATA(STR__NONE) > 0)
			{
				str_data.Format("%1d", i+1);

				l_info.RemoveAll();
				l_info.Add(MODULE__BUFFER1);
				l_info.Add(str_data);

				return 1;
			}
		}
	}

	// OBJ : SIDE BUFFER (2) ...
	if(sCH__SCH_DB_ST2_USE_FLAG->Check__DATA(STR__ENABLE) > 0)
	{
		int cfg_max = (int) xEXT_CH__BUFFER2__CFG_SLOT_MAX->Get__VALUE();
		int i;

		if(cfg_max > CFG_LP__SLOT_MAX)		
		{
			cfg_max = CFG_LP__SLOT_MAX;
		}

		for(i=0; i<cfg_max; i++)
		{
			if(xEXT_CH__BUFFER2__SLOT_STATUS[i]->Check__DATA(STR__NONE) > 0)
			{
				str_data.Format("%1d", i+1);

				l_info.RemoveAll();
				l_info.Add(MODULE__BUFFER2);
				l_info.Add(str_data);

				return 1;
			}
		}
	}

	return -1;
}
int  CObj__DUAL_ARM_STD
::_QUERY__CHECK_BUSY_SLOT_OF_BUUFER(const int log_enable, const CStringArray& l_para)
{
	CString para_module;
	CString para_slot;

	int limit = l_para.GetSize();
	int i;

	for(i=0; i<limit; i++)
	{
		CString para_data = l_para[i];

		switch(i)
		{
		case 0:		para_module = para_data;		break;
		case 1:		para_slot   = para_data;		break;
		}
	}

	if(log_enable > 0)
	{
		CString log_msg;
		CString log_bff;

		log_msg = "Parameter Info ... \n";
		log_bff.Format("  * para_module <- %s \n", para_module);
		log_msg += log_bff;
		log_bff.Format("  * para_slot   <- %s \n", para_slot);
		log_msg += log_bff;

		xAPP_LOG_CTRL->WRITE__LOG(log_msg);
	}

	// ...
	CString str_data;

	// OBJ : SIDE BUFFER (1) ...
	if(para_module.CompareNoCase(MODULE__BUFFER1) == 0)
	{
		if(sCH__SCH_DB_ST1_USE_FLAG->Check__DATA(STR__ENABLE) > 0)
		{
			int slot_index = atoi(para_slot) - 1;
			int cfg_max = (int) xEXT_CH__BUFFER1__CFG_SLOT_MAX->Get__VALUE();

			if((slot_index >= 0) && (slot_index < cfg_max))
			{
				double cur_sec = xEXT_CH__BUFFER1__SLOT_WAIT_SEC[slot_index]->Get__VALUE();

				if(cur_sec > 0.1)
				{
					return 1;
				}
			}
		}
		return -1;
	}

	// OBJ : SIDE BUFFER (2) ...
	if(para_module.CompareNoCase(MODULE__BUFFER2) == 0)
	{
		if(sCH__SCH_DB_ST2_USE_FLAG->Check__DATA(STR__ENABLE) > 0)
		{
			int slot_index = atoi(para_slot) - 1;
			int cfg_max = (int) xEXT_CH__BUFFER2__CFG_SLOT_MAX->Get__VALUE();

			if((slot_index >= 0) && (slot_index < cfg_max))
			{
				double cur_sec = xEXT_CH__BUFFER2__SLOT_WAIT_SEC[slot_index]->Get__VALUE();

				if(cur_sec > 0.1)
				{
					return 1;
				}
			}
		}
		return -1;
	}

	return 0;
}
