#include "StdAfx.h"
#include "CObj__BROOKS_RS232.h"
#include "CObj__BROOKS_RS232__DEF.h"


//--------------------------------------------------------------------------------
CObj__BROOKS_RS232::CObj__BROOKS_RS232()
{

}
CObj__BROOKS_RS232::~CObj__BROOKS_RS232()
{

}

//--------------------------------------------------------------------------------
int CObj__BROOKS_RS232::__DEFINE__CONTROL_MODE(obj,l_mode)
{
	sObject_Name = obj;

	// ...
	{
		ADD__CTRL_VAR(sMODE__INIT,		 "INIT");
		ADD__CTRL_VAR(sMODE__HOME,       "HOME");

		ADD__CTRL_VAR(sMODE__PREPLOAD,	 "PREPLOAD");
		ADD__CTRL_VAR(sMODE__LOAD,		 "LOAD");
		ADD__CTRL_VAR(sMODE__UNLOAD,	 "UNLOAD");
		ADD__CTRL_VAR(sMODE__RLSUNLOAD,	 "RLSUNLOAD");

		ADD__CTRL_VAR(sMODE__MAP,		 "MAP");

		ADD__CTRL_VAR(sMODE__CLAMP,		 "CLAMP");
		ADD__CTRL_VAR(sMODE__UNCLAMP,	 "UNCLAMP");

		ADD__CTRL_VAR(sMODE__CLOSEDOOR,	 "CLOSEDOOR");
		ADD__CTRL_VAR(sMODE__OPENDOOR,	 "OPENDOOR");
	}

	return 1;
}
int CObj__BROOKS_RS232::__DEFINE__VERSION_HISTORY(version)
{
	version = "JG.Lee ";

	return 1;
}


// ...
#define  MON_ID__IO_MONITOR						1
#define  MON_ID__IO_SET_VAL_LOWER_OBJ           2
#define  MON_ID__IO_INFCH_OUT_MONITOR			3

#define  MON_ID__LD_BTN_MONITOR					5
#define  MON_ID__UNLD_BTN_MONITOR				6

#define  MON_ID__CMD_RST_MONITOR				7
#define  MON_ID__RECV_QUE_MONITOR				8

// ...
#define  MON_ID__E84MAIN_MONITOR				9
#define	 MON_ID__IO_STATUS_CH_MONITOR			10
#define	 MON_ID__IO_PIO_LOG_MONITOR				11
#define	 MON_ID__IO_VALID_MONITOR				12
#define	 MON_ID__IO_HO_AVBL_MONITOR				13
#define	 MON_ID__IO_LIGHT_CURTAIN_MONITOR		14
#define	 MON_ID__IO_CLAMP_STS_MONITOR			15
#define  MON_ID__PIO_TRANSFER_MONITOR	        16
#define  MON_ID__IO_CST_EXIST_MONITOR			17

// ...
#define APP_DSP__LP_OFF_ON						\
"OFF ON"

#define APP_DSP__LP_UNLOCK_LOCK					\
"NONE UNLOCK LOCK"

#define APP_DSP__LP_WFR_SLOT_STS				\
"UNKNOWN ABSENT PRESENT DOUBLE CROSSED INVALID"

#define APP_DSP__RB_TARGET_MOVE					\
"UNKNOWN HOME READY LP1 LP2 LP3 LP4 LBA LBB VIS1 AL1"


