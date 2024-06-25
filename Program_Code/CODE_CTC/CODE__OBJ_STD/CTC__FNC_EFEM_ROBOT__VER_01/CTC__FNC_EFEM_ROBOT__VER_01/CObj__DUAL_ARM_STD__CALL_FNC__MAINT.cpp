#include "StdAfx.h"
#include "CObj__DUAL_ARM_STD.h"

#include "Macro_Function.h"


// ...
int  CObj__DUAL_ARM_STD
::Call__MAINT_PICK(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm)
{
	DECLARE__EXT_CTRL(p_variable);

	// ...
	CString para_arm;
	CString para_module;
	CString para_slot;

	CStringArray l_para;
	CString para_data;

	int limit;
	int i;

	p_ext_mode_ctrl->Get__FNC_PARAMETER(l_para);
	limit = l_para.GetSize();

	for(i=0;i<limit;i++)
	{
		para_data = l_para[i];

		switch(i)
		{
			case 0:		para_arm    = para_data;		break;
			case 1:		para_module = para_data;		break;
			case 2:		para_slot   = para_data;		break;
		}
	}

	if(MANUAL_MOVE__Check_System_Module_Error(1,
												para_arm,
												para_module,
												para_slot,
												p_alarm) < 0)
	{
		return -1;
	}

	return Seq__MAINT_PICK(p_variable,
							p_alarm,
							para_arm,
							para_module,
							para_slot);
}
int  CObj__DUAL_ARM_STD
::Seq__MAINT_PICK(CII_OBJECT__VARIABLE *p_variable,
				  CII_OBJECT__ALARM *p_alarm,
				  const CString& para_arm,
				  const CString& para_module,
				  const CString& para_slot)
{
	DECLARE__EXT_CTRL(p_variable);

	// ...
LOOP_RETRY:

	if(xCH__ATM_RB_PAUSE_FLAG->Check__DATA(STR__YES) > 0)
	{
		int alarm_id = ALID__MAINT_PICK_ERROR__ATM_RB_PAUSED;
		CString alm_msg;
		CString alm_bff;
		CString r_act;

		// ...
		{
			alm_bff.Format("Material Move :  %s(%s) -> Arm(%s) \n", 
				para_module, para_slot, para_arm);
			alm_msg += alm_bff;
		}

		p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,alm_msg,r_act);
		if(r_act.CompareNoCase(STR__RETRY) == 0)
		{
			goto LOOP_RETRY;
		}
		return -1;
	}

	if(para_module.CompareNoCase(MODULE__AL1) == 0)
	{
		if(xCH__AL1__ALIGN_RETRY_FLAG->Check__DATA(STR__YES) > 0)
		{
			try
			{
				bool active__ll_all_mode = true;

				if(CFG__LBx__CHECK_ONLY_INPUNT() > 0)
				{
					active__ll_all_mode = true;
				}

				if(AUTO_CTRL__Retry_ALIGN(p_variable,p_alarm, active__ll_all_mode) < 0)
				{
					return -1;
				}
			}
			catch(const int err_flag)
			{
				if(AL1__Check_CtcInUse_Or_Standby() < 0)
				{
					return -1;
				}
			}

			while(AL1__Is_Available() < 0)
			{
				Sleep(490);

				if(AL1__Check_CtcInUse_Or_Standby() < 0)
				{
					return -1;
				}

				if(p_variable->Check__CTRL_ABORT() > 0)
				{
					return -1;
				}
			}
		}
	}

	if(p_variable->Check__CTRL_ABORT() > 0)
	{
		return -1;
	}

	// ...
	bool ex_flag = false;
	CStringArray l_para;
	CString arm_type;

	if((para_arm.CompareNoCase(ARM_A) == 0)
	|| (para_arm.CompareNoCase(ARM_B) == 0))
	{
		arm_type = para_arm;
	}
	else if(para_arm == "X")
	{
		if(ATM_RB__Get_Empty__Arm_Type(arm_type) < 0)
		{
			// Error

			return -1;
		}
	}
	else
	{
		// Error

		return -1;
	}

	if(SCH__CHECK_PICK_MODULE__READY(p_variable,
									 ex_flag,
									 para_module,
									 para_slot) < 0)
	{
		return -1;
	}

	if(para_module.CompareNoCase(MODULE__AL1) == 0)
	{
		Update__MAINT_TARGET_ALIGN(p_alarm, "", "");
	}

	if(SCH__PICK_FROM(false,
					  p_variable,
					  p_alarm,
					  ex_flag,
				  	  arm_type,
					  para_module,
					  para_slot) < 0)
	{
		return -1;
	}

	if(SCH__CHECK_PICK_MODULE__COMPLETE(p_variable,
										ex_flag,
										para_module,
										para_slot) < 0)
	{
		return -1;
	}

	// ...
	{
		l_para.RemoveAll();
		l_para.Add(arm_type);

		p_ext_mode_ctrl->Set__RETURN_FNC_PARAMETER(l_para);
	}
	return 1;
}

