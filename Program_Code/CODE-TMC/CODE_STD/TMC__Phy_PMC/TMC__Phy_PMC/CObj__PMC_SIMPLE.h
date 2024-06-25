#pragma once

#include "Interface_Code.h"

#include "CCommon_System.h"
#include "CCommon_Error.h"


#define _DEF__PMx_SLOT_VLV_TYPE__IO				1
#define _DEF__PMx_SLOT_VLV_TYPE__OBJ			2


class CObj__PMC_SIMPLE : public __IOBJ__STD_TYPE
{
private:
	//-------------------------------------------------------------------------
	CString sObject_Name;

	// ...
	int iPM_LIMIT;

	SCX__USER_LOG_CTRL xLOG_CTRL;
	int iActive__SIM_MODE;
	//

	//-------------------------------------------------------------------------
	// INTERNAL PROPERTY

	// OBJ ...
	CX__VAR_STRING_CTRL  sCH__OBJ_ACTIVE_MODE;

	// PARA ...
	CX__VAR_ANALOG_CTRL  aCH__PARA_PMC_ID;

	// CFG ...
	CX__VAR_DIGITAL_CTRL dCH__CFG_IO_OFF_MODE;

	//
	CX__VAR_DIGITAL_CTRL dCH__CFG_HANDSHAKE_MODE[CFG_PMx__SIZE];

	//
	CX__VAR_ANALOG_CTRL  aCH__CFG_VALVE_OPEN_TIMEOUT[CFG_PMx__SIZE];
	CX__VAR_ANALOG_CTRL  aCH__CFG_VALVE_CLOSE_TIMEOUT[CFG_PMx__SIZE];

	//
	CX__VAR_DIGITAL_CTRL dCH__CFG_USE_SHUTTER[CFG_PMx__SIZE];
	CX__VAR_DIGITAL_CTRL dCH__CFG_USE_LIFT_PIN[CFG_PMx__SIZE];
	CX__VAR_DIGITAL_CTRL dCH__CFG_USE_CR_POSITION[CFG_PMx__SIZE];

	// 
	CX__VAR_ANALOG_CTRL  aCH__PRESSURE_TORR[CFG_PMx__SIZE];
	CX__VAR_STRING_CTRL  sCH__PRESSURE_STATUS[CFG_PMx__SIZE];

	CX__VAR_DIGITAL_CTRL dCH__SLIT_VALVE_STATUS[CFG_PMx__SIZE];
	CX__VAR_STRING_CTRL  sCH__CHUCK_STATUS[CFG_PMx__SIZE];
	CX__VAR_STRING_CTRL  sCH__LIFT_PIN_STATUS[CFG_PMx__SIZE];
	CX__VAR_STRING_CTRL  sCH__CR_POSITION_STS[CFG_PMx__SIZE];

	//
	CX__VAR_DIGITAL_CTRL dCH__SLOT01_STATUS[CFG_PMx__SIZE];
	CX__VAR_STRING_CTRL  sCH__SLOT01_TITLE[CFG_PMx__SIZE];

	CX__VAR_DIGITAL_CTRL dCH__PRC_STATUS[CFG_PMx__SIZE];
	CX__VAR_DIGITAL_CTRL dCH__PRESS_STATUS[CFG_PMx__SIZE];

	//
	CX__VAR_DIGITAL_CTRL dCH__CYCLE_TEST_USE_LIFT_PIN_DOWN_PM_X[CFG_PMx__SIZE];
	
	CX__VAR_STRING_CTRL  sCH__CYCLE_TEST_CFG_PMC_ID;
	CX__VAR_STRING_CTRL  sCH__CYCLE_TEST_CFG_SV_CLOSE_MODE;

	CX__VAR_ANALOG_CTRL  aCH__CYCLE_TEST_CFG_CYCLE_COUNT;
	CX__VAR_STRING_CTRL  sCH__CYCLE_TEST_CUR_CYCLE_COUNT;

	CX__VAR_ANALOG_CTRL  aCH__CYCLE_TEST_CFG_DELAY_SEC;
	CX__VAR_STRING_CTRL  sCH__CYCLE_TEST_CUR_DELAY_SEC;

	CX__VAR_STRING_CTRL  sCH__CYCLE_TEST_MSG;

	CX__VAR_STRING_CTRL  sCH__CYCLE_TEST_OP_TIME_CUR;
	CX__VAR_STRING_CTRL  sCH__CYCLE_TEST_OP_TIME_AVG;
	CX__VAR_STRING_CTRL  sCH__CYCLE_TEST_OP_TIME_MIN;
	CX__VAR_STRING_CTRL  sCH__CYCLE_TEST_OP_TIME_MAX;

	CX__VAR_STRING_CTRL  sCH__CYCLE_TEST_CL_TIME_CUR;
	CX__VAR_STRING_CTRL  sCH__CYCLE_TEST_CL_TIME_AVG;
	CX__VAR_STRING_CTRL  sCH__CYCLE_TEST_CL_TIME_MIN;
	CX__VAR_STRING_CTRL  sCH__CYCLE_TEST_CL_TIME_MAX;

