#include "StdAfx.h"
#include "CObj__OHT_E84_IO.h"
#include "CObj__OHT_E84_IO__DEF.h"


//-------------------------------------------------------------------------	
int  CObj__OHT_E84_IO::
Call__INIT(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	return Fnc__INIT(p_variable, p_alarm);
}


int  CObj__OHT_E84_IO::
Fnc__LOAD(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	int state = OBJ_ABORT;

CHK_RTY:

	if(iActive__SIM_MODE > 0)
	{
		dEXT_CH__LPx_IO__FOUP_PLACED_DI->Set__DATA(_OFF);
		dEXT_CH__LPx_IO__FOUP_PRESENT_DI->Set__DATA(_OFF);

		Sleep(1000);
	}

	if(dCH__CFG_E84LOAD_CLAMP_CHECK->Check__DATA("TRUE") > 0)
	{
		if(dEXT_CH__LPx_IO__CLAMP_DI->Check__DATA(_ON) > 0)
		{
			// Alarm..
			int alarm_id = ALID__LP_E84A_LOAD_CLAMP_CHECK;
			CString r_act;

			p_alarm->Popup__ALARM(alarm_id, r_act);

			if(r_act.CompareNoCase(ACT__RETRY) == 0)
			{
				goto CHK_RTY;
			}
			return OBJ_ABORT;
		}
	}

	// ...
	{
		dCH__REPORT_PIO_E84_RUN_STATE->Set__DATA(_RUN); 
		dEXT_CH__LINK_PIO_TRANSFER_STATE->Set__DATA(_YES);

		xPIO_LOG_CTRL->WRITE__LOG("<< LOAD START !!>>");
		xPIO_LOG_CTRL->WRITE__LOG("      IN[ VALID  CS_0  COMPT  CS_1  TR_REQ  BUSY  CONT ] OUT[ L_REQ  U_REQ  READY  HO_AVBL  ES     ]");

		state = Fnc__LOAD_PIO(p_variable, p_alarm);
		if(state == SEQ_COMPLETE)
		{
			dCH__REPORT_PIO_E84_RUN_STATE->Set__DATA(_DONE); 
			xPIO_LOG_CTRL->WRITE__LOG("<< LOAD COMPLETE !!>> \n");
			
			_sleep(1000);
		}
		else
		{
			xPIO_LOG_CTRL->WRITE__LOG("<< LOAD FAIL !!>> \n");
		}

		Is_CS_VALID_OFF(p_variable, p_alarm);
	}

	return state;	
}

int  CObj__OHT_E84_IO::
Fnc__UNLOAD(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	if(iActive__SIM_MODE > 0)
	{
		dEXT_CH__LPx_IO__FOUP_PLACED_DI->Set__DATA(_ON);
		dEXT_CH__LPx_IO__FOUP_PRESENT_DI->Set__DATA(_ON);

		Sleep(1000);
	}

	if(Fnc__UNCLAMP(p_variable, p_alarm, "E84_UNLOAD") != OBJ_AVAILABLE)
	{
		return OBJ_ABORT;
	}

	dCH__REPORT_PIO_E84_RUN_STATE->Set__DATA(_RUN);
	dEXT_CH__LINK_PIO_TRANSFER_STATE->Set__DATA(_YES);

	xPIO_LOG_CTRL->WRITE__LOG("<< UNLOAD START !!>>");
	xPIO_LOG_CTRL->WRITE__LOG("      IN[ VALID  CS_0  COMPT  CS_1  TR_REQ  BUSY  CONT ] OUT[ L_REQ  U_REQ  READY  HO_AVBL  ES     ]");

	int state = Fnc__UNLOAD_PIO(p_variable, p_alarm);
	if(state == SEQ_COMPLETE)
	{
		dCH__REPORT_PIO_E84_RUN_STATE->Set__DATA(_DONE);
		xPIO_LOG_CTRL->WRITE__LOG("<< UNLOAD COMPLETE !!>> \n");

		_sleep(1000);
	}
	else
	{
		xPIO_LOG_CTRL->WRITE__LOG("<< UNLOAD FAIL !!>> \n");
	}

	Is_CS_VALID_OFF(p_variable, p_alarm);
	return state;
}
