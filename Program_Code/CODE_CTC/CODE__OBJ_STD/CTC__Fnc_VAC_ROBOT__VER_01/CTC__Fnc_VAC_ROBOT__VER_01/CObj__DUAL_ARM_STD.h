#pragma once

#include "Interface_Code.h"

#include "CCommon_Error.h"
#include "CCommon_Define.h"
#include "CCommon_System.h"

#include "CObj__DUAL_ARM_STD__ALID.h"
#include "CObj__DUAL_ARM_STD__DEF.h"


class CObj__DUAL_ARM_STD : public __IOBJ__STD_TYPE
{
private:
	//-------------------------------------------------------------------------
	CString sObject_Name;
	int iPMx_SIZE;
	int iJOB_FLAG;

	CCommon_Error__DEF_VARIABLE mERROR__DEF_VAR;
	CCommon_Error__USER_FNC     mERROR__USER_FNC;

	SCX__CTC__SCH_MATERIAL_CTRL xSCH_MATERIAL_CTRL;
	SCX__ASYNC_TIMER_CTRL xTIMER__WAFER_ON_ARM;

	SCX__USER_LOG_CTRL xAPP_LOG_CTRL;
	int iLOG_CTRL_FLAG;

	void Fnc__LOG_CTRL(const CString& log_msg);


	//-------------------------------------------------------------------------
	//  INTERFACE PROPERTY

	int iActive_SIM;
	int iActive__FROMT_PMo;

	CX__VAR_STRING_CTRL  sCH__SCH_LOOP_ID;
	CX__VAR_DIGITAL_CTRL dCH__QUERY_LOG_MODE;

	// WAC ...
	CX__VAR_DIGITAL_CTRL dCH__INFO_WAC_FRONT_ACTIVE;

	// CFG PHYSICAL ...
	CX__VAR_ANALOG_CTRL  aCH__CFG_WAFER_WAITING_LIMIT_TIME_ON_ARM;
	CX__VAR_STRING_CTRL  sCH__CUR_WAFER_WAITING_LIMIT_TIME_ON_ARM;

	CX__VAR_ANALOG_CTRL  xCH_CFG__PHY_FRONT_TIME;
	CX__VAR_ANALOG_CTRL  xCH_CFG__PHY_PICK_TIME;
	CX__VAR_ANALOG_CTRL  xCH_CFG__PHY_PLACE_TIME;

	CX__VAR_ANALOG_CTRL  xCH_CFG__PHY_CONCURRENT_MARGIN_TIME;
	CX__VAR_ANALOG_CTRL  xCH_CFG__PMC_CLN_PROC_TIME_FOR_LBx_PICK;

	CX__VAR_DIGITAL_CTRL xCH_CFG__PICK_ARM_TYPE_FROM_LBX;
	CX__VAR_DIGITAL_CTRL xCH_CFG__PICK_ARM_TYPE_FROM_PMC;

	CX__VAR_DIGITAL_CTRL xCH_CFG__A_ARM_PLACE_TO_LBx_SLOT;
	CX__VAR_DIGITAL_CTRL xCH_CFG__B_ARM_PLACE_TO_LBx_SLOT;

	CX__VAR_DIGITAL_CTRL xCH_CFG__ROBOT_ARM_DIRECTION_TYPE;

	//
	CX__VAR_DIGITAL_CTRL xCH_CFG__SCH_EXCHANGE_PRIORITY;
	CX__VAR_DIGITAL_CTRL xCH_CFG__SCH_LLx_EXCHANGE_WHEN_PMC_EMPTY;

	// ...
	CX__VAR_DIGITAL_CTRL xCH_CFG__PICK_DELAY_FLAG;
	CX__VAR_ANALOG_CTRL  xCH_CFG__PICK_DELAY_FIRST_TIME;
	CX__VAR_ANALOG_CTRL  xCH_CFG__PICK_DELAY_SECOND_TIME;
	CX__VAR_ANALOG_CTRL  xCH_CFG__PICK_DELAY_THIRD_TIME;
	CX__VAR_ANALOG_CTRL  xCH_CFG__PICK_DELAY_REMAIN_TIME;

	CX__VAR_DIGITAL_CTRL xCH_CFG__CLEAN_MATERIAL_FLAG;
	CX__VAR_DIGITAL_CTRL xCH_CFG__MATERIAL_COUNT_FLAG;

	CX__VAR_DIGITAL_CTRL xCH_CFG__PMx_SWAP_FLAG;
	CX__VAR_DIGITAL_CTRL xCH_CFG__LBx_SWAP_FLAG;

	//
	CX__VAR_ANALOG_CTRL  xCH_CFG__PICK_COUNT_FROM_LBx;
	CX__VAR_ANALOG_CTRL  xCH_CFG__PICK_DELAY_TIME;

	SCX__ASYNC_TIMER_CTRL xTIMER_CTRL__LLx_PICK_DELAY;

	// ...
	CX__VAR_STRING_CTRL  xCH__CHECK_MODULE;

	// ...
	CX__VAR_STRING_CTRL  xCH__INFOx__MATERIAL_TO_LP[CFG_LP_LIMIT];

	// ...
	CX__VAR_STRING_CTRL  xCH__VAC_RB_PAUSE_FLAG;

	// ...
	CX__VAR_STRING_CTRL  xCH__SCH_DB_CHECK;

	CX__VAR_DIGITAL_CTRL xCH_CFG__SCH_LLx_PREPMATER_USE;
	CX__VAR_DIGITAL_CTRL xCH_CFG__SCH_LLx_COMPMATER_USE;

	// ...
	int iPMx_IN_COUNT;
	CX__VAR_STRING_CTRL  xCH__PMx__IN_COUNT[CFG_PM_LIMIT];

	// REPORT : MATERIAL_USER_INFO ...
	CX__VAR_STRING_CTRL  sCH__REPORT_TRG_NAME_OF_ARM_A;
	CX__VAR_STRING_CTRL  sCH__REPORT_TRG_NAME_OF_ARM_B;

	CX__VAR_STRING_CTRL  sCH__REPORT_TRG_SLOT_OF_ARM_A;
	CX__VAR_STRING_CTRL  sCH__REPORT_TRG_SLOT_OF_ARM_B;

	CX__VAR_STRING_CTRL  sCH__REPORT_TRG_ANGLE_OF_ARM_A;
	CX__VAR_STRING_CTRL  sCH__REPORT_TRG_ANGLE_OF_ARM_B;

	//
	CX__VAR_STRING_CTRL  sCH__REPORT_GOTO_PM_OF_ARM_A;
	CX__VAR_STRING_CTRL  sCH__REPORT_GOTO_PM_OF_ARM_B;
	//


	//-------------------------------------------------------------------------
	//  EXTERNAL PROPERTY

	// OBJ : DB_CFG ...

	// JOB START READY ...
	CX__VAR_DIGITAL_CTRL  dEXT_CH__CFG_JOB_START_READY_CTRL_MODE;
	CX__VAR_DIGITAL_CTRL  dEXT_CH__CFG_JOB_START_READY_PMx_USE[CFG_PM_LIMIT];

	// SYSTEM IDLE ...
	CX__VAR_ANALOG_CTRL   aEXT_CH__SYSTEM_IDLE_CFG_TIME_SEC;
	CX__VAR_STRING_CTRL   sEXT_CH__SYSTEM_IDLE_CUR_TIME_SEC;

	CX__VAR_DIGITAL_CTRL  dEXT_CH__SYSTEM_IDLE_CFG_PMx_USE[CFG_PM_LIMIT];
	CX__VAR_ANALOG_CTRL   aEXT_CH__SYSTEM_IDLE_CFG_PMx_SEC[CFG_PM_LIMIT];

