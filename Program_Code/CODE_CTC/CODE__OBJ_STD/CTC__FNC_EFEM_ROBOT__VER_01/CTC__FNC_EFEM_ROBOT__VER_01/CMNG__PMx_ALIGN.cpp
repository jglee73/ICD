#include "stdafx.h"
#include "CMNG__PMx_ALIGN.h"

#include "Macro_Function.h"


// ...
CMNG__PMx_ALIGN::CMNG__PMx_ALIGN()
{

}
CMNG__PMx_ALIGN::~CMNG__PMx_ALIGN()
{

}

// ...
void CMNG__PMx_ALIGN::Reset__ALL_INFO()
{
	for(int i=0; i<CFG_PM_LIMIT; i++)
	{
		iINFO__ALIGN_COUNT[i] = 0;
	}

	// ...
	{
		sList__WAFER_TITLE.RemoveAll();
		
		sList__TRG_NAME.RemoveAll();
		sList__TRG_SLOT.RemoveAll();
		sList__AL_ANGLE.RemoveAll();

		iList__ALIGN_STATE.RemoveAll();
	}
}

CString CMNG__PMx_ALIGN::Get__NEXT_ALIGN(const CString& para_module, const CString& para_slot)
{
	CString sch_name;

	if(para_slot.GetLength() < 1)		sch_name = para_module;
	else								sch_name.Format("%s+%s", para_module,para_slot);

	// ...
	CUIntArray l__pmc_id;

	// ...
	{
		CStringArray l__pmc_name;
		CStringArray l__rcp_name;

		pSCH_MATERIAL_CTRL->Get__CUR_PROCESS_INFO(sch_name, l__pmc_name,l__rcp_name);

		Macro__Convert_PMC_NAME_TO_PMC_ID(l__pmc_name, l__pmc_id);
	}
	
	// ...
	{
		CUIntArray l__align_count;

		int i_limit = l__pmc_id.GetSize();
		for(int i=0; i<i_limit; i++)
		{
			int pmc_id = l__pmc_id[i];
			int pmc_index = pmc_id - 1;

			int cur_count = iINFO__ALIGN_COUNT[pmc_index];
			
			l__align_count.Add(cur_count);
		}

		/*
		// ...
		{
			CString log_msg;
			CString log_bff;

			log_msg = "Get__NEXT_ALIGN() - Start \n";

			int k_limit = l__pmc_id.GetSize();
			for(int k=0; k<k_limit; k++)
			{
				log_bff.Format(" * %1d) [%1d] [%1d] \n", k, l__pmc_id[k], l__align_count[k]);
				log_msg += log_bff;
			}

			printf(log_msg);
		}
		*/

		Macro__Get_Ascending_Order(l__align_count, l__pmc_id);

		/*
		// ...
		{
			CString log_msg;
			CString log_bff;

			log_msg = "Get__NEXT_ALIGN() - End \n";

			int k_limit = l__pmc_id.GetSize();
			for(int k=0; k<k_limit; k++)
			{
				log_bff.Format(" * %1d) [%1d] [%1d] \n", k, l__pmc_id[k], l__align_count[k]);
				log_msg += log_bff;
			}

			printf(log_msg);
		}
		*/
	}

	// ...
	int  active__wfr_normal = 1;

	// ...
	{
		IDS__SCH_MATERIAL_INFO ds_info;
		pSCH_MATERIAL_CTRL->Get__MATERIAL_INFO(sch_name, ds_info);

		int wfr__port_id = ds_info.iSRC__PTN;
		int wfr__slot_id = ds_info.iSRC__STN;

		// CHECK : LPx ...
		if((wfr__port_id >  0) 
		&& (wfr__port_id <= iLP_SIZE))
		{
			int port_index = wfr__port_id - 1;

			if(pCH__LPx_CFG_PROCESS_TYPE[port_index]->Check__DATA(STR__TEST) > 0)
			{
				active__wfr_normal = -1;
			}
		}

		// ...
		CString str__wfr_info;
		str__wfr_info.Format("P%1d:%02d", wfr__port_id, wfr__slot_id);

		int db_index = _Get__DB_INDEX(str__wfr_info);

		/*
		printf("sch_name <- %s \n", sch_name);
		printf(" * wfr_info <- %s \n", str__wfr_info);
		printf(" * db_index <- %1d \n", db_index);
		*/

		if(db_index >= 0)
		{
			CString str__trg_name = sList__TRG_NAME[db_index];

			int pmc_index = Macro__Get_PMC_INDEX(str__trg_name);
			if(pmc_index >= 0)
			{
				int trg__pmc_id = pmc_index + 1;
				l__pmc_id.InsertAt(0, trg__pmc_id);
			}

			/*
			printf("  ** trg_align <- %s \n", str__trg_name);
			printf("  ** pmc_index <- %1d \n", pmc_index);
			*/
		}

		/*
		// ...
		{
			CString log_msg;
			CString log_bff;

			log_msg = "Get__NEXT_ALIGN() - Last \n";

			int k_limit = l__pmc_id.GetSize();
			for(int k=0; k<k_limit; k++)
			{
				log_bff.Format(" * %1d) [%1d] \n", k, l__pmc_id[k]);
				log_msg += log_bff;
			}

			printf(log_msg);
		}
		*/
	}

	// ...
	CString str__pm_name;

	// ...
	{
		int count__wfr_type = -1;

		int k_limit = l__pmc_id.GetSize();
		for(int k=0; k<k_limit; k++)
		{
			int pm_id = l__pmc_id[k];
			int pm_index = pm_id - 1;

			if(pm_index <  0)				continue;
			if(pm_index >= iPMx_SIZE)		continue;

			if(active__wfr_normal < 0)
			{
				if(pCH__PMx_PROCESS_TYPE[pm_index]->Check__DATA(STR__TEST) < 0)
				{
					continue;
				}
			}

			count__wfr_type = 1;
		}

		if(count__wfr_type < 0)
		{
			active__wfr_normal = 1;
		}
	}

	// ...
	{
		int k_limit = l__pmc_id.GetSize();

		for(int k=0; k<k_limit; k++)
		{
			int pm_id = l__pmc_id[k];
			int pm_index = pm_id - 1;

			if(pm_index <  0)				continue;
			if(pm_index >= iPMx_SIZE)		continue;

			// ...
			if(pCH__CFG_PMx_USE[pm_index]->Check__DATA(STR__ENABLE) < 0)
			{
				continue;
			}

			if(active__wfr_normal > 0)
			{
				if(pCH__PMx_PROCESS_TYPE[pm_index]->Check__DATA(STR__NORMAL) < 0)
				{
					continue;
				}
			}
			else
			{
				if(pCH__PMx_PROCESS_TYPE[pm_index]->Check__DATA(STR__TEST) < 0)
				{
					continue;
				}
			}
			
			// ...
			CString obj_sts = pCH__PMx_OBJ_VIRTUAL_STATUS[pm_index]->Get__STRING();

			if(obj_sts.CompareNoCase(STR__STANDBY) == 0)
			{

			}
			else if(obj_sts.CompareNoCase(STR__CTCINUSE) == 0)
			{
				if(pCH__PMx_PROC_INFO_PROCESS_STATUS[pm_index]->Check__DATA(STR__ALARM) > 0)
				{
					continue;
				}
			}
			else
			{
				continue;
			}

			str__pm_name.Format("PM%1d", pm_id);
			break;
		}

	}

	return str__pm_name;
}

