#include "StdAfx.h"
#include "CObj__DUAL_ARM_STD.h"

#include "Macro_Function.h"


// ...
int  CObj__DUAL_ARM_STD
::Call__CLEAR(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm)
{
	TOOL_CLEAR__Check_System_Module_Error(p_alarm);

	if(ATM_RB__Is_Available() < 0)
	{
		return -1;
	}

	// ...
	CStringArray l_para;

	CString arm_type;
	CString para_module;
	CString para_slot;

	CString wfr_title;
	CString wfr_state;
	CString str_ptn;
	CString str_slot;
	int slot_id;

	// ATM_RB -> LPx
	if(ATM_RB__Get_Occupied__Arm_Type(arm_type) > 0)
	{
		if(ATM_RB__Get_Occupied__Wfr_Title_Of_Arm_Type(arm_type, wfr_title) > 0)
		{
			if(Macro__Analyze_Title(wfr_title,str_ptn,str_slot) < 0)
			{
				CString wfr_pos;
				wfr_pos.Format("ATM_Robot's %s-Arm", arm_type);

				Alarm_Post__ToolClear__Wafer_Title(p_alarm,	wfr_pos, wfr_title);
				return -1;
			}

			if(Seq__MAINT_PLACE(p_variable,p_alarm,arm_type,str_ptn,str_slot) < 0)
			{
				Alarm_Post__ToolClear__Place_Error(p_alarm,	arm_type, str_ptn, str_slot);
				return -1;
			}
		}
	}

	// AL -> LPx
	if(AL1__Check_CtcInUse_Or_Standby() > 0)
	{
		int al_slot_id;
		int i;

		for(i=0;i<2;i++)
		{
			al_slot_id = i + 1;
			if(i >= 2)		continue;

			if(AL1__Check_Occupied__SlotStatus(al_slot_id,wfr_title) > 0)
			{
				if(Macro__Analyze_Title(wfr_title,str_ptn,str_slot) < 0)
				{
					Alarm_Post__ToolClear__Wafer_Title(p_alarm,"AL1",wfr_title);
					return -1;
				}

				if(ATM_RB__Get_Empty__Arm_Type(arm_type) > 0)
				{
					para_module = "AL1";
					para_slot.Format("%1d",al_slot_id);

					if(Seq__MAINT_PICK(p_variable,p_alarm,arm_type,para_module,para_slot) < 0)
					{
						Alarm_Post__ToolClear__Pick_Error(p_alarm,arm_type,para_module,para_slot);
						return -1;
					}

					if(Seq__MAINT_PLACE(p_variable,p_alarm,arm_type,str_ptn,str_slot) < 0)
					{
						Alarm_Post__ToolClear__Place_Error(p_alarm,arm_type,str_ptn,str_slot);
						return -1;
					}
				}
			}
		}
	}

	// Side Buffer (1) -> LPx
	if(sCH__SCH_DB_ST1_USE_FLAG->Check__DATA(STR__ENABLE) > 0)
	{
		if(Buffer1__Get_Occupied__Slot(slot_id,wfr_title) > 0)
		{
			int slot_index = slot_id - 1;
			int cfg_max = (int) xEXT_CH__BUFFER1__CFG_SLOT_MAX->Get__VALUE();

			if((slot_index >= 0) && (slot_index < cfg_max))
			{
				double cur_sec = xEXT_CH__BUFFER1__SLOT_WAIT_SEC[slot_index]->Get__VALUE();

				if(cur_sec < 0.1)
				{
					if(Macro__Analyze_Title(wfr_title,str_ptn,str_slot) < 0)
					{
						Alarm_Post__ToolClear__Wafer_Title(p_alarm,MODULE__BUFFER1,wfr_title);
						return -1;
					}

					if(ATM_RB__Get_Empty__Arm_Type(arm_type) > 0)
					{
						para_module = MODULE__BUFFER1;
						para_slot.Format("%1d",slot_id);

						if(Seq__MAINT_PICK(p_variable,p_alarm,arm_type,para_module,para_slot) < 0)
						{
							Alarm_Post__ToolClear__Pick_Error(p_alarm,arm_type,para_module,para_slot);
							return -1;
						}
						if(Seq__MAINT_PLACE(p_variable,p_alarm,arm_type,str_ptn,str_slot) < 0)
						{
							Alarm_Post__ToolClear__Place_Error(p_alarm,arm_type,str_ptn,str_slot);
							return -1;		
						}
					}
				}
			}
		}
	}
	// Side Buffer (2) -> LPx
	if(sCH__SCH_DB_ST2_USE_FLAG->Check__DATA(STR__ENABLE) > 0)
	{
		if(Buffer2__Get_Occupied__Slot(slot_id,wfr_title) > 0)
		{
			int slot_index = slot_id - 1;
			int cfg_max = (int) xEXT_CH__BUFFER1__CFG_SLOT_MAX->Get__VALUE();

			if((slot_index >= 0) && (slot_index < cfg_max))
			{
				double cur_sec = xEXT_CH__BUFFER1__SLOT_WAIT_SEC[slot_index]->Get__VALUE();

				if(cur_sec < 0.1)
				{
					if(Macro__Analyze_Title(wfr_title,str_ptn,str_slot) < 0)
					{
						Alarm_Post__ToolClear__Wafer_Title(p_alarm,MODULE__BUFFER2,wfr_title);
						return -1;
					}

					if(ATM_RB__Get_Empty__Arm_Type(arm_type) > 0)
					{
						para_module = MODULE__BUFFER2;
						para_slot.Format("%1d",slot_id);

						if(Seq__MAINT_PICK(p_variable,p_alarm,arm_type,para_module,para_slot) < 0)
						{
							Alarm_Post__ToolClear__Pick_Error(p_alarm,arm_type,para_module,para_slot);
							return -1;
						}
						if(Seq__MAINT_PLACE(p_variable,p_alarm,arm_type,str_ptn,str_slot) < 0)
						{
							Alarm_Post__ToolClear__Place_Error(p_alarm,arm_type,str_ptn,str_slot);
							return -1;		
						}
					}
				}
			}
		}
	}

	// LLx -> STx ...
	bool active__stx_check = false;
	
	bool active__wafer_all = false;
	bool active__only_processed = false;

	if(dCH__CFG_BUFFER_USE_DURING_MAINT_MODE->Check__DATA(STR__ENABLE) > 0)
	{
		if(dEXT_CH__BUFFER1__CFG_USE_FLAG->Check__DATA(STR__ENABLE) > 0)			active__stx_check = true;
		if(dEXT_CH__BUFFER2__CFG_USE_FLAG->Check__DATA(STR__ENABLE) > 0)			active__stx_check = true;

		if(dCH__CFG_WAFER_TO_STx_DURING_MAINT_MODE->Check__DATA(STR__ONLY_PROCESSED) > 0)
		{
			active__only_processed = true;
		}
	}
	else
	{
		active__wafer_all = true;
	}

	if(active__stx_check)
	{
		int i_limit = iLLx_SIZE;
		int ll_i;

		for(ll_i=0; ll_i<i_limit; ll_i++)
		{
			if(xEXT_CH__LLx__CFG_USE_FLAG[ll_i]->Check__DATA(STR__ENABLE) < 0)
			{
				continue;
			}

			if(ATM_RB__Get_Empty__Arm_Type(arm_type) < 0)
			{
				continue;;
			}

			if(LLx__Check_CtcInUse_Or_Standby(ll_i) < 0)
			{
				continue;
			}
			if(LLx__Get_Occupied__Slot(ll_i, slot_id, wfr_title,wfr_state) < 0)
			{
				continue;
			}

			if(active__only_processed)
			{
				if((wfr_state.CompareNoCase(STR__MAPPED) == 0)
				|| (wfr_state.CompareNoCase(STR__EXIST) == 0))
				{
					continue;
				}
			}

			// ...
			{
				para_module = Get__LLx_NAME(ll_i);
				para_slot.Format("%1d", slot_id);

				// ...
				CString bff_module = "";
				CString bff_slot   = "";

				if(bff_module == "")
				{
					if(dEXT_CH__BUFFER1__CFG_USE_FLAG->Check__DATA(STR__ENABLE) > 0)
					{
						if(Buffer1__Get_Empty__Slot_To_Process(slot_id) > 0)
						{
							bff_module = MODULE__BUFFER1;
							bff_slot.Format("%1d", slot_id);
						}
					}
				}
				else if(bff_module == "")
				{
					if(dEXT_CH__BUFFER2__CFG_USE_FLAG->Check__DATA(STR__ENABLE) > 0)
					{
						if(Buffer2__Get_Empty__Slot_To_Process(slot_id) > 0)
						{
							bff_module = MODULE__BUFFER2;
							bff_slot.Format("%1d", slot_id);
						}
					}
				}

				if(bff_module.GetLength() > 0)
				{
					if(Seq__MAINT_PICK(p_variable,p_alarm, arm_type, para_module,para_slot) < 0)
					{
						Alarm_Post__ToolClear__Pick_Error(p_alarm,arm_type,para_module,para_slot);
						return -1;
					}

					if(Seq__MAINT_PLACE(p_variable,p_alarm, arm_type, bff_module,bff_slot) < 0)
					{
						Alarm_Post__ToolClear__Place_Error(p_alarm, arm_type, bff_module,bff_slot);
						return -1;		
					}
				}
			}

			// ...
		}
	}

	if((active__wafer_all)
	|| (active__only_processed))
	{
		// LLx -> LPx ...
		int i_limit = iLLx_SIZE;
		int ll_i;

		for(ll_i=0; ll_i<i_limit; ll_i++)
		{
			if(xEXT_CH__LLx__CFG_USE_FLAG[ll_i]->Check__DATA(STR__ENABLE) < 0)
			{
				continue;
			}

			if(ATM_RB__Get_Empty__Arm_Type(arm_type) < 0)
			{
				continue;
			}

			if(LLx__Check_CtcInUse_Or_Standby(ll_i) < 0)
			{
				continue;
			}
			if(LLx__Get_Occupied__Slot(ll_i, slot_id, wfr_title,wfr_state) < 0)
			{
				continue;
			}

			if(active__only_processed)
			{
				if((wfr_state.CompareNoCase(STR__MAPPED) != 0)
				&& (wfr_state.CompareNoCase(STR__EXIST)  != 0))
				{
					continue;
				}
			}

			// ...
			{
				para_module = Get__LLx_NAME(ll_i);

				if(Macro__Analyze_Title(wfr_title, str_ptn,str_slot) < 0)
				{
					Alarm_Post__ToolClear__Wafer_Title(p_alarm, para_module,wfr_title);
					return -1;
				}

				para_slot.Format("%1d", slot_id);

				if(Seq__MAINT_PICK(p_variable,p_alarm,arm_type,para_module,para_slot) < 0)
				{
					Alarm_Post__ToolClear__Pick_Error(p_alarm,arm_type,para_module,para_slot);
					return -1;
				}
				if(Seq__MAINT_PLACE(p_variable,p_alarm,arm_type,str_ptn,str_slot) < 0)
				{
					Alarm_Post__ToolClear__Place_Error(p_alarm,arm_type,str_ptn,str_slot);
					return -1;		
				}
			}

			// ...
		}
	}

	return 1;
}


