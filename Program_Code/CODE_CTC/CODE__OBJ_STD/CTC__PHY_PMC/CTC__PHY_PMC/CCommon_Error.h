#pragma once

#include "Interface_Code.h"


// ...
#define  ALARM_DEFINE__MODULE_OBJECT										\
l_act.RemoveAll();															\
l_act.Add("CHECK");															\
																			\
alarm_title  = title;														\
alarm_title += "MODULE OBJECT - ABORTED";									\
ADD__ALARM_EX(ALID__MODULE_OBJ__ABORTED,alarm_title,alarm_msg,l_act);		\
																			\
alarm_title  = title;														\
alarm_title += "MODULE OBJECT - OFFLINE";									\
ADD__ALARM_EX(ALID__MODULE_OBJ__OFFLINE,alarm_title,alarm_msg,l_act);		\
																			\
alarm_title  = title;														\
alarm_title += "MODULE OBJECT - NOT STANDBY";								\
ADD__ALARM_EX(ALID__MODULE_OBJ__NOT_STANDBY,alarm_title,alarm_msg,l_act);	\
																			\
alarm_title  = title;														\
alarm_title += "MODULE OBJECT - CALL ERROR";								\
ADD__ALARM_EX(ALID__MODULE_OBJ__CALL_ERROR,alarm_title,alarm_msg,l_act);

// ...
#define  ALID__MODULE_OBJ__ABORTED								1
#define  ALID__MODULE_OBJ__OFFLINE								2
#define  ALID__MODULE_OBJ__NOT_STANDBY							3
#define  ALID__MODULE_OBJ__CALL_ERROR							4


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
