#include "StdAfx.h"
#include "CObj_Opr__AUTO_MODE.h"

#include "Macro_Function.h"


// ...
void CObj_Opr__AUTO_MODE
::Mon__ALL_STATE(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm)
{
	// ...
	{
		xCH__RESYNC_EDIT_STATUS_CUR->Set__DATA(STR__DISABLE);			
	}

	// ...
	{
		SCX__FILE_CTRL x_file_ctrl;
		CString ref_root;

		ref_root  = sDIR_ROOT;
		ref_root += "\\EQP_LOG";
		x_file_ctrl->Create__LOG_DIR(ref_root);

		ref_root += "\\Wafer_Data";
		x_file_ctrl->Create__LOG_DIR(ref_root);

		x_file_ctrl->Delete__LOG_DIR(ref_root, 90);
	}


	while(1)
	{
		p_variable->Wait__SINGLE_OBJECT(0.1);


		// ...
		{
			CString ch_state = xCH__JOB_STATUS->Get__STRING();	
			CString ch_data = "System ???";

			if(ch_state.CompareNoCase(STR__PROCESS) == 0)
			{
				ch_data = "System Process";
			}
			else if(ch_state.CompareNoCase(STR__END) == 0)
			{
				ch_data = "System End";
			}
			else if(ch_state.CompareNoCase(STR__IDLE) == 0)
			{
				ch_data = "System Idle";

				sEXT_CH__FNC_MSG->Set__DATA("");
			}
			else if(ch_state.CompareNoCase(STR__MAINT) == 0)
			{
				ch_data = "System Maint";

				sEXT_CH__FNC_MSG->Set__DATA("");
			}

			sEXT_CH__SYS_MSG->Set__DATA(ch_data);
		}

		Fnc__SYSTEM_IDLE(p_variable, p_alarm);
		Fnc__MANUAL_CTRL(p_variable, p_alarm);
		Fnc__SYS_STATUS(p_variable, p_alarm);
		Fnc__RESYNC_EDIT_CTRL(p_variable, p_alarm);
		Fnc__DUMMY_SLOT_COUNT_WARNING(p_variable, p_alarm);
	}
}

void CObj_Opr__AUTO_MODE
::Fnc__SYSTEM_IDLE(CII_OBJECT__VARIABLE *p_variable,
				   CII_OBJECT__ALARM *p_alarm)
{
	if(sCH_APP__DUMMY_PORT_CUR_COUNT_RESET_REQ->Check__DATA("") < 0)
	{
		CString cur_req;
		CString str_data;

		sCH_APP__DUMMY_PORT_CUR_COUNT_RESET_REQ->Get__DATA(cur_req);
		sCH_APP__DUMMY_PORT_CUR_COUNT_RESET_REQ->Set__DATA("");

			 if(cur_req.CompareNoCase(STR__YES)     == 0)		str_data = "0";
		else if(cur_req.CompareNoCase("LIMIT.TEST") == 0)		str_data = "100";

		// ...
		int i_limit = CFG_LP__SLOT_MAX;
		int i;

		for(i=0; i<i_limit; i++)
		{
			sEXT_CH__APP_DUMMY_PORT_CUR_COUNT_SLOTx[i]->Set__DATA(str_data);
		}
	}
}

