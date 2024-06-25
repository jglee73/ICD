#include "StdAfx.h"
#include "CObj_Opr__MAINT_MODE.h"

#include "Macro_Function.h"


// ...
void CObj_Opr__MAINT_MODE
::Mon__MACRO_MOVE_CTRL(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm)
{
	CString pre__log_status;
	int pre__log_count = 0;

	while(1)
	{
		p_variable->Wait__SINGLE_OBJECT(0.1);

		//
		if(sCH__MACRO_MOVE__LOG_STATUS->Check__DATA(STR__START) > 0)
		{
			if(pre__log_status.CompareNoCase(STR__START) != 0)
			{
				pre__log_status = STR__START;

				// ...
				{
					mMacro_Log.Clear__ALL_HISTORY();

					for(int i=0;i<CFG_SIZE__MOVE_HISTORY;i++)
					{
						sCH__MACRO_MOVE__HISTORY[i]->Set__DATA("");
					}
					sCH__MACRO_MOVE__LOG_COUNT->Set__DATA("__");
				}
	
				// ...
				{
					SCX__SEQ_INFO x_seq_info;
					int i_year,i_mon,i_day;
					int i_hour,i_min,i_sec,i_msec;
					CString file_name;

					x_seq_info->Get__DAY(i_year,i_mon,i_day);
					x_seq_info->Get__TIME(i_hour,i_min,i_sec,i_msec);

					file_name.Format("MOVE-%00004d%002d%002d_%002d%002d%002d", 
									i_year,i_mon,i_day,
									i_hour,i_min,i_sec);

					sCH__MACRO_MOVE__LOG_FILE_NAME->Set__DATA(file_name);
				}
			}
		}
		else
		{
			if(sCH__MACRO_MOVE__LOG_STATUS->Check__DATA(STR__SAVE) > 0)
			{
				sCH__MACRO_MOVE__LOG_STATUS->Set__DATA("");

				Sleep(500);

				printf(" * log_state <- save ! \n");
			}

			if(pre__log_status.CompareNoCase(STR__START) == 0)
			{
				pre__log_status = "";	

				// ...
				{
					mMacro_Log.Clear__ALL_HISTORY();

					for(int i=0;i<CFG_SIZE__MOVE_HISTORY;i++)
					{
						sCH__MACRO_MOVE__HISTORY[i]->Set__DATA("");
					}
					sCH__MACRO_MOVE__LOG_COUNT->Set__DATA("__");
				}

				printf(" * log_end ! \n");
			}
		}

		//
		if(sCH__MACRO_MOVE__LOG_VIEW_REQ->Check__DATA(STR__YES) > 0)
		{
			sCH__MACRO_MOVE__LOG_VIEW_REQ->Set__DATA("");

			sCH__MACRO_MOVE__SCR_NAME->Set__DATA(SCR_NAME__POPUP_MACRO_MOVE_HISTORY_VIEW);
		}

		//
		if(sCH__MACRO_MOVE__LOG_SAVE_REQ->Check__DATA(STR__YES) > 0)
		{
			printf(" * log_save_req ! \n");

			Sleep(500);
			
			sCH__MACRO_MOVE__LOG_SAVE_REQ->Set__DATA("");
			sCH__MACRO_MOVE__LOG_STATUS->Set__DATA("");

			mMacro_Log.Clear__ALL_HISTORY();

			// ...
			{
				CString cmmd_name;
				CString src_module;
				CString src_slot;
				CString trg_module;
				CString trg_slot;

				CString var_data = sCH__MACRO_MOVE__LOG_COUNT->Get__STRING();
				int i_limit = atoi(var_data);
				if(i_limit > CFG_SIZE__MOVE_HISTORY)		i_limit = CFG_SIZE__MOVE_HISTORY;

				for(int i=0; i<i_limit; i++)
				{						
					CString str__cmmd_line;
					sCH__MACRO_MOVE__HISTORY[i]->Get__DATA(str__cmmd_line);

					CString str_cmmd = str__cmmd_line;
					str_cmmd.MakeUpper();

					// ...
					int cmmd_type;

					CString str__para_1 = "";
					CString str__para_2 = "";
					CString str__para_3 = "";
					CString str__para_4 = "";

					if(str_cmmd.Find("TOOL_CLEAR") == 0)
					{
						cmmd_type = _CMMD__MACRO_MOVE__TOOL_CLEAR;
					}
					else if(str_cmmd.Find("MOVE") == 0)
					{
						cmmd_type = _CMMD__MACRO_MOVE__MANUAL_MOVE;
					}
					else if(str_cmmd.Find("PROCESS") == 0)
					{
						cmmd_type = _CMMD__MACRO_MOVE__PROCESS;
					}
					else if(str_cmmd.Find("REPEAT") == 0)
					{
						cmmd_type = _CMMD__MACRO_MOVE__LOOP;
					}
					else
					{
						cmmd_type = _CMMD__MACRO_MOVE__ERROR;
						str__para_1 = str__cmmd_line;
					}

					if((cmmd_type != _CMMD__MACRO_MOVE__TOOL_CLEAR)
					&& (cmmd_type != _CMMD__MACRO_MOVE__ERROR))
					{
						CStringArray l_para;
						Macro__Get_Para_List_From_String(str_cmmd, l_para);

						int k_limit = l_para.GetSize();
						int k;

						for(k=0; k<k_limit; k++)
						{
							CString str_para = l_para[k];

							     if(k == 0)			str__para_1 = str_para;
							else if(k == 1)			str__para_2 = str_para;
							else if(k == 2)			str__para_3 = str_para;
							else if(k == 3)			str__para_4 = str_para;
						}
					}

					//
					mMacro_Log.Load__MOVE_INFO(cmmd_type, str__para_1,str__para_2,str__para_3,str__para_4, str__cmmd_line);
				}
			}

			Fnc__MOVE_HISTORY_TO_SCR();		
		}

		// EDITOR ...
		{
			Fnc__MACRO_MOVE_EDITOR(p_variable, p_alarm);
		}
	}
}

