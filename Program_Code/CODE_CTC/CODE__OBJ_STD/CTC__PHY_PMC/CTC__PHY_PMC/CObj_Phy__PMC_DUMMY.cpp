#include "StdAfx.h"
#include "CObj_Phy__PMC_DUMMY.h"
#include "CObj_Phy__PMC_DUMMY__ALID.h"


//-------------------------------------------------------------------------
CObj_Phy__PMC_DUMMY::CObj_Phy__PMC_DUMMY()
{

}
CObj_Phy__PMC_DUMMY::~CObj_Phy__PMC_DUMMY()
{

}

//-------------------------------------------------------------------------
int CObj_Phy__PMC_DUMMY::__DEFINE__CONTROL_MODE(obj,l_mode)
{
	sObject_Name = obj;

	iPRC_STS = -1;
	iFNC_STS = -1;

	// ...
	{
		ADD__CTRL_VAR(sMODE__TIME_INIT,   "TIME_INIT");
		ADD__CTRL_VAR(sMODE__SYSTEM_INIT, "SYSTEM_INIT");
		ADD__CTRL_VAR(sMODE__AUTO_INIT,   "AUTO_INIT");

		ADD__CTRL_VAR(sMODE__SYSTEM_MAINT,"SYSTEM_MAINT");
		
		ADD__CTRL_VAR(sMODE__PUMP, "PUMP");
		ADD__CTRL_VAR(sMODE__VENT, "VENT");
		
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

		ADD__CTRL_VAR(sMODE__CLN_READY, "CLN_READY");
		ADD__CTRL_VAR(sMODE__CLN_START, "CLN_START");
		ADD__CTRL_VAR(sMODE__AUTO_CLN,  "AUTO_CLN" );

		ADD__CTRL_VAR(sMODE__LAYER_CLN_READY, "LAYER_CLN_READY");
		ADD__CTRL_VAR(sMODE__LAYER_CLN_START, "LAYER_CLN_START");
		ADD__CTRL_VAR(sMODE__AUTO_LAYER_CLN,  "AUTO_LAYER_CLN" );

		ADD__CTRL_VAR(sMODE__MNT_READY, "MNT_READY");
		ADD__CTRL_VAR(sMODE__MNT_START, "MNT_START");
		ADD__CTRL_VAR(sMODE__AUTO_MNT,  "AUTO_MNT" );

		ADD__CTRL_VAR(sMODE__LEAK_CHECK,   "LEAK_CHECK");
		ADD__CTRL_VAR(sMODE__LEAK_CHECK_EX,"LEAK_CHECK_EX");

		ADD__CTRL_VAR(sMODE__NEXT_PROC, "NEXT_PROC");

		ADD__CTRL_VAR(sMODE__JOB_START_READY, "JOB_START_READY");
		ADD__CTRL_VAR(sMODE__JOB_END_READY,   "JOB_END_READY");
	}

	return 1;
}
int CObj_Phy__PMC_DUMMY::__DEFINE__VERSION_HISTORY(version)
{
	version  = "JG.Lee \n";

	return 1;
}


// ...
#define  MON_ID__OBJ_STATUS						101

#define  MON_ID__MODULE_STATUS					1
#define  MON_ID__PRESSURE_STATUS				2
#define  MON_ID__PROCESS_TIME					3
#define  MON_ID__LEAK_CHECK						4
#define  MON_ID__GOTO_MAINT						5
#define  MON_ID__LPx_WAFER_COUNT				6
#define  MON_ID__CLEAN_RCP_COUNT				7

#define  MON_ID__FA_STEP_REPORT					11
#define  MON_ID__FA_RMS_REPORT					12

// ...
#define  DSP__MODE													\
"TIME_INIT  SYSTEM_INIT   AUTO_INIT									\
PUMP  VENT															\
PICK_READY   PICK_X_READY   PICK_COMPLETE   PICK_X_COMPLETE			\
PLACE_READY  PLACE_X_READY  PLACE_COMPLETE  PLACE_X_COMPLETE		\
PRO_READY  PRO_START												\
CLN_READY  CLN_START												\
MNT_READY  MNT_START												\
LEAK_CHECK  HOT_IDLE  SYSTEM_MAINT									\
JOB_START_READY  JOB_END_READY"

