#include "StdAfx.h"
#include "CObj__DUAL_ARM_STD.h"


// ...
CObj__DUAL_ARM_STD::CObj__DUAL_ARM_STD()
{

}
CObj__DUAL_ARM_STD::~CObj__DUAL_ARM_STD()
{

}

// ...
int CObj__DUAL_ARM_STD
::__DEFINE__CONTROL_MODE(obj,l_mode)
{
	sObject_Name = obj;

	// ...
	{
		int i;

		for(i=0; i<CFG_LLx_LIMIT; i++)
		{
			pLLx__OBJ_CTRL[i] = NULL;
		}

		for(i=0;i<CFG_PM_LIMIT;i++)
		{
			pPMx__OBJ_CTRL[i] = NULL;
		}
		iPMx_SIZE = 0;
	}

	// ...
	{
		ADD__CTRL_VAR(sMODE__MAINT_PICK, "MAINT_PICK" );
		ADD__CTRL_VAR(sMODE__MAINT_PLACE,"MAINT_PLACE");

		ADD__CTRL_VAR(sMODE__CLEAR,"CLEAR");

		//
		ADD__CTRL_VAR(sMODE__CHECK_MATERIAL, "CHECK_MATERIAL");
		ADD__CTRL_VAR(sMODE__CHECK_STANDBY,  "CHECK_STANDBY");

		//
		ADD__CTRL_VAR(sMODE__WAFER_RESYNC_CLEAR_SYS, "WAFER_RESYNC.CLEAR_SYS");
		ADD__CTRL_VAR(sMODE__WAFER_RESYNC_CLEAR_PMx, "WAFER_RESYNC.CLEAR_PMx");
		ADD__CTRL_VAR(sMODE__WAFER_RESYNC_CLEAR_LLx, "WAFER_RESYNC.CLEAR_LLx");

		//
		ADD__CTRL_VAR(sMODE__SCH_DB_CHECK,    "SCH_DB.CHECK");
		ADD__CTRL_VAR(sMODE__USER_INFO_CHECK, "USER_INFO.CHECK");
	}
	return 1;
}

int CObj__DUAL_ARM_STD
::__DEFINE__VERSION_HISTORY(version)
{
	version = "JG.Lee \n";

	return 1;
}


// ...
#define  MON_ID__ALL_STATE					1

#define  MON_ID__AUTO_MODE					3
#define  MON_ID__PMx_CTRL					4

// ...
#define  DSP__ARM_TYPE						"ALL  A  B"
#define  DSP__PICK_DELAY_FLAG				"DISABLE  ENABLE"
#define  DSP__DISABLE_ENABLE				"DISABLE  ENABLE"
#define  DSP__MATERIAL_FLAG					"NONE  EXIST"
#define  DSP__PLACE_TO_LBx_SLOT				"ALL  1  2"
#define  DSP__SCH_ENABLE_DISABLE			"ENABLE  DISABLE"

#define  APP_DSP__ARM_DIRECTION				"UNI_DIRECTION  BI_DIRECTION"


