#include "StdAfx.h"
#include "CObj__BROOKS_IO.h"
#include "CObj__BROOKS_IO__DEF.h"


//-------------------------------------------------------------------------	
int  CObj__BROOKS_IO::
Fnc__INIT(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller)
{
	CString fnc_name = "Fnc__INIT()";

	// ...
	CString str_log;

	str_log.Format("---> From : [%s], CALL : [%s]", str_caller, fnc_name);
	Fnc__APP_LOG(str_log);

	// ...
	int r_flag;

	r_flag = Is__ONLINE(p_variable, p_alarm);
	if(r_flag < 0)				return -1;

	// Slot Init.
	if(dCH__CFG_INIT_SLOT_STATE_UNKNOWN->Check__DATA(STR__YES) > 0)
	{
		for(int i=0; i<WAFER_SLOT_MAX; i++) 
		{
			sCH__RES_MAP_SLOT[i]->Set__DATA(STR__UNKNOWN);
		}
	}

	// SET : LAMP ...
	{
		r_flag = Seq_LAMP(LAMP_ALL, "OFF", "LAMP_ALL", "Fnc__INIT()");
		if(r_flag < 0)			return -11;
	}

	// ROBOT-ARM CHECK ...
	{
		Fnc__APP_LOG("1. Robot Arm Check !");
		
		r_flag = IS_ROBOT_ARM_RETRACT(p_variable, p_alarm);
		if(r_flag < 0)			return -12;
	}

	// INIT ...
	{
		Fnc__APP_LOG("2. INIT Seq Start... !");

		r_flag = Seq_INIT(p_variable, p_alarm, fnc_name);
		if(r_flag < 0)			return -13;
	}

	str_log.Format("<--- END ...\n");
	Fnc__APP_LOG(str_log);
	return 1;
}

