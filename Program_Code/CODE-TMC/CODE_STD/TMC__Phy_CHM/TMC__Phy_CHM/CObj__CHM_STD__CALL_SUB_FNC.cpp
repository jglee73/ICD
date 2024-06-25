#include "StdAfx.h"
#include "CObj__CHM_STD.h"
#include "CObj__CHM_STD__DEF.h"
#include "CObj__CHM_STD__ALID.h"


//----------------------------------------------------------------------------------------------------
int  CObj__CHM_STD
::Check__VENT_ALL_VLV__CLOSE(CII_OBJECT__ALARM* p_alarm)
{
	if(doEXT_CH__FAST_VENT_VLV__SET->Check__DATA(STR__CLOSE) < 0)			return -1;

	if(bActive__SOFT_VENT_VLV__SET)
	{
		if(doEXT_CH__SOFT_VENT_VLV__SET->Check__DATA(STR__CLOSE) < 0)		return -1;
	}

	return 1;
}
int  CObj__CHM_STD
::Check__PUMP_ALL_VLV__CLOSE(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm)
{
	if(doEXT_CH__FAST_PUMP_VLV__SET->Check__DATA(STR__CLOSE) < 0)
	{
		return -1;
	}
	if(bActive__SOFT_PUMP_VLV__SET)
	{
		if(doEXT_CH__SOFT_PUMP_VLV__SET->Check__DATA(STR__CLOSE) < 0)		return -2;
	}

	// SR.VLV CHECK ...
	if((bActive__DI_SR_VLV_CLOSE)
	|| (bActive__DI_SR_VLV_OPEN))
	{
		if(Check__SR_VLV_CLOSE(p_variable,p_alarm, false) < 0)				return -11;
	}
	// FR.VLV CHECK ...
	if((bActive__DI_FR_VLV_CLOSE)
	|| (bActive__DI_FR_VLV_OPEN))
	{
		if(Check__FR_VLV_CLOSE(p_variable,p_alarm, false) < 0)				return -21;
	}

	return 1;
}

int  CObj__CHM_STD
::Check__PUMP_VLV__OPEN(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm)
{
	if(doEXT_CH__FAST_PUMP_VLV__SET->Check__DATA(STR__OPEN) > 0)
	{
		return 1;
	}
	if(bActive__SOFT_PUMP_VLV__SET)
	{
		if(doEXT_CH__SOFT_PUMP_VLV__SET->Check__DATA(STR__OPEN) > 0)		return 2;
	}

	// SR.VLV CHECK ...
	if((bActive__DI_SR_VLV_CLOSE)
	|| (bActive__DI_SR_VLV_OPEN))
	{
		if(Check__SR_VLV_OPEN(p_variable,p_alarm, false) > 0)				return 11;
	}
	// FR.VLV CHECK ...
	if((bActive__DI_FR_VLV_CLOSE)
	|| (bActive__DI_FR_VLV_OPEN))
	{
		if(Check__FR_VLV_OPEN(p_variable,p_alarm, false) > 0)				return 21;
	}

	return -1;
}
void CObj__CHM_STD
::Update__PUMP_VLV_STS(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm)
{
	if(Check__PUMP_VLV__OPEN(p_variable, p_alarm) > 0)
	{
		sCH__PUMP_VLV_OPEN_FLAG->Set__DATA(STR__YES);
	}
	else
	{
		sCH__PUMP_VLV_OPEN_FLAG->Set__DATA("");
	}	
}

