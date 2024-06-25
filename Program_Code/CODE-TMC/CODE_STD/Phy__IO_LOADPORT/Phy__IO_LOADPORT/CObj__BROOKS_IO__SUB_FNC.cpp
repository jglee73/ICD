#include "StdAfx.h"
#include "CObj__BROOKS_IO.h"
#include "CObj__BROOKS_IO__DEF.h"

#include "math.h"


//-------------------------------- [LOG && Message] ----------------------------------------------
void CObj__BROOKS_IO::
Fnc__APP_LOG(const CString& log_msg)
{
	if(iFlag__APP_LOG < 1)		return;

	xAPP_LOG_CTRL->WRITE__LOG(log_msg);
}

int  CObj__BROOKS_IO::
Is__LP_AUTO_MODE()
{
	if(sCH__MON_ACTIVE_FA_AUTO->Check__DATA(STR__YES) > 0)
	{
		return 1;
	}

	return -1;
}

//-------------------------------------------------------------------------
// Is__Abort
int CObj__BROOKS_IO::
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

int CObj__BROOKS_IO::
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
int CObj__BROOKS_IO::
Seq_INIT(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm, const CString str_caller)
{
	CString str_log;
	CString fnc_name = "Seq_INIT";
	int state = -1;

	SCX__TIMER_CTRL xTimer;
	xTimer->REGISTER__ABORT_OBJECT(sObject_Name);

	// 1. DOOR UP.			(do.LP.Door.Up)
	// 2. DOOR RETRACT.		(do.LP.Door.Ret)
	// 3. VAC OFF			(do.LP.Vac)
	// 4. UNLATCH			(di.LP.Latch.Horizontal)
	// 5. SHUTTLE OUT		(di.LP.Shtle.Out)
	// 6. UNCLAMP			(do.LP.Unclamp)

	str_log.Format("---> From : [%s], CALL : [%s]", str_caller, fnc_name);
	Fnc__APP_LOG(str_log);

	state = Fnc__UPPADDLE(p_variable, p_alarm, fnc_name);
	if(state > 0) state = Fnc__RETRACTPADDLE(p_variable, p_alarm, fnc_name);

	if(state > 0) state = Fnc__VACOFF(p_variable, p_alarm, fnc_name);

	// if(state > 0) state = Fnc__UNLATCH(p_variable, p_alarm, fnc_name);
	if(state > 0) state = Fnc__LATCH(p_variable, p_alarm, fnc_name);

	if(state > 0) state = Fnc__SHUTTLE_OUT(p_variable, p_alarm, fnc_name);

	if(state > 0) state = Fnc__UNCLAMP(p_variable, p_alarm, fnc_name);

	str_log.Format("<--- END : From : [%s], CALL : [%s], ret:[%d]\n", str_caller, fnc_name, state);
	Fnc__APP_LOG(str_log);

	// Shuttle In ...
	if(state > 0)
	{
		str_log.Format("<--- And... : Shuttle In and.. Out Start...\n");
		Fnc__APP_LOG(str_log);

		state = Fnc__SHUTTLE_IN(p_variable, p_alarm, fnc_name);
	}

	// Shuttle Out ...
	if(state > 0)
	{
		if(xTimer->WAIT(2.0) < 0)	return OBJ_ABORT;

		state = Fnc__SHUTTLE_OUT(p_variable, p_alarm, fnc_name);
	}

	str_log.Format("<--- Complete : Init ret:[%d]\n", str_caller, fnc_name, state);
	Fnc__APP_LOG(str_log);

	return state;
}

int CObj__BROOKS_IO::
Seq_LAMP(const int lamp_id, const CString ctrl_mode, const CString str_lamp_name, const CString str_caller)
{
	CString str_log;
	CString fnc_name = "Seq_LAMP";

	// 	str_log.Format("---> From : [%s], CALL : [%s]", str_caller, fnc_name);
	// 	Fnc__APP_LOG(str_log);
	// 
	// 	str_log.Format("LAMP NAME : %s, ID : %d", str_lamp_name, lamp_id);
	// 	Fnc__APP_LOG(str_log);

	if(lamp_id == LAMP_ALL)
	{
		for (int i=0; i<LAMP_MAX; i++)
		{
			doEXT_CH__LPx__LAMP[i]->Set__DATA(ctrl_mode);
		}
	}
	else
	{
		if(lamp_id > LAMP_MAX)
		{
			Fnc__APP_LOG("UNKNOWN LAMP_ID...");
			return -1;
		}
		doEXT_CH__LPx__LAMP[lamp_id]->Set__DATA(ctrl_mode);
	}

	// 	str_log.Format("<--- END : From : [%s], CALL : [%s], ret:[1]\n", str_caller, fnc_name);
	// 	Fnc__APP_LOG(str_log);

	return 1;
}

int CObj__BROOKS_IO::
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

int  CObj__BROOKS_IO::
Is__ONLINE(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm)
{
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

int  CObj__BROOKS_IO::
Is__LOAD_CONDITION(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm)
{
	CString str_log;
	CString str_value;

Chk_Again:

	// 1. Foup Exist ??
	dCH__MON_FOUP_STATUS->Get__DATA(str_value);
	if(str_value.CompareNoCase(STR__EXIST) != 0)
	{
		str_log.Format("The state of FOUP is \"%s\" !", str_value);
		Fnc__APP_LOG(str_log);
		
		// Alarm
		int alarm_id = ALID__LOAD_FOUP_NONE_ALARM;
		CString r_act;
		p_alarm->Popup__ALARM(alarm_id,r_act);

		if(r_act.CompareNoCase(ACT__RETRY) == 0)
			goto Chk_Again;

		return -1;
	}

	// 2. ATM Robot Arm Retracted ??
	{

	}

	return 1;
}

int  CObj__BROOKS_IO::
Is__LP_CLOSE_CND(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm)
{
	CString str_log;
	CString str_value;

Chk_Again:

	if(iSIM_FLAG > 0)
	{
		diEXT_CH__LPx__WFR_SLID_OUT_SNS->Set__DATA("OFF");
	}

	if(dCFG__WAFER_SLIDE_SNS_CHECK->Check__DATA("FALSE") > 0)
	{
		Fnc__APP_LOG("Wafer Slide Sns Check Skip Ok...");
		return 1;
	}

	// 1. Wafer Slide Out Sns ??
	diEXT_CH__LPx__WFR_SLID_OUT_SNS->Get__DATA(str_value);
	if(str_value.CompareNoCase("OFF") != 0)
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

int  CObj__BROOKS_IO::
Is__Paddle_Up_Down_CND(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm)
{
	CString str_log;
	CString str_value;

Chk_Again:

	if(iSIM_FLAG > 0)
	{
		diEXT_CH__LPx__DOOR_EXT->Set__DATA("ON");
	}

	// 1. Paddle이 Extend 되어 있어야 한다.
	diEXT_CH__LPx__DOOR_EXT->Get__DATA(str_value);
	if(str_value.CompareNoCase("ON") != 0)
	{
		str_log.Format("PADDLE EXTEND SNS [%s] Status.. so Alarm Occur !!", str_value);
		Fnc__APP_LOG(str_log);

		// Alarm ...
		{
			int alarm_id = ALID__LP_PADDLE_SNS__NOT_EXT_STS_ALARM;
			CString r_act;

			p_alarm->Popup__ALARM(alarm_id,r_act);

			if(r_act.CompareNoCase("RETRY") == 0)
			{
				goto Chk_Again;
			}
			return -1;
		}
	}

	return 1;
}
