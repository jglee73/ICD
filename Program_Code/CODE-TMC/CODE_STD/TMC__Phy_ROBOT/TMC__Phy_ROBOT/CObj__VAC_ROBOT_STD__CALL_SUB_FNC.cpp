#include "StdAfx.h"
#include "CObj__VAC_ROBOT_STD.h"
#include "CObj__VAC_ROBOT_STD__DEF.h"
#include "CObj__VAC_ROBOT_STD__ALID.h"

#include "math.h"
#include "Macro_Function.h"


//------------------------------------------------------------------------------------
void  CObj__VAC_ROBOT_STD::
Fnc__APP_LOG(const CString& log_msg)
{
	if(iFlag__APP_LOG < 1)		return;

	xAPP_LOG_CTRL->WRITE__LOG(log_msg);
}

// ...
void  CObj__VAC_ROBOT_STD::
Set_ANI__ROBOT_EXTEND(const CString& arm_type,
					  const CString& stn_name,
					  const CString& stn_slot)
{
	CStringArray l__arm_type;
	CStringArray l__stn_name;
	CStringArray l__stn_slot;

	_Get__ARM_INFO(arm_type,stn_name,stn_slot, l__arm_type,l__stn_name,l__stn_slot);

	int i_limit = l__arm_type.GetSize();
	for(int i=0; i<i_limit; i++)
	{
		CString cur__arm_type = l__arm_type[i];
		CString cur__stn_name = l__stn_name[i];
		CString cur__stn_slot = l__stn_slot[i];

		_Set_ANI__ROBOT_EXTEND(cur__arm_type, cur__stn_name,cur__stn_slot);
	}

	if(iActive_SIM > 0)
	{
		if(arm_type.CompareNoCase(_ARM_A) == 0)
		{
			if(bActive__ROBOT_RNE_SNS__ARM_A)			dEXT_CH__ROBOT_RNE_SNS__ARM_A->Set__DATA(sDATA__RNE_OFF);
		}
		else if(arm_type.CompareNoCase(_ARM_B) == 0)
		{
			if(bActive__ROBOT_RNE_SNS__ARM_B)			dEXT_CH__ROBOT_RNE_SNS__ARM_B->Set__DATA(sDATA__RNE_OFF);
		}
	}
}
void  CObj__VAC_ROBOT_STD::
_Set_ANI__ROBOT_EXTEND(const CString& arm_type,
					   const CString& stn_name,
					   const CString& stn_slot)
{
	bool active__arm_a = false;
	bool active__arm_b = false;

		 if(arm_type.CompareNoCase(_ARM_A) == 0)		active__arm_a = true;
	else if(arm_type.CompareNoCase(_ARM_B) == 0)		active__arm_b = true;

	// Arm ...
	{
		dCH__OTR_OUT_MON__ACT_ARM->Set__DATA(arm_type);

			 if(active__arm_a)			dCH__OTR_OUT_MON__ARM_A_ACT->Set__DATA(STR__EXTEND);
		else if(active__arm_b)			dCH__OTR_OUT_MON__ARM_B_ACT->Set__DATA(STR__EXTEND);
	}

	// PM Check ...
	{
		int pm_index = Macro__CHECK_PMx_INDEX(stn_name);

		if((pm_index >= 0) && (pm_index < m_nPM_LIMIT))
		{
			sCH__OTR_OUT_MON__PMx_ARM_STATE[pm_index]->Set__DATA(STR__EXTEND);

				 if(active__arm_a)		dCH__OTR_OUT_MON__PMx_ARM_A_ACT[pm_index]->Set__DATA(STR__EXTEND);
			else if(active__arm_b)		dCH__OTR_OUT_MON__PMx_ARM_B_ACT[pm_index]->Set__DATA(STR__EXTEND);
		}
	}
	
	// LL Check ...
	{
		int ll_index = Macro__CHECK_LLx_INDEX(stn_name);

		if((ll_index >= 0) && (ll_index < iSIZE__LLx))
		{
			dCH__OTR_OUT_MON__LLx_ARM_STATE[ll_index]->Set__DATA(STR__EXTEND);

				 if(active__arm_a)		dCH__OTR_OUT_MON__LBx_ARM_A_ACT[ll_index]->Set__DATA(STR__EXTEND);
			else if(active__arm_b)		dCH__OTR_OUT_MON__LBx_ARM_B_ACT[ll_index]->Set__DATA(STR__EXTEND);
		}
	}
}

