#include "StdAfx.h"
#include "CObj__ATM_ROBOT_STD.h"
#include "CObj__ATM_ROBOT_STD__DEF.h"
#include "CObj__ATM_ROBOT_STD__ALID.h"

#include "math.h"
#include "Macro_Function.h"


//------------------------------------------------------------------------------------
void  CObj__ATM_ROBOT_STD::
Fnc__APP_LOG(const CString& log_msg)
{
	if(iFlag__APP_LOG < 1)		return;

	xAPP_LOG_CTRL->WRITE__LOG(log_msg);
}


// ...
void  CObj__ATM_ROBOT_STD::
Set_ANI__ROBOT_EXTEND(const CString& arm_type,
					  const CString& stn_name,
					  const CString& stn_slot)
{
	dCH__OTR_OUT_MON__ACT_ARM->Set__DATA(arm_type);

	if(arm_type.CompareNoCase(ARM_A) == 0)
	{
		dCH__OTR_OUT_MON__ARM_A_ACT->Set__DATA(STR__EXTEND);
	}
	else if(arm_type.CompareNoCase(ARM_B) == 0)
	{
		dCH__OTR_OUT_MON__ARM_B_ACT->Set__DATA(STR__EXTEND);
	}

	if(iActive_SIM > 0)
	{
		if(bActive__ROBOT_ARM_RNE_SNS)			dEXT_CH__ROBOT_ARM_RNE_SNS->Set__DATA(STR__Extend);
	}
}

void  CObj__ATM_ROBOT_STD::
Set_ANI__ROBOT_ALL_RETRACT()
{
	dCH__OTR_OUT_MON__ARM_A_ACT->Set__DATA(STR__RETRACT);
	dCH__OTR_OUT_MON__ARM_B_ACT->Set__DATA(STR__RETRACT);

	if(iActive_SIM > 0)
	{
		if(bActive__ROBOT_ARM_RNE_SNS)			dEXT_CH__ROBOT_ARM_RNE_SNS->Set__DATA(STR__None);

		for(int i=0; i<iLLx_SIZE; i++)
		{
			if(bActive__LLx_ARM_RNE_SNS[i])
				dEXT_CH__LLx_ARM_RNE_SNS[i]->Set__DATA(sDATA__LLx_ARM_RNE_ON_STS[i]);			

			if(bActive__LLx_LOAD_ENABLE_SNS[i])
				dEXT_CH__LLx_LOAD_ENABLE_SNS[i]->Set__DATA(sDATA__LLx_LOAD_ENABLE_OFF_STS[i]);
		}
	}
}
void  CObj__ATM_ROBOT_STD::
Set_ANI__ROBOT_RETRACT(const CString& arm_type,
					   const CString& stn_name,
					   const CString& stn_slot)
{
	dCH__OTR_OUT_MON__ACT_ARM->Set__DATA(arm_type);

	if(arm_type.CompareNoCase(ARM_A) == 0)
	{
		dCH__OTR_OUT_MON__ARM_A_ACT->Set__DATA(STR__RETRACT);
	}
	else if(arm_type.CompareNoCase(ARM_B) == 0)
	{
		dCH__OTR_OUT_MON__ARM_B_ACT->Set__DATA(STR__RETRACT);
	}

	if(iActive_SIM > 0)
	{
		if(bActive__ROBOT_ARM_RNE_SNS)			dEXT_CH__ROBOT_ARM_RNE_SNS->Set__DATA(STR__None);
	}
}

void  CObj__ATM_ROBOT_STD::
Set_ANI__ROBOT_ROTATE(const CString& arm_type,
					  const CString& stn_name)
{
	dCH__OTR_OUT_MON__ACT_ARM->Set__DATA(arm_type);

	dCH__OTR_OUT_MON__ARM_A_ACT->Set__DATA(STR__RETRACT);
	dCH__OTR_OUT_MON__ARM_B_ACT->Set__DATA(STR__RETRACT);

	dCH__OTR_OUT_MON__TRG_MOVE->Set__DATA(stn_name);
	dCH__OTR_OUT_MON__TRG_ROTATE->Set__DATA(stn_name);
}

int CObj__ATM_ROBOT_STD::
Check__EFEM_DOORS(CII_OBJECT__ALARM* p_alarm,
				  CII_OBJECT__VARIABLE* p_variable)
{
	CString str_data;
	CString recovery_act;

Chk_Retry:
	
	dEXT_CH__IO_EFEM_DOOR_STS->Get__DATA(str_data);

	if(str_data.CompareNoCase("None") != 0)
	{
		// Alarm Occur !!
		p_alarm->Check__ALARM(ALID__INTLK__EFEM_DOOR_NOT_CLOSE, recovery_act);
		p_alarm->Popup__ALARM(ALID__INTLK__EFEM_DOOR_NOT_CLOSE, recovery_act);

		     if(recovery_act.CompareNoCase("RETRY") == 0)	goto Chk_Retry;
		else if(recovery_act.CompareNoCase("ABORT") == 0)	return OBJ_ABORT;
		else												return OBJ_AVAILABLE;
	}

	return OBJ_AVAILABLE;	
}

