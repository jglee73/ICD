#include "StdAfx.h"
#include "CObj__STD_TYPE.h"
#include "CObj__STD_TYPE__DEF.h"



//-------------------------------------------------------------------------	
int  CObj__STD_TYPE
::Is__LP_AUTO_MODE()
{
	if((sCH__OTR_IN_LP__FA_MODE->Check__DATA("AUTO") > 0) 
	&& (sCH__OTR_IN_LP__FA_SERVICE_MODE->Check__DATA("YES") > 0))
	{
		return 1;
	}

	return -1;
}

//-------------------------------------------------------------------------	
int  CObj__STD_TYPE
::Fnc__INIT(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller)
{
	int state = -1;

	CString sMsg;
	CString str_log;
	CString fnc_name = "Fnc__INIT()";

	str_log.Format("---> From : [%s], CALL : [%s]", str_caller, fnc_name);
	Fnc__APP_LOG(str_log);

	// Msg Write !!
	sMsg.Format("INIT Starting ...");
	Fnc__APP_MSG(sMsg);
	
	// Check Online !!
	state = Is__ONLINE(p_variable, p_alarm);

	// Slot Init.
	if(dCH__CFG_INIT_SLOT_STATE_UNKNOWN->Check__DATA(STR__YES) > 0)
	{
		for(int i=0; i<WAFER_SLOT_MAX; i++) 
		{
			dCH__LP_SLOT_STS[i]->Set__DATA("UNKNOWN");
		}
	}

	if(state > 0)
	{
		state = Seq_LAMP(LAMP_ALL, "OFF", "LAMP_ALL", "Fnc__INIT()");
	}

	if(state > 0)
	{
		Fnc__APP_LOG("1. Robot Arm Check !");
		state = IS_ROBOT_ARM_RETRACT(p_variable, p_alarm);
	}
	
	if(state > 0)
	{
		Fnc__APP_LOG("2. INIT Seq Start... !");
		state = pLPx__OBJ_CTRL->Call__OBJECT("INIT");
	}

	if(state < 0)	sMsg = "INIT Failed..";
	else			sMsg = "INIT Completed..";
	Fnc__APP_MSG(sMsg);

	str_log.Format("<--- END : From : [%s], CALL : [%s], ret:[%d]\n", str_caller, fnc_name, state);
	Fnc__APP_LOG(str_log);
	
	return state;
}

