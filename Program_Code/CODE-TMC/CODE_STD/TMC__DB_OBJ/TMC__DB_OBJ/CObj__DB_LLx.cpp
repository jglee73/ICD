#include "StdAfx.h"
#include "CObj__DB_LLx.h"


//-------------------------------------------------------------------------
CObj__DB_LLx::CObj__DB_LLx()
{

}
CObj__DB_LLx::~CObj__DB_LLx()
{

}

//-------------------------------------------------------------------------
int CObj__DB_LLx::__DEFINE__CONTROL_MODE(obj,l_mode)
{
	sObject_Name = obj;

	return 1;
}
int CObj__DB_LLx::__DEFINE__VERSION_HISTORY(version)
{

	return 1;
}

int CObj__DB_LLx::__DEFINE__VARIABLE_STD(p_variable)
{
	DECLARE__STD_VARIABLE;

	// ...
	CString dl__slot_sts;
	p_variable->Get__STD_DESCRIPTION(DSP__SLOT_STATUS, dl__slot_sts);

	// ...
	CString var_name;

	// MNT_PROC ...
	{
		var_name = "SYSTEM.MSG";
		STD__ADD_STRING(var_name);
		LINK__VAR_STRING_CTRL(sCH__SYSTEM_MSG, var_name);

		var_name = "FUNCTION.MSG";		
		STD__ADD_STRING(var_name);
		LINK__VAR_STRING_CTRL(sCH__FNC_MSG, var_name);
	}

	// SLOT INFO ...
	{
		var_name = "SLOT01.RECIPE";
		STD__ADD_STRING(var_name);
		LINK__VAR_STRING_CTRL(sCH__SLOT01_RCP_NAME, var_name);

		var_name = "SLOT01.STATUS";
		STD__ADD_DIGITAL(var_name, dl__slot_sts);
		LINK__VAR_DIGITAL_CTRL(dCH__SLOT01_STATUS, var_name);

		var_name = "SLOT01.TITLE";
		STD__ADD_STRING(var_name);
		LINK__VAR_STRING_CTRL(sCH__SLOT01_TITLE, var_name);

		var_name = "SLOT01.LOTID";
		STD__ADD_STRING(var_name);
		LINK__VAR_STRING_CTRL(sCH__SLOT01_LOTID, var_name);

		var_name = "SLOT01.MATERIALID";
		STD__ADD_STRING(var_name);
		LINK__VAR_STRING_CTRL(sCH__SLOT01_MATERIALID, var_name);

		var_name = "SLOT01.PORTID";
		STD__ADD_STRING(var_name);
		LINK__VAR_STRING_CTRL(sCH__SLOT01_PORTID, var_name);

		var_name = "SLOT01.SLOTID";
		STD__ADD_STRING(var_name);
		LINK__VAR_STRING_CTRL(sCH__SLOT01_SLOTID, var_name);
	}

	// SYSTEM_PROCESS INFO ...
	{
		var_name = "SYSTEM.PROCESS.FLAG";
		STD__ADD_STRING(var_name);
		LINK__VAR_STRING_CTRL(sCH__SYSTEM_PROCESS_FLAG, var_name);

		var_name = "SYSTEM.PROCESS.STEP.ID";
		STD__ADD_STRING(var_name);
		LINK__VAR_STRING_CTRL(sCH__SYSTEM_PROCESS_STEP_ID, var_name);

		var_name = "SYSTEM.PROCESS.LAST.STEP.FLAG";
		STD__ADD_STRING(var_name);
		LINK__VAR_STRING_CTRL(sCH__SYSTEM_PROCESS_LAST_STEP_FLAG, var_name);
	}

	// SYSTEM_MESSAGE INFO ...
	{
		var_name = "SYS.CTRL.NAME";
		STD__ADD_STRING(var_name);
		LINK__VAR_STRING_CTRL(sCH__SYS_CTRL_NAME, var_name);

		var_name = "SYS.CTRL.MSG";
		STD__ADD_STRING(var_name);
		LINK__VAR_STRING_CTRL(sCH__SYS_CTRL_MSG, var_name);

		var_name = "SYS.CTRL.LOCK";
		STD__ADD_STRING(var_name);
		LINK__VAR_STRING_CTRL(sCH__SYS_CTRL_LOCK, var_name);

		var_name = "SYS.ALWAYS.LOCK.FLAG";
		STD__ADD_STRING(var_name);
		LINK__VAR_STRING_CTRL(sCH__SYS_ALWAYS_LOCK_FLAG, var_name);

		//
		var_name = "SYS.FNC.NAME";
		STD__ADD_STRING(var_name);
		LINK__VAR_STRING_CTRL(sCH__SYS_FNC_NAME, var_name);

		var_name = "SYS.FNC.MSG";
		STD__ADD_STRING(var_name);
		LINK__VAR_STRING_CTRL(sCH__SYS_FNC_MSG, var_name);

		var_name = "SYS.FNC.LOCK";
		STD__ADD_STRING(var_name);
		LINK__VAR_STRING_CTRL(sCH__SYS_FNC_LOCK, var_name);
	}

	return 1;
}

int CObj__DB_LLx::__DEFINE__ALARM(p_alarm)
{
	DECLARE__ALARM;

	return 1;
}

//-------------------------------------------------------------------------
int CObj__DB_LLx::__INITIALIZE__OBJECT(p_variable,p_ext_obj_create)
{

	return 1;
}

//-------------------------------------------------------------------------
int CObj__DB_LLx::__CALL__CONTROL_MODE(mode,p_debug,p_variable,p_alarm)
{

	return 1;
}

int CObj__DB_LLx::__CALL__MONITORING(id,p_variable,p_alarm)
{

	return 1;
}