void CObj_Opr__MAINT_MODE
::Fnc__MACRO_MOVE_EDITOR(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm)
{
	CString var_data;
	int i;

	// ...
	CString cur_act = sCH__MACRO_MOVE__EDITOR_ACT_CMMD->Get__STRING();

	if(cur_act.CompareNoCase("MOVE") == 0)
	{
		int r_flag = 1;

		CString src_module;
		CString src_slot;

		CString trg_module;
		CString trg_slot;

		sCH__MACRO_MOVE__SRC_MODULE->Get__DATA(src_module);
		sCH__MACRO_MOVE__SRC_SLOT->Get__DATA(src_slot);

		sCH__MACRO_MOVE__TRG_MODULE->Get__DATA(trg_module);				
		sCH__MACRO_MOVE__TRG_SLOT->Get__DATA(trg_slot);

		if((src_module.GetLength() < 1)
		|| (trg_module.GetLength() < 1))	
		{
			CString err_title;
			CString err_msg;
			CString err_bff;
			CStringArray l_act;
			CString r_act;

			err_title = "Move Parameter Error !";

			err_bff.Format("Source Module : [%s] \n", src_module);
			err_msg += err_bff;

			err_bff.Format("Source Slot   : [%s] \n", src_slot);
			err_msg += err_bff;

			err_bff.Format("Target Module : [%s] \n", trg_module);
			err_msg += err_bff;

			err_bff.Format("Target Slot   : [%s] \n", trg_slot);
			err_msg += err_bff;

			l_act.RemoveAll();
			l_act.Add("CHECK");

			p_alarm->Popup__MESSAGE_BOX(err_title,err_msg,l_act,r_act);

			r_flag = -1;
		}

		if(r_flag > 0)
		{
			Fnc__MACRO_MOVE_CHANGE_INFO();

			sCH__MACRO_MOVE__EDITOR_CUR_INDEX->Get__DATA(var_data);
			int db_index = atoi(var_data);
			if(db_index < 0)			db_index = 0;

			if(db_index < CFG_SIZE__MOVE_HISTORY)
			{
				CString src_module;
				CString src_slot;

				CString trg_module;
				CString trg_slot;

				sCH__MACRO_MOVE__SRC_MODULE->Get__DATA(src_module);
				sCH__MACRO_MOVE__SRC_SLOT->Get__DATA(src_slot);

				sCH__MACRO_MOVE__TRG_MODULE->Get__DATA(trg_module);				
				sCH__MACRO_MOVE__TRG_SLOT->Get__DATA(trg_slot);

				CString str__act_msg;
				str__act_msg.Format("Move : [%s][%s] -> [%s][%s]",
									src_module,src_slot,
									trg_module,trg_slot);
				sCH__MACRO_MOVE__EDITOR_ACTION_NAME[db_index]->Set__DATA(str__act_msg);

				var_data.Format("%1d", db_index+1);
				sCH__MACRO_MOVE__EDITOR_CUR_INDEX->Set__DATA(var_data);

				sCH__MACRO_MOVE__EDITOR_CHANGE_FLAG->Set__DATA("YES");
			}
		}
	}
	else if(cur_act.CompareNoCase("LOOP") == 0)
	{
		int r_flag = 1;

		CString para__loop_start;
		CString para__loop_end;
		CString para__loop_repeat;

		aCH__MACRO_MOVE__LOOP_START->Get__DATA(para__loop_start);
		aCH__MACRO_MOVE__LOOP_END->Get__DATA(para__loop_end);
		aCH__MACRO_MOVE__LOOP_REPEAT->Get__DATA(para__loop_repeat);

		// Error Check ...
		{

		}

		if(r_flag > 0)
		{
			sCH__MACRO_MOVE__EDITOR_CUR_INDEX->Get__DATA(var_data);
			int db_index = atoi(var_data);
			if(db_index < 0)			db_index = 0;

			if(db_index < CFG_SIZE__MOVE_HISTORY)
			{
				aCH__MACRO_MOVE__LOOP_START->Get__DATA(para__loop_start);
				aCH__MACRO_MOVE__LOOP_END->Get__DATA(para__loop_end);
				aCH__MACRO_MOVE__LOOP_REPEAT->Get__DATA(para__loop_repeat);

				CString str__act_msg;
				str__act_msg.Format("Repeat[%s] : [%s] -> [%s]",
									para__loop_repeat,
									para__loop_start,
									para__loop_end);
				sCH__MACRO_MOVE__EDITOR_ACTION_NAME[db_index]->Set__DATA(str__act_msg);

				var_data.Format("%1d", db_index+1);
				sCH__MACRO_MOVE__EDITOR_CUR_INDEX->Set__DATA(var_data);

				sCH__MACRO_MOVE__EDITOR_CHANGE_FLAG->Set__DATA("YES");
			}
		}
	}
	else if(cur_act.CompareNoCase("PROCESS") == 0)
	{
		CString para__prc_pm = dCH__MACRO_MOVE__PROCESS_PMx->Get__STRING();

		sCH__MACRO_MOVE__EDITOR_CUR_INDEX->Get__DATA(var_data);
		int db_index = atoi(var_data);
		if(db_index < 0)			db_index = 0;

		if(db_index < CFG_SIZE__MOVE_HISTORY)
		{
			CString str__act_msg;

			str__act_msg.Format("Process : [%s]", para__prc_pm);
			sCH__MACRO_MOVE__EDITOR_ACTION_NAME[db_index]->Set__DATA(str__act_msg);

			var_data.Format("%1d", db_index+1);
			sCH__MACRO_MOVE__EDITOR_CUR_INDEX->Set__DATA(var_data);

			sCH__MACRO_MOVE__EDITOR_CHANGE_FLAG->Set__DATA("YES");
		}
	}
	else if(cur_act.CompareNoCase("TOOL_CLEAR") == 0)
	{
		Fnc__MACRO_MOVE_INIT_INFO();

		sCH__MACRO_MOVE__EDITOR_CUR_INDEX->Get__DATA(var_data);
		int db_index = atoi(var_data);
		if(db_index < 0)			db_index = 0;

		if(db_index < CFG_SIZE__MOVE_HISTORY)
		{
			CString str__act_msg = "TOOL_CLEAR";
			sCH__MACRO_MOVE__EDITOR_ACTION_NAME[db_index]->Set__DATA(str__act_msg);

			var_data.Format("%1d", db_index+1);
			sCH__MACRO_MOVE__EDITOR_CUR_INDEX->Set__DATA(var_data);

			sCH__MACRO_MOVE__EDITOR_CHANGE_FLAG->Set__DATA("YES");
		}
	}
	else if(cur_act.CompareNoCase("ALL_CLEAR") == 0)
	{
		Fnc__MACRO_MOVE_EDITOR_CLEAR();

		sCH__MACRO_MOVE__EDITOR_CHANGE_FLAG->Set__DATA("YES");
	}
	else if(cur_act.CompareNoCase("WAFER_RESET") == 0)
	{
		Fnc__MACRO_MOVE_WAFER_RESET();

		sCH__MACRO_MOVE__EDITOR_CHANGE_FLAG->Set__DATA("YES");
	}

	if(cur_act.GetLength() > 0)
	{
		sCH__MACRO_MOVE__EDITOR_ACT_CMMD->Set__DATA("");
	}
}