	// ...
	SCX__ASYNC_TIMER_CTRL xI_ASYNC_TIMER;
	CX__VAR_ANALOG_CTRL  aCH__TIME_COUNT;
	
	CX__VAR_ANALOG_CTRL  aCH_SV_OPEN__TIME_COUNT[CFG_PMx__SIZE];
	CX__VAR_ANALOG_CTRL  aCH_SV_CLOSE__TIME_COUNT[CFG_PMx__SIZE];
	CX__VAR_ANALOG_CTRL  aCH_SV_UP__TIME_COUNT[CFG_PMx__SIZE];
	CX__VAR_ANALOG_CTRL  aCH_SV_DOWN__TIME_COUNT[CFG_PMx__SIZE];
	
	CX__VAR_STRING_CTRL  sCH_SHUTTER_STS[CFG_PMx__SIZE];
	CX__VAR_STRING_CTRL  sCH_LIFT_PIN_POS_STS[CFG_PMx__SIZE];
	CX__VAR_STRING_CTRL  sCH_CR_POSITION_STS[CFG_PMx__SIZE];

	CX__VAR_ANALOG_CTRL  aCH__CFG_MAX_DIFF_PRESS[CFG_PMx__SIZE];

	CX__VAR_STRING_CTRL  sCH__ATM_SNS[CFG_PMx__SIZE];
	CX__VAR_STRING_CTRL  sCH__VAC_SNS[CFG_PMx__SIZE];

	// TAS Channel ...
	CX__VAR_STRING_CTRL  sCH__TAS_RESET_REQ__PMx[CFG_PMx__SIZE];
	CX__VAR_STRING_CTRL  sCH__TAS_ACTION_TIME_NOW__PMx[CFG_ACT__SIZE][CFG_PMx__SIZE];
	CX__VAR_STRING_CTRL  sCH__TAS_ACTION_TIME_MIN__PMx[CFG_ACT__SIZE][CFG_PMx__SIZE];
	CX__VAR_STRING_CTRL  sCH__TAS_ACTION_TIME_MAX__PMx[CFG_ACT__SIZE][CFG_PMx__SIZE];
	CX__VAR_STRING_CTRL  sCH__TAS_ACTION_TIME_AVG__PMx[CFG_ACT__SIZE][CFG_PMx__SIZE];
	CX__VAR_STRING_CTRL  sCH__TAS_ACTION_TIME_AVG_F__PMx[CFG_ACT__SIZE][CFG_PMx__SIZE];
	CX__VAR_ANALOG_CTRL  aCH__TAS_ACTION_TIME_CNT__PMx[CFG_ACT__SIZE][CFG_PMx__SIZE];
	//


	//-------------------------------------------------------------------------
	// EXTERNAL PROPERTY

	// DB OBJECT -----
	CX__VAR_DIGITAL_CTRL dEXT_CH__CFG_TRANSFER_MODE;

	CX__VAR_DIGITAL_CTRL dEXT_CH__CFG_PMx_EXIST_FLAG[CFG_PMx__SIZE];
	CX__VAR_DIGITAL_CTRL dEXT_CH__CFG_PMx_SV_USE[CFG_PMx__SIZE];
	CX__VAR_DIGITAL_CTRL dEXT_CH__CFG_PMx_CHM_TYPE[CFG_PMx__SIZE];

	CX__VAR_ANALOG_CTRL  aEXT_CH__CFG_SIM_SLOT_VLV_CLOSE_TIME;
	CX__VAR_ANALOG_CTRL  aEXT_CH__CFG_SIM_SLOT_VLV_OPEN_TIME;

	CX__VAR_DIGITAL_CTRL dEXT_CH__SCH_TEST_CFG_PM_DUMMY_BY_CTC;

	// PHY_TM OBJECT -----
	CX__VAR_DIGITAL_CTRL dEXT_CH__PHY_TM__PRESS_STS;
	CX__VAR_ANALOG_CTRL  aEXT_CH__PHY_TM__PRESS_TORR;

	// PMx_SLOT_VLV.CTRL_TYPE ...
	int iDATA__PMx_SLOT_VLV_CTRL_TYPE;
	
	CII_EXT_OBJECT__CTRL* pOBJ_CTRL__PMx_HANDOFF;
	CString sMODE__PMx_SLOT_VLV__OPEN;
	CString sMODE__PMx_SLOT_VLV__CLOSE;

	CX__VAR_ANALOG_CTRL  aEXT_CH__PARA_PMC_ID;

	CX__VAR_DIGITAL_CTRL dEXT_CH__PMx_SLOT_VLV__OBJ_STATUS_X[CFG_PMx__SIZE];

	// I/O OBJECT -----
	CX__VAR_DIGITAL_CTRL diEXT_CH__DNET_COMM_STS;

	//
	bool bActive__PMx__SV_OPEN_X[CFG_PMx__SIZE];
	CX__VAR_DIGITAL_CTRL doEXT_CH__PMx__SV_OPEN_X[CFG_PMx__SIZE];