int  CObj__ATM_ROBOT_STD
::Check__STN_EXIST(CII_OBJECT__ALARM* p_alarm,
				   const CString& stn_name)
{
	CString module_name;
	int i;

LOOP_CHECK:

	// LPx Check -----
	for(i=0;i<CFG_LPx__SIZE;i++)
	{
		module_name.Format("LP%1d",i+1);
		if(module_name.CompareNoCase(stn_name) != 0)
		{
			continue;
		}

		if(dEXT_CH__CFG_LPx_EXIST_FLAG[i]->Check__DATA(STR__YES) > 0)
		{
			return 1;
		}

		// ...
		{
			int alarm_id = ALID__STN__NOT_EXIST_ERROR;
			CString alarm_msg;
			CString r_act;

			alarm_msg.Format("The exist flag of (%s) STN is not \"YES\", in config page.\n",stn_name);
			p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,alarm_msg,r_act);

			if(r_act == ACT__RETRY)	
			{
				goto LOOP_CHECK;
			}
		}
		return -1;
	}

	// AL1 Check -----
	module_name = "AL1";
	if(module_name.CompareNoCase(stn_name) == 0)
	{
		if(dEXT_CH__CFG_AL1_EXIST_FLAG->Check__DATA(STR__YES) > 0)
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

			if(r_act == ACT__RETRY)
			{
				goto LOOP_CHECK;
			}
		}
		return -1;
	}

	// VIS1 Check -----
	module_name = "VIS1";
	if(module_name.CompareNoCase(stn_name) == 0)
	{
		if(dEXT_CH__CFG_VIS1_EIXST_FLAG->Check__DATA(STR__YES) > 0)
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

			if(r_act == ACT__RETRY)
			{
				goto LOOP_CHECK;
			}
		}
		return -1;
	}

	// ...
	int ll_i = Macro__CHECK_LLx_INDEX(stn_name);
	if(ll_i >= 0)
	{
		if(dEXT_CH__CFG_LLx_EXIST_FLAG[ll_i]->Check__DATA(STR__YES) > 0)
		{
			return 1;
		}

		// ...
		{
			int alarm_id = ALID__STN__NOT_EXIST_ERROR + ll_i;
			CString alarm_msg;
			CString r_act;

			alarm_msg.Format("The exist flag of (%s) STN is not \"YES\", in config page.\n",stn_name);
			p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,alarm_msg,r_act);

			if(r_act == ACT__RETRY)
			{
				goto LOOP_CHECK;
			}
		}
		return -1;
	}

	// BUFF1 Check -----
	module_name = "BUFF1";
	if(module_name.CompareNoCase(stn_name) == 0)
	{
		if(dEXT_CH__CFG_BUFF1_EXIST_FLAG->Check__DATA(STR__YES) > 0)
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

			if(r_act == ACT__RETRY)
			{
				goto LOOP_CHECK;
			}
		}
		return -1;
	}

	// BUFF2 Check -----
	module_name = "BUFF2";
	if(module_name.CompareNoCase(stn_name) == 0)
	{
		if(dEXT_CH__CFG_BUFF2_EXIST_FLAG->Check__DATA(STR__YES) > 0)
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

			if(r_act == ACT__RETRY)
			{
				goto LOOP_CHECK;
			}
		}
		return -1;
	}

	// FULL_BUFF Check -----
	module_name = "FULL_BUFF";
	if(module_name.CompareNoCase(stn_name) == 0)
	{
		if(dEXT_CH__CFG_FULL_BUFF_EXIST_FLAG->Check__DATA(STR__YES) > 0)
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

			if(r_act == ACT__RETRY)
			{
				goto LOOP_CHECK;
			}
		}
		return -1;
	}

	return 1;
}

int  CObj__ATM_ROBOT_STD::
Update__MAP_INFO(CII_OBJECT__VARIABLE* p_variable,
				 CII_OBJECT__ALARM* p_alarm,
				 const CString& stn_name)
{

	int lp_index = Macro__CHECK_LPx_INDEX(stn_name);
	if(lp_index < 0)		return -1;

	// ...
	CString str__slot_sts;
	CString var_data;
	int i;

	for(i=0;i<CFG_LPx__SLOT_SIZE;i++)
	{
		dEXT_CH__ROBOT_MAP_RESULT[i]->Get__DATA(var_data);

		if(var_data.CompareNoCase(STR__ABSENT) == 0)
		{
			str__slot_sts = STR__NONE;
		}
		else if(var_data.CompareNoCase(STR__PRESENT) == 0)
		{
			str__slot_sts = STR__EXIST;
		}
		else if(var_data.CompareNoCase(STR__CROSSED) == 0)
		{
			str__slot_sts = STR__CROSSED;
		}
		else
		{
			str__slot_sts = STR__UNKNOWN;
		}

		dEXT_CH__LPx_SLOT_MAP_STATUS[lp_index][i]->Set__DATA(str__slot_sts);

		// Crossed Wafer가 있으면.. Alarm을 Post 한다.
		if(dCH__CFG__WAFER_CROSSED_CHECK_AFTER_MAPPING->Check__DATA("TRUE") > 0)
		{
			if(str__slot_sts.CompareNoCase(STR__CROSSED) == 0)
			{
				int alarm_id = ALID__AFTER_MAPPING_CROSSED_WAFER_DETECTED;
				CString alarm_msg;
				CString r_act;

				alarm_msg.Format("After Map End,  Crossed wafer detected in the %s STN. \n",stn_name);
				p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,alarm_msg,r_act);

				if(r_act.CompareNoCase("ABORT") == 0)
				{
					return OBJ_ABORT;
				}
			}
		}
	}

	return 1;
}
