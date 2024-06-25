#pragma once

#include "Interface_Code.h"
#include "CCommon_System.h"


class CObj__DB_RCP : public __IOBJ__STD_TYPE
{
private:
	//-------------------------------------------------------------------------
	CString sObject_Name;


	//------------------------------------------------------------------------------
	//  INTERNAL PROPERTY

	// STEP RECIPE ...
	CX__VAR_STRING_CTRL  sCH__RCP_STEP_MESSAGE;
	CX__VAR_DIGITAL_CTRL dCH__RCP_STEP_MODE;
	CX__VAR_ANALOG_CTRL  aCH__RCP_STEP_TIME;

	CX__VAR_DIGITAL_CTRL dCH__RCP_APC_MODE;
	CX__VAR_ANALOG_CTRL  aCH__RCP_APC_PRESSURE;
	CX__VAR_ANALOG_CTRL  aCH__RCP_APC_POSITION;
	CX__VAR_ANALOG_CTRL  aCH__RCP_APC_PRESET_POS;
	CX__VAR_ANALOG_CTRL  aCH__RCP_APC_HOLD_DELAY;

	CX__VAR_ANALOG_CTRL  aCH__RCP_GAS_N2_FLOW;

	CX__VAR_DIGITAL_CTRL dCH__RCP_LIFT_PIN_MODE;

	CX__VAR_ANALOG_CTRL  aCH__RCP_LOOP_START;
	CX__VAR_ANALOG_CTRL  aCH__RCP_LOOP_REPEAT;

	// CONFIG RECIPE ...

	// CONFIG RANGE ...
	CX__VAR_ANALOG_CTRL  aCH__CFG_PARA_MFC_N2_RANGE_MIN;
	CX__VAR_ANALOG_CTRL  aCH__CFG_PARA_MFC_N2_RANGE_MAX;
	CX__VAR_ANALOG_CTRL  aCH__CFG_PARA_MFC_N2_RANGE_DEC;
	//

public:
	CObj__DB_RCP();
	~CObj__DB_RCP();

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
