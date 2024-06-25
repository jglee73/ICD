#include "StdAfx.h"
#include "CObj__LBx_CHM_SIMPLE.h"
#include "CObj__LBx_CHM_SIMPLE__DEF.h"
#include "CObj__LBx_CHM_SIMPLE__ALID.h"

#include "math.h"


//----------------------------------------------------------------------------------------------------
int  CObj__LBx_CHM_SIMPLE
::Check__PRESSURE_ATM(CII_OBJECT__ALARM* p_alarm,const int alarm_id)
{
LOOP_RETRY:

	// ...
	double  cur_press;
	double  ref_press;
	CString var_data;
	CString var_prs_sts;
	CString str_log;
	CString str_TM_prss_sts;

	aiEXT_CH__LBx__PRESSURE_TORR->Get__DATA(var_data);
	cur_press = atof(var_data);

	aEXT_CH__CFG_REF_ATM_PRESSURE->Get__DATA(var_data);
	ref_press = atof(var_data);

	dCH__PRESSURE_STATUS->Get__DATA(var_prs_sts);

	dEXT_CH__PHY_TM__PRESS_STS->Get__DATA(str_TM_prss_sts);

	// ...
	{
		str_log.Format("Check Prs ATM: Reference Prs:%f, Current Prs:%f, Prs Sts:%s", 
			ref_press, 
			cur_press, 
			var_prs_sts);

		Fnc__LOG(str_log);
	}

	if((cur_press < ref_press)
		|| (var_prs_sts.CompareNoCase(STR__IO_ATM)     != 0)
		|| (str_TM_prss_sts.CompareNoCase(STR__IO_ATM) != 0))
	{
		CString alarm_msg;
		CString alarm_bff;
		CString r_act;

		alarm_msg.Format("1. The pressure of %s is %s Sts.\n",
			m_sLBx__MODULE_NAME, var_prs_sts);

		alarm_bff.Format("2. The pressure of TM is %s\n",
			str_TM_prss_sts);

		alarm_msg += alarm_bff;

		alarm_bff.Format("3. The pressure of %s is %.3f and The config pressure is %.3f.\n",
			m_sLBx__MODULE_NAME, cur_press, ref_press);
		alarm_msg += alarm_bff;

		alarm_bff.Format("The %s's pressure must be more than %.3f and ATM Sts..\n",
			m_sLBx__MODULE_NAME, ref_press);
		alarm_msg += alarm_bff;

		p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,alarm_msg,r_act);

		if(r_act.CompareNoCase("RETRY") == 0)
		{	
			goto LOOP_RETRY;
		}
		return -1;
	}

	return 1;
}

int  CObj__LBx_CHM_SIMPLE
::Check__PRESSURE_ATM_TO_DV_OPEN(CII_OBJECT__ALARM* p_alarm,const int alarm_id)
{
LOOP_RETRY:

	if((diEXT_CH__LBx__DV_OPEN->Check__DATA(STR__ON)   > 0)
		&& (diEXT_CH__LBx__DV_CLOSE->Check__DATA(STR__OFF) > 0))
	{
		CString str_log;

		str_log.Format("Already... Open sts.. %s Door Valve.", m_sLBx__MODULE_NAME);
		Fnc__LOG(str_log);
		return 1;
	}

	// ...
	double  cur_press;
	double  ref_press;
	CString var_data;
	CString var_prs_sts;
	CString str_log;
	CString str_TM_prss_sts;

	aiEXT_CH__LBx__PRESSURE_TORR->Get__DATA(var_data);
	cur_press = atof(var_data);

	aEXT_CH__CFG_REF_ATM_PRESSURE->Get__DATA(var_data);
	ref_press = atof(var_data);

	dCH__PRESSURE_STATUS->Get__DATA(var_prs_sts);

	// ...
	{
		str_log.Format("Check Prs ATM: Reference Prs:%f, Current Prs:%f, Prs Sts:%s", 
			ref_press, 
			cur_press, 
			var_prs_sts);

		Fnc__LOG(str_log);
	}

	if((cur_press < ref_press)
		|| (var_prs_sts.CompareNoCase(STR__IO_ATM) != 0))
	{
		CString alarm_msg;
		CString alarm_bff;
		CString r_act;

		alarm_msg.Format("1. The pressure of %s is %s Sts.\n",
			m_sLBx__MODULE_NAME, var_prs_sts);

		alarm_bff.Format("2. The pressure of %s is %.3f and The config pressure is %.3f.\n",
			m_sLBx__MODULE_NAME, cur_press, ref_press);
		alarm_msg += alarm_bff;

		alarm_bff.Format("The %s's pressure must be more than %.3f and ATM Sts..\n",
			m_sLBx__MODULE_NAME, ref_press);
		alarm_msg += alarm_bff;

		p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,alarm_msg,r_act);

		if(r_act.CompareNoCase("RETRY") == 0)
		{	
			goto LOOP_RETRY;
		}
		return -1;
	}

	return 1;
}

