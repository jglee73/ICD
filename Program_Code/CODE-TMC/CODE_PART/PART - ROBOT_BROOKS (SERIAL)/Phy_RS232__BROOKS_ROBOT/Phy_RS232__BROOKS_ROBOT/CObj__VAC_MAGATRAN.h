#pragma once

#include "Interface_Code.h"

#include "CCommon_Error.h"
#include "CCommon_SYSTEM.h"


class CObj__VAC_MAGATRAN : public __IOBJ__IO_TYPE
{
private:
	//-------------------------------------------------------------------------
	CString	sObject_Name;

	// ...
	CCommon_Error__USER_FNC mERROR__USER_FNC;

	SCX__USER_LOG_CTRL xAPP_LOG_CTRL;
	SCX__USER_LOG_CTRL xDRV_LOG_CTRL;

	int iActive_SIM;

	// ...
	CII__EXT_FNC_MODE_CTRL *pII__EXT_MODE_CTRL;
	CII__EXT_FNC_VAR_CTRL  *pII__EXT_VAR_CTRL;
	CII__EXT_USER_FNC_CTRL *pII__EXT_FNC_CTRL;

	CString m_szRoot;

	SCX__SERIAL	mX_Serial;

	CString m_Term_Str;
	int	 m_nCommState;
	int	 m_Rcv_Time;
	int	 m_Out_Time;

	//
	int  iFlag__APP_LOG;
	int  iFlag__DRV_LOG;

	// ...
	CString  m_sErrorID;
	CString  m_sStationA;
	CString  m_sStationB;
	CString  m_sCptr_StationA;
	CString  m_sCptr_StationB;
	CString  m_sRPos;
	CString  m_sZPos;
	CString  m_sSlot;
	CString  m_sWfrSnsIntlk;

	CString  m_sRO_OFFSET;
	CString  m_sTO_OFFSET;

	CString  m_sSTN_R_SET_VAL;
	CString  m_sSTN_T_SET_VAL;

	// 
	CString  m_sBefore_Stn_Retract_ArmA;
	CString  m_sBefore_Stn_Retract_ArmB;

	struct CPTR_TUNED_VAL 
	{
		double ro_tuned_val;
		double to_tuned_val;
	};
	CPTR_TUNED_VAL ARM_A_m_set_cptr_tune_val;
	CPTR_TUNED_VAL ARM_B_m_set_cptr_tune_val;


	//-------------------------------------------------------------------------
	//  INTERNAL PROPERTY

	// DA CHART ...
	CX__VAR_DIGITAL_CTRL dDA_CHART_ARM_A_UPDATE_FLAG_PM_X[CFG_PMx__SIZE];
	CX__VAR_DIGITAL_CTRL dDA_CHART_ARM_B_UPDATE_FLAG_PM_X[CFG_PMx__SIZE];
	CX__VAR_DIGITAL_CTRL dDA_CHART_DATA_CLEAR_PM_X[CFG_PMx__SIZE];

	CX__VAR_DIGITAL_CTRL dDA_CHART_ARM_A_UPDATE_FLAG_LL_X[CFG_LLx__SIZE];
	CX__VAR_DIGITAL_CTRL dDA_CHART_ARM_B_UPDATE_FLAG_LL_X[CFG_LLx__SIZE];
	CX__VAR_DIGITAL_CTRL dDA_CHART_DATA_CLEAR_LL_X[CFG_LLx__SIZE];

	CX__VAR_STRING_CTRL	 sDA_CHART_ARM_A_R_OFFSET_DISPLAY_PM_X[CFG_PMx__SIZE];
	CX__VAR_STRING_CTRL	 sDA_CHART_ARM_A_T_OFFSET_DISPLAY_PM_X[CFG_PMx__SIZE];
	CX__VAR_STRING_CTRL	 sDA_CHART_ARM_B_R_OFFSET_DISPLAY_PM_X[CFG_PMx__SIZE];
	CX__VAR_STRING_CTRL	 sDA_CHART_ARM_B_T_OFFSET_DISPLAY_PM_X[CFG_PMx__SIZE];

	CX__VAR_STRING_CTRL  sDA_CHART_ARM_A_R_OFFSET_DISPLAY_LL_X[CFG_LLx__SIZE];
	CX__VAR_STRING_CTRL  sDA_CHART_ARM_A_T_OFFSET_DISPLAY_LL_X[CFG_LLx__SIZE];
	CX__VAR_STRING_CTRL  sDA_CHART_ARM_B_R_OFFSET_DISPLAY_LL_X[CFG_LLx__SIZE];
	CX__VAR_STRING_CTRL  sDA_CHART_ARM_B_T_OFFSET_DISPLAY_LL_X[CFG_LLx__SIZE];

	CX__VAR_STRING_CTRL  sDA_CHART_R_OFFSET_DISPLAY_PM_X[CFG_PMx__SIZE];
	CX__VAR_STRING_CTRL	 sDA_CHART_T_OFFSET_DISPLAY_PM_X[CFG_PMx__SIZE];

	CX__VAR_STRING_CTRL  sDA_CHART_R_OFFSET_DISPLAY_LL_X[CFG_LLx__SIZE];
	CX__VAR_STRING_CTRL  sDA_CHART_T_OFFSET_DISPLAY_LL_X[CFG_LLx__SIZE];

	// DA RESULT ...
	CX__VAR_STRING_CTRL	 sCH__DA_RESULT_R_OFFSET_MM;
	CX__VAR_STRING_CTRL	 sCH__DA_RESULT_T_OFFSET_DEG;

	CX__VAR_STRING_CTRL	 sCH__DA_RESULT_X_OFFSET_MM;
	CX__VAR_STRING_CTRL	 sCH__DA_RESULT_Y_OFFSET_MM;

	// ...
	CX__VAR_STRING_CTRL  sCH__MON_COMMUNICATION_STATE;
	CX__VAR_STRING_CTRL  sCH__MON_COMMUNICATION_INFO;

