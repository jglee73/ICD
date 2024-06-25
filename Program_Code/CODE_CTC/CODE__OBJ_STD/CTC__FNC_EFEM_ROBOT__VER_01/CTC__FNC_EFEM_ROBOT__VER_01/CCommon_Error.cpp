#include "StdAfx.h"
#include "CCommon_Error.h"


//-----------------------------------------------------------------------------------
CCommon_Error__MODULE_OBJ::CCommon_Error__MODULE_OBJ()
{

}
CCommon_Error__MODULE_OBJ::~CCommon_Error__MODULE_OBJ()
{

}

// ...
int CCommon_Error__MODULE_OBJ
::Call__ERROR__MODULE_OBJ(const CString& upper_obj,
							const CString& ch_ctrl,
							const CString& ch_mode,
							const CString& ch_status,
							const int err_type,
							const CString& err_msg,
							CII_OBJECT__ALARM *p_alarm)
{
	CString msg;
	CString bff;

	bff.Format("Upper Object : %s\n",upper_obj);
	msg += bff;

	bff.Format("Ctrl Channel : %s\n",ch_ctrl);
	msg += bff;

	bff.Format("Mode Channel : %s\n",ch_mode);
	msg += bff;

	bff.Format("Status Channel : %s\n",ch_status);
	msg += bff;

	bff.Format("Error Type : %1d\n",err_type);
	msg += bff;

	bff.Format("Error MSG : [%s]\n",err_msg);
	msg += bff;

	MessageBox(NULL,msg,"CCommon_Error__MODULE_OBJ::Call__ERROR__MODULE_OBJ()",MB_OK|MB_TOPMOST|MB_ICONERROR);
	return 1;
}

//
int CCommon_Error__MODULE_OBJ
::Call__ERROR__ABORTED(const CString& upper_obj,
					   const CString& obj_mode,
					   CII_OBJECT__ALARM *p_alarm)
{
	CString msg;
	CString bff;

	bff.Format("Upper Object : %s\n",upper_obj);
	msg += bff;

	bff.Format("Object Mode  : %s\n",obj_mode);
	msg += bff;

	MessageBox(NULL,msg,"CCommon_Error__MODULE_OBJ::Call__ERROR__ABORTED()",MB_OK|MB_TOPMOST|MB_ICONERROR);
	return 1;
}

int CCommon_Error__MODULE_OBJ
::Call__ERROR__OFFLINE(const CString& upper_obj,
					   const CString& obj_mode,
					   CII_OBJECT__ALARM *p_alarm)
{
	CString msg;
	CString bff;

	bff.Format("Upper Object : %s\n",upper_obj);
	msg += bff;

	bff.Format("Object Mode  : %s\n",obj_mode);
	msg += bff;

	MessageBox(NULL,msg,"CCommon_Error__MODULE_OBJ::Call__ERROR__OFFLINE()",MB_OK|MB_TOPMOST|MB_ICONERROR);
	return 1;
}

int CCommon_Error__MODULE_OBJ
::Call__ERROR__NOT_STANDBY(const CString& upper_obj,
						   const CString& obj_mode,
						   CII_OBJECT__ALARM *p_alarm)
{
	CString msg;
	CString bff;

	bff.Format("Upper Object : %s\n",upper_obj);
	msg += bff;

	bff.Format("Object Mode  : %s\n",obj_mode);
	msg += bff;

	MessageBox(NULL,msg,"CCommon_Error__MODULE_OBJ::Call__ERROR__NOT_STANDBY()",MB_OK|MB_TOPMOST|MB_ICONERROR);
	return 1;
}


//------------------------------------------------------------------------------------------------
CCommon_Error__USER_FNC::CCommon_Error__USER_FNC()
{

}
CCommon_Error__USER_FNC::~CCommon_Error__USER_FNC()
{

}