int  CObj__LBx_CHM_SIMPLE
::Check__PRESSURE_VAC(CII_OBJECT__ALARM* p_alarm)
{
LOOP_RETRY:

	// ...
	double  cur_press;
	double  ref_press;
	CString var_data;
	CString var_prs_sts;
	CString str_log;
	CString str_TM_prss_sts;

	// ...
	{
		aiEXT_CH__LBx__PRESSURE_TORR->Get__DATA(var_data);
		cur_press = atof(var_data);

		aEXT_CH__CFG_REF_VAC_PRESSURE->Get__DATA(var_data);
		ref_press = atof(var_data);
	}

	// 1. Already Sns Check..    SV TM OPEN  && SV TM DOWN check, already SV OPEN  4 sensor check 
	if((diEXT_CH__LBx__SV_OPEN->Check__DATA(STR__ON)   > 0) 
	&& (diEXT_CH__LBx__SV_CLOSE->Check__DATA(STR__OFF) > 0))
	{
		// ...
		{
			str_log.Format("Already... SV TM OPEN sts.. %s Slot Valve.", m_sLBx__MODULE_NAME);

			Fnc__LOG(str_log);
		}
		return 1;
	}

	// ...
	{
		dCH__PRESSURE_STATUS->Get__DATA(var_prs_sts);
		dEXT_CH__PHY_TM__PRESS_STS->Get__DATA(str_TM_prss_sts);
	}

	// ...
	{
		str_log.Format("Check Prs VAC: Reference Prs:%f, Current Prs:%f, Prs Sts:%s, TM_PRS_STS:%s", 
						ref_press, 
						cur_press, 
						var_prs_sts, 
						str_TM_prss_sts);

		Fnc__LOG(str_log);
	}

	if((cur_press > ref_press)
	|| (var_prs_sts.CompareNoCase(STR__IO_VAC)     != 0)
	|| (str_TM_prss_sts.CompareNoCase(STR__IO_VAC) != 0))
	{
		int alarm_id = ALID__SV_OPEN__NOT_VAC_ERROR;

		CString alarm_msg;
		CString alarm_bff;
		CString r_act;

		alarm_msg.Format("1. The pressure of %s is %s Sts.\n",
						 m_sLBx__MODULE_NAME, var_prs_sts);

		alarm_bff.Format("2. The pressure of TM is %s\n",
						 str_TM_prss_sts);
		alarm_msg += alarm_bff;

		alarm_bff.Format("3. The pressure of %s is %.3f and The config pressure is %.3f.\n",
						 m_sLBx__MODULE_NAME, cur_press, ref_press);
		alarm_msg += alarm_bff;

		alarm_bff.Format("The %s's pressure must be less than %.3f and VAC Sts..\n",
						 m_sLBx__MODULE_NAME, ref_press);
		alarm_msg += alarm_bff;

		p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,alarm_msg,r_act);

		if(r_act.CompareNoCase("RETRY") == 0)
		{	
			goto LOOP_RETRY;
		}
		return -1;
	}

	return 1;
}