void CObj_Opr__MAINT_MODE
::Fnc__MOVE_HISTORY_TO_SCR()
{
	// ...
	{
		for(int i=0;i<CFG_SIZE__MOVE_HISTORY;i++)
		{
			sCH__MACRO_MOVE__HISTORY[i]->Set__DATA("");
		}
	}

	// ...
	{
		CUIntArray   l_cmmd_type;
		CStringArray l_para_1;
		CStringArray l_para_2;
		CStringArray l_para_3;
		CStringArray l_para_4;
		CString str_msg;

		mMacro_Log.Get__ALL_MOVE_HISTORY(l_cmmd_type, l_para_1,l_para_2,l_para_3,l_para_4);

		// ...
		int limit = l_cmmd_type.GetSize();
		int i;

		if(limit > CFG_SIZE__MOVE_HISTORY)
		{
			limit = CFG_SIZE__MOVE_HISTORY;
		}

		for(i=0;i<limit;i++)
		{
			int move_type = l_cmmd_type[i];
			CString str_para1 = l_para_1[i];
			CString str_para2 = l_para_2[i];
			CString str_para3 = l_para_3[i];
			CString str_para4 = l_para_4[i];

			if(move_type == _CMMD__MACRO_MOVE__MANUAL_MOVE)
			{
				str_msg.Format("#MOVE [%s][%s] -> [%s][%s]", 
								str_para1,
								str_para2,
								str_para3,
								str_para4);
			}
			else if(move_type == _CMMD__MACRO_MOVE__TOOL_CLEAR)
			{
				str_msg = "#TOOL_CLEAR";
			}
			else if(move_type == _CMMD__MACRO_MOVE__PROCESS)
			{
				str_msg.Format("#PROCESS : [%s]", 
								str_para1);
			}
			else if(move_type == _CMMD__MACRO_MOVE__LOOP)
			{
				str_msg.Format("#LOOP [%s] : [%s] -> [%s]", 
								str_para1,
								str_para2,
								str_para3);
			}
			else
			{
				str_msg.Format("ERROR : [%s]", str_para1);
			}

			sCH__MACRO_MOVE__HISTORY[i]->Set__DATA(str_msg);
		}
	}

	// ...
}

