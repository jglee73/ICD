#pragma once

#include "Interface_Code.h"

#include "CCommon_System.h"
#include "CCommon_Error.h"


class CObj__PMC_STD : public __IOBJ__STD_TYPE
{
private:
	//-------------------------------------------------------------------------
	CString sObject_Name;
	int m_nPM_LIMIT;

	SCX__USER_LOG_CTRL xLOG_CTRL;
	int iSim_Flag;


	//-------------------------------------------------------------------------
	// INTERNAL PROPERTY

	CX__VAR_STRING_CTRL  sCH__OBJ_ACTIVE_MODE;

	//
	CX__VAR_ANALOG_CTRL  aCH__PARA_PMC_ID;

	//
	CX__VAR_DIGITAL_CTRL dCH__CFG_HANDSHAKE_MODE[CFG_PMx__SIZE];
	CX__VAR_DIGITAL_CTRL dCH__CFG_USE_SHUTTER[CFG_PMx__SIZE];
	CX__VAR_DIGITAL_CTRL dCH__CFG_USE_LIFT_PIN[CFG_PMx__SIZE];
	CX__VAR_DIGITAL_CTRL dCH__CFG_USE_CR_POSITION[CFG_PMx__SIZE];

	//
	CX__VAR_ANALOG_CTRL  aCH__CFG_VALVE_OPEN_TIMEOUT[CFG_PMx__SIZE];
	CX__VAR_ANALOG_CTRL  aCH__CFG_VALVE_CLOSE_TIMEOUT[CFG_PMx__SIZE];

	// 
	CX__VAR_STRING_CTRL  sCH__OBJ_STATUS[CFG_PMx__SIZE];
	CX__VAR_ANALOG_CTRL  aCH__PRESSURE_TORR[CFG_PMx__SIZE];
	CX__VAR_STRING_CTRL  sCH__PRESSURE_STATUS[CFG_PMx__SIZE];

	CX__VAR_DIGITAL_CTRL dCH__SLIT_VALVE_STATUS[CFG_PMx__SIZE];
	CX__VAR_STRING_CTRL  sCH__CHUCK_STATUS[CFG_PMx__SIZE];
	CX__VAR_STRING_CTRL  sCH__LIFT_PIN_STATUS[CFG_PMx__SIZE];

	//
	CX__VAR_DIGITAL_CTRL dCH__SLOT01_STATUS[CFG_PMx__SIZE];
	CX__VAR_STRING_CTRL  sCH__SLOT01_TITLE[CFG_PMx__SIZE];

	CX__VAR_DIGITAL_CTRL dCH__PRC_STATUS[CFG_PMx__SIZE];
	CX__VAR_DIGITAL_CTRL dCH__PRESS_STATUS[CFG_PMx__SIZE];

	//
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
	CX__VAR_ANALOG_CTRL   aCH__TIME_COUNT;
	CX__VAR_ANALOG_CTRL   aCH_SV_OPEN__TIME_COUNT[CFG_PMx__SIZE];
	CX__VAR_ANALOG_CTRL   aCH_SV_CLOSE__TIME_COUNT[CFG_PMx__SIZE];
	CX__VAR_ANALOG_CTRL   aCH_SV_UP__TIME_COUNT[CFG_PMx__SIZE];
	CX__VAR_ANALOG_CTRL   aCH_SV_DOWN__TIME_COUNT[CFG_PMx__SIZE];
	CX__VAR_STRING_CTRL   sCH_SHUTTER_STS[CFG_PMx__SIZE];
	CX__VAR_STRING_CTRL   sCH_CR_POSITION_STS[CFG_PMx__SIZE];
	CX__VAR_STRING_CTRL   sCH_LIFT_PIN_POS_STS[CFG_PMx__SIZE];
	CX__VAR_ANALOG_CTRL   aCH__CFG_MAX_DIFF_PRESS[CFG_PMx__SIZE];
	//


	//-------------------------------------------------------------------------
	// EXTERNAL PROPERTY

	// DB OBJ ...
	CX__VAR_DIGITAL_CTRL dEXT_CH__CFG_SETUP_TEST_MODE;
	CX__VAR_DIGITAL_CTRL dEXT_CH__CFG_SETUP_AUTO_VENTING_PUMPING;
	CX__VAR_DIGITAL_CTRL dEXT_CH__CFG_SETUP_LLx_DOOR_SLOT_VLV_CONTROL;

	CX__VAR_DIGITAL_CTRL dEXT_CH__CFG_TRANSFER_MODE;

	CX__VAR_DIGITAL_CTRL dEXT_CH__CFG_PMx_EXIST_FLAG[CFG_PMx__SIZE];

	CX__VAR_ANALOG_CTRL  aEXT_CH__CFG_SIM_SLOT_VLV_CLOSE_TIME;
	CX__VAR_ANALOG_CTRL  aEXT_CH__CFG_SIM_SLOT_VLV_OPEN_TIME;

