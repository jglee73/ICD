#include "StdAfx.h"
#include "CObj_Phy__PMC_STD.h"
#include "CObj_Phy__PMC_STD__ALID.h"


// ...
void CObj_Phy__PMC_STD
::Fnc__Auto_PM(CII_OBJECT__VARIABLE *p_variable,
			   CII_OBJECT__ALARM *p_alarm)
{
	CString ref_time;
	CString elapse_time;
	CString cur_time;

	if(xCH__AUTO_PM_WARNING_USE_MODE->Check__DATA(STR__ENABLE) > 0)
	{
		CString var_data;
		xEXT_CH__PMC_AUTO_PM_FLAG->Get__DATA(var_data);

		if((var_data.CompareNoCase("YES") == 0)
		|| (var_data.CompareNoCase("REQ") == 0))
		{
			xCH__AUTO_PM_WARNING_TIMEOUT->Get__DATA(var_data);
			double ref_sec = atof(var_data);

			xCH__AUTO_PM_WARNING_CUR_TIME->Get__DATA(var_data);
			double cur_sec = atoi(var_data) + 1.0;

			if(cur_sec > ref_sec)
			{
				int alarm_id = ALID__PMC_AUTO_PM_TIMEOVER;		
				CString alm_msg;
				CString alm_bff;
				CString r_act;

				alm_msg  = "";
				alm_bff.Format("PM%1d의 기간 Auto_PM 시간이 경과 되었습니다. \n", iPMx_ID);
				alm_msg += alm_bff;
				alm_bff.Format("Auto_PM Timeout은 %.0f sec 입니다. \n", ref_sec);
				alm_msg += alm_bff;

				p_alarm->Check__ALARM(alarm_id,r_act);
				p_alarm->Post__WARNING_With_MESSAGE(alarm_id,alm_msg);
			}
			else
			{
				var_data.Format("%.0f", cur_sec);
				xCH__AUTO_PM_WARNING_CUR_TIME->Set__DATA(var_data);
			}
		}
		else
		{
			xCH__AUTO_PM_WARNING_CUR_TIME->Set__DATA("");
		}
	}

	if(xEXT_CH_CFG__PMC_USE->Check__DATA(STR__ENABLE) < 0)
	{
		xEXT_CH__PMC_AUTO_PM_FLAG->Set__DATA("");
		return;
	}
	if(xCH_CFG__AUTO_PM_USE_MODE->Check__DATA(STR__ENABLE) < 0)
	{
		if(xCH__AUTO_PM_RUN_COMPLETE->Check__DATA(STR__YES) < 0)
		{
			xCH__AUTO_PM_RUN_DAY->Set__DATA("");				
		}

		xEXT_CH__PMC_AUTO_PM_FLAG->Set__DATA("");
		return;
	}
	if(xEXT_CH__AUTO_PM__IDLE_USE_FLAG->Check__DATA(STR__ENABLE) > 0)
	{
		return;
	}

	if(xEXT_CH_CFG__AUTO_PM__TIME_MODE->Check__DATA(STR__DATE_TIME) > 0)
	{
		xCH_CFG__AUTO_PM_CUR_TIME->Set__DATA("0");

		CString var_data;
		int check_day;
		int start_hour;
		int start_min;
		int start_sec;
		int end_hour;
		int end_min;
		int end_sec;

		xCH__AUTO_PM_RUN_DAY->Get__DATA(var_data);
		check_day = atoi(var_data);

		// ...
		{
			xCH__AUTO_PM_START_TIME_HOUR->Get__DATA(var_data);
			start_hour = atoi(var_data);

			xCH__AUTO_PM_START_TIME_MINUTE->Get__DATA(var_data);
			start_min = atoi(var_data);

			xCH__AUTO_PM_START_TIME_SECOND->Get__DATA(var_data);
			start_sec = atoi(var_data);
		}

		// ...
		{
			xCH__AUTO_PM_END_TIME_HOUR->Get__DATA(var_data);
			end_hour = atoi(var_data);

			xCH__AUTO_PM_END_TIME_MINUTE->Get__DATA(var_data);
			end_min = atoi(var_data);

			xCH__AUTO_PM_END_TIME_SECOND->Get__DATA(var_data);
			end_sec = atoi(var_data);
		}

		// ...
		CTime t = CTime::GetCurrentTime();

		int cur_day  = t.GetDay();
		int cur_hour = t.GetHour();
		int cur_min  = t.GetMinute();
		int cur_sec  = t.GetSecond();

		if(check_day != cur_day)
		{
			if(xCH__AUTO_PM_RUN_FLAG->Check__DATA("") < 0)
			{
				xCH__AUTO_PM_RUN_FLAG->Set__DATA("");
			}

			if(xCH__AUTO_PM_RUN_COMPLETE->Check__DATA("") < 0)
			{
				xCH__AUTO_PM_RUN_COMPLETE->Set__DATA("");
			}
		}

		if(xCH__AUTO_PM_RUN_FLAG->Check__DATA("") > 0)
		{
			int check_flag = -1;

			int s_time = (start_hour * 10000) + (start_min * 100) + start_sec;
			int e_time = (end_hour * 10000)   + (end_min * 100)   + end_sec;
			int c_time = (cur_hour * 10000)   + (cur_min * 100)   + cur_sec;

			if((c_time >= s_time) && (c_time <= e_time))
			{
				check_flag = 1;
			}

			if(check_flag > 0)
			{
				if(xEXT_CH__PMC_AUTO_PM_FLAG->Check__DATA("YES") < 0)
				{
					CString req_day;
					req_day.Format("%1d",t.GetDay());
					xCH__AUTO_PM_RUN_DAY->Set__DATA(req_day);
				}

				xCH__AUTO_PM_RUN_FLAG->Set__DATA("YES");
				xEXT_CH__PMC_AUTO_PM_FLAG->Set__DATA("YES");
				xEXT_CH__AUTO_PM__SCH_SEQUENCE_MODE__REQ_FLAG->Set__DATA("YES");
			}
		}

		// ...
		{
			int flag_timeover = -1;
			int flag_req = -1;

			if(cur_hour > end_hour)
			{
				flag_timeover = 1;
			}
			else if(cur_hour == end_hour)
			{
				if(cur_min > end_min)
				{
					flag_timeover = 1;
				}
			}

			if(flag_timeover > 0)
			{
				if(xEXT_CH__PMC_AUTO_PM_FLAG->Check__DATA("YES") > 0)
				{
					flag_req = 1;
				}
				else if(cur_day != check_day)
				{
					flag_req = 1;
				}

				if(flag_req > 0)
				{	
					xCH__AUTO_PM_WARNING_CUR_TIME->Get__DATA(var_data);
					double cur_sec = atof(var_data);

					if(cur_sec < 1)
					{
						if(xEXT_CH__PMC_AUTO_PM_FLAG->Check__DATA("YES") < 0)
						{
							xEXT_CH__PMC_AUTO_PM_FLAG->Set__DATA("REQ");
						}

						xCH__AUTO_PM_WARNING_CUR_TIME->Set__DATA("1");
					}
				}
			}

			if((flag_timeover < 0) || (flag_req < 0))	
			{
				xCH__AUTO_PM_WARNING_CUR_TIME->Set__DATA("");
			}
		}
	}
	else
	{
		xCH_CFG__AUTO_PM_REF_TIME->Get__DATA(ref_time);

		xCH_CFG__AUTO_PM_CUR_TIME->Get__DATA(cur_time);
		elapse_time.Format("%.1f",atof(cur_time) + 1.0);

		if(atof(ref_time) < atof(elapse_time))
		{
			if((xCH__MOVE_FLAG->Check__DATA("NO") > 0)
			&& (Check__Standby_Or_CtcInUse() > 0))
			{
				if(xCH_CFG__AUTO_PM_MODE->Check__DATA(STR__AUTO_PM) > 0)
				{
					if(iPRC_STS > 0)
					{
						elapse_time = "0.0";

						xCH__NEXT_PROCESS->Set__DATA(STR__AUTO_PM);
					}
					else
					{	
						int material_flag = -1;
						int i;

						for(i=0; i<iPMx_SLOT_MAX; i++)
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

							// ...
							{
								CString log_msg;
								CString log_bff;
								CString var_data;

								log_msg = "Start(1) : Call__OBJECT(Auto_PM)\n";

								xCH_CFG__AUTO_PM_MODE->Get__DATA(var_data);
								log_bff.Format("   Auto_PM_Mode <- %s \n", var_data);
								log_msg += log_bff;
								log_bff.Format("   Material DB Size <- %1d \n", xSCH_MATERIAL_CTRL->Get__MATERIAL_DB_SIZE());
								log_msg += log_bff;

								xLOG_CTRL->WRITE__LOG(log_msg);
							}

							pOBJ_CTRL->Call__OBJECT(sMODE__AUTO_PM);
						}
					}
				}
				else		// JOB
				{
					int material_flag = -1;
					int i;

					for(i=0; i<iPMx_SLOT_MAX; i++)
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

								// ...
								{
									CString log_msg;
									CString log_bff;
									CString var_data;

									log_msg = "Start(2) : Call__OBJECT(Auto_PM)\n";

									xCH_CFG__AUTO_PM_MODE->Get__DATA(var_data);
									log_bff.Format("   Auto_PM_Mode <- %s \n", var_data);
									log_msg += log_bff;
									log_bff.Format("   Material DB Size <- %1d \n", xSCH_MATERIAL_CTRL->Get__MATERIAL_DB_SIZE());
									log_msg += log_bff;

									xLOG_CTRL->WRITE__LOG(log_msg);
								}

								pOBJ_CTRL->Call__OBJECT(sMODE__AUTO_PM);
							}
						}
					}
				}
			}
		}

		if(xCH__NEXT_PROCESS->Check__DATA("") > 0)
		{
			xCH_CFG__AUTO_PM_CUR_TIME->Set__DATA(elapse_time);
		}
	}

	// ...
}
