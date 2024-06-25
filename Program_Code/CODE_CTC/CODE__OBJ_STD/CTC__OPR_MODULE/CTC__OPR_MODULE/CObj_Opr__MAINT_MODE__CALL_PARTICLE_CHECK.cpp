#include "StdAfx.h"
#include "CObj_Opr__MAINT_MODE.h"
#include "CObj_Opr__MAINT_MODE__DEF.h"

#include "Macro_Function.h"


// ...
int  CObj_Opr__MAINT_MODE::Call__PARTICLE_CHECK(CII_OBJECT__VARIABLE *p_variable)
{
	DECLARE__EXT_CTRL(p_variable);	

	// ...
	CString str__src_module;
	CString str__src_slot;
	CString str__trg_info;
	CString str__trg_module;
	CString str__trg_slot;

	int prc_check__pm_index = -1;
	int prc_check__ll_index = -1;
	CString ch_data;

	//
	sCH__PARTICLE_PARA_SRC_MODULE->Get__DATA(str__src_module);
	sCH__PARTICLE_PARA_SRC_SLOT->Get__DATA(str__src_slot);

	if(str__src_module.GetLength() < 1)		return -11;
	if(str__src_slot.GetLength() < 1)		return -12;

	// ...
	bool active__atm_rb = false;
	bool active__vac_rb = false;

	dCH__PARTICLE_PARA_TRG_MODULE->Get__DATA(str__trg_info);

	// ...
	{
		bool active__trg_check = true;
		bool active__proc_sts = false;

		if(active__trg_check)
		{
			int md_index = mDB__TRG_INFO.Get__PMx_INFO(str__trg_info, str__trg_module,str__trg_slot, active__proc_sts);
			if(md_index >= 0)
			{
				active__trg_check = false;
				active__vac_rb = true;

				if(active__proc_sts)		prc_check__pm_index = md_index;
			}
		}
		if(active__trg_check)
		{
			int md_index = mDB__TRG_INFO.Get__LLx_INFO(str__trg_info, str__trg_module,str__trg_slot, active__proc_sts);
			if(md_index >= 0)
			{
				active__trg_check = false;
				active__atm_rb = true;

				if(active__proc_sts)		prc_check__ll_index = md_index;
			}
		}
		if(active__trg_check)
		{
			int md_index = mDB__TRG_INFO.Get__TARGET_INFO(str__trg_info, str__trg_module,str__trg_slot);
			if(md_index >= 0)
			{
				active__trg_check = false;
				active__atm_rb = true;
			}
		}

		// ...
		{
			printf(" * trg_info <- %s \n", str__trg_info);

			printf(" * trg_module <- %s \n", str__trg_module);
			printf(" * trg_slot <- %s \n", str__trg_slot);

			printf(" * active__proc_sts <- %s \n", active__proc_sts ? "true" : "false");

			printf(" * prc_check__pm_index <- %1d \n", prc_check__pm_index);
			printf(" * prc_check__ll_index <- %1d \n", prc_check__ll_index);

			printf(" * active__atm_rb <- %s \n", active__atm_rb ? "true" : "false");
			printf(" * active__vac_rb <- %s \n", active__vac_rb ? "true" : "false");

			printf("\n");
		}

		if(active__trg_check)
		{
			// Unknown Module : Error 
			return -13;
		}
	}

	// PM Process Check ...
	bool active__pm = false;
	bool active__ll = false;

	if((prc_check__pm_index >= 0)
	|| (prc_check__ll_index >= 0))
	{
		if(prc_check__pm_index >= 0)			active__pm = true;
		if(prc_check__ll_index >= 0)			active__ll = true;

		ch_data = dCH__PARTICLE_PARA_PRC_APPLY->Get__STRING();
		sCH__PARTICLE_DATA_PRC_APPLY->Set__DATA(ch_data);

		if(ch_data.CompareNoCase(STR__ENABLE) == 0)
		{
			// ch_data = dCH__PARTICLE_PARA_CYCLE_MODULE->Get__STRING();
			ch_data = str__trg_module;
			sCH__PARTICLE_DATA_CYCLE_MODULE->Set__DATA(ch_data);

			aCH__PARTICLE_PARA_PROC_CYCLE_COUNT->Get__DATA(ch_data);
			sCH__PARTICLE_DATA_PROC_CYCLE_COUNT->Set__DATA(ch_data);
		}
		else
		{
			sCH__PARTICLE_DATA_CYCLE_MODULE->Set__DATA("__");
			sCH__PARTICLE_DATA_PROC_CYCLE_COUNT->Set__DATA("__");
		}
	}
	else
	{
		sCH__PARTICLE_DATA_PRC_APPLY->Set__DATA("__");
		sCH__PARTICLE_DATA_CYCLE_MODULE->Set__DATA("__");
		sCH__PARTICLE_DATA_PROC_CYCLE_COUNT->Set__DATA("__");
	}

	// ...
	{
		int lp_index = Macro__Get_LP_INDEX(str__src_module);

		if((lp_index >= 0)
			&& (lp_index <  iLPx_SIZE))
		{
			sEXT_CH__LPx_LOCK_FLAG[lp_index]->Set__DATA(STR__YES);
		}
	}

	// LPx & PMx : Check ...
	{
		// ...
		{
			sEXT_CH__VAC_RB__CHECK_MODULE->Set__DATA(str__src_module);

			if(pVAC_RB__OBJ_CTRL->Call__OBJECT(VAC_CMMD__CHECK_STANDBY) < 0)
			{
				return -101;
			}
		}

		// ...
		{
			sEXT_CH__VAC_RB__CHECK_MODULE->Set__DATA(str__trg_module);

			if(pVAC_RB__OBJ_CTRL->Call__OBJECT(VAC_CMMD__CHECK_STANDBY) < 0)
			{
				return -102;
			}	
		}
	}

	// LPx -> Module ...
	{
		// ...
		{
			sCH__MOVE_SRC_MODULE->Set__DATA(str__src_module);
			sCH__MOVE_SRC_SLOT->Set__DATA(str__src_slot);

			sCH__MOVE_TRG_MODULE->Set__DATA(str__trg_module);
			sCH__MOVE_TRG_SLOT->Set__DATA(str__trg_slot);
		}

		if(Call__MANUAL_MOVE(p_variable) < 0)
		{
			return -11;
		}
	}

	// CYCLE : PICK & PLACE ...
	{
		ch_data = aCH__PARTICLE_PARA_PICK_PLACE_CYCLE_COUNT->Get__STRING();
		sCH__PARTICLE_DATA_PICK_PLACE_CYCLE_COUNT->Set__DATA(ch_data);

		int i_limit = atoi(ch_data);
		for(int i=i_limit-1; i>=0; i--)
		{
			CString arm_type;
			int r_act;

			r_act = _Fnc__PARTICLE_CHECK__WAITING(p_variable, str__trg_module,str__trg_slot, active__pm,active__ll, prc_check__pm_index,prc_check__ll_index);
			if(r_act < 0)		return -201;

			if(i < 1)
			{
				sCH__PARTICLE_DATA_PICK_PLACE_CYCLE_COUNT->Set__DATA("0");
				break;
			}

			// VAC.RB : PICK % PLACE ...
			if(active__vac_rb)
			{
				CStringArray l_para;

				r_act = pVAC_RB__OBJ_CTRL->Call__QUERY_LIST(_QRY__GET_EMPTY_ARM, l_para);
				if(r_act < 0)			return -301;

				if(l_para.GetSize() > 0)		arm_type = l_para[0];
				else							return -302;

				//
				r_act = Seq__VAC_RB__Pick(p_variable, arm_type, str__trg_module,str__trg_slot); 
				if(r_act < 0)			return -311;

				// Time Wait ...
				{
					SCX__TIMER_CTRL x_timer_ctrl;

					x_timer_ctrl->REGISTER__ABORT_OBJECT(sObject_Name);
					x_timer_ctrl->REGISTER__COUNT_CHANNEL(sCH__PARTICLE_PARA_VAC_ROBOT_CUR_TIME_COUNT->Get__CHANNEL_NAME());

					double cfg_sec = aCH__PARTICLE_PARA_VAC_ROBOT_CFG_TIME_SEC->Get__VALUE();
					if(x_timer_ctrl->WAIT(cfg_sec) < 0)			return -312;
				}

				//
				r_act = Seq__VAC_RB__Place(p_variable, arm_type, str__trg_module,str__trg_slot);
				if(r_act < 0)			return -313;
			}
			// ATM.RB : PICK % PLACE ...
			else if(active__atm_rb)
			{
				CStringArray l_para;

				r_act = pATM_RB__OBJ_CTRL->Call__QUERY_LIST(_QRY__GET_EMPTY_ARM, l_para);
				if(r_act < 0)			return -401;

				if(l_para.GetSize() > 0)		arm_type = l_para[0];
				else							return -402;

				//
				r_act = Seq__ATM_RB__Pick(p_variable, arm_type, str__trg_module,str__trg_slot); 
				if(r_act < 0)			return -411;

				// Time Wait ...
				{
					SCX__TIMER_CTRL x_timer_ctrl;

					x_timer_ctrl->REGISTER__ABORT_OBJECT(sObject_Name);
					x_timer_ctrl->REGISTER__COUNT_CHANNEL(sCH__PARTICLE_PARA_ATM_ROBOT_CUR_TIME_COUNT->Get__CHANNEL_NAME());

					double cfg_sec = aCH__PARTICLE_PARA_ATM_ROBOT_CFG_TIME_SEC->Get__VALUE();
					if(x_timer_ctrl->WAIT(cfg_sec) < 0)			return -412;
				}

				//
				r_act = Seq__ATM_RB__Place(p_variable, arm_type, str__trg_module,str__trg_slot);
				if(r_act < 0)			return -413;
			}
			else
			{
				sCH__PARTICLE_DATA_PICK_PLACE_CYCLE_COUNT->Set__DATA("___");
				break;
			}

			//
			ch_data.Format("%1d", i);
			sCH__PARTICLE_DATA_PICK_PLACE_CYCLE_COUNT->Set__DATA(ch_data);
		}
	}

	// ...
	CString atm_rb__name = "ATM_RB1_A";
	CString atm_rb__slot = "A";

	// Module -> ATM_RB ...
	{
		// ...
		{
			sCH__MOVE_SRC_MODULE->Set__DATA(str__trg_module);
			sCH__MOVE_SRC_SLOT->Set__DATA(str__trg_slot);

			sCH__MOVE_TRG_MODULE->Set__DATA(atm_rb__name);
			sCH__MOVE_TRG_SLOT->Set__DATA(atm_rb__slot);
		}

		if(Call__MANUAL_MOVE(p_variable) < 0)
		{
			return -13;
		}
	}

	// jglee ...
	CString buff__trg_module = atm_rb__name;
	CString buff__trg_slot   = atm_rb__slot;

	// ATM_RB -> BUFFx ...
	bool active__stx_check = false;

	if(active__vac_rb)
	{
		if(dEXT_CH__ATM_RB__CFG_BUFFER_USE_DURING_MAINT_MODE->Check__DATA(STR__ENABLE) > 0)
		{
			if((str__trg_module.CompareNoCase(STR__BUFF1) != 0)
			&& (str__trg_module.CompareNoCase(STR__BUFF2) != 0))
			{
				active__stx_check = true;
			}
		}
	}

	if(active__stx_check)
	{
		CString query_name = "GET_EMPTY_SLOT_OF_BUUFER";
		CStringArray l_sub_query;
		CStringArray l_data;

		if(pATM_RB__OBJ_CTRL->Call__QUERY_LIST(query_name,l_sub_query, l_data) > 0)
		{
			if(l_data.GetSize() >= 2)
			{
				int limit = l_data.GetSize();

				for(int i=0; i<limit; i++)
				{
					CString ch_data = l_data[i];

					switch(i)
					{
					case 0:		buff__trg_module = ch_data;		break;
					case 1:		buff__trg_slot   = ch_data;		break;
					}
				}
			}
			else
			{
				return -141;
			}
		}
		else
		{
			return -14;
		}

		// ...
		{
			sCH__MOVE_SRC_MODULE->Set__DATA(atm_rb__name);
			sCH__MOVE_SRC_SLOT->Set__DATA(atm_rb__slot);

			sCH__MOVE_TRG_MODULE->Set__DATA(buff__trg_module);
			sCH__MOVE_TRG_SLOT->Set__DATA(buff__trg_slot);
		}

		if(Call__MANUAL_MOVE(p_variable) < 0)
		{
			return -15;
		}

		// ...
		{
			CString query_name = "CHECK_BUSY_SLOT_OF_BUUFER";
			CStringArray l_sub_query;
			CStringArray l_data;

			l_sub_query.RemoveAll();
			l_sub_query.Add(buff__trg_module);
			l_sub_query.Add(buff__trg_slot);

			while(1)
			{
				Sleep(900);

				if(pATM_RB__OBJ_CTRL->Call__QUERY_LIST(query_name,l_sub_query, l_data) > 0)
				{
					if(l_data.GetSize() > 0)
					{
						CString str_state = l_data[0];

						if(str_state.CompareNoCase("NO") == 0)
						{
							break;
						}
					}
					else
					{
						-151;
					}
				}
				else
				{
					return -152;
				}
			}
		}
	}

	// ATM_RB or BUFFx -> LPx ...
	{
		// ...
		{
			sCH__MOVE_SRC_MODULE->Set__DATA(buff__trg_module);
			sCH__MOVE_SRC_SLOT->Set__DATA(buff__trg_slot);

			sCH__MOVE_TRG_MODULE->Set__DATA(str__src_module);
			sCH__MOVE_TRG_SLOT->Set__DATA(str__src_slot);
		}

		if(Call__MANUAL_MOVE(p_variable) < 0)
		{
			return -21;
		}
	}

	return 1;
}
int  CObj_Opr__MAINT_MODE::
_Fnc__PARTICLE_CHECK__WAITING(CII_OBJECT__VARIABLE *p_variable, 
							  const CString& para__trg_module,
							  const CString& para__trg_slot,
							  const bool active__pm,
							  const bool active__ll,
							  const int prc_check__pm_index,
							  const int prc_check__ll_index)
{
	// Process Test ...
	if(sCH__PARTICLE_DATA_PRC_APPLY->Check__DATA(STR__ENABLE) > 0)
	{
		CString rcp_name__main = "";
		CString rcp_name__cln  = "";
		CString rcp_name__pre  = "";
		CString str__rcp_name  = "";

		// Main.Process ...
		{
				 if(active__pm)		str__rcp_name = sCH__PARTICLE_PARA_PMx_RECIPE[prc_check__pm_index]->Get__STRING();
			else if(active__ll)		str__rcp_name = sCH__PARTICLE_PARA_LLx_RECIPE[prc_check__ll_index]->Get__STRING();

			if(str__rcp_name.GetLength() > 0)
			{
				if(str__rcp_name.Find("\\") > 0)		rcp_name__main = str__rcp_name;
				else									rcp_name__main.Format("PRC\\%s", str__rcp_name);
			}
		}
		// Post.Process ...
		{
				 if(active__pm)		str__rcp_name = sCH__PARTICLE_PARA_PMx_RCP_CLN[prc_check__pm_index]->Get__STRING();
			else if(active__ll)		str__rcp_name = sCH__PARTICLE_PARA_LLx_RCP_CLN[prc_check__ll_index]->Get__STRING();

			if(str__rcp_name.GetLength() > 0)
			{
				if(str__rcp_name.Find("\\") > 0)		rcp_name__cln = str__rcp_name;
				else									rcp_name__cln.Format("CLN\\%s", str__rcp_name);
			}
		}
		// Pre.Recipe ...
		{
				 if(active__pm)		str__rcp_name = sCH__PARTICLE_PARA_PMx_RCP_PRE[prc_check__pm_index]->Get__STRING();
			else if(active__ll)		str__rcp_name = sCH__PARTICLE_PARA_LLx_RCP_PRE[prc_check__ll_index]->Get__STRING();

			if(str__rcp_name.GetLength() > 0)
			{
				if(str__rcp_name.Find("\\") > 0)		rcp_name__pre = str__rcp_name;
				else									rcp_name__pre.Format("PRE\\%s", str__rcp_name);
			}
		}

		CString cycle_module_name = sCH__PARTICLE_DATA_CYCLE_MODULE->Get__STRING();
		CString cycle_module_slot = "1";

		CString ch_data = sCH__PARTICLE_DATA_PROC_CYCLE_COUNT->Get__STRING();
		int cycle_count = atoi(ch_data);
		int i;

		for(cycle_count--; cycle_count >= 0; cycle_count--)
		{
			ch_data.Format("%1d", cycle_count);
			sCH__PARTICLE_DATA_PROC_CYCLE_COUNT->Set__DATA(ch_data);

			while(1)
			{
				Sleep(90);

				if(p_variable->Check__CTRL_ABORT() > 0)
				{
					return -111;
				}

				//
				sEXT_CH__VAC_RB__CHECK_MODULE->Set__DATA(para__trg_module);

				if(pVAC_RB__OBJ_CTRL->Call__OBJECT(VAC_CMMD__CHECK_STANDBY) > 0)
				{
					break;
				}
			}

			// PMx Process
			if((active__pm) && (prc_check__pm_index < iPMx_SIZE))
			{
				if(rcp_name__main.GetLength() > 0)
				{
					int db_index = prc_check__pm_index;

					CII_EXT_OBJECT__CTRL *p_obj_ctrl = pPMx__OBJ_CTRL[db_index];
					if(p_obj_ctrl == NULL)		return -21;

					// ...
					CStringArray l_para;
					l_para.RemoveAll();

					l_para.Add(rcp_name__main);
					l_para.Add("");
					l_para.Add("");

					p_obj_ctrl->Run__OBJ_MODE("AUTO_MNT", l_para);

					while(1)
					{
						Sleep(500);

						if(p_variable->Check__CTRL_ABORT() > 0)
						{
							return -1011;
						}

						p_obj_ctrl->Get__OBJECT_STATUS(ch_data);
						if((ch_data.CompareNoCase(STR__ABORTED)   == 0)
							|| (ch_data.CompareNoCase(STR__AVAILABLE) == 0))
						{
							break;
						}
					}
				}
			}
			// LLx Process
			else if((active__ll) && (prc_check__ll_index < iLLx_SIZE))
			{
				if(rcp_name__main.GetLength() > 0)
				{
					int db_index = prc_check__ll_index;

					CII_EXT_OBJECT__CTRL *p_obj_ctrl = pLLx__OBJ_CTRL[db_index];
					if(p_obj_ctrl == NULL)		return -21;

					sEXT_CH__LLx_PARA_PROC_RECIPE_NAME[db_index]->Set__DATA(rcp_name__main);
					sEXT_CH__LLx_PARA_PROC_SCH_NAME[db_index]->Set__DATA("");

					p_obj_ctrl->Run__OBJECT("PROC_MAINT");

					while(1)
					{
						Sleep(500);

						if(p_variable->Check__CTRL_ABORT() > 0)
						{
							return -1021;
						}

						p_obj_ctrl->Get__OBJECT_STATUS(ch_data);
						if((ch_data.CompareNoCase(STR__ABORTED)   == 0)
							|| (ch_data.CompareNoCase(STR__AVAILABLE) == 0))
						{
							break;
						}
					}
				}
			}
			else
			{
				return -1012;
			}

			while(1)
			{
				Sleep(90);

				if(p_variable->Check__CTRL_ABORT() > 0)
				{
					return -112;
				}

				//
				sEXT_CH__VAC_RB__CHECK_MODULE->Set__DATA(para__trg_module);

				if(pVAC_RB__OBJ_CTRL->Call__OBJECT(VAC_CMMD__CHECK_STANDBY) > 0)
				{
					break;
				}
			}

			// ...
			CString vac_rb__module = "";
			CString vac_rb__slot   = "";

			// PMx -> VAC_RB
			{
				CString arm_type = "X";

				if(VAC_RB__Pick_Module_With_Arm(p_variable, arm_type, para__trg_module,para__trg_slot) < 0)
				{
					return -21;
				}

				vac_rb__module.Format("VAC_RB1_%s", arm_type);
				vac_rb__slot = arm_type;
			}

			// PMx : Post_Process
			if((active__pm) && (prc_check__pm_index < iPMx_SIZE))
			{
				if(rcp_name__cln.GetLength() > 0)
				{
					CStringArray l_para;
					l_para.RemoveAll();

					l_para.Add(rcp_name__cln);
					l_para.Add("");
					l_para.Add("");

					if(pPMx__OBJ_CTRL[prc_check__pm_index]->Run__OBJ_MODE("AUTO_MNT", l_para) < 0)
					{
						return -1021;
					}
				}
			}
			// LLx : Post_Process
			else if((active__ll) && (prc_check__ll_index < iLLx_SIZE))
			{
				if(rcp_name__cln.GetLength() > 0)
				{
					int db_index = prc_check__ll_index;

					CII_EXT_OBJECT__CTRL *p_obj_ctrl = pLLx__OBJ_CTRL[db_index];
					if(p_obj_ctrl == NULL)		return -21;

					sEXT_CH__LLx_PARA_PROC_RECIPE_NAME[db_index]->Set__DATA(rcp_name__cln);
					sEXT_CH__LLx_PARA_PROC_SCH_NAME[db_index]->Set__DATA("");

					p_obj_ctrl->Run__OBJECT("PROC_MAINT");

					while(1)
					{
						Sleep(500);

						if(p_variable->Check__CTRL_ABORT() > 0)
						{
							return -1022;
						}

						p_obj_ctrl->Get__OBJECT_STATUS(ch_data);
						if((ch_data.CompareNoCase(STR__ABORTED)   == 0)
						|| (ch_data.CompareNoCase(STR__AVAILABLE) == 0))
						{
							break;
						}
					}
				}
			}

			// ...
			CString str__trg_module = para__trg_module;
			CString str__trg_slot   = para__trg_slot;

			if(cycle_count < 1)
			{
				str__trg_module = vac_rb__module;
				str__trg_slot   = vac_rb__slot;
				break;
			}

			// Time Wait ...
			{
				SCX__TIMER_CTRL x_timer_ctrl;

				x_timer_ctrl->REGISTER__ABORT_OBJECT(sObject_Name);
				x_timer_ctrl->REGISTER__COUNT_CHANNEL(sCH__PARTICLE_PARA_VAC_ROBOT_CUR_TIME_COUNT->Get__CHANNEL_NAME());

				double cfg_sec = aCH__PARTICLE_PARA_VAC_ROBOT_CFG_TIME_SEC->Get__VALUE();

				if(x_timer_ctrl->WAIT(cfg_sec) < 0)
				{
					return -2001;
				}
			}

			while(1)
			{
				Sleep(90);

				if(p_variable->Check__CTRL_ABORT() > 0)
				{
					return -122;
				}

				//
				sEXT_CH__VAC_RB__CHECK_MODULE->Set__DATA(str__trg_module);

				if(pVAC_RB__OBJ_CTRL->Call__OBJECT(VAC_CMMD__CHECK_STANDBY) > 0)
				{
					break;
				}
			}

			// VAC_RB -> PMx 
			{
				// ...
				{
					sCH__MOVE_SRC_MODULE->Set__DATA(vac_rb__module);
					sCH__MOVE_SRC_SLOT->Set__DATA(vac_rb__slot);

					sCH__MOVE_TRG_MODULE->Set__DATA(str__trg_module);
					sCH__MOVE_TRG_SLOT->Set__DATA(str__trg_slot);
				}

				if(Call__MANUAL_MOVE(p_variable) < 0)
				{
					return -22;
				}
			}

			while(1)
			{
				Sleep(90);

				if(p_variable->Check__CTRL_ABORT() > 0)
				{
					return -131;
				}

				//
				sEXT_CH__VAC_RB__CHECK_MODULE->Set__DATA(str__trg_module);

				if(pVAC_RB__OBJ_CTRL->Call__OBJECT(VAC_CMMD__CHECK_STANDBY) > 0)
				{
					break;
				}
			}
		}

		ch_data.Format("%1d", cycle_count);
		sCH__PARTICLE_DATA_PROC_CYCLE_COUNT->Set__DATA(ch_data);
	}
	// Delay Time .....
	else
	{
		SCX__TIMER_CTRL x_timer_ctrl;
		CString var_data;
		double delay_sec;

		aCH__PARTICLE_PARA_DELAY_TIME->Get__DATA(var_data);
		delay_sec = atof(var_data);

		x_timer_ctrl->REGISTER__ABORT_OBJECT(sObject_Name);
		x_timer_ctrl->REGISTER__COUNT_CHANNEL(aCH__PARTICLE_PARA_DELAY_COUNT->Get__CHANNEL_NAME());

		if(x_timer_ctrl->WAIT(delay_sec) < 0)
		{
			return -12;
		}
	}

	return 1;
}

