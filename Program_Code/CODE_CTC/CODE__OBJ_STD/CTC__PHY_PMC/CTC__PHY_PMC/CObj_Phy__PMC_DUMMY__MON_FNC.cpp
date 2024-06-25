#include "StdAfx.h"
#include "CObj_Phy__PMC_DUMMY.h"
#include "CObj_Phy__PMC_DUMMY__ALID.h"


// ...
void CObj_Phy__PMC_DUMMY
::Mon__Obj_Status(CII_OBJECT__VARIABLE *p_variable,
				  CII_OBJECT__ALARM *p_alarm)
{
	int r_count = 0;

	while(1)
	{
		Sleep(490);

		if(iPRC_STS < 0)
		{
			if(xCH__OBJ_STATUS->Check__DATA("CTCINUSE") > 0)
			{
				r_count++;

				if(r_count > 3)
				{
					r_count = 0;

					//.....
					xI_Sync_Ctrl.Thread__LOCK();

					if(iPRC_STS < 0)
					{
						xCH__OBJ_STATUS->Set__DATA("STANDBY");
					}

					xI_Sync_Ctrl.Thread__UNLOCK();
					//
				}
			}
			else
			{
				r_count = 0;
			}
		}

		// ...
	}
}

void CObj_Phy__PMC_DUMMY
::Mon__Module_Status(CII_OBJECT__VARIABLE *p_variable,
					 CII_OBJECT__ALARM *p_alarm)
{
	DECLARE__EXT_CTRL(p_variable);

	// ...
	SCX__ALARM_CTRL cx_alarm_ctrl;

	CString obj_ctrl;
	CString var_data;
	int pmc_alarm_flag;

	while(1)
	{
		Sleep(490);

		xEXT_CH__PMC_CONNECT_STATUS->Set__DATA("ONLINE");

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
				{
					continue;
				}

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
					{
						Post__PMx_ALARM(p_alarm);
					}
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
					{
						Clear__PMx_ALARM(p_alarm);
					}
				}
			}
		}

		// ...
	}
}

void CObj_Phy__PMC_DUMMY
::Post__PMx_ALARM(CII_OBJECT__ALARM *p_alarm)
{
	// ...
	{
		int alarm_id = ALID__PROCESS_ALARM;
		CString r_act;

		p_alarm->Check__ALARM(alarm_id,r_act);
		p_alarm->Post__ALARM(alarm_id);
	}

	// ...
	CString pm__lotid;
	CString str__lp_id;
	CString pm__lp_id;
	CString pm__lp_slotid;

	for(int i=0; i<iPMx_SLOT_MAX; i++)
	{
		if(xCH__SLOT_STATUS[i]->Check__DATA("NONE") > 0)
		{
			continue;
		}

		xCH__SLOT_LOTID[i]->Get__DATA(pm__lotid);
		xCH__SLOT_LPx_PORT_ID[i]->Get__DATA(pm__lp_id);
		xCH__SLOT_LPx_SLOT_ID[i]->Get__DATA(pm__lp_slotid);

		str__lp_id.Format("PORT%s",pm__lp_id);

		xSCH_MATERIAL_CTRL->Post__ALARM_INFO(atoi(pm__lp_id),atoi(pm__lp_slotid));
	}
}
void CObj_Phy__PMC_DUMMY
::Clear__PMx_ALARM(CII_OBJECT__ALARM *p_alarm)
{
	CString pm__lotid;
	CString str__lp_id;
	CString pm__lp_id;
	CString pm__lp_slotid;

	for(int i=0; i<iPMx_SLOT_MAX; i++)
	{
		if(xCH__SLOT_STATUS[i]->Check__DATA("NONE") > 0)
		{
			continue;
		}

		xCH__SLOT_LOTID[i]->Get__DATA(pm__lotid);
		xCH__SLOT_LPx_PORT_ID[i]->Get__DATA(pm__lp_id);
		xCH__SLOT_LPx_SLOT_ID[i]->Get__DATA(pm__lp_slotid);

		str__lp_id.Format("PORT%s",pm__lp_id);

		xSCH_MATERIAL_CTRL->Clear__ALARM_INFO(atoi(pm__lp_id),atoi(pm__lp_slotid));
	}
}