int  CObj__LBx_CHM_SIMPLE
::Check__LLx_LID__CLOSE(CII_OBJECT__ALARM* p_alarm,
						const CString& act_name)
{
LOOP_RETRY:

	if(!bActive__LID_CLOSE)
	{
		return 1;
	}

	if(diEXT_CH__LID_CLOSE_SNS->Check__DATA(STR__ON) > 0)
	{
		return 1;
	}

	// ...
	{
		int alm_id = ALID__LLx_LID__NOT_CLOSE;
		CString alm_msg;
		CString r_act;

		alm_msg.Format("Can not %s, because LLx's lid is not closed !\n", act_name);

		p_alarm->Popup__ALARM_With_MESSAGE(alm_id, alm_msg, r_act);

		if(r_act.CompareNoCase(ACT__RETRY) == 0)
		{
			goto LOOP_RETRY;
		}
	}
	return -1;
}
int  CObj__LBx_CHM_SIMPLE
::Check__TRANSFER_VLV__CLOSE(CII_OBJECT__ALARM* p_alarm,
							 const CString& act_name)
{
	// ...
	{
		dCH__DOOR_VALVE_STATUS->Link__UPPER_OBJECT_ABORT(sObject_Name);
		dCH__DOOR_VALVE_STATUS->Enable__UPPER_OBJECT_ABORT_CHECK();

		dCH__SLIT_VALVE_STATUS->Link__UPPER_OBJECT_ABORT(sObject_Name);
		dCH__SLIT_VALVE_STATUS->Enable__UPPER_OBJECT_ABORT_CHECK();
	}

	// ...
	CString str_log;
	CString str_temp;

LOOP_RETRY:

	// ...
	{
		Fnc__LOG("Door and Slit Valve Close Check.. Start");
		Fnc__LOG("Door Valve Close Check..");
	}

	if(iSim_Flag > 0)
	{
		diEXT_CH__LBx__SV_OPEN->Set__DATA(STR__OFF);
		diEXT_CH__LBx__SV_CLOSE->Set__DATA(STR__ON);

		diEXT_CH__LBx__DV_OPEN->Set__DATA(STR__OFF);
		diEXT_CH__LBx__DV_CLOSE->Set__DATA(STR__ON);
	}

	int nRet = dCH__DOOR_VALVE_STATUS->When__DATA(STR__CLOSE, 5.0);	// 0:Aborted, -1:Timeout

	// ...
	{
		str_log.Format("DOOR VLV STS Ret:%d", nRet);
		Fnc__LOG(str_log);
	}

	if(nRet < 0)
	{
		int alarm_id = ALID__DOOR_VALVE__NOT_CLOSE;
		CString alarm_msg;
		CString r_act;

		str_temp.Format("Can not %s, Cause Door Valve Not Close Sts !!\n",act_name);
		alarm_msg = str_temp;

		Fnc__LOG(alarm_msg);

		p_alarm->Popup__ALARM_With_MESSAGE(alarm_id, alarm_msg, r_act);

		if(r_act.CompareNoCase(ACT__RETRY) == 0)
		{
			goto LOOP_RETRY;
		}
		else if(r_act.CompareNoCase(ACT__ABORT) == 0)
		{
			return -1;
		}
	}
	else if(nRet == 0)
	{
		return OBJ_ABORT;
	}

	// ...
	{
		Fnc__LOG("Slit Valve Close Check..");
	}

	nRet = dCH__SLIT_VALVE_STATUS->When__DATA(STR__CLOSE, 5.0);

	// ...
	{
		str_log.Format("SLIT VLV STS Ret:%d", nRet);
		Fnc__LOG(str_log);
	}

	if(nRet < 0)
	{
		int alarm_id = ALID__SLIT_VALVE__NOT_CLOSE;
		CString alarm_msg;
		CString r_act;

		alarm_msg.Format("Can not %s !\n",act_name);

		p_alarm->Popup__ALARM_With_MESSAGE(alarm_id, alarm_msg, r_act);

		if(r_act.CompareNoCase(ACT__RETRY) == 0)
		{
			goto LOOP_RETRY;
		}
		// 2009, 08, 04 ADD !!
		else if(r_act.CompareNoCase(ACT__ABORT) == 0)
		{
			// ...
			return -1;
		}
	}
	else if(nRet == 0)
	{
		return OBJ_ABORT;
	}

	Fnc__LOG("Door and Slit Valve Close Check.. Complete");
	return 1;
}

