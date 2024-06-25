#include "StdAfx.h"
#include "CObj_Phy__PMC_STD.h"
#include "CObj_Phy__PMC_STD__ALID.h"


//-------------------------------------------------------------------------
CObj_Phy__PMC_STD::CObj_Phy__PMC_STD()
{

}
CObj_Phy__PMC_STD::~CObj_Phy__PMC_STD()
{

}

//-------------------------------------------------------------------------
int CObj_Phy__PMC_STD::__DEFINE__CONTROL_MODE(obj,l_mode)
{
	sObject_Name = obj;

	// ...
	{
		iPRC_STS = -1;
		iFNC_STS = -1;
	}

	// ...
	{
		ADD__CTRL_VAR(sMODE__TIME_INIT,   "TIME_INIT");
		ADD__CTRL_VAR(sMODE__SYSTEM_INIT, "SYSTEM_INIT");
		ADD__CTRL_VAR(sMODE__AUTO_INIT,   "AUTO_INIT");

		ADD__CTRL_VAR(sMODE__SYSTEM_MAINT,"SYSTEM_MAINT");
	
		ADD__CTRL_VAR(sMODE__PUMP, "PUMP");
		ADD__CTRL_VAR(sMODE__VENT, "VENT");
	
		ADD__CTRL_VAR(sMODE__SLOT_OPEN,  "SLOT.OPEN");
		ADD__CTRL_VAR(sMODE__SLOT_CLOSE, "SLOT.CLOSE");

		ADD__CTRL_VAR(sMODE__PIN_UP,   "PIN.UP");
		ADD__CTRL_VAR(sMODE__PIN_DOWN, "PIN.DOWN");

		ADD__CTRL_VAR(sMODE__PICK_READY,      "PICK_READY");
		ADD__CTRL_VAR(sMODE__PICK_X_READY,    "PICK_X_READY");
		ADD__CTRL_VAR(sMODE__PICK_COMPLETE,   "PICK_COMPLETE");
		ADD__CTRL_VAR(sMODE__PICK_X_COMPLETE, "PICK_X_COMPLETE");
	
		ADD__CTRL_VAR(sMODE__PLACE_READY,     "PLACE_READY");
		ADD__CTRL_VAR(sMODE__PLACE_X_READY,   "PLACE_X_READY");
		ADD__CTRL_VAR(sMODE__PLACE_COMPLETE,  "PLACE_COMPLETE");
		ADD__CTRL_VAR(sMODE__PLACE_X_COMPLETE,"PLACE_X_COMPLETE");

		ADD__CTRL_VAR(sMODE__PRO_READY,   "PRO_READY");
		ADD__CTRL_VAR(sMODE__PRO_START,   "PRO_START");
		ADD__CTRL_VAR(sMODE__AUTO_PRO,    "AUTO_PRO" );
		ADD__CTRL_VAR(sMODE__AUTO_PRO_EX, "AUTO_PRO_EX");

		ADD__CTRL_VAR(sMODE__PRE_RCP_READY, "PRE_RCP_READY");
		ADD__CTRL_VAR(sMODE__PRE_RCP_START, "PRE_RCP_START");
		ADD__CTRL_VAR(sMODE__AUTO_PRE_RCP,  "AUTO_PRE_RCP");

		ADD__CTRL_VAR(sMODE__CLN_READY, "CLN_READY");
		ADD__CTRL_VAR(sMODE__CLN_START, "CLN_START");
		ADD__CTRL_VAR(sMODE__AUTO_CLN,  "AUTO_CLN" );
		ADD__CTRL_VAR(sMODE__POST_CLN,	"POST_CLN" );

		ADD__CTRL_VAR(sMODE__AUTO_DUMMY_CLN,  "AUTO_DUMMY_CLN" );

		ADD__CTRL_VAR(sMODE__LAYER_CLN_READY, "LAYER_CLN_READY");
		ADD__CTRL_VAR(sMODE__LAYER_CLN_START, "LAYER_CLN_START");
		ADD__CTRL_VAR(sMODE__AUTO_LAYER_CLN,  "AUTO_LAYER_CLN" );

		ADD__CTRL_VAR(sMODE__MNT_READY, "MNT_READY");
		ADD__CTRL_VAR(sMODE__MNT_START, "MNT_START");
		ADD__CTRL_VAR(sMODE__AUTO_MNT,  "AUTO_MNT" );

		ADD__CTRL_VAR(sMODE__LEAK_CHECK,   "LEAK_CHECK");
		ADD__CTRL_VAR(sMODE__LEAK_CHECK_EX,"LEAK_CHECK_EX");

		ADD__CTRL_VAR(sMODE__AUTO_PM,    "AUTO_PM");
		ADD__CTRL_VAR(sMODE__AUTO_PM_EX, "AUTO_PM_EX");

		ADD__CTRL_VAR(sMODE__NEXT_PROC, "NEXT_PROC");

		ADD__CTRL_VAR(sMODE__JOB_START_READY, "JOB_START_READY");
		ADD__CTRL_VAR(sMODE__JOB_END_READY,   "JOB_END_READY");

		ADD__CTRL_VAR(sMODE__IDLE_START, "IDLE_START");

		//
		ADD__CTRL_VAR(sMODE__LOAD,   "LOAD");
		ADD__CTRL_VAR(sMODE__UNLOAD, "UNLOAD");
		ADD__CTRL_VAR(sMODE__READY_TO_UNLOAD, "READY_TO_UNLOAD");

		//
		ADD__CTRL_VAR(sMODE__SCH_TEST_CFG, "SCH_TEST.CFG");
	}

	return 1;
}

int CObj_Phy__PMC_STD::__DEFINE__VERSION_HISTORY(version)
{
	version  = "JG.Lee \n";

	return 1;
}


// ...
#define  MON_ID__ALL_STATE						1
#define  MON_ID__FA_STEP_REPORT					2


// ...
#define  DSP__MODE													\
"TIME_INIT  SYSTEM_INIT   AUTO_INIT									\
PUMP  VENT															\
SLOT.OPEN  SLOT.CLOSE												\
PIN.UP  PIN.DOWN    												\
PICK_READY   PICK_X_READY   PICK_COMPLETE   PICK_X_COMPLETE			\
PLACE_READY  PLACE_X_READY  PLACE_COMPLETE  PLACE_X_COMPLETE		\
PRO_READY  PRO_START												\
CLN_READY  CLN_START												\
PRE_READY  PRE_START												\
MNT_READY  MNT_START												\
LEAK_CHECK  HOT_IDLE  SYSTEM_MAINT									\
AUTO_PM                          									\
JOB_START_READY  JOB_END_READY										\
IDLE_START															\
LOAD  UNLOAD  READY_TO_UNLOAD"

#define  DSP__NO_YES			"NO  YES"
#define  DSP__PRC_STS			"IDLE  PROCESSING"
#define  DSP__DISABLE_ENABLE	"DISABLE  ENABLE"
#define  DSP__LEAK_CHECK_MODE   "JOB  LEAK_CHECK"
#define  DSP__AUTO_PM_MODE      "JOB  AUTO_PM"


