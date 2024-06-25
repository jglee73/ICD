#include "StdAfx.h"
#include "CObj__DUAL_ARM_STD.h"

#include "Macro_Function.h"


// ...
int  CObj__DUAL_ARM_STD
::Call__CLEAR(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm)
{
	CStringArray l_para;
	
	CString arm_type;
	CString para_module;
	CString para_slot;
	
	CString wfr_title;

	// VAC_RB -> LBx
	if(VAC_RB__Check_Occupied__A_Arm(wfr_title) > 0)
	{
		arm_type = "A";
		para_module = "LBO";
		para_slot   = "X";

		int flag = Seq__MAINT_PLACE(p_variable,
									p_alarm,
									arm_type,
									para_module,
									para_slot,
									1);

		if(flag < 0)
		{
			Alarm_Post__ToolClear__Place_Error(p_alarm,arm_type,para_module,para_slot);
			return -1;
		}
		if(flag == 0)
		{
			return 1;
		}
	}

	if(VAC_RB__Check_Occupied__B_Arm(wfr_title) > 0)
	{
		arm_type = "B";
		para_module = "LBO";
		para_slot   = "X";

		int flag = Seq__MAINT_PLACE(p_variable,
									p_alarm,
									arm_type,
									para_module,
									para_slot,
									1);

		if(flag <  0)
		{
			Alarm_Post__ToolClear__Place_Error(p_alarm,arm_type,para_module,para_slot);			
			return -1;
		}
		if(flag == 0)
		{
			return 1;
		}
	}

	// PMx -> LBx
	CString obj_sts;
	CUIntArray l_pm_err;
	int i;

	for(i=0;i<iPMx_SIZE;i++)
	{
		if(xEXT_CH__CFG__PMx_USE[i]->Check__DATA("ENABLE") < 0)		continue;
		if(PMx__Check_Occupied__SlotStatus(i) < 0)					continue;

		if(xEXT_CH__CFG__TOOL_CLEAR_PMC_MATERIAL_MODE->Check__DATA(STR__NOT_ERROR_WAFER) > 0)
		{
			if(xCH__PMx__SLOT_STATUS[0]->Check__DATA(STR__ABORTED) > 0)
			{
				continue;
			}
		}

		// ...
		{
			xCH__PMx__OBJ_STATUS[i]->Get__DATA(obj_sts);

			if((obj_sts.CollateNoCase("STANDBY")  != 0)
			&& (obj_sts.CollateNoCase("CTCINUSE") != 0))
			{
				l_pm_err.Add(i+1);
				continue;
			}
		}

		if(VAC_RB__Get_Empty__Arm_Type_From_PMx(arm_type) > 0)
		{
			para_module.Format("PM%1d",i+1);
			para_slot = "1";

			if(Seq__MAINT_PICK(p_variable,p_alarm,arm_type,para_module,para_slot) < 0)
			{
				Alarm_Post__ToolClear__Pick_Error(p_alarm,arm_type,para_module,para_slot);
				return -1;
			}

			// ...
			para_module = "LBO";
			para_slot = "1";

			if(Seq__MAINT_PLACE(p_variable,
								p_alarm,
								arm_type,
								para_module,
								para_slot,
								1) < 0)	
			{
				Alarm_Post__ToolClear__Place_Error(p_alarm,arm_type,para_module,para_slot);
				return -1;
			}
		}
	}

	if(l_pm_err.GetSize() > 0)
	{
		int alarm_id = ALID__PMC__NOT_AVAILABLE;
		CString r_act;

		CString err_msg;
		CString err_bff;

		err_msg  = "ToolClear - Error\n";
		err_msg += "Please, check the following pmc module;\n";

		int limit = l_pm_err.GetSize();
		int i;

		for(i=0;i<limit;i++)
		{
			err_bff.Format("   PM%1d\n",l_pm_err.GetAt(i));
			err_msg += err_bff;
		}

		p_alarm->Check__ALARM(alarm_id,r_act);
		p_alarm->Post__ALARM_With_MESSAGE(alarm_id, err_msg);
	}
	return 1;
}

int  CObj__DUAL_ARM_STD::
Alarm_Post__ToolClear__Place_Error(CII_OBJECT__ALARM *p_alarm,
								   const CString& module_name,
								   const CString& para_module,
								   const CString& para_slot)
{
	int alarm_id = ALID__TOOL_CLEAR__ACTION_ERROR;
	CString alarm_msg;
	CString alarm_bff;
	CString r_act;

	//
	alarm_msg = "Place Sequence Error\n";
			
	alarm_bff.Format("%s -> %s's %s\n",module_name,para_module,para_slot);
	alarm_msg += alarm_bff;

	//
	p_alarm->Check__ALARM(alarm_id,r_act);
	p_alarm->Post__ALARM_With_MESSAGE(alarm_id,alarm_msg);
	return 1;
}
int  CObj__DUAL_ARM_STD::
Alarm_Post__ToolClear__Pick_Error(CII_OBJECT__ALARM *p_alarm,
								  const CString& module_name,
								  const CString& para_module,
								  const CString& para_slot)
{
	int alarm_id = ALID__TOOL_CLEAR__ACTION_ERROR;
	CString alarm_msg;
	CString alarm_bff;
	CString r_act;

	//
	alarm_msg = "Pick Sequence Error\n";
			
	alarm_bff.Format("%s <- %s's %s\n",module_name,para_module,para_slot);
	alarm_msg += alarm_bff;

	//
	p_alarm->Check__ALARM(alarm_id,r_act);
	p_alarm->Post__ALARM_With_MESSAGE(alarm_id,alarm_msg);
	return 1;
}
