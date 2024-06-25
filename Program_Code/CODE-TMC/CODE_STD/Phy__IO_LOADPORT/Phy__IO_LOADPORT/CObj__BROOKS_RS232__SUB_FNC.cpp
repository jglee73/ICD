#include "StdAfx.h"
#include "CObj__BROOKS_RS232.h"
#include "CObj__BROOKS_RS232__DEF.h"

#include "math.h"


//-------------------------------- [LOG && Message] ----------------------------------------------
void CObj__BROOKS_RS232::
Fnc__PIO_LOG(const CString& log_msg)
{
	if(iFlag__PIO_LOG < 1)		return;

	xPIO_LOG_CTRL->WRITE__LOG(log_msg);
}

void CObj__BROOKS_RS232::
Fnc__E84_LOG(const CString& log_msg)
{
	if(iFlag__E84_LOG < 1)		return;

	xE84_LOG_CTRL->WRITE__LOG(log_msg);
}

void CObj__BROOKS_RS232::
Fnc__APP_LOG(const CString& log_msg)
{
	if(iFlag__APP_LOG < 1)		return;

	xAPP_LOG_CTRL->WRITE__LOG(log_msg);
}

void CObj__BROOKS_RS232::
Fnc__DRV_LOG(const CString& log_msg)
{
	if(iFlag__DRV_LOG < 1)		return;

	xDRV_LOG_CTRL->WRITE__LOG(log_msg);
}

void CObj__BROOKS_RS232::
Fnc__APP_MSG(const CString& log_msg)
{
	if(iFlag__APP_LOG < 1)		return;

	sCH__INR_LP__MSG->Set__DATA(log_msg);
}

int  CObj__BROOKS_RS232::
Is__LP_AUTO_MODE()
{
	if((sCH__OTR_IN_LP__FA_MODE->Check__DATA(STR__AUTO)     > 0) 
	&& (sCH__OTR_IN_LP__FA_SERVICE_MODE->Check__DATA("YES") > 0))
	{
		return 1;
	}

	return -1;
}

//-------------------------------------------------------------------------
// Is__Abort
int CObj__BROOKS_RS232::
Is__Abort(CII_OBJECT__VARIABLE* p_variable)
{
	CString szData;
	p_variable->Get__CTRL_STATUS(szData);

	if( (szData.CompareNoCase("ABORT")   == 0)
	||	(szData.CompareNoCase("ABORTED") == 0) )
	{
		return 1;
	}
	return -1;
}

int CObj__BROOKS_RS232::
Is__RELOAD_FLAG_YES(CII_OBJECT__VARIABLE* p_variable)
{
	CString sRldData, sTrnsData;
	CString logdata;
	int nRet = -1;

	Fnc__APP_LOG("Fnc_Is_RELOAD_FLAG_YES -- START !!");

	// From CTC [ABORT]
	dCH__OTR_IN_sLP_RELOAD_FLAG->Get__DATA(sRldData);

	logdata.Format("CH: [%s], DATA : [%s]",dCH__OTR_IN_sLP_RELOAD_FLAG->Get__VARIABLE_NAME(), sRldData);
	Fnc__APP_LOG(logdata);

	// Transfer [NO]
	dCH__OTR_OUT_dLP_PIO_TRANSFER->Get__DATA(sTrnsData);

	logdata.Format("CH: [%s], DATA : [%s]",dCH__OTR_OUT_dLP_PIO_TRANSFER->Get__VARIABLE_NAME(), sTrnsData);
	Fnc__APP_LOG(logdata);

	if(sRldData.CompareNoCase("YES") == 0)
	{
		if((sTrnsData.CompareNoCase("NO") == 0) && (Is__LP_AUTO_MODE() < 0))
		{
			nRet = 1;
		}
		else
		{
			nRet = -1;
		}
	}
	else
	{
		nRet = -1;
	}

	logdata.Format("Fnc_Is_RELOAD_FLAG_YES(ret : %d) -- END !!",nRet);
	Fnc__APP_LOG(logdata);

	return nRet;
}