int  CMNG__PMx_ALIGN::_Get__DB_INDEX(const CString& str__wfr_title)
{
	int k_limit = sList__WAFER_TITLE.GetSize();

	for(int k=0; k<k_limit; k++)
	{
		if(str__wfr_title.CompareNoCase(sList__WAFER_TITLE[k]) != 0)
		{
			continue;
		}

		return k;
	}
	return -1;
}
int  CMNG__PMx_ALIGN::_Get__START_INDEX()
{
	int k_limit = iList__ALIGN_STATE.GetSize();

	for(int k=0; k<k_limit; k++)
	{
		if(iList__ALIGN_STATE[k] != _ALIGN_STS__START)		
		{
			continue;
		}

		return k;
	}
	return -1;
}
int  CMNG__PMx_ALIGN::_Get__MAX_ALIGN_COUNT()
{
	int max_count = 0;

	for(int i=0; i<CFG_PM_LIMIT; i++)
	{
		int cur_count = iINFO__ALIGN_COUNT[i];

		if(cur_count > max_count)		max_count = cur_count;
	}
	return max_count;
}

int  CMNG__PMx_ALIGN::_Fnc__PICK_AL(const CString& str__wfr_title, 
									const CString& str__trg_name,
									const CString& str__trg_slot,
									const int act_state)
{
	if(act_state == _ALIGN_STS__START)
	{
		int  db_index = _Get__DB_INDEX(str__wfr_title);
		if(db_index >= 0)
		{
			sList__TRG_NAME[db_index] = str__trg_name;
			sList__TRG_SLOT[db_index] = str__trg_slot;
			sList__AL_ANGLE[db_index] = "?";
			
			iList__ALIGN_STATE[db_index] = act_state;
		}
		else
		{
			sList__WAFER_TITLE.Add(str__wfr_title);
			
			sList__TRG_NAME.Add(str__trg_name);
			sList__TRG_SLOT.Add(str__trg_slot);
			sList__AL_ANGLE.Add("");
			
			iList__ALIGN_STATE.Add(act_state);

			if(sList__WAFER_TITLE.GetSize() > 10)
			{
				sList__WAFER_TITLE.RemoveAt(0);

				sList__TRG_NAME.RemoveAt(0);
				sList__TRG_SLOT.RemoveAt(0);
				sList__AL_ANGLE.RemoveAt(0);

				iList__ALIGN_STATE.RemoveAt(0);
			}
		}
	}

	return 1;
}

