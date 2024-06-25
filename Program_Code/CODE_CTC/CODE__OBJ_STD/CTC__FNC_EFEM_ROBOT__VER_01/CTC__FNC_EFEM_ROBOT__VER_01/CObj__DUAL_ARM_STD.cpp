#include "StdAfx.h"
#include "CObj__DUAL_ARM_STD.h"


// ...
CObj__DUAL_ARM_STD::CObj__DUAL_ARM_STD()
{
	iLPx_SIZE = -1;
}
CObj__DUAL_ARM_STD::~CObj__DUAL_ARM_STD()
{

}

// ...
int CObj__DUAL_ARM_STD::__DEFINE__CONTROL_MODE(obj,l_mode)
{
	// ...
	{
		sObject_Name = obj;
	}

	// ...
	{
		int i_limit = CFG_LLx_LIMIT;
		int i;

		for(i=0; i<i_limit; i++)
		{
			pLLx__OBJ_CTRL[i] = NULL;
		}
	}

	// ...
	{
		ADD__CTRL_VAR(sMODE__MAINT_PICK,      "MAINT_PICK");
		ADD__CTRL_VAR(sMODE__MAINT_PLACE,     "MAINT_PLACE");

		ADD__CTRL_VAR(sMODE__CLEAR,           "CLEAR");

		ADD__CTRL_VAR(sMODE__CHECK_MATERIAL,  "CHECK_MATERIAL");
		ADD__CTRL_VAR(sMODE__CHECK_STANDBY,   "CHECK_STANDBY");

		ADD__CTRL_VAR(sMODE__WAFER_RESYNC_CLEAR_SYS, "WAFER_RESYNC.CLEAR_SYS");

		ADD__CTRL_VAR(sMODE__SCH_DB_CHECK,    "SCH_DB.CHECK");
	}
	return 1;
}

int CObj__DUAL_ARM_STD::__DEFINE__VERSION_HISTORY(version)
{
	version += "JG.Lee \n";

	return 1;
}


// ...
#define  MON_ID__AUTO_MODE							1
#define  MON_ID__LBx_CFG							2

#define  MON_ID__DB_CHECK							11

// ...
#define  APP_DSP__DISABLE_ENABLE					\
"DISABLE  ENABLE"

#define  APP_DSP__ENABLE_DISABLE					\
"ENABLE  DISABLE"

#define APP_DSP__LLx_FRONT_MODE						\
"ENABLE  DISABLE  ENABLE.LPx  ENABLE.ALx"

#define APP_DSP__FRONT_LPx_MODULE					\
"LPx  LP1  LP2  LP3"

#define APP_DSP__SCH_ARM_MODE						\
"SINGLE  DUAL"


