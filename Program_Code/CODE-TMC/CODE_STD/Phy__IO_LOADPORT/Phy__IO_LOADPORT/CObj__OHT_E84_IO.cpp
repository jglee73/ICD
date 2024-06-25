#include "StdAfx.h"
#include "CObj__OHT_E84_IO.h"
#include "CObj__OHT_E84_IO__DEF.h"



//--------------------------------------------------------------------------------
CObj__OHT_E84_IO::CObj__OHT_E84_IO()
{

}
CObj__OHT_E84_IO::~CObj__OHT_E84_IO()
{

}

//--------------------------------------------------------------------------------
int CObj__OHT_E84_IO::__DEFINE__CONTROL_MODE(obj,l_mode)
{
	sObject_Name = obj;

	// ...
	{
		ADD__CTRL_VAR(sMODE__INIT,   "INIT");
	}
	return 1;
}
int CObj__OHT_E84_IO::__DEFINE__VERSION_HISTORY(version)
{
	version = "JG.Lee \n";

	return 1;
}


// ...
#define MON_ID__ALL_STATE								1
#define MON_ID__E84_MAIN								2


int CObj__OHT_E84_IO::__DEFINE__VARIABLE_STD(p_variable)
{
	DECLARE__STD_VARIABLE

	CCommon_Error__DEF_VARIABLE m_err_def_variable;
	p_variable->Link__DEF_VARIABLE__ERROR_FEEDBACK(&m_err_def_variable);

	// ...
	CString str_name;

	int i_limit;
	int i;

	// ...
	{
		str_name = "APP.CMD.TIMEOUT.CNT";
		STD__ADD_STRING(str_name);
	}

	// ...
	{
		str_name = "sAPP.COMM.STS";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__COMM_STS,str_name);

		str_name = "INR.sLP.MSG";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__INR_LP__MSG,str_name);

		//
		str_name = "ACTIVE.PIO.LOG";
		STD__ADD_DIGITAL(str_name, "NO YES");
		LINK__VAR_DIGITAL_CTRL(dCH__ACTIVE_PIO_LOG, str_name);
	}

	// REPORT_PIO ...
	{
		str_name = "REPORT_PIO.E84_RUN.STATE";
		STD__ADD_DIGITAL(str_name, "UNKNOWN RUN DONE STOP");
		LINK__VAR_DIGITAL_CTRL(dCH__REPORT_PIO_E84_RUN_STATE, str_name);

		str_name = "REPORT_PIO.ERROR.ACTIVE";
		STD__ADD_DIGITAL(str_name, "OFF ON");
		LINK__VAR_DIGITAL_CTRL(dCH__REPORT_PIO_ERROR_ACTIVE, str_name);
	}

	// ...
	{
		str_name = "INR.aLP.TP.TIME.COUNT";
		STD__ADD_ANALOG(str_name, "0", 0, 0.1, 999999);
		LINK__VAR_ANALOG_CTRL(aCH__LP_TP_TIME_COUNT,str_name);

		str_name = "INR.aLP.PRE.TIME.COUNT";
		STD__ADD_ANALOG(str_name, "0", 0, 0.1, 999999);
		LINK__VAR_ANALOG_CTRL(aCH__LP_PRE_TIME_COUNT,str_name);

		str_name = "INR.aLP.PLS.TIME.COUNT";
		STD__ADD_ANALOG(str_name, "0", 0, 0.1, 999999);
		LINK__VAR_ANALOG_CTRL(aCH__LP_PLS_TIME_COUNT,str_name);
	}

	// PIO RESET ...
	{
		str_name = "PIO.RESET";
		STD__ADD_DIGITAL(str_name, "OFF ON");
		LINK__VAR_DIGITAL_CTRL(dCH__PIO_RESET, str_name);
	}

	// CFG : PIO.Tx ...
	{
		i_limit = _CFG__PIO_TPx;
		for(i=0; i<i_limit; i++)
		{
			str_name.Format("CFG.PIO.TP%1d", i+1);
			STD__ADD_ANALOG_WITH_X_OPTION(str_name, "0", 0, 1, 999, "");
			LINK__VAR_ANALOG_CTRL(aCH__EXT_CFG_PIO_TPx[i], str_name);
		}

		i_limit = _CFG__PIO_TDx;
		for(i=0; i<i_limit; i++)
		{
			str_name.Format("CFG.PIO.TD%1d", i+1);
			STD__ADD_ANALOG_WITH_X_OPTION(str_name, "0", 0, 1, 999, "");
			LINK__VAR_ANALOG_CTRL(aCH__EXT_CFG_PIO_TDx[i], str_name);
		}

		//
		str_name = "CUR.CHECK.TPx";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__CUR_CHECK_TPx, str_name);
	}

	// CFG ...
	{
		str_name = "CFG.aPIO.SIGNAL.CHATTERING.TIME";	// 신호가 순간 정전으로 OFF --> ON 되었을 때 시간..
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "msec", 0, 100, 900, "");
		LINK__VAR_ANALOG_CTRL(aCH__CFG_PIO_SIGNAL_CHATTERING_TIME, str_name);

		//
		str_name = "CFG.aPIO.FOUP.VIOLATION.TIME";		// FOUP VIOLATION TIME..
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "sec", 0, 0, 999, "");
		LINK__VAR_ANALOG_CTRL(aCH__CFG_PIO_FOUP_VIOLATION_TIME, str_name);

		//
		str_name = "CFG.aPIO.FOUP.LOGIC.ERROR.TIME";	// FOUP LOGIC ERROR TIME..
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "sec", 0, 1, 999, "");
		LINK__VAR_ANALOG_CTRL(aCH__CFG_PIO_FOUP_LOGIC_ERROR_TIME, str_name);
	}

	// ...
	{
		str_name = "INR.sCST.UNKNOWN.TIMER.DISPLAY";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__INR_CST_UNKNOWN_TIMER_COUNTER,str_name);

		str_name = "INR.sTP.TIMER.DISPLAY";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__INR_TP_TIMER_COUNTER,str_name);

		str_name = "INR.sTIMER.PRESENT.DISPLAY";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__INR_CST_PRESENT_TIMER_DISPLAY,str_name);

		str_name = "INR.sTIMER.PLACE.DISPLAY";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__INR_CST_PLACE_TIMER_DISPLAY,str_name);

		str_name = "INR.sTIMER.NONE.DISPLAY";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__INR_CST_NONE_TIMER_DISPLAY,str_name);

		str_name = "INR.sTIMER.EXIST.DISPLAY";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__INR_CST_EXIST_TIMER_DISPLAY,str_name);
	}

	// ...
	{
		str_name = "CFG.dE84CheckForClampStateWhenPodAbsent";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, "FALSE TRUE", "");
		LINK__VAR_DIGITAL_CTRL(dCH__CFG_E84LOAD_CLAMP_CHECK, str_name);

		// AGV1LightCurtainAlarmAutoClear
		str_name = "CFG.dLIGHT.CURTN.AUTO.CLEAR";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, "FALSE TRUE", "");
		LINK__VAR_DIGITAL_CTRL(dCH__CFG_LT_CURTN_AUTO_CLEAR, str_name);

		// AGV1LightCurtainCheckedAtIdle
		str_name = "CFG.dLIGHT.CURTN.CHECK.WHEN.IDLE";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, "FALSE TRUE", "");
		LINK__VAR_DIGITAL_CTRL(dCH__CFG_LT_CURTN_CHECK_WHEN_IDLE, str_name);

		// AGV1LightCurtainCheckedAtIdle
		str_name = "CFG.LT.CURTN.USE";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, "FALSE TRUE", "");
		LINK__VAR_DIGITAL_CTRL(dCH__CFG_LT_CURTN_USE_OPT, str_name);

		//
		str_name = "INR.dLOGIC.ERROR.FLAG";
		STD__ADD_DIGITAL(str_name, "OFF ON");
		LINK__VAR_DIGITAL_CTRL(dCH__INR_LOGIC_ERROR_FLAG, str_name);
	}

	// ...
	{
		str_name = "INR.dNONE.VIOLATION.ERROR.FLAG";
		STD__ADD_DIGITAL(str_name, "OFF ON");
		LINK__VAR_DIGITAL_CTRL(dCH__INR_NONE_VIOLATION_ERROR_FLAG, str_name);

		str_name = "INR.dEXIST.VIOLATION.ERROR.FLAG";
		STD__ADD_DIGITAL(str_name, "OFF ON");
		LINK__VAR_DIGITAL_CTRL(dCH__INR_EXIST_VIOLATION_ERROR_FLAG, str_name);
	}

	// ...
	{
		p_variable->Add__MONITORING_PROC(1.0,MON_ID__ALL_STATE);
		p_variable->Add__MONITORING_PROC(1.0,MON_ID__E84_MAIN);
	}
	return 1;
}
int CObj__OHT_E84_IO::__DEFINE__ALARM(p_alarm)
{
	DECLARE__ALARM;

	//.....
	CString title;
	title.Format("%s - ",sObject_Name);

	CString alarm_title;
	CString alarm_msg;
	CStringArray l_act;
	int alarm_id;

	//.....
	alarm_id = ALID__OFFLINE_ALARM;

	alarm_title  = title;
	alarm_title += "Offline.";

	alarm_msg  = "Communication is Offline.\n";
	alarm_msg += "Please, Check DeviceNet Communication Status !\n";

	l_act.RemoveAll();
	l_act.Add("RETRY");
	l_act.Add("COMPLETE");

	ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);

	//.....
	alarm_id = ALID__LOADPORT_OFFLINE_ALARM;

	alarm_title  = title;
	alarm_title += "Offline.";

	alarm_msg  = "Controller is Offline.\n";
	alarm_msg += "Please, Check LoadPort Communication Status !\n";

	l_act.RemoveAll();
	l_act.Add("RETRY");
	l_act.Add("COMPLETE");

	ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);

	//.....
	alarm_id = ALID__CMD_TIMEOUT_ALARM;

	alarm_title  = title;
	alarm_title += "Command Timeout.";

	alarm_msg  = "After Command Send, Result Can't Receive.\n";

	l_act.RemoveAll();
	l_act.Add("RETRY");
	l_act.Add("COMPLETE");

	ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);

	//.....
	alarm_id = ALID__LP_E84A_TP1;

	alarm_title  = title;
	alarm_title += "TP1 LOAD Timeout";

	alarm_msg  = "TP1 Timeout in LOAD Transfer";

	l_act.RemoveAll();
	l_act.Add("RETRY");
	l_act.Add("COMPLETE");

	ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);

	//.....
	alarm_id = ALID__LP_E84A_TP2;

	alarm_title  = title;
	alarm_title += "TP2 LOAD Timeout";

	alarm_msg  = "TP2 Timeout in LOAD Transfer";

	l_act.RemoveAll();
	l_act.Add("RETRY");
	l_act.Add("COMPLETE");

	ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);

	//.....
	alarm_id = ALID__LP_E84A_TP3;

	alarm_title  = title;
	alarm_title += "TP3 LOAD Timeout";

	alarm_msg  = "TP3 Timeout in LOAD Transfer";

	l_act.RemoveAll();
	l_act.Add("RETRY");
	l_act.Add("COMPLETE");

	ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);

	//.....
	alarm_id = ALID__LP_E84A_TP4;

	alarm_title  = title;
	alarm_title += "TP4 LOAD Timeout";

	alarm_msg  = "TP4 Timeout in LOAD Transfer";

	l_act.RemoveAll();
	l_act.Add("RETRY");
	l_act.Add("COMPLETE");

	ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);

	//.....
	alarm_id = ALID__LP_E84A_TP5;

	alarm_title  = title;
	alarm_title += "TP5 LOAD Timeout";

	alarm_msg  = "TP5 Timeout in LOAD Transfer";

	l_act.RemoveAll();
	l_act.Add("RETRY");
	l_act.Add("COMPLETE");

	ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);

	//.....
	alarm_id = ALID__LP_E84A_TP1_UNLOAD;

	alarm_title  = title;
	alarm_title += "TP1 UNLOAD Timeout";

	alarm_msg  = "TP1 Timeout in UNLOAD Transfer";

	l_act.RemoveAll();
	l_act.Add("RETRY");
	l_act.Add("COMPLETE");

	ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);

	//.....
	alarm_id = ALID__LP_E84A_TP2_UNLOAD;

	alarm_title  = title;
	alarm_title += "TP2 UNLOAD Timeout";

	alarm_msg  = "TP2 Timeout in UNLOAD Transfer";

	l_act.RemoveAll();
	l_act.Add("RETRY");
	l_act.Add("COMPLETE");

	ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);

	//.....
	alarm_id = ALID__LP_E84A_TP3_UNLOAD;

	alarm_title  = title;
	alarm_title += "TP3 UNLOAD Timeout";

	alarm_msg  = "TP3 Timeout in UNLOAD Transfer";

	l_act.RemoveAll();
	l_act.Add("RETRY");
	l_act.Add("COMPLETE");

	ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);

	//.....
	alarm_id = ALID__LP_E84A_TP4_UNLOAD;

	alarm_title  = title;
	alarm_title += "TP4 UNLOAD Timeout";

	alarm_msg  = "TP4 Timeout in UNLOAD Transfer";

	l_act.RemoveAll();
	l_act.Add("RETRY");
	l_act.Add("COMPLETE");

	ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);

	//.....
	alarm_id = ALID__LP_E84A_TP5_UNLOAD;

	alarm_title  = title;
	alarm_title += "TP5 UNLOAD Timeout";

	alarm_msg  = "TP5 Timeout in UNLOAD Transfer";

	l_act.RemoveAll();
	l_act.Add("RETRY");
	l_act.Add("COMPLETE");

	ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);

	//.....
	alarm_id = ALID__LP_E84A_UNLOAD_UNCLAMP_ERR;

	alarm_title  = title;
	alarm_title += "UNCLAMP Error";

	alarm_msg  = "When UNLOAD Transfer, Now CLAMP Sts...";

	l_act.RemoveAll();
	l_act.Add("RETRY");
	l_act.Add("COMPLETE");

	ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);

	//.....
	alarm_id = ALID__LP_LIGHT_CURTAIN;

	alarm_title  = title;
	alarm_title += "ERR = Light Curtain Cut";

	alarm_msg  = "In FullAuto Mode, Light Curtain is cut";

	l_act.RemoveAll();
	l_act.Add("RETRY");
	l_act.Add("COMPLETE");

	ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);

	//.....
	alarm_id = ALID__LP_E84A_LOAD_FOUP_EXIST;

	alarm_title  = title;
	alarm_title += "FOUP Violation Error(EXIST), When wait CS0[ON], VALID[ON]";

	alarm_msg  = "Waiting Load PIO Sequence Start, FOUP or MATERIAL Exist Error";

	l_act.RemoveAll();
	l_act.Add("RETRY");
	l_act.Add("COMPLETE");

	ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);

	//.....
	alarm_id = ALID__LP_E84A_UNLOAD_FOUP_NONE;

	alarm_title  = title;
	alarm_title += "FOUP Violation Error(NONE), When wait CS0[ON], VALID[ON]";

	alarm_msg  = "Waiting Unload PIO Sequence Start, FOUP Not Exist Error.";

	l_act.RemoveAll();
	l_act.Add("RETRY");
	l_act.Add("COMPLETE");

	ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);

	//.....
	alarm_id = ALID__LP_E84A_PRESENT_SNS_ON;

	alarm_title  = title;
	alarm_title += "FOUP Logic Error(PRESENT SNS ON), When wait CS0, VALID";

	alarm_msg  = "Waiting PIO Sequence Start, FOUP Sts Unknown Error";

	l_act.RemoveAll();
	l_act.Add("RETRY");
	l_act.Add("COMPLETE");

	ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);

	//.....
	alarm_id = ALID__LP_E84A_PLACE_SNS_ON;

	alarm_title  = title;
	alarm_title += "FOUP Logic Error(PLACE SNS ON), When wait CS0, VALID";

	alarm_msg  = "Waiting PIO Sequence Start, FOUP Sts Unknown Error";

	l_act.RemoveAll();
	l_act.Add("RETRY");
	l_act.Add("COMPLETE");

	ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);

	//.....
	alarm_id = ALID__LP_E84A_LOAD_CLAMP_CHECK;

	alarm_title  = title;
	alarm_title += "LP CLAMP ERROR.";

	alarm_msg  = "Before LP Receive New Foup, Now Not Unclamp but Clamp Status.\n";
	alarm_msg += "Please, Check Clamp Status !\n";

	l_act.RemoveAll();
	l_act.Add("RETRY");
	l_act.Add("COMPLETE");

	ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);

	//.....
	alarm_id = ALID__LP_E84A_ACTIVE_ABNORMAL_SIGNAL_OCCUR;

	alarm_title  = title;
	alarm_title += "Active Abnormal Signal Occur.";

	alarm_msg  = "During PIO Communication, Active's Abnormal Signal Received.\n";
	alarm_msg += "Please, Check OHT's Signal Status !\n";

	l_act.RemoveAll();
	l_act.Add("RETRY");
	l_act.Add("COMPLETE");

	ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);

	//.....
	alarm_id = ALID__LP_E84A_ACTIVE_VALID_ABNORMAL_ERROR_OCCUR;

	alarm_title  = title;
	alarm_title += "Valid Signal Off Occur.";

	alarm_msg  = "During PIO Communication, Active's Abnormal Signal Received.\n";
	alarm_msg += "Please, Check OHT's Valid Signal Status !\n";

	l_act.RemoveAll();
	l_act.Add("RETRY");
	l_act.Add("COMPLETE");

	ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);

	//.....
	alarm_id = ALID__LP_E84A_FOUP_LOGIC_ERROR;

	alarm_title  = title;
	alarm_title += "Foup Logic Error Occur.";

	alarm_msg  = "During PIO Communication, Foup Logic Error Occur.\n";
	alarm_msg += "Please, Check Place or Present Sensor Status !\n";

	l_act.RemoveAll();
	l_act.Add("RETRY");
	l_act.Add("COMPLETE");

	ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);

	//.....
	alarm_id = ALID__LP_E84A_FOUP_VIOLATION_ERROR;

	alarm_title  = title;
	alarm_title += "Foup Violation Error Occur.";

	alarm_msg  = "During PIO Communication, Foup Violation Error Occur.\n";
	alarm_msg += "Please, Check Place and Present Sensor Status !\n";

	l_act.RemoveAll();
	l_act.Add("RETRY");
	l_act.Add("COMPLETE");

	ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);

	return 1;
}