// ...
int CCommon_Error__USER_FNC
::Call__ERROR__EXT_FNC_MODE(const CString& fnc_name,
							const CString& obj_name,
							const CString& obj_mode,
							const CString& err_msg,
							CII_OBJECT__ALARM *p_alarm)
{
	CString msg;
	CString bff;

	bff.Format("Fnc_Name : %s\n",fnc_name);
	msg += bff;

	bff.Format("Obj_Name : %s\n",obj_name);
	msg += bff;

	bff.Format("Obj_Mode : %s\n",obj_mode);
	msg += bff;

	bff.Format("Err_Msg  : %s\n",err_msg);
	msg += bff;

	MessageBox(NULL,msg,"CCommon_Error__USER_FNC - Call__ERROR__EXT_FNC_MODE()",MB_OK|MB_TOPMOST);
	return 1;
}
int CCommon_Error__USER_FNC
::Call__ERROR__EXT_USER_FNC(const int fnc_type,
							const CString& fnc_name,
							const CString& obj_name,
							const CString& obj_fnc,
							const CString& err_msg,
							CII_OBJECT__ALARM *p_alarm)
{
	CString msg;
	CString bff;

	msg += "FNC_TYPE : ";

	switch(fnc_type)
	{
		case USER_FNC_TYPE__RET_INT:		
			msg += "FNC_RET_INT\n";
			break;

		case USER_FNC_TYPE__RET_DOUBLE:
			msg += "FNC_RET_DOUBLE\n";
			break;

		case USER_FNC_TYPE__RET_STRING:
			msg += "FNC_RET_STRING\n";
			break;

		case USER_FNC_TYPE__RET_CONDITION:
			msg += "FNC_RET_CONDITION\n";
			break;

		default:
			msg += "???\n";
			break;
	}

	bff.Format("Fnc_Name : %s\n",fnc_name);
	msg += bff;

	bff.Format("Obj_Name : %s\n",obj_name);
	msg += bff;

	bff.Format("Obj_Fnc  : %s\n",obj_fnc);
	msg += bff;

	bff.Format("Err_Msg  : %s\n",err_msg);
	msg += bff;

	MessageBox(NULL,msg,"CCommon_Error__USER_FNC - Call__ERROR__EXT_USER_FNCE()",MB_OK|MB_TOPMOST);
	return 1;
}

// ...
int CCommon_Error__USER_FNC
::Register__ERROR__EXT_FNC_MODE(const CString& mode_name)
{
	CString msg;
	msg.Format("Mode Name : %s",mode_name);

	CString title;
	title.Format("%s - CCommon_Error__USER_FNC::Register__ERROR__EXT_FNC_MODE()",sObj_Name);

	MessageBox(NULL,msg,title,MB_OK|MB_TOPMOST|MB_ICONERROR);
	return 1;
}
int CCommon_Error__USER_FNC
::Register__ERROR__EXT_FNC_VAR(const CString& var_name)
{
	CString msg;
	msg.Format("Variable Name : %s",var_name);

	CString title;
	title.Format("%s - CCommon_Error__USER_FNC::Register__ERROR__EXT_FNC_MODE()",sObj_Name);

	MessageBox(NULL,msg,title,MB_OK|MB_TOPMOST|MB_ICONERROR);
	return 1;
}

int CCommon_Error__USER_FNC
::Register__ERROR__EXT_RET_INT(const CString& fnc_name)
{
	CString msg;
	msg.Format("Fnc Name : %s",fnc_name);

	CString title;
	title.Format("%s - CCommon_Error__USER_FNC::Register__ERROR__EXT_FNC_MODE()",sObj_Name);

	MessageBox(NULL,msg,title,MB_OK|MB_TOPMOST|MB_ICONERROR);
	return 1;
}
int CCommon_Error__USER_FNC
::Register__ERROR__EXT_RET_DOUBLE(const CString& fnc_name)
{
	CString msg;
	msg.Format("Fnc Name : %s",fnc_name);

	CString title;
	title.Format("%s - CCommon_Error__USER_FNC::Register__ERROR__EXT_FNC_MODE()",sObj_Name);

	MessageBox(NULL,msg,title,MB_OK|MB_TOPMOST|MB_ICONERROR);
	return 1;
}
int CCommon_Error__USER_FNC
::Register__ERROR__EXT_RET_STRING(const CString& fnc_name)
{
	CString msg;
	msg.Format("Fnc Name : %s",fnc_name);

	CString title;
	title.Format("%s - CCommon_Error__USER_FNC::Register__ERROR__EXT_FNC_MODE()",sObj_Name);

	MessageBox(NULL,msg,title,MB_OK|MB_TOPMOST|MB_ICONERROR);
	return 1;
}
int CCommon_Error__USER_FNC
::Register__ERROR__EXT_RET_CONDITION(const CString& fnc_name)
{
	CString msg;
	msg.Format("Fnc Name : %s",fnc_name);

	CString title;
	title.Format("%s - CCommon_Error__USER_FNC::Register__ERROR__EXT_FNC_MODE()",sObj_Name);

	MessageBox(NULL,msg,title,MB_OK|MB_TOPMOST|MB_ICONERROR);
	return 1;
}