	CX__VAR_STRING_CTRL	 sCH__ACT_MSG;
	CX__VAR_STRING_CTRL	 sCH__ERRID;
	CX__VAR_STRING_CTRL	 sCH__szRoot;

	// PARA CHANNEL
	CX__VAR_DIGITAL_CTRL dCH__OTR_IN_PARA__STN_NAME;
	CX__VAR_DIGITAL_CTRL dCH__OTR_IN_PARA__STN_SLOT;
	CX__VAR_DIGITAL_CTRL dCH__OTR_IN_PARA__ARM_TYPE;
	CX__VAR_STRING_CTRL  sCH__ACTION_STS;
	//

	CX__VAR_STRING_CTRL sCH__ARM_A_BEFORE_MODE_STS;
	CX__VAR_STRING_CTRL sCH__ARM_B_BEFORE_MODE_STS;

	CX__VAR_ANALOG_CTRL  aCH__SET_STN_R_VALUE;
	CX__VAR_ANALOG_CTRL  aCH__SET_STN_T_VALUE;

	CX__VAR_STRING_CTRL  sCH__Estimated_R_OFFSET;
	CX__VAR_STRING_CTRL  sCH__Estimated_T_OFFSET;

	// CONFIG CHANNEL
	CX__VAR_DIGITAL_CTRL dCH__CFG_WAFER_DRIVER_CHECK;
	CX__VAR_DIGITAL_CTRL dCH__CFG_CPTR_SNS_CHECK;

	CX__VAR_DIGITAL_CTRL dCH__CFG_MACRO_ACTION_PICK_USE;
	CX__VAR_DIGITAL_CTRL dCH__CFG_MACRO_ACTION_PLACE_USE;

	CX__VAR_ANALOG_CTRL  aCH__OTR_IN_CFG__R_OFFSET_HARD_TOLERANCE_LBA_RET;
	CX__VAR_ANALOG_CTRL  aCH__OTR_IN_CFG__T_OFFSET_HARD_TOLERANCE_LBA_RET;
	CX__VAR_ANALOG_CTRL  aCH__OTR_IN_CFG__R_OFFSET_SOFT_TOLERANCE_LBA_RET;
	CX__VAR_ANALOG_CTRL  aCH__OTR_IN_CFG__T_OFFSET_SOFT_TOLERANCE_LBA_RET;

	CX__VAR_ANALOG_CTRL  aCH__OTR_IN_CFG__R_OFFSET_HARD_TOLERANCE_LBB_RET;
	CX__VAR_ANALOG_CTRL  aCH__OTR_IN_CFG__T_OFFSET_HARD_TOLERANCE_LBB_RET;
	CX__VAR_ANALOG_CTRL  aCH__OTR_IN_CFG__R_OFFSET_SOFT_TOLERANCE_LBB_RET;
	CX__VAR_ANALOG_CTRL  aCH__OTR_IN_CFG__T_OFFSET_SOFT_TOLERANCE_LBB_RET;

	CX__VAR_ANALOG_CTRL  aCH__OTR_IN_CFG__LBA_STN_NUM;
	CX__VAR_ANALOG_CTRL  aCH__OTR_IN_CFG__LBB_STN_NUM;
	CX__VAR_ANALOG_CTRL  aCH__OTR_IN_CFG__PMx_STN_NUM[CFG_PMx__SIZE];
	
	//
	CX__VAR_ANALOG_CTRL  aCH__OTR_IN_CFG_PMx_CPTR_SNS_NUM[CFG_PMx__SIZE];
	CX__VAR_ANALOG_CTRL  aCH__OTR_IN_CFG__R_OFFSET_HARD_TOLERANCE_RET[CFG_PMx__SIZE];
	CX__VAR_ANALOG_CTRL  aCH__OTR_IN_CFG__T_OFFSET_HARD_TOLERANCE_RET[CFG_PMx__SIZE];
	CX__VAR_ANALOG_CTRL  aCH__OTR_IN_CFG__R_OFFSET_SOFT_TOLERANCE_RET[CFG_PMx__SIZE];
	CX__VAR_ANALOG_CTRL  aCH__OTR_IN_CFG__T_OFFSET_SOFT_TOLERANCE_RET[CFG_PMx__SIZE];
	
	// HIGH
	// ARM_A
	CX__VAR_ANALOG_CTRL  aCH__OTR_IN_ARM_A_CALED_PMx_CPTR_R_HIGH_SNS_VAL[CFG_PMx__SIZE];
	CX__VAR_ANALOG_CTRL  aCH__OTR_IN_ARM_A_CALED_PMx_CPTR_T_HIGH_SNS_VAL[CFG_PMx__SIZE];
	CX__VAR_ANALOG_CTRL  aCH__OTR_IN_ARM_A_CALED_PMx_CPTR_Z_HIGH_SNS_VAL[CFG_PMx__SIZE];

	// ARM_B
	CX__VAR_ANALOG_CTRL  aCH__OTR_IN_ARM_B_CALED_PMx_CPTR_R_HIGH_SNS_VAL[CFG_PMx__SIZE];
	CX__VAR_ANALOG_CTRL  aCH__OTR_IN_ARM_B_CALED_PMx_CPTR_T_HIGH_SNS_VAL[CFG_PMx__SIZE];
	CX__VAR_ANALOG_CTRL  aCH__OTR_IN_ARM_B_CALED_PMx_CPTR_Z_HIGH_SNS_VAL[CFG_PMx__SIZE];

	// LOW
	// ARM_A
	CX__VAR_ANALOG_CTRL  aCH__OTR_IN_ARM_A_CALED_PMx_CPTR_R_LOW_SNS_VAL[CFG_PMx__SIZE];
	CX__VAR_ANALOG_CTRL  aCH__OTR_IN_ARM_A_CALED_PMx_CPTR_T_LOW_SNS_VAL[CFG_PMx__SIZE];
	CX__VAR_ANALOG_CTRL  aCH__OTR_IN_ARM_A_CALED_PMx_CPTR_Z_LOW_SNS_VAL[CFG_PMx__SIZE];