void CObj_Opr__MAINT_MODE
::Fnc__MACRO_MOVE_CHANGE_INFO()
{
	CString src_slot_status;
	CString src_slot_title;

	// Source ...
	{
		int check_flag = -1;

		CString src_module;
		CString src_slot;

		sCH__MACRO_MOVE__SRC_MODULE->Get__DATA(src_module);
		sCH__MACRO_MOVE__SRC_SLOT->Get__DATA(src_slot);

		// LPx ...
		if(check_flag < 0)
		{
			int lp_index   = Macro__Get_LP_INDEX(src_module);
			int slot_index = atoi(src_slot) - 1; 

			if(lp_index >= 0)
			{
				check_flag = 1;

				if((slot_index >= 0)
				&& (slot_index <  CFG_LP__SLOT_MAX))	
				{
					sCH__MACRO_MOVE__LPx_SLOT_STATUS[lp_index][slot_index]->Get__DATA(src_slot_status);
					src_slot_title.Format("P%1d:%1d", lp_index+1,slot_index+1);

					sCH__MACRO_MOVE__LPx_SLOT_STATUS[lp_index][slot_index]->Set__DATA(STR__NONE);
				}
			}
		}

		// ATM_RB ...
		if(check_flag < 0)
		{
			if(src_module.CompareNoCase("ATM_RB1_A") == 0)
			{
				check_flag = 1;

				// ...
				{
					sCH__MACRO_MOVE__ATM_RB__A_ARM_SLOT_STATUS->Get__DATA(src_slot_status);
					sCH__MACRO_MOVE__ATM_RB__A_ARM_SLOT_TITLE->Get__DATA(src_slot_title);

					sCH__MACRO_MOVE__ATM_RB__A_ARM_SLOT_STATUS->Set__DATA(STR__NONE);
					sCH__MACRO_MOVE__ATM_RB__A_ARM_SLOT_TITLE->Set__DATA("");
				}
			}
			else if(src_module.CompareNoCase("ATM_RB1_B") == 0)
			{
				check_flag = 1;

				// ...
				{
					sCH__MACRO_MOVE__ATM_RB__B_ARM_SLOT_STATUS->Get__DATA(src_slot_status);
					sCH__MACRO_MOVE__ATM_RB__B_ARM_SLOT_TITLE->Get__DATA(src_slot_title);

					sCH__MACRO_MOVE__ATM_RB__B_ARM_SLOT_STATUS->Set__DATA(STR__NONE);
					sCH__MACRO_MOVE__ATM_RB__B_ARM_SLOT_TITLE->Set__DATA("");
				}
			}
		}

		// AL1 ...
		if(check_flag < 0)
		{
			if(src_module.CompareNoCase("AL1") == 0)
			{
				int slot_index = atoi(src_slot) - 1; 

				if((slot_index >= 0)
				&& (slot_index <  CFG_AL__SLOT_MAX))	
				{
					check_flag = 1;

					// ...
					{
						sCH__MACRO_MOVE__AL_SLOT_STATUS[slot_index]->Get__DATA(src_slot_status);
						sCH__MACRO_MOVE__AL_SLOT_TITLE[slot_index]->Get__DATA(src_slot_title);

						sCH__MACRO_MOVE__AL_SLOT_STATUS[slot_index]->Set__DATA(STR__NONE);
						sCH__MACRO_MOVE__AL_SLOT_TITLE[slot_index]->Set__DATA("");
					}
				}
			}
		}

		// BUFFER1 ...
		if(check_flag < 0)
		{
			if(src_module.CompareNoCase(STR__BUFF1) == 0)
			{
				check_flag = 1;

				// ...
				{
					int slot_index = atoi(src_slot) - 1;

					if((slot_index >= 0)
						&& (slot_index <  CFG_BUFFER__SLOT_MAX))	
					{
						sCH__MACRO_MOVE__BUFFER1_SLOT_STATUS[slot_index]->Get__DATA(src_slot_status);
						sCH__MACRO_MOVE__BUFFER1_SLOT_TITLE[slot_index]->Get__DATA(src_slot_title);

						sCH__MACRO_MOVE__BUFFER1_SLOT_STATUS[slot_index]->Set__DATA(STR__NONE);
						sCH__MACRO_MOVE__BUFFER1_SLOT_TITLE[slot_index]->Set__DATA("");
					}
				}
			}
		}

		// BUFFER2 ...
		if(check_flag < 0)
		{
			if(src_module.CompareNoCase(STR__BUFF2) == 0)
			{
				check_flag = 1;

				// ...
				{
					int slot_index = atoi(src_slot) - 1;

					if((slot_index >= 0)
						&& (slot_index <  CFG_BUFFER__SLOT_MAX))	
					{
						sCH__MACRO_MOVE__BUFFER2_SLOT_STATUS[slot_index]->Get__DATA(src_slot_status);
						sCH__MACRO_MOVE__BUFFER2_SLOT_TITLE[slot_index]->Get__DATA(src_slot_title);

						sCH__MACRO_MOVE__BUFFER2_SLOT_STATUS[slot_index]->Set__DATA(STR__NONE);
						sCH__MACRO_MOVE__BUFFER2_SLOT_TITLE[slot_index]->Set__DATA("");
					}
				}
			}
		}

		// LLx ...
		if(check_flag < 0)
		{
			int ll_i = Get__LLx_INDEX(src_module);
			if(ll_i >= 0)
			{
				check_flag = 1;
				
				// ...
				{
					int slot_index = atoi(src_slot) - 1;

					if((slot_index >= 0)
					&& (slot_index <  CFG_LLx__SLOT_MAX))	
					{
						sCH__MACRO_MOVE__LLx_SLOT_STATUS[ll_i][slot_index]->Get__DATA(src_slot_status);
						sCH__MACRO_MOVE__LLx_SLOT_TITLE[ll_i][slot_index]->Get__DATA(src_slot_title);

						sCH__MACRO_MOVE__LLx_SLOT_STATUS[ll_i][slot_index]->Set__DATA(STR__NONE);
						sCH__MACRO_MOVE__LLx_SLOT_TITLE[ll_i][slot_index]->Set__DATA("");
					}
				}
			}
		}

		// VAC_RB ...
		if(check_flag < 0)
		{
			if(src_module.CompareNoCase("VAC_RB1_A") == 0)
			{
				check_flag = 1;

				// ...
				{
					sCH__MACRO_MOVE__VAC_RB__A_ARM_SLOT_STATUS->Get__DATA(src_slot_status);
					sCH__MACRO_MOVE__VAC_RB__A_ARM_SLOT_TITLE->Get__DATA(src_slot_title);

					sCH__MACRO_MOVE__VAC_RB__A_ARM_SLOT_STATUS->Set__DATA(STR__NONE);
					sCH__MACRO_MOVE__VAC_RB__A_ARM_SLOT_TITLE->Set__DATA("");
				}
			}
			if(src_module.CompareNoCase("VAC_RB1_B") == 0)
			{
				check_flag = 1;

				// ...
				{
					sCH__MACRO_MOVE__VAC_RB__B_ARM_SLOT_STATUS->Get__DATA(src_slot_status);
					sCH__MACRO_MOVE__VAC_RB__B_ARM_SLOT_TITLE->Get__DATA(src_slot_title);

					sCH__MACRO_MOVE__VAC_RB__B_ARM_SLOT_STATUS->Set__DATA(STR__NONE);
					sCH__MACRO_MOVE__VAC_RB__B_ARM_SLOT_TITLE->Set__DATA("");
				}
			}
		}

		// PMC ...
		if(check_flag < 0)
		{
			int pm_index = Macro__Get_PM_INDEX(src_module);

			if((pm_index >= 0)
				&& (pm_index <  CFG_PM_LIMIT))
			{
				check_flag = 1;

				// ...
				{
					sCH__MACRO_MOVE__PMC_SLOT_STATUS[pm_index]->Get__DATA(src_slot_status);
					sCH__MACRO_MOVE__PMC_SLOT_TITLE[pm_index]->Get__DATA(src_slot_title);

					sCH__MACRO_MOVE__PMC_SLOT_STATUS[pm_index]->Set__DATA(STR__NONE);
					sCH__MACRO_MOVE__PMC_SLOT_TITLE[pm_index]->Set__DATA("");
				}
			}
		}						

		// ...
		if(check_flag < 0)
		{
			return;
		}
	}	


	// Target ...
	{
		int check_flag = -1;

		CString trg_module;
		CString trg_slot;

		sCH__MACRO_MOVE__TRG_MODULE->Get__DATA(trg_module);				
		sCH__MACRO_MOVE__TRG_SLOT->Get__DATA(trg_slot);

		// LPx ...
		if(check_flag < 0)
		{
			int lp_index   = Macro__Get_LP_INDEX(trg_module);
			int slot_index = atoi(trg_slot) - 1; 

			if(lp_index >= 0)
			{
				check_flag = 1;

				if((slot_index >= 0)
					&& (slot_index <  CFG_LP__SLOT_MAX))	
				{
					sCH__MACRO_MOVE__LPx_SLOT_STATUS[lp_index][slot_index]->Set__DATA(src_slot_status);
				}
			}
		}

		// ATM_RB ...
		if(check_flag < 0)
		{
			if(trg_module.CompareNoCase("ATM_RB1_A") == 0)
			{
				check_flag = 1;

				// ...
				{
					sCH__MACRO_MOVE__ATM_RB__A_ARM_SLOT_STATUS->Set__DATA(src_slot_status);
					sCH__MACRO_MOVE__ATM_RB__A_ARM_SLOT_TITLE->Set__DATA(src_slot_title);
				}
			}
			else if(trg_module.CompareNoCase("ATM_RB1_B") == 0)
			{
				check_flag = 1;

				// ...
				{
					sCH__MACRO_MOVE__ATM_RB__B_ARM_SLOT_STATUS->Set__DATA(src_slot_status);
					sCH__MACRO_MOVE__ATM_RB__B_ARM_SLOT_TITLE->Set__DATA(src_slot_title);
				}
			}
		}

		// AL1 ...
		if(check_flag < 0)
		{
			if(trg_module.CompareNoCase("AL1") == 0)
			{
				int slot_index = atoi(trg_slot) - 1; 

				if((slot_index >= 0)
				&& (slot_index <  CFG_AL__SLOT_MAX))	
				{
					check_flag = 1;

					// ...
					{
						sCH__MACRO_MOVE__AL_SLOT_STATUS[slot_index]->Set__DATA(src_slot_status);
						sCH__MACRO_MOVE__AL_SLOT_TITLE[slot_index]->Set__DATA(src_slot_title);
					}
				}
			}
		}

		// BUFFER1 ...
		if(check_flag < 0)
		{
			if(trg_module.CompareNoCase(STR__BUFF1) == 0)
			{
				check_flag = 1;

				// ...
				{
					int slot_index = atoi(trg_slot) - 1;

					if((slot_index >= 0)
						&& (slot_index <  CFG_BUFFER__SLOT_MAX))	
					{
						sCH__MACRO_MOVE__BUFFER1_SLOT_STATUS[slot_index]->Set__DATA(src_slot_status);
						sCH__MACRO_MOVE__BUFFER1_SLOT_TITLE[slot_index]->Set__DATA(src_slot_title);
					}
				}
			}
		}

		// BUFFER2 ...
		if(check_flag < 0)
		{
			if(trg_module.CompareNoCase(STR__BUFF2) == 0)
			{
				check_flag = 1;

				// ...
				{
					int slot_index = atoi(trg_slot) - 1;

					if((slot_index >= 0)
						&& (slot_index <  CFG_BUFFER__SLOT_MAX))	
					{
						sCH__MACRO_MOVE__BUFFER2_SLOT_STATUS[slot_index]->Set__DATA(src_slot_status);
						sCH__MACRO_MOVE__BUFFER2_SLOT_TITLE[slot_index]->Set__DATA(src_slot_title);
					}
				}
			}
		}

		// LLx ...
		if(check_flag < 0)
		{
			int ll_i = Get__LLx_INDEX(trg_module);
			if(ll_i >= 0)
			{
				check_flag = 1;

				// ...
				{
					int slot_index = atoi(trg_slot) - 1;

					if((slot_index >= 0)
					&& (slot_index <  CFG_LLx__SLOT_MAX))	
					{
						sCH__MACRO_MOVE__LLx_SLOT_STATUS[ll_i][slot_index]->Set__DATA(src_slot_status);
						sCH__MACRO_MOVE__LLx_SLOT_TITLE[ll_i][slot_index]->Set__DATA(src_slot_title);
					}
				}
			}
		}

		// VAC_RB ...
		if(check_flag < 0)
		{
			if(trg_module.CompareNoCase("VAC_RB1_A") == 0)
			{
				check_flag = 1;

				// ...
				{
					sCH__MACRO_MOVE__VAC_RB__A_ARM_SLOT_STATUS->Set__DATA(src_slot_status);
					sCH__MACRO_MOVE__VAC_RB__A_ARM_SLOT_TITLE->Set__DATA(src_slot_title);
				}
			}
			if(trg_module.CompareNoCase("VAC_RB1_B") == 0)
			{
				check_flag = 1;

				// ...
				{
					sCH__MACRO_MOVE__VAC_RB__B_ARM_SLOT_STATUS->Set__DATA(src_slot_status);
					sCH__MACRO_MOVE__VAC_RB__B_ARM_SLOT_TITLE->Set__DATA(src_slot_title);
				}
			}
		}

		// PMC ...
		if(check_flag < 0)
		{
			int pm_index = Macro__Get_PM_INDEX(trg_module);

			if((pm_index >= 0)
				&& (pm_index <  CFG_PM_LIMIT))
			{
				check_flag = 1;

				// ...
				{
					sCH__MACRO_MOVE__PMC_SLOT_STATUS[pm_index]->Set__DATA(src_slot_status);
					sCH__MACRO_MOVE__PMC_SLOT_TITLE[pm_index]->Set__DATA(src_slot_title);
				}
			}
		}						

		// ...
	}
}