int  CObj__LBx_CHM_SIMPLE
::Check__VENT_ALL_VLV__CLOSE(CII_OBJECT__ALARM* p_alarm)
{
	if(doEXT_CH__SOFT_VENT_VLV__SET->Check__DATA(STR__IO_Close) < 0)
	{
		return -1;
	}
	if(doEXT_CH__FAST_VENT_VLV__SET->Check__DATA(STR__IO_Close) < 0)
	{
		return -1;
	}
	if(doEXT_CH__ATM_EQUAL_VLV__SET->Check__DATA(STR__CLOSE) < 0)
	{
		return -1;
	}

	return 1;
}
int  CObj__LBx_CHM_SIMPLE
::Check__PUMP_ALL_VLV__CLOSE(CII_OBJECT__ALARM* p_alarm)
{
	if(doEXT_CH__SOFT_PUMP_VLV__SET->Check__DATA(STR__IO_Close) < 0)
	{
		return -1;
	}
	if(doEXT_CH__FAST_PUMP_VLV__SET->Check__DATA(STR__IO_Close) < 0)
	{
		return -1;
	}

	return 1;
}
int  CObj__LBx_CHM_SIMPLE
::Check__PUMP_VLV__OPEN(CII_OBJECT__ALARM* p_alarm)
{
	if(doEXT_CH__SOFT_PUMP_VLV__SET->Check__DATA(STR__IO_Open) > 0)
	{
		return 1;
	}
	if(doEXT_CH__FAST_PUMP_VLV__SET->Check__DATA(STR__IO_Open) > 0)
	{
		return 1;
	}

	return -1;
}

void CObj__LBx_CHM_SIMPLE
::Update__PUMP_VLV_STS(CII_OBJECT__ALARM* p_alarm)
{
	if(Check__PUMP_VLV__OPEN(p_alarm) > 0)
	{
		sCH__PUMP_VLV_OPEN_FLAG->Set__DATA("YES");
	}
	else
	{
		sCH__PUMP_VLV_OPEN_FLAG->Set__DATA("");
	}
}

int  CObj__LBx_CHM_SIMPLE
::Is_SV_CLOSE_CONDITION(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	diEXT_CH__VAC_RB_RNE->Link__UPPER_OBJECT_ABORT(sObject_Name);

LOOP_RETRY:

	if(iSim_Flag > 0)
	{
		diEXT_CH__VAC_RB_RNE->Set__DATA(STR__ON);
	}

	int r_flag = diEXT_CH__VAC_RB_RNE->When__DATA(STR__ON, 2);
	if(r_flag == 0)			return -11;

	CString var_data;
	diEXT_CH__VAC_RB_RNE->Get__DATA(var_data);

	if(var_data.CompareNoCase(STR__ON) != 0)
	{
		int alarm_id = ALID__VAC_RB_NOT_RETRACTED;

		CString err_msg;
		CString r_act;

		err_msg.Format("VAC Robot's Arm [%s] Status", var_data);	
		Fnc__LOG(err_msg);	

		p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,r_act, err_msg);

		if(r_act.CompareNoCase(ACT__RETRY) == 0)
		{
			goto LOOP_RETRY;
		}
		return -1;
	}

	return 1;
}

int  CObj__LBx_CHM_SIMPLE
::Is_DV_CLOSE_CONDITION(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	diEXT_CH__ATM_RB_RNE->Link__UPPER_OBJECT_ABORT(sObject_Name);

LOOP_RETRY:

	if(iSim_Flag > 0)
	{
		diEXT_CH__ATM_RB_RNE->Set__DATA(STR__ON);
	}

	int r_flag = diEXT_CH__ATM_RB_RNE->When__DATA(STR__ON, 2);
	if(r_flag < 0)			return -11;

	CString var_data;
	diEXT_CH__ATM_RB_RNE->Get__DATA(var_data);

	if(var_data.CompareNoCase(STR__ON) != 0)
	{
		int alarm_id = ALID__ATM_RB_NOT_RETRACTED;

		CString err_msg;
		CString r_act;

		err_msg.Format("ATM Robot's Arm [%s] Status", var_data);
		Fnc__LOG(err_msg);	

		p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,r_act, err_msg);

		if(r_act.CompareNoCase(ACT__RETRY) == 0)
		{
			goto LOOP_RETRY;
		}
		return -1;
	}

	return 1;
}