	// PRE_RCP ...
	CX__VAR_DIGITAL_CTRL  dEXT_CH__PRE_RCP_CFG_PMx_USE[CFG_PM_LIMIT];
	CX__VAR_ANALOG_CTRL   aEXT_CH__PRE_RCP_CFG_PMx_WAFER_COUNT[CFG_PM_LIMIT];

	// POST_RCP ...
	CX__VAR_DIGITAL_CTRL  dEXT_CH__POST_RCP_CFG_PMx_USE[CFG_PM_LIMIT];
	CX__VAR_ANALOG_CTRL   aEXT_CH__POST_RCP_CFG_PMx_WAFER_COUNT[CFG_PM_LIMIT];

	// TRANSFER
	CX__VAR_DIGITAL_CTRL  xEXT_CH__CFG__TRANSFER_MODE;
	CX__VAR_ANALOG_CTRL   xEXT_CH__CFG__REF_ATM_PRESSURE;
	CX__VAR_ANALOG_CTRL   xEXT_CH__CFG__REF_VAC_PRESSURE;

	// SCHEDULER 
	CX__VAR_DIGITAL_CTRL  xEXT_CH__SCH_CONCURRENT_ALWAYS_APPLY;

	CX__VAR_STRING_CTRL   sEXT_CH__PMx__CUR_PRE_COUNT[CFG_PM_LIMIT];
	CX__VAR_STRING_CTRL   sEXT_CH__PMx__CUR_POST_COUNT[CFG_PM_LIMIT];
	CX__VAR_STRING_CTRL   sEXT_CH__PMx__CUR_MAIN_COUNT[CFG_PM_LIMIT];

	// SCH.MODE.XXX ...
	CX__VAR_STRING_CTRL   sEXT_CH__SCH_MODE_LLx_IN;
	CX__VAR_STRING_CTRL   sEXT_CH__SCH_MODE_LLx_OUT;

	// CFG LLx ...
	CX__VAR_DIGITAL_CTRL  xEXT_CH__LLx__CFG_PREPMATER_USE[CFG_LLx_LIMIT];
	CX__VAR_DIGITAL_CTRL  xEXT_CH__LLx__CFG_COMPMATER_USE[CFG_LLx_LIMIT];

	CX__VAR_DIGITAL_CTRL  xEXT_CH__LLx__CFG_PRE_RCP_USE[CFG_LLx_LIMIT];
	CX__VAR_ANALOG_CTRL   xEXT_CH__LLx__CFG_PRE_RCP_WAFER_COUNT[CFG_LLx_LIMIT];

	CX__VAR_DIGITAL_CTRL  xEXT_CH__LLx__CFG_POST_RCP_USE[CFG_LLx_LIMIT];
	CX__VAR_ANALOG_CTRL   xEXT_CH__LLx__CFG_POST_RCP_WAFER_COUNT[CFG_LLx_LIMIT];

	// SCH LLx ...
	CX__VAR_DIGITAL_CTRL  xEXT_CH__SCH_DB_LLx_USE_FLAG[CFG_LLx_LIMIT];
	CX__VAR_DIGITAL_CTRL  xEXT_CH__SCH_DB_LLx_SLOT_MAX[CFG_LLx_LIMIT];

	CX__VAR_DIGITAL_CTRL  xEXT_CH__SCH_DB_LLx_MODE_TYPE[CFG_LLx_LIMIT];
	CX__VAR_DIGITAL_CTRL  dEXT_CH__CFG_DB_LLx_SLOT_STATUS[CFG_LLx_LIMIT][CFG_LLx__SLOT_MAXx];
	CX__VAR_DIGITAL_CTRL  dEXT_CH__SCH_DB_LLx_SLOT_STATUS[CFG_LLx_LIMIT][CFG_LLx__SLOT_MAXx];
	CX__VAR_DIGITAL_CTRL  xEXT_CH__SCH_DB_LLx_SLOT_MODE[CFG_LLx_LIMIT][CFG_LLx__SLOT_MAXx];

	CX__VAR_STRING_CTRL   xEXT_CH__ATM_RB__SCH_STS_TO_LLx[CFG_LLx_LIMIT];

	// PMx
	CX__VAR_DIGITAL_CTRL  xEXT_CH__CFG__PMx_USE[CFG_PM_LIMIT];
	CX__VAR_DIGITAL_CTRL  xEXT_CH__CFG__PMx_LEFT_STAGE_USE[CFG_PM_LIMIT];
	CX__VAR_DIGITAL_CTRL  xEXT_CH__CFG__PMx_RIGHT_STAGE_USE[CFG_PM_LIMIT];
	CX__VAR_STRING_CTRL   xEXT_CH__PMx_LEAK_CHECK_FLAG[CFG_PM_LIMIT];
	CX__VAR_STRING_CTRL   xEXT_CH__PMx_AUTO_PM_FLAG[CFG_PM_LIMIT];
	CX__VAR_DIGITAL_CTRL  xEXT_CH__PMx_PROCESS_TYPE[CFG_PM_LIMIT];

	//
	CX__VAR_STRING_CTRL   xEXT_CH__VAC_AREA_MATERIAL_COUNT;
	CX__VAR_STRING_CTRL   xEXT_CH__VAC_ROBOT_OUT_MATERIAL_COUNT;
	CX__VAR_STRING_CTRL   xEXT_CH__PMC_MATERIAL_COUNT;
	CX__VAR_STRING_CTRL   xEXT_CH__TOTAL_MATERIAL_COUNT_IN_SYSTEM;

	CX__VAR_STRING_CTRL   xEXT_CH__SYSTEM_LEAK_CHECK_REQ;
	CX__VAR_STRING_CTRL   xEXT_CH__SYSTEM_AUTO_PM_REQ;

	//
	CX__VAR_DIGITAL_CTRL xEXT_CH__CFG__PRESSURE_CHECK_MODE;
	CX__VAR_DIGITAL_CTRL xEXT_CH__CFG__TOOL_CLEAR_PMC_MATERIAL_MODE;

	// WAC.PMx ...
	CX__VAR_DIGITAL_CTRL dEXT_CH__SCH__WAC_PMC_WAFER_TYPE;

	CX__VAR_DIGITAL_CTRL dEXT_CH__SCH__WAC_PM_USE_X[CFG_PM_LIMIT];
	CX__VAR_DIGITAL_CTRL dEXT_CH__SCH__WAC_PM_PICK_CHECK_X[CFG_PM_LIMIT];
	CX__VAR_DIGITAL_CTRL dEXT_CH__SCH__WAC_PM_PLACE_CHECK_X[CFG_PM_LIMIT];
	CX__VAR_DIGITAL_CTRL dEXT_CH__SCH__WAC_PM_PRE_FRONT_X[CFG_PM_LIMIT];
	CX__VAR_DIGITAL_CTRL dEXT_CH__SCH__WAC_PM_POS_ID_X[CFG_PM_LIMIT];
	CX__VAR_DIGITAL_CTRL dEXT_CH__SCH__WAC_PM_POS_SLOT_X[CFG_PM_LIMIT];
	CX__VAR_ANALOG_CTRL  aEXT_CH__SCH__WAC_PM_DELAY_SEC_X[CFG_PM_LIMIT];

	// JOB.LPX ...
	CX__VAR_STRING_CTRL  xEXT_CH__SCH__LP_JOB_LIST[CFG_LP_LIMIT];

	// JOB CTRL
	CX__VAR_STRING_CTRL  xEXT_CH__JOB_CTRL;
	//


	//-------------------------------------------------------------------------
	// STATUS CHANNEL

	// OBJ : VAC_ROBOT
	int Run__ROBOT_OBJ(const CString& obj_mode, const CString& log_id = "");
	int Run__ROBOT_OBJ(const CString& obj_mode, const CStringArray& l_para, const CString& log_id = "");

	int Call__ROBOT_OBJ(const CString& obj_mode, const CString& log_id = "");
	int Call__ROBOT_OBJ(const CString& obj_mode, CStringArray& l_para, const CString& log_id = "");

