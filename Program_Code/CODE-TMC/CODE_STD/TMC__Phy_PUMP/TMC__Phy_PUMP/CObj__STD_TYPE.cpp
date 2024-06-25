#include "StdAfx.h"
#include "CObj__STD_TYPE.h"
#include "CObj__STD_TYPE__ALID.h"


//-------------------------------------------------------------------------
CObj__STD_TYPE::CObj__STD_TYPE()
{

}
CObj__STD_TYPE::~CObj__STD_TYPE()
{

}

//-------------------------------------------------------------------------
int CObj__STD_TYPE::__DEFINE__CONTROL_MODE(obj,l_mode)
{
	sObject_Name = obj;

	// ...
	{
		ADD__CTRL_VAR(sMODE__INIT,    "INIT");
		ADD__CTRL_VAR(sMODE__PUMP_ON, "PUMP_ON");
	}
	return 1;
}
int CObj__STD_TYPE::__DEFINE__VERSION_HISTORY(version)
{

	return 1;
}


// ...
#define  MON_ID__IO_MONITOR				1

// ...
#define  APP_DSP__CLOSE_OPEN			\
"UNKNOWN  CLOSE  OPEN"


int CObj__STD_TYPE::__DEFINE__VARIABLE_STD(p_variable)
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

	// ...
	{
		p_variable->Add__MONITORING_PROC(3.0, MON_ID__IO_MONITOR);
	}
	return 1;
}


// ...
#define  ACT__RETRY_ABORT						\
l_act.RemoveAll();								\
l_act.Add("RETRY");								\
l_act.Add("ABORT");


int CObj__STD_TYPE::__DEFINE__ALARM(p_alarm)
{
	DECLARE__ALARM;

	// ...
	CString title;
	title.Format("%s - ",sObject_Name);

	CString alarm_title;
	CString alarm_msg;
	CStringArray l_act;
	int alarm_id;

	// ...
	{
		alarm_id = ALID__PUMP_PRESSURE__TIMEOUT;

		alarm_title  = title;
		alarm_title += "Pump Pressure TimeOut.";

		alarm_msg = "Please, check pump pressure config !\n";

		ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}
	return 1;
}

int CObj__STD_TYPE::__Define__USER_FUNCTION(CII_DEFINE__FUNCTION *p_fnc_ctrl)
{
	p_fnc_ctrl->Link__ERROR_FEEDBACK(&mERROR__USER_FNC);

	return 1;
}

//-------------------------------------------------------------------------
int CObj__STD_TYPE::__INITIALIZE__OBJECT(p_variable,p_ext_obj_create)
{
	xI_ASYNC_TIMER->REGISTER__COUNT_CHANNEL(sObject_Name,aCH__TIME_COUNT->Get__VARIABLE_NAME());

	// ...
	CString def_name;
	CString def_data;
	CString str_name;
	CString obj_name;
	CString var_name;

	// DB OBJECT -----
	{
		def_name = "OBJ__DB";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name,def_data);

	}

	// PUMP OBJECT -----
	{
		def_name = "OBJ__PUMP";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name,def_data);

		pPUMP__OBJ_CTRL = p_ext_obj_create->Create__OBJECT_CTRL(def_data);

		// ...
		str_name = "sAPP.COMM.STS";
		LINK__EXT_VAR_STRING_CTRL(sEXT_CH__PUMP_COMM_STS, def_data,str_name);

		str_name = "OTR.OUT.MON.sPUMP.STS";
		LINK__EXT_VAR_STRING_CTRL(sEXT_CH__PUMP_STS, def_data,str_name);
	}

	// IO -----
	{
		def_name = "VAR__IO_AI_VACLINE_PRESS";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name,def_data);
		p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name, var_name);

		LINK__EXT_VAR_ANALOG_CTRL(aiEXT_CH__VACLINE_PRESS, obj_name,var_name);
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
	return 1;
}

//-------------------------------------------------------------------------
int CObj__STD_TYPE::__CALL__CONTROL_MODE(mode,p_debug,p_variable,p_alarm)
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
		ELSE_IF__CTRL_MODE(sMODE__PUMP_ON)		flag = Call__PUMP_ON(p_variable,p_alarm);
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

int CObj__STD_TYPE::__CALL__MONITORING(id,p_variable,p_alarm)
{
	switch(id)
	{
		case MON_ID__IO_MONITOR:
			Mon__IO_MONITOR(p_variable,p_alarm);
			break;
	}

	return 1;
}