int  CObj__BROOKS_IO::
Fnc__PREPLOAD(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller)
{
	// ACTIVE.CHECK ...
	{
		sCH__ACTIVE_CHECK_FOUP_PRESENT_DI->Set__DATA(STR__YES);
		sCH__ACTIVE_CHECK_FOUP_PLACED_DI->Set__DATA(STR__YES);
	}

	// ...
	CString fnc_name = "Fnc__PREPLOAD()";

	// ...
	CString str_log;

	str_log.Format("---> START : %s By [%s] ", fnc_name, str_caller);
	Fnc__APP_LOG(str_log);

	// ...
	int r_flag;

	r_flag = Is__ONLINE(p_variable, p_alarm);
	if(r_flag < 0)			return -1;

	if(dCH__MON_FOUP_STATUS->Check__DATA(STR__NONE) > 0)
	{
		Fnc__UNCLAMP(p_variable, p_alarm, fnc_name);
	}

	// Wafer Exist 상태 초기화...
	for(int i=0; i<WAFER_SLOT_MAX; i++)  
	{
		sCH__RES_MAP_SLOT[i]->Set__DATA(STR__ABSENT);	// prepload이므로..
	}

	if(iSIM_FLAG > 0)
	{
		if(Is__LP_AUTO_MODE() < 0)
		{
			diEXT_CH__LPx__FOUP_PRESENT->Set__DATA(STR__ON);
			diEXT_CH__LPx__FOUP_PLACED->Set__DATA(STR__ON);

			// Latch
			diEXT_CH__LPx__LATCH->Set__DATA(STR__OFF);
			diEXT_CH__LPx__UNLATCH->Set__DATA(STR__ON);

			// Paddle.Retract
			diEXT_CH__LPx__DOOR_RET->Set__DATA(STR__ON);
			diEXT_CH__LPx__DOOR_EXT->Set__DATA(STR__OFF);

			// Door.Up
			diEXT_CH__LPx__DOOR_UP->Set__DATA(STR__ON);
			diEXT_CH__LPx__DOOR_DN->Set__DATA(STR__OFF);

			// Shuttle.Out
			diEXT_CH__LPx__SHUTTLE_IN->Set__DATA(STR__OFF);
			diEXT_CH__LPx__SHUTTLE_OUT->Set__DATA(STR__ON);

			if(dCH__MON_CLAMP_STATUS->Check__DATA(STR__CLAMP) < 0)
			{
				if(dCH__CFG_LOAD_CLAMP_OPT->Check__DATA(STR__BUTTON) > 0)
				{
					Fnc__APP_LOG("PrepLoad SimMode Waiting for button pushed... Clamp Box");

					int r_flag = MessageBox(NULL, 
											"Click ""LOAD"" button if you want to clamp", 
											sObject_Name, 
											MB_TOPMOST | MB_SETFOREGROUND | MB_OK);

					if(r_flag == IDOK)
					{
						diEXT_CH__LPx__LOAD_BTN->Set__DATA(STR__ON);

						Sleep(1000);

						diEXT_CH__LPx__LOAD_BTN->Set__DATA(STR__OFF);
					}

				}
			}
		}
	}

	// ACTIVE.ERROR ...
	{
		sCH__ACTIVE_CHECK_FOUP_PRESENT_DI->Set__DATA(STR__ERROR);
		sCH__ACTIVE_CHECK_FOUP_PLACED_DI->Set__DATA(STR__ERROR);
	}

	do 
	{
		Sleep(100);

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			return -11;
		}

		// Manual Mode
		if(Is__LP_AUTO_MODE() < 0)
		{
			// Foup Exist Check..
			if(dCH__MON_FOUP_STATUS->Check__DATA(STR__EXIST) > 0)
			{
				if(dCH__CFG_LOAD_CLAMP_OPT->Check__DATA(STR__AUTO) > 0)
				{
					r_flag = Fnc__CLAMP(p_variable, p_alarm, fnc_name);
					if(r_flag < 0)			return -21;

					break;
				}

				if(dCH__MON_CLAMP_STATUS->Check__DATA(STR__CLAMP) > 0)
				{
					break;
				}
			}
		}
		else   // Auto Mode
		{
			bool active__lp_check = true;

			if(dCH__MON_FOUP_STATUS->Check__DATA(STR__EXIST) < 0)
			{
				active__lp_check = false;
			}

			if(bActive__LINK_PIO)
			{
				if(dEXT_CH__LPx_PIO__RUN_SNS->Check__DATA("DONE") < 0)
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

	str_log.Format("<--- END ... \n");
	Fnc__APP_LOG(str_log);
	return 1;
}

int  CObj__BROOKS_IO::
Fnc__LOAD(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller)
{
	CString fnc_name = "Fnc__LOAD()";

	// ...
	CString str_log;

	str_log.Format("---> START : %s By [%s] ", fnc_name, str_caller);
	Fnc__APP_LOG(str_log);

	// ...
	int r_flag;

	r_flag = Is__ONLINE(p_variable, p_alarm);
	if(r_flag < 0)			return -1;

	// 1. Clamp
	r_flag = Fnc__CLAMP(p_variable, p_alarm, fnc_name);
	if(r_flag < 0)			return -11;

	// 2. Retract Shuttle
	r_flag = Fnc__SHUTTLE_IN(p_variable, p_alarm, fnc_name);
	if(r_flag < 0)			return -12;

	// 3. Vac On
	r_flag = Fnc__VACON(p_variable, p_alarm, fnc_name);
	if(r_flag < 0)			return -13;

	// 4. Latch
	r_flag = Fnc__LATCH(p_variable, p_alarm, fnc_name);
	if(r_flag < 0)			return -14;

	// 5. Paddle Ext
	r_flag = Fnc__EXTENDPADDLE(p_variable, p_alarm, fnc_name);
	if(r_flag < 0)			return -15;

	// 6. Paddle Down
	r_flag = Fnc__DOWNPADDLE(p_variable, p_alarm, fnc_name);
	if(r_flag < 0)			return -16;

	str_log.Format("<--- END ... \n");
	Fnc__APP_LOG(str_log);
	return 1;
}

int  CObj__BROOKS_IO::
Fnc__DOCK_and_EXTEND(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller)
{
	CString fnc_name = "Fnc__DOCK_and_EXTEND()";

	// ...
	CString str_log;

	str_log.Format("---> START : %s By [%s] ", fnc_name, str_caller);
	Fnc__APP_LOG(str_log);

	// ...
	int r_flag;

	r_flag = Is__ONLINE(p_variable, p_alarm);
	if(r_flag < 0)			return -1;

	// 1. Clamp
	r_flag = Fnc__CLAMP(p_variable, p_alarm, fnc_name);
	if(r_flag < 0)			return -11;

	// 2. Retract Shuttle
	r_flag = Fnc__SHUTTLE_IN(p_variable, p_alarm, fnc_name);
	if(r_flag < 0)			return -12;

	// 3. Vac On
	r_flag = Fnc__VACON(p_variable, p_alarm, fnc_name);
	if(r_flag < 0)			return -13;

	// 4. Latch
	r_flag = Fnc__LATCH(p_variable, p_alarm, fnc_name);
	if(r_flag < 0)			return -14;

	// 5. Paddle Ext
	r_flag = Fnc__EXTENDPADDLE(p_variable, p_alarm, fnc_name);
	if(r_flag < 0)			return -15;

	// 6. Paddle Down
	//if(state > 0)	state = Fnc__DOWNPADDLE(p_variable, p_alarm, fnc_name);

	str_log.Format("<--- END ... \n");
	Fnc__APP_LOG(str_log);
	return 1;
}

int  CObj__BROOKS_IO::
Fnc__UNLOAD(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller)
{
	CString fnc_name = "Fnc__UNLOAD()";

	// ...
	CString str_log;

	str_log.Format("---> START : %s By [%s] ", fnc_name, str_caller);
	Fnc__APP_LOG(str_log);

	// ...
	int r_flag;

	r_flag = Is__ONLINE(p_variable, p_alarm);
	if(r_flag < 0)			return -1;

	// 1. Paddle Up
	r_flag = Fnc__UPPADDLE(p_variable, p_alarm, fnc_name);
	if(r_flag < 0)			return -11;

	// 2. Paddle Ret
	r_flag = Fnc__RETRACTPADDLE(p_variable, p_alarm, fnc_name);
	if(r_flag < 0)			return -12;

	// 4. Unlatch
	r_flag = Fnc__UNLATCH(p_variable, p_alarm, fnc_name);
	if(r_flag < 0)			return -13;

	// 4. Vac Off
	r_flag = Fnc__VACOFF(p_variable, p_alarm, fnc_name);
	if(r_flag < 0)			return -14;

	// 5. Extract Shuttle
	r_flag = Fnc__SHUTTLE_OUT(p_variable, p_alarm, fnc_name);
	if(r_flag < 0)			return -15;

	// 6. Unclamp
	r_flag = Fnc__UNCLAMP(p_variable, p_alarm, fnc_name);
	if(r_flag < 0)			return -16;

	str_log.Format("<--- END ... \n");
	Fnc__APP_LOG(str_log);
	return 1;
}

int  CObj__BROOKS_IO::
Fnc__RLSUNLOAD(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller)
{
	CString fnc_name = "Fnc__RLSUNLOAD()";

	// ...
	CString str_log;

	str_log.Format("---> START : %s By [%s] ", fnc_name, str_caller);
	Fnc__APP_LOG(str_log);

	// ...
	int r_flag;

	r_flag = Is__ONLINE(p_variable, p_alarm);
	if(r_flag < 0)			return -1;

	// 1. Paddle Up
	r_flag = Fnc__UPPADDLE(p_variable, p_alarm, fnc_name);
	if(r_flag < 0)			return -11;

	// 2. Paddle Ret
	r_flag = Fnc__RETRACTPADDLE(p_variable, p_alarm, fnc_name);
	if(r_flag < 0)			return -12;

	// 3. Vac Off
	r_flag = Fnc__VACOFF(p_variable, p_alarm, fnc_name);
	if(r_flag < 0)			return -13;

	// 4. Unlatch
	r_flag = Fnc__UNLATCH(p_variable, p_alarm, fnc_name);
	if(r_flag < 0)			return -14;

	// 5. Extend Shuttle
	r_flag = Fnc__SHUTTLE_OUT(p_variable, p_alarm, fnc_name);
	if(r_flag < 0)			return -15;

	str_log.Format("<--- END ... \n");
	Fnc__APP_LOG(str_log);
	return 1;
}

int  CObj__BROOKS_IO::
Fnc__MAP(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller)
{
	CString fnc_name = "Fnc__MAP()";

	// ...
	CString str_log;

	str_log.Format("---> START : %s By [%s] ", fnc_name, str_caller);
	Fnc__APP_LOG(str_log);

	// ...
	int r_flag;

	r_flag = Is__ONLINE(p_variable, p_alarm);
	if(r_flag < 0)			return -1;

	// Get Mapping Result..
	r_flag = Fnc__MAP_RESULT(p_variable, p_alarm, fnc_name);
	if(r_flag < 0)			return -11;

	str_log.Format("<--- END ... \n");
	Fnc__APP_LOG(str_log);
	return 1;
}

int  CObj__BROOKS_IO::
Fnc__MAP_RESULT(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller)
{
	CString fnc_name = "Fnc__MAP_RESULT()";

	// ...
	CString str_log;

	str_log.Format("---> START : %s By [%s] ", fnc_name, str_caller);
	Fnc__APP_LOG(str_log);

	int r_flag = Is__ONLINE(p_variable, p_alarm);
	if(r_flag < 0)			return -1;

	if(iSIM_FLAG > 0)
	{
		for (int i=0; i<WAFER_SLOT_MAX; i++)
		{
			sCH__RES_MAP_SLOT[i]->Set__DATA(STR__PRESENT);
		}
	}

	str_log.Format("<--- END ... \n");
	Fnc__APP_LOG(str_log);
	return 1;
}

int  CObj__BROOKS_IO::
Fnc__LATCH(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller)
{
	// ACTIVE CHECK
	{
		sCH__ACTIVE_CHECK_LATCH_CLOSE_DO->Set__DATA(STR__YES);
		sCH__ACTIVE_CHECK_LATCH_OPEN_DO->Set__DATA(STR__YES);

		sCH__ACTIVE_CHECK_LATCH_CLOSE_DI->Set__DATA(STR__YES);
		sCH__ACTIVE_CHECK_LATCH_OPEN_DI->Set__DATA(STR__YES);
	}

	int r_flag = _Fnc__LATCH(p_variable,p_alarm, str_caller);

	if(r_flag < 0)
	{
		CString str_log;
		CString str_bff;

		str_log = " * Error Report ... \n";

		str_bff.Format("  %s <- %s \n", 
						diEXT_CH__LPx__LATCH->Get__CHANNEL_NAME(),
						diEXT_CH__LPx__LATCH->Get__STRING());
		str_log += str_bff;

		str_bff.Format("  %s <- %s \n", 
						diEXT_CH__LPx__UNLATCH->Get__CHANNEL_NAME(),
						diEXT_CH__LPx__UNLATCH->Get__STRING());
		str_log += str_bff;

		Fnc__APP_LOG(str_log);
	}
	return r_flag;
}
int  CObj__BROOKS_IO::
_Fnc__LATCH(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller)
{
	CString fnc_name = "_Fnc__LATCH()";

	// ...
	CString str_log;

	str_log.Format("---> START : %s By [%s] ", fnc_name, str_caller);
	Fnc__APP_LOG(str_log);

	// ...
	int r_flag;

	r_flag = Is__ONLINE(p_variable, p_alarm);
	if(r_flag < 0)				return -1;

	// SET : LATCH ...
	{
		doEXT_CH__LPx__UNLATCH->Set__DATA(STR__OFF);
		doEXT_CH__LPx__LATCH->Set__DATA(STR__ON);
	}

	// ACTIVE ERROR
	{
		sCH__ACTIVE_CHECK_LATCH_CLOSE_DI->Set__DATA(STR__ERROR);
		sCH__ACTIVE_CHECK_LATCH_OPEN_DI->Set__DATA(STR__ERROR);
	}

	// CHECK : LATCH ...
	{
		CString cmd_name = "LATCH";

		CII__VAR_DIGITAL_CTRL* p_ch__di_on = diEXT_CH__LPx__LATCH.Get__PTR();
		CString str__di_on = STR__ON;

		CII__VAR_DIGITAL_CTRL* p_ch__di_off = diEXT_CH__LPx__UNLATCH.Get__PTR();
		CString str__di_off = STR__OFF;

		int r_flag = _Wait_CHECK(p_variable,p_alarm, cmd_name, p_ch__di_on,str__di_on, p_ch__di_off,str__di_off);
		if(r_flag < 0)			return r_flag;
	}

	// ACTIVE OK
	{
		sCH__ACTIVE_CHECK_LATCH_CLOSE_DI->Set__DATA(STR__OK);
		sCH__ACTIVE_CHECK_LATCH_OPEN_DI->Set__DATA(STR__OK);
	}

	str_log.Format("<--- END \n");
	Fnc__APP_LOG(str_log);
	return 1;
}

int  CObj__BROOKS_IO::
Fnc__UNLATCH(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller)
{
	// ACTIVE CHECK
	{
		sCH__ACTIVE_CHECK_LATCH_CLOSE_DO->Set__DATA(STR__YES);
		sCH__ACTIVE_CHECK_LATCH_OPEN_DO->Set__DATA(STR__YES);

		sCH__ACTIVE_CHECK_LATCH_CLOSE_DI->Set__DATA(STR__YES);
		sCH__ACTIVE_CHECK_LATCH_OPEN_DI->Set__DATA(STR__YES);
	}

	int r_flag = _Fnc__UNLATCH(p_variable,p_alarm, str_caller);
	
	if(r_flag < 0)
	{
		CString str_log;
		CString str_bff;

		str_log = " * Error Report ... \n";

		str_bff.Format("  %s <- %s \n", 
						diEXT_CH__LPx__LATCH->Get__CHANNEL_NAME(),
						diEXT_CH__LPx__LATCH->Get__STRING());
		str_log += str_bff;

		str_bff.Format("  %s <- %s \n", 
						diEXT_CH__LPx__UNLATCH->Get__CHANNEL_NAME(),
						diEXT_CH__LPx__UNLATCH->Get__STRING());
		str_log += str_bff;

		Fnc__APP_LOG(str_log);
	}
	return r_flag;
}
int  CObj__BROOKS_IO::
_Fnc__UNLATCH(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller)
{
	CString fnc_name = "_Fnc__UNLATCH()";

	// ...
	CString str_log;

	str_log.Format("---> START : %s By [%s]", fnc_name, str_caller);
	Fnc__APP_LOG(str_log);

	// ...
	int r_flag;

	r_flag = Is__ONLINE(p_variable, p_alarm);
	if(r_flag < 0)			return -1;

	// SET : UNLATCH
	{
		doEXT_CH__LPx__LATCH->Set__DATA(STR__OFF);
		doEXT_CH__LPx__UNLATCH->Set__DATA(STR__ON);
	}

	// ACTIVE ERROR
	{
		sCH__ACTIVE_CHECK_LATCH_CLOSE_DI->Set__DATA(STR__ERROR);
		sCH__ACTIVE_CHECK_LATCH_OPEN_DI->Set__DATA(STR__ERROR);
	}

	// CHECK : UNLATCH ...
	{
		CString cmd_name = "UNLATCH";

		CII__VAR_DIGITAL_CTRL* p_ch__di_on = diEXT_CH__LPx__UNLATCH.Get__PTR();
		CString str__di_on = STR__ON;

		CII__VAR_DIGITAL_CTRL* p_ch__di_off = diEXT_CH__LPx__LATCH.Get__PTR();
		CString str__di_off = STR__OFF;

		int r_flag = _Wait_CHECK(p_variable,p_alarm, cmd_name, p_ch__di_on,str__di_on, p_ch__di_off,str__di_off);
		if(r_flag < 0)			return r_flag;
	}

	// ACTIVE OK
	{
		sCH__ACTIVE_CHECK_LATCH_CLOSE_DI->Set__DATA(STR__OK);
		sCH__ACTIVE_CHECK_LATCH_OPEN_DI->Set__DATA(STR__OK);
	}

	str_log.Format("<--- END \n");
	Fnc__APP_LOG(str_log);
	return 1;
}

int  CObj__BROOKS_IO::
Fnc__RETRACTPADDLE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller)
{
	// ACTIVE CHECK ...
	{
		sCH__ACTIVE_CHECK_DOOR_EXT_DO->Set__DATA(STR__YES);
		sCH__ACTIVE_CHECK_DOOR_RET_DO->Set__DATA(STR__YES);

		sCH__ACTIVE_CHECK_DOOR_EXT_DI->Set__DATA(STR__YES);
		sCH__ACTIVE_CHECK_DOOR_RET_DI->Set__DATA(STR__YES);
	}

	int r_flag = _Fnc__RETRACTPADDLE(p_variable,p_alarm, str_caller);

	if(r_flag < 0)
	{
		CString str_log;
		CString str_bff;

		str_log = " * Error Report ... \n";

		str_bff.Format("  %s <- %s \n", 
						diEXT_CH__LPx__DOOR_EXT->Get__CHANNEL_NAME(),
						diEXT_CH__LPx__DOOR_EXT->Get__STRING());
		str_log += str_bff;

		str_bff.Format("  %s <- %s \n", 
						diEXT_CH__LPx__DOOR_RET->Get__CHANNEL_NAME(),
						diEXT_CH__LPx__DOOR_RET->Get__STRING());
		str_log += str_bff;

		Fnc__APP_LOG(str_log);
	}
	return r_flag;
}
int  CObj__BROOKS_IO::
_Fnc__RETRACTPADDLE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller)
{
	CString fnc_name = "_Fnc__RETRACTPADDLE()";

	// ...
	CString str_log;

	str_log.Format("---> START : %s By [%s] ", fnc_name, str_caller);
	Fnc__APP_LOG(str_log);

	// ...
	int r_flag;

	r_flag = Is__ONLINE(p_variable, p_alarm);
	if(r_flag < 0)			return -1;

	// SET : PADDLE.RETRACT ...
	{
		doEXT_CH__LPx__DOOR_EXT->Set__DATA(STR__OFF);
		doEXT_CH__LPx__DOOR_RET->Set__DATA(STR__ON);
	}

	// ACTIVE ERROR ...
	{
		sCH__ACTIVE_CHECK_DOOR_EXT_DI->Set__DATA(STR__ERROR);
		sCH__ACTIVE_CHECK_DOOR_RET_DI->Set__DATA(STR__ERROR);
	}

	// CHECK : PADDLE.RETRACT ...
	{
		CString cmd_name = "PADDLE.RETRACT";

		CII__VAR_DIGITAL_CTRL* p_ch__di_on = diEXT_CH__LPx__DOOR_RET.Get__PTR();
		CString str__di_on = STR__ON;

		CII__VAR_DIGITAL_CTRL* p_ch__di_off = diEXT_CH__LPx__DOOR_EXT.Get__PTR();
		CString str__di_off = STR__OFF;

		int r_flag = _Wait_CHECK(p_variable,p_alarm, cmd_name, p_ch__di_on,str__di_on, p_ch__di_off,str__di_off);
		if(r_flag < 0)			return r_flag;
	}

	// ACTIVE OK ...
	{
		sCH__ACTIVE_CHECK_DOOR_EXT_DI->Set__DATA(STR__OK);
		sCH__ACTIVE_CHECK_DOOR_RET_DI->Set__DATA(STR__OK);
	}

	str_log.Format("<--- END \n");
	Fnc__APP_LOG(str_log);
	return 1;
}

