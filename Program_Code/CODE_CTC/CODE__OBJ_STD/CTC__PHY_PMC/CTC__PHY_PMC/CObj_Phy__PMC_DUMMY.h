#pragma once

#include "Interface_Code.h"
#include "CCommon_Error.h"

#include "CObj_Phy__PMC_DUMMY__DEF.h"


#define  CFG__PMx_MAX_SLOT							5
#define  CFG__LPx_SIZE								5


class CObj_Phy__PMC_DUMMY : public __IOBJ__STD_TYPE
{
private:
	//-----------------------------------------------------------
	CString sObject_Name;

	CCommon_Error__DEF_VARIABLE mERROR__DEF_VAR;
	CCommon_Error__MODULE_OBJ   mERROR__MODULE_OBJ;

	// ...
	SCI__THREAD_SYNC_CTRL xI_Sync_Ctrl;
	int iPRC_STS;
	int iFNC_STS;

	int iPRC_ALARM_FLAG;

	SCX__CTC__SCH_MATERIAL_CTRL xSCH_MATERIAL_CTRL;
	SCX__USER_LOG_CTRL xLOG_CTRL;
	//


	//-----------------------------------------------------------
	// INTERNAL PROPERTY

	//  STATUS 
	CX__VAR_STRING_CTRL  xCH__OBJ_VIRTUAL_STATUS;
	CX__VAR_STRING_CTRL  xCH__OBJ_VIRTUAL_STAGE1_STATUS;
	CX__VAR_STRING_CTRL  xCH__OBJ_VIRTUAL_STAGE2_STATUS;

	CX__VAR_DIGITAL_CTRL xCH__OBJ_STATUS;
	CX__VAR_STRING_CTRL  xCH__OBJ_MSG;

	CString dVAR__SLIT_VLV_STS;
	CString sVAR__LIFT_PIN_STS;
	CString dVAR__PRC_STS;

	CX__VAR_DIGITAL_CTRL xCH__MOVE_FLAG;
	CX__VAR_STRING_CTRL  xCH__X_PICK_FLAG;
	CX__VAR_STRING_CTRL  xCH__JOB_READY_FLAG;

	CX__VAR_ANALOG_CTRL  xCH__SLOT_NO;

	CX__VAR_STRING_CTRL  xCH__PRESSURE_VALUE;
	CX__VAR_DIGITAL_CTRL xCH__PRESSURE_STATUS;

	//
	CX__VAR_STRING_CTRL  xCH__MODULE_TIME;
	CX__VAR_STRING_CTRL  xCH__LOG_SUB_DIRECTORY;
	CX__VAR_STRING_CTRL  xCH__LOG_FILE_NAME;

	CX__VAR_STRING_CTRL  xCH__SLOT_LOG_SUB_DIRECTORY[CFG__PMx_MAX_SLOT];
	CX__VAR_STRING_CTRL  xCH__SLOT_LOG_FILE_NAME[CFG__PMx_MAX_SLOT];

	//
	CX__VAR_STRING_CTRL  xCH__FDC_RECIPE;
	CX__VAR_STRING_CTRL  xCH__FDC_LOTID;

	//
	CX__VAR_STRING_CTRL  xCH__RECIPE;
	CX__VAR_STRING_CTRL  xCH__LOTID;
	CX__VAR_STRING_CTRL  xCH__MATERIAL_ID;
	CX__VAR_STRING_CTRL  xCH__MATERIAL_SIZE;
	CX__VAR_STRING_CTRL  xCH__LPx_CID;
	CX__VAR_STRING_CTRL  xCH__LPx_PORT_ID;
	CX__VAR_STRING_CTRL  xCH__LPx_SLOT_ID;

	CX__VAR_STRING_CTRL  xCH__SLOT_RECIPE[CFG__PMx_MAX_SLOT];
	CX__VAR_STRING_CTRL  xCH__SLOT_LOTID[CFG__PMx_MAX_SLOT];
	CX__VAR_STRING_CTRL  xCH__SLOT_MATERIALID[CFG__PMx_MAX_SLOT];
	CX__VAR_STRING_CTRL  xCH__SLOT_LPx_CID[CFG__PMx_MAX_SLOT];
	CX__VAR_STRING_CTRL  xCH__SLOT_LPx_PORT_ID[CFG__PMx_MAX_SLOT];
	CX__VAR_STRING_CTRL  xCH__SLOT_LPx_SLOT_ID[CFG__PMx_MAX_SLOT];

