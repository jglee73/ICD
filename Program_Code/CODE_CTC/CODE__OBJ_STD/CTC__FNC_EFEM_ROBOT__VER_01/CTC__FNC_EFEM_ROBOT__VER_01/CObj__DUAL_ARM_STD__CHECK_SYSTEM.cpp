#include "StdAfx.h"
#include "CObj__DUAL_ARM_STD.h"


// ...
int  CObj__DUAL_ARM_STD
::MANUAL_MOVE__Check_System_Module_Error(const int pick_flag,
										 const CString& para_arm,
										 const CString& para_module,
										 const CString& para_slot,
										 CII_OBJECT__ALARM *p_alarm)
{
	CString err_msg;
	CString r_act;

	// ATM_ROBOT
	if(ATM_RB__Is_Available() < 0)
	{
		if(pick_flag > 0)
		{
			err_msg = "Manual Move - Pick : ATM Robot Status Error !";
		}
		else
		{
			err_msg = "Manual Move - Place : ATM Robot Status Error !";
		}

		p_alarm->Check__ALARM(ALID__MANUAL_MOVE__MODULE_STATUS_ERROR,r_act);
		p_alarm->Post__ALARM_With_MESSAGE(ALID__MANUAL_MOVE__MODULE_STATUS_ERROR,err_msg);
		return -1;
	}

	int ll_i = Get__LLx_INDEX(para_module);

	// AL1
	if(para_module.CompareNoCase(MODULE__AL1) == 0)
	{
		if(AL1__Is_Available() < 0)
		{
			if(pick_flag > 0)
			{
				err_msg = "Manual Move - Pick : AL1 Status Error !";
			}
			else
			{
				err_msg = "Manual Move - Place : AL1 Status Error !";
			}

			p_alarm->Check__ALARM(ALID__MANUAL_MOVE__MODULE_STATUS_ERROR,r_act);
			p_alarm->Post__ALARM_With_MESSAGE(ALID__MANUAL_MOVE__MODULE_STATUS_ERROR,err_msg);
			return -1;
		}
	}
	// LLx
	else if(ll_i >= 0)
	{
		CString ll_name = Get__LLx_NAME(ll_i);

		if(LLx__Is_Available(ll_i) < 0)
		{
			if(pick_flag > 0)
			{
				err_msg.Format("Manual Move - Pick : %s Status Error !", ll_name);
			}
			else
			{
				err_msg.Format("Manual Move - Place : %s Status Error !", ll_name);
			}

			p_alarm->Check__ALARM(ALID__MANUAL_MOVE__MODULE_STATUS_ERROR,r_act);
			p_alarm->Post__ALARM_With_MESSAGE(ALID__MANUAL_MOVE__MODULE_STATUS_ERROR,err_msg);
			return -1;
		}

		int lbx_slot_id = atoi(para_slot);

		if(LLx__Check__Slot_Enable(ll_i, lbx_slot_id) < 0)
		{
			if(pick_flag > 0)
			{
				err_msg.Format("Manual Move - Pick : %s's %s slot is not enabled !", ll_name,para_slot);
			}
			else
			{
				err_msg.Format("Manual Move - Place : %s's %s slot is not enabled !", ll_name,para_slot);
			}

			p_alarm->Check__ALARM(ALID__MANUAL_MOVE__MODULE_STATUS_ERROR,r_act);
			p_alarm->Post__ALARM_With_MESSAGE(ALID__MANUAL_MOVE__MODULE_STATUS_ERROR,err_msg);
			return -1;
		}
	}
	// Side Buffer (1)
	else if(para_module.CompareNoCase(MODULE__BUFFER1) == 0)
	{
		if(sCH__SCH_DB_ST1_USE_FLAG->Check__DATA(STR__ENABLE) < 0)
		{
			if(pick_flag > 0)
			{
				err_msg = "Manual Move - Pick : Side-Buffer(1) is not enabled !";
			}
			else 
			{
				err_msg = "Manual Move - Place : Side-Buffer(1) is not enabled !";
			}

			p_alarm->Check__ALARM(ALID__MANUAL_MOVE__MODULE_STATUS_ERROR,r_act);
			p_alarm->Post__ALARM_With_MESSAGE(ALID__MANUAL_MOVE__MODULE_STATUS_ERROR,err_msg);
			return -1;
		}
	}
	// Side Buffer (2)
	else if(para_module.CompareNoCase(MODULE__BUFFER2) == 0)
	{
		if(sCH__SCH_DB_ST2_USE_FLAG->Check__DATA(STR__ENABLE) < 0)
		{
			if(pick_flag > 0)
			{
				err_msg = "Manual Move - Pick : Side-Buffer(2) is not enabled !";
			}
			else 
			{
				err_msg = "Manual Move - Place : Side-Buffer(2) is not enabled !";
			}

			p_alarm->Check__ALARM(ALID__MANUAL_MOVE__MODULE_STATUS_ERROR,r_act);
			p_alarm->Post__ALARM_With_MESSAGE(ALID__MANUAL_MOVE__MODULE_STATUS_ERROR,err_msg);
			return -1;
		}
	}
	// LP1 ~ LP5
	else 
	{
		CString lpx_name;
		int ptn;
		int i;

		for(i=0;i<CFG_LP_LIMIT;i++)
		{
			ptn = i + 1;

			lpx_name.Format("LP%1d",ptn);
			if(lpx_name.CompareNoCase(para_module) != 0)		continue;

			//.....
			if(LPx__Is_Available(i) < 0)
			{
				if(pick_flag > 0)
				{
					err_msg.Format("Manual Move - Pick : LP%1d Status Error !",ptn);
				}
				else
				{
					err_msg.Format("Manual Move - Place : LP%1d Status Error !",ptn);
				}

				p_alarm->Check__ALARM(ALID__MANUAL_MOVE__MODULE_STATUS_ERROR,r_act);
				p_alarm->Post__ALARM_With_MESSAGE(ALID__MANUAL_MOVE__MODULE_STATUS_ERROR,err_msg);
				return -1;
			}
		}
	}

	return 1;
}