void CObj_Phy__PMC_DUMMY
::Mon__Pressure_Status(CII_OBJECT__VARIABLE *p_variable)
{
	double ref_atm_press;
	double ref_vac_press;
	double cur_press;
	double pre_press = -1;

	CString str_cur_press;
	CString var_data;


	while(1)
	{
		p_variable->Wait__SINGLE_OBJECT(0.1);


		// ...
		{
			if(xEXT_CH_CFG__PMC_USE->Check__DATA("DISABLE") > 0)
			{
				xCH__OBJ_VIRTUAL_STATUS->Set__DATA("DISABLE");

				xCH__OBJ_VIRTUAL_STAGE1_STATUS->Set__DATA("DISABLE");
				xCH__OBJ_VIRTUAL_STAGE2_STATUS->Set__DATA("DISABLE");
			}
			else
			{
				xCH__OBJ_STATUS->Get__DATA(var_data);
				xCH__OBJ_VIRTUAL_STATUS->Set__DATA(var_data);

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
}

void CObj_Phy__PMC_DUMMY
::Mon__Process_Time(CII_OBJECT__VARIABLE *p_variable)
{
	CString	data;
	double	cur_time, total_time, percent, time, change_time;
	double	wait_time;

	total_time = 0;
	percent = 0;

	xCH__CHANGE_PRC_TIME->Set__DATA("0");


	while(1)
	{
		p_variable->Wait__SINGLE_OBJECT(1.0);


		wait_time = xSCH_MATERIAL_CTRL->Get__PMx_FRONT_TIME();

		xCH__TOTAL_PRC_TIME->Get__DATA(data);
		time = atof(data);

		if(time > 0)
		{
			xCH__PROCESS_FLAG->Set__DATA("YES");
			total_time = time;

			//
			xCH__CHANGE_PRC_TIME->Set__DATA("0");
			xCH__TOTAL_PRC_TIME->Set__DATA("0");

			//
			data.Format("%.1f",total_time);

			xCH__CURRENT_PRC_TIME->Set__DATA(data);
			xCH__PROCESS_STATUS->Set__DATA("");
		}

		xCH__CURRENT_PRC_TIME->Get__DATA(data);
		cur_time = atof(data);

		if(cur_time > 0)
		{
			if(iPRC_STS < 0)
			{
				xCH__CHANGE_PRC_TIME->Set__DATA("0");
				xCH__CURRENT_PRC_TIME->Set__DATA("0.0");
				xCH__PROCESS_STATUS->Set__DATA("");

				cur_time = 0;
			}
		}

		if((total_time >= 1)&&(cur_time > 0))
		{
			xCH__CHANGE_PRC_TIME->Get__DATA(data);
			change_time = atoi(data);

			if(change_time != 0)
			{
				cur_time += change_time;
				if(cur_time >= total_time)
				{
					total_time = cur_time;
				}

				//
				data.Format("%1d sec",cur_time);

				xCH__CURRENT_PRC_TIME->Set__DATA(data);	
				xCH__CHANGE_PRC_TIME->Set__DATA("0");
			}

			percent = (cur_time/total_time)*100;

			//
			cur_time--;

			data.Format("%.1f",cur_time);
			xCH__CURRENT_PRC_TIME->Set__DATA(data);

			if(cur_time <= wait_time)
				xCH__PROCESS_STATUS->Set__DATA("WAIT");
			else
				xCH__PROCESS_STATUS->Set__DATA("");
		}
		else
		{
			percent = 0;
			xCH__PROCESS_STATUS->Set__DATA("");
		}

		data.Format("%.1f",percent);	
		xCH__PROCESS_PERCENT->Set__DATA(data);
	}
}

void CObj_Phy__PMC_DUMMY
::Mon__Leak_Check(CII_OBJECT__VARIABLE *p_variable)
{
	CString ref_time;
	CString elapse_time;
	CString cur_time;


	while(1)
	{
		p_variable->Wait__SINGLE_OBJECT(1.0);


		if(xEXT_CH_CFG__PMC_USE->Check__DATA(STR__ENABLE)         < 0)		continue;
		if(xCH_CFG__AUTO_LEAK_CHECK_USE->Check__DATA(STR__ENABLE) < 0)		continue;

		if(xEXT_CH_CFG__LEAK_CHECK__TIME_MODE->Check__DATA(STR__DATE_TIME) > 0)
		{
			xCH_CFG__AUTO_LEAK_CHECK_CUR_TIME->Set__DATA("0");

			// ...
			CString var_data;
			int check_day;
			int start_hour;
			int start_min;
			int start_sec;
			int end_hour;
			int end_min;
			int end_sec;

			//
			xCH__AUTO_LEAK_CHECK_RUN_DAY->Get__DATA(var_data);
			check_day = atoi(var_data);

			//
			xEXT_CH_CFG__LEAK_CHECK_START_TIME__HOUR->Get__DATA(var_data);
			start_hour = atoi(var_data);

			xEXT_CH_CFG__LEAK_CHECK_START_TIME__MINUTE->Get__DATA(var_data);
			start_min = atoi(var_data);

			xEXT_CH_CFG__LEAK_CHECK_START_TIME__SECOND->Get__DATA(var_data);
			start_sec = atoi(var_data);

			//
			xEXT_CH_CFG__LEAK_CHECK_END_TIME__HOUR->Get__DATA(var_data);
			end_hour = atoi(var_data);

			xEXT_CH_CFG__LEAK_CHECK_END_TIME__MINUTE->Get__DATA(var_data);
			end_min = atoi(var_data);

			xEXT_CH_CFG__LEAK_CHECK_END_TIME__SECOND->Get__DATA(var_data);
			end_sec = atoi(var_data);

			// ...
			CTime t = CTime::GetCurrentTime();

			int cur_day  = t.GetDay();
			int cur_hour = t.GetHour();
			int cur_min  = t.GetMinute();
			int cur_sec  = t.GetSecond();

			if(check_day != cur_day)
			{
				if(xCH__AUTO_LEAK_CHECK_RUN_FLAG->Check__DATA("") < 0)
				{
					xCH__AUTO_LEAK_CHECK_RUN_FLAG->Set__DATA("");
				}
			}

			if((xCH__AUTO_LEAK_CHECK_RUN_FLAG->Check__DATA("") > 0)
			&& (cur_hour >= start_hour))
			{
				int check_flag = -1;

				if(cur_hour < end_hour)
				{
					check_flag = 1;
				}
				else if(cur_hour == end_hour)
				{
					if(cur_min >= start_min)
					{
						if(cur_min < end_min)
						{
							check_flag = 1;
						}
						else if(cur_min == end_min)
						{
							check_flag = 1;
						}
					}
				}

				if(check_flag > 0)
				{
					if(xEXT_CH__PMC_LEAK_CHECK_FLAG->Check__DATA("YES") < 0)
					{
						CString req_day;
						req_day.Format("%1d",t.GetDay());
						xCH__AUTO_LEAK_CHECK_RUN_DAY->Set__DATA(req_day);
					}

					xCH__AUTO_LEAK_CHECK_RUN_FLAG->Set__DATA("YES");
					xEXT_CH__PMC_LEAK_CHECK_FLAG->Set__DATA("YES");
					xEXT_CH__LEAK_CHECK__SCH_SEQUENCE_MODE__REQ_FLAG->Set__DATA("YES");
				}
			}
		}
		else
		{
			xCH_CFG__AUTO_LEAK_CHECK_REF_TIME->Get__DATA(ref_time);

			xCH_CFG__AUTO_LEAK_CHECK_CUR_TIME->Get__DATA(cur_time);
			elapse_time.Format("%.1f",atof(cur_time) + 1.0);

			if(atof(ref_time) < atof(elapse_time))
			{
				if((xCH__MOVE_FLAG->Check__DATA("NO") > 0)
				&& (Check__Standby_Or_CtcInUse() > 0))
				{
					if(xCH_CFG__AUTO_LEAK_CHECK_MODE->Check__DATA(STR__LEAK_CHECK) > 0)
					{
						if(iPRC_STS > 0)
						{
							elapse_time = "0.0";

							xCH__NEXT_PROCESS->Set__DATA(STR__LEAK_CHECK);
						}
						else
						{	
							int material_flag = -1;
							int i;

							for(i=0;i<iPMx_SLOT_MAX;i++)
							{
								if(xCH__SLOT_STATUS[i]->Check__DATA("NONE") > 0)
								{
									continue;
								}

								material_flag = 1;
								break;
							}

							if(material_flag < 0)
							{
								elapse_time = "0.0";

								pOBJ_CTRL->Call__OBJECT(sMODE__LEAK_CHECK);
							}
						}
					}
					else		// JOB
					{
						int material_flag = -1;
						int i;

						for(i=0;i<iPMx_SLOT_MAX;i++)
						{
							if(xCH__SLOT_STATUS[i]->Check__DATA("NONE") > 0)
							{
								continue;
							}

							material_flag = 1;
							break;
						}

						if(material_flag < 0)
						{
							if(xSCH_MATERIAL_CTRL->Get__MATERIAL_DB_SIZE() < 1)
							{
								if(iPRC_STS < 0)
								{
									elapse_time = "0.0";

									pOBJ_CTRL->Call__OBJECT(sMODE__LEAK_CHECK);
								}
							}
						}
					}
				}
			}

			xCH_CFG__AUTO_LEAK_CHECK_CUR_TIME->Set__DATA(elapse_time);
		}

		// ...
	}
}

void CObj_Phy__PMC_DUMMY
::Mon__GoTo_Maint(CII_OBJECT__VARIABLE *p_variable)
{

	while(1)
	{
		p_variable->Wait__SINGLE_OBJECT(0.3);


		if(xCH__MOVE_FLAG->Check__DATA("NO") < 0)
		{
			continue;
		}

		if(xSCH_MATERIAL_CTRL->Check__PMx_GoTo_MAINT(iPMx_ID) < 0)
		{
			continue;
		}

		xSCH_MATERIAL_CTRL->Clear__PMx_GoTo_MAINT(iPMx_ID);

		if(Check__Standby_Or_CtcInUse() > 0)
		{
			if(iPRC_STS > 0)
			{
				xCH__NEXT_PROCESS->Set__DATA(STR__GOTO_MAINT);
			}
			else
			{	
				int material_flag = -1;
				int i;

				for(i=0;i<iPMx_SLOT_MAX;i++)
				{
					if(xCH__SLOT_STATUS[i]->Check__DATA("NONE") > 0)
					{
						continue;
					}

					material_flag = 1;
					break;
				}

				if(material_flag < 0)
				{
					pOBJ_CTRL->Call__OBJECT(sMODE__SYSTEM_MAINT);
				}
				else
				{
					xCH__NEXT_PROCESS->Set__DATA(STR__GOTO_MAINT);
				}
			}
		}

		// ...
	}
}

void CObj_Phy__PMC_DUMMY
::Mon__LPx_Wafer_Count(CII_OBJECT__VARIABLE *p_variable)
{
	int lp_check_flag;
	int i;


	while(1)
	{
		p_variable->Wait__SINGLE_OBJECT(0.3);


		lp_check_flag = -1;

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
}

void CObj_Phy__PMC_DUMMY
::Mon__Clean_Rcp_Count(CII_OBJECT__VARIABLE *p_variable,
					   CII_OBJECT__ALARM *p_alarm)
{

	while(1)
	{
		p_variable->Wait__SINGLE_OBJECT(0.3);


		if((xEXT_CH_CFG__PMC_USE->Check__DATA(STR__ENABLE) < 0)
		|| (xCH_CFG__CLEAN_USE->Check__DATA(STR__ENABLE)   < 0))
		{
			continue;
		}

		if(xCH__SLOT_STATUS[0]->Check__DATA("NONE") < 0)
		{
			continue;
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

			if(f_ref_count < 1)			continue;
			if(s_ref_count < 1)			continue;

			// ...
			int cur_count;

			xCH_CFG__CLEAN_COUNT->Get__DATA(var_data);
			cur_count = atoi(var_data);

			if(cur_count < f_ref_count)
			{
				continue;
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
						continue;
					}
				}
				else
				{
					double cur_idle_time;

					xCH__SYS_IDLE_TIME->Get__DATA(var_data);
					cur_idle_time = atof(var_data);

					if(ref_idle_time > cur_idle_time)
					{
						continue;
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
				Call__NEXT_PROC(p_variable,p_alarm);
			}
		}

		// ...
	}
}

int  CObj_Phy__PMC_DUMMY
::Check__Standby_Or_CtcInUse()
{
	CString obj_sts;
	xCH__OBJ_STATUS->Get__DATA(obj_sts);

	if(obj_sts.CompareNoCase("STANDBY")  == 0)		return 1;
	if(obj_sts.CompareNoCase("CTCINUSE") == 0)		return 1;

	return -1;
}

void CObj_Phy__PMC_DUMMY
::Check__PMx_Slot_Max()
{
	CString var_data;
	xEXT_CH_CFG__PMC_SLOT_MAX->Get__DATA(var_data);

	iPMx_SLOT_MAX = atoi(var_data);

		 if(iPMx_SLOT_MAX > CFG__PMx_MAX_SLOT)		iPMx_SLOT_MAX = CFG__PMx_MAX_SLOT;
	else if(iPMx_SLOT_MAX < 1)						iPMx_SLOT_MAX = 1;
}
int  CObj_Phy__PMC_DUMMY
::Get__PMx_Occupied_Slot()
{
	for(int i=0; i<iPMx_SLOT_MAX; i++)
	{
		if(xCH__SLOT_STATUS[i]->Check__DATA("NONE") > 0)
		{
			continue;
		}

		return (i+1);
	}
	return 0;
}