	// PHY_TM OBJECT -----
	CX__VAR_DIGITAL_CTRL dEXT_CH__PHY_TM__PRESS_STS;
	CX__VAR_ANALOG_CTRL  aEXT_CH__PHY_TM__PRESS_TORR;

	// I/O OBJECT -----
	// DO
	CX__VAR_DIGITAL_CTRL doEXT_CH__PMx__SV_PM_OPEN[CFG_PMx__SIZE];
	CX__VAR_DIGITAL_CTRL doEXT_CH__PMx__SV_PM_CLOSE[CFG_PMx__SIZE];
	CX__VAR_DIGITAL_CTRL doEXT_CH__PMx__SV_TM_OPEN[CFG_PMx__SIZE];
	CX__VAR_DIGITAL_CTRL doEXT_CH__PMx__SV_TM_CLOSE[CFG_PMx__SIZE];
	CX__VAR_DIGITAL_CTRL doEXT_CH__PMx__SV_UP[CFG_PMx__SIZE];
	CX__VAR_DIGITAL_CTRL doEXT_CH__PMx__SV_DOWN[CFG_PMx__SIZE];
	// DI
	CX__VAR_DIGITAL_CTRL diEXT_CH__PMx__SV_PM_OPEN[CFG_PMx__SIZE];
	CX__VAR_DIGITAL_CTRL diEXT_CH__PMx__SV_PM_CLOSE[CFG_PMx__SIZE];
	CX__VAR_DIGITAL_CTRL diEXT_CH__PMx__SV_TM_OPEN[CFG_PMx__SIZE];
	CX__VAR_DIGITAL_CTRL diEXT_CH__PMx__SV_TM_CLOSE[CFG_PMx__SIZE];
	CX__VAR_DIGITAL_CTRL diEXT_CH__PMx__SV_UP[CFG_PMx__SIZE];
	CX__VAR_DIGITAL_CTRL diEXT_CH__PMx__SV_DOWN[CFG_PMx__SIZE];

	CX__VAR_DIGITAL_CTRL diEXT_CH__PMx__ATM_SNS[CFG_PMx__SIZE];
	CX__VAR_DIGITAL_CTRL diEXT_CH__PMx__VAC_SNS[CFG_PMx__SIZE];
	CX__VAR_DIGITAL_CTRL diEXT_CH__DNET_COMM_STS;
	
	CX__VAR_DIGITAL_CTRL diEXT_CH__VAC_RB_RNE;

	CX__VAR_DIGITAL_CTRL diEXT_CH__TM_VAC_SNS;
	CX__VAR_DIGITAL_CTRL diEXT_CH__TM_ATM_SNS;
	//


	//------------------------------------------------------------------------------
	// CHECK SLIT VLV ALL CLOSE -----
	CString sMODE__CHECK_SV_PM_ALL_CLOSE;
	int  Call__CHECK_SV_PM_ALL_CLOSE(CII_OBJECT__VARIABLE* p_variable,
								     CII_OBJECT__ALARM* p_alarm);

	int  Fnc__CHECK_SV_PM_ALL_CLOSE(CII_OBJECT__VARIABLE* p_variable,
								    CII_OBJECT__ALARM* p_alarm);

	// INIT -----
	CString sMODE__INIT;
	int  Call__INIT(CII_OBJECT__VARIABLE* p_variable,
					CII_OBJECT__ALARM* p_alarm);

	int  Fnc__INIT(CII_OBJECT__VARIABLE* p_variable,
				   CII_OBJECT__ALARM* p_alarm);

	// SLIT VLV CLOSE -----
	CString sMODE__SV_PM_CLOSE;
	int  Call__SV_PM_CLOSE(CII_OBJECT__VARIABLE* p_variable,
						   CII_OBJECT__ALARM* p_alarm,
						   const int pmc_id);

	int  Fnc__SV_PM_CLOSE(CII_OBJECT__VARIABLE* p_variable,
					      CII_OBJECT__ALARM* p_alarm,
						  const int pmc_id,
						  const int close_check = -1);

	int  Is__SV_PM_CLOSE(CII_OBJECT__VARIABLE* p_variable,
						 CII_OBJECT__ALARM* p_alarm,
						 const int pmc_id);

	//
	CString sMODE__SV_TM_CLOSE;
	int  Call__SV_TM_CLOSE(CII_OBJECT__VARIABLE* p_variable,
						   CII_OBJECT__ALARM* p_alarm,
						   const int pmc_id);
	
	int  Fnc__SV_TM_CLOSE(CII_OBJECT__VARIABLE* p_variable,
						  CII_OBJECT__ALARM* p_alarm,
						  const int pmc_id);
	int  Is__SV_TM_CLOSE(CII_OBJECT__VARIABLE* p_variable,
						 CII_OBJECT__ALARM* p_alarm,
						 const int pmc_id);
	int  Is__SV_FULL_OPEN(CII_OBJECT__VARIABLE* p_variable,
						  CII_OBJECT__ALARM* p_alarm,
						  const int pmc_id);