	//
	CII_EXT_OBJECT__CTRL *pVAC_RB__OBJ_CTRL;

	CX__VAR_DIGITAL_CTRL xCH__VAC_RB__OBJ_CTRL;
	CX__VAR_DIGITAL_CTRL xCH__VAC_RB__OBJ_MODE;
	CX__VAR_DIGITAL_CTRL xCH__VAC_RB__OBJ_STATUS;

	CX__VAR_DIGITAL_CTRL xCH__VAC_RB__SLOT01_STATUS;
	CX__VAR_DIGITAL_CTRL xCH__VAC_RB__SLOT02_STATUS;

	CX__VAR_STRING_CTRL  xCH__VAC_RB__SLOT01_TITLE;
	CX__VAR_STRING_CTRL  xCH__VAC_RB__SLOT02_TITLE;

	CX__VAR_DIGITAL_CTRL xCH__VAC_RB__ROTE_ARM;
	CX__VAR_DIGITAL_CTRL xCH__VAC_RB__TRG_ROTE;

	CX__VAR_DIGITAL_CTRL xCH__VAC_RB__CFG_A_ARM_USE_MODE;
	CX__VAR_DIGITAL_CTRL xCH__VAC_RB__CFG_B_ARM_USE_MODE;

	CX__VAR_DIGITAL_CTRL dCH__VAC_RB__INFO_PM_RESERVE_X[CFG_PM_LIMIT];

	// LLx : Scheduler - Dual Only Input & Output ...
	CX__VAR_DIGITAL_CTRL dCH__VAC_RB__CFG_DUAL_ARM_MOVING_AT_THE_SAME_TIME;
	CX__VAR_DIGITAL_CTRL dCH__VAC_RB__CFG_LLx_CTRL_ONLY_INPUT_OUTPUT_MODE;

	// ARM-CONSTRAINT ...
	CX__VAR_DIGITAL_CTRL dCH__VAC_RB__CFG_A_ARM_CONSTRAINT_LL;
	CX__VAR_DIGITAL_CTRL dCH__VAC_RB__CFG_A_ARM_CONSTRAINT_PM;

	CX__VAR_DIGITAL_CTRL dCH__VAC_RB__CFG_B_ARM_CONSTRAINT_LL;
	CX__VAR_DIGITAL_CTRL dCH__VAC_RB__CFG_B_ARM_CONSTRAINT_PM;

	CX__VAR_DIGITAL_CTRL dCH__VAC_RB__CFG_PMx_To_PMx_CONSTRAINT;
	CX__VAR_DIGITAL_CTRL dCH__VAC_RB__CFG_PMx_To_PMx_MOVE_PATH;
	CX__VAR_DIGITAL_CTRL dCH__VAC_RB__CFG_PMx_To_PMx_PICK_CONDITION;

	// CFG : WAFER PICK PARAMETER ...
	CX__VAR_DIGITAL_CTRL dCH__VAC_RB__CFG_PICK_WAFER_CONDITION;

	// OBJ : TMC_CHM ...
	int Run__TMC_OBJ(const CString& obj_mode);
	int Call__TMC_OBJ(const CString& obj_mode);

	//
	CII_EXT_OBJECT__CTRL *pTMC_CHM__OBJ_CTRL;

	CX__VAR_DIGITAL_CTRL xCH__TMC_CHM__OBJ_CTRL;
	CX__VAR_DIGITAL_CTRL xCH__TMC_CHM__OBJ_MODE;
	CX__VAR_DIGITAL_CTRL xCH__TMC_CHM__OBJ_STATUS;

	CX__VAR_DIGITAL_CTRL xCH__TMC_CHM__PRESSURE_STATUS;


	// OBJ : LLx ...
	int Run__LLx_OBJ(const int ll_i, const CString& obj_mode);
	int Run__LLx_OBJ(const int ll_i, const CString& obj_mode, const CStringArray& l_para);

	int Call__LLx_OBJ(const int ll_i, const CString& obj_mode);
	int Call__LLx_OBJ(const int ll_i, const CString& obj_mode, CStringArray& l_para);

	int Set__COOLING_SEC_OF_LLx_OBJ(const int ll_i, const CString& obj_mode);

	//
	int iLBx_OUT_COUNT;

	int iLLx_SLOT_MAX[CFG_LLx_LIMIT];
	int iLLx_LIMIT;

	// 
	CII_EXT_OBJECT__CTRL *pLLx__OBJ_CTRL[CFG_LLx_LIMIT];

	CX__VAR_ANALOG_CTRL  aEXT_CH__LLx__PARA_VENT_COOLING_SEC[CFG_LLx_LIMIT];

	CX__VAR_STRING_CTRL  sEXT_CH__LLx__UPPER_OBJ_MSG[CFG_LLx_LIMIT];

	CX__VAR_STRING_CTRL  sEXT_CH__LLx__NEXT_CTRL_REQ[CFG_LLx_LIMIT];
	CX__VAR_DIGITAL_CTRL xEXT_CH__LLx__OBJ_CTRL[CFG_LLx_LIMIT];
	CX__VAR_DIGITAL_CTRL xEXT_CH__LLx__OBJ_MODE[CFG_LLx_LIMIT];
	CX__VAR_DIGITAL_CTRL xEXT_CH__LLx__OBJ_STATUS[CFG_LLx_LIMIT];

	CX__VAR_DIGITAL_CTRL dEXT_CH__LLx__ACTIVE_VAC_AREA_CTRL[CFG_LLx_LIMIT];
	CX__VAR_DIGITAL_CTRL dEXT_CH__LLx__ACTIVE_ATM_AREA_CTRL[CFG_LLx_LIMIT];

	CX__VAR_STRING_CTRL  xEXT_CH__LLx__IN_OUT_FLAG[CFG_LLx_LIMIT];
	CX__VAR_STRING_CTRL  xEXT_CH__LLx__IN_COUNT[CFG_LLx_LIMIT];
	CX__VAR_STRING_CTRL  xEXT_CH__LLx__OUT_COUNT[CFG_LLx_LIMIT];
	CX__VAR_STRING_CTRL  xEXT_CH__LLx__IDLE_COUNT[CFG_LLx_LIMIT];

	CX__VAR_DIGITAL_CTRL xEXT_CH__LLx__PRESSURE_STATUS[CFG_LLx_LIMIT];
	CX__VAR_ANALOG_CTRL  xEXT_CH__LLx__PRESSURE_VALUE[CFG_LLx_LIMIT];

	CX__VAR_DIGITAL_CTRL xEXT_CH__LLx__SLOT_STATUS[CFG_LLx_LIMIT][CFG_LLx__SLOT_MAXx];
	CX__VAR_STRING_CTRL  xEXT_CH__LLx__SLOT_TITLE[CFG_LLx_LIMIT][CFG_LLx__SLOT_MAXx];

	CX__VAR_STRING_CTRL  xEXT_CH__LLx__VENTING_FLAG[CFG_LLx_LIMIT];
	CX__VAR_STRING_CTRL  xEXT_CH__LLx__VENTING_COMP_FLAG[CFG_LLx_LIMIT];

	CX__VAR_STRING_CTRL  sEXT_CH__LLx__PARA_PROC_RECIPE_NAME[CFG_LLx_LIMIT];
	CX__VAR_STRING_CTRL  sEXT_CH__LLx__PARA_PROC_SCH_NAME[CFG_LLx_LIMIT];


	// OBJ : PMx ...
	int Run__PMx_OBJ(const int pm_i, const CString& obj_mode);
	int Run__PMx_OBJ(const int pm_i, const CString& obj_mode, const CStringArray& l_para);

	int Call__PMx_OBJ(const int pm_i, const CString& obj_mode);
	int Call__PMx_OBJ(const int pm_i, const CString& obj_mode, CStringArray& l_para);

