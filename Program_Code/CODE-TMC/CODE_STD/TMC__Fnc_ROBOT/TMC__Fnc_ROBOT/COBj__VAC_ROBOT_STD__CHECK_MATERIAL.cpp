#include "StdAfx.h"
#include "CObj__VAC_ROBOT_STD.h"
#include "CObj__VAC_ROBOT_STD__DEF.h"
#include "CObj__VAC_ROBOT_STD__ALID.h"

#include "Macro_Function.h"


// ...
int  CObj__VAC_ROBOT_STD::
Interlock__CHECK_MATERIAL(CII_OBJECT__ALARM* p_alarm,
						  const int place_flag,
						  const CString& arm_type,
						  const CString& stn_name,
						  const CString& stn_slot)
{
LOOP_RETRY:

	// ...
	int slot_index = atoi(stn_slot) - 1;
	CString wfr_sts;

	if(arm_type.CompareNoCase(ARM_A) == 0)
	{
		dEXT_CH__PHY_ROBOT__ARM_A_MATERIAL_STATUS->Get__DATA(wfr_sts);

		if(place_flag > 0)
		{
			if(wfr_sts.CompareNoCase(STR__NONE) == 0)
			{
				int alarm_id = ALID__ARM_A__MATERIAL_NONE_ERROR;
				CString err_msg;
				CString r_act;

				err_msg = "Material on Arm-A must exist.\n";

				p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,err_msg,r_act);

				if(r_act.CompareNoCase(ACT__RETRY) == 0)		goto LOOP_RETRY;
				return -1;
			}
		}
		else
		{
			if(wfr_sts.CompareNoCase(STR__NONE) != 0)
			{
				int alarm_id = ALID__ARM_A__MATERIAL_EXIST_ERROR;
				CString err_msg;
				CString r_act;

				err_msg = "No material on Arm-A must exist.\n";

				p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,err_msg,r_act);

				if(r_act.CompareNoCase(ACT__RETRY) == 0)		goto LOOP_RETRY;
				return -1;
			}
		}
	}
	else if(arm_type.CompareNoCase(ARM_B) == 0)
	{
		dEXT_CH__PHY_ROBOT__ARM_B_MATERIAL_STATUS->Get__DATA(wfr_sts);

		if(place_flag > 0)
		{
			if(wfr_sts.CompareNoCase(STR__NONE) == 0)
			{
				int alarm_id = ALID__ARM_B__MATERIAL_NONE_ERROR;
				CString err_msg;
				CString r_act;

				err_msg = "Material on Arm-B must exist.\n";

				p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,err_msg,r_act);

				if(r_act.CompareNoCase(ACT__RETRY) == 0)		goto LOOP_RETRY;
				return -1;
			}
		}
		else
		{
			if(wfr_sts.CompareNoCase(STR__NONE) != 0)
			{
				int alarm_id = ALID__ARM_B__MATERIAL_EXIST_ERROR;
				CString err_msg;
				CString r_act;

				err_msg = "No material on Arm-B must exist.\n";

				p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,err_msg,r_act);

				if(r_act.CompareNoCase(ACT__RETRY) == 0)		goto LOOP_RETRY;
				return -1;
			}
		}
	}

	//.....
	int pm_index = Macro__CHECK_PMx_INDEX(stn_name);

	if((pm_index >= 0)&&(pm_index < m_nPM_LIMIT))
	{
		if(slot_index == 0)
		{
			dEXT_CH__PHY_PMx_SLOT01_STATUS[pm_index]->Get__DATA(wfr_sts);

			if(place_flag < 0)
			{
				if(wfr_sts.CompareNoCase(STR__NONE) == 0)
				{
					int alarm_id = ALID__PMx__MATERIAL_NONE_ERROR;
					CString err_msg;
					CString r_act;

					err_msg.Format("Material in %s(%s) must exist.\n",stn_name,stn_slot);

					p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,err_msg,r_act);

					if(r_act.CompareNoCase(ACT__RETRY) == 0)		goto LOOP_RETRY;
					return -1;
				}
			}
			else
			{
				if(wfr_sts.CompareNoCase(STR__NONE) != 0)
				{
					int alarm_id = ALID__PMx__MATERIAL_EXIST_ERROR;
					CString err_msg;
					CString r_act;

					err_msg.Format("No material in %s(%s) must exist.\n",stn_name,stn_slot);

					p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,err_msg,r_act);

					if(r_act.CompareNoCase(ACT__RETRY) == 0)		goto LOOP_RETRY;
					return -1;
				}
			}
		}
	}

	//
	return 1;
}


