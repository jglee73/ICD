#include "StdAfx.h"
#include "CObj__DUAL_ARM_STD.h"

#include "Macro_Function.h"


// ...
void CObj__DUAL_ARM_STD
::Mon__LBx_CFG(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm)
{
	int ll_count[CFG_LLx_LIMIT];

	CString err_msg;
	CString var_data;

	int slot_max;
	int lba_count;
	int lbb_count;
	int i;

	while(1)
	{
		p_variable->Wait__SINGLE_OBJECT(0.3);

		// LPx ...
		for(i=0; i<iLPx_SIZE; i++)
		{
			if((xEXT_CH__LPx_MAP_REQ[i]->Check__DATA(STR__ERROR) > 0)
				&& (xCH__LPx__PORT_STATUS[i]->Check__DATA("WAITING") > 0))
			{
				xCH__LPx_MAP_LOCK[i]->Set__DATA("");
			}
			else
			{
				xCH__LPx_MAP_LOCK[i]->Set__DATA("LOCK");
			}
		}

		// CFG : LLx ...
		{
			int ll_limit = iLLx_SIZE;
			int ll_i;

			for(ll_i=0; ll_i<ll_limit; ll_i++)
			{
				xEXT_CH__SCH_DB_LLx_USE_FLAG[ll_i]->Get__DATA(var_data);
				xEXT_CH__LLx__CFG_USE_FLAG[ll_i]->Set__DATA(var_data);

				xEXT_CH__SCH_DB_LLx_SLOT_MAX[ll_i]->Get__DATA(var_data);
				slot_max = atoi(var_data);
				if(slot_max > CFG_LLx__SLOT_MAXx)			slot_max = CFG_LLx__SLOT_MAXx;

				iLLx_SLOT_MAX[ll_i] = slot_max;
				ll_count[ll_i] = 0;

				for(i=0; i<slot_max; i++)
				{
					if((dEXT_CH__SCH_DB_LLx_SLOT_STATUS[ll_i][i]->Check__DATA(SLOT_STS__ENABLE) < 0)	
					|| (dEXT_CH__CFG_DB_LLx_SLOT_STATUS[ll_i][i]->Check__DATA(SLOT_STS__ENABLE) < 0))
					{
						continue;
					}

					ll_count[ll_i]++;
				}

				var_data.Format("%1d", ll_count[ll_i]);
				xCH__LLx_AVAILABLE_SLOT_COUNT[ll_i]->Set__DATA(var_data);
			}

			// Error Check ...
			{
				err_msg = "";

				CStringArray l__ll_name;
				CStringArray l__ll_mode_type;

				int active__all_mode    = -1;
				int active__only_input  = -1;
				int active__only_output = -1;

				int active__slot_use   = -1;
				int active__slot_equal =  1;
				int pre__slot_size = -1;

				for(ll_i=0; ll_i<ll_limit; ll_i++)
				{
					if(xEXT_CH__SCH_DB_LLx_USE_FLAG[ll_i]->Check__DATA(STR__ENABLE) < 0)
					{
						continue;
					}

					// LLx Name ...
					CString ll_name = Get__LLx_NAME(ll_i);
					l__ll_name.Add(ll_name);

					// Mode_Type ...
					CString str__mode_type = xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_i]->Get__STRING();
					l__ll_mode_type.Add(str__mode_type);

					if(str__mode_type.CompareNoCase(LBx_MODE__ONLY_INPUT) == 0)
					{
						active__only_input = 1;
					}
					else if(str__mode_type.CompareNoCase(LBx_MODE__ONLY_OUTPUT) == 0)
					{
						active__only_output = 1;
					}
					else if(str__mode_type.CompareNoCase(LBx_MODE__ALL) == 0)
					{
						active__all_mode = 1;
					}

					// Slot Check ...
					{
						int cur__slot_size = ll_count[ll_i];

						if(pre__slot_size < 0)
						{
							pre__slot_size = cur__slot_size;
						}
						else
						{
							if(pre__slot_size != cur__slot_size)
								active__slot_equal = -1;
						}

						if(cur__slot_size > 0)
						{
							active__slot_use = 1;
						}
					}
				}

				if(active__slot_use < 0)
				{
					err_msg = "There is no available slot in LBx.";
				}
				else if(active__all_mode > 0)
				{
					if((active__only_input  > 0)
					|| (active__only_output > 0))
					{
						CString err_bff;

						err_msg  = "Please, check LBA's and LBB's mode.\n";
						err_msg += "(Both All) or (Only_Input / Only_Output)";
						err_msg += "\n\n";

						int k_limit = l__ll_name.GetSize();
						int k;

						for(k=0; k<k_limit; k++)
						{
							CString ll_name   = l__ll_name.GetAt(k);
							CString mode_type = l__ll_mode_type.GetAt(k);

							err_bff.Format("   %s-MODE : [%s]\n", ll_name,mode_type);
							err_msg += err_bff;
						}
					}
					else if(active__slot_use < 0)
					{
						err_msg  = "All LBA's and LBB's Use Mode must be enabled.\n";
						err_msg += "Or, at least, one among LBA and LBB must be ALL Type.\n";
					}
					else if(active__slot_equal < 0)
					{
						err_msg = "In LBx All mode, LLx's slots must be equal.";
					}
				}
				else if((active__only_input  > 0)
					 || (active__only_output > 0))
				{
					if((active__only_input  < 0)
					|| (active__only_output < 0))
					{
						CString err_bff;

						err_msg  = "Please, check LLx's mode.\n";
						err_msg += "(Any All) or (Only_Input / Only_Output)";
						err_msg += "\n\n";

						int k_limit = l__ll_name.GetSize();
						int k;

						for(k=0; k<k_limit; k++)
						{
							CString ll_name   = l__ll_name.GetAt(k);
							CString mode_type = l__ll_mode_type.GetAt(k);

							err_bff.Format("   %s-MODE : [%s]\n", ll_name,mode_type);
							err_msg += err_bff;
						}
					}
				}
				else
				{
					err_msg = "There is no LBx with Use Enable Mode.";
				}

				if(err_msg.GetLength() > 0)
				{
					xSCH_MATERIAL_CTRL->Set__LBx_SCH_MODE__STATUS_ERROR(err_msg);

					// ...
					{
						int alarm_id = ALID__LBx_CONFIG_ERROR;
						CString r_act;

						p_alarm->Check__ALARM(alarm_id,r_act);
						p_alarm->Post__ALARM_With_MESSAGE(alarm_id,err_msg);
					}
				}
				else
				{
					xSCH_MATERIAL_CTRL->Clear__LBx_SCH_MODE__STATUS_ERROR();
				}
			}
		}

		// System Check ...
		{
			if(xCH__ATM_RB_PAUSE_FLAG->Check__DATA(STR__YES) > 0)
			{
				int alarm_id = ALID__ATM_ROBOT__MOVE_PAUSED;
				CString r_act;

				p_alarm->Check__ALARM(alarm_id,r_act);
				p_alarm->Post__ALARM(alarm_id);
			}

			SCH__SYSTEM_CHECK(p_variable,p_alarm,iJOB_FLAG);
		}
	}
}