	// ARM_B
	CX__VAR_ANALOG_CTRL  aCH__OTR_IN_ARM_B_CALED_PMx_CPTR_R_LOW_SNS_VAL[CFG_PMx__SIZE];
	CX__VAR_ANALOG_CTRL  aCH__OTR_IN_ARM_B_CALED_PMx_CPTR_T_LOW_SNS_VAL[CFG_PMx__SIZE];
	CX__VAR_ANALOG_CTRL  aCH__OTR_IN_ARM_B_CALED_PMx_CPTR_Z_LOW_SNS_VAL[CFG_PMx__SIZE];

	// ----------------------------------------- LL1
	// HIGH
	// ARM_A
	CX__VAR_ANALOG_CTRL  aCH__OTR_IN_ARM_A_CALED_LL1_CPTR_R_HIGH_SNS_VAL;
	CX__VAR_ANALOG_CTRL  aCH__OTR_IN_ARM_A_CALED_LL1_CPTR_T_HIGH_SNS_VAL;
	CX__VAR_ANALOG_CTRL  aCH__OTR_IN_ARM_A_CALED_LL1_CPTR_Z_HIGH_SNS_VAL;

	// ARM_B
	CX__VAR_ANALOG_CTRL  aCH__OTR_IN_ARM_B_CALED_LL1_CPTR_R_HIGH_SNS_VAL;
	CX__VAR_ANALOG_CTRL  aCH__OTR_IN_ARM_B_CALED_LL1_CPTR_T_HIGH_SNS_VAL;
	CX__VAR_ANALOG_CTRL  aCH__OTR_IN_ARM_B_CALED_LL1_CPTR_Z_HIGH_SNS_VAL;

	// LOW
	// ARM_A
	CX__VAR_ANALOG_CTRL  aCH__OTR_IN_ARM_A_CALED_LL1_CPTR_R_LOW_SNS_VAL;
	CX__VAR_ANALOG_CTRL  aCH__OTR_IN_ARM_A_CALED_LL1_CPTR_T_LOW_SNS_VAL;
	CX__VAR_ANALOG_CTRL  aCH__OTR_IN_ARM_A_CALED_LL1_CPTR_Z_LOW_SNS_VAL;

	// ARM_B
	CX__VAR_ANALOG_CTRL  aCH__OTR_IN_ARM_B_CALED_LL1_CPTR_R_LOW_SNS_VAL;
	CX__VAR_ANALOG_CTRL  aCH__OTR_IN_ARM_B_CALED_LL1_CPTR_T_LOW_SNS_VAL;
	CX__VAR_ANALOG_CTRL  aCH__OTR_IN_ARM_B_CALED_LL1_CPTR_Z_LOW_SNS_VAL;

	// ----------------------------------------- LL2
	// HIGH
	// ARM_A
	CX__VAR_ANALOG_CTRL  aCH__OTR_IN_ARM_A_CALED_LL2_CPTR_R_HIGH_SNS_VAL;
	CX__VAR_ANALOG_CTRL  aCH__OTR_IN_ARM_A_CALED_LL2_CPTR_T_HIGH_SNS_VAL;
	CX__VAR_ANALOG_CTRL  aCH__OTR_IN_ARM_A_CALED_LL2_CPTR_Z_HIGH_SNS_VAL;

	// ARM_B
	CX__VAR_ANALOG_CTRL  aCH__OTR_IN_ARM_B_CALED_LL2_CPTR_R_HIGH_SNS_VAL;
	CX__VAR_ANALOG_CTRL  aCH__OTR_IN_ARM_B_CALED_LL2_CPTR_T_HIGH_SNS_VAL;
	CX__VAR_ANALOG_CTRL  aCH__OTR_IN_ARM_B_CALED_LL2_CPTR_Z_HIGH_SNS_VAL;

	// LOW
	// ARM_A
	CX__VAR_ANALOG_CTRL  aCH__OTR_IN_ARM_A_CALED_LL2_CPTR_R_LOW_SNS_VAL;
	CX__VAR_ANALOG_CTRL  aCH__OTR_IN_ARM_A_CALED_LL2_CPTR_T_LOW_SNS_VAL;
	CX__VAR_ANALOG_CTRL  aCH__OTR_IN_ARM_A_CALED_LL2_CPTR_Z_LOW_SNS_VAL;

	// ARM_B
	CX__VAR_ANALOG_CTRL  aCH__OTR_IN_ARM_B_CALED_LL2_CPTR_R_LOW_SNS_VAL;
	CX__VAR_ANALOG_CTRL  aCH__OTR_IN_ARM_B_CALED_LL2_CPTR_T_LOW_SNS_VAL;
	CX__VAR_ANALOG_CTRL  aCH__OTR_IN_ARM_B_CALED_LL2_CPTR_Z_LOW_SNS_VAL;

	CX__VAR_ANALOG_CTRL  aCH__OTR_IN_CFG_LL1_CPTR_SNS_NUM;
	CX__VAR_ANALOG_CTRL  aCH__OTR_IN_CFG_LL2_CPTR_SNS_NUM;

	// MATERIAL CHANNEL
	CX__VAR_DIGITAL_CTRL dCH__MON_ARM_A_MATERIAL_STATUS;
	CX__VAR_DIGITAL_CTRL dCH__MON_ARM_B_MATERIAL_STATUS;
	
	CX__VAR_STRING_CTRL  sCH__MON_ARM_A_MATERIAL_TITLE;
	CX__VAR_STRING_CTRL  sCH__MON_ARM_B_MATERIAL_TITLE;
	//

	// ANIMATION CHANNEL
	CX__VAR_DIGITAL_CTRL dCH__OTR_OUT_MON__ACT_ARM;
	CX__VAR_DIGITAL_CTRL dCH__OTR_OUT_MON__ARM_A_ACT;
	CX__VAR_DIGITAL_CTRL dCH__OTR_OUT_MON__ARM_B_ACT;
	CX__VAR_DIGITAL_CTRL dCH__OTR_OUT_MON__TRG_MOVE;
	CX__VAR_DIGITAL_CTRL dCH__OTR_OUT_MON__TRG_ROTATE;

