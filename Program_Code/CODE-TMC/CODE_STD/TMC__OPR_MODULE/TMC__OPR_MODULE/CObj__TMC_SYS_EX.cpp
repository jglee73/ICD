#include "StdAfx.h"
#include "CObj__TMC_SYS_EX.h"


//-------------------------------------------------------------------------
CObj__TMC_SYS_EX::CObj__TMC_SYS_EX()
{

}
CObj__TMC_SYS_EX::~CObj__TMC_SYS_EX()
{

}

//-------------------------------------------------------------------------
int CObj__TMC_SYS_EX::__DEFINE__CONTROL_MODE(obj,l_mode)
{
	sObject_Name = obj;

	// ...
	{
		ADD__CTRL_VAR(sMODE__SYSTEM_INIT,  "SYSTEM_INIT");
		ADD__CTRL_VAR(sMODE__SYSTEM_MAINT, "SYSTEM_MAINT");
	}
	return 1;
}
int CObj__TMC_SYS_EX::__DEFINE__VERSION_HISTORY(version)
{

	return 1;
}

int CObj__TMC_SYS_EX::__DEFINE__VARIABLE_STD(p_variable)
{
	DECLARE__STD_VARIABLE;

	CCommon_Error__DEF_VARIABLE m_err_def_variable;
	p_variable->Link__DEF_VARIABLE__ERROR_FEEDBACK(&m_err_def_variable);

	// ...
	CString str_name;
	CString d_list;
	int i;

	// ...
	{
		p_variable->Get__STD_DESCRIPTION("STD_OBJ_STATUS", d_list);

		str_name = "OBJ.STATUS";
		STD__ADD_DIGITAL_WITH_COMMENT(str_name, d_list, "");
		LINK__VAR_DIGITAL_CTRL(dCH__OBJ_STATUS, str_name);
	}

	for(i=0; i<INIT_MSG_MAX; i++)
	{
		str_name.Format("GEN.Message_Initial%02d", i+1);
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__INIT_SEQ_MSG_X[i], str_name);
	}

	for(i=0; i<_CFG_SIZE__INTI_OBJ; i++)
	{
		str_name.Format("INIT.MSG.%1d", i+1);
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__INIT_MSG_X[i], str_name);
	}

	return 1;
}
int CObj__TMC_SYS_EX::__DEFINE__ALARM(p_alarm)
{

	return 1;
}

int CObj__TMC_SYS_EX::__Define__USER_FUNCTION(CII_DEFINE__FUNCTION *p_fnc_ctrl)
{
	p_fnc_ctrl->Link__ERROR_FEEDBACK(&mERROR__USER_FNC);

	return 1;
}

//-------------------------------------------------------------------------
int CObj__TMC_SYS_EX::__INITIALIZE__OBJECT(p_variable,p_ext_obj_create)
{
	CString def_name;
	CString def_data;
	CString str_name;
	CString var_data;

	CString obj_name, var_name;
	int i;

	// ...
	{
		def_name = "EXECUTE_MSG_POPUP";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);
		p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name, var_name);
		LINK__EXT_VAR_STRING_CTRL(sEXT_CH__GUI_POPUP_INIT_SEQ_MSG, obj_name, var_name);
	}

	// DB_CFG
	{
		def_name = "PM_SIZE";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);

		m_nPM_SIZE = atoi(def_data);
		if(m_nPM_SIZE > CFG_PM_LIMIT)			m_nPM_SIZE = CFG_PM_LIMIT;
	}

	// INIT_OBJ ...
	{
		def_name = "SIZE__OBJ_INIT";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);

		iSIZE__INIT_OBJ = atoi(def_data);
		if(iSIZE__INIT_OBJ > _CFG_SIZE__INTI_OBJ)		iSIZE__INIT_OBJ = _CFG_SIZE__INTI_OBJ;

		for(i=0; i<iSIZE__INIT_OBJ; i++)
		{
			def_name.Format("OBJ__INIT.NAME.%1d", i+1);
			p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);
			pOBJ_CTRL__INIT_X[i] = p_ext_obj_create->Create__OBJECT_CTRL(def_data);

			def_name.Format("OBJ__INIT.MSG.%1d", i+1);
			p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);
			sCH__INIT_MSG_X[i]->Set__DATA(def_data);
		}
	}

	// LPx
	{
		p_ext_obj_create->Get__DEF_CONST_DATA("LP_SIZE",var_data);

		iLPx_LIMIT = atoi(var_data);
		if(iLPx_LIMIT > CFG_LP_LIMIT)    iLPx_LIMIT = CFG_LP_LIMIT;

		for(i=0;i<iLPx_LIMIT;i++)
		{
			def_name.Format("OBJ__LP%1d",i+1);
			p_ext_obj_create->Get__DEF_CONST_DATA(def_name,def_data);

			pLPx__OBJ_CTRL[i] = p_ext_obj_create->Create__OBJECT_CTRL(def_data);

			str_name = "OBJ.STATUS";
			LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__LPx_OBJ_STATUS[i], def_data,str_name);
		}
	}

	// PMx Slot Valve ...
	{
		def_name = "OBJ__PMx_VLV";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);

		pPMx_SV__OBJ_CTRL = p_ext_obj_create->Create__OBJECT_CTRL(def_data);

		def_name = "OTR.IN.PARA.aPMC.ID";
		LINK__EXT_VAR_ANALOG_CTRL(aCH__PARA_PMC_ID, def_data, def_name);
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
int CObj__TMC_SYS_EX::__CALL__CONTROL_MODE(mode,p_debug,p_variable,p_alarm)
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
		IF__CTRL_MODE(sMODE__SYSTEM_INIT)		flag = Call__SYSTEM_INIT(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__SYSTEM_MAINT)		flag = Call__SYSTEM_MAINT(p_variable);

		else									
		{

		}
	}

	if((flag < 0)||(p_variable->Check__CTRL_ABORT() > 0))
	{
		dCH__OBJ_STATUS->Set__DATA("MAINTMODE");

		CString log_msg;

		log_msg.Format("Aborted ... :  [%s]", mode);
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
int CObj__TMC_SYS_EX::__CALL__MONITORING(id,p_variable,p_alarm)
{

	return 1;
}
