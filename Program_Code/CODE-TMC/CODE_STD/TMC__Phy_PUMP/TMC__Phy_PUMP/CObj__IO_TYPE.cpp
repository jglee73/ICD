#include "StdAfx.h"
#include "CObj__IO_TYPE.h"
#include "CObj__IO_TYPE__ALID.h"


//-------------------------------------------------------------------------
CObj__IO_TYPE::CObj__IO_TYPE()
{

}
CObj__IO_TYPE::~CObj__IO_TYPE()
{

}

//-------------------------------------------------------------------------
int CObj__IO_TYPE::__DEFINE__CONTROL_MODE(obj,l_mode)
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
int CObj__IO_TYPE::__DEFINE__VERSION_HISTORY(version)
{

	return 1;
}


// ...
#define  MON_ID__IO_MONITOR				1

// ...
#define  APP_DSP__CLOSE_OPEN			\
"UNKNOWN  CLOSE  OPEN"


int CObj__IO_TYPE::__DEFINE__VARIABLE_STD(p_variable)
{
	DECLARE__STD_VARIABLE;

	CCommon_Error__DEF_VARIABLE m_err_def_variable;
	p_variable->Link__DEF_VARIABLE__ERROR_FEEDBACK(&m_err_def_variable);

	// ...
	CString str_name;
	CString list_item;

	// ...
	{
		p_variable->Get__STD_DESCRIPTION("STD_OBJ_STATUS",list_item);

		str_name = "OBJ.STATUS";
		STD__ADD_DIGITAL_WITH_COMMENT(str_name, list_item, "");
		LINK__VAR_DIGITAL_CTRL(dCH__OBJ_STATUS, str_name);

		str_name = "OBJ.MSG";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__OBJ_MSG, str_name);
	}

	// ...
	{
		str_name = "CFG.ON.DELAY";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "sec", 0, 0, 100, "");
		LINK__VAR_ANALOG_CTRL(aCH__CFG_ON_DELAY, str_name);

		str_name = "APP.DELAY.COUNT";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__APP_DELAY_COUNT, str_name);
	}	

	// ...
	{
		str_name = "MON.PUMP.COMM_STS";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__MON_PUMP_COMM_STS, str_name);

		str_name = "MON.PUMP.RUN_STS";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__MON_PUMP_RUN_STS, str_name);

		str_name = "MON.PUMP.STATE";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__MON_PUMP_STATE, str_name);
	}

	// ...
	{
		p_variable->Add__MONITORING_PROC(3.0, MON_ID__IO_MONITOR);
	}
	return 1;
}


// ...
#define  _LIST_ACT__RETRY_ABORT					\
l_act.RemoveAll();								\
l_act.Add("RETRY");								\
l_act.Add("ABORT");

#define  _LIST_ACT__CHECK						\
l_act.RemoveAll();								\
l_act.Add("CHECK");


int CObj__IO_TYPE::__DEFINE__ALARM(p_alarm)
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
		alarm_id = ALID__PUMP_STATE_RUNNING;

		alarm_title  = title;
		alarm_title += "The state of pump is not running !";

		alarm_msg = "Please, check the state of pump !\n";

		_LIST_ACT__CHECK;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}
	// ...
	{
		alarm_id = ALID__PUMP_BLOWER_RUNNING;

		alarm_title  = title;
		alarm_title += "The state of pump blower is not running !";

		alarm_msg = "Please, check the state of pump !\n";

		_LIST_ACT__CHECK;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}
	// ...
	{
		alarm_id = ALID__PUMP_WARNING_NORMAL;

		alarm_title  = title;
		alarm_title += "The state of pump warning is not normal !";

		alarm_msg = "Please, check the state of pump !\n";

		_LIST_ACT__CHECK;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}
	// ...
	{
		alarm_id = ALID__PUMP_ALARM_NORMAL;

		alarm_title  = title;
		alarm_title += "The state of pump alarm is not normal !";

		alarm_msg = "Please, check the state of pump !\n";

		_LIST_ACT__CHECK;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}
	// ...
	{
		alarm_id = ALID__PUMP_OPERATION_READY;

		alarm_title  = title;
		alarm_title += "The state of pump operation is not ready !";

		alarm_msg = "Please, check the state of pump !\n";

		_LIST_ACT__CHECK;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}
	// ...
	{
		alarm_id = ALID__PUMP_OPERATION_READY;

		alarm_title  = title;
		alarm_title += "The state of pump operation is not ready !";

		alarm_msg = "Please, check the state of pump !\n";

		_LIST_ACT__CHECK;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}
	// ...
	{
		alarm_id = ALID__PUMP_ALARM_ERROR;

		alarm_title  = title;
		alarm_title += "The state of pump alarm is error !";

		alarm_msg = "Please, check the state of pump !\n";

		_LIST_ACT__CHECK;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__INIT_ERROR;

		alarm_title  = title;
		alarm_title += "Pump Initial Error !";

		alarm_msg = "Please, check the state of pump !\n";

		_LIST_ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}
	// ...
	{
		alarm_id = ALID__PUMP_VLV_NOT_CLOSE;

		alarm_title  = title;
		alarm_title += "Pump valve is not close !";

		alarm_msg = "Please, check the pump valve !\n";

		_LIST_ACT__RETRY_ABORT;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	return 1;
}

