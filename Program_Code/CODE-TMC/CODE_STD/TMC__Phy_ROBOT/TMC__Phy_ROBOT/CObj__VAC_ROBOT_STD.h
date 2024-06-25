#pragma once

#include "Interface_Code.h"

#include "CCommon_Error.h"
#include "CCommon_System.h"


class CObj__VAC_ROBOT_STD : public __IOBJ__STD_TYPE
{
private:
	CCommon_Error__USER_FNC mERROR__USER_FNC;

	//-------------------------------------------------------------------------
	CString	sObject_Name;
	
	SCX__USER_LOG_CTRL xAPP_LOG_CTRL;
	int  iFlag__APP_LOG;

	int m_nPM_LIMIT;
	CString sDir_Root;

	int iActive_SIM;
	//

	//-------------------------------------------------------------------------
	//  INTERNAL PROPERTY

	// OBJ ...
	CX__VAR_STRING_CTRL  sCH__OBJ_MESSAGE;

	// MON.ACTIVE ...
	CX__VAR_DIGITAL_CTRL dCH__MON_ACTIVE_ROBOT_ACTION_BUSY;

	CX__VAR_DIGITAL_CTRL dCH__MON_ACTIVE_ROBOT_ACTION_TO_LLx[CFG_LLx__SIZE];
	CX__VAR_DIGITAL_CTRL dCH__MON_ACTIVE_ROBOT_ACTION_TO_PMx[CFG_PMx__SIZE];

	// PARA CHANNEL ...
	CX__VAR_DIGITAL_CTRL dCH__OTR_IN_PARA__STN_NAME;
	CX__VAR_DIGITAL_CTRL dCH__OTR_IN_PARA__STN_SLOT;
	CX__VAR_DIGITAL_CTRL dCH__OTR_IN_PARA__ARM_TYPE;

	// PARA.ACTIVE_HANDOFF ...
	CX__VAR_DIGITAL_CTRL dCH__PARA_ACTIVE_HANDOFF_REQ;
	CX__VAR_DIGITAL_CTRL dCH__PARA_ACTIVE_HANDOFF_ACT;

	// CFG ...
	CX__VAR_DIGITAL_CTRL dCH__CFG_ACTION_CHECK;

	CX__VAR_DIGITAL_CTRL dCH__CFG_ARM_A_USE;
	CX__VAR_DIGITAL_CTRL dCH__CFG_ARM_B_USE;

	// MATERIAL CHANNEL
	CX__VAR_DIGITAL_CTRL dCH__OTR_OUT_MON__ARM_A_MATERIAL_STATUS;
	CX__VAR_DIGITAL_CTRL dCH__OTR_OUT_MON__ARM_B_MATERIAL_STATUS;

	CX__VAR_STRING_CTRL  sCH__OTR_OUT_MON__ARM_A_MATERIAL_TITLE;
	CX__VAR_STRING_CTRL  sCH__OTR_OUT_MON__ARM_B_MATERIAL_TITLE;

	// WAFER INFO ...
	CX__VAR_STRING_CTRL  sCH__WFR_INFO_PORTID__ARM_A;
	CX__VAR_STRING_CTRL  sCH__WFR_INFO_SLOTID__ARM_A;
	CX__VAR_STRING_CTRL  sCH__WFR_INFO_PPID__ARM_A;
	CX__VAR_STRING_CTRL  sCH__WFR_INFO_LOTID__ARM_A;
	CX__VAR_STRING_CTRL  sCH__WFR_INFO_MATERIALID__ARM_A;

	CX__VAR_STRING_CTRL  sCH__WFR_INFO_PORTID__ARM_B;
	CX__VAR_STRING_CTRL  sCH__WFR_INFO_SLOTID__ARM_B;
	CX__VAR_STRING_CTRL  sCH__WFR_INFO_PPID__ARM_B;
	CX__VAR_STRING_CTRL  sCH__WFR_INFO_LOTID__ARM_B;
	CX__VAR_STRING_CTRL  sCH__WFR_INFO_MATERIALID__ARM_B;

	// ANIMATION CHANNEL
	CX__VAR_DIGITAL_CTRL dCH__OTR_OUT_MON__ACT_ARM;
	CX__VAR_DIGITAL_CTRL dCH__OTR_OUT_MON__ARM_A_ACT;
	CX__VAR_DIGITAL_CTRL dCH__OTR_OUT_MON__ARM_B_ACT;
	CX__VAR_DIGITAL_CTRL dCH__OTR_OUT_MON__TRG_MOVE;
	CX__VAR_DIGITAL_CTRL dCH__OTR_OUT_MON__TRG_ROTATE;

