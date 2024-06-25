#include "StdAfx.h"
#include "CObj__DUAL_ARM_STD.h"


// ...
int  CObj__DUAL_ARM_STD
::Call__MAINT_PICK(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm)
{
	if(Module__Check("CObj__DUAL_ARM_STD::Call__MAINT_PICK()",p_alarm) < 0)
	{
		return -1;
	}

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

	return Seq__MAINT_PICK(p_variable,p_alarm,para_arm,para_module,para_slot);
}
int  CObj__DUAL_ARM_STD::
Seq__MAINT_PICK(CII_OBJECT__VARIABLE *p_variable,
				CII_OBJECT__ALARM *p_alarm,
				const CString& para_arm,
				const CString& para_module,
				const CString& para_slot)
{
	DECLARE__EXT_CTRL(p_variable);

LOOP_RETRY:

	// ...
	{
		CString log_msg;
		CString log_bff;

		log_msg = "Seq__MAINT_PICK() : Started ... \n";
		log_bff.Format(" * para_arm <- %s \n", para_arm);
		log_msg += log_bff;
		log_bff.Format(" * para_module <- %s \n", para_module);
		log_msg += log_bff;
		log_bff.Format(" * para_slot <- %s \n", para_slot);
		log_msg += log_bff;

		Fnc__LOG_CTRL(log_msg);
	}

	if(xCH__VAC_RB_PAUSE_FLAG->Check__DATA(STR__YES) > 0)
	{
		int alarm_id = ALID__MAINT_PICK_ERROR__VAC_RB_PAUSED;
		CString alm_msg;
		CString alm_bff;
		CString r_act;

		// ...
		{
			alm_bff.Format("Material Move :  %s(%s) -> Arm(%s) \n", para_module, para_slot, para_arm);
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
	CString arm_type;

	if((para_arm == "A")||(para_arm == "B"))
	{
		arm_type = para_arm;

		if(para_arm == "A")
		{
			if(VAC_RB__Check_A_Arm_Enable() < 0)
			{
				int alarm_id = ALID__VAC_ROBOT__A_ARM__NOT_USED;
				CString alarm_msg;
				CString r_act;

				alarm_msg = "Please, check VAC-Robot's A Arm USE mode in config page.\n";

				p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,alarm_msg,r_act);
				return -1;
			}
		}
		else if(para_arm == "B")
		{
			if(VAC_RB__Check_B_Arm_Enable() < 0)
			{
				int alarm_id = ALID__VAC_ROBOT__B_ARM__NOT_USED;
				CString alarm_msg;
				CString r_act;

				alarm_msg = "Please, check VAC-Robot's B Arm USE mode in config page.\n";

				p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,alarm_msg,r_act);
				return -1;
			}
		}
	}
	else if(para_arm == "X")
	{
		int ll_i = Get__LLx_INDEX(para_module);
		if(ll_i >= 0)
		{
			if(VAC_RB__Get_Empty__Arm_Type_From_LBx(arm_type) < 0)
			{
				// Error

				return -1;
			}
		}
		else
		{
			int pm_i = SCH__CHECK_PM_INDEX(para_module);

			if(pm_i >= 0)
			{
				if(VAC_RB__Get_Empty__Arm_Type_From_PMx(arm_type) < 0)
				{
					// Error

					return -1;
				}
			}
		}
	}
	else
	{
		// Error

		return -1;
	}

	try
	{	
		return _Seq__MAINT_PICK(p_variable,p_alarm,arm_type,para_module,para_slot);
	}
	catch(const int err_flag)
	{
		return -1;
	}

	return 1;
}
int  CObj__DUAL_ARM_STD::
_Seq__MAINT_PICK(CII_OBJECT__VARIABLE *p_variable,
				 CII_OBJECT__ALARM *p_alarm,
				 const CString& arm_type,
				 const CString& para_module,
				 const CString& para_slot)
{
	DECLARE__EXT_CTRL(p_variable);

	bool ex_flag = false;

	if(SCH__CHECK_PICK_MODULE__READY(p_variable,ex_flag, arm_type,para_module,para_slot) < 0)
	{
		return -1;
	}

	if(SCH__PICK_FROM(false, p_variable,p_alarm, "", ex_flag, arm_type,para_module,para_slot) < 0)
	{
		return -1;
	}

	// ...
	CString sch_name;
	sch_name.Format("%s-%s", para_module,para_slot);

	if(SCH__CHECK_PICK_MODULE__COMPLETE(p_variable,p_alarm,ex_flag, arm_type,para_module,para_slot, sch_name) < 0)
	{
		return -1;
	}

	// ...
	{
		CStringArray l_para;

		l_para.RemoveAll();
		l_para.Add(arm_type);

		p_ext_mode_ctrl->Set__RETURN_FNC_PARAMETER(l_para);
	}
	return 1;
}

