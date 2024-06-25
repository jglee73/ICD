#include "StdAfx.h"
#include "CObj__DUAL_ARM_STD.h"


// ...
int  CObj__DUAL_ARM_STD
::Call__CHECK_STANDBY(CII_OBJECT__VARIABLE *p_variable,
					  CII_OBJECT__ALARM *p_alarm)
{
	DECLARE__EXT_CTRL(p_variable);

	// ...
	int i_lp = -1;

	CString str_module;
	CString str_name;
	int i;

	xCH__CHECK_MODULE->Get__DATA(str_module);

	// LPx Check ...
	{
		for(i=0; i<CFG_LP_LIMIT; i++)
		{
			str_name.Format("LP%1d",i+1);
			if(str_module.CompareNoCase(str_name) != 0)
			{
				continue;
			}

			i_lp = i;
			break;
		}

		if(i_lp >= 0)
		{
			if(LPx__Is_Available(i_lp) > 0)
			{
				return 1;
			}
			else
			{
				int alarm_id = ALID__LPx_STATUS__IS_NOT_AVAILABLE;
				CString alm_msg;
				CString alm_bff;

				alm_msg.Format("LP%1d's status is not available !\n", i_lp+1);
				alm_bff.Format("Please, check LP%1d's status.\n", i_lp+1);
				alm_msg += alm_bff;

				p_alarm->Post__ALARM_With_MESSAGE(alarm_id,alm_msg);
				return -1;
			}
		}
	}

	// LLx Check ...
	{
		int ll_i = Get__LLx_INDEX(str_module);
		if(ll_i >= 0)
		{
			if(LLx__Is_Available(ll_i) > 0)
			{
				return  1;
			}
			else
			{
				int alarm_id = ALID__LLx_STATUS__IS_NOT_AVAILABLE;
				CString alm_msg;
				CString alm_bff;

				int ll_id = ll_i + 1;

				alm_msg.Format("LL%1d's status is not available !\n", ll_id);
				alm_bff.Format("Please, check LL1's status.\n");
				alm_msg += alm_bff;

				p_alarm->Post__ALARM_With_MESSAGE(alarm_id,alm_msg);
				return -1;
			}
		}
	}

	return 1;
}

int  CObj__DUAL_ARM_STD
::Call__CHECK_MATERIAL(CII_OBJECT__VARIABLE *p_variable)
{
	DECLARE__EXT_CTRL(p_variable);

	// ...
	CStringArray l_para;
	CString para_data;

	int count = 0;

	// ATM_RBx
	if(ATM_RB__Check_Occupied__Arm_Type() > 0)
	{
		count++;
	}

	// AL
	if(AL1__Check_Empty__All_Slot() < 0)
	{
		count++;
	}

	// BUFFER1
	if(sCH__SCH_DB_ST1_USE_FLAG->Check__DATA(STR__ENABLE) > 0)
	{
		if(Buffer1__Check_Empty__All_Slot() < 0)
		{
			count++;
		}
	}
	// BUFFER2
	if(sCH__SCH_DB_ST2_USE_FLAG->Check__DATA(STR__ENABLE) > 0)
	{
		if(Buffer2__Check_Empty__All_Slot() < 0)
		{
			count++;
		}
	}

	// LBx
	{
		int ll_limit = iLLx_SIZE;
		int ll_i;

		for(ll_i=0; ll_i<ll_limit; ll_i++)
		{
			count += LLx__Get_Occupied__TotalSlot(ll_i);
		}
	}

	// ...
	{
		l_para.RemoveAll();

		para_data.Format("%1d",count);
		l_para.Add(para_data);

		p_ext_mode_ctrl->Set__RETURN_FNC_PARAMETER(l_para);
	}
	return 1;
}