void CObj_Opr__MAINT_MODE
::Fnc__MACRO_MOVE_INIT_INFO()
{
	int i;
	int j;

	// LPx ...
	for(i=0; i<CFG_LP_LIMIT; i++)
	{
		for(j=0; j<CFG_LP__SLOT_MAX; j++)
		{
			sCH__MACRO_MOVE__LPx_SLOT_STATUS[i][j]->Set__DATA(STR__EXIST);
		}
	}

	// ATM_RB ...
	{
		sCH__MACRO_MOVE__ATM_RB__A_ARM_SLOT_STATUS->Set__DATA(STR__NONE);
		sCH__MACRO_MOVE__ATM_RB__A_ARM_SLOT_TITLE->Set__DATA("");

		sCH__MACRO_MOVE__ATM_RB__B_ARM_SLOT_STATUS->Set__DATA(STR__NONE);
		sCH__MACRO_MOVE__ATM_RB__B_ARM_SLOT_TITLE->Set__DATA("");
	}

	// AL1 ...
	for(i=0; i<CFG_AL__SLOT_MAX; i++)
	{
		sCH__MACRO_MOVE__AL_SLOT_STATUS[i]->Set__DATA(STR__NONE);
		sCH__MACRO_MOVE__AL_SLOT_TITLE[i]->Set__DATA("");
	}

	// BUFFERx ...
	{
		// BUFFER1
		for(i=0; i<CFG_BUFFER__SLOT_MAX; i++)
		{
			sCH__MACRO_MOVE__BUFFER1_SLOT_STATUS[i]->Set__DATA(STR__NONE);
			sCH__MACRO_MOVE__BUFFER1_SLOT_TITLE[i]->Set__DATA("");
		}
		
		// BUFFER2 ...
		for(i=0; i<CFG_BUFFER__SLOT_MAX; i++)
		{
			sCH__MACRO_MOVE__BUFFER2_SLOT_STATUS[i]->Set__DATA(STR__NONE);
			sCH__MACRO_MOVE__BUFFER2_SLOT_TITLE[i]->Set__DATA("");
		}
	}

	// LLx ...
	{
		int ll_i;

		for(ll_i=0; ll_i<iLLx_SIZE; ll_i++)
		{
			for(i=0; i<CFG_LLx__SLOT_MAX; i++)
			{
				sCH__MACRO_MOVE__LLx_SLOT_STATUS[ll_i][i]->Set__DATA(STR__NONE);
				sCH__MACRO_MOVE__LLx_SLOT_TITLE[ll_i][i]->Set__DATA("");
			}
		}
	}

	// VAC_RB ...
	{
		sCH__MACRO_MOVE__VAC_RB__A_ARM_SLOT_STATUS->Set__DATA(STR__NONE);
		sCH__MACRO_MOVE__VAC_RB__A_ARM_SLOT_TITLE->Set__DATA("");

		sCH__MACRO_MOVE__VAC_RB__B_ARM_SLOT_STATUS->Set__DATA(STR__NONE);
		sCH__MACRO_MOVE__VAC_RB__B_ARM_SLOT_TITLE->Set__DATA("");
	}

	// PMC ...
	for(i=0; i<CFG_PM_LIMIT; i++)
	{
		sCH__MACRO_MOVE__PMC_SLOT_STATUS[i]->Set__DATA(STR__NONE);
		sCH__MACRO_MOVE__PMC_SLOT_TITLE[i]->Set__DATA("");
	}
}