	//
	CII_EXT_OBJECT__CTRL *pPMx__OBJ_CTRL[CFG_PM_LIMIT];

	CX__VAR_DIGITAL_CTRL xCH__PMx__OBJ_CTRL[CFG_PM_LIMIT];
	CX__VAR_DIGITAL_CTRL xCH__PMx__OBJ_MODE[CFG_PM_LIMIT];
	CX__VAR_DIGITAL_CTRL xCH__PMx__OBJ_STATUS[CFG_PM_LIMIT];
	CX__VAR_STRING_CTRL  xCH__PMx__OBJ_VIRTUAL_STATUS[CFG_PM_LIMIT];

	CX__VAR_STRING_CTRL  xCH__PMC_PROC_READY_FLAG[CFG_PM_LIMIT];

	CX__VAR_STRING_CTRL  xCH__PMx__FIRST_WAFER_FLAG[CFG_PM_LIMIT];
	CX__VAR_STRING_CTRL  xCH__PMx__IDLE_TIME[CFG_PM_LIMIT];
	CX__VAR_STRING_CTRL  xCH__PMx__PROCESS_PERCENT[CFG_PM_LIMIT];
	CX__VAR_STRING_CTRL  xCH__PMx__CURRENT_PRC_TIME[CFG_PM_LIMIT];

	CX__VAR_STRING_CTRL  xCH__PMx__X_PICK_FLAG[CFG_PM_LIMIT];
	CX__VAR_STRING_CTRL  xCH__PMx__X_PLACE_FLAG[CFG_PM_LIMIT];

	CX__VAR_DIGITAL_CTRL xCH__PMx__PRESSURE_STATUS[CFG_PM_LIMIT];
	CX__VAR_ANALOG_CTRL  xCH__PMx__PRESSURE_VALUE[CFG_PM_LIMIT];

	CX__VAR_STRING_CTRL  xCH__PMx__PRC_PRE_RECIPE[CFG_PM_LIMIT];

	CX__VAR_STRING_CTRL  xCH__PMx__POST_CLEAN_RECIPE[CFG_PM_LIMIT];
	CX__VAR_STRING_CTRL  xCH__PMx__PROCESS_RECIPE[CFG_PM_LIMIT];
	CX__VAR_STRING_CTRL  xCH__PMx__PROCESS_STATUS[CFG_PM_LIMIT];

	CX__VAR_DIGITAL_CTRL xCH__PMx__SLOT_STATUS[CFG_PM_LIMIT];
	CX__VAR_STRING_CTRL  xCH__PMx__SLOT_TITLE[CFG_PM_LIMIT];

	CX__VAR_STRING_CTRL  xCH__PMx__NEXT_PROCESS[CFG_PM_LIMIT];
	CX__VAR_DIGITAL_CTRL xCH__PMx__AUTO_MATERIAL_COUNT_FLAG[CFG_PM_LIMIT];

	CX__VAR_DIGITAL_CTRL xCH__PMx__CFG_AUTO_LEAK_CHECK_MODE[CFG_PM_LIMIT];
	CX__VAR_DIGITAL_CTRL xCH__PMx__CFG_AUTO_PM_MODE[CFG_PM_LIMIT];

	CX__VAR_STRING_CTRL  xCH__PMx__PROC_INFO__PROCESS_STATUS[CFG_PM_LIMIT];

	CX__VAR_STRING_CTRL  sEXT_CH__PMx_PRE_INFO_JOB_START_READY_PORTID[CFG_PM_LIMIT];
	CX__VAR_STRING_CTRL  sEXT_CH__PMx_PRE_INFO_JOB_START_READY_JOBID[CFG_PM_LIMIT];
	CX__VAR_STRING_CTRL  sEXT_CH__PMx_REQ_INFO_JOB_START_READY_PORTID[CFG_PM_LIMIT];
	CX__VAR_STRING_CTRL  sEXT_CH__PMx_REQ_INFO_JOB_START_READY_JOBID[CFG_PM_LIMIT];

	// OBJ : LP1 ~ LP5
	int iLPx_SIZE;

	CX__VAR_DIGITAL_CTRL xCH__LPx__OBJ_CTRL[CFG_LP_LIMIT];
	CX__VAR_DIGITAL_CTRL xCH__LPx__OBJ_MODE[CFG_LP_LIMIT];
	CX__VAR_DIGITAL_CTRL xCH__LPx__OBJ_STATUS[CFG_LP_LIMIT];

	CX__VAR_DIGITAL_CTRL xCH__LPx__CFG_USE[CFG_LP_LIMIT];
	CX__VAR_DIGITAL_CTRL xCH__LPx__CST_STATUS[CFG_LP_LIMIT];
	CX__VAR_DIGITAL_CTRL xCH__LPx__CFG_PROCESS_TYPE[CFG_LP_LIMIT];
	//


	//-------------------------------------------------------------------------	
	// ...
	CString sMODE__MAINT_PICK;
	int  Call__MAINT_PICK(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);

	// ...
	CString sMODE__MAINT_PLACE;
	int  Call__MAINT_PLACE(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);

	// ...
	CString sMODE__CLEAR;
	int  Call__CLEAR(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);

	// ...
	int  Alarm_Post__ToolClear__Place_Error(CII_OBJECT__ALARM *p_alarm,
											const CString& module_name,
											const CString& para_module,
											const CString& para_slot);
	int  Alarm_Post__ToolClear__Pick_Error(CII_OBJECT__ALARM *p_alarm,
											const CString& module_name,
											const CString& para_module,
											const CString& para_slot);

	// ...
	CString sMODE__CHECK_MATERIAL;
	int  Call__CHECK_MATERIAL(CII_OBJECT__VARIABLE *p_variable);

	CString sMODE__CHECK_STANDBY;
	int  Call__CHECK_STANDBY(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);

	// ...
	CString sMODE__WAFER_RESYNC_CLEAR_SYS;
	int  Call__WAFER_RESYNC_CLEAR_SYS();

	CString sMODE__WAFER_RESYNC_CLEAR_PMx;
	int  Call__WAFER_RESYNC_CLEAR_PMx();

	CString sMODE__WAFER_RESYNC_CLEAR_LLx;
	int  Call__WAFER_RESYNC_CLEAR_LLx();

	// ...
	CString sMODE__SCH_DB_CHECK;
	int  Call__SCH_DB_CHECK(CII_OBJECT__VARIABLE *p_variable);

	CString sMODE__USER_INFO_CHECK;
	int  Call__USER_INFO_CHECK(CII_OBJECT__VARIABLE *p_variable);
	//


	// ...
	void Mon__ALL_STATE(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);
	void Fnc__CFG_PARAMETER(CII_OBJECT__VARIABLE *p_variable);
	void Fnc__LBx_CFG(CII_OBJECT__VARIABLE *p_variable);
	void Fnc__SYSTEM_CHECK(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);
	void Fnc__PMx_LEAK_CHECK_CTRL(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);
	void Fnc__PMx_AUTO_PM_CTRL(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);

	void Mon__AUTO_CTRL(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);
	void Mon__PMx_CTRL(CII_OBJECT__VARIABLE *p_variable);

	int  Check_Alarm__Scheduler_Config(CII_OBJECT__ALARM* p_alarm);

	int  Fnc__Get_PMx_List(const CString& sch_name, CStringArray& l_pm_name);
	int  Fnc__Get_PMx_List(const CString& sch_name, CStringArray& l_pm_name,CStringArray& l_pm_rcp);
	CString Fnc__Get_PM_Rcp(const CString& pm_name, const CStringArray& l_pm_name,const CStringArray& l_pm_rcp);
	int  Fnc__Get_Find_Index(const CString& pm_name, const CStringArray& l_pm_name);

