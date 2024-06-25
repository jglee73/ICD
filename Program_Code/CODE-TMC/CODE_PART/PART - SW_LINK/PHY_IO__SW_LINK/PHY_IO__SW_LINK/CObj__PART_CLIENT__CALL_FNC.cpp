#include "stdafx.h"
#include "CObj__PART_CLIENT.h"


// ...
int CObj__PART_CLIENT::
Call__INIT(CII_OBJECT__VARIABLE *p_variable, CII_OBJECT__ALARM *p_alarm)
{

	return 1;
}

// ...
int CObj__PART_CLIENT::
Call__TRANSFER_START(CII_OBJECT__VARIABLE *p_variable, CII_OBJECT__ALARM *p_alarm)
{
	CString str_cmd = _CMD__ACT_TRANSFER_START;

	return Fnc__COMMAND_SEND(p_alarm, str_cmd);
}

int CObj__PART_CLIENT::
Call__TRANSFER_END(CII_OBJECT__VARIABLE *p_variable, CII_OBJECT__ALARM *p_alarm)
{
	CString str_cmd = _CMD__ACT_TRANSFER_END;

	return Fnc__COMMAND_SEND(p_alarm, str_cmd);
}

//
int CObj__PART_CLIENT::
Call__PIN_UP(CII_OBJECT__VARIABLE *p_variable, CII_OBJECT__ALARM *p_alarm)
{
	CString str_cmd = _CMD__ACT_PIN_UP;

	return Fnc__COMMAND_SEND(p_alarm, str_cmd);
}

int CObj__PART_CLIENT::
Call__PIN_DOWN(CII_OBJECT__VARIABLE *p_variable, CII_OBJECT__ALARM *p_alarm)
{
	CString str_cmd = _CMD__ACT_PIN_DOWN;

	return Fnc__COMMAND_SEND(p_alarm, str_cmd);
}

//
int CObj__PART_CLIENT::
Call__CHAMBER_PUMP(CII_OBJECT__VARIABLE *p_variable, CII_OBJECT__ALARM *p_alarm)
{
	CString str_cmd = _CMD__ACT_CHAMBER_PUMP;

	return Fnc__COMMAND_SEND(p_alarm, str_cmd);
}

int CObj__PART_CLIENT::
Call__CHAMBER_VENT(CII_OBJECT__VARIABLE *p_variable, CII_OBJECT__ALARM *p_alarm)
{
	CString str_cmd = _CMD__ACT_CHAMBER_VENT;

	return Fnc__COMMAND_SEND(p_alarm, str_cmd);
}

int CObj__PART_CLIENT::
Call__CHAMBER_ABORT(CII_OBJECT__VARIABLE *p_variable, CII_OBJECT__ALARM *p_alarm)
{
	CString str_cmd = _CMD__ACT_CHAMBER_ABORT;

	return Fnc__COMMAND_SEND(p_alarm, str_cmd);
}

//
int CObj__PART_CLIENT::
Call__PROCESS_START(CII_OBJECT__VARIABLE *p_variable, CII_OBJECT__ALARM *p_alarm)
{
	CString str_cmd = _CMD__ACT_PROCESS_START;

	return Fnc__COMMAND_SEND(p_alarm, str_cmd);
}

int CObj__PART_CLIENT::
Call__CLEAN_START(CII_OBJECT__VARIABLE *p_variable, CII_OBJECT__ALARM *p_alarm)
{
	CString str_cmd = _CMD__ACT_CLEAN_START;

	return Fnc__COMMAND_SEND(p_alarm, str_cmd);
}

int CObj__PART_CLIENT::
Call__PROCESS_ABORT(CII_OBJECT__VARIABLE *p_variable, CII_OBJECT__ALARM *p_alarm)
{
	CString str_cmd = _CMD__ACT_PROCESS_ABORT;

	return Fnc__COMMAND_SEND(p_alarm, str_cmd);
}

// ...
int CObj__PART_CLIENT::
Fnc__COMMAND_SEND(CII_OBJECT__ALARM *p_alarm, const CString& str_cmd)
{
	int r_flag = doCH__Control_SET->Set__DATA(str_cmd);

	// Error Check 
	if(r_flag < 0)
	{

	}
	return r_flag;
}