void  CObj__VAC_ROBOT_STD
::Set_ANI__ROBOT_ALL_RETRACT()
{
	dCH__OTR_OUT_MON__ARM_A_ACT->Set__DATA(STR__RETRACT);
	dCH__OTR_OUT_MON__ARM_B_ACT->Set__DATA(STR__RETRACT);

	// ...
	int i;

	for(i=0; i<m_nPM_LIMIT; i++)
	{
		sCH__OTR_OUT_MON__PMx_ARM_STATE[i]->Set__DATA(STR__RETRACT);
		
		dCH__OTR_OUT_MON__PMx_ARM_A_ACT[i]->Set__DATA(STR__RETRACT);
		dCH__OTR_OUT_MON__PMx_ARM_B_ACT[i]->Set__DATA(STR__RETRACT);
	}

	for(i=0; i<iSIZE__LLx; i++)
	{
		dCH__OTR_OUT_MON__LLx_ARM_STATE[i]->Set__DATA(STR__RETRACT);

		dCH__OTR_OUT_MON__LBx_ARM_A_ACT[i]->Set__DATA(STR__RETRACT);
		dCH__OTR_OUT_MON__LBx_ARM_B_ACT[i]->Set__DATA(STR__RETRACT);
	}

	if(iActive_SIM > 0)
	{
		if(bActive__ROBOT_RNE_SNS__ARM_A)			dEXT_CH__ROBOT_RNE_SNS__ARM_A->Set__DATA(sDATA__RNE_ON);
		if(bActive__ROBOT_RNE_SNS__ARM_B)			dEXT_CH__ROBOT_RNE_SNS__ARM_B->Set__DATA(sDATA__RNE_ON);
	}
}
void  CObj__VAC_ROBOT_STD
::Set_ANI__ROBOT_RETRACT(const CString& arm_type,
					     const CString& stn_name,
					     const CString& stn_slot)
{
	CStringArray l__arm_type;
	CStringArray l__stn_name;
	CStringArray l__stn_slot;

	_Get__ARM_INFO(arm_type,stn_name,stn_slot, l__arm_type,l__stn_name,l__stn_slot);

	int i_limit = l__arm_type.GetSize();
	for(int i=0; i<i_limit; i++)
	{
		CString cur__arm_type = l__arm_type[i];
		CString cur__stn_name = l__stn_name[i];
		CString cur__stn_slot = l__stn_slot[i];

		_Set_ANI__ROBOT_RETRACT(cur__arm_type, cur__stn_name,cur__stn_slot);
	}

	if(iActive_SIM > 0)
	{
		if(arm_type.CompareNoCase(_ARM_A) == 0)
		{
			if(bActive__ROBOT_RNE_SNS__ARM_A)			dEXT_CH__ROBOT_RNE_SNS__ARM_A->Set__DATA(sDATA__RNE_ON);
		}
		else if(arm_type.CompareNoCase(_ARM_B) == 0)
		{
			if(bActive__ROBOT_RNE_SNS__ARM_B)			dEXT_CH__ROBOT_RNE_SNS__ARM_B->Set__DATA(sDATA__RNE_ON);
		}
	}
}
void  CObj__VAC_ROBOT_STD
::_Set_ANI__ROBOT_RETRACT(const CString& arm_type,
						  const CString& stn_name,
						  const CString& stn_slot)
{
	bool active__arm_a = false;
	bool active__arm_b = false;

		 if(arm_type.CompareNoCase(_ARM_A) == 0)			active__arm_a = true;
	else if(arm_type.CompareNoCase(_ARM_B) == 0)			active__arm_b = true;

	// ...
	dCH__OTR_OUT_MON__ACT_ARM->Set__DATA(arm_type);

		 if(active__arm_a)			dCH__OTR_OUT_MON__ARM_A_ACT->Set__DATA(STR__RETRACT);
	else if(active__arm_b)			dCH__OTR_OUT_MON__ARM_B_ACT->Set__DATA(STR__RETRACT);

	// PM Check ...
	int pm_index = Macro__CHECK_PMx_INDEX(stn_name);
	if((pm_index >= 0) && (pm_index < m_nPM_LIMIT))
	{
		sCH__OTR_OUT_MON__PMx_ARM_STATE[pm_index]->Set__DATA(STR__RETRACT);

			 if(active__arm_a)		dCH__OTR_OUT_MON__PMx_ARM_A_ACT[pm_index]->Set__DATA(STR__RETRACT);
		else if(active__arm_b)		dCH__OTR_OUT_MON__PMx_ARM_B_ACT[pm_index]->Set__DATA(STR__RETRACT);
	}

	// LL Check ...
	int ll_index = Macro__CHECK_LLx_INDEX(stn_name);
	if((ll_index >= 0) && (ll_index < iSIZE__LLx))
	{
		dCH__OTR_OUT_MON__LLx_ARM_STATE[ll_index]->Set__DATA(STR__RETRACT);

			 if(active__arm_a)		dCH__OTR_OUT_MON__LBx_ARM_A_ACT[ll_index]->Set__DATA(STR__RETRACT);
		else if(active__arm_b)		dCH__OTR_OUT_MON__LBx_ARM_B_ACT[ll_index]->Set__DATA(STR__RETRACT);
	}
}

