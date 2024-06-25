#pragma once

#include "Interface_Code.h"

#include "CCommon_Error.h"
#include "CObj__SYSTEM__DEF.h"


class CObj__SYSTEM : public __IOBJ__STD_TYPE
{
private:
	CCommon_Error__USER_FNC mERROR__USER_FNC;

	//------------------------------------------------------------------------------
	CString sObject_Name;

	int iActive__SIM_MODE;


	//------------------------------------------------------------------------------
	// INTERNAL PROPERTY

	CX__VAR_DIGITAL_CTRL dCH__CFG_CHECK_CDA_FLOW;
	CX__VAR_DIGITAL_CTRL dCH__CFG_CHECK_N2_FLOW;
	CX__VAR_DIGITAL_CTRL dCH__CFG_CHECK_EFEM_DOOR_CLOSE;

	CX__VAR_DIGITAL_CTRL dCH__CFG_CHECK_FFU_DIFF_PRESSURE;
	CX__VAR_ANALOG_CTRL  aCH__CFG_FFU_DIFF_PRESSURE_LOW_LIMIT;


	//------------------------------------------------------------------------------
	// EXTERNAL PROPERTY

	bool bActive__DI_CDA_FLOW;
	CX__VAR_DIGITAL_CTRL dEXT_CH__DI_CDA_FLOW;

	bool bActive__DI_N2_FLOW;
	CX__VAR_DIGITAL_CTRL dEXT_CH__DI_N2_FLOW;

	bool bActive__DI_EFEM_DOOR_CLOSE;
	CX__VAR_DIGITAL_CTRL dEXT_CH__DI_EFEM_DOOR_CLOSE;

	bool bActive__AI_FFU_DIFF_PRESSURE;
	CX__VAR_ANALOG_CTRL  aEXT_CH__AI_FFU_DIFF_PRESSURE;


	//------------------------------------------------------------------------------
	void Mon__IO_MONITOR(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm);
	void Mon__N2_SENSOR_MONITOR(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);
	void Mon__EFEM_DOORS_MONITOR(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm);

	bool _Check__DEF_USE(const CString& str_def)
	{
		if(str_def.CompareNoCase("NO")   == 0)		return false;
		if(str_def.CompareNoCase("NONE") == 0)		return false;
		if(str_def.CompareNoCase("NULL") == 0)		return false;

		return true;
	}

public:
	CObj__SYSTEM();
	~CObj__SYSTEM();

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