int  CObj__BROOKS_IO::
Fnc__EXTENDPADDLE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller)
{
	// ACTIVE CHECK ...
	{
		sCH__ACTIVE_CHECK_DOOR_EXT_DO->Set__DATA(STR__YES);
		sCH__ACTIVE_CHECK_DOOR_RET_DO->Set__DATA(STR__YES);

		sCH__ACTIVE_CHECK_DOOR_EXT_DI->Set__DATA(STR__YES);
		sCH__ACTIVE_CHECK_DOOR_RET_DI->Set__DATA(STR__YES);
	}

	int r_flag = _Fnc__EXTENDPADDLE(p_variable,p_alarm, str_caller);

	if(r_flag < 0)
	{
		CString str_log;
		CString str_bff;

		str_log = " * Error Report ... \n";

		str_bff.Format("  %s <- %s \n", 
						diEXT_CH__LPx__DOOR_EXT->Get__CHANNEL_NAME(),
						diEXT_CH__LPx__DOOR_EXT->Get__STRING());
		str_log += str_bff;

		str_bff.Format("  %s <- %s \n", 
						diEXT_CH__LPx__DOOR_RET->Get__CHANNEL_NAME(),
						diEXT_CH__LPx__DOOR_RET->Get__STRING());
		str_log += str_bff;

		Fnc__APP_LOG(str_log);
	}
	return r_flag;
}
int  CObj__BROOKS_IO::
_Fnc__EXTENDPADDLE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller)
{
	CString fnc_name = "Fnc__EXTENDPADDLE()";

	// ...
	CString str_log;

	str_log.Format("---> START : %s By [%s] ", fnc_name, str_caller);
	Fnc__APP_LOG(str_log);

	// ...
	int r_flag;

	r_flag = Is__ONLINE(p_variable, p_alarm);
	if(r_flag < 0)				return -1;

	// SET : PADDLE.EXTEND ...
	{
		doEXT_CH__LPx__DOOR_RET->Set__DATA(STR__OFF);
		doEXT_CH__LPx__DOOR_EXT->Set__DATA(STR__ON);
	}

	// ACTIVE ERROR ...
	{
		sCH__ACTIVE_CHECK_DOOR_EXT_DI->Set__DATA(STR__ERROR);
		sCH__ACTIVE_CHECK_DOOR_RET_DI->Set__DATA(STR__ERROR);
	}

	// CHECK : PADDLE.EXTEND ...
	{
		CString cmd_name = "PADDLE.EXTEND";

		CII__VAR_DIGITAL_CTRL* p_ch__di_on = diEXT_CH__LPx__DOOR_EXT.Get__PTR();
		CString str__di_on = STR__ON;

		CII__VAR_DIGITAL_CTRL* p_ch__di_off = diEXT_CH__LPx__DOOR_RET.Get__PTR();
		CString str__di_off = STR__OFF;

		int r_flag = _Wait_CHECK(p_variable,p_alarm, cmd_name, p_ch__di_on,str__di_on, p_ch__di_off,str__di_off);
		if(r_flag < 0)			return r_flag;
	}

	// ACTIVE OK ...
	{
		sCH__ACTIVE_CHECK_DOOR_EXT_DI->Set__DATA(STR__OK);
		sCH__ACTIVE_CHECK_DOOR_RET_DI->Set__DATA(STR__OK);
	}

	str_log.Format("<--- END \n");
	Fnc__APP_LOG(str_log);
	return 1;
}