void CObj_Opr__AUTO_MODE
::Fnc__MANUAL_CTRL(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm)
{
	CString str_lotid;
	CString str_ppid;

	CString var_data;
	int i;

	for(i=0; i<iLPx_UNIT_SIZE; i++)
	{
		int flag__ctrl  = -1;
		int flag__cycle = -1;

		if(xCH__PORT_MANUAL_CTRL_FLAG[i]->Check__DATA(STR__YES) > 0)
		{
			flag__ctrl = 1;

			CString str_msg = "Manual";
			xCH__PORT_DB_CYCLE_MSG[i]->Set__DATA(str_msg);

			xCH__PORT_CYCLE_CTRL_FLAG[i]->Set__DATA("");
		}
		if(xCH__PORT_CYCLE_CTRL_FLAG[i]->Check__DATA(STR__YES) > 0)
		{
			xCH__PORT_CYCLE_CTRL_FLAG[i]->Set__DATA(STR__RUNNING);

			flag__ctrl  = 1;
			flag__cycle = 1;
		}

		if(flag__ctrl > 0)
		{
			SCX__SEQ_INFO x_seq_info;

			// ...
			{
				int i_hour, i_min, i_sec, i_msec;

				x_seq_info->Get__TIME(i_hour,i_min,i_sec,i_msec);

				xCH__PORT_DB_LOTID[i]->Get__DATA(var_data);
				str_lotid.Format("%s_%002d%002d%002d", var_data,i_hour,i_min,i_sec);

				xCH__PORT_DB_ROUTEID[i]->Get__DATA(str_ppid);
			}

			if(xCH__PORT_STATUS[i]->Check__DATA("WAITING") > 0)
			{
				int job_file_flag = 1;

				CString str_jobid;
				CString str_job;
				CString str_css;
				CString str_bff;
				CString dir_proc;
				CString dir_path;

				int lp_id = i + 1;

				// Job ...
				{
					str_jobid.Format("LP%1d_LocalJOB", lp_id);

					//
					str_bff.Format("@CASSETTE-RECIPE,LP%1d_LocalJOB;\n", lp_id);
					str_job += str_bff;

					str_bff.Format("@SOURCE,LP%1d;\n", lp_id);
					str_job += str_bff;

					str_bff.Format("@TARGET,LP%1d;\n", lp_id);
					str_job += str_bff;

					str_bff.Format("@IO_MODE,DUMP;\n");
					str_job += str_bff;

					// ...
					{
						x_seq_info->Get__DIR_PROCESS(dir_proc);
						dir_path.Format("%s\\LP%1d\\LP%1d_LocalJOB.job", dir_proc,lp_id,lp_id);

						// ...
						FILE *out = fopen(dir_path,"w");

						if(out != NULL)
						{
							fputs(str_job,out);
							fclose(out);
						}
						else
						{
							job_file_flag = -1;

							// Alarm Report ...
							{
								int alarm_id = ALID__MANUAL_JOB_START__ERROR;
								CString alarm_msg;
								CString alarm_bff;
								CString r_act;

								alarm_msg.Format("LP%1d's manual job error ! \n", lp_id);
								alarm_bff.Format("Job path is \"%s\". \n", dir_path);
								alarm_msg += alarm_bff;

								p_alarm->Check__ALARM(alarm_id,r_act);
								p_alarm->Post__ALARM_With_MESSAGE(alarm_id,alarm_msg);
							}
						}
					}
				}

				// css ...
				{
					xCH__PORT_CFG_SLOT_MAX[i]->Get__DATA(var_data);
					int slot_max = atoi(var_data);

					xCH__PORT_DB_CYCLE_START_SLOT[i]->Get__DATA(var_data);
					int s_slot = atoi(var_data);
					
					xCH__PORT_DB_CYCLE_END_SLOT[i]->Get__DATA(var_data);
					int e_slot = atoi(var_data);

					int cur_slot;
					int k;

					for(k=0; k<slot_max; k++)
					{
						cur_slot = k + 1;

						// ...
						bool active__slot_check = true;

						xCH__PORT_SLOT_STS[i][k]->Get__DATA(var_data);

						if(flag__cycle > 0)
						{
							if(var_data.CompareNoCase(STR__NONE) == 0)
							{
								active__slot_check = false;
							}
						}
						else
						{
							if((var_data.CompareNoCase(STR__EXIST)  != 0)
							&& (var_data.CompareNoCase(STR__MAPPED) != 0))
							{
								active__slot_check = false;
							}
						}

						if((cur_slot < s_slot) || (cur_slot > e_slot))
						{
							active__slot_check = false;
						}

						if(active__slot_check)
						{
							str_bff.Format("#SLOT%002d,%s,,%s,,X,,\n", k+1,str_lotid,str_ppid);
							str_css += str_bff;
						}
						else
						{
							str_bff.Format("!SLOT%002d,%s,,,,\n", k+1,str_lotid);
							str_css += str_bff;
						}
					}

					// ...
					{
						x_seq_info->Get__DIR_PROCESS(dir_proc);
						dir_path.Format("%s\\LP%1d\\LP%1d_LocalJOB.css", dir_proc,lp_id,lp_id);

						// ...
						FILE *out = fopen(dir_path,"w");

						if(out != NULL)
						{
							fputs(str_css,out);
							fclose(out);
						}
						else
						{
							job_file_flag = -1;

							// Alarm Report ...
							{
								int alarm_id = ALID__MANUAL_JOB_START__ERROR;
								CString alarm_msg;
								CString alarm_bff;
								CString r_act;

								alarm_msg.Format("LP%1d's manual job error ! \n", lp_id);
								alarm_bff.Format("Cassete path is \"%s\". \n", dir_path);
								alarm_msg += alarm_bff;

								p_alarm->Check__ALARM(alarm_id,r_act);
								p_alarm->Post__ALARM_With_MESSAGE(alarm_id,alarm_msg);
							}
						}
					}
				}

				if(job_file_flag > 0)
				{
					if(flag__cycle > 0)
					{
						CString str_msg;

						xCH__PORT_CUR_CYCLE_COUNT[i]->Set__DATA("");

						xCH__PORT_DB_CYCLE_COUNT[i]->Get__DATA(var_data);								
						str_msg.Format("Start (%s)", var_data);
						xCH__PORT_DB_CYCLE_MSG[i]->Set__DATA(str_msg);

						xCH__PORT_CYCLE_CTRL_RUNNING_FLAG[i]->Set__DATA(STR__YES);
					}
					else
					{
						xCH__PORT_MANUAL_CTRL_RUNNING_FLAG[i]->Set__DATA(STR__YES);
					}

					// ...
					{
						xCH__PORT_END_REQ_FLAG[i]->Set__DATA("");

						xCH__PORT_JOB_POPUP_TYPE[i]->Set__DATA("");
						xCH__PORT_JOB_NAME[i]->Set__DATA(str_jobid);
					
						xCH__PORT_CTRL[i]->Set__DATA("RUN");
					}
				}
			}
			else
			{
				// Alarm Report ...
				{
					int alarm_id = ALID__MANUAL_JOB_START__ERROR;
					CString alarm_msg;
					CString alarm_bff;
					CString r_act;

					alarm_msg.Format("LP%1d's manual job error ! \n", i+1);
					
					xCH__PORT_STATUS[i]->Get__DATA(var_data);
					alarm_bff.Format("Current port status is \"%s\". \n", var_data);
					alarm_msg += alarm_bff;

					p_alarm->Check__ALARM(alarm_id,r_act);
					p_alarm->Post__ALARM_With_MESSAGE(alarm_id,alarm_msg);
				}
			}

			// ...
			{
				xCH__PORT_MANUAL_CTRL_FLAG[i]->Set__DATA("");	
			
				if(xCH__PORT_CYCLE_CTRL_FLAG[i]->Check__DATA(STR__RUNNING) < 0)
				{
					xCH__PORT_CYCLE_CTRL_FLAG[i]->Set__DATA("");
				}
			}
		}

		// ...
		{
			CString str__port_ctrl;
			CString str__port_sts;

			xCH__PORT_CTRL[i]->Get__DATA(str__port_ctrl);
			xCH__PORT_STATUS[i]->Get__DATA(str__port_sts);

			if((str__port_ctrl.CompareNoCase("RUN")    != 0)
			&& (str__port_sts.CompareNoCase("START")   != 0)	
			&& (str__port_sts.CompareNoCase(STR__RESERVE) != 0)
			&& (str__port_sts.CompareNoCase(STR__BUSY)    != 0)
			&& (str__port_sts.CompareNoCase("PAUSED")  != 0))
			{
				xCH__PORT_MANUAL_CTRL_RUNNING_FLAG[i]->Set__DATA(STR__NO);
				xCH__PORT_CYCLE_CTRL_RUNNING_FLAG[i]->Set__DATA(STR__NO);
			}
		}
	}
}