#define  DSP__NO_YES			"NO  YES"
#define  DSP__PRC_STS			"IDLE  PROCESSING"
#define  DSP__DISABLE_ENABLE	"DISABLE  ENABLE"	
#define  DSP__LEAK_CHECK_MODE   "LEAK_CHECK  JOB"


int CObj_Phy__PMC_DUMMY::__DEFINE__VARIABLE_STD(p_variable)
{
	DECLARE__STD_VARIABLE;

	CString str_name;
	CString dsp_item;
	int i;

	int slot_limit = CFG__PMx_MAX_SLOT;

	// ...
	{
		p_variable->Get__STD_DESCRIPTION("STD_OBJ_STATUS",dsp_item);

		//
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

		//
		str_name = "MOVE.FLAG";
		STD__ADD_DIGITAL(str_name,DSP__NO_YES);
		LINK__VAR_DIGITAL_CTRL(xCH__MOVE_FLAG,str_name);

		str_name = "X_PICK.FLAG";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(xCH__X_PICK_FLAG,str_name);

		str_name = "JOB.READY.FLAG";
		STD__ADD_STRING_WITH_X_OPTION(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__JOB_READY_FLAG,str_name);
	}

	// ...
	{
		p_variable->Get__STD_DESCRIPTION("STD_VALVE_STATUS",dsp_item);

		dVAR__SLIT_VLV_STS = "SLIT.VLV.STS";
		STD__ADD_DIGITAL(dVAR__SLIT_VLV_STS,dsp_item);

		sVAR__LIFT_PIN_STS = "LIFT.PIN.STS";
		STD__ADD_STRING(sVAR__LIFT_PIN_STS);

		dVAR__PRC_STS = "PRC.STS";
		STD__ADD_DIGITAL(dVAR__PRC_STS,DSP__PRC_STS);

		str_name = "SLOT.NO";
		STD__ADD_ANALOG(str_name,"num",0,1.0,slot_limit);
		LINK__VAR_ANALOG_CTRL(xCH__SLOT_NO,str_name);
	}
	
	// ...
	{
		p_variable->Get__STD_DESCRIPTION("STD_PRESSURE_STATUS",dsp_item);

		str_name = "PRESSURE.VALUE";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(xCH__PRESSURE_VALUE,str_name);

		str_name = "PRESSURE.STATUS";
		STD__ADD_DIGITAL(str_name,dsp_item);
		LINK__VAR_DIGITAL_CTRL(xCH__PRESSURE_STATUS,str_name);
	}

	// ...
	{
		str_name = "MODULE.TIME";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__MODULE_TIME,str_name);

		str_name = "LOG.SUB_DIRECTORY";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__LOG_SUB_DIRECTORY,str_name);

		str_name = "LOG.FILE_NAME";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__LOG_FILE_NAME,str_name);

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
		str_name = "FDC.RECIPE";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__FDC_RECIPE,str_name);

		str_name = "FDC.LOTID";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__FDC_LOTID,str_name);

		//
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

		//
		str_name = "LPx.CID";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(xCH__LPx_CID,str_name);

		str_name = "LPx.PORT_ID";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(xCH__LPx_PORT_ID,str_name);

		str_name = "LPx.SLOT_ID";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(xCH__LPx_SLOT_ID,str_name);

		//
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
	}

	// ...
	{
		str_name = "CFG.PRC.TIME";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",1,0,9999,"");
		LINK__VAR_ANALOG_CTRL(xCH__CFG_PRC_TIME,str_name);

		//
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

		//
		str_name = "ACC.MATERIAL.COUNT";
		STD__ADD_STRING_WITH_X_OPTION(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__ACC_MATERIAL_COUNT,str_name);

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
	}

	// ...
	{
		str_name = "CFG.PMC.USE";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,DSP__DISABLE_ENABLE,"");
		LINK__VAR_DIGITAL_CTRL(xCH_CFG__PMC_USE,str_name);

		// 
		str_name = "NEXT.PROCESS";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__NEXT_PROCESS,str_name);

		str_name = "CFG.AUTO.MATERIAL_COUNT.FLAG";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,DSP__DISABLE_ENABLE,"");
		LINK__VAR_DIGITAL_CTRL(xCH_CFG__AUTO_MATERIAL_COUNT_FLAG,str_name);

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

		str_name = "LAYER.CLEAN.RECIPE";
		STD__ADD_STRING_WITH_X_OPTION(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__LAYER_CLEAN_RECIPE,str_name);

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

		//
		str_name = "CFG.AUTO.LEAK_CHECK.USE";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,DSP__DISABLE_ENABLE,"");
		LINK__VAR_DIGITAL_CTRL(xCH_CFG__AUTO_LEAK_CHECK_USE,str_name);

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
		str_name = "AUTO.LEAK_CHECK.RUN_FLAG";
		STD__ADD_STRING_WITH_X_OPTION(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__AUTO_LEAK_CHECK_RUN_FLAG,str_name);

		str_name = "AUTO.LEAK_CHECK.RUN_DAY";
		STD__ADD_STRING_WITH_X_OPTION(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__AUTO_LEAK_CHECK_RUN_DAY,str_name);

		//
		str_name = "STEP.START";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__STEP_START,str_name);

		str_name = "STEP.END";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__STEP_END,str_name);

		//
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

		//
		str_name = "MATERIAL_CLEAR.REQ";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__MATERIAL_CLEAR_REQ,str_name);

		str_name = "MATERIAL_CLEAR.SLOT";
		STD__ADD_STRING_WITH_COMMENT(str_name,"");
		LINK__VAR_STRING_CTRL(xCH__MATERIAL_CLEAR_SLOT,str_name);

		//
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
		str_name = "ALARM.LOG.MODE";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name,"MATERIAL_ABORTED  ALL_ALARM","");
		LINK__VAR_DIGITAL_CTRL(xCH__ALARM_LOG_MODE,str_name);

		//
		str_name = "SYS.IDLE.TIME";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",1,0.0,9999999,"");
		LINK__VAR_ANALOG_CTRL(xCH__SYS_IDLE_TIME,str_name);
	}

	// ...
	{
		p_variable->Add__MONITORING_PROC(3.0,MON_ID__OBJ_STATUS);

		//
		p_variable->Add__MONITORING_PROC(5.0,MON_ID__MODULE_STATUS);
		p_variable->Add__MONITORING_PROC(1.0,MON_ID__PRESSURE_STATUS);
		p_variable->Add__MONITORING_PROC(1.0,MON_ID__PROCESS_TIME);
		p_variable->Add__MONITORING_PROC(3.0,MON_ID__LEAK_CHECK);
		p_variable->Add__MONITORING_PROC(3.0,MON_ID__GOTO_MAINT);
		p_variable->Add__MONITORING_PROC(1.0,MON_ID__LPx_WAFER_COUNT);
		p_variable->Add__MONITORING_PROC(3.0,MON_ID__CLEAN_RCP_COUNT);

		p_variable->Add__MONITORING_PROC(3.0,MON_ID__FA_STEP_REPORT);
		p_variable->Add__MONITORING_PROC(3.0,MON_ID__FA_RMS_REPORT);
	}
	return 1;
}