int  CObj__BROOKS_IO::
Fnc__UPPADDLE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, CString str_caller)
{
	// ACTIVE.CHECK ...
	{
		sCH__ACTIVE_CHECK_DOOR_DN_DO->Set__DATA(STR__YES);
		sCH__ACTIVE_CHECK_DOOR_DN_DI->Set__DATA(STR__YES);

		sCH__ACTIVE_CHECK_DOOR_UP_DO->Set__DATA(STR__YES);
		sCH__ACTIVE_CHECK_DOOR_UP_DI->Set__DATA(STR__YES);

		sCH__ACTIVE_CHECK_WAFER_SLID_OUT_DI->Set__DATA(STR__YES);
	}

	int r_flag = _Fnc__UPPADDLE(p_variable,p_alarm, str_caller);

	if(r_flag < 0)
	{
		CString str_log;
		CString str_bff;

		str_log = " * Error Report ... \n";

		str_bff.Format("  %s <- %s \n", 
					   diEXT_CH__LPx__DOOR_DN->Get__CHANNEL_NAME(),
					   diEXT_CH__LPx__DOOR_DN->Get__STRING());
		str_log += str_bff;

		str_bff.Format("  %s <- %s \n", 
					   diEXT_CH__LPx__DOOR_UP->Get__CHANNEL_NAME(),
					   diEXT_CH__LPx__DOOR_UP->Get__STRING());
		str_log += str_bff;

		Fnc__APP_LOG(str_log);
	}
	return r_flag;
}
int  CObj__BROOKS_IO::
_Fnc__UPPADDLE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, CString str_caller)
{
	CString str_log;

	str_log.Format("---> START : _Fnc__UPPADDLE() By [%s] ", str_caller);
	Fnc__APP_LOG(str_log);

	// ...
	{
		int r_flag = Is__ONLINE(p_variable, p_alarm);
		if(r_flag < 0)			return -1;
	}

	// SET : DOOR_UUP ...
	{
		doEXT_CH__LPx__DOOR_DN->Set__DATA(STR__OFF);
		doEXT_CH__LPx__DOOR_UP->Set__DATA(STR__ON);
	}

	// ACTIVE.ERROR ...
	{
		sCH__ACTIVE_CHECK_DOOR_UP_DI->Set__DATA(STR__ERROR);
		sCH__ACTIVE_CHECK_DOOR_DN_DI->Set__DATA(STR__ERROR);

		sCH__ACTIVE_CHECK_WAFER_SLID_OUT_DI->Set__DATA(STR__ERROR);
	}

	// CHECK : DOOR_UP ...
	{
		CString cmd_name = "PADDLE.UP";

		CII__VAR_DIGITAL_CTRL* p_ch__di_on = diEXT_CH__LPx__DOOR_UP.Get__PTR();
		CString str__di_on = STR__ON;

		CII__VAR_DIGITAL_CTRL* p_ch__di_off = diEXT_CH__LPx__DOOR_DN.Get__PTR();
		CString str__di_off = STR__OFF;

		int r_flag = _Wait_CHECK(p_variable,p_alarm, cmd_name, p_ch__di_on,str__di_on, p_ch__di_off,str__di_off);
		if(r_flag < 0)			return r_flag;
	}

	// ACTIVE.OK ...
	{
		sCH__ACTIVE_CHECK_DOOR_UP_DI->Set__DATA(STR__OK);
		sCH__ACTIVE_CHECK_DOOR_DN_DI->Set__DATA(STR__OK);

		sCH__ACTIVE_CHECK_WAFER_SLID_OUT_DI->Set__DATA(STR__OK);
	}

	str_log.Format("<--- END ... \n");
	Fnc__APP_LOG(str_log);
	return 1;
}

