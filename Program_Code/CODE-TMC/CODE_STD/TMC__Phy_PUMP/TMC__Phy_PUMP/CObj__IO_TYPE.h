#pragma once

#include "Interface_Code.h"

#include "CCommon_Error.h"
#include "CObj__IO_TYPE__DEF.h"


class CObj__IO_TYPE : public __IOBJ__STD_TYPE
{
private:
	CCommon_Error__USER_FNC mERROR__USER_FNC;

	//-------------------------------------------------------------------------
	CString sObject_Name;

	SCX__USER_LOG_CTRL xLOG_CTRL;


	//-------------------------------------------------------------------------
	// INTERNAL PROPERTY

	CX__VAR_DIGITAL_CTRL dCH__OBJ_STATUS;
	CX__VAR_STRING_CTRL  sCH__OBJ_MSG;

	CX__VAR_ANALOG_CTRL  aCH__CFG_ON_DELAY;
	CX__VAR_STRING_CTRL  sCH__APP_DELAY_COUNT;

	CX__VAR_STRING_CTRL  sCH__MON_PUMP_COMM_STS;
	CX__VAR_STRING_CTRL  sCH__MON_PUMP_RUN_STS;
	CX__VAR_STRING_CTRL  sCH__MON_PUMP_STATE;
	//

	//-------------------------------------------------------------------------
	// EXTERNAL PROPERTY

	// IO : DO ...
	CX__VAR_DIGITAL_CTRL doEXT_CH__PUMP_POWER;
	CX__VAR_DIGITAL_CTRL doEXT_CH__PUMP_IDLE_MODE;
	CX__VAR_DIGITAL_CTRL doEXT_CH__PUMP_REMOTE_MODE;

	// IO : DI ...
	CX__VAR_DIGITAL_CTRL diEXT_CH__PUMP_STATE_RUNNING;
	CX__VAR_DIGITAL_CTRL diEXT_CH__PUMP_BLOWER_RUNNING;
	CX__VAR_DIGITAL_CTRL diEXT_CH__PUMP_WARNING_NORMAL;
	CX__VAR_DIGITAL_CTRL diEXT_CH__PUMP_ALARM_NORMAL;
	CX__VAR_DIGITAL_CTRL diEXT_CH__PUMP_OPERATION_READY;

	CX__VAR_DIGITAL_CTRL diEXT_CH__PUMP_ALARM_ERROR;

	// IO : VLV ...
	CX__VAR_DIGITAL_CTRL diEXT_CH__PUMP_SLOW_VLV;
	CX__VAR_DIGITAL_CTRL diEXT_CH__PUMP_FAST_VLV;
	//

	//------------------------------------------------------------------------------
	CString sMODE__INIT;
	int Call__INIT(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	CString sMODE__MAINT;
	int Call__MAINT(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	CString sMODE__PUMP_ON;
	int Call__PUMP_ON(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	CString sMODE__PUMP_OFF;
	int Call__PUMP_OFF(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	// ...
	int  _Check__INTERLOCK__PUMP_VLV(CII_OBJECT__ALARM* p_alarm);

	void _Check__ALM__PUMP_STATE_RUNNING(CII_OBJECT__ALARM* p_alarm);
	void _Check__ALM__PUMP_BLOWER_RUNNING(CII_OBJECT__ALARM* p_alarm);
	void _Check__ALM__PUMP_WARNING_NORMAL(CII_OBJECT__ALARM* p_alarm);
	void _Check__ALM__PUMP_ALARM_NORMAL(CII_OBJECT__ALARM* p_alarm);
	void _Check__ALM__PUMP_OPERATION_READY(CII_OBJECT__ALARM* p_alarm);
	void _Check__ALM__PUMP_ALARM_ERROR(CII_OBJECT__ALARM* p_alarm);

	void _Check__ALM_POST(CII_OBJECT__ALARM* p_alarm, 
						  const int alm_id, 
						  CII__VAR_DIGITAL_CTRL* p_ch);
	//

	//------------------------------------------------------------------------------
	void Mon__IO_MONITOR(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);
	//


public:
	CObj__IO_TYPE();
	~CObj__IO_TYPE();

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