int  CMNG__PMx_ALIGN::
_Report__PICK_AL(const int act_state,
				 const CString& str__al_angle)
{
	if((act_state == _ALIGN_STS__END)
	|| (act_state == _ALIGN_STS__ABORT))
	{
		int  db_index = _Get__START_INDEX();
		if(db_index >= 0)
		{
			if(act_state == _ALIGN_STS__END)
			{
				sList__AL_ANGLE[db_index] = str__al_angle;

				CString str__trg_name = sList__TRG_NAME[db_index];
				CString str__trg_slot = sList__TRG_SLOT[db_index];

				int pmc_index = Macro__Get_PMC_INDEX(str__trg_name);
				if((pmc_index >= 0) && (pmc_index < iPMx_SIZE))
				{
					int max_count = _Get__MAX_ALIGN_COUNT() + 1;

					iINFO__ALIGN_COUNT[pmc_index] = max_count;

					// ...
					CString str__wfr_info = sList__WAFER_TITLE[db_index];
					
					int wfr__port_id = -1;
					int wfr__slot_id = -1;

					Macro__Analyze_Title_Info(str__wfr_info, wfr__port_id,wfr__slot_id);

					if((wfr__port_id > 0)
					&& (wfr__slot_id > 0))
					{
						CString def_name;
						CString def_data;

						// 1.
						{
							def_name = USER_DEF__TRG_MDx;
							def_data = str__trg_name;
							
							pSCH_MATERIAL_CTRL->Set__MATERIAL_USER_INFO(wfr__port_id,wfr__slot_id, def_name,def_data);
						}
						// 2.
						{
							def_name = USER_DEF__TRG_SLOT;
							def_data = str__trg_slot;

							pSCH_MATERIAL_CTRL->Set__MATERIAL_USER_INFO(wfr__port_id,wfr__slot_id, def_name,def_data);
						}
						// 3.
						{
							def_name = USER_DEF__TRG_ANGLE;
							def_data = str__al_angle;

							pSCH_MATERIAL_CTRL->Set__MATERIAL_USER_INFO(wfr__port_id,wfr__slot_id, def_name,def_data);
						}
					}
				}

				/*
				// ...
				{
					CString log_msg;
					CString log_bff;

					log_msg.Format("_Fnc__PICK_AL() - End ! \n");

					log_bff.Format(" * Target_PMC <- %s \n", str__trg_pmc);
					log_msg += log_bff;

					log_bff.Format(" * Align_Count <- %1d \n", iINFO__ALIGN_COUNT[pmc_index]);
					log_msg += log_bff;
					log_msg += "\n";

					printf(log_msg);
				}
				*/
			}

			iList__ALIGN_STATE[db_index] = act_state;
		}
	}

	return 1;
}