int  CObj__BROOKS_IO::
Fnc__DOWNPADDLE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller)
{
	// ACTIVE.CHECK ...
	{
		sCH__ACTIVE_CHECK_DOOR_DN_DO->Set__DATA(STR__YES);
		sCH__ACTIVE_CHECK_DOOR_DN_DI->Set__DATA(STR__YES);

		sCH__ACTIVE_CHECK_DOOR_UP_DO->Set__DATA(STR__YES);
		sCH__ACTIVE_CHECK_DOOR_UP_DI->Set__DATA(STR__YES);
	}

	int r_flag = _Fnc__DOWNPADDLE(p_variable,p_alarm, str_caller);

	if(r_flag < 0)
	{
		CString str_log;
		CString str_bff;

		str_log = " * Error Report ... \n";

		str_bff.Format("  %s <- %s \n", 
						diEXT_CH__LPx__DOOR_DN->Get__CHANNEL_NAME(),
						diEXT_CH__LPx__DOOR_DN->Get__STRING());
		str_log += str_bff;

		str_bff.Format("  %s <- %s \n", 
						diEXT_CH__LPx__DOOR_UP->Get__CHANNEL_NAME(),
						diEXT_CH__LPx__DOOR_UP->Get__STRING());
		str_log += str_bff;

		Fnc__APP_LOG(str_log);
	}
	return r_flag;
}
int  CObj__BROOKS_IO::
_Fnc__DOWNPADDLE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller)
{
	CString str_log;

	str_log.Format("---> START : _Fnc__DOWNPADDLE() By [%s] ", str_caller);
	Fnc__APP_LOG(str_log);

	// ...
	{
		int r_flag = Is__ONLINE(p_variable, p_alarm);
		if(r_flag < 0)			return -1;
	}

	// SET : DOOR_DOWN ...
	{
		doEXT_CH__LPx__DOOR_UP->Set__DATA(STR__OFF);
		doEXT_CH__LPx__DOOR_DN->Set__DATA(STR__ON);
	}

	// ACTIVE.ERROR ...
	{
		sCH__ACTIVE_CHECK_DOOR_UP_DI->Set__DATA(STR__ERROR);
		sCH__ACTIVE_CHECK_DOOR_DN_DI->Set__DATA(STR__ERROR);
	}

	// CHECK : DOOR_DOWN ...
	{
		CString cmd_name = "PADDLE.DOWN";

		CII__VAR_DIGITAL_CTRL* p_ch__di_on = diEXT_CH__LPx__DOOR_DN.Get__PTR();
		CString str__di_on = STR__ON;

		CII__VAR_DIGITAL_CTRL* p_ch__di_off = diEXT_CH__LPx__DOOR_UP.Get__PTR();
		CString str__di_off = STR__OFF;

		int r_flag = _Wait_CHECK(p_variable,p_alarm, cmd_name, p_ch__di_on,str__di_on, p_ch__di_off,str__di_off);
		if(r_flag < 0)			return r_flag;
	}

	// ACTIVE.OK ...
	{
		sCH__ACTIVE_CHECK_DOOR_UP_DI->Set__DATA(STR__OK);
		sCH__ACTIVE_CHECK_DOOR_DN_DI->Set__DATA(STR__OK);
	}

	// DELAY ...
	{
		SCX__TIMER_CTRL x_timer_ctrl;
		x_timer_ctrl->REGISTER__ABORT_OBJECT(sObject_Name);

		double cfg_sec = aCFG_PADDLE_DOWN_DELAY->Get__VALUE();

		if(x_timer_ctrl->WAIT( cfg_sec ) < 0)
		{
			return -21;
		}
	}

	str_log.Format("<--- END ... \n");
	Fnc__APP_LOG(str_log);
	return 1;
}