int  CObj__STD_TYPE
::Fnc__PREPLOAD(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller)
{
	int state = -1;

	CString sMsg;
	CString var_data;
	CString str_log;
	CString fnc_name = "Fnc__PREPLOAD()";

	str_log.Format("---> From : [%s], CALL : [%s]", str_caller, fnc_name);
	Fnc__APP_LOG(str_log);

	// Msg Write !!
	sMsg.Format("PREPLOAD Starting ...");
	Fnc__APP_MSG(sMsg);

	// Check Online !!
	if(Is__ONLINE(p_variable, p_alarm) < 0)
	{
		sMsg = "PREPLOAD Failed..";
		Fnc__APP_MSG(sMsg);

		str_log.Format("<--- END : From : [%s], CALL : [%s], ret:[-1]\n", str_caller, fnc_name);
		Fnc__APP_LOG(str_log);
		return -1;
	}

	// Wafer Exist 상태 초기화...
	for(int i=0; i<WAFER_SLOT_MAX; i++)  
	{
		dCH__LP_SLOT_STS[i]->Set__DATA("ABSENT");	// prepload이므로..
	}

	if((iSIM_FLAG > 0) 
	&& (Is__LP_AUTO_MODE() < 0))
	{
		pLPx__OBJ_CTRL->Call__OBJECT("SIM.FOUP_EXIST");

		if(dCH__OTR_OUT_MON_dLP_CLAMP_STS->Check__DATA("CLAMP") < 0)
		{
			if(dCH__INR_CFG_DYNAMIC_USE->Check__DATA("NO") > 0)
			{
				Fnc__APP_LOG("PrepLoad SimMode Waiting for button pushed... Clamp Box");

				if(IDOK == ::MessageBox(NULL, "Click  ""LOAD"" button if you want to clamp", sObject_Name, MB_TOPMOST | MB_SETFOREGROUND | MB_OK))
				{
					sEXT_CH__LPx__MON_LOAD_BTN_SNS->Set__DATA("ON");
					Sleep(2000);
					sEXT_CH__LPx__MON_LOAD_BTN_SNS->Set__DATA("OFF");
				}
			}
		}
	}

	do 
	{
		Sleep(200);

		if(Is__Abort(p_variable) > 0)
		{
			state = -1;
			Fnc__APP_LOG("Object Aborted...");
			break;
		}

		// Manual Mode
		if(Is__LP_AUTO_MODE() < 0)
		{
			// Foup Exist Check..
			if(dCH__OTR_OUT_MON_dLP_CST_STS->Check__DATA("EXIST") > 0)
			{
				// Dynamic Tracking Check...
				if(dCH__INR_CFG_DYNAMIC_USE->Check__DATA("YES") > 0)
				{
					state = Fnc__CLAMP(p_variable, p_alarm, fnc_name);
					if(state < 0)	Fnc__APP_LOG("CLAMP Failed..");
					
					break;
				}
				// Dynamic Tracking 아니면... Load Button Waiting !!
				else
				{
					Fnc__APP_LOG("Waiting for Clamp Status...");

					if(dCH__OTR_OUT_MON_dLP_CLAMP_STS->Check__DATA("CLAMP") > 0)
					{
						Fnc__APP_LOG("Clamp Completed...");
						state = 1;
						break;
					}
					/*
					if(diEXT_CH__LPx__LOAD_BTN->Check__DATA("ON") > 0)
					{
						state = Fnc__CLAMP(p_variable, p_alarm, fnc_name);
						if(state < 0)	Fnc__APP_LOG("CLAMP Failed..");
						
						break;
					}
					*/
				}
			}
		}
		// Auto Mode
		else
		{
			if((dCH__OTR_OUT_MON_dLP_CST_STS->Check__DATA("EXIST") > 0) 
			&& (dEXT_CH__PHY_LPx__PIO_RUN_SNS->Check__DATA("DONE") > 0))
			{
				state = 1;

				Fnc__APP_LOG("AUTO MODE Foup Exist...state:1");
				break;
			}
		}
	} 
	while(1);

	if(state < 0)	sMsg = "PREPLOAD Failed..";
	else			sMsg = "PREPLOAD Completed..";
	Fnc__APP_MSG(sMsg);

	str_log.Format("<--- END : From : [%s], CALL : [%s], ret:[%d]\n", str_caller, fnc_name, state);
	Fnc__APP_LOG(str_log);

	return state;
}

int  CObj__STD_TYPE
::Fnc__LOAD(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller)
{
	int state = -1;
	CString sMsg;
	CString str_log;

	// Msg Write !!
	CString fnc_name = "Fnc__LOAD()";

	str_log.Format("---> From : [%s], CALL : [%s]", str_caller, fnc_name);
	Fnc__APP_LOG(str_log);
	
	sMsg.Format("LOAD Starting ...");
	Fnc__APP_MSG(sMsg);

	// Check Online !!
	state = Is__ONLINE(p_variable, p_alarm);

	if(state > 0)
	{
		state = pLPx__OBJ_CTRL->Call__OBJECT("LOAD");

		Fnc__MAP_RESULT(p_variable, p_alarm, fnc_name);
	}

	if(state < 0)	sMsg = "LOAD Failed..";
	else			sMsg = "LOAD Completed..";
	Fnc__APP_MSG(sMsg);

	str_log.Format("<--- END : From : [%s], CALL : [%s], ret:[%d]\n", str_caller, fnc_name, state);
	Fnc__APP_LOG(str_log);

	return state;
}