	CX__VAR_DIGITAL_CTRL dCH__OTR_OUT_MON__PMx_ARM_A_ACT[CFG_PMx__SIZE];
	CX__VAR_DIGITAL_CTRL dCH__OTR_OUT_MON__PMx_ARM_B_ACT[CFG_PMx__SIZE];
	//
	CX__VAR_DIGITAL_CTRL dCH__CFG_PMx_DYNAMIC_ALIGN_USE[CFG_PMx__SIZE];
	CX__VAR_DIGITAL_CTRL dCH__CFG_LL1_DYNAMIC_ALIGN_USE;
	CX__VAR_DIGITAL_CTRL dCH__CFG_LL2_DYNAMIC_ALIGN_USE;

	// SIMULATION CHANNEL -----
	CX__VAR_ANALOG_CTRL  aCH__SIM_CFG__ROTATE_TIME;

	// LBx
	CX__VAR_ANALOG_CTRL  aCH__SIM_CFG__LBx_PICK_TIME;
	CX__VAR_ANALOG_CTRL  aCH__SIM_CFG__LBx_PLACE_TIME;

	// PMx
	CX__VAR_ANALOG_CTRL  aCH__SIM_CFG__PMx_PICK_TIME;
	CX__VAR_ANALOG_CTRL  aCH__SIM_CFG__PMx_PLACE_TIME;
	
	// EXTEND, RETRACT
	CX__VAR_ANALOG_CTRL  aCH__SIM_CFG__EXTEND_TIME;
	CX__VAR_ANALOG_CTRL  aCH__SIM_CFG__RETRACT_TIME;

	CX__VAR_STRING_CTRL  sCH__CPTR_SNS_FULL_VAL;
	
	// ----------------- ARM A -------------------------
	// After EXTEND.. HIGH Sns
	CX__VAR_STRING_CTRL  sCH__CPTR_ARM_A_SNS_R_EX_VALUE_HIGH;
	CX__VAR_STRING_CTRL  sCH__CPTR_ARM_A_SNS_T_EX_VALUE_HIGH;
	CX__VAR_STRING_CTRL  sCH__CPTR_ARM_A_SNS_Z_EX_VALUE_HIGH;

	// After EXTEND.. LOW Sns
	CX__VAR_STRING_CTRL  sCH__CPTR_ARM_A_SNS_R_EX_VALUE_LOW;
	CX__VAR_STRING_CTRL  sCH__CPTR_ARM_A_SNS_T_EX_VALUE_LOW;
	CX__VAR_STRING_CTRL  sCH__CPTR_ARM_A_SNS_Z_EX_VALUE_LOW;

	// After RETRACT.. HIGH Sns
	CX__VAR_STRING_CTRL  sCH__CPTR_ARM_A_SNS_R_RE_VALUE_HIGH;
	CX__VAR_STRING_CTRL  sCH__CPTR_ARM_A_SNS_T_RE_VALUE_HIGH;
	CX__VAR_STRING_CTRL  sCH__CPTR_ARM_A_SNS_Z_RE_VALUE_HIGH;

	// After RETRACT.. LOW Sns
	CX__VAR_STRING_CTRL  sCH__CPTR_ARM_A_SNS_R_RE_VALUE_LOW;
	CX__VAR_STRING_CTRL  sCH__CPTR_ARM_A_SNS_T_RE_VALUE_LOW;
	CX__VAR_STRING_CTRL  sCH__CPTR_ARM_A_SNS_Z_RE_VALUE_LOW;
	// ---------

	// ----------------- ARM B -------------------------
	// After EXTEND.. HIGH Sns
	CX__VAR_STRING_CTRL  sCH__CPTR_ARM_B_SNS_R_EX_VALUE_HIGH;
	CX__VAR_STRING_CTRL  sCH__CPTR_ARM_B_SNS_T_EX_VALUE_HIGH;
	CX__VAR_STRING_CTRL  sCH__CPTR_ARM_B_SNS_Z_EX_VALUE_HIGH;

	// After EXTEND.. LOW Sns
	CX__VAR_STRING_CTRL  sCH__CPTR_ARM_B_SNS_R_EX_VALUE_LOW;
	CX__VAR_STRING_CTRL  sCH__CPTR_ARM_B_SNS_T_EX_VALUE_LOW;
	CX__VAR_STRING_CTRL  sCH__CPTR_ARM_B_SNS_Z_EX_VALUE_LOW;

	// After RETRACT.. HIGH Sns
	CX__VAR_STRING_CTRL  sCH__CPTR_ARM_B_SNS_R_RE_VALUE_HIGH;
	CX__VAR_STRING_CTRL  sCH__CPTR_ARM_B_SNS_T_RE_VALUE_HIGH;
	CX__VAR_STRING_CTRL  sCH__CPTR_ARM_B_SNS_Z_RE_VALUE_HIGH;

	// After RETRACT.. LOW Sns
	CX__VAR_STRING_CTRL  sCH__CPTR_ARM_B_SNS_R_RE_VALUE_LOW;
	CX__VAR_STRING_CTRL  sCH__CPTR_ARM_B_SNS_T_RE_VALUE_LOW;
	CX__VAR_STRING_CTRL  sCH__CPTR_ARM_B_SNS_Z_RE_VALUE_LOW;
	// ---------

	CX__VAR_STRING_CTRL  sCH__RQ_STN_FULL_VAL;
	CX__VAR_STRING_CTRL  sCH__RQSTN_SNS_R_VALUE;
	CX__VAR_STRING_CTRL  sCH__RQSTN_SNS_T_VALUE;
	CX__VAR_STRING_CTRL  sCH__RQSTN_SNS_Z_VALUE;

