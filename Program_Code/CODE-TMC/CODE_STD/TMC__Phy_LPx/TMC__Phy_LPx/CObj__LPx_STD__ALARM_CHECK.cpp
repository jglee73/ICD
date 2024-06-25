#include "StdAfx.h"
#include "CObj__LPx_STD.h"
#include "CObj__LPx_STD__DEF.h"
#include "CObj__LPx_STD__ALID.h"


// ...
int  CObj__LPx_STD::Alarm_Check__ARM_NOT_RETRACTED(CII_OBJECT__ALARM* p_alarm)
{
LOOP_RETRY:

	if(bActive__ROBOT_ARM_RNE_SNS)
	{
		if(dEXT_CH__ROBOT_ARM_RNE_SNS->Check__DATA(sDATA__RNE_ON) < 0)
		{
			int alm_id = ALID__ROBOT_ARM__NOT_RETRACTED;

			CString alm_msg;
			CString alm_bff;
			CString r_act;

			alm_msg.Format(" * %s <- %s \n", 
				dEXT_CH__ROBOT_ARM_RNE_SNS->Get__CHANNEL_NAME(),
				dEXT_CH__ROBOT_ARM_RNE_SNS->Get__STRING());

			alm_bff.Format(" * RNE_ON.STATE  <- %s \n", sDATA__RNE_ON);
			alm_msg += alm_bff;

			alm_bff.Format(" * RNE_OFF.STATE <- %s \n", sDATA__RNE_OFF);
			alm_msg += alm_bff;

			p_alarm->Popup__ALARM_With_MESSAGE(alm_id, alm_msg, r_act);

			if(r_act.CompareNoCase(ACT__RETRY) == 0)		goto LOOP_RETRY;
			return -1;
		}
	}

	return 1;
}
