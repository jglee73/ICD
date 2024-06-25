#pragma once

#include "Interface_Code.h"

#include "CCommon_Error.h"
#include "CCommon_Define.h"
#include "CCommon_System.h"

#include "CObj__DUAL_ARM_STD__ALID.h"
#include "CObj__DUAL_ARM_STD__DEF.h"

#include "CMNG__PMx_ALIGN.h"


class CObj__DUAL_ARM_STD : public __IOBJ__STD_TYPE
{
private:
	//-------------------------------------------------------------------------
	CString sObject_Name;
	int iJOB_FLAG;

	CCommon_Error__DEF_VARIABLE mERROR__DEF_VAR;
	CCommon_Error__USER_FNC     mERROR__USER_FNC;

	SCX__CTC__SCH_MATERIAL_CTRL xSCH_MATERIAL_CTRL;
	SCX__USER_LOG_CTRL xAPP_LOG_CTRL;

	int iActive_SIM;

	// ...
	CMNG__PMx_ALIGN mMNG__PMx_ALIGN;
	//

	//-------------------------------------------------------------------------
	//  INTERNAL PROPERTY

	// ...
	CX__VAR_STRING_CTRL  sCH__SCH_LOOP_ID;
	CX__VAR_DIGITAL_CTRL dCH__CFG_QUERY_LOG_MODE;

	// PARA.MANUAL_MOVE ...
	CX__VAR_STRING_CTRL  sCH__PARA_MANUAL_MOVE_ALIGN_TARGET_PMx;
	CX__VAR_STRING_CTRL  sCH__PARA_MANUAL_MOVE_ALIGN_SKIP;

	// SCH.TEST ...
	CX__VAR_DIGITAL_CTRL dCH__SCH_TEST_ONLY_PLACE_TO_AL;

	// Config Channel ...
	CX__VAR_DIGITAL_CTRL dCH__CFG_PRE_ALIGN_USE;

	CX__VAR_ANALOG_CTRL  xCH_CFG__PHY_FRONT_TIME;
	CX__VAR_ANALOG_CTRL  xCH_CFG__PHY_PICK_TIME;
	CX__VAR_ANALOG_CTRL  xCH_CFG__PHY_PLACE_TIME;

	CX__VAR_ANALOG_CTRL  xCH_CFG__PHY_ALIGN_TIME;

	CX__VAR_ANALOG_CTRL  xCH_CFG__PHY_LLx_PUMP_TIME[CFG_LLx_LIMIT];
	CX__VAR_ANALOG_CTRL  xCH_CFG__PHY_LLx_VENT_TIME[CFG_LLx_LIMIT];

	CX__VAR_DIGITAL_CTRL xCH_CFG__ALx_SWAP_FLAG;
	CX__VAR_DIGITAL_CTRL xCH_CFG__LBx_SWAP_FLAG;

	//
	CX__VAR_DIGITAL_CTRL dCH__CFG_PLACE_FRONT_TO_LLx_DOOR;
	CX__VAR_DIGITAL_CTRL dCH__CFG_PICK_FRONT_TO_LLx_DOOR;
	CX__VAR_ANALOG_CTRL  aCH__CFG_MOVE_TO_LLx_DELAY_SEC;
	CX__VAR_DIGITAL_CTRL dCH__CFG_FRONT_LPx_ID;
	CX__VAR_ANALOG_CTRL  aCH__CFG_FRONT_LPx_SLOT;

	CX__VAR_STRING_CTRL  sCH__INFO_FUME_CLEAR_STATE_FROM_TMC;
	CX__VAR_STRING_CTRL  sCH__INFO_FUME_CLEAR_POS_FROM_TMC;
	CX__VAR_STRING_CTRL  sCH__INFO_FUME_CLEAR_DELAY_COUNT_FROM_TMC;

	//
	CX__VAR_STRING_CTRL  xCH__CHECK_MODULE;

	CX__VAR_DIGITAL_CTRL dCH__CFG_BUFFER_USE_DURING_MAINT_MODE;
	CX__VAR_DIGITAL_CTRL dCH__CFG_WAFER_TO_STx_DURING_MAINT_MODE;

	CX__VAR_DIGITAL_CTRL dCH__CFG_BUFFER_LOTID_CHECK;

	CX__VAR_DIGITAL_CTRL xCH__CFG_DB_STx_TRANSFER_MODE;
	CX__VAR_DIGITAL_CTRL xCH__SCH_DB_STx_TRANSFER_MODE;

	CX__VAR_DIGITAL_CTRL xCH__CFG_BUFFER_PRE_ALIGN_MODE;

	CX__VAR_DIGITAL_CTRL xCH__CFG_DB_STx_WAFER_PICK_MODE;
	CX__VAR_DIGITAL_CTRL xCH__SCH_DB_ST1_WAFER_PICK_MODE;
	CX__VAR_DIGITAL_CTRL xCH__SCH_DB_ST2_WAFER_PICK_MODE;
	
	CX__VAR_DIGITAL_CTRL xCH__CFG_DB_STx_APPLY_MODE;
	CX__VAR_DIGITAL_CTRL xCH__SCH_DB_STx_APPLY_MODE;
	
	CX__VAR_DIGITAL_CTRL dCH__CFG_DB_STx_OUT_MODE_BUFFER_TO_LPo;
	CX__VAR_DIGITAL_CTRL dCH__SCH_DB_ST1_OUT_MODE_BUFFER_TO_LPo;
	CX__VAR_DIGITAL_CTRL dCH__SCH_DB_ST2_OUT_MODE_BUFFER_TO_LPo;

	CX__VAR_ANALOG_CTRL  xCH__CFG_DB_BUFFER_WAFER_CLEAN_TIME;
	CX__VAR_ANALOG_CTRL  xCH__SCH_DB_ST1_WAFER_CLEAN_TIME;
	CX__VAR_ANALOG_CTRL  xCH__SCH_DB_ST2_WAFER_CLEAN_TIME;

	CX__VAR_STRING_CTRL  xCH__LPx_MAP_LOCK[CFG_LP_LIMIT];

	CX__VAR_STRING_CTRL  xCH__ATM_RB_PAUSE_FLAG;
	CX__VAR_STRING_CTRL  xCH__DB_CHECK_REQ_FLAG;

	CX__VAR_DIGITAL_CTRL xCH_CFG__SCH_LLx_PREPMATER_USE;
	CX__VAR_DIGITAL_CTRL xCH_CFG__SCH_LLx_COMPMATER_USE;