int CObj__DUAL_ARM_STD::__DEFINE__VARIABLE_STD(p_variable)
{
	DECLARE__STD_VARIABLE;

	CString str_name;
	CString ch_list;
	int i;

	// ...
	{
		str_name = "SCH.LOOP.ID";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__SCH_LOOP_ID, str_name);

		str_name = "CFG.QUERY.LOG.MODE";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, "DISABLE ENABLE", "");
		LINK__VAR_DIGITAL_CTRL(dCH__CFG_QUERY_LOG_MODE, str_name);
	}

	// PARA.MANUAL_MOVE ...
	{
		str_name = "PARA.MANUAL_MOVE.ALIGN_TARGET.PMx";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__PARA_MANUAL_MOVE_ALIGN_TARGET_PMx, str_name);

		str_name = "PARA.MANUAL_MOVE.ALIGN.SKIP";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__PARA_MANUAL_MOVE_ALIGN_SKIP, str_name);
	}

	// LPx
	for(i=0;i<CFG_LP_LIMIT;i++)
	{
		str_name.Format("LP%1d.MAP.LOCK", i+1);
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__LPx_MAP_LOCK[i],str_name);
	}

	// ...
	{
		str_name = "ATM_RB.PAUSE.FLAG";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__ATM_RB_PAUSE_FLAG,str_name);

		str_name = "DB_CHECK.RES.FLAG";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__DB_CHECK_REQ_FLAG,str_name);

		//
		str_name = "CFG.SCH.LLx_PREPMATER.USE";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, APP_DSP__DISABLE_ENABLE,"");
		LINK__VAR_DIGITAL_CTRL(xCH_CFG__SCH_LLx_PREPMATER_USE, str_name);

		str_name = "CFG.SCH.LLx_COMPMATER.USE";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, APP_DSP__DISABLE_ENABLE,"");
		LINK__VAR_DIGITAL_CTRL(xCH_CFG__SCH_LLx_COMPMATER_USE, str_name);

		//
		str_name = "CFG.SCH.ARM.MODE";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, APP_DSP__SCH_ARM_MODE,"");
		LINK__VAR_DIGITAL_CTRL(xCH_CFG__SCH_ARM_MODE, str_name);
	}

	// LLx ...
	{
		int ll_limit = CFG_LLx_LIMIT;
		int ll_i;

		for(ll_i=0; ll_i<ll_limit; ll_i++)
		{
			CString ll_name = Get__LLx_NAME(ll_i);

			//
			str_name.Format("%s.AVAILABLE_SLOT.COUNT", ll_name);
			STD__ADD_ANALOG_WITH_COMMENT(str_name, "num", 0,0.0,10.0, "");
			LINK__VAR_ANALOG_CTRL(xCH__LLx_AVAILABLE_SLOT_COUNT[ll_i], str_name);

			//
			str_name.Format("CFG.PHY.%s.PUMP_TIME", ll_name);
			STD__ADD_ANALOG_WITH_COMMENT(str_name, "sec", 1,0.0,60, "");
			LINK__VAR_ANALOG_CTRL(xCH_CFG__PHY_LLx_PUMP_TIME[ll_i], str_name);

			str_name.Format("CFG.PHY.%s.VENT_TIME", ll_name);
			STD__ADD_ANALOG_WITH_COMMENT(str_name, "sec", 1,0.0,60, "");
			LINK__VAR_ANALOG_CTRL(xCH_CFG__PHY_LLx_VENT_TIME[ll_i], str_name);
		}
	}

	// SCH.TEST ...
	{
		str_name = "SCH_TEST.ONLY_PLACE_TO_AL";
		STD__ADD_DIGITAL(str_name, "NO YES");
		LINK__VAR_DIGITAL_CTRL(dCH__SCH_TEST_ONLY_PLACE_TO_AL, str_name);
	}

	// CFG ...
	{
		str_name = "CFG.PRE_ALIGN.USE";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, "NO YES", "");
		LINK__VAR_DIGITAL_CTRL(dCH__CFG_PRE_ALIGN_USE, str_name);

		//
		str_name = "CFG.PHY.FRONT_TIME";
		STD__ADD_ANALOG_WITH_COMMENT(str_name,"sec",1,0.0,60,"");
		LINK__VAR_ANALOG_CTRL(xCH_CFG__PHY_FRONT_TIME,str_name);

		str_name = "CFG.PHY.PICK_TIME";
		STD__ADD_ANALOG_WITH_COMMENT(str_name,"sec",1,0.0,60,"");
		LINK__VAR_ANALOG_CTRL(xCH_CFG__PHY_PICK_TIME,str_name);

		str_name = "CFG.PHY.PLACE_TIME";
		STD__ADD_ANALOG_WITH_COMMENT(str_name,"sec",1,0.0,60,"");
		LINK__VAR_ANALOG_CTRL(xCH_CFG__PHY_PLACE_TIME,str_name);
	
		//
		str_name = "CFG.PHY.ALIGN_TIME";
		STD__ADD_ANALOG_WITH_COMMENT(str_name,"sec",1,0.0,60,"");
		LINK__VAR_ANALOG_CTRL(xCH_CFG__PHY_ALIGN_TIME,str_name);

		//
		str_name = "EXCHANGE_LOCK.FLAG";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__ATM_RB__EXCHANGE_LOCK_FLAG,str_name);

		str_name = "EXCHANGE_LOCK.LBx";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__ATM_RB__EXCHANGE_LOCK_LBx,str_name);

		str_name = "EXCHANGE_LOCK.SLOT";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__ATM_RB__EXCHANGE_LOCK_SLOT,str_name);

		//
		str_name = "CFG.ALx_SWAP.FLAG";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,APP_DSP__DISABLE_ENABLE,"");
		LINK__VAR_DIGITAL_CTRL(xCH_CFG__ALx_SWAP_FLAG,str_name);

		str_name = "CFG.LBx_SWAP.FLAG";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,APP_DSP__DISABLE_ENABLE,"");
		LINK__VAR_DIGITAL_CTRL(xCH_CFG__LBx_SWAP_FLAG,str_name);
	}

	// CFG ...
	{
		str_name = "CFG.PLACE_FRONT_TO_LLx_DOOR";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, APP_DSP__LLx_FRONT_MODE, "");
		LINK__VAR_DIGITAL_CTRL(dCH__CFG_PLACE_FRONT_TO_LLx_DOOR, str_name);

		str_name = "CFG.PICK_FRONT_TO_LLx_DOOR";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, APP_DSP__LLx_FRONT_MODE, "");
		LINK__VAR_DIGITAL_CTRL(dCH__CFG_PICK_FRONT_TO_LLx_DOOR, str_name);

		//
		str_name = "CFG.MOVE_TO_LLx.DELAY.SEC";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "sec", 1, 0, 5, "");
		LINK__VAR_ANALOG_CTRL(aCH__CFG_MOVE_TO_LLx_DELAY_SEC, str_name);

		str_name = "CFG.FRONT_LPx_ID";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, APP_DSP__FRONT_LPx_MODULE, "");
		LINK__VAR_DIGITAL_CTRL(dCH__CFG_FRONT_LPx_ID, str_name);

		str_name = "CFG.FRONT_LPx_SLOT";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "slot", 0, 1, 25, "");
		LINK__VAR_ANALOG_CTRL(aCH__CFG_FRONT_LPx_SLOT, str_name);
	}

	// INFO ...
	{
		str_name = "INFO.FUME_CLEAR.STATE.FROM_TMC";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__INFO_FUME_CLEAR_STATE_FROM_TMC, str_name);

		str_name = "INFO.FUME_CLEAR.POS.FROM_TMC";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__INFO_FUME_CLEAR_POS_FROM_TMC, str_name);

		str_name = "INFO.FUME_CLEAR.DELAY.COUNT.FROM_TMC";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__INFO_FUME_CLEAR_DELAY_COUNT_FROM_TMC, str_name);
	}

	// ...
	{
		str_name = "CHECK.MODULE";
		STD__ADD_STRING_WITH_COMMENT(str_name, "");
		LINK__VAR_STRING_CTRL(xCH__CHECK_MODULE, str_name);

		str_name = "CFG.BUFFER.USE_DURING_MAINT_MODE";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, APP_DSP__ENABLE_DISABLE, "");
		LINK__VAR_DIGITAL_CTRL(dCH__CFG_BUFFER_USE_DURING_MAINT_MODE, str_name);

		str_name = "CFG.WAFER_TO_STx_DURING_MAINT_MODE";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, "ALL  ONLY.PROCESSED", "");
		LINK__VAR_DIGITAL_CTRL(dCH__CFG_WAFER_TO_STx_DURING_MAINT_MODE, str_name);

		str_name = "CFG.BUFFER.LOTID.CHECK";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, APP_DSP__ENABLE_DISABLE, "");
		LINK__VAR_DIGITAL_CTRL(dCH__CFG_BUFFER_LOTID_CHECK, str_name);
	}

	// ...
	{
		str_name = "CFG.BUFFER.TRANSFER.MODE";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"Before_Process After_Process","");
		LINK__VAR_DIGITAL_CTRL(xCH__CFG_DB_STx_TRANSFER_MODE, str_name);

		str_name = "SCH_DB.CFG.BUFFER.TRANSFER.MODE";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"After_Process","");
		LINK__VAR_DIGITAL_CTRL(xCH__SCH_DB_STx_TRANSFER_MODE, str_name);

		//
		str_name = "CFG.BUFFER.PRE.ALIGN.MODE";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"ENABLE DISABLE","");
		LINK__VAR_DIGITAL_CTRL(xCH__CFG_BUFFER_PRE_ALIGN_MODE,str_name);

		//
		str_name = "SCH_DB.ST1.USE.FLAG";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__SCH_DB_ST1_USE_FLAG, str_name);

		str_name = "SCH_DB.ST2.USE.FLAG";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__SCH_DB_ST2_USE_FLAG, str_name);

		//
		str_name = "CFG.BUFFER.WAFER.CLEAN.TIME";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",0,0,1000,"");
		LINK__VAR_ANALOG_CTRL(xCH__CFG_DB_BUFFER_WAFER_CLEAN_TIME,str_name);

		str_name = "SCH_DB.ST1.WAFER.CLEAN.TIME";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",0,0,1000,"");
		LINK__VAR_ANALOG_CTRL(xCH__SCH_DB_ST1_WAFER_CLEAN_TIME,str_name);

		str_name = "SCH_DB.ST2.WAFER.CLEAN.TIME";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",0,0,1000,"");
		LINK__VAR_ANALOG_CTRL(xCH__SCH_DB_ST2_WAFER_CLEAN_TIME,str_name);

		//
		str_name = "CFG.BUFFER.WAFER.PICK.MODE";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"BOTTOM_TO_UP UP_TO_BOTTOM","");
		LINK__VAR_DIGITAL_CTRL(xCH__CFG_DB_STx_WAFER_PICK_MODE,str_name);

		str_name = "SCH_DB.CFG.ST1.WAFER.PICK.MODE";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"BOTTOM_TO_UP UP_TO_BOTTOM","");
		LINK__VAR_DIGITAL_CTRL(xCH__SCH_DB_ST1_WAFER_PICK_MODE,str_name);

		str_name = "SCH_DB.CFG.ST2.WAFER.PICK.MODE";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"BOTTOM_TO_UP UP_TO_BOTTOM","");
		LINK__VAR_DIGITAL_CTRL(xCH__SCH_DB_ST2_WAFER_PICK_MODE,str_name);

		//
		str_name = "CFG.BUFFER.APPLY.MODE";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"DISABLE ENABLE ROUTE", "");
		LINK__VAR_DIGITAL_CTRL(xCH__CFG_DB_STx_APPLY_MODE,str_name);

		str_name = "SCH_DB.STx.CFG.APPLY.MODE";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"DISABLE ENABLE ROUTE", "");
		LINK__VAR_DIGITAL_CTRL(xCH__SCH_DB_STx_APPLY_MODE,str_name);

		//
		ch_list  = STR__TIME;
		ch_list += " ";
		ch_list += STR__LPx_EMPTY;
		ch_list += " ";
		ch_list += STR__LPx_TIME;
		ch_list += " ";
		ch_list += STR__LPx_OPTIMIZATION;

		str_name = "CFG.BUFFER.OUT_MODE.BUFFER_TO_LPo";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, ch_list, "");
		LINK__VAR_DIGITAL_CTRL(dCH__CFG_DB_STx_OUT_MODE_BUFFER_TO_LPo, str_name);

		str_name = "SCH_DB.ST1.OUT_MODE.BUFFER_TO_LPo";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, ch_list, "");
		LINK__VAR_DIGITAL_CTRL(dCH__SCH_DB_ST1_OUT_MODE_BUFFER_TO_LPo, str_name);

		str_name = "SCH_DB.ST2.OUT_MODE.BUFFER_TO_LPo";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, ch_list, "");
		LINK__VAR_DIGITAL_CTRL(dCH__SCH_DB_ST2_OUT_MODE_BUFFER_TO_LPo, str_name);
	}

	// ...
	{
		p_variable->Add__MONITORING_PROC(1.0,MON_ID__AUTO_MODE);
		p_variable->Add__MONITORING_PROC(1.0,MON_ID__LBx_CFG);

		// p_variable->Add__MONITORING_PROC(5.0,MON_ID__DB_CHECK);
	}
	return 1;
}