	int  AUTO_CTRL__LBi_RB__ONLY_MODE(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);
	int  _AUTO_CTRL__LBi_RB__ONLY_MODE_WITH_DUAL_TYPE(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);
	int  _AUTO_CTRL__LBi_RB__ONLY_MODE(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);

	int  AUTO_CTRL__LBi_To_RB_With_Ready_PMC__ALL_MODE(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);
	int  _GET__WAFER_LIST_OF_LLx_IN__ALL_MODE(CStringArray& l__sch_name,
											  CStringArray& l__ll_name,
											  CStringArray& l__ll_slot,
											  const bool active__log_msg = false);
	int  _CHECK__FIRST_WAFER_OF_LLx_IN__ALL_MODE(const CString& ll_name, 
												 const CString& ll_slot);

	int  _CHECK__LBi_To_RB_With_Ready_PMC__ALL_MODE(const bool active__pm_ready = false);
	int  _CHECK__LBi_To_RB_With_Ready_PMC__ALL_MODE(const CString& ll_name, const int ll_slot, const bool active__pm_ready = false);
	int  _CHECK__LBi_To_RB_With_Ready_PMC__ALL_MODE(const CString& ll_name, const CString& ll_slot, const bool active__pm_ready = false);
	int  _GET__LBi_To_RB_With_Ready_PMC__ALL_MODE(const CString& ll_name, 
												  const CString& ll_slot, 
												  int& active__lp_normal, 
												  int& count__pmc_type_ready, 
												  CStringArray& l__pmc_name,
												  const bool active__pm_ready = false);

	int  _CHECK__Ready_PMC_OF_SCH_NAME(const CString& sch_name, const bool active__pm_ready = false);
	int  _GET__Ready_PMC_OF_SCH_NAME(const CString& sch_name, 
									 int& active__lp_normal, 
									 int& count__pmc_type_ready, 
									 CStringArray& l__pmc_name,
									 const bool active__pm_ready = false);

	// ...
	int  Fnc__LBi_To_RB_With_Ready_PMC__ALL_MODE(CII_OBJECT__VARIABLE *p_variable,
												 CII_OBJECT__ALARM *p_alarm,
												 const CString& check_lbx_type,
												 const CStringArray& l_lba_pmc_name,
												 const CStringArray& l_lba_pmc_rcp,
												 const CStringArray& l_lbb_pmc_name,
												 const CStringArray& l_lbb_pmc_rcp);
	
	int  Get__First_Wafer_In_LBi(CII_OBJECT__VARIABLE *p_variable,
									CII_OBJECT__ALARM *p_alarm,
									const CString& check_lbx_type,
									int& lba_flag,
									int& lbb_flag);
	int  Check__First_Wafer_In_LBi(CII_OBJECT__VARIABLE *p_variable,
								   CII_OBJECT__ALARM *p_alarm,
								   int& llx_id);

	int  Get__Arm_List_To_Process(CStringArray& l_arm);
	int  Get__Arm_List_To_LPx(CStringArray& l_arm);
	int  _Get__Arm_List(CStringArray& l_arm, const int proc_check);

	int  Change_Order__In_Count_Of_LLx(CUIntArray& l_cnt,
									   CStringArray& l_name,
									   CStringArray& l_slot);

	// ...
	// int  AUTO_CTRL__LBi_RB__ALL_MODE(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);
	int  AUTO_CTRL__LBi_RB__1_SLOT_ALL_MODE_NEW(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);

	int  AUTO_CTRL__LBi_PMx__ONLY_MODE(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);
	int  _AUTO_CTRL__LBi_PMx__ONLY_MODE(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);
	int  AUTO_CTRL__LBi_To_Ready_PMx__ALL_MODE(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);
	// int  AUTO_CTRL__LBi_PMx__ALL_MODE(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);
	int  AUTO_CTRL__LBi_PMx__1_SLOT_ALL_MODE(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);

	int  AUTO_CTRL__RB_PMx(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);
	int  _AUTO_CTRL__RB_PMx(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm,const CString& arm_type);
	int  _AUTO_CTRL__RB_PMx_WITH_DUAL_TYPE(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);

	int  AUTO_CTRL__RB_TO_LLx_PROC(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);
	int  _AUTO_CTRL__RB_TO_LLx_PROC(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm,const CString& arm_type);

	int  AUTO_CTRL__LLx_PROC_TO_RB(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);
	int  _AUTO_CTRL__LLx_PROC_TO_RB(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);

	int  AUTO_CTRL__PMo_RB(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);
	int  _AUTO_CTRL__PMo_RB_WITH_DUAL_TYPE(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);
	int  _SCH_CHECK__PMo_RB_WITH_DUAL_TYPE(const bool active__dual_type, const bool active__pm_constaint);
	int  _AUTO_CTRL__PMo_RB__CTRL_MODE(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm, const bool active__only_mode);
	int  _SCH_CHECK__PMo_RB__ONLY_MODE();
	int  _SCH_CHECK__PMo_RB__ALL_MODE();

	int  AUTO_CTRL__PMx_RB(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);
	int  _AUTO_CTRL__PMx_RB_WITH_DUAL_TYPE(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);
	int  _AUTO_CTRL__PMx_RB__ALL_MODE(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);

	int  AUTO_CTRL__RB_LBo__ONLY_MODE(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);
	int  _AUTO_CTRL__RB_LBo__ONLY_MODE_WITH_DUAL_TYPE(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);
	int  _AUTO_CTRL__RB_LBo__ONLY_MODE(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);

	int  AUTO_CTRL__RB_LBo__ALL_MODE(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm,const int loop_count);
	int  AUTO_CTRL__RB_LBo__1_SLOT_ALL_MODE(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);

	int  Fnc__Get_LLx__Empty_Out_Slot(CII_OBJECT__VARIABLE *p_variable,
									  CII_OBJECT__ALARM *p_alarm,
									  const int ok_check,
									  const int ll_i,
									  CString& ll_name,CString& ll_slot);

	int  Fnc__PMC_Empty_Ready_Check(const CString& arm_type,
									const int ex_pick_check = -1);
	int  Fnc__PMx_Empty_Ready_Check(const int flag__lp_normal,
									const int pm_i,
									const int only_available_flag,
									const int ex_pick_check = -1);

	int  Fnc__PMC_first_Path_Check(const CString& pmc_name, const bool active__wfr_check);

	int  Get__PM_ID_OF_SCH_USER_DEF_TRG_MDx(const CString& log_id, const CString& sch_name);
	int  Get__PM_INDEX_OF_SCH_USER_DEF_GOTO_PMX(const CString& log_id, const CString& sch_name, const CStringArray& l_pm_name);

	//
	void AUTO_CTRL__PMx_ABORT(CII_OBJECT__VARIABLE *p_variable);
	void AUTO_CTRL__PMx_PUMP(CII_OBJECT__VARIABLE *p_variable);
	void AUTO_CTRL__TMC_PUMP(CII_OBJECT__VARIABLE *p_variable);
	void AUTO_CTRL__LBo_PUMP(CII_OBJECT__VARIABLE *p_variable);
	
	int  _Get__MIN_PMx_INDEX_OF_PMx_IN_COUNT(const CStringArray& l__pm_name);
	int  _Get__MIN_PMx_INDEX_OF_PMx_IN_COUNT(const CUIntArray& l__pm_index);

	void AUTO_CTRL__FRONT_WAC_POS(CII_OBJECT__VARIABLE *p_variable);
	void AUTO_CTRL__FRONT_PMo(CII_OBJECT__VARIABLE *p_variable);

	void AUTO_CTRL__LLx_Exception_In_Idle_State(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);
	void AUTO_CTRL__LLx_Exception_Of_Only_Mode(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);
	void AUTO_CTRL__LLx_Exception_Of_All_Mode(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);

	void AUTO_CTRL__Deadlock_Report(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);