	int iDATA__PMx_RETRACT_COUNT;
	CX__VAR_STRING_CTRL  sCH__OTR_OUT_MON__PMx_ARM_STATE[CFG_PMx__SIZE];
	CX__VAR_DIGITAL_CTRL dCH__OTR_OUT_MON__PMx_ARM_A_ACT[CFG_PMx__SIZE];
	CX__VAR_DIGITAL_CTRL dCH__OTR_OUT_MON__PMx_ARM_B_ACT[CFG_PMx__SIZE];

	CX__VAR_DIGITAL_CTRL dCH__OTR_OUT_MON__LLx_ARM_STATE[CFG_LLx__SIZE];
	CX__VAR_DIGITAL_CTRL dCH__OTR_OUT_MON__LBx_ARM_A_ACT[CFG_LLx__SIZE];
	CX__VAR_DIGITAL_CTRL dCH__OTR_OUT_MON__LBx_ARM_B_ACT[CFG_LLx__SIZE];

	// TAS Channel ...
	CX__VAR_STRING_CTRL  sCH__TAS_RESET_REQ;

	CX__VAR_STRING_CTRL  sCH__TAS_ACTION_TIME_NOW__RB[CFG_ACT__SIZE];
	CX__VAR_STRING_CTRL  sCH__TAS_ACTION_TIME_MIN__RB[CFG_ACT__SIZE];
	CX__VAR_STRING_CTRL  sCH__TAS_ACTION_TIME_MAX__RB[CFG_ACT__SIZE];
	CX__VAR_STRING_CTRL  sCH__TAS_ACTION_TIME_AVG__RB[CFG_ACT__SIZE];
	CX__VAR_STRING_CTRL  sCH__TAS_ACTION_TIME_AVG_F__RB[CFG_ACT__SIZE];
	CX__VAR_ANALOG_CTRL  aCH__TAS_ACTION_TIME_CNT__RB[CFG_ACT__SIZE];

	CX__VAR_STRING_CTRL  sCH__TAS_ACTION_TIME_NOW__LLx_X[CFG_ACT__SIZE][CFG_LLx__SIZE];
	CX__VAR_STRING_CTRL  sCH__TAS_ACTION_TIME_MIN__LLx_X[CFG_ACT__SIZE][CFG_LLx__SIZE];
	CX__VAR_STRING_CTRL  sCH__TAS_ACTION_TIME_MAX__LLx_X[CFG_ACT__SIZE][CFG_LLx__SIZE];
	CX__VAR_STRING_CTRL  sCH__TAS_ACTION_TIME_AVG__LLx_X[CFG_ACT__SIZE][CFG_LLx__SIZE];
	CX__VAR_STRING_CTRL  sCH__TAS_ACTION_TIME_AVG_F__LLx_X[CFG_ACT__SIZE][CFG_LLx__SIZE];
	CX__VAR_ANALOG_CTRL  aCH__TAS_ACTION_TIME_CNT__LLx_X[CFG_ACT__SIZE][CFG_LLx__SIZE];

	CX__VAR_STRING_CTRL  sCH__TAS_ACTION_TIME_NOW__LLx_SLOT[CFG_ACT__SIZE][CFG_LLx__SIZE][CFG_LLx__SLOT_SIZE];
	CX__VAR_STRING_CTRL  sCH__TAS_ACTION_TIME_MIN__LLx_SLOT[CFG_ACT__SIZE][CFG_LLx__SIZE][CFG_LLx__SLOT_SIZE];
	CX__VAR_STRING_CTRL  sCH__TAS_ACTION_TIME_MAX__LLx_SLOT[CFG_ACT__SIZE][CFG_LLx__SIZE][CFG_LLx__SLOT_SIZE];
	CX__VAR_STRING_CTRL  sCH__TAS_ACTION_TIME_AVG__LLx_SLOT[CFG_ACT__SIZE][CFG_LLx__SIZE][CFG_LLx__SLOT_SIZE];
	CX__VAR_STRING_CTRL  sCH__TAS_ACTION_TIME_AVG_F__LLx_SLOT[CFG_ACT__SIZE][CFG_LLx__SIZE][CFG_LLx__SLOT_SIZE];
	CX__VAR_ANALOG_CTRL  aCH__TAS_ACTION_TIME_CNT__LLx_SLOT[CFG_ACT__SIZE][CFG_LLx__SIZE][CFG_LLx__SLOT_SIZE];

