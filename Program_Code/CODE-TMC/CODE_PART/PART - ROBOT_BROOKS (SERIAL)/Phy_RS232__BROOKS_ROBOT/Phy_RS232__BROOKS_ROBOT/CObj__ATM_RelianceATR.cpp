#include "StdAfx.h"
#include "CObj__ATM_RelianceATR.h"
#include "CObj__ATM_RelianceATR__DEF.h"
#include "CObj__ATM_RelianceATR__ALID.h"

#include "CCommon_DEF.h"
#include "Macro_Function.h"


//--------------------------------------------------------------------------------
CObj__ATM_RelianceATR::CObj__ATM_RelianceATR()
{
	m_sErrorID  =   "0";
	m_sStationA =	"1";
	m_sStationB =	"1";
	m_sRPos     =	"RE"; 
	m_sZPos		=	"DN"; 
	m_sSlot		=	"1"; 

	m_nCommState = ONLINE;
}
CObj__ATM_RelianceATR::~CObj__ATM_RelianceATR()
{

}

//--------------------------------------------------------------------------------
int CObj__ATM_RelianceATR::__DEFINE__CONTROL_MODE(obj,l_mode)
{
	sObject_Name = obj;

	// ...
	{
		ADD__CTRL_VAR(sMODE__INIT, "INIT");							// OK !!

		ADD__CTRL_VAR(sMODE__PICK,	"PICK");						// OK !!
		ADD__CTRL_VAR(sMODE__PLACE,	"PLACE");						// OK !!

		ADD__CTRL_VAR(sMODE__ROTATE, "ROTATE");						// OK !!
		
		ADD__CTRL_VAR(sMODE__VAC_ON,  "VACON");						// OK !!
		ADD__CTRL_VAR(sMODE__VAC_OFF, "VACOFF");					// OK !!
		
		ADD__CTRL_VAR(sMODE__PLACE_ALIGN_PICK, "PLACE_PICK");		// OK !!  :  ALIGNER에 PLACE+EXTEND+ALIGN+PICK
		ADD__CTRL_VAR(sMODE__AL_ALIGN_PICK,	   "ALGNPICK");
		ADD__CTRL_VAR(sMODE__ALIGN_PLACE,      "ALGNPLACE");
		ADD__CTRL_VAR(sMODE__AL_ALIGN,		   "ALGN");
		ADD__CTRL_VAR(sMODE__AL_ALIGN_RESULT,  "ALIGN_RESULT");
		ADD__CTRL_VAR(sMODE__AL_SCAN,		   "AL_SCAN");
		ADD__CTRL_VAR(sMODE__AL_HOME,		   "AL_HOME");
		ADD__CTRL_VAR(sMODE__AL_RESET,		   "AL_RESET");
		ADD__CTRL_VAR(sMODE__AL_ALIGN_INIT,	   "ALIGN_INIT");	

		ADD__CTRL_VAR(sMODE__MAP,	  "MAP");
		ADD__CTRL_VAR(sMODE__EXTEND,  "EXTEND");
		ADD__CTRL_VAR(sMODE__RETRACT, "RETRACT");
		ADD__CTRL_VAR(sMODE__GOUP,	  "GOUP");
		ADD__CTRL_VAR(sMODE__GODOWN,  "GODOWN");

		ADD__CTRL_VAR(sMODE__HOME,		  "HOME");
		ADD__CTRL_VAR(sMODE__VACOFF_HOME, "VAC_OFF.HOME");

		ADD__CTRL_VAR(sMODE__LP_MAP,  "LP_MAP");
	}
	return 1;
}
int CObj__ATM_RelianceATR::__DEFINE__VERSION_HISTORY(version)
{
	version = "JG.Lee \n";

	return 1;
}


// ...
#define  MON_ID__IO_MONITOR									1
#define  MON_ID__HYPER_TERMINAL								2

// ...
#define APP_DSP__RB_TARGET_MOVE								\
"UNKNOWN HOME READY											\
LBA LBB														\
AL1															\
LP1 LP2 LP3 LP4 LP5											\
BUFF1 BUFF2"

#define APP_DSP__STN_NAME									\
"LP1 LP2 LP3 LP4 LP5										\
LBA LBB														\
AL1															\
BUFF1 BUFF2"

#define APP_DSP__ARM_TYPE									\
"A B"

#define APP_DSP__RET_EXT									\
"Retract Extend"

#define APP_DSP__DOWN_UP									\
"Down Up"

#define APP_DSP__ARM_STS_ANI								\
"UNKNOWN RETRACT EXTEND"

#define APP_DSP__LP_WFR_SLOT_STS							\
"UNKNOWN INVALID ABSENT PRESENT DOUBLE CROSSED"

#define APP_DSP__DSlot										\
"1  2  3  4  5  6  7  8  9  10								\
11 12 13 14 15 16 17 18 19 20								\
21 22 23 24 25 26 27 28 29 30"


