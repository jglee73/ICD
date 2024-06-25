#include "StdAfx.h"
#include "CObj__VIRTUAL_TYPE.h"
#include "CObj__VIRTUAL_TYPE__DEF.h"


//-------------------------------------------------------------------------
CObj__VIRTUAL_TYPE::CObj__VIRTUAL_TYPE()
{

}
CObj__VIRTUAL_TYPE::~CObj__VIRTUAL_TYPE()
{

}

//-------------------------------------------------------------------------
int CObj__VIRTUAL_TYPE::__DEFINE__CONTROL_MODE(obj,l_mode)
{
	sObject_Name = obj;

	// ...
	{
		ADD__CTRL_VAR(sMODE__INIT,  "INIT");
		ADD__CTRL_VAR(sMODE__MAINT, "MAINT");

		ADD__CTRL_VAR(sMODE__PUMP_ON,  "PUMP_ON");
		ADD__CTRL_VAR(sMODE__PUMP_OFF, "PUMP_OFF");
	}
	return 1;
}
int CObj__VIRTUAL_TYPE::__DEFINE__VERSION_HISTORY(version)
{

	return 1;
}


int CObj__VIRTUAL_TYPE::__DEFINE__VARIABLE_STD(p_variable)
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

		str_name = "INR.sMSG";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__SEQUENCE_MSG,str_name);

		// ...
		str_name = "OTR.IN.CFG.aPUMP.TIMEOUT";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"sec",0,0,9999,"");
		LINK__VAR_ANALOG_CTRL(aCH__CFG_PUMP_TIMEOUT, str_name);

		str_name = "OTR.IN.CFG.aPUMP.PRESSURE.TORR";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name,"torr",3,0,300,"");
		LINK__VAR_ANALOG_CTRL(aCH__CFG_PUMP_PRESSURE_TORR, str_name);

		// ...
		str_name = "OTR.OUT.MON.aPRESSURE.TORR";
		STD__ADD_ANALOG_WITH_COMMENT(str_name,"torr",3,0,1000,"");
		LINK__VAR_ANALOG_CTRL(aCH__PRESSURE_TORR, str_name);

		// ...
		str_name = "aTIME.COUNT";
		STD__ADD_ANALOG_WITH_COMMENT(str_name,"sec",0,0,9999,"");
		LINK__VAR_ANALOG_CTRL(aCH__TIME_COUNT,str_name);

		// ...
		str_name = "MON.PUMP.COMM_STS";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__MON_PUMP_COMM_STS,str_name);

		str_name = "MON.PUMP.RUN_STS";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__MON_PUMP_RUN_STS,str_name);
	}

	return 1;
}


int CObj__VIRTUAL_TYPE::__DEFINE__ALARM(p_alarm)
{
	DECLARE__ALARM;

	return 1;
}

int CObj__VIRTUAL_TYPE::__Define__USER_FUNCTION(CII_DEFINE__FUNCTION *p_fnc_ctrl)
{
	p_fnc_ctrl->Link__ERROR_FEEDBACK(&mERROR__USER_FNC);

	return 1;
}

//-------------------------------------------------------------------------
int CObj__VIRTUAL_TYPE::__INITIALIZE__OBJECT(p_variable,p_ext_obj_create)
{
	xI_ASYNC_TIMER->REGISTER__COUNT_CHANNEL(sObject_Name,aCH__TIME_COUNT->Get__VARIABLE_NAME());

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
		sCH__MON_PUMP_COMM_STS->Set__DATA("ONLINE");
	}
	return 1;
}

//-------------------------------------------------------------------------
int CObj__VIRTUAL_TYPE::__CALL__CONTROL_MODE(mode,p_debug,p_variable,p_alarm)
{
	// ...
	{
		CString log_msg;
		log_msg.Format("Start ... :  [%s]", mode);

		sCH__SEQUENCE_MSG->Set__DATA(log_msg);
		xLOG_CTRL->WRITE__LOG(log_msg);
	}

	int flag = -1;

	// ...
	{
			 IF__CTRL_MODE(sMODE__INIT)			flag = Call__INIT(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__MAINT)		flag = Call__MAINT(p_variable,p_alarm);

		ELSE_IF__CTRL_MODE(sMODE__PUMP_ON)		flag = Call__PUMP_ON(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__PUMP_OFF)		flag = Call__PUMP_OFF(p_variable,p_alarm);
	}

	xI_ASYNC_TIMER->STOP();

	if((flag < 0)||(p_variable->Check__CTRL_ABORT() > 0))
	{
		CString log_msg;
		log_msg.Format("Aborted ... :  [%s]",mode);

		sCH__SEQUENCE_MSG->Set__DATA(log_msg);
		xLOG_CTRL->WRITE__LOG(log_msg);
		dCH__OBJ_STATUS->Set__DATA(STR__MAINTMODE);
	}
	else
	{
		CString log_msg;
		log_msg.Format("Completed ... :  [%s]",mode);

		sCH__SEQUENCE_MSG->Set__DATA(log_msg);
		xLOG_CTRL->WRITE__LOG(log_msg);
	}
	return flag;
}

int CObj__VIRTUAL_TYPE::__CALL__MONITORING(id,p_variable,p_alarm)
{

	return 1;
}