int  CObj__BROOKS_IO::
Fnc__SHUTTLE_OUT(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller)
{
	// ACTIVE.CHECK ...
	{
		sCH__ACTIVE_CHECK_SHUTTLE_IN_DO->Set__DATA(STR__YES);
		sCH__ACTIVE_CHECK_SHUTTLE_IN_DI->Set__DATA(STR__YES);

		sCH__ACTIVE_CHECK_SHUTTLE_OUT_DO->Set__DATA(STR__YES);
		sCH__ACTIVE_CHECK_SHUTTLE_OUT_DI->Set__DATA(STR__YES);
	}

	int r_flag = _Fnc__SHUTTLE_OUT(p_variable,p_alarm, str_caller);
	if(r_flag < 0)
	{
		CString str_log;
		CString str_bff;

		str_log = " * Error Report ... \n";

		str_bff.Format("  %s <- %s \n", 
						doEXT_CH__LPx__SHUTTLE_OUT->Get__CHANNEL_NAME(),
						doEXT_CH__LPx__SHUTTLE_OUT->Get__STRING());
		str_log += str_bff;

		str_bff.Format("  %s <- %s \n", 
						doEXT_CH__LPx__SHUTTLE_IN->Get__CHANNEL_NAME(),
						doEXT_CH__LPx__SHUTTLE_IN->Get__STRING());
		str_log += str_bff;

		Fnc__APP_LOG(str_log);
	}

	return r_flag;
}
int  CObj__BROOKS_IO::
_Fnc__SHUTTLE_OUT(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller)
{
	CString str_log;

	str_log.Format("---> START : _Fnc__EXTENDSHUTTLE() By [%s]", str_caller);
	Fnc__APP_LOG(str_log);

	// ...
	{
		int r_flag = Is__ONLINE(p_variable, p_alarm);
		if(r_flag < 0)		return r_flag;
	}

	// SET : SHUTTLE.EXTEND ...
	{
		doEXT_CH__LPx__SHUTTLE_IN->Set__DATA(STR__OFF);
		doEXT_CH__LPx__SHUTTLE_OUT->Set__DATA(STR__ON);
	}

	// ACTIVE.ERROR ...
	{
		sCH__ACTIVE_CHECK_SHUTTLE_IN_DI->Set__DATA(STR__ERROR);
		sCH__ACTIVE_CHECK_SHUTTLE_OUT_DI->Set__DATA(STR__ERROR);
	}

	// CHECK : SHUTTLE.EXTEND ...
	{
		CString cmd_name = "SHUTTLE.EXTEND";

		CII__VAR_DIGITAL_CTRL* p_ch__di_on = diEXT_CH__LPx__SHUTTLE_OUT.Get__PTR();
		CString str__di_on = STR__ON;

		CII__VAR_DIGITAL_CTRL* p_ch__di_off = diEXT_CH__LPx__SHUTTLE_IN.Get__PTR();
		CString str__di_off = STR__OFF;

		int r_flag = _Wait_CHECK(p_variable,p_alarm, cmd_name, p_ch__di_on,str__di_on, p_ch__di_off,str__di_off);
		if(r_flag < 0)			return r_flag;
	}

	// ACTIVE.OK ...
	{
		sCH__ACTIVE_CHECK_SHUTTLE_IN_DI->Set__DATA(STR__OK);
		sCH__ACTIVE_CHECK_SHUTTLE_OUT_DI->Set__DATA(STR__OK);
	}

	str_log.Format("<--- END ... \n");
	Fnc__APP_LOG(str_log);
	return 1;
}

int  CObj__BROOKS_IO::
Fnc__SHUTTLE_IN(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller)
{
	// ACTIVE.CHECK ...
	{
		sCH__ACTIVE_CHECK_SHUTTLE_IN_DO->Set__DATA(STR__YES);
		sCH__ACTIVE_CHECK_SHUTTLE_IN_DI->Set__DATA(STR__YES);

		sCH__ACTIVE_CHECK_SHUTTLE_OUT_DO->Set__DATA(STR__YES);
		sCH__ACTIVE_CHECK_SHUTTLE_OUT_DI->Set__DATA(STR__YES);
	}

	int r_flag = _Fnc__SHUTTLE_IN(p_variable,p_alarm, str_caller);
	if(r_flag < 0)
	{
		CString str_log;
		CString str_bff;

		str_log = " * Error Report ... \n";

		str_bff.Format("  %s <- %s \n", 
						doEXT_CH__LPx__SHUTTLE_OUT->Get__CHANNEL_NAME(),
						doEXT_CH__LPx__SHUTTLE_OUT->Get__STRING());
		str_log += str_bff;

		str_bff.Format("  %s <- %s \n", 
						doEXT_CH__LPx__SHUTTLE_IN->Get__CHANNEL_NAME(),
						doEXT_CH__LPx__SHUTTLE_IN->Get__STRING());
		str_log += str_bff;

		Fnc__APP_LOG(str_log);
	}

	return r_flag;
}
int  CObj__BROOKS_IO::
_Fnc__SHUTTLE_IN(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller)
{
	CString str_log;

	str_log.Format("---> START : _Fnc__RETRACTSHUTTLE() By [%s] ", str_caller);
	Fnc__APP_LOG(str_log);

	// ...
	{
		int r_flag = Is__ONLINE(p_variable, p_alarm);
		if(r_flag < 0)		return -1;
	}

	// SET : SHUTTLE.RETRACT ...
	{
		doEXT_CH__LPx__SHUTTLE_OUT->Set__DATA(STR__OFF);
		doEXT_CH__LPx__SHUTTLE_IN->Set__DATA(STR__ON);
	}

	// ACTIVE.ERROR ...
	{
		sCH__ACTIVE_CHECK_SHUTTLE_IN_DI->Set__DATA(STR__ERROR);
		sCH__ACTIVE_CHECK_SHUTTLE_OUT_DI->Set__DATA(STR__ERROR);
	}

	// CHECK : SHUTTLE.RETRACT ...
	{
		CString cmd_name = "SHUTTLE.RETRACT";

		CII__VAR_DIGITAL_CTRL* p_ch__di_on = diEXT_CH__LPx__SHUTTLE_IN.Get__PTR();
		CString str__di_on = STR__ON;

		CII__VAR_DIGITAL_CTRL* p_ch__di_off = diEXT_CH__LPx__SHUTTLE_OUT.Get__PTR();
		CString str__di_off = STR__OFF;

		int r_flag = _Wait_CHECK(p_variable,p_alarm, cmd_name, p_ch__di_on,str__di_on, p_ch__di_off,str__di_off);
		if(r_flag < 0)			return r_flag;
	}

	// ACTIVE.OK ...
	{
		sCH__ACTIVE_CHECK_SHUTTLE_IN_DI->Set__DATA(STR__OK);
		sCH__ACTIVE_CHECK_SHUTTLE_OUT_DI->Set__DATA(STR__OK);
	}

	str_log.Format("<--- END ... \n");
	Fnc__APP_LOG(str_log);
	return 1;
}