	//
	CX__VAR_ANALOG_CTRL  xCH__CFG_PRC_TIME;

	CX__VAR_STRING_CTRL  xCH__PROCESS_FLAG;
	CX__VAR_STRING_CTRL  xCH__TOTAL_PRC_TIME;
	CX__VAR_STRING_CTRL  xCH__CHANGE_PRC_TIME;
	CX__VAR_STRING_CTRL  xCH__CURRENT_PRC_TIME;
	CX__VAR_STRING_CTRL  xCH__PROCESS_ASYNC_TIME;
	CX__VAR_STRING_CTRL  xCH__PROCESS_PERCENT;
	CX__VAR_STRING_CTRL  xCH__PROCESS_STATUS;

	//
	CX__VAR_DIGITAL_CTRL xCH__SLOT_STATUS[CFG__PMx_MAX_SLOT];
	CX__VAR_STRING_CTRL  xCH__SLOT_TITLE[CFG__PMx_MAX_SLOT];

	CX__VAR_STRING_CTRL  xCH__ACC_MATERIAL_COUNT;

	CX__VAR_ANALOG_CTRL  xCH__RCP_PAS__MATERIAL_COUNT;
	CX__VAR_ANALOG_CTRL  xCH__RCP_NP__MATERIAL_COUNT;
	CX__VAR_ANALOG_CTRL  xCH__RCP_ACT__MATERIAL_COUNT;
	CX__VAR_ANALOG_CTRL  xCH__RCP_OTHER__MATERIAL_COUNT;
	CX__VAR_ANALOG_CTRL  xCH__RCP_TOTAL__MATERIAL_COUNT;

	// ...
	CX__VAR_DIGITAL_CTRL xCH_CFG__PMC_USE;

	CX__VAR_STRING_CTRL  xCH__NEXT_PROCESS;
	CX__VAR_DIGITAL_CTRL xCH_CFG__AUTO_MATERIAL_COUNT_FLAG;

	CX__VAR_STRING_CTRL  xCH_CFG__CLEAN_MODE;
	CX__VAR_DIGITAL_CTRL xCH_CFG__CLEAN_USE;
	CX__VAR_ANALOG_CTRL  xCH_CFG__CLEAN_FREQUENCY;
	CX__VAR_ANALOG_CTRL  xCH_CFG__CLEAN_COUNT;
	CX__VAR_STRING_CTRL  xCH_CFG__CLEAN_RECIPE;

	//
	CX__VAR_STRING_CTRL  xCH__LAYER_CLEAN_RECIPE;

	CX__VAR_ANALOG_CTRL  xCH_CFG__CLEAN_SYS_IDLE_REF_DELAY_TIME;
	CX__VAR_ANALOG_CTRL  xCH_CFG__CLEAN_FIRST_COUNT;
	CX__VAR_ANALOG_CTRL  xCH_CFG__CLEAN_SECOND_COUNT;

	CX__VAR_DIGITAL_CTRL xCH_CFG__CLEAN_RF_USE;
	CX__VAR_ANALOG_CTRL  xCH_CFG__CLEAN_RF_REF_TIME;
	CX__VAR_ANALOG_CTRL  xCH_CFG__CLEAN_RF_CUR_TIME;
	CX__VAR_STRING_CTRL  xCH_CFG__CLEAN_RF_RECIPE;

	// ...
	CX__VAR_DIGITAL_CTRL xCH_CFG__AUTO_LEAK_CHECK_USE;
	CX__VAR_ANALOG_CTRL  xCH_CFG__AUTO_LEAK_CHECK_REF_TIME;
	CX__VAR_ANALOG_CTRL  xCH_CFG__AUTO_LEAK_CHECK_CUR_TIME;
	CX__VAR_DIGITAL_CTRL xCH_CFG__AUTO_LEAK_CHECK_MODE;

