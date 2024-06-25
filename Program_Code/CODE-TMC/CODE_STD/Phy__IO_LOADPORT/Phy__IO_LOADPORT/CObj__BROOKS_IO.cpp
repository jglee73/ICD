#include "StdAfx.h"
#include "CObj__BROOKS_IO.h"
#include "CObj__BROOKS_IO__DEF.h"


//--------------------------------------------------------------------------------
CObj__BROOKS_IO::CObj__BROOKS_IO()
{

}
CObj__BROOKS_IO::~CObj__BROOKS_IO()
{

}

//--------------------------------------------------------------------------------
int CObj__BROOKS_IO::__DEFINE__CONTROL_MODE(obj,l_mode)
{
	sObject_Name = obj;

	// ...
	{
		ADD__CTRL_VAR(sMODE__INIT, "INIT");
		ADD__CTRL_VAR(sMODE__HOME, "HOME");

		ADD__CTRL_VAR(sMODE__PREPLOAD,	"PREPLOAD");
		ADD__CTRL_VAR(sMODE__LOAD,		"LOAD");
		ADD__CTRL_VAR(sMODE__UNLOAD,	"UNLOAD");
		ADD__CTRL_VAR(sMODE__RLSUNLOAD, "RLSUNLOAD");
		
		ADD__CTRL_VAR(sMODE__CYCLE,	"CYCLE");
		ADD__CTRL_VAR(sMODE__MAP,	"MAP");
		
		ADD__CTRL_VAR(sMODE__LATCH,	  "LATCH");
		ADD__CTRL_VAR(sMODE__UNLATCH, "UNLATCH");
		
		ADD__CTRL_VAR(sMODE__EXTENDPADDLE,	"EXTENDPADDLE");
		ADD__CTRL_VAR(sMODE__RETRACTPADDLE,	"RETRACTPADDLE");
		ADD__CTRL_VAR(sMODE__UPPADDLE,		"PADDLEUP");
		ADD__CTRL_VAR(sMODE__DOWNPADDLE,	"PADDLEDOWN");

		ADD__CTRL_VAR(sMODE__SHUTTLE_OUT, "SHUTTLE_OUT");
 		ADD__CTRL_VAR(sMODE__SHUTTLE_IN,  "SHUTTLE_IN");
		
		ADD__CTRL_VAR(sMODE__VACON,	 "VACON");
		ADD__CTRL_VAR(sMODE__VACOFF, "VACOFF");
		
		ADD__CTRL_VAR(sMODE__CLAMP,	  "CLAMP");
 		ADD__CTRL_VAR(sMODE__UNCLAMP, "UNCLAMP");
		
		ADD__CTRL_VAR(sMODE__DOOR_CLOSE, "DOOR_CLOSE");
		ADD__CTRL_VAR(sMODE__DOOR_OPEN,	 "DOOR_OPEN");

		ADD__CTRL_VAR(sMODE__OPENDOOR_WITH_NO_UNLATCH, "OPENDOOR_WITH_NO_UNLATCH");

		ADD__CTRL_VAR(sMODE__SIM_FOUP_EXIST, "SIM.FOUP_EXIST");
		ADD__CTRL_VAR(sMODE__SIM_FOUP_NONE,  "SIM.FOUP_NONE");
	}
	return 1;
}
int CObj__BROOKS_IO::__DEFINE__VERSION_HISTORY(version)
{
	version = "JG.Lee ";

	return 1;
}


// ...
#define  MON_ID__IO_MONITOR						1
#define  MON_ID__IO_INFCH_OUT_MONITOR			2

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

// ...
#define APP_DSP__LP_OFF_ON						\
"OFF ON"

#define APP_DSP__LP_UNLOCK_LOCK					\
"NONE UNLOCK LOCK"

#define APP_DSP__LP_WFR_SLOT_STS				\
"UNKNOWN ABSENT PRESENT DOUBLE CROSSED INVALID"

#define APP_DSP__RB_TARGET_MOVE					\
"UNKNOWN HOME READY LP1 LP2 LP3 LP4 LBA LBB VIS1 AL1"