int  CMNG__PMx_ALIGN::Start__PICK_AL()
{
	CString str__wfr_title = pCH__WAFER_TITLE_OF_AL->Get__STRING();

	CString str__trg_name = pCH__TRG_NAME_OF_AL->Get__STRING();
	CString str__trg_slot = pCH__TRG_SLOT_OF_AL->Get__STRING();

	/*
	// ...
	{
		CString log_msg;
		CString log_bff;

		log_msg = "Start__PICK_AL() ... \n";

		log_bff.Format(" * wfr_title <- %s \n", str__wfr_title);
		log_msg += log_bff;

		log_bff.Format(" * trg_align <- %s \n", str__trg_align);
		log_msg += log_bff;

		printf(log_msg);
	}
	*/

	return _Fnc__PICK_AL(str__wfr_title, str__trg_name,str__trg_slot, _ALIGN_STS__START);
}
int  CMNG__PMx_ALIGN::End__PICK_AL(const CString& str__al_anble)
{
	// printf("End__PICK_AL() ... \n");

	return _Report__PICK_AL(_ALIGN_STS__END, str__al_anble);
}
int  CMNG__PMx_ALIGN::Abort__PICK_AL()
{
	// printf("Abort__PICK_AL() ... \n");

	return _Report__PICK_AL(_ALIGN_STS__ABORT, "");
}

CString CMNG__PMx_ALIGN::Get__CURRENT_ALIGN_INFO(const CString& arm_type)
{
	CString str__wfr_title;

		 if(arm_type.CompareNoCase(ARM_A) == 0)			str__wfr_title = pCH__WAFER_TITLE_OF_ARM_A->Get__STRING();
	else if(arm_type.CompareNoCase(ARM_B) == 0)			str__wfr_title = pCH__WAFER_TITLE_OF_ARM_B->Get__STRING();

	// ...
	int k_limit = sList__WAFER_TITLE.GetSize();

	for(int k=0; k<k_limit; k++)
	{
		if(str__wfr_title.CompareNoCase(sList__WAFER_TITLE[k]) != 0)
		{
			continue;
		}

		return sList__TRG_NAME[k];
	}

	return Get__NEXT_ALIGN(arm_type);
}

int  CMNG__PMx_ALIGN::Check__ALIGN_INFO(const CString& arm_type)
{
	CString str__wfr_title;
	CString str__trg_name;

	if(arm_type.CompareNoCase(ARM_A) == 0)
	{
		str__wfr_title = pCH__WAFER_TITLE_OF_ARM_A->Get__STRING();
		str__trg_name  = pCH__TRG_NAME_OF_ARM_A->Get__STRING();
	}
	else if(arm_type.CompareNoCase(ARM_B) == 0)
	{
		str__wfr_title = pCH__WAFER_TITLE_OF_ARM_B->Get__STRING();
		str__trg_name  = pCH__TRG_NAME_OF_ARM_B->Get__STRING();
	}

	// ...
	int k_limit = sList__WAFER_TITLE.GetSize();

	for(int k=0; k<k_limit; k++)
	{
		if(str__wfr_title.CompareNoCase(sList__WAFER_TITLE[k]) != 0)
		{
			continue;
		}

		if(str__trg_name.CompareNoCase(sList__TRG_NAME[k]) != 0)
		{
			return -1;
		}
		return 1;
	}

	return -11;
}
