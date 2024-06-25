#include "StdAfx.h"
#include "CObj__LPx_STD.h"


//-------------------------------------------------------------------------
CObj__LPx_STD::CObj__LPx_STD()
{

}
CObj__LPx_STD::~CObj__LPx_STD()
{

}

//-------------------------------------------------------------------------
int CObj__LPx_STD::__DEFINE__CONTROL_MODE(obj,l_mode)
{
	sObject_Name = obj;

	// ...
	{
		ADD__CTRL_VAR(sMODE__INIT,  "INIT");
		ADD__CTRL_VAR(sMODE__MAINT, "MAINT");

		ADD__CTRL_VAR(sMODE__HOME,  "HOME");
		ADD__CTRL_VAR(sMODE__ALARM_RESET, "ALARM.RESET");

		ADD__CTRL_VAR(sMODE__PREPLOAD, "PREPLOAD");
		ADD__CTRL_VAR(sMODE__LOAD,     "LOAD");

		ADD__CTRL_VAR(sMODE__RLSUNLOAD, "RLSUNLOAD");
		ADD__CTRL_VAR(sMODE__UNLOAD,    "UNLOAD");

		ADD__CTRL_VAR(sMODE__CYCLE, "CYCLE");

		ADD__CTRL_VAR(sMODE__CLAMP,   "CLAMP");
		ADD__CTRL_VAR(sMODE__UNCLAMP, "UNCLAMP");

		ADD__CTRL_VAR(sMODE__CID_READ,   "CID_READ");
		ADD__CTRL_VAR(sMODE__PAGE_READ,  "PAGE_READ");
		ADD__CTRL_VAR(sMODE__CID_WRITE,  "CID_WRITE");
		ADD__CTRL_VAR(sMODE__PAGE_WRITE, "PAGE_WRITE");

		ADD__CTRL_VAR(sMODE__MAP, "MAP");

		ADD__CTRL_VAR(sMODE__EXTENDPADDLE,  "EXTENDPADDLE");
		ADD__CTRL_VAR(sMODE__RETRACTPADDLE, "RETRACTPADDLE");

		ADD__CTRL_VAR(sMODE__PADDLE_DOWN, "PADDLEDOWN");
		ADD__CTRL_VAR(sMODE__PADDLE_UP,   "PADDLEUP");

		ADD__CTRL_VAR(sMODE__LATCH,     "LATCH");
		ADD__CTRL_VAR(sMODE__UNLATCH,   "UNLATCH");
		ADD__CTRL_VAR(sMODE__CLOSEDOOR, "CLOSEDOOR");
		ADD__CTRL_VAR(sMODE__OPENDOOR,  "OPENDOOR");
		
		ADD__CTRL_VAR(sMODE__VACON,  "VACON");
		ADD__CTRL_VAR(sMODE__VACOFF, "VACOFF");

		ADD__CTRL_VAR(sMODE__EXTENDSHUTTLE,  "SHUTTLEOUT");
		ADD__CTRL_VAR(sMODE__RETRACTSHUTTLE, "SHUTTLEIN");
	}
	return 1;
}
int CObj__LPx_STD::__DEFINE__VERSION_HISTORY(version)
{

	return 1;
}

int CObj__LPx_STD::__DEFINE__VARIABLE_STD(p_variable)
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
	}
	return 1;
}
int CObj__LPx_STD::__DEFINE__ALARM(p_alarm)
{

	return 1;
}

int CObj__LPx_STD::__Define__USER_FUNCTION(CII_DEFINE__FUNCTION *p_fnc_ctrl)
{
	p_fnc_ctrl->Link__ERROR_FEEDBACK(&mERROR__USER_FNC);

	return 1;
}