	CX__VAR_STRING_CTRL  sCH__TAS_ACTION_TIME_NOW__PMx[CFG_ACT__SIZE][CFG_PMx__SIZE];
	CX__VAR_STRING_CTRL  sCH__TAS_ACTION_TIME_MIN__PMx[CFG_ACT__SIZE][CFG_PMx__SIZE];
	CX__VAR_STRING_CTRL  sCH__TAS_ACTION_TIME_MAX__PMx[CFG_ACT__SIZE][CFG_PMx__SIZE];
	CX__VAR_STRING_CTRL  sCH__TAS_ACTION_TIME_AVG__PMx[CFG_ACT__SIZE][CFG_PMx__SIZE];
	CX__VAR_STRING_CTRL  sCH__TAS_ACTION_TIME_AVG_F__PMx[CFG_ACT__SIZE][CFG_PMx__SIZE];
	CX__VAR_ANALOG_CTRL  aCH__TAS_ACTION_TIME_CNT__PMx[CFG_ACT__SIZE][CFG_PMx__SIZE];

	// DA : Config ...
	CX__VAR_ANALOG_CTRL  aCH__CFG__R_OFFSET_HARD_ERROR__LLx_X[CFG_LLx__SIZE];
	CX__VAR_ANALOG_CTRL  aCH__CFG__T_OFFSET_HARD_ERROR__LLx_X[CFG_LLx__SIZE];
	CX__VAR_ANALOG_CTRL  aCH__CFG__R_OFFSET_SOFT_ERROR__LLx_X[CFG_PMx__SIZE];
	CX__VAR_ANALOG_CTRL  aCH__CFG__T_OFFSET_SOFT_ERROR__LLx_X[CFG_PMx__SIZE];

	CX__VAR_ANALOG_CTRL  aCH__CFG__R_OFFSET_HARD_ERROR__LLx_SLOT[CFG_LLx__SIZE][CFG_LLx__SLOT_SIZE];
	CX__VAR_ANALOG_CTRL  aCH__CFG__T_OFFSET_HARD_ERROR__LLx_SLOT[CFG_LLx__SIZE][CFG_LLx__SLOT_SIZE];
	CX__VAR_ANALOG_CTRL  aCH__CFG__R_OFFSET_SOFT_ERROR__LLx_SLOT[CFG_LLx__SIZE][CFG_LLx__SLOT_SIZE];
	CX__VAR_ANALOG_CTRL  aCH__CFG__T_OFFSET_SOFT_ERROR__LLx_SLOT[CFG_LLx__SIZE][CFG_LLx__SLOT_SIZE];

	//
	CX__VAR_ANALOG_CTRL  aCH__CFG__R_OFFSET_HARD_ERROR__PMx[CFG_LLx__SIZE];
	CX__VAR_ANALOG_CTRL  aCH__CFG__T_OFFSET_HARD_ERROR__PMx[CFG_LLx__SIZE];
	CX__VAR_ANALOG_CTRL  aCH__CFG__R_OFFSET_SOFT_ERROR__PMx[CFG_LLx__SIZE];
	CX__VAR_ANALOG_CTRL  aCH__CFG__T_OFFSET_SOFT_ERROR__PMx[CFG_LLx__SIZE];

	//
	CX__VAR_ANALOG_CTRL  aCH__CFG__DA_CHART_AXIS_MAX;
	CX__VAR_ANALOG_CTRL  aCH__CFG__DA_CHART_ERR_RANGE;

	// DA Result ...
	CX__VAR_STRING_CTRL  sCH__MON_R_OFFSET_A_ARM__ROBOT;
	CX__VAR_STRING_CTRL  sCH__MON_R_OFFSET_B_ARM__ROBOT;
	CX__VAR_STRING_CTRL  sCH__MON_T_OFFSET_A_ARM__ROBOT;
	CX__VAR_STRING_CTRL  sCH__MON_T_OFFSET_B_ARM__ROBOT;

	//
	CX__VAR_STRING_CTRL  sCH__MON_STS_OFFSET_PICK__LLx_X[CFG_LLx__SIZE];
	CX__VAR_STRING_CTRL  sCH__MON_STS_OFFSET_PLACE__LLx_X[CFG_LLx__SIZE];

	CX__VAR_STRING_CTRL  sCH__MON_RESULT_OFFSET_PICK__LLx_X[CFG_LLx__SIZE];
	CX__VAR_STRING_CTRL  sCH__MON_RESULT_OFFSET_PLACE__LLx_X[CFG_LLx__SIZE];

	CX__VAR_STRING_CTRL  sCH__MON_R_OFFSET_PICK__LLx_X[CFG_LLx__SIZE];
	CX__VAR_STRING_CTRL  sCH__MON_R_OFFSET_PLACE__LLx_X[CFG_LLx__SIZE];
	CX__VAR_STRING_CTRL  sCH__MON_T_OFFSET_PICK__LLx_X[CFG_LLx__SIZE];
	CX__VAR_STRING_CTRL  sCH__MON_T_OFFSET_PLACE__LLx_X[CFG_LLx__SIZE];