void  CObj__VAC_ROBOT_STD
::Set_ANI__ROBOT_ROTATE(const CString& arm_type,
					    const CString& stn_name,
					    const CString& stn_slot)
{
	dCH__OTR_OUT_MON__ACT_ARM->Set__DATA(arm_type);

	dCH__OTR_OUT_MON__ARM_A_ACT->Set__DATA(STR__RETRACT);
	dCH__OTR_OUT_MON__ARM_B_ACT->Set__DATA(STR__RETRACT);

	_Set_ANI__ROBOT_ROTATE(arm_type, stn_name, stn_slot);
}
void  CObj__VAC_ROBOT_STD
::_Set_ANI__ROBOT_ROTATE(const CString& arm_type,
						 const CString& stn_name,
						 const CString& stn_slot)
{
	CString ani_str;

	if(arm_type.CompareNoCase(_ARM_AB) == 0)
	{
		int ll_i = Macro__CHECK_LLx_INDEX(stn_name);
		int pm_i = Macro__CHECK_PMx_INDEX(stn_name);

		if(ll_i >= 0)
		{
			int id_left  = 1;
			int id_right = 2;

			ani_str.Format("%s-%1d%1d", stn_name,id_left,id_right);
		}
		else if(pm_i >= 0)
		{
			// 0 1 : 0 * 2 + 1 -> 1
			// 2 3 : 1 * 2 + 1 -> 3
            // 4 5 : 2 * 2 + 1 -> 5
			int pm_id = ((pm_i / 2) * 2) + 1;

			int id_left  = pm_id;
			int id_right = id_left + 1;

			ani_str.Format("PM%1d%1d", id_left,id_right);
		}
	}
	else
	{
		if(bActive__LLx_MULTI_SLOT_VALVE)
		{
			int ll_i = Macro__CHECK_LLx_INDEX(stn_name);
			if(ll_i >= 0)		ani_str.Format("%s-%s", stn_name,stn_slot);
		}
	}

	if(ani_str.GetLength() > 0)
	{
		dCH__OTR_OUT_MON__TRG_MOVE->Set__DATA(ani_str);
		dCH__OTR_OUT_MON__TRG_ROTATE->Set__DATA(ani_str);
	}
	else
	{
		dCH__OTR_OUT_MON__TRG_MOVE->Set__DATA(stn_name);
		dCH__OTR_OUT_MON__TRG_ROTATE->Set__DATA(stn_name);
	}

	if(iActive_SIM > 0)
	{
		if(arm_type.CompareNoCase(_ARM_A) == 0)
		{
			if(bActive__ROBOT_RNE_SNS__ARM_A)			dEXT_CH__ROBOT_RNE_SNS__ARM_A->Set__DATA(sDATA__RNE_ON);
		}
		else if(arm_type.CompareNoCase(_ARM_B) == 0)
		{
			if(bActive__ROBOT_RNE_SNS__ARM_B)			dEXT_CH__ROBOT_RNE_SNS__ARM_B->Set__DATA(sDATA__RNE_ON);
		}
		else if(arm_type.CompareNoCase(_ARM_AB) == 0)
		{
			if(bActive__ROBOT_RNE_SNS__ARM_A)			dEXT_CH__ROBOT_RNE_SNS__ARM_A->Set__DATA(sDATA__RNE_ON);
			if(bActive__ROBOT_RNE_SNS__ARM_B)			dEXT_CH__ROBOT_RNE_SNS__ARM_B->Set__DATA(sDATA__RNE_ON);
		}
	}
}