void CObj_Opr__MAINT_MODE
::Fnc__MACRO_MOVE_EDITOR_CLEAR()
{
	int i;

	// Table ...
	for(i=0; i<CFG_SIZE__MOVE_HISTORY; i++)
	{
		sCH__MACRO_MOVE__EDITOR_ACTION_NAME[i]->Set__DATA("");
	}

	// Move ...
	{
		sCH__MACRO_MOVE__SRC_MODULE->Set__DATA("");
		sCH__MACRO_MOVE__SRC_SLOT->Set__DATA("");

		sCH__MACRO_MOVE__TRG_MODULE->Set__DATA("");
		sCH__MACRO_MOVE__TRG_SLOT->Set__DATA("");
	}

	// Loop ...
	{
		aCH__MACRO_MOVE__LOOP_START->Set__DATA("0");
		aCH__MACRO_MOVE__LOOP_END->Set__DATA("0");
		aCH__MACRO_MOVE__LOOP_REPEAT->Set__DATA("0");
	}

	sCH__MACRO_MOVE__EDITOR_CUR_INDEX->Set__DATA("");
}
void CObj_Opr__MAINT_MODE
::Fnc__MACRO_MOVE_WAFER_RESET()
{
	int i;
	int k;
	
	// PMx ...
	for(i=0; i<CFG_PM_LIMIT; i++)
	{
		sCH__MACRO_MOVE__PMC_SLOT_STATUS[i]->Set__DATA(STR__NONE);;
		sCH__MACRO_MOVE__PMC_SLOT_TITLE[i]->Set__DATA("");
	}

	// VAC_RB ...
	{
		sCH__MACRO_MOVE__VAC_RB__A_ARM_SLOT_STATUS->Set__DATA(STR__NONE);
		sCH__MACRO_MOVE__VAC_RB__A_ARM_SLOT_TITLE->Set__DATA("");
		
		sCH__MACRO_MOVE__VAC_RB__B_ARM_SLOT_STATUS->Set__DATA(STR__NONE);
		sCH__MACRO_MOVE__VAC_RB__B_ARM_SLOT_TITLE->Set__DATA("");
	}

	// ATM_RB ...
	{
		sCH__MACRO_MOVE__ATM_RB__A_ARM_SLOT_STATUS->Set__DATA(STR__NONE);
		sCH__MACRO_MOVE__ATM_RB__A_ARM_SLOT_TITLE->Set__DATA("");

		sCH__MACRO_MOVE__ATM_RB__B_ARM_SLOT_STATUS->Set__DATA(STR__NONE);
		sCH__MACRO_MOVE__ATM_RB__B_ARM_SLOT_TITLE->Set__DATA("");
	}

	// LLX ...
	{
		for(i=0; i<CFG_LLx_LIMIT; i++)
		{
			for(k=0; k<CFG_LLx__SLOT_MAX; k++)
			{
				sCH__MACRO_MOVE__LLx_SLOT_STATUS[i][k]->Set__DATA(STR__NONE);
				sCH__MACRO_MOVE__LLx_SLOT_TITLE[i][k]->Set__DATA("");
			}
		}
	}

	// AL ...
	{
		for(i=0; i<CFG_AL__SLOT_MAX; i++)
		{
			sCH__MACRO_MOVE__AL_SLOT_STATUS[i]->Set__DATA(STR__NONE);
			sCH__MACRO_MOVE__AL_SLOT_TITLE[i]->Set__DATA("");
		}
	}

	// ST1 ...
	{
		for(i=0; i<CFG_BUFFER__SLOT_MAX; i++)
		{
			sCH__MACRO_MOVE__BUFFER1_SLOT_STATUS[i]->Set__DATA(STR__NONE);
			sCH__MACRO_MOVE__BUFFER1_SLOT_TITLE[i]->Set__DATA("");
		}
	}
	// ST2 ...
	{
		for(i=0; i<CFG_BUFFER__SLOT_MAX; i++)
		{
			sCH__MACRO_MOVE__BUFFER2_SLOT_STATUS[i]->Set__DATA(STR__NONE);
			sCH__MACRO_MOVE__BUFFER2_SLOT_TITLE[i]->Set__DATA("");
		}
	}

	// LPx ...
	{
		for(i=0; i<CFG_LP_LIMIT; i++)
		{
			for(k=0; k<CFG_LP__SLOT_MAX; k++)
			{
				sCH__MACRO_MOVE__LPx_SLOT_STATUS[i][k]->Set__DATA(STR__EXIST);
			}
		}
	}

	// ...
}
