#pragma once

#include "Interface_Code.h"

#include "CCommon_Error.h"
#include "CCommon_System.h"

#include "CObj__OHT_E84_IO__DEF.h"


// ...
#define	 TIMEOUT_ERROR_RET			-9999


class CObj__OHT_E84_IO : public __IOBJ__STD_TYPE
{
private:
	CCommon_Error__USER_FNC mERROR__USER_FNC;

	//-------------------------------------------------------------------------
	CString	sObject_Name;

	SCX__USER_LOG_CTRL xPIO_LOG_CTRL;
	SCX__USER_LOG_CTRL xE84_LOG_CTRL;

	int iActive__SIM_MODE;

	// E84 ...
	int	m_nTP1;
	int	m_nTP2;
	int	m_nTP3;
	int	m_nTP4;
	int	m_nTP5;

	int	m_nTD1;

	int	iFlag__PIO_LOG;
	int	iFlag__E84_LOG;
	int iFlag__APP_LOG;
	int iFlag__DRV_LOG;
	int iFlag__MON_CH_LOG;
	//


	//-------------------------------------------------------------------------
	//  INTERNAL PROPERTY

	// VIRTUAL CHANNEL
	CX__VAR_STRING_CTRL  sCH__COMM_STS;
	CX__VAR_STRING_CTRL  sCH__INR_LP__MSG;

	CX__VAR_DIGITAL_CTRL dCH__ACTIVE_PIO_LOG;

	// REPORT_PIO ...
	CX__VAR_DIGITAL_CTRL dCH__REPORT_PIO_E84_RUN_STATE;
	CX__VAR_DIGITAL_CTRL dCH__REPORT_PIO_ERROR_ACTIVE;

	// IN.MON ...
	CX__VAR_ANALOG_CTRL  aCH__LP_TP_TIME_COUNT;
	CX__VAR_ANALOG_CTRL  aCH__LP_PRE_TIME_COUNT;
	CX__VAR_ANALOG_CTRL  aCH__LP_PLS_TIME_COUNT;

	// ...
	CX__VAR_STRING_CTRL  sCH__INR_CST_UNKNOWN_TIMER_COUNTER;
	CX__VAR_STRING_CTRL  sCH__INR_TP_TIMER_COUNTER;
	CX__VAR_STRING_CTRL  sCH__INR_CST_PRESENT_TIMER_DISPLAY;
	CX__VAR_STRING_CTRL  sCH__INR_CST_PLACE_TIMER_DISPLAY;
	CX__VAR_STRING_CTRL  sCH__INR_CST_NONE_TIMER_DISPLAY;
	CX__VAR_STRING_CTRL  sCH__INR_CST_EXIST_TIMER_DISPLAY;

	CX__VAR_DIGITAL_CTRL dCH__CFG_LT_CURTN_USE_OPT;
	CX__VAR_DIGITAL_CTRL dCH__INR_LOGIC_ERROR_FLAG;
	CX__VAR_DIGITAL_CTRL dCH__INR_NONE_VIOLATION_ERROR_FLAG;
	CX__VAR_DIGITAL_CTRL dCH__INR_EXIST_VIOLATION_ERROR_FLAG;

	CX__VAR_ANALOG_CTRL  aCH__EXT_CFG_PIO_TPx[_CFG__PIO_TPx];
	CX__VAR_ANALOG_CTRL  aCH__EXT_CFG_PIO_TDx[_CFG__PIO_TDx];
	CX__VAR_DIGITAL_CTRL dCH__PIO_RESET;
	CX__VAR_STRING_CTRL  sCH__CUR_CHECK_TPx;

	CX__VAR_ANALOG_CTRL  aCH__CFG_PIO_SIGNAL_CHATTERING_TIME;
	CX__VAR_ANALOG_CTRL  aCH__CFG_PIO_FOUP_VIOLATION_TIME;
	CX__VAR_ANALOG_CTRL  aCH__CFG_PIO_FOUP_LOGIC_ERROR_TIME;
	CX__VAR_STRING_CTRL	 sCH__INR_TIME_CNT_CH;

	// ...
	CX__VAR_DIGITAL_CTRL dCH__CFG_E84LOAD_CLAMP_CHECK;
	CX__VAR_DIGITAL_CTRL dCH__CFG_LT_CURTN_AUTO_CLEAR;
	CX__VAR_DIGITAL_CTRL dCH__CFG_LT_CURTN_CHECK_WHEN_IDLE;
	//


	//-------------------------------------------------------------------------
	//  EXTERNAL PROPERTY

	// LINK_PIO ...
	CX__VAR_DIGITAL_CTRL dEXT_CH__LINK_PIO_TRANSFER_STATE;

	CX__VAR_DIGITAL_CTRL dEXT_CH__LINK_PIO_ACTIVE_RUN;
	CX__VAR_DIGITAL_CTRL dEXT_CH__LINK_PIO_ACTIVE_FA_AUTO;
	CX__VAR_DIGITAL_CTRL dEXT_CH__LINK_PIO_ACTIVE_LOAD_REQ;
	CX__VAR_DIGITAL_CTRL dEXT_CH__LINK_PIO_ACTIVE_UNLOAD_REQ;

	// OBJ : LPx_INFO ...
	CX__VAR_DIGITAL_CTRL dEXT_CH__LPx_INFO__FOUP_STATE;
	CX__VAR_DIGITAL_CTRL dEXT_CH__LPx_INFO__CLAMP_STATE;