	CX__VAR_DIGITAL_CTRL xCH_CFG__SCH_ARM_MODE;
	//

	//-------------------------------------------------------------------------
	//  EXTERNAL PROPERTY

	// CFG_PMx_USE ...
	CX__VAR_DIGITAL_CTRL dEXT_CH__CFG_PMx_USE[CFG_PM_LIMIT];
	CX__VAR_DIGITAL_CTRL dEXT_CH__PMx_PROCESS_TYPE[CFG_PM_LIMIT];

	// TRANSFER
	CX__VAR_DIGITAL_CTRL xEXT_CH__CFG__TRANSFER_MODE;
	CX__VAR_ANALOG_CTRL  xEXT_CH__CFG__REF_ATM_PRESSURE;
	CX__VAR_ANALOG_CTRL  xEXT_CH__CFG__REF_VAC_PRESSURE;

	CX__VAR_STRING_CTRL  xEXT_CH__VAC_AREA_MATERIAL_COUNT;
	CX__VAR_STRING_CTRL  xEXT_CH__VAC_ROBOT_OUT_MATERIAL_COUNT;
	CX__VAR_STRING_CTRL  xEXT_CH__PMC_MATERIAL_COUNT;

	CX__VAR_STRING_CTRL  xEXT_CH__SYSTEM_LEAK_CHECK_REQ;

	// SCH PARAMETER
	CX__VAR_DIGITAL_CTRL xEXT_CH__SCH_MAP_DEVICE;
	CX__VAR_DIGITAL_CTRL xEXT_CH__SCH_ALIGN_DEVICE;
	CX__VAR_STRING_CTRL  xEXT_CH__LPx_MAP_REQ[CFG_LP_LIMIT];

	// CFG_DB : AL1 ...
	CX__VAR_DIGITAL_CTRL  xEXT_CH__CFG_DB_AL1_USE_FLAG;

	CX__VAR_DIGITAL_CTRL  xEXT_CH__CFG_DB_AL1_SLOT01_USE_FLAG;
	CX__VAR_DIGITAL_CTRL  xEXT_CH__CFG_DB_AL1_SLOT02_USE_FLAG;

	// SCH_DB : AL1 ...
	CX__VAR_DIGITAL_CTRL  xEXT_CH__SCH_DB_AL1_USE_FLAG;

	CX__VAR_DIGITAL_CTRL  xEXT_CH__SCH_DB_AL1_SLOT01_USE_FLAG;
	CX__VAR_DIGITAL_CTRL  xEXT_CH__SCH_DB_AL1_SLOT02_USE_FLAG;
	//
	
	// SCH DB ...
	// SCH.MODE.XXX
	CX__VAR_STRING_CTRL   sEXT_CH__SCH_MODE_LLx_IN;
	CX__VAR_STRING_CTRL   sEXT_CH__SCH_MODE_LLx_OUT;

	// CFG LLx ...
	CX__VAR_DIGITAL_CTRL  xEXT_CH__LLx__CFG_PREPMATER_USE[CFG_LLx_LIMIT];
	CX__VAR_DIGITAL_CTRL  xEXT_CH__LLx__CFG_COMPMATER_USE[CFG_LLx_LIMIT];

	CX__VAR_DIGITAL_CTRL  xEXT_CH__LLx__CFG_LLx_MODE_TYPE[CFG_LLx_LIMIT];

	// SCH LLx ...
	CX__VAR_DIGITAL_CTRL  xEXT_CH__SCH_DB_LLx_USE_FLAG[CFG_LLx_LIMIT];
	CX__VAR_DIGITAL_CTRL  xEXT_CH__SCH_DB_LLx_SLOT_MAX[CFG_LLx_LIMIT];
	CX__VAR_DIGITAL_CTRL  xEXT_CH__SCH_DB_LLx_IDLE_STATUS[CFG_LLx_LIMIT];

	CX__VAR_DIGITAL_CTRL  xEXT_CH__SCH_DB_LLx_MODE_TYPE[CFG_LLx_LIMIT];
	CX__VAR_DIGITAL_CTRL  dEXT_CH__CFG_DB_LLx_SLOT_STATUS[CFG_LLx_LIMIT][CFG_LLx__SLOT_MAXx];
	CX__VAR_DIGITAL_CTRL  dEXT_CH__SCH_DB_LLx_SLOT_STATUS[CFG_LLx_LIMIT][CFG_LLx__SLOT_MAXx];
	CX__VAR_DIGITAL_CTRL  xEXT_CH__SCH_DB_LLx_SLOT_MODE[CFG_LLx_LIMIT][CFG_LLx__SLOT_MAXx];

	// ...
	CX__VAR_STRING_CTRL  xCH__ATM_RB__EXCHANGE_LOCK_FLAG;
	CX__VAR_STRING_CTRL  xCH__ATM_RB__EXCHANGE_LOCK_LBx;
	CX__VAR_STRING_CTRL  xCH__ATM_RB__EXCHANGE_LOCK_SLOT;

	// LLx : Scheduler - Dual Only Input & Output ...
	CX__VAR_DIGITAL_CTRL dEXT_CH__CFG_LLx_CTRL_ONLY_INPUT_OUTPUT_MODE;

	//
	CX__VAR_DIGITAL_CTRL xEXT_CH__CFG__PRESSURE_CHECK_MODE;

	// JOB CTRL
	CX__VAR_STRING_CTRL  xEXT_CH__JOB_CTRL;
	//

	//-------------------------------------------------------------------------
	// STATUS CHANNEL

	// OBJ : ATM_ROBOT
	int Run__ROBOT_OBJ(const CString& obj_mode, const CString& log_id = "");
	int Run__ROBOT_OBJ(const CString& obj_mode, const CStringArray& l_para, const CString& log_id = "");

	int Call__ROBOT_OBJ(const CString& obj_mode, const CString& log_id = "");
	int Call__ROBOT_OBJ(const CString& obj_mode, CStringArray& l_para, const CString& log_id = "");

	//
	CII_EXT_OBJECT__CTRL *pATM_RB__OBJ_CTRL;

	CX__VAR_DIGITAL_CTRL xCH__ATM_RB__OBJ_CTRL;
	CX__VAR_DIGITAL_CTRL xCH__ATM_RB__OBJ_MODE;
	CX__VAR_DIGITAL_CTRL xCH__ATM_RB__OBJ_STATUS;

	CX__VAR_DIGITAL_CTRL xCH__ATM_RB__SLOT01_STATUS;	
	CX__VAR_STRING_CTRL  xCH__ATM_RB__SLOT01_TITLE;