void CObj_Opr__AUTO_MODE::
Fnc__SYS_STATUS(CII_OBJECT__VARIABLE *p_varlable,CII_OBJECT__ALARM *p_alarm)
{
	CString ch_data;

	if(xSCH_MATERIAL_CTRL->Check__SYSTEM_STATUS__PROCESS() > 0)
	{
		xCH__SYS_STATUS->Set__DATA(STR__PROCESS);
	}
	else
	{
		if(xSCH_MATERIAL_CTRL->Check__SYSTEM_STATUS__MAINT() > 0)
		{
			xCH__SYS_STATUS->Set__DATA(STR__MAINT);
		}
		else 
		{
			xCH__SYS_STATUS->Set__DATA(STR__IDLE);
		}
	}

	// ...
	{
		int bff_count = 0;

		if(sEXT_CH__SCH_DB_ST1_USE_FLAG->Check__DATA(STR__ENABLE) > 0)
		{
			bff_count++;
		}
		if(sEXT_CH__SCH_DB_ST2_USE_FLAG->Check__DATA(STR__ENABLE) > 0)
		{
			bff_count++;
		}

		ch_data.Format("%1d", bff_count);
		xEXT_CH__CUR_STx_SIZE->Set__DATA(ch_data);
	}
}

void CObj_Opr__AUTO_MODE
::Fnc__RESYNC_EDIT_CTRL(CII_OBJECT__VARIABLE *p_variable,
					    CII_OBJECT__ALARM *p_alarm)
{
	int alarm_id;
	CString alm_msg;
	CString alm_bff;
	CString r_act;

	if(xCH__RESYNC_EDIT_STATUS_REQ->Check__DATA(STR__YES) > 0)
	{
		xCH__RESYNC_EDIT_STATUS_REQ->Set__DATA("");
		xCH__RESYNC_EDIT_STATUS_CUR->Set__DATA(STR__ENABLE);			
	}
	else if(xCH__RESYNC_EDIT_STATUS_REQ->Check__DATA(STR__NO) > 0)
	{
		xCH__RESYNC_EDIT_STATUS_REQ->Set__DATA("");
		
		// ...
		int err_flag = -1;

		// Wafer 중복 Check ...
		{	
			CStringArray l_err_pos;
			CStringArray l_err_wfr_status;
			CStringArray l_err_wfr_title;

			// ...
			{
				CStringArray l_ui_name;
				CStringArray l_ui_wafer_status;
				CStringArray l_ui_wafer_title;

				Fnc__Get_All_Wafer_Info_Of_UI(l_ui_name,l_ui_wafer_status,l_ui_wafer_title);

				Fnc__Check_Wafer_Duplication_Of_UI(l_ui_name,l_ui_wafer_status,l_ui_wafer_title, 
					                               l_err_pos,l_err_wfr_status,l_err_wfr_title);
			}

			if(l_err_pos.GetSize() > 0)
			{
				err_flag = 1;

				alarm_id = ALID__RESYNC_EDIT_WAFER_DUPLICATE;
				
				// ...
				{
					alm_msg.Format("중복된 Wafer 정보는 아래와 같습니다. \n");
				
					int k_limit = l_err_pos.GetSize();
					int k;

					for(k=0; k<k_limit; k++)
					{
						alm_bff.Format(" %1d)  \"%s\"의 \"%s\" \n", k+1,l_err_pos[k],l_err_wfr_title[k]);
						alm_msg += alm_bff;
					}
				}
				
				p_alarm->Check__ALARM(alarm_id, r_act);
				p_alarm->Post__ALARM_With_MESSAGE(alarm_id, alm_msg);
			}
		}

		if(err_flag < 0)
		{
			if(xSCH_MATERIAL_CTRL->Get__MATERIAL_DB_SIZE() > 0)
			{				
				// UI & Scheduler DB Check ...
				{
					CStringArray l_err_pos;
					CStringArray l_err_wfr_status;
					CStringArray l_err_wfr_title;

					CStringArray l_sch_pos;
					CStringArray l_sch_wfr_status;
					CStringArray l_sch_wfr_title;

					CStringArray l_ui_pos;
					CStringArray l_ui_wfr_status;
					CStringArray l_ui_wfr_title;
					
					Fnc__Get_All_Wafer_Info_Of_SCH(l_sch_pos,l_sch_wfr_status,l_sch_wfr_title);
					Fnc__Get_All_Wafer_Info_Of_UI( l_ui_pos, l_ui_wfr_status, l_ui_wfr_title );

					int flag = Fnc__Check_UI_Wafer_And_SCH_Wafer(l_ui_pos,  l_ui_wfr_status,  l_ui_wfr_title, 
										                         l_sch_pos, l_sch_wfr_status, l_sch_wfr_title,
																 l_err_pos, l_err_wfr_status, l_err_wfr_title);
					if(flag < 0)
					{
						err_flag = 1;
					}

					if(err_flag > 0)
					{
						alarm_id = ALID__RESYNC_EDIT_WAFER_UI_SCH_DB_ERROR;
	
						// ...
						{
							int t_limit = l_err_pos.GetSize();
							int t;

							alm_msg = "Scheduler DB 기준에 불일치하는 Wafer 정보입니다. \n";

							for(t=0; t<t_limit; t++)
							{
								alm_bff.Format("  %1d)  Position[%s] Wafer_Title[%s] \n", 
									           t+1,
											   l_err_pos[t],
											   l_err_wfr_title[t]);
								alm_msg += alm_bff;
							}
						}

						p_alarm->Check__ALARM(alarm_id, r_act);
						p_alarm->Post__ALARM_With_MESSAGE(alarm_id, alm_msg);

						if(r_act.CompareNoCase(STR__ABORT) == 0)
						{
							err_flag = -1;
							xSCH_MATERIAL_CTRL->Abort__ALL_PORT();
						}
					}
				}

				/*
				// All Robot Resume ...
				if(err_flag < 0)
				{
					sEXT_CH__ATM_ROBOT__PAUSE_FLAG->Set__DATA(STR__NO);
					sEXT_CH__VAC_ROBOT___PAUSE_FLAG->Set__DATA(STR__NO);
				}
				*/
			}
		}

		if(err_flag < 0)
		{
			// Alarm Post ...
			{
				alarm_id = ALID__RESYNC_EDIT_DISABLE;
				alm_msg  = "";
			
				p_alarm->Check__ALARM(alarm_id, r_act);
				p_alarm->Post__ALARM_With_MESSAGE(alarm_id, alm_msg);				
			}

			xCH__RESYNC_EDIT_STATUS_CUR->Set__DATA(STR__DISABLE);			
		}
	}

	if(xCH__RESYNC_EDIT_STATUS_CUR->Check__DATA(STR__ENABLE) > 0)
	{
		if(xSCH_MATERIAL_CTRL->Get__MATERIAL_DB_SIZE() > 0)
		{
			sEXT_CH__ATM_ROBOT__PAUSE_FLAG->Set__DATA(STR__YES);
			sEXT_CH__VAC_ROBOT__PAUSE_FLAG->Set__DATA(STR__YES);
		}

		// Alarm Post ...
		{
			alarm_id = ALID__RESYNC_EDIT_ENABLE;
			
			if(xSCH_MATERIAL_CTRL->Get__MATERIAL_DB_SIZE() > 0)
			{
				alm_msg  = "모든 Robot들은 정지 됩니다. \n";
				alm_msg += "단, 공정 진행 중인 Wafer들은 정상 처리됩니다. \n";					
			}
			else
			{
				alm_msg = "...";
			}

			p_alarm->Check__ALARM(alarm_id, r_act);
			p_alarm->Post__ALARM_With_MESSAGE(alarm_id, alm_msg);
		}			
	}

	// ...
}