int CObj__BROOKS_RS232::__DEFINE__VARIABLE_STD(p_variable)
{
	DECLARE__STD_VARIABLE

	CCommon_Error__DEF_VARIABLE m_err_def_variable;
	p_variable->Link__DEF_VARIABLE__ERROR_FEEDBACK(&m_err_def_variable);

	// ...
	CString str_name;

	// ...
	{
		str_name = "APP.CMD.TIMEOUT.CNT";
		STD__ADD_STRING(str_name);
	}

	// ...
	{
		str_name = "OTR.IN.CFG.aLOAD.COMPLETE.DELAY.TIME";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "sec", 0, 0, 30, "");
		LINK__VAR_ANALOG_CTRL(aCH__OTR_IN_CFG__LOAD_COMPT_DELAY_TIME,str_name);

		str_name = "sAPP.COMM.STS";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__COMM_STS,str_name);

		str_name = "INR.sLP.MSG";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__INR_LP__MSG,str_name);
	}

	// ...
	{
		str_name.Format("OTR.OUT.dLP.STATUS");
		STD__ADD_DIGITAL(str_name, "OFFLINE UNINIT NOTREADY STANDBY CTCINUSE MAINTMODE DISABLE");
		LINK__VAR_DIGITAL_CTRL(dCH__OTR_OUT_dLP_STATUS,str_name);

		str_name.Format("OTR.OUT.MON.dFOUP.EXIST.FLAG");
		STD__ADD_DIGITAL(str_name, "UNKNOWN NONE EXIST");
		LINK__VAR_DIGITAL_CTRL(dCH__OTR_OUT_MON_dLP_CST_STS,str_name);

		str_name.Format("OTR.OUT.MON.dDOOR.STATUS");
		STD__ADD_DIGITAL(str_name, "UNKNOWN CLOSE OPEN");
		LINK__VAR_DIGITAL_CTRL(dCH__OTR_OUT_MON_dLP_DOOR_STS,str_name);

		str_name.Format("OTR.OUT.MON.dFOUP.POS.STATUS");
		STD__ADD_DIGITAL(str_name, "UNKNOWN UNLOAD LOAD");
		LINK__VAR_DIGITAL_CTRL(dCH__OTR_OUT_MON_dLP_SHUTL_STS,str_name);

		str_name.Format("OTR.OUT.MON.dFOUP.CLAMP.STATUS");
		STD__ADD_DIGITAL(str_name, "UNKNOWN UNCLAMP CLAMP");
		LINK__VAR_DIGITAL_CTRL(dCH__OTR_OUT_MON_dLP_CLAMP_STS,str_name);

		str_name.Format("OTR.OUT.MON.sTRANSFER.PIO.STATUS");
		STD__ADD_DIGITAL(str_name, "NO YES");
		LINK__VAR_DIGITAL_CTRL(dCH__OTR_OUT_dLP_PIO_TRANSFER,str_name);
	}

	// ...
	{
		str_name = "OTR.IN.sLP.FA.MODE"; 
		STD__ADD_STRING(str_name);								// "MANUAL AUTO"
		LINK__VAR_STRING_CTRL(sCH__OTR_IN_LP__FA_MODE,str_name);

		str_name = "OTR.IN.sLP.FA.SERVICE.MODE";
		STD__ADD_STRING(str_name);								// "NO YES"
		LINK__VAR_STRING_CTRL(sCH__OTR_IN_LP__FA_SERVICE_MODE,str_name);

		str_name = "OTR.IN.MON.dATM_RB1.ARM";					//INF.dATM_RB1.ARM
		STD__ADD_DIGITAL(str_name, "A B");
		LINK__VAR_DIGITAL_CTRL(dCH__OTR_IN_MON_dATM_RB1_ARM,str_name);

		str_name = "OTR.IN.MON.dATM_RB1.TRG.MOVE";				//INF.dATM_RB1.TRG.MOVE
		STD__ADD_DIGITAL(str_name, APP_DSP__RB_TARGET_MOVE);
		LINK__VAR_DIGITAL_CTRL(dCH__OTR_IN_MON_dATM_RB1_TRG_MOVE,str_name);

		str_name = "OTR.IN.MON.dATM_RB1.ARM_A.ACT";				//INF.dATM_RB1.ARM_A.ACT
		STD__ADD_DIGITAL(str_name, "UNKNOWN EXTEND RETRACT");
		LINK__VAR_DIGITAL_CTRL(dCH__OTR_IN_MON_dATM_RB1_ARM_A_ACT,str_name);

		str_name = "OTR.IN.MON.dATM_RB1.ARM_B.ACT";				//INF.dATM_RB1.ARM_B.ACT
		STD__ADD_DIGITAL(str_name, "UNKNOWN EXTEND RETRACT");
		LINK__VAR_DIGITAL_CTRL(dCH__OTR_IN_MON_dATM_RB1_ARM_B_ACT,str_name);

		str_name = "OTR.IN.sLP.RELOAD.FLAG";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(dCH__OTR_IN_sLP_RELOAD_FLAG,str_name);
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

		for(int i=0; i<WAFER_SLOT_MAX; i++)
		{
			str_name.Format("OTR.OUT.MON.dSLOT%02d.MAP.RESULT", i+1);
			STD__ADD_DIGITAL(str_name, APP_DSP__LP_WFR_SLOT_STS);
			LINK__VAR_DIGITAL_CTRL(dCH__LP_SLOT_STS[i],str_name);
		}
	}

	// ...
	{
		str_name = "INR.dCFG.DYNAMIC.USE";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, "NO YES", "");
		LINK__VAR_DIGITAL_CTRL(dCH__INR_CFG_DYNAMIC_USE,str_name);

		str_name = "INR.dCFG.UNLOADCLAMP.OPT";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, "Type2 Type1", "");	// Type2:Auto, type1:Button
		LINK__VAR_DIGITAL_CTRL(dCH__INR_CFG_UNLD_CLAMP_OPT,str_name);

		//
		str_name = "INR.sE84.TIME.CNT.CH";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__INR_TIME_CNT_CH,str_name);

		str_name = "INR.dLP.CLAMP.STS";
		STD__ADD_DIGITAL(str_name, APP_DSP__LP_OFF_ON);
		LINK__VAR_DIGITAL_CTRL(dCH__LP__CLAMP_STS,str_name);

		str_name = "INR.dLP.UNCLAMP.STS";
		STD__ADD_DIGITAL(str_name, APP_DSP__LP_OFF_ON);
		LINK__VAR_DIGITAL_CTRL(dCH__LP__UNCLAMP_STS,str_name);

		// ...
		str_name = "OTR.OUT.sLP.OBJ.STS";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__LP__OBJ_STS,str_name);

		str_name = "OTR.OUT.sLP.MODE";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__LP__OBJ_MODE,str_name);

		// ...
		str_name = "CFG.dDOOR.CLOSE.VAC.ON.RECHECK";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, "FALSE TRUE", "");
		LINK__VAR_DIGITAL_CTRL(dCFG__DOOR_CLOSE_VAC_ON_RECHECK,str_name);

		str_name = "CFG.dBEFORE.LOAD.CST.CHECK";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, "TRUE FALSE", "");
		LINK__VAR_DIGITAL_CTRL(dCFG__BEFORE_LOAD_CST_CHECK,str_name);

		str_name = "CFG.dWAFER.SLIDE.SNS.CHECK";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, "FALSE TRUE", "");
		LINK__VAR_DIGITAL_CTRL(dCFG__WAFER_SLIDE_SNS_CHECK,str_name);

		str_name = "CFG.dLOAD.MODE";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, "MODE1 MODE2", "rec:MODE1");
		LINK__VAR_DIGITAL_CTRL(dCFG__LOAD_MODE,str_name);

		str_name = "CFG.aACCESS.SW.TIMEOUT";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "sec", 0, 1, 600, "rec:1");
		LINK__VAR_ANALOG_CTRL(aCFG_ACCESS_SW_TIMEOUT,str_name);

		str_name = "CFG.aCST.PROPERLY.EXIST.TIME";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "msec", 0, 500, 2000, "rec:500");
		LINK__VAR_ANALOG_CTRL(aCFG_CST_PROPERLY_EXIST_TIME,str_name);

		str_name = "CFG.dPOST.CARRIER.ALARM";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, "UNKNOWN DONOTPOST", "");
		LINK__VAR_DIGITAL_CTRL(dCFG_POST_CARRIER_ALARM,str_name);

		str_name = "CFG.aPADDLE.DOWN.DELAY";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "sec", 0, 1, 12, "rec:1");
		LINK__VAR_ANALOG_CTRL(aCFG_PADDLE_DOWN_DELAY,str_name);

		// ...
		str_name = "PIO.dCOMM.READY.STS";
		STD__ADD_DIGITAL(str_name, "NOTREADY READY");
		LINK__VAR_DIGITAL_CTRL(dPIO_COMM_READY_STS,str_name);		// PIO와의 통신 가능 상태 채널..
	}

	// ...
	{
		str_name = "CFG.MAPPING.SENSOR.USE";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, "NO", "");
		LINK__VAR_DIGITAL_CTRL(dCH__CFG_MAPPING_SENSOR_USE, str_name);

		str_name = "CFG.CID.TAG.SENSOR.USE";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, "NO", "");
		LINK__VAR_DIGITAL_CTRL(dCH__CFG_CID_TAG_SENSOR_USE, str_name);
	}

	// ...
	{
		// p_variable->Add__MONITORING_PROC(7.0,MON_ID__IO_MONITOR);
		p_variable->Add__MONITORING_PROC(1.0,MON_ID__IO_SET_VAL_LOWER_OBJ);
		p_variable->Add__MONITORING_PROC(1.0,MON_ID__IO_INFCH_OUT_MONITOR);

		//
		p_variable->Add__MONITORING_PROC(1.0,MON_ID__LD_BTN_MONITOR);
		p_variable->Add__MONITORING_PROC(1.0,MON_ID__UNLD_BTN_MONITOR);

		p_variable->Add__MONITORING_PROC(1.0,MON_ID__E84MAIN_MONITOR);
		p_variable->Add__MONITORING_PROC(1.0,MON_ID__IO_STATUS_CH_MONITOR);
		p_variable->Add__MONITORING_PROC(1.0,MON_ID__IO_PIO_LOG_MONITOR);
		p_variable->Add__MONITORING_PROC(1.0,MON_ID__IO_VALID_MONITOR);
		p_variable->Add__MONITORING_PROC(1.0,MON_ID__IO_HO_AVBL_MONITOR);
		p_variable->Add__MONITORING_PROC(1.0,MON_ID__IO_LIGHT_CURTAIN_MONITOR);
		p_variable->Add__MONITORING_PROC(1.0,MON_ID__IO_CLAMP_STS_MONITOR);
		p_variable->Add__MONITORING_PROC(1.0,MON_ID__PIO_TRANSFER_MONITOR);
		p_variable->Add__MONITORING_PROC(1.0,MON_ID__IO_CST_EXIST_MONITOR);
	}
	return 1;
}
int CObj__BROOKS_RS232::__DEFINE__ALARM(p_alarm)
{
	DECLARE__ALARM;

	// ...
	CString title;
	title.Format("%s - ",sObject_Name);

	CString alarm_title;
	CString alarm_msg;
	CStringArray l_act;
	int alarm_id;

	// ...
	{
		alarm_id = ALID__OFFLINE_ALARM;

		alarm_title  = title;
		alarm_title += "Offline.";

		alarm_msg  = "Communication is Offline.\n";
		alarm_msg += "Please, Check DeviceNet Communication Status !\n";

		l_act.RemoveAll();
		l_act.Add(ACT__RETRY);
		l_act.Add(ACT__ABORT);

		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}
	// ...
	{
		alarm_id = ALID__LOADPORT_OFFLINE_ALARM;

		alarm_title  = title;
		alarm_title += "Offline.";

		alarm_msg  = "Controller is Offline.\n";
		alarm_msg += "Please, Check LoadPort Communication Status !\n";

		l_act.RemoveAll();
		l_act.Add(ACT__RETRY);
		l_act.Add(ACT__ABORT);

		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}
	// ...
	{
		alarm_id = ALID__CMD_TIMEOUT_ALARM;

		alarm_title  = title;
		alarm_title += "Command Timeout.";

		alarm_msg  = "After Command Send, Result Can't Receive.\n";

		l_act.RemoveAll();
		l_act.Add(ACT__RETRY);
		l_act.Add(ACT__ABORT);

		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}
	// ...
	{
		alarm_id = ALID__LOAD_FOUP_NONE_ALARM;

		alarm_title  = title;
		alarm_title += "LOAD... FOUP NONE ALARM.";

		alarm_msg  = "Before Foup Load Action, Foup Not Exist on the loadport.\n";
		alarm_msg += "Please, Check Foup properly placed !\n";

		l_act.RemoveAll();
		l_act.Add(ACT__RETRY);
		l_act.Add(ACT__ABORT);

		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}
	// ...
	{
		alarm_id = ALID__LP_WFR_SLIDE_SNS_ALARM;

		alarm_title  = title;
		alarm_title += "LP PADDLE UP... WAFER SLIDE OUT SENSOR ALARM.";

		alarm_msg  = "Before LP Paddle Up Action, Wafer Slide Out Sensor On Status.\n";
		alarm_msg += "Please, Check Wafer Status in the loadport !\n";

		l_act.RemoveAll();
		l_act.Add(ACT__RETRY);
		l_act.Add(ACT__ABORT);

		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}
	// ...
	{
		alarm_id = ALID__LP_PADDLE_SNS__NOT_EXT_STS_ALARM;

		alarm_title  = title;
		alarm_title += "LP PADDLE UP/DN... PADDLE SENSOR NOT EXT STATUS ALARM.";

		alarm_msg  = "Before LP Paddle Up/Down Action, Paddle Not Extend Status.\n";
		alarm_msg += "Please, Check Paddle Extend Status !\n";

		l_act.RemoveAll();
		l_act.Add(ACT__RETRY);
		l_act.Add(ACT__ABORT);

		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}
	// ...
	{
		alarm_id = ALID__LP_MAPPING_SENSOR_NOT_USE;

		alarm_title  = title;
		alarm_title += "LP Mapping Sensor Not Use";

		alarm_msg = "Please, check config option !\n";

		l_act.RemoveAll();
		l_act.Add(ACT__RETRY);
		l_act.Add(ACT__ABORT);

		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	return 1;
}

int CObj__BROOKS_RS232::__Define__USER_FUNCTION(CII_DEFINE__FUNCTION *p_fnc_ctrl)
{
	p_fnc_ctrl->Link__ERROR_FEEDBACK(&mERROR__USER_FNC);

	return 1;
}

//--------------------------------------------------------------------------------
int CObj__BROOKS_RS232::__INITIALIZE__OBJECT(p_variable,p_ext_obj_create)
{
	CString def_name;
	CString def_data;
	CString str_name;
	CString obj_name;
	int i;

	// LPx OBJECT
	{
		def_name = "OBJ.LPx";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, obj_name);
	
		pLPx__OBJ_CTRL = p_ext_obj_create->Create__OBJECT_CTRL(obj_name);

		//
		str_name = "dCH.COMM.STATE";
		LINK__EXT_VAR_DIGITAL_CTRL(diEXT_CH__LPx__COMM_STS, obj_name,str_name);

		//
		str_name = "MON.DOOR.STATUS";
		LINK__EXT_VAR_STRING_CTRL(sEXT_CH__LPx__MON_DOOR_STATUS, obj_name,str_name);

		str_name = "MON.CLAMP.STATUS";
		LINK__EXT_VAR_STRING_CTRL(sEXT_CH__LPx__MON_CLAMP_STATUS, obj_name,str_name);

		str_name = "MON.FOUP.STATUS";
		LINK__EXT_VAR_STRING_CTRL(sEXT_CH__LPx__MON_FOUP_STATUS, obj_name,str_name);

		str_name = "MON.FOUP.POS.STATUS";
		LINK__EXT_VAR_STRING_CTRL(sEXT_CH__LPx__MON_FOUP_POS_STATUS, obj_name,str_name);

		//
		str_name = "MON.WFR.SLIDE_OUT.SNS";
		LINK__EXT_VAR_STRING_CTRL(sEXT_CH__LPx__MON_WFR_SLID_OUT_SNS, obj_name,str_name);

		str_name = "MON.LOAD_BUTTON.SNS";
		LINK__EXT_VAR_STRING_CTRL(sEXT_CH__LPx__MON_LOAD_BTN_SNS, obj_name,str_name);

		str_name = "MON.UNLOAD_BUTTON.SNS";
		LINK__EXT_VAR_STRING_CTRL(sEXT_CH__LPx__MON_UNLOAD_BTN_SNS, obj_name,str_name);

		//
		for(i=0; i<LAMP_MAX; i++)
		{
			str_name.Format("LINK.LAMP%1d.SET", i+1);
			LINK__EXT_VAR_STRING_CTRL(sEXT_CH__LPx__LINK_LAMP_SET[i], obj_name,str_name);
		}
	}

	// LPx_PIO OBJECT -----
	{
		def_name.Format("VAR__IO_PHY_LPx_PIO_FA_MODE");
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);
		p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name, str_name);
		LINK__EXT_VAR_STRING_CTRL(sEXT_CH__PHY_LPx__PIO_FA_MODE, obj_name,str_name);

		//...
		def_name.Format("VAR__IO_PHY_LPx_PIO_SERV_REQ");
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);
		p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name, str_name);
		LINK__EXT_VAR_STRING_CTRL(sEXT_CH__PHY_LPx__PIO_SERV_REQ, obj_name,str_name);

		//...
		def_name.Format("VAR__IO_PHY_LPx_PIO_MODE");
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);
		p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name, str_name);
		LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__PHY_LPx__PIO_MODE, obj_name,str_name);

		//...
		def_name.Format("VAR__IO_PHY_LPx_PIO_RUN_SNS");
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);
		p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name, str_name);
		LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__PHY_LPx__PIO_RUN_SNS, obj_name,str_name);

		//...
		def_name.Format("VAR__IO_PHY_LPx_PIO_TRANSFER_SNS");
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);
		p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name, str_name);
		LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__PHY_LPx__PIO_TRANSFER_SNS, obj_name,str_name);

	}


	// ...
	{
		SCX__SEQ_INFO x_seq_info;

		if(x_seq_info->Is__SIMULATION_MODE() > 0)		iSIM_FLAG =  1;
		else											iSIM_FLAG = -1;
	}

	iFlag__APP_LOG = 1;

	// ...
	{
		CString file_name;
		CString log_msg;

		file_name.Format("%s_App.log", sObject_Name);

		log_msg  = "\n\n";
		log_msg += "//------------------------------------------------------------------------";

		xAPP_LOG_CTRL->CREATE__SUB_DIRECTORY(sObject_Name);
		xAPP_LOG_CTRL->SET__PROPERTY(file_name,24*5,60);

		xAPP_LOG_CTRL->DISABLE__TIME_LOG();
		xAPP_LOG_CTRL->WRITE__LOG(log_msg);

		xAPP_LOG_CTRL->ENABLE__TIME_LOG();
		xAPP_LOG_CTRL->WRITE__LOG("   START   \n");
	}

	iFlag__MON_CH_LOG = 1;

	// ...
	{
		CString file_name;
		CString log_msg;

		file_name.Format("%s_Mon.log", sObject_Name);

		log_msg  = "\n\n";
		log_msg += "//------------------------------------------------------------------------";

		xMON_CH_LOG_CTRL->CREATE__SUB_DIRECTORY(sObject_Name);
		xMON_CH_LOG_CTRL->SET__PROPERTY(file_name,24*5,60);

		xMON_CH_LOG_CTRL->DISABLE__TIME_LOG();
		xMON_CH_LOG_CTRL->WRITE__LOG(log_msg);

		xMON_CH_LOG_CTRL->ENABLE__TIME_LOG();
		xMON_CH_LOG_CTRL->WRITE__LOG("   START   \n");
	}
	return 1;
}

