#pragma once

#include "Interface_Code.h"

#include "CCommon_Error.h"
#include "CCommon_System.h"


class CObj__BROOKS_IO : public __IOBJ__STD_TYPE
{
private:
	CCommon_Error__USER_FNC mERROR__USER_FNC;

	//-------------------------------------------------------------------------
	CString	sObject_Name;

	// ...
	int iSIM_FLAG;

	SCX__USER_LOG_CTRL xAPP_LOG_CTRL;

	// ...
	CRITICAL_SECTION  mCS_Cmd_Rst_Lock;

	// E84 --------
	CRITICAL_SECTION  mCS_LP_Lock;
	CRITICAL_SECTION  mCS_PIO_Lock;
	CRITICAL_SECTION  mCS_PIO_ES_Lock;
	CRITICAL_SECTION  mCS_PIO_HOAVBL_Lock;

	int	 m_nCommState;
	int  iTP1,iTP2,iTP3,iTP4,iTP5;

	int	 iFlag__PIO_LOG;
	int	 iFlag__E84_LOG;
	int  iFlag__APP_LOG;
	int  iFlag__DRV_LOG;
	int  iFlag__MON_CH_LOG;


	//-------------------------------------------------------------------------
	//  INTERNAL PROPERTY

	// ...
	CX__VAR_ANALOG_CTRL  aCH__OTR_IN_CFG__LOAD_COMPT_DELAY_TIME;
	CX__VAR_STRING_CTRL  sCH__COMM_STS;
	CX__VAR_STRING_CTRL  sCH__INR_LP__MSG;

	// CFG ...
	CX__VAR_DIGITAL_CTRL dCH__CFG_SIM_ERROR_ACTIVE;

	CX__VAR_ANALOG_CTRL  aCH__PARA_CYCLE_COUNT;
	CX__VAR_ANALOG_CTRL  aCH__CUR_CYCLE_COUNT;

	// ACTIVE CHECK ...
	CX__VAR_STRING_CTRL  sCH__ACTIVE_CHECK_CLAMP_DO;
	CX__VAR_STRING_CTRL  sCH__ACTIVE_CHECK_CLAMP_DI;

	CX__VAR_STRING_CTRL  sCH__ACTIVE_CHECK_UNCLAMP_DO;
	CX__VAR_STRING_CTRL  sCH__ACTIVE_CHECK_UNCLAMP_DI;

	CX__VAR_STRING_CTRL  sCH__ACTIVE_CHECK_DOOR_EXT_DO;
	CX__VAR_STRING_CTRL  sCH__ACTIVE_CHECK_DOOR_EXT_DI;

	CX__VAR_STRING_CTRL  sCH__ACTIVE_CHECK_DOOR_RET_DO;
	CX__VAR_STRING_CTRL  sCH__ACTIVE_CHECK_DOOR_RET_DI;

	CX__VAR_STRING_CTRL  sCH__ACTIVE_CHECK_DOOR_DN_DO;
	CX__VAR_STRING_CTRL  sCH__ACTIVE_CHECK_DOOR_DN_DI;

	CX__VAR_STRING_CTRL  sCH__ACTIVE_CHECK_DOOR_UP_DO;
	CX__VAR_STRING_CTRL  sCH__ACTIVE_CHECK_DOOR_UP_DI;

	CX__VAR_STRING_CTRL  sCH__ACTIVE_CHECK_LATCH_OPEN_DO;
	CX__VAR_STRING_CTRL  sCH__ACTIVE_CHECK_LATCH_OPEN_DI;

	CX__VAR_STRING_CTRL  sCH__ACTIVE_CHECK_LATCH_CLOSE_DO;
	CX__VAR_STRING_CTRL  sCH__ACTIVE_CHECK_LATCH_CLOSE_DI;

	CX__VAR_STRING_CTRL  dCH__ACTIVE_CHECK_DOOR_VAC_DO;
	CX__VAR_STRING_CTRL  dCH__ACTIVE_CHECK_DOOR_VAC_DI;

	CX__VAR_STRING_CTRL  sCH__ACTIVE_CHECK_SHUTTLE_IN_DO;
	CX__VAR_STRING_CTRL  sCH__ACTIVE_CHECK_SHUTTLE_IN_DI;

	CX__VAR_STRING_CTRL  sCH__ACTIVE_CHECK_SHUTTLE_OUT_DO;
	CX__VAR_STRING_CTRL  sCH__ACTIVE_CHECK_SHUTTLE_OUT_DI;