	CX__VAR_STRING_CTRL  sCH__MON_DA_RT_OFFSET__LLx_X[CFG_LLx__SIZE];

	CX__VAR_STRING_CTRL  sCH__DA_CHART_R_OFFSET_DISPLAY__LLx_X[CFG_LLx__SIZE];
	CX__VAR_STRING_CTRL  sCH__DA_CHART_T_OFFSET_DISPLAY__LLx_X[CFG_LLx__SIZE];

	//
	CX__VAR_STRING_CTRL  sCH__MON_STS_OFFSET_PICK__LLx_SLOT[CFG_LLx__SIZE][CFG_LLx__SLOT_SIZE];
	CX__VAR_STRING_CTRL  sCH__MON_STS_OFFSET_PLACE__LLx_SLOT[CFG_LLx__SIZE][CFG_LLx__SLOT_SIZE];

	CX__VAR_STRING_CTRL  sCH__MON_RESULT_OFFSET_PICK__LLx_SLOT[CFG_LLx__SIZE][CFG_LLx__SLOT_SIZE];
	CX__VAR_STRING_CTRL  sCH__MON_RESULT_OFFSET_PLACE__LLx_SLOT[CFG_LLx__SIZE][CFG_LLx__SLOT_SIZE];

	CX__VAR_STRING_CTRL  sCH__MON_R_OFFSET_PICK__LLx_SLOT[CFG_LLx__SIZE][CFG_LLx__SLOT_SIZE];
	CX__VAR_STRING_CTRL  sCH__MON_R_OFFSET_PLACE__LLx_SLOT[CFG_LLx__SIZE][CFG_LLx__SLOT_SIZE];
	CX__VAR_STRING_CTRL  sCH__MON_T_OFFSET_PICK__LLx_SLOT[CFG_LLx__SIZE][CFG_LLx__SLOT_SIZE];
	CX__VAR_STRING_CTRL  sCH__MON_T_OFFSET_PLACE__LLx_SLOT[CFG_LLx__SIZE][CFG_LLx__SLOT_SIZE];

	CX__VAR_STRING_CTRL  sCH__MON_DA_RT_OFFSET__LLx_SLOT[CFG_LLx__SIZE][CFG_LLx__SLOT_SIZE];

	CX__VAR_STRING_CTRL  sCH__DA_CHART_R_OFFSET_DISPLAY__LLx_SLOT[CFG_LLx__SIZE][CFG_LLx__SLOT_SIZE];
	CX__VAR_STRING_CTRL  sCH__DA_CHART_T_OFFSET_DISPLAY__LLx_SLOT[CFG_LLx__SIZE][CFG_LLx__SLOT_SIZE];

	//
	CX__VAR_STRING_CTRL  sCH__MON_STS_OFFSET_PICK__PMx[CFG_PMx__SIZE];
	CX__VAR_STRING_CTRL  sCH__MON_STS_OFFSET_PLACE__PMx[CFG_PMx__SIZE];

	CX__VAR_STRING_CTRL  sCH__MON_RESULT_OFFSET_PICK__PMx[CFG_PMx__SIZE];
	CX__VAR_STRING_CTRL  sCH__MON_RESULT_OFFSET_PLACE__PMx[CFG_PMx__SIZE];

	CX__VAR_STRING_CTRL  sCH__MON_R_OFFSET_PICK__PMx[CFG_PMx__SIZE];
	CX__VAR_STRING_CTRL  sCH__MON_R_OFFSET_PLACE__PMx[CFG_PMx__SIZE];
	CX__VAR_STRING_CTRL  sCH__MON_T_OFFSET_PICK__PMx[CFG_PMx__SIZE];
	CX__VAR_STRING_CTRL  sCH__MON_T_OFFSET_PLACE__PMx[CFG_PMx__SIZE];

	CX__VAR_STRING_CTRL  sCH__MON_DA_RT_OFFSET__PMx[CFG_PMx__SIZE];

	// DA Link ...
	CX__VAR_STRING_CTRL  sCH__DA_CHART_R_OFFSET_DISPLAY__PMx[CFG_PMx__SIZE];
	CX__VAR_STRING_CTRL	 sCH__DA_CHART_T_OFFSET_DISPLAY__PMx[CFG_PMx__SIZE];

	CX__VAR_DIGITAL_CTRL dCH__DA_OFFSET_RESULT_REPORT__PMx[CFG_PMx__SIZE];