int  CObj__DUAL_ARM_STD
::Call__MAINT_PLACE(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm)
{
	DECLARE__EXT_CTRL(p_variable);

	// ...
	CString para_arm;
	CString para_module;
	CString para_slot;

	CStringArray l_para;
	CString para_data;

	int limit;
	int i;

	p_ext_mode_ctrl->Get__FNC_PARAMETER(l_para);
	limit = l_para.GetSize();

	for(i=0;i<limit;i++)
	{
		para_data = l_para[i];

		switch(i)
		{
			case 0:		para_arm    = para_data;		break;
			case 1:		para_module = para_data;		break;
			case 2:		para_slot   = para_data;		break;
		}
	}

	if(MANUAL_MOVE__Check_System_Module_Error(-1,
											  para_arm,
											  para_module,
											  para_slot,
											  p_alarm) < 0)
	{
		return -1;
	}

	// Init : Align Parameter ...
	{
		xCH__ATM_RB__TARGET_LLx_NAME_SET_OF_ARM_A->Set__DATA("");
		xCH__ATM_RB__TARGET_LLx_SLOT_SET_OF_ARM_A->Set__DATA("");
		
		xCH__ATM_RB__TARGET_LLx_NAME_SET_OF_ARM_B->Set__DATA("");
		xCH__ATM_RB__TARGET_LLx_SLOT_SET_OF_ARM_B->Set__DATA("");
	}

	if(para_module.CompareNoCase(MODULE__AL1) == 0)
	{
		xCH__AL1__ALIGN_RETRY_FLAG->Set__DATA("");
	}

	return Seq__MAINT_PLACE(p_variable,p_alarm,para_arm,para_module,para_slot);
}
int  CObj__DUAL_ARM_STD
::Seq__MAINT_PLACE(CII_OBJECT__VARIABLE *p_variable,
				   CII_OBJECT__ALARM *p_alarm,
				   const CString& para_arm,
				   const CString& str_module,
				   const CString& str_slot)
{
	DECLARE__EXT_CTRL(p_variable);

	CString log_id = "Seq__MAINT_PLACE()";
	CString ch_data;

	// ...
LOOP_RETRY:

	CString para_module = str_module;
	CString para_slot   = str_slot;

	if(xCH__ATM_RB_PAUSE_FLAG->Check__DATA(STR__YES) > 0)
	{
		int alarm_id = ALID__MAINT_PLACE_ERROR__ATM_RB_PAUSED;
		CString alm_msg;
		CString alm_bff;
		CString r_act;

		// ...
		{
			alm_bff.Format("Material Move :  Arm(%s) -> %s(%s) \n", 
				para_arm, para_module, para_slot);
			alm_msg += alm_bff;
		}

		p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,alm_msg,r_act);
		if(r_act.CompareNoCase(STR__RETRY) == 0)
		{
			goto LOOP_RETRY;
		}
		return -1;
	}

	// ...
	CStringArray l_para;
	CString para_data;

	bool ex_flag = false;
	CString arm_type;

	if((para_arm.CompareNoCase(ARM_A) == 0)
	|| (para_arm.CompareNoCase(ARM_B) == 0))
	{
		arm_type = para_arm;
	}
	else
	{
		// Error

		return -1;
	}

	if(para_module.CompareNoCase("LBI") == 0)
	{
		int in_slot = -1;

		int ll_limit = iLLx_SIZE;
		int ll_i;

		for(ll_i=0; ll_i<ll_limit; ll_i++)
		{
			if(in_slot < 0)
			{
				if((LLx__Is_Available(ll_i) > 0)
				&& (LLx__Is_ATM(ll_i) > 0))
				{
					if(LLx__Get_Empty__InSlot(ll_i, in_slot) > 0)
					{
						para_module = Get__LLx_NAME(ll_i);
					}
				}
			}
		}

		for(ll_i=0; ll_i<ll_limit; ll_i++)
		{
			if(in_slot < 0)
			{
				if(LLx__Is_Available(ll_i) > 0)
				{
					if(LLx__Get_Empty__InSlot(ll_i, in_slot) > 0)
					{
						para_module = Get__LLx_NAME(ll_i);
					}
				}
			}
		}

		if(in_slot < 0)
		{
			// Error
			return -1;
		}

		para_slot.Format("%1d",in_slot);
	}

	// ...
	{
		CString log_msg;
		CString log_bff;

		log_msg = "Seq__MAINT_PLACE() : Started ! \n";

		log_bff.Format(" * para_arm <- %s \n", para_arm);
		log_msg += log_bff;
		log_bff.Format(" * str_module <- %s \n", str_module);
		log_msg += log_bff;
		log_bff.Format(" * str_slot <- %s \n", str_slot);
		log_msg += log_bff;

		xAPP_LOG_CTRL->WRITE__LOG(log_msg);
	}

	if(SCH__CHECK_PLACE_MODULE__READY(p_variable,
									  ex_flag,
									  para_module,
									  para_slot) < 0)
	{
		return -1;
	}

	if(Get__LLx_INDEX(para_module) >= 0)
	{
		bool active__ll_align = false;

		CString para_skip = sCH__PARA_MANUAL_MOVE_ALIGN_SKIP->Get__STRING();
		if(para_skip.Find(STR__YES) < 0)
		{		
			if(Update__MAINT_TARGET_ALIGN(p_alarm, para_module,para_slot) > 0)
			{
				active__ll_align = true;
			}
		}

		if(active__ll_align)
		{
			int cur_count = (int) aCH__ATM_RB__CFG_ALIGN_RETRY->Get__VALUE();

			ch_data.Format("%1d", cur_count);
			sCH__ATM_RB__CUR_ALIGN_RETRY->Set__DATA(ch_data);

			// Align_Retry ...
			while(cur_count > 0)
			{
				cur_count--;

				// ...
				if(AL1__Is_Available() < 0)					return -1;
				if(AL1__Check_Empty__All_Slot() < 0)		return -1;

				// ...
				CString sch_module;
				CString al_name = "AL1";
				CString al_slot = "1";

				sch_module.Format("%s-%s", al_name,al_slot);

				// Place ...
				if(SCH__PLACE_TO(false,
								 p_variable,
								 p_alarm,
								 false,
								 arm_type,
								 al_name,
								 al_slot) < 0)
				{
					return -1;
				}
	
				// Pick ...
				if(SCH__PICK_FROM(false,
								  p_variable,
								  p_alarm,
							 	  false,
								  arm_type,
								  al_name,
								  al_slot) < 0)
				{
					return -1;
				}

				ch_data.Format("%1d", cur_count);
				sCH__ATM_RB__CUR_ALIGN_RETRY->Set__DATA(ch_data);
			}		
		}
	}

	if(SCH__PLACE_TO(false,
					 p_variable,
					 p_alarm,
					 ex_flag,
					 arm_type,
					 para_module,
					 para_slot) < 0)
	{
		return -1;
	}

	if(SCH__CHECK_PLACE_MODULE__COMPLETE(p_variable,
										 ex_flag,
										 arm_type,
										 para_module,
										 para_slot) < 0)
	{
		return -1;
	}

	// ...
	{
		l_para.RemoveAll();
		l_para.Add(para_module);
		l_para.Add(para_slot);

		p_ext_mode_ctrl->Set__RETURN_FNC_PARAMETER(l_para);
	}
	return 1;
}