	CX__VAR_STRING_CTRL  xCH__AUTO_LEAK_CHECK_RUN_FLAG;
	CX__VAR_STRING_CTRL  xCH__AUTO_LEAK_CHECK_RUN_DAY;

	// ...
	CX__VAR_STRING_CTRL  xCH__STEP_START;
	CX__VAR_STRING_CTRL  xCH__STEP_END;

	CX__VAR_STRING_CTRL  xCH__RMS_ID;
	CX__VAR_STRING_CTRL  xCH__RMS_ACT;
	CX__VAR_STRING_CTRL  xCH__RMS_FILE_TYPE;
	CX__VAR_STRING_CTRL  xCH__RMS_FILE_NAME;
	CX__VAR_STRING_CTRL  xCH__RMS_CREATE_TIME;
	CX__VAR_STRING_CTRL  xCH__RMS_UPDATE_TIME;

	// ...
	CX__VAR_STRING_CTRL  xCH__MATERIAL_CLEAR_REQ;
	CX__VAR_STRING_CTRL  xCH__MATERIAL_CLEAR_SLOT;

	CX__VAR_STRING_CTRL  xCH__LPx_WAFER_COUNT;
	CX__VAR_STRING_CTRL  xCH_CFG__LPx_WAFER_COUNT[CFG__LPx_SIZE];

	// ...
	CX__VAR_DIGITAL_CTRL xCH__ALARM_LOG_MODE;

	void Post__PMx_ALARM(CII_OBJECT__ALARM *p_alarm);
	void Clear__PMx_ALARM(CII_OBJECT__ALARM *p_alarm);

	// ...
	SCX__ASYNC_TIMER_CTRL xTIMER__SYS_IDLE;
	CX__VAR_ANALOG_CTRL   xCH__SYS_IDLE_TIME;
	//

	//-----------------------------------------------------------
	// EXTERNAL PROPERTY

	int iLOG_ID;
	int iLOG_SLOT_MAX;
	int iSEQ_ID;
	int iPMx_ID;
	int iPMx_SLOT_MAX;

	// ...
	CX__VAR_DIGITAL_CTRL xEXT_CH_CFG__PMC_USE;
	CX__VAR_DIGITAL_CTRL xEXT_CH_CFG__PMC_STAGE1_USE;
	CX__VAR_DIGITAL_CTRL xEXT_CH_CFG__PMC_STAGE2_USE;

	CX__VAR_DIGITAL_CTRL xEXT_CH_CFG__PMC_SLOT_MAX;

	CX__VAR_ANALOG_CTRL  xEXT_CH_CFG__REF_ATM_PRESSURE;
	CX__VAR_ANALOG_CTRL  xEXT_CH_CFG__REF_VAC_PRESSURE;

	// LEAK CHECK TIME ...
	CX__VAR_STRING_CTRL  xEXT_CH__LEAK_CHECK__SCH_SEQUENCE_MODE__REQ_FLAG;

	CX__VAR_DIGITAL_CTRL xEXT_CH_CFG__LEAK_CHECK__TIME_MODE;

	CX__VAR_ANALOG_CTRL  xEXT_CH_CFG__LEAK_CHECK_START_TIME__HOUR;
	CX__VAR_ANALOG_CTRL  xEXT_CH_CFG__LEAK_CHECK_START_TIME__MINUTE;
	CX__VAR_ANALOG_CTRL  xEXT_CH_CFG__LEAK_CHECK_START_TIME__SECOND;

	CX__VAR_ANALOG_CTRL  xEXT_CH_CFG__LEAK_CHECK_END_TIME__HOUR;
	CX__VAR_ANALOG_CTRL  xEXT_CH_CFG__LEAK_CHECK_END_TIME__MINUTE;
	CX__VAR_ANALOG_CTRL  xEXT_CH_CFG__LEAK_CHECK_END_TIME__SECOND;	

	CX__VAR_STRING_CTRL  xEXT_CH__PMC_LEAK_CHECK_FLAG;

	// ...
	CX__VAR_STRING_CTRL  xEXT_CH_CFG__PMC_MSG_WIN_CTRL;
	CX__VAR_STRING_CTRL  xEXT_CH_CFG__PMC_MSG_WIN_NAME;

