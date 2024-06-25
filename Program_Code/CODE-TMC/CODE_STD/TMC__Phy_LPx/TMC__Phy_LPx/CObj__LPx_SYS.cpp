#include "StdAfx.h"
#include "CObj__LPx_SYS.h"
#include "CObj__LPx_SYS__ALID.h"

#include "Macro_Function.h"


//-------------------------------------------------------------------------
CObj__LPx_SYS::CObj__LPx_SYS()
{

}
CObj__LPx_SYS::~CObj__LPx_SYS()
{

}

//-------------------------------------------------------------------------
int CObj__LPx_SYS::__DEFINE__CONTROL_MODE(obj,l_mode)
{
	sObject_Name = obj;

	// ...
	{
		ADD__CTRL_VAR(sMODE__INIT, "INIT");
		ADD__CTRL_VAR(sMODE__HOME, "HOME");

		ADD__CTRL_VAR(sMODE__ALARM_RESET, "ALARM.RESET");

		ADD__CTRL_VAR(sMODE__DOOR_OPEN,  "DOOR_OPEN");
		ADD__CTRL_VAR(sMODE__CHECK_DOOR_OPEN, "CHECK_DOOR_OPEN");

		ADD__CTRL_VAR(sMODE__LOAD,   "LOAD");
		ADD__CTRL_VAR(sMODE__UNLOAD, "UNLOAD");

		ADD__CTRL_VAR(sMODE__PREPLOAD,  "PREPLOAD");
		ADD__CTRL_VAR(sMODE__RLSUNLOAD, "RLSUNLOAD");

		ADD__CTRL_VAR(sMODE__CYCLE, "CYCLE");

		ADD__CTRL_VAR(sMODE__CID_READ,	"CID_READ");
		ADD__CTRL_VAR(sMODE__PAGE_READ,	"PAGE_READ");

		ADD__CTRL_VAR(sMODE__CID_WRITE,	 "CID_WRITE");
		ADD__CTRL_VAR(sMODE__PAGE_WRITE, "PAGE_WRITE");

		ADD__CTRL_VAR(sMODE__MAP, "MAP");

		ADD__CTRL_VAR(sMODE__CLAMP,	  "CLAMP");
		ADD__CTRL_VAR(sMODE__UNCLAMP, "UNCLAMP");

		ADD__CTRL_VAR(sMODE__SHUTTLE_IN,  "SHUTTLE_IN");
		ADD__CTRL_VAR(sMODE__SHUTTLE_OUT, "SHUTTLE_OUT");

		// 
		ADD__CTRL_VAR(sMODE__EXTENDPADDLE,  "EXTENDPADDLE");
		ADD__CTRL_VAR(sMODE__RETRACTPADDLE, "RETRACTPADDLE");
		ADD__CTRL_VAR(sMODE__PADDLEDOWN,    "PADDLEDOWN");
		ADD__CTRL_VAR(sMODE__PADDLEUP,      "PADDLEUP");
		ADD__CTRL_VAR(sMODE__LATCH,      "LATCH");
		ADD__CTRL_VAR(sMODE__UNLATCH,    "UNLATCH");
		ADD__CTRL_VAR(sMODE__CLOSEDOOR,  "CLOSEDOOR");

		ADD__CTRL_VAR(sMODE__OPENDOOR,   "OPENDOOR");
		ADD__CTRL_VAR(sMODE__OPENDOOR_WITH_NO_UNLATCH, "OPENDOOR_WITH_NO_UNLATCH");

		ADD__CTRL_VAR(sMODE__VACON,      "VACON");
		ADD__CTRL_VAR(sMODE__VACOFF,     "VACOFF");

		ADD__CTRL_VAR(sMODE__EXTENDSHUTTLE,  "SHUTTLEOUT");
		ADD__CTRL_VAR(sMODE__RETRACTSHUTTLE, "SHUTTLEIN");
	}

	return 1;
}

int CObj__LPx_SYS::__DEFINE__VERSION_HISTORY(version)
{

	return 1;
}


// ...
#define  MON_ID__IO_MONITOR				1