int CObj__ATM_RelianceATR::__DEFINE__VARIABLE_STD(p_variable)
{
	DECLARE__STD_VARIABLE

	CCommon_Error__DEF_VARIABLE m_err_def_variable;
	p_variable->Link__DEF_VARIABLE__ERROR_FEEDBACK(&m_err_def_variable);

	// ...
	CString str_name;
	int i;

	// Hyper Terminal ...
	{
		str_name = "sHYPER.TERMINAL.UPDATE.FLAG";
		STD__ADD_STRING_WITH_COMMENT(str_name, "");
		LINK__VAR_STRING_CTRL(xCH__UPDATE_FLAG, str_name);

		str_name = "dHYPER.TERMINAL.LOCK.FLAG";
		STD__ADD_DIGITAL_WITH_COMMENT(str_name, "YES NO", "");
		LINK__VAR_DIGITAL_CTRL(xCH__LOCK_FLAG, str_name);

		str_name = "dHYPER.TERMINAL.SEND.REQ";
		STD__ADD_DIGITAL(str_name, "NO YES");
		LINK__VAR_DIGITAL_CTRL(dCH__HYPER_TERMINAL_SEND_REQ,str_name);

		str_name = "sHYPER.TERMINAL.SEND.MSG";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__HYPER_TERMINAL_SEND_MSG,str_name);

		str_name = "sHYPER.TERMINAL.RECV.MSG";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__HYPER_TERMINAL_RECV_MSG,str_name);

		str_name = "aHYPER.TERMINAL.CMD.TIMEOUT";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "sec", 0, 1, 60, "");
		LINK__VAR_ANALOG_CTRL(aCH__HYPER_TERMINAL_CMD_TIMEOUT,str_name);
	}

	// Simulation ...
	{
		str_name = "OTR.IN.CFG.sSIMUL.ALIGN.TIME";
		STD__ADD_STRING_WITH_X_OPTION(str_name, "");
		LINK__VAR_STRING_CTRL(sCH__OTR_IN_CFG__SIMUL_ALIGN_TIME,str_name);
	}

	// ...
	{
		str_name = "INR.RB1.ACT.MSG";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__ACT_MSG, str_name);

		//
		str_name = "MON.COMMUNICATION.STATE";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__MON_COMMUNICATION_STATE, str_name);

		str_name = "MON.COMMUNICATION.INFO";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__MON_COMMUNICATION_INFO, str_name);

		//
		str_name = "INR.sERROR.ID";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__INR__ERROR_ID,str_name);

		str_name = "INR.sALGINER.ERROR.ID";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__INR__ALGINER_ERROR_ID,str_name);

		str_name = "INR.FLAG.dALGN.STS";
		STD__ADD_DIGITAL(str_name, "ALIGNING ALIGNED");
		LINK__VAR_DIGITAL_CTRL(dCH__INR_FLAG__ALGN_STS,str_name);
	}

	// PARA CHANNEL ...
	{
		str_name = "PARA.STN.NAME";
		STD__ADD_DIGITAL(str_name, APP_DSP__STN_NAME);
		LINK__VAR_DIGITAL_CTRL(dCH__PARA_STN_NAME, str_name);

		str_name = "PARA.STN.SLOT";
		STD__ADD_DIGITAL(str_name, APP_DSP__DSlot);
		LINK__VAR_DIGITAL_CTRL(dCH__PARA_STN_SLOT, str_name);

		str_name = "PARA.ARM.TYPE";
		STD__ADD_DIGITAL(str_name, APP_DSP__ARM_TYPE);
		LINK__VAR_DIGITAL_CTRL(dCH__PARA_ARM_TYPE, str_name);

		//
		str_name = "PARA.AL1.CCD.POS";
		STD__ADD_ANALOG(str_name, "deg",1,0.0,360.0);
		LINK__VAR_ANALOG_CTRL(aCH__PARA_AL1_CCD_POS, str_name);
	}

	// CONFIG CHANNEL ...
	{
		str_name = "CFG.ACTION.CONFIRM.FLAG";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"DISABLE ENABLE","");
		LINK__VAR_DIGITAL_CTRL(dCH__CFG_ACTION_CONFIRM_FLAG, str_name);

		str_name = "CFG.WAFER.DRIVER.CHECK";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"YES NO","");
		LINK__VAR_DIGITAL_CTRL(dCH__CFG_WAFER_DRIVER_CHECK, str_name);

		// ALx
		str_name = "CFG.AL1.STN.NUM";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "num", 0, 1, 16, "");
		LINK__VAR_ANALOG_CTRL(aCH__CFG_AL1_STN_NUM,str_name);

		// LLx
		str_name = "CFG.LBA.STN.NUM";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "num", 0, 1, 16, "");
		LINK__VAR_ANALOG_CTRL(aCH__CFG_LBA_STN_NUM,str_name);

		str_name = "CFG.LBB.STN.NUM";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "num", 0, 1, 16, "");
		LINK__VAR_ANALOG_CTRL(aCH__CFG_LBB_STN_NUM,str_name);

		// STx
		str_name = "CFG.ST1.STN.NUM";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "num", 0, 1, 16, "");
		LINK__VAR_ANALOG_CTRL(aCH__CFG_BUF1_STN_NUM,str_name);

		str_name = "CFG.ST2.STN.NUM";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "num", 0, 1, 16, "");
		LINK__VAR_ANALOG_CTRL(aCH__CFG_BUF2_STN_NUM,str_name);

		// LPx
		for(i=0; i<CFG_LPx__SIZE; i++)
		{
			str_name.Format("CFG.LP%1d.STN.NUM", i+1);
			STD__ADD_ANALOG_WITH_X_OPTION(str_name, "num", 0, 1, 16, "");
			LINK__VAR_ANALOG_CTRL(aCH__CFG_LPx_STN_NUM[i],str_name);
		}
	}

	// ROBOT ...
	{
		str_name = "OTR.OUT.CFG.ROM.RATE";
		STD__ADD_STRING_WITH_X_OPTION(str_name, "");
		LINK__VAR_STRING_CTRL(sCH__OTR_OUT_CFG__ROM_DATE, str_name);

		str_name = "OTR.OUT.CFG.ROM.VERSION";
		STD__ADD_STRING_WITH_X_OPTION(str_name, "");
		LINK__VAR_STRING_CTRL(sCH__OTR_OUT_CFG__ROM_VERSION, str_name);
	}

	// SIMULATION CHANNEL ...
	{
		str_name = "SIM.CFG.aROTATE.TIME";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",1,0,9999,"");
		LINK__VAR_ANALOG_CTRL(aCH__SIM_CFG__ROTATE_TIME, str_name);

		// LPx ...
		str_name = "SIM.CFG.aLPx.PICK.TIME";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",1,0,9999,"");
		LINK__VAR_ANALOG_CTRL(aCH__SIM_CFG__LPx_PICK_TIME, str_name);

		str_name = "SIM.CFG.aLPx.PLACE.TIME";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",1,0,9999,"");
		LINK__VAR_ANALOG_CTRL(aCH__SIM_CFG__LPx_PLACE_TIME, str_name);

		// ALx
		str_name = "SIM.CFG.aALx.PICK.TIME";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",1,0,9999,"");
		LINK__VAR_ANALOG_CTRL(aCH__SIM_CFG__ALx_PICK_TIME, str_name);

		str_name = "SIM.CFG.aALx.PLACE.TIME";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",1,0,9999,"");
		LINK__VAR_ANALOG_CTRL(aCH__SIM_CFG__ALx_PLACE_TIME, str_name);

		// LBx
		str_name = "SIM.CFG.aLBx.PICK.TIME";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",1,0,9999,"");
		LINK__VAR_ANALOG_CTRL(aCH__SIM_CFG__LBx_PICK_TIME, str_name);

		str_name = "SIM.CFG.aLBx.PLACE.TIME";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",1,0,9999,"");
		LINK__VAR_ANALOG_CTRL(aCH__SIM_CFG__LBx_PLACE_TIME, str_name);

		// VISx
		str_name = "SIM.CFG.aVISx.PICK.TIME";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",1,0,9999,"");
		LINK__VAR_ANALOG_CTRL(aCH__SIM_CFG__VISx_PICK_TIME, str_name);

		str_name = "SIM.CFG.aVISx.PLACE.TIME";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",1,0,9999,"");
		LINK__VAR_ANALOG_CTRL(aCH__SIM_CFG__VISx_PLACE_TIME, str_name);
	}

	// MATERIAL CHANNEL ...
	{
		CString dsp_slot_sts;
		p_variable->Get__STD_DESCRIPTION("STD_SLOT_STATUS", dsp_slot_sts);

		// Material Status
		str_name = "MON.ARM_A.MATERIAL.STATUS";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, dsp_slot_sts, "");
		LINK__VAR_DIGITAL_CTRL(dCH__MON_ARM_A_MATERIAL_STATUS, str_name);

		str_name = "MON.ARM_B.MATERIAL.STATUS";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, dsp_slot_sts,"");
		LINK__VAR_DIGITAL_CTRL(dCH__MON_ARM_B_MATERIAL_STATUS, str_name);

		// Material Title
		str_name = "MON.ARM_A.MATERIAL.TITLE";
		STD__ADD_STRING_WITH_X_OPTION(str_name,"");
		LINK__VAR_STRING_CTRL(sCH__MON_ARM_A_MATERIAL_TITLE, str_name);

		str_name = "MON.ARM_B.MATERIAL.TITLE";
		STD__ADD_STRING_WITH_X_OPTION(str_name,"");
		LINK__VAR_STRING_CTRL(sCH__MON_ARM_B_MATERIAL_TITLE, str_name);

		// AL Material 
		str_name = "MON.AL.MATERIAL.STATUS";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, dsp_slot_sts, "");
		LINK__VAR_DIGITAL_CTRL(dCH__MON_AL_MATERIAL_STATUS,str_name);
	}

	// ANIMATION CHANNEL ...
	{
		str_name = "OTR.OUT.MON.dACT.ARM";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, APP_DSP__ARM_TYPE,"");
		LINK__VAR_DIGITAL_CTRL(dCH__OTR_OUT_MON__ACT_ARM,str_name);

		str_name = "OTR.OUT.MON.dARM_A.ACT";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, APP_DSP__ARM_STS_ANI,"");
		LINK__VAR_DIGITAL_CTRL(dCH__OTR_OUT_MON__ARM_A_ACT,str_name);

		str_name = "OTR.OUT.MON.dARM_B.ACT";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, APP_DSP__ARM_STS_ANI,"");
		LINK__VAR_DIGITAL_CTRL(dCH__OTR_OUT_MON__ARM_B_ACT,str_name);

		str_name = "OTR.OUT.MON.dTRG.MOVE";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, APP_DSP__RB_TARGET_MOVE,"");
		LINK__VAR_DIGITAL_CTRL(dCH__OTR_OUT_MON__TRG_MOVE,str_name);

		str_name = "OTR.OUT.MON.dTRG.ROTATE";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, APP_DSP__RB_TARGET_MOVE,"");
		LINK__VAR_DIGITAL_CTRL(dCH__OTR_OUT_MON__TRG_ROTATE,str_name);
	}

	// ...
	{
		str_name = "ROM.VERSION";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(sCH__ROM_VER,str_name);

		str_name = "ROM.DATE";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(sCH__ROM_DATE,str_name);
	}

	for(i=0;i<CFG_LPx__SLOT_SIZE;i++)
	{
		str_name.Format("OTR.OUT.MON.dSLOT%02d.MAP.RESULT", i+1);
		STD__ADD_DIGITAL_WITH_COMMENT(str_name,APP_DSP__LP_WFR_SLOT_STS,"");
		LINK__VAR_DIGITAL_CTRL(dCH__LPx_SLOT_STATUS[i],str_name);
	}

	// ATMAlignAndPickTime		: Excel Num 82
	str_name = "CFG.aALIGN.AND.PICK.TIMEOUT";
	STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec", 1, 15.0, 100.0,"recommand:9 sec");
	LINK__VAR_ANALOG_CTRL(aCH__CFG__ALIGN_AND_PICK_TIMEOUT, str_name);
	//


	// ...
	{
		p_variable->Add__MONITORING_PROC(7.0, MON_ID__IO_MONITOR);
		p_variable->Add__MONITORING_PROC(3.0, MON_ID__HYPER_TERMINAL);
	}
	return 1;
}