int CObj__DUAL_ARM_STD::__DEFINE__ALARM(p_alarm)
{
	DECLARE__ALARM;

	// ...
	CString title;
	title.Format("%s - ",sObject_Name);

	int alarm_id;
	CString alarm_title;
	CString alarm_msg;
	CStringArray l_act;

	// ...
	{
		alarm_title  = title;
		alarm_title += "OBJECT UNKNOWN MODE";

		alarm_msg = "";

		l_act.RemoveAll();
		l_act.Add("ABORT");

		ADD__ALARM_EX(ALID__OBJECT_MODE_UNKNOWN,alarm_title,alarm_msg,l_act);
	}

	// ...
	alarm_title  = title;
	alarm_title += "JOB ERROR";

	// ...
	{
		alarm_msg = "ATM-Robot status is not standby !";

		l_act.RemoveAll();
		l_act.Add("CHECK");

		ADD__ALARM_EX(ALID__JOB_ERROR__ATM_RB__NOT_STANDBY,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_msg = "AL1 status is not standby !";

		l_act.RemoveAll();
		l_act.Add("CHECK");

		ADD__ALARM_EX(ALID__JOB_ERROR__AL1__NOT_STANDBY,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_msg = "LBA status is not standby !";

		l_act.RemoveAll();
		l_act.Add("CHECK");

		ADD__ALARM_EX(ALID__JOB_ERROR__LBA__NOT_STANDBY,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_msg = "LBB status is not standby !";

		l_act.RemoveAll();
		l_act.Add("CHECK");

		ADD__ALARM_EX(ALID__JOB_ERROR__LBB__NOT_STANDBY,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_title  = title;
		alarm_title += "JOB ERROR";

		alarm_msg = "Please, check LBA's atm pressure.";

		l_act.RemoveAll();
		l_act.Add("CHECK");

		ADD__ALARM_EX(ALID__JOB_ERROR__LBA__CHECK_ATM_PRESSURE,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_title  = title;
		alarm_title += "JOB ERROR";

		alarm_msg = "Please, check LBB's atm pressure.";

		l_act.RemoveAll();
		l_act.Add("CHECK");

		ADD__ALARM_EX(ALID__JOB_ERROR__LBB__CHECK_ATM_PRESSURE,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_title  = title;
		alarm_title += "JOB ERROR";

		alarm_msg  = "Please, check wafer title on the arm of atm robot.\n";
		alarm_msg += "I recommend that you will abort all job.\n";

		l_act.RemoveAll();
		l_act.Add("CHECK");

		ADD__ALARM_EX(ALID__JOB_ERROR__WAFER_INFO_MISMATCH,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_title  = title;
		alarm_title += "Module Move - Module Status Error";

		alarm_msg = "";

		l_act.RemoveAll();
		l_act.Add("CHECK");

		ADD__ALARM_EX(ALID__MANUAL_MOVE__MODULE_STATUS_ERROR,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_title  = title;
		alarm_title += "Tool Clear - Module Status Error";

		alarm_msg = "";

		l_act.RemoveAll();
		l_act.Add("CHECK");

		ADD__ALARM_EX(ALID__TOOL_CLEAR__MODULE_STATUS_ERROR,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__MOVE_ERROR__WAFER_CONFIRM;

		alarm_title  = title;
		alarm_title += "Wafer Moving Error !";

		alarm_msg = "Wafer 위치를 확인 바랍니다. \n";

		l_act.RemoveAll();
		l_act.Add(STR__YES);
		l_act.Add(STR__NO);
		l_act.Add(STR__IGNORE);

		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__USER_CHECK__WAFER_CONFIRM;

		alarm_title  = title;
		alarm_title += "GUI상의 Wafer 정보와 설비내의 실제 Wafer 정보가 일치하는지 확인바랍니다.";

		alarm_msg = "";

		l_act.RemoveAll();
		l_act.Add(STR__CHECK);

		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__PICK_ERROR__WAFER_MITMATCH;

		alarm_title  = title;
		alarm_title += "Pick 명령 처리 후 Wafer 정보에 문제가 발생 했습니다.";

		alarm_msg = "";

		l_act.RemoveAll();
		l_act.Add(STR__ABORT);
		l_act.Add(STR__RETRY);

		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__PLACE_ERROR__WAFER_MITMATCH;

		alarm_title  = title;
		alarm_title += "Place 명령 처리 후 Wafer 정보에 문제가 발생 했습니다.";

		alarm_msg = "";

		l_act.RemoveAll();
		l_act.Add(STR__ABORT);
		l_act.Add(STR__RETRY);

		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_title  = title;
		alarm_title += "LBx Configuration Error";

		alarm_msg = "";

		l_act.RemoveAll();
		l_act.Add("CHECK");

		ADD__ALARM_EX(ALID__LBx_CONFIG_ERROR,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__BUFFERx_CONFIG_ERROR;

		alarm_title  = title;
		alarm_title += "BUFFERx Configuration Error";

		alarm_msg = "";

		l_act.RemoveAll();
		l_act.Add("CHECK");

		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_title  = title;
		alarm_title += "ATM-ROBOT - SWAP ERROR !";

		alarm_msg = "Job will be aborted !\n";

		l_act.RemoveAll();
		l_act.Add("ABORT");

		ADD__ALARM_EX(ALID__SWAP_ERROR,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_title  = title;
		alarm_title += "ATM-Robot - Manual Move Error !";

		alarm_msg = "";

		l_act.RemoveAll();
		l_act.Add("CHECK");

		ADD__ALARM_EX(ALID__MANUAL_MOVE__ACTION_ERROR,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_title  = title;
		alarm_title += "ATM-Robot - Tool Clear Error !";

		alarm_msg = "";

		l_act.RemoveAll();
		l_act.Add("CHECK");

		ADD__ALARM_EX(ALID__TOOL_CLEAR__ACTION_ERROR,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_title  = title;
		alarm_title += "LLx Status is not available !";

		alarm_msg = "";

		l_act.RemoveAll();
		l_act.Add("CHECK");

		ADD__ALARM_EX(ALID__LLx_STATUS__IS_NOT_AVAILABLE,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_title  = title;
		alarm_title += "LPx Status is not available !";

		alarm_msg = "";

		l_act.RemoveAll();
		l_act.Add("CHECK");

		ADD__ALARM_EX(ALID__LPx_STATUS__IS_NOT_AVAILABLE,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_title  = title;
		alarm_title += "LPx material-pick error occurred in job running !";

		alarm_msg = "";

		l_act.RemoveAll();
		l_act.Add("CHECK");

		ADD__ALARM_EX(ALID__LPx_MATERIAL_PICK__JOB_ERROR,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_title  = title;
		alarm_title += "LPx material-place error occurred in job running !";

		alarm_msg = "";

		l_act.RemoveAll();
		l_act.Add("CHECK");

		ADD__ALARM_EX(ALID__LPx_MATERIAL_PLACE__JOB_ERROR,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_title  = title;
		alarm_title += "LPx's mapping request is rejected !";

		alarm_msg = "";

		l_act.RemoveAll();
		l_act.Add("CHECK");

		ADD__ALARM_EX(ALID__LPx__MAP_ABORTED,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_title  = title;
		alarm_title += "AL's L/L target name error !";

		alarm_msg = "";

		l_act.RemoveAll();
		l_act.Add("CHECK");

		ADD__ALARM_EX(ALID__ALx_LLx_TARGET_NAME_ERROR,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_title  = title;
		alarm_title += "ATM Robot's moving is paused by user.";

		alarm_msg = "Please, check the \"ATM Robot - PAUSE\" button on the operate page. \n";

		l_act.RemoveAll();
		l_act.Add("CHECK");

		ADD__ALARM_EX(ALID__ATM_ROBOT__MOVE_PAUSED,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__MAINT_PICK_ERROR__ATM_RB_PAUSED;

		alarm_title  = title;
		alarm_title += "Maint Pick Error : ATM Robot Paused !";

		alarm_msg = "Please, check \"ATM Robot - PAUSE\"'s button on CTC operate page. \n";

		l_act.RemoveAll();
		l_act.Add(STR__ABORT);
		l_act.Add(STR__RETRY);

		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__MAINT_PLACE_ERROR__ATM_RB_PAUSED;

		alarm_title  = title;
		alarm_title += "Maint Place Error : ATM Robot Paused !";

		alarm_msg = "Please, check \"ATM Robot - PAUSE\"'s button on CTC operate page. \n";

		l_act.RemoveAll();
		l_act.Add(STR__ABORT);
		l_act.Add(STR__RETRY);

		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	return 1;
}

int CObj__DUAL_ARM_STD::__Define__USER_FUNCTION(CII_DEFINE__FUNCTION *p_fnc_ctrl)
{
	mERROR__USER_FNC.Set__OBJ_NAME(sObject_Name);
	p_fnc_ctrl->Link__ERROR_FEEDBACK(&mERROR__USER_FNC);

	// ...
	{

	}
	return 1;
}

//-------------------------------------------------------------------------
int CObj__DUAL_ARM_STD::__INITIALIZE__OBJECT(p_variable,p_ext_obj_create)
{
	// mMacro_Log.Init_LOG();

	p_ext_obj_create->Link__DEF_VARIABLE__ERROR_FEEDBACK(&mERROR__DEF_VAR);
	iJOB_FLAG = -1;

	// ...
	CString def_name;
	CString def_data;
	CString db_name;
	CString str_name;
	int i;
	int j;

	// ...
	{
		CString file_name;
		CString log_msg;

		file_name.Format("%s.log", sObject_Name);

		log_msg  = "\n\n";
		log_msg += "//------------------------------------------------------------------------";

		xAPP_LOG_CTRL->CREATE__SUB_DIRECTORY(sObject_Name);
		xAPP_LOG_CTRL->SET__PROPERTY(file_name,24*5,60);

		xAPP_LOG_CTRL->DISABLE__TIME_LOG();
		xAPP_LOG_CTRL->WRITE__LOG(log_msg);

		xAPP_LOG_CTRL->ENABLE__TIME_LOG();
		xAPP_LOG_CTRL->WRITE__LOG("   START   \n");
	}

	// DB_CFG ...
	{
		CString db_cfg_name;

		def_name = "DB_CFG_NAME";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name,db_cfg_name);

		// CFG_PMx_USE ...
		for(i=0; i<CFG_PM_LIMIT; i++)
		{
			int pm_id = i + 1;

			str_name.Format("PM%1d.USE", pm_id);
			LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__CFG_PMx_USE[i], db_cfg_name,str_name);

			str_name.Format("PM%1d.PROCESS.TYPE", pm_id);
			LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__PMx_PROCESS_TYPE[i], db_cfg_name,str_name);
		}

		// TRANSFER
		{
			str_name = "TRANSFER.MODE";
			LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__CFG__TRANSFER_MODE, db_cfg_name,str_name);

			str_name = "REF.ATM.PRESSURE";
			LINK__EXT_VAR_ANALOG_CTRL(xEXT_CH__CFG__REF_ATM_PRESSURE, db_cfg_name,str_name);

			str_name = "REF.VAC.PRESSURE";
			LINK__EXT_VAR_ANALOG_CTRL(xEXT_CH__CFG__REF_VAC_PRESSURE, db_cfg_name,str_name);
		}

		// ...
		{
			str_name = "VAC_AREA.MATERIAL.COUNT";
			LINK__EXT_VAR_STRING_CTRL(xEXT_CH__VAC_AREA_MATERIAL_COUNT, db_cfg_name,str_name);

			str_name = "VAC_ROBOT.OUT_MATERIAL.COUNT";
			LINK__EXT_VAR_STRING_CTRL(xEXT_CH__VAC_ROBOT_OUT_MATERIAL_COUNT, db_cfg_name,str_name);

			str_name = "PMC.MATERIAL.COUNT";
			LINK__EXT_VAR_STRING_CTRL(xEXT_CH__PMC_MATERIAL_COUNT, db_cfg_name,str_name);

			//
			str_name = "SYSTEM.LEAK_CHECK.REQ";
			LINK__EXT_VAR_STRING_CTRL(xEXT_CH__SYSTEM_LEAK_CHECK_REQ, db_cfg_name,str_name);
		}

		// SCH PARAMETER
		{
			str_name = "SCH.MAP.DEVICE";
			LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__SCH_MAP_DEVICE, db_cfg_name,str_name);

			str_name = "SCH.ALIGN.DEVICE";
			LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__SCH_ALIGN_DEVICE, db_cfg_name,str_name);

			for(i=0;i<CFG_LP_LIMIT;i++)
			{
				str_name.Format("LP%1d.MAP.REQ",i+1);
				LINK__EXT_VAR_STRING_CTRL(xEXT_CH__LPx_MAP_REQ[i], db_cfg_name,str_name);
			}
		}

		// CFG_DB : AL1 ...
		{
			str_name = "CFG.AL1.USE.FLAG";
			LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__CFG_DB_AL1_USE_FLAG, db_cfg_name,str_name);

			str_name = "CFG.AL1.SLOT01.USE.FLAG";
			LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__CFG_DB_AL1_SLOT01_USE_FLAG, db_cfg_name,str_name);

			str_name = "CFG.AL1.SLOT02.USE.FLAG";
			LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__CFG_DB_AL1_SLOT02_USE_FLAG, db_cfg_name,str_name);
		}
		// SCH_DB : AL1 ...
		{
			str_name = "SCH_DB.CFG.AL1.USE.FLAG";
			LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__SCH_DB_AL1_USE_FLAG, db_cfg_name,str_name);

			str_name = "SCH_DB.CFG.AL1.SLOT01.USE.FLAG";
			LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__SCH_DB_AL1_SLOT01_USE_FLAG, db_cfg_name,str_name);

			str_name = "SCH_DB.CFG.AL1.SLOT02.USE.FLAG";
			LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__SCH_DB_AL1_SLOT02_USE_FLAG, db_cfg_name,str_name);
		}

		// SCH_DB ...
		{
			for(i=0; i<CFG_LLx_LIMIT; i++)
			{
				iLLx_SLOT_MAX[i] = CFG_LLx__SLOT_MAXx;
			}

			// SCH.MODE.XXX ...
			{
				str_name = "SCH.MODE.LLx_IN";
				LINK__EXT_VAR_STRING_CTRL(sEXT_CH__SCH_MODE_LLx_IN, db_cfg_name,str_name);

				str_name = "SCH.MODE.LLx_OUT";
				LINK__EXT_VAR_STRING_CTRL(sEXT_CH__SCH_MODE_LLx_OUT, db_cfg_name,str_name);
			}

			// LLx ...
			{
				int ll_limit = CFG_LLx_LIMIT;
				int ll_i;

				for(ll_i=0; ll_i<ll_limit; ll_i++)
				{
					CString ll_name = Get__LLx_NAME(ll_i);

					//
					str_name.Format("%s.CFG.PREPMATER.USE", ll_name);
					LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__LLx__CFG_PREPMATER_USE[ll_i], db_cfg_name,str_name);

					str_name.Format("%s.CFG.COMPMATER.USE", ll_name);
					LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__LLx__CFG_COMPMATER_USE[ll_i], db_cfg_name,str_name);

					str_name.Format("%s.MODE_TYPE", ll_name);
					LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__LLx__CFG_LLx_MODE_TYPE[ll_i], db_cfg_name,str_name);

					//
					str_name.Format("SCH_DB.%s.USE.FLAG", ll_name);
					LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__SCH_DB_LLx_USE_FLAG[ll_i], db_cfg_name,str_name);

					str_name.Format("SCH_DB.%s.SLOT_MAX", ll_name);
					LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__SCH_DB_LLx_SLOT_MAX[ll_i], db_cfg_name,str_name);

					str_name.Format("SCH_DB.%s.IDLE.STATUS", ll_name);
					LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__SCH_DB_LLx_IDLE_STATUS[ll_i], db_cfg_name,str_name);

					str_name.Format("SCH_DB.%s.MODE_TYPE", ll_name);
					LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_i], db_cfg_name,str_name);

					for(i=0; i<CFG_LLx__SLOT_MAXx; i++)
					{
						str_name.Format("%s.SLOT%1d_STATUS", ll_name,i+1);
						LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__CFG_DB_LLx_SLOT_STATUS[ll_i][i], db_cfg_name,str_name);

						str_name.Format("SCH_DB.%s.SLOT%1d_STATUS", ll_name,i+1);
						LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__SCH_DB_LLx_SLOT_STATUS[ll_i][i], db_cfg_name,str_name);

						//
						str_name.Format("SCH_DB.%s.SLOT%1d_MODE", ll_name,i+1);
						LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__SCH_DB_LLx_SLOT_MODE[ll_i][i], db_cfg_name,str_name);
					}
				}
			}
		}

		//
		str_name = "PRESSURE_CHECK.MODE";
		LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__CFG__PRESSURE_CHECK_MODE, db_cfg_name,str_name);

		for(i=0; i<CFG_LLx_LIMIT; i++)
		{
			str_name.Format("SCH.STS.ATM_RB_TO_LL%1d", i+1);
			LINK__EXT_VAR_STRING_CTRL(xCH__ATM_RB__SCH_STS_TO_LLx[i], db_cfg_name,str_name);
		}
	}

	// JOB CTRL ...
	{
		def_name = "OBJ__JOB_CTRL";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name,db_name);

		str_name = "JOB.CTRL";
		LINK__EXT_VAR_STRING_CTRL(xEXT_CH__JOB_CTRL, db_name,str_name);
	}

	// OBJ : ATM_ROBOT ...
	{
		def_name = "OBJ__ATM_ROBOT";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name,db_name);

		//
		pATM_RB__OBJ_CTRL = p_ext_obj_create->Create__OBJECT_CTRL(db_name);

		LINK__EXT_VAR_DIGITAL_CTRL(xCH__ATM_RB__OBJ_CTRL,   db_name, "CTRL");
		LINK__EXT_VAR_DIGITAL_CTRL(xCH__ATM_RB__OBJ_MODE,   db_name, "MODE");
		LINK__EXT_VAR_DIGITAL_CTRL(xCH__ATM_RB__OBJ_STATUS, db_name, "OBJ.STATUS");

		LINK__EXT_VAR_DIGITAL_CTRL(xCH__ATM_RB__SLOT01_STATUS, db_name, "ARM.SLOT01.STATUS");
		LINK__EXT_VAR_STRING_CTRL( xCH__ATM_RB__SLOT01_TITLE,  db_name, "ARM.SLOT01.TITLE");

		LINK__EXT_VAR_DIGITAL_CTRL(xCH__ATM_RB__SLOT02_STATUS, db_name, "ARM.SLOT02.STATUS");
		LINK__EXT_VAR_STRING_CTRL( xCH__ATM_RB__SLOT02_TITLE,  db_name, "ARM.SLOT02.TITLE");

		LINK__EXT_VAR_DIGITAL_CTRL(xCH__ATM_RB__ANI_TRG_ROTE, db_name, "ANI.TRG.ROTE");
		LINK__EXT_VAR_DIGITAL_CTRL(xCH__ATM_RB__ANI_TRG_MOVE, db_name, "ANI.TRG.MOVE");

		//
		str_name = "CFG.ALIGN.RETRY";
		LINK__EXT_VAR_ANALOG_CTRL(aCH__ATM_RB__CFG_ALIGN_RETRY, db_name,str_name);

		str_name = "CUR.ALIGN.RETRY";
		LINK__EXT_VAR_STRING_CTRL(sCH__ATM_RB__CUR_ALIGN_RETRY, db_name,str_name);

		//
		LINK__EXT_VAR_DIGITAL_CTRL(xCH__ATM_RB__TARGET_LLx_MODE, db_name, "TARGET.LLx.MODE");
		LINK__EXT_VAR_DIGITAL_CTRL(xCH__ATM_RB__TARGET_LLx_SLOT_CHECK, db_name, "TARGET.LLx.SLOT.CHECK");

		LINK__EXT_VAR_STRING_CTRL(xCH__ATM_RB__TARGET_LLx_NAME_SET_OF_ARM_A, db_name, "TARGET.LLx.NAME.SET_OF_ARM_A");
		LINK__EXT_VAR_STRING_CTRL(xCH__ATM_RB__TARGET_LLx_SLOT_SET_OF_ARM_A, db_name, "TARGET.LLx.SLOT.SET_OF_ARM_A");

		LINK__EXT_VAR_STRING_CTRL(xCH__ATM_RB__TARGET_LLx_NAME_SET_OF_ARM_B, db_name, "TARGET.LLx.NAME.SET_OF_ARM_B");
		LINK__EXT_VAR_STRING_CTRL(xCH__ATM_RB__TARGET_LLx_SLOT_SET_OF_ARM_B, db_name, "TARGET.LLx.SLOT.SET_OF_ARM_B");

		LINK__EXT_VAR_STRING_CTRL(xCH__ATM_RB__TARGET_LLx_NAME_SET_ALL, db_name, "TARGET.LLx.NAME.SET");
		LINK__EXT_VAR_STRING_CTRL(xCH__ATM_RB__TARGET_LLx_SLOT_SET_ALL, db_name, "TARGET.LLx.SLOT.SET");

		str_name = "TARGET.LLx.ANGLE.GET";
		LINK__EXT_VAR_STRING_CTRL(xCH__ATM_RB__TARGET_LLx_ANGLE_GET, db_name,str_name);

		//
		LINK__EXT_VAR_DIGITAL_CTRL(xCH__ATM_RB__TARGET_PMx_MODE, db_name, "TARGET.PMx.MODE");

		//
		LINK__EXT_VAR_DIGITAL_CTRL(xCH__ATM_RB__CFG_A_ARM_USE_MODE, db_name,"CFG.A.ARM.USE.FLAG");
		LINK__EXT_VAR_DIGITAL_CTRL(xCH__ATM_RB__CFG_B_ARM_USE_MODE, db_name,"CFG.B.ARM.USE.FLAG");

		// LLx : Scheduler - Dual Only Input & Output ...
		str_name = "CFG.LLx_CTRL.ONLY_INPUT_OUTPUT.MODE";
		LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__CFG_LLx_CTRL_ONLY_INPUT_OUTPUT_MODE, db_name,str_name);

		//
		str_name = "CFG.DUAL_ARM_MOVING_AT_THE_SAME_TIME";
		LINK__EXT_VAR_DIGITAL_CTRL(dCH__ATM_RB__CFG_DUAL_ARM_MOVING_AT_THE_SAME_TIME, db_name,str_name);

		str_name = "CFG.DUAL_ARM_MOVING_AT_LPx";
		LINK__EXT_VAR_DIGITAL_CTRL(dCH__ATM_RB__CFG_DUAL_ARM_MOVING_AT_LPx, db_name,str_name);

		str_name = "CFG.DUAL_ARM_MOVING_AT_STx";
		LINK__EXT_VAR_DIGITAL_CTRL(dCH__ATM_RB__CFG_DUAL_ARM_MOVING_AT_STx, db_name,str_name);

		str_name = "CFG.DUAL_ARM_MOVING_AT_LLx";
		LINK__EXT_VAR_DIGITAL_CTRL(dCH__ATM_RB__CFG_DUAL_ARM_MOVING_AT_LLx, db_name,str_name);

		// LLx : CONTRAINT ...
		str_name = "CFG.LL.CONSTRAINT.1";
		LINK__EXT_VAR_DIGITAL_CTRL(dCH__ATM_RB__CFG_LL_CONSTRAINT_1, db_name,str_name);

		str_name = "CFG.LL.CONSTRAINT.2";
		LINK__EXT_VAR_DIGITAL_CTRL(dCH__ATM_RB__CFG_LL_CONSTRAINT_2, db_name,str_name);

		// CFG : WAFER PICK PARAMETER ...
		str_name = "CFG.PICK_WAFER_CONDITION";
		LINK__EXT_VAR_DIGITAL_CTRL(dCH__ATM_RB__CFG_PICK_WAFER_CONDITION, db_name,str_name);

		// INFO.LLx ...
		for(i=0; i<CFG_LLx_LIMIT; i++)
		{
			int id = i + 1;

			str_name.Format("INFO.LL%1d.RESERVE", id);
			LINK__EXT_VAR_DIGITAL_CTRL(dCH__ATM_RB__INFO_LL_RESERVE_X[i], db_name,str_name);
		}
	}

	// OBJ : AL ...
	{
		def_name = "OBJ__AL";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name,db_name);

		//
		pAL1__OBJ_CTRL = p_ext_obj_create->Create__OBJECT_CTRL(db_name);

		LINK__EXT_VAR_DIGITAL_CTRL(xCH__AL1__OBJ_CTRL,   db_name, "CTRL");
		LINK__EXT_VAR_DIGITAL_CTRL(xCH__AL1__OBJ_MODE,   db_name, "MODE");
		LINK__EXT_VAR_DIGITAL_CTRL(xCH__AL1__OBJ_STATUS, db_name, "OBJ.STATUS");

		LINK__EXT_VAR_DIGITAL_CTRL(xCH__AL1__SLOT01_STATUS, db_name, "SLOT01.STATUS");
		LINK__EXT_VAR_STRING_CTRL( xCH__AL1__SLOT01_TITLE,  db_name, "SLOT01.TITLE");

		LINK__EXT_VAR_DIGITAL_CTRL(xCH__AL1__SLOT02_STATUS, db_name, "SLOT02.STATUS");
		LINK__EXT_VAR_STRING_CTRL( xCH__AL1__SLOT02_TITLE,  db_name, "SLOT02.TITLE");

		LINK__EXT_VAR_STRING_CTRL(xCH__AL1__ALIGN_RETRY_FLAG, db_name,"ALIGN_RETRY.FLAG");
	}

	// OBJ : LLx ...
	{
		int ll_limit;
		int ll_i;

		def_name = "LLx_SIZE";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);
		ll_limit = atoi(def_data);

		if(ll_limit > CFG_LLx_LIMIT)		ll_limit = CFG_LLx_LIMIT;
		iLLx_SIZE = ll_limit;

		for(ll_i=0; ll_i<ll_limit; ll_i++)
		{
			def_name.Format("OBJ__LL%1d", ll_i+1);
			p_ext_obj_create->Get__DEF_CONST_DATA(def_name, db_name);

			//
			pLLx__OBJ_CTRL[ll_i] = p_ext_obj_create->Create__OBJECT_CTRL(db_name);

			LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__LLx__OBJ_CTRL[ll_i],   db_name,"CTRL");
			LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__LLx__OBJ_MODE[ll_i],   db_name,"MODE");
			LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__LLx__OBJ_STATUS[ll_i], db_name,"OBJ.STATUS");

			//
			LINK__EXT_VAR_STRING_CTRL(sEXT_CH__LLx__UPPER_OBJ_MSG[ll_i], db_name,"UPPER_OBJ.MSG");

			//
			LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__LLx__CFG_USE_FLAG[ll_i], db_name,"CFG.USE.FLAG");

			//
			str_name = "MOVE.FLAG";
			LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__LLx__MOVE_FLAG[ll_i], db_name,str_name);

			str_name = "ACTIVE.VAC_AREA.CTRL";
			LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__LLx__ACTIVE_VAC_AREA_CTRL[ll_i], db_name,str_name);

			str_name = "ACTIVE.ATM_AREA.CTRL";
			LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__LLx__ACTIVE_ATM_AREA_CTRL[ll_i], db_name,str_name);

			//
			LINK__EXT_VAR_STRING_CTRL(xEXT_CH__LLx__IN_OUT_FLAG[ll_i], db_name,"SCH.IN_OUT.FLAG");
			LINK__EXT_VAR_STRING_CTRL(xEXT_CH__LLx__IN_COUNT[ll_i],    db_name,"SCH.IN_COUNT");
			LINK__EXT_VAR_STRING_CTRL(xEXT_CH__LLx__OUT_COUNT[ll_i],   db_name,"SCH.OUT_COUNT");
			LINK__EXT_VAR_STRING_CTRL(xEXT_CH__LLx__IDLE_COUNT[ll_i],  db_name,"SCH.IDLE_COUNT");

			LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__LLx__PRESSURE_STATUS[ll_i], db_name,"PRESSURE.STATUS");
			LINK__EXT_VAR_ANALOG_CTRL(xEXT_CH__LLx__PRESSURE_VALUE[ll_i],   db_name,"PRESSURE.VALUE");

			for(i=0; i<CFG_LLx__SLOT_MAXx; i++)
			{
				str_name.Format("SLOT%002d.STATUS",i+1);
				LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__LLx__SLOT_STATUS[ll_i][i], db_name,str_name);

				str_name.Format("SLOT%002d.TITLE",i+1);
				LINK__EXT_VAR_STRING_CTRL(xEXT_CH__LLx__SLOT_TITLE[ll_i][i], db_name,str_name);
			}

			//
			LINK__EXT_VAR_STRING_CTRL(xEXT_CH__LLx__VENTING_FLAG[ll_i],      db_name,"VENTING.FLAG");
			LINK__EXT_VAR_STRING_CTRL(xEXT_CH__LLx__VENTING_COMP_FLAG[ll_i], db_name,"VENTING.COMP.FLAG");

			LINK__EXT_VAR_STRING_CTRL(xEXT_CH__LLx__PRE_MAINT_FLAG[ll_i],    db_name,"PRE_MAINT_FLAG");
		}
	}

	// OBJ : SIDE BUFFER (1) ...
	{
		def_name = "OBJ__SIDE_BUFFER1";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name,db_name);

		//
		str_name = "CFG.USE.FLAG";
		LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__BUFFER1__CFG_USE_FLAG, db_name,str_name);

		str_name = "CFG.SLOT.MAX";
		LINK__EXT_VAR_ANALOG_CTRL(xEXT_CH__BUFFER1__CFG_SLOT_MAX, db_name,str_name);

		str_name = "CFG.SLOT.WAIT.SEC";
		LINK__EXT_VAR_STRING_CTRL(xEXT_CH__BUFFER1__CFG_SLOT_WAIT_SEC, db_name,str_name);

		str_name = "APP.NEXT.SLOT.TO.PLACE";
		LINK__EXT_VAR_ANALOG_CTRL(xEXT_CH__BUFFER1__APP_NEXT_SLOT_TO_PLACE, db_name,str_name);

		str_name = "APP.NEXT.SLOT.TO.PICK";
		LINK__EXT_VAR_ANALOG_CTRL(xEXT_CH__BUFFER1__APP_NEXT_SLOT_TO_PICK, db_name,str_name);

		str_name = "APP.CUR.LOTID";
		LINK__EXT_VAR_STRING_CTRL(xEXT_CH__BUFFER1__APP_CUR_LOTID, db_name,str_name);

		for(i=0;i<CFG_LP__SLOT_MAX;i++)
		{
			str_name.Format("SLOT%002d.STATUS", i+1);
			LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__BUFFER1__SLOT_STATUS[i], db_name,str_name);

			str_name.Format("SLOT%002d.TITLE", i+1);
			LINK__EXT_VAR_STRING_CTRL(xEXT_CH__BUFFER1__SLOT_TITLE[i], db_name,str_name);

			str_name.Format("SLOT%002d.WAIT.SEC", i+1);
			LINK__EXT_VAR_ANALOG_CTRL(xEXT_CH__BUFFER1__SLOT_WAIT_SEC[i], db_name,str_name);
		}
	}
	// OBJ : SIDE BUFFER (2) ...
	{
		def_name = "OBJ__SIDE_BUFFER2";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name,db_name);

		//
		str_name = "CFG.USE.FLAG";
		LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__BUFFER2__CFG_USE_FLAG, db_name,str_name);

		str_name = "CFG.SLOT.MAX";
		LINK__EXT_VAR_ANALOG_CTRL(xEXT_CH__BUFFER2__CFG_SLOT_MAX, db_name,str_name);

		str_name = "CFG.SLOT.WAIT.SEC";
		LINK__EXT_VAR_STRING_CTRL(xEXT_CH__BUFFER2__CFG_SLOT_WAIT_SEC, db_name,str_name);

		str_name = "APP.NEXT.SLOT.TO.PLACE";
		LINK__EXT_VAR_ANALOG_CTRL(xEXT_CH__BUFFER2__APP_NEXT_SLOT_TO_PLACE, db_name,str_name);

		str_name = "APP.NEXT.SLOT.TO.PICK";
		LINK__EXT_VAR_ANALOG_CTRL(xEXT_CH__BUFFER2__APP_NEXT_SLOT_TO_PICK, db_name,str_name);

		str_name = "APP.CUR.LOTID";
		LINK__EXT_VAR_STRING_CTRL(xEXT_CH__BUFFER2__APP_CUR_LOTID, db_name,str_name);

		for(i=0;i<CFG_LP__SLOT_MAX;i++)
		{
			str_name.Format("SLOT%002d.STATUS", i+1);
			LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__BUFFER2__SLOT_STATUS[i], db_name,str_name);

			str_name.Format("SLOT%002d.TITLE", i+1);
			LINK__EXT_VAR_STRING_CTRL(xEXT_CH__BUFFER2__SLOT_TITLE[i], db_name,str_name);

			str_name.Format("SLOT%002d.WAIT.SEC", i+1);
			LINK__EXT_VAR_ANALOG_CTRL(xEXT_CH__BUFFER2__SLOT_WAIT_SEC[i], db_name,str_name);
		}
	}

	// OBJ : LP1 ~ LP5
	{
		def_name = "LP_SIZE";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name,db_name);

		int lp_size = atoi(db_name);
		if(lp_size > CFG_LP_LIMIT)		lp_size = CFG_LP_LIMIT;
		iLPx_SIZE = lp_size;

		for(i=0;i<lp_size;i++)
		{
			// 
			str_name.Format("OBJ__LP%1d",i+1);
			if(p_ext_obj_create->Get__DEF_CONST_DATA(str_name,db_name) < 0)
			{
				continue;
			}

			//
			LINK__EXT_VAR_DIGITAL_CTRL(xCH__LPx__OBJ_CTRL[i],   db_name, "CTRL");
			LINK__EXT_VAR_DIGITAL_CTRL(xCH__LPx__OBJ_MODE[i],   db_name, "MODE");
			LINK__EXT_VAR_DIGITAL_CTRL(xCH__LPx__OBJ_STATUS[i], db_name, "OBJ.STATUS");

			//
			LINK__EXT_VAR_DIGITAL_CTRL(xCH__LPx__CFG_USE_TYPE[i],  db_name, "CFG.USE");
			LINK__EXT_VAR_DIGITAL_CTRL(xCH__LPx__CFG_PROCESS_TYPE[i], db_name, "CFG.PROCESS.TYPE");

			LINK__EXT_VAR_STRING_CTRL(xCH__LPx__PORT_STATUS[i],    db_name, "APP.PORT.STATUS");
			LINK__EXT_VAR_DIGITAL_CTRL(xCH__LPx__CST_STATUS[i],    db_name, "CST.STATUS");
			LINK__EXT_VAR_ANALOG_CTRL(xCH__LPx__CFG_SLOT_MAX[i],   db_name, "CFG.SLOT.MAX");
			LINK__EXT_VAR_ANALOG_CTRL(xCH__LPx__LAST_PICK_SLOT[i], db_name, "APP.LAST.PICK.SLOT");

			//
			LINK__EXT_VAR_STRING_CTRL(xCH__LPx__PICK_COUNT[i],      db_name, "PICK_COUNT");
			LINK__EXT_VAR_STRING_CTRL(xCH__LPx__RETURN_REQ_FLAG[i], db_name, "LPx.RETURN.REQ.FLAG");			
			LINK__EXT_VAR_STRING_CTRL(xCH__LPx__SIDE_BUFFER_USE_FLAG[i], db_name, "SIDE_BUFFER.USE.FLAG");

			//
			for(j=0;j<CFG_LP__SLOT_MAX;j++)
			{
				str_name.Format("SLOT%002d.STATUS",j+1);
				LINK__EXT_VAR_DIGITAL_CTRL(xCH__LPx__SLOT_STATUS[i][j], db_name, str_name);
			}		
		}
	}

	// OBJ : PMx ...
	{
		def_name = "PM_SIZE";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, db_name);

		int pm_size = atoi(db_name);
		if(pm_size > CFG_PM_LIMIT)		pm_size = CFG_PM_LIMIT;
		iPMx_SIZE = pm_size;

		for(i=0; i<pm_size; i++)
		{
			int pm_id = i + 1;

			str_name.Format("OBJ__PM%1d", pm_id);
			if(p_ext_obj_create->Get__DEF_CONST_DATA(str_name, db_name) < 0)
			{
				continue;
			}

			//
			str_name = "OBJ.VIRTUAL.STATUS";
			LINK__EXT_VAR_STRING_CTRL(sCH__PMx__OBJ_VIRTUAL_STATUS[i], db_name, str_name);

			str_name = "PROC_INFO.PROCESS_STATUS";
			LINK__EXT_VAR_STRING_CTRL(sCH__PMx__PROC_INFO_PROCESS_STATUS[i], db_name, str_name);
		}
	}

	// ...
	{
		CMNG__PMx_ALIGN* p_mng = &mMNG__PMx_ALIGN;

		p_mng->pSCH_MATERIAL_CTRL = xSCH_MATERIAL_CTRL.Get__PTR();

		// PMx ...
		{
			p_mng->iPMx_SIZE = iPMx_SIZE;

			for(i=0; i<iPMx_SIZE; i++)
			{
				p_mng->pCH__CFG_PMx_USE[i] = dEXT_CH__CFG_PMx_USE[i].Get__PTR();
				p_mng->pCH__PMx_PROCESS_TYPE[i]= dEXT_CH__PMx_PROCESS_TYPE[i].Get__PTR();

				p_mng->pCH__PMx_OBJ_VIRTUAL_STATUS[i] = sCH__PMx__OBJ_VIRTUAL_STATUS[i].Get__PTR();
				p_mng->pCH__PMx_PROC_INFO_PROCESS_STATUS[i] = sCH__PMx__PROC_INFO_PROCESS_STATUS[i].Get__PTR();
			}
		}

		// LPx ...
		{
			p_mng->iLP_SIZE = iLPx_SIZE;
		
			for(i=0; i<iLPx_SIZE; i++)
			{
				p_mng->pCH__LPx_CFG_PROCESS_TYPE[i] = xCH__LPx__CFG_PROCESS_TYPE[i].Get__PTR();
			}
		}

		// ...
		{
			p_mng->pCH__CFG_TARGET_PMx_MODE = xCH__ATM_RB__TARGET_PMx_MODE.Get__PTR();

			p_mng->pCH__WAFER_TITLE_OF_AL = xCH__AL1__SLOT01_TITLE.Get__PTR();

			p_mng->pCH__TRG_NAME_OF_AL = xCH__ATM_RB__TARGET_LLx_NAME_SET_ALL.Get__PTR();
			p_mng->pCH__TRG_SLOT_OF_AL = xCH__ATM_RB__TARGET_LLx_SLOT_SET_ALL.Get__PTR();
			p_mng->pCH__RESULT_ALIGN_ANGLE = xCH__ATM_RB__TARGET_LLx_ANGLE_GET.Get__PTR();

			p_mng->pCH__WAFER_TITLE_OF_ARM_A = xCH__ATM_RB__SLOT01_TITLE.Get__PTR();
			p_mng->pCH__TRG_NAME_OF_ARM_A    = xCH__ATM_RB__TARGET_LLx_NAME_SET_OF_ARM_A.Get__PTR();

			p_mng->pCH__WAFER_TITLE_OF_ARM_B = xCH__ATM_RB__SLOT02_TITLE.Get__PTR();
			p_mng->pCH__TRG_NAME_OF_ARM_B    = xCH__ATM_RB__TARGET_LLx_NAME_SET_OF_ARM_B.Get__PTR();
		}
	}

	// ...
	{
		SCX__SEQ_INFO x_seq_info;

		iActive_SIM = x_seq_info->Is__SIMULATION_MODE();

		if(iActive_SIM > 0)
		{
			Call__WAFER_RESYNC_CLEAR_SYS();
		}
	}
	return 1;
}