int CObj_Phy__PMC_DUMMY::__DEFINE__ALARM(p_alarm)
{
	DECLARE__ALARM;

	// ...
	CString title;
	title.Format("%s : ",sObject_Name);

	CString alarm_title;
	CString alarm_msg;
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
		l_act.Add("CHECK");

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
		alarm_title += "PROCESS ALARM";

		alarm_msg = "PMC Alarm occured in processing.\n";

		l_act.RemoveAll();
		l_act.Add("CHECK");

		ADD__ALARM_EX(ALID__PROCESS_ALARM,alarm_title,alarm_msg,l_act);
	}	

	return 1;
}

//-------------------------------------------------------------------------
int CObj_Phy__PMC_DUMMY::__INITIALIZE__OBJECT(p_variable,p_ext_obj_create)
{
	p_ext_obj_create->Link__DEF_VARIABLE__ERROR_FEEDBACK(&mERROR__DEF_VAR);

	// ...
	{
		pOBJ_CTRL = p_ext_obj_create->Create__OBJECT_CTRL(sObject_Name);
		xTIMER__SYS_IDLE->REGISTER__COUNT_CHANNEL(sObject_Name,xCH__SYS_IDLE_TIME->Get__VARIABLE_NAME());
	}

	// ...
	CString str_log_id;
	CString str_log_slot_max;
	CString str_pm_id;
	CString str_seq_id;
	CString module_name;
	CString cfg_db_name;
	CString str_name;
	int i;

	p_ext_obj_create->Get__DEF_CONST_DATA_EX("LOG_ID",str_log_id,-1);
	p_ext_obj_create->Get__DEF_CONST_DATA_EX("LOG_SLOT_MAX",str_log_slot_max,-1);
	p_ext_obj_create->Get__DEF_CONST_DATA("PM_ID", str_pm_id);
	p_ext_obj_create->Get__DEF_CONST_DATA_EX("SEQ_ID",str_seq_id,-1);
	p_ext_obj_create->Get__DEF_CONST_DATA("DB_CFG_NAME",cfg_db_name);

	//
	if(str_log_id.GetLength() < 1)			iLOG_ID = -1;
	else									iLOG_ID = atoi(str_log_id);

	//
	iLOG_SLOT_MAX = atoi(str_log_slot_max);
	if(iLOG_SLOT_MAX < 1)		iLOG_SLOT_MAX = 1;

	//
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

	// ...
	{
		str_name.Format("PM%1d.USE",pm_id);
		LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH_CFG__PMC_USE,cfg_db_name,str_name);

		str_name.Format("PM%1d.STAGE1.USE",pm_id);
		LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH_CFG__PMC_STAGE1_USE,cfg_db_name,str_name);

		str_name.Format("PM%1d.STAGE2.USE",pm_id);
		LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH_CFG__PMC_STAGE2_USE,cfg_db_name,str_name);

		str_name.Format("PM%1d.SLOT.MAX",pm_id);
		LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH_CFG__PMC_SLOT_MAX,cfg_db_name,str_name);
		Check__PMx_Slot_Max();
	}

	LINK__EXT_VAR_ANALOG_CTRL(xEXT_CH_CFG__REF_ATM_PRESSURE,cfg_db_name,"REF.ATM.PRESSURE");
	LINK__EXT_VAR_ANALOG_CTRL(xEXT_CH_CFG__REF_VAC_PRESSURE,cfg_db_name,"REF.VAC.PRESSURE");

	// LEAK CHECK TIME ...
	LINK__EXT_VAR_STRING_CTRL(xEXT_CH__LEAK_CHECK__SCH_SEQUENCE_MODE__REQ_FLAG, cfg_db_name,"LEAK_CHECK.SCH.SEQUENCE_MODE.REQ_FLAG");

	//
	LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH_CFG__LEAK_CHECK__TIME_MODE, cfg_db_name,"LEAK_CHECK.TIME.MODE");

	LINK__EXT_VAR_ANALOG_CTRL(xEXT_CH_CFG__LEAK_CHECK_START_TIME__HOUR,   cfg_db_name,"LEAK_CHECK.START_TIME.HOUR");
	LINK__EXT_VAR_ANALOG_CTRL(xEXT_CH_CFG__LEAK_CHECK_START_TIME__MINUTE, cfg_db_name,"LEAK_CHECK.START_TIME.MINUTE");
	LINK__EXT_VAR_ANALOG_CTRL(xEXT_CH_CFG__LEAK_CHECK_START_TIME__SECOND, cfg_db_name,"LEAK_CHECK.START_TIME.SECOND");

	LINK__EXT_VAR_ANALOG_CTRL(xEXT_CH_CFG__LEAK_CHECK_END_TIME__HOUR,   cfg_db_name,"LEAK_CHECK.END_TIME.HOUR");
	LINK__EXT_VAR_ANALOG_CTRL(xEXT_CH_CFG__LEAK_CHECK_END_TIME__MINUTE, cfg_db_name,"LEAK_CHECK.END_TIME.MINUTE");
	LINK__EXT_VAR_ANALOG_CTRL(xEXT_CH_CFG__LEAK_CHECK_END_TIME__SECOND, cfg_db_name,"LEAK_CHECK.END_TIME.SECOND");

	//
	str_name.Format("PM%1d.LEAK_CHECK.FLAG",iPMx_ID);
	LINK__EXT_VAR_STRING_CTRL(xEXT_CH__PMC_LEAK_CHECK_FLAG, cfg_db_name,str_name);

	//
	LINK__EXT_VAR_STRING_CTRL(xEXT_CH_CFG__PMC_MSG_WIN_CTRL,cfg_db_name,"PMC_MSG.WIN_CTRL");
	LINK__EXT_VAR_STRING_CTRL(xEXT_CH_CFG__PMC_MSG_WIN_NAME,cfg_db_name,"PMC_MSG.WIN_NAME");

	// ...
	{
		for(i=0;i<CFG__LPx_SIZE;i++)
		{
			str_name.Format("CFG.LP%1d.JOB_START.FLAG",i+1);
			LINK__EXT_VAR_STRING_CTRL(xEXT_CH__LPx_JOB_START_FLAG[i], cfg_db_name,str_name);
		}

		// ...
		{
			CString obj_name;

			obj_name = "__EQP";
			str_name.Format("##SEQ%1d.CONNECT.STATUS",iSEQ_ID);

			LINK__EXT_VAR_DIGITAL_CTRL(xEXT_CH__PMC_CONNECT_STATUS, obj_name,str_name);
		}
	}

	// ...
	{
		iFLAG__STEP_REPORT = -1;
		iPRC_STS = -1;
		iFNC_STS = -1;
	}

	// ...
	{
		CString file_name;
		CString log_msg;

		file_name.Format("%s_App.log", sObject_Name);

		log_msg  = "\n\n";
		log_msg += "//------------------------------------------------------------------------";

		xLOG_CTRL->CREATE__SUB_DIRECTORY(sObject_Name);
		xLOG_CTRL->SET__PROPERTY(file_name,24,60);

		xLOG_CTRL->DISABLE__TIME_LOG();
		xLOG_CTRL->WRITE__LOG(log_msg);

		xLOG_CTRL->ENABLE__TIME_LOG();
		xLOG_CTRL->WRITE__LOG("   START   \n");
	}
	return 1;
}
int CObj_Phy__PMC_DUMMY::__START__OBJECT()
{
	xSCH_MATERIAL_CTRL->Link__SERVICE_DB("");
	xCH__OBJ_STATUS->Set__DATA("STANDBY");

	return 1;
}