// ...
#define  ACT__RETRY_ABORT						\
l_act.RemoveAll();								\
l_act.Add("RETRY");								\
l_act.Add("ABORT");

#define  ACT__ABORT_ALARM						\
l_act.RemoveAll();								\
l_act.Add("ABORT");

#define  ACT__NO_YES_ALARM						\
l_act.RemoveAll();								\
l_act.Add("NO");								\
l_act.Add("YES");


int CObj__ATM_RelianceATR::__DEFINE__ALARM(p_alarm)
{
	DECLARE__ALARM;

	// ...
	CString title;
	title.Format("%s - ",sObject_Name);

	CString alarm_title;
	CString alarm_msg;
	CString alarm_bff;
	CStringArray l_act;
	int alarm_id;
	int i;

	// ...
	{
		alarm_id = ALID__OFFLINE_ALARM_MON;

		alarm_title  = title;
		alarm_title += "Offline (Monitoring)";

		alarm_msg  = "Controller is Offline.\n";
		alarm_msg += "Please, Check Communication Status !\n";

		l_act.RemoveAll();
		l_act.Add("CHECK");

		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}
	// ...
	{
		alarm_id = ALID__OFFLINE_ALARM_ACT;

		alarm_title  = title;
		alarm_title += "Offline (Action)";

		alarm_msg  = "Controller is Offline.\n";
		alarm_msg += "Please, Check Communication Status !\n";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ALGN_AND_PICK_TIMEOUT_ALARM;

		alarm_title  = title;
		alarm_title += "Align and Pick Action Timeout Occur.";

		alarm_msg  = "Align and Pick Action Timeout Occur.\n";
		alarm_msg += "Please, Check ATM Robot Status !\n";

		ACT__ABORT_ALARM;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__MAPPING_DISABLE_CONFIG_ALARM;

		alarm_title  = title;
		alarm_title += "Config Option, Mapping Disable Alarm.";

		alarm_msg  = "Now, Mapping Disable Config Option Selected.\n";
		alarm_msg += "Would you like to continue ??\n";

		ACT__NO_YES_ALARM;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__LBA__NOT_ATM_PRESSURE;

		alarm_title  = title;
		alarm_title += "LBA's Pressure is not ATM.";

		alarm_msg  = "Please, Check LBA's Pressure !\n";
		alarm_msg += "LBA's Pressure must be ATM.\n";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__LBB__NOT_ATM_PRESSURE;

		alarm_title  = title;
		alarm_title += "LBB's pressure is not ATM.";

		alarm_msg  = "Please, check LBB's pressure !\n";
		alarm_msg += "LBB's pressure must be ATM.\n";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	for(i=0; i<5; i++)
	{
		// DOOR NOT OPEN
		{
			alarm_id = ALID__LP1__NOT_DOOR_OPEN + i;

			alarm_title  = title;
			alarm_bff.Format("LP%1d's door status is not open.",i+1);
			alarm_title += alarm_bff;

			alarm_msg.Format("Please, check LP%1d's door status !\n",i+1);

			ACT__RETRY_ABORT;
			ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
		}

		// FOUP NOT EXIST
		{
			alarm_id = ALID__LP1__FOUP_NOT_EXIST + i;

			alarm_title  = title;
			alarm_bff.Format("LP%1d's foup does not exist.",i+1);
			alarm_title += alarm_bff;

			alarm_msg.Format("Please, check LP%1d's foup does not exist !\n",i+1);

			ACT__RETRY_ABORT;
			ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
		}

		// FOUP NOT CLAMP
		{
			alarm_id = ALID__LP1__FOUP_NOT_CLAMP + i;

			alarm_title  = title;
			alarm_bff.Format("LP%1d's foup does not clamp.",i+1);
			alarm_title += alarm_bff;

			alarm_msg.Format("Please, check LP%1d's foup does not clamp !\n",i+1);

			ACT__RETRY_ABORT;
			ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
		}

		// FOUP NOT LOAD
		{
			alarm_id = ALID__LP1__FOUP_NOT_LOAD + i;

			alarm_title  = title;
			alarm_bff.Format("LP%1d's foup position is not load.",i+1);
			alarm_title += alarm_bff;

			alarm_msg.Format("Please, check LP%1d's foup position is not load !\n",i+1);

			ACT__RETRY_ABORT;
			ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
		}

		// LP1, 2, 3, 4, 5... WAFER SLIDE OUT SNS
		{
			alarm_id = ALID__LP1_WFR_SLIDE_SNS_ALARM + i;

			alarm_title  = title;
			alarm_bff.Format("LP%1d's Wafer Slide Out Sns is On Status.",i+1);
			alarm_title += alarm_bff;

			alarm_msg.Format("Please, check LP%1d's Wafer Slide Status... !\n",i+1);

			ACT__RETRY_ABORT;
			ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
		}
	}

	for(i=0; i<2; i++)
	{
		alarm_id = ALID__LL1_RNE_SNS_TIMEOUT + i;

		alarm_title  = title;
		alarm_bff.Format("LL%1d's RNE Sensor Timeout !", i+1);
		alarm_title += alarm_bff;

		alarm_msg.Format("Please, check LL%1d's RNE Sensor. \n", i+1);

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__LBA__NOT_DOOR_OPEN;

		alarm_title  = title;
		alarm_title += "LBA's door status is not open.";

		alarm_msg = "Please, check LBA's door status !\n";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__LBB__NOT_DOOR_OPEN;

		alarm_title  = title;
		alarm_title += "LBB's door status is not open.";

		alarm_msg = "Please, check LBB's door status !\n";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__INVALID__NOT_AL1_STATION;

		alarm_title  = title;
		alarm_title += "Invalid Target Station Eorror.";

		alarm_msg = "Please, check Target Station's Name !\n";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);	
	}

	Register__ALARM_INF(p_alarm);
	return 1;
}