//-------------------------------------------------------------------------
int CObj__LPx_STD::__INITIALIZE__OBJECT(p_variable,p_ext_obj_create)
{
	CString def_name;
	CString def_data;
	CString str_name;

	// ...
	{
		def_name = "OBJ.LPx";
		p_variable->Get__DEF_CONST_DATA(def_name, def_data);

		pLPx__OBJ_CTRL = p_ext_obj_create->Create__OBJECT_CTRL(def_data);
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
int CObj__LPx_STD::__CALL__CONTROL_MODE(mode,p_debug,p_variable,p_alarm)
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

		ELSE_IF__CTRL_MODE(sMODE__HOME)				flag = Call__HOME(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__ALARM_RESET)		flag = Call__ALARM_RESET(p_variable);

		ELSE_IF__CTRL_MODE(sMODE__PREPLOAD)			flag = Call__PREPLOAD(mode,p_variable);
		ELSE_IF__CTRL_MODE(sMODE__LOAD)				flag = Call__LOAD(mode,p_variable);
		ELSE_IF__CTRL_MODE(sMODE__RLSUNLOAD)		flag = Call__RLSUNLOAD(mode,p_variable);
		ELSE_IF__CTRL_MODE(sMODE__UNLOAD)			flag = Call__UNLOAD(mode,p_variable);
		
		ELSE_IF__CTRL_MODE(sMODE__CYCLE)			flag = Call__CYCLE(mode,p_variable);

		ELSE_IF__CTRL_MODE(sMODE__CLAMP)			flag = Call__CLAMP(mode,p_variable);
		ELSE_IF__CTRL_MODE(sMODE__UNCLAMP)			flag = Call__UNCLAMP(mode,p_variable);

		ELSE_IF__CTRL_MODE(sMODE__MAP)				flag = Call__MAP(mode,p_variable);
		ELSE_IF__CTRL_MODE(sMODE__EXTENDPADDLE)		flag = Call__EXTENDPADDLE(mode,p_variable);
		ELSE_IF__CTRL_MODE(sMODE__RETRACTPADDLE)	flag = Call__RETRACTPADDLE(mode,p_variable);
		ELSE_IF__CTRL_MODE(sMODE__PADDLE_DOWN)		flag = Call__PADDLE_DOWN(mode,p_variable);
		ELSE_IF__CTRL_MODE(sMODE__PADDLE_UP)  		flag = Call__PADDLE_UP(mode,p_variable);

		ELSE_IF__CTRL_MODE(sMODE__LATCH)			flag = Call__LATCH(mode,p_variable);
		ELSE_IF__CTRL_MODE(sMODE__UNLATCH)			flag = Call__UNLATCH(mode,p_variable);
		
		ELSE_IF__CTRL_MODE(sMODE__CLOSEDOOR)		flag = Call__CLOSEDOOR(mode,p_variable);
		ELSE_IF__CTRL_MODE(sMODE__OPENDOOR)			flag = Call__OPENDOOR(mode,p_variable);
		
		ELSE_IF__CTRL_MODE(sMODE__VACON)			flag = Call__VACON(mode,p_variable);
		ELSE_IF__CTRL_MODE(sMODE__VACOFF)			flag = Call__VACOFF(mode,p_variable);
		
		ELSE_IF__CTRL_MODE(sMODE__EXTENDSHUTTLE)	flag = Call__EXTENDSHUTTLE(mode,p_variable);
		ELSE_IF__CTRL_MODE(sMODE__RETRACTSHUTTLE)	flag = Call__RETRACTSHUTTLE(mode,p_variable);
		ELSE_IF__CTRL_MODE(sMODE__HALT)				flag = Call__HALT(mode,p_variable);

		ELSE_IF__CTRL_MODE(sMODE__CID_READ)			flag = Call__CID_READ(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__PAGE_READ)		flag = Call__PAGE_READ(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__CID_WRITE)		flag = Call__CID_WRITE(p_variable);
		ELSE_IF__CTRL_MODE(sMODE__PAGE_WRITE)		flag = Call__PAGE_WRITE(p_variable);
	}

	if((flag < 0)||(p_variable->Check__CTRL_ABORT() > 0))
	{
		dCH__OBJ_STATUS->Set__DATA("MAINTMODE");

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
int CObj__LPx_STD::__CALL__MONITORING(id,p_variable,p_alarm)
{

	return 1;
}
