#include "StdAfx.h"
#include "CObj__IO_OBJ.h"
#include "CObj__IO_OBJ__DEF.h"
#include "CObj__IO_OBJ__ALID.h"


int  CObj__IO_OBJ::Call__INIT(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm)
{
LOOP_RETRY:

	// DI CHECK ...
	{
		CII__VAR_DIGITAL_CTRL *p_ch;
		CString ch_sts;

		int active_err = -1;
		CString err_msg;
		CString err_bff;

		int i_limit = 10;
		int i;

		for(i=0; i<i_limit; i++)
		{
			if(i == 0)
			{
				if(!bActive__DI_PUMP_STATE_RUNNING)		continue;

				p_ch = diEXT_CH__DI_PUMP_STATE_RUNNING.Get__PTR();
				ch_sts = STR__ON;
			}
			else if(i == 1)
			{
				if(!bActive__DI_PUMP_BLOWER_RUNNING)		continue;

				p_ch = diEXT_CH__DI_PUMP_BLOWER_RUNNING.Get__PTR();
				ch_sts = STR__ON;
			}
			else if(i == 2)
			{
				if(!bActive__DI_PUMP_WARNING_NORMAL)		continue;

				p_ch = diEXT_CH__DI_PUMP_WARNING_NORMAL.Get__PTR();
				ch_sts = STR__ON;
			}
			else if(i == 3)
			{
				if(!bActive__DI_PUMP_ALARM_NORMAL)			continue;

				p_ch = diEXT_CH__DI_PUMP_ALARM_NORMAL.Get__PTR();
				ch_sts = STR__ON;
			}
			else if(i == 4)
			{
				if(!bActive__DI_PUMP_OPERATION_READY)		continue;

				p_ch = diEXT_CH__DI_PUMP_OPERATION_READY.Get__PTR();
				ch_sts = STR__ON;
			}
			else if(i == 5)
			{
				if(!bActive__DI_PUMP_ALARM_ERROR)			continue;

				p_ch = diEXT_CH__DI_PUMP_ALARM_ERROR.Get__PTR();
				ch_sts = STR__OFF;
			}
			else
			{
				continue;
			}

			if(p_ch->Check__DATA(ch_sts) > 0)
			{
				continue;
			}

			// ...
			{
				active_err = 1;

				err_bff.Format(" * %s <- %s \n", 
								p_ch->Get__CHANNEL_NAME(),
								p_ch->Get__STRING());
				err_msg += err_bff;
			}
		}

		if(active_err > 0)
		{
			int alm_id = ALID__INIT_ERROR;
			CString r_act;

			p_alarm->Popup__ALARM_With_MESSAGE(alm_id,err_msg,r_act);

			if(r_act.CompareNoCase(ACT__RETRY) == 0)		
				goto LOOP_RETRY;

			return -1;
		}
	}

	// PUMP : INIT ...
	if(pOBJ_CTRL__PUMP->Call__OBJECT(sOBJ_MODE__PUMP_INIT) < 0)
	{
		return -11;
	}

	dCH__OBJ_STATUS->Set__DATA(STR__STANDBY);
	return 1;
}

int  CObj__IO_OBJ::Call__MAINT(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm)
{
	dCH__OBJ_STATUS->Set__DATA(STR__MAINTMODE);
	return 1;
}

int  CObj__IO_OBJ::Call__PUMP_ON(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm)
{
	if(_Check__INTERLOCK__PUMP_VLV(p_alarm) < 0)
	{
		return -1;
	}

	// PUMP : ON ...
	if(pOBJ_CTRL__PUMP->Call__OBJECT(sOBJ_MODE__PUMP_ON) < 0)
	{
		return -1;
	}

	// ...
	{
		SCX__TIMER_CTRL x_timer;
		x_timer->REGISTER__ABORT_OBJECT(sObject_Name);
		x_timer->REGISTER__COUNT_CHANNEL(sCH__APP_DELAY_COUNT->Get__CHANNEL_NAME());

		double cfg_sec = aCH__CFG_ON_DELAY->Get__VALUE();
		if(x_timer->WAIT(cfg_sec) < 0)		return -11;
	}
	return 1;
}
int  CObj__IO_OBJ::Call__PUMP_OFF(CII_OBJECT__VARIABLE* p_variable,	CII_OBJECT__ALARM* p_alarm)
{
	if(_Check__INTERLOCK__PUMP_VLV(p_alarm) < 0)
	{
		return -1;
	}

	// PUMP : OFF ...
	return pOBJ_CTRL__PUMP->Call__OBJECT(sOBJ_MODE__PUMP_OFF);
}

int  CObj__IO_OBJ
::_Check__INTERLOCK__PUMP_VLV(CII_OBJECT__ALARM* p_alarm)
{
LOOP_RETRY:

	// ...
	bool active__err_check = false;

	CString err_msg = "";
	CString err_bff;

	// CHECK : DO.PUMP_LINE_VALVE ...
	{
		for(int i= 0; i<iSIZE__DO_PUMP_LINE_VLV; i++)
		{
			if(doEXT_CH__DO_PUMP_LINE_VLV_X[i]->Check__DATA(STR__CLOSE) < 0)
			{
				active__err_check = true;

				err_bff.Format(" * %s <- %s \n",
								doEXT_CH__DO_PUMP_LINE_VLV_X[i]->Get__CHANNEL_NAME(),
								doEXT_CH__DO_PUMP_LINE_VLV_X[i]->Get__STRING());
				err_msg += err_bff;
			}
		}
	}
	
	// CHECK : DI.PUMP_CLOSE_VALVE ...
	{
		for(int i= 0; i<iSIZE__DI_PUMP_CLOSE_VLV; i++)
		{
			if(diEXT_CH__DI_PUMP_CLOSE_VLV_X[i]->Check__DATA(STR__ON) < 0)
			{
				active__err_check = true;

				err_bff.Format(" * %s <- %s \n",
								diEXT_CH__DI_PUMP_CLOSE_VLV_X[i]->Get__CHANNEL_NAME(),
								diEXT_CH__DI_PUMP_CLOSE_VLV_X[i]->Get__STRING());
				err_msg += err_bff;
			}
		}
	}
	// CHECK : DI.PUMP_OPEN_VALVE ...
	{
		for(int i= 0; i<iSIZE__DI_PUMP_OPEN_VLV; i++)
		{
			if(diEXT_CH__DI_PUMP_OPEN_VLV_X[i]->Check__DATA(STR__OFF) < 0)
			{
				active__err_check = true;

				err_bff.Format(" * %s <- %s \n",
								diEXT_CH__DI_PUMP_OPEN_VLV_X[i]->Get__CHANNEL_NAME(),
								diEXT_CH__DI_PUMP_OPEN_VLV_X[i]->Get__STRING());
				err_msg += err_bff;
			}
		}
	}

	if(active__err_check)
	{
		int alm_id = ALID__PUMP_VLV_NOT_CLOSE;
		CString r_act;

		p_alarm->Popup__ALARM_With_MESSAGE(alm_id, err_msg, r_act);

		if(r_act.CompareNoCase(ACT__RETRY) == 0)
		{
			goto LOOP_RETRY;
		}
		return -1;
	}

	return 1;
}