// ...
#define APP_DSP__Cmd_Set								\
"Goto Pick ALPick AlgnPick Place PlaceAlgn				\
CheckLoad SetVac Home Reset MapStn GetMap				\
SetALStn StoALStn										\
InitAlgn ALSetComm ALStoComm ALAlign ALScan				\
ALHome ALReset ALResult ALLoadPP ALSetPP ALSetWfr		\
Reinit MapClear"

#define APP_DSP__Arm_ID				"A  B"
#define APP_DSP__Wfr    			"Unknown  Absent  Present"
#define APP_DSP__StrLoc 			"Normal  Retract2  NoRetract"
#define APP_DSP__Axises				"ALL  R  T  Z  W  S"

#define APP_DSP__Stn									\
"1  2  3  4  5  6  7  8  9  10							\
11 12 13 14 15 16 17 18 19 20							\
21 22 23 24 25 26 27 28 29 30"

#define APP_DSP__CurStn									\
"0														\
1  2  3  4  5  6  7  8  9  10							\
11 12 13 14 15 16 17 18 19 20							\
21 22 23 24 25 26 27 28 29 30"

#define APP_DSP__DDnUp				"DN  UP"
#define APP_DSP__OffOn              "OFF ON"
#define APP_DSP__RetExt             "RE  EX"
#define APP_DSP__ComSts             "Offline Online"
#define APP_DSP__YesNo              "Y   N"
#define APP_DSP__PrsAbs				"ON  OFF"
#define APP_DSP__ALWfr				"?  NO  YES"

#define APP_DSP__Material								\
"? 0 1 2 3 4 5 6 7 20 21 22 23 FP"

#define APP_DSP__Substrates								\
"1 2 3 4 5 6 7 20 21 22 23"

#define APP_DSP__FDCLs				"None  Flat  Ntch"
#define APP_DSP__WfrShapes			"Round  Square"
#define APP_DSP__CCDPos				"1  2  3"
#define APP_DSP__DOffOn				"Off  On"
#define APP_DSP__DZPos				"Unknown  Up  Down"
#define APP_DSP__DRPos				"Unknown  Retract  Extend"

#define APP_DSP__DTPos									\
"Unknown												\
1  2  3  4  5  6  7  8  9  10							\
11 12 13 14 15 16 17 18 19 20							\
21 22 23 24 25 26 27 28 29 30"

#define APP_DSP__DSPos									\
"Unknown												\
1  2  3  4  5  6  7  8  9  10							\
11 12 13 14 15 16 17 18 19 20							\
21 22 23 24 25 26 27 28 29 30"


