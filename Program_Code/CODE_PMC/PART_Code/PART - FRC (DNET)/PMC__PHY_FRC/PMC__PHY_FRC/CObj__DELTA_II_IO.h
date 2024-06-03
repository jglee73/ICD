#pragma once

#include "Interface_Code.h"
#include "CCommon_Error.h"


class CObj__DELTA_II_IO : public __IOBJ__STD_TYPE
{
private:
	//-------------------------------------------------------------------------
	CString	sObject_Name;

	int iActive__SIM_MODE;
	SCX__USER_LOG_CTRL xI_LOG_CTRL;


	//-------------------------------------------------------------------------
	CX__VAR_STRING_CTRL  sCH__OBJ_MSG;
	CX__VAR_STRING_CTRL  sCH__OBJ_STATUS;

	CX__VAR_ANALOG_CTRL  aCH__CFG_CH2_SET_RATIO;

	CX__VAR_ANALOG_CTRL  aCH__PARA_CH1_SET_RATIO;
	CX__VAR_ANALOG_CTRL  aCH__PARA_CH2_SET_RATIO;

	CX__VAR_STRING_CTRL  sCH__MON_EXCEPTION_STATE;
	CX__VAR_STRING_CTRL  sCH__MON_SW_RATIO;
	CX__VAR_STRING_CTRL  sCH__MON_CH1_GET_RATIO;
	CX__VAR_STRING_CTRL  sCH__MON_CH2_GET_RATIO;

	// Stable Channel ...
	CX__VAR_ANALOG_CTRL  aCH__CFG_WARNING_LIMIT;
	CX__VAR_ANALOG_CTRL  aCH__CFG_FAULT_LIMIT;

	CX__VAR_ANALOG_CTRL  aCH__CFG_CHECKING_TIME_FOR_STABLE;
	CX__VAR_ANALOG_CTRL  aCH__CFG_CHECK_TIMEOUT_FOR_ERROR;
	CX__VAR_ANALOG_CTRL  aCH__CFG_STABLE_TIME_FOR_SETPINT_CHANGE;

	CX__VAR_STRING_CTRL  sCH__MON_CH1_STABLE_STATE;
	CX__VAR_STRING_CTRL  sCH__MON_CH1_STABLE_TIME;
	CX__VAR_STRING_CTRL  sCH__MON_CH1_FAULT_STATE;
	CX__VAR_STRING_CTRL  sCH__MON_CH1_FAULT_TIME;
	CX__VAR_STRING_CTRL  sCH__MON_CH1_CHANGE_STATE;
	CX__VAR_STRING_CTRL  sCH__MON_CH1_CHANGE_TIME;

	CX__VAR_STRING_CTRL  sCH__MON_CH2_STABLE_STATE;
	CX__VAR_STRING_CTRL  sCH__MON_CH2_STABLE_TIME;
	CX__VAR_STRING_CTRL  sCH__MON_CH2_FAULT_STATE;
	CX__VAR_STRING_CTRL  sCH__MON_CH2_FAULT_TIME;
	CX__VAR_STRING_CTRL  sCH__MON_CH2_CHANGE_STATE;
	CX__VAR_STRING_CTRL  sCH__MON_CH2_CHANGE_TIME;

	CX__VAR_STRING_CTRL  sCH__MON_STABLE_FLAG;
	CX__VAR_STRING_CTRL  sCH__MON_FAULT_FLAG;


	//-------------------------------------------------------------------------
	CX__VAR_STRING_CTRL   sEXT_CH__IO_IN_BYTE_HEXA;
	CX__VAR_STRING_CTRL   sEXT_CH__IO_OUT_BYTE_HEXA;


	//-------------------------------------------------------------------------
	CString sMODE__INIT;
	int Call__INIT(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);

	CString sMODE__CH1_SET_RATIO;
	int Call__CH1_SET_RATIO(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);
	int Fnc__CH1_SET_RATIO(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);

	//
	CString sMODE__ALL_CLOSE;
	int Call__ALL_CLOSE(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);

	CString sMDDE__CONTROL_CFG;
	int Call__CONTROL_CFG(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);

	CString sMDDE__CONTROL;
	int Call__CONTROL(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);
	
	CString sMDDE__GASLINE_PUMP;
	int Call__GASLINE_PUMP(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);


	// ...
	int Mon__STATUS(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);
	
	int Mon__STABLE(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);
	int Fnc__STABLE_CHECK(CII_OBJECT__ALARM* p_alarm, const int ch_type, const int alm_report = 1);
	//


public:
	CObj__DELTA_II_IO();
	~CObj__DELTA_II_IO();

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
