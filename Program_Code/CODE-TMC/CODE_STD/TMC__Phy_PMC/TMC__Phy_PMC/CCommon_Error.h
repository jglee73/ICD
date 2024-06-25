#pragma once

#include "Interface_Code.h"


class CCommon_Error__MODULE_OBJ : public CII__ERROR_FEEDBACK__MODULE_OBJ
{
public:
	CCommon_Error__MODULE_OBJ();
	~CCommon_Error__MODULE_OBJ();

	// ...
	int Call__ERROR__MODULE_OBJ(const CString& upper_obj,
								const CString& ch_ctrl,
								const CString& ch_mode,
								const CString& ch_status,
								const int err_type,
								const CString& err_msg,
								CII_OBJECT__ALARM *p_alarm);

	int Call__ERROR__ABORTED(const CString& upper_obj,
							 const CString& obj_mode,
							 CII_OBJECT__ALARM *p_alarm);

	int Call__ERROR__OFFLINE(const CString& upper_obj,
							 const CString& obj_mode,
							 CII_OBJECT__ALARM *p_alarm);

	int Call__ERROR__NOT_STANDBY(const CString& upper_obj,
								 const CString& obj_mode,
								 CII_OBJECT__ALARM *p_alarm);
};

class CCommon_Error__USER_FNC : public CII__ERROR_FEEDBACK__USER_FNC
{
private:
	CString sObj_Name;

public:
	CCommon_Error__USER_FNC();
	~CCommon_Error__USER_FNC();

	// ...
	void Set__OBJ_NAME(const CString& obj_name)
	{
		sObj_Name = obj_name;
	}

	// ...
	int Call__ERROR__EXT_FNC_MODE(const CString& fnc_name,
								  const CString& obj_name,
								  const CString& obj_mode,
								  const CString& err_msg,
								  CII_OBJECT__ALARM *p_alarm);

	int Call__ERROR__EXT_USER_FNC(const int fnc_type,
								  const CString& fnc_name,
								  const CString& obj_name,
								  const CString& obj_fnc,
								  const CString& err_msg,
								  CII_OBJECT__ALARM *p_alarm);

	// ...
	int Register__ERROR__EXT_FNC_MODE(const CString& mode_name);
	int Register__ERROR__EXT_FNC_VAR(const CString& var_name);

	int Register__ERROR__EXT_RET_INT(const CString& fnc_name);
	int Register__ERROR__EXT_RET_DOUBLE(const CString& fnc_name);
	int Register__ERROR__EXT_RET_STRING(const CString& fnc_name);
	int Register__ERROR__EXT_RET_CONDITION(const CString& fnc_name);

	int Register__ERROR__EXT_RET_INT__INT_STRING(const CString& fnc_name);
	int Register__ERROR__EXT_RET_INT__STRING_STRING(const CString& fnc_name);
	int Register__ERROR__EXT_RET_STRING__INT(const CString& fnc_name);
	int Register__ERROR__EXT_RET_STRING__STRING(const CString& fnc_name);
};

class CCommon_Error__DEF_VARIABLE : public CII__ERROR_FEEDBACK__DEF_VARIABLE
{
public:
	CCommon_Error__DEF_VARIABLE();
	~CCommon_Error__DEF_VARIABLE();

	// ...
	int Call__ERROR__GET_DEF_DIGITAL_DESCRIPTION(const CString& obj_name,const CString& dsc_name);

	int Call__ERROR__GET_DEF_CONST_DATA(const CString& obj_name,const CString& def_var);

	int Call__ERROR__GET_CHANNEL_TYPE_CTRL(const CString& ch_type,const CString& obj_name,const CString& var_name);
};