int  CObj__LBx_CHM_SIMPLE
::Fnc__VENT_ALL_VLV__CLOSE(CII_OBJECT__ALARM* p_alarm)
{
	int flag_close = 1;

	Fnc__LOG("Set... Soft Vent Vlv [Close], Fast Vent Vlv [Close]");

	// 1.
	if(doEXT_CH__SOFT_VENT_VLV__SET->Check__DATA(STR__IO_Close) < 0)
	{
		flag_close = -1;
	}
	doEXT_CH__SOFT_VENT_VLV__SET->Set__DATA(STR__IO_Close);

	// 2.
	if(doEXT_CH__FAST_VENT_VLV__SET->Check__DATA(STR__IO_Close) < 0)
	{
		flag_close = -1;
	}
	doEXT_CH__FAST_VENT_VLV__SET->Set__DATA(STR__IO_Close);

	// 3.
	if(doEXT_CH__ATM_EQUAL_VLV__SET->Check__DATA(STR__CLOSE) < 0)
	{
		flag_close = -1;
	}
	doEXT_CH__ATM_EQUAL_VLV__SET->Set__DATA(STR__CLOSE);	

	if(flag_close < 0)
	{
		SCX__TIMER_CTRL x_timer_ctrl;
		double delay_time;
		CString var_data;

		aCH__CFG_VALVE_CLOSE_DELAY_TIME->Get__DATA(var_data);
		delay_time = atof(var_data);
		if(delay_time < 0.3)	delay_time = 0.3;

		if(x_timer_ctrl->WAIT(delay_time) < 0)
		{
			return -1;
		}
	}

	return 1;
}
int  CObj__LBx_CHM_SIMPLE
::Fnc__VENT_ALL_VLV__CLOSE_WITHOUT_EQUAL_VLV(CII_OBJECT__ALARM* p_alarm)
{
	int flag_close = 1;

	// 1.
	if(doEXT_CH__SOFT_VENT_VLV__SET->Check__DATA(STR__IO_Close) < 0)
	{
		flag_close = -1;
	}
	doEXT_CH__SOFT_VENT_VLV__SET->Set__DATA(STR__IO_Close);

	// 2.
	if(doEXT_CH__FAST_VENT_VLV__SET->Check__DATA(STR__IO_Close) < 0)
	{
		flag_close = -1;
	}
	doEXT_CH__FAST_VENT_VLV__SET->Set__DATA(STR__IO_Close);

	if(flag_close < 0)
	{
		SCX__TIMER_CTRL x_timer_ctrl;
		double delay_time;
		CString var_data;

		aCH__CFG_VALVE_CLOSE_DELAY_TIME->Get__DATA(var_data);
		delay_time = atof(var_data);
		if(delay_time < 0.3)	delay_time = 0.3;

		if(x_timer_ctrl->WAIT(delay_time) < 0)
		{
			return -1;
		}
	}
	return 1;
}
int  CObj__LBx_CHM_SIMPLE
::Fnc__PUMP_ALL_VLV__CLOSE(CII_OBJECT__ALARM* p_alarm)
{
	int flag_close = 1;

	Fnc__LOG("Set... Soft Pump Vlv [Close], Fast Pump Vlv [Close]");

	// 1.
	if(doEXT_CH__SOFT_PUMP_VLV__SET->Check__DATA(STR__IO_Close) < 0)
	{
		flag_close = -1;
	}
	doEXT_CH__SOFT_PUMP_VLV__SET->Set__DATA(STR__IO_Close);

	// 2.
	if(doEXT_CH__FAST_PUMP_VLV__SET->Check__DATA(STR__IO_Close) < 0)
	{
		flag_close = -1;
	}
	doEXT_CH__FAST_PUMP_VLV__SET->Set__DATA(STR__IO_Close);

	if(flag_close < 0)
	{
		SCX__TIMER_CTRL x_timer_ctrl;
		double delay_time;
		CString var_data;

		aCH__CFG_VALVE_CLOSE_DELAY_TIME->Get__DATA(var_data);
		delay_time = atof(var_data);
		if(delay_time < 0.3)	delay_time = 0.3;

		if(x_timer_ctrl->WAIT(delay_time) < 0)
		{
			return -1;
		}
	}
	return 1;
}