int  CObj__DUAL_ARM_STD
::Alarm_Post__ToolClear__Wafer_Title(CII_OBJECT__ALARM *p_alarm,
									 const CString& module_name,
									 const CString& wfr_title)
{
	int alarm_id = ALID__TOOL_CLEAR__ACTION_ERROR;
	CString alarm_msg;
	CString alarm_bff;
	CString r_act;

	alarm_msg.Format("Wafer Title Error On %s\n",module_name);

	alarm_bff.Format("Wafer Title : %s\n",wfr_title);
	alarm_msg += alarm_bff;

	p_alarm->Check__ALARM(alarm_id,r_act);
	p_alarm->Post__ALARM_With_MESSAGE(alarm_id,alarm_msg);
	return 1;
}
int  CObj__DUAL_ARM_STD
::Alarm_Post__ToolClear__Place_Error(CII_OBJECT__ALARM *p_alarm,
									 const CString& module_name,
									 const CString& para_module,
									 const CString& para_slot)
{
	int alarm_id = ALID__TOOL_CLEAR__ACTION_ERROR;
	CString alarm_msg;
	CString alarm_bff;
	CString r_act;

	alarm_msg = "Place Sequence Error\n";

	alarm_bff.Format("%s -> %s's %s\n",module_name,para_module,para_slot);
	alarm_msg += alarm_bff;

	p_alarm->Check__ALARM(alarm_id,r_act);
	p_alarm->Post__ALARM_With_MESSAGE(alarm_id,alarm_msg);
	return 1;
}
int  CObj__DUAL_ARM_STD
::Alarm_Post__ToolClear__Pick_Error(CII_OBJECT__ALARM *p_alarm,
									const CString& module_name,
									const CString& para_module,
									const CString& para_slot)
{
	int alarm_id = ALID__TOOL_CLEAR__ACTION_ERROR;
	CString alarm_msg;
	CString alarm_bff;
	CString r_act;

	alarm_msg = "Pick Sequence Error\n";

	alarm_bff.Format("%s <- %s's %s\n",module_name,para_module,para_slot);
	alarm_msg += alarm_bff;

	p_alarm->Check__ALARM(alarm_id,r_act);
	p_alarm->Post__ALARM_With_MESSAGE(alarm_id,alarm_msg);
	return 1;
}