	CX__VAR_DIGITAL_CTRL xCH__ATM_RB__SLOT02_STATUS;	
	CX__VAR_STRING_CTRL  xCH__ATM_RB__SLOT02_TITLE;

	CX__VAR_DIGITAL_CTRL xCH__ATM_RB__ANI_TRG_ROTE;
	CX__VAR_DIGITAL_CTRL xCH__ATM_RB__ANI_TRG_MOVE;

	//
	CX__VAR_ANALOG_CTRL  aCH__ATM_RB__CFG_ALIGN_RETRY;
	CX__VAR_STRING_CTRL  sCH__ATM_RB__CUR_ALIGN_RETRY;

	CX__VAR_DIGITAL_CTRL xCH__ATM_RB__TARGET_LLx_MODE;
	CX__VAR_DIGITAL_CTRL xCH__ATM_RB__TARGET_LLx_SLOT_CHECK;

	CX__VAR_STRING_CTRL  xCH__ATM_RB__TARGET_LLx_NAME_SET_OF_ARM_A;
	CX__VAR_STRING_CTRL  xCH__ATM_RB__TARGET_LLx_SLOT_SET_OF_ARM_A;

	CX__VAR_STRING_CTRL  xCH__ATM_RB__TARGET_LLx_NAME_SET_OF_ARM_B;
	CX__VAR_STRING_CTRL  xCH__ATM_RB__TARGET_LLx_SLOT_SET_OF_ARM_B;

	CX__VAR_STRING_CTRL  xCH__ATM_RB__TARGET_LLx_NAME_SET_ALL;
	CX__VAR_STRING_CTRL  xCH__ATM_RB__TARGET_LLx_SLOT_SET_ALL;

	CX__VAR_STRING_CTRL  xCH__ATM_RB__TARGET_LLx_ANGLE_GET;

	//
	CX__VAR_DIGITAL_CTRL xCH__ATM_RB__TARGET_PMx_MODE;

	//
	CX__VAR_STRING_CTRL  xCH__ATM_RB__SCH_STS_TO_LLx[CFG_LLx_LIMIT];

	CX__VAR_DIGITAL_CTRL xCH__ATM_RB__CFG_A_ARM_USE_MODE;
	CX__VAR_DIGITAL_CTRL xCH__ATM_RB__CFG_B_ARM_USE_MODE;

	// LLx : Scheduler - Dual Only Input & Output ...
	CX__VAR_DIGITAL_CTRL dCH__ATM_RB__CFG_DUAL_ARM_MOVING_AT_THE_SAME_TIME;

	CX__VAR_DIGITAL_CTRL dCH__ATM_RB__CFG_DUAL_ARM_MOVING_AT_LPx;
	CX__VAR_DIGITAL_CTRL dCH__ATM_RB__CFG_DUAL_ARM_MOVING_AT_STx;
	CX__VAR_DIGITAL_CTRL dCH__ATM_RB__CFG_DUAL_ARM_MOVING_AT_LLx;

	// LLx : CONTRAINT ...
	CX__VAR_DIGITAL_CTRL dCH__ATM_RB__CFG_LL_CONSTRAINT_1;
	CX__VAR_DIGITAL_CTRL dCH__ATM_RB__CFG_LL_CONSTRAINT_2;

	// CFG : WAFER PICK PARAMETER ...
	CX__VAR_DIGITAL_CTRL dCH__ATM_RB__CFG_PICK_WAFER_CONDITION;

	// INFO.LLx ...
	CX__VAR_DIGITAL_CTRL dCH__ATM_RB__INFO_LL_RESERVE_X[CFG_LLx_LIMIT];


	// OBJ : AL1 ...
	CII_EXT_OBJECT__CTRL *pAL1__OBJ_CTRL;

	CX__VAR_DIGITAL_CTRL xCH__AL1__OBJ_CTRL;
	CX__VAR_DIGITAL_CTRL xCH__AL1__OBJ_MODE;
	CX__VAR_DIGITAL_CTRL xCH__AL1__OBJ_STATUS;

	CX__VAR_DIGITAL_CTRL xCH__AL1__SLOT01_STATUS;
	CX__VAR_STRING_CTRL  xCH__AL1__SLOT01_TITLE;

	CX__VAR_DIGITAL_CTRL xCH__AL1__SLOT02_STATUS;
	CX__VAR_STRING_CTRL  xCH__AL1__SLOT02_TITLE;

	CX__VAR_STRING_CTRL  xCH__AL1__ALIGN_RETRY_FLAG;

	// LLx
	int iLLx_IN_COUNT;
	CX__VAR_ANALOG_CTRL  xCH__LLx_AVAILABLE_SLOT_COUNT[CFG_LLx_LIMIT];

	// OBJ : LLx
	int Run__LLx_OBJ(const int ll_i, const CString& obj_mode);
	int Run__LLx_OBJ(const int ll_i, const CString& obj_mode, const CStringArray& l_para);

	int Call__LLx_OBJ(const int ll_i, const CString& obj_mode);
	int Call__LLx_OBJ(const int ll_i, const CString& obj_mode, CStringArray& l_para);

	//
	int iLLx_SIZE;
	int iLLx_SLOT_MAX[CFG_LLx_LIMIT];

	CII_EXT_OBJECT__CTRL* pLLx__OBJ_CTRL[CFG_LLx_LIMIT];

	CX__VAR_STRING_CTRL  sEXT_CH__LLx__UPPER_OBJ_MSG[CFG_LLx_LIMIT];
	CX__VAR_DIGITAL_CTRL xEXT_CH__LLx__CFG_USE_FLAG[CFG_LLx_LIMIT];

	CX__VAR_DIGITAL_CTRL xEXT_CH__LLx__MOVE_FLAG[CFG_LLx_LIMIT];
	CX__VAR_DIGITAL_CTRL dEXT_CH__LLx__ACTIVE_VAC_AREA_CTRL[CFG_LLx_LIMIT];
	CX__VAR_DIGITAL_CTRL dEXT_CH__LLx__ACTIVE_ATM_AREA_CTRL[CFG_LLx_LIMIT];

	CX__VAR_STRING_CTRL  xEXT_CH__LLx__IN_OUT_FLAG[CFG_LLx_LIMIT];
	CX__VAR_STRING_CTRL  xEXT_CH__LLx__IN_COUNT[CFG_LLx_LIMIT];
	CX__VAR_STRING_CTRL  xEXT_CH__LLx__OUT_COUNT[CFG_LLx_LIMIT];
	CX__VAR_STRING_CTRL  xEXT_CH__LLx__IDLE_COUNT[CFG_LLx_LIMIT];