	// Config
	CX__VAR_ANALOG_CTRL  aCH__CFG_ACTION_TIMEOUT_DOWN;
	CX__VAR_ANALOG_CTRL  aCH__CFG_ACTION_TIMEOUT_UP;
	CX__VAR_ANALOG_CTRL  aCH__CFG_ACTION_TIMEOUT_EXTEND;
	CX__VAR_ANALOG_CTRL  aCH__CFG_ACTION_TIMEOUT_GOTO_DA_TIME;
	CX__VAR_ANALOG_CTRL  aCH__CFG_ACTION_TIMEOUT_RETRACT_DA_TIME;
	CX__VAR_ANALOG_CTRL  aCH__CFG_ACTION_TIMEOUT_RETRACT_TIME;
	CX__VAR_ANALOG_CTRL  aCH__CFG_ACTION_TIMEOUT_ROTATE_TIME;

	CX__VAR_ANALOG_CTRL  aCH__CFG_ACTION_TIMEOUT_PICK_DA_TIME;
	CX__VAR_ANALOG_CTRL  aCH__CFG_ACTION_TIMEOUT_PICK_TIME;
	CX__VAR_ANALOG_CTRL  aCH__CFG_ACTION_TIMEOUT_PLACE_TIME;
	CX__VAR_ANALOG_CTRL  aCH__CFG_ACTION_TIMEOUT_PLACE_ADJUST_TIME;

	CX__VAR_DIGITAL_CTRL dCH__CFG_PAD_TYPE;
	CX__VAR_DIGITAL_CTRL dCH__CFG_WAFER_SIZE;

	// SWAP...
	CX__VAR_ANALOG_CTRL aCH__CFG_SWAP_LL_BVL_TIME;
	CX__VAR_ANALOG_CTRL aCH__CFG_SWAP_LL_DA_TIME;
	CX__VAR_ANALOG_CTRL aCH__CFG_SWAP_LL_TIME;
	CX__VAR_ANALOG_CTRL aCH__CFG_SWAP_BVL_NO_Z_TIME;
	CX__VAR_ANALOG_CTRL aCH__CFG_SWAP_BVL_TIME;
	CX__VAR_ANALOG_CTRL aCH__CFG_SWAP_DA_NO_Z_TIME;
	CX__VAR_ANALOG_CTRL aCH__CFG_SWAP_DA_TIME;
	CX__VAR_ANALOG_CTRL aCH__CFG_SWAP_NO_Z_TIME;
	CX__VAR_ANALOG_CTRL aCH__CFG_SWAP_TIME;

	CX__VAR_ANALOG_CTRL aCH__CFG_PM1_ROTATE_TIME;
	CX__VAR_ANALOG_CTRL aCH__CFG_PM2_ROTATE_TIME;
	CX__VAR_ANALOG_CTRL aCH__CFG_PM3_ROTATE_TIME;
	CX__VAR_ANALOG_CTRL aCH__CFG_PM4_ROTATE_TIME;

	CX__VAR_DIGITAL_CTRL dCH__CFG_WFR_SNS_CHECK;

	// IO CHANNEL  -----------------------------------

	// SI
	CX__VAR_STRING_CTRL siCH__VERSION;

	// SO
	CX__VAR_STRING_CTRL  xCH__UPDATE_FLAG;
	CX__VAR_DIGITAL_CTRL xCH__LOCK_FLAG;
	CX__VAR_STRING_CTRL  soCH__HYPER_TERMINAL_CMD;
	CX__VAR_STRING_CTRL  sCH__HYPER_TERMINAL_RECV_MSG;
	CX__VAR_ANALOG_CTRL  aCH__HYPER_TERMINAL_CMD_TIMEOUT;
	CX__VAR_STRING_CTRL  sCH__HYPER_TERMINAL_SEND_MSG;
	CX__VAR_DIGITAL_CTRL dCH__HYPER_TERMINAL_SEND_REQ;

	// AI  ----------
	CX__VAR_ANALOG_CTRL aiCH__ERRID;

	// DO  ----------
	CX__VAR_DIGITAL_CTRL doCH__COMMAND;
	CX__VAR_DIGITAL_CTRL doCH__STN_A;
	CX__VAR_DIGITAL_CTRL doCH__STN_B;
	CX__VAR_DIGITAL_CTRL doCH__CPTR_STN_A;
	CX__VAR_DIGITAL_CTRL doCH__CPTR_STN_B;
	CX__VAR_DIGITAL_CTRL doCH__SLOT;
	CX__VAR_DIGITAL_CTRL doCH__ZPOS;
	CX__VAR_DIGITAL_CTRL doCH__RPOS;
	CX__VAR_DIGITAL_CTRL doCH__ARM_A_MAT;
	CX__VAR_DIGITAL_CTRL doCH__ARM_B_MAT;
	CX__VAR_DIGITAL_CTRL doCH__ARM_WFR_SNS;
	
	// DI  ----------
	CX__VAR_DIGITAL_CTRL diCH__COMM_STS;
	CX__VAR_DIGITAL_CTRL diCH__ARM_A_READ;
	CX__VAR_DIGITAL_CTRL diCH__ARM_B_READ;
	CX__VAR_DIGITAL_CTRL diCH__ARM_A_MAT_READ;
	CX__VAR_DIGITAL_CTRL diCH__ARM_B_MAT_READ;

	// AO  ----------
	CX__VAR_ANALOG_CTRL aoCH_RO_OFFSET;
	CX__VAR_ANALOG_CTRL aoCH_TO_OFFSET;
	CX__VAR_ANALOG_CTRL aoCH_STN_R_VAL;
	CX__VAR_ANALOG_CTRL aoCH_STN_T_VAL;

	// DA Check Count ...
	CX__VAR_ANALOG_CTRL aDA_DA_RETRY_CHECK_COUNT;
	//

	//-------------------------------------------------------------------------
	//  EXTERNAL PROPERTY


	//-------------------------------------------------------------------------
	// Init -----
	CString sMODE__INIT;
	int  Call__INIT(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm);

	int  Fnc__SIM_INIT(CII_OBJECT__VARIABLE* p_variable,
					   CII_OBJECT__ALARM* p_alarm);
	int  Fnc__INIT(CII_OBJECT__VARIABLE* p_variable,
				   CII_OBJECT__ALARM* p_alarm);