int CObj__DUAL_ARM_STD
::__DEFINE__VARIABLE_STD(p_variable)
{
	DECLARE__STD_VARIABLE;

	CString str_name;
	int i;

	// ...
	{
		str_name = "SCH.LOOP.ID";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__SCH_LOOP_ID, str_name);

		str_name = "QUERY.LOG.MODE";
		STD__ADD_DIGITAL(str_name, DSP__DISABLE_ENABLE);
		LINK__VAR_DIGITAL_CTRL(dCH__QUERY_LOG_MODE, str_name);
	}

	// WAC ...
	{
		str_name = "INFO.WAC.FRONT.ACTIVE";
		STD__ADD_DIGITAL(str_name, "OFF ON");
		LINK__VAR_DIGITAL_CTRL(dCH__INFO_WAC_FRONT_ACTIVE, str_name);
	}

	// CFG PHYSICAL ...
	{
		str_name = "CFG.WAFER_WAITING_LIMIT_TIME_ON_ARM";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec", 0, 0.0, 60,"");
		LINK__VAR_ANALOG_CTRL(aCH__CFG_WAFER_WAITING_LIMIT_TIME_ON_ARM, str_name);

		str_name = "CUR.WAFER_WAITING_LIMIT_TIME_ON_ARM";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__CUR_WAFER_WAITING_LIMIT_TIME_ON_ARM, str_name);

		//
		str_name = "CFG.PHY.FRONT_TIME";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",1,0.0,60,"");
		LINK__VAR_ANALOG_CTRL(xCH_CFG__PHY_FRONT_TIME,str_name);

		str_name = "CFG.PHY.PICK_TIME";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",1,0.0,60,"");
		LINK__VAR_ANALOG_CTRL(xCH_CFG__PHY_PICK_TIME,str_name);

		str_name = "CFG.PHY.PLACE_TIME";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",1,0.0,60,"");
		LINK__VAR_ANALOG_CTRL(xCH_CFG__PHY_PLACE_TIME,str_name);

		//
		str_name = "CFG.CONCURRENT.MARGIN_TIME";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",1,0.0,60,"");
		LINK__VAR_ANALOG_CTRL(xCH_CFG__PHY_CONCURRENT_MARGIN_TIME,str_name);

		str_name = "CFG.PMC_CLN_PROC_TIME.FOR_LBx_PICK";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",0,10.0,100,"");
		LINK__VAR_ANALOG_CTRL(xCH_CFG__PMC_CLN_PROC_TIME_FOR_LBx_PICK,str_name);

		//
		str_name = "CFG.ARM_TYPE.PICK_FROM_LBX";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,DSP__ARM_TYPE,"");
		LINK__VAR_DIGITAL_CTRL(xCH_CFG__PICK_ARM_TYPE_FROM_LBX,str_name);

		str_name = "CFG.ARM_TYPE.PICK_FROM_PMC";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,DSP__ARM_TYPE,"");
		LINK__VAR_DIGITAL_CTRL(xCH_CFG__PICK_ARM_TYPE_FROM_PMC,str_name);

		//
		str_name = "CFG.A_ARM.PLACE_TO_LBx_SLOT";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,DSP__PLACE_TO_LBx_SLOT,"");
		LINK__VAR_DIGITAL_CTRL(xCH_CFG__A_ARM_PLACE_TO_LBx_SLOT,str_name);

		str_name = "CFG.B_ARM.PLACE_TO_LBx_SLOT";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,DSP__PLACE_TO_LBx_SLOT,"");
		LINK__VAR_DIGITAL_CTRL(xCH_CFG__B_ARM_PLACE_TO_LBx_SLOT,str_name);

		//
		str_name = "CFG.ROBOT.ARM_DIRECTION_TYPE";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,APP_DSP__ARM_DIRECTION,"");
		LINK__VAR_DIGITAL_CTRL(xCH_CFG__ROBOT_ARM_DIRECTION_TYPE,str_name);

		//
		str_name = "CFG.SCH.EXCHANGE.PRIORITY";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"PMC WAFER","");
		LINK__VAR_DIGITAL_CTRL(xCH_CFG__SCH_EXCHANGE_PRIORITY,str_name);

		str_name = "CFG.SCH.LLx_EXCHANGE.WHEN_PMC_EMPTY";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,DSP__SCH_ENABLE_DISABLE,"");
		LINK__VAR_DIGITAL_CTRL(xCH_CFG__SCH_LLx_EXCHANGE_WHEN_PMC_EMPTY,str_name);

		//
		str_name = "CFG.PICK_DELAY.FLAG";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,DSP__PICK_DELAY_FLAG,"");
		LINK__VAR_DIGITAL_CTRL(xCH_CFG__PICK_DELAY_FLAG,str_name);

		str_name = "CFG.PICK_DELAY.FIRST_TIME";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",1,0.0,100,"");
		LINK__VAR_ANALOG_CTRL(xCH_CFG__PICK_DELAY_FIRST_TIME,str_name);

		str_name = "CFG.PICK_DELAY.SECOND_TIME";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",1,0.0,100,"");
		LINK__VAR_ANALOG_CTRL(xCH_CFG__PICK_DELAY_SECOND_TIME,str_name);

		str_name = "CFG.PICK_DELAY.THIRD_TIME";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",1,0.0,100,"");
		LINK__VAR_ANALOG_CTRL(xCH_CFG__PICK_DELAY_THIRD_TIME,str_name);

		str_name = "CFG.PICK_DELAY.REMAIN_TIME";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",1,0.0,100,"");
		LINK__VAR_ANALOG_CTRL(xCH_CFG__PICK_DELAY_REMAIN_TIME,str_name);

		//
		str_name = "CFG.CLEAN_MATERIAL.FLAG";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,DSP__MATERIAL_FLAG,"");
		LINK__VAR_DIGITAL_CTRL(xCH_CFG__CLEAN_MATERIAL_FLAG,str_name);

		str_name = "CFG.MATERIAL_COUNT.FLAG";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,DSP__DISABLE_ENABLE,"");
		LINK__VAR_DIGITAL_CTRL(xCH_CFG__MATERIAL_COUNT_FLAG,str_name);

		str_name = "CFG.PMx_SWAP.FLAG";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,DSP__DISABLE_ENABLE,"");
		LINK__VAR_DIGITAL_CTRL(xCH_CFG__PMx_SWAP_FLAG,str_name);

		str_name = "CFG.LBx_SWAP.FLAG";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,DSP__DISABLE_ENABLE,"");
		LINK__VAR_DIGITAL_CTRL(xCH_CFG__LBx_SWAP_FLAG,str_name);

		//
		{
			str_name = "CFG.PICK_COUNT.FROM_LBx";
			STD__ADD_ANALOG_WITH_COMMENT(str_name,"num",0,0,10000,"");
			LINK__VAR_ANALOG_CTRL(xCH_CFG__PICK_COUNT_FROM_LBx,str_name);

			str_name = "CFG.PICK_DELAY.TIME";
			STD__ADD_ANALOG_WITH_COMMENT(str_name,"sec",0,0,10000,"");
			LINK__VAR_ANALOG_CTRL(xCH_CFG__PICK_DELAY_TIME,str_name);

			xTIMER_CTRL__LLx_PICK_DELAY->REGISTER__COUNT_CHANNEL(sObject_Name,xCH_CFG__PICK_DELAY_TIME->Get__VARIABLE_NAME());
		}
	}

	//
	{
		str_name = "CHECK.MODULE";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__CHECK_MODULE,str_name);

		for(i=0;i<CFG_LP_LIMIT;i++)
		{
			str_name.Format("INFO%1d.MATERIAL_TO_LP", i+1);
			STD__ADD_STRING_WITH_COMMENT(str_name,"");
			LINK__VAR_STRING_CTRL(xCH__INFOx__MATERIAL_TO_LP[i],str_name);
		}
	}

	// ...
	{
		str_name = "VAC_RB.PAUSE.FLAG";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__VAC_RB_PAUSE_FLAG,str_name);

		str_name = "SCH.DB.CHECK";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__SCH_DB_CHECK,str_name);

		//
		str_name = "CFG.SCH.LLx_PREPMATER.USE";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,DSP__SCH_ENABLE_DISABLE,"");
		LINK__VAR_DIGITAL_CTRL(xCH_CFG__SCH_LLx_PREPMATER_USE, str_name);

		str_name = "CFG.SCH.LLx_COMPMATER.USE";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,DSP__SCH_ENABLE_DISABLE,"");
		LINK__VAR_DIGITAL_CTRL(xCH_CFG__SCH_LLx_COMPMATER_USE, str_name);
	}

	// ...
	{
		iPMx_IN_COUNT = 0;

		for(i=0; i<CFG_PM_LIMIT; i++)
		{
			str_name.Format("PM%1d.IN_COUNT", i+1);
			STD__ADD_STRING_WITH_COMMENT(str_name, "");
			LINK__VAR_STRING_CTRL(xCH__PMx__IN_COUNT[i], str_name);
		}
	}

	// REPORT : MATERIAL_USER_INFO ...
	{
		str_name = "REPORT.TRG_NAME_OF_ARM_A";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__REPORT_TRG_NAME_OF_ARM_A, str_name);

		str_name = "REPORT.TRG_NAME_OF_ARM_B";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__REPORT_TRG_NAME_OF_ARM_B, str_name);

		//
		str_name = "REPORT.TRG_SLOT_OF_ARM_A";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__REPORT_TRG_SLOT_OF_ARM_A, str_name);

		str_name = "REPORT.TRG_SLOT_OF_ARM_B";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__REPORT_TRG_SLOT_OF_ARM_B, str_name);

		//
		str_name = "REPORT.TRG_ANGLE_OF_ARM_A";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__REPORT_TRG_ANGLE_OF_ARM_A, str_name);

		str_name = "REPORT.TRG_ANGLE_OF_ARM_B";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__REPORT_TRG_ANGLE_OF_ARM_B, str_name);

		//
		str_name = "REPORT.GOTO_PM_OF_ARM_A";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__REPORT_GOTO_PM_OF_ARM_A, str_name);

		str_name = "REPORT.GOTO_PM_OF_ARM_B";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__REPORT_GOTO_PM_OF_ARM_B, str_name);
	}

	// ...
	{
		p_variable->Add__MONITORING_PROC(5.0,MON_ID__ALL_STATE);
	
		p_variable->Add__MONITORING_PROC(1.0,MON_ID__AUTO_MODE);
		p_variable->Add__MONITORING_PROC(1.0,MON_ID__PMx_CTRL);
	}
	return 1;
}