int CObj__LPx_SYS::__DEFINE__VARIABLE_STD(p_variable)
{
	DECLARE__STD_VARIABLE;

	CCommon_Error__DEF_VARIABLE m_err_def_variable;
	p_variable->Link__DEF_VARIABLE__ERROR_FEEDBACK(&m_err_def_variable);

	// ...
	CString str_name;
	CString dsc_item_list;
	int i;

	// OBJ ...
	{
		str_name = "OBJ.MSG";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__OBJ_MSG, str_name);
	}

	// PARA ...
	{
		str_name = "PARA.PAGE.ID";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "page", 0, 1, 17, "");
		LINK__VAR_ANALOG_CTRL(aCH__PARA_PAGE_ID, str_name);

		str_name = "PARA.PAGE.CID";
		STD__ADD_STRING_WITH_X_OPTION(str_name, "");
		LINK__VAR_STRING_CTRL(sCH__PARA_PAGE_CID, str_name);
		
		str_name = "PARA.PAGE.DATA";
		STD__ADD_STRING_WITH_X_OPTION(str_name, "");
		LINK__VAR_STRING_CTRL(sCH__PARA_PAGE_DATA, str_name);
	}

	// MON ...
	{
		p_variable->Get__STD_DESCRIPTION(DSP__SLOT_STATUS,dsc_item_list);

		for(i=0;i<CFG_LPx__SLOT_MAX;i++)
		{
			int id = i + 1;

			str_name.Format("OTR.OUT.MON.dSLOT%002d.STATUS", id);
			STD__ADD_DIGITAL_WITH_X_OPTION(str_name, dsc_item_list,"");
			LINK__VAR_DIGITAL_CTRL(dCH__MON_SLOT_STATUS[i], str_name);

			str_name.Format("OTR.OUT.MON.sSLOT%002d.TITLE", id);
			STD__ADD_STRING_WITH_X_OPTION(str_name, "");
			LINK__VAR_STRING_CTRL(sCH__MON_SLOT_TITLE[i], str_name);
		}

		// ...
		{
			str_name = "OTR.OUT.MON.dFOUP.CLAMP.STATUS";
			STD__ADD_DIGITAL(str_name, "UNKNOWN  CLAMP  UNCLAMP");
			LINK__VAR_DIGITAL_CTRL(dCH__MON_CLAMP_STS, str_name);

			str_name = "OTR.OUT.MON.dFOUP.EXIST.FLAG";
			STD__ADD_DIGITAL(str_name, "UNKNOWN  EXIST  NONE");
			LINK__VAR_DIGITAL_CTRL(dCH__MON_FOUP_STS, str_name);

			str_name = "OTR.OUT.MON.dFOUP.POS.STATUS";
			STD__ADD_DIGITAL(str_name, "UNKNOWN  LOAD  UNLOAD");
			LINK__VAR_DIGITAL_CTRL(dCH__MON_FOUP_POS, str_name);

			str_name = "OTR.OUT.MON.dDOOR.STATUS";
			STD__ADD_DIGITAL(str_name, "UNKNOWN  CLOSE  OPEN");
			LINK__VAR_DIGITAL_CTRL(dCH__MON_DOOR_STS, str_name);

			//
			str_name = "OTR.OUT.MON.sTRANSFER.PIO.STATUS";
			STD__ADD_DIGITAL(str_name, "NO  YES");
			LINK__VAR_DIGITAL_CTRL(dCH__MON_TRANSFER_PIO_STATUS, str_name);
		}
	}

	// LINK_PIO ...
	{
		str_name = "LINK_PIO.ACTIVE.RUN";
		STD__ADD_DIGITAL(str_name, "OFF ON");
		LINK__VAR_DIGITAL_CTRL(dCH__LINK_PIO_ACTIVE_RUN, str_name);

		//
		str_name = "LINK_PIO.ACTIVE.FA_AUTO";
		STD__ADD_DIGITAL(str_name, "OFF ON");
		LINK__VAR_DIGITAL_CTRL(dCH__LINK_PIO_ACTIVE_FA_AUTO, str_name);

		//
		str_name = "LINK_PIO.ACTIVE.LOAD_REQ";
		STD__ADD_DIGITAL(str_name, "OFF ON");
		LINK__VAR_DIGITAL_CTRL(dCH__LINK_PIO_ACTIVE_LOAD_REQ, str_name);

		str_name = "LINK_PIO.ACTIVE.UNLOAD_REQ";
		STD__ADD_DIGITAL(str_name, "OFF ON");
		LINK__VAR_DIGITAL_CTRL(dCH__LINK_PIO_ACTIVE_UNLOAD_REQ, str_name);
	}

	// SIM ...
	{
		str_name = "SIM.PAUSE.ACTIVE";
		STD__ADD_DIGITAL(str_name, "OFF ON");
		LINK__VAR_DIGITAL_CTRL(dCH__SIM_PAUSE_ACTIVE, str_name);
	}

	// CFG : LOAD & UNLOAF ...
	{
		str_name = "CFG.MAX.SLOT.COUNT";
		STD__ADD_DIGITAL(str_name, "25");
		LINK__VAR_DIGITAL_CTRL(dCH__CFG_MAX_SLOT_COUNT, str_name);

		//
		str_name = "CFG.MACRO_LOAD.USE";
		STD__ADD_DIGITAL(str_name, "YES NO");
		LINK__VAR_DIGITAL_CTRL(dCH__CFG_MACRO_LOAD_USE, str_name);

		str_name = "CFG.MACRO_UNLOAD.USE";
		STD__ADD_DIGITAL(str_name, "YES NO");
		LINK__VAR_DIGITAL_CTRL(dCH__CFG_MACRO_UNLOAD_USE, str_name);
	}

	// ...
	{
		str_name = "OTR.IN.CFG.FA.MODE";
		STD__ADD_STRING_WITH_X_OPTION(str_name,"");
		LINK__VAR_STRING_CTRL(sCH__CFG_FA_MODE,str_name);

		str_name = "OTR.IN.CFG.FA.SERVICE.MODE";
		STD__ADD_STRING_WITH_X_OPTION(str_name,"");
		LINK__VAR_STRING_CTRL(sCH__CFG_FA_SERVICE_MODE,str_name);

		//
		str_name = "OTR.IN.CFG.BYPASS.READ.ID.FLAG";
		STD__ADD_STRING_WITH_X_OPTION(str_name,"");
		LINK__VAR_STRING_CTRL(sCH__CFG_BYPASS_READ_ID_FLAG,str_name);

		str_name = "OTR.IN.CFG.RELOAD.FLAG";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__CFG_RELOAD_FLAG,str_name);
	}

	// ...
	{
		str_name = "CFG.RF.EXIST.FLAG";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, "YES NO","");
		LINK__VAR_DIGITAL_CTRL(dCH__CFG_RF_EXIST_FLAG,str_name);

		//
		str_name = "OTR.OUT.MON.FOUP.CID.STRING";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__FOUP_CID_STRING,str_name);

		str_name = "OTR.OUT.MON.FOUP.PAGE.STRING";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__FOUP_PAGE_STRING,str_name);
	}

	// ...
	{
		str_name = "MAP_SEQ.LOCK";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(sCH__MAP_SEQ_LOCK,str_name);
	}

	// CFG ...
	{
		str_name = "CFG.PREPLOAD.CID.MODE";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, "DISABLE ENABLE", "");
		LINK__VAR_DIGITAL_CTRL(dCH__CFG_PREPLOAD_CID_MODE, str_name);

		str_name = "CFG.LOAD.MAP.MODE";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, "ENABLE DISABLE", "");
		LINK__VAR_DIGITAL_CTRL(dCH__CFG_LOAD_MAP_MODE, str_name);

		str_name = "CFG.LOAD_ACT.AUTO_MAP.MODE";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, "DISABLE ENABLE", "");
		LINK__VAR_DIGITAL_CTRL(dCH__CFG_LOAD_ACT_AUTO_MAP_MODE, str_name);

		//
		str_name = "CFG.MAPPING.SENSOR.USE";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, "NO YES", "");
		LINK__VAR_DIGITAL_CTRL(dCH__CFG_MAPPING_SENSOR_USE, str_name);

		str_name = "CFG.MAPPING.SENSOR.TYPE";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, "LPx ROBOT", "");
		LINK__VAR_DIGITAL_CTRL(dCH__CFG_MAPPING_SENSOR_TYPE, str_name);

		//
		str_name = "CFG.BEFORE.LOAD.CST.CHECK";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, "TRUE FALSE", "");
		LINK__VAR_DIGITAL_CTRL(dCH__CFG_BEFORE_LOAD_CST_CHECK, str_name);

		str_name = "CFG.LOAD.DOOR.OPEN";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, "NO YES", "");
		LINK__VAR_DIGITAL_CTRL(dCH__CFG_LOAD_DOOR_OPEN, str_name);

		str_name = "CFG.LOAD.COMPLETE.DELAY.TIME";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "sec", 0, 0, 30, "");
		LINK__VAR_ANALOG_CTRL(aCH__CFG_LOAD_COMPT_DELAY_TIME, str_name);

		str_name = "CFG.DYNAMIC.USE";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, "NO YES", "");
		LINK__VAR_DIGITAL_CTRL(dCH__CFG_DYNAMIC_USE, str_name);

		//
		str_name = "CFG.UNLOAD.CLAMP.OPT";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, "AUTO  BUTTON", "");
		LINK__VAR_DIGITAL_CTRL(dCH__CFG_UNLD_CLAMP_OPT, str_name);

		//
		str_name = "CFG.DOOR.OPEN.COMPLETE.DELAY.TIME";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "sec", 1, 0.5, 5, "");
		LINK__VAR_ANALOG_CTRL(aCH__CFG_DOOR_OPEN_COMPLETE_DELAY_TIME, str_name);

		str_name = "CFG.WAFER.SLIDE.SNS.CHECK";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, "FALSE  TRUE", "");
		LINK__VAR_DIGITAL_CTRL(dCH__CFG_WAFER_SLIDE_SNS_CHECK, str_name);

		//
		str_name = "CFG.INIT.SLOT.STATE.UNKNOWN";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, "NO YES", "");
		LINK__VAR_DIGITAL_CTRL(dCH__CFG_INIT_SLOT_STATE_UNKNOWN, str_name);

		//
		str_name = "CFG.LOAD_POS.STATE.USE";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, "NO YES", "");
		LINK__VAR_DIGITAL_CTRL(dCH__CFG_LOAD_POS_STATE_USE, str_name);
	}

	// CFG.INTERLOCK ...
	{
		str_name = "CFG.INTERLOCK.ROBOT_AND_LPx_LOAD";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, "NO YES", "");
		LINK__VAR_DIGITAL_CTRL(dCH__CFG_INTERLOCK_ROBOT_AND_LPx_LOAD, str_name);
	}

	// CFG.CYCLE ...
	{
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
		p_variable->Add__MONITORING_PROC(1.0, MON_ID__IO_MONITOR);
	}
	return 1;
}


