#include "StdAfx.h"
#include "CObj__VIS_STD.h"


//-------------------------------------------------------------------------
CObj__VIS_STD::CObj__VIS_STD()
{

}
CObj__VIS_STD::~CObj__VIS_STD()
{

}

//-------------------------------------------------------------------------
int CObj__VIS_STD::__DEFINE__CONTROL_MODE(obj,l_mode)
{
	sObject_Name = obj;

	// ...
	{
		ADD__CTRL_VAR(sMODE__INIT,  "INIT");
		ADD__CTRL_VAR(sMODE__MAINT, "MAINT");

		ADD__CTRL_VAR(sMODE__PRO_READY, "PRO_READY");
		ADD__CTRL_VAR(sMODE__PRO_START, "PRO_START");

		ADD__CTRL_VAR(sMODE__PLACE_READY,   "PLACE_READY");
		ADD__CTRL_VAR(sMODE__PLACE_COMPLETE,"PLACE_COMPLETE");

		ADD__CTRL_VAR(sMODE__PICK_READY,   "PICK_READY");
		ADD__CTRL_VAR(sMODE__PICK_COMPLETE,"PICK_COMPLETE");
	}
	return 1;
}
int CObj__VIS_STD::__DEFINE__VERSION_HISTORY(version)
{

	return 1;
}

int CObj__VIS_STD::__DEFINE__VARIABLE_STD(p_variable)
{
	DECLARE__STD_VARIABLE;

	CCommon_Error__DEF_VARIABLE m_err_def_variable;
	p_variable->Link__DEF_VARIABLE__ERROR_FEEDBACK(&m_err_def_variable);

	// ...
	CString str_name;
	CString dsc_item_list;

	// ...
	{
		p_variable->Get__STD_DESCRIPTION("STD_OBJ_STATUS",dsc_item_list);

		str_name = "OBJ.STATUS";
		STD__ADD_DIGITAL_WITH_COMMENT(str_name,dsc_item_list,"");
		LINK__VAR_DIGITAL_CTRL(xCH__OBJ_STATUS,str_name);
	}
	return 1;
}
int CObj__VIS_STD::__DEFINE__ALARM(p_alarm)
{

	return 1;
}

int CObj__VIS_STD::__Define__USER_FUNCTION(CII_DEFINE__FUNCTION *p_fnc_ctrl)
{
	p_fnc_ctrl->Link__ERROR_FEEDBACK(&mERROR__USER_FNC);

	return 1;
}

//-------------------------------------------------------------------------
int CObj__VIS_STD::__INITIALIZE__OBJECT(p_variable,p_ext_obj_create)
{
	CString def_name;
	CString def_data;
	CString str_name;

	// ...
	{
		def_name = "OBJ__VIS";
		p_variable->Get__DEF_CONST_DATA(def_name, def_data);

		pVIS__OBJ_CTRL = p_ext_obj_create->Create__OBJECT_CTRL(def_data);
	}

	// ...
	{
		CString file_name;
		CString log_msg;

		file_name.Format("%s_App.log", sObject_Name);

		log_msg  = "\n\n";
		log_msg += "//------------------------------------------------------------------------";

		xLOG_CTRL->CREATE__SUB_DIRECTORY(sObject_Name);
		xLOG_CTRL->SET__PROPERTY(file_name,24*5,60);

		xLOG_CTRL->DISABLE__TIME_LOG();
		xLOG_CTRL->WRITE__LOG(log_msg);

		xLOG_CTRL->ENABLE__TIME_LOG();
		xLOG_CTRL->WRITE__LOG("   START   \n");
	}

	// ...
	{
		xCH__OBJ_STATUS->Set__DATA(STR__MAINTMODE);
	}
	return 1;
}

//-------------------------------------------------------------------------
int CObj__VIS_STD::__CALL__CONTROL_MODE(mode,p_debug,p_variable,p_alarm)
{
	// ...
	{
		CString log_msg;

		log_msg.Format("Start ... :  [%s]", mode);
		xLOG_CTRL->WRITE__LOG(log_msg);
	}

	int flag = -1;

	// ...
	{
			 IF__CTRL_MODE(sMODE__INIT)				flag = Call__INIT(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__MAINT)			flag = Call__MAINT(p_variable);

		ELSE_IF__CTRL_MODE(sMODE__PRO_READY)		flag = Call__PRO_READY(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__PRO_START)		flag = Call__PRO_START(p_variable);
		
		ELSE_IF__CTRL_MODE(sMODE__PLACE_READY)		flag = Call__PLACE_READY(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__PLACE_COMPLETE)	flag = Call__PLACE_COMPLETE(p_variable);
		
		ELSE_IF__CTRL_MODE(sMODE__PICK_READY)		flag = Call__PICK_READY(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__PICK_COMPLETE)	flag = Call__PICK_COMPLETE(p_variable);

		else									
		{

		}
	}

	if((flag < 0)||(p_variable->Check__CTRL_ABORT() > 0))
	{
		xCH__OBJ_STATUS->Set__DATA("MAINTMODE");

		// ...
		CString log_msg;

		log_msg.Format("Aborted ... :  [%s]", mode);
		xLOG_CTRL->WRITE__LOG(log_msg);
	}
	else
	{
		CString log_msg;

		log_msg.Format("Completed ... :  [%s]", mode);
		xLOG_CTRL->WRITE__LOG(log_msg);
	}
	return flag;
}
int CObj__VIS_STD::__CALL__MONITORING(id,p_variable,p_alarm)
{

	return 1;
}
