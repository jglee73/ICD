#include "StdAfx.h"
#include "CObj__ATM_ROBOT_FUSION.h"
#include "CObj__ATM_ROBOT_FUSION__DEF.h"
#include "CObj__ATM_ROBOT_FUSION__ALID.h"

#include "math.h"
#include "Macro_Function.h"


//------------------------------------------------------------------------------------
void  CObj__ATM_ROBOT_FUSION::
Fnc__APP_LOG(const CString& log_msg)
{
	if(iFlag__APP_LOG < 1)		return;

	xAPP_LOG_CTRL->WRITE__LOG(log_msg);
}


// ...
void  CObj__ATM_ROBOT_FUSION::
Set_ANI__ROBOT_EXTEND(const CString& arm_type,
					  const CString& stn_name,
					  const CString& stn_slot)
{
	dCH__OTR_OUT_MON__ACT_ARM->Set__DATA(arm_type);

	if((arm_type.CompareNoCase(_ARM_A)  == 0)
	|| (arm_type.CompareNoCase(_ARM_AB) == 0))
	{
		dCH__OTR_OUT_MON__ARM_A_ACT->Set__DATA(STR__EXTEND);
	}
	if((arm_type.CompareNoCase(_ARM_B)  == 0)
	|| (arm_type.CompareNoCase(_ARM_AB) == 0))
	{
		dCH__OTR_OUT_MON__ARM_B_ACT->Set__DATA(STR__EXTEND);
	}

	if(iActive__SIM_MODE > 0)
	{
		if((arm_type.CompareNoCase(_ARM_A)  == 0)			
		|| (arm_type.CompareNoCase(_ARM_AB) == 0))
		{
			if(bActive__ROBOT_RNE_SNS__ARM_A)			dEXT_CH__ROBOT_RNE_SNS__ARM_A->Set__DATA(sDATA__RNE_OFF);
		}		
		if((arm_type.CompareNoCase(_ARM_B)  == 0)			
		|| (arm_type.CompareNoCase(_ARM_AB) == 0))
		{
			if(bActive__ROBOT_RNE_SNS__ARM_B)			dEXT_CH__ROBOT_RNE_SNS__ARM_B->Set__DATA(sDATA__RNE_OFF);
		}
	}
}

void  CObj__ATM_ROBOT_FUSION::
Set_ANI__ROBOT_ALL_RETRACT()
{
	dCH__OTR_OUT_MON__ARM_A_ACT->Set__DATA(STR__RETRACT);
	dCH__OTR_OUT_MON__ARM_B_ACT->Set__DATA(STR__RETRACT);

	if(iActive__SIM_MODE > 0)
	{
		if(bActive__ROBOT_RNE_SNS__ARM_A)				dEXT_CH__ROBOT_RNE_SNS__ARM_A->Set__DATA(sDATA__RNE_ON);
		if(bActive__ROBOT_RNE_SNS__ARM_B)				dEXT_CH__ROBOT_RNE_SNS__ARM_B->Set__DATA(sDATA__RNE_ON);
	}
}
void  CObj__ATM_ROBOT_FUSION::
Set_ANI__ROBOT_RETRACT(const CString& arm_type,
					   const CString& stn_name,
					   const CString& stn_slot)
{
	dCH__OTR_OUT_MON__ACT_ARM->Set__DATA(arm_type);

	if((arm_type.CompareNoCase(_ARM_A)  == 0)
	|| (arm_type.CompareNoCase(_ARM_AB) == 0))
	{
		dCH__OTR_OUT_MON__ARM_A_ACT->Set__DATA(STR__RETRACT);
	}
	if((arm_type.CompareNoCase(_ARM_B)  == 0)
	|| (arm_type.CompareNoCase(_ARM_AB) == 0))
	{
		dCH__OTR_OUT_MON__ARM_B_ACT->Set__DATA(STR__RETRACT);
	}

	if(iActive__SIM_MODE > 0)
	{
		if((arm_type.CompareNoCase(_ARM_A)  == 0)			
		|| (arm_type.CompareNoCase(_ARM_AB) == 0))
		{
			if(bActive__ROBOT_RNE_SNS__ARM_A)			dEXT_CH__ROBOT_RNE_SNS__ARM_A->Set__DATA(sDATA__RNE_ON);
		}
		if((arm_type.CompareNoCase(_ARM_B)  == 0)			
		|| (arm_type.CompareNoCase(_ARM_AB) == 0))
		{
			if(bActive__ROBOT_RNE_SNS__ARM_B)			dEXT_CH__ROBOT_RNE_SNS__ARM_B->Set__DATA(sDATA__RNE_ON);
		}
	}
}