int  CObj__VAC_ROBOT_STD
::Check__STN_EXIST(CII_OBJECT__ALARM* p_alarm, const CString& stn_name,const CString& stn_slot)
{
	CString module_name;
	int i;

LOOP_CHECK:

	// LLx ...
	int ll_i = Macro__CHECK_LLx_INDEX(stn_name);
	if(ll_i >= 0)
	{
		if(bActive__LLx_MULTI_SLOT_VALVE)
		{
			if(stn_slot.CompareNoCase("1") == 0)		
			{
				if(dEXT_CH__CFG_LLx_1_EXIST_FLAG[ll_i]->Check__DATA(STR__YES) > 0)			return 1;
			}
			else if(stn_slot.CompareNoCase("2") == 0)		
			{
				if(dEXT_CH__CFG_LLx_2_EXIST_FLAG[ll_i]->Check__DATA(STR__YES) > 0)			return 1;
			}
		}
		else
		{
			if(dEXT_CH__CFG_LLx_EXIST_FLAG[ll_i]->Check__DATA(STR__YES) > 0)			return 1;
		}

		// ...
		{
			int alarm_id = ALID__STN__NOT_EXIST_ERROR + ll_i;
			CString alarm_msg;
			CString r_act;

			alarm_msg.Format("The exist flag of (%s) STN is not \"YES\", in config page.\n",stn_name);
			p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,alarm_msg,r_act);

			if(r_act == ACT__RETRY)			goto LOOP_CHECK;
		}
		return -1;
	}

	// PMx -----
	for(i=0;i<m_nPM_LIMIT;i++)
	{
		module_name.Format("PM%1d",i+1);
		if(module_name.CompareNoCase(stn_name) != 0)
		{
			continue;
		}

		if(dEXT_CH__CFG_PMx_EXIST_FLAG[i]->Check__DATA(STR__YES) > 0)
		{
			return 1;
		}

		//.....
		{
			int alarm_id = ALID__STN__NOT_EXIST_ERROR;
			CString alarm_msg;
			CString r_act;

			alarm_msg.Format("The exist flag of (%s) STN is not \"YES\", in config page.\n",stn_name);
			p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,alarm_msg,r_act);

			if(r_act == ACT__RETRY)			goto LOOP_CHECK;
		}
		return -1;
	}

	return 1;
}