void CObj__DUAL_ARM_STD
::Mon__DB_CHECK(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm)
{

	while(1)
	{
		p_variable->Wait__SINGLE_OBJECT(0.5);

		if(xCH__DB_CHECK_REQ_FLAG->Check__DATA(STR__YES) > 0)
		{
			xCH__DB_CHECK_REQ_FLAG->Set__DATA("");

			xSCH_MATERIAL_CTRL->Check__RCP_DB();
			xSCH_MATERIAL_CTRL->Check__STS_DB();

			// ...
			{
				CUIntArray l_ptn;
				CString msg;
				CString bff;

				xSCH_MATERIAL_CTRL->Get__JOB_PORT_LIST(l_ptn);

				int limit;
				int i;

				// 
				limit = l_ptn.GetSize();
				msg += "Job Port List ... \n";

				for(i=0;i<limit;i++)
				{
					int port_id = (int) l_ptn[i];

					bff.Format("   %1d) LP%1d \n", i+1,port_id);
					msg += bff;
				}

				l_ptn.RemoveAll();
				l_ptn.Add(1);
				l_ptn.Add(2);
				l_ptn.Add(3);

				limit = l_ptn.GetSize();
				msg += "LPx Wafer Flag ... \n";

				for(i=0;i<limit;i++)
				{
					int port_id = (int) l_ptn[i];
					int wafer_flag = xSCH_MATERIAL_CTRL->Check__MATERIAL_TO_PROCESS_WITH_PMC_IN_LPx(port_id);

					bff.Format("%1d) LP%1d-Wafer_Flag(%1d) \n", i+1,port_id,wafer_flag);
					msg += bff;
				}

				MessageBox(NULL,msg,"CHECK - Mon__DB_CHECK()",MB_OK|MB_TOPMOST);
			}
		}

		// ...
	}
}