	CX__VAR_STRING_CTRL  sCH__ACTIVE_CHECK_WAFER_SLID_OUT_DI;
	CX__VAR_STRING_CTRL  sCH__ACTIVE_CHECK_FOUP_PRESENT_DI;
	CX__VAR_STRING_CTRL  sCH__ACTIVE_CHECK_FOUP_PLACED_DI;

	// MON ...
	CX__VAR_DIGITAL_CTRL dCH__OTR_OUT_dLP_STATUS;

	CX__VAR_DIGITAL_CTRL dCH__MON_DOOR_STATUS;
	CX__VAR_DIGITAL_CTRL dCH__MON_FOUP_POS_STATUS;
	CX__VAR_DIGITAL_CTRL dCH__MON_FOUP_STATUS;
	CX__VAR_DIGITAL_CTRL dCH__MON_CLAMP_STATUS;
	CX__VAR_DIGITAL_CTRL dCH__OTR_OUT_dLP_PIO_TRANSFER;

	// IN.MON ...
	CX__VAR_STRING_CTRL  sCH__MON_ACTIVE_FA_AUTO;

	CX__VAR_DIGITAL_CTRL dCH__OTR_IN_MON_dATM_RB1_ARM;
	CX__VAR_DIGITAL_CTRL dCH__OTR_IN_MON_dATM_RB1_TRG_MOVE;
	CX__VAR_DIGITAL_CTRL dCH__OTR_IN_MON_dATM_RB1_ARM_A_ACT;
	CX__VAR_DIGITAL_CTRL dCH__OTR_IN_MON_dATM_RB1_ARM_B_ACT;
	CX__VAR_STRING_CTRL  dCH__OTR_IN_sLP_RELOAD_FLAG;

	CX__VAR_ANALOG_CTRL  aCH__LP_TP_TIME_COUNT;
	CX__VAR_ANALOG_CTRL  aCH__LP_PRE_TIME_COUNT;
	CX__VAR_ANALOG_CTRL  aCH__LP_PLS_TIME_COUNT;

	// ...
	CX__VAR_STRING_CTRL  sCH__RES_MAP_SLOT[WAFER_SLOT_MAX];
	CX__VAR_DIGITAL_CTRL dCH__CFG_LOAD_CLAMP_OPT;
	CX__VAR_DIGITAL_CTRL dCH__CFG_UNLD_CLAMP_OPT;

	CX__VAR_STRING_CTRL	 sCH__INR_TIME_CNT_CH;

	CX__VAR_DIGITAL_CTRL dCH__LP__CLAMP_STS;
	CX__VAR_DIGITAL_CTRL dCH__LP__UNCLAMP_STS;

	// ...
	CX__VAR_STRING_CTRL  sCH__LP__OBJ_STS;
	CX__VAR_STRING_CTRL  sCH__LP__OBJ_MODE;

	CX__VAR_DIGITAL_CTRL dCFG__UNLATCH_FOUP_VAC_ON_CHECK;
	CX__VAR_DIGITAL_CTRL dCFG__LATCH_FOUP_VAC_OFF_CHECK;

	CX__VAR_DIGITAL_CTRL dCFG__BEFORE_LOAD_CST_CHECK;
	CX__VAR_DIGITAL_CTRL dCFG__WAFER_SLIDE_SNS_CHECK;
	CX__VAR_DIGITAL_CTRL dCFG__LOAD_MODE;

	CX__VAR_ANALOG_CTRL  aCFG_ACCESS_SW_TIMEOUT;
	CX__VAR_ANALOG_CTRL  aCFG_CST_PROPERLY_EXIST_TIME;
	CX__VAR_DIGITAL_CTRL dCFG_POST_CARRIER_ALARM;
	CX__VAR_ANALOG_CTRL  aCFG_PADDLE_DOWN_DELAY;
	CX__VAR_DIGITAL_CTRL dPIO_COMM_READY_STS;

	CX__VAR_DIGITAL_CTRL dCH__CFG_INIT_SLOT_STATE_UNKNOWN;
	//


	//-------------------------------------------------------------------------
	//  EXTERNAL PROPERTY

	// LINK PIO ...
	bool bActive__LINK_PIO;

	CX__VAR_STRING_CTRL  sEXT_CH__LPx_PIO__MON_ACTIVE_FA_AUTO;

	CX__VAR_DIGITAL_CTRL dEXT_CH__LPx_PIO__RUN_SNS;
	CX__VAR_DIGITAL_CTRL dEXT_CH__LPx_PIO__TRANSFER_SNS;