//-------------------------------------------------------------------------
int CObj__DUAL_ARM_STD::__CALL__CONTROL_MODE(mode,p_debug,p_variable,p_alarm)
{
	DECLARE__EXT_CTRL(p_variable);

	// ...
	int flag = -1;

	// ...
	{
		CString para_arm;
		CString para_module;
		CString para_slot;

		CStringArray l_para;
		CString para_data;

		int limit;
		int i;

		p_ext_mode_ctrl->Get__FNC_PARAMETER(l_para);
		limit = l_para.GetSize();

		for(i=0;i<limit;i++)
		{
			para_data = l_para[i];

			switch(i)
			{
				case 0:		para_arm    = para_data;		break;
				case 1:		para_module = para_data;		break;
				case 2:		para_slot   = para_data;		break;
			}
		}

		// ...
		{
			CString log_msg;

			log_msg.Format("[%s] started ... (%s,%s,%s)", 
				           mode, para_arm,para_module,para_slot);

			xAPP_LOG_CTRL->WRITE__LOG(log_msg);
		}
	}

	try
	{
			 IF__CTRL_MODE(sMODE__MAINT_PICK)					flag = Call__MAINT_PICK(p_variable, p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__MAINT_PLACE)					flag = Call__MAINT_PLACE(p_variable, p_alarm);

		ELSE_IF__CTRL_MODE(sMODE__CLEAR)						flag = Call__CLEAR(p_variable, p_alarm);

		ELSE_IF__CTRL_MODE(sMODE__CHECK_MATERIAL)				flag = Call__CHECK_MATERIAL(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__CHECK_STANDBY)				flag = Call__CHECK_STANDBY(p_variable, p_alarm);

		ELSE_IF__CTRL_MODE(sMODE__WAFER_RESYNC_CLEAR_SYS)		flag = Call__WAFER_RESYNC_CLEAR_SYS();

		ELSE_IF__CTRL_MODE(sMODE__SCH_DB_CHECK)					flag = Call__SCH_DB_CHECK(p_variable, p_alarm);

		else
		{
			CString bff;
			CString alarm_msg;
			CString r_act;

			bff.Format("Object Name : %s\n",sObject_Name);
			alarm_msg  = bff;
			bff.Format("Unknown Object Mode : \"%s\"\n",mode);
			alarm_msg += bff;

			p_alarm->Popup__ALARM_With_MESSAGE(ALID__OBJECT_MODE_UNKNOWN,alarm_msg,r_act);
		}
	}
	catch(int err_flag)
	{

	}

	// ...
	{
		CString log_msg;

		log_msg.Format("[%s] : Result Flag [%1d]", mode,flag);

		xAPP_LOG_CTRL->WRITE__LOG(log_msg);
	}

	Set__TMC_CtcInUse_To_Standby();
	return flag;
}
int CObj__DUAL_ARM_STD::__CALL__MONITORING(id,p_variable,p_alarm)
{
	switch(id)
	{
		case MON_ID__AUTO_MODE:
			Mon__AUTO_CTRL(p_variable,p_alarm);
			break;

		case MON_ID__LBx_CFG:
			Mon__LBx_CFG(p_variable,p_alarm);
			break;

		case MON_ID__DB_CHECK:
			Mon__DB_CHECK(p_variable,p_alarm);
			break;
	}

	return 1;
}