//-------------------------------- [Functions] ----------------------------------------------
int CObj__BROOKS_RS232::
Seq_LAMP(const int lamp_id, const CString ctrl_mode, const CString str_lamp_name, const CString str_caller)
{
	CString str_log;
	CString fnc_name = "Seq_LAMP";

	if(lamp_id == LAMP_ALL)
	{
		for (int i=0; i<LAMP_MAX; i++)
		{
			sEXT_CH__LPx__LINK_LAMP_SET[i]->Set__DATA(ctrl_mode);
		}
	}
	else
	{
		if(lamp_id > LAMP_MAX)
		{
			Fnc__APP_LOG("UNKNOWN LAMP_ID...");
			return -1;
		}
		
		sEXT_CH__LPx__LINK_LAMP_SET[lamp_id]->Set__DATA(ctrl_mode);
	}

	return 1;
}

int CObj__BROOKS_RS232::
IS_ROBOT_ARM_RETRACT(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm)
{
	int ret;
	CString sTarget;
	CString sActiveArm;
	CString sArmAction;

	dCH__OTR_IN_MON_dATM_RB1_ARM->Get__DATA(sActiveArm);
	dCH__OTR_IN_MON_dATM_RB1_TRG_MOVE->Get__DATA(sTarget);

	/*
	if(m_sLPx_Name.CompareNoCase(sTarget) == 0)
	{
		if(sActiveArm.CompareNoCase("A") == 0)	
		{
			ret = dCH__OTR_IN_MON_dATM_RB1_ARM_A_ACT->Check__DATA("RETRACT");
		}
		else
		{
			ret = dCH__OTR_IN_MON_dATM_RB1_ARM_A_ACT->Check__DATA("RETRACT");
		}
		if(ret < 1)
		{
			return -1;
		}
	}
	*/

	return 1;
}

int  CObj__BROOKS_RS232::
Is__ONLINE(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm)
{
	return 1;

	// ...
	CString title;
	CString msg;

COMM_RTY:

	if(sCH__COMM_STS->Check__DATA("OFFLINE") > 0)
	{
		int alarm_id = ALID__LOADPORT_OFFLINE_ALARM;
		CString r_act;

		p_alarm->Popup__ALARM(alarm_id,r_act);

		if(r_act.CompareNoCase("RETRY") == 0)		goto 	COMM_RTY;
		else										return -1;
	}

	return 1;
}

int  CObj__BROOKS_RS232::
Is__LOAD_CONDITION(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm)
{
	CString str_log;
	CString str_value;

Chk_Again:

	// 1. Foup Exist ??
	dCH__OTR_OUT_MON_dLP_CST_STS->Get__DATA(str_value);
	if(str_value.CompareNoCase("EXIST") != 0)
	{
		str_log.Format("FOUP %s Status.. so Alarm Occur !!", str_value);
		Fnc__APP_LOG(str_log);
		
		// Alarm
		int alarm_id = ALID__LOAD_FOUP_NONE_ALARM;
		CString r_act;
		p_alarm->Popup__ALARM(alarm_id,r_act);

		if(r_act.CompareNoCase("RETRY") == 0)	goto Chk_Again;
		return -1;
	}

	// 2. ATM Robot Arm Retracted ??
	{

	}

	return 1;
}

int  CObj__BROOKS_RS232::
Is__LP_CLOSE_CND(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm)
{
	CString str_log;
	CString str_value;

Chk_Again:

	if(iSIM_FLAG > 0)
	{
		sEXT_CH__LPx__MON_WFR_SLID_OUT_SNS->Set__DATA("OFF");
	}

	if(dCFG__WAFER_SLIDE_SNS_CHECK->Check__DATA("FALSE") > 0)
	{
		Fnc__APP_LOG("Wafer Slide Sns Check Skip Ok...");
		return 1;
	}

	// 1. Wafer Slide Out Sns ??
	if(sEXT_CH__LPx__MON_WFR_SLID_OUT_SNS->Check__DATA("OFF") < 0)
	{
		str_log.Format("WAFER SLIDE OUT SNS [%s] Status.. so Alarm Occur !!", str_value);
		Fnc__APP_LOG(str_log);

		// Alarm
		int alarm_id = ALID__LP_WFR_SLIDE_SNS_ALARM;
		CString r_act;
		p_alarm->Popup__ALARM(alarm_id,r_act);

		if(r_act.CompareNoCase("RETRY") == 0)
		{
			goto Chk_Again;
		}
		return -1;
	}

	return 1;
}