int CObj__IO_TYPE::__Define__USER_FUNCTION(CII_DEFINE__FUNCTION *p_fnc_ctrl)
{
	p_fnc_ctrl->Link__ERROR_FEEDBACK(&mERROR__USER_FNC);

	return 1;
}

//-------------------------------------------------------------------------
int CObj__IO_TYPE::__INITIALIZE__OBJECT(p_variable,p_ext_obj_create)
{
	CString def_name;
	CString def_data;
	CString obj_name;
	CString var_name;

	// IO : DO ...
	{
		def_name = "CH.DO_PUMP_POWER";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name,def_data);
		p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name, var_name);
		LINK__EXT_VAR_DIGITAL_CTRL(doEXT_CH__PUMP_POWER, obj_name,var_name);

		def_name = "CH.DO_PUMP_IDLE_MODE";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name,def_data);
		p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name, var_name);
		LINK__EXT_VAR_DIGITAL_CTRL(doEXT_CH__PUMP_IDLE_MODE, obj_name,var_name);

		def_name = "CH.DO_PUMP_REMOTE_MODE";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name,def_data);
		p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name, var_name);
		LINK__EXT_VAR_DIGITAL_CTRL(doEXT_CH__PUMP_REMOTE_MODE, obj_name,var_name);
	}
	// IO : DI ...
	{
		def_name = "CH.DI_PUMP_STATE_RUNNING";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name,def_data);
		p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name, var_name);
		LINK__EXT_VAR_DIGITAL_CTRL(diEXT_CH__PUMP_STATE_RUNNING, obj_name,var_name);

		def_name = "CH.DI_PUMP_BLOWER_RUNNING";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name,def_data);
		p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name, var_name);
		LINK__EXT_VAR_DIGITAL_CTRL(diEXT_CH__PUMP_BLOWER_RUNNING, obj_name,var_name);
		
		def_name = "CH.DI_PUMP_WARNING_NORMAL";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name,def_data);
		p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name, var_name);
		LINK__EXT_VAR_DIGITAL_CTRL(diEXT_CH__PUMP_WARNING_NORMAL, obj_name,var_name);
		
		def_name = "CH.DI_PUMP_ALARM_NORMAL";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name,def_data);
		p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name, var_name);
		LINK__EXT_VAR_DIGITAL_CTRL(diEXT_CH__PUMP_ALARM_NORMAL, obj_name,var_name);
		
		def_name = "CH.DI_PUMP_OPERATION_READY";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name,def_data);
		p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name, var_name);
		LINK__EXT_VAR_DIGITAL_CTRL(diEXT_CH__PUMP_OPERATION_READY, obj_name,var_name);

		//
		def_name = "CH.DI_PUMP_ALARM_ERROR";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name,def_data);
		p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name, var_name);
		LINK__EXT_VAR_DIGITAL_CTRL(diEXT_CH__PUMP_ALARM_ERROR, obj_name,var_name);
	}

	// IO : VLV ...
	{
		def_name = "CH.DI_PUMP_SLOW_VALVE";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name,def_data);
		p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name, var_name);
		LINK__EXT_VAR_DIGITAL_CTRL(diEXT_CH__PUMP_SLOW_VLV, obj_name,var_name);

		def_name = "CH.DI_PUMP_FAST_VALVE";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name,def_data);
		p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name, var_name);
		LINK__EXT_VAR_DIGITAL_CTRL(diEXT_CH__PUMP_FAST_VLV, obj_name,var_name);
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
int CObj__IO_TYPE::__CALL__CONTROL_MODE(mode,p_debug,p_variable,p_alarm)
{
	// ...
	{
		CString log_msg;
		log_msg.Format("Start ... :  [%s]", mode);

		sCH__OBJ_MSG->Set__DATA(log_msg);
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

	if((flag < 0)||(p_variable->Check__CTRL_ABORT() > 0))
	{
		CString log_msg;
		log_msg.Format("Aborted ... :  [%s]",mode);

		sCH__OBJ_MSG->Set__DATA(log_msg);
		xLOG_CTRL->WRITE__LOG(log_msg);

		dCH__OBJ_STATUS->Set__DATA(STR__MAINTMODE);
	}
	else
	{
		CString log_msg;
		log_msg.Format("Completed ... :  [%s]",mode);

		sCH__OBJ_MSG->Set__DATA(log_msg);
		xLOG_CTRL->WRITE__LOG(log_msg);
	}
	return flag;
}

int CObj__IO_TYPE::__CALL__MONITORING(id,p_variable,p_alarm)
{
	switch(id)
	{
		case MON_ID__IO_MONITOR:
			Mon__IO_MONITOR(p_variable,p_alarm);
			break;
	}
	return 1;
}