	// ...
	void _Get__IDLE_PM_INDEX_LIST_WITH_WAFER(CUIntArray& l_pm_index);
	void _Get__PM_SORT_WITH_IDLE_TIME(const CUIntArray& l_pm_index_in, CUIntArray& l_pm_index_out);
	void _Get__PM_NAME_FROM_PM_ID(const CUIntArray& l_pm_id, CStringArray& l_pm_name);
	void _Get__PM_ID_FROM_PM_NAME(const CStringArray& l_pm_name, CUIntArray& l_pm_id);

	// ...
	void SCH__SYSTEM_INIT();
	void SCH__SYSTEM_CHECK(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm,const int job_flag = 1); 
	void SCH__MATERIAL_CHECK();
	void SCH__PMC_CHECK();
	void SCH__MODULE_CHECK();

	// ...
	int  SCH__SWAP_PMx(CII_OBJECT__VARIABLE *p_variable,
						CII_OBJECT__ALARM    *p_alarm,
						const CString& pick_arm,
						const CString& place_arm,
						const CString& str_module,
						const CString& str_slot,
						const CString& sch_pmc);
	int  _SCH__SWAP_PMx(CII_OBJECT__VARIABLE *p_variable,
						CII_OBJECT__ALARM    *p_alarm,
						const CString& pick_arm,
						const CString& place_arm,
						const CString& str_module,
						const CString& str_slot,
						const CString& sch_pmc);

	int  SCH__SWAP_LBx(CII_OBJECT__VARIABLE *p_variable,
						CII_OBJECT__ALARM    *p_alarm,
						const CString& para_module,
						const CString& place_arm,
						const CString& place_slot,
						const CString& pick_arm,
						const CString& pick_slot,
						const CString& sch_place_module,
						const CString& sch_pick_module);
	int  _SCH__SWAP_LBx(CII_OBJECT__VARIABLE *p_variable,
						CII_OBJECT__ALARM    *p_alarm,
						const CString& para_module,
						const CString& place_arm,
						const CString& place_slot,
						const CString& pick_arm,
						const CString& pick_slot,
						const CString& sch_place_module,
						const CString& sch_pick_module);

	int  SCH__PICK_MODULE(CII_OBJECT__VARIABLE *p_variable,
							CII_OBJECT__ALARM *p_alarm,
							const CString& log_id,
							const bool ex_flag,
							const CString& arm_type,const CString& str_module,const CString& str_slot,
							const CString& from_sch_module,
							const int maint_flag = -1);
	int  SCH__PICK_MODULE(CII_OBJECT__VARIABLE *p_variable,
							CII_OBJECT__ALARM *p_alarm,
							const CString& log_id,
							const bool ex_flag,
							const CString& arm_type,const CString& str_module,const CString& str_slot,
							const CStringArray& l_from_sch,
							const int maint_flag = -1);
	int  _SCH__PICK_MODULE(CII_OBJECT__VARIABLE *p_variable,
							CII_OBJECT__ALARM *p_alarm,
							const CString& log_id,
							const bool ex_flag,
							const CString& arm_type,const CString& str_module,const CString& str_slot,
							const CStringArray& l_from_sch,
							const int maint_flag = -1);

	int  SCH__PLACE_MODULE(CII_OBJECT__VARIABLE *p_variable,
							CII_OBJECT__ALARM *p_alarm,
							const CString& log_id,
							const bool ex_flag,
							const CString& arm_type,const CString& str_module,const CString& str_slot,
							const CString& to_sch_module,
							const int maint_flag = -1);
	int  SCH__PLACE_MODULE(CII_OBJECT__VARIABLE *p_variable,
							CII_OBJECT__ALARM *p_alarm,
							const CString& log_id,
							const bool ex_flag,
							const CString& arm_type,const CString& str_module,const CString& str_slot,
							const CString& to_sch_01,
							const CString& to_sch_02,
							const int maint_flag = -1);
	int  _SCH__PLACE_MODULE(CII_OBJECT__VARIABLE *p_variable,
							CII_OBJECT__ALARM *p_alarm,
							const CString& log_id,
							const bool ex_flag,
							const CString& arm_type,const CString& str_module,const CString& str_slot,
							const CString& to_sch_01,
							const CString& to_sch_02,
							const int maint_flag = -1);

	int  SCH__Seq_SWAP_PMx(CII_OBJECT__VARIABLE *p_variable,
							const CString& arm_type,
							const CString& para_module,
							const CString& para_slot);
	int  SCH__Seq_SWAP_LBx(CII_OBJECT__VARIABLE *p_variable,
							const CString& place_arm,
							const CString& pick_arm,
							const CString& para_module,
							const CString& place_slot,
							const CString& pick_slot);

	//
	int  SCH__PICK_FROM(const bool active__auto_mode,
						CII_OBJECT__VARIABLE *p_variable,
						CII_OBJECT__ALARM *p_alarm,
						const CString& log_id,
						const bool ex_flag,
						const CString& arm_type,
						const CString& str_module,
						const CString& str_slot);
	int  _SCH__PICK_FROM(CII_OBJECT__VARIABLE *p_variable,
						 CII_OBJECT__ALARM *p_alarm,
						 const CString& log_id,
						 const bool ex_flag,
						 const CString& arm_type,
						 const CString& str_module,
						 const CString& str_slot);

	//
	int  SCH__PLACE_TO(const bool active__auto_mode,
					   CII_OBJECT__VARIABLE *p_variable,
					   CII_OBJECT__ALARM *p_alarm,
					   const bool ex_flag,
					   const CString& arm_type,
					   const CString& str_module,
					   const CString& str_slot);
	int  _SCH__PLACE_TO(CII_OBJECT__VARIABLE *p_variable,
						CII_OBJECT__ALARM *p_alarm,
						const bool ex_flag,
						const CString& arm_type,
						const CString& str_module,
						const CString& str_slot);

	//
	int  SCH__LLx_PROC(CII_OBJECT__VARIABLE *p_variable,
					   CII_OBJECT__ALARM *p_alarm,
					   const CString& ll_name,
					   const CString& ll_sch_name,
					   const CString& ll_rcp,
					   const CString& cmmd_proc);

	// 
	int  SCH_RUN__LLx_VENT(const int ll_index, const CString& log_id, const CString& sub_id);
	int  SCH_CALL__LLx_VENT(const int ll_index, const CString& log_id);

	int  SCH_RUN__LLx_PUMP(const int ll_index, const CString& log_id, const CString& sub_id);
	int  SCH_CALL__LLx_PUMP(const int ll_index, const CString& log_id);

	//
	int  SCH__CHECK_PICK_MODULE__READY(CII_OBJECT__VARIABLE* p_variable,
										const bool ex_flag,
										const CString& para_arm,
										const CString& para_module,
										const CString& para_slot,
										const int maint_flag = 1);
	int  SCH__CHECK_PICK_MODULE__COMPLETE(CII_OBJECT__VARIABLE* p_variable,
										  CII_OBJECT__ALARM* p_alarm,
										  const bool ex_flag,
										  const CString& para_arm,
										  const CString& para_module,
										  const CString& para_slot,
										  const CString& from_sch_name,
										  const int maint_flag = 1);
	int  SCH__CHECK_PICK_MODULE__COMPLETE(CII_OBJECT__VARIABLE* p_variable,
										  CII_OBJECT__ALARM* p_alarm,
										  const bool ex_flag,
										  const CString& para_arm,
										  const CString& para_module,
										  const CString& para_slot,
										  const CStringArray& l_from_sch,
										  const int maint_flag = 1);

	int  SCH__CHECK_PLACE_MODULE__READY(CII_OBJECT__VARIABLE* p_variable,
										const bool ex_flag,
										const CString& arm_type,
										const CString& para_module,const CString& para_slot,
										const int maint_flag = 1);
	int  SCH__CHECK_PLACE_MODULE__COMPLETE(CII_OBJECT__VARIABLE* p_variable,
										const bool ex_flag,
										const CString& arm_type,
										const CString& para_module,const CString& para_slot,
										const int maint_flag = 1);