int CObj__OHT_E84_IO::__Define__USER_FUNCTION(CII_DEFINE__FUNCTION *p_fnc_ctrl)
{
	p_fnc_ctrl->Link__ERROR_FEEDBACK(&mERROR__USER_FNC);

	return 1;
}

//--------------------------------------------------------------------------------
int CObj__OHT_E84_IO::__INITIALIZE__OBJECT(p_variable,p_ext_obj_create)
{
	// ...
	{
		CString file_name;
		CString log_msg;

		file_name.Format("%s_App.log", sObject_Name);

		log_msg  = "\n\n";
		log_msg += "//------------------------------------------------------------------------";

		xPIO_LOG_CTRL->CREATE__SUB_DIRECTORY(sObject_Name);
		xPIO_LOG_CTRL->SET__PROPERTY(file_name,24*5,60);

		xPIO_LOG_CTRL->DISABLE__TIME_LOG();
		xPIO_LOG_CTRL->WRITE__LOG(log_msg);

		xPIO_LOG_CTRL->ENABLE__TIME_LOG();
		xPIO_LOG_CTRL->WRITE__LOG("   START   \n");
	}

	iFlag__PIO_LOG = 1;

	// ...
	{
		CString file_name;
		CString log_msg;

		file_name.Format("%s_Mon.log", sObject_Name);

		log_msg  = "\n\n";
		log_msg += "//------------------------------------------------------------------------";

		xE84_LOG_CTRL->CREATE__SUB_DIRECTORY(sObject_Name);
		xE84_LOG_CTRL->SET__PROPERTY(file_name,24*5,60);

		xE84_LOG_CTRL->DISABLE__TIME_LOG();
		xE84_LOG_CTRL->WRITE__LOG(log_msg);

		xE84_LOG_CTRL->ENABLE__TIME_LOG();
		xE84_LOG_CTRL->WRITE__LOG("   START   \n");
	}

	iFlag__E84_LOG = 1;

	// ...
	CString def_name;
	CString ch_name;
	CString obj_name;
	CString var_name;

	// LINK_PIO ...
	{
		def_name = "CH.LINK_PIO.TRANSFER_STATE";			// NO  YES
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, ch_name);
		p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(ch_name, obj_name,var_name);
		LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__LINK_PIO_TRANSFER_STATE, obj_name,var_name);

		//
		def_name = "CH.LINK_PIO.ACTIVE.RUN";				// OFF ON
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, ch_name);
		p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(ch_name, obj_name,var_name);
		LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__LINK_PIO_ACTIVE_RUN, obj_name,var_name);

		def_name = "CH.LINK_PIO.ACTIVE.FA_AUTO";			// OFF ON
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, ch_name);
		p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(ch_name, obj_name,var_name);
		LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__LINK_PIO_ACTIVE_FA_AUTO, obj_name,var_name);

		def_name = "CH.LINK_PIO.ACTIVE.LOAD_REQ";			// OFF ON
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, ch_name);
		p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(ch_name, obj_name,var_name);
		LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__LINK_PIO_ACTIVE_LOAD_REQ, obj_name,var_name);

		def_name = "CH.LINK_PIO.ACTIVE.UNLOAD_REQ";			// OFF ON
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, ch_name);
		p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(ch_name, obj_name,var_name);
		LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__LINK_PIO_ACTIVE_UNLOAD_REQ, obj_name,var_name);
	}

	// OBJ : LPx_INFO ...
	{
		def_name = "CH.FOUP_STATE";      // NONE  EXIST
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, ch_name);
		p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(ch_name, obj_name,var_name);
		LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__LPx_INFO__FOUP_STATE, obj_name,var_name);

		def_name = "CH.CLAMP_STATE";     // CLAMP  UNCLAMP
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, ch_name);
		p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(ch_name, obj_name,var_name);
		LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__LPx_INFO__CLAMP_STATE, obj_name,var_name);
	}

	// OBJ : IO ...
	{
		// DI 
		{
			def_name = "CH.FOUP_PLACED.DI";			// OFF  ON
			p_ext_obj_create->Get__DEF_CONST_DATA(def_name, ch_name);
			p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(ch_name, obj_name,var_name);
			LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__LPx_IO__FOUP_PLACED_DI, obj_name,var_name);

			def_name = "CH.FOUP_PRESENT.DI";		// OFF  ON
			p_ext_obj_create->Get__DEF_CONST_DATA(def_name, ch_name);
			p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(ch_name, obj_name,var_name);
			LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__LPx_IO__FOUP_PRESENT_DI, obj_name,var_name);
		}
		
		// CLAMP : DIO
		{
			def_name = "CH.CLAMP.DO";
			p_ext_obj_create->Get__DEF_CONST_DATA(def_name, ch_name);
			p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(ch_name, obj_name,var_name);
			LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__LPx_IO__CLAMP_DO, obj_name,var_name);

			def_name = "CH.UNCLAMP.DO";
			p_ext_obj_create->Get__DEF_CONST_DATA(def_name, ch_name);
			p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(ch_name, obj_name,var_name);
			LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__LPx_IO__UNCLAMP_DO, obj_name,var_name);

			//
			def_name = "CH.CLAMP.DI";
			p_ext_obj_create->Get__DEF_CONST_DATA(def_name, ch_name);
			p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(ch_name, obj_name,var_name);
			LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__LPx_IO__CLAMP_DI, obj_name,var_name);

			def_name = "CH.UNCLAMP.DI";
			p_ext_obj_create->Get__DEF_CONST_DATA(def_name, ch_name);
			p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(ch_name, obj_name,var_name);
			LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__LPx_IO__UNCLAMP_DI, obj_name,var_name);
		}

		// DI
		{
			def_name = "CH.LIGHT_CURTAIN.DI";
			p_ext_obj_create->Get__DEF_CONST_DATA(def_name, ch_name);
			p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(ch_name, obj_name,var_name);
			LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__LPx_IO__LIGHT_CURTAIN_DI, obj_name,var_name);
		}

		// P(SYSTEM) -> A(OHT) ...
		{
			p_ext_obj_create->Get__DEF_CONST_DATA("VAR__IO_DO_E84_LREQ", ch_name);
			p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(ch_name, obj_name,var_name);
			LINK__EXT_VAR_DIGITAL_CTRL(doEXT_CH__LPx__E84_L_REQ, obj_name,var_name);

			p_ext_obj_create->Get__DEF_CONST_DATA("VAR__IO_DO_E84_UREQ", ch_name);;
			p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(ch_name, obj_name,var_name);
			LINK__EXT_VAR_DIGITAL_CTRL(doEXT_CH__LPx__E84_U_REQ, obj_name,var_name);

			p_ext_obj_create->Get__DEF_CONST_DATA("VAR__IO_DO_E84_NC1", ch_name);
			p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(ch_name, obj_name,var_name);
			LINK__EXT_VAR_DIGITAL_CTRL(doEXT_CH__LPx__E84_NC1, obj_name,var_name);

			p_ext_obj_create->Get__DEF_CONST_DATA("VAR__IO_DO_E84_READY", ch_name);
			p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(ch_name, obj_name,var_name);
			LINK__EXT_VAR_DIGITAL_CTRL(doEXT_CH__LPx__E84_READY, obj_name,var_name);

			p_ext_obj_create->Get__DEF_CONST_DATA("VAR__IO_DO_E84_NC2", ch_name);
			p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(ch_name, obj_name,var_name);
			LINK__EXT_VAR_DIGITAL_CTRL(doEXT_CH__LPx__E84_NC2, obj_name,var_name);

			p_ext_obj_create->Get__DEF_CONST_DATA("VAR__IO_DO_E84_NC3", ch_name);
			p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(ch_name, obj_name,var_name);
			LINK__EXT_VAR_DIGITAL_CTRL(doEXT_CH__LPx__E84_NC3, obj_name,var_name);

			p_ext_obj_create->Get__DEF_CONST_DATA("VAR__IO_DO_E84_HO_AVBL", ch_name);
			p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(ch_name, obj_name,var_name);
			LINK__EXT_VAR_DIGITAL_CTRL(doEXT_CH__LPx__E84_HO_AVBL, obj_name,var_name);

			p_ext_obj_create->Get__DEF_CONST_DATA("VAR__IO_DO_E84_ES", ch_name);
			p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(ch_name, obj_name,var_name);
			LINK__EXT_VAR_DIGITAL_CTRL(doEXT_CH__LPx__E84_ES, obj_name,var_name);
		}

		// P(SYSTEM) <- A(OHT) ...
		{
			p_ext_obj_create->Get__DEF_CONST_DATA("VAR__IO_DI_E84_VALID", ch_name);;
			p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(ch_name, obj_name,var_name);
			LINK__EXT_VAR_DIGITAL_CTRL(diEXT_CH__LPx__E84_VALID, obj_name,var_name);

			p_ext_obj_create->Get__DEF_CONST_DATA("VAR__IO_DI_E84_CS_0", ch_name);
			p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(ch_name, obj_name,var_name);
			LINK__EXT_VAR_DIGITAL_CTRL(diEXT_CH__LPx__E84_CS_0, obj_name,var_name);

			p_ext_obj_create->Get__DEF_CONST_DATA("VAR__IO_DI_E84_CS_1", ch_name);
			p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(ch_name, obj_name,var_name);
			LINK__EXT_VAR_DIGITAL_CTRL(diEXT_CH__LPx__E84_CS_1, obj_name,var_name);

			p_ext_obj_create->Get__DEF_CONST_DATA("VAR__IO_DI_E84_NC1", ch_name);
			p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(ch_name, obj_name,var_name);
			LINK__EXT_VAR_DIGITAL_CTRL(diEXT_CH__LPx__E84_NC1, obj_name,var_name);

			p_ext_obj_create->Get__DEF_CONST_DATA("VAR__IO_DI_E84_TR_REQ", ch_name);
			p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(ch_name, obj_name,var_name);
			LINK__EXT_VAR_DIGITAL_CTRL(diEXT_CH__LPx__E84_TR_REQ, obj_name,var_name);

			p_ext_obj_create->Get__DEF_CONST_DATA("VAR__IO_DI_E84_BUSY", ch_name);
			p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(ch_name, obj_name,var_name);
			LINK__EXT_VAR_DIGITAL_CTRL(diEXT_CH__LPx__E84_BUSY, obj_name,var_name);

			p_ext_obj_create->Get__DEF_CONST_DATA("VAR__IO_DI_E84_COMPT", ch_name);
			p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(ch_name, obj_name,var_name);
			LINK__EXT_VAR_DIGITAL_CTRL(diEXT_CH__LPx__E84_COMPT, obj_name,var_name);

			p_ext_obj_create->Get__DEF_CONST_DATA("VAR__IO_DI_E84_CONT", ch_name);
			p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(ch_name, obj_name,var_name);
			LINK__EXT_VAR_DIGITAL_CTRL(diEXT_CH__LPx__E84_CONT, obj_name,var_name);
		}
	}

	// ...
	{
		SCX__SEQ_INFO x_seq_info;

		iActive__SIM_MODE = x_seq_info->Is__SIMULATION_MODE();
	}

	return 1;
}