int CObj_Phy__PMC_STD::__DEFINE__VARIABLE_STD(p_variable)
{
	DECLARE__STD_VARIABLE;

	CString str_name;
	int i;

	// ...
	{
		CString dsp_item;
		p_variable->Get__STD_DESCRIPTION("STD_OBJ_CTRL",dsp_item);

		str_name = "PMC.CTRL";
		STD__ADD_DIGITAL(str_name,dsp_item);
		LINK__VAR_DIGITAL_CTRL(xCH__OBJ_CTRL,str_name);

		dVAR__MODE = "PMC.MODE";
		STD__ADD_DIGITAL(dVAR__MODE,DSP__MODE);
	}

	// ...
	{
		CString dsp_item;
		p_variable->Get__STD_DESCRIPTION("STD_OBJ_STATUS",dsp_item);

		str_name = "OBJ.VIRTUAL.STATUS";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(xCH__OBJ_VIRTUAL_STATUS,str_name);

		str_name = "OBJ.VIRTUAL.STAGE1.STATUS";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(xCH__OBJ_VIRTUAL_STAGE1_STATUS,str_name);

		str_name = "OBJ.VIRTUAL.STAGE2.STATUS";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(xCH__OBJ_VIRTUAL_STAGE2_STATUS,str_name);

		//
		str_name = "OBJ.STATUS";
		STD__ADD_DIGITAL(str_name,dsp_item);
		LINK__VAR_DIGITAL_CTRL(xCH__OBJ_STATUS,str_name);

		str_name = "OBJ.MSG";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(xCH__OBJ_MSG,str_name);

		str_name = "OBJ.FNC.MSG";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(xCH__OBJ_FNC_MSG,str_name);

		str_name = "PMC.PROC_READY.FLAG";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(xCH__PMC_PROC_READY_FLAG,str_name);

		str_name = "ACTIVE.INIT_STATE";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__ACTIVE_INIT_STATE, str_name);

		//
		str_name = "CFG.SLIT.VALVE.CLOSE.CHECK";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,DSP__DISABLE_ENABLE,"");
		LINK__VAR_DIGITAL_CTRL(dCH__CFG_SLIT_VALVE_CLOSE_CHECK,str_name);
	}

	// PMC REQUEST ...
	{
		str_name = "WAFER_STATUS_CONVERTED_TO_PROCESSED.BY.PMC_REQ_COUNT";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__WAFER_STATUS_CONVERTED_TO_PROCESSED_BY_PMC_REQ_COUNT, str_name);
	}

	// ...
	{
		str_name = "MOVE.FLAG";
		STD__ADD_DIGITAL(str_name,DSP__NO_YES);
		LINK__VAR_DIGITAL_CTRL(xCH__MOVE_FLAG,str_name);

		str_name = "X_PICK.FLAG";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(xCH__X_PICK_FLAG,str_name);

		str_name = "X_PLACE.FLAG";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(xCH__X_PLACE_FLAG,str_name);

		str_name = "JOB.READY.FLAG";
		STD__ADD_STRING_WITH_X_OPTION(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__JOB_READY_FLAG,str_name);
	}

	// ...
	{
		CString dsp_item;
		p_variable->Get__STD_DESCRIPTION("STD_VALVE_STATUS",dsp_item);
	
		str_name = "SLIT.VLV.STS";
		STD__ADD_DIGITAL(str_name, dsp_item);
		LINK__VAR_DIGITAL_CTRL(dCH__SLIT_VLV_STS, str_name);

		str_name = "LIFT.PIN.STS";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__LIFT_PIN_STS, str_name);

		str_name = "CHUCK.STS";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__CHUCK_STS, str_name);

		str_name = "PRC.STS";
		STD__ADD_DIGITAL(str_name, DSP__PRC_STS);
		LINK__VAR_DIGITAL_CTRL(dCH__PRC_STS, str_name);

		str_name = "VAC.SNS";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__VAC_SNS, str_name);

		str_name = "RF.ON.STS";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__RF_ON_STS, str_name);
	
		str_name = "WAP.CUR.POS.STS";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__WAP_CUR_POS_STS, str_name);
	}

	// ...
	{
		int slot_limit = 5;

		str_name = "SLOT.NO";
		STD__ADD_ANALOG(str_name,"num",0,1.0,slot_limit);
		LINK__VAR_ANALOG_CTRL(xCH__SLOT_NO,str_name);
	}

	// ...
	{
		CString dsp_item;
		p_variable->Get__STD_DESCRIPTION("STD_PRESSURE_STATUS",dsp_item);

		str_name = "PRESSURE.STATUS";
		STD__ADD_DIGITAL(str_name,dsp_item);
		LINK__VAR_DIGITAL_CTRL(xCH__PRESSURE_STATUS,str_name);

		str_name = "PRESSURE.VALUE";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(xCH__PRESSURE_VALUE,str_name);
	}

	// ...
	{
		str_name = "MODULE.TIME";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(sCH__MODULE_TIME,str_name);

		//
		str_name = "TIME.ACT.START";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__TIME_ACT_START, str_name);

		str_name = "TIME.ACT.END";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__TIME_ACT_END, str_name);

		str_name = "TIME.ACT.TACK";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__TIME_ACT_TACK, str_name);

		str_name = "TIME.ACT.RESULT";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__TIME_ACT_RESULT, str_name);
	}

	// ...
	{
		str_name = "LOG.SUB_DIRECTORY";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__LOG_SUB_DIRECTORY,str_name);

		str_name = "LOG.FILE_NAME";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__LOG_FILE_NAME,str_name);

		str_name = "WFR.LOG.INFO";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__WFR_LOG_INFO, str_name);

		for(i=0;i<CFG__PMx_MAX_SLOT;i++)
		{
			str_name.Format("SLOT%1d.LOG.SUB_DIRECTORY",i+1);
			STD__ADD_STRING_WITH_COMMENT(str_name,"");
			LINK__VAR_STRING_CTRL(xCH__SLOT_LOG_SUB_DIRECTORY[i],str_name);
			
			str_name.Format("SLOT%1d.LOG.FILE_NAME",i+1);
			STD__ADD_STRING_WITH_COMMENT(str_name,"");
			LINK__VAR_STRING_CTRL(xCH__SLOT_LOG_FILE_NAME[i],str_name);
		}
	}

	// ...
	{
		str_name = "FDC.CHM.ID";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(sCH__FDC_CHM_ID,str_name);

		str_name = "FDC.RECIPE";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__FDC_RECIPE,str_name);

		str_name = "FDC.CLN.RECIPE";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__FDC_CLN_RECIPE,str_name);

		str_name = "FDC.PRE.RECIPE";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__FDC_PRE_RECIPE,str_name);

		str_name = "FDC.LOTID";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__FDC_LOTID,str_name);
	}

	// ...
	{
		str_name = "CJOBID";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__CJOBID,str_name);

		str_name = "PJOBID";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__PJOBID,str_name);

		str_name = "RECIPE";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__RECIPE,str_name);

		str_name = "LOTID";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__LOTID,str_name);

		str_name = "MATERIALID";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__MATERIAL_ID,str_name);

		str_name = "MATERIAL_SIZE";
		STD__ADD_STRING_WITH_X_OPTION(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__MATERIAL_SIZE,str_name);
	}

	// ...
	{
		str_name = "LPx.CID";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(xCH__LPx_CID,str_name);

		str_name = "LPx.PORT_ID";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(xCH__LPx_PORT_ID,str_name);

		str_name = "LPx.SLOT_ID";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(xCH__LPx_SLOT_ID,str_name);
	}

	// ...
	{
		str_name = "PRE.LOTID";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(xCH_PRE__LOTID,str_name);
	
		str_name = "PRE.MATERIALID";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(xCH_PRE__MATERIAL_ID,str_name);
	
		str_name = "PRE.LPx.PORTID";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(xCH_PRE__LPx_PORT_ID,str_name);
	
		str_name = "PRE.LPx.SLOTID";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(xCH_PRE__LPx_SLOT_ID,str_name);
	
		str_name = "PRE.LPx.CID";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(xCH_PRE__LPx_CID,str_name);
	}

	// ...
	{
		str_name = "PMC_SEND.APC.APPLY";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__PMC_SEND_APC_APPLY, str_name);
	
		str_name = "PMC_SEND.APC.DATA";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__PMC_SEND_APC_DATA, str_name);
	}

	for(i=0;i<CFG__PMx_MAX_SLOT;i++)
	{
		str_name.Format("SLOT%1d.RECIPE",i+1);
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(xCH__SLOT_RECIPE[i],str_name);
		
		str_name.Format("SLOT%1d.LOTID",i+1);
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(xCH__SLOT_LOTID[i],str_name);

		str_name.Format("SLOT%1d.MATERIALID",i+1);
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(xCH__SLOT_MATERIALID[i],str_name);
	
		str_name.Format("SLOT%1d.LPx.CID",i+1);
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(xCH__SLOT_LPx_CID[i],str_name);
	
		str_name.Format("SLOT%1d.LPx.PORT_ID",i+1);
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(xCH__SLOT_LPx_PORT_ID[i],str_name);
	
		str_name.Format("SLOT%1d.LPx.SLOT_ID",i+1);
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(xCH__SLOT_LPx_SLOT_ID[i],str_name);
	}

	// ...
	{
		str_name = "PROCESS.FLAG";
		STD__ADD_STRING_WITH_COMMENT(str_name,"YES NO");
		LINK__VAR_STRING_CTRL(xCH__PROCESS_FLAG,str_name);

		str_name = "TOTAL.PRC_TIME";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(xCH__TOTAL_PRC_TIME,str_name);

		str_name = "CHANGE.PRC_TIME";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(xCH__CHANGE_PRC_TIME,str_name);

		str_name = "CURRENT.PRC_TIME";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(xCH__CURRENT_PRC_TIME,str_name);

		str_name = "PROCESS.ASYNC_TIME";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(xCH__PROCESS_ASYNC_TIME,str_name);

		str_name = "PROCESS.PERCENT";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(xCH__PROCESS_PERCENT,str_name);

		str_name = "PROCESS.STATUS";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(xCH__PROCESS_STATUS,str_name);

		//
		str_name = "PRC.TIME.COUNT";
		STD__ADD_ANALOG(str_name, "sec", 0, 0, 999999);
		LINK__VAR_ANALOG_CTRL(xCH__PRC_TIME_COUNT, str_name);
	}

	// ...
	{
		CString dsp_slot_sts;
		p_variable->Get__STD_DESCRIPTION("STD_SLOT_STATUS",dsp_slot_sts);

		for(i=0;i<CFG__PMx_MAX_SLOT;i++)
		{
			str_name.Format("SLOT%002d.STATUS",i+1);
			STD__ADD_DIGITAL(str_name,dsp_slot_sts);
			LINK__VAR_DIGITAL_CTRL(xCH__SLOT_STATUS[i],str_name);

			str_name.Format("SLOT%002d.TITLE",i+1);
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(xCH__SLOT_TITLE[i],str_name);
		}

		str_name = "SVID.SLOT_DATA";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(xCH__SVID_SLOT_DATA,str_name);
	}

	// ...
	{
		str_name = "ACC.MATERIAL.COUNT";
		STD__ADD_STRING_WITH_X_OPTION(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__ACC_MATERIAL_COUNT,str_name);

		str_name = "ACC.PROCESSED.COUNT";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"num",0,0,9999999,"");
		LINK__VAR_ANALOG_CTRL(aCH__ACC_PROCESSED_COUNT,str_name);

		str_name = "ACC.ABORTED.COUNT";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"num",0,0,9999999,"");
		LINK__VAR_ANALOG_CTRL(aCH__ACC_ABORTED_COUNT,str_name);

		//
		str_name = "RCP.PAS.MATERIAL.COUNT";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"num",0,0,9999999,"");
		LINK__VAR_ANALOG_CTRL(xCH__RCP_PAS__MATERIAL_COUNT,str_name);

		str_name = "RCP.NP.MATERIAL.COUNT";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"num",0,0,9999999,"");
		LINK__VAR_ANALOG_CTRL(xCH__RCP_NP__MATERIAL_COUNT,str_name);

		str_name = "RCP.ACT.MATERIAL.COUNT";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"num",0,0,9999999,"");
		LINK__VAR_ANALOG_CTRL(xCH__RCP_ACT__MATERIAL_COUNT,str_name);

		str_name = "RCP.OTHER.MATERIAL.COUNT";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"num",0,0,9999999,"");
		LINK__VAR_ANALOG_CTRL(xCH__RCP_OTHER__MATERIAL_COUNT,str_name);

		str_name = "RCP.TOTAL.MATERIAL.COUNT";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"num",0,0,9999999,"");
		LINK__VAR_ANALOG_CTRL(xCH__RCP_TOTAL__MATERIAL_COUNT,str_name);

		//
		str_name = "UTILITY.CFG_TO_CSV.REQ";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(xCH__UTILITY_CFG_TO_CSV_REQ,str_name);
	}

	// ...
	{
		str_name = "CFG.PMC.USE";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,DSP__DISABLE_ENABLE,"");
		LINK__VAR_DIGITAL_CTRL(xCH_CFG__PMC_USE,str_name);

		str_name = "CFG.PROCESS_TYPE.BONDING.FLAG";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"NO YES","");
		LINK__VAR_DIGITAL_CTRL(xCH__PROCESS_TYPE__BONDING_FLAG,str_name);

		// 
		str_name = "REQ.GOTO_MAINT";
		STD__ADD_STRING_WITH_COMMENT(str_name, "");
		LINK__VAR_STRING_CTRL(sCH_REQ__GOTO_MAINT, str_name);

		str_name = "LINK_REQ.GOTO_MAINT";
		STD__ADD_DIGITAL(str_name, "NO YES");
		LINK__VAR_DIGITAL_CTRL(dCH_LINK_REQ__GOTO_MAINT, str_name);

		//
		str_name = "NEXT.PROCESS";
		STD__ADD_STRING_WITH_COMMENT(str_name, "");
		LINK__VAR_STRING_CTRL(xCH__NEXT_PROCESS, str_name);

		str_name = "NEXT.PROCESS.REQ";
		STD__ADD_STRING_WITH_COMMENT(str_name, "");
		LINK__VAR_STRING_CTRL(xCH__NEXT_PROCESS_REQ, str_name);

		str_name = "NEXT.PROCESS.END.FLAG";
		STD__ADD_STRING_WITH_COMMENT(str_name, "");
		LINK__VAR_STRING_CTRL(xCH__NEXT_PROCESS_END_FLAG, str_name);

		str_name = "CFG.AUTO.MATERIAL_COUNT.FLAG";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,DSP__DISABLE_ENABLE, "");
		LINK__VAR_DIGITAL_CTRL(xCH_CFG__AUTO_MATERIAL_COUNT_FLAG, str_name);

		//
		str_name = "PRC.PRE.RECIPE";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__PRC_PRE_RECIPE, str_name);

		//
		str_name = "CFG.CLEAN.MODE";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(xCH_CFG__CLEAN_MODE,str_name);

		str_name = "CFG.CLEAN.USE";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,DSP__DISABLE_ENABLE,"");
		LINK__VAR_DIGITAL_CTRL(xCH_CFG__CLEAN_USE,str_name);

		str_name = "CFG.CLEAN.FREQUENCY";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"num",0,0,10000000,"");
		LINK__VAR_ANALOG_CTRL(xCH_CFG__CLEAN_FREQUENCY,str_name);

		str_name = "CFG.CLEAN.COUNT";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"num",0,0,10000000,"");
		LINK__VAR_ANALOG_CTRL(xCH_CFG__CLEAN_COUNT,str_name);

		str_name = "CFG.CLEAN.RECIPE";
		STD__ADD_STRING_WITH_X_OPTION(str_name,"");
		LINK__VAR_STRING_CTRL(xCH_CFG__CLEAN_RECIPE,str_name);

		str_name = "CUR.CLEAN.WAFER_COUNT";
		STD__ADD_STRING(str_name,"");
		LINK__VAR_STRING_CTRL(sCH_CUR__CLEAN_WAFER_COUNT, str_name);

		//
		str_name = "SYSTEM.TOTAL.MATERIAL.COUNT";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"num",0,-1,10000000,"");
		LINK__VAR_ANALOG_CTRL(xCH__SYSTEM_TOTAL_MATERIAL_COUNT,str_name);

		str_name = "LAYER.CLEAN.RECIPE";
		STD__ADD_STRING_WITH_X_OPTION(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__LAYER_CLEAN_RECIPE,str_name);

		str_name = "POST.CLEAN.RECIPE";
		STD__ADD_STRING_WITH_X_OPTION(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__POST_CLEAN_RECIPE,str_name);

		//
		str_name = "CFG.DUMMY.CLEAN.USE";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,DSP__DISABLE_ENABLE,"");
		LINK__VAR_DIGITAL_CTRL(xCH_CFG__DUMMY_CLEAN_USE,str_name);

		str_name = "CFG.DUMMY.CLEAN.RECIPE";
		STD__ADD_STRING_WITH_X_OPTION(str_name,"");
		LINK__VAR_STRING_CTRL(xCH_CFG__DUMMY_CLEAN_RECIPE,str_name);

		//
		str_name = "CFG.DUMMY.PROC.RECIPE";
		STD__ADD_STRING_WITH_X_OPTION(str_name,"");
		LINK__VAR_STRING_CTRL(xCH_CFG__DUMMY_PROC_RECIPE,str_name);

		str_name = "CFG.DUMMY.CLN.RECIPE";
		STD__ADD_STRING_WITH_X_OPTION(str_name,"");
		LINK__VAR_STRING_CTRL(xCH_CFG__DUMMY_CLN_RECIPE,str_name);

		str_name = "CFG.DUMMY.PRE.RECIPE";
		STD__ADD_STRING_WITH_X_OPTION(str_name,"");
		LINK__VAR_STRING_CTRL(xCH_CFG__DUMMY_PRE_RECIPE,str_name);

		str_name = "PMC.IDLE.TIME";
		STD__ADD_ANALOG_WITH_COMMENT(str_name,"sec",0,0,100000,"");
		LINK__VAR_ANALOG_CTRL(xCH__PMC_IDLE_TIME,str_name);

		str_name = "PMC.FIRST.WAFER.FLAG";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__PMC_FIRST_WAFER_FLAG,str_name);

		//
		str_name = "CFG.CLEAN.SYS_IDLE.REF_DELAY_TIME";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",0,0,9999,"");
		LINK__VAR_ANALOG_CTRL(xCH_CFG__CLEAN_SYS_IDLE_REF_DELAY_TIME,str_name);

		str_name = "CFG.CLEAN.FIRST.COUNT";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"num",0,0,10000000,"");
		LINK__VAR_ANALOG_CTRL(xCH_CFG__CLEAN_FIRST_COUNT,str_name);

		str_name = "CFG.CLEAN.SECOND.COUNT";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"num",0,0,10000000,"");
		LINK__VAR_ANALOG_CTRL(xCH_CFG__CLEAN_SECOND_COUNT,str_name);

		//
		str_name = "CFG.CLEAN.RF.USE";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,DSP__DISABLE_ENABLE,"");
		LINK__VAR_DIGITAL_CTRL(xCH_CFG__CLEAN_RF_USE,str_name);

		str_name = "CFG.CLEAN.RF.REF_TIME";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"hour",0,0,10000,"");
		LINK__VAR_ANALOG_CTRL(xCH_CFG__CLEAN_RF_REF_TIME,str_name);

		str_name = "CFG.CLEAN.RF.CUR_TIME";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"hour",0,0,10000,"");
		LINK__VAR_ANALOG_CTRL(xCH_CFG__CLEAN_RF_CUR_TIME,str_name);

		str_name = "CFG.CLEAN.RF.RECIPE";
		STD__ADD_STRING_WITH_X_OPTION(str_name,"");
		LINK__VAR_STRING_CTRL(xCH_CFG__CLEAN_RF_RECIPE,str_name);
	}

	// CTC LEAK_CHECK ...
	{
		str_name = "CTC.LEAK_CHECK.OVER_PUMPING.TIME";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "sec", 0, 0.0, 120,"");
		LINK__VAR_ANALOG_CTRL(aCH_CFG__CTC_LEAK_CHECK_OVER_PUMPING_TIME, str_name);

		str_name = "CTC.LEAK_CHECK.STABLE.TIME";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "sec", 0, 0.0, 120,"");
		LINK__VAR_ANALOG_CTRL(aCH_CFG__CTC_LEAK_CHECK_STABLE_TIME, str_name);

		str_name = "CTC.LEAK_CHECK.TIME.MIN";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "min", 0, 1.0, 7.0,"");
		LINK__VAR_ANALOG_CTRL(aCH_CFG__CTC_LEAK_CHECK_TIME_MIN, str_name);

		//
		str_name = "CTC.LEAK_CHECK.MESSAGE";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__CTC_LEAK_CHECK_MESSAGE, str_name);

		str_name = "CTC.LEAK_CHECK.TIME.COUNT";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__CTC_LEAK_CHECK_TIME_COUNT, str_name);
	}
	// AUTO LEAK_CHECK ...
	{
		str_name = "CFG.AUTO.LEAK_CHECK.USE_MODE";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,DSP__DISABLE_ENABLE,"");
		LINK__VAR_DIGITAL_CTRL(xCH_CFG__AUTO_LEAK_CHECK_USE_MODE,str_name);

		str_name = "CFG.AUTO.LEAK_CHECK.REF_TIME";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",0,0.0,10000000,"");
		LINK__VAR_ANALOG_CTRL(xCH_CFG__AUTO_LEAK_CHECK_REF_TIME,str_name);
		
		str_name = "CFG.AUTO.LEAK_CHECK.CUR_TIME";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",0,0.0,10000000,"");
		LINK__VAR_ANALOG_CTRL(xCH_CFG__AUTO_LEAK_CHECK_CUR_TIME,str_name);
		
		str_name = "CFG.AUTO.LEAK_CHECK.MODE";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,DSP__LEAK_CHECK_MODE,"");
		LINK__VAR_DIGITAL_CTRL(xCH_CFG__AUTO_LEAK_CHECK_MODE,str_name);

		//
		str_name = "AUTO.LEAK_CHECK.RUN_DATE";
		STD__ADD_STRING_WITH_X_OPTION(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__AUTO_LEAK_CHECK_RUN_DATE,str_name);

		str_name = "AUTO.LEAK_CHECK.RUN_FLAG";
		STD__ADD_STRING_WITH_X_OPTION(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__AUTO_LEAK_CHECK_RUN_FLAG,str_name);

		str_name = "AUTO.LEAK_CHECK.RUN_DAY";
		STD__ADD_STRING_WITH_X_OPTION(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__AUTO_LEAK_CHECK_RUN_DAY,str_name);

		str_name = "AUTO.LEAK_CHECK.RUN_COMPLETE";
		STD__ADD_STRING_WITH_X_OPTION(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__AUTO_LEAK_CHECK_RUN_COMPLETE,str_name);

		//
		str_name = "LEAK_CHECK.IDLE.REF.TIME";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",0,0,36000,"");
		LINK__VAR_ANALOG_CTRL(xCH__LEAK_CHECK_IDLE_REF_TIME, str_name);

		str_name = "LEAK_CHECK.IDLE.CUR.TIME";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(xCH__LEAK_CHECK_IDLE_CUR_TIME, str_name);

		//
		str_name = "LEAK_CHECK.IDLE.REF.COUNT";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"num",0,0,100,"");
		LINK__VAR_ANALOG_CTRL(xCH__LEAK_CHECK_IDLE_REF_COUNT, str_name);

		str_name = "LEAK_CHECK.IDLE.CUR.COUNT";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(xCH__LEAK_CHECK_IDLE_CUR_COUNT, str_name);

		//
		str_name = "LEAK_CHECK.IDLE.RUN.DATE";
		STD__ADD_STRING_WITH_X_OPTION(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__LEAK_CHECK_IDLE_RUN_DATE, str_name);

		//
		str_name = "LEAK_CHECK.START_TIME.HOUR";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"hour",0,0,23,"");
		LINK__VAR_ANALOG_CTRL(xCH__LEAK_CHECK_START_TIME_HOUR, str_name);

		str_name = "LEAK_CHECK.START_TIME.MINUTE";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"min",0,0,59,"");
		LINK__VAR_ANALOG_CTRL(xCH__LEAK_CHECK_START_TIME_MINUTE, str_name);

		str_name = "LEAK_CHECK.START_TIME.SECOND";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",0,0,59,"");
		LINK__VAR_ANALOG_CTRL(xCH__LEAK_CHECK_START_TIME_SECOND, str_name);

		//
		str_name = "LEAK_CHECK.END_TIME.HOUR";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"hour",0,0,23,"");
		LINK__VAR_ANALOG_CTRL(xCH__LEAK_CHECK_END_TIME_HOUR, str_name);

		str_name = "LEAK_CHECK.END_TIME.MINUTE";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"min",0,0,59,"");
		LINK__VAR_ANALOG_CTRL(xCH__LEAK_CHECK_END_TIME_MINUTE, str_name);

		str_name = "LEAK_CHECK.END_TIME.SECOND";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",0,0,59,"");
		LINK__VAR_ANALOG_CTRL(xCH__LEAK_CHECK_END_TIME_SECOND, str_name);

		//
		str_name = "LEAK_CHECK.WARNING.USE_MODE";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,DSP__DISABLE_ENABLE,"");
		LINK__VAR_DIGITAL_CTRL(xCH__LEAK_CHECK_WARNING_USE_MODE, str_name);
		
		str_name = "LEAK_CHECK.WARNING.TIMEOUT";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",0,0,99999,"");
		LINK__VAR_ANALOG_CTRL(xCH__LEAK_CHECK_WARNING_TIMEOUT, str_name);
		
		str_name = "LEAK_CHECK.WARNING.CUR_TIME";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(xCH__LEAK_CHECK_WARNING_CUR_TIME, str_name);
	}

	// AUTO_PM ...
	{
		str_name = "CFG.AUTO_PM.USE_MODE";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,DSP__DISABLE_ENABLE,"");
		LINK__VAR_DIGITAL_CTRL(xCH_CFG__AUTO_PM_USE_MODE,str_name);

		str_name = "CFG.AUTO_PM.REF_TIME";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",0,0.0,10000000,"");
		LINK__VAR_ANALOG_CTRL(xCH_CFG__AUTO_PM_REF_TIME,str_name);

		str_name = "CFG.AUTO_PM.CUR_TIME";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",0,0.0,10000000,"");
		LINK__VAR_ANALOG_CTRL(xCH_CFG__AUTO_PM_CUR_TIME,str_name);

		str_name = "CFG.AUTO_PM.MODE";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,DSP__AUTO_PM_MODE,"");
		LINK__VAR_DIGITAL_CTRL(xCH_CFG__AUTO_PM_MODE,str_name);

		//
		str_name = "AUTO_PM.RUN_DATE";
		STD__ADD_STRING_WITH_X_OPTION(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__AUTO_PM_RUN_DATE,str_name);

		str_name = "AUTO_PM.RUN_FLAG";
		STD__ADD_STRING_WITH_X_OPTION(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__AUTO_PM_RUN_FLAG,str_name);

		str_name = "AUTO_PM.RUN_DAY";
		STD__ADD_STRING_WITH_X_OPTION(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__AUTO_PM_RUN_DAY,str_name);

		str_name = "AUTO_PM.RUN_COMPLETE";
		STD__ADD_STRING_WITH_X_OPTION(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__AUTO_PM_RUN_COMPLETE,str_name);

		//
		str_name = "AUTO_PM.IDLE.REF.TIME";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",0,0,36000,"");
		LINK__VAR_ANALOG_CTRL(xCH__AUTO_PM_IDLE_REF_TIME, str_name);

		str_name = "AUTO_PM.IDLE.CUR.TIME";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(xCH__AUTO_PM_IDLE_CUR_TIME, str_name);

		//
		str_name = "AUTO_PM.IDLE.REF.COUNT";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"num",0,0,100,"");
		LINK__VAR_ANALOG_CTRL(xCH__AUTO_PM_IDLE_REF_COUNT, str_name);

		str_name = "AUTO_PM.IDLE.CUR.COUNT";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(xCH__AUTO_PM_IDLE_CUR_COUNT, str_name);

		//
		str_name = "AUTO_PM.IDLE.RUN.DATE";
		STD__ADD_STRING_WITH_X_OPTION(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__AUTO_PM_IDLE_RUN_DATE, str_name);

		//
		str_name = "AUTO_PM.START_TIME.HOUR";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"hour",0,0,23,"");
		LINK__VAR_ANALOG_CTRL(xCH__AUTO_PM_START_TIME_HOUR, str_name);

		str_name = "AUTO_PM.START_TIME.MINUTE";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"min",0,0,59,"");
		LINK__VAR_ANALOG_CTRL(xCH__AUTO_PM_START_TIME_MINUTE, str_name);

		str_name = "AUTO_PM.START_TIME.SECOND";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",0,0,59,"");
		LINK__VAR_ANALOG_CTRL(xCH__AUTO_PM_START_TIME_SECOND, str_name);

		//
		str_name = "AUTO_PM.END_TIME.HOUR";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"hour",0,0,23,"");
		LINK__VAR_ANALOG_CTRL(xCH__AUTO_PM_END_TIME_HOUR, str_name);

		str_name = "AUTO_PM.END_TIME.MINUTE";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"min",0,0,59,"");
		LINK__VAR_ANALOG_CTRL(xCH__AUTO_PM_END_TIME_MINUTE, str_name);

		str_name = "AUTO_PM.END_TIME.SECOND";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",0,0,59,"");
		LINK__VAR_ANALOG_CTRL(xCH__AUTO_PM_END_TIME_SECOND, str_name);

		//
		str_name = "AUTO_PM.WARNING.USE_MODE";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,DSP__DISABLE_ENABLE,"");
		LINK__VAR_DIGITAL_CTRL(xCH__AUTO_PM_WARNING_USE_MODE, str_name);

		str_name = "AUTO_PM.WARNING.TIMEOUT";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",0,0,99999,"");
		LINK__VAR_ANALOG_CTRL(xCH__AUTO_PM_WARNING_TIMEOUT, str_name);

		str_name = "AUTO_PM.WARNING.CUR_TIME";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(xCH__AUTO_PM_WARNING_CUR_TIME, str_name);
	}

	// ...
	{
		str_name = "STEP.START";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__STEP_START,str_name);

		str_name = "STEP.END";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__STEP_END,str_name);
	}

	// ...
	{
		str_name = "PROC_INFO.TOTAL_PROC_TIME";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__PROC_INFO__TOTAL_PROC_TIME,str_name);

		str_name = "PROC_INFO.CUR_PROC_TIME";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__PROC_INFO__CUR_PROC_TIME,str_name);

		str_name = "PROC_INFO.TOTAL_STEP_COUNT";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__PROC_INFO__TOTAL_STEP_COUNT,str_name);

		str_name = "PROC_INFO.CUR_STEP_COUNT";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__PROC_INFO__CUR_STEP_COUNT,str_name);

		str_name = "PROC_INFO.STEP_TOTAL_TIME";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__PROC_INFO__STEP_TOTAL_TIME,str_name);

		str_name = "PROC_INFO.STEP_CUR_TIME";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__PROC_INFO__STEP_CUR_TIME,str_name);

		str_name = "PROC_INFO.STEP_MSG";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__PROC_INFO__STEP_MSG,str_name);

		str_name = "PROC_INFO.PROCESS_STATUS";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__PROC_INFO__PROCESS_STATUS,str_name);
	}

	// ...
	{
		str_name = "RMS.ID";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__RMS_ID,str_name);

		str_name = "RMS.ACT";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__RMS_ACT,str_name);

		str_name = "RMS.FILE_TYPE";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__RMS_FILE_TYPE,str_name);

		str_name = "RMS.FILE_NAME";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__RMS_FILE_NAME,str_name);

		str_name = "RMS.CREATE_TIME";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__RMS_CREATE_TIME,str_name);

		str_name = "RMS.UPDATE_TIME";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__RMS_UPDATE_TIME,str_name);
	}

	// ...
	{
		str_name = "MATERIAL_CLEAR.REQ";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__MATERIAL_CLEAR_REQ,str_name);

		str_name = "MATERIAL_CLEAR.SLOT";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__MATERIAL_CLEAR_SLOT,str_name);
	}

	// ...
	{
		str_name = "LPx_TO_PMC.WAFER.COUNT";
		STD__ADD_STRING_WITH_X_OPTION(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__LPx_WAFER_COUNT,str_name);

		for(i=0;i<CFG__LPx_SIZE;i++)
		{
			str_name.Format("CFG.LP%1d.WAFER.COUNT",i+1);
			STD__ADD_STRING_WITH_X_OPTION(str_name,"");
			LINK__VAR_STRING_CTRL(xCH_CFG__LPx_WAFER_COUNT[i],str_name);
		}

		//
		str_name = "CFG.2st_SLOT.EMPTY.CHECK";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,DSP__DISABLE_ENABLE,"");
		LINK__VAR_DIGITAL_CTRL(xCH__CFG_2st_SLOT_EMPTY_CHECK,str_name);

		//
		str_name = "ALARM.LOG.MODE";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"MATERIAL_ABORTED  ALL_ALARM","");
		LINK__VAR_DIGITAL_CTRL(xCH__ALARM_LOG_MODE,str_name);

		//
		str_name = "SYS.IDLE.TIME";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",1,0.0,9999999,"");
		LINK__VAR_ANALOG_CTRL(xCH__SYS_IDLE_TIME,str_name);
		
		// ...
		str_name = "ECS.MODE";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__ECS_MODE,str_name);
	}

	// ECS TRANSFER ...
	{
		str_name = "ECS.PIO_FLAG";
		STD__ADD_STRING_WITH_X_OPTION(str_name,"");
		LINK__VAR_STRING_CTRL(xCH_TRANSFER__ECS_PIO_FLAG,str_name);

		str_name = "CFG.USE.FLAG";
		STD__ADD_STRING_WITH_X_OPTION(str_name,"");
		LINK__VAR_STRING_CTRL(xCH_TRANSFER__CFG_USE_FLAG,str_name);

		str_name = "ECS.EXCHANGE.FLAG";
		STD__ADD_STRING_WITH_X_OPTION(str_name,"");
		LINK__VAR_STRING_CTRL(xCH_TRANSFER__ECS_EXCHANGE_FLAG,str_name);

		str_name = "ECS.EXIT.FLAG";
		STD__ADD_STRING_WITH_X_OPTION(str_name,"");
		LINK__VAR_STRING_CTRL(xCH_TRANSFER__ECS_EXIT_FLAG,str_name);
	}

	// EXCEPTION CTRL ...
	{
		CString var_name;

		//
		str_name = "ABORT_LOCK.FLAG";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__ABORT_LOCK_FLAG, str_name);

		str_name = "END_LOCK.FLAG";	
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__END_LOCK_FLAG, str_name);
		
		str_name = "SKIP_LOCK.FLAG";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__SKIP_LOCK_FLAG, str_name);
		
		str_name = "JUMP_LOCK.FLAG";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__JUMP_LOCK_FLAG, str_name);

		//
		str_name = "EXCEPTION.ACT";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__EXCEPTION_ACT, str_name);

		str_name = "EXCEPTION.MSG";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__EXCEPTION_MSG, str_name);

		//
		str_name = "APP.FLAG.ABORT_BY_USER";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__APP_FLAG_ABORT_BY_USER, str_name);

		//
		str_name = "FDC.DA.RT.OFFSET";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__FDC_DA_RT_OFFSET, str_name);

		str_name = "FDC.DA.R.OFFSET";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__FDC_DA_R_OFFSET, str_name);

		str_name = "FDC.DA.T.OFFSET";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__FDC_DA_T_OFFSET, str_name);
	}

	// JOB READY ...
	{
		str_name = "PRE_INFO.JOB_START_READY.PORTID";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__PRE_INFO_JOB_START_READY_PORTID, str_name);

		str_name = "PRE_INFO.JOB_START_READY.JOBID";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__PRE_INFO_JOB_START_READY_JOBID, str_name);

		//
		str_name = "REQ_INFO.JOB_START_READY.PORTID";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__REQ_INFO_JOB_START_READY_PORTID, str_name);

		str_name = "REQ_INFO.JOB_START_READY.JOBID";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__REQ_INFO_JOB_START_READY_JOBID, str_name);
	}

	// ...
	{
		str_name = "PARA.TEMPLATE_FILE";
		STD__ADD_STRING_WITH_X_OPTION(str_name, "");
		LINK__VAR_STRING_CTRL(sCH__PARA_TEMPLATE_FILE, str_name);
	}

	// CFG.ALARM_POST.PMC_DUMMY_BY_CTC
	{
		str_name = "CFG.ALARM_POST.PMC_DUMMY_BY_CTC";
		STD__ADD_DIGITAL(str_name, "NO YES");
		LINK__VAR_DIGITAL_CTRL(dCH__CFG_ALARM_POST_PMC_DUMMY_BY_CTC, str_name);
	}

	// PHY_PMC - PROCESS INFO ...
	{
		str_name = "PHY_PMC.PROC.LOTID";
		STD__ADD_STRING(str_name);
		// LINK__VAR_STRING_CTRL(sCH__PHY_PMC_PROC_LOTID, str_name);

		str_name = "PHY_PMC.PROC.RECIPEID";
		STD__ADD_STRING(str_name);
		// LINK__VAR_STRING_CTRL(sCH__PHY_PMC_PROC_RECIPEID, str_name);
	
		str_name = "PHY_PMC.PROC.CUR_TIME";
		STD__ADD_STRING(str_name);
		// LINK__VAR_STRING_CTRL(sCH__PHY_PMC_PROC_CUR_TIME, str_name);

		str_name = "PHY_PMC.PROC.TOTAL_TIME";
		STD__ADD_STRING(str_name);
		// LINK__VAR_STRING_CTRL(sCH__PHY_PMC_PROC_TOTAL_TIME, str_name);

		str_name = "PHY_PMC.PROC.CUR_STEP";
		STD__ADD_STRING(str_name);
		// LINK__VAR_STRING_CTRL(sCH__PHY_PMC_PROC_CUR_STEP, str_name);

		str_name = "PHY_PMC.PROC.TOTAL_STEP";
		STD__ADD_STRING(str_name);
		// LINK__VAR_STRING_CTRL(sCH__PHY_PMC_PROC_TOTAL_STEP, str_name);

		//
		str_name = "PHY_PMC.STEP.CUR_TIME";
		STD__ADD_STRING(str_name);
		// LINK__VAR_STRING_CTRL(sCH__PHY_PMC_STEP_CUR_TIME, str_name);

		str_name = "PHY_PMC.STEP.TOTAL_TIME";
		STD__ADD_STRING(str_name);
		// LINK__VAR_STRING_CTRL(sCH__PHY_PMC_STEP_TOTAL_TIME, str_name);

		str_name = "PHY_PMC.STEP.CUR_NAME";
		STD__ADD_STRING(str_name);
		// LINK__VAR_STRING_CTRL(sCH__PHY_PMC_STEP_CUR_NAME, str_name);

		str_name = "PHY_PMC.STEP.CUR_TYPE";
		STD__ADD_STRING(str_name);
		// LINK__VAR_STRING_CTRL(sCH__PHY_PMC_STEP_CUR_TYPE, str_name);

		str_name = "PHY_PMC.STEP.EPD_DETECT_TIME";
		STD__ADD_STRING(str_name);
		// LINK__VAR_STRING_CTRL(sCH__PHY_PMC_STEP_EPD_DETECT_TIME, str_name);

		str_name = "PHY_PMC.STEP.EPD_OVERETCH_TIME";
		STD__ADD_STRING(str_name);
		// LINK__VAR_STRING_CTRL(sCH__PHY_PMC_STEP_EPD_OVERETCH_TIME, str_name);

		str_name = "PHY_PMC.STEP.EPD_OVERETCH_PERCENT";
		STD__ADD_STRING(str_name);
		// LINK__VAR_STRING_CTRL(sCH__PHY_PMC_STEP_EPD_OVERETCH_PERCENT, str_name);

		str_name = "PHY_PMC.STEP.OBJ_MAIN_MSG";
		STD__ADD_STRING(str_name);
		// LINK__VAR_STRING_CTRL(sCH__PHY_PMC_STEP_OBJ_MAIN_MSG, str_name);

		str_name = "PHY_PMC.STEP.OBJ_SUB_MSG";
		STD__ADD_STRING(str_name);
		// LINK__VAR_STRING_CTRL(sCH__PHY_PMC_STEP_OBJ_SUB_MSG, str_name);

		//
		str_name = "PHY_PMC.SENSOR.CHM.BALLAST";
		STD__ADD_STRING(str_name);

		str_name = "PHY_PMC.SENSOR.PIN.UP";
		STD__ADD_STRING(str_name);

		str_name = "PHY_PMC.SENSOR.PIN.MIDDLE";
		STD__ADD_STRING(str_name);

		str_name = "PHY_PMC.SENSOR.PIN.DOWN";
		STD__ADD_STRING(str_name);

		str_name = "PHY_PMC.SENSOR.ATM";
		STD__ADD_STRING(str_name);

		str_name = "PHY_PMC.SENSOR.VAC";
		STD__ADD_STRING(str_name);

		str_name = "PHY_PMC.SENSOR.500mTorr";
		STD__ADD_STRING(str_name);

		str_name = "PHY_PMC.SENSOR.100mTorr";
		STD__ADD_STRING(str_name);
		
		//
		str_name = "PHY_PMC.APC.CUR.PRESSURE.mTORR";
		STD__ADD_STRING(str_name);

		str_name = "PHY_PMC.APC.CUR.POSITION.GET";
		STD__ADD_STRING(str_name);
  
		str_name = "PHY_PMC.APC.CUR.POSITION.PER";
		STD__ADD_STRING(str_name);
	}

	// ...
	{
		str_name = "SCH_TEST.CFG.PMC_PROC_PRE_SEC";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "sec", 0, 1, 9999, "");
		LINK__VAR_ANALOG_CTRL(aCH__SCH_TEST_CFG_PMC_PROC_PRE_SEC, str_name);

		str_name = "SCH_TEST.CFG.PMC_PROC_MAIN_SEC";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "sec", 0, 1, 9999, "");
		LINK__VAR_ANALOG_CTRL(aCH__SCH_TEST_CFG_PMC_PROC_MAIN_SEC, str_name);

		str_name = "SCH_TEST.CFG.PMC_PROC_POST_SEC";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "sec", 0, 1, 9999, "");
		LINK__VAR_ANALOG_CTRL(aCH__SCH_TEST_CFG_PMC_PROC_POST_SEC, str_name);

		//
		str_name = "SCH_TEST.CFG.PMC_JOB_START_READY_SEC";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "sec", 0, 1, 9999, "");
		LINK__VAR_ANALOG_CTRL(aCH__SCH_TEST_CFG_PMC_JOB_START_READY_SEC, str_name);

		str_name = "SCH_TEST.CFG.PMC_JOB_END_READY_SEC";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "sec", 0, 1, 9999, "");
		LINK__VAR_ANALOG_CTRL(aCH__SCH_TEST_CFG_PMC_JOB_END_READY_SEC, str_name);

		//
		str_name = "SCH_TEST.CFG.PMC_IDLE_START_SEC";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "sec", 0, 1, 9999, "");
		LINK__VAR_ANALOG_CTRL(aCH__SCH_TEST_CFG_PMC_IDLE_START_SEC, str_name);

		//
		str_name = "SCH_TEST.CFG.PMC_LEAK_CHECK_SEC";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "sec", 0, 1, 9999, "");
		LINK__VAR_ANALOG_CTRL(aCH__SCH_TEST_CFG_PMC_LEAK_CHECK_SEC, str_name);
		
		str_name = "SCH_TEST.CFG.PMC_AUTO_PM_SEC";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "sec", 0, 1, 9999, "");
		LINK__VAR_ANALOG_CTRL(aCH__SCH_TEST_CFG_PMC_AUTO_PM_SEC, str_name);
	}

	// ...
	{
		p_variable->Add__MONITORING_PROC(3.0, MON_ID__ALL_STATE);
		p_variable->Add__MONITORING_PROC(3.0, MON_ID__FA_STEP_REPORT);
	}
	return 1;
}

