#include "StdAfx.h"
#include "CObj__ALx_STD.h"


//-------------------------------------------------------------------------
CObj__ALx_STD::CObj__ALx_STD()
{

}
CObj__ALx_STD::~CObj__ALx_STD()
{

}

//-------------------------------------------------------------------------
int CObj__ALx_STD::__DEFINE__CONTROL_MODE(obj,l_mode)
{
	sObject_Name = obj;

	// ...
	{
		ADD__CTRL_VAR(sMODE__INIT,  "INIT" );
		ADD__CTRL_VAR(sMODE__MAINT, "MAINT");
		ADD__CTRL_VAR(sMODE__ALIGN, "ALIGN");
	}
	return 1;
}
int CObj__ALx_STD::__DEFINE__VERSION_HISTORY(version)
{

	return 1;
}

int CObj__ALx_STD::__DEFINE__VARIABLE_STD(p_variable)
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
		STD__ADD_DIGITAL_WITH_COMMENT(str_name, dsc_item_list, "");
		LINK__VAR_DIGITAL_CTRL(dCH__OBJ_STATUS, str_name);
	}
	// ...
	{
		str_name = "PARA.SLOT";
		STD__ADD_DIGITAL(str_name, "1 2");
		LINK__VAR_DIGITAL_CTRL(dCH__PARA_SLOT, str_name);
	}
	return 1;
}
int CObj__ALx_STD::__DEFINE__ALARM(p_alarm)
{

	return 1;
}

int CObj__ALx_STD::__Define__USER_FUNCTION(CII_DEFINE__FUNCTION *p_fnc_ctrl)
{
	p_fnc_ctrl->Link__ERROR_FEEDBACK(&mERROR__USER_FNC);

	return 1;
}

//-------------------------------------------------------------------------
int CObj__ALx_STD::__INITIALIZE__OBJECT(p_variable,p_ext_obj_create)
{	
	CString def_name;
	CString obj_name;
	CString var_name;

	// ...
	{
		def_name = "OBJ__AL";
		p_variable->Get__DEF_CONST_DATA(def_name, obj_name);

		pAL__OBJ_CTRL = p_ext_obj_create->Create__OBJECT_CTRL(obj_name);

		//
		var_name = "PARA.SLOT";
		LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__ALx_PARA_SLOT, obj_name,var_name);
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
		dCH__OBJ_STATUS->Set__DATA(STR__MAINTMODE);
	}
	return 1;
}

//-------------------------------------------------------------------------
int CObj__ALx_STD::__CALL__CONTROL_MODE(mode,p_debug,p_variable,p_alarm)
{
	// ...
	{
		CString log_msg;

		log_msg.Format("Start ... :  [%s]",mode);
		xLOG_CTRL->WRITE__LOG(log_msg);
	}

	int flag = -1;

	// ...
	{
		     IF__CTRL_MODE(sMODE__INIT)			flag = Call__INIT(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__MAINT)		flag = Call__MAINT(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__ALIGN)		flag = Call__ALIGN(p_variable);

		else									
		{

		}
	}

	if((flag < 0)||(p_variable->Check__CTRL_ABORT() > 0))
	{
		dCH__OBJ_STATUS->Set__DATA("MAINTMODE");

		// ...
		CString log_msg;
		
		log_msg.Format("Aborted ... :  [%s]",mode);
		xLOG_CTRL->WRITE__LOG(log_msg);
	}
	else
	{
		CString log_msg;

		log_msg.Format("Completed ... :  [%s]",mode);
		xLOG_CTRL->WRITE__LOG(log_msg);
	}
	return flag;
}
int CObj__ALx_STD::__CALL__MONITORING(id,p_variable,p_alarm)
{

	return 1;
}