int CObj__DUAL_ARM_STD
::Update__MAINT_TARGET_ALIGN(CII_OBJECT__ALARM *p_alarm,
							 const CString& para_module,
							 const CString& para_slot)
{
	if((xCH__ATM_RB__TARGET_LLx_MODE->Check__DATA(STR__ENABLE) > 0)
	|| (xCH__ATM_RB__TARGET_PMx_MODE->Check__DATA(STR__ENABLE) > 0))
	{
		CString trg__al_name;
		CString trg__al_slot;

		if(xCH__ATM_RB__TARGET_LLx_MODE->Check__DATA(STR__ENABLE) > 0)
		{
			if(Get__LLx_INDEX(para_module) >= 0)
			{
				trg__al_name = para_module;
				trg__al_slot = para_slot;
			}
			else
			{
				trg__al_name = SCH__Get_LBx_To_PLACE("", trg__al_slot, true);
			}
		}
		else if(xCH__ATM_RB__TARGET_PMx_MODE->Check__DATA(STR__ENABLE) > 0)
		{
			CString para__trg_module = sCH__PARA_MANUAL_MOVE_ALIGN_TARGET_PMx->Get__STRING();
			
			int para__pmc_index = Macro__Get_PMC_INDEX(para__trg_module);
			int para__pmc_id = para__pmc_index + 1;
			
			if(para__pmc_id > 0)
			{
				trg__al_name.Format("PM%1d", para__pmc_id);
				trg__al_slot = "1";
			}
			else
			{
				CString box_title;
				CString box_msg;
				CStringArray l_act;
				CString r_act;

				box_title = "Select a PM to align.";
				box_msg   = "Please, select a PM to align.";

				for(int k=0; k<iPMx_SIZE; k++)
				{
					int pmc_id = k + 1;

					if(dEXT_CH__CFG_PMx_USE[k]->Check__DATA(STR__ENABLE) < 0)
					{
						continue;
					}

					// ...
					CString str_pmc;
	
					str_pmc.Format("PM%1d", pmc_id);
					l_act.Add(str_pmc);
				}

				p_alarm->Popup__MESSAGE_BOX(box_title, box_msg, l_act, r_act);

				trg__al_name = r_act;
				trg__al_slot = "1";
			}
		}

		xCH__ATM_RB__TARGET_LLx_NAME_SET_ALL->Set__DATA(trg__al_name);
		xCH__ATM_RB__TARGET_LLx_SLOT_SET_ALL->Set__DATA(trg__al_slot);

		return 1;
	}

	// ...
	{
		xCH__ATM_RB__TARGET_LLx_NAME_SET_ALL->Set__DATA("");
		xCH__ATM_RB__TARGET_LLx_SLOT_SET_ALL->Set__DATA("");
	}
	return -1;
}