	CX__VAR_STRING_CTRL  xEXT_CH__LPx_JOB_START_FLAG[CFG__LPx_SIZE];

	CX__VAR_DIGITAL_CTRL xEXT_CH__PMC_CONNECT_STATUS;
	//


	//-----------------------------------------------------------
	CII_EXT_OBJECT__CTRL *pOBJ_CTRL;

	// ...
	CString sMODE__TIME_INIT;
	int  Call__TIME_INIT(CII_OBJECT__VARIABLE* p_variable);

	CString sMODE__SYSTEM_INIT;
	int  Call__SYSTEM_INIT(CII_OBJECT__VARIABLE* p_variable);

	CString sMODE__AUTO_INIT;
	int  Call__AUTO_INIT(CII_OBJECT__VARIABLE* p_variable);

	// ...
	CString sMODE__SYSTEM_MAINT;
	int  Call__SYSTEM_MAINT(CII_OBJECT__VARIABLE* p_variable);

	//
	CString sMODE__PUMP;
	int  Call__PUMP(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM *p_alarm);

	CString sMODE__VENT;
	int  Call__VENT(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM *p_alarm);

	//
	CString sMODE__PICK_READY;
	int  Call__PICK_READY(CII_OBJECT__VARIABLE* p_variable);

	CString sMODE__PICK_X_READY;
	int  Call__PICK_X_READY(CII_OBJECT__VARIABLE* p_variable);
	int  Seq__PICK_X_READY(CII_OBJECT__VARIABLE* p_variable);

	CString sMODE__PICK_COMPLETE;
	int  Call__PICK_COMPLETE(CII_OBJECT__VARIABLE* p_variable);

	CString sMODE__PICK_X_COMPLETE;
	int  Call__PICK_X_COMPLETE(CII_OBJECT__VARIABLE* p_variable);

	//
	CString sMODE__PLACE_READY;
	int  Call__PLACE_READY(CII_OBJECT__VARIABLE* p_variable);

	CString sMODE__PLACE_X_READY;
	int  Call__PLACE_X_READY(CII_OBJECT__VARIABLE* p_variable);

	CString sMODE__PLACE_COMPLETE;
	int  Call__PLACE_COMPLETE(CII_OBJECT__VARIABLE* p_variable);
	int  Seq__PLACE_COMPLETE(CII_OBJECT__VARIABLE* p_variable);

	CString sMODE__PLACE_X_COMPLETE;
	int  Call__PLACE_X_COMPLETE(CII_OBJECT__VARIABLE* p_variable);

	//
	CString sMODE__PRO_READY;
	int  Call__PRO_READY(CII_OBJECT__VARIABLE* p_variable);
	int  Seq__PRO_READY(CII_OBJECT__VARIABLE* p_variable);

	CString sMODE__PRO_START;
	int  Call__PRO_START(CII_OBJECT__VARIABLE* p_variable,
							CII_OBJECT__ALARM* p_alarm,
							const int fa_report_flag = -1);
	int  Seq__PRO_START(CII_OBJECT__VARIABLE* p_variable,
							CII_OBJECT__ALARM* p_alarm,
							const int fa_report_flag);

