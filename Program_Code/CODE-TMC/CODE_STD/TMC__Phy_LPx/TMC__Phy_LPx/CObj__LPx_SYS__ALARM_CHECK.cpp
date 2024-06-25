#include "StdAfx.h"
#include "CObj__LPx_SYS.h"
#include "CObj__LPx_SYS__DEF.h"
#include "CObj__LPx_SYS__ALID.h"


// ...
int  CObj__LPx_SYS
::Alarm_Check__ARM_NOT_RETRACTED(CII_OBJECT__ALARM* p_alarm)
{
LOOP_RETRY:

	// ...
	int check_count = 0;

	while(1)
	{
		bool active__arm_extend = false;

		if(bActive__DI_ROBOT_ARM_RNE_SNS)
		{
			CII__VAR_DIGITAL_CTRL* p_ch__arm_retract = dEXT_CH__ROBOT_ARM_RNE_SNS.Get__PTR();
	
			if(p_ch__arm_retract->Check__DATA(sDATA__RNE_ON) < 0)		active__arm_extend = true;
		}
		if(bActive__ROBOT_OBJ_CTRL)
		{
			CII__VAR_DIGITAL_CTRL* p_ch__arm_extend = dEXT_CH__ROBOT__MON_ACTIVE_ACTION_TO_LP.Get__PTR();

			if(p_ch__arm_extend->Check__DATA(STR__OFF) < 0)				active__arm_extend = true;
		}

		if(active__arm_extend)
		{
			check_count++;
			if(check_count < 50)
			{
				Sleep(100);
				continue;
			}

			// ...
			{
				int alm_id = ALID__ROBOT_ARM__NOT_RETRACTED;

				CString alm_msg;
				CString alm_bff;
				CString r_act;

				if(bActive__DI_ROBOT_ARM_RNE_SNS)
				{
					alm_bff.Format(" * %s <- %s \n", 
								dEXT_CH__ROBOT_ARM_RNE_SNS->Get__CHANNEL_NAME(),
								dEXT_CH__ROBOT_ARM_RNE_SNS->Get__STRING());
					alm_msg += alm_bff;
	
					alm_bff.Format(" * RNE_ON.STATE  <- %s \n", sDATA__RNE_ON);
					alm_msg += alm_bff;

					alm_bff.Format(" * RNE_OFF.STATE <- %s \n", sDATA__RNE_OFF);
					alm_msg += alm_bff;
				}

				if(bActive__ROBOT_OBJ_CTRL)
				{
					alm_bff.Format(" * %s <- %s \n", 
									dEXT_CH__ROBOT__MON_ACTIVE_ACTION_TO_LP->Get__CHANNEL_NAME(),
									dEXT_CH__ROBOT__MON_ACTIVE_ACTION_TO_LP->Get__STRING());
					alm_msg += alm_bff;
				}

				p_alarm->Popup__ALARM_With_MESSAGE(alm_id, alm_msg, r_act);
	
				if(r_act.CompareNoCase(ACT__RETRY) == 0)		goto LOOP_RETRY;
				return -1;
			}
		}
		else
		{
			// OK !!!
			break;
		}
	}

	return 1;
}

int  CObj__LPx_SYS
::Is__LOAD_CONDITION(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm)
{
LOOP_RETRY:

	// 1. Foup Exist ...
	if(dCH__MON_FOUP_STS->Check__DATA(STR__EXIST) < 0)
	{
		int alarm_id = ALID__LOAD_FOUP_NONE_ALARM;
			
		CString alm_msg;
		CString r_act;
			
		alm_msg.Format(" * %s <- %s", 
						dCH__MON_FOUP_STS->Get__CHANNEL_NAME(),
						dCH__MON_FOUP_STS->Get__STRING());
							
		p_alarm->Popup__ALARM_With_MESSAGE(alarm_id, alm_msg, r_act);

		if(r_act.CompareNoCase(ACT__RETRY) == 0)		goto LOOP_RETRY;
		return -1;
	}

	// 2. ATM Robot Arm Retracted ??
	{

	}

	return 1;
}

int  CObj__LPx_SYS
::Is__LP_CLOSE_CND(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm)
{
	CString str_log;
	CString str_value;

Chk_Again:

	if(iActive__SIM_MODE > 0)
	{
		if(bActive__DI_SLIDE_OUT_SNS)			diEXT_CH__LPx_WAFER_SLID_OUT->Set__DATA(STR__OFF);
	}

	if(dCH__CFG_WAFER_SLIDE_SNS_CHECK->Check__DATA(STR__FALSE) > 0)
	{
		return 1;
	}

	if(bActive__DI_SLIDE_OUT_SNS)			
	{
		if(diEXT_CH__LPx_WAFER_SLID_OUT->Check__DATA(STR__OFF) < 0)
		{
			int alarm_id = ALID__LP_WFR_SLIDE_SNS_ALARM;
			CString r_act;

			p_alarm->Popup__ALARM(alarm_id,r_act);
	
			if(r_act.CompareNoCase(ACT__RETRY) == 0)
			{
				goto Chk_Again;
			}
			return -1;
		}
	}

	return 1;
}