	int  _SCH__PROC_PMC_START(CII_OBJECT__VARIABLE* p_variable, 
							  const CString& arm_type, 
							  const CString& para_slot,
							  const int pm_i, 
							  const int maint_flag);

	// ...
	int  SCH__Check_In_Slot_Of_Ohter_LLx(const CString& para__ll_name, 
									     const CString& para__in_slot);

	// ...
	int  SCH__Check_Material_Place(const CString& arm_type);
	int  SCH__Check_Material_Pick(const CString& arm_type);

	int  SCH__Change_Current_Route_Step(CII_OBJECT__VARIABLE* p_variable,
										CII_OBJECT__ALARM* p_alarm,
										const CString& arm_type);

	int  SCH__Get_Job_LP_List(CUIntArray& l_job_lp);

	int  SCH__LBx__DLEAY_TIME();

	int  SCH__LBx__CHECK_1_SLOT_ALL_MODE(const int llx_id);
	int  SCH__LBx__CHECK_ONLY_MODE();
	int  SCH__LBx__CHECK_EMPTY_OF_ONLY_OUTPUT();

	int  SCH__LBi__CHECK_InSLOT_OCCUPIED();

	//
	int  SCH__LLx_CHECK_PRESSURE(const int ll_index,
								 CII_OBJECT__VARIABLE *p_variable,
								 CII_OBJECT__ALARM *p_alarm,
								 const int check_mode = -1);

	int  SCH__LLx_PUMP(const int ll_index, CII_OBJECT__VARIABLE *p_variable,const int seq_flag = -1);
	int  SCH__LBx_CAN_VENT(const CString& sch_lbx_name);

	int  SCH__LLx_PREPMATER(const int ll_index, const CString& str_slot);
	int  SCH__LLx_COMPMATER(const int ll_index, const CString& str_slot, const int main_flag);

	// ...
	int  SCH__PMx_PICK_READY(const int pm_i,const CString& str_slot);
	int  SCH__PMx_XPICK_READY(const int pm_i,const CString& str_slot);

	int  SCH__PMx_PICK_COMPLETE(const int pm_i,const CString& str_slot);
	int  SCH__PMx_XPICK_COMPLETE(const int pm_i,const CString& str_slot);

	int  SCH__PMx_PLACE_READY(const int pm_i,
								const CString& str_slot,
								const CString& lpx_port_id,
								const CString& lpx_slot_id,
								const CString& lpx_cid);
	int  SCH__PMx_XPLACE_READY(const int pm_i,
								const CString& str_slot,
								const CString& lpx_port_id,
								const CString& lpx_slot_id,
								const CString& lpx_cid);

	int  SCH__PMx_PLACE_COMPLETE(const int pm_i,const CString& str_slot);
	int  SCH__PMx_XPLACE_COMPLETE(const int pm_i,const CString& str_slot);

	// ...
	int  SCH__PMx_PRO_START(const int pm_i,
							const CString& recipe,
							const CString& lot_id,
							const CString& material_id);
	int  SCH__PMx_PRO_START_EX(const int pm_i,
							   const CString& para_slot,
							   const CString& recipe,
							   const CString& lot_id,
							   const CString& material_id);

	int  SCH__PMx_POST_CLN_START(const int pm_i,const CString& cln_recipe);
	int  SCH__PMx_CLN_START(const int pm_i);
	int  SCH__PMx_DUMMY_CLN_START(const int pm_i);

	int  SCH__PMx_NEXT_PROC_START(const int pm_i);

	int  SCH__CHECK_PM_INDEX(const CString& pm_name);

	int  SCH__PMx_CHECK_PRESSURE(const int pm_i,
									CII_OBJECT__VARIABLE *p_variable,
									CII_OBJECT__ALARM *p_alarm);
	int  SCH__PMx_PUMP(const int pm_i, CII_OBJECT__VARIABLE *p_variable);

	int  SCH__CHECK_MATERIAL_CLEAR(const bool active__alarm_check);

	// ...
	int  Seq__MAINT_PICK(CII_OBJECT__VARIABLE *p_variable,
							CII_OBJECT__ALARM *p_alarm,
							const CString& para_arm,
							const CString& para_module,
							const CString& para_slot);
	int  _Seq__MAINT_PICK(CII_OBJECT__VARIABLE *p_variable,
							CII_OBJECT__ALARM *p_alarm,
							const CString& para_arm,
							const CString& para_module,
							const CString& para_slot);

	int  Seq__MAINT_PLACE(CII_OBJECT__VARIABLE *p_variable,
							CII_OBJECT__ALARM *p_alarm,
							const CString& para_arm,
							const CString& para_module,
							const CString& para_slot,
							const int tool_clear_flag = -1);
	int  _Seq__MAINT_PLACE(CII_OBJECT__VARIABLE *p_variable,
							CII_OBJECT__ALARM *p_alarm,
							const CString& para_arm,
							const CString& para_module,
							const CString& para_slot);

	int  Seq__LBi_PMx(CII_OBJECT__VARIABLE *p_variable,
						CII_OBJECT__ALARM *p_alarm,
						const CString& arm_type,
						const CString& lbx_name,
						const CString& lbx_slot,
						const int  pm_i,
						const CString& pm_name,
						const CString& pm_slot,
						const CString& pm_rcp);

	// PMx JOB_START_READY ...
	int SCH__PMx_JOB_START_READY(const int check_pm_index, const CString& sch_name, const CString& log_id);
	int SCH__PMx_JOB_START_READY(const CString& sch_name, const CString& log_id);
	int Run__PMx_JOB_START_READY(const int pm_index, const CString& port_id,const CString& job_id);
	int Check__PMx_JOB_START_READY(const int pm_index, const CString& port_id,const CString& job_id);

	// ...
	int  Set__TMC_CtcInUse_To_Standby();
	int  Set__PMx_CtcInUse_To_Standby();

	// VAC_ROBOT
	int  VAC_RB__Is_Available();

	int  VAC_RB__Set_Standby_To_CtcInUse();
	int  VAC_RB__Set_CtcInUse_To_Standby();
	int  VAC_RB__Check_CtcInUse_Or_Standby();
	int  VAC_RB__Set_To_MaintMode();

	// TMC_CHM
	int  TMC_CHM__Is_Available();

	int  TMC_CHM__Set_Standby_To_CtcInUse();
	int  TMC_CHM__Set_CtcInUse_To_Standby();
	int  TMC_CHM__Check_CtcInUse_Or_Standby();
	int  TMC_CHM__Set_To_MaintMode();

	int  TMC_CHM__Is_ATM();
	int  TMC_CHM__Is_VAC();

	// LLx ...
	CString Get__LLx_NAME(const int ll_index);
	int Get__LLx_INDEX(const CString& ll_name);
	int Check__LLx_NAME(const CStringArray& l_llx);

	void LLx__Update_PRESSURE(const int ll_index);

	int	 LLx__Get_Size__All_Slot_Status(const int ll_index);

	int  LLx__Check_All_Empty(const int ll_index);
	int  LLx__Check_Empty__SlotStatus(const int ll_index,const int slot_id);
	
	int  LLx__Check_Empty__InSlot_With_1_Slot_All_Mode(const int ll_index);
	int  LLx__Check_Empty__InSlot(const int ll_index);

	int  LLx__Check_Occupied__InSlot_With_1_Slot_All_Mode(const int ll_index);
	int  LLx__Check_Occupied__InSlot(const int ll_index);

	int  LLx__Get_Empty__InSlot_With_1_Slot_All_Mode(const int ll_index,int& slot_id);
	int  LLx__Get_Empty__InSlot(const int ll_index,int& slot_id);

