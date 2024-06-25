#include "StdAfx.h"
#include "CObj__DUAL_ARM_STD.h"


// ...
int  CObj__DUAL_ARM_STD
::Call__CHECK_STANDBY(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm)
{
	DECLARE__EXT_CTRL(p_variable);

	// ...
	int i_pm = -1;

	CString str_module;
	CString str_name;
	int i;

	xCH__CHECK_MODULE->Get__DATA(str_module);

	// PMx Check ...
	{
		for(i=0;i<CFG_PM_LIMIT;i++)
		{
			str_name.Format("PM%1d",i+1);
			if(str_module.CompareNoCase(str_name) != 0)
			{
				continue;
			}

			i_pm = i;
			break;
		}

		if(i_pm >= 0)
		{
			if(PMx__Is_Available(i_pm) > 0)
			{
				return 1;
			}
			else
			{
				int alarm_id = ALID__PMx_STATUS__IS_NOT_AVAILABLE;
				CString alm_msg;
				CString alm_bff;

				alm_msg.Format("PM%1d's status is not available !\n", i_pm+1);
				alm_bff.Format("Please, check PM%1d's status.\n", i_pm+1);
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
				return 1;
			}
			else
			{
				int alarm_id = ALID__LLx_STATUS__IS_NOT_AVAILABLE;
				CString alm_msg;
				CString alm_bff;

				int ll_id = ll_i + 1;

				alm_msg.Format("LL%1d's status is not available !\n", ll_id);
				alm_bff.Format("Please, check LL%1d's status.\n", ll_id);
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

	// VAC_RBx
	if(VAC_RB__Check_Occupied__A_Arm() > 0)
	{
		count++;
	}
	if(VAC_RB__Check_Occupied__B_Arm() > 0)
	{
		count++;
	}

	// PMx
	int i;

	for(i=0;i<iPMx_SIZE;i++)
	{
		if(xEXT_CH__CFG__PMx_USE[i]->Check__DATA("ENABLE") < 0)		continue;
		if(PMx__Check_Occupied__SlotStatus(i) < 0)					continue;
		if(PMx__Is_Available(i) < 0)								continue;

		count++;
	}

	// ...
	l_para.RemoveAll();

	para_data.Format("%1d",count);
	l_para.Add(para_data);

	p_ext_mode_ctrl->Set__RETURN_FNC_PARAMETER(l_para);
	return 1;
}