	// TEST ...
	CX__VAR_STRING_CTRL  sCH__TEST_R_OFFSET_MM;
	CX__VAR_STRING_CTRL  sCH__TEST_T_OFFSET_DEG;

	CX__VAR_STRING_CTRL  sCH__TEST_X_OFFSET_MM;
	CX__VAR_STRING_CTRL  sCH__TEST_Y_OFFSET_MM;
	//


	//-------------------------------------------------------------------------
	//  EXTERNAL PROPERTY

	// DB_CFG -----
	CX__VAR_DIGITAL_CTRL dEXT_CH__CFG_SETUP_TEST_MODE;

	CX__VAR_DIGITAL_CTRL dEXT_CH__CFG_TRANSFER_MODE;
	CX__VAR_ANALOG_CTRL  aEXT_CH__CFG_REF_ATM_PRESSURE;
	CX__VAR_ANALOG_CTRL  aEXT_CH__CFG_REF_VAC_PRESSURE;

	CX__VAR_DIGITAL_CTRL dEXT_CH__CFG_LLx_SLOT_MAX[CFG_LLx__SIZE];
	CX__VAR_DIGITAL_CTRL dEXT_CH__CFG_LLx_SLOT_USE_X[CFG_LLx__SIZE][CFG_LLx__SLOT_SIZE];
	CX__VAR_DIGITAL_CTRL dEXT_CH__CFG_LLx_1_EXIST_FLAG[CFG_LLx__SIZE];
	CX__VAR_DIGITAL_CTRL dEXT_CH__CFG_LLx_2_EXIST_FLAG[CFG_LLx__SIZE];
	CX__VAR_DIGITAL_CTRL dEXT_CH__CFG_LLx_EXIST_FLAG[CFG_LLx__SIZE];
	CX__VAR_DIGITAL_CTRL dEXT_CH__CFG_PMx_EXIST_FLAG[CFG_PMx__SIZE];

	CX__VAR_DIGITAL_CTRL dEXT_CH__CFG_PMx_SV_USE[CFG_PMx__SIZE];

	//
	CX__VAR_DIGITAL_CTRL dEXT_CH__CFG_DA_PICK_USE__LLx[CFG_LLx__SIZE];
	CX__VAR_DIGITAL_CTRL dEXT_CH__CFG_DA_PLACE_USE__LLx[CFG_LLx__SIZE];

	CX__VAR_DIGITAL_CTRL dEXT_CH__CFG_DA_PICK_USE__PMx[CFG_PMx__SIZE];
	CX__VAR_DIGITAL_CTRL dEXT_CH__CFG_DA_PLACE_USE__PMx[CFG_PMx__SIZE];

	// ROBOT -----
	CII_EXT_OBJECT__CTRL *pROBOT__OBJ_CTRL;

	CX__VAR_DIGITAL_CTRL dEXT_CH__ROBOT_ARM_A_MATERIAL_STATUS;
	CX__VAR_DIGITAL_CTRL dEXT_CH__ROBOT_ARM_B_MATERIAL_STATUS;

	CX__VAR_STRING_CTRL  sEXT_CH__ROBOT_DA_RESULT_R_OFFSET_MM;     // Radius (mm)
	CX__VAR_STRING_CTRL  sEXT_CH__ROBOT_DA_RESULT_T_OFFSET_DEG;    // Theta (degree)
	
	CX__VAR_STRING_CTRL  sEXT_CH__ROBOT_DA_RESULT_X_OFFSET_MM;     // 221017:KMS
	CX__VAR_STRING_CTRL  sEXT_CH__ROBOT_DA_RESULT_Y_OFFSET_MM;     // 221017:KMS

	// ARM_RNE.SENSOR ...
	bool bActive__ROBOT_RNE_SNS__ARM_A;
	bool bActive__ROBOT_RNE_SNS__ARM_B;

	CX__VAR_DIGITAL_CTRL dEXT_CH__ROBOT_RNE_SNS__ARM_A;
	CX__VAR_DIGITAL_CTRL dEXT_CH__ROBOT_RNE_SNS__ARM_B;

	// ARM_RNE.STATE ...
	CString sDATA__RNE_ON;
	CString sDATA__RNE_OFF;
	//

	// TMC CHM -----
	CX__VAR_ANALOG_CTRL  aEXT_CH__TMC_CHM_PRESSURE_TORR;

	// PMx -----
	CX__VAR_ANALOG_CTRL  aEXT_CH__PMx_PRESSURE_TORR[CFG_PMx__SIZE];
	CX__VAR_DIGITAL_CTRL dEXT_CH__PMx_SLIT_VALVE_STATUS[CFG_PMx__SIZE];