	// LINK IO ...
	CX__VAR_DIGITAL_CTRL diEXT_CH__LPx__COMM_STS;

	CX__VAR_DIGITAL_CTRL doEXT_CH__LPx__DOOR_UP;
	CX__VAR_DIGITAL_CTRL diEXT_CH__LPx__DOOR_UP;
	CX__VAR_DIGITAL_CTRL doEXT_CH__LPx__DOOR_DN;
	CX__VAR_DIGITAL_CTRL diEXT_CH__LPx__DOOR_DN;
	CX__VAR_DIGITAL_CTRL doEXT_CH__LPx__DOOR_EXT;
	CX__VAR_DIGITAL_CTRL diEXT_CH__LPx__DOOR_EXT;
	CX__VAR_DIGITAL_CTRL doEXT_CH__LPx__DOOR_RET;
	CX__VAR_DIGITAL_CTRL diEXT_CH__LPx__DOOR_RET;

	bool bActive__LPx_DOOR_VENT;
	CX__VAR_DIGITAL_CTRL doEXT_CH__LPx__DOOR_VENT;
	CX__VAR_DIGITAL_CTRL doEXT_CH__LPx__DOOR_VAC;
	CX__VAR_DIGITAL_CTRL diEXT_CH__LPx__DOOR_VAC;

	CX__VAR_DIGITAL_CTRL doEXT_CH__LPx__LATCH;
	CX__VAR_DIGITAL_CTRL diEXT_CH__LPx__LATCH;
	CX__VAR_DIGITAL_CTRL doEXT_CH__LPx__UNLATCH;
	CX__VAR_DIGITAL_CTRL diEXT_CH__LPx__UNLATCH;

	CX__VAR_DIGITAL_CTRL doEXT_CH__LPx__SHUTTLE_IN;
	CX__VAR_DIGITAL_CTRL diEXT_CH__LPx__SHUTTLE_IN;
	CX__VAR_DIGITAL_CTRL doEXT_CH__LPx__SHUTTLE_OUT;
	CX__VAR_DIGITAL_CTRL diEXT_CH__LPx__SHUTTLE_OUT;

	CX__VAR_DIGITAL_CTRL doEXT_CH__LPx__CLAMP;
	CX__VAR_DIGITAL_CTRL diEXT_CH__LPx__CLAMP;
	CX__VAR_DIGITAL_CTRL diEXT_CH__LPx__UNCLAMP;
	CX__VAR_DIGITAL_CTRL doEXT_CH__LPx__UNCLAMP;

	CX__VAR_DIGITAL_CTRL diEXT_CH__LPx__WFR_SLID_OUT_SNS;

	CX__VAR_DIGITAL_CTRL diEXT_CH__LPx__FOUP_PRESENT;
	CX__VAR_DIGITAL_CTRL diEXT_CH__LPx__FOUP_PLACED;

	CX__VAR_DIGITAL_CTRL diEXT_CH__LPx__LOAD_BTN;
	CX__VAR_DIGITAL_CTRL diEXT_CH__LPx__UNLOAD_BTN;

	CX__VAR_DIGITAL_CTRL doEXT_CH__LPx__LAMP[LAMP_MAX];
	//


	//-------------------------------------------------------------------------
	CString sMODE__INIT;
	CString sMODE__HOME;
	int  Call__INIT(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);
	int  Fnc__INIT(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller);
	int  Seq__INIT(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller);

	CString sMODE__PREPLOAD;
	int  Call__PREPLOAD(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);
	int  Fnc__PREPLOAD(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller);

	CString sMODE__LOAD;
	int  Call__LOAD(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);
	int  Fnc__LOAD(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller);
	int  Fnc__DOCK_and_EXTEND(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller);

	CString sMODE__UNLOAD;
	int  Call__UNLOAD(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);
	int  Fnc__UNLOAD(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller);

	CString sMODE__RLSUNLOAD;
	int  Call__RLSUNLOAD(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);
	int  Fnc__RLSUNLOAD(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller);

	CString sMODE__CYCLE;
	int  Call__CYCLE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	CString sMODE__MAP;
	int  Call__MAP(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);
	int  Fnc__MAP(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller);

	CString sMODE__LATCH;
	int  Call__LATCH(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);
	int  Fnc__LATCH(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller);
	int  _Fnc__LATCH(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller);

	CString sMODE__UNLATCH;
	int  Call__UNLATCH(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);
	int  Fnc__UNLATCH(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller);
	int  _Fnc__UNLATCH(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller);