	// Pick -----
	CString sMODE__PICK;
	int  Call__PICK(CII_OBJECT__VARIABLE* p_variable,
					CII_OBJECT__ALARM* p_alarm,
					const CString& para__stn_name,
					const CString& para__arm_type,
					const CString& para__stn_slot);

	int  Fnc__SIM_PICK(CII_OBJECT__VARIABLE* p_variable,
					   CII_OBJECT__ALARM* p_alarm,
					   const CString& para__stn_name,
					   const CString& para__arm_type,
					   const CString& para__stn_slot);
	int  Fnc__PICK(CII_OBJECT__VARIABLE* p_variable,
				   CII_OBJECT__ALARM* p_alarm,
				   const CString& para__stn_name,
				   const CString& para__arm_type,
				   const CString& para__stn_slot);
	
	int  Fnc__PICK_DYNAMIC_ALIGN(CII_OBJECT__VARIABLE* p_variable,
								 CII_OBJECT__ALARM* p_alarm,
								 const CString& stn_name,
								 const CString& arm_type,
								 const CString& stn_slot,
								 const int is_act_handoff);

	int  Fnc__PICK_DYNAMIC_ALIGN_N(CII_OBJECT__VARIABLE* p_variable,
								   CII_OBJECT__ALARM* p_alarm,
								   const CString& stn_name,
								   const CString& arm_type,
								   const CString& stn_slot,
								   const int is_act_handoff);

	// Place -----
	CString sMODE__PLACE;
	int  Call__PLACE(CII_OBJECT__VARIABLE* p_variable,
					 CII_OBJECT__ALARM* p_alarm,
					 const CString& para__stn_name,
					 const CString& para__arm_type,
					 const CString& para__stn_slot);
	int  _Call__PLACE(CII_OBJECT__VARIABLE* p_variable,
					  CII_OBJECT__ALARM* p_alarm,
					  const CString& para__stn_name,
					  const CString& para__arm_type,
					  const CString& para__stn_slot);

	int  Fnc__SIM_PLACE(CII_OBJECT__VARIABLE* p_variable,
						CII_OBJECT__ALARM* p_alarm,
						const CString& para__stn_name,
						const CString& para__arm_type,
						const CString& para__stn_slot);
	int  Fnc__PLACE(CII_OBJECT__VARIABLE* p_variable,
					CII_OBJECT__ALARM* p_alarm,
					const CString& para__stn_name,
					const CString& para__arm_type,
					const CString& para__stn_slot);
	int  _Act__PLACE(CII_OBJECT__VARIABLE* p_variable,
					 CII_OBJECT__ALARM* p_alarm,
					 const CString& para__stn_name,
					 const CString& para__arm_type,
					 const CString& para__stn_slot);

	int  Fnc__PLACE_DYNAMIC_ALIGN(CII_OBJECT__VARIABLE* p_variable,
								  CII_OBJECT__ALARM* p_alarm,
								  const CString& stn_name,
								  const CString& before_stn_name,
								  const CString& arm_type,
								  const CString& stn_slot,
								  const int is_act_handoff);
	
	int  Fnc__PLACE_DYNAMIC_ALIGN_N(CII_OBJECT__VARIABLE* p_variable,
									CII_OBJECT__ALARM* p_alarm,
									const CString& stn_name,
									const CString& before_stn_name,
									const CString& arm_type,
									const CString& stn_slot,
									const int is_act_handoff,
									const CString& da_usage);

	// Rotate -----
	CString sMODE__ROTATE;
	int  Call__ROTATE(CII_OBJECT__VARIABLE* p_variable,
					  CII_OBJECT__ALARM* p_alarm,
					  const CString& para__stn_name, 
					  const CString& para__arm_type,
					  const CString& para__stn_slot);

	int  Fnc__SIM_ROTATE(CII_OBJECT__VARIABLE* p_variable,
						 CII_OBJECT__ALARM* p_alarm,
						 const CString& para__stn_name, 
						 const CString& para__arm_type,
						 const CString& para__stn_slot);
	int  Fnc__ROTATE(CII_OBJECT__VARIABLE* p_variable,
					 CII_OBJECT__ALARM* p_alarm,
					 const CString& para__stn_name, 
					 const CString& para__arm_type,
					 const CString& para__stn_slot);

	// Home -----
	CString sMODE__HOME;
	int  Call__HOME(CII_OBJECT__VARIABLE* p_variable,
					CII_OBJECT__ALARM* p_alarm);

	int  Fnc__SIM_HOME(CII_OBJECT__VARIABLE* p_variable,
					   CII_OBJECT__ALARM* p_alarm);
	int  Fnc__HOME(CII_OBJECT__VARIABLE* p_variable,
				   CII_OBJECT__ALARM* p_alarm);

	// Map -----
	CString sMODE__MAP;
	int  Call__MAP(CII_OBJECT__VARIABLE* p_variable,
				   CII_OBJECT__ALARM* p_alarm);

	int  Fnc__SIM_MAP(CII_OBJECT__VARIABLE* p_variable,
				      CII_OBJECT__ALARM* p_alarm);
	int  Fnc__MAP(CII_OBJECT__VARIABLE* p_variable,
				  CII_OBJECT__ALARM* p_alarm);

	// Extend -----
	CString sMODE__EXTEND;

	int  Call__EXTEND(CII_OBJECT__VARIABLE* p_variable,
					 CII_OBJECT__ALARM* p_alarm,
					 const CString& para__stn_name, 
					 const CString& para__arm_type,
					 const CString& para__stn_slot,
					 const int is_active_handoff);

	int  Fnc__EXTEND(CII_OBJECT__VARIABLE* p_variable,
					 CII_OBJECT__ALARM* p_alarm,
					 const CString& para__stn_name, 
					 const CString& para__arm_type,
					 const CString& para__stn_slot,
					 const int is_active_handoff);