int  CObj__DUAL_ARM_STD
::Call__MAINT_PLACE(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM *p_alarm)
{
	if(Module__Check("CObj__DUAL_ARM_STD::Call__MAINT_PLACE()",p_alarm) < 0)
	{
		return -11;
	}

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

	return Seq__MAINT_PLACE(p_variable,p_alarm,para_arm,para_module,para_slot);
}
int  CObj__DUAL_ARM_STD::
Seq__MAINT_PLACE(CII_OBJECT__VARIABLE *p_variable,
				 CII_OBJECT__ALARM *p_alarm,
				 const CString& para_arm,
				 const CString& str_module,
				 const CString& str_slot,
				 const int tool_clear_flag)
{
	DECLARE__EXT_CTRL(p_variable);

LOOP_RETRY:

	// ...
	{
		CString log_msg;
		CString log_bff;

		log_msg = "Seq__MAINT_PLACE() : Started ... \n";
		log_bff.Format(" * para_arm <- %s \n", para_arm);
		log_msg += log_bff;
		log_bff.Format(" * str_module <- %s \n", str_module);
		log_msg += log_bff;
		log_bff.Format(" * str_slot <- %s \n", str_slot);
		log_msg += log_bff;
		log_bff.Format(" * tool_clear_flag <- %1d \n", tool_clear_flag);
		log_msg += log_bff;

		Fnc__LOG_CTRL(log_msg);
	}

	// ...
	CString para_module = str_module;
	CString para_slot   = str_slot;

	if(xCH__VAC_RB_PAUSE_FLAG->Check__DATA(STR__YES) > 0)
	{
		int alarm_id = ALID__MAINT_PLACE_ERROR__VAC_RB_PAUSED;
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
	CString arm_type;

	if((para_arm == "A")||(para_arm == "B"))
	{
		arm_type = para_arm;
	}
	else
	{
		// Error

		return -1;
	}

	// Module Check
	if(para_module == "LBO")
	{
		int out_slot = -1;

		if(arm_type.CompareNoCase("A") == 0)
		{
			if(xCH_CFG__A_ARM_PLACE_TO_LBx_SLOT->Check__DATA("ALL") < 0)
			{
				CString var_data;
				xCH_CFG__A_ARM_PLACE_TO_LBx_SLOT->Get__DATA(var_data);

				out_slot = atoi(var_data);
			}
		}
		else if(arm_type.CompareNoCase("B") == 0)
		{
			if(xCH_CFG__B_ARM_PLACE_TO_LBx_SLOT->Check__DATA("ALL") < 0)
			{
				CString var_data;
				xCH_CFG__B_ARM_PLACE_TO_LBx_SLOT->Get__DATA(var_data);

				out_slot = atoi(var_data);
			}
		}

		if(out_slot < 0)
		{
			int ll_limit = iLLx_LIMIT;
			int ll_i;

			for(ll_i=0; ll_i<ll_limit; ll_i++)
			{
				if(out_slot < 0)
				{
					if(LLx__Is_Available(ll_i) > 0)
					{
						if(LLx__Get_Empty__OutSlot(ll_i, out_slot) > 0)
						{
							para_module = Get__LLx_NAME(ll_i);
						}
					}
				}
			}

			if(out_slot < 0)
			{
				if(tool_clear_flag > 0)
				{
					return 0;
				}
				return -1;
			}
		}
		else
		{
			int active__para_module = -1;

			int ll_limit = iLLx_LIMIT;
			int ll_i;

			for(ll_i=0; ll_i<ll_limit; ll_i++)
			{
				if((LLx__Is_Available(ll_i) > 0)
				&& (LLx__Check_Empty__SlotStatus(ll_i, out_slot) > 0))
				{
					para_module = Get__LLx_NAME(ll_i);

					active__para_module = 1;
				}
			}

			if(active__para_module < 0)
			{
				return -1;
			}
		}

		para_slot.Format("%1d",out_slot);
	}

	try
	{
		return _Seq__MAINT_PLACE(p_variable,p_alarm,arm_type,para_module,para_slot);
	}
	catch(const int err_flag)
	{
		return -1;
	}

	return 1;
}
int  CObj__DUAL_ARM_STD::
_Seq__MAINT_PLACE(CII_OBJECT__VARIABLE *p_variable,
				  CII_OBJECT__ALARM *p_alarm,
				  const CString& arm_type,
				  const CString& para_module,
				  const CString& para_slot)
{
	DECLARE__EXT_CTRL(p_variable);

	bool ex_flag = false;

	if(SCH__CHECK_PLACE_MODULE__READY(p_variable,ex_flag,arm_type,para_module,para_slot) < 0)
	{
		return -1;
	}

	if(SCH__PLACE_TO(false, p_variable,p_alarm,ex_flag,arm_type,para_module,para_slot) < 0)
	{
		return -1;
	}

	if(SCH__CHECK_PLACE_MODULE__COMPLETE(p_variable,ex_flag,arm_type,para_module,para_slot) < 0)
	{
		return -1;
	}

	// ...
	{
		CStringArray l_para;

		l_para.RemoveAll();
		l_para.Add(para_module);
		l_para.Add(para_slot);

		p_ext_mode_ctrl->Set__RETURN_FNC_PARAMETER(l_para);
	}
	return 1;
}