	// ...
	int  Check_SV__PM_TM_OPEN_Before_SV_UPDOWN(CII_OBJECT__VARIABLE* p_variable,
											   CII_OBJECT__ALARM* p_alarm, 
											   const int pmc_id);

	// TM ...
	int  Sub__SV_TM_OPEN(CII_OBJECT__VARIABLE* p_variable,
						 CII_OBJECT__ALARM* p_alarm, 
						 const int pmc_id,
						 double dblTimeout);

	int  Sub__SV_TM_CLOSE(CII_OBJECT__VARIABLE* p_variable,
						  CII_OBJECT__ALARM* p_alarm, 
						  const int pmc_id,
						  double dblTimeout);

	// PM ...
	int  Sub__SV_PM_OPEN(CII_OBJECT__VARIABLE* p_variable,
						 CII_OBJECT__ALARM* p_alarm, 
						 const int pmc_id,
						 double dblTimeout);

	int  Sub__SV_PM_CLOSE(CII_OBJECT__VARIABLE* p_variable,
						  CII_OBJECT__ALARM* p_alarm, 
						  const int pmc_id,
						  double dblTimeout);

	// UP ...
	int  Sub__SV_UP(CII_OBJECT__VARIABLE* p_variable,
					CII_OBJECT__ALARM* p_alarm, 
					const int pmc_id,
					double dblTimeout);

	// DOWN ...
	int  Sub__SV_DOWN(CII_OBJECT__VARIABLE* p_variable,
					  CII_OBJECT__ALARM* p_alarm, 
					  const int pmc_id,
					  double dblTimeout);

	int  Is_CLOSE_CONDITION(CII_OBJECT__VARIABLE* p_variable,
							CII_OBJECT__ALARM* p_alarm,
							const CString str_sv_type,
							const int pmc_id);

	// SLIT VLV ALL CLOSE -----
	// PM SV
	CString sMODE__SV_PM_ALL_CLOSE;
	int  Call__SV_PM_ALL_CLOSE(CII_OBJECT__VARIABLE* p_variable,
							   CII_OBJECT__ALARM* p_alarm);

	// TM SV
	CString sMODE__SV_TM_ALL_CLOSE;
	int  Call__SV_TM_ALL_CLOSE(CII_OBJECT__VARIABLE* p_variable,
							   CII_OBJECT__ALARM* p_alarm);

	// ...
	int  Check__PRESSURE_ATM(CII_OBJECT__ALARM* p_alarm,
							 const int pmc_id,
							 const int alarm_id);
	int  Check__PRESSURE_VAC(CII_OBJECT__ALARM* p_alarm,
							 const int pmc_id,
							 const int alarm_id);

	int  Check__SAME_PRESSURE_FOR_VAC_Or_ATM(CII_OBJECT__ALARM* p_alarm,
											 const int pmc_id,
											 const int alarm_id,
											 const CString& act_msg);

	// SLIT VLV OPEN -----
	CString sMODE__SV_OPEN;
	int  Call__SV_OPEN(CII_OBJECT__VARIABLE* p_variable,
					   CII_OBJECT__ALARM* p_alarm,
					   const int pmc_id);

	int  Fnc__SV_OPEN(CII_OBJECT__VARIABLE* p_variable,
					  CII_OBJECT__ALARM* p_alarm,
					  const int pmc_id);

	// ...
	CString sMODE__SV_CLOSE;
	int  Call__SV_CLOSE(CII_OBJECT__VARIABLE* p_variable,
						CII_OBJECT__ALARM* p_alarm,
						const int pmc_id);

	int  Fnc__SV_CLOSE(CII_OBJECT__VARIABLE* p_variable,
					   CII_OBJECT__ALARM* p_alarm,
					   const int pmc_id,
					   const int close_check = -1);

	// ...
	CString sMODE__INIT_SV_CLOSE;
	int  Call__INIT_SV_CLOSE(CII_OBJECT__VARIABLE* p_variable,
							 CII_OBJECT__ALARM* p_alarm,
							 const int pmc_id);

	// CYCLE TEST ...
	CString sMODE__CYCLE_TEST;
	int  Call__CYCLE_TEST(CII_OBJECT__VARIABLE* p_variable,
						  CII_OBJECT__ALARM* p_alarm);

	CString Get__AVG_DATA(const int n_count,
						  const double cur_data,
						  const double pre_avg);

	void Fnc__Check_MIN_MAX_DATA(const int n_count,
								 CII__VAR_STRING_CTRL* p_ch_min,
								 CII__VAR_STRING_CTRL* p_ch_max,
								 const double cur_data);

	// ...
	void Mon__IO_MONITOR(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	void Fnc_App_Log(const CString str_log);
	//


public:
	CObj__PMC_STD();
	~CObj__PMC_STD();

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