	CX__VAR_DIGITAL_CTRL xEXT_CH__LLx__OBJ_CTRL[CFG_LLx_LIMIT];
	CX__VAR_DIGITAL_CTRL xEXT_CH__LLx__OBJ_MODE[CFG_LLx_LIMIT];
	CX__VAR_DIGITAL_CTRL xEXT_CH__LLx__OBJ_STATUS[CFG_LLx_LIMIT];	

	CX__VAR_DIGITAL_CTRL xEXT_CH__LLx__PRESSURE_STATUS[CFG_LLx_LIMIT];
	CX__VAR_ANALOG_CTRL  xEXT_CH__LLx__PRESSURE_VALUE[CFG_LLx_LIMIT];

	CX__VAR_DIGITAL_CTRL xEXT_CH__LLx__SLOT_STATUS[CFG_LLx_LIMIT][CFG_LLx__SLOT_MAXx];
	CX__VAR_STRING_CTRL  xEXT_CH__LLx__SLOT_TITLE[CFG_LLx_LIMIT][CFG_LLx__SLOT_MAXx];

	CX__VAR_STRING_CTRL  xEXT_CH__LLx__VENTING_FLAG[CFG_LLx_LIMIT];
	CX__VAR_STRING_CTRL  xEXT_CH__LLx__VENTING_COMP_FLAG[CFG_LLx_LIMIT];

	CX__VAR_STRING_CTRL  xEXT_CH__LLx__PRE_MAINT_FLAG[CFG_LLx_LIMIT];
	//

	// OBJ : SIDE BUFFER (1)
	CX__VAR_STRING_CTRL  sCH__SCH_DB_ST1_USE_FLAG;

	CX__VAR_DIGITAL_CTRL dEXT_CH__BUFFER1__CFG_USE_FLAG;
	CX__VAR_ANALOG_CTRL  xEXT_CH__BUFFER1__CFG_SLOT_MAX;
	CX__VAR_STRING_CTRL  xEXT_CH__BUFFER1__CFG_SLOT_WAIT_SEC;
	CX__VAR_ANALOG_CTRL  xEXT_CH__BUFFER1__APP_NEXT_SLOT_TO_PLACE;
	CX__VAR_ANALOG_CTRL  xEXT_CH__BUFFER1__APP_NEXT_SLOT_TO_PICK;
	CX__VAR_STRING_CTRL  xEXT_CH__BUFFER1__APP_CUR_LOTID;

	CX__VAR_DIGITAL_CTRL xEXT_CH__BUFFER1__SLOT_STATUS[CFG_LP__SLOT_MAX];
	CX__VAR_STRING_CTRL  xEXT_CH__BUFFER1__SLOT_TITLE[CFG_LP__SLOT_MAX];
	CX__VAR_ANALOG_CTRL  xEXT_CH__BUFFER1__SLOT_WAIT_SEC[CFG_LP__SLOT_MAX];

	// OBJ : SIDE BUFFER (2)
	CX__VAR_STRING_CTRL  sCH__SCH_DB_ST2_USE_FLAG;

	CX__VAR_DIGITAL_CTRL dEXT_CH__BUFFER2__CFG_USE_FLAG;
	CX__VAR_ANALOG_CTRL  xEXT_CH__BUFFER2__CFG_SLOT_MAX;
	CX__VAR_STRING_CTRL  xEXT_CH__BUFFER2__CFG_SLOT_WAIT_SEC;
	CX__VAR_ANALOG_CTRL  xEXT_CH__BUFFER2__APP_NEXT_SLOT_TO_PLACE;
	CX__VAR_ANALOG_CTRL  xEXT_CH__BUFFER2__APP_NEXT_SLOT_TO_PICK;
	CX__VAR_STRING_CTRL  xEXT_CH__BUFFER2__APP_CUR_LOTID;

	CX__VAR_DIGITAL_CTRL xEXT_CH__BUFFER2__SLOT_STATUS[CFG_LP__SLOT_MAX];
	CX__VAR_STRING_CTRL  xEXT_CH__BUFFER2__SLOT_TITLE[CFG_LP__SLOT_MAX];
	CX__VAR_ANALOG_CTRL  xEXT_CH__BUFFER2__SLOT_WAIT_SEC[CFG_LP__SLOT_MAX];

	// OBJ : LP1 ~ LP5
	int iLPx_SIZE;

	CX__VAR_DIGITAL_CTRL xCH__LPx__OBJ_CTRL[CFG_LP_LIMIT];
	CX__VAR_DIGITAL_CTRL xCH__LPx__OBJ_MODE[CFG_LP_LIMIT];
	CX__VAR_DIGITAL_CTRL xCH__LPx__OBJ_STATUS[CFG_LP_LIMIT];

	CX__VAR_DIGITAL_CTRL xCH__LPx__CFG_USE_TYPE[CFG_LP_LIMIT];
	CX__VAR_DIGITAL_CTRL xCH__LPx__CFG_PROCESS_TYPE[CFG_LP_LIMIT];

	CX__VAR_STRING_CTRL  xCH__LPx__PORT_STATUS[CFG_LP_LIMIT];
	CX__VAR_DIGITAL_CTRL xCH__LPx__CST_STATUS[CFG_LP_LIMIT];
	CX__VAR_ANALOG_CTRL  xCH__LPx__CFG_SLOT_MAX[CFG_LP_LIMIT];
	CX__VAR_ANALOG_CTRL  xCH__LPx__LAST_PICK_SLOT[CFG_LP_LIMIT];
	CX__VAR_DIGITAL_CTRL xCH__LPx__SLOT_STATUS[CFG_LP_LIMIT][CFG_LP__SLOT_MAX];

	CX__VAR_STRING_CTRL  xCH__LPx__PICK_COUNT[CFG_LP_LIMIT];
	CX__VAR_STRING_CTRL  xCH__LPx__RETURN_REQ_FLAG[CFG_LP_LIMIT];
	CX__VAR_STRING_CTRL  xCH__LPx__SIDE_BUFFER_USE_FLAG[CFG_LP_LIMIT];

	// OBJ : PMx ...
	int iPMx_SIZE;

	CX__VAR_STRING_CTRL  sCH__PMx__OBJ_VIRTUAL_STATUS[CFG_PM_LIMIT];
	CX__VAR_STRING_CTRL  sCH__PMx__PROC_INFO_PROCESS_STATUS[CFG_PM_LIMIT];