int CObj_Phy__PMC_STD::__DEFINE__ALARM(p_alarm)
{
	DECLARE__ALARM;

	// ...
	CString title;
	title.Format("%s : ",sObject_Name);

	int alarm_id;
	CString alarm_title;
	CString alarm_msg;
	CString alarm_bff;
	CStringArray l_act;

	// ...
	ALARM_DEFINE__MODULE_OBJECT;

	// ...
	{
		alarm_title  = title;
		alarm_title += "OBJECT MODE : UNKNOWN";

		alarm_msg = "";

		l_act.RemoveAll();
		l_act.Add("ABORT");
	
		ADD__ALARM_EX(ALID__OBJECT_MODE_UNKNOWN,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_title  = title;
		alarm_title += "PMC OFFLINE";

		alarm_msg.Format("Object Name : %s\n",sObject_Name);
		alarm_msg += "PMC OFFLINE";

		l_act.RemoveAll();
		l_act.Add(ALM_ACT__CHECK);
		l_act.Add(ALM_ACT__RECONNECT);

		ADD__ALARM_EX(ALID__PMC_OFFLINE,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_title  = title;
		alarm_title += "VENT ERROR";

		alarm_msg = "";

		l_act.RemoveAll();
		l_act.Add("ABORT");
		l_act.Add("RETRY");

		ADD__ALARM_EX(ALID__VENT_ERROR,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_title  = title;
		alarm_title += "PUMP ERROR";

		alarm_msg = "";
	
		l_act.RemoveAll();
		l_act.Add("ABORT");
		l_act.Add("RETRY");

		ADD__ALARM_EX(ALID__PUMP_ERROR,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_title  = title;
		alarm_title += "SLIT VALVE NOT CLOSE";

		alarm_msg = "Please, check the status of slit valve.";
	
		l_act.RemoveAll();
		l_act.Add("ABORT");
		l_act.Add("IGNORE");
		l_act.Add("RETRY");

		ADD__ALARM_EX(ALID__SLIT_VALVE_NOT_CLOSE,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_title  = title;
		alarm_title += "PROCESS ALARM";

		alarm_msg = "PMC Alarm occured in processing.\n";
	
		l_act.RemoveAll();
		l_act.Add("CHECK");

		ADD__ALARM_EX(ALID__PROCESS_ALARM,alarm_title,alarm_msg,l_act);
	}
	
	// ...
	{
		alarm_title  = title;
		alarm_title += "PMC's 2st slot is not empty.";

		alarm_msg  = "There is glass on 2st slot. So, cann't process.\n";
		alarm_msg += "Please, clear glass on 2st slot.\n";

		l_act.RemoveAll();
		l_act.Add("ABORT");
		l_act.Add("RETRY");

		ADD__ALARM_EX(ALID__2st_SLOT_NOT_EMPTY,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__PMC_ENABLE_TO_DISABLE;

		alarm_title  = title;
		alarm_title += "PMC's status is converted to \"DISABLE\".";

		alarm_msg = "";

		l_act.RemoveAll();
		l_act.Add("CLEAR");

		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__PMC_DISABLE_TO_ENABLE;

		alarm_title  = title;
		alarm_title += "PMC's status is converted to \"ENABLE\".";

		alarm_msg = "";

		l_act.RemoveAll();
		l_act.Add("CLEAR");

		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__PMC_PROCESS_TYPE_TO_NORMAL;
		
		alarm_title  = title;
		alarm_title += "PMC's process type is converted to \"Normal\".";
		
		alarm_msg = "";
		
		l_act.RemoveAll();
		l_act.Add("CHECK");
		
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}
	
	// ...
	{
		alarm_id = ALID__PMC_PROCESS_TYPE_TO_TEST;
		
		alarm_title  = title;
		alarm_title += "PMC's process type is converted to \"Test\".";
		
		alarm_msg = "";
		
		l_act.RemoveAll();
		l_act.Add("CHECK");
		
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__PMC_PROCESS_START_RETRY;

		alarm_title  = title;
		alarm_title += "PMC's \"Process Start\" command is retried.";

		alarm_msg = "";

		l_act.RemoveAll();
		l_act.Add("CHECK");

		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__PMC_CLN_PROC_START_RETRY;
		
		alarm_title  = title;
		alarm_title += "PMC's \"Clean Process Start\" command is retried.";
		
		alarm_msg = "";
		
		l_act.RemoveAll();
		l_act.Add("CHECK");
		
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__PMC_LEAK_CHECK_TIMEOVER;

		alarm_title  = title;
		alarm_title += "PMC의 기간 LeakCheck Time이 경과 되었습니다.";

		alarm_msg = "PMC를 LeakCheck 하시기 바랍니다. \n";	

		l_act.RemoveAll();
		l_act.Add("CHECK");

		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}
	// ...
	{
		alarm_id = ALID__PMC_AUTO_PM_TIMEOVER;

		alarm_title  = title;
		alarm_title += "PMC의 기간 Auto_PM Time이 경과 되었습니다.";

		alarm_msg = "PMC를 Auto_PM 하시기 바랍니다. \n";	

		l_act.RemoveAll();
		l_act.Add("CHECK");

		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__ALARM_POST_PMC_DUMMY_BY_CTC;

		alarm_title  = title;
		alarm_title += "Alarm Test In PMC_Dummy By CTC !";

		alarm_msg = "\"IGNORE\"를 선택하면, 정상 진행된다. \n";	

		l_act.RemoveAll();
		l_act.Add("ABORT");
		l_act.Add("IGNORE");

		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	return 1;
}

//-------------------------------------------------------------------------
int CObj_Phy__PMC_STD::__INITIALIZE__OBJECT(p_variable,p_ext_obj_create)
{
	p_ext_obj_create->Link__DEF_VARIABLE__ERROR_FEEDBACK(&mERROR__DEF_VAR);

	// ...
	{
		pOBJ_CTRL = p_ext_obj_create->Create__OBJECT_CTRL(sObject_Name);

		xTIMER__SYS_IDLE->REGISTER__COUNT_CHANNEL(sObject_Name,xCH__SYS_IDLE_TIME->Get__VARIABLE_NAME());
	}

	// MODULE CTRL & MODE ...
	{
		CString str_name;

		str_name = "CTRL";
		LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__MODULE_OBJ_CTRL, sObject_Name,str_name);

		str_name = "MODE";
		LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__MODULE_OBJ_MODE, sObject_Name,str_name);
	}

	// ...
	CString str_log_id;
	CString str_log_slot_max;
	CString str_pm_id;
	CString str_seq_id;
	CString module_name;
	CString cfg_db_name;
	CString str_name;
	CString def_name;
	CString var_data;
	int i;

	// RMS SVID ...
	{
		iRMS_SVID__PP_CREATE = 1;

		def_name = "RMS_SVID__PP_CREATE";
		if(p_ext_obj_create->Get__DEF_CONST_DATA_EX(def_name,var_data,-1) > 0)
		{
			iRMS_SVID__PP_CREATE = atoi(var_data);
		}

		//
		iRMS_SVID__PP_UPDATE = 2;

		def_name = "RMS_SVID__PP_UPDATE";
		if(p_ext_obj_create->Get__DEF_CONST_DATA_EX(def_name,var_data,-1) > 0)
		{
			iRMS_SVID__PP_UPDATE = atoi(var_data);
		}
		
		//
		iRMS_SVID__PP_DELETE = 3;
		
		def_name = "RMS_SVID__PP_DELETE";
		if(p_ext_obj_create->Get__DEF_CONST_DATA_EX(def_name,var_data,-1) > 0)
		{
			iRMS_SVID__PP_DELETE = atoi(var_data);
		}
	}

	// ...
	{
		p_ext_obj_create->Get__DEF_CONST_DATA_EX("LOG_ID",str_log_id,-1);
		p_ext_obj_create->Get__DEF_CONST_DATA_EX("LOG_SLOT_MAX",str_log_slot_max,-1);
		p_ext_obj_create->Get__DEF_CONST_DATA("PM_ID", str_pm_id);
		p_ext_obj_create->Get__DEF_CONST_DATA_EX("SEQ_ID",str_seq_id,-1);
		p_ext_obj_create->Get__DEF_CONST_DATA("DB_CFG_NAME",cfg_db_name);
	}

	// ...
	{
		CString def_data;
		p_ext_obj_create->Get__DEF_CONST_DATA_EX("2st_SLOT_EMPTY_CHECK",def_data,-1);

		if(def_data.CompareNoCase("YES") == 0)
		{
			xCH__CFG_2st_SLOT_EMPTY_CHECK->Set__DATA(STR__ENABLE);
		}
	}

	// ...
	{
		if(str_log_id.GetLength() < 1)			iLOG_ID = -1;
		else									iLOG_ID = atoi(str_log_id);
	}

	// ...
	{
		iLOG_SLOT_MAX = atoi(str_log_slot_max);
		if(iLOG_SLOT_MAX < 1)		iLOG_SLOT_MAX = 1;
	}

	// ...
	{
		int pm_id = atoi(str_pm_id);
		module_name.Format("PM%1d",pm_id);

		iPMx_ID = pm_id;
	
		if(str_seq_id.GetLength() > 0)
		{
			iSEQ_ID = atoi(str_seq_id);
		}
		else
		{
			iSEQ_ID = pm_id + 1; 
		}

		var_data.Format("%1d", iPMx_ID);
		sCH__FDC_CHM_ID->Set__DATA(var_data);

		//
		str_name.Format("SCH_TEST.CFG.PM%1d_DUMMY_BY_CTC", iPMx_ID);
		LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__SCH_TEST_CFG_PMC_DUMMY_BY_CTC, cfg_db_name,str_name);

		//
		str_name.Format("PM%1d.USE",pm_id);
		LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH_CFG__PMC_USE,cfg_db_name,str_name);

		str_name.Format("PM%1d.STAGE1.USE",pm_id);
		LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH_CFG__PMC_STAGE1_USE,cfg_db_name,str_name);

		str_name.Format("PM%1d.STAGE2.USE",pm_id);
		LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH_CFG__PMC_STAGE2_USE,cfg_db_name,str_name);

		str_name.Format("PM%1d.PROCESS.TYPE",pm_id);
		LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH_CFG__PMC_PROCESS_TYPE,cfg_db_name,str_name);

		str_name.Format("PM%1d.SLOT.MAX",pm_id);
		LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH_CFG__PMC_SLOT_MAX,cfg_db_name,str_name);
		Check__PMx_Slot_Max();

		//
		str_name = "REF.ATM.PRESSURE";
		LINK__EXT_VAR_ANALOG_CTRL(xEXT_CH_CFG__REF_ATM_PRESSURE, cfg_db_name,str_name);

		str_name = "REF.VAC.PRESSURE";
		LINK__EXT_VAR_ANALOG_CTRL(xEXT_CH_CFG__REF_VAC_PRESSURE, cfg_db_name,str_name);

		str_name = "TRANSFER.MODE";
		LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH_CFG__TRANSFER_MODE, cfg_db_name,str_name);

		// LEAK CHECK TIME  ...
		{
			str_name = "LEAK_CHECK.SCH.SEQUENCE_MODE.REQ_FLAG";
			LINK__EXT_VAR_STRING_CTRL(xEXT_CH__LEAK_CHECK__SCH_SEQUENCE_MODE__REQ_FLAG, cfg_db_name,str_name);

			//
			str_name = "LEAK_CHECK.IDLE.USE.FLAG";
			LINK__EXT_VAR_STRING_CTRL(xEXT_CH__LEAK_CHECK__IDLE_USE_FLAG, cfg_db_name,str_name);

			str_name = "LEAK_CHECK.TIME.MODE";
			LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH_CFG__LEAK_CHECK__TIME_MODE, cfg_db_name,str_name);

			str_name.Format("PM%1d.LEAK_CHECK.FLAG",iPMx_ID);
			LINK__EXT_VAR_STRING_CTRL(xEXT_CH__PMC_LEAK_CHECK_FLAG, cfg_db_name,str_name);

			LINK__EXT_VAR_STRING_CTRL(xEXT_CH_CFG__PMC_MSG_WIN_CTRL,cfg_db_name,"PMC_MSG.WIN_CTRL");
			LINK__EXT_VAR_STRING_CTRL(xEXT_CH_CFG__PMC_MSG_WIN_NAME,cfg_db_name,"PMC_MSG.WIN_NAME");

			for(i=0; i<CFG__LPx_SIZE; i++)
			{
				str_name.Format("CFG.LP%1d.JOB_START.FLAG",i+1);
				LINK__EXT_VAR_STRING_CTRL(xEXT_CH__LPx_JOB_START_FLAG[i], cfg_db_name,str_name);
			}
		}

		// AUTO_PM ...
		{
			str_name = "AUTO_PM.SCH.SEQUENCE_MODE.REQ_FLAG";
			LINK__EXT_VAR_STRING_CTRL(xEXT_CH__AUTO_PM__SCH_SEQUENCE_MODE__REQ_FLAG, cfg_db_name,str_name);

			//
			str_name = "AUTO_PM.IDLE.USE.FLAG";
			LINK__EXT_VAR_STRING_CTRL(xEXT_CH__AUTO_PM__IDLE_USE_FLAG, cfg_db_name,str_name);

			str_name = "AUTO_PM.TIME.MODE";
			LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH_CFG__AUTO_PM__TIME_MODE, cfg_db_name,str_name);

			str_name.Format("PM%1d.AUTO_PM.FLAG",iPMx_ID);
			LINK__EXT_VAR_STRING_CTRL(xEXT_CH__PMC_AUTO_PM_FLAG, cfg_db_name,str_name);
		}

		// MARATHON_TEST ...
		{
			str_name = "PMx.MARATHON_TEST.USE";
			LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__ALL_PM_MARATHON_TEST_USE, cfg_db_name,str_name);

			//
			str_name.Format("PM%1d.MARATHON_TEST.USE", iPMx_ID);
			LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__PM_ID_MARATHON_TEST_USE, cfg_db_name,str_name);

			str_name.Format("PM%1d.MARATHON_TEST.WAIT.SEC", iPMx_ID);
			LINK__EXT_VAR_ANALOG_CTRL(aEXT_CH__PM_ID_MARATHON_TEST_WAIT_SEC, cfg_db_name,str_name);
		}
	}

	// ...
	{
		CString obj_name;

		obj_name = "__EQP";
		str_name.Format("##SEQ%1d.CONNECT.STATUS",iSEQ_ID);

		LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__PMC_CONNECT_STATUS, obj_name,str_name);
	}

	// ...
	{
		xI_Module_Obj->Link__ERROR_FEEDBACK(&mERROR__MODULE_OBJ);

		//
		xI_Module_Obj->Link__Object_Name(sObject_Name);

		xI_Module_Obj->Register__Module_Status_Channel(xCH__OBJ_STATUS->Get__VARIABLE_NAME());
		xI_Module_Obj->Register__Module_Mode_Channel(dVAR__MODE);
		xI_Module_Obj->Register__Module_Ctrl_Channel(xCH__OBJ_CTRL->Get__VARIABLE_NAME());

		xI_Module_Obj->Disable__CTCINUSE_TO_STANDBY();

		// ...
		{
			CString data_const;
			p_ext_obj_create->Get__DEF_CONST_DATA_EX("FLAG.REPORT_ABORTED_ALARM",data_const,-1);

			if(data_const.CompareNoCase("DISABLE") == 0)
			{
				xI_Module_Obj->Disable__REPORT_ABORTED_ALARM();
			}
		}
	}

	// ...
	{
		iFLAG__STEP_REPORT = -1;
		iPRC_STS = -1;
		iFNC_STS = -1;

		bActive__Init_Act = false;
	}

	// ...
	{
		CString file_name;
		CString log_msg;

		file_name.Format("%s.log", sObject_Name);

		log_msg  = "\n\n";
		log_msg += "//------------------------------------------------------------------------";

		xLOG_CTRL->CREATE__SUB_DIRECTORY(sObject_Name);
		xLOG_CTRL->SET__PROPERTY(file_name,24,60);

		xLOG_CTRL->DISABLE__TIME_LOG();
		xLOG_CTRL->WRITE__LOG(log_msg);

		xLOG_CTRL->ENABLE__TIME_LOG();
		xLOG_CTRL->WRITE__LOG("   START   \n");

		xLOG_CTRL->LINK__OBJECT(sObject_Name);
	}

	// ...
	{
		SCX__SEQ_INFO x_seq_info;

		x_seq_info->Get__DIR_ROOT(sDir_ROOT);

		iSIM_MODE = x_seq_info->Is__SIMULATION_MODE();
	}
	return 1;
}
int CObj_Phy__PMC_STD::__START__OBJECT()
{
	xSCH_MATERIAL_CTRL->Link__SERVICE_DB("");

	iFLAG__WAC_PROC = -1;
	return 1;
}

