#include "StdAfx.h"
#include "CObj__DB_RCP.h"


//-------------------------------------------------------------------------
CObj__DB_RCP::CObj__DB_RCP()
{

}
CObj__DB_RCP::~CObj__DB_RCP()
{

}

//-------------------------------------------------------------------------
int CObj__DB_RCP::__DEFINE__CONTROL_MODE(obj,l_mode)
{
	sObject_Name = obj;

	return 1;
}
int CObj__DB_RCP::__DEFINE__VERSION_HISTORY(version)
{

	return 1;
}

int CObj__DB_RCP::__DEFINE__VARIABLE_STD(p_variable)
{
	DECLARE__STD_VARIABLE;

	// ...
	CString dl__slot_sts;
	p_variable->Get__STD_DESCRIPTION(DSP__SLOT_STATUS, dl__slot_sts);

	// ...
	CString var_name;

	// RECIPE ...
	{
		var_name = "RCP.STEP.MESSAGE";
		STD__ADD_STRING(var_name);
		LINK__VAR_STRING_CTRL(sCH__RCP_STEP_MESSAGE, var_name);

		var_name = "RCP.STEP.MODE";
		STD__ADD_DIGITAL(var_name, "Time Stable End");
		LINK__VAR_DIGITAL_CTRL(dCH__RCP_STEP_MODE, var_name);

		var_name = "RCP.STEP.TIME";
		STD__ADD_ANALOG(var_name, "sec", 1, 0, 9999);
		LINK__VAR_ANALOG_CTRL(aCH__RCP_STEP_TIME, var_name);

		//
		var_name = "RCP.APC.MODE";
		STD__ADD_DIGITAL(var_name, "Position Pressure");
		LINK__VAR_DIGITAL_CTRL(dCH__RCP_APC_MODE, var_name);

		var_name = "RCP.APC.PRESSURE";
		STD__ADD_ANALOG(var_name, "torr", 3, 0, 10);
		LINK__VAR_ANALOG_CTRL(aCH__RCP_APC_PRESSURE, var_name);
		
		var_name = "RCP.APC.POSITION";
		STD__ADD_ANALOG(var_name, "%", 3, 0, 100);
		LINK__VAR_ANALOG_CTRL(aCH__RCP_APC_POSITION, var_name);

		var_name = "RCP.APC.PRESET.POS";
		STD__ADD_ANALOG(var_name, "%", 3, 0, 100);
		LINK__VAR_ANALOG_CTRL(aCH__RCP_APC_PRESET_POS, var_name);

		var_name = "RCP.APC.HOLD.DELAY";
		STD__ADD_ANALOG(var_name, "sec", 1, 0, 10);
		LINK__VAR_ANALOG_CTRL(aCH__RCP_APC_HOLD_DELAY, var_name);

		//
		var_name = "RCP.GAS.N2.FLOW";
		STD__ADD_ANALOG(var_name, "sccm", 3, 0, 10000);
		LINK__VAR_ANALOG_CTRL(aCH__RCP_GAS_N2_FLOW, var_name);

		//
		var_name = "RCP.LIFT_PIN.MODE";
		STD__ADD_DIGITAL(var_name, "Down Up");
		LINK__VAR_DIGITAL_CTRL(dCH__RCP_LIFT_PIN_MODE, var_name);

		//
		var_name = "RCP.LOOP_START";
		STD__ADD_ANALOG(var_name, "step", 0, 0, 100);
		LINK__VAR_ANALOG_CTRL(aCH__RCP_LOOP_START, var_name);

		var_name = "RCP.LOOP_REPEAT";
		STD__ADD_ANALOG(var_name, "step", 0, 0, 1000);
		LINK__VAR_ANALOG_CTRL(aCH__RCP_LOOP_REPEAT, var_name);
	}

	// CONFIG RECIPE ...
	{

	}

	// CONFGI RANGE ...
	{
		var_name = "CFG.PARA.MFC.N2.RANGE.MIN";
		STD__ADD_ANALOG_WITH_X_OPTION(var_name, "sccm", 0, 0, 0, "");
		LINK__VAR_ANALOG_CTRL(aCH__CFG_PARA_MFC_N2_RANGE_MIN, var_name);

		var_name = "CFG.PARA.MFC.N2.RANGE.MAX";
		STD__ADD_ANALOG_WITH_X_OPTION(var_name, "sccm", 0, 0, 10000, "");
		LINK__VAR_ANALOG_CTRL(aCH__CFG_PARA_MFC_N2_RANGE_MAX, var_name);

		var_name = "CFG.PARA.MFC.N2.RANGE.DEC";
		STD__ADD_ANALOG_WITH_X_OPTION(var_name, "dec", 0, 0, 5, "");
		LINK__VAR_ANALOG_CTRL(aCH__CFG_PARA_MFC_N2_RANGE_DEC, var_name);
	}

	return 1;
}

int CObj__DB_RCP::__DEFINE__ALARM(p_alarm)
{
	DECLARE__ALARM;

	return 1;
}

//-------------------------------------------------------------------------
int CObj__DB_RCP::__INITIALIZE__OBJECT(p_variable,p_ext_obj_create)
{

	return 1;
}

//-------------------------------------------------------------------------
int CObj__DB_RCP::__CALL__CONTROL_MODE(mode,p_debug,p_variable,p_alarm)
{

	return 1;
}

int CObj__DB_RCP::__CALL__MONITORING(id,p_variable,p_alarm)
{

	return 1;
}