	bool bActive__PMx__SV_CLOSE_X[CFG_PMx__SIZE];
	CX__VAR_DIGITAL_CTRL doEXT_CH__PMx__SV_CLOSE_X[CFG_PMx__SIZE];

	//
	CX__VAR_DIGITAL_CTRL diEXT_CH__PMx__SV_OPEN[CFG_PMx__SIZE];
	CX__VAR_DIGITAL_CTRL diEXT_CH__PMx__SV_CLOSE[CFG_PMx__SIZE];

	CX__VAR_DIGITAL_CTRL diEXT_CH__PMx__ATM_SNS[CFG_PMx__SIZE];
	CX__VAR_DIGITAL_CTRL diEXT_CH__PMx__VAC_SNS[CFG_PMx__SIZE];

	//
	int iSIZE_PMx__WAFER_OUT_X[CFG_PMx__SIZE];
	CX__VAR_DIGITAL_CTRL diEXT_CH__PMx__WAFER_OUT_XY[CFG_PMx__SIZE][CFG_WAFER_OUT__SIZE];

	//
	bool bActive__VAC_RB__RNE_PM_X[CFG_PMx__SIZE];
	CX__VAR_DIGITAL_CTRL diEXT_CH__VAC_RB__RNE_PM_X[CFG_PMx__SIZE];

	// ATM & VAC STATE ...
	CString sDATA__ATM_ON;
	CString sDATA__ATM_OFF;

	CString sDATA__VAC_ON;
	CString sDATA__VAC_OFF;

	// ARM_RNE.STATE ...
	CString sDATA__RNE_ON;
	CString sDATA__RNE_OFF;
	//


	//------------------------------------------------------------------------------
	// INIT -----
	CString sMODE__INIT;
	int  Call__INIT(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);
	
	int  Check__PRESSURE_ATM(CII_OBJECT__ALARM* p_alarm, const int pmc_id,const int alarm_id);
	int  Check__PRESSURE_VAC(CII_OBJECT__ALARM* p_alarm, const int pmc_id,const int alarm_id);

	int  Check__SAME_PRESSURE_FOR_VAC_Or_ATM(CII_OBJECT__ALARM* p_alarm,const int pmc_id, const int alarm_id,const CString& act_msg);

	int  Is__CLOSE_CONDITION(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const int pmc_id);

	int  Is__SV_OPEN(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const int pmc_id);
	int  Is__SV_CLOSE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const int pmc_id);

	int  Set_IO__SV_OPEN(const int pmc_id);
	int  End_IO__SV_OPEN(const int pmc_id);

	int  Set_IO__SV_CLOSE(const int pmc_id);
	int  End_IO__SV_CLOSE(const int pmc_id);

	//
	CString sMODE__INIT_SV_CLOSE;
	int  Call__INIT_SV_CLOSE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const int pmc_id);

	// CHECK SLIT VLV ALL CLOSE -----
	CString sMODE__CHECK_SV_ALL_CLOSE;
	int  Call__CHECK_SV_ALL_CLOSE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);


	// SLIT VLV OPEN -----
	CString sMODE__SV_OPEN;
	int  Call__SV_OPEN(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const int pmc_id);

	int  _Call__SV_OPEN(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const int pmc_id);
	int  Fnc__SV_OPEN(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const int pmc_id,const double cfg_timeout);

	//
	CString sMODE__SV_CLOSE;
	CString sMODE__SV_ALL_CLOSE;
	int  Call__SV_CLOSE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const int pmc_id);
	int  _Call__SV_CLOSE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const int pmc_id, const int alarm_check);

	int  Fnc__SV_CLOSE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const int pmc_id,const double cfg_timeout);

	int  _Update__ACTION_MIN_MAX(const int db_i,const int pm_i, const double cur_sec);


	// CYCLE TEST ...
	CString sMODE__CYCLE_TEST;
	int  Call__CYCLE_TEST(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	CString Get__AVG_DATA(const int n_count,const double cur_data, const double pre_avg);

	void Fnc__Check_MIN_MAX_DATA(const int n_count,CII__VAR_STRING_CTRL* p_ch_min, CII__VAR_STRING_CTRL* p_ch_max,const double cur_data);


	// TIME TEST ...
	CString sMODE__TIME_TEST;
	int  Call__TIME_TEST(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);
	//

	//------------------------------------------------------------------------------
	void Mon__IO_MONITOR(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	void Update__IO_MONITOR();
	void Fnc_App_Log(const CString str_log);
	//


public:
	CObj__PMC_SIMPLE();
	~CObj__PMC_SIMPLE();

	//-------------------------------------------------------------------------
	int __DEFINE__CONTROL_MODE(obj,l_mode);

	int __DEFINE__VERSION_HISTORY(version);

	int __DEFINE__VARIABLE_STD(p_variable);
	int __DEFINE__ALARM(p_alarm);

	//-------------------------------------------------------------------------
	int __INITIALIZE__OBJECT(p_variable,p_ext_obj_create);

	//-------------------------------------------------------------------------
	int __CALL__CONTROL_MODE(mode,p_debug,p_variable,p_alarm);
	int __CALL__MONITORING(id,p_variable,p_alarm);
};
