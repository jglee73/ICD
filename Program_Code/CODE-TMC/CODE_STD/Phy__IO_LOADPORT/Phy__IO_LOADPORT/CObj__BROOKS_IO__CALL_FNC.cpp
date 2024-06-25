#include "StdAfx.h"
#include "CObj__BROOKS_IO.h"
#include "CObj__BROOKS_IO__DEF.h"


//-------------------------------------------------------------------------	
int  CObj__BROOKS_IO::
Call__INIT(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	CString fnc_name = "Call__INIT()";

	int state = Fnc__INIT(p_variable, p_alarm, fnc_name);
	if(state > 0)
	{
		dCH__OTR_OUT_dLP_STATUS->Set__DATA("STANDBY");
	}

	dPIO_COMM_READY_STS->Set__DATA("READY");
	return state;
}

int  CObj__BROOKS_IO::
Call__PREPLOAD(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	CString fnc_name = "Call__PREPLOAD";

	return Fnc__PREPLOAD(p_variable, p_alarm, fnc_name);
}

int  CObj__BROOKS_IO::
Call__LOAD(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	int state = -1;
	CString fnc_name = "Call__LOAD";
	CString str_var;

	SCX__TIMER_CTRL xTimer;
	xTimer->REGISTER__ABORT_OBJECT(sObject_Name);

	if(dCFG__BEFORE_LOAD_CST_CHECK->Check__DATA("TRUE") > 0)
	{
		state = Is__LOAD_CONDITION(p_variable, p_alarm);
	}
	else
	{
		state = 1;
	}

	// ...
	dCFG__LOAD_MODE->Get__DATA(str_var);

	if(str_var.CompareNoCase("MODE1") == 0)
	{
		if(state > 0)
		{
			state = Fnc__LOAD(p_variable, p_alarm, fnc_name);
		}
	}
	else
	{
		if(state > 0)
		{
			state = Fnc__DOCK_and_EXTEND(p_variable, p_alarm, fnc_name);
		}
	}

	if(state > 0)	
	{
		aCH__OTR_IN_CFG__LOAD_COMPT_DELAY_TIME->Get__DATA(str_var);

		if(xTimer->WAIT(atof(str_var)) < 0)		return OBJ_ABORT;
	}
	return state;	
}

int  CObj__BROOKS_IO::
Call__UNLOAD(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	// ACTIVE.CHECK ...
	{
		sCH__ACTIVE_CHECK_FOUP_PRESENT_DI->Set__DATA(STR__YES);
		sCH__ACTIVE_CHECK_FOUP_PLACED_DI->Set__DATA(STR__YES);
	}

	// ...
	CString fnc_name = "Call__UNLOAD";

	// ...
	int r_flag;

	if(Is__LP_AUTO_MODE() > 0)
	{
		r_flag = Fnc__RLSUNLOAD(p_variable, p_alarm, fnc_name);
		if(r_flag < 0)			return r_flag;
	}
	else
	{
		CString cfg_option = dCH__CFG_UNLD_CLAMP_OPT->Get__STRING();

		if(cfg_option.CompareNoCase(STR__BUTTON) == 0)
			r_flag = Fnc__RLSUNLOAD(p_variable, p_alarm, fnc_name);
		else
			r_flag = Fnc__UNLOAD(p_variable, p_alarm, fnc_name);

		if(r_flag < 0)			return r_flag;
	}

	// ACTIVE.ERROR ...
	{
		sCH__ACTIVE_CHECK_FOUP_PRESENT_DI->Set__DATA(STR__ERROR);
		sCH__ACTIVE_CHECK_FOUP_PLACED_DI->Set__DATA(STR__ERROR);
	}

	do 
	{
		Sleep(20);

		if(p_variable->Check__CTRL_ABORT() > 0)	
		{
			return -11;
		}

		// Manual Mode
		if(Is__LP_AUTO_MODE() < 0)
		{
			CString cfg_option = dCH__CFG_UNLD_CLAMP_OPT->Get__STRING();
			
			if(iSIM_FLAG > 0) 
			{
				if(cfg_option.CompareNoCase(STR__BUTTON) == 0)
				{
					MessageBox(NULL, 
							  "Click ""UNLOAD"" button if you want to unclamp !", 
							  sObject_Name, 
							  MB_TOPMOST | MB_SETFOREGROUND | MB_OK);

					diEXT_CH__LPx__UNLOAD_BTN->Set__DATA(STR__ON);
					Sleep(2000);
					diEXT_CH__LPx__UNLOAD_BTN->Set__DATA("OFF");
				}

				// ...
				int r_flag = MessageBox(NULL, 
									    "Click ""OK"" button if you want to remove foup.", 
										sObject_Name, 
										MB_TOPMOST | MB_SETFOREGROUND | MB_OK);

				if(r_flag == IDOK)
				{
					diEXT_CH__LPx__FOUP_PRESENT->Set__DATA(STR__OFF);
					diEXT_CH__LPx__FOUP_PLACED->Set__DATA(STR__OFF);

					Sleep(500);		// Monitorning 부분과 Sync 위해..
				}
			}
		
			if(dCH__MON_FOUP_STATUS->Check__DATA(STR__NONE) > 0)
			{
				Fnc__APP_LOG("MANUAL MODE : foup removed ... state:1");
				break;
			}
		}
		else   // Auto Mode
		{
			bool active__lp_check = true;

			// 1. PIO Signal... COMPT [ON]
			if(dCH__MON_FOUP_STATUS->Check__DATA(STR__NONE) < 0) 
			{
				active__lp_check = false;
			}

			// 2. FOUP [NONE]
			if(bActive__LINK_PIO)
			{
				if(dEXT_CH__LPx_PIO__RUN_SNS->Check__DATA(STR__DONE) < 0)		
					active__lp_check = false;
			}

			if(active__lp_check)
			{
				break;
			}
		}
	} 
	while(1);

	// ACTIVE.OK ...
	{
		sCH__ACTIVE_CHECK_FOUP_PRESENT_DI->Set__DATA(STR__OK);
		sCH__ACTIVE_CHECK_FOUP_PLACED_DI->Set__DATA(STR__OK);
	}

	// Slot Init ...
	{
		for(int i=0; i<WAFER_SLOT_MAX; i++)  
		{
			sCH__RES_MAP_SLOT[i]->Set__DATA(STR__UNKNOWN);
		}
	}

	return 1;
}