	CString sMODE__AUTO_PRO;
	int  Call__AUTO_PRO(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	CString sMODE__AUTO_PRO_EX;
	int  Call__AUTO_PRO_EX(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	//
	CString sMODE__CLN_READY;
	int  Call__CLN_READY(CII_OBJECT__VARIABLE* p_variable);

	CString sMODE__CLN_START;
	int  Call__CLN_START(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	CString sMODE__AUTO_CLN;
	int  Call__AUTO_CLN(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	//
	CString sMODE__LAYER_CLN_READY;
	int  Call__LAYER_CLN_READY(CII_OBJECT__VARIABLE* p_variable);

	CString sMODE__LAYER_CLN_START;
	int  Call__LAYER_CLN_START(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	CString sMODE__AUTO_LAYER_CLN;
	int  Call__AUTO_LAYER_CLN(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	//
	CString sMODE__MNT_READY;
	int  Call__MNT_READY(CII_OBJECT__VARIABLE* p_variable);

	CString sMODE__MNT_START;
	int  Call__MNT_START(CII_OBJECT__VARIABLE* p_variable);

	CString sMODE__AUTO_MNT;
	int  Call__AUTO_MNT(CII_OBJECT__VARIABLE* p_variable);

	//
	CString sMODE__LEAK_CHECK;
	int  Call__LEAK_CHECK(CII_OBJECT__VARIABLE* p_variable);

	CString sMODE__LEAK_CHECK_EX;
	int  Call__LEAK_CHECK_EX(CII_OBJECT__VARIABLE* p_variable);

	//
	CString sMODE__NEXT_PROC;
	int  Call__NEXT_PROC(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	//
	CString sMODE__JOB_START_READY;
	int  Call__JOB_START_READY(CII_OBJECT__VARIABLE* p_variable);

	CString sMODE__JOB_END_READY;
	int  Call__JOB_END_READY(CII_OBJECT__VARIABLE* p_variable);

	void Fnc__SET_TIME();
	//

	// ...
	void Mon__Obj_Status(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);
	void Mon__Module_Status(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);
	void Mon__Pressure_Status(CII_OBJECT__VARIABLE *p_variable);
	void Mon__Process_Time(CII_OBJECT__VARIABLE *p_variable);
	void Mon__Leak_Check(CII_OBJECT__VARIABLE *p_variable);
	void Mon__GoTo_Maint(CII_OBJECT__VARIABLE *p_variable);
	void Mon__LPx_Wafer_Count(CII_OBJECT__VARIABLE *p_variable);
	void Mon__Clean_Rcp_Count(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);

	int  iFLAG__STEP_REPORT;
	void Mon__FA_Step_Report(CII_OBJECT__VARIABLE* p_variable);

	void Mon__FA_RMS_Report(CII_OBJECT__VARIABLE* p_variable);

	// ...
	void Set__Module_Time();
	void Set__Place_Ready_Parameter(CII_OBJECT__VARIABLE *p_variable);
	void Set__Slot_No_Parameter(CII_OBJECT__VARIABLE *p_variable);
	void Set__Prc_Ready_Parameter(CII_OBJECT__VARIABLE *p_variable);
	void Set__Cln_Ready_Parameter(CII_OBJECT__VARIABLE *p_variable);
	void Set__Layer_Cln_Ready_Parameter(CII_OBJECT__VARIABLE *p_variable);

	void Set__Slot_Info(CII_OBJECT__VARIABLE *p_variable);

	void Clear__PMC_Info();

	void Check__Material_Count(CII_OBJECT__VARIABLE *p_variable);
	void Check__RF_Time(CII_OBJECT__VARIABLE *p_variable);
	void Check__Material_Clear();

	int  Check__Standby_Or_CtcInUse();
	void Check__PMx_Slot_Max();
	
	int  Get__PMx_Occupied_Slot();

	// ...
	void Datalog__Write_Lot_Slot(const int slot_log_count,
									const int pm_slot_id,
									const IDS__SCH_MATERIAL_INFO& db_info,
									CString& pm_log_path);

	void Datalog__Write_Lot_Slot_End(const int pm_slot_id,
										const CString& lotid,
										const CString& lp_id,
										const int lp_slot_id);
	//


public:
	int Is_Available();
	int Is_Ready();

	CString Get_SlotStatus(const int slot_id);
	CString Get_SlotTitle(const int slot_id);

public:
	CObj_Phy__PMC_DUMMY();
	~CObj_Phy__PMC_DUMMY();

	//-------------------------------------------------------------------------
	int __DEFINE__CONTROL_MODE(obj,l_mode);
	int __DEFINE__VERSION_HISTORY(version);

	int __DEFINE__VARIABLE_STD(p_variable);
	int __DEFINE__ALARM(p_alarm);

	//-------------------------------------------------------------------------
	int __INITIALIZE__OBJECT(p_variable,p_ext_obj_create);
	int __START__OBJECT();

	//-------------------------------------------------------------------------
	int __CALL__CONTROL_MODE(mode,p_debug,p_variable,p_alarm);
	int __CALL__MONITORING(id,p_variable,p_alarm);
};