int  CObj__DUAL_ARM_STD
::TOOL_CLEAR__Check_System_Module_Error(CII_OBJECT__ALARM *p_alarm)
{
	int err_flag = -1;
	CString err_msg;
	CString err_bff;
	CString r_act;

	err_msg = "Tool Clear Error : Please, check the following modules;\n\n";

	// ATM_ROBOT
	if(ATM_RB__Is_Available() < 0)
	{
		err_flag = 1;
		err_msg += "     ATM Robot's status is not available.\n";
	}
	// AL1
	if(AL1__Is_Available() < 0)
	{
		if(AL1__Check_CtcInUse_Or_Standby() < 0)
		{
			err_flag = 1;
			err_msg += "     AL1's status is not \"Standby\" or \"CtcInUse\".\n";
		}
	}

	// LLx ...
	{
		int ll_limit = iLLx_SIZE;
		int ll_i;

		for(ll_i=0; ll_i<ll_limit; ll_i++)
		{
			if((xEXT_CH__LLx__CFG_USE_FLAG[ll_i]->Check__DATA("ENABLE") > 0)
				&& (LLx__Is_Available(ll_i) < 0))
			{
				if(LLx__Check_CtcInUse_Or_Standby(ll_i) < 0)
				{				
					CString ll_name = Get__LLx_NAME(ll_i);

					err_bff.Format("     %s's Status is not \"Standby\" or \"CtcInUse\".\n", ll_name);
					err_msg += err_bff;

					err_flag = 1;
				}
			}
		}
	}

	if(err_flag > 0)
	{
		CString r_act;

		p_alarm->Check__ALARM(ALID__TOOL_CLEAR__MODULE_STATUS_ERROR,r_act);
		p_alarm->Post__ALARM_With_MESSAGE(ALID__TOOL_CLEAR__MODULE_STATUS_ERROR,err_msg);
	}
	return 1;
}