int  CObj__VAC_ROBOT_STD::
Fnc__CHANGE_MATERIAL_INFO(const int place_flag,
						  const CString& arm_type,
						  const CString& stn_name,
						  const CString& stn_slot)
{
	int check_flag = -1;
	int slot_index = atoi(stn_slot) - 1;

	CString wfr_sts;
	CString wfr_title;

	if(place_flag > 0)
	{
		if(arm_type.CompareNoCase(ARM_A) == 0)
		{
			dEXT_CH__PHY_ROBOT__ARM_A_MATERIAL_STATUS->Get__DATA(wfr_sts);
			sEXT_CH__PHY_ROBOT__ARM_A_MATERIAL_TITLE->Get__DATA(wfr_title);

			dEXT_CH__PHY_ROBOT__ARM_A_MATERIAL_STATUS->Set__DATA(STR__NONE);
			sEXT_CH__PHY_ROBOT__ARM_A_MATERIAL_TITLE->Set__DATA("");
		}
		else if(arm_type.CompareNoCase(ARM_B) == 0)
		{
			dEXT_CH__PHY_ROBOT__ARM_B_MATERIAL_STATUS->Get__DATA(wfr_sts);
			sEXT_CH__PHY_ROBOT__ARM_B_MATERIAL_TITLE->Get__DATA(wfr_title);

			dEXT_CH__PHY_ROBOT__ARM_B_MATERIAL_STATUS->Set__DATA(STR__NONE);
			sEXT_CH__PHY_ROBOT__ARM_B_MATERIAL_TITLE->Set__DATA("");
		}
	}

	if(check_flag < 0)
	{
		int ll_i = Macro__CHECK_LLx_INDEX(stn_name);
		if(ll_i >= 0)
		{
			check_flag = 1;

			if((slot_index >= 0) 
			&& (slot_index <  CFG_LLx_SLOT_LIMIT))
			{
				if(place_flag > 0)
				{
					dEXT_CH__LLx__SLOTx_STATUS[ll_i][slot_index]->Set__DATA(wfr_sts);
					sEXT_CH__LLx__SLOTx_TITLE[ll_i][slot_index]->Set__DATA(wfr_title);
				}
				else
				{
					dEXT_CH__LLx__SLOTx_STATUS[ll_i][slot_index]->Get__DATA(wfr_sts);
					sEXT_CH__LLx__SLOTx_TITLE[ll_i][slot_index]->Get__DATA(wfr_title);

					dEXT_CH__LLx__SLOTx_STATUS[ll_i][slot_index]->Set__DATA(STR__NONE);
					sEXT_CH__LLx__SLOTx_TITLE[ll_i][slot_index]->Set__DATA("");
				}
			}
		}
	}
	if(check_flag < 0)
	{
		int pm_index = Macro__CHECK_PMx_INDEX(stn_name);

		if((pm_index >= 0)&&(pm_index < m_nPM_LIMIT))
		{
			check_flag = 1;

			if(slot_index == 0)
			{
				if(place_flag > 0)
				{
					dEXT_CH__PHY_PMx_SLOT01_STATUS[pm_index]->Set__DATA(wfr_sts);
					sEXT_CH__PHY_PMx_SLOT01_TITLE[pm_index]->Set__DATA(wfr_title);
				}
				else
				{
					dEXT_CH__PHY_PMx_SLOT01_STATUS[pm_index]->Get__DATA(wfr_sts);
					sEXT_CH__PHY_PMx_SLOT01_TITLE[pm_index]->Get__DATA(wfr_title);

					dEXT_CH__PHY_PMx_SLOT01_STATUS[pm_index]->Set__DATA(STR__NONE);
					sEXT_CH__PHY_PMx_SLOT01_TITLE[pm_index]->Set__DATA("");
				}
			}
		}
	}

	if(place_flag < 0)
	{
		if(wfr_sts.GetLength() > 0)
		{
			if(arm_type.CompareNoCase(ARM_A) == 0)
			{
				dEXT_CH__PHY_ROBOT__ARM_A_MATERIAL_STATUS->Set__DATA(wfr_sts);
				sEXT_CH__PHY_ROBOT__ARM_A_MATERIAL_TITLE->Set__DATA(wfr_title);
			}
			else if(arm_type.CompareNoCase(ARM_B) == 0)
			{
				dEXT_CH__PHY_ROBOT__ARM_B_MATERIAL_STATUS->Set__DATA(wfr_sts);
				sEXT_CH__PHY_ROBOT__ARM_B_MATERIAL_TITLE->Set__DATA(wfr_title);
			}
		}
	}

	return 1;
}