int  CObj__BROOKS_IO::
Call__RLSUNLOAD(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	CString fnc_name = "Call_RLSUNLOAD()";
	return Fnc__RLSUNLOAD(p_variable, p_alarm, fnc_name);	
}

int  CObj__BROOKS_IO::
Call__CYCLE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	CString fnc_name = "Call_CYCLE()";
	CString str_log;

	double cfg_count = aCH__PARA_CYCLE_COUNT->Get__VALUE();
	aCH__CUR_CYCLE_COUNT->Set__VALUE(cfg_count);

	while(1)
	{
		Sleep(10);

		if(p_variable->Check__CTRL_ABORT() > 0)						return -1;

		str_log = "CYCLE - LOAD";
		sCH__INR_LP__MSG->Set__DATA(str_log);

		if(Fnc__LOAD(p_variable, p_alarm, fnc_name) < 0)			return -11;

		str_log = "CYCLE - UNLOAD";
		sCH__INR_LP__MSG->Set__DATA(str_log);

		if(Fnc__RLSUNLOAD(p_variable, p_alarm, fnc_name) < 0)		return -12;

		cfg_count--;
		aCH__CUR_CYCLE_COUNT->Set__VALUE(cfg_count);
		if(cfg_count < 1)		break;
	}
	return 1;
}

int  CObj__BROOKS_IO::
Call__MAP(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	CString fnc_name = "Call__MAP()";
	return Fnc__MAP(p_variable, p_alarm, fnc_name);	
}

int  CObj__BROOKS_IO::
Call__LATCH(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	CString fnc_name = "Call__LATCH()";
	return Fnc__LATCH(p_variable, p_alarm, fnc_name);
}

int  CObj__BROOKS_IO::
Call__UNLATCH(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	CString fnc_name = "Call__UNLATCH()";
	return Fnc__UNLATCH(p_variable, p_alarm, fnc_name);
}

int  CObj__BROOKS_IO::
Call__RETRACTPADDLE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	CString fnc_name = "Call__RETRACTPADDLE()";
	return Fnc__RETRACTPADDLE(p_variable, p_alarm,fnc_name);
}

int  CObj__BROOKS_IO::
Call__EXTENDPADDLE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	CString fnc_name = "Call__EXTENDPADDLE()";
	return Fnc__EXTENDPADDLE(p_variable, p_alarm,fnc_name);
}

int  CObj__BROOKS_IO::
Call__UPPADDLE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	CString fnc_name = "Call__UPPADDLE()";
	return Fnc__UPPADDLE(p_variable, p_alarm,fnc_name);
}

int  CObj__BROOKS_IO::
Call__DOWNPADDLE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	CString fnc_name = "Call__DOWNPADDLE()";
	return Fnc__DOWNPADDLE(p_variable, p_alarm,fnc_name);
}

int  CObj__BROOKS_IO::
Call__SHUTTLE_OUT(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	CString fnc_name = "Call__SHUTTLE_OUT()";

	return Fnc__SHUTTLE_OUT(p_variable, p_alarm, fnc_name);
}

int  CObj__BROOKS_IO::
Call__SHUTTLE_IN(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	CString fnc_name = "Call__SHUTTLE_IN()";

	return Fnc__SHUTTLE_IN(p_variable, p_alarm, fnc_name);
}

int  CObj__BROOKS_IO::
Call__VACON(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	CString fnc_name = "Call__VACON()";
	return Fnc__VACON(p_variable, p_alarm,fnc_name);
}

int  CObj__BROOKS_IO::
Call__VACOFF(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	CString fnc_name = "Call__VACOFF()";
	return Fnc__VACOFF(p_variable, p_alarm,fnc_name);
}

int  CObj__BROOKS_IO::
Call__CLAMP(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	CString fnc_name = "Call__CLAMP()";
	return Fnc__CLAMP(p_variable, p_alarm,fnc_name);
}

int  CObj__BROOKS_IO::
Call__UNCLAMP(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	CString fnc_name = "Call__UNCLAMP()";
	return Fnc__UNCLAMP(p_variable, p_alarm,fnc_name);
}

int  CObj__BROOKS_IO::
Call__DOOR_CLOSE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	CString fnc_name = "Call__DOOR_CLOSE()";

	return Fnc__DOOR_CLOSE(p_variable, p_alarm,fnc_name);
}

int  CObj__BROOKS_IO::
Call__DOOR_OPEN(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	CString fnc_name = "Call__DOOR_OPEN()";

	return Fnc__DOOR_OPEN(p_variable, p_alarm,fnc_name);
}

int  CObj__BROOKS_IO::
Call__OPENDOOR_WITH_NO_UNLATCH(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	CString fnc_name = "Call__OPENDOOR_WITH_NO_UNLATCH()";

	return Fnc__OPENDOOR_WITH_NO_UNLATCH(p_variable, p_alarm,fnc_name);
}
