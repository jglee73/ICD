#include "StdAfx.h"
#include "CObj_Phy__PMC_STD.h"
#include "CObj_Phy__PMC_STD__ALID.h"


// ...
void CObj_Phy__PMC_STD
::Mon__All_State(CII_OBJECT__VARIABLE *p_variable,
				 CII_OBJECT__ALARM *p_alarm)
{
	// Process Time ...
	double	total_time = 0;

	xCH__CHANGE_PRC_TIME->Set__DATA("0");

	// ...
	SCX__ASYNC_TIMER_CTRL x_timer__pmc_idle;
	int idle_flag = -1;

	x_timer__pmc_idle->REGISTER__COUNT_CHANNEL_NAME(xCH__PMC_IDLE_TIME->Get__CHANNEL_NAME());

	// ...	
	CString pre__proc_type;
	xEXT_CH_CFG__PMC_PROCESS_TYPE->Get__DATA(pre__proc_type);

	// ...
	int r_count = 0;
	CString obj_ctrl;

	// ...
	SCX__ALARM_CTRL cx_alarm_ctrl;

	int online_flag;
	int pmc_alarm_flag;

	xEXT_CH__PMC_CONNECT_STATUS->Set__DATA("OFFLINE");

	// ...
	double ref_atm_press;
	double ref_vac_press;
	double cur_press;
	double pre_press = -1;

	CString str_cur_press;

	CString pre_pmc_use;
	xEXT_CH_CFG__PMC_USE->Get__DATA(pre_pmc_use);

	// ...
	CString pre__pmc_req_count;
	CString var_data;

	// ...
	int loop_count = 0;


	while(1)
	{
		p_variable->Wait__SINGLE_OBJECT(0.1);

		loop_count++;
		if(loop_count > 9)		loop_count = 0;


		// Process Time ...
		if(loop_count == 1)
		{
			Fnc__Process_Time(total_time);
		}

		if((loop_count % 5)	== 0)
		{
			// DA Offset ...
			{
				CString r_offset;
				CString t_offset;

				xCH__FDC_DA_R_OFFSET->Get__DATA(r_offset);
				xCH__FDC_DA_T_OFFSET->Get__DATA(t_offset);

				var_data.Format("%s___%s", r_offset,t_offset);
				xCH__FDC_DA_RT_OFFSET->Set__DATA(var_data);
			}

			// Object State ...
			{
				xEXT_CH_CFG__PMC_PROCESS_TYPE->Get__DATA(var_data);

				if(pre__proc_type.CompareNoCase(var_data) != 0)
				{
					pre__proc_type = var_data;

					if(pre__proc_type.CompareNoCase(STR__NORMAL) == 0)
					{
						int alarm_id = ALID__PMC_PROCESS_TYPE_TO_NORMAL;
						CString alarm_msg;
						CString r_act;

						alarm_msg.Format("PM%1d's process type is converted to \"Normal\".",iPMx_ID);

						p_alarm->Check__ALARM(alarm_id,r_act);
						p_alarm->Post__ALARM_With_MESSAGE(alarm_id,alarm_msg);
					}
					else if(pre__proc_type.CompareNoCase(STR__TEST) == 0)
					{
						int alarm_id = ALID__PMC_PROCESS_TYPE_TO_TEST;
						CString alarm_msg;
						CString r_act;

						alarm_msg.Format("PM%1d's process type is converted to \"Test\".",iPMx_ID);

						p_alarm->Check__ALARM(alarm_id,r_act);
						p_alarm->Post__ALARM_With_MESSAGE(alarm_id,alarm_msg);
					}
				}
			}

			if(iPRC_STS < 0)
			{
				xCH__OBJ_CTRL->Get__DATA(obj_ctrl);

				if((obj_ctrl.CompareNoCase("AVAILABLE") == 0)
				|| (obj_ctrl.CompareNoCase("ABORTED")   == 0))
				{
					if(xCH__OBJ_STATUS->Check__DATA("CTCINUSE") > 0)
					{
						r_count++;

						if(r_count > 3)
						{
							r_count = 0;

							xI_Sync_Ctrl.Thread__LOCK();

							if(iPRC_STS < 0)
							{
								xCH__OBJ_STATUS->Set__DATA("STANDBY");
							}

							xI_Sync_Ctrl.Thread__UNLOCK();
						}
					}
					else
					{
						r_count = 0;
					}
				}
				else
				{
					r_count = 0;
				}
			}

			// Idle Check ...
			{
				int material_count = 0;
				int i;

				for(i=0;i<iPMx_SLOT_MAX;i++)
				{
					if(xCH__SLOT_STATUS[i]->Check__DATA("NONE") > 0)
					{
						continue;
					}

					material_count++;
				}

				if(material_count > 0)
				{
					xCH__SVID_SLOT_DATA->Set__DATA("1");
				}
				else
				{
					xCH__SVID_SLOT_DATA->Set__DATA("0");
				}

				if((iPRC_STS < 0)
				&& (iFNC_STS < 0))
				{
					if(material_count < 1)
					{
						xCH__RECIPE->Set__DATA("");
						xCH__LOTID->Set__DATA("");
						xCH__MATERIAL_ID->Set__DATA("");
					}
				}

				if(iPRC_STS > 0)
				{
					if(idle_flag > 0)
					{
						idle_flag = -1;

						x_timer__pmc_idle->STOP();
						x_timer__pmc_idle->START__COUNT_DOWN(0.0);
					}
				}
				else
				{
					if(idle_flag < 0)
					{
						idle_flag = 1;

						x_timer__pmc_idle->START__COUNT_UP(99999);			
					}
				}
			}

			// Check Module_Status ...
			{
				if(iSIM_MODE > 0)
				{
					if(dEXT_CH__SCH_TEST_CFG_PMC_DUMMY_BY_CTC->Check__DATA(STR__YES) > 0)
						iSIM__PMC_DUMMY_MODE = 1;
					else
						iSIM__PMC_DUMMY_MODE = -1;
				}

				if(iSIM__PMC_DUMMY_MODE > 0)
				{
					online_flag = 1;

					xEXT_CH__PMC_CONNECT_STATUS->Set__DATA("ONLINE");
				}
				else
				{
					online_flag = xI__MODULE_INFO->Is__ONLINE(iSEQ_ID);

					if(online_flag > 0)
					{
						xEXT_CH__PMC_CONNECT_STATUS->Set__DATA("ONLINE");
					}
					else
					{
						xEXT_CH__PMC_CONNECT_STATUS->Set__DATA("OFFLINE");

						xCH__X_PICK_FLAG->Set__DATA("");
						xCH__X_PLACE_FLAG->Set__DATA("");
					}
				}

				xEXT_CH_CFG__PMC_USE->Get__DATA(var_data);
				xCH_CFG__PMC_USE->Set__DATA(var_data);

				if(var_data.CompareNoCase(STR__ENABLE) == 0)
				{
					CString r_act;
					p_alarm->Check__ALARM(ALID__PMC_OFFLINE,r_act);

					if(online_flag < 0)
					{
						p_alarm->Post__ALARM(ALID__PMC_OFFLINE);
					}

					if(r_act.CompareNoCase(ALM_ACT__RECONNECT) == 0)
					{
						xI__MODULE_INFO->Reconnect_EQP(iSEQ_ID);
					}
				}

				pmc_alarm_flag = -1;

				// ...
				{
					CUIntArray l_seqid;			
					cx_alarm_ctrl->Get__ALARM_MODULE_LIST(l_seqid);

					int limit = l_seqid.GetSize();
					int i;

					for(i=0;i<limit;i++)
					{
						if(iSEQ_ID != (int) l_seqid[i])		
							continue;

						pmc_alarm_flag = 1;
						break;
					}
				}

				if(pmc_alarm_flag > 0)
				{
					if(iPRC_STS > 0)
					{
						if(iPRC_ALARM_FLAG < 0)
						{
							iPRC_ALARM_FLAG = 1;

							if(xCH__ALARM_LOG_MODE->Check__DATA("ALL_ALARM") > 0)
								Post__PMx_ALARM(p_alarm);
						}
					}
				}
				else
				{
					if(iPRC_STS > 0)
					{
						if(iPRC_ALARM_FLAG > 0)
						{
							iPRC_ALARM_FLAG = 0;

							if(xCH__ALARM_LOG_MODE->Check__DATA("ALL_ALARM") > 0)
								Clear__PMx_ALARM(p_alarm);
						}
					}
				}
			}
		}

		// Pressure Check ...
		{
			if(xEXT_CH_CFG__PMC_USE->Check__DATA(pre_pmc_use) < 0)
			{
				xEXT_CH_CFG__PMC_USE->Get__DATA(pre_pmc_use);

				int alarm_id;
				CString alarm_msg;
				CString r_act;

				if(pre_pmc_use.CompareNoCase(STR__DISABLE) == 0)
				{
					alarm_id = ALID__PMC_ENABLE_TO_DISABLE;

					alarm_msg.Format("PM%1d's status is converted to \"DISABLE\" by User or Host.\n",
						iPMx_ID);
					alarm_msg += "Please, check config page or FDC interlock.\n";

					p_alarm->Check__ALARM(alarm_id,r_act);
					p_alarm->Post__ALARM_With_MESSAGE(alarm_id,alarm_msg);
				}
				else
				{
					alarm_id = ALID__PMC_DISABLE_TO_ENABLE;

					alarm_msg.Format("PM%1d's status is converted to \"ENABLE\" by User or Host.\n",
						iPMx_ID);
					alarm_msg += "Please, check config page or FDC interlock.\n";

					p_alarm->Check__ALARM(alarm_id,r_act);
					p_alarm->Post__ALARM_With_MESSAGE(alarm_id,alarm_msg);
				}
			}

			if(xEXT_CH_CFG__PMC_USE->Check__DATA("DISABLE") > 0)
			{
				xCH__OBJ_VIRTUAL_STATUS->Set__DATA("DISABLE");

				xCH__OBJ_VIRTUAL_STAGE1_STATUS->Set__DATA("DISABLE");
				xCH__OBJ_VIRTUAL_STAGE2_STATUS->Set__DATA("DISABLE");

				// ...
				{
					xCH__X_PICK_FLAG->Set__DATA("");
					xCH__X_PLACE_FLAG->Set__DATA("");
				}
			}
			else
			{
				xCH__OBJ_STATUS->Get__DATA(var_data);
				xCH__OBJ_VIRTUAL_STATUS->Set__DATA(var_data);

				if((var_data.CompareNoCase(STR__STANDBY)  != 0)
					&& (var_data.CompareNoCase(STR__CTCINUSE) != 0))	
				{
					xCH__X_PICK_FLAG->Set__DATA("");
					xCH__X_PLACE_FLAG->Set__DATA("");
				}

				if(xEXT_CH_CFG__PMC_STAGE1_USE->Check__DATA("DISABLE") > 0)
				{
					xCH__OBJ_VIRTUAL_STAGE1_STATUS->Set__DATA("DISABLE");
				}
				else
				{
					xCH__OBJ_VIRTUAL_STAGE1_STATUS->Set__DATA(var_data);
				}

				if(xEXT_CH_CFG__PMC_STAGE2_USE->Check__DATA("DISABLE") > 0)
				{
					xCH__OBJ_VIRTUAL_STAGE2_STATUS->Set__DATA("DISABLE");
				}
				else
				{
					xCH__OBJ_VIRTUAL_STAGE2_STATUS->Set__DATA(var_data);
				}
			}

			xEXT_CH_CFG__REF_ATM_PRESSURE->Get__DATA(var_data);
			ref_atm_press = atof(var_data);

			xEXT_CH_CFG__REF_VAC_PRESSURE->Get__DATA(var_data);
			ref_vac_press = atof(var_data);

			xCH__PRESSURE_VALUE->Get__DATA(str_cur_press);
			cur_press = atof(str_cur_press);

			if(cur_press != pre_press)
			{
				pre_press = cur_press;

				xCH__PRESSURE_VALUE->Set__DATA(str_cur_press);
			}

			if(cur_press <= ref_vac_press)
			{
				xCH__PRESSURE_STATUS->Set__DATA("VAC");
			}	
			else if(cur_press >= ref_atm_press)
			{
				xCH__PRESSURE_STATUS->Set__DATA("ATM");
			}
			else
			{
				xCH__PRESSURE_STATUS->Set__DATA("BTW");
			}
		}

		if((loop_count % 5)	== 0)
		{
			// Leak_Check ...
			Fnc__Leak_Check(p_variable, p_alarm);

			// Auto_PM ...
			Fnc__Auto_PM(p_variable, p_alarm);

			// GoTo_Maint ...
			if(xCH__MOVE_FLAG->Check__DATA("NO") > 0)
			{
				bool active__goto_maint = false;

				if((xSCH_MATERIAL_CTRL->Check__PMx_GoTo_MAINT(iPMx_ID) > 0))		active__goto_maint = true;
				if(sCH_REQ__GOTO_MAINT->Check__DATA(STR__YES) > 0)					active__goto_maint = true;
				if(dCH_LINK_REQ__GOTO_MAINT->Check__DATA(STR__YES) > 0)				active__goto_maint = true;

				if(active__goto_maint)
				{
					xSCH_MATERIAL_CTRL->Clear__PMx_GoTo_MAINT(iPMx_ID);
					sCH_REQ__GOTO_MAINT->Set__DATA("");
					dCH_LINK_REQ__GOTO_MAINT->Set__DATA("NO");

					if(Check__Standby_Or_CtcInUse() > 0)
					{
						if(iPRC_STS > 0)
						{
							xCH__NEXT_PROCESS->Set__DATA(STR__GOTO_MAINT);
						}
						else
						{	
							int material_count = Get__Material_Count();

							if(material_count < 1)		pOBJ_CTRL->Call__OBJECT(sMODE__SYSTEM_MAINT);
							else						xCH__NEXT_PROCESS->Set__DATA(STR__GOTO_MAINT);
						}
					}
				}
			}
	
			// Count_Ctrl ...
			if(sCH__WAFER_STATUS_CONVERTED_TO_PROCESSED_BY_PMC_REQ_COUNT->Check__DATA(pre__pmc_req_count) < 0)
			{
				sCH__WAFER_STATUS_CONVERTED_TO_PROCESSED_BY_PMC_REQ_COUNT->Get__DATA(pre__pmc_req_count);

				if(pre__pmc_req_count.GetLength() > 0)
				{
					for(int i=0; i<iPMx_SLOT_MAX; i++)
					{
						if(xCH__SLOT_STATUS[i]->Check__DATA(STR__NONE) < 0)
						{
							CString pmc_sch_name;
							pmc_sch_name.Format("PM%1d-%1d", iPMx_ID,i+1);

							if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(pmc_sch_name) > 0)
								xCH__SLOT_STATUS[i]->Set__DATA(STR__PROCESSED);
							else
								xCH__SLOT_STATUS[i]->Set__DATA(STR__COMPLETED);
						}
					}
				}
			}

			Fnc__LPx_Wafer_Count(p_variable,p_alarm);
			Fnc__Clean_Rcp_Count(p_variable,p_alarm);
		}

		if((loop_count % 5)	== 0)
		{
			CString obj_sts = xCH__OBJ_STATUS->Get__STRING();
			CString cfg_use = xCH_CFG__PMC_USE->Get__STRING();

			if(cfg_use.CompareNoCase(STR__ENABLE) == 0)
			{
				if(bActive__Init_Act)
				{
					sCH__ACTIVE_INIT_STATE->Set__DATA("INIT");
				}
				else
				{
						 if((obj_sts.CompareNoCase("OFFLINE")  == 0))		sCH__ACTIVE_INIT_STATE->Set__DATA("ALL");
					else if(obj_sts.CompareNoCase("MAINTMODE") == 0)		sCH__ACTIVE_INIT_STATE->Set__DATA("INIT");
					else if(obj_sts.CompareNoCase("STANDBY")   == 0)		sCH__ACTIVE_INIT_STATE->Set__DATA("MAINT");
					else													sCH__ACTIVE_INIT_STATE->Set__DATA("DISABLE");
				}
			}
			else
			{
				sCH__ACTIVE_INIT_STATE->Set__DATA("DISABLE");
			}
		}
	}
}