// ...
#define  ACT__RETRY_ABORT						\
l_act.RemoveAll();								\
l_act.Add("RETRY");								\
l_act.Add("ABORT");


int CObj__LPx_SYS::__DEFINE__ALARM(p_alarm)
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
		alarm_id = ALID__PUMP_PRESSURE__TIMEOUT;

		alarm_title  = title;
		alarm_title += "Pump Pressure TimeOut.";

		alarm_msg = "Please, check pump pressure config !\n";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}
	// ...
	{
		alarm_id = ALID__DOOR__NOT_OPEN;

		alarm_title  = title;
		alarm_title += "Door Status is not open !";

		alarm_msg = "Please, check loadport door status !\n";
	
		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}
	// ...
	{
		alarm_id = ALID__MAP_SEQ_LOCK__INTERLOCK;

		alarm_title  = title;
		alarm_title += "Currently, Load Port's status is \"Mapping_Sequence Lock\".";

		alarm_msg = "Please, check ATM Robot's status !\n";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}
	// ...
	{
		alarm_id = ALID__ROBOT_ARM__NOT_RETRACTED;

		alarm_title  = title;
		alarm_title += "Robot's arm is not retracted !\".";

		alarm_msg = "Please, check the RNE sensor of ATM Robot's arm ! \n";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__LOAD_FOUP_NONE_ALARM;

		alarm_title  = title;
		alarm_title += "LOAD... FOUP NONE ALARM.";

		alarm_msg  = "Before load action, foup dosen't exist on the load-port.\n";
		alarm_msg += "Please, Check if foup is properly placed !\n";

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
		alarm_id = ALID__MAP_ERROR__DOOR_NOT_OPEN;

		alarm_title  = title;
		alarm_title += "MAP ERROR (Door doesn't open) !";

		alarm_msg  = "The \"Map\" command cannot be executed, because the door of loadport is not open. \n";
		alarm_msg += "Please, open the door of loadport ! \n";

		l_act.RemoveAll();
		l_act.Add(ACT__RETRY);
		l_act.Add(ACT__ABORT);

		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}
	// ...
	{
		alarm_id = ALID__MAP_ERROR__FOUP_NOT_EXIST;

		alarm_title  = title;
		alarm_title += "MAP ERROR (Foup doesn't exist) !";

		alarm_msg  = "The \"Map\" command cannot be executed, because foup doesn't exist. \n";
		alarm_msg += "Please, check the state of foup ! \n";

		l_act.RemoveAll();
		l_act.Add(ACT__RETRY);
		l_act.Add(ACT__ABORT);

		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}
	// ...
	{
		alarm_id = ALID__MAP_ERROR__FOUP_POS_NOT_LOAD;

		alarm_title  = title;
		alarm_title += "MAP ERROR (Foup isn't loadded) !";

		alarm_msg  = "The \"Map\" command cannot be executed, because foup isn't loaded. \n";
		alarm_msg += "Please, check the state of foup ! \n";

		l_act.RemoveAll();
		l_act.Add(ACT__RETRY);
		l_act.Add(ACT__ABORT);

		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__EFEM_ROBOT__STATE_ERROR;

		alarm_title  = title;
		alarm_title += "EFEM Robot의 상태를 확인 바랍니다 !";

		alarm_msg  = "Robot이 비정상 종료된 상태입니다. \n";
		alarm_msg += "\"HOME\" 수행 후 동작을 수행하시기 바랍니다. \n";
		alarm_msg += "Please, check the state of EFEM-Robot ! \n";

		l_act.RemoveAll();
		l_act.Add(ACT__ABORT);

		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	return 1;
}