	CX__VAR_DIGITAL_CTRL dEXT_CH__PMx_SLOT01_STATUS[CFG_PMx__SIZE];
	CX__VAR_STRING_CTRL  sEXT_CH__PMx_SLOT01_TITLE[CFG_PMx__SIZE];

	// LLx -----
	int iSIZE__LLx;
	int iLLx_SLOT_SIZE;
	bool bActive__LLx_MULTI_DOOR_VALVE;
	bool bActive__LLx_MULTI_SLOT_VALVE;

	CX__VAR_ANALOG_CTRL  aEXT_CH__LLx_PRESSURE_TORR[CFG_LLx__SIZE];

	CX__VAR_DIGITAL_CTRL dEXT_CH__LLx_SLIT_VALVE_STATUS_A[CFG_LLx__SIZE];
	CX__VAR_DIGITAL_CTRL dEXT_CH__LLx_SLIT_VALVE_STATUS_XX[CFG_LLx__SIZE][CFG_LLx__SLOT_SIZE];

	CX__VAR_DIGITAL_CTRL dEXT_CH__LLx_SLOT_STATUS[CFG_LLx__SIZE][CFG_LLx__SLOT_SIZE];
	CX__VAR_STRING_CTRL  sEXT_CH__LLx_SLOT_TITLE[CFG_LLx__SIZE][CFG_LLx__SLOT_SIZE];
	//

	//-------------------------------------------------------------------------
	CString sMODE__INIT;
	int  Call__INIT(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm);

	CString sMODE__SET_PARA;
	int  Call__SET_PARA(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm);

	// Pick -----
	CString sMODE__PICK;
	int  Call__PICK(CII_OBJECT__VARIABLE* p_variable,
					CII_OBJECT__ALARM* p_alarm,
					const CString& arm_type,
					const CString& stn_name,
					const CString& stn_slot,
					const int handoff_mode);

	int  Fnc__PICK(CII_OBJECT__VARIABLE* p_variable,
				   CII_OBJECT__ALARM* p_alarm,
				   const CString& arm_type,
				   const CString& stn_name,
				   const CString& stn_slot,
				   const int handoff_mode);

	int  Fnc__ACT_HOFF_PICK(CII_OBJECT__VARIABLE* p_variable,
							CII_OBJECT__ALARM* p_alarm,
							const CString& arm_type,
							const CString& stn_name,
							const CString& stn_slot);

	int  Fnc__ACTION(const CString& arm_type,
					 const CString& stn_name,
					 const CString& stn_slot,
					 const CString& cmmd_act);

	int  _Update__ACTION_MIN_MAX(CII__VAR_STRING_CTRL* p_ch_min,
								 CII__VAR_STRING_CTRL* p_ch_max,
								 CII__VAR_STRING_CTRL* p_ch_avg,
								 CII__VAR_STRING_CTRL* p_ch_avg_f,
								 CII__VAR_ANALOG_CTRL* p_ch_cnt,
								 const double cur_sec);

	// Place -----
	CString sMODE__PLACE;
	int  Call__PLACE(CII_OBJECT__VARIABLE* p_variable,
					CII_OBJECT__ALARM* p_alarm,
					const CString& arm_type,
					const CString& stn_name,
					const CString& stn_slot,
					const int handoff_mode);

	int  Fnc__PLACE(CII_OBJECT__VARIABLE* p_variable,
					CII_OBJECT__ALARM* p_alarm,
					const CString& arm_type,
					const CString& stn_name,
					const CString& stn_slot,
					const int handoff_mode);

	int  Fnc__ACT_HOFF_PLACE(CII_OBJECT__VARIABLE* p_variable,
							CII_OBJECT__ALARM* p_alarm,
							const CString& arm_type,
							const CString& stn_name,
							const CString& stn_slot);

	// Swap -----
	CString sMODE__SWAP;
	int  Call__SWAP(CII_OBJECT__VARIABLE* p_variable,
					CII_OBJECT__ALARM* p_alarm,
					const CString& arm_type,
					const CString& stn_name,
					const CString& stn_slot,
					const int handoff_mode);

	int  Fnc__ACT_HOFF_SWAP(CII_OBJECT__VARIABLE* p_variable,
							CII_OBJECT__ALARM* p_alarm,
							const CString& arm_type,
							const CString& stn_name,
							const CString& stn_slot);

	// Rotate -----
	CString sMODE__ROTATE;
	int  Call__ROTATE(CII_OBJECT__VARIABLE* p_variable,
					CII_OBJECT__ALARM* p_alarm,
					const CString& arm_type,
					const CString& stn_name, 
					const CString& stn_slot);