int CObj__BROOKS_IO::__DEFINE__VARIABLE_STD(p_variable)
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

	// CFG ...
	{
		str_name = "CFG.SIM.ERROR.ACTIVE";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, "NO YES", "");
		LINK__VAR_DIGITAL_CTRL(dCH__CFG_SIM_ERROR_ACTIVE, str_name);

		//
		str_name = "PARA.CYCLE.COUNT";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "count", 0, 1, 99999, "");
		LINK__VAR_ANALOG_CTRL(aCH__PARA_CYCLE_COUNT, str_name);

		str_name = "CUR.CYCLE.COUNT";
		STD__ADD_ANALOG(str_name, "count", 0, 0, 99999);
		LINK__VAR_ANALOG_CTRL(aCH__CUR_CYCLE_COUNT, str_name);
	}

	// ACTIVE CHECK ...
	{
		str_name = "ACTIVE.CHECK.CLAMP.DO";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__ACTIVE_CHECK_CLAMP_DO, str_name);

		str_name = "ACTIVE.CHECK.UNCLAMP.DO";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__ACTIVE_CHECK_UNCLAMP_DO, str_name);

		//
		str_name = "ACTIVE.CHECK.CLAMP.DI";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__ACTIVE_CHECK_CLAMP_DI, str_name);

		str_name = "ACTIVE.CHECK.UNCLAMP.DI";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__ACTIVE_CHECK_UNCLAMP_DI, str_name);

		//
		str_name = "ACTIVE.CHECK.DOOR.EXT.DO";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__ACTIVE_CHECK_DOOR_EXT_DO, str_name);

		str_name = "ACTIVE.CHECK.DOOR.RET.DO";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__ACTIVE_CHECK_DOOR_RET_DO, str_name);

		//
		str_name = "ACTIVE.CHECK.DOOR.EXT.DI";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__ACTIVE_CHECK_DOOR_EXT_DI, str_name);

		str_name = "ACTIVE.CHECK.DOOR.RET.DI";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__ACTIVE_CHECK_DOOR_RET_DI, str_name);

		//
		str_name = "ACTIVE.CHECK.DOOR.DOWN.DO";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__ACTIVE_CHECK_DOOR_DN_DO, str_name);

		str_name = "ACTIVE.CHECK.DOOR.DOWN.DI";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__ACTIVE_CHECK_DOOR_DN_DI, str_name);

		//
		str_name = "ACTIVE.CHECK.DOOR.UP.DO";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__ACTIVE_CHECK_DOOR_UP_DO, str_name);

		str_name = "ACTIVE.CHECK.DOOR.UP.DI";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__ACTIVE_CHECK_DOOR_UP_DI, str_name);

		//
		str_name = "ACTIVE.CHECK.LATCH.OPEN.DO";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__ACTIVE_CHECK_LATCH_OPEN_DO, str_name);

		str_name = "ACTIVE.CHECK.LATCH.CLOSE.DO";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__ACTIVE_CHECK_LATCH_CLOSE_DO, str_name);

		//
		str_name = "ACTIVE.CHECK.LATCH.OPEN.DI";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__ACTIVE_CHECK_LATCH_OPEN_DI, str_name);

		str_name = "ACTIVE.CHECK.LATCH.CLOSE.DI";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__ACTIVE_CHECK_LATCH_CLOSE_DI, str_name);

		//
		str_name = "ACTIVE.CHECK.DOOR.VAC.DO";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(dCH__ACTIVE_CHECK_DOOR_VAC_DO, str_name);

		str_name = "ACTIVE.CHECK.DOOR.VAC.DI";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(dCH__ACTIVE_CHECK_DOOR_VAC_DI, str_name);

		//
		str_name = "ACTIVE.CHECK.SHUTTLE.IN.DO";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__ACTIVE_CHECK_SHUTTLE_IN_DO, str_name);

		str_name = "ACTIVE.CHECK.SHUTTLE.IN.DI";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__ACTIVE_CHECK_SHUTTLE_IN_DI, str_name);

		//
		str_name = "ACTIVE.CHECK.SHUTTLE.OUT.DO";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__ACTIVE_CHECK_SHUTTLE_OUT_DO, str_name);

		str_name = "ACTIVE.CHECK.SHUTTLE.OUT.DI";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__ACTIVE_CHECK_SHUTTLE_OUT_DI, str_name);

		//
		str_name = "ACTIVE.CHECK.WAFER.SLID_OUT.DI";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__ACTIVE_CHECK_WAFER_SLID_OUT_DI, str_name);

		str_name = "ACTIVE.CHECK.FOUP.PRESENT.DI";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__ACTIVE_CHECK_FOUP_PRESENT_DI, str_name);

		str_name = "ACTIVE.CHECK.FOUP.PLACED.DI";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__ACTIVE_CHECK_FOUP_PLACED_DI, str_name);
	}

	// ...
	{
		str_name.Format("OTR.OUT.dLP.STATUS");
		STD__ADD_DIGITAL(str_name, "OFFLINE UNINIT NOTREADY STANDBY CTCINUSE MAINTMODE DISABLE");
		LINK__VAR_DIGITAL_CTRL(dCH__OTR_OUT_dLP_STATUS,str_name);
		
		//
		str_name = "MON.DOOR.STATUS";
		STD__ADD_DIGITAL(str_name, "UNKNOWN CLOSE OPEN");
		LINK__VAR_DIGITAL_CTRL(dCH__MON_DOOR_STATUS, str_name);

		str_name = "MON.FOUP.POS.STATUS";
		STD__ADD_DIGITAL(str_name, "UNKNOWN UNLOAD LOAD");
		LINK__VAR_DIGITAL_CTRL(dCH__MON_FOUP_POS_STATUS, str_name);

		str_name = "MON.FOUP.STATUS";
		STD__ADD_DIGITAL(str_name, "UNKNOWN NONE EXIST");
		LINK__VAR_DIGITAL_CTRL(dCH__MON_FOUP_STATUS, str_name);

		str_name = "MON.CLAMP.STATUS";
		STD__ADD_DIGITAL(str_name, "UNKNOWN UNCLAMP CLAMP");
		LINK__VAR_DIGITAL_CTRL(dCH__MON_CLAMP_STATUS, str_name);

		//
		str_name.Format("OTR.OUT.MON.sTRANSFER.PIO.STATUS");
		STD__ADD_DIGITAL(str_name, "NO YES");
		LINK__VAR_DIGITAL_CTRL(dCH__OTR_OUT_dLP_PIO_TRANSFER,str_name);
	}

	//  ...
	{
		str_name = "MON.ACTIVE.FA.AUTO";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__MON_ACTIVE_FA_AUTO, str_name);

		//
		str_name = "OTR.IN.MON.dATM_RB1.ARM"; //INF.dATM_RB1.ARM
		STD__ADD_DIGITAL(str_name, "A B");
		LINK__VAR_DIGITAL_CTRL(dCH__OTR_IN_MON_dATM_RB1_ARM,str_name);

		str_name = "OTR.IN.MON.dATM_RB1.TRG.MOVE"; //INF.dATM_RB1.TRG.MOVE
		STD__ADD_DIGITAL(str_name, APP_DSP__RB_TARGET_MOVE);
		LINK__VAR_DIGITAL_CTRL(dCH__OTR_IN_MON_dATM_RB1_TRG_MOVE,str_name);

		str_name = "OTR.IN.MON.dATM_RB1.ARM_A.ACT"; //INF.dATM_RB1.ARM_A.ACT
		STD__ADD_DIGITAL(str_name, "UNKNOWN EXTEND RETRACT");
		LINK__VAR_DIGITAL_CTRL(dCH__OTR_IN_MON_dATM_RB1_ARM_A_ACT,str_name);

		str_name = "OTR.IN.MON.dATM_RB1.ARM_B.ACT"; //INF.dATM_RB1.ARM_B.ACT
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
			str_name.Format("RES.MAP.SLOT%02d", i+1);
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__RES_MAP_SLOT[i], str_name);
		}
	}

	// ...
	{
		str_name = "CFG.LOAD.CLAMP.OPT";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, "AUTO  BUTTON", "");
		LINK__VAR_DIGITAL_CTRL(dCH__CFG_LOAD_CLAMP_OPT, str_name);

		str_name = "CFG.UNLOAD.CLAMP.OPT";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, "AUTO  BUTTON", "");
		LINK__VAR_DIGITAL_CTRL(dCH__CFG_UNLD_CLAMP_OPT, str_name);

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
		str_name = "CFG.UNLATCH.FOUP.VAC.ON.CHECK";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, "FALSE TRUE", "");
		LINK__VAR_DIGITAL_CTRL(dCFG__UNLATCH_FOUP_VAC_ON_CHECK, str_name);

		str_name = "CFG.LATCH.FOUP.VAC.OFF.CHECK";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, "FALSE TRUE", "");
		LINK__VAR_DIGITAL_CTRL(dCFG__LATCH_FOUP_VAC_OFF_CHECK, str_name);

		//
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

		//
		str_name = "PIO.dCOMM.READY.STS";
		STD__ADD_DIGITAL(str_name, "NOTREADY READY");
		LINK__VAR_DIGITAL_CTRL(dPIO_COMM_READY_STS,str_name);		// PIO와의 통신 가능 상태 채널 ...

		//
		str_name = "CFG.INIT.SLOT.STATE.UNKNOWN";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, "NO YES", "");
		LINK__VAR_DIGITAL_CTRL(dCH__CFG_INIT_SLOT_STATE_UNKNOWN, str_name);
	}

	// ...
	{
  		p_variable->Add__MONITORING_PROC(7.0,MON_ID__IO_MONITOR);
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
	}
	return 1;
}
int CObj__BROOKS_IO::__DEFINE__ALARM(p_alarm)
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
	l_act.Add("ABORT");
	
	ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);

	//.....
	alarm_id = ALID__LOADPORT_OFFLINE_ALARM;

	alarm_title  = title;
	alarm_title += "Offline.";

	alarm_msg  = "Controller is Offline.\n";
	alarm_msg += "Please, Check LoadPort Communication Status !\n";
	
	l_act.RemoveAll();
	l_act.Add("RETRY");
	l_act.Add("ABORT");
	
	ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);

	// ...
	{
		alarm_id = ALID__CMD_TIMEOUT_ALARM;

		alarm_title  = title;
		alarm_title += "Command timeout !";

		alarm_msg  = "After command send, result can't receive. \n";
	
		l_act.RemoveAll();
		l_act.Add(ACT__RETRY);
		l_act.Add(ACT__ABORT);
	
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	//.....
	alarm_id = ALID__LOAD_FOUP_NONE_ALARM;

	alarm_title  = title;
	alarm_title += "LOAD... FOUP NONE ALARM.";

	alarm_msg  = "Before Foup Load Action, Foup Not Exist on the loadport.\n";
	alarm_msg += "Please, Check Foup properly placed !\n";
	
	l_act.RemoveAll();
	l_act.Add("RETRY");
	l_act.Add("ABORT");

	ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);

	//.....
	alarm_id = ALID__LP_WFR_SLIDE_SNS_ALARM;

	alarm_title  = title;
	alarm_title += "LP PADDLE UP... WAFER SLIDE OUT SENSOR ALARM.";

	alarm_msg  = "Before LP Paddle Up Action, Wafer Slide Out Sensor On Status.\n";
	alarm_msg += "Please, Check Wafer Status in the loadport !\n";
	
	l_act.RemoveAll();
	l_act.Add("RETRY");
	l_act.Add("ABORT");

	ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);

	//.....
	alarm_id = ALID__LP_PADDLE_SNS__NOT_EXT_STS_ALARM;

	alarm_title  = title;
	alarm_title += "LP PADDLE UP/DN... PADDLE SENSOR NOT EXT STATUS ALARM.";

	alarm_msg  = "Before LP Paddle Up/Down Action, Paddle Not Extend Status.\n";
	alarm_msg += "Please, Check Paddle Extend Status !\n";
	
	l_act.RemoveAll();
	l_act.Add("RETRY");
	l_act.Add("ABORT");

	ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);

	return 1;
}