//-------------------------------------------------------------------------
int CObj_Phy__PMC_STD::__CALL__CONTROL_MODE(mode,p_debug,p_variable,p_alarm)
{
	// ...
	{
		xI_Sync_Ctrl.Thread__LOCK();

		iPRC_STS = 1;
		iFNC_STS = 1;
	}

	// ...
	DECLARE__EXT_CTRL(p_variable);

	CString upper_obj_name = p_ext_mode_ctrl->Get__UPPER_OBJECT_NAME();

	// ...
	int flag = -1;

	iFLAG__STEP_REPORT = -1;

	// ...
	xI_Module_Obj->Enable__ERROR_CHECK__NOT_STANDBY();

	Check__PMx_Slot_Max();

	// ...
	CString pmc_name;
	CString msg;

	pmc_name.Format("PM%1d",iPMx_ID);

	// ...
	{
		msg.Format("[%s] - [%s]  Starting ...",pmc_name,mode);
		xCH__OBJ_MSG->Set__DATA(msg);

		msg.Format("%s ...",mode);
		xCH__OBJ_FNC_MSG->Set__DATA(msg);
	}

	// ...
	{
		CString log_string;
		
		log_string.Format("START : [%s]...  Called By \"%s\".",mode,upper_obj_name);
		xLOG_CTRL->WRITE__LOG(log_string);
	}

	// ...
	{
		xCH__X_PICK_FLAG->Set__DATA("");
		xCH__X_PLACE_FLAG->Set__DATA("");
	}

	// ...
	int sim__dummy_mode = -1;

	if(iSIM_MODE > 0)
	{
		if(dEXT_CH__SCH_TEST_CFG_PMC_DUMMY_BY_CTC->Check__DATA(STR__YES) > 0)
		{
			sim__dummy_mode = 1;
		}
	}
	iSIM__PMC_DUMMY_MODE = sim__dummy_mode;

	if(sim__dummy_mode < 0)
	{
			 IF__CTRL_MODE(sMODE__TIME_INIT)			flag = Call__TIME_INIT(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__SYSTEM_INIT)			flag = Call__SYSTEM_INIT(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__AUTO_INIT)			flag = Call__AUTO_INIT(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__SYSTEM_MAINT)			flag = Call__SYSTEM_MAINT(p_variable);

		ELSE_IF__CTRL_MODE(sMODE__PUMP)					flag = Call__PUMP(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__VENT)					flag = Call__VENT(p_variable,p_alarm);

		ELSE_IF__CTRL_MODE(sMODE__SLOT_OPEN)			flag = Call__SLOT_OPEN(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__SLOT_CLOSE)			flag = Call__SLOT_CLOSE(p_variable,p_alarm);
		
		ELSE_IF__CTRL_MODE(sMODE__PIN_UP)				flag = Call__PIN_UP(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__PIN_DOWN)				flag = Call__PIN_DOWN(p_variable,p_alarm);

		ELSE_IF__CTRL_MODE(sMODE__PICK_READY)			flag = Call__PICK_READY(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__PICK_X_READY)			flag = Call__PICK_X_READY(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__PICK_COMPLETE)		flag = Call__PICK_COMPLETE(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__PICK_X_COMPLETE)		flag = Call__PICK_X_COMPLETE(p_variable);
		
		ELSE_IF__CTRL_MODE(sMODE__PLACE_READY)			flag = Call__PLACE_READY(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__PLACE_X_READY)		flag = Call__PLACE_X_READY(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__PLACE_COMPLETE)		flag = Call__PLACE_COMPLETE(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__PLACE_X_COMPLETE)		flag = Call__PLACE_X_COMPLETE(p_variable);

		ELSE_IF__CTRL_MODE(sMODE__PRO_READY)			flag = Call__PRO_READY(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__PRO_START)			flag = Call__PRO_START(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__AUTO_PRO)				flag = Call__AUTO_PRO(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__AUTO_PRO_EX)			flag = Call__AUTO_PRO_EX(p_variable,p_alarm);

		ELSE_IF__CTRL_MODE(sMODE__AUTO_DUMMY_CLN)		flag = Call__AUTO_DUMMY_CLN(p_variable,p_alarm);

		ELSE_IF__CTRL_MODE(sMODE__PRE_RCP_READY)		flag = Call__PRE_RCP_READY(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__CLN_READY)			flag = Call__CLN_READY(p_variable);

		ELSE_IF__CTRL_MODE(sMODE__PRE_RCP_START)		flag = Call__PRE_RCP_START(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__CLN_START)			flag = Call__CLN_START(p_variable,p_alarm);

		ELSE_IF__CTRL_MODE(sMODE__AUTO_PRE_RCP)			flag = Call__AUTO_PRE_RCP(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__AUTO_CLN)				flag = Call__AUTO_CLN(p_variable,p_alarm);
		
		ELSE_IF__CTRL_MODE(sMODE__POST_CLN)
		{
			iFLAG__WAC_PROC = 1;

			flag = Call__POST_CLN(p_variable,p_alarm);
	
			iFLAG__WAC_PROC = -1;
		}

		ELSE_IF__CTRL_MODE(sMODE__LAYER_CLN_READY)		flag = Call__LAYER_CLN_READY(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__LAYER_CLN_START)		flag = Call__LAYER_CLN_START(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__AUTO_LAYER_CLN)		flag = Call__AUTO_LAYER_CLN(p_variable,p_alarm);

		ELSE_IF__CTRL_MODE(sMODE__MNT_READY)			flag = Call__MNT_READY(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__MNT_START)			flag = Call__MNT_START(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__AUTO_MNT)				flag = Call__AUTO_MNT(p_variable);

		ELSE_IF__CTRL_MODE(sMODE__LEAK_CHECK)			flag = Call__LEAK_CHECK(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__LEAK_CHECK_EX)		flag = Call__LEAK_CHECK_EX(p_variable);

		ELSE_IF__CTRL_MODE(sMODE__AUTO_PM)				flag = Call__AUTO_PM(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__AUTO_PM_EX)			flag = Call__AUTO_PM_EX(p_variable);

		ELSE_IF__CTRL_MODE(sMODE__NEXT_PROC)			flag = Call__NEXT_PROC(p_variable,p_alarm);

		ELSE_IF__CTRL_MODE(sMODE__JOB_START_READY)		flag = Call__JOB_START_READY(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__JOB_END_READY)		flag = Call__JOB_END_READY(p_variable);

		ELSE_IF__CTRL_MODE(sMODE__IDLE_START)			flag = Call__IDLE_START(p_variable);

		ELSE_IF__CTRL_MODE(sMODE__LOAD)					flag = Call__LOAD(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__UNLOAD)				flag = Call__UNLOAD(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__READY_TO_UNLOAD)		flag = Call__READY_TO_UNLOAD(p_variable,p_alarm);

		ELSE_IF__CTRL_MODE(sMODE__SCH_TEST_CFG)			flag = Call__SCH_TEST_CFG(p_variable,p_alarm);

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
	else
	{
			 IF__CTRL_MODE(sMODE__TIME_INIT)			flag = Call__CTC_SIM__TIME_INIT(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__SYSTEM_INIT)			flag = Call__CTC_SIM__SYSTEM_INIT(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__AUTO_INIT)			flag = Call__CTC_SIM__AUTO_INIT(p_variable, p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__SYSTEM_MAINT)			flag = Call__CTC_SIM__SYSTEM_MAINT(p_variable);

		ELSE_IF__CTRL_MODE(sMODE__PUMP)					flag = Call__CTC_SIM__PUMP(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__VENT)					flag = Call__CTC_SIM__VENT(p_variable,p_alarm);

		ELSE_IF__CTRL_MODE(sMODE__SLOT_OPEN)			flag = Call__CTC_SIM__SLOT_OPEN(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__SLOT_CLOSE)			flag = Call__CTC_SIM__SLOT_CLOSE(p_variable,p_alarm);

		ELSE_IF__CTRL_MODE(sMODE__PIN_UP)				flag = Call__CTC_SIM__PIN_UP(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__PIN_DOWN)				flag = Call__CTC_SIM__PIN_DOWN(p_variable,p_alarm);

		ELSE_IF__CTRL_MODE(sMODE__PICK_READY)			flag = Call__CTC_SIM__PICK_READY(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__PICK_X_READY)			flag = Call__CTC_SIM__PICK_X_READY(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__PICK_COMPLETE)		flag = Call__CTC_SIM__PICK_COMPLETE(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__PICK_X_COMPLETE)		flag = Call__CTC_SIM__PICK_X_COMPLETE(p_variable);

		ELSE_IF__CTRL_MODE(sMODE__PLACE_READY)			flag = Call__CTC_SIM__PLACE_READY(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__PLACE_X_READY)		flag = Call__CTC_SIM__PLACE_X_READY(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__PLACE_COMPLETE)		flag = Call__CTC_SIM__PLACE_COMPLETE(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__PLACE_X_COMPLETE)		flag = Call__CTC_SIM__PLACE_X_COMPLETE(p_variable);

		ELSE_IF__CTRL_MODE(sMODE__PRO_READY)			flag = Call__CTC_SIM__PRO_READY(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__PRO_START)			flag = Call__CTC_SIM__PRO_START(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__AUTO_PRO)				flag = Call__CTC_SIM__AUTO_PRO(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__AUTO_PRO_EX)			flag = Call__CTC_SIM__AUTO_PRO_EX(p_variable,p_alarm);

		ELSE_IF__CTRL_MODE(sMODE__AUTO_DUMMY_CLN)		flag = Call__CTC_SIM__AUTO_DUMMY_CLN(p_variable,p_alarm);

		ELSE_IF__CTRL_MODE(sMODE__PRE_RCP_READY)		flag = Call__CTC_SIM__PRE_RCP_READY(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__PRE_RCP_START)		flag = Call__CTC_SIM__PRE_RCP_START(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__AUTO_PRE_RCP)			flag = Call__CTC_SIM__AUTO_PRE_RCP(p_variable,p_alarm);

		ELSE_IF__CTRL_MODE(sMODE__CLN_READY)			flag = Call__CTC_SIM__CLN_READY(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__CLN_START)			flag = Call__CTC_SIM__CLN_START(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__AUTO_CLN)				flag = Call__CTC_SIM__AUTO_CLN(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__POST_CLN)
		{
			iFLAG__WAC_PROC = 1;

			flag = Call__CTC_SIM__POST_CLN(p_variable,p_alarm);

			iFLAG__WAC_PROC = -1;
		}

		ELSE_IF__CTRL_MODE(sMODE__MNT_READY)			flag = Call__CTC_SIM__MNT_READY(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__MNT_START)			flag = Call__CTC_SIM__MNT_START(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__AUTO_MNT)				flag = Call__CTC_SIM__AUTO_MNT(p_variable);

		ELSE_IF__CTRL_MODE(sMODE__LEAK_CHECK)			flag = Call__CTC_SIM__LEAK_CHECK(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__LEAK_CHECK_EX)		flag = Call__CTC_SIM__LEAK_CHECK_EX(p_variable);

		ELSE_IF__CTRL_MODE(sMODE__AUTO_PM)				flag = Call__CTC_SIM__AUTO_PM(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__AUTO_PM_EX)			flag = Call__CTC_SIM__AUTO_PM_EX(p_variable);

		ELSE_IF__CTRL_MODE(sMODE__NEXT_PROC)			flag = Call__CTC_SIM__NEXT_PROC(p_variable,p_alarm);

		ELSE_IF__CTRL_MODE(sMODE__JOB_START_READY)		flag = Call__CTC_SIM__JOB_START_READY(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__JOB_END_READY)		flag = Call__CTC_SIM__JOB_END_READY(p_variable);

		ELSE_IF__CTRL_MODE(sMODE__IDLE_START)			flag = Call__CTC_SIM__IDLE_START(p_variable);

		ELSE_IF__CTRL_MODE(sMODE__SCH_TEST_CFG)			flag = Call__SCH_TEST_CFG(p_variable,p_alarm);

		else
		{
			CString alarm_msg;
			CString r_act;
			CString bff;

			bff.Format("CTC - PMC_DUMMY MODE ... \n");
			alarm_msg  = bff;
			bff.Format("Object Name : %s\n",sObject_Name);
			alarm_msg  = bff;
			bff.Format("Unknown Object Mode : \"%s\"\n",mode);
			alarm_msg += bff;

			p_alarm->Popup__ALARM_With_MESSAGE(ALID__OBJECT_MODE_UNKNOWN,alarm_msg,r_act);		
		}

		if(flag > 0)
		{
			if(dCH__CFG_ALARM_POST_PMC_DUMMY_BY_CTC->Check__DATA(STR__YES) > 0)
			{
				dCH__CFG_ALARM_POST_PMC_DUMMY_BY_CTC->Set__DATA(STR__NO);
				xCH__PROC_INFO__PROCESS_STATUS->Set__DATA("Alarm");

				// ...
				{
					int alm_id = ALID__ALARM_POST_PMC_DUMMY_BY_CTC;
					CString alm_msg;
					CString r_act;

					alm_msg.Format("Action.Mode <- %s \n", mode);

					p_alarm->Popup__ALARM_With_MESSAGE(alm_id, alm_msg, r_act);
				
					if(r_act.CompareNoCase("ABORT") == 0)		flag = -1;
				}

				xCH__PROC_INFO__PROCESS_STATUS->Set__DATA("Idle");
			}
		}
	}

	if(xCH__PROCESS_TYPE__BONDING_FLAG->Check__DATA(STR__YES) > 0)
	{
		if((mode.CompareNoCase(sMODE__PRO_START)   == 0)
		|| (mode.CompareNoCase(sMODE__AUTO_PRO_EX) == 0)
		|| (mode.CompareNoCase(sMODE__MNT_START)   == 0)
		|| (mode.CompareNoCase(sMODE__AUTO_MNT)	   == 0))
		{
			if(flag < 0)
			{

			}

			if((xCH__SLOT_STATUS[0]->Check__DATA(STR__NONE) < 0)
			&& (xCH__SLOT_STATUS[1]->Check__DATA(STR__NONE) < 0))
			{
				xCH__SLOT_STATUS[1]->Set__DATA(STR__NONE);

				CString sch_name;
				sch_name.Format("PM%1d-2", iPMx_ID);
				
				xSCH_MATERIAL_CTRL->Clear__MATERIAL_INFO(sch_name);
			}
		}
	}

	if(flag < 0)
	{
		xCH__X_PICK_FLAG->Set__DATA("");
		xCH__X_PLACE_FLAG->Set__DATA("");
	}

	// ...
	{
		if(flag > 0)		msg.Format("[%s] - [%s]  Completed ...",pmc_name,mode);
		else				msg.Format("[%s] - [%s]  Aborted ...",pmc_name,mode);

		xCH__OBJ_MSG->Set__DATA(msg);
	}

	// ...
	{
		if((mode.CompareNoCase(sMODE__PRO_START)   == 0)
		|| (mode.CompareNoCase(sMODE__AUTO_PRO)    == 0)
		|| (mode.CompareNoCase(sMODE__AUTO_PRO_EX) == 0))
		{
			if(flag > 0)
			{
				xCH__OBJ_FNC_MSG->Set__DATA("Process End");
			}
			else
			{
				xCH__OBJ_FNC_MSG->Set__DATA("Process Aborted");
			}
		}
		else
		{
			xCH__OBJ_FNC_MSG->Set__DATA("");
		}
	}

	// ...
	{
		CString log_string;

		if(flag > 0)		log_string.Format("COMPLETE : [%s]", mode);
		else				log_string.Format("ABORTED(%1d) : [%s]", flag,mode);

		xLOG_CTRL->WRITE__LOG(log_string);
	}

	if(iFLAG__STEP_REPORT > 0)		iFLAG__STEP_REPORT =  0;
	else							iFLAG__STEP_REPORT = -1;

	if(flag > 0)
	{
		if(iPRC_STS > 0)
		{
			if(xCH__NEXT_PROCESS->Check__DATA(STR__GOTO_MAINT) > 0)
			{
				int material_count = Get__Material_Count();

				if(material_count < 1)
				{
					if(sim__dummy_mode < 0)			Call__SYSTEM_MAINT(p_variable);
					else							Call__CTC_SIM__SYSTEM_MAINT(p_variable);
				}
			}
		}
	}

	// ...
	{
		bActive__Init_Act = false;

		iPRC_STS = -1;
		iFNC_STS = -1;

		xI_Sync_Ctrl.Thread__UNLOCK();
	}
	return flag;
}

int CObj_Phy__PMC_STD::__CALL__MONITORING(id,p_variable,p_alarm)
{
	switch(id)
	{
		case MON_ID__ALL_STATE:
			Mon__All_State(p_variable,p_alarm);
			break;

		//			
		case MON_ID__FA_STEP_REPORT:
			Mon__FA_Report(p_variable);
			break;
	}

	return 1;
}
