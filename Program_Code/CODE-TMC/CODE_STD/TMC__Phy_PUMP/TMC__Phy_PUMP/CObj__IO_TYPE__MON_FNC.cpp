#include "StdAfx.h"
#include "CObj__IO_TYPE.h"
#include "CObj__IO_TYPE__DEF.h"
#include "CObj__IO_TYPE__ALID.h"


// ...
void CObj__IO_TYPE
::Mon__IO_MONITOR(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	sCH__MON_PUMP_COMM_STS->Set__DATA(STR__ONLINE);

	while(1)
	{
		p_variable->Wait__SINGLE_OBJECT(0.1);

		// ...
		{
			int active__pmp_err = -1;
			int active__pmp_on  = -1;

			if((diEXT_CH__PUMP_WARNING_NORMAL->Check__DATA(STR__ON) < 0)
			&& (diEXT_CH__PUMP_ALARM_NORMAL->Check__DATA(STR__ON)   < 0)
			&& (diEXT_CH__PUMP_ALARM_ERROR->Check__DATA(STR__OFF)   < 0))
			{
				active__pmp_err = 1;
			}
			if((diEXT_CH__PUMP_STATE_RUNNING->Check__DATA(STR__ON)  > 0)
			&& (diEXT_CH__PUMP_BLOWER_RUNNING->Check__DATA(STR__ON) > 0))
			{
				active__pmp_on = 1;
			}

				 if(active__pmp_err > 0)			sCH__MON_PUMP_STATE->Set__DATA("Error");
			else if(active__pmp_on  > 0)			sCH__MON_PUMP_STATE->Set__DATA("Running");
			else									sCH__MON_PUMP_STATE->Set__DATA("Stop");

			if(active__pmp_on > 0)					sCH__MON_PUMP_RUN_STS->Set__DATA(STR__ON);
			else									sCH__MON_PUMP_RUN_STS->Set__DATA(STR__OFF);
		}

		_Check__ALM__PUMP_STATE_RUNNING(p_alarm);
		_Check__ALM__PUMP_BLOWER_RUNNING(p_alarm);
		_Check__ALM__PUMP_WARNING_NORMAL(p_alarm);
		_Check__ALM__PUMP_ALARM_NORMAL(p_alarm);
		_Check__ALM__PUMP_OPERATION_READY(p_alarm);
		_Check__ALM__PUMP_ALARM_ERROR(p_alarm);
	}	
}

void CObj__IO_TYPE
::_Check__ALM__PUMP_STATE_RUNNING(CII_OBJECT__ALARM* p_alarm)
{
	CII__VAR_DIGITAL_CTRL *p_ch = diEXT_CH__PUMP_STATE_RUNNING.Get__PTR();
	if(p_ch->Check__DATA(STR__ON) > 0)		return;

	// ...
	int alm_id = ALID__PUMP_STATE_RUNNING;

	_Check__ALM_POST(p_alarm, alm_id, p_ch);
}
void CObj__IO_TYPE
::_Check__ALM__PUMP_BLOWER_RUNNING(CII_OBJECT__ALARM* p_alarm)
{
	CII__VAR_DIGITAL_CTRL *p_ch = diEXT_CH__PUMP_BLOWER_RUNNING.Get__PTR();
	if(p_ch->Check__DATA(STR__ON) > 0)		return;

	// ...
	int alm_id = ALID__PUMP_BLOWER_RUNNING;

	_Check__ALM_POST(p_alarm, alm_id, p_ch);
}
void CObj__IO_TYPE
::_Check__ALM__PUMP_WARNING_NORMAL(CII_OBJECT__ALARM* p_alarm)
{
	CII__VAR_DIGITAL_CTRL *p_ch = diEXT_CH__PUMP_WARNING_NORMAL.Get__PTR();
	if(p_ch->Check__DATA(STR__ON) > 0)		return;

	// ...
	int alm_id = ALID__PUMP_WARNING_NORMAL;

	_Check__ALM_POST(p_alarm, alm_id, p_ch);
}
void CObj__IO_TYPE
::_Check__ALM__PUMP_ALARM_NORMAL(CII_OBJECT__ALARM* p_alarm)
{
	CII__VAR_DIGITAL_CTRL *p_ch = diEXT_CH__PUMP_WARNING_NORMAL.Get__PTR();
	if(p_ch->Check__DATA(STR__ON) > 0)		return;

	// ...
	int alm_id = ALID__PUMP_ALARM_NORMAL;;

	_Check__ALM_POST(p_alarm, alm_id, p_ch);
}
void CObj__IO_TYPE
::_Check__ALM__PUMP_OPERATION_READY(CII_OBJECT__ALARM* p_alarm)
{
	CII__VAR_DIGITAL_CTRL *p_ch = diEXT_CH__PUMP_OPERATION_READY.Get__PTR();
	if(p_ch->Check__DATA(STR__ON) > 0)		return;

	// ...
	int alm_id = ALID__PUMP_OPERATION_READY;

	_Check__ALM_POST(p_alarm, alm_id, p_ch);
}

void CObj__IO_TYPE
::_Check__ALM__PUMP_ALARM_ERROR(CII_OBJECT__ALARM* p_alarm)
{
	CII__VAR_DIGITAL_CTRL *p_ch = diEXT_CH__PUMP_ALARM_ERROR.Get__PTR();
	if(p_ch->Check__DATA(STR__OFF) > 0)		return;

	// ...
	int alm_id = ALID__PUMP_ALARM_ERROR;

	_Check__ALM_POST(p_alarm, alm_id, p_ch);
}

void CObj__IO_TYPE
::_Check__ALM_POST(CII_OBJECT__ALARM* p_alarm, 
				   const int alm_id, 
				   CII__VAR_DIGITAL_CTRL* p_ch)
{
	CString alm_msg;
	CString r_act;

	alm_msg.Format(" %s <- %s", 
					p_ch->Get__CHANNEL_NAME(),
					p_ch->Get__STRING());

	p_alarm->Check__ALARM(alm_id, r_act);
	p_alarm->Post__ALARM_With_MESSAGE(alm_id, alm_msg);
}