int  CObj__STD_TYPE
::Fnc__DOCK_and_EXTEND(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller)
{
	int state = -1;
	CString sMsg;
	CString str_log;

	// Msg Write !!
	CString fnc_name = "Fnc__DOCK_and_EXTEND()";

	str_log.Format("---> From : [%s], CALL : [%s]", str_caller, fnc_name);
	Fnc__APP_LOG(str_log);
	
	sMsg.Format("DOCK and EXTEND Starting ...");
	Fnc__APP_MSG(sMsg);

	// Check Online !!
	state = Is__ONLINE(p_variable, p_alarm);

	if(state > 0)
	{
		state = pLPx__OBJ_CTRL->Call__OBJECT("LOAD");
	}
	if(state > 0)
	{
		state = pLPx__OBJ_CTRL->Call__OBJECT("OPEN");
	}

	if(state < 0)	sMsg = "DOCK and EXTEND Failed..";
	else			sMsg = "DOCK and EXTEND Completed..";
	Fnc__APP_MSG(sMsg);

	str_log.Format("<--- END : From : [%s], CALL : [%s], ret:[%d]\n", str_caller, fnc_name, state);
	Fnc__APP_LOG(str_log);

	return state;
}

int  CObj__STD_TYPE
::Fnc__UNLOAD(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller)
{
	int state = -1;
	CString sMsg;
	CString str_log;

	CString fnc_name = "Fnc__UNLOAD()";

	str_log.Format("---> From : [%s], CALL : [%s]", str_caller, fnc_name);
	Fnc__APP_LOG(str_log);
	
	// Msg Write !!
	sMsg.Format("UNLOAD Starting ...");
	Fnc__APP_MSG(sMsg);

	// Check Online !!
	state = Is__ONLINE(p_variable, p_alarm);

	if(state > 0)
	{
		state = pLPx__OBJ_CTRL->Call__OBJECT("UNLOAD");
	}
	if(state > 0)
	{
		state = pLPx__OBJ_CTRL->Call__OBJECT("UNCLAMP");
	}

	if(state < 0)	sMsg = "UNLOAD CMD Failed..";
	else			sMsg = "UNLOAD CMD Completed..";
	Fnc__APP_MSG(sMsg);

	str_log.Format("<--- END : From : [%s], CALL : [%s], ret:[%d]\n", str_caller, fnc_name, state);
	Fnc__APP_LOG(str_log);

	return state;
}

int  CObj__STD_TYPE
::Fnc__RLSUNLOAD(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller)
{
	int state = -1;
	CString sMsg;
	CString str_log;

	CString fnc_name = "Fnc__RLSUNLOAD()";

	str_log.Format("---> From : [%s], CALL : [%s]", str_caller, fnc_name);
	Fnc__APP_LOG(str_log);

	// Msg Write !!
	sMsg.Format("RLSUNLOAD Starting ...");
	Fnc__APP_MSG(sMsg);

	// Check Online !!
	state = Is__ONLINE(p_variable, p_alarm);

	if(state > 0)
	{
		state = pLPx__OBJ_CTRL->Call__OBJECT("UNLOAD");
	}

	if(state < 0)	sMsg = "RLSUNLOAD Failed..";
	else			sMsg = "RLSUNLOAD Completed..";
	Fnc__APP_MSG(sMsg);

	str_log.Format("<--- END : From : [%s], CALL : [%s], ret:[%d]\n", str_caller, fnc_name, state);
	Fnc__APP_LOG(str_log);

	return state;
}

