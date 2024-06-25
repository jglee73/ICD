#include "StdAfx.h"
#include "CObj_Opr__AUTO_MODE.h"


//-------------------------------------------------------------------------
CObj_Opr__AUTO_MODE::CObj_Opr__AUTO_MODE()
{

}
CObj_Opr__AUTO_MODE::~CObj_Opr__AUTO_MODE()
{

}

//-------------------------------------------------------------------------
int CObj_Opr__AUTO_MODE::__DEFINE__CONTROL_MODE(obj,l_mode)
{
	// ...
	{
		sObject_Name = obj;
		iMAINT_FLAG = -1;
	}

	// ...
	{
		ADD__CTRL_VAR(sMODE__MAINT, "MAINT");
	}
	return 1;
}
int CObj_Opr__AUTO_MODE::__DEFINE__VERSION_HISTORY(version)
{
	version = "JG.Lee \n";

	return 1;
}


// ...
#define MON_ID__ALL_STATE				1
#define MON_ID__PORT_CTRL				2

#define MON_ID__DB_CHECK				11


// ...
#define DSP__SCH_MODE					\
"SEQUENTIAL  PIPELINE  CONCURRENT"

#define DSP__SCH_REF					\
"CONFIG  ROUTE"

#define DSP__LPx_PICK_MODE				\
"BOTTOM_UP  TOP_DOWN"


int CObj_Opr__AUTO_MODE::__DEFINE__VARIABLE_STD(p_variable)
{
	DECLARE__STD_VARIABLE;

	CString str_name;
	int i;

	// STATUS
	str_name = "JOB.CTRL";
	STD__ADD_STRING_WITH_COMMENT(str_name,"");
	LINK__VAR_STRING_CTRL(xCH__JOB_CTRL,str_name);

	str_name = "PRE.JOB.CTRL";
	STD__ADD_STRING_WITH_COMMENT(str_name,"");
	LINK__VAR_STRING_CTRL(xCH_PRE__JOB_CTRL,str_name);

	//
	str_name = "JOB.STATUS";
	STD__ADD_STRING_WITH_COMMENT(str_name,"");
	LINK__VAR_STRING_CTRL(xCH__JOB_STATUS,str_name);

	str_name = "PRE.JOB.STATUS";
	STD__ADD_STRING_WITH_COMMENT(str_name,"");
	LINK__VAR_STRING_CTRL(xCH_PRE__JOB_STATUS,str_name);

	str_name = "JOB.READY.START.FLAG";
	STD__ADD_STRING_WITH_COMMENT(str_name,"");
	LINK__VAR_STRING_CTRL(xCH__JOB_READY_START_FLAG,str_name);

	//
	str_name = "RESYNC.EDIT.STATUS.CUR";
	STD__ADD_STRING_WITH_COMMENT(str_name,"");
	LINK__VAR_STRING_CTRL(xCH__RESYNC_EDIT_STATUS_CUR, str_name);

	str_name = "RESYNC.EDIT.STATUS.REQ";
	STD__ADD_STRING_WITH_COMMENT(str_name,"");
	LINK__VAR_STRING_CTRL(xCH__RESYNC_EDIT_STATUS_REQ, str_name);

	//
	str_name = "CFG.SCH.MODE";
	STD__ADD_DIGITAL_WITH_COMMENT(str_name,DSP__SCH_MODE,"");
	LINK__VAR_DIGITAL_CTRL(xCH__CFG_SCH_MODE,str_name);

	str_name = "CFG.SCH.REF";
	STD__ADD_DIGITAL_WITH_COMMENT(str_name,DSP__SCH_REF,"");
	LINK__VAR_DIGITAL_CTRL(xCH__CFG_SCH_REF,str_name);

	str_name = "SYS.STATUS";
	STD__ADD_STRING_WITH_COMMENT(str_name,"");
	LINK__VAR_STRING_CTRL(xCH__SYS_STATUS,str_name);

	//
	for(i=0;i<CFG_LP_LIMIT;i++)
	{
		str_name.Format("LP%1d.JOB_FILE.UPLOAD_FLAG",i+1);
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__PORT_UPLOAD_FILE_FLAG[i],str_name);

		str_name.Format("LP%1d.PJOBID",i+1);
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__PORT_PJOBID[i],str_name);
	}

	// LEAK_CHECK ...
	{
		str_name = "LEAK_CHECK.START.FLAG";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__LEAK_CHECK_START_FLAG,str_name);

		str_name = "LEAK_CHECK.IDLE.RUN.DATE";
		STD__ADD_STRING_WITH_X_OPTION(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__LEAK_CHECK__IDLE_RUN_DATE,str_name);

		str_name = "LEAK_CHECK.IDLE.PRE.DATE";
		STD__ADD_STRING_WITH_X_OPTION(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__LEAK_CHECK__IDLE_PRE_DATE,str_name);

		str_name = "LEAK_CHECK.IDLE.PRE.STATUS";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__LEAK_CHECK__IDLE_PRE_STATUS,str_name);
	}
	// AUTO_PM ...
	{
		str_name = "AUTO_PM.START.FLAG";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__AUTO_PM_START_FLAG,str_name);

		str_name = "AUTO_PM.IDLE.RUN.DATE";
		STD__ADD_STRING_WITH_X_OPTION(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__AUTO_PM__IDLE_RUN_DATE,str_name);

		str_name = "AUTO_PM.IDLE.PRE.DATE";
		STD__ADD_STRING_WITH_X_OPTION(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__AUTO_PM__IDLE_PRE_DATE,str_name);

		str_name = "AUTO_PM.IDLE.PRE.STATUS";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__AUTO_PM__IDLE_PRE_STATUS,str_name);
	}

	// CYCLE PARA
	str_name = "CYCLE.MODE";
	STD__ADD_DIGITAL_WITH_COMMENT(str_name,"DISABLE ENABLE FOREVER","");
	LINK__VAR_DIGITAL_CTRL(xCH_CFG__CYCLE_MODE,str_name);

	str_name = "CYCLE.LIMIT";
	STD__ADD_ANALOG_WITH_COMMENT(str_name,"num",0,0.0,100000,"");
	LINK__VAR_ANALOG_CTRL(xCH_CFG__CYCLE_LIMIT,str_name);

	str_name = "CYCLE.COUNT.PICK";
	STD__ADD_ANALOG_WITH_COMMENT(str_name,"num",0,0.0,100000,"");
	LINK__VAR_ANALOG_CTRL(xCH_CFG__CYCLE_COUNT_PICK,str_name);

	str_name = "CYCLE.COUNT.PLACE";
	STD__ADD_ANALOG_WITH_COMMENT(str_name,"num",0,0.0,100000,"");
	LINK__VAR_ANALOG_CTRL(xCH_CFG__CYCLE_COUNT_PLACE,str_name);

	str_name = "CYCLE.START_TIME";
	STD__ADD_STRING_WITH_COMMENT(str_name,"");
	LINK__VAR_STRING_CTRL(xCH_CFG__CYCLE_START_TIME,str_name);

	str_name = "CYCLE.ELAPSE_TIME";
	STD__ADD_STRING_WITH_COMMENT(str_name,"");
	LINK__VAR_STRING_CTRL(xCH_CFG__CYCLE_ELAPSE_TIME,str_name);
	
	str_name = "CYCLE.THROUGHPUT";
	STD__ADD_STRING_WITH_COMMENT(str_name,"");
	LINK__VAR_STRING_CTRL(xCH_CFG__CYCLE_THROUGHPUT,str_name);

	//
	str_name = "CYCLE.STEADY.ELAPSE_TIME";
	STD__ADD_STRING_WITH_COMMENT(str_name,"");
	LINK__VAR_STRING_CTRL(xCH_CFG__CYCLE_STEADY_ELAPSE_TIME,str_name);

	str_name = "CYCLE.STEADY.THROUGHPUT";
	STD__ADD_STRING_WITH_COMMENT(str_name,"");
	LINK__VAR_STRING_CTRL(xCH_CFG__CYCLE_STEADY_THROUGHPUT,str_name);

	str_name = "CYCLE.STEADY.COUNT.LIMIT";
	STD__ADD_ANALOG_WITH_X_OPTION(str_name,"num",0,0.0,100000,"");
	LINK__VAR_ANALOG_CTRL(xCH_CFG__CYCLE_STEADY_COUNT_LIMIT,str_name);

	//
	str_name = "SCH.TEST.PORT.CURRENT.APPLY";
	STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"DISABLE ENABLE","");
	LINK__VAR_DIGITAL_CTRL(xCH_CFG__SCH_TEST_PORT_CURRENT_APPLY,str_name);

	// jglee : 2021.11.11
	str_name = "CFG.LLx_STATE_CHECK_FOR_JOB_START";
	STD__ADD_DIGITAL_WITH_X_OPTION(str_name, "ENABLE DISABLE", "");
	LINK__VAR_DIGITAL_CTRL(dCH_CFG__LLx_STATE_CHECK_FOR_JOB_START, str_name);

	//
	str_name = "CFG.DUMMY_PORT.USE.MODE";
	STD__ADD_DIGITAL_WITH_X_OPTION(str_name, "DISABLE ENABLE", "");
	LINK__VAR_DIGITAL_CTRL(dCH_CFG__DUMMY_PORT_USE_MODE,str_name);

	//
	str_name = "CFG.DUMMY_PORT.CTRL.TYPE";
	STD__ADD_DIGITAL_WITH_X_OPTION(str_name, "IDLE JOB", "");
	LINK__VAR_DIGITAL_CTRL(dCH_CFG__DUMMY_PORT_CTRL_TYPE,str_name);

	//
	str_name = "CFG.DUMMY_PORT.WAFER_PER_PMC";
	STD__ADD_DIGITAL_WITH_X_OPTION(str_name, "1 2", "");
	LINK__VAR_DIGITAL_CTRL(dCH_CFG__DUMMY_PORT_WAFER_PER_PMC,str_name);

	//
	str_name = "CFG.DUMMY_PORT.PMC.IDLE.MODE";
	STD__ADD_DIGITAL_WITH_X_OPTION(str_name, "ALL PMx", "");
	LINK__VAR_DIGITAL_CTRL(dCH_CFG__DUMMY_PORT_PMC_IDLE_MODE, str_name);

	str_name = "CFG.DUMMY_PORT.PMC.IDLE.TIME";
	STD__ADD_ANALOG_WITH_X_OPTION(str_name, "sec", 0, 0, 9999, "");
	LINK__VAR_ANALOG_CTRL(aCH_CFG__DUMMY_PORT_PMC_IDLE_TIME, str_name);

	for(i=0; i<CFG_PM_LIMIT; i++)
	{
		str_name.Format("CFG.DUMMY_PORT.PM%1d.IDLE.TIME", i+1);
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "sec", 0, 0, 9999, "");
		LINK__VAR_ANALOG_CTRL(aCH_CFG__DUMMY_PORT_PMx_IDLE_TIME[i], str_name);
	}

	for(i=0; i<CFG_LP__SLOT_MAX; i++)
	{
		str_name.Format("CFG.DUMMY_PORT.USE.SLOT%1d", i+1);
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, "ENABLE DISABLE", "");
		LINK__VAR_DIGITAL_CTRL(xCH_CFG__DUMMY_PORT_SLOT_USE[i],str_name);

		str_name.Format("CFG.DUMMY_PORT.MAX_COUNT.SLOT%1d", i+1);
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "sec", 0, 5, 20, "");
		LINK__VAR_ANALOG_CTRL(aCH_CFG__DUMMY_PORT_SLOT_MAX_COUNT[i], str_name);
	}

	str_name = "APP.DUMMY_PORT.CUR_COUNT.RESET.REQ";
	STD__ADD_STRING(str_name);
	LINK__VAR_STRING_CTRL(sCH_APP__DUMMY_PORT_CUR_COUNT_RESET_REQ, str_name);

	str_name = "CFG.DUMMY_PORT.COUNT.WARNING.ALARM";
	STD__ADD_ANALOG_WITH_X_OPTION(str_name, "sec", 0, 0, 100, "");
	LINK__VAR_ANALOG_CTRL(aCH_CFG__DUMMY_PORT_COUNT_WARNING_ALARM, str_name);

	str_name = "CFG.LPx.PICK_MODE";
	STD__ADD_DIGITAL_WITH_X_OPTION(str_name,  DSP__LPx_PICK_MODE, "");
	LINK__VAR_DIGITAL_CTRL(xCH_CFG__LPx_PICK_MODE,str_name);

	// ... 
	{
		p_variable->Add__MONITORING_PROC(1.0, MON_ID__ALL_STATE);
		p_variable->Add__MONITORING_PROC(1.0, MON_ID__PORT_CTRL);
		
		// p_variable->Add__MONITORING_PROC(1.0, MON_ID__DB_CHECK);
	}
	return 1;
}

