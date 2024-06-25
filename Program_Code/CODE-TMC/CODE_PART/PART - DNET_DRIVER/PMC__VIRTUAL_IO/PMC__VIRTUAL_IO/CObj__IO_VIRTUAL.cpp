#include "StdAfx.h"
#include "CObj__IO_VIRTUAL.h"


//--------------------------------------------------------------------------------
CObj__IO_VIRTUAL::CObj__IO_VIRTUAL()
{

}
CObj__IO_VIRTUAL::~CObj__IO_VIRTUAL()
{

}


//--------------------------------------------------------------------------------
int CObj__IO_VIRTUAL::__DEFINE__CONTROL_MODE(obj, l_mode)
{
	sObject_Name = obj;

	return 1;
}
int CObj__IO_VIRTUAL::__DEFINE__VERSION_HISTORY(version)
{

	return 1;
}

int CObj__IO_VIRTUAL::__DEFINE__VARIABLE_STD(p_variable)
{
	DECLARE__STD_VARIABLE

	// ...
	CString var_name;

	// ...
	{
		var_name = "OBJ.MSG";
		STD__ADD_STRING(var_name);
		LINK__VAR_STRING_CTRL(sCH__OBJ_MSG, var_name);

		//
		var_name = "MON.COMM.STATE";
		STD__ADD_STRING(var_name);
		LINK__VAR_STRING_CTRL(sCH__MON_COMM_STATE, var_name);
	}

	return 1;
}
int CObj__IO_VIRTUAL::__DEFINE__ALARM(p_alarm)
{
	DECLARE__ALARM;

	return 1;
}

int CObj__IO_VIRTUAL::__DEFINE__VARIABLE_IO(p_io_variable)
{
	DECLARE__IO_VARIABLE;

	return 1;
}


//--------------------------------------------------------------------------------
int CObj__IO_VIRTUAL::__INITIALIZE__OBJECT(p_variable,p_ext_obj_create)
{

	return 1;
}
int CObj__IO_VIRTUAL::__INITIALIZE__IO(p_io_para)
{
	printf("%s : __INITIALIZE__IO() \n", sObject_Name);

	// ...
	{
		sCH__MON_COMM_STATE->Set__DATA("ONLINE");
	}
	return 1;
}


//--------------------------------------------------------------------------------
int CObj__IO_VIRTUAL
::__CALL__CONTROL_MODE(mode, p_debug, p_variable, p_alarm)
{
	CString msg;
	int flag = -1;

	// ...
	{
		msg.Format("%s Started...",mode);
		sCH__OBJ_MSG->Set__DATA(msg);
	}

	// ...
	{

	}

	if((flag < 0)||(p_variable->Check__CTRL_ABORT() > 0))
	{
		msg.Format("%s Aborted (%1d)", mode, flag);
	}
	else
	{
		msg.Format("%s Completed (%1d)", mode, flag);
	}	

	// ...
	{
		sCH__OBJ_MSG->Set__DATA(msg);
	}
	return flag;
}

int CObj__IO_VIRTUAL::__CALL__MONITORING(id, p_variable, p_alarm)
{

	return 1;
}


//--------------------------------------------------------------------------------
int CObj__IO_VIRTUAL
::__CLOSE__OBJECT()
{

	return 1;
}