	// Home -----
	CString sMODE__HOME;
	int  Call__HOME(CII_OBJECT__VARIABLE* p_variable,
					CII_OBJECT__ALARM* p_alarm);


	// Map -----
	CString sMODE__MAP;
	int  Call__MAP(CII_OBJECT__VARIABLE* p_variable,
					CII_OBJECT__ALARM* p_alarm);

	// RETRACT -----
	CString sMODE__RETRACT;
	int  Call__RETRACT(CII_OBJECT__VARIABLE* p_variable,
						CII_OBJECT__ALARM* p_alarm,
						const CString& arm_type,
						const CString& stn_name, 
						const CString& stn_slot);
	int  Fnc__RETRACT(CII_OBJECT__VARIABLE* p_variable,
					  CII_OBJECT__ALARM* p_alarm,
					  const CString& arm_type,
					  const CString& stn_name, 
					  const CString& stn_slot);

	// EXTEND -----
	CString sMODE__EXTEND;
	int  Call__EXTEND(CII_OBJECT__VARIABLE* p_variable,
						CII_OBJECT__ALARM* p_alarm,
						const CString& arm_type,
						const CString& stn_name, 
						const CString& stn_slot);
	int  Fnc__EXTEND(CII_OBJECT__VARIABLE* p_variable,
					 CII_OBJECT__ALARM* p_alarm,
					 const CString& arm_type,
					 const CString& stn_name, 
					 const CString& stn_slot);

	// GOUP -----
	CString sMODE__GOUP;
	int  Call__GOUP(CII_OBJECT__VARIABLE* p_variable,
					CII_OBJECT__ALARM* p_alarm,
					const CString& arm_type,
					const CString& stn_name, 
					const CString& stn_slot);

	// GODOWN -----
	CString sMODE__GODOWN;
	int  Call__GODOWN(CII_OBJECT__VARIABLE* p_variable,
					CII_OBJECT__ALARM* p_alarm,
					const CString& arm_type,
					const CString& stn_name, 
					const CString& stn_slot);

	// -----
	CString sMODE__RQ_STN;
	int  Call__RQ_STN(CII_OBJECT__VARIABLE* p_variable,
					CII_OBJECT__ALARM* p_alarm,
					const CString& arm_type,
					const CString& stn_name);

	CString sMODE__SET_STN;
	int  Call__SET_STN(CII_OBJECT__VARIABLE* p_variable,
					CII_OBJECT__ALARM* p_alarm,
					const CString& arm_type,
					const CString& stn_name);

	CString sMODE__TEACHED_CPTR_SAVE;
	int  Call__TEACHED_CPTR_SAVE(CII_OBJECT__VARIABLE* p_variable,
								CII_OBJECT__ALARM* p_alarm,
								const CString& arm_type,
								const CString& stn_name);