void CObj_Opr__AUTO_MODE
::Fnc__DUMMY_SLOT_COUNT_WARNING(CII_OBJECT__VARIABLE *p_variable,
								CII_OBJECT__ALARM *p_alarm)
{
	CString slot, al_msg, buff;
	double Res_Cnt = 0;
	double Ena_Cnt = 0;
	int i;

	for(i=0; i<CFG_LP__SLOT_MAX; i++)
	{
		if(xCH_CFG__DUMMY_PORT_SLOT_USE[i]->Check__DATA(STR__ENABLE) > 0)
		{
			Ena_Cnt++;

			sEXT_CH__APP_DUMMY_PORT_CUR_COUNT_SLOTx[i]->Get__DATA(slot);
			double cur_num = atof(slot);

			Res_Cnt += cur_num;
		}
	}

	if(Ena_Cnt < 1)
	{
		return;
	}

	aCH_CFG__DUMMY_PORT_COUNT_WARNING_ALARM->Get__DATA(slot);
	double War_Cnt = atof(slot);

	if(War_Cnt < 1.0)
	{
		return;
	}

	double Average = Res_Cnt/Ena_Cnt;
	if(Average >= War_Cnt)
	{
		int alm_id = ALID__DUMMY_PORT_COUNT_WARNING;
		CString r_act;

		al_msg.Format("Dummy Port 현재 평균 사용량 : %.0f\nWarning Alarm 설정 값 : %.0f", Average, War_Cnt);

		p_alarm->Check__ALARM(alm_id,r_act);
		p_alarm->Post__ALARM_With_MESSAGE(alm_id,al_msg);
	}
}