int CObj_Opr__AUTO_MODE::__DEFINE__ALARM(p_alarm)
{
	DECLARE__ALARM;

	// ...
	CString title;
	title.Format("%s : ",sObject_Name);

	// ...
	int alarm_id;
	CString alarm_title;
	CString alarm_msg;
	CStringArray l_act;
	int i;
	
	// ...
	{
		alarm_title  = title;
		alarm_title += "OBJECT MODE : UNKNOWN";

		alarm_msg = "";

		l_act.RemoveAll();
		l_act.Add(ACT__ABORT);
	
		ADD__ALARM_EX(ALID__OBJECT_MODE_UNKNOWN,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_title  = title;
		alarm_title += "SYSTEM IN PROCESS";

		alarm_msg = "Can't convert system status when job is running !";

		l_act.RemoveAll();
		l_act.Add(ACT__CHECK);

		ADD__ALARM_EX(ALID__SYSTEM_IN_PROCESS,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_title  = title;
		alarm_title += "SYSTEM IN MAINT MODE";

		alarm_msg = "Can't run job in maint mode !";

		l_act.RemoveAll();
		l_act.Add(ACT__CHECK);

		ADD__ALARM_EX(ALID__SYSTEM_IN_MAINT_MODE,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__JOB_START__ROUTE_RCP_TYPE_OLD;

		alarm_title  = title;
		alarm_title += "JOB START ERROR (100)";

		alarm_msg = "Old type의 route recipe는 시용할 수 없습니다.";

		l_act.RemoveAll();
		l_act.Add(ACT__CHECK);

		ADD__ALARM_EX(alarm_id, alarm_title,alarm_msg,l_act);
	}
	// ...
	{
		alarm_id = ALID__JOB_START__ROUTE_RCP_DIFFRENT;

		alarm_title  = title;
		alarm_title += "JOB START ERROR (101)";

		alarm_msg = "Route recipe is different !";

		l_act.RemoveAll();
		l_act.Add(ACT__CHECK);

		ADD__ALARM_EX(alarm_id, alarm_title,alarm_msg,l_act);
	}
	// ...
	{
		alarm_id = ALID__JOB_START__ROUTE_RCP_NO_SLOT_LIST;

		alarm_title  = title;
		alarm_title += "JOB START ERROR (102)";

		alarm_msg = "Route recipe has no slot list !";

		l_act.RemoveAll();
		l_act.Add(ACT__CHECK);

		ADD__ALARM_EX(alarm_id, alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_title  = title;
		alarm_title += "JOB START ERROR (1)";

		alarm_msg = "";

		l_act.RemoveAll();
		l_act.Add(ACT__ABORT);

		ADD__ALARM_EX(ALID__JOB_START__MATERIAL_ERROR,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_title  = title;
		alarm_title += "JOB START ERROR (2)";

		alarm_msg = "";

		l_act.RemoveAll();
		l_act.Add(ACT__ABORT);

		ADD__ALARM_EX(ALID__JOB_START__PM_ERROR,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_title  = title;
		alarm_title += "JOB START ERROR (3)";

		alarm_msg = "";
	
		l_act.RemoveAll();
		l_act.Add(ACT__ABORT);

		ADD__ALARM_EX(ALID__JOB_START__MODULE_ERROR,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_title  = title;
		alarm_title += "JOB START ERROR (4)";

		alarm_msg = "";

		l_act.RemoveAll();
		l_act.Add(ACT__CHECK);

		ADD__ALARM_EX(ALID__JOB_START__NO_SLOT_ERROR,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_title  = title;
		alarm_title += "JOB START ERROR (5.1)";

		alarm_msg = "";

		l_act.RemoveAll();
		l_act.Add(ACT__CHECK);

		ADD__ALARM_EX(ALID__JOB_START__LBx_MODE_ERROR,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_title  = title;
		alarm_title += "JOB START ERROR (5.2)";

		alarm_msg = "";

		l_act.RemoveAll();
		l_act.Add(ACT__CHECK);

		ADD__ALARM_EX(ALID__JOB_START__LLx_ALL_MODE_ERROR, alarm_title,alarm_msg,l_act);
	}

	// ...
	for(i=0;i<CFG_LP_LIMIT;i++)
	{
		alarm_title  = title;
		alarm_title += "JOB START ERROR (6)";

		alarm_msg = "";

		l_act.RemoveAll();
		l_act.Add(ACT__CHECK);

		ADD__ALARM_EX(ALID__JOB_START__NEW_JOB_ERROR_IN_SYS_END+i,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_title  = title;
		alarm_title += "JOB START ERROR (7)";

		alarm_msg = "";

		l_act.RemoveAll();
		l_act.Add(ACT__CHECK);

		ADD__ALARM_EX(ALID__JOB_START__NO_SLOT_MAP_ERROR,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_title  = title;
		alarm_title += "JOB START ERROR (8)";

		alarm_msg = "";

		l_act.RemoveAll();
		l_act.Add(ACT__CHECK);

		ADD__ALARM_EX(ALID__JOB_START__CID_NULL_ERROR,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_title  = title;
		alarm_title += "JOB START ERROR (9)";

		alarm_msg = "";

		l_act.RemoveAll();
		l_act.Add(ACT__CHECK);

		ADD__ALARM_EX(ALID__JOB_START__CID_DUPLICATE_ERROR,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_title  = title;
		alarm_title += "JOB START ERROR (10)";

		alarm_msg = "";

		l_act.RemoveAll();
		l_act.Add(ACT__CHECK);

		ADD__ALARM_EX(ALID__JOB_START__JOB_FILE_ERROR,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_title  = title;
		alarm_title += "JOB START ERROR (10)";
	
		alarm_msg = "";

		l_act.RemoveAll();
		l_act.Add(ACT__CHECK);

		ADD__ALARM_EX(ALID__JOB_START__ROUTE_RECIPE_ERROR,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_title  = title;
		alarm_title += "JOB START ERROR (11)";

		alarm_msg = "";

		l_act.RemoveAll();
		l_act.Add(ACT__CHECK);

		ADD__ALARM_EX(ALID__JOB_START__PM_RECIPE_ERROR,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_title  = title;
		alarm_title += "There is no dummy port !";

		alarm_msg  = "Please, check dummy port on config page.\n";
		alarm_msg += "To run dummy clean recipe, dummy port must exist.\n";

		l_act.RemoveAll();
		l_act.Add(ACT__CHECK);

		ADD__ALARM_EX(ALID__JOB_START__DUMMY_PORT__NOT_EXIST,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_title  = title;
		alarm_title += "There is no dummy empty slot !";

		alarm_msg = "Please, check material in dummy port.\n";

		l_act.RemoveAll();
		l_act.Add(ACT__CHECK);

		ADD__ALARM_EX(ALID__JOB_START__DUMMY_MATERIAL__NOT_EXIST,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_title  = title;
		alarm_title += "Any job at dummy port can not be executed !";

		alarm_msg = "Please, check configuration about LPx.\n";

		l_act.RemoveAll();
		l_act.Add(ACT__CHECK);

		ADD__ALARM_EX(ALID__JOB_START__ERROR_AT_DUMMY_PORT, alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_title  = title;
		alarm_title += "All job were ended by user !";

		alarm_msg = "All job were ended by user.\n";

		l_act.RemoveAll();
		l_act.Add(ACT__CHECK);

		ADD__ALARM_EX(ALID__ALL_JOB__END_BY_USER, alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_title  = title;
		alarm_title += "New job can not use buffer. !";

		alarm_msg  = "Please, check buffer paramter in config page. \n";
		alarm_msg += "At least, one buffer's use status must be \"ENABLE\". \n";

		l_act.RemoveAll();
		l_act.Add(ACT__CHECK);

		ADD__ALARM_EX(ALID__JOB_START__BUFFER_NOT_USE, alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_title  = title;
		alarm_title += "Manual Job Start Error !";

		alarm_msg  = "Please, check manual job start's parameter. \n";

		l_act.RemoveAll();
		l_act.Add(ACT__CHECK);

		ADD__ALARM_EX(ALID__MANUAL_JOB_START__ERROR, alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_title  = title;
		alarm_title += "Manual Job Start Error !";

		alarm_msg  = "Please, check cycle job start's parameter. \n";

		l_act.RemoveAll();
		l_act.Add(ACT__CHECK);

		ADD__ALARM_EX(ALID__CYCLE_JOB_START__ERROR, alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__JOB_START__PM_NORMAL_TYPE__NOT_EXIST;

		alarm_title  = title;
		alarm_title += "Job Start Error - There is no PMC with \"NORMAL\" process type. ";

		alarm_msg  = "Please, check the process type of LPx or PMx. \n";

		l_act.RemoveAll();
		l_act.Add(ACT__CHECK);
	
		ADD__ALARM_EX(alarm_id, alarm_title,alarm_msg,l_act);
	}
	// ...
	{
		alarm_id = ALID__JOB_START__PM_TEST_TYPE__NOT_EXIST;

		alarm_title  = title;
		alarm_title += "Job Start Error - There is no PMC with \"TEST\" process type. ";
		
		alarm_msg  = "Please, check the process type of LPx or PMx. \n";
		
		l_act.RemoveAll();
		l_act.Add(ACT__CHECK);
		
		ADD__ALARM_EX(alarm_id, alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__SYSTEM_DEADLOCK_ERROR;

		alarm_title  = title;
		alarm_title += "Job Start Error - Scheduler deadlock occurred !";

		alarm_msg  = "Job 수행중 Scheduler에서 Deadlock이 발생하여 \n";
		alarm_msg += " 설비 전체가 멈춘 것으로 판단됩니다. \n";
		alarm_msg += "설비 상태를 확인 바랍니다. \n";

		l_act.RemoveAll();
		l_act.Add(ACT__CHECK);

		ADD__ALARM_EX(alarm_id, alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_title  = title;
		alarm_title += "Dummy Port Count Warning !";

		alarm_msg  = "Dummy Port 의 평균 사용 횟수가 최대 사용 횟수에 근접했습니다.! \n";
		alarm_msg += "확인 바랍니다. \n";

		l_act.RemoveAll();
		l_act.Add(ACT__CHECK);

		ADD__ALARM_EX(ALID__DUMMY_PORT_COUNT_WARNING,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__RESYNC_EDIT_ENABLE;

		alarm_title  = title;
		alarm_title += "Resync Edit Enable !";

		alarm_msg = "Resync Edit Mode가 ENABLE로 전환되었습니다.";

		l_act.RemoveAll();
		l_act.Add(ACT__CHECK);

		ADD__ALARM_EX(alarm_id, alarm_title,alarm_msg,l_act);
	}
	// ...
	{
		alarm_id = ALID__RESYNC_EDIT_DISABLE;

		alarm_title  = title;
		alarm_title += "Resync Edit Disable !";

		alarm_msg  = "Resync Edit Mode가 DISABLE로 전환되었습니다.";

		l_act.RemoveAll();
		l_act.Add(ACT__CHECK);

		ADD__ALARM_EX(alarm_id, alarm_title,alarm_msg,l_act);
	}
	// ...
	{
		alarm_id = ALID__RESYNC_EDIT_WAFER_DUPLICATE;

		alarm_title  = title;
		alarm_title += "Resync Edit Wafer 중복 !";

		alarm_msg  = "Resync Edit시 Wafer 정보가 중복되는 문제가 발생했습니다. \n";
		alarm_msg += "중보된 Wafer를 확인 바랍니다.";

		l_act.RemoveAll();
		l_act.Add(ACT__CHECK);

		ADD__ALARM_EX(alarm_id, alarm_title,alarm_msg,l_act);
	}
	// ...
	{
		alarm_id = ALID__RESYNC_EDIT_WAFER_UI_SCH_DB_ERROR;

		alarm_title  = title;
		alarm_title += "Resync Edit : UI & Scheduler 정보 불일치 !";

		alarm_msg  = "Resync Edit시 UI 정보와 Scheduler 정보가 일치하지 않습니다. \n";
		alarm_msg += "UI 정보를 확인 바랍니다.";
		alarm_msg += "\n";
		alarm_msg += "만약 \"ABORT\"를 선택하면, JOB이 Abort 됩니다. \n";

		l_act.RemoveAll();
		l_act.Add(ACT__CHECK);
		l_act.Add(STR__ABORT);

		ADD__ALARM_EX(alarm_id, alarm_title,alarm_msg,l_act);
	}

	return 1;
}

int CObj_Opr__AUTO_MODE::__Define__USER_FUNCTION(CII_DEFINE__FUNCTION *p_fnc_ctrl)
{
	mERROR__USER_FNC.Set__OBJ_NAME(sObject_Name);
	p_fnc_ctrl->Link__ERROR_FEEDBACK(&mERROR__USER_FNC);

	return 1;
}

//-------------------------------------------------------------------------
int CObj_Opr__AUTO_MODE::__INITIALIZE__OBJECT(p_variable,p_ext_obj_create)
{
	int i;

	// ...
	{
		xSCH_MATERIAL_CTRL->Create__SERVICE_DB("");
		xSCH_MATERIAL_CTRL->Link__SERVICE_DB("");

		xSCH_JOB_FILE_CTRL->Link__Material_DB("");
		for(i=0;i<CFG_LP_LIMIT;i++)
		{
			xLPx_JOB_FILE_CTRL[i]->Link__Material_DB("");
		}
		xSEQ_INFO->Get__DIR_PROCESS(sDIR_PROCESS);

		iSIM_MODE = xSEQ_INFO->Is__SIMULATION_MODE();
	}

	// ...
	{
		p_ext_obj_create->Link__DEF_VARIABLE__ERROR_FEEDBACK(&mERROR__DEF_VAR);
	}

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

	CString str_name;
	CString def_name;
	CString def_data;
	CString ch_name;
	CString obj_name;
	CString var_name;
	
	// CFG -----
	{
		def_name = "DB_CFG_NAME";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name,def_data);

		// MSG ...
		{
			str_name = "SYS.MSG";
			LINK__EXT_VAR_STRING_CTRL(sEXT_CH__SYS_MSG, def_data,str_name);

			str_name = "FNC.MSG";
			LINK__EXT_VAR_STRING_CTRL(sEXT_CH__FNC_MSG, def_data,str_name);
		}

		// JOB START READY ...
		{
			str_name = "CFG.JOB_START_READY.CTRL.MODE";
			LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__CFG_JOB_START_READY_CTRL_MODE, def_data,str_name);

			for(i=0; i<CFG_PM_LIMIT; i++)
			{
				str_name.Format("CFG.JOB_START_READY.PM%1d.USE", i+1);
				LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__CFG_JOB_START_READY_PMx_USE[i], def_data,str_name);
			}
		}
		// JOB END READY ...
		{
			for(i=0; i<CFG_PM_LIMIT; i++)
			{
				str_name.Format("CFG.JOB_END_READY.PM%1d.USE", i+1);
				LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__CFG_JOB_END_READY_PMx_USE[i], def_data,str_name);
			}
		}

		for(i=0; i<CFG_PM_LIMIT; i++)
		{
			str_name.Format("SCH_TEST.CFG.PM%1d_DUMMY_BY_CTC", i+1);
			LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__SCH_TEST_CFG_PMX_DUMMY_BY_CTC[i], def_data,str_name);

			str_name = "SCH.CONCURRENT.ALWAYS.APPLY";
			LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__SCH_CONCURRENT_ALWAYS_APPLY, def_data,str_name);
		}

		// ...
		{
			str_name = "CFG.DUMMY.CLEAN.USE.FLAG";
			LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__CFG_DUMMY_CLEAN_USE_FLAG, def_data,str_name);

			str_name = "CFG.DUMMY.CLEAN.SYSTEM.IDLE.TIME";
			LINK__EXT_VAR_ANALOG_CTRL(xEXT_CH__CFG_DUMMY_CLEAN_SYSTEM_IDLE_TIME, def_data,str_name);

			str_name = "DUMMY.CLEAN.SYSTEM.IDLE.TIME.COUNT";
			LINK__EXT_VAR_ANALOG_CTRL(xEXT_CH__DUMMY_CLEAN_SYSTEM_IDLE_TIME_COUNT, def_data,str_name);

			str_name = "CFG.DUMMY.CLEAN.WAFER.PER.PMC";
			LINK__EXT_VAR_ANALOG_CTRL(xEXT_CH__CFG_DUMMY_CLEAN_WAFER_PER_PMC, def_data,str_name);
		}

		// ...
		{
			str_name = "SYS.USER.LEVEL";
			LINK__EXT_VAR_STRING_CTRL(xEXT_CH__USER_LEVEL, def_data,str_name);

			str_name = "SYS.USER.ID";
			LINK__EXT_VAR_STRING_CTRL(xEXT_CH__USER_ID, def_data,str_name);
		}

		// SCH.START_JOB.XXX ...
		{
			str_name = "SCH.START_JOB.LLx_IN.SIZE";
			LINK__EXT_VAR_STRING_CTRL(sEXT_CH__SCH_START_JOB_LLx_IN_SIZE, def_data,str_name);

			str_name = "SCH.START_JOB.LLx_OUT.SIZE";
			LINK__EXT_VAR_STRING_CTRL(sEXT_CH__SCH_START_JOB_LLx_OUT_SIZE, def_data,str_name);
		}
		// SCH.MODE.XXX ...
		{
			str_name = "SCH.MODE.LLx_IN";
			LINK__EXT_VAR_STRING_CTRL(sEXT_CH__SCH_MODE_LLx_IN, def_data,str_name);

			str_name = "SCH.MODE.LLx_OUT";
			LINK__EXT_VAR_STRING_CTRL(sEXT_CH__SCH_MODE_LLx_OUT, def_data,str_name);
		}

		// CFG_DB : AL1 ...
		{
			str_name = "CFG.AL1.USE.FLAG";
			LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__CFG_DB_AL1_USE_FLAG, def_data,str_name);

			str_name = "CFG.AL1.SLOT01.USE.FLAG";
			LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__CFG_DB_AL1_SLOT01_USE_FLAG, def_data,str_name);

			str_name = "CFG.AL1.SLOT02.USE.FLAG";
			LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__CFG_DB_AL1_SLOT02_USE_FLAG, def_data,str_name);
		}
		// SCH_DB : AL1 ...
		{
			str_name = "SCH_DB.CFG.AL1.USE.FLAG";
			LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__SCH_DB_AL1_USE_FLAG, def_data,str_name);

			str_name = "SCH_DB.CFG.AL1.SLOT01.USE.FLAG";
			LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__SCH_DB_AL1_SLOT01_USE_FLAG, def_data,str_name);

			str_name = "SCH_DB.CFG.AL1.SLOT02.USE.FLAG";
			LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__SCH_DB_AL1_SLOT02_USE_FLAG, def_data,str_name);
		}

		// CFG_DB ...
		{
			// LLx ...
			for(int ll_i=0; ll_i<CFG_LLx_LIMIT; ll_i++)
			{
				CString ll_name = Get__LLx_NAME(ll_i);

				//
				str_name.Format("%s.USE.FLAG", ll_name);
				LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__CFG_DB_LLx_USE_FLAG[ll_i], def_data,str_name);

				str_name.Format("%s.SLOT_MAX", ll_name);
				LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__CFG_DB_LLx_SLOT_MAX[ll_i], def_data,str_name);

				str_name.Format("%s.IDLE.STATUS", ll_name);
				LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__CFG_DB_LLx_IDLE_STATUS[ll_i], def_data,str_name);

				//
				str_name.Format("%s.MODE_TYPE", ll_name);
				pEXT_CH__CFG_DB_LLx_MODE_TYPE[ll_i] = p_ext_obj_create->Get__VAR_DIGITAL_CTRL(def_data,str_name);

				for(i=0; i<CFG_LLx__SLOT_MAX; i++)
				{
					str_name.Format("%s.SLOT%1d_STATUS", ll_name,i+1);
					pEXT_CH__CFG_DB_LLx_SLOT_STATUS[ll_i][i] = p_ext_obj_create->Get__VAR_DIGITAL_CTRL(def_data,str_name);

					str_name.Format("%s.SLOT%1d_MODE", ll_name,i+1);
					pEXT_CH__CFG_DB_LLx_SLOT_MODE[ll_i][i] = p_ext_obj_create->Get__VAR_DIGITAL_CTRL(def_data,str_name);
				}
			}

			// PMx 
			for(i=0; i<CFG_PM_LIMIT; i++)
			{
				str_name.Format("PM%1d.CUR.PRE_COUNT", i+1);
				LINK__EXT_VAR_STRING_CTRL(sEXT_CH__PMx__CUR_PRE_COUNT[i], def_data,str_name);

				str_name.Format("PM%1d.CUR.POST_COUNT", i+1);
				LINK__EXT_VAR_STRING_CTRL(sEXT_CH__PMx__CUR_POST_COUNT[i], def_data,str_name);
			}
		}
		// SCH_DB ...
		{
			// LLx ...
			for(int ll_i=0; ll_i<CFG_LLx_LIMIT; ll_i++)
			{
				CString ll_name = Get__LLx_NAME(ll_i);

				//
				str_name.Format("SCH_DB.%s.USE.FLAG", ll_name);
				LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__SCH_DB_LLx_USE_FLAG[ll_i], def_data,str_name);

				str_name.Format("SCH_DB.%s.SLOT_MAX", ll_name);
				LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__SCH_DB_LLx_SLOT_MAX[ll_i], def_data,str_name);

				str_name.Format("SCH_DB.%s.IDLE.STATUS", ll_name);
				LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__SCH_DB_LLx_IDLE_STATUS[ll_i], def_data,str_name);

				//
				str_name.Format("SCH_DB.%s.MODE_TYPE", ll_name);
				pEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_i] = p_ext_obj_create->Get__VAR_DIGITAL_CTRL(def_data,str_name);

				for(i=0; i<CFG_LLx__SLOT_MAX; i++)
				{
					str_name.Format("SCH_DB.%s.SLOT%1d_STATUS", ll_name,i+1);
					pEXT_CH__SCH_DB_LLx_SLOT_STATUS[ll_i][i] = p_ext_obj_create->Get__VAR_DIGITAL_CTRL(def_data,str_name);
				}
				for(i=0; i<CFG_LLx__SLOT_MAX; i++)
				{
					str_name.Format("SCH_DB.%s.SLOT%1d_MODE", ll_name,i+1);
					pEXT_CH__SCH_DB_LLx_SLOT_MODE[ll_i][i] = p_ext_obj_create->Get__VAR_DIGITAL_CTRL(def_data,str_name);
				}
			}
		}

		// STx ...
		{
			str_name = "CFG.BUFFER.JOB.WAIT.MODULE";
			LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__CFG_DB_STx_JOB_WAIT_MODULE, def_data,str_name);

			str_name = "CUR.BUFFER.SIZE";
			LINK__EXT_VAR_STRING_CTRL(xEXT_CH__CUR_STx_SIZE, def_data,str_name);
		}

		// PMx COMMON ...
		{
			for(i=0;i<CFG_PM_LIMIT;i++)
			{
				int pm_id = i + 1;

				str_name.Format("PM%1d.USE",pm_id);
				LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__PMx_CFG_USE[i], def_data,str_name);

				str_name.Format("PM%1d.PROCESS.TYPE",pm_id);
				LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__PMx_PROCESS_TYPE[i], def_data,str_name);
			}
		}

		// LEAK CHECK .....
		{
			for(i=0;i<CFG_PM_LIMIT;i++)
			{
				int pm_id = i + 1;

				str_name.Format("PM%1d.LEAK_CHECK.FLAG",pm_id);
				LINK__EXT_VAR_STRING_CTRL(xEXT_CH__PMx_LEAK_CHECK_FLAG[i], def_data,str_name);
			}

			LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__LEAK_CHECK__IDLE_USE_FLAG,  def_data,"LEAK_CHECK.IDLE.USE.FLAG");
			LINK__EXT_VAR_ANALOG_CTRL( xEXT_CH__LEAK_CHECK__IDLE_REF_TIME,  def_data,"LEAK_CHECK.IDLE.REF.TIME");
			LINK__EXT_VAR_ANALOG_CTRL( xEXT_CH__LEAK_CHECK__IDLE_CUR_TIME,  def_data,"LEAK_CHECK.IDLE.CUR.TIME");
			LINK__EXT_VAR_ANALOG_CTRL( xEXT_CH__LEAK_CHECK__IDLE_REF_COUNT, def_data,"LEAK_CHECK.IDLE.REF.COUNT");
			LINK__EXT_VAR_ANALOG_CTRL( xEXT_CH__LEAK_CHECK__IDLE_CUR_COUNT, def_data,"LEAK_CHECK.IDLE.CUR.COUNT");

			xI_LEAK_CHECK_IDLE_TIMER->REGISTER__COUNT_CHANNEL(def_data, xEXT_CH__LEAK_CHECK__IDLE_CUR_TIME->Get__VARIABLE_NAME());
		}
		// AUTO_PM .....
		{
			for(i=0;i<CFG_PM_LIMIT;i++)
			{
				int pm_id = i + 1;

				str_name.Format("PM%1d.AUTO_PM.FLAG",pm_id);
				LINK__EXT_VAR_STRING_CTRL(xEXT_CH__PMx_AUTO_PM_FLAG[i], def_data,str_name);
			}

			LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__AUTO_PM__IDLE_USE_FLAG,  def_data,"AUTO_PM.IDLE.USE.FLAG");
			LINK__EXT_VAR_ANALOG_CTRL( xEXT_CH__AUTO_PM__IDLE_REF_TIME,  def_data,"AUTO_PM.IDLE.REF.TIME");
			LINK__EXT_VAR_ANALOG_CTRL( xEXT_CH__AUTO_PM__IDLE_CUR_TIME,  def_data,"AUTO_PM.IDLE.CUR.TIME");
			LINK__EXT_VAR_ANALOG_CTRL( xEXT_CH__AUTO_PM__IDLE_REF_COUNT, def_data,"AUTO_PM.IDLE.REF.COUNT");
			LINK__EXT_VAR_ANALOG_CTRL( xEXT_CH__AUTO_PM__IDLE_CUR_COUNT, def_data,"AUTO_PM.IDLE.CUR.COUNT");

			xI_AUTO_PM_IDLE_TIMER->REGISTER__COUNT_CHANNEL(def_data, xEXT_CH__AUTO_PM__IDLE_CUR_TIME->Get__VARIABLE_NAME());
		}

		// CYCLE MODE ...
		{
			LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__CFG_CYCLE_FOUP_CTRL_MODE, def_data,"CFG.CYCLE.FOUP.MODE");
		}

		for(i=0; i<CFG_LP__SLOT_MAX; i++)
		{
			str_name.Format("APP.DUMMY_PORT.CUR_COUNT.SLOT%1d", i+1);
			LINK__EXT_VAR_STRING_CTRL(sEXT_CH__APP_DUMMY_PORT_CUR_COUNT_SLOTx[i], def_data,str_name);
		}

		for(i=0; i<CFG_PM_LIMIT; i++)
		{
			str_name.Format("PM%1d.USE", i+1);
			LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__CFG__PMx_USE[i], def_data,str_name);
		}
	}

	// ATM RB -----
	{
		def_name = "OBJ__ATM_RB";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name,def_data);

		// ...
		{
			str_name = "CFG.BUFFER.APPLY.MODE";
			LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__CFG_DB_STx_APPLY_MODE, def_data,str_name);

			str_name = "CFG.BUFFER.TRANSFER.MODE";
			LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__CFG_DB_STx_TRANSFER_MODE, def_data,str_name);

			str_name = "CFG.BUFFER.WAFER.CLEAN.TIME";
			LINK__EXT_VAR_ANALOG_CTRL(xEXT_CH__CFG_DB_BUFFER_WAFER_CLEAN_TIME, def_data,str_name);

			str_name = "CFG.BUFFER.WAFER.PICK.MODE";
			LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__CFG_DB_STx_WAFER_PICK_MODE, def_data,str_name);

			str_name = "CFG.BUFFER.OUT_MODE.BUFFER_TO_LPo";
			LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__CFG_DB_STx_OUT_MODE_BUFFER_TO_LPo, def_data,str_name);

			str_name = "CFG.BUFFER.LOTID.CHECK";
			LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__CFG_DB_STx_LOTID_CHECK, def_data,str_name);
		}

		// ...
		{
			str_name = "SCH_DB.CFG.BUFFER.TRANSFER.MODE";
			LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__SCH_DB_STx_TRANSFER_MODE, def_data,str_name);

			//
			str_name = "SCH_DB.ST1.USE.FLAG";
			LINK__EXT_VAR_STRING_CTRL(sEXT_CH__SCH_DB_ST1_USE_FLAG, def_data,str_name);

			str_name = "SCH_DB.ST2.USE.FLAG";
			LINK__EXT_VAR_STRING_CTRL(sEXT_CH__SCH_DB_ST2_USE_FLAG, def_data,str_name);

			//
			str_name = "SCH_DB.ST1.WAFER.CLEAN.TIME";
			LINK__EXT_VAR_ANALOG_CTRL(xEXT_CH__SCH_DB_ST1_WAFER_CLEAN_TIME, def_data,str_name);

			str_name = "SCH_DB.ST2.WAFER.CLEAN.TIME";
			LINK__EXT_VAR_ANALOG_CTRL(xEXT_CH__SCH_DB_ST2_WAFER_CLEAN_TIME, def_data,str_name);

			//
			str_name = "SCH_DB.CFG.ST1.WAFER.PICK.MODE";
			LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__SCH_DB_ST1_WAFER_PICK_MODE, def_data,str_name);

			str_name = "SCH_DB.CFG.ST2.WAFER.PICK.MODE";
			LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__SCH_DB_ST2_WAFER_PICK_MODE, def_data,str_name);

			//
			str_name = "SCH_DB.STx.CFG.APPLY.MODE";
			LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__SCH_DB_STx_APPLY_MODE, def_data,str_name);

			//
			str_name = "SCH_DB.ST1.OUT_MODE.BUFFER_TO_LPo";
			LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__SCH_DB_ST1_OUT_MODE_BUFFER_TO_LPo, def_data,str_name);

			str_name = "SCH_DB.ST2.OUT_MODE.BUFFER_TO_LPo";
			LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__SCH_DB_ST2_OUT_MODE_BUFFER_TO_LPo, def_data,str_name);
		}
	}

	// LPx -----
	{
		def_name = "LPx_UNIT_SIZE";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name,def_data);
	
		iLPx_UNIT_SIZE = atoi(def_data);
		if(iLPx_UNIT_SIZE > CFG_LP_LIMIT)		iLPx_UNIT_SIZE = CFG_LP_LIMIT;

		for(i=0;i<iLPx_UNIT_SIZE;i++)
		{
			def_name.Format("OBJ__LP%1d",i+1);
			p_ext_obj_create->Get__DEF_CONST_DATA(def_name,def_data);

			pPORT__OBJ_CTRL[i] = p_ext_obj_create->Create__OBJECT_CTRL(def_data);

			// ...
			LINK__EXT_VAR_DIGITAL_CTRL(xCH__PORT_CFG_USE_TYEP[i], def_data,"CFG.USE");
			LINK__EXT_VAR_DIGITAL_CTRL(xCH__PORT_CFG_SLOT_MAX[i], def_data,"CFG.SLOT.MAX");

			LINK__EXT_VAR_DIGITAL_CTRL(xCH__PORT_CST_STATUS[i], def_data,"CST.STATUS");

			LINK__EXT_VAR_STRING_CTRL(sCH__PORT_HOQ_REQ[i], def_data,"LPx.HOQ.REQ");

			//
			LINK__EXT_VAR_ANALOG_CTRL(xCH__PORT_LAST_PICK_SLOT[i],    def_data,"APP.LAST.PICK.SLOT");
			LINK__EXT_VAR_ANALOG_CTRL(xCH__PORT_LAST_RESERVE_SLOT[i], def_data,"APP.LAST.RESERVE.SLOT");

			//
			LINK__EXT_VAR_DIGITAL_CTRL(xCH__PORT_CTRL[i],   def_data,"APP.PORT.CTRL");
			LINK__EXT_VAR_STRING_CTRL(xCH__PORT_JOB_POPUP_TYPE[i], def_data,"APP.JOB.POPUP.TYPE");
			LINK__EXT_VAR_STRING_CTRL(xCH__PORT_CJOBID[i],  def_data,"APP.CJOBID");
			LINK__EXT_VAR_STRING_CTRL(xCH__PORT_JOB_NAME[i],def_data,"APP.JOB.NAME");
			LINK__EXT_VAR_STRING_CTRL(xCH__CSTID[i],        def_data,"CID.STRING");
			LINK__EXT_VAR_STRING_CTRL(xCH__PORT_STATUS[i],  def_data,"APP.PORT.STATUS");

			LINK__EXT_VAR_STRING_CTRL(xCH__PORT_LOTID[i],   def_data,"APP.PORT.LOTID");
			LINK__EXT_VAR_STRING_CTRL(xCH__PORT_PPID[i],    def_data,"APP.PORT.PPID");

			LINK__EXT_VAR_STRING_CTRL(xCH__PORT_EXCEPTION[i], def_data,"APP.PORT.EXCEPTION");
			LINK__EXT_VAR_STRING_CTRL(xCH__PORT_LAMP_STATUS[i], def_data,"LAMP.STATUS");

			//
			str_name = "DB.LOTID";
			LINK__EXT_VAR_STRING_CTRL(xCH__PORT_DB_LOTID[i], def_data,str_name);

			str_name = "DB.ROUTEID";
			LINK__EXT_VAR_STRING_CTRL(xCH__PORT_DB_ROUTEID[i], def_data,str_name);

			str_name = "DB.CYCLE.START.SLOT";
			LINK__EXT_VAR_ANALOG_CTRL(xCH__PORT_DB_CYCLE_START_SLOT[i], def_data,str_name);

			str_name = "DB.CYCLE.END.SLOT";
			LINK__EXT_VAR_ANALOG_CTRL(xCH__PORT_DB_CYCLE_END_SLOT[i], def_data,str_name);

			str_name = "DB.CYCLE.COUNT";
			LINK__EXT_VAR_ANALOG_CTRL(xCH__PORT_DB_CYCLE_COUNT[i], def_data,str_name);

			str_name = "CUR.CYCLE.COUNT";
			LINK__EXT_VAR_ANALOG_CTRL(xCH__PORT_CUR_CYCLE_COUNT[i], def_data,str_name);

			str_name = "LPx.END.REQ.FLAG";
			LINK__EXT_VAR_STRING_CTRL(xCH__PORT_END_REQ_FLAG[i], def_data,str_name);

			str_name = "DB.CYCLE.MSG";
			LINK__EXT_VAR_STRING_CTRL(xCH__PORT_DB_CYCLE_MSG[i], def_data,str_name);

			//
			str_name = "MANUAL.CTRL.FLAG";
			LINK__EXT_VAR_STRING_CTRL(xCH__PORT_MANUAL_CTRL_FLAG[i], def_data,str_name);

			str_name = "CYCLE.CTRL.FLAG";
			LINK__EXT_VAR_STRING_CTRL(xCH__PORT_CYCLE_CTRL_FLAG[i], def_data,str_name);

			//
			str_name = "MANUAL.CTRL.RUNNING.FLAG";
			LINK__EXT_VAR_STRING_CTRL(xCH__PORT_MANUAL_CTRL_RUNNING_FLAG[i], def_data,str_name);

			str_name = "CYCLE.CTRL.RUNNING.FLAG";
			LINK__EXT_VAR_STRING_CTRL(xCH__PORT_CYCLE_CTRL_RUNNING_FLAG[i], def_data,str_name);

			//
			str_name = "CFG.PROCESS.TYPE";
			LINK__EXT_VAR_DIGITAL_CTRL(xCH__LPx__CFG_PROCESS_TYPE[i], def_data,str_name);

			//
			str_name = "LPx.END.REQ.FLAG";
			LINK__EXT_VAR_STRING_CTRL(xCH__LPx_END_REQ_FLAG[i], def_data,str_name);
			
			str_name = "LPx.RETURN.REQ.FLAG";
			LINK__EXT_VAR_STRING_CTRL(xCH__LPx_RETURN_REQ_FLAG[i], def_data,str_name);

			// ...
			int k;

			for(k=0;k<CFG_LP__SLOT_MAX;k++)
			{
				str_name.Format("SLOT%002d.STATUS",k+1);
				LINK__EXT_VAR_DIGITAL_CTRL(xCH__PORT_SLOT_STS[i][k], def_data,str_name);
			}
		}
	}

	// BUFFERx ...
	{
		def_name = "BUFFERx_UNIT_SIZE";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name,def_data);

		int unit_size = atoi(def_data);
		if(unit_size > CFG_BUFFER__UNIT_LIMIT)			unit_size = CFG_BUFFER__UNIT_LIMIT;

		iBUFF_UNIT_SIZE = unit_size;

		for(i=0; i<iBUFF_UNIT_SIZE; i++)
		{
			def_name.Format("BUFFER%1d_OBJECT", i+1);
			p_ext_obj_create->Get__DEF_CONST_DATA(def_name, obj_name);

			def_name.Format("BUFFER%1d_SLOT_SIZE", i+1);
			p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);

			int slot_size = atoi(def_data);
			if(slot_size > CFG_BUFFER__SLOT_MAX)		slot_size = CFG_BUFFER__SLOT_MAX;

			iBUFF_SLOT_SIZE = slot_size;

			for(int k=0; k<iBUFF_SLOT_SIZE; k++)
			{
				var_name.Format("SLOT%02d.STATUS", k+1);
				LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__BUFFx_SLOT[i][k], obj_name,var_name);
			}

			var_name = "CFG.USE.FLAG";
			LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__CFG_DB_STx_USE_FLAG[i], obj_name,var_name);
		}
	}

	// ATM_RB ...
	{
		for(i=0; i<2; i++)
		{
				 if(i == 0)		def_name = "CH.ATM_RB.A.MATERIAL_STATUS.ARM";
			else if(i == 1)		def_name = "CH.ATM_RB.A.MATERIAL_TITLE.ARM";
			else				continue;

			p_ext_obj_create->Get__DEF_CONST_DATA(def_name, ch_name);
			p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(ch_name, obj_name,var_name);

			if(i == 0)
			{
				LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__ATM_RB_A__MATERIAL_STATUS_ARM, obj_name,var_name);
			}
			else if(i == 1)
			{
				LINK__EXT_VAR_STRING_CTRL(sEXT_CH__ATM_RB_A__MATERIAL_TITLE_ARM,   obj_name,var_name);
			}
		}
	}

	// AL1 ...
	{
		def_name = "AL1_SLOT_SIZE";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name,def_data);

		int slot_size = atoi(def_data);
		if(slot_size > CFG_AL__SLOT_MAX)		slot_size = CFG_AL__SLOT_MAX;

		iAL1_SLOT_SIZE = slot_size;
		
		for(i=0; i<iAL1_SLOT_SIZE; i++)
		{
			int slot_id = i + 1;

			for(int k=0; k<2; k++)
			{
					 if(k == 0)		def_name.Format("CH.AL1.%1d.MATERIAL_STATUS.SLOT", slot_id);
				else if(k == 1)		def_name.Format("CH.AL1.%1d.MATERIAL_TITLE.SLOT",  slot_id);
				else				continue;

				p_ext_obj_create->Get__DEF_CONST_DATA(def_name, ch_name);
				p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(ch_name, obj_name,var_name);

				if(k == 0)
				{
					LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__AL1_X__MATERIAL_STATUS_SLOT[i], obj_name,var_name);
				}
				else if(k == 1)
				{
					LINK__EXT_VAR_STRING_CTRL(sEXT_CH__AL1_X__MATERIAL_TITLE_SLOT[i],   obj_name,var_name);
				}
			}
		}
	}

	// LLx ...
	{
		// LLx_SIZE ...
		{
			def_name = "LLx_SIZE";
			p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);

			iLLx_SIZE = atoi(def_data);
			if(iLLx_SIZE > CFG_LLx_LIMIT)			iLLx_SIZE = CFG_LLx_LIMIT;
		}
		// LLx_SLOT_MAX ...
		{
			def_name = "LLx_SLOT_MAX";
			p_ext_obj_create->Get__DEF_CONST_DATA(def_name,def_data);

			int slot_size = atoi(def_data);
			if(slot_size > CFG_LLx__SLOT_MAX)		slot_size = CFG_LLx__SLOT_MAX;

			iLLx__SLOT_SIZE = slot_size;
		}

		for(int ll_i=0; ll_i<iLLx_SIZE; ll_i++)
		{
			int ll_id = ll_i + 1;

			for(i=0; i<iLLx__SLOT_SIZE; i++)
			{
				int slot_id = i + 1;

				for(int k=0; k<2; k++)
				{
						 if(k == 0)		def_name.Format("CH.LL%1d.%1d.MATERIAL_STATUS.SLOT", ll_id,slot_id);
					else if(k == 1)		def_name.Format("CH.LL%1d.%1d.MATERIAL_TITLE.SLOT",  ll_id,slot_id);
					else				continue;

					p_ext_obj_create->Get__DEF_CONST_DATA(def_name, ch_name);
					p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(ch_name, obj_name,var_name);

					if(k == 0)
					{
						LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__LLx_X__MATERIAL_STATUS_SLOT[ll_i][i], obj_name,var_name);
					}
					else if(k == 1)
					{
						LINK__EXT_VAR_STRING_CTRL(sEXT_CH__LLx_X__MATERIAL_TITLE_SLOT[ll_i][i], obj_name,var_name);
					}
				}
			}
		}
	}

	// VAC_RB ...
	{
		int arm_size = 2;
		CString arm_type;

		for(i=0; i<arm_size; i++)
		{
				 if(i == 0)		arm_type = "A";
			else if(i == 1)		arm_type = "B";
			else				continue;

			for(int k=0; k<2; k++)
			{ 
					 if(k == 0)		def_name.Format("CH.VAC_RB.%s.MATERIAL_STATUS.ARM", arm_type);
				else if(k == 1)		def_name.Format("CH.VAC_RB.%s.MATERIAL_TITLE.ARM",  arm_type);
				else				continue;

				p_ext_obj_create->Get__DEF_CONST_DATA(def_name, ch_name);
				p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(ch_name, obj_name,var_name);

				if(i == 0)
				{
					if(k == 0)
					{
						LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__VAC_RB_A__MATERIAL_STATUS_ARM, obj_name,var_name);
					}
					else if(k == 1)
					{
						LINK__EXT_VAR_STRING_CTRL(sEXT_CH__VAC_RB_A__MATERIAL_TITLE_ARM,   obj_name,var_name);
					}
				}
				else if(i == 1)
				{
					if(k == 0)
					{
						LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__VAC_RB_B__MATERIAL_STATUS_ARM, obj_name,var_name);
					}
					else if(k == 1)
					{
						LINK__EXT_VAR_STRING_CTRL(sEXT_CH__VAC_RB_B__MATERIAL_TITLE_ARM,   obj_name,var_name);
					}
				}
			}
		}
	}

	// PMx -----
	{
		def_name = "PMx_UNIT_SIZE";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name,def_data);

		iPMx_UNIT_SIZE = atoi(def_data);
		if(iPMx_UNIT_SIZE > CFG_PM_LIMIT)		iPMx_UNIT_SIZE = CFG_PM_LIMIT;
			
		for(i=0;i<iPMx_UNIT_SIZE;i++)
		{
			def_name.Format("OBJ__PM%1d",i+1);
			p_ext_obj_create->Get__DEF_CONST_DATA(def_name,def_data);

			pPMx__OBJ_CTRL[i] = p_ext_obj_create->Create__OBJECT_CTRL(def_data);

			// ...
			str_name = "CTRL";
			LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__PMx__OBJ_CTRL[i], def_data,str_name);

			str_name = "OBJ.VIRTUAL.STATUS";
			LINK__EXT_VAR_STRING_CTRL(xEXT_CH__PMx_OBJECT_STATUS[i], def_data,str_name);

			str_name = "OBJ.STATUS";
			LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__PMx__OBJ_STATUS[i], def_data,str_name);

			// ...
			str_name = "CFG.DUMMY.PROC.RECIPE";
			LINK__EXT_VAR_STRING_CTRL(xEXT_CH__PMx_CFG_DUMMY_PROC_RCP[i], def_data,str_name);

			str_name = "CFG.DUMMY.CLN.RECIPE";
			LINK__EXT_VAR_STRING_CTRL(xEXT_CH__PMx_CFG_DUMMY_CLN_RCP[i], def_data,str_name);

			str_name = "CFG.DUMMY.PRE.RECIPE";
			LINK__EXT_VAR_STRING_CTRL(xEXT_CH__PMx_CFG_DUMMY_PRE_RCP[i], def_data,str_name);

			// ...
			str_name = "PMC.IDLE.TIME";
			LINK__EXT_VAR_ANALOG_CTRL(xEXT_CH__PMx_CUR_IDLE_TIME[i], def_data,str_name);

			// ...
			str_name = "SLOT01.STATUS";
			LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__PMx_SLOT_STATUS[i], def_data,str_name);

			// ...
			str_name = "PRE_INFO.JOB_START_READY.PORTID";
			LINK__EXT_VAR_STRING_CTRL(sEXT_CH__PMx_PRE_INFO_JOB_START_READY_PORTID[i], def_data,str_name);

			str_name = "PRE_INFO.JOB_START_READY.JOBID";
			LINK__EXT_VAR_STRING_CTRL(sEXT_CH__PMx_PRE_INFO_JOB_START_READY_JOBID[i], def_data,str_name);

			//
			str_name = "REQ_INFO.JOB_START_READY.PORTID";
			LINK__EXT_VAR_STRING_CTRL(sEXT_CH__PMx_REQ_INFO_JOB_START_READY_PORTID[i], def_data,str_name);

			str_name = "REQ_INFO.JOB_START_READY.JOBID";
			LINK__EXT_VAR_STRING_CTRL(sEXT_CH__PMx_REQ_INFO_JOB_START_READY_JOBID[i], def_data,str_name);
		}
	}

	// ROBOT PAUSE ...
	{
		obj_name = "FNC_ATM_ROBOT";
		var_name = "ATM_RB.PAUSE.FLAG";
		LINK__EXT_VAR_STRING_CTRL(sEXT_CH__ATM_ROBOT__PAUSE_FLAG, obj_name,var_name);

		obj_name = "FNC_VAC_ROBOT";
		var_name = "VAC_RB.PAUSE.FLAG";
		LINK__EXT_VAR_STRING_CTRL(sEXT_CH__VAC_ROBOT__PAUSE_FLAG, obj_name,var_name);
	}

	// ...
	{
		SCX__SEQ_INFO x_seq_info;

		x_seq_info->Get__DIR_ROOT(sDIR_ROOT);
	}

	// ...
	{
		xEXT_CH__SCH_CONCURRENT_ALWAYS_APPLY->Set__DATA(STR__PPID_CHECK);
	}
	return 1;
}

//-------------------------------------------------------------------------
int CObj_Opr__AUTO_MODE::__CALL__CONTROL_MODE(mode,p_debug,p_variable,p_alarm)
{
	int flag = -1;

	IF__CTRL_MODE(sMODE__MAINT)
	{
		flag = Call__MAINT_MODE(p_variable,p_alarm);
	}
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

	return flag;
}

int CObj_Opr__AUTO_MODE::__CALL__MONITORING(id,p_variable,p_alarm)
{
	switch(id)
	{
		case MON_ID__ALL_STATE:
			Mon__ALL_STATE(p_variable,p_alarm);
			break;

		case MON_ID__PORT_CTRL:
			Mon__PORT_CTRL(p_variable,p_alarm);
			break;

		case MON_ID__DB_CHECK:
			xSCH_MATERIAL_CTRL->Check__STS_DB();
			break;
	}

	return 1;
}