int CCommon_Error__USER_FNC
::Register__ERROR__EXT_RET_INT__INT_STRING(const CString& fnc_name)
{
	CString msg;
	msg.Format("Fnc Name : %s",fnc_name);

	CString title;
	title.Format("%s - CCommon_Error__USER_FNC::Register__ERROR__EXT_FNC_MODE()",sObj_Name);

	MessageBox(NULL,msg,title,MB_OK|MB_TOPMOST|MB_ICONERROR);
	return 1;
}
int CCommon_Error__USER_FNC
::Register__ERROR__EXT_RET_INT__STRING_STRING(const CString& fnc_name)
{
	CString msg;
	msg.Format("Fnc Name : %s",fnc_name);

	CString title;
	title.Format("%s - CCommon_Error__USER_FNC::Register__ERROR__EXT_FNC_MODE()",sObj_Name);

	MessageBox(NULL,msg,title,MB_OK|MB_TOPMOST|MB_ICONERROR);
	return 1;
}
int CCommon_Error__USER_FNC
::Register__ERROR__EXT_RET_STRING__INT(const CString& fnc_name)
{
	CString msg;
	msg.Format("Fnc Name : %s",fnc_name);

	CString title;
	title.Format("%s - CCommon_Error__USER_FNC::Register__ERROR__EXT_FNC_MODE()",sObj_Name);

	MessageBox(NULL,msg,title,MB_OK|MB_TOPMOST|MB_ICONERROR);
	return 1;
}
int CCommon_Error__USER_FNC
::Register__ERROR__EXT_RET_STRING__STRING(const CString& fnc_name)
{
	CString msg;
	msg.Format("Fnc Name : %s",fnc_name);

	CString title;
	title.Format("%s - CCommon_Error__USER_FNC::Register__ERROR__EXT_FNC_MODE()",sObj_Name);

	MessageBox(NULL,msg,title,MB_OK|MB_TOPMOST|MB_ICONERROR);
	return 1;
}


//------------------------------------------------------------------------------------------------
CCommon_Error__DEF_VARIABLE::CCommon_Error__DEF_VARIABLE()
{

}
CCommon_Error__DEF_VARIABLE::~CCommon_Error__DEF_VARIABLE()
{

}

// ...
int CCommon_Error__DEF_VARIABLE
::Call__ERROR__GET_DEF_DIGITAL_DESCRIPTION(const CString& obj_name,const CString& dsc_name)
{
	CString msg;
	CString bff;

	bff.Format("Object Name : %s\n",obj_name);
	msg += bff;

	bff.Format("Description Name : %s\n",dsc_name);
	msg += bff;

	MessageBox(NULL,msg,"CCommon_Error__DEF_VARIABLE::Call__ERROR__GET_DEF_DIGITAL_DESCRIPTION()",MB_OK|MB_TOPMOST|MB_ICONERROR);
	return 1;
}

int CCommon_Error__DEF_VARIABLE
::Call__ERROR__GET_DEF_CONST_DATA(const CString& obj_name,const CString& def_var)
{
	CString msg;
	CString bff;

	bff.Format("Object Name : %s\n",obj_name);
	msg += bff;

	bff.Format("Define Variable : %s\n",def_var);
	msg += bff;

	MessageBox(NULL,msg,"CCommon_Error__DEF_VARIABLE::Call__ERROR__GET_DEF_CONST_DATA()",MB_OK|MB_TOPMOST|MB_ICONERROR);
	return 1;
}

int CCommon_Error__DEF_VARIABLE
::Call__ERROR__GET_CHANNEL_TYPE_CTRL(const CString& ch_type,const CString& obj_name,const CString& var_name)
{
	CString msg;
	CString bff;

	bff.Format("Channel Type : %s\n",ch_type);
	msg += bff;

	bff.Format("Object Name : %s\n",obj_name);
	msg += bff;

	bff.Format("Variable Name : %s\n",var_name);
	msg += bff;

	MessageBox(NULL,msg,"CCommon_Error__DEF_VARIABLE::Call__ERROR__GET_CHANNEL_TYPE_CTRL()",MB_OK|MB_TOPMOST|MB_ICONERROR);
	return 1;
}