int  CObj__CHM_STD
::Fnc__VENT_ALL_VLV__CLOSE(CII_OBJECT__ALARM* p_alarm)
{
	int flag_close = 1;

	if(doEXT_CH__FAST_VENT_VLV__SET->Check__DATA(STR__CLOSE) < 0)
	{
		flag_close = -1;
	}
	if(bActive__SOFT_VENT_VLV__SET)
	{
		if(doEXT_CH__SOFT_VENT_VLV__SET->Check__DATA(STR__CLOSE) < 0)
			flag_close = -1;
	}
	if(bActive__ATM_EQUAL_VLV)
	{
		if(doEXT_CH__ATM_EQUAL_VLV__SET->Check__DATA(STR__CLOSE) < 0)
			flag_close = -1;
	}

	//
	doEXT_CH__FAST_VENT_VLV__SET->Set__DATA(STR__CLOSE);
	
	if(bActive__SOFT_VENT_VLV__SET)
	{
		doEXT_CH__SOFT_VENT_VLV__SET->Set__DATA(STR__CLOSE);
	}
	if(bActive__ATM_EQUAL_VLV)
	{
		doEXT_CH__ATM_EQUAL_VLV__SET->Set__DATA(STR__CLOSE);
	}

	//
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
int  CObj__CHM_STD
::Fnc__VENT_ALL_VLV__CLOSE_WITHOUT_EQUAL_VLV(CII_OBJECT__ALARM* p_alarm)
{
	int flag_close = 1;

	if(doEXT_CH__FAST_VENT_VLV__SET->Check__DATA(STR__CLOSE) < 0)
	{
		flag_close = -1;
	}
	if(bActive__SOFT_VENT_VLV__SET)
	{
		if(doEXT_CH__SOFT_VENT_VLV__SET->Check__DATA(STR__CLOSE) < 0)
			flag_close = -1;
	}

	//
	doEXT_CH__FAST_VENT_VLV__SET->Set__DATA(STR__CLOSE);

	if(bActive__SOFT_VENT_VLV__SET)
	{
		doEXT_CH__SOFT_VENT_VLV__SET->Set__DATA(STR__CLOSE);
	}

	//
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
int  CObj__CHM_STD
::Fnc__PUMP_ALL_VLV__CLOSE(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm)
{
	int flag_close = 1;

	if(doEXT_CH__FAST_PUMP_VLV__SET->Check__DATA(STR__CLOSE) < 0)
	{
		flag_close = -1;
	}
	if(bActive__SOFT_PUMP_VLV__SET)
	{
		if(doEXT_CH__SOFT_PUMP_VLV__SET->Check__DATA(STR__CLOSE) < 0)
			flag_close = -1;
	}

	doEXT_CH__FAST_PUMP_VLV__SET->Set__DATA(STR__CLOSE);
	if(bActive__SOFT_PUMP_VLV__SET)
	{
		doEXT_CH__SOFT_PUMP_VLV__SET->Set__DATA(STR__CLOSE);
	}

	if(bActive__DO_BALLAST_VALVE_SET)
	{
		doEXT_CH__DO_BALLAST_VALVE_SET->Set__DATA(STR__CLOSE);
	}

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

	// SR.VLV : CLOSE CHECK ...
	if(Check__SR_VLV_CLOSE(p_variable, p_alarm, true) < 0)
	{
		return -11;
	}

	// FR.VLV : CLOSE CHECK ...
	if(Check__FR_VLV_CLOSE(p_variable, p_alarm, true) < 0)
	{
		return -21;
	}

	return 1;
}

int  CObj__CHM_STD
::Fnc__PUMP_FAST_VLV__OPEN(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm)
{
	if(bActive__SOFT_PUMP_VLV__SET)
	{
		doEXT_CH__SOFT_PUMP_VLV__SET->Set__DATA(STR__CLOSE);
	}

	doEXT_CH__FAST_PUMP_VLV__SET->Set__DATA(STR__OPEN);

	return Check__FR_VLV_OPEN(p_variable,p_alarm, true);
}
int  CObj__CHM_STD
::Fnc__PUMP_SOFT_VLV__OPEN(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm)
{
	doEXT_CH__FAST_PUMP_VLV__SET->Set__DATA(STR__CLOSE);

	if(bActive__SOFT_PUMP_VLV__SET)
	{
		doEXT_CH__SOFT_PUMP_VLV__SET->Set__DATA(STR__OPEN);
	}

	return Check__SR_VLV_OPEN(p_variable,p_alarm, true);
}

int  CObj__CHM_STD
::Fnc__VENT_FAST_VLV__OPEN(CII_OBJECT__ALARM* p_alarm)
{
	if(bActive__SOFT_VENT_VLV__SET)
		doEXT_CH__SOFT_VENT_VLV__SET->Set__DATA(STR__CLOSE);

	doEXT_CH__FAST_VENT_VLV__SET->Set__DATA(STR__OPEN);
	
	return 1;
}
int  CObj__CHM_STD
::Fnc__VENT_SOFT_VLV__OPEN(CII_OBJECT__ALARM* p_alarm)
{
	doEXT_CH__FAST_VENT_VLV__SET->Set__DATA(STR__CLOSE);

	if(bActive__SOFT_VENT_VLV__SET)
		doEXT_CH__SOFT_VENT_VLV__SET->Set__DATA(STR__OPEN);
	
	return 1;
}

void CObj__CHM_STD
::Fnc__MSG(const CString &sMsg)		
{
	sCH__MSG->Set__DATA(sMsg);
}
void CObj__CHM_STD
::Fnc__LOG(const CString &sLog)
{
	xLOG_CTRL->WRITE__LOG(sLog);
}

int CObj__CHM_STD
::Check__CHM_LID_CLOSE(CII_OBJECT__ALARM* p_alarm)
{
LOOP_RETRY:

	if(!bActive__LID_CLOSE)
	{
		return 1;
	}

	if(diEXT_CH__CHM_LID_CLOSE_SMS->Check__DATA(STR__ON) > 0)
	{
		return 1;
	}
	
	// ...
	{
		int alm_id = ALID__CHM_LID_NOT_CLOSED;
		CString alm_msg;
		CString r_act;

		alm_msg = "You can't execute the \"PUMP\" command !";

		p_alarm->Popup__ALARM_With_MESSAGE(alm_id,alm_msg,r_act);

		if(r_act.CompareNoCase(ACT__RETRY) == 0)
		{
			goto LOOP_RETRY;
		}
	}
	return -1;
}

int CObj__CHM_STD
::Check__SR_VLV_CLOSE(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm, const bool active__wait_check)
{
	return _Check__SR_VLV_STATE(p_variable,p_alarm, active__wait_check, true);
}
int CObj__CHM_STD
::Check__SR_VLV_OPEN(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm, const bool active__wait_check)
{
	return _Check__SR_VLV_STATE(p_variable,p_alarm, active__wait_check, false);
}
int CObj__CHM_STD
::_Check__SR_VLV_STATE(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm, const bool active__wait_check, const bool active__close_check)
{
LOOP_RETRY:

	// SR.VLV : CLOSE CHECK ...
	int ref__error_count = 100;

	while(1)
	{
		bool active__sr_close = true;

		// SR.VLV ...
		{
			int count__sensor_check = 0;

			if(bActive__DI_SR_VLV_CLOSE)
			{
				count__sensor_check++;

				if(diEXT_CH__DI_SR_VLV_CLOSE->Check__DATA(STR__ON) < 0)			active__sr_close = false;
			}
			if(bActive__DI_SR_VLV_OPEN)
			{
				count__sensor_check++;

				if(diEXT_CH__DI_SR_VLV_OPEN->Check__DATA(STR__ON) > 0)			active__sr_close = false;
			}

			if(count__sensor_check == 0)		return 1;
		}

		if(active__close_check)
		{
			if(active__sr_close)			return 1;
		}
		else
		{
			if(!active__sr_close)			return 1;
		}
		
		if(active__wait_check)
		{
			ref__error_count--;
			if(ref__error_count < 1)
			{
				// Alarm 
				int alm_id;
				CString alm_msg;
				CString alm_bff;
				CString r_act;

				if(active__close_check)			alm_id = ALID__SR_VALVE_NOT_CLOSE;
				else							alm_id = ALID__SR_VALVE_NOT_OPEN;

				alm_msg = "SR.Valve Sensor State  ... \n";

				if(bActive__DI_SR_VLV_CLOSE)
				{
					alm_bff.Format(" * %s <- %s \n",
									diEXT_CH__DI_SR_VLV_CLOSE->Get__CHANNEL_NAME(),
									diEXT_CH__DI_SR_VLV_CLOSE->Get__STRING());
					alm_msg += alm_bff;
				}
				if(bActive__DI_SR_VLV_OPEN)
				{
					alm_bff.Format(" * %s <- %s \n",
									diEXT_CH__DI_SR_VLV_OPEN->Get__CHANNEL_NAME(),
									diEXT_CH__DI_SR_VLV_OPEN->Get__STRING());
					alm_msg += alm_bff;
				}

				p_alarm->Popup__ALARM_With_MESSAGE(alm_id,alm_msg,r_act);

				if(r_act.CompareNoCase(ACT__RETRY) == 0)
				{
					goto LOOP_RETRY;
				}
				return -11;
			}
		}
		else
		{
			return -1;
		}

		Sleep(50);

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			return -21;
		}
	}

	return -31;
}

int CObj__CHM_STD
::Check__FR_VLV_CLOSE(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm, const bool active__wait_check)
{
	return _Check__FR_VLV_STATE(p_variable,p_alarm, active__wait_check, true);
}
int CObj__CHM_STD
::Check__FR_VLV_OPEN(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm, const bool active__wait_check)
{
	return _Check__FR_VLV_STATE(p_variable,p_alarm, active__wait_check, false);
}
int CObj__CHM_STD
::_Check__FR_VLV_STATE(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm, const bool active__wait_check, const bool active__close_check)
{
LOOP_RETRY:

	// FR.VLV : CLOSE CHECK ...
	int ref__error_count = 100;

	while(1)
	{
		bool active__fr_close = true;

		// FR.VLV ...
		{
			int count__sensor_check = 0;

			if(bActive__DI_FR_VLV_CLOSE)
			{
				count__sensor_check++;

				if(diEXT_CH__DI_FR_VLV_CLOSE->Check__DATA(STR__ON) < 0)			active__fr_close = false;
			}
			if(bActive__DI_FR_VLV_OPEN)
			{
				count__sensor_check++;

				if(diEXT_CH__DI_FR_VLV_OPEN->Check__DATA(STR__ON) > 0)			active__fr_close = false;
			}

			if(count__sensor_check == 0)		return 1;
		}

		if(active__close_check)
		{
			if(active__fr_close)			return 1;
		}
		else
		{
			if(!active__fr_close)			return 1;
		}

		if(active__wait_check)
		{
			ref__error_count--;
			if(ref__error_count < 1)
			{
				// Alarm 
				int alm_id;
				CString alm_msg;
				CString alm_bff;
				CString r_act;

				if(active__close_check)			alm_id = ALID__FR_VALVE_NOT_CLOSE;
				else							alm_id = ALID__FR_VALVE_NOT_OPEN;

				alm_msg = "FR.Valve Sensor State  ... \n";

				if(bActive__DI_FR_VLV_CLOSE)
				{
					alm_bff.Format(" * %s <- %s \n",
									diEXT_CH__DI_FR_VLV_CLOSE->Get__CHANNEL_NAME(),
									diEXT_CH__DI_FR_VLV_CLOSE->Get__STRING());
					alm_msg += alm_bff;
				}
				if(bActive__DI_FR_VLV_OPEN)
				{
					alm_bff.Format(" * %s <- %s \n",
									diEXT_CH__DI_FR_VLV_OPEN->Get__CHANNEL_NAME(),
									diEXT_CH__DI_FR_VLV_OPEN->Get__STRING());
					alm_msg += alm_bff;
				}

				p_alarm->Popup__ALARM_With_MESSAGE(alm_id,alm_msg,r_act);

				if(r_act.CompareNoCase(ACT__RETRY) == 0)
				{
					goto LOOP_RETRY;
				}
				return -11;
			}
		}
		else
		{
			return -1;
		}

		Sleep(50);

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			return -21;
		}
	}

	return -31;
}