int  CObj__LBx_CHM_SIMPLE
::Fnc__PUMP_SOFT_VLV__OPEN(CII_OBJECT__ALARM* p_alarm)
{
	doEXT_CH__SOFT_PUMP_VLV__SET->Set__DATA(STR__IO_Open);
	return 1;
}

int  CObj__LBx_CHM_SIMPLE
::Fnc__PUMP_SOFT_VLV__CLOSE(CII_OBJECT__ALARM* p_alarm)
{
	doEXT_CH__SOFT_PUMP_VLV__SET->Set__DATA(STR__IO_Close);
	return 1;
}

int  CObj__LBx_CHM_SIMPLE
::Fnc__PUMP_FAST_VLV__OPEN(CII_OBJECT__ALARM* p_alarm)
{
	doEXT_CH__FAST_PUMP_VLV__SET->Set__DATA(STR__IO_Open);
	return 1;
}

int  CObj__LBx_CHM_SIMPLE
::Fnc__VENT_SOFT_VLV__OPEN(CII_OBJECT__ALARM* p_alarm)
{
	doEXT_CH__SOFT_VENT_VLV__SET->Set__DATA(STR__IO_Open);
	return 1;
}
int  CObj__LBx_CHM_SIMPLE
::Fnc__VENT_SOFT_VLV__CLOSE(CII_OBJECT__ALARM* p_alarm)
{
	doEXT_CH__SOFT_VENT_VLV__SET->Set__DATA(STR__IO_Close);
	return 1;
}

int  CObj__LBx_CHM_SIMPLE
::Fnc__VENT_FAST_VLV__OPEN(CII_OBJECT__ALARM* p_alarm)
{
	doEXT_CH__FAST_VENT_VLV__SET->Set__DATA(STR__IO_Open);
	return 1;
}

int  CObj__LBx_CHM_SIMPLE
::Sub__SV_OPEN(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, double dblTimeout)
{
	CString str_log;
	CString var_data;
	double  dblDefault_Timeout = 9999.0;

	SCX__ASYNC_TIMER_CTRL xI_ASYNC_TIMER_SV;
	xI_ASYNC_TIMER_SV->REGISTER__COUNT_CHANNEL(sObject_Name,aCH_SV_OPEN__TIME_COUNT->Get__VARIABLE_NAME());

	if(iSim_Flag > 0)
	{
		diEXT_CH__LBx__SV_OPEN->Set__DATA(STR__ON);
	}

	// 1. Already Sns Check..
	if((diEXT_CH__LBx__SV_OPEN->Check__DATA(STR__ON)   > 0)
	&& (diEXT_CH__LBx__SV_CLOSE->Check__DATA(STR__OFF) > 0))
	{
		str_log.Format("Already... SV TM_OPEN sts.. %s Slot Valve.", m_sLBx__MODULE_NAME);
		Fnc__LOG(str_log);
		return 1;
	}

	if(dCH__CFG_IO_OFF_MODE->Check__DATA(STR__ENABLE) > 0)
	{
		doEXT_CH__LBx__SV_CLOSE->Set__DATA(STR__OFF);
		doEXT_CH__LBx__SV_OPEN->Set__DATA(STR__ON);
	}
	else
	{
		doEXT_CH__LBx__SV_CLOSE->Set__DATA(STR__OFF);
		Sleep(100);
		doEXT_CH__LBx__SV_OPEN->Set__DATA(STR__ON);
	}

	xI_ASYNC_TIMER_SV->START__COUNT_UP(dblDefault_Timeout);

	if(iSim_Flag > 0)
	{
		SCX__TIMER_CTRL sim_timer;

		diEXT_CH__LBx__SV_CLOSE->Set__DATA(STR__OFF);
		diEXT_CH__LBx__SV_OPEN->Set__DATA(STR__OFF);

		if(sim_timer->WAIT(0.5) < 0)
		{
			return -1;
		}

		diEXT_CH__LBx__SV_CLOSE->Set__DATA(STR__OFF);
		diEXT_CH__LBx__SV_OPEN->Set__DATA(STR__ON);
	}

	while(1)
	{
		Sleep(3);

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			return -1;
		}

		if((diEXT_CH__LBx__SV_CLOSE->Check__DATA(STR__OFF)   > 0)
		&& (diEXT_CH__LBx__SV_OPEN->Check__DATA(STR__ON) > 0))
		{
			str_log.Format("%s SV TM_OPEN Completed..", m_sLBx__MODULE_NAME);	
			Fnc__LOG(str_log);
			return 1;
		}

		if(xI_ASYNC_TIMER_SV->Get__CURRENT_TIME() >= dblTimeout)
		{
			str_log.Format("%s SV TM_OPEN Timeout..", m_sLBx__MODULE_NAME);	
			Fnc__LOG(str_log);
			break;
		}
	}

	return 0;
}