int CObj__DUAL_ARM_STD
::__DEFINE__ALARM(p_alarm)
{
	DECLARE__ALARM;

	// ...
	CString title;
	title.Format("%s : ",sObject_Name);

	int alarm_id;
	CString alarm_title;
	CString alarm_msg;
	CStringArray l_act;

	// ...
	{
		alarm_id = ALID__OBJECT_MODE_UNKNOWN;

		alarm_title  = title;
		alarm_title += "OBJECT MODE : UNKNOWN";

		alarm_msg = "";

		l_act.RemoveAll();
		l_act.Add("ABORT");

		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_title  = title;
		alarm_title += "JOB ERROR";

		l_act.RemoveAll();
		l_act.Add("CHECK");

		//
		alarm_msg = "VAC-Robot status is not standby !";
		ADD__ALARM_EX(ALID__JOB_ERROR__VAC_RB__NOT_STANDBY,alarm_title,alarm_msg,l_act);

		alarm_msg = "TMC Chamber status is not standby !";
		ADD__ALARM_EX(ALID__JOB_ERROR__TMC_CHM__NOT_STANDBY,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__CLEAN_ERROR__DUMMY_PORT__NOT_EXIST;

		alarm_title  = title;
		alarm_title += "CLEAN ERROR";

		alarm_msg  = "There is no dummy port in system.";
		alarm_msg += "Please, check config page or foup status.\n";

		l_act.RemoveAll();
		l_act.Add("CHECK");

		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__A_ARM__CONFIG_ERROR;

		alarm_title  = title;
		alarm_title += "VAC-Robot's A Arm : Config Error";

		alarm_msg = "Please, check config ctc scheduler page's \"vac-robot parameter\". \n";

		l_act.RemoveAll();
		l_act.Add("CHECK");

		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__B_ARM__CONFIG_ERROR;

		alarm_title  = title;
		alarm_title += "VAC-Robot's B Arm : Config Error";

		alarm_msg = "Please, check config ctc scheduler page's \"vac-robot parameter\". \n";

		l_act.RemoveAll();
		l_act.Add("CHECK");

		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__NO_ARM__CONFIG_ERROR;

		alarm_title  = title;
		alarm_title += "VAC-Robot's any arm is not used : Config Error !";

		alarm_msg = "Please, check config ctc scheduler page's \"vac-robot parameter\". \n";

		l_act.RemoveAll();
		l_act.Add("CHECK");

		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__PICK_ARM__CONFIG_ERROR;

		alarm_title  = title;
		alarm_title += "VAC-Robot's PICK ARM CONFIG ERROR !";

		alarm_msg = "Please, check config ctc scheduler page's \"vac-robot parameter\". \n";

		l_act.RemoveAll();
		l_act.Add("CHECK");

		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__JOB_ERROR__LLx__CHECK_VAC_PRESSURE;

		alarm_title  = title;
		alarm_title += "JOB ERROR";

		alarm_msg = "Please, check LLx's vacuum pressure.";

		l_act.RemoveAll();
		l_act.Add("CHECK");

		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__JOB_ERROR__PMx__CHECK_VAC_PRESSURE;

		alarm_title  = title;
		alarm_title += "JOB ERROR";

		alarm_msg = "";

		l_act.RemoveAll();
		l_act.Add("CHECK");

		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__MOVE_ERROR__WAFER_CONFIRM;

		alarm_title  = title;
		alarm_title += "Wafer Moving Error !";

		alarm_msg = "Wafer 위치를 확인 바랍니다. \n";

		l_act.RemoveAll();
		l_act.Add(STR__YES);
		l_act.Add(STR__ABORT);
		// l_act.Add(STR__NO);
		// l_act.Add(STR__IGNORE);

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
		alarm_id = ALID__VAC_ROBOT__NOT_STANDBY_ERROR;

		alarm_title  = title;
		alarm_title += "VAC_ROBOT STATUS ERROR";

		alarm_msg = "VAC_ROBOT's status is not standby !";

		l_act.RemoveAll();
		l_act.Add("CHECK");

		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VAC_TMC__NOT_STANDBY_ERROR;

		alarm_title  = title;
		alarm_title += "VAC_TMC STATUS ERROR";

		alarm_msg = "VAC_TMC's status is not standby !";

		l_act.RemoveAll();
		l_act.Add("CHECK");

		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__PMC__NOT_AVAILABLE;

		alarm_title  = title;
		alarm_title += "PMC STATUS ERROR";

		alarm_msg = "PMC is not available !";

		l_act.RemoveAll();
		l_act.Add("CHECK");

		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}
	// ...
	{
		alarm_id = ALID__LLx__NOT_AVAILABLE;

		alarm_title  = title;
		alarm_title += "LLx STATUS ERROR";

		alarm_msg = "LLx is not available !";

		l_act.RemoveAll();
		l_act.Add("CHECK");

		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__SWAP_ERROR;

		alarm_title  = title;
		alarm_title += "VAC-ROBOT - SWAP ERROR !";

		alarm_msg = "Job will be aborted !\n";

		l_act.RemoveAll();
		l_act.Add("ABORT");

		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__MANUAL_MOVE__ACTION_ERROR;

		alarm_title  = title;
		alarm_title += "VAC-Robot - Manual Move Error !";

		alarm_msg = "";

		l_act.RemoveAll();
		l_act.Add("CHECK");

		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__TOOL_CLEAR__ACTION_ERROR;

		alarm_title  = title;
		alarm_title += "VAC-Robot - Tool Clear Error !";

		alarm_msg = "";

		l_act.RemoveAll();
		l_act.Add("CHECK");

		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_title  = title;
		alarm_title += "VAC_Robot - Not Used Arm Error !";

		alarm_msg = "";

		l_act.RemoveAll();
		l_act.Add("CHECK");

		//
		alarm_id = ALID__VAC_ROBOT__A_ARM__NOT_USED;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);

		//
		alarm_id = ALID__VAC_ROBOT__B_ARM__NOT_USED;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__PMx_STATUS__IS_NOT_AVAILABLE;

		alarm_title  = title;
		alarm_title += "PMx Status is not available !";

		alarm_msg = "";

		l_act.RemoveAll();
		l_act.Add("CHECK");

		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__LLx_STATUS__IS_NOT_AVAILABLE;

		alarm_title  = title;
		alarm_title += "LLx Status is not available !";

		alarm_msg = "";

		l_act.RemoveAll();
		l_act.Add("CHECK");

		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VAC_ROBOT__MOVE_PAUSED;

		alarm_title  = title;
		alarm_title += "VAC Robot's moving is paused by user.";

		alarm_msg = "Please, check the \"VAC Robot - PAUSE\" button on the operate page. \n";

		l_act.RemoveAll();
		l_act.Add("CHECK");

		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__JOB_ERROR__MATERIAL_AND_PMC_PROC_TYPE_MISMATCH;

		alarm_title  = title;
		alarm_title += "JOB Error : Material and PMC's process type is not mismatched.";

		alarm_msg = "Please, check LPx's process type on config's page. \n";

		l_act.RemoveAll();
		l_act.Add("CHECK");

		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__MAINT_PICK_ERROR__VAC_RB_PAUSED;

		alarm_title  = title;
		alarm_title += "Maint Pick Error : Vacuum Robot Paused !";

		alarm_msg = "Please, check \"VAC Robot - PAUSE\"'s button on CTC operate page. \n";

		l_act.RemoveAll();
		l_act.Add(STR__ABORT);
		l_act.Add(STR__RETRY);

		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__MAINT_PLACE_ERROR__VAC_RB_PAUSED;

		alarm_title  = title;
		alarm_title += "Maint Place Error : Vacuum Robot Paused !";

		alarm_msg = "Please, check \"VAC Robot - PAUSE\"'s button on CTC operate page. \n";

		l_act.RemoveAll();
		l_act.Add(STR__ABORT);
		l_act.Add(STR__RETRY);

		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__VAC_ROBOT__DEADLOCK_CONDITION;

		alarm_title  = title;
		alarm_title += "Deadlock condition !";

		alarm_msg = "Please, check PM state or route recipe. \n";

		l_act.RemoveAll();
		l_act.Add(STR__CHECK);

		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}
	// ...
	{
		alarm_id = ALID__VAC_ROBOT__ABNORMAL_WAFER_MOVING;

		alarm_title  = title;
		alarm_title += "Abnormal wafer moving !";

		alarm_msg = "Please, check PM state or route recipe. \n";

		l_act.RemoveAll();
		l_act.Add(STR__CHECK);

		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}
	// ...
	{
		alarm_id = ALID__PMx_DEADLOCK_CONDITION;

		alarm_title  = title;
		alarm_title += "Deadlock condition !";

		alarm_msg = "Please, check PM state or route recipe. \n";

		l_act.RemoveAll();
		l_act.Add(STR__CHECK);

		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}
	// ...
	{
		alarm_id = ALID__PMx_ALIGN_MISMATCH_GOTO_PMx;

		alarm_title  = title;
		alarm_title += "Goto PMx : PM_Align Mismatch !";

		alarm_msg = "Please, check the current of PMx. \n";

		l_act.RemoveAll();
		l_act.Add(STR__CHECK);

		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	return 1;
}

int CObj__DUAL_ARM_STD
::__Define__USER_FUNCTION(CII_DEFINE__FUNCTION *p_fnc_ctrl)
{
	mERROR__USER_FNC.Set__OBJ_NAME(sObject_Name);
	p_fnc_ctrl->Link__ERROR_FEEDBACK(&mERROR__USER_FNC);

	// ...
	{

	}
	return 1;
}

//-------------------------------------------------------------------------
int CObj__DUAL_ARM_STD
::__INITIALIZE__OBJECT(p_variable,p_ext_obj_create)
{
	// ...
	{
		p_ext_obj_create->Link__DEF_VARIABLE__ERROR_FEEDBACK(&mERROR__DEF_VAR);

		iJOB_FLAG = -1;
	}

	// ...
	CString def_name;
	CString def_data;

	CString db_name;
	CString db_cfg_name;

	CString str_name;
	int i;

	// ...
	{
		// iLOG_CTRL_FLAG = -1;
		iLOG_CTRL_FLAG = 1;

		// ...
		CString file_name;
		CString log_msg;
		CString log_bff;

		//
		file_name.Format("%s.log", sObject_Name);

		//
		log_msg  = "\n\n";
		log_msg += "//------------------------------------------------------------------------";
		log_msg += "\n";

		log_bff.Format("// Log Ctrl Flag <- [%1d] \n", iLOG_CTRL_FLAG);
		log_msg += log_bff;

		log_msg += "\n";

		//
		xAPP_LOG_CTRL->CREATE__SUB_DIRECTORY(sObject_Name);
		xAPP_LOG_CTRL->SET__PROPERTY(file_name,24*5,60);

		xAPP_LOG_CTRL->DISABLE__TIME_LOG();
		xAPP_LOG_CTRL->WRITE__LOG(log_msg);

		xAPP_LOG_CTRL->ENABLE__TIME_LOG();
	}

	// OBJ DB ...
	{
		def_name = "DB_CFG_NAME";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, db_cfg_name);

		// JOB START READY ...
		{
			str_name = "CFG.JOB_START_READY.CTRL.MODE";
			LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__CFG_JOB_START_READY_CTRL_MODE, db_cfg_name,str_name);

			for(i=0; i<CFG_PM_LIMIT; i++)
			{
				str_name.Format("CFG.JOB_START_READY.PM%1d.USE", i+1);
				LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__CFG_JOB_START_READY_PMx_USE[i], db_cfg_name,str_name);
			}
		}

		// SYSTEM IDLE ...
		{
			// ...
			{
				str_name = "SYSTEM_IDLE.CFG.TIME.SEC";
				LINK__EXT_VAR_ANALOG_CTRL(aEXT_CH__SYSTEM_IDLE_CFG_TIME_SEC, db_cfg_name,str_name);

				str_name = "SYSTEM_IDLE.CUR.TIME.SEC";
				LINK__EXT_VAR_STRING_CTRL(sEXT_CH__SYSTEM_IDLE_CUR_TIME_SEC, db_cfg_name,str_name);
			}

			for(i=0; i<CFG_PM_LIMIT; i++)
			{
				str_name.Format("SYSTEM_IDLE.CFG.PM%1d.USE", i+1);
				LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__SYSTEM_IDLE_CFG_PMx_USE[i], db_cfg_name,str_name);

				str_name.Format("SYSTEM_IDLE.CFG.PM%1d.SEC", i+1);
				LINK__EXT_VAR_ANALOG_CTRL(aEXT_CH__SYSTEM_IDLE_CFG_PMx_SEC[i], db_cfg_name,str_name);
			}
		}

		// PRE_RCP ...
		{
			for(i=0; i<CFG_PM_LIMIT; i++)
			{
				str_name.Format("PRE_RCP.CFG.PM%1d.USE", i+1);
				LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__PRE_RCP_CFG_PMx_USE[i], db_cfg_name,str_name);

				str_name.Format("PRE_RCP.CFG.PM%1d.WAFER_COUNT", i+1);
				LINK__EXT_VAR_ANALOG_CTRL(aEXT_CH__PRE_RCP_CFG_PMx_WAFER_COUNT[i], db_cfg_name,str_name);
			}
		}
		// POST_RCP ...
		{
			for(i=0; i<CFG_PM_LIMIT; i++)
			{
				str_name.Format("POST_RCP.CFG.PM%1d.USE", i+1);
				LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__POST_RCP_CFG_PMx_USE[i], db_cfg_name,str_name);

				str_name.Format("POST_RCP.CFG.PM%1d.WAFER_COUNT", i+1);
				LINK__EXT_VAR_ANALOG_CTRL(aEXT_CH__POST_RCP_CFG_PMx_WAFER_COUNT[i], db_cfg_name,str_name);
			}
		}

		// TRANSFER ...
		{
			str_name = "TRANSFER.MODE";
			LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__CFG__TRANSFER_MODE, db_cfg_name,str_name);

			str_name = "REF.ATM.PRESSURE";
			LINK__EXT_VAR_ANALOG_CTRL(xEXT_CH__CFG__REF_ATM_PRESSURE, db_cfg_name,str_name);

			str_name = "REF.VAC.PRESSURE";
			LINK__EXT_VAR_ANALOG_CTRL(xEXT_CH__CFG__REF_VAC_PRESSURE, db_cfg_name,str_name);
		}
		
		// SCHEDULER ...
		{
			str_name = "SCH.CONCURRENT.ALWAYS.APPLY";
			LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__SCH_CONCURRENT_ALWAYS_APPLY, db_cfg_name,str_name);
		}
	
		// SCH.MODE.XXX ...
		{
			str_name = "SCH.MODE.LLx_IN";
			LINK__EXT_VAR_STRING_CTRL(sEXT_CH__SCH_MODE_LLx_IN, db_cfg_name,str_name);

			str_name = "SCH.MODE.LLx_OUT";
			LINK__EXT_VAR_STRING_CTRL(sEXT_CH__SCH_MODE_LLx_OUT, db_cfg_name,str_name);
		}
	
		// CFG_DB ...
		{
			for(i=0; i<CFG_PM_LIMIT; i++)
			{
				str_name.Format("PM%1d.CUR.PRE_COUNT", i+1);
				LINK__EXT_VAR_STRING_CTRL(sEXT_CH__PMx__CUR_PRE_COUNT[i], db_cfg_name,str_name);

				str_name.Format("PM%1d.CUR.POST_COUNT", i+1);
				LINK__EXT_VAR_STRING_CTRL(sEXT_CH__PMx__CUR_POST_COUNT[i], db_cfg_name,str_name);

				str_name.Format("PM%1d.CUR.MAIN_COUNT", i+1);
				LINK__EXT_VAR_STRING_CTRL(sEXT_CH__PMx__CUR_MAIN_COUNT[i], db_cfg_name,str_name);
			}
		}
		// SCH_DB : LLx ...
		{
			int ll_limit = CFG_LLx_LIMIT;
			int ll_i;

			for(ll_i=0; ll_i<ll_limit; ll_i++)
			{
				iLLx_SLOT_MAX[ll_i] = CFG_LLx__SLOT_MAXx;

				// ...
				CString ll_name = Get__LLx_NAME(ll_i);

				// ...
				str_name.Format("%s.CFG.PREPMATER.USE", ll_name);
				LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__LLx__CFG_PREPMATER_USE[ll_i], db_cfg_name,str_name);

				str_name.Format("%s.CFG.COMPMATER.USE", ll_name);
				LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__LLx__CFG_COMPMATER_USE[ll_i], db_cfg_name,str_name);

				// PRE_RCP ...
				str_name.Format("%s.PRE_RCP.CFG.USE", ll_name);
				LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__LLx__CFG_PRE_RCP_USE[ll_i], db_cfg_name,str_name);

				str_name.Format("%s.PRE_RCP.CFG.WAFER_COUNT", ll_name);
				LINK__EXT_VAR_ANALOG_CTRL(xEXT_CH__LLx__CFG_PRE_RCP_WAFER_COUNT[ll_i], db_cfg_name,str_name);

				// POST_RCP ...
				str_name.Format("%s.POST_RCP.CFG.USE", ll_name);
				LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__LLx__CFG_POST_RCP_USE[ll_i], db_cfg_name,str_name);

				str_name.Format("%s.POST_RCP.CFG.WAFER_COUNT", ll_name);
				LINK__EXT_VAR_ANALOG_CTRL(xEXT_CH__LLx__CFG_POST_RCP_WAFER_COUNT[ll_i], db_cfg_name,str_name);

				// ...
				str_name.Format("SCH_DB.%s.USE.FLAG", ll_name);
				LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__SCH_DB_LLx_USE_FLAG[ll_i], db_cfg_name,str_name);

				str_name.Format("SCH_DB.%s.SLOT_MAX", ll_name);
				LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__SCH_DB_LLx_SLOT_MAX[ll_i], db_cfg_name,str_name);

				str_name.Format("SCH_DB.%s.MODE_TYPE", ll_name);
				LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_i], db_cfg_name,str_name);

				//
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

		// LLx ...
		for(i=0; i<CFG_LLx_LIMIT; i++)
		{
			str_name.Format("SCH.STS.ATM_RB_TO_LL%1d", i+1);
			LINK__EXT_VAR_STRING_CTRL(xEXT_CH__ATM_RB__SCH_STS_TO_LLx[i], db_cfg_name,str_name);
		}

		// PMx ...
		for(i=0; i<CFG_PM_LIMIT; i++)
		{
			int pm_id = i + 1;

			//
			str_name.Format("PM%1d.USE",pm_id);
			LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__CFG__PMx_USE[i], db_cfg_name,str_name);

			str_name.Format("PM%1d.STAGE1.USE",pm_id);
			LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__CFG__PMx_LEFT_STAGE_USE[i], db_cfg_name,str_name);

			str_name.Format("PM%1d.STAGE2.USE",pm_id);
			LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__CFG__PMx_RIGHT_STAGE_USE[i], db_cfg_name,str_name);

			//
			str_name.Format("PM%1d.LEAK_CHECK.FLAG",pm_id);
			LINK__EXT_VAR_STRING_CTRL(xEXT_CH__PMx_LEAK_CHECK_FLAG[i], db_cfg_name,str_name);

			str_name.Format("PM%1d.AUTO_PM.FLAG",pm_id);
			LINK__EXT_VAR_STRING_CTRL(xEXT_CH__PMx_AUTO_PM_FLAG[i], db_cfg_name,str_name);

			str_name.Format("PM%1d.PROCESS.TYPE",pm_id);
			LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__PMx_PROCESS_TYPE[i], db_cfg_name,str_name);
		}

		// ...
		{
			str_name = "VAC_AREA.MATERIAL.COUNT";
			LINK__EXT_VAR_STRING_CTRL(xEXT_CH__VAC_AREA_MATERIAL_COUNT, db_cfg_name,str_name);

			str_name = "VAC_ROBOT.OUT_MATERIAL.COUNT";
			LINK__EXT_VAR_STRING_CTRL(xEXT_CH__VAC_ROBOT_OUT_MATERIAL_COUNT, db_cfg_name,str_name);

			str_name = "PMC.MATERIAL.COUNT";
			LINK__EXT_VAR_STRING_CTRL(xEXT_CH__PMC_MATERIAL_COUNT, db_cfg_name,str_name);

			str_name = "TOTAL.MATERIAL.COUNT.IN.SYSTEM";
			LINK__EXT_VAR_STRING_CTRL(xEXT_CH__TOTAL_MATERIAL_COUNT_IN_SYSTEM, db_cfg_name,str_name);

			//
			str_name = "SYSTEM.LEAK_CHECK.REQ";
			LINK__EXT_VAR_STRING_CTRL(xEXT_CH__SYSTEM_LEAK_CHECK_REQ, db_cfg_name,str_name);

			str_name = "SYSTEM.AUTO_PM.REQ";
			LINK__EXT_VAR_STRING_CTRL(xEXT_CH__SYSTEM_AUTO_PM_REQ, db_cfg_name,str_name);
		}

		// ...
		{
			str_name = "PRESSURE_CHECK.MODE";
			LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__CFG__PRESSURE_CHECK_MODE, db_cfg_name,str_name);

			str_name = "TOOL_CLEAR.PMC_MATERIAL.MODE";
			LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__CFG__TOOL_CLEAR_PMC_MATERIAL_MODE, db_cfg_name,str_name);
		}

		// WAC.PMx ...
		{
			// ...
			{
				str_name = "SCH.WAC.PMC.WAFER.TYPE.TO_TMC";
				LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__SCH__WAC_PMC_WAFER_TYPE, db_cfg_name,str_name);
			}

			for(i=0; i<CFG_PM_LIMIT; i++)
			{
				int id = i + 1;

				str_name.Format("SCH.WAC.PM%1d.USE.TO_TMC", id);
				LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__SCH__WAC_PM_USE_X[i], db_cfg_name,str_name);

				str_name.Format("SCH.WAC.PM%1d.PICK.CHECK.TO_TMC", id);
				LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__SCH__WAC_PM_PICK_CHECK_X[i], db_cfg_name,str_name);

				str_name.Format("SCH.WAC.PM%1d.PLACE.CHECK.TO_TMC", id);
				LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__SCH__WAC_PM_PLACE_CHECK_X[i], db_cfg_name,str_name);

				str_name.Format("SCH.WAC.PM%1d.PRE_FRONT", id);
				LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__SCH__WAC_PM_PRE_FRONT_X[i], db_cfg_name,str_name);

				//
				str_name.Format("SCH.WAC.PM%1d.POS.ID.TO_TMC", id);
				LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__SCH__WAC_PM_POS_ID_X[i], db_cfg_name,str_name);

				str_name.Format("SCH.WAC.PM%1d.POS.SLOT.TO_TMC", id);
				LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__SCH__WAC_PM_POS_SLOT_X[i], db_cfg_name,str_name);

				str_name.Format("SCH.WAC.PM%1d.DELAY.SEC.TO_TMC", id);
				LINK__EXT_VAR_ANALOG_CTRL(aEXT_CH__SCH__WAC_PM_DELAY_SEC_X[i], db_cfg_name,str_name);
			}
		}

		// JOB.LPx ...
		for(i=0; i<CFG_LP_LIMIT; i++)
		{
			int id = i + 1;

			str_name.Format("SCH.LP_JOB_LIST.%1d", id);
			LINK__EXT_VAR_STRING_CTRL(xEXT_CH__SCH__LP_JOB_LIST[i], db_cfg_name,str_name);
		}
	}

	// JOB CTRL ...
	{
		def_name = "OBJ__JOB_CTRL";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name,db_name);

		str_name = "JOB.CTRL";
		LINK__EXT_VAR_STRING_CTRL(xEXT_CH__JOB_CTRL, db_name,str_name);
	}

	// OBJ : VAC_ROBOT ...
	{
		def_name = "OBJ__VAC_ROBOT";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name,db_name);

		//
		pVAC_RB__OBJ_CTRL = p_ext_obj_create->Create__OBJECT_CTRL(db_name);

		LINK__EXT_VAR_DIGITAL_CTRL(xCH__VAC_RB__OBJ_CTRL,    db_name, "CTRL");
		LINK__EXT_VAR_DIGITAL_CTRL(xCH__VAC_RB__OBJ_MODE,    db_name, "MODE");
		LINK__EXT_VAR_DIGITAL_CTRL(xCH__VAC_RB__OBJ_STATUS,  db_name, "OBJ.STATUS");

		LINK__EXT_VAR_DIGITAL_CTRL(xCH__VAC_RB__SLOT01_STATUS, db_name, "ARM.SLOT01.STATUS");
		LINK__EXT_VAR_DIGITAL_CTRL(xCH__VAC_RB__SLOT02_STATUS, db_name, "ARM.SLOT02.STATUS");
		LINK__EXT_VAR_STRING_CTRL( xCH__VAC_RB__SLOT01_TITLE,  db_name, "ARM.SLOT01.TITLE");
		LINK__EXT_VAR_STRING_CTRL( xCH__VAC_RB__SLOT02_TITLE,  db_name, "ARM.SLOT02.TITLE");

		LINK__EXT_VAR_DIGITAL_CTRL(xCH__VAC_RB__ROTE_ARM, db_name, "ANI.ROTE.ARM");
		LINK__EXT_VAR_DIGITAL_CTRL(xCH__VAC_RB__TRG_ROTE, db_name, "ANI.TRG.ROTE");

		LINK__EXT_VAR_DIGITAL_CTRL(xCH__VAC_RB__CFG_A_ARM_USE_MODE, db_name,"CFG.A.ARM.USE.FLAG");
		LINK__EXT_VAR_DIGITAL_CTRL(xCH__VAC_RB__CFG_B_ARM_USE_MODE, db_name,"CFG.B.ARM.USE.FLAG");

		//
		for(i=0; i<CFG_PM_LIMIT; i++)
		{
			int id = i + 1;

			str_name.Format("INFO.PM%1d.RESERVE", id);
			LINK__EXT_VAR_DIGITAL_CTRL(dCH__VAC_RB__INFO_PM_RESERVE_X[i], db_name,str_name);
		}

		// LLx : Scheduler - Dual Only Input & Output ...
		str_name = "CFG.LLx_CTRL.ONLY_INPUT_OUTPUT.MODE";
		LINK__EXT_VAR_DIGITAL_CTRL(dCH__VAC_RB__CFG_LLx_CTRL_ONLY_INPUT_OUTPUT_MODE, db_name,str_name);

		str_name = "CFG.DUAL_ARM_MOVING_AT_THE_SAME_TIME";
		LINK__EXT_VAR_DIGITAL_CTRL(dCH__VAC_RB__CFG_DUAL_ARM_MOVING_AT_THE_SAME_TIME, db_name,str_name);

		// ARM-CONSTAINT ...
		// A ARM
		str_name = "CFG.A_ARM.CONSTRAINT.LL";
		LINK__EXT_VAR_DIGITAL_CTRL(dCH__VAC_RB__CFG_A_ARM_CONSTRAINT_LL, db_name,str_name);

		str_name = "CFG.A_ARM.CONSTRAINT.PM";
		LINK__EXT_VAR_DIGITAL_CTRL(dCH__VAC_RB__CFG_A_ARM_CONSTRAINT_PM, db_name,str_name);

		// B ARM
		str_name = "CFG.B_ARM.CONSTRAINT.LL";
		LINK__EXT_VAR_DIGITAL_CTRL(dCH__VAC_RB__CFG_B_ARM_CONSTRAINT_LL, db_name,str_name);

		str_name = "CFG.B_ARM.CONSTRAINT.PM";
		LINK__EXT_VAR_DIGITAL_CTRL(dCH__VAC_RB__CFG_B_ARM_CONSTRAINT_PM, db_name,str_name);

		// PMx -> PMx
		str_name = "CFG.PMx_To_PMx.CONSTRAINT";
		LINK__EXT_VAR_DIGITAL_CTRL(dCH__VAC_RB__CFG_PMx_To_PMx_CONSTRAINT, db_name,str_name);

		str_name = "CFG.PMx_To_PMx.MOVE_PATH";
		LINK__EXT_VAR_DIGITAL_CTRL(dCH__VAC_RB__CFG_PMx_To_PMx_MOVE_PATH, db_name,str_name);

		str_name = "CFG.PMx_To_PMx.PICK_CONDITION";
		LINK__EXT_VAR_DIGITAL_CTRL(dCH__VAC_RB__CFG_PMx_To_PMx_PICK_CONDITION, db_name,str_name);

		// CFG : WAFER PICK PARAMETER ...
		str_name = "CFG.PICK_WAFER_CONDITION";
		LINK__EXT_VAR_DIGITAL_CTRL(dCH__VAC_RB__CFG_PICK_WAFER_CONDITION, db_name,str_name);
	}

	// OBJ : TMC_CHM ..
	{
		def_name = "OBJ__TMC_CHM";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name,db_name);

		//
		pTMC_CHM__OBJ_CTRL = p_ext_obj_create->Create__OBJECT_CTRL(db_name);

		LINK__EXT_VAR_DIGITAL_CTRL(xCH__TMC_CHM__OBJ_CTRL,    db_name, "CTRL");
		LINK__EXT_VAR_DIGITAL_CTRL(xCH__TMC_CHM__OBJ_MODE,    db_name, "MODE");
		LINK__EXT_VAR_DIGITAL_CTRL(xCH__TMC_CHM__OBJ_STATUS,  db_name, "OBJ.STATUS");

		LINK__EXT_VAR_DIGITAL_CTRL(xCH__TMC_CHM__PRESSURE_STATUS, db_name, "PRESSURE.STATUS");
	}

	// OBJ : LLx ...
	{
		def_name = "LLx_SIZE";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);

		iLLx_LIMIT = atoi(def_data);
		if(iLLx_LIMIT > CFG_LLx_LIMIT)			iLLx_LIMIT = CFG_LLx_LIMIT;

		// ...
		int ll_limit = iLLx_LIMIT;
		int ll_i;

		for(ll_i=0; ll_i<ll_limit; ll_i++)
		{
			def_name.Format("OBJ__LL%1d", ll_i+1);
			p_ext_obj_create->Get__DEF_CONST_DATA(def_name, db_name);
			
			pLLx__OBJ_CTRL[ll_i] = p_ext_obj_create->Create__OBJECT_CTRL(db_name);

			//
			str_name = "PARA.VENT_COOLING_SEC";
			LINK__EXT_VAR_ANALOG_CTRL(aEXT_CH__LLx__PARA_VENT_COOLING_SEC[ll_i], db_name,str_name);

			//
			str_name = "NEXT.CTRL_REQ";
			LINK__EXT_VAR_STRING_CTRL(sEXT_CH__LLx__NEXT_CTRL_REQ[ll_i], db_name,str_name);

			//
			str_name = "CTRL";
			LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__LLx__OBJ_CTRL[ll_i], db_name,str_name);
			
			str_name = "MODE";
			LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__LLx__OBJ_MODE[ll_i], db_name,str_name);

			str_name = "OBJ.STATUS";
			LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__LLx__OBJ_STATUS[ll_i], db_name,str_name);

			str_name = "UPPER_OBJ.MSG";
			LINK__EXT_VAR_STRING_CTRL(sEXT_CH__LLx__UPPER_OBJ_MSG[ll_i], db_name,str_name);

			//
			str_name = "ACTIVE.VAC_AREA.CTRL";
			LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__LLx__ACTIVE_VAC_AREA_CTRL[ll_i], db_name,str_name);

			str_name = "ACTIVE.ATM_AREA.CTRL";
			LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__LLx__ACTIVE_ATM_AREA_CTRL[ll_i], db_name,str_name);

			//
			str_name = "SCH.IN_OUT.FLAG";
			LINK__EXT_VAR_STRING_CTRL(xEXT_CH__LLx__IN_OUT_FLAG[ll_i], db_name,str_name);

			str_name = "SCH.IN_COUNT";
			LINK__EXT_VAR_STRING_CTRL(xEXT_CH__LLx__IN_COUNT[ll_i], db_name,str_name);

			str_name = "SCH.OUT_COUNT";
			LINK__EXT_VAR_STRING_CTRL(xEXT_CH__LLx__OUT_COUNT[ll_i], db_name,str_name);

			str_name = "SCH.IDLE_COUNT";
			LINK__EXT_VAR_STRING_CTRL(xEXT_CH__LLx__IDLE_COUNT[ll_i], db_name,str_name);

			str_name = "PRESSURE.STATUS";
			LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__LLx__PRESSURE_STATUS[ll_i], db_name,str_name);

			str_name = "PRESSURE.VALUE";
			LINK__EXT_VAR_ANALOG_CTRL(xEXT_CH__LLx__PRESSURE_VALUE[ll_i], db_name,str_name);

			//
			int slot_max = iLLx_SLOT_MAX[ll_i];

			for(i=0; i<slot_max; i++)
			{
				str_name.Format("SLOT%002d.STATUS",i+1);
				LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__LLx__SLOT_STATUS[ll_i][i], db_name,str_name);

				str_name.Format("SLOT%002d.TITLE",i+1);
				LINK__EXT_VAR_STRING_CTRL(xEXT_CH__LLx__SLOT_TITLE[ll_i][i], db_name,str_name);
			}

			//
			str_name = "VENTING.FLAG";
			LINK__EXT_VAR_STRING_CTRL(xEXT_CH__LLx__VENTING_FLAG[ll_i], db_name,str_name);

			str_name = "VENTING.COMP.FLAG";
			LINK__EXT_VAR_STRING_CTRL(xEXT_CH__LLx__VENTING_COMP_FLAG[ll_i], db_name,str_name);

			//
			str_name = "PARA.PROC.RECIPE.NAME";
			LINK__EXT_VAR_STRING_CTRL(sEXT_CH__LLx__PARA_PROC_RECIPE_NAME[ll_i], db_name,str_name);

			str_name = "PARA.PROC.SCH.NAME";
			LINK__EXT_VAR_STRING_CTRL(sEXT_CH__LLx__PARA_PROC_SCH_NAME[ll_i], db_name,str_name);
		}
	}

	// OBJ : PMx
	{
		def_name = "PM_SIZE";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name,db_name);

		int pm_size = atoi(db_name);
		if(pm_size > CFG_PM_LIMIT)		pm_size = CFG_PM_LIMIT;
		iPMx_SIZE = pm_size;

		for(i=0;i<pm_size;i++)
		{
			def_name.Format("OBJ__PM%1d",i+1);
			if(p_ext_obj_create->Get__DEF_CONST_DATA(def_name,db_name) < 0)		continue;

			//
			pPMx__OBJ_CTRL[i] = p_ext_obj_create->Create__OBJECT_CTRL(db_name);

			//
			LINK__EXT_VAR_DIGITAL_CTRL(xCH__PMx__OBJ_CTRL[i],          db_name, "CTRL");
			LINK__EXT_VAR_DIGITAL_CTRL(xCH__PMx__OBJ_MODE[i],          db_name, "MODE");
			LINK__EXT_VAR_DIGITAL_CTRL(xCH__PMx__OBJ_STATUS[i],        db_name, "OBJ.STATUS");
			LINK__EXT_VAR_STRING_CTRL(xCH__PMx__OBJ_VIRTUAL_STATUS[i], db_name, "OBJ.VIRTUAL.STATUS");

			LINK__EXT_VAR_STRING_CTRL(xCH__PMC_PROC_READY_FLAG[i],db_name, "PMC.PROC_READY.FLAG");

			LINK__EXT_VAR_STRING_CTRL(xCH__PMx__X_PICK_FLAG[i],  db_name, "X_PICK.FLAG");
			LINK__EXT_VAR_STRING_CTRL(xCH__PMx__X_PLACE_FLAG[i], db_name, "X_PLACE.FLAG");

			LINK__EXT_VAR_DIGITAL_CTRL(xCH__PMx__PRESSURE_STATUS[i], db_name, "PRESSURE.STATUS");
			LINK__EXT_VAR_ANALOG_CTRL(xCH__PMx__PRESSURE_VALUE[i],   db_name, "PRESSURE.VALUE");

			LINK__EXT_VAR_STRING_CTRL(xCH__PMx__PRC_PRE_RECIPE[i], db_name, "PRC.PRE.RECIPE");

			LINK__EXT_VAR_STRING_CTRL(xCH__PMx__POST_CLEAN_RECIPE[i], db_name, "POST.CLEAN.RECIPE" );
			LINK__EXT_VAR_STRING_CTRL(xCH__PMx__PROCESS_RECIPE[i],    db_name, "RECIPE" );
			LINK__EXT_VAR_STRING_CTRL(xCH__PMx__PROCESS_STATUS[i],    db_name, "PROCESS.STATUS" );

			LINK__EXT_VAR_DIGITAL_CTRL(xCH__PMx__SLOT_STATUS[i], db_name, "SLOT01.STATUS");
			LINK__EXT_VAR_STRING_CTRL( xCH__PMx__SLOT_TITLE[i],  db_name, "SLOT01.TITLE" );

			//
			LINK__EXT_VAR_STRING_CTRL(xCH__PMx__NEXT_PROCESS[i],              db_name, "NEXT.PROCESS");
			LINK__EXT_VAR_DIGITAL_CTRL(xCH__PMx__AUTO_MATERIAL_COUNT_FLAG[i], db_name, "CFG.AUTO.MATERIAL_COUNT.FLAG");

			//
			str_name = "CFG.AUTO.LEAK_CHECK.MODE";
			LINK__EXT_VAR_DIGITAL_CTRL(xCH__PMx__CFG_AUTO_LEAK_CHECK_MODE[i], db_name,str_name);

			str_name = "CFG.AUTO_PM.MODE";
			LINK__EXT_VAR_DIGITAL_CTRL(xCH__PMx__CFG_AUTO_PM_MODE[i], db_name,str_name);

			//
			LINK__EXT_VAR_STRING_CTRL(xCH__PMx__PROC_INFO__PROCESS_STATUS[i], db_name,"PROC_INFO.PROCESS_STATUS");

			//
			LINK__EXT_VAR_STRING_CTRL(xCH__PMx__FIRST_WAFER_FLAG[i], db_name,"PMC.FIRST.WAFER.FLAG");
			LINK__EXT_VAR_STRING_CTRL(xCH__PMx__IDLE_TIME[i],        db_name,"PMC.IDLE.TIME");
			LINK__EXT_VAR_STRING_CTRL(xCH__PMx__PROCESS_PERCENT[i],  db_name,"PROCESS.PERCENT");
			LINK__EXT_VAR_STRING_CTRL(xCH__PMx__CURRENT_PRC_TIME[i], db_name,"CURRENT.PRC_TIME");

			// ...
			str_name = "PRE_INFO.JOB_START_READY.PORTID";
			LINK__EXT_VAR_STRING_CTRL(sEXT_CH__PMx_PRE_INFO_JOB_START_READY_PORTID[i], db_name,str_name);

			str_name = "PRE_INFO.JOB_START_READY.JOBID";
			LINK__EXT_VAR_STRING_CTRL(sEXT_CH__PMx_PRE_INFO_JOB_START_READY_JOBID[i], db_name,str_name);

			//
			str_name = "REQ_INFO.JOB_START_READY.PORTID";
			LINK__EXT_VAR_STRING_CTRL(sEXT_CH__PMx_REQ_INFO_JOB_START_READY_PORTID[i], db_name,str_name);

			str_name = "REQ_INFO.JOB_START_READY.JOBID";
			LINK__EXT_VAR_STRING_CTRL(sEXT_CH__PMx_REQ_INFO_JOB_START_READY_JOBID[i], db_name,str_name);
		}
	}

	// OBJ : LP1 ~ LP5 ...
	{
		def_name = "LP_SIZE";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name,db_name);

		int lp_size = atoi(db_name);
		if(lp_size > CFG_LP_LIMIT)		lp_size = CFG_LP_LIMIT;
		iLPx_SIZE = lp_size;

		for(i=0;i<lp_size;i++)
		{
			str_name.Format("OBJ__LP%1d",i+1);
			
			if(p_ext_obj_create->Get__DEF_CONST_DATA(str_name,db_name) < 0)
			{
				continue;
			}

			//
			LINK__EXT_VAR_DIGITAL_CTRL(xCH__LPx__OBJ_CTRL[i],   db_name,"CTRL");
			LINK__EXT_VAR_DIGITAL_CTRL(xCH__LPx__OBJ_MODE[i],   db_name,"MODE");
			LINK__EXT_VAR_DIGITAL_CTRL(xCH__LPx__OBJ_STATUS[i], db_name,"OBJ.STATUS");

			//
			LINK__EXT_VAR_DIGITAL_CTRL(xCH__LPx__CFG_USE[i],          db_name,"CFG.USE");
			LINK__EXT_VAR_DIGITAL_CTRL(xCH__LPx__CST_STATUS[i],       db_name,"CST.STATUS");
			LINK__EXT_VAR_DIGITAL_CTRL(xCH__LPx__CFG_PROCESS_TYPE[i], db_name,"CFG.PROCESS.TYPE");
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
int CObj__DUAL_ARM_STD
::__CALL__CONTROL_MODE(mode,p_debug,p_variable,p_alarm)
{
	int flag = -1;

	// ...
	{
		CString log_msg;
		log_msg.Format("[%s] started ...", mode);

		Fnc__LOG_CTRL(log_msg);
	}

	try
	{
			 IF__CTRL_MODE(sMODE__MAINT_PICK)					flag = Call__MAINT_PICK(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__MAINT_PLACE)					flag = Call__MAINT_PLACE(p_variable,p_alarm);
		
		ELSE_IF__CTRL_MODE(sMODE__CLEAR)						flag = Call__CLEAR(p_variable,p_alarm);
		
		ELSE_IF__CTRL_MODE(sMODE__CHECK_MATERIAL)				flag = Call__CHECK_MATERIAL(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__CHECK_STANDBY)				flag = Call__CHECK_STANDBY(p_variable,p_alarm);
	
		ELSE_IF__CTRL_MODE(sMODE__WAFER_RESYNC_CLEAR_SYS)		flag = Call__WAFER_RESYNC_CLEAR_SYS();
		ELSE_IF__CTRL_MODE(sMODE__WAFER_RESYNC_CLEAR_PMx)		flag = Call__WAFER_RESYNC_CLEAR_PMx();
		ELSE_IF__CTRL_MODE(sMODE__WAFER_RESYNC_CLEAR_LLx)		flag = Call__WAFER_RESYNC_CLEAR_LLx();

		ELSE_IF__CTRL_MODE(sMODE__SCH_DB_CHECK)					flag = Call__SCH_DB_CHECK(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__USER_INFO_CHECK)				flag = Call__USER_INFO_CHECK(p_variable);

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

		Fnc__LOG_CTRL(log_msg);
	}

	// ...
	{
		Set__TMC_CtcInUse_To_Standby();
		Set__PMx_CtcInUse_To_Standby();
	}

	return flag;
}
int CObj__DUAL_ARM_STD
::__CALL__MONITORING(id,p_variable,p_alarm)
{
	switch(id)
	{
		case MON_ID__ALL_STATE:
			Mon__ALL_STATE(p_variable,p_alarm);
			break;

		case MON_ID__AUTO_MODE:
			Mon__AUTO_CTRL(p_variable,p_alarm);
			break;

		case MON_ID__PMx_CTRL:
			Mon__PMx_CTRL(p_variable);
			break;
	}

	return 1;
}