int CObj__BROOKS_IO::__Define__USER_FUNCTION(CII_DEFINE__FUNCTION *p_fnc_ctrl)
{
	p_fnc_ctrl->Link__ERROR_FEEDBACK(&mERROR__USER_FNC);

	return 1;
}

//--------------------------------------------------------------------------------
int CObj__BROOKS_IO::__INITIALIZE__OBJECT(p_variable,p_ext_obj_create)
{
	CString def_name;
	CString def_data;
	CString obj_name;
	CString var_name;
	
	// LINK PIO ...
	{
		def_name = "OBJ.LPx_PIO";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, obj_name);

		if((obj_name.CompareNoCase(STR__NO)   == 0)
		|| (obj_name.CompareNoCase(STR__NONE) == 0)
		|| (obj_name.CompareNoCase(STR__NULL) == 0))
		{
			bActive__LINK_PIO = false;
		}
		else
		{
			bActive__LINK_PIO = true;
		}

		if(bActive__LINK_PIO)
		{
			var_name = "MON.ACTIVE.FA.AUTO";
			LINK__EXT_VAR_STRING_CTRL(sEXT_CH__LPx_PIO__MON_ACTIVE_FA_AUTO, obj_name,var_name);

			//
			def_name = "VAR__PIO.RUN_SNS";
			p_ext_obj_create->Get__DEF_CONST_DATA(def_name, var_name);
			LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__LPx_PIO__RUN_SNS, obj_name,var_name);

			def_name = "VAR__PIO.TRANSFER_SNS";
			p_ext_obj_create->Get__DEF_CONST_DATA(def_name, var_name);
			LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__LPx_PIO__TRANSFER_SNS, obj_name,var_name);
		}
	}

	// LINK IO ...
	{
		def_name = "CH__IO_DI.COMM_STS";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);
		p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name,var_name);
		LINK__EXT_VAR_DIGITAL_CTRL(diEXT_CH__LPx__COMM_STS, obj_name,var_name);

		// 
		def_name = "CH__IO_DO.DOOR_UP";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);
		p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name,var_name);
		LINK__EXT_VAR_DIGITAL_CTRL(doEXT_CH__LPx__DOOR_UP, obj_name,var_name);

		def_name = "CH__IO_DI.DOOR_UP";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);
		p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name,var_name);
		LINK__EXT_VAR_DIGITAL_CTRL(diEXT_CH__LPx__DOOR_UP, obj_name,var_name);

		def_name = "CH__IO_DO.DOOR_DN";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);
		p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name,var_name);
		LINK__EXT_VAR_DIGITAL_CTRL(doEXT_CH__LPx__DOOR_DN, obj_name,var_name);

		def_name = "CH__IO_DI.DOOR_DN";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);
		p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name,var_name);
		LINK__EXT_VAR_DIGITAL_CTRL(diEXT_CH__LPx__DOOR_DN, obj_name,var_name);

		def_name = "CH__IO_DO.DOOR_EXT";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);
		p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name,var_name);
		LINK__EXT_VAR_DIGITAL_CTRL(doEXT_CH__LPx__DOOR_EXT, obj_name,var_name);

		def_name = "CH__IO_DI.DOOR_EXT";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);
		p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name,var_name);
		LINK__EXT_VAR_DIGITAL_CTRL(diEXT_CH__LPx__DOOR_EXT, obj_name,var_name);

		def_name = "CH__IO_DO.DOOR_RET";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);
		p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name,var_name);
		LINK__EXT_VAR_DIGITAL_CTRL(doEXT_CH__LPx__DOOR_RET, obj_name,var_name);

		def_name = "CH__IO_DI.DOOR_RET";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);
		p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name,var_name);
		LINK__EXT_VAR_DIGITAL_CTRL(diEXT_CH__LPx__DOOR_RET, obj_name,var_name);

		//
		def_name = "CH__IO_DO.DOOR_VENT";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);

		if((def_data.CompareNoCase(STR__NO)   == 0)
		|| (def_data.CompareNoCase(STR__NONE) == 0)
		|| (def_data.CompareNoCase(STR__NULL) == 0))
		{
			bActive__LPx_DOOR_VENT = false;
		}
		else
		{
			bActive__LPx_DOOR_VENT = true;
		}

		if(bActive__LPx_DOOR_VENT)
		{
			p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name,var_name);
			LINK__EXT_VAR_DIGITAL_CTRL(doEXT_CH__LPx__DOOR_VENT, obj_name,var_name);
		}

		def_name = "CH__IO_DO.DOOR_VAC";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);
		p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name,var_name);
		LINK__EXT_VAR_DIGITAL_CTRL(doEXT_CH__LPx__DOOR_VAC, obj_name,var_name);

		def_name = "CH__IO_DI.DOOR_VAC";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);
		p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name,var_name);
		LINK__EXT_VAR_DIGITAL_CTRL(diEXT_CH__LPx__DOOR_VAC, obj_name,var_name);

		//
		def_name = "CH__IO_DO.LATCH";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);
		p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name,var_name);
		LINK__EXT_VAR_DIGITAL_CTRL(doEXT_CH__LPx__LATCH, obj_name,var_name);

		def_name = "CH__IO_DI.LATCH";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);
		p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name,var_name);
		LINK__EXT_VAR_DIGITAL_CTRL(diEXT_CH__LPx__LATCH, obj_name,var_name);

		def_name = "CH__IO_DO.UNLATCH";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);
		p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name,var_name);
		LINK__EXT_VAR_DIGITAL_CTRL(doEXT_CH__LPx__UNLATCH, obj_name,var_name);

		def_name = "CH__IO_DI.UNLATCH";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);
		p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name,var_name);
		LINK__EXT_VAR_DIGITAL_CTRL(diEXT_CH__LPx__UNLATCH, obj_name,var_name);

		def_name = "CH__IO_DO.SHUTTLE_IN";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);
		p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name,var_name);
		LINK__EXT_VAR_DIGITAL_CTRL(doEXT_CH__LPx__SHUTTLE_IN, obj_name,var_name);

		def_name = "CH__IO_DI.SHUTTLE_IN";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);
		p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name,var_name);
		LINK__EXT_VAR_DIGITAL_CTRL(diEXT_CH__LPx__SHUTTLE_IN, obj_name,var_name);

		def_name = "CH__IO_DO.SHUTTLE_OUT";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);
		p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name,var_name);
		LINK__EXT_VAR_DIGITAL_CTRL(doEXT_CH__LPx__SHUTTLE_OUT, obj_name,var_name);

		def_name = "CH__IO_DI.SHUTTLE_OUT";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);
		p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name,var_name);
		LINK__EXT_VAR_DIGITAL_CTRL(diEXT_CH__LPx__SHUTTLE_OUT, obj_name,var_name);

		def_name = "CH__IO_DO.CLAMP";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);
		p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name,var_name);
		LINK__EXT_VAR_DIGITAL_CTRL(doEXT_CH__LPx__CLAMP, obj_name,var_name);

		def_name = "CH__IO_DO.UNCLAMP";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);
		p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name,var_name);
		LINK__EXT_VAR_DIGITAL_CTRL(doEXT_CH__LPx__UNCLAMP, obj_name,var_name);

		def_name = "CH__IO_DI.CLAMP";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);
		p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name,var_name);
		LINK__EXT_VAR_DIGITAL_CTRL(diEXT_CH__LPx__CLAMP, obj_name,var_name);

		def_name = "CH__IO_DI.UNCLAMP";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);
		p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name,var_name);
		LINK__EXT_VAR_DIGITAL_CTRL(diEXT_CH__LPx__UNCLAMP, obj_name,var_name);

		//
		def_name = "CH__IO_DI.SLIDE_OUT_SNS";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);
		p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name,var_name);
		LINK__EXT_VAR_DIGITAL_CTRL(diEXT_CH__LPx__WFR_SLID_OUT_SNS, obj_name,var_name);

		def_name = "CH__IO_DI.FOUP_PRESENT";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);
		p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name,var_name);
		LINK__EXT_VAR_DIGITAL_CTRL(diEXT_CH__LPx__FOUP_PRESENT, obj_name,var_name);

		def_name = "CH__IO_DI.FOUP_PLACED";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);
		p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name,var_name);
		LINK__EXT_VAR_DIGITAL_CTRL(diEXT_CH__LPx__FOUP_PLACED, obj_name,var_name);

		//
		def_name = "CH__IO_DI.LOAD_BUTTON";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);
		p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name,var_name);
		LINK__EXT_VAR_DIGITAL_CTRL(diEXT_CH__LPx__LOAD_BTN, obj_name,var_name);

		def_name = "CH__IO_DI.UNLOAD_BUTTON";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);
		p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name,var_name);
		LINK__EXT_VAR_DIGITAL_CTRL(diEXT_CH__LPx__UNLOAD_BTN, obj_name,var_name);

		// LAMP ...
		for(int i=0; i<LAMP_MAX; i++)
		{
			def_name.Format("CH__IO_DO.LAMP%02d", i+1);
			p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);
			p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name,var_name);
			LINK__EXT_VAR_DIGITAL_CTRL(doEXT_CH__LPx__LAMP[i], obj_name,var_name);
		}
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

	return 1;
}

