#include "StdAfx.h"
#include "CObj_Opr__PMC_STD.h"


//-------------------------------------------------------------------------
CObj_Opr__PMC_STD::CObj_Opr__PMC_STD()
{

}
CObj_Opr__PMC_STD::~CObj_Opr__PMC_STD()
{

}

//-------------------------------------------------------------------------
int CObj_Opr__PMC_STD::__DEFINE__CONTROL_MODE(obj,l_mode)
{
	sObject_Name = obj;

	// ...
	{
		CString str_name;

		str_name = "PRO_START";
		ADD__CTRL_VAR(sMODE__PRO_START,str_name);
	}
	return 1;
}
int CObj_Opr__PMC_STD::__DEFINE__VERSION_HISTORY(version)
{

	return 1;
}

int CObj_Opr__PMC_STD::__DEFINE__VARIABLE_STD(p_variable)
{
	DECLARE__STD_VARIABLE;

	// ...
	CString str_name;

	sVAR__RECIPE = "RECIPE";
	STD__ADD_STRING_WITH_COMMENT(sVAR__RECIPE, "");

	return 1;
}

int CObj_Opr__PMC_STD::__DEFINE__ALARM(p_alarm)
{
	DECLARE__ALARM;

	// ...
	CString title;
	title.Format("%s [CObj_Opr__PMC_STD] : ",sObject_Name);

	CString alarm_title;
	CString alarm_msg;
	CStringArray l_act;

	// ...
	{
		alarm_title  = title;
		alarm_title += "OBJECT MODE : UNKNOWN";

		alarm_msg = "";

		l_act.RemoveAll();
		l_act.Add("ABORT");

		ADD__ALARM_EX(ALID__OBJECT_MODE_UNKNOWN,alarm_title,alarm_msg,l_act);
	}
	return 1;
}

int CObj_Opr__PMC_STD::__Define__USER_FUNCTION(CII_DEFINE__FUNCTION *p_fnc_ctrl)
{
	mERROR__USER_FNC.Set__OBJ_NAME(sObject_Name);
	p_fnc_ctrl->Link__ERROR_FEEDBACK(&mERROR__USER_FNC);

	// ...
	CString str_name;

	str_name = "OBJ_MODE__PRO_READY";
	ADD__EXT_FNC_MODE_WITH_COMMENT(sEXT_MODE__PRO_READY,str_name,"");

	str_name = "OBJ_MODE__PRO_START";
	ADD__EXT_FNC_MODE_WITH_COMMENT(sEXT_MODE__PRO_START,str_name,"");

	return 1;
}

//-------------------------------------------------------------------------
int CObj_Opr__PMC_STD::__INITIALIZE__OBJECT(p_variable,p_ext_obj_create)
{

	return 1;
}

//-------------------------------------------------------------------------
int CObj_Opr__PMC_STD::__CALL__CONTROL_MODE(mode,p_debug,p_variable,p_alarm)
{
	int flag = -1;

	IF__CTRL_MODE(sMODE__PRO_START)		flag = Call__PRO_START(p_variable);

	else
	{
		CString bff;
		CString alarm_msg;
		CString r_act;

		bff.Format("Object Name : %s\n",sObject_Name);
		alarm_msg  = bff;
		bff.Format("Unknown Object Mode : \"%s\"\n",mode);
		alarm_msg += bff;

		p_alarm->Popup__ALARM_With_MESSAGE(ALID__OBJECT_MODE_UNKNOWN,alarm_msg,r_act);
	}

	return flag;
}

int CObj_Opr__PMC_STD::__CALL__MONITORING(id,p_variable,p_alarm)
{

	return 1;
}