	int  Fnc__EXTEND_OFFSET(CII_OBJECT__VARIABLE* p_variable,
							CII_OBJECT__ALARM* p_alarm,
							const CString& para__stn_name, 
							const CString& para__arm_type,
							const CString& para__stn_slot,
							const int is_active_handoff);

	int  Fnc__EXTEND_DYNAMIC_ALIGN(CII_OBJECT__VARIABLE* p_variable,
								   CII_OBJECT__ALARM* p_alarm,
								   const CString& para__stn_name, 
								   const CString& para__arm_type,
								   const CString& para__stn_slot,
								   const int is_active_handoff);

	int  Fnc__EXTEND_DYNAMIC_ALIGN_N(CII_OBJECT__VARIABLE* p_variable,
									 CII_OBJECT__ALARM* p_alarm,
									 const CString& para__stn_name, 
									 const CString& para__arm_type,
									 const CString& para__stn_slot,
								     const int is_active_handoff,
									 const CString& da_usage);

	int  Fnc__RETRACT_DYNAMIC_ALIGN(CII_OBJECT__VARIABLE* p_variable,
								    CII_OBJECT__ALARM* p_alarm,
								    const CString& para__stn_name, 
								    const CString& para__arm_type,
								    const CString& para__stn_slot,
								    const int is_active_handoff);

	int  Fnc__RETRACT_DYNAMIC_ALIGN_N(CII_OBJECT__VARIABLE* p_variable,
									  CII_OBJECT__ALARM* p_alarm,
									  const CString& para__stn_name, 
									  const CString& para__arm_type,
									  const CString& para__stn_slot,
								      const int is_active_handoff);

	int  Fnc__WAFER_CHECK_ON_ARM(const CString& para__arm_type, 
								 const bool active__act_pick,
								 const bool active__act_retract);

	// Retract -----
	CString sMODE__RETRACT;

	int  Call__RETRACT(CII_OBJECT__VARIABLE* p_variable,
					 CII_OBJECT__ALARM* p_alarm,
					 const CString& para__stn_name, 
					 const CString& para__arm_type,
					 const CString& para__stn_slot,
					 const int is_active_handoff);

	int  Fnc__RETRACT(CII_OBJECT__VARIABLE* p_variable,
					 CII_OBJECT__ALARM* p_alarm,
					 const CString& para__stn_name, 
					 const CString& para__arm_type,
					 const CString& para__stn_slot,
					 const int is_active_handoff);

	// Goup -----
	CString sMODE__GOUP;

	int  Call__GOUP(CII_OBJECT__VARIABLE* p_variable,
					 CII_OBJECT__ALARM* p_alarm,
					 const CString& para__stn_name, 
					 const CString& para__arm_type,
					 const CString& para__stn_slot);

	int  Fnc__GOUP(CII_OBJECT__VARIABLE* p_variable,
					 CII_OBJECT__ALARM* p_alarm,
					 const CString& para__stn_name, 
					 const CString& para__arm_type,
					 const CString& para__stn_slot);

	// GoDown -----
	CString sMODE__GODOWN;

	int  Call__GODOWN(CII_OBJECT__VARIABLE* p_variable,
					 CII_OBJECT__ALARM* p_alarm,
					 const CString& para__stn_name, 
					 const CString& para__arm_type,
					 const CString& para__stn_slot);

	int  Fnc__GODOWN(CII_OBJECT__VARIABLE* p_variable,
					 CII_OBJECT__ALARM* p_alarm,
					 const CString& para__stn_name, 
					 const CString& para__arm_type,
					 const CString& para__stn_slot);

	// 
	CString sMODE__RQ_STN;
	int  Call__RQ_STN(CII_OBJECT__VARIABLE* p_variable,
					 CII_OBJECT__ALARM* p_alarm,
					 const CString& para__stn_name, 
					 const CString& para__arm_type);
					 
	int  Fnc__RQ_STN(CII_OBJECT__VARIABLE* p_variable,
					 CII_OBJECT__ALARM* p_alarm,
					 const CString& stn_name,
					 const CString& arm_type);

	CString sMODE__SET_STN;
	int  Call__SET_STN(CII_OBJECT__VARIABLE* p_variable,
					   CII_OBJECT__ALARM* p_alarm,
					   const CString& para__stn_name, 
					   const CString& para__arm_type);

	int  Fnc__SET_STN(CII_OBJECT__VARIABLE* p_variable,
					  CII_OBJECT__ALARM* p_alarm,
					  const CString& stn_name,
					  const CString& arm_type);

	CString sMODE__TEACHED_CPTR_SAVE;
	int  Call__TEACHED_CPTR_SAVE(CII_OBJECT__VARIABLE* p_variable,
     						     CII_OBJECT__ALARM* p_alarm,
							     const CString& para__stn_name, 
							     const CString& para__arm_type);

	// Swap -----
	CString sMODE__SWAP;
	int  Call__SWAP(CII_OBJECT__VARIABLE* p_variable,
					CII_OBJECT__ALARM* p_alarm,
					const CString& para__stn_name,
					const CString& para__stn_arm,
					const CString& para__stn_slot);

	int  Fnc__SIM_SWAP(CII_OBJECT__VARIABLE* p_variable,
					   CII_OBJECT__ALARM* p_alarm,
					   const CString& para__stn_name,
					   const CString& para__stn_arm,
					   const CString& para__stn_slot);
	int  Fnc__SWAP(CII_OBJECT__VARIABLE* p_variable,
				   CII_OBJECT__ALARM* p_alarm,
				   const CString& para__stn_name,
				   const CString& para__stn_arm,
				   const CString& para__stn_slot);

	// ...
	void Mon__ANI_MONITOR(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);
	void Mon__IO_MONITOR(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);
	void Mon__HYPER_TERMINAL_SEND(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm);

	// ...
	int  Is__LOCAL(CII_OBJECT__ALARM* p_alarm);
	int  Is__OFFLINE(CII_OBJECT__ALARM* p_alarm);
	int  Is__ERROR_EXIST(CII_OBJECT__ALARM* p_alarm);
	