	int  PMx__Is_Ready(const int pm_i);
	//

	//-------------------------------------------------------------------------
	// ...
	CString sMODE__MAINT_PICK;
	int  Call__MAINT_PICK(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);

	CString sMODE__MAINT_PLACE;
	int  Call__MAINT_PLACE(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);

	// ...
	CString sMODE__CLEAR;
	int  Call__CLEAR(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);

	int  Alarm_Post__ToolClear__Wafer_Title(CII_OBJECT__ALARM *p_alarm,
											const CString& module_name,
											const CString& wfr_title);
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

	// ...
	CString sMODE__SCH_DB_CHECK;
	int  Call__SCH_DB_CHECK(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);
	//


	// ...
	void Mon__LBx_CFG(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);
	void Mon__AUTO_CTRL(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);

	void Mon__DB_CHECK(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);

	//
	int  CHECK__ROBOT_MAP(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);

	int  AUTO_CTRL__LPi_RB(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);
	int  _Update__JOB_CTRL__GoTo_JOB_PORT();

	int  AUTO_CTRL__RB_LBi__ONLY_MODE(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);
	int  AUTO_CTRL__RB_LBi__ALL_MODE(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);
	int  _Fnc__RB_LBi__ALL_MODE(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm, const CString& arm_type);

	int  AUTO_CTRL__AL_LBi__ONLY_MODE(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);
	int  AUTO_CTRL__AL_LBi__ALL_MODE(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);
	int  AUTO_CTRL__Retry_ALIGN(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm, const bool active__ll_all_mode);

	int  AUTO_CTRL__RB_LPo(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);

	int  _Check__ARM_LIST_TO_PLACE_LPx();
	int  _Get__ARM_LIST_TO_PLACE_LPx(CStringArray& l__arm_type);
	int  _Get__ARM_LIST_TO_PLACE_LPx(bool& active__dual_arm, 
									 CStringArray& l__arm_type);

	int  _Get__ARM_LIST_TO_PLACE_STx(bool& active__dual_arm, 
									 CStringArray& l__arm_type,
									 CStringArray& l__arm_lotid,
									 CStringArray& l__stx_name, 
									 CUIntArray& l__stx_slot);
	int  _Get__ARM_LIST_TO_PICK_STx(CStringArray& l__stx_name, 
									CUIntArray& l__stx_slot);

	int  _Check__SLOT_COUNT_TO_PICK_STx(const double ref_sec, const int max_check);
	int  _Check__SAME_PTN_IN_STx(const CString& stx_name,const int stx_id, const int port_id);

	// ...
	int  AUTO_CTRL__LBo_RB(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);
	int  AUTO_CTRL__RB_BUFFERx(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);
	int  AUTO_CTRL__BUFFERx_RB(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);

	void AUTO_CTRL__LBi_VENT(CII_OBJECT__VARIABLE *p_variable);
	void AUTO_CTRL__LBo_PUMP(CII_OBJECT__VARIABLE *p_variable);
	
	// ...
	void Start__LBi_VENT(CII_OBJECT__VARIABLE *p_variable);
	void Start__ALL_LBx_VENT(CII_OBJECT__VARIABLE *p_variable);
	void Start__ALL_LBx_PUMP(CII_OBJECT__VARIABLE *p_variable);
	void Start__ALL_LBx_To_IDLE_STATE(CII_OBJECT__VARIABLE *p_variable);

	void AUTO_CTRL__LBx_PUMP__ALL_MODE(CII_OBJECT__VARIABLE *p_variable);
	void AUTO_CTRL__LBi_PUMP__ONLY_MODE(CII_OBJECT__VARIABLE *p_variable);
	int  Seq__LBx_VENT__ALL_MODE(CII_OBJECT__VARIABLE *p_variable);
	int  Seq__LBx_VENT__INTO_LBi(CII_OBJECT__VARIABLE *p_variable);

	int  iLBx_FRONT_FLAG;
	void AUTO_CTRL__FRONT_LBi(CII_OBJECT__VARIABLE *p_variable);
	void AUTO_CTRL__FRONT_LBo(CII_OBJECT__VARIABLE *p_variable);

	void AUTO_CTRL__Robot_Exception_Proc(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);
	void AUTO_CTRL__Robot_Abort_Exception(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);
	int  Check__Robot_Abort_Exception();
	void AUTO_CTRL__Venting_Exception_Proc(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);

	int  Fnc__RB_TO_AL(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);
	int  Fnc__AUTO_CTRL__ERROR();

	// ...
	int  Seq__MAINT_PICK(CII_OBJECT__VARIABLE *p_variable,
							CII_OBJECT__ALARM *p_alarm,
							const CString& para_arm,
							const CString& para_module,
							const CString& para_slot);
	int  Seq__MAINT_PLACE(CII_OBJECT__VARIABLE *p_variable,
							CII_OBJECT__ALARM *p_alarm,
							const CString& para_arm,
							const CString& para_module,
							const CString& para_slot);

	int  Update__MAINT_TARGET_ALIGN(CII_OBJECT__ALARM *p_alarm,
									const CString& para_module,
									const CString& para_slot);

	// ...
	void SCH__SYSTEM_INIT(CII_OBJECT__VARIABLE *p_variable);
	void SCH__SYSTEM_CHECK(CII_OBJECT__VARIABLE *p_variable,
							CII_OBJECT__ALARM *p_alarm,
							const int job_flag = 1);

	void SCH__MATERIAL_CHECK();
	void SCH__MODULE_CHECK();

	// ...
	int  SCH__FRONT_MODULE(CII_OBJECT__VARIABLE *p_variable,
							const CString& str_module,const CString& str_slot);

	int  SCH__PICK_MODULE(CII_OBJECT__VARIABLE *p_variable,
							CII_OBJECT__ALARM *p_alarm,
							const CString& log_id,
							const bool ex_flag,
							const CString& arm_type,const CString& str_module,const CString& str_slot,
							const CString& from_sch_module,
							const int maint_flag = -1);

	int  SCH__PLACE_MODULE(CII_OBJECT__VARIABLE *p_variable,
							CII_OBJECT__ALARM *p_alarm,
							const CString& log_id,
							const bool ex_flag,
							const CString& arm_type,const CString& str_module,const CString& str_slot,
							const CString& to_sch_module,
							const int maint_flag = -1);
	int  _SCH__PLACE_MODULE(CII_OBJECT__VARIABLE *p_variable,
							CII_OBJECT__ALARM *p_alarm,
							const CString& log_id,
							const bool ex_flag,
							const CString& arm_type,const CString& str_module,const CString& str_slot,
							const CString& to_sch_module,
							const int maint_flag = -1);