//--------------------------------------------------------------------------------
int CObj__OHT_E84_IO::__CALL__CONTROL_MODE(mode, p_debug, p_variable, p_alarm)
{
	// ...
	{
		Fnc__E84_LOG("\n");

		CString log_msg;
		log_msg.Format("Start ... :  [%s]",mode);

		Fnc__APP_MSG(log_msg);
		Fnc__E84_LOG(log_msg);
	}

	// ...
	int flag = -1;

    IF__CTRL_MODE(sMODE__INIT)				flag = Call__INIT(p_variable,p_alarm);

	if(flag < 0)
	{
		CString log_msg;

		Set__AllOff_Except_ES();
		Set__HOAVBL("OFF");

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			log_msg.Format("Obj Aborted ... :  [%s]", mode);
			Fnc__E84_LOG(log_msg);
		}
		else
		{
			log_msg.Format("Aborted ... :  [%s]", mode);

			Fnc__APP_MSG(log_msg);
			Fnc__E84_LOG(log_msg);
		}
	}
	else
	{
		CString log_msg;
		log_msg.Format("<------------ Completed ... :  [%s]", mode);

		Fnc__APP_MSG(log_msg);
		Fnc__E84_LOG(log_msg);
	}
	return flag;
}

int CObj__OHT_E84_IO::__CALL__MONITORING(id, p_variable, p_alarm)
{
	switch(id)
	{
		case MON_ID__ALL_STATE:
			Mon__ALL_STATE(p_variable,p_alarm);
			break;

		case MON_ID__E84_MAIN:
			Mon__IO_E84_MAIN(p_variable,p_alarm);
			break;
	}

	return 1;
}