int  CObj__STD_TYPE
::Fnc__MAP(CII_OBJECT__VARIABLE* p_variable,
		   CII_OBJECT__ALARM* p_alarm, 
		   const CString str_caller)
{
LOOP_RETRY:

	int state = -1;
	CString sMsg;
	CString str_log;

	CString fnc_name = "Fnc__MAP()";

	str_log.Format("---> From : [%s], CALL : [%s]", str_caller, fnc_name);
	Fnc__APP_LOG(str_log);
	
	// Msg Write !!
	sMsg.Format("MAP Starting ...");
	Fnc__APP_MSG(sMsg);

	// Check Online !!
	state = Is__ONLINE(p_variable, p_alarm);

	// Get Mapping Result..
	if(state > 0)
	{
		if(dCH__CFG_MAPPING_SENSOR_USE->Check__DATA(STR__YES) > 0)
		{
			state = pLPx__OBJ_CTRL->Call__OBJECT("MAP");
		}
		else
		{
			int alm_id = ALID__LP_MAPPING_SENSOR_NOT_USE;
			CString alm_msg;
			CString r_act;

			alm_msg.Format(" Config Channel(%s) <- %s \n", 
						   dCH__CFG_MAPPING_SENSOR_USE->Get__CHANNEL_NAME(),
						   dCH__CFG_MAPPING_SENSOR_USE->Get__STRING());

			p_alarm->Popup__ALARM_With_MESSAGE(alm_id, alm_msg, r_act);

			if(r_act.CompareNoCase(ACT__RETRY) == 0)		goto LOOP_RETRY;
			return -11;
		}
	}
	if(state > 0)
	{
		state = Fnc__MAP_RESULT(p_variable, p_alarm, fnc_name);
	}

	if(state < 0)	sMsg = "MAP Failed..";
	else			sMsg = "MAP Completed..";
	Fnc__APP_MSG(sMsg);

	str_log.Format("<--- END : From : [%s], CALL : [%s], ret:[%d]\n", str_caller, fnc_name, state);
	Fnc__APP_LOG(str_log);

	return state;
}

int  CObj__STD_TYPE
::Fnc__MAP_RESULT(CII_OBJECT__VARIABLE* p_variable,
				  CII_OBJECT__ALARM* p_alarm, 
				  const CString str_caller)
{
	int state = -1;
	CString sMsg;
	CString str_log;

	CString fnc_name = "Fnc__MAP_RESULT()";

	str_log.Format("---> From : [%s], CALL : [%s]", str_caller, fnc_name);
	Fnc__APP_LOG(str_log);


	// Msg Write !!
	sMsg.Format("MAP_RESULT Starting ...");
	Fnc__APP_MSG(sMsg);

	// Check Online !!
	state = Is__ONLINE(p_variable, p_alarm);

	if(iSIM_FLAG > 0)
	{
		state = 1;
		
		for (int i=0; i<WAFER_SLOT_MAX; i++)
		{
			dCH__LP_SLOT_STS[i]->Set__DATA("PRESENT");
		}
	}
	else
	{
		CString ch_data;

		for (int i=0; i<WAFER_SLOT_MAX; i++)
		{
			ch_data = sEXT_CH__LPx__RES_MAP_SLOT[i]->Get__STRING();
			dCH__LP_SLOT_STS[i]->Set__DATA(ch_data);
		}
	}

	if(state < 0)	sMsg = "MAP_RESULT Failed..";
	else			sMsg = "MAP_RESULT Completed..";
	
	
	Fnc__APP_MSG(sMsg);

	str_log.Format("<--- END : From : [%s], CALL : [%s], ret:[%d]\n", str_caller, fnc_name, state);
	Fnc__APP_LOG(str_log);

	return state;
}

int  CObj__STD_TYPE
::Fnc__CLAMP(CII_OBJECT__VARIABLE* p_variable,
			 CII_OBJECT__ALARM* p_alarm, 
			 const CString str_caller)
{
	int state = -1;
	CString sMsg;
	CString str_log;

	CString fnc_name = "Fnc__CLAMP()";

	str_log.Format("---> From : [%s], CALL : [%s]", str_caller, fnc_name);
	Fnc__APP_LOG(str_log);

	// Msg Write !!
	sMsg.Format("CLAMP Starting ...");
	Fnc__APP_MSG(sMsg);

	// Check Online !!
	state = Is__ONLINE(p_variable, p_alarm);

	if(state > 0)
	{
		state = pLPx__OBJ_CTRL->Call__OBJECT("CLAMP");
	}

	if(state < 0)	sMsg = "CLAMP Failed..";
	else			sMsg = "CLAMP Completed..";
	Fnc__APP_MSG(sMsg);

	str_log.Format("<--- END : From : [%s], CALL : [%s], ret:[%d]\n", str_caller, fnc_name, state);
	Fnc__APP_LOG(str_log);

	return state;
}