	// TIME TEST ...
	CString sMODE__TIME_TEST;
	int  Call__TIME_TEST(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	// TEST ...
	CString sMODE__TEST_RT_TO_XY;
	int  Call__TEST_RT_TO_XY(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	CString sMODE__TEST_XY_TO_RT;
	int  Call__TEST_XY_TO_RT(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	// ...
	int  Check__STN_EXIST(CII_OBJECT__ALARM* p_alarm, const CString& stn_name,const CString& stn_slot);
	//

	//-------------------------------------------------------------------------
	// ...
	int  Interlock__LLx_SLOT_CHECK(CII_OBJECT__ALARM* p_alarm,
								   const CString& stn_name, 
								   const CString& para_slot);

	int  Interlock__CHECK_MATERIAL(CII_OBJECT__ALARM* p_alarm,
								   const bool active_place,
								   const CString& arm_type,
								   const CString& stn_name,
								   const CString& stn_slot,
								   const CString& act_name);
	int  _Interlock__CHECK_MATERIAL(CII_OBJECT__ALARM* p_alarm,
									const bool active_place,
									const CString& arm_type,
									const CString& stn_name,
									const CString& stn_slot,
									const CString& act_name);

	int  Interlock__CHECK_VALVE_OPEN(CII_OBJECT__ALARM* p_alarm,
									 const CString& arm_type,
									 const CString& stn_name,
									 const CString& stn_slot,
									 const CString& act_name);
	int  _Interlock__CHECK_VALVE_OPEN(CII_OBJECT__ALARM* p_alarm,
									  const CString& arm_type,
									  const CString& stn_name,
									  const CString& stn_slot,
									  const CString& act_name);

	int  Interlock__CHECK_PRESSURE_VAC(CII_OBJECT__ALARM* p_alarm,
									   const CString& stn_name,
									   const CString& act_name);
	int  Interlock__CHECK_PRESSURE_ATM(CII_OBJECT__ALARM* p_alarm,
									   const CString& stn_name,
									   const CString& act_name);

	int  Fnc__CHANGE_MATERIAL_INFO(const int place_flag,
								   const CString& arm_type,
								   const CString& stn_name,
								   const CString& stn_slot);
	int  _Fnc__CHANGE_MATERIAL_INFO(const int place_flag,
									const CString& arm_type,
									const CString& stn_name,
									const CString& stn_slot);

	int  _Update__MATERIAL_INFO();

	// ...
	void Set_ANI__ROBOT_EXTEND(const CString& arm_type,const CString& stn_name,const CString& stn_slot);
	void _Set_ANI__ROBOT_EXTEND(const CString& arm_type,const CString& stn_name,const CString& stn_slot);

	void Set_ANI__ROBOT_ALL_RETRACT();
	void Set_ANI__ROBOT_RETRACT(const CString& arm_type, const CString& stn_name,const CString& stn_slot);
	void _Set_ANI__ROBOT_RETRACT(const CString& arm_type, const CString& stn_name,const CString& stn_slot);

	void Set_ANI__ROBOT_ROTATE(const CString& arm_type, const CString& stn_name,const CString& stn_slot);
	void _Set_ANI__ROBOT_ROTATE(const CString& arm_type, const CString& stn_name,const CString& stn_slot);

	// ...
	int _Report__DA_OFFSET(CII_OBJECT__ALARM* p_alarm, const CString& arm_type,const CString& stn_name,const CString& stn_slot, const bool active__place_act);
	int __Report__DA_OFFSET(CII_OBJECT__ALARM* p_alarm, const CString& arm_type,const CString& stn_name,const CString& stn_slot, const bool active__place_act);
	
	int _Set__DA_RT_OFFSET(const CString& stn_name, const CString& stn_slot);
	int _Clear__DA_RT_OFFSET(const CString& stn_name, const CString& stn_slot);

	// ...
	void _Get__ARM_INFO(const CString& arm_type,
						const CString& stn_name,
						const CString& stn_slot,
						CStringArray& l__arm_type,
						CStringArray& l__stn_name,
						CStringArray& l__stn_slot);

	int _Save__DA_OFFSET(const CString& arm_type,
						 const CString& stn_name,
						 const CString& stn_slot,
						 const bool active_place);
	int __Save__DA_OFFSET(const CString& arm_type,
						  const CString& stn_name,
						  const CString& stn_slot,
						  const bool active_place);

	int _Active__DA_STS(const bool active_place,
						const CString& arm_type,
						const CString& stn_name,
						const CString& stn_slot,
						const bool result_clear);
	int __Active__DA_STS(const bool active_place,
						 const CString& arm_type,
						 const CString& stn_name,
						 const CString& stn_slot,
						 const bool result_clear);

	int _Clear__DA_STS(const bool active_place,
					   const CString& arm_type,
					   const CString& stn_name,
					   const CString& stn_slot);
	int __Clear__DA_STS(const bool active_place,
						const CString& arm_type,
						const CString& stn_name,
						const CString& stn_slot);

	int _Get__CFG_DA_OFFSET_OF_STN(const CString& stn_name, 
								   const CString& stn_slot, 
								   double& cfg__soft_r,
								   double& cfg__hard_r,
								   double& cfg__soft_t,
								   double& cfg__hard_t);

	CString _Get__CFG_DYNAMIC_ALGN_USE(const CString& stn_name, const bool active__place_act);

	// ...
	void Fnc__APP_LOG(const CString& log_msg);

	// ...
	void Mon__STATE_MONITOR(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM *p_alarm);
	//


public:
	CObj__VAC_ROBOT_STD();
	~CObj__VAC_ROBOT_STD();

	//-------------------------------------------------------------------------
	int __DEFINE__CONTROL_MODE(obj, l_mode);
	int __DEFINE__VERSION_HISTORY(version);

	int __DEFINE__VARIABLE_STD(p_variable);
	int __DEFINE__ALARM(p_alarm);

	int __Define__USER_FUNCTION(CII_DEFINE__FUNCTION *p_fnc_ctrl);

	//-------------------------------------------------------------------------
	int __INITIALIZE__OBJECT(p_variable,p_ext_obj_create);

	//-------------------------------------------------------------------------
	int __CALL__CONTROL_MODE(mode, p_debug, p_variable, p_alarm);
	int __CALL__MONITORING(id, p_variable, p_alarm);
};