int  CObj__LBx_CHM_SIMPLE
::Sub__SV_CLOSE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, double dblTimeout)
{
	CString str_log;
	CString var_data;
	double  dblDefault_Timeout = 9999.0;

	SCX__ASYNC_TIMER_CTRL xI_ASYNC_TIMER_SV;
	xI_ASYNC_TIMER_SV->REGISTER__COUNT_CHANNEL(sObject_Name,aCH_SV_CLOSE__TIME_COUNT->Get__VARIABLE_NAME());

	// 1. Already Sns Check..
	if((diEXT_CH__LBx__SV_OPEN->Check__DATA(STR__OFF) > 0)
	&& (diEXT_CH__LBx__SV_CLOSE->Check__DATA(STR__ON) > 0))
	{
		str_log.Format("Already... SV TM_CLOSE sts.. %s Slot Valve.", m_sLBx__MODULE_NAME);
		Fnc__LOG(str_log);
		return 1;
	}

	if(dCH__CFG_IO_OFF_MODE->Check__DATA(STR__ENABLE) > 0)
	{
		doEXT_CH__LBx__SV_OPEN->Set__DATA(STR__OFF);
		doEXT_CH__LBx__SV_CLOSE->Set__DATA(STR__ON);
	}
	else
	{
		doEXT_CH__LBx__SV_OPEN->Set__DATA(STR__OFF);
		Sleep(100);
		doEXT_CH__LBx__SV_CLOSE->Set__DATA(STR__ON);
	}

	xI_ASYNC_TIMER_SV->START__COUNT_UP(dblDefault_Timeout);

	if(iSim_Flag > 0)
	{
		SCX__TIMER_CTRL sim_timer;

		diEXT_CH__LBx__SV_CLOSE->Set__DATA(STR__OFF);
		diEXT_CH__LBx__SV_OPEN->Set__DATA(STR__OFF);

		if(sim_timer->WAIT(0.5) < 0)		return -1;

		diEXT_CH__LBx__SV_CLOSE->Set__DATA(STR__ON);
		diEXT_CH__LBx__SV_OPEN->Set__DATA(STR__OFF);
	}

	while(1)
	{
		Sleep(3);

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			return -1;
		}

		if((diEXT_CH__LBx__SV_CLOSE->Check__DATA(STR__ON)   > 0)
		&& (diEXT_CH__LBx__SV_OPEN->Check__DATA(STR__OFF) > 0))
		{
			str_log.Format("%s SV TM_CLOSE Completed..", m_sLBx__MODULE_NAME);	
			Fnc__LOG(str_log);
			return 1;
		}

		if(xI_ASYNC_TIMER_SV->Get__CURRENT_TIME() >= dblTimeout)
		{
			str_log.Format("%s SV TM_CLOSE Timeout..", m_sLBx__MODULE_NAME);	
			Fnc__LOG(str_log);
			break;
		}
	}

	return 0;
}