//-------------------------------------------------------------------------
int CObj__LPx_SYS::__INITIALIZE__OBJECT(p_variable,p_ext_obj_create)
{
	// ...
	{
		CString file_name;
		CString log_msg;

		file_name.Format("%s_App.log", sObject_Name);

		log_msg  = "\n\n";
		log_msg += "//------------------------------------------------------------------------";

		xLOG_CTRL->CREATE__SUB_DIRECTORY(sObject_Name);
		xLOG_CTRL->SET__PROPERTY(file_name,24*5,60);

		xLOG_CTRL->DISABLE__TIME_LOG();
		xLOG_CTRL->WRITE__LOG(log_msg);

		xLOG_CTRL->ENABLE__TIME_LOG();
		xLOG_CTRL->WRITE__LOG("   START   \n");
	}

	// ...
	CString def_name;
	CString def_data;
	CString ch_name;
	CString obj_name;
	CString var_name;
	int i;


	// CH_LINK : OBJ_STATE ...
	{
		def_name = "CH__OBJ_STATE";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, ch_name);
		p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(ch_name, obj_name,var_name);
		LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__OBJ_STATE, obj_name,var_name);
	}

	// LPx.ID ...
	{
		def_name = "LPx.ID";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);

		iLPx_ID = atoi(def_data);
	}

	// OBJ : LPx ...
	{
		def_name = "OBJ.LPx";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, obj_name);

		pLPx__OBJ_CTRL = p_ext_obj_create->Create__OBJECT_CTRL(obj_name);

		// ...
		{
			var_name = "MON.DOOR.STATUS";
			LINK__EXT_VAR_STRING_CTRL(sEXT_CH__LPx__DOOR_STS, obj_name,var_name);

			var_name = "MON.FOUP.POS.STATUS";
			LINK__EXT_VAR_STRING_CTRL(sEXT_CH__LPx__FOUP_POS, obj_name,var_name);

			var_name = "MON.FOUP.STATUS";
			LINK__EXT_VAR_STRING_CTRL(sEXT_CH__LPx__FOUP_STS, obj_name,var_name);

			var_name = "MON.CLAMP.STATUS";
			LINK__EXT_VAR_STRING_CTRL(sEXT_CH__LPx__CLAMP_STS, obj_name,var_name);
		}

		for(i=0; i<CFG_LPx__SLOT_MAX; i++)
		{
			var_name.Format("RES.MAP.SLOT%002d", i+1);
			LINK__EXT_VAR_STRING_CTRL(sEXT_CH__LPx__RES_MAP_SLOT_X[i], obj_name,var_name);
		}
	}

	// OBJ : RFx ...
	{
		def_name = "OBJ.RFx";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, obj_name);

		bool def_check = MACRO__Check_Def_Data(obj_name);
		bActive__RFx_OBJ = def_check;

		if(def_check)
		{
			pRFx__OBJ_CTRL = p_ext_obj_create->Create__OBJECT_CTRL(obj_name);

			// ...
			{
				var_name = "PARA.PAGE.ID";
				LINK__EXT_VAR_ANALOG_CTRL(aEXT_CH__RFx__PARA_PAGE_ID, obj_name,var_name);

				var_name = "PARA.CID.DATA";
				LINK__EXT_VAR_STRING_CTRL(sEXT_CH__RFx__PARA_CID_DATA, obj_name,var_name);

				var_name = "PARA.PAGE.DATA";
				LINK__EXT_VAR_STRING_CTRL(sEXT_CH__RFx__PARA_PAGE_DATA, obj_name,var_name);

				//
				var_name = "RSP.CID.DATA";
				LINK__EXT_VAR_STRING_CTRL(sEXT_CH__RFx__RSP_CID_DATA, obj_name,var_name);

				var_name = "RSP.PAGE.DATA";
				LINK__EXT_VAR_STRING_CTRL(sEXT_CH__RFx__RSP_PAGE_DATA, obj_name,var_name);
			}
		}
	}

	// OBJ : OHT ...
	{
		def_name = "OBJ.OHT";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, obj_name);

		bool def_check = MACRO__Check_Def_Data(obj_name);
		bActive__LPx_OHT = def_check;

		if(def_check)
		{
			var_name = "REPORT_PIO.E84_RUN.STATE";
			LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__REPORT_PIO_E84_RUN_STATE, obj_name,var_name);
		}
	}

	// ARM_RNE.SENSOR ...
	{
		def_name = "CH.ARM_RNE_SNS.DI";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, ch_name);

		bool def_check = MACRO__Check_Def_Data(ch_name);
		bActive__DI_ROBOT_ARM_RNE_SNS = def_check;

		if(def_check)
		{
			p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(ch_name, obj_name,var_name);
			LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__ROBOT_ARM_RNE_SNS, obj_name,var_name);

			//
			def_name = "DATA.RNE_ON";
			p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);
			sDATA__RNE_ON = def_data;

			def_name = "DATA.RNE_OFF";
			p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);
			sDATA__RNE_OFF = def_data;
		}
	}

	// LINK_IO ...
	{
		def_name = "CH.SLIDE_OUT.DI";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, ch_name);

		bool def_check = MACRO__Check_Def_Data(ch_name);
		bActive__DI_SLIDE_OUT_SNS = def_check;

		if(def_check)
		{
			p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(ch_name, obj_name,var_name);
			LINK__EXT_VAR_DIGITAL_CTRL(diEXT_CH__LPx_WAFER_SLID_OUT, obj_name,var_name);
		}
	}

	// OBJ.ROBOT ...
	{
		def_name = "OBJ.ROBOT";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, obj_name);

		bool def_check = MACRO__Check_Def_Data(obj_name);
		bActive__ROBOT_OBJ_CTRL = def_check;
		
		if(def_check)
		{
			pROBOT__OBJ_CTRL = p_ext_obj_create->Create__OBJECT_CTRL(obj_name);

			// ...
			{
				var_name = "MON.ACTIVE.ROBOT_ACTION_BUSY";
				LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__ROBOT__MON_ACTIVE_ROBOT_ACTION_BUSY, obj_name,var_name);

				//
				var_name.Format("MON.ACTIVE.INTERLOCK_BY_LP.%1d", iLPx_ID);
				LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__ROBOT__MON_ACTIVE_INTERLOCK_BY_LP, obj_name,var_name);

				var_name.Format("MON.ACTIVE.WAIT_BY_LP.%1d", iLPx_ID);
				LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__ROBOT__MON_ACTIVE_WAIT_BY_LP, obj_name,var_name);

				//
				var_name.Format("MON.ACTIVE.ROBOT_ACTION_TO_LP.%1d", iLPx_ID);
				LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__ROBOT__MON_ACTIVE_ACTION_TO_LP, obj_name,var_name);
			}
		}
	}

	// ...
	{
		SCX__SEQ_INFO x_seq_info;

		iActive__SIM_MODE = x_seq_info->Is__SIMULATION_MODE();
	}

	if(iActive__SIM_MODE > 0)
	{
		if(bActive__DI_ROBOT_ARM_RNE_SNS)		dEXT_CH__ROBOT_ARM_RNE_SNS->Set__DATA(sDATA__RNE_ON);
		if(bActive__DI_SLIDE_OUT_SNS)			diEXT_CH__LPx_WAFER_SLID_OUT->Set__DATA(STR__OFF);
	}

	return 1;
}