void  CObj__ATM_ROBOT_FUSION::
Set_ANI__ROBOT_ROTATE(const CString& arm_type,
					  const CString& stn_name,
					  const CString& stn_slot)
{
	// ...
	{
		dCH__OTR_OUT_MON__ACT_ARM->Set__DATA(arm_type);

		dCH__OTR_OUT_MON__ARM_A_ACT->Set__DATA(STR__RETRACT);
		dCH__OTR_OUT_MON__ARM_B_ACT->Set__DATA(STR__RETRACT);
	}

	if(iActive__SIM_MODE > 0)
	{
		if((arm_type.CompareNoCase(_ARM_A)  == 0)			
		|| (arm_type.CompareNoCase(_ARM_AB) == 0))
		{
			if(bActive__ROBOT_RNE_SNS__ARM_A)			dEXT_CH__ROBOT_RNE_SNS__ARM_A->Set__DATA(sDATA__RNE_ON);
		}
		if((arm_type.CompareNoCase(_ARM_B)  == 0)			
		|| (arm_type.CompareNoCase(_ARM_AB) == 0))
		{
			if(bActive__ROBOT_RNE_SNS__ARM_B)			dEXT_CH__ROBOT_RNE_SNS__ARM_B->Set__DATA(sDATA__RNE_ON);
		}
	}

	_Set_ANI__ROBOT_ROTATE(arm_type, stn_name, stn_slot);
}
void  CObj__ATM_ROBOT_FUSION
::_Set_ANI__ROBOT_ROTATE(const CString& arm_type,
						 const CString& stn_name,
						 const CString& stn_slot)
{
	if(bActive__LLx_MULTI_SLOT_VALVE)
	{
		CString ch_data;

		int ll_i = Macro__CHECK_LLx_INDEX(stn_name);

		if(ll_i >= 0)			ch_data.Format("%s-%s", stn_name,stn_slot);
		else					ch_data = stn_name;

		dCH__OTR_OUT_MON__TRG_MOVE->Set__DATA(ch_data);
		dCH__OTR_OUT_MON__TRG_ROTATE->Set__DATA(ch_data);
	}
	else
	{
		dCH__OTR_OUT_MON__TRG_MOVE->Set__DATA(stn_name);
		dCH__OTR_OUT_MON__TRG_ROTATE->Set__DATA(stn_name);
	}
}

int CObj__ATM_ROBOT_FUSION::
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

int  CObj__ATM_ROBOT_FUSION
::Check__STN_EXIST(CII_OBJECT__ALARM* p_alarm,
				   const CString& stn_name,
				   const CString& stn_slot)
{
	CString module_name;
	int i;

LOOP_CHECK:

	// LPx Check ...
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

	// AL1 Check ...
	module_name = "AL1";
	if(module_name.CompareNoCase(stn_name) == 0)
	{
		if(dEXT_CH__CFG_AL1_EXIST_FLAG->Check__DATA(STR__YES) > 0)
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

	// LLx Check ...
	{
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
				if(dEXT_CH__CFG_LLx_EXIST_FLAG[ll_i]->Check__DATA(STR__YES) > 0)				return 1;
			}

			// ...
			{
				int alarm_id = ALID__STN__NOT_EXIST_ERROR + ll_i;
				CString alarm_msg;
				CString r_act;

				alarm_msg.Format("The exist flag of %s(%s) STN is not \"YES\", in config page.\n", stn_name,stn_slot);
				p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,alarm_msg,r_act);

				if(r_act == ACT__RETRY)
				{
					goto LOOP_CHECK;
				}
			}
			return -1;
		}
	}

	// BUFF1 Check ...
	{
		if((stn_name.CompareNoCase(STR__BUFF1) == 0)
		|| (stn_name.CompareNoCase(STR__ST1) == 0))
		{
			if(dEXT_CH__CFG_BUFF1_EXIST_FLAG->Check__DATA(STR__YES) > 0)
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
	}
	// BUFF2 Check ...
	{
		if((stn_name.CompareNoCase(STR__BUFF2) == 0)
		|| (stn_name.CompareNoCase(STR__ST2) == 0))
		{
			if(dEXT_CH__CFG_BUFF2_EXIST_FLAG->Check__DATA(STR__YES) > 0)
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
	}

	// FULL_BUFF Check ...
	{
		module_name = "FULL_BUFF";
		if(module_name.CompareNoCase(stn_name) == 0)
		{
			if(dEXT_CH__CFG_FULL_BUFF_EXIST_FLAG->Check__DATA(STR__YES) > 0)
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
	}

	return 1;
}