int  CObj__BROOKS_IO::
Fnc__VACON(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller)
{
	// ACTIVE.CHECK ...
	{
		dCH__ACTIVE_CHECK_DOOR_VAC_DO->Set__DATA(STR__YES);
		dCH__ACTIVE_CHECK_DOOR_VAC_DI->Set__DATA(STR__YES);
	}

	int r_flag = _Fnc__VACON(p_variable,p_alarm, str_caller);

	if(r_flag < 0)
	{
		CString str_log;
		CString str_bff;

		str_log = " * Error Report ... \n";

		// ...
		{
			str_bff.Format("  %s <- %s \n", 
							doEXT_CH__LPx__DOOR_VAC->Get__CHANNEL_NAME(),
							doEXT_CH__LPx__DOOR_VAC->Get__STRING());
			str_log += str_bff;
		}

		if(bActive__LPx_DOOR_VENT)
		{
			str_bff.Format("  %s <- %s \n", 
							doEXT_CH__LPx__DOOR_VENT->Get__CHANNEL_NAME(),
							doEXT_CH__LPx__DOOR_VENT->Get__STRING());
			str_log += str_bff;
		}

		Fnc__APP_LOG(str_log);
	}
	return r_flag;
}
int  CObj__BROOKS_IO::
_Fnc__VACON(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller)
{
	CString fnc_name = "_Fnc__VACON()";

	// ...
	CString str_log;

	str_log.Format("---> START : %s By [%s] ", fnc_name, str_caller);
	Fnc__APP_LOG(str_log);

	// ...
	int r_flag;

	r_flag = Is__ONLINE(p_variable, p_alarm);
	if(r_flag < 0)			return -1;

	// SET : VAC.ON ...
	{
		if(bActive__LPx_DOOR_VENT)
		{
			doEXT_CH__LPx__DOOR_VENT->Set__DATA(STR__OFF);
			Sleep(500);
		}

		doEXT_CH__LPx__DOOR_VAC->Set__DATA(STR__ON);
	}

	// ACTIVE.ERROR ...
	{
		dCH__ACTIVE_CHECK_DOOR_VAC_DI->Set__DATA(STR__ERROR);
	}

	// CHECK : VAC.ON ...
	{
		CString cmd_name = "VAC.ON";

		CII__VAR_DIGITAL_CTRL* p_ch__di_on = diEXT_CH__LPx__DOOR_VAC.Get__PTR();
		CString str__di_on = STR__ON;

		CII__VAR_DIGITAL_CTRL* p_ch__di_off = NULL;
		CString str__di_off = "";

		int r_flag = _Wait_CHECK(p_variable,p_alarm, cmd_name, p_ch__di_on,str__di_on, p_ch__di_off,str__di_off);
		if(r_flag < 0)			return r_flag;
	}

	// ACTIVE.OK ...
	{
		dCH__ACTIVE_CHECK_DOOR_VAC_DI->Set__DATA(STR__OK);
	}

	str_log.Format("<--- END ... \n");
	Fnc__APP_LOG(str_log);
	return 1;
}

int  CObj__BROOKS_IO::
Fnc__VACOFF(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller)
{
	// ACTIVE.CHECK ...
	{
		dCH__ACTIVE_CHECK_DOOR_VAC_DO->Set__DATA(STR__YES);
		dCH__ACTIVE_CHECK_DOOR_VAC_DI->Set__DATA(STR__YES);
	}

	int r_flag = _Fnc__VACOFF(p_variable,p_alarm, str_caller);

	if(r_flag < 0)
	{
		CString str_log;
		CString str_bff;

		str_log = " * Error Report ... \n";

		// ...
		{
			str_bff.Format("  %s <- %s \n", 
							doEXT_CH__LPx__DOOR_VAC->Get__CHANNEL_NAME(),
							doEXT_CH__LPx__DOOR_VAC->Get__STRING());
			str_log += str_bff;
		}

		if(bActive__LPx_DOOR_VENT)
		{
			str_bff.Format("  %s <- %s \n", 
							doEXT_CH__LPx__DOOR_VENT->Get__CHANNEL_NAME(),
							doEXT_CH__LPx__DOOR_VENT->Get__STRING());
			str_log += str_bff;
		}

		Fnc__APP_LOG(str_log);
	}
	return r_flag;
}
int  CObj__BROOKS_IO::
_Fnc__VACOFF(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller)
{
	CString fnc_name = "_Fnc__VACOFF()";

	// ...
	CString str_log;

	str_log.Format("---> START : %s By [%s]", fnc_name, str_caller);
	Fnc__APP_LOG(str_log);

	// ...
	int r_flag;

	r_flag = Is__ONLINE(p_variable, p_alarm);
	if(r_flag < 0)			return -1;

	// SET : VAC.OFF ...
	{
		doEXT_CH__LPx__DOOR_VAC->Set__DATA(STR__OFF);

		if(bActive__LPx_DOOR_VENT)
		{
			Sleep(500);
			doEXT_CH__LPx__DOOR_VENT->Set__DATA(STR__ON);
		}
	}

	// ACTIVE.ERROR ...
	{
		dCH__ACTIVE_CHECK_DOOR_VAC_DI->Set__DATA(STR__ERROR);
	}

	// CHECK : VAC.OFF ...
	{
		CString cmd_name = "VAC.OFF";

		CII__VAR_DIGITAL_CTRL* p_ch__di_on = NULL;
		CString str__di_on = "";

		CII__VAR_DIGITAL_CTRL* p_ch__di_off = diEXT_CH__LPx__DOOR_VAC.Get__PTR();
		CString str__di_off = STR__OFF;

		int r_flag = _Wait_CHECK(p_variable,p_alarm, cmd_name, p_ch__di_on,str__di_on, p_ch__di_off,str__di_off);
		if(r_flag < 0)			return r_flag;
	}

	// ACTIVE.OK ...
	{
		dCH__ACTIVE_CHECK_DOOR_VAC_DI->Set__DATA(STR__OK);
	}

	str_log.Format("<--- END ... \n");
	Fnc__APP_LOG(str_log);
	return 1;
}

int  CObj__BROOKS_IO::
Fnc__CLAMP(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller)
{
	// ACTIVE CHECK ...
	{
		sCH__ACTIVE_CHECK_CLAMP_DO->Set__DATA(STR__YES);
		sCH__ACTIVE_CHECK_UNCLAMP_DO->Set__DATA(STR__YES);

		sCH__ACTIVE_CHECK_CLAMP_DI->Set__DATA(STR__YES);
		sCH__ACTIVE_CHECK_UNCLAMP_DI->Set__DATA(STR__YES);
	}

	int r_flag = _Fnc__CLAMP(p_variable,p_alarm, str_caller);

	if(r_flag < 0)
	{
		CString str_log;
		CString str_bff;

		str_log = " * Error Report ... \n";
		
		str_bff.Format("  %s <- %s \n", 
					   diEXT_CH__LPx__CLAMP->Get__CHANNEL_NAME(),
					   diEXT_CH__LPx__CLAMP->Get__STRING());
		str_log += str_bff;

		str_bff.Format("  %s <- %s \n", 
						diEXT_CH__LPx__UNCLAMP->Get__CHANNEL_NAME(),
						diEXT_CH__LPx__UNCLAMP->Get__STRING());
		str_log += str_bff;

		Fnc__APP_LOG(str_log);
	}
	return r_flag;
}
int  CObj__BROOKS_IO::
_Fnc__CLAMP(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller)
{
_RETRY_LOOP:

	CString str_log;

	str_log.Format("---> START : _Fnc__CLAMP() By [%s] ", str_caller);
	Fnc__APP_LOG(str_log);

	// ...
	{
		int r_flag = Is__ONLINE(p_variable, p_alarm);
		if(r_flag < 0)			return -1;
	}

	// SET : CLAMP ...
	{
		doEXT_CH__LPx__UNCLAMP->Set__DATA(STR__OFF);
		doEXT_CH__LPx__CLAMP->Set__DATA(STR__ON);
	}

	// ACTIVE ERROR ...
	{
		sCH__ACTIVE_CHECK_CLAMP_DI->Set__DATA(STR__ERROR);
		sCH__ACTIVE_CHECK_UNCLAMP_DI->Set__DATA(STR__ERROR);
	}

	// CHECK : CLAMP ...
	{
		CString cmd_name = "CLAMP";

		CII__VAR_DIGITAL_CTRL* p_ch__di_on = diEXT_CH__LPx__CLAMP.Get__PTR();
		CString str__di_on = STR__ON;

		CII__VAR_DIGITAL_CTRL* p_ch__di_off = diEXT_CH__LPx__UNCLAMP.Get__PTR();
		CString str__di_off = STR__OFF;

		int r_flag = _Wait_CHECK(p_variable,p_alarm, cmd_name, p_ch__di_on,str__di_on, p_ch__di_off,str__di_off);
		if(r_flag < 0)			return r_flag;
	}

	// ACTIVE OK ...
	{
		sCH__ACTIVE_CHECK_CLAMP_DI->Set__DATA(STR__OK);
		sCH__ACTIVE_CHECK_UNCLAMP_DI->Set__DATA(STR__OK);
	}

	str_log.Format("<--- END ... \n");
	Fnc__APP_LOG(str_log);	
	return 1;
}