void CObj_Phy__PMC_STD
::Fnc__LPx_Wafer_Count(CII_OBJECT__VARIABLE *p_variable,
					   CII_OBJECT__ALARM *p_alarm)
{
	int lp_check_flag = -1;
	int i;

	for(i=0;i<CFG__LPx_SIZE;i++)
	{
		if(xEXT_CH__LPx_JOB_START_FLAG[i]->Check__DATA("YES") > 0)
		{
			lp_check_flag = 1;
			continue;
		}

		xCH_CFG__LPx_WAFER_COUNT[i]->Set__DATA("0");
	}

	if(lp_check_flag < 0)
	{
		xCH__LPx_WAFER_COUNT->Set__DATA("0");
	}
}
void CObj_Phy__PMC_STD
::Fnc__Clean_Rcp_Count(CII_OBJECT__VARIABLE *p_variable,
					   CII_OBJECT__ALARM *p_alarm)
{
	if(xCH__ECS_MODE->Check__DATA(ECS_MODE__LOAD_PROCESS) < 0)
	{
		return;
	}

	if((xEXT_CH_CFG__PMC_USE->Check__DATA(STR__ENABLE) < 0)
		|| (xCH_CFG__CLEAN_USE->Check__DATA(STR__ENABLE)   < 0))
	{
		return;
	}

	if(xCH__SLOT_STATUS[0]->Check__DATA("NONE") < 0)
	{
		if((xCH_CFG__CLEAN_MODE->Check__DATA(STR__CLEAN_MODE__FIRST_SECOND) > 0)
			|| (xCH_CFG__CLEAN_MODE->Check__DATA(STR__CLEAN_MODE__SYSTEM_IDLE)  > 0))
		{
			CString var_data;
			int f_ref_count;
			int s_ref_count;

			xCH_CFG__CLEAN_FIRST_COUNT->Get__DATA(var_data);
			f_ref_count = atoi(var_data);

			xCH_CFG__CLEAN_SECOND_COUNT->Get__DATA(var_data);
			s_ref_count = atoi(var_data);

			if(f_ref_count < 1)			return;
			if(s_ref_count < 1)			return;

			// ...
			int cur_count;

			if(xCH_CFG__CLEAN_MODE->Check__DATA(STR__CLEAN_MODE__SYSTEM_IDLE) > 0)
			{
				xCH__SYSTEM_TOTAL_MATERIAL_COUNT->Get__DATA(var_data);
			}
			else
			{
				xCH_CFG__CLEAN_COUNT->Get__DATA(var_data);
			}
			cur_count = atoi(var_data);

			if(cur_count < f_ref_count)
			{
				return;
			}

			if(cur_count >= s_ref_count)
			{
				if(xCH__NEXT_PROCESS_REQ->Check__DATA("") > 0)
				{
					xCH__NEXT_PROCESS_REQ->Set__DATA("YES");
				}
			}
		}
		return;
	}
	else
	{
		xCH__NEXT_PROCESS_REQ->Set__DATA("");
	}

	if((xCH_CFG__CLEAN_MODE->Check__DATA(STR__CLEAN_MODE__FIRST_SECOND) > 0)
	|| (xCH_CFG__CLEAN_MODE->Check__DATA(STR__CLEAN_MODE__SYSTEM_IDLE)  > 0))
	{
		CString var_data;
		int f_ref_count;
		int s_ref_count;

		xCH_CFG__CLEAN_FIRST_COUNT->Get__DATA(var_data);
		f_ref_count = atoi(var_data);

		xCH_CFG__CLEAN_SECOND_COUNT->Get__DATA(var_data);
		s_ref_count = atoi(var_data);

		if(f_ref_count < 1)			return;
		if(s_ref_count < 1)			return;

		// ...
		int cur_count;

		if(xCH_CFG__CLEAN_MODE->Check__DATA(STR__CLEAN_MODE__SYSTEM_IDLE) > 0)
		{
			xCH__SYSTEM_TOTAL_MATERIAL_COUNT->Get__DATA(var_data);
		}
		else
		{
			xCH_CFG__CLEAN_COUNT->Get__DATA(var_data);
		}
		cur_count = atoi(var_data);

		if(cur_count < f_ref_count)
		{
			return;
		}

		if(cur_count < s_ref_count)
		{
			double ref_idle_time;

			xCH_CFG__CLEAN_SYS_IDLE_REF_DELAY_TIME->Get__DATA(var_data);
			ref_idle_time = atof(var_data);

			if(xCH_CFG__CLEAN_MODE->Check__DATA(STR__CLEAN_MODE__SYSTEM_IDLE) < 0)
			{
				if(ref_idle_time > xTIMER__SYS_IDLE->Get__CURRENT_TIME())
				{
					return;
				}
			}
			else
			{
				double cur_idle_time;

				xCH__SYS_IDLE_TIME->Get__DATA(var_data);
				cur_idle_time = atof(var_data);

				if(ref_idle_time > cur_idle_time)
				{
					return;
				}
			}
		}

		if(xCH__NEXT_PROCESS->Check__DATA("") > 0)
		{
			xCH__NEXT_PROCESS->Set__DATA(STR__CLEAN_COUNT);
		}

		if(xCH_CFG__CLEAN_MODE->Check__DATA(STR__CLEAN_MODE__SYSTEM_IDLE) < 0)
		{
			xTIMER__SYS_IDLE->STOP();
		}

		if((xCH__OBJ_STATUS->Check__DATA("STANDBY") > 0)
			&& (xCH__MOVE_FLAG->Check__DATA("NO") > 0))
		{
			xEXT_CH__MODULE_OBJ_MODE->Set__DATA(sMODE__NEXT_PROC);
			xEXT_CH__MODULE_OBJ_CTRL->Set__DATA("RUN");

			xEXT_CH__MODULE_OBJ_CTRL->When__DATA("AVAILABLE","ABORTED");
		}
	}
}

