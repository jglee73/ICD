#pragma once

#include "Interface_Code.h"

#include "CCommon_Error.h"


class CObj__VIRTUAL_TYPE : public __IOBJ__STD_TYPE
{
private:
	CCommon_Error__USER_FNC mERROR__USER_FNC;

	//-------------------------------------------------------------------------
	CString sObject_Name;

	SCX__USER_LOG_CTRL xLOG_CTRL;


	//-------------------------------------------------------------------------
	// INTERNAL PROPERTY

	CX__VAR_DIGITAL_CTRL dCH__OBJ_STATUS;
	CX__VAR_STRING_CTRL  sCH__SEQUENCE_MSG;

	CX__VAR_ANALOG_CTRL  aCH__CFG_PUMP_TIMEOUT;
	CX__VAR_ANALOG_CTRL  aCH__CFG_PUMP_PRESSURE_TORR;

	CX__VAR_ANALOG_CTRL  aCH__PRESSURE_TORR;

	//
	SCX__ASYNC_TIMER_CTRL xI_ASYNC_TIMER;
	CX__VAR_ANALOG_CTRL  aCH__TIME_COUNT;

	CX__VAR_STRING_CTRL  sCH__MON_PUMP_COMM_STS;
	CX__VAR_STRING_CTRL  sCH__MON_PUMP_RUN_STS;


	//------------------------------------------------------------------------------
	CString sMODE__INIT;
	int  Call__INIT(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	CString sMODE__MAINT;
	int  Call__MAINT(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	CString sMODE__PUMP_ON;
	int  Call__PUMP_ON(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	CString sMODE__PUMP_OFF;
	int  Call__PUMP_OFF(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);
	//

	//------------------------------------------------------------------------------
	void Mon__IO_MONITOR(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);
	//

public:
	CObj__VIRTUAL_TYPE();
	~CObj__VIRTUAL_TYPE();

	//-------------------------------------------------------------------------
	int __DEFINE__CONTROL_MODE(obj,l_mode);
	int __DEFINE__VERSION_HISTORY(version);

	int __DEFINE__VARIABLE_STD(p_variable);
	int __DEFINE__ALARM(p_alarm);

	int __Define__USER_FUNCTION(CII_DEFINE__FUNCTION *p_fnc_ctrl);

	//-------------------------------------------------------------------------
	int __INITIALIZE__OBJECT(p_variable,p_ext_obj_create);

	//-------------------------------------------------------------------------
	int __CALL__CONTROL_MODE(mode,p_debug,p_variable,p_alarm);
	int __CALL__MONITORING(id,p_variable,p_alarm);
};
