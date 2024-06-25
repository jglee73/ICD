#include "StdAfx.h"
#include "CObj__TMC_CHM_MainFold_X.h"


//-------------------------------------------------------------------------
CObj__TMC_CHM_MainFold_X::CObj__TMC_CHM_MainFold_X()
{

}
CObj__TMC_CHM_MainFold_X::~CObj__TMC_CHM_MainFold_X()
{

}

//-------------------------------------------------------------------------
int CObj__TMC_CHM_MainFold_X::__DEFINE__CONTROL_MODE(obj,l_mode)
{
	sObject_Name = obj;

	// ...
	{
		ADD__CTRL_VAR(sMODE__INIT,  "INIT");
		ADD__CTRL_VAR(sMODE__MAINT, "MAINT");

		ADD__CTRL_VAR(sMODE__PUMP,  "PUMP");
		ADD__CTRL_VAR(sMODE__VENT,  "VENT");

		ADD__CTRL_VAR(sMODE__ISOLATE,     "ISOLATE");
		ADD__CTRL_VAR(sMODE__LEAK_CHECK,  "LEAK_CHECK");
		ADD__CTRL_VAR(sMODE__CYCLE_PURGE, "CYCLE_PURGE");
	}
	return 1;
}
int CObj__TMC_CHM_MainFold_X::__DEFINE__VERSION_HISTORY(version)
{

	return 1;
}


// ...
#define MON_ID__IO_MONITOR				1


int CObj__TMC_CHM_MainFold_X::__DEFINE__VARIABLE_STD(p_variable)
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
		LINK__VAR_DIGITAL_CTRL(dCH__OBJ_STATUS,str_name);

		str_name = "sOBJ.SEQ.MSG";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__OBJ_SEQ_MSG,str_name);
	}

	// ...
	{
		p_variable->Add__MONITORING_PROC(1.0, MON_ID__IO_MONITOR);
	}
	return 1;
}
int CObj__TMC_CHM_MainFold_X::__DEFINE__ALARM(p_alarm)
{

	return 1;
}

int CObj__TMC_CHM_MainFold_X::__Define__USER_FUNCTION(CII_DEFINE__FUNCTION *p_fnc_ctrl)
{
	p_fnc_ctrl->Link__ERROR_FEEDBACK(&mERROR__USER_FNC);

	return 1;
}

//-------------------------------------------------------------------------
int CObj__TMC_CHM_MainFold_X::__INITIALIZE__OBJECT(p_variable,p_ext_obj_create)
{
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
	CString def_name;
	CString def_data;
	CString obj_name;
	CString str_name;
	int i;

	// TMC_CHM -----
	{
		def_name = "OBJ__TMC_CHM";
		p_variable->Get__DEF_CONST_DATA(def_name, obj_name);

		pTMC_CHM__OBJ_CTRL = p_ext_obj_create->Create__OBJECT_CTRL(obj_name);

		//
		str_name = "OTR.OUT.MON.OBJ.STATUS";
		LINK__EXT_VAR_STRING_CTRL(sEXT_CH__TMC_CHM__OBJ_STATUS, obj_name,str_name);
	}

	// TMC_VLV -----
	{
		def_name = "OBJ__TMC_VLV";
		p_variable->Get__DEF_CONST_DATA(def_name, obj_name);

		pTMC_VLV__OBJ_CTRL = p_ext_obj_create->Create__OBJECT_CTRL(obj_name);
	}

	// LLx -----
	{
		def_name = "DATA.LLx_SIZE";
		p_variable->Get__DEF_CONST_DATA(def_name, def_data);

		iDATA__LLx_SIZE = atoi(def_data);
		if(iDATA__LLx_SIZE > CFG_LLx_LIMIT)			iDATA__LLx_SIZE = CFG_LLx_LIMIT;

		for(i=0; i<iDATA__LLx_SIZE; i++)
		{
			def_name.Format("LINK_LLx.OBJ_FNC.%1d", i+1);
			p_variable->Get__DEF_CONST_DATA(def_name, obj_name);

			pLLx__OBJ_CTRL[i] = p_ext_obj_create->Create__OBJECT_CTRL(obj_name);
		}
	}

	// ...
	{
		dCH__OBJ_STATUS->Set__DATA(STR__MAINTMODE);
	}
	return 1;
}

//-------------------------------------------------------------------------
int CObj__TMC_CHM_MainFold_X::__CALL__CONTROL_MODE(mode,p_debug,p_variable,p_alarm)
{
	CString log_msg;

	// ...
	{
		log_msg.Format("Start ... :  [%s]",mode);

		xLOG_CTRL->WRITE__LOG(log_msg);
		sCH__OBJ_SEQ_MSG->Set__DATA(log_msg);
	}

	int flag = -1;

	// ...
	{
		IF__CTRL_MODE(sMODE__INIT)				flag = Call__INIT(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__MAINT)			flag = Call__MAINT(p_variable);

		ELSE_IF__CTRL_MODE(sMODE__PUMP)				flag = Call__PUMP(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__VENT)				flag = Call__VENT(p_variable);

		ELSE_IF__CTRL_MODE(sMODE__ISOLATE)			flag = Call__ISOLATE(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__LEAK_CHECK)		flag = Call__LEAK_CHECK(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__CYCLE_PURGE)		flag = Call__CYCLE_PURGE(p_variable);

		else									
		{

		}
	}

	if((flag < 0)||(p_variable->Check__CTRL_ABORT() > 0))
	{
		dCH__OBJ_STATUS->Set__DATA("MAINTMODE");

		log_msg.Format("Aborted ... :  [%s]",mode);
		xLOG_CTRL->WRITE__LOG(log_msg);
	}
	else
	{
		log_msg.Format("Completed ... :  [%s]",mode);
		xLOG_CTRL->WRITE__LOG(log_msg);
	}

	sCH__OBJ_SEQ_MSG->Set__DATA(log_msg);
	return flag;
}
int CObj__TMC_CHM_MainFold_X::__CALL__MONITORING(id,p_variable,p_alarm)
{
	switch(id)
	{
	case MON_ID__IO_MONITOR:
		Mon__IO_MONITOR(p_variable,p_alarm);
		break;
	}

	return 1;
}