	// BIT.IO ...
	CX__VAR_DIGITAL_CTRL dEXT_CH__LPx_IO__FOUP_PLACED_DI;
	CX__VAR_DIGITAL_CTRL dEXT_CH__LPx_IO__FOUP_PRESENT_DI;

	CX__VAR_DIGITAL_CTRL dEXT_CH__LPx_IO__CLAMP_DO;
	CX__VAR_DIGITAL_CTRL dEXT_CH__LPx_IO__UNCLAMP_DO;

	CX__VAR_DIGITAL_CTRL dEXT_CH__LPx_IO__CLAMP_DI;
	CX__VAR_DIGITAL_CTRL dEXT_CH__LPx_IO__UNCLAMP_DI;

	CX__VAR_DIGITAL_CTRL dEXT_CH__LPx_IO__LIGHT_CURTAIN_DI;

	// E84 PIO ...
	// P(SYSTEM) -> A(OHT)
	CX__VAR_DIGITAL_CTRL doEXT_CH__LPx__E84_L_REQ;
	CX__VAR_DIGITAL_CTRL doEXT_CH__LPx__E84_U_REQ;
	CX__VAR_DIGITAL_CTRL doEXT_CH__LPx__E84_NC1;
	CX__VAR_DIGITAL_CTRL doEXT_CH__LPx__E84_READY;
	CX__VAR_DIGITAL_CTRL doEXT_CH__LPx__E84_NC2;
	CX__VAR_DIGITAL_CTRL doEXT_CH__LPx__E84_NC3;
	CX__VAR_DIGITAL_CTRL doEXT_CH__LPx__E84_HO_AVBL;
	CX__VAR_DIGITAL_CTRL doEXT_CH__LPx__E84_ES;
	// P(SYSTEM) <- A(OHT)
	CX__VAR_DIGITAL_CTRL diEXT_CH__LPx__E84_VALID;
	CX__VAR_DIGITAL_CTRL diEXT_CH__LPx__E84_CS_0;
	CX__VAR_DIGITAL_CTRL diEXT_CH__LPx__E84_CS_1;
	CX__VAR_DIGITAL_CTRL diEXT_CH__LPx__E84_NC1;
	CX__VAR_DIGITAL_CTRL diEXT_CH__LPx__E84_TR_REQ;
	CX__VAR_DIGITAL_CTRL diEXT_CH__LPx__E84_BUSY;
	CX__VAR_DIGITAL_CTRL diEXT_CH__LPx__E84_COMPT;
	CX__VAR_DIGITAL_CTRL diEXT_CH__LPx__E84_CONT;
	//


	//-------------------------------------------------------------------------
	CString sMODE__INIT;
	int  Call__INIT(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);
	int  Fnc__INIT(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	// ...
	int  Fnc__LOAD(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);
	int  Fnc__LOAD_PIO(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	int  Fnc__UNLOAD(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);
	int  Fnc__UNLOAD_PIO(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	// ...
	int  Fnc__CS_VALID_WAITING(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString& sz_ctrl_mode);
	//

	//-------------------------------------------------------------------------
	void Mon__ALL_STATE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);
	void Fnc__IO_LP_PIO_LOG(CII_OBJECT__VARIABLE* p_variable);
	
	void Mon__IO_E84_MAIN(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	// ...
	int  Fnc__CLAMP(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller);
	int  Fnc__UNCLAMP(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const CString str_caller);

	// ...
	int  CMD_STAMP(const CString ctrl_mode,
				   const CString cmd_name,
				   CII__VAR_DIGITAL_CTRL* pch_set_cmd, 
				   CII__VAR_DIGITAL_CTRL* pch_get_sns,
				   CII_OBJECT__ALARM* p_alarm);

	// E84
	int  Set__UREQ(const CString& ch_data);
	int  Set__LREQ(const CString& ch_data);
	int	 Is__Sns_TRREQ();					
	int  Set__READY(const CString& ch_data);	

	int  Is__FOUP_EXIST();					
	int  Is__FOUP_NONE();
	
	CString Is__FOUP_STATE_PIO();			
	int  Get__Read_TP_Config();				
	int  Is__Sns_BUSY();						
	int  Is__Sns_COMPT();					
	int  Is__Sns_CS0();						
	int  Is__Sns_VALID();					
	int  Is__Sns_CS0_VALID();
	int  Active_Signal_Check(const bool active__load_seq, const CString tp_time, CString& str_log, CString& valid_chk);

	int  Set__HOAVBL(const CString& ch_data);
	int  Set__AllOff();
	int  Set__HO_ON();
	int  Is__Abort(CII_OBJECT__VARIABLE* p_variable);

	int  Set__ES(const CString& ch_data);
	int  Set__AllOff_Except_ES();

	int  Is__ONLINE(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm);	
	int  Is__LOCAL(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm);
	int	 Is__LP_AUTO_MODE();
	int	 Is__RELOAD_FLAG_YES(CII_OBJECT__VARIABLE* p_variable);
	int  Is__LT_BROKEN(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm);
	int  Is__FOUP_LOGIC_ERROR(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm, const CString& rc_act);
	int  Is__FOUP_VIOLATION_ERROR(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm, const CString& mode);
	int  Is_CS_VALID_OFF(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm);

	void Fnc__PIO_LOG(const CString& log_msg);
	void Fnc__E84_LOG(const CString& log_msg);
	void Fnc__APP_MSG(const CString& log_msg);


public:
	CObj__OHT_E84_IO();
	~CObj__OHT_E84_IO();

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