//-------------------------------------------------------------------------
int CObj__LPx_SYS::__CALL__CONTROL_MODE(mode,p_debug,p_variable,p_alarm)
{
LOOP_RETRY:

	// ...
	{
		CString log_msg;

		log_msg.Format("Start ... :  [%s]", mode);
		xLOG_CTRL->WRITE__LOG(log_msg);

		sCH__OBJ_MSG->Set__DATA(log_msg);
	}

	if(sCH__MAP_SEQ_LOCK->Check__DATA("YES") > 0)
	{
		if(mode.CompareNoCase(sMODE__CHECK_DOOR_OPEN) != 0)
		{
			CString log_msg;

			log_msg.Format("Current MAP_SEQ Lock occurred !");
			xLOG_CTRL->WRITE__LOG(log_msg);

			// ...
			{
				int alarm_id = ALID__MAP_SEQ_LOCK__INTERLOCK;
				CString r_act;

				p_alarm->Popup__ALARM(alarm_id,r_act);
	
				if(r_act.CompareNoCase(ACT__RETRY) == 0)
				{
					goto LOOP_RETRY;
				}
			}
			return -1;
		}
	}

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
	int flag = 1;

	// ...
	{
		dCH__LINK_PIO_ACTIVE_RUN->Set__DATA(STR__OFF);
		dCH__LINK_PIO_ACTIVE_LOAD_REQ->Set__DATA(STR__OFF);
		dCH__LINK_PIO_ACTIVE_UNLOAD_REQ->Set__DATA(STR__OFF);
	}

	// LPx.INTERLOCK CHECK ...
	if(dCH__CFG_INTERLOCK_ROBOT_AND_LPx_LOAD->Check__DATA(STR__YES) > 0)
	{
		bool active__robot_check = false;

		if((mode.CompareNoCase(sMODE__INIT) == 0)
		|| (mode.CompareNoCase(sMODE__HOME) == 0)
		|| (mode.CompareNoCase(sMODE__DOOR_OPEN) == 0)
		|| (mode.CompareNoCase(sMODE__LOAD) == 0)
		|| (mode.CompareNoCase(sMODE__RLSUNLOAD) == 0)
		|| (mode.CompareNoCase(sMODE__CYCLE) == 0)
		|| (mode.CompareNoCase(sMODE__MAP) == 0)
		|| (mode.CompareNoCase(sMODE__EXTENDPADDLE) == 0)
		|| (mode.CompareNoCase(sMODE__PADDLEDOWN) == 0)
		|| (mode.CompareNoCase(sMODE__PADDLEUP) == 0)
		|| (mode.CompareNoCase(sMODE__RETRACTPADDLE) == 0)
		|| (mode.CompareNoCase(sMODE__CLOSEDOOR) == 0)
		|| (mode.CompareNoCase(sMODE__OPENDOOR) == 0)
		|| (mode.CompareNoCase(sMODE__OPENDOOR_WITH_NO_UNLATCH) == 0))
		{
			active__robot_check = true;
		}

		if(active__robot_check)
		{
			dEXT_CH__ROBOT__MON_ACTIVE_INTERLOCK_BY_LP->Set__DATA(STR__ON);
			dEXT_CH__ROBOT__MON_ACTIVE_WAIT_BY_LP->Set__DATA(STR__OFF);

			// ...
			int count__robot_check = 0;

			while(1)
			{
				if(dEXT_CH__ROBOT__MON_ACTIVE_ROBOT_ACTION_BUSY->Check__DATA(STR__OFF) > 0)
				{
					break;
				}

				if(pROBOT__OBJ_CTRL->Is__OBJ_BUSY() < 0)
				{
					count__robot_check++;
						
					if(count__robot_check > 10)
					{
						int alm_id = ALID__EFEM_ROBOT__STATE_ERROR;
						CString alm_msg;
						CString r_act;

						alm_msg.Format(" * %s <- %s \n",
										dEXT_CH__ROBOT__MON_ACTIVE_ROBOT_ACTION_BUSY->Get__CHANNEL_NAME(),
										dEXT_CH__ROBOT__MON_ACTIVE_ROBOT_ACTION_BUSY->Get__STRING());

						p_alarm->Popup__ALARM_With_MESSAGE(alm_id, alm_msg, r_act);

						flag = -1001;
						break;
					}
				}

				dEXT_CH__ROBOT__MON_ACTIVE_WAIT_BY_LP->Set__DATA(STR__ON);

				if(p_variable->Check__CTRL_ABORT() > 0)
				{
					flag = -1011;
					break;
				}

				Sleep(90);
			}
		}
	}
	else
	{
		dEXT_CH__ROBOT__MON_ACTIVE_INTERLOCK_BY_LP->Set__DATA(STR__OFF);
		dEXT_CH__ROBOT__MON_ACTIVE_WAIT_BY_LP->Set__DATA(STR__OFF);
	}

	if(flag > 0)
	{
		if(iActive__SIM_MODE > 0)
		{
			while(dCH__SIM_PAUSE_ACTIVE->Check__DATA(STR__ON) > 0)
			{
				Sleep(90);
			}
		}

			 IF__CTRL_MODE(sMODE__INIT)					flag = Call__INIT(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__HOME)					flag = Call__HOME(p_variable,p_alarm);

		ELSE_IF__CTRL_MODE(sMODE__ALARM_RESET)			flag = Call__ALARM_RESET(p_variable,p_alarm);

		ELSE_IF__CTRL_MODE(sMODE__DOOR_OPEN)			flag = Call__DOOR_OPEN(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__CHECK_DOOR_OPEN)		flag = Call__CHECK_DOOR_OPEN(p_variable,p_alarm);

		ELSE_IF__CTRL_MODE(sMODE__PREPLOAD)
		{
			dCH__LINK_PIO_ACTIVE_LOAD_REQ->Set__DATA(STR__ON);
			dCH__LINK_PIO_ACTIVE_RUN->Set__DATA(STR__ON);

			flag = Call__PREPLOAD(p_variable,p_alarm);
		}
		ELSE_IF__CTRL_MODE(sMODE__LOAD)
		{
			flag = Call__LOAD(p_variable,p_alarm);
		}
		ELSE_IF__CTRL_MODE(sMODE__RLSUNLOAD)
		{
			flag = Call__RLSUNLOAD(p_variable,p_alarm);
		}
		ELSE_IF__CTRL_MODE(sMODE__UNLOAD)
		{
			dCH__LINK_PIO_ACTIVE_UNLOAD_REQ->Set__DATA(STR__ON);
			dCH__LINK_PIO_ACTIVE_RUN->Set__DATA(STR__ON);

			flag = Call__UNLOAD(p_variable,p_alarm);
		}

		ELSE_IF__CTRL_MODE(sMODE__CYCLE)				flag = Call__CYCLE(p_variable,p_alarm);

		ELSE_IF__CTRL_MODE(sMODE__CID_READ)				flag = Call__CID_READ(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__PAGE_READ)			flag = Call__PAGE_READ(p_variable,p_alarm);

		ELSE_IF__CTRL_MODE(sMODE__CID_WRITE)			flag = Call__CID_WRITE(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__PAGE_WRITE)			flag = Call__PAGE_WRITE(p_variable,p_alarm);

		ELSE_IF__CTRL_MODE(sMODE__MAP)					flag = Call__MAP(p_variable,p_alarm);

		ELSE_IF__CTRL_MODE(sMODE__CLAMP)				flag = Call__CLAMP(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__UNCLAMP)				flag = Call__UNCLAMP(p_variable,p_alarm);

		ELSE_IF__CTRL_MODE(sMODE__SHUTTLE_IN)			flag = Call__SHUTTLE_IN(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__SHUTTLE_OUT)			flag = Call__SHUTTLE_OUT(p_variable,p_alarm);

		ELSE_IF__CTRL_MODE(sMODE__EXTENDPADDLE)			flag = Call__EXTENDPADDLE(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__PADDLEDOWN)			flag = Call__PADDLEDOWN(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__PADDLEUP)				flag = Call__PADDLEUP(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__RETRACTPADDLE)		flag = Call__RETRACTPADDLE(p_variable,p_alarm);

		ELSE_IF__CTRL_MODE(sMODE__LATCH)				flag = Call__LATCH(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__UNLATCH)				flag = Call__UNLATCH(p_variable,p_alarm);

		ELSE_IF__CTRL_MODE(sMODE__CLOSEDOOR)					flag = Call__CLOSEDOOR(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__OPENDOOR)						flag = Call__OPENDOOR(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__OPENDOOR_WITH_NO_UNLATCH)     flag = Call__OPENDOOR_WITH_NO_UNLATCH(p_variable,p_alarm);

		ELSE_IF__CTRL_MODE(sMODE__VACON)				flag = Call__VACON(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__VACOFF)				flag = Call__VACOFF(p_variable,p_alarm);

		ELSE_IF__CTRL_MODE(sMODE__EXTENDSHUTTLE)		flag = Call__EXTENDSHUTTLE(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__RETRACTSHUTTLE)		flag = Call__RETRACTSHUTTLE(p_variable,p_alarm);
	}

	// ...
	{
		dCH__LINK_PIO_ACTIVE_RUN->Set__DATA(STR__OFF);
		dCH__LINK_PIO_ACTIVE_LOAD_REQ->Set__DATA(STR__OFF);
		dCH__LINK_PIO_ACTIVE_UNLOAD_REQ->Set__DATA(STR__OFF);

		//
		dEXT_CH__ROBOT__MON_ACTIVE_INTERLOCK_BY_LP->Set__DATA(STR__OFF);
		dEXT_CH__ROBOT__MON_ACTIVE_WAIT_BY_LP->Set__DATA(STR__OFF);
	}

	if((flag < 0)||(p_variable->Check__CTRL_ABORT() > 0))
	{
		CString log_msg;

		log_msg.Format("Aborted (%1d) ... :  [%s]", flag, mode);
		xLOG_CTRL->WRITE__LOG(log_msg);

		sCH__OBJ_MSG->Set__DATA(log_msg);
	}
	else
	{
		CString log_msg;

		log_msg.Format("Completed ... :  [%s]", mode);
		xLOG_CTRL->WRITE__LOG(log_msg);

		sCH__OBJ_MSG->Set__DATA(log_msg);
	}

	return flag;
}

int CObj__LPx_SYS::__CALL__MONITORING(id,p_variable,p_alarm)
{
	switch(id)
	{
		case MON_ID__IO_MONITOR:
			Mon__IO_MONITOR(p_variable,p_alarm);
			break;
	}

	return 1;
}