	int  LLx__Get_Occupied__InSlot_With_1_Slot_All_Mode(const int ll_index,int& slot_id);
	int  LLx__Get_Occupied__InSlot(const int ll_index,int& slot_id);
	int  LLx__Get_Occupied__InSlot(const int ll_index,const int db_index,int& slot_id);
	int  LLx__Get_Occupied__InSlot(const int ll_index,CUIntArray& l_slot_id);
	int  LLx__Get_Occupied__InSlot_Count(const int ll_index);

	bool LLx__Check_Empty__OutSlot();
	int  LLx__Check_Empty__OutSlot(const int ll_index);
	int  LLx__Check_Occupied__OutSlot(const int ll_index);
	int  LLx__Check_Empty__OutSlot(const int ll_index,const int slot_id);
	
	int  LLx__Get_Empty__OutSlot_With_1_Slot_All_Mode(const int ll_index,int& slot_id);
	int  LLx__Get_Empty__OutSlot(const int ll_index,int& slot_id);
	int  LLx__Get_Empty__OutSlot(const int ll_index,CUIntArray& l_slotid);
	int  LLx__Get_Empty__OutSlot(CUIntArray& l_ll_index);

	int  LLx__Get_Occupied__OutSlot(const int ll_index,int& slot_id);
	int  LLx__Get_Occupied__OutSlot(const int ll_index,CUIntArray& l_slotid);

	int  LLx__Is_Available(const int ll_index);
	int  LLx__Is_Idle(const int ll_index);
	int  LLx__Is_Standby_CTCInUse(const int ll_index);
	
	int  LLx__Set_Standby_To_CtcInUse(const int ll_index);
	int  LLx__Set_CtcInUse_To_Standby(const int ll_index);
	int  LLx__Check_CtcInUse_Or_Standby(const int ll_index);
	int  LLx__Set_To_MaintMode(const int ll_index);

	int  LLx__Is_ATM(const int ll_index);
	int  LLx__Is_VAC(const int ll_index);

	// PMx
	int  PMx__Is_Available(const int pm_i);
	int  PMx__Is_Idle(const int pm_i);
	int  PMx__Is_Ready(const int pm_i);

	int  PMx__Set_Standby_To_CtcInUse(const int pm_i);
	int  PMx__Set_CtcInUse_To_Standby(const int pm_i);
	int  PMx__Check_CtcInUse_Or_Standby(const int pm_i);
	int  PMx__Set_To_MaintMode(const int pm_i);

	int  PMx__Is_ATM(const int pm_i);
	int  PMx__Is_VAC(const int pm_i);
	void PMx__Update_PRESSURE(const int pm_i);

	// LPx
	int  LPx__Get_Process_Type__Normal(const CString& sch_name,int& flag__lp_normal);
	int  LPx__Check_Process_Type__Normal(const int lp_index);

	// ...
	int  Module__Check(const CString& fnc_title,CII_OBJECT__ALARM* p_alarm);

	// ...
	int  VAC_RB__Check_Dual_Arm_Enable();
	int  VAC_RB__Check_A_Arm_Enable();
	int  VAC_RB__Check_B_Arm_Enable();

	int  VAC_RB__Check_Empty__Arm_Type(const CString& arm_type);
	int  VAC_RB__Check_Empty__Arm_Type();
	int  VAC_RB__Check_Empty__A_Arm();
	int  VAC_RB__Check_Empty__B_Arm();

	int  VAC_RB__Check_Occupied__Arm_Type();
	int  VAC_RB__Check_Occupied__Arm_Type(const CString& arm_type);
	int  VAC_RB__Check_Occupied__A_Arm();
	int  VAC_RB__Check_Occupied__A_Arm(CString& title);
	int  VAC_RB__Check_Occupied__B_Arm();
	int  VAC_RB__Check_Occupied__B_Arm(CString& title);

	bool VAC_RB__Check_Empty__All_Type_Of_LL_Constraint(const CString& arm_type);
	int  VAC_RB__Get_Empty__Arm_Type_With_LL_Constraint(CString& empty_arm, const int ll_slot_id);
	int  _VAC_RB__Check_LL_Constraint(const CString& ll_type, const int ll_slot_id);

	int  VAC_RB__Get_Empty__Arm_Type_With_PMx_Odd_Even(CString& empty_arm, const CString& pm_name);

	int  VAC_RB__Get_Empty__Arm_Type_With_PMx_Constraint(CString& empty_arm, const CString& pm_name);
	int  VAC_RB__Get_Empty__Arm_Type_With_PMx_Constraint(CString& empty_arm, const CStringArray& l_pm_name);
	bool VAC_RB__Check_Empty__Arm_Type_With_PMx_Constraint(const CString& empty_arm, const CString& pm_name);
	bool VAC_RB__Check__Arm_Type_With_PMx_Constraint(const CString& arm_type, const CString& pm_name);
	bool _Check__Odd_PMx(const CStringArray& l_pm_name);
	bool _Check__Odd_PMx(const CString& pm_name);
	bool _Check__Even_PMx(const CStringArray& l_pm_name);
	bool _Check__Even_PMx(const CString& pm_name);
	bool _Check__PM_Name(const CString& pm_name, const CStringArray& l_pm_name);

	int  VAC_RB__Get_Empty__Arm_Type_From_All(CString& arm_type);
	int  VAC_RB__Get_Empty__Arm_Type_From_LBx(CString& arm_type);
	int  VAC_RB__Get__Arm_Type_From_LBx(CString& arm_type);
	int  VAC_RB__Get_Empty__Arm_Type_From_PMx(CString& arm_type);

	int  VAC_RB__Get_Occupied__Arm_Count();
	int  VAC_RB__Get_Occupied__Arm_Type(CString& arm_type);
	int  VAC_RB__Get_Occupied__Arm_Type(CStringArray& l_arm_type);
	int  VAC_RB__Check_All_Empty();
	int  VAC_RB__Check_Empty__Dual_Arm();
	int  VAC_RB__Check_Occupied__Dual_Arm();

	// ...
	int  PMx__Check_Empty__SlotStatus(const int pm_i);
	int  PMx__Check_Occupied__SlotStatus(const int pm_i);

	int  PMx__Check_Next_Process(const int pm_i,CII_OBJECT__ALARM *p_alarm);
	CString PMx__Get_Next_Process(const int pm_i);

	// ...
	void Fnc__LOG_INFO(const CString& fnc_id,
					   const CString& log_id,
					   const CString& para_info);
	void Fnc__LOG_PROC(const CString& fnc_id,
					   const CString& log_id,
					   const CString& sch_name,
					   const bool active_next);

	// ...
	void _Get__ARM_INFO(const CString& arm_type,
						const CString& stn_name,
						const CString& stn_slot,
						CStringArray& l__arm_type,
						CStringArray& l__stn_name,
						CStringArray& l__stn_slot);
	//

public:
	CObj__DUAL_ARM_STD();
	~CObj__DUAL_ARM_STD();

	//-------------------------------------------------------------------------
	int __DEFINE__CONTROL_MODE(obj,l_mode);
	int __DEFINE__VERSION_HISTORY(version);

	int __DEFINE__VARIABLE_STD(p_variable);
	int __DEFINE__ALARM(p_alarm);

	int __Define__USER_FUNCTION(CII_DEFINE__FUNCTION *p_def_fnc);

	//-------------------------------------------------------------------------
	int __INITIALIZE__OBJECT(p_variable,p_ext_obj_create);

	//-------------------------------------------------------------------------
	int __CALL__CONTROL_MODE(mode,p_debug,p_variable,p_alarm);
	int __CALL__MONITORING(id,p_variable,p_alarm);

	//-------------------------------------------------------------------------
	int __Define__QUERY(CStringArray& l_query);

	int __Call__QUERY_LIST(const CString& query_name, CStringArray& l_data);
	int __Call__QUERY_LIST(const CString& query_name,const CStringArray& l_sub_query, CStringArray& l_data);
};