//--------------------------------------------------------------------------------
int CObj__BROOKS_IO::__CALL__CONTROL_MODE(mode, p_debug, p_variable, p_alarm)
{
	// ...
	{
		CString log_msg;
		
		log_msg.Format("[%s] Start ... ", mode);
		Fnc__APP_LOG(log_msg);

		sCH__INR_LP__MSG->Set__DATA(log_msg);
	}

	int flag = -1;

	// ACTIVE CHECK : INIT ...
	{
		sCH__ACTIVE_CHECK_CLAMP_DO->Set__DATA(STR__NO);
		sCH__ACTIVE_CHECK_CLAMP_DI->Set__DATA(STR__NO);

		sCH__ACTIVE_CHECK_UNCLAMP_DO->Set__DATA(STR__NO);
		sCH__ACTIVE_CHECK_UNCLAMP_DI->Set__DATA(STR__NO);

		//
		sCH__ACTIVE_CHECK_DOOR_EXT_DO->Set__DATA(STR__NO);
		sCH__ACTIVE_CHECK_DOOR_EXT_DI->Set__DATA(STR__NO);

		sCH__ACTIVE_CHECK_DOOR_RET_DO->Set__DATA(STR__NO);
		sCH__ACTIVE_CHECK_DOOR_RET_DI->Set__DATA(STR__NO);

		//
		sCH__ACTIVE_CHECK_DOOR_DN_DO->Set__DATA(STR__NO);
		sCH__ACTIVE_CHECK_DOOR_UP_DO->Set__DATA(STR__NO);

		sCH__ACTIVE_CHECK_DOOR_DN_DI->Set__DATA(STR__NO);
		sCH__ACTIVE_CHECK_DOOR_UP_DI->Set__DATA(STR__NO);

		//
		sCH__ACTIVE_CHECK_LATCH_OPEN_DO->Set__DATA(STR__NO);
		sCH__ACTIVE_CHECK_LATCH_OPEN_DI->Set__DATA(STR__NO);

		sCH__ACTIVE_CHECK_LATCH_CLOSE_DO->Set__DATA(STR__NO);
		sCH__ACTIVE_CHECK_LATCH_CLOSE_DI->Set__DATA(STR__NO);

		//
		dCH__ACTIVE_CHECK_DOOR_VAC_DO->Set__DATA(STR__NO);
		dCH__ACTIVE_CHECK_DOOR_VAC_DI->Set__DATA(STR__NO);

		//
		sCH__ACTIVE_CHECK_SHUTTLE_IN_DO->Set__DATA(STR__NO);
		sCH__ACTIVE_CHECK_SHUTTLE_IN_DI->Set__DATA(STR__NO);

		sCH__ACTIVE_CHECK_SHUTTLE_OUT_DO->Set__DATA(STR__NO);
		sCH__ACTIVE_CHECK_SHUTTLE_OUT_DI->Set__DATA(STR__NO);

		//
		sCH__ACTIVE_CHECK_WAFER_SLID_OUT_DI->Set__DATA(STR__NO);
		sCH__ACTIVE_CHECK_FOUP_PRESENT_DI->Set__DATA(STR__NO);
		sCH__ACTIVE_CHECK_FOUP_PLACED_DI->Set__DATA(STR__NO);
	}

	// ...
	{
			 IF__CTRL_MODE(sMODE__INIT)				flag = Call__INIT(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__HOME)				flag = Call__INIT(p_variable,p_alarm);

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

		ELSE_IF__CTRL_MODE(sMODE__CYCLE)			flag = Call__CYCLE(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__MAP)				flag = Call__MAP(p_variable,p_alarm);
		
		ELSE_IF__CTRL_MODE(sMODE__LATCH)			flag = Call__LATCH(p_variable,p_alarm);
 		ELSE_IF__CTRL_MODE(sMODE__UNLATCH)			flag = Call__UNLATCH(p_variable,p_alarm);
		
		ELSE_IF__CTRL_MODE(sMODE__EXTENDPADDLE)		flag = Call__EXTENDPADDLE(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__RETRACTPADDLE)	flag = Call__RETRACTPADDLE(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__UPPADDLE)			flag = Call__UPPADDLE(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__DOWNPADDLE)		flag = Call__DOWNPADDLE(p_variable,p_alarm);

		ELSE_IF__CTRL_MODE(sMODE__SHUTTLE_OUT)	    flag = Call__SHUTTLE_OUT(p_variable, p_alarm);
 		ELSE_IF__CTRL_MODE(sMODE__SHUTTLE_IN)	    flag = Call__SHUTTLE_IN(p_variable, p_alarm);
		
		ELSE_IF__CTRL_MODE(sMODE__VACON)			flag = Call__VACON(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__VACOFF)			flag = Call__VACOFF(p_variable,p_alarm);
		
		ELSE_IF__CTRL_MODE(sMODE__CLAMP)			flag = Call__CLAMP(p_variable,p_alarm);
 		ELSE_IF__CTRL_MODE(sMODE__UNCLAMP)			flag = Call__UNCLAMP(p_variable,p_alarm);
	  	
		ELSE_IF__CTRL_MODE(sMODE__DOOR_CLOSE)
		{
			flag = Call__DOOR_CLOSE(p_variable, p_alarm);
			
			if(flag > 0)
			{
				flag = Call__LATCH(p_variable,p_alarm);
			}
			if(flag > 0)
			{
				if(dCFG__LATCH_FOUP_VAC_OFF_CHECK->Check__DATA("TRUE") > 0)
					flag = Call__VACOFF(p_variable,p_alarm);
			}
		}
 		ELSE_IF__CTRL_MODE(sMODE__DOOR_OPEN)
		{
			flag = Call__DOOR_OPEN(p_variable, p_alarm);
		}
		ELSE_IF__CTRL_MODE(sMODE__OPENDOOR_WITH_NO_UNLATCH)
		{
			flag = Call__OPENDOOR_WITH_NO_UNLATCH(p_variable,p_alarm);
		}

		ELSE_IF__CTRL_MODE(sMODE__SIM_FOUP_EXIST)
		{
			flag = 1;

			// ...
			{
				diEXT_CH__LPx__FOUP_PRESENT->Set__DATA(STR__ON);
				diEXT_CH__LPx__FOUP_PLACED->Set__DATA(STR__ON);

				dCH__MON_FOUP_STATUS->Set__DATA(STR__EXIST);
			}
		}
		ELSE_IF__CTRL_MODE(sMODE__SIM_FOUP_NONE)
		{
			flag = 1;

			// ...
			{
				diEXT_CH__LPx__FOUP_PRESENT->Set__DATA(STR__OFF);
				diEXT_CH__LPx__FOUP_PLACED->Set__DATA(STR__OFF);

				dCH__MON_FOUP_STATUS->Set__DATA(STR__NONE);
			}
		}
	}

	if((flag < 0)
	|| (p_variable->Check__CTRL_ABORT() > 0))
	{
		CString log_msg;

		log_msg.Format("[%s] Aborted ... (%1d)", mode, flag);
		Fnc__APP_LOG(log_msg);

		sCH__INR_LP__MSG->Set__DATA(log_msg);

		dCH__OTR_OUT_dLP_STATUS->Set__DATA("MAINTMODE");
	}
	else
	{
		CString log_msg;

		log_msg.Format("[%s] Completed ...", mode);
		Fnc__APP_LOG(log_msg);

		sCH__INR_LP__MSG->Set__DATA(log_msg);
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

int CObj__BROOKS_IO::__CALL__MONITORING(id, p_variable, p_alarm)
{
	switch(id)
	{
		case MON_ID__IO_MONITOR:
			Mon__IO_MONITOR(p_variable,p_alarm);
			break;

		case MON_ID__PIO_TRANSFER_MONITOR:
			Mon__PIO_TRANSFER_MONITOR(p_variable,p_alarm);
			break;

		case MON_ID__IO_INFCH_OUT_MONITOR:	
			Mon__IO_CH_OUT_MONITOR(p_variable,p_alarm);		// OK
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