int CObj__ATM_RelianceATR::__DEFINE__VARIABLE_IO(p_io_variable)
{
	DECLARE__IO_VARIABLE;

	CString str_name;

	// Analog ...
	{
		// AO  -----------------------------
		{
			str_name = "ao.Angle";
			IO__ADD_ANALOG_WRITE(str_name, "deg", 1,0.0,360.0);
			LINK__IO_VAR_ANALOG_CTRL(aoCH__ANGLE, str_name);
		}

		// AI  -----------------------------
		{
			str_name = "ai.AngPos";
			IO__ADD_ANALOG_READ__MANUAL(str_name, "deg", 1,-360.0,360.0);
			LINK__IO_VAR_ANALOG_CTRL(aiCH__ANG_POS, str_name);

			str_name = "ai.AngEcc";
			IO__ADD_ANALOG_READ__MANUAL(str_name, "deg", 1,-360.0,360.0);
			LINK__IO_VAR_ANALOG_CTRL(aiCH__ANG_ECC, str_name);

			str_name = "ai.EccMag";
			IO__ADD_ANALOG_READ__MANUAL(str_name, "mils",1,0.0,300.0);
			LINK__IO_VAR_ANALOG_CTRL(aiCH__ECC_MAG, str_name);

			str_name = "ai.AvrgRd";
			IO__ADD_ANALOG_READ__MANUAL(str_name, "deg",1,-360.0,360.0);
			LINK__IO_VAR_ANALOG_CTRL(aiCH__AVRG_RD, str_name);

			str_name = "ai.ErrorId";
			IO__ADD_ANALOG_READ__MANUAL(str_name, "num",0,0,99999);
			LINK__IO_VAR_ANALOG_CTRL(aiCH__ERROR_ID, str_name);
		}
	}

	// Digital ...
	{
		// DO  -----------------------------
		{
			str_name = "do.Command";
			IO__ADD_DIGITAL_WRITE(str_name, APP_DSP__Cmd_Set);
			LINK__IO_VAR_DIGITAL_CTRL(doCH__COMMAND, str_name);

			str_name = "do.Stn";
			IO__ADD_DIGITAL_WRITE(str_name, APP_DSP__Stn);
			LINK__IO_VAR_DIGITAL_CTRL(doCH__STN, str_name);

			str_name = "do.Arm";
			IO__ADD_DIGITAL_WRITE(str_name, APP_DSP__Arm_ID);
			LINK__IO_VAR_DIGITAL_CTRL(doCH__ARM, str_name);

			str_name = "do.Slot";
			IO__ADD_DIGITAL_WRITE(str_name, APP_DSP__DSlot);
			LINK__IO_VAR_DIGITAL_CTRL(doCH__SLOT, str_name);

			str_name = "do.ZPos";
			IO__ADD_DIGITAL_WRITE(str_name, APP_DSP__DDnUp);
			LINK__IO_VAR_DIGITAL_CTRL(doCH__ZPOS, str_name);

			str_name = "do.Vacuum";
			IO__ADD_DIGITAL_WRITE(str_name, APP_DSP__OffOn);
			LINK__IO_VAR_DIGITAL_CTRL(doCH__VACUUM, str_name);

			str_name = "do.STRT";
			IO__ADD_DIGITAL_WRITE(str_name, APP_DSP__StrLoc);
			LINK__IO_VAR_DIGITAL_CTRL(doCH__STRT, str_name);

			str_name = "do.ENRT";
			IO__ADD_DIGITAL_WRITE(str_name, APP_DSP__StrLoc);
			LINK__IO_VAR_DIGITAL_CTRL(doCH__ENRT, str_name);

			str_name = "do.HomeAxis";
			IO__ADD_DIGITAL_WRITE(str_name, APP_DSP__Axises);
			LINK__IO_VAR_DIGITAL_CTRL(doCH__HOME_AXIS, str_name);

			str_name = "do.RPos";
			IO__ADD_DIGITAL_WRITE(str_name, APP_DSP__RetExt);
			LINK__IO_VAR_DIGITAL_CTRL(doCH__RPOS, str_name);

			str_name = "do.ArmAWfr";
			IO__ADD_DIGITAL_WRITE(str_name, APP_DSP__Wfr);
			LINK__IO_VAR_DIGITAL_CTRL(doCH__ARM_A_WFR, str_name);

			str_name = "do.WfrSize";
			IO__ADD_DIGITAL_WRITE(str_name, APP_DSP__Substrates);
			LINK__IO_VAR_DIGITAL_CTRL(doCH__WFR_SIZE, str_name);

			str_name = "do.WfrFDCL";
			IO__ADD_DIGITAL_WRITE(str_name, APP_DSP__FDCLs);
			LINK__IO_VAR_DIGITAL_CTRL(doCH__WFR_FDCL, str_name);

			str_name = "do.WfrShape";
			IO__ADD_DIGITAL_WRITE(str_name, APP_DSP__WfrShapes);
			LINK__IO_VAR_DIGITAL_CTRL(doCH__WFR_SHAPE, str_name);

			str_name = "do.WfrCCD";
			IO__ADD_DIGITAL_WRITE(str_name, APP_DSP__CCDPos);
			LINK__IO_VAR_DIGITAL_CTRL(doCH__WFR_CCD, str_name);

			str_name = "do.Material";
			IO__ADD_DIGITAL_WRITE(str_name, APP_DSP__Material);
			LINK__IO_VAR_DIGITAL_CTRL(doCH__MATERIAL, str_name);
		}

		// DI  -----------------------------
		{
			str_name = "di.ComSts";
			IO__ADD_DIGITAL_READ(str_name, APP_DSP__ComSts);
			LINK__IO_VAR_DIGITAL_CTRL(diCH__COMM_STS, str_name);

			str_name = "di.ArmAWfr.Manual";
			IO__ADD_DIGITAL_READ__MANUAL(str_name, APP_DSP__Wfr);
			LINK__IO_VAR_DIGITAL_CTRL(diCH__ARM_A_WFR_MANUAL, str_name);

			str_name = "di.VacSnsA";
			IO__ADD_DIGITAL_READ__MANUAL(str_name, APP_DSP__PrsAbs);
			LINK__IO_VAR_DIGITAL_CTRL(diCH__VAC_SNS_A, str_name);

			str_name = "di.Rescan";
			IO__ADD_DIGITAL_READ__MANUAL(str_name, APP_DSP__YesNo);
			LINK__IO_VAR_DIGITAL_CTRL(diCH__RESCAN, str_name);

			str_name = "di.CurrStn";
			IO__ADD_DIGITAL_READ__MANUAL(str_name, APP_DSP__CurStn);
			LINK__IO_VAR_DIGITAL_CTRL(diCH__CURR_STN, str_name);

			str_name = "di.VacStsA";
			IO__ADD_DIGITAL_READ__MANUAL(str_name, APP_DSP__OffOn);
			LINK__IO_VAR_DIGITAL_CTRL(diCH__VAC_STS_A, str_name);

			str_name = "di.ALWfr";
			IO__ADD_DIGITAL_READ__MANUAL(str_name, APP_DSP__ALWfr);
			LINK__IO_VAR_DIGITAL_CTRL(diCH__AL_WFR, str_name);

			// ...
			str_name = "di.Robot.CurrPos";
			IO__ADD_DIGITAL_READ__MANUAL(str_name, APP_DSP__DOffOn);
			LINK__IO_VAR_DIGITAL_CTRL(diCH__ROBOT_CURR_POS, str_name);

			//
			str_name = "di.Robot.Z.Pos";
			IO__ADD_DIGITAL_READ__MANUAL(str_name, APP_DSP__DZPos);
			LINK__IO_VAR_DIGITAL_CTRL(diCH__ROBOT_Z_POS, str_name);

			str_name = "di.Robot.AR.Pos";
			IO__ADD_DIGITAL_READ__MANUAL(str_name, APP_DSP__DRPos);
			LINK__IO_VAR_DIGITAL_CTRL(diCH__ROBOT_AR_POS, str_name);

			str_name = "di.Robot.T.Pos";
			IO__ADD_DIGITAL_READ__MANUAL(str_name, APP_DSP__DTPos);
			LINK__IO_VAR_DIGITAL_CTRL(diCH__ROBOT_T_POS, str_name);
		}
	}

	// String ...
	{
		// SO  -----------------------------
		{
			str_name = "eso.RB1.HyperTerminal.Command";
			IO__ADD_STRING_WRITE(str_name);
			LINK__IO_VAR_STRING_CTRL(soCH__HYPER_TERMINAL_CMD, str_name);
		}

		// SI  -----------------------------
		{

		}
	}

	return 1;
}