int  CObj__STD_TYPE
::Fnc__UNCLAMP(CII_OBJECT__VARIABLE* p_variable,
			   CII_OBJECT__ALARM* p_alarm, 
			   const CString str_caller)
{
	int state = -1;
	CString sMsg;
	CString str_log;

	CString fnc_name = "Fnc__UNCLAMP()";

	str_log.Format("---> From : [%s], CALL : [%s]", str_caller, fnc_name);
	Fnc__APP_LOG(str_log);

	// Msg Write !!
	sMsg.Format("UNCLAMP Starting ...");
	Fnc__APP_MSG(sMsg);

	// Check Online !!
	state = Is__ONLINE(p_variable, p_alarm);

	if(state > 0)
	{
		state = pLPx__OBJ_CTRL->Call__OBJECT("UNCLAMP");
	}

	if(state < 0)	sMsg = "UNCLAMP Failed..";
	else			sMsg = "UNCLAMP Completed..";
	Fnc__APP_MSG(sMsg);

	str_log.Format("<--- END : From : [%s], CALL : [%s], ret:[%d]\n", str_caller, fnc_name, state);
	Fnc__APP_LOG(str_log);

	return state;
}

int  CObj__STD_TYPE
::Fnc__CLOSEDOOR(CII_OBJECT__VARIABLE* p_variable,
				 CII_OBJECT__ALARM* p_alarm, 
				 const CString str_caller)
{
	int state = -1;
	CString sMsg;
	CString str_log;

	CString fnc_name = "Fnc__CLOSEDOOR()";

	str_log.Format("---> From : [%s], CALL : [%s]", str_caller, fnc_name);
	Fnc__APP_LOG(str_log);

	// Msg Write !!
	sMsg.Format("CLOSEDOOR Starting ...");
	Fnc__APP_MSG(sMsg);

	// Check Online !!
	state = Is__ONLINE(p_variable, p_alarm);

	if(state > 0)
	{
		state = pLPx__OBJ_CTRL->Call__OBJECT("CLOSE");
	}
	
	if(state < 0)	sMsg = "CLOSEDOOR Failed..";
	else			sMsg = "CLOSEDOOR Completed..";
	Fnc__APP_MSG(sMsg);

	str_log.Format("<--- END : From : [%s], CALL : [%s], ret:[%d]\n", str_caller, fnc_name, state);
	Fnc__APP_LOG(str_log);

	return state;
}

int  CObj__STD_TYPE
::Fnc__OPENDOOR(CII_OBJECT__VARIABLE* p_variable,
				CII_OBJECT__ALARM* p_alarm, 
				const CString str_caller)
{
	int state = -1;
	CString sMsg;
	CString str_log;

	CString fnc_name = "Fnc__OPENDOOR()";

	str_log.Format("---> From : [%s], CALL : [%s]", str_caller, fnc_name);
	Fnc__APP_LOG(str_log);

	// Msg Write !!
	sMsg.Format("OPENDOOR Starting ...");
	Fnc__APP_MSG(sMsg);

	// Check Online !!
	state = Is__ONLINE(p_variable, p_alarm);

	if(state > 0)
	{
		state = pLPx__OBJ_CTRL->Call__OBJECT("OPEN");
	}

	if(state < 0)	sMsg = "OPENDOOR Failed..";
	else			sMsg = "OPENDOOR Completed..";
	Fnc__APP_MSG(sMsg);

	str_log.Format("<--- END : From : [%s], CALL : [%s], ret:[%d]\n", str_caller, fnc_name, state);
	Fnc__APP_LOG(str_log);

	return state;
}