//--------------------------------------------------------------------------------
int CObj__BROOKS_RS232::__CALL__CONTROL_MODE(mode, p_debug, p_variable, p_alarm)
{
	// ...
	{
		CString log_msg;

		log_msg.Format("------------> Start ... :  [%s] ------------>",mode);
		Fnc__APP_LOG(log_msg);

		log_msg.Format("%s Starting ...", mode);
		Fnc__APP_MSG(log_msg);
	}

	int flag = -1;

	// ...
	{
		IF__CTRL_MODE(sMODE__INIT)
		{
			flag = Call__INIT(p_variable,p_alarm);
		}
		ELSE_IF__CTRL_MODE(sMODE__HOME)
		{
			flag = Call__HOME(p_variable,p_alarm);
		}

		ELSE_IF__CTRL_MODE(sMODE__PREPLOAD)
		{
			sCH__LP__OBJ_STS->Set__DATA("RUN");
			sCH__LP__OBJ_MODE->Set__DATA(sMODE__PREPLOAD);

			// ...
			{
				Seq_LAMP(LAMP_UNLOAD, "OFF", "UNLOAD_LAMP", "PROC__SEQ");
				Seq_LAMP(LAMP_LOAD,   "ON",  "LOAD_LAMP",   "PROC__SEQ");
			}

			flag = Call__PREPLOAD(p_variable,p_alarm);
		}
		ELSE_IF__CTRL_MODE(sMODE__LOAD)
		{
			// ...
			{
				Seq_LAMP(LAMP_UNLOAD, "OFF", "UNLOAD_LAMP", "PROC__SEQ");
				Seq_LAMP(LAMP_LOAD,   "OFF", "LOAD_LAMP",   "PROC__SEQ");
			}

			flag = Call__LOAD(p_variable,p_alarm);
		}
		ELSE_IF__CTRL_MODE(sMODE__RLSUNLOAD)
		{
			flag = Call__RLSUNLOAD(p_variable,p_alarm);
		}
		ELSE_IF__CTRL_MODE(sMODE__UNLOAD)
		{
			sCH__LP__OBJ_STS->Set__DATA("RUN");
			sCH__LP__OBJ_MODE->Set__DATA(sMODE__UNLOAD);

			// ...
			{
				Seq_LAMP(LAMP_UNLOAD, "ON",  "UNLOAD_LAMP", "PROC__SEQ");
				Seq_LAMP(LAMP_LOAD,   "OFF", "LOAD_LAMP",   "PROC__SEQ");
			}

			flag = Call__UNLOAD(p_variable,p_alarm);
			if(flag <= 0)
			{
				if(Is__RELOAD_FLAG_YES(p_variable) > 0)		flag = 1;
			}
		}

		ELSE_IF__CTRL_MODE(sMODE__MAP)				flag = Call__MAP(p_variable,p_alarm);

		ELSE_IF__CTRL_MODE(sMODE__CLAMP)			flag = Call__CLAMP(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__UNCLAMP)			flag = Call__UNCLAMP(p_variable,p_alarm);

		ELSE_IF__CTRL_MODE(sMODE__CLOSEDOOR)		flag = Call__CLOSEDOOR(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__OPENDOOR)			flag = Call__OPENDOOR(p_variable,p_alarm);
	}

	if(flag < 0)
	{
		CString log_msg;

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			log_msg.Format("<------------ Obj Aborted ... :  [%s]\n", mode);
			Fnc__APP_LOG(log_msg);
		}
		else
		{
			log_msg.Format("<------------ Aborted ... :  [%s]\n",mode);
			Fnc__APP_LOG(log_msg);
		}

		dCH__OTR_OUT_dLP_STATUS->Set__DATA("MAINTMODE");

		log_msg.Format("%s Aborted ...", mode);
		Fnc__APP_MSG(log_msg);
	}
	else
	{
		CString log_msg;

		log_msg.Format("<------------ Completed ... :  [%s]\n",mode);
		Fnc__APP_LOG(log_msg);

		log_msg.Format("%s Completed ...", mode);
		Fnc__APP_MSG(log_msg);
	}

	// ...
	{
		CString str_var;
		p_variable->Get__CTRL_STATUS(str_var);

		sCH__LP__OBJ_STS->Set__DATA(str_var);
		sCH__LP__OBJ_MODE->Set__DATA(mode);
	}
	return flag;
}

int CObj__BROOKS_RS232::__CALL__MONITORING(id, p_variable, p_alarm)
{
	switch(id)
	{
		case MON_ID__IO_MONITOR:
			Mon__IO_MONITOR(p_variable,p_alarm);
			break;

		case MON_ID__IO_SET_VAL_LOWER_OBJ:
			Mon__SetVal_To_Lower_Obj(p_variable,p_alarm);
			break;

		case MON_ID__PIO_TRANSFER_MONITOR:
			Mon__PIO_TRANSFER_MONITOR(p_variable,p_alarm);
			break;

		case MON_ID__IO_INFCH_OUT_MONITOR:	
			Mon__IO_CH_OUT_MONITOR(p_variable,p_alarm);			// OK
			break;

		case MON_ID__IO_CST_EXIST_MONITOR:	
			Mon__IO_CST_EXIST_MONITOR(p_variable,p_alarm);		// OK
			break;

		case MON_ID__LD_BTN_MONITOR:	
			Fnc__LD_buttonMon(p_variable,p_alarm);
			break;

		case MON_ID__UNLD_BTN_MONITOR:	
			Fnc__UNLD_buttonMon(p_variable,p_alarm);
			break;
	}

	return 1;
}