int  CObj_Phy__PMC_STD
::Check__Standby_Or_CtcInUse()
{
	CString obj_sts;
	xCH__OBJ_STATUS->Get__DATA(obj_sts);

	if(obj_sts.CompareNoCase("STANDBY")  == 0)		return 1;
	if(obj_sts.CompareNoCase("CTCINUSE") == 0)		return 1;

	return -1;
}

void CObj_Phy__PMC_STD
::Check__PMx_Slot_Max()
{
	CString var_data;
	xEXT_CH_CFG__PMC_SLOT_MAX->Get__DATA(var_data);

	int i_max = atoi(var_data);

	if(i_max > CFG__PMx_MAX_SLOT)		i_max = CFG__PMx_MAX_SLOT;
	else if(i_max < 1)						i_max = 1;

	iPMx_SLOT_MAX = i_max;
}
int CObj_Phy__PMC_STD
::Get__PMx_Occupied_Slot()
{
	int i;

	for(i=0; i<iPMx_SLOT_MAX; i++)
	{
		if(xCH__SLOT_STATUS[i]->Check__DATA("NONE") > 0)
		{
			continue;
		}

		return (i+1);
	}
	return 0;
}

int CObj_Phy__PMC_STD
::Get__Material_Count()
{
	int i_count = 0;

	for(int i=0; i<iPMx_SLOT_MAX; i++)
	{
		if(xCH__SLOT_STATUS[i]->Check__DATA("NONE") > 0)
			continue;

		i_count++;
	}
	return i_count;
}