//--------------------------------------------------------------------------------
int CObj__ATM_RelianceATR::__INITIALIZE__OBJECT(p_variable,p_ext_obj_create)
{
	// ...
	{
		CString file_name;
		CString log_msg;

		file_name.Format("%s_Drv.log", sObject_Name);

		log_msg  = "\n\n";
		log_msg += "//------------------------------------------------------------------------";

		xDRV_LOG_CTRL->CREATE__SUB_DIRECTORY(sObject_Name);
		xDRV_LOG_CTRL->SET__PROPERTY(file_name,24*5,60);

		xDRV_LOG_CTRL->DISABLE__TIME_LOG();
		xDRV_LOG_CTRL->WRITE__LOG(log_msg);

		xDRV_LOG_CTRL->ENABLE__TIME_LOG();
		xDRV_LOG_CTRL->WRITE__LOG("   START   \n");
	}

	iFlag__DRV_LOG = 1;

	// ...
	CString def_name;
	CString def_data;
	CString str_name;
	int i;
	int j;

	CString obj_name, var_name;

	// DB_CFG -----
	{
		def_name = "OBJ__DB";
		p_variable->Get__DEF_CONST_DATA(def_name,def_data);

		//
		str_name = "CFG.REF.ATM.PRESSURE";
		LINK__EXT_VAR_ANALOG_CTRL(aEXT_CH__CFG_REF_ATM_PRESSURE, def_data,str_name);


		// AL1 -----
		str_name = "CFG.AL1.EXIST.FLAG";
		LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__CFG_AL1_EXIST_FLAG, def_data,str_name);

		str_name = "CFG.AL1.MATERIAL.TYPE";
		LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__CFG_AL1_MATERIAL_TYPE, def_data,str_name);

		//
		str_name = "CFG.AL1.FDCL";
		LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__CFG_AL1_FDCL, def_data,str_name);

		str_name = "CFG.dALIGN.DEVICE";
		LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__CFG_ALIGN_DEVICE, def_data,str_name);

	}

	// AL1 -----
	{
		def_name = "OBJ__AL1";
		p_variable->Get__DEF_CONST_DATA(def_name,def_data);

		// Slot Status
		str_name = "OTR.OUT.MON.dSLOT01.STATUS";
		LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__AL1_SLOT01_STATUS, def_data,str_name);

		// Slot Title
		str_name = "OTR.OUT.MON.sSLOT01.TITLE";
		LINK__EXT_VAR_STRING_CTRL(sEXT_CH__AL1_SLOT01_TITLE, def_data,str_name);
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

	// ...
	{
		SCX__SEQ_INFO seq_info;

		iActive_SIM = seq_info->Is__SIMULATION_MODE();
	}
	return 1;
}
int CObj__ATM_RelianceATR::__INITIALIZE__IO(p_io_para)
{
	memset(m_Term_Str, 0, sizeof(m_Term_Str));		

	//-------------------------------------------------------
	// Set Driver Parameter
	//------------------------------------------------------
	int com_port = -1;
	
	int nBaudRate = 9600;
	int nDataBit  = 8;
	int nParity   = 0;			// None
	int nStopBit  = 1;			// 0, 1, 2, 3, 4  // 0 ~ 4 : No, Odd, Even, Mark, Space

	// ...
	{
		CString para_cmmd;
		CString para_data;

		// 1.
		{
			para_cmmd = "COM_PORT";
			
			if(p_io_para->Get__PARAMETER_DATA(para_cmmd, para_data) > 0)
			{
				com_port = atoi(para_data);
			}
		}
	}

	// ...
	{
		m_Term_Str[0] = '_';  
		m_Term_Str[1] = 'R';  
		m_Term_Str[2] = 'D';  
		m_Term_Str[3] = 'Y';  
		m_Term_Str[4] = CR ;
		m_Term_Str[5] = '\0' ;
	}

	// ...
	{
		iDRV__ALGNER_ERROR_ID = 0;
		iDRV__ERROR_ID = 0;
		iDRV__STN_NO   = 1;
		sDRV__ARM_NO   = "A";

		sDRV__R_POS = "RE";
		sDRV__Z_POS = "DN";
		iDRV__SLOT_NO = 1;
		sDRV__RB_STRT = "R1";
		sDRV__RB_ENRT = "R1";
		sDRV__RB_VAC_SNS_A = "OFF";
		sDRV__RB_VAC_SNS_B = "OFF";
		sDRV__RB_VACUUM    = "OFF";
		sDRV__RB_HOME_AXIS = "ALL";
		dDRV__RB_FIND_BIAS_SET = 0.0;
		sDRV__RB_MATERIAL  = "7";       // 6: 200mm  7: 300mm
		sDRV__RB_WFR_SHAPE = "ROUND";
		sDRV__RB_WFR_SIZE  = "7";
		sDRV__RB_WFR_FDCL  = "Ntch";
		dDRV__RB_ANGLE     = 0.0;
		dDRV__RB_ANGLE_POS = 0.0;
		dDRV__RB_ANGLE_ECC = 0.0;
		dDRV__RB_ECC_MAG   = 0.0;
		dDRV__RB_AVG_RD    = 0.0;
		sDRV__RB_ARM_A_WFR_SET = "Unknown";
		sDRV__RB_ARM_B_WFR_SET = "Unknown";

		iDRV__RB_WFR_CCD = 2;			// 2: 300mm

		sDRV__RB_Z_POS  = "Unknown";
		sDRV__RB_AR_POS = "Unknown";
		sDRV__RB_BR_POS = "Unknown";
		sDRV__RB_T_POS  = "Unknown";
		sDRV__RB_S_POS  = "Unknown";

		iDRV__ECC_MAG   = 0;
		iDRV__ANGLE_POS = 0;
		iDRV__ANGLE_ECC = 0;
		iDRV__AVG_RD    = 0;

		dDRV__DELTA_R = 0.0;
		dDRV__DELTA_T = 0.0; 

		sDRV__RESCAN = "N";
	}

	// ...
	CString log_msg;
	CString log_bff;

	CString str_data;

	// ...
	{
		log_msg.Format("Connecting to Serial ... \n");

		log_bff.Format(" * Com Port: %d \n", com_port);
		log_msg += log_bff;

		log_bff.Format(" * Baud Rate: %1d \n", nBaudRate);
		log_msg += log_bff;

		log_bff.Format(" * Data Bit: %1d \n", nDataBit);
		log_msg += log_bff;

		// ...
		{
			str_data = Macro__Get_Parity(nParity);

			log_bff.Format(" * Parity Bit: %s (%1d) \n", str_data,nParity);
			log_msg += log_bff;
		}

		log_bff.Format(" * Stop Bit: %1d \n", nStopBit);
		log_msg += log_bff;

		Fnc__DRV_LOG_FORMAT(log_msg);
	}

	// ...
	{
		CString str_info;

		str_info.Format("Com_Port(%1d) Buad_Rate(%1d) Data_Bit(%1d) Stop_Bit(%1d) Parity(%s)",
						com_port,
						nBaudRate,
						nDataBit,
						nStopBit,
						Macro__Get_Parity(nParity));

		sCH__MON_COMMUNICATION_INFO->Set__DATA(str_info);
	}	

	if(iActive_SIM > 0)
	{

	}
	else
	{
		if(mX_Serial->INIT__COMPORT(com_port, nBaudRate, nDataBit, nStopBit, nParity) < 0)
		{
			log_msg.Format("Fail to do INIT__COMPORT: %d", com_port);
			Fnc__DRV_LOG_FORMAT(log_msg);
			return -1;
		}
		else
		{
			log_msg.Format("Initialize RS-232 Complete");
			Fnc__DRV_LOG_FORMAT(log_msg);
		}
	}

	// ...
	{
		m_nRetryCount = 2;

		log_msg = "\n";
		log_bff.Format(" * Retry Count: %1d \n", m_nRetryCount);
		log_msg += log_bff;
	
		//
		m_nTimeout = 2*1000;
		m_Rcv_Time = m_nTimeout;

		log_bff.Format(" * Recv_Timeout: %1d sec \n", m_nTimeout);
		log_msg += log_bff;

		//
		m_Out_Time = 30000;		// 30 sec
		
		log_bff.Format("Action_Timeout: %1d sec \n", m_Out_Time);
		log_msg += log_bff;

		xAPP_LOG_CTRL->WRITE__LOG(log_msg);
		xDRV_LOG_CTRL->WRITE__LOG(log_msg);
	}

	// Communication Init ...
	{
		doCH__COMMAND->Set__DATA("Reinit");
	}
	return 1;
}