//-------------------------------------------------------------------------
int CObj_Phy__PMC_DUMMY::__CALL__CONTROL_MODE(mode,p_debug,p_variable,p_alarm)
{
	xI_Sync_Ctrl.Thread__LOCK();
	iPRC_STS = 1;
	iFNC_STS = 1;
	
	xCH__OBJ_STATUS->Set__DATA(STR__CTCINUSE);

	// ...
	DECLARE__EXT_CTRL(p_variable);

	CString upper_obj_name = p_ext_mode_ctrl->Get__UPPER_OBJECT_NAME();

	//.....
	int flag = -1;

	iFLAG__STEP_REPORT = -1;

	Check__PMx_Slot_Max();

	//.....
	CString pmc_name;
	CString msg;

	pmc_name.Format("PM%1d",iPMx_ID);

	msg.Format("[%s] - [%s]  Starting ...",pmc_name,mode);
	xCH__OBJ_MSG->Set__DATA(msg);

	// ...
	{
		CString log_string;
		
		log_string.Format("START : [%s]...  Called By \"%s\".",mode,upper_obj_name);
		xLOG_CTRL->WRITE__LOG(log_string);
	}

	xCH__X_PICK_FLAG->Set__DATA("");

	// ...
	{
			 IF__CTRL_MODE(sMODE__TIME_INIT)			flag = Call__TIME_INIT(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__SYSTEM_INIT)			flag = Call__SYSTEM_INIT(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__AUTO_INIT)			flag = Call__AUTO_INIT(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__SYSTEM_MAINT)			flag = Call__SYSTEM_MAINT(p_variable);

		ELSE_IF__CTRL_MODE(sMODE__PUMP)					flag = Call__PUMP(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__VENT)					flag = Call__VENT(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__PICK_READY)			flag = Call__PICK_READY(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__PICK_X_READY)			flag = Call__PICK_X_READY(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__PICK_COMPLETE)		flag = Call__PICK_COMPLETE(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__PICK_X_COMPLETE)		flag = Call__PICK_X_COMPLETE(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__PLACE_READY)			flag = Call__PLACE_READY(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__PLACE_X_READY)		flag = Call__PLACE_X_READY(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__PLACE_COMPLETE)		flag = Call__PLACE_COMPLETE(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__PLACE_X_COMPLETE)		flag = Call__PLACE_X_COMPLETE(p_variable);

		ELSE_IF__CTRL_MODE(sMODE__PRO_READY)			flag = Call__PRO_READY(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__PRO_START)			flag = Call__PRO_START(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__AUTO_PRO)				flag = Call__AUTO_PRO(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__AUTO_PRO_EX)			flag = Call__AUTO_PRO_EX(p_variable,p_alarm);

		ELSE_IF__CTRL_MODE(sMODE__CLN_READY)			flag = Call__CLN_READY(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__CLN_START)			flag = Call__CLN_START(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__AUTO_CLN)				flag = Call__AUTO_CLN(p_variable,p_alarm);

		ELSE_IF__CTRL_MODE(sMODE__LAYER_CLN_READY)		flag = Call__LAYER_CLN_READY(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__LAYER_CLN_START)		flag = Call__LAYER_CLN_START(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__AUTO_LAYER_CLN)		flag = Call__AUTO_LAYER_CLN(p_variable,p_alarm);

		ELSE_IF__CTRL_MODE(sMODE__MNT_READY)			flag = Call__MNT_READY(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__MNT_START)			flag = Call__MNT_START(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__AUTO_MNT)				flag = Call__AUTO_MNT(p_variable);
	
		ELSE_IF__CTRL_MODE(sMODE__LEAK_CHECK)			flag = Call__LEAK_CHECK(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__LEAK_CHECK_EX)		flag = Call__LEAK_CHECK_EX(p_variable);

		ELSE_IF__CTRL_MODE(sMODE__NEXT_PROC)			flag = Call__NEXT_PROC(p_variable,p_alarm);

		ELSE_IF__CTRL_MODE(sMODE__JOB_START_READY)		flag = Call__JOB_START_READY(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__JOB_END_READY)		flag = Call__JOB_END_READY(p_variable);

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

	if(flag < 0)
	{
		xCH__X_PICK_FLAG->Set__DATA("");
	}

	if(flag > 0)		msg.Format("[%s] - [%s]  Completed ...",pmc_name,mode);
	else				msg.Format("[%s] - [%s]  Aborted ...",pmc_name,mode);

	xCH__OBJ_MSG->Set__DATA(msg);

	// ...
	{
		CString log_string;

		if(flag > 0)		log_string.Format("COMPLETE : [%s]",mode);
		else				log_string.Format("ABORTED  : [%s]",mode);

		xLOG_CTRL->WRITE__LOG(log_string);
	}

	if(iFLAG__STEP_REPORT > 0)		iFLAG__STEP_REPORT =  0;
	else							iFLAG__STEP_REPORT = -1;

	if((flag < 0)||(p_variable->Check__CTRL_ABORT() > 0))
	{
		xCH__OBJ_STATUS->Set__DATA(STR__MAINTMODE);
	}
	else
	{
		if(xCH__OBJ_STATUS->Check__DATA(STR__CTCINUSE) > 0)
		{
			xCH__OBJ_STATUS->Set__DATA(STR__STANDBY);
		}
	}

	iPRC_STS = -1;
	iFNC_STS = -1;

	xI_Sync_Ctrl.Thread__UNLOCK();
	
	return flag;
}
int CObj_Phy__PMC_DUMMY::__CALL__MONITORING(id,p_variable,p_alarm)
{
	switch(id)
	{
		case MON_ID__OBJ_STATUS:
			Mon__Obj_Status(p_variable,p_alarm);
			break;

		case MON_ID__MODULE_STATUS:
			Mon__Module_Status(p_variable,p_alarm);
			break;

		case MON_ID__PRESSURE_STATUS:
			Mon__Pressure_Status(p_variable);
			break;

		case MON_ID__PROCESS_TIME:
			Mon__Process_Time(p_variable);
			break;

		case MON_ID__LEAK_CHECK:
			Mon__Leak_Check(p_variable);
			break;

		case MON_ID__GOTO_MAINT:
			Mon__GoTo_Maint(p_variable);
			break;

		case MON_ID__LPx_WAFER_COUNT:
			Mon__LPx_Wafer_Count(p_variable);
			break;

		case MON_ID__CLEAN_RCP_COUNT:
			Mon__Clean_Rcp_Count(p_variable,p_alarm);
			break;

		case MON_ID__FA_STEP_REPORT:
			Mon__FA_Step_Report(p_variable);
			break;

		case MON_ID__FA_RMS_REPORT:
			Mon__FA_RMS_Report(p_variable);
			break;
	}

	return 1;
}