	CString sMODE__EXTENDPADDLE;
	int  Call__EXTENDPADDLE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);
	int  Fnc__EXTENDPADDLE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller);
	int  _Fnc__EXTENDPADDLE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller);

	CString sMODE__RETRACTPADDLE;
	int  Call__RETRACTPADDLE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);
	int  Fnc__RETRACTPADDLE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller);
	int  _Fnc__RETRACTPADDLE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller);

	CString sMODE__UPPADDLE;
	int  Call__UPPADDLE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);
	int  Fnc__UPPADDLE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller);
	int  _Fnc__UPPADDLE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller);

	CString sMODE__DOWNPADDLE;
	int  Call__DOWNPADDLE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);
	int  Fnc__DOWNPADDLE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller);
	int  _Fnc__DOWNPADDLE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller);

	CString sMODE__SHUTTLE_OUT;
	int  Call__SHUTTLE_OUT(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);
	int  Fnc__SHUTTLE_OUT(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller);
	int  _Fnc__SHUTTLE_OUT(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller);

	CString sMODE__SHUTTLE_IN;
	int  Call__SHUTTLE_IN(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);
	int  Fnc__SHUTTLE_IN(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller);
	int  _Fnc__SHUTTLE_IN(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller);

	CString sMODE__VACON;
	int  Call__VACON(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);
	int  Fnc__VACON(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller);
	int  _Fnc__VACON(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller);

	CString sMODE__VACOFF;
	int  Call__VACOFF(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);
	int  Fnc__VACOFF(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller);
	int  _Fnc__VACOFF(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller);

	CString sMODE__CLAMP;
	int  Call__CLAMP(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);
	int  Fnc__CLAMP(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller);
	int  _Fnc__CLAMP(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller);

	CString sMODE__UNCLAMP;
	int  Call__UNCLAMP(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);
	int  Fnc__UNCLAMP(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller);
	int  _Fnc__UNCLAMP(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller);

	CString sMODE__DOOR_CLOSE;
	int  Call__DOOR_CLOSE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);
	int  Fnc__DOOR_CLOSE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller);

	CString sMODE__DOOR_OPEN;
	int  Call__DOOR_OPEN(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);
	int  Fnc__DOOR_OPEN(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller);

	CString sMODE__OPENDOOR_WITH_NO_UNLATCH;
	int  Call__OPENDOOR_WITH_NO_UNLATCH(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);
	int  Fnc__OPENDOOR_WITH_NO_UNLATCH(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller);

	// ...
	CString sMODE__SIM_FOUP_EXIST;
	CString sMODE__SIM_FOUP_NONE;

	// ...
	int  Fnc__MAP_RESULT(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller);
	int  Seq_LAMP(const int lamp_id, const CString ctrl_mode, const CString str_lamp_name, const CString str_caller);

	int  Fnc__LD_buttonMon(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM *p_alarm);
	int  Fnc__UNLD_buttonMon(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM *p_alarm);

	// ...
	void Mon__PIO_TRANSFER_MONITOR(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm);	
	void Mon__IO_MONITOR(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm);		
	void Mon__IO_CH_OUT_MONITOR(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm);

	int Is__Abort(CII_OBJECT__VARIABLE* p_variable);

	// ...
	int  Is__ONLINE(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm);	// OK
	int  Is__LOAD_CONDITION(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm);
	int  Is__LP_CLOSE_CND(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm);
	int  Is__Paddle_Up_Down_CND(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm);
	int	 Is__LP_AUTO_MODE();
	int	 Is__RELOAD_FLAG_YES(CII_OBJECT__VARIABLE* p_variable);

	// ...
	void Fnc__APP_LOG(const CString& log_msg);

	// ...
	int Seq_INIT(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm, const CString str_caller);

	int INIT_LAMP(const CString ctrl_mode);
	int IS_ROBOT_ARM_RETRACT(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm);

	int _Wait_CHECK(CII_OBJECT__VARIABLE* p_variable,
				    CII_OBJECT__ALARM* p_alarm, 
				    const CString& cmd_name,
				    CII__VAR_DIGITAL_CTRL* p_ch__di_01,
					const CString& str__di_01,
					CII__VAR_DIGITAL_CTRL* p_ch__di_02,
					const CString& str__di_02);
	//

public:
	CObj__BROOKS_IO();
	~CObj__BROOKS_IO();

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