int  CObj__BROOKS_IO::
Fnc__UNCLAMP(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller)
{
	// ACTIVE CHECK ...
	{
		sCH__ACTIVE_CHECK_CLAMP_DO->Set__DATA(STR__YES);
		sCH__ACTIVE_CHECK_UNCLAMP_DO->Set__DATA(STR__YES);

		sCH__ACTIVE_CHECK_CLAMP_DI->Set__DATA(STR__YES);
		sCH__ACTIVE_CHECK_UNCLAMP_DI->Set__DATA(STR__YES);
	}

	int r_flag = _Fnc__UNCLAMP(p_variable,p_alarm, str_caller);

	if(r_flag < 0)
	{
		CString str_log;
		CString str_bff;

		str_log = " * Error Report ... \n";

		str_bff.Format("  %s <- %s \n", 
					   diEXT_CH__LPx__CLAMP->Get__CHANNEL_NAME(),
					   diEXT_CH__LPx__CLAMP->Get__STRING());
		str_log += str_bff;

		str_bff.Format("  %s <- %s \n", 
					   diEXT_CH__LPx__UNCLAMP->Get__CHANNEL_NAME(),
					   diEXT_CH__LPx__UNCLAMP->Get__STRING());
		str_log += str_bff;

		Fnc__APP_LOG(str_log);
	}
	return r_flag;
}
int  CObj__BROOKS_IO::
_Fnc__UNCLAMP(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller)
{
	CString str_log;

	str_log.Format("---> START : _Fnc__UNCLAMP() By [%s] ", str_caller);
	Fnc__APP_LOG(str_log);

	// ...
	{
		int r_flag = Is__ONLINE(p_variable, p_alarm);
		if(r_flag < 0)			return -1;
	}

	// SET : UNCLAMP ...
	{
		doEXT_CH__LPx__CLAMP->Set__DATA(STR__OFF);
		doEXT_CH__LPx__UNCLAMP->Set__DATA(STR__ON);
	}

	// ACTIVE ERROR ...
	{
		sCH__ACTIVE_CHECK_CLAMP_DI->Set__DATA(STR__ERROR);
		sCH__ACTIVE_CHECK_UNCLAMP_DI->Set__DATA(STR__ERROR);
	}

	// CHECK : UNCLAMP ...
	{
		CString cmd_name = "UNCLAMP";

		CII__VAR_DIGITAL_CTRL* p_ch__di_on = diEXT_CH__LPx__UNCLAMP.Get__PTR();
		CString str__di_on = STR__ON;

		CII__VAR_DIGITAL_CTRL* p_ch__di_off = diEXT_CH__LPx__CLAMP.Get__PTR();
		CString str__di_off = STR__OFF;

		int r_flag = _Wait_CHECK(p_variable,p_alarm, cmd_name, p_ch__di_on,str__di_on, p_ch__di_off,str__di_off);
		if(r_flag < 0)			return r_flag;
	}

	// ACTIVE OK ...
	{
		sCH__ACTIVE_CHECK_CLAMP_DI->Set__DATA(STR__OK);
		sCH__ACTIVE_CHECK_UNCLAMP_DI->Set__DATA(STR__OK);
	}

	str_log.Format("<--- END ... \n");
	Fnc__APP_LOG(str_log);
	return 1;
}

int  CObj__BROOKS_IO::
Fnc__DOOR_CLOSE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller)
{
	CString fnc_name = "Fnc__CLOSEDOOR()";

	// ...
	CString str_log;

	str_log.Format("---> START : %s By [%s]", fnc_name, str_caller);
	Fnc__APP_LOG(str_log);

	// ...
	int r_flag;

	r_flag = Is__ONLINE(p_variable, p_alarm);
	if(r_flag < 0)			return -1;

	r_flag = Fnc__UPPADDLE(p_variable, p_alarm, fnc_name);
	if(r_flag < 0)			return -11;

	r_flag = Fnc__RETRACTPADDLE(p_variable, p_alarm, fnc_name);
	if(r_flag < 0)			return -12;
	
	str_log.Format("<--- END ... \n");
	Fnc__APP_LOG(str_log);
	return 1;
}

int  CObj__BROOKS_IO::
Fnc__DOOR_OPEN(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller)
{
	CString fnc_name = "Fnc__OPENDOOR()";

	// ...
	CString str_log;

	str_log.Format("---> START : %s By [%s] ", fnc_name, str_caller);
	Fnc__APP_LOG(str_log);

	// ...
	int r_flag;

	r_flag = Is__ONLINE(p_variable, p_alarm);
	if(r_flag < 0)			return -1;

	if(dCH__MON_FOUP_STATUS->Check__DATA(STR__NONE) > 0)
	{
		r_flag = Fnc__LATCH(p_variable, p_alarm, fnc_name);
		if(r_flag < 0)			return -11;	
	}
	else
	{
		r_flag = Fnc__UNLATCH(p_variable, p_alarm, fnc_name);
		if(r_flag < 0)			return -11;	

		if(dCFG__UNLATCH_FOUP_VAC_ON_CHECK->Check__DATA("TRUE") > 0)
		{
			r_flag = Fnc__VACON(p_variable, p_alarm, fnc_name);
			if(r_flag < 0)			return -12;	
		}
	}
	
	r_flag = Fnc__EXTENDPADDLE(p_variable, p_alarm, fnc_name);
	if(r_flag < 0)			return -13;

	r_flag = Fnc__DOWNPADDLE(p_variable, p_alarm, fnc_name);
	if(r_flag < 0)			return -14;

	str_log.Format("<--- END ... \n");
	Fnc__APP_LOG(str_log);
	return 1;
}

int  CObj__BROOKS_IO::
Fnc__OPENDOOR_WITH_NO_UNLATCH(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller)
{
	CString fnc_name = "Fnc__OPENDOOR_WITH_NO_UNLATCH()";
	
	// ...
	CString str_log;

	str_log.Format("---> START : %s By [%s]", fnc_name, str_caller);
	Fnc__APP_LOG(str_log);

	// ...
	int r_flag;

	r_flag = Is__ONLINE(p_variable, p_alarm);
	if(r_flag < 0)			return -1;

	r_flag = Fnc__EXTENDPADDLE(p_variable, p_alarm, fnc_name);
	if(r_flag < 0)			return -11;

	r_flag = Fnc__DOWNPADDLE(p_variable, p_alarm, fnc_name);
	if(r_flag < 0)			return -12;
	
	str_log.Format("<--- END ...\n");
	Fnc__APP_LOG(str_log);
	return 1;
}