int CObj__LBx_CHM_SIMPLE
::Check__SV_OPEN_MAX_DIFFERENTIAL_PRESS(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	double dblMax_Diff_Press_mtorr;
	double dblLLx_press_mtorr;
	double dblTm_press_mtorr;

	CString str_data;

LOOP_RETRY:

	// ...
	aCH__CFG_MAX_SV_OPEN_DIFF_PRESS->Get__DATA(str_data);
	dblMax_Diff_Press_mtorr = atof(str_data);				// mtorr..

	aiEXT_CH__LBx__PRESSURE_TORR->Get__DATA(str_data);
	dblLLx_press_mtorr = atof(str_data);					// torr

	aEXT_CH__PHY_TM__PRESS_TORR->Get__DATA(str_data);
	dblTm_press_mtorr = atof(str_data);						// torr

	if(dblMax_Diff_Press_mtorr < (fabs(dblLLx_press_mtorr-dblTm_press_mtorr)*1000)) 
	{
		CString alarm_msg;
		CString alarm_bff;
		CString r_act;

		int alarm_id = ALID__MAX_DIFFER_PRESS_ALARM;

		alarm_msg.Format("The config pressure differ Max of %s is [%.3f] mtorr.\n",
						 m_sLBx__MODULE_NAME, dblMax_Diff_Press_mtorr);

		alarm_bff.Format("The pressure difference of TM and %s is [%.3f] mtorr.\n",
						 m_sLBx__MODULE_NAME, fabs(dblLLx_press_mtorr-dblTm_press_mtorr)*1000);
		alarm_msg += alarm_bff;

		p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,alarm_msg,r_act);

		if(r_act.CompareNoCase("RETRY") == 0)
		{	
			goto LOOP_RETRY;
		}
		if(r_act.CompareNoCase("ABORT") == 0)
		{	
			return OBJ_ABORT;
		}
	}

	return 1;
}

void CObj__LBx_CHM_SIMPLE::Fnc__MSG(const CString &sMsg)
{
	sCH__MSG->Set__DATA(sMsg);
}
void CObj__LBx_CHM_SIMPLE::Fnc__LOG(const CString &sMsg)
{
	xLOG_CTRL->WRITE__LOG(sMsg);
}


int  CObj__LBx_CHM_SIMPLE
::_Update__ACTION_MIN_MAX(const int db_i,const double cur_sec)
{
	CII__VAR_STRING_CTRL* p_ch_now = sCH__TAS_ACTION_TIME_NOW[db_i].Get__PTR();
	CII__VAR_STRING_CTRL* p_ch_min = sCH__TAS_ACTION_TIME_MIN[db_i].Get__PTR();
	CII__VAR_STRING_CTRL* p_ch_max = sCH__TAS_ACTION_TIME_MAX[db_i].Get__PTR();

	CII__VAR_STRING_CTRL* p_ch_avg = sCH__TAS_ACTION_TIME_AVG[db_i].Get__PTR();
	CII__VAR_STRING_CTRL* p_ch_avg_f = sCH__TAS_ACTION_TIME_AVG_F[db_i].Get__PTR();
	CII__VAR_ANALOG_CTRL* p_ch_cnt = aCH__TAS_ACTION_TIME_CNT[db_i].Get__PTR();

	// ...
	CString ch_data;
	CString str_cur;

	str_cur.Format("%.1f", cur_sec);
	p_ch_now->Set__DATA(str_cur);

	// Avg ...
	{
		ch_data = p_ch_avg_f->Get__STRING();
		double pre_avg = atof(ch_data);
		
		double n_count = p_ch_cnt->Get__VALUE();
		p_ch_cnt->Inc__VALUE();

		double cur_avg;
		if(n_count > 0)		cur_avg = (pre_avg * (n_count - 1) / n_count) + (cur_sec / n_count);
		else				cur_avg = cur_sec;

		ch_data.Format("%.5f", cur_avg);
		p_ch_avg_f->Set__DATA(ch_data);

		ch_data.Format("%.1f", cur_avg);
		p_ch_avg->Set__DATA(ch_data);
	}
	// Min ...
	{
		ch_data = p_ch_min->Get__STRING();

		if(ch_data.GetLength() < 1)
		{
			p_ch_min->Set__DATA(str_cur);
		}
		else
		{
			double cur_min = atof(ch_data);

			if(cur_sec < cur_min)		p_ch_min->Set__DATA(str_cur);
		}
	}
	// Max ...
	{
		ch_data = p_ch_max->Get__STRING();

		if(ch_data.GetLength() < 1)
		{
			p_ch_max->Set__DATA(str_cur);
		}
		else
		{
			double cur_max = atof(ch_data);

			if(cur_sec > cur_max)		p_ch_max->Set__DATA(str_cur);
		}
	}

	return 1;
}