	//
	int  SCH__PICK_FROM(const bool active__auto_mode,
						CII_OBJECT__VARIABLE *p_variable,
						CII_OBJECT__ALARM *p_alarm,
						const bool ex_flag,
						const CString& arm_type,
						const CString& str_module,
						const CString& str_slot);
	int  _SCH__PICK_FROM(CII_OBJECT__VARIABLE *p_variable,
						 CII_OBJECT__ALARM *p_alarm,
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
	int  SCH__Get_LPx_INDEX(const CString& str_module);

	//
	int  SCH__CHECK_PICK_MODULE__READY(CII_OBJECT__VARIABLE* p_variable,
										const bool ex_flag,
										const CString& para_module,const CString& para_slot,
										const int maint_flag = 1);
	int  SCH__CHECK_PICK_MODULE__COMPLETE(CII_OBJECT__VARIABLE* p_variable,
										const bool ex_flag,
										const CString& para_module,const CString& para_slot,
										const int maint_flag = 1);

	int  SCH__CHECK_PLACE_MODULE__READY(CII_OBJECT__VARIABLE* p_variable,
										const bool ex_flag,
										const CString& para_module,const CString& para_slot,
										const int maint_flag = 1);
	int  SCH__CHECK_PLACE_MODULE__COMPLETE(CII_OBJECT__VARIABLE* p_variable,
										const bool ex_flag,
										const CString& arm_type,
										const CString& para_module,const CString& para_slot,
										const int maint_flag = 1);

	// ...
	int  SEQ__MAP_BY_ATM_RB(CII_OBJECT__VARIABLE *p_variable,const CString& str_module);
	int  SEQ__ATM_RB__HOME(const int run_call);

	// ...
	int  SCH__LPx_TO_LLx_OF_ROUTE_TYPE(CII_OBJECT__VARIABLE *p_variable,
									   const int port_id,
									   const int slot_id);

	int  SCH__LBx_PUMP(CII_OBJECT__VARIABLE *p_variable,const CString& lbx_name, const CString& log_id);
	int  SCH__CHECK_LBo_OCCUPIED();
	int  SCH__CHECK_LBi_EMPTY();
	
	int  SCH__CAN_PLACE_LBi(const int vent_check = -1);
	
	int  SCH__CAN_PICK_LBo();
	int  SCH__LBx__CHECK_ONLY_INPUNT();
	int  SCH__LBx__CHECK_ONLY_OUTPUT();

	int  CFG__LBx__CHECK_ONLY_INPUNT();

	CString SCH__Get_LBx_To_PLACE(const CString& sch_name, CString& ll_slot, const bool active__maint_mode = false);
	CString SCH__Get_LBx_To_PLACE(const CString& sch_name, const int ll_slot_count, CString& ll_slot, const bool active__maint_mode = false);

	CString SCH__Get_PMx_To_PICK_ALIGN(const CString& para_module, const CString& para_slot);

	//
	int  SCH__CHECK_LBi_ATM_PRESSURE(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM* p_alarm);
	int  SCH__CHECK_DUMMY_PORT(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM* p_alarm);

	//
	int  SCH__LLx_PREPMATER(const int ll_index,const CString& str_slot);
	int  SCH__LLx_COMPMATER(const int ll_index,const CString& str_slot);

	int  SCH_CHECK__LLx_PUMP(const int ll_index);
	int  SCH_RUN__LLx_PUMP(const int ll_index,
						   CII_OBJECT__VARIABLE *p_variable, const CString& log_id, const CString& sub_id);

	int  SCH_RUN__LLx_VENT(const int  ll_index,
						   CII_OBJECT__VARIABLE *p_variable, const CString& log_id, const CString& sub_id);
	int  SCH_CALL__LLx_VENT(const int ll_index, 
						    CII_OBJECT__VARIABLE *p_variable, const CString& log_id, const CString& sub_id);

	int  SCH__LLx_PRESSURE_ERROR(const int ll_index, 
								 CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);

	int  SCH__ALIGN(CII_OBJECT__VARIABLE *p_variable,const CString& str_slot);

	// ...
	int  Fnc__PICK_LPi_WITH_ARM(CII_OBJECT__VARIABLE *p_variable,
							    CII_OBJECT__ALARM *p_alarm,
							    const CString& robot_arm,
								const int port_id,
								const int slot_id,
								const int ll_id);

	int  Fnc__PICK_LPi_In_BUFFER_MODE(CII_OBJECT__VARIABLE *p_variable,
										CII_OBJECT__ALARM *p_alarm,
										const int port_id,
										const int slot_id,
										const int ll_id,
										CString& arm_type);

	//
	int  Fnc__Check_ARM_WAFER_INFO_MISMATCH(CII_OBJECT__VARIABLE *p_variable,
											CII_OBJECT__ALARM *p_alarm,
											const CString& arm_type);

	//
	int  Fnc__PLACE_AL(CII_OBJECT__VARIABLE *p_variable,
					   CII_OBJECT__ALARM *p_alarm,
					   const CString& log_id,
					   const CString& arm_type,
					   const int al_start_flag = 1,
					   const int ll_slot_count = 0);
	int  Fnc__PLACE_BUFFERx(CII_OBJECT__VARIABLE *p_variable,
							CII_OBJECT__ALARM *p_alarm,
							const CString& arm_type,
							const CString& bff_name,
							const int bff_slot,
							const CString& bff_lotid);

	//
	int  Fnc__PICK_LBx_EXCEPTION(CII_OBJECT__VARIABLE *p_variable,
									CII_OBJECT__ALARM *p_alarm,
									const CString& para_module,
									const CString& para_slot);

	//
	int  Fnc__PICK_AL(CII_OBJECT__VARIABLE *p_variable,
					  CII_OBJECT__ALARM *p_alarm,
					  CString& arm_type,
					  const bool active__arm_use = false);

	int  Fnc__PICK_AL_TO_RB(CII_OBJECT__VARIABLE *p_variable,
						    CII_OBJECT__ALARM *p_alarm);

	int  Fnc__ALIGN_RETRY_TO_PLACE_LLx(CII_OBJECT__VARIABLE *p_variable,
									   CII_OBJECT__ALARM *p_alarm,
									   const bool active__ll_all_mode, 
									   const CString& arm_type, 
									   const int ll_slot_count);

	//
	int  Fnc__SWAP_ALx(CII_OBJECT__VARIABLE *p_variable,
						const CString& para_module,
						const CString& place_arm,
						const CString& place_slot,
						const CString& pick_arm,
						const CString& pick_slot,
						const CString& sch_place_module,
						const CString& sch_pick_module);
	int  Fnc__Seq_SWAP_ALx(CII_OBJECT__VARIABLE *p_variable,
						const CString& place_arm,
						const CString& pick_arm,
						const CString& para_module,
						const CString& place_slot,
						const CString& pick_slot);

	//
	int  Fnc__PUMP_LBi(CII_OBJECT__VARIABLE *p_variable,const CString& para_module);

	//
	int  Fnc__PLACE_LBi__ONLY_MODE(CII_OBJECT__VARIABLE *p_variable,
								   CII_OBJECT__ALARM *p_alarm,
								   const bool ex_flag,
								   const CString& arm_type);
	
	int  Fnc__PLACE_LBi__ALL_MODE(CII_OBJECT__VARIABLE *p_variable,
								  CII_OBJECT__ALARM *p_alarm,
								  const bool ex_flag,
								  const CString& arm_type,
								  const CString& para_module,
								  const CString& para_slot);

	//
	int  Fnc__PICK_LBx(CII_OBJECT__VARIABLE *p_variable,
						CII_OBJECT__ALARM *p_alarm,
						const bool ex_flag,
						const CString& arm_type,
						const CString& para_module,
						const CString& para_slot);

	int  Fnc__SWAP_LBx(CII_OBJECT__VARIABLE *p_variable,
						CII_OBJECT__ALARM    *p_alarm,
						const CString& para_module,
						const CString& place_arm,
						const CString& place_slot,
						const CString& pick_arm,
						const CString& pick_slot,
						const CString& sch_place_module,
						const CString& sch_pick_module);
	int  Fnc__Seq_SWAP_LBx(CII_OBJECT__VARIABLE *p_variable,
						const CString& place_arm,
						const CString& pick_arm,
						const CString& para_module,
						const CString& place_slot,
						const CString& pick_slot);

	//
	int  Fnc__PLACE_LPx(CII_OBJECT__VARIABLE *p_variable,
						CII_OBJECT__ALARM *p_alarm,
						const CString& arm_type);

	// 
	int  Fnc__LBo_To_RB__ONLY_MODE(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);
	int  _Check__LBo_To_RB__ONLY_MODE(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);
	int  Fnc__LBo_To_RB__ALL_MODE(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);

	int  Fnc__LBo_To_RB(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);

	//
	int  Fnc__GET_DUMMY_PORT();

	int  Fnc__LPx_SideBuffer_Use_Flag_Check(const int port_id,const int slot_id);
	int  Fnc__LPx_SideBuffer_Use_Flag_Clear();

	//
	int  Get__LPx_SideBuffer_Info_To_Use(const int port_id,
											const int slot_id,
											CString& buffer_name,
											int& buffer_slot);
	int  Get__LPx_SideBuffer_Info_To_Use(const int port_id,const int slot_id);

	int  Get__LPx_SideBuffer_Info_To_Use(const CString& pos_name,
											CString& buffer_name,
											int& buffer_slot);
	int  Get__LPx_SideBuffer_Info_To_Use(const CString& pos_name);

	int  _Get__LPx_SideBuffer_Info_To_Use(const CString& sch_lotid,
											CString& buffer_name,
											int& buffer_slot);
	CString _Get__STx_LotID();

	// ...
	int  MANUAL_MOVE__Check_System_Module_Error(const int pick_flag,
											const CString& para_arm,
											const CString& para_module,
											const CString& para_slot,
											CII_OBJECT__ALARM *p_alarm);

	int  TOOL_CLEAR__Check_System_Module_Error(CII_OBJECT__ALARM *p_alarm);

	// ...
	int  Set__TMC_CtcInUse_To_Standby();

	// ATM_ROBOT
	int  ATM_RB__Is_Available();

	int  ATM_RB__Set_Standby_To_CtcInUse();
	int  ATM_RB__Set_CtcInUse_To_Standby();
	int  ATM_RB__Check_CtcInUse_Or_Standby();
	int  ATM_RB__Set_To_MaintMode();

	// AL1
	int  AL1__Is_Available();

	int  AL1__Set_Standby_To_CtcInUse();
	int  AL1__Set_CtcInUse_To_Standby();
	int  AL1__Check_CtcInUse_Or_Standby();
	int  AL1__Set_To_MaintMode();

	/*
	// LBA
	int  LBA__Set_Standby_To_CtcInUse();
	int  LBA__Set_CtcInUse_To_Standby();
	int  LBA__Check_CtcInUse_Or_Standby();
	int  LBA__Set_To_MaintMode();

	// LBB
	int  LBB__Set_Standby_To_CtcInUse();
	int  LBB__Set_CtcInUse_To_Standby();
	int  LBB__Check_CtcInUse_Or_Standby();
	int  LBB__Set_To_MaintMode();
	*/

	// LP1 ~ LP5
	int  LPx__Is_Available(const int lp_i);

	int  LPx__Set_Standby_To_CtcInUse(const int lp_i);
	int  LPx__Set_CtcInUse_To_Standby(const int lp_i);
	int  LPx__Check_CtcInUse_Or_Standby(const int lp_i);

	int  LPx__Check_Occupied__Slot_Status(const int lp_id,const int slot_id);
	int  LPx__Check_Empty__Slot_Status(const int lp_id,const int slot_id);

	int  LPx__Check_Not_Processed_Wafer(const int lp_id);

	// ...
	int  AL1__Check_Empty__All_Slot();

	int  AL1__Get_Empty__SlotID(int& slot_id);
	int  AL1__Get_Occupied__SlotID(int& slot_id);
	int  AL1__Get_Occupied__Total_Count();

	int  AL1__Check_Occupied__SlotStatus(const int slot_id);
	int  AL1__Check_Occupied__SlotStatus(const int slot_id,CString& title);

	// ...
	int  ATM_RB__Get_Cfg_Arm_Count();
	int  ATM_RB__Get_Empty_Arm_Count();
	int  ATM_RB__Get_Occupied_Arm_Count();

	int  ATM_RB__Check_Empty__Arm_Type();
	int  ATM_RB__Check_Empty__Arm_Type(const CString& arm_type);
	int  ATM_RB__Get_Empty__Arm_Type(CString& arm_type);

	int  ATM_RB__Check_Process_Wafer_On_Arm();
	int  ATM_RB__Get_Process_Wafer_Count_On_Arm();

	int  ATM_RB__Check_Occupied__Arm_Type();
	int  ATM_RB__Check_Occupied__Arm_Type(const CString& arm_type);
	int  ATM_RB__Check_Occupied__A_Arm();
	int  ATM_RB__Check_Occupied__B_Arm();
	int  ATM_RB__Get_Occupied__Arm_Type(CStringArray& l_arm_type);
	int  ATM_RB__Get_Occupied__Arm_Type(CString& arm_type);
	int  ATM_RB__Get_Occupied__Arm_Type(const int arm_index, CString& arm_type);
	int  ATM_RB__Get_Occupied__Wfr_Title_Of_Arm_Type(const CString& arm_type, CString& wfr_title);
	int  ATM_RB__Check_Occupied__Wfr_Title_Of_Arm_Type(CString& wfr_title);

	// LLx ...
	CString Get__LLx_NAME(const int ll_index);
	int Get__LLx_INDEX(const CString& ll_name);

	int  LLx__Is_ATM(const int ll_index);
	int  LLx__Is_VAC(const int ll_index);

	int  LLx__Is_Available(const int ll_index);

	int  LLx__Set_Standby_To_CtcInUse(const int ll_index);
	int  LLx__Set_CtcInUse_To_Standby(const int ll_index);
	int  LLx__Check_CtcInUse_Or_Standby(const int ll_index);
	int  LLx__Set_To_MaintMode(const int ll_index);

	void LLx__Update_PRESSURE(const int ll_index);
	int  LLx__Check_Venting(const int ll_index);
	
	int  LLx__Check__Slot_Enable(const int ll_index,const int slot_id);
	int  LLx__Check_All_Empty(const int ll_index);
	
	int  LLx__Check_Empty__SlotStatus(const int ll_index,const int slot_id);
	int  LLx__Get_Empty__Total_InSlot(const int ll_index);
	int  LLx__Get_Empty__Total_InSlot();

	int  LLx__Get_Occupied__Slot(const int ll_index, int& slot_id, CString& wfr_title,CString& wfr_state);
	int  LLx__Get_Occupied__TotalSlot(const int ll_index);
	int  LLx__Get_Occupied__Slot_List(const int ll_index,CUIntArray& l_slot_id);

	int  LLx__Get_Occupied__Total_OutSlot(const int ll_index);
	int  LLx__Get_Occupied__Total_InSlot(const int ll_index,const int prc_check = -1);
	int  LLx__Get_Occupied__Total_InSlot();

	int  LLx__Check_Occupied__OutSlot(const int ll_index,const int slot_index);
	int  LLx__Get_Occupied__OutSlot(const int ll_index,int& slot_id);
	int  LLx__Get_Occupied__Only_Output(const int ll_index,CUIntArray& l_slot_id);

	int  LLx__Get_Empty__InSlot(const int ll_index,int& slot_id);
	bool LLx__Check_Empty__InSlot_Of_Odd_Type();
	int  LLx__Get_Empty__InSlot_Of_Odd_Type(const int ll_index, int& slot_id);
	bool LLx__Check_Empty__InSlot_Of_Even_Type();
	int  LLx__Get_Empty__InSlot_Of_Even_Type(const int ll_index, int& slot_id);
	int  _LLx__Get_Empty__InSlot(const int ll_index, const int db_start,const int db_offset, int& slot_id);

	int  LLx__Get_Occupied__InSlot(const int ll_index, int& slot_id);
	int  LLx__Get_Occupied__InSlot(const int ll_index, CUIntArray& l_slot_id);
	int  LLx__Check_All_Occupied__InSlot_Of_Odd_Type(const int ll_index);
	int  LLx__Check_All_Occupied__InSlot_Of_Even_Type(const int ll_index);
	int  _LLx__Check_All_Occupied__InSlot(const int ll_index, const int db_start,const int db_offset, int& slot_id);

	int  LLx__Check_Empty__InSlot(const int ll_index);
	int  LLx__Check_Empty__InSlot(const int ll_index, CUIntArray& l_ll_slot);

	int  LLx__Check_Occupied__InSlot(const int ll_index);
	int  LLx__Check_Occupied__OutSlot(const int ll_index);
	//

	// ...
	int  Buffer1__Get_Occupied__Slot(int& slot_id,CString& title);
	int  Buffer1__Get_Occupied__Slot_To_Process(int& slot_id);	
	int  Buffer1__Get_Occupied__Slot_To_Process(CUIntArray& l__stx_slot_id);	
	
	int  Buffer1__Get_Empty__Slot_To_Process(int& slot_id);
	int  Buffer1__Get_Empty__Slot_To_Process(CUIntArray& l__slot_id, const bool active_slot_to_place = false);	

	int  Buffer1__Check_Empty_Slot(int& slot_id);
	int  Buffer1__Check_Empty__All_Slot();
	int  Buffer1__Check_Empty__Any_Slot();

	int  Buffer1__Check_Wait_Sec(const int slot_id, const double ref_sec = 0);
	CString Buffer1__Get_LotID();

	// ...
	int  Buffer2__Get_Occupied__Slot(int& slot_id,CString& title);
	int  Buffer2__Get_Occupied__Slot_To_Process(int& slot_id);
	int  Buffer2__Get_Occupied__Slot_To_Process(CUIntArray& l__stx_slot_id);	

	int  Buffer2__Get_Empty__Slot_To_Process(int& slot_id);
	int  Buffer2__Get_Empty__Slot_To_Process(CUIntArray& l__slot_id, const bool active_slot_to_place = false);

	int  Buffer2__Check_Empty_Slot(int& slot_id);
	int  Buffer2__Check_Empty__All_Slot();
	int  Buffer2__Check_Empty__Any_Slot();

	int  Buffer2__Check_Wait_Sec(const int slot_id, const double ref_sec = 0);
	CString Buffer2__Get_LotID();

	// ...
	void Fnc__LOG_INFO(const CString& fnc_id, const CString& log_id, const CString& para_info);

	// ...
	int _QUERY__GET_EMPTY_SLOT_OF_BUUFER(const int log_enable, CStringArray& l_info);
	int _QUERY__CHECK_BUSY_SLOT_OF_BUUFER(const int log_enable, const CStringArray& l_para);

	int _Get__PM_INDEX(const CString& pm_name);
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

	int __Call__QUERY(const CString& query_name,CString& query_data);
	int __Call__QUERY_LIST(const CString& query_name,CStringArray& l_data);
	int __Call__QUERY_LIST(const CString& query_name,const CStringArray& l_sub_query, CStringArray& l_data);
};