int CObj__ATM_RelianceATR::__Define__USER_FUNCTION(CII_DEFINE__FUNCTION *p_fnc_ctrl)
{
	p_fnc_ctrl->Link__ERROR_FEEDBACK(&mERROR__USER_FNC);

	return 1;
}

//--------------------------------------------------------------------------------
int CObj__ATM_RelianceATR::__CALL__CONTROL_MODE(mode, p_debug, p_variable, p_alarm)
{
	// For Hyper Terminal Interlock !!
	xCH__LOCK_FLAG->Set__DATA("YES");		//	Lock을 건다.

	// ...
	int flag = -1;

	CString para__arm_type;
	CString para__stn_name;
	CString para__stn_slot;

	CString szUpperObj;

	CStringArray l_para;
	p_variable->Get__FNC_PARAMETER(l_para);

	if(l_para.GetSize() > 0)
	{
		CString para_data;
		int limit = l_para.GetSize();
		int i;

		for(i=0;i<limit;i++)
		{
			para_data = l_para[i];

			switch(i)
			{
				case 0:		
					dCH__PARA_ARM_TYPE->Set__DATA(para_data);
					break;

				case 1:
					dCH__PARA_STN_NAME->Set__DATA(para_data);
					break;

				case 2:
					dCH__PARA_STN_SLOT->Set__DATA(para_data);
					break;
			}	
		}
	} 

	dCH__PARA_ARM_TYPE->Get__DATA(para__arm_type);
	dCH__PARA_STN_NAME->Get__DATA(para__stn_name);
	dCH__PARA_STN_SLOT->Get__DATA(para__stn_slot);

	// ...
	{
		CString log_msg;
		CString log_bff;

		log_msg.Format("Start ... :  [%s] (%s,%s,%s)",
						mode,
						para__arm_type,
						para__stn_name,
						para__stn_slot);

		Fnc__ACT_MSG(log_msg);

		log_msg += "\n";
		log_bff.Format("  * %s <- %s \n", 
					   aCH__PARA_AL1_CCD_POS->Get__VARIABLE_NAME(),
					   aCH__PARA_AL1_CCD_POS->Get__STRING());
		log_msg += log_bff;

		Fnc__APP_LOG(log_msg);
	}

	// ...
	int seq_flag = 1;

	if(dCH__CFG_ACTION_CONFIRM_FLAG->Check__DATA(STR__ENABLE) > 0)
	{
		CString stn_id = "Error !";
		Get__CFG_STN_NUM(para__stn_name, stn_id);

		// ...
		CString log_msg;

		CString str_title;
		CString str_msg;
		CString str_bff;
		CStringArray l_act;
		CString r_act;

		str_title.Format("%s - Action Confirm Flag", sObject_Name);

		str_bff.Format("Object Mode : [%s] \n", mode);
		str_msg += str_bff;
		str_bff.Format("Arm Type : [%s] \n", para__arm_type);
		str_msg += str_bff;
		str_bff.Format("Station Name : [%s] \n", para__stn_name);
		str_msg += str_bff;
		str_bff.Format("Station Slot : [%s] \n", para__stn_slot);
		str_msg += str_bff;
		str_bff.Format("Station ID : [%s] \n", stn_id);
		str_msg += str_bff;

		l_act.RemoveAll();
		l_act.Add(STR__YES);
		l_act.Add(STR__NO);

		p_alarm->Popup__MESSAGE_BOX(str_title,str_msg,l_act,r_act);

		if(r_act.CompareNoCase(STR__NO) == 0)
		{
			seq_flag = -1;

			CString log_msg = "Confirm : Action No Clicked !";

			Fnc__APP_LOG(log_msg);
			Fnc__ACT_MSG(log_msg);
		}
	}

	if(seq_flag > 0)
	{
		pII__EXT_MODE_CTRL = p_variable->Get__EXT_FNC_MODE_CTRL();
		pII__EXT_VAR_CTRL  = p_variable->Get__EXT_FNC_VAR_CTRL();
		pII__EXT_FNC_CTRL  = p_variable->Get__EXT_USER_FNC_CTRL();

		// ...
		{
			CString log_msg;

			szUpperObj = pII__EXT_MODE_CTRL->Get__UPPER_OBJECT_NAME();	
		
			log_msg.Format("Object Call: [%s] ==> [%s]",  szUpperObj,sObject_Name);
			Fnc__APP_LOG(log_msg);
		}
		// ...

			 IF__CTRL_MODE(sMODE__INIT)			flag = Call__INIT(p_variable,p_alarm);
	    ELSE_IF__CTRL_MODE(sMODE__HOME)			flag = Call__HOME(p_variable,p_alarm, -1);
		ELSE_IF__CTRL_MODE(sMODE__VACOFF_HOME)	flag = Call__HOME(p_variable,p_alarm,  1);

		ELSE_IF__CTRL_MODE(sMODE__PICK)			flag = Call__PICK(p_variable,p_alarm,  para__arm_type,para__stn_name,para__stn_slot);
		ELSE_IF__CTRL_MODE(sMODE__PLACE)		flag = Call__PLACE(p_variable,p_alarm, para__arm_type,para__stn_name,para__stn_slot);

		ELSE_IF__CTRL_MODE(sMODE__ROTATE)		flag = Call__ROTATE(p_variable,p_alarm, para__arm_type,para__stn_name,para__stn_slot);

		ELSE_IF__CTRL_MODE(sMODE__VAC_ON)		flag = Call__VAC_ON(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__VAC_OFF)		flag = Call__VAC_OFF(p_variable,p_alarm);

		ELSE_IF__CTRL_MODE(sMODE__MAP)			flag = Call__MAP(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__LP_MAP)		flag = Call__LP_MAP(p_variable,p_alarm,para__stn_name);

		ELSE_IF__CTRL_MODE(sMODE__PLACE_ALIGN_PICK)		flag = Call__PLACE_ALGNPICK(p_variable,p_alarm, para__arm_type,para__stn_name,para__stn_slot);
		ELSE_IF__CTRL_MODE(sMODE__AL_PICK)				flag = Call__AL_PICK(p_variable,p_alarm);		// just define
		ELSE_IF__CTRL_MODE(sMODE__AL_ALIGN_PICK)		flag = Call__AL_ALIGN_PICK(p_variable,p_alarm, para__arm_type,para__stn_name,para__stn_slot);
		ELSE_IF__CTRL_MODE(sMODE__ALIGN_PLACE)			flag = Call__ALIGN_PLACE(p_variable,p_alarm, para__arm_type,para__stn_name,para__stn_slot);

		ELSE_IF__CTRL_MODE(sMODE__AL_ALIGN)				flag = Call__AL_ALIGN(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__AL_ALIGN_INIT)		flag = Call__AL_ALIGN_INIT(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__AL_ALIGN_RESULT)		flag = Call__AL_ALIGN_RESULT(p_variable,p_alarm);

		ELSE_IF__CTRL_MODE(sMODE__AL_SCAN)		flag = Call__AL_SCAN(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__AL_HOME)		flag = Call__AL_HOME(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__AL_RESET)		flag = Call__AL_RESET(p_variable,p_alarm);

		ELSE_IF__CTRL_MODE(sMODE__EXTEND)		flag = Call__EXTEND(p_variable,p_alarm, para__arm_type,para__stn_name,para__stn_slot);
		ELSE_IF__CTRL_MODE(sMODE__RETRACT)		flag = Call__RETRACT(p_variable,p_alarm, para__arm_type,para__stn_name,para__stn_slot);

		ELSE_IF__CTRL_MODE(sMODE__GOUP)			flag = Call__GOUP(p_variable,p_alarm, para__arm_type,para__stn_name,para__stn_slot);
		ELSE_IF__CTRL_MODE(sMODE__GODOWN)		flag = Call__GODOWN(p_variable,p_alarm,	para__arm_type,para__stn_name,para__stn_slot);
	}
	else
	{
		flag = -1;
	}

	if((flag < 0)||(p_variable->Check__CTRL_ABORT() > 0))
	{
		CString log_msg;	
		log_msg.Format("Aborted ... :  [%s]\n", mode);

		Fnc__APP_LOG(log_msg);
		Fnc__ACT_MSG(log_msg);
	}
	else
	{
		CString log_msg;
		log_msg.Format("Completed ... :  [%s]\n", mode);

		Fnc__APP_LOG(log_msg);
		Fnc__ACT_MSG(log_msg);
	}

	return flag;
}

int CObj__ATM_RelianceATR::__CALL__MONITORING(id, p_variable, p_alarm)
{
	switch(id)
	{
		case MON_ID__IO_MONITOR:
			Mon__IO_MONITOR(p_variable,p_alarm);
			break;

		case MON_ID__HYPER_TERMINAL:
			Mon__HYPER_TERMINAL_SEND(p_variable,p_alarm);
			break;
	}

	return 1;
}