	int  Fnc__calculation_CPTR_SNS_VAL(const CString& stn_name,
									   const CString& arm_type,
									   CPTR_TUNED_VAL& ro_to_tune_val);

	int  Fnc__calculation_CPTR_SNS_VAL_N(const CString& stn_name,
									   const CString& arm_type,
									   CPTR_TUNED_VAL& ro_to_tune_val);

	int  Get__CALED_CPTR_SNS_VAL(const CString& stn_name,
								const CString& arm_type,
								double& r_hi, double& t_hi, double& z_hi,
								double& r_lo, double& t_lo, double& z_lo);

	int  Init__CALED_CPTR_SNS_VAL_CH(const CString& stn_name, const CString& arm_type);
	int  Save__CALED_CPTR_SNS_VAL(const CString& stn_name,
								const CString& arm_type,
								double& r_hi, double& t_hi, double& z_hi,
								double& r_lo, double& t_lo, double& z_lo);

	CString	Get__CFG_STN_NUM(const CString& stn_name);	
	CString	Get__CFG_DYNAMIC_ALGN_USE(const CString& stn_name);	
	CString Get__CFG_STN_CPTR_NUM(const CString& stn_name);
	int Get__CFG_STN_OFFSET_RANGE_RET(const CString& stn_name, 
									  CString& str_r_soft_tol, CString& str_r_hard_tol,
									  CString& str_t_soft_tol, CString& str_t_hard_tol);

	// ...
	int  Drv__CMD_COM_INIT(const CString& var_name);

	int	 Drv__SEND_MESSAGE_RETURN(const CString& var_name, 
							      CString str__send_data, 
								  const CString& str__for_error);

	// ...
	int Set__CPTR_SNS_OFF_ON(CII_OBJECT__VARIABLE* p_variable,
							 CII_OBJECT__ALARM* p_alarm,
		                     const CString& arm_type,
		                     const CString& stn_name);

	int Set__CPTR_SNS_OFF_N(CII_OBJECT__VARIABLE* p_variable,
						  CII_OBJECT__ALARM* p_alarm,
						  const CString& arm_type,
		                  const CString& stn_name);

	int Set__CPTR_SNS_ON_N(CII_OBJECT__VARIABLE* p_variable,
						 CII_OBJECT__ALARM* p_alarm,
						 const CString& arm_type,
		                 const CString& stn_name);

	int Get__CPTR_SNS(CII_OBJECT__VARIABLE* p_variable,
					  CII_OBJECT__ALARM* p_alarm,
		              const CString& arm_type,
		              const CString& stn_name,
					  const CString& act_type);

	int Get__CPTR_SNS_N(CII_OBJECT__VARIABLE* p_variable,
						CII_OBJECT__ALARM* p_alarm,
						const CString& arm_type,
						const CString& stn_name,
						const CString& act_type);

	void Calculation_Sns_Val(double caled_r_hi, double caled_t_hi, double caled_z_hi,
							 double caled_r_lo, double caled_t_lo, double caled_z_lo,
							 double r_hi, double t_hi, double z_hi,
							 double r_lo, double t_lo, double z_lo,
							 double &r_tune_val,
							 double &t_tune_val, 
							 const CString& stn_name);

	void Calculation_Sns_Val_N(double caled_r_hi, double caled_t_hi, double caled_z_hi,
							   double caled_r_lo, double caled_t_lo, double caled_z_lo,
							   double r_hi, double t_hi, double z_hi,
							   double r_lo, double t_lo, double z_lo,
							   double &r_tune_val,
							   double &t_tune_val, 
							   const CString& stn_name);

	int Check_Offset_Range(const CString& stn_name, 
						   CII_OBJECT__VARIABLE* p_variable,
						   CII_OBJECT__ALARM* p_alarm,
						   const CPTR_TUNED_VAL set_cptr_tune_val);
	
	void Create_Directory(CString &dirPath);

	// ...
	void Fnc__APP_LOG(const CString& log_msg);
	void Fnc__DRV_LOG(const CString& log_msg);
	void Fnc__ACT_MSG(const CString& act_msg);		// sCH__ACT_MSG

	// ...
	void Register__ALARM_INF(CII_DEFINE__ALARM* p_alarm);
	//


public:
	CObj__VAC_MAGATRAN();
	~CObj__VAC_MAGATRAN();

	//-------------------------------------------------------------------------
	int __DEFINE__CONTROL_MODE(obj,l_mode);
	int __DEFINE__VERSION_HISTORY(version);

	int __DEFINE__VARIABLE_STD(p_variable);
	int __DEFINE__VARIABLE_IO(p_io_variable);

	int __DEFINE__ALARM(p_alarm);

	int __Define__USER_FUNCTION(CII_DEFINE__FUNCTION *p_fnc_ctrl);

	//-------------------------------------------------------------------------
	int __INITIALIZE__OBJECT(p_variable,p_ext_obj_create);	
	int __INITIALIZE__IO(p_io_para);

	//-------------------------------------------------------------------------
	int __CALL__CONTROL_MODE(mode,p_debug,p_variable,p_alarm);
	int __CALL__MONITORING(id,p_variable,p_alarm);

	//-------------------------------------------------------------------------
	int __Read__ANALOG(const CString& var_name, const CDS_IO__CHANNEL_INFO& io_info, double& read_data);
	int __Read__DIGITAL(const CString& var_name,const CDS_IO__CHANNEL_INFO& io_info, CString& read_data,int& item_index);
	int __Read__STRING(const CString& var_name, const CDS_IO__CHANNEL_INFO& io_info, CString& read_data);

	int __Write__ANALOG(const CString& var_name, const CDS_IO__CHANNEL_INFO& io_info, const double set_data);
	int __Write__DIGITAL(const CString& var_name,const CDS_IO__CHANNEL_INFO& io_info, const CString& set_data,const int item_index);
	int __Write__STRING(const CString& var_name, const CDS_IO__CHANNEL_INFO& io_info, const CString& set_data);
};
