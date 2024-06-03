#include "StdAfx.h"
#include "CObj__MADEE_IO.h"
#include "CObj__MADEE_IO__ALID.h"


//-------------------------------------------------------------------------
CObj__MADEE_IO::CObj__MADEE_IO()
{

}
CObj__MADEE_IO::~CObj__MADEE_IO()
{

}

//-------------------------------------------------------------------------
int CObj__MADEE_IO::__DEFINE__CONTROL_MODE(obj,l_mode)
{
	sObject_Name = obj;

	// ...
	{
		ADD__CTRL_VAR(sMODE__INIT, "INIT");

		ADD__CTRL_VAR(sMODE__ALL_CLOSE,    "ALL_CLOSE");
		ADD__CTRL_VAR(sMDDE__CONTROL_CFG,  "CONTROL.CFG");
		ADD__CTRL_VAR(sMDDE__CONTROL,      "CONTROL");
	}
	return 1;
}
int CObj__MADEE_IO::__DEFINE__VERSION_HISTORY(version)
{

	return 1;
}


// ...
#define MON_ID__STATUS							1
#define MON_ID__STABLE							2


int CObj__MADEE_IO::__DEFINE__VARIABLE_STD(p_variable)
{
	DECLARE__STD_VARIABLE;

	CCommon_Error__DEF_VARIABLE m_err_def_variable;
	p_variable->Link__DEF_VARIABLE__ERROR_FEEDBACK(&m_err_def_variable);

	// ...
	CString str_name;
	CString item_list;

	// OBJ ...
	{
		str_name = "APP.OBJ.MSG";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__OBJ_MSG, str_name);

		str_name = "APP.OBJ.STATUS";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__OBJ_STATUS, str_name);
	}

	// CFG ...
	{
		str_name = "CFG.SET_RATIO.MASTER";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "%", 0, 0.0, 100.0, "");
		LINK__VAR_ANALOG_CTRL(aCH__CFG_SET_RATIO_MASTER, str_name);
	}

	// PARA ...
	{
		str_name = "PARA.SET_RATIO.MASTER";
		STD__ADD_ANALOG_WITH_OPTION(str_name, "%", 0, 0.0, 100.0, 1, "L", "");
		LINK__VAR_ANALOG_CTRL(aCH__PARA_SET_RATIO_MASTER, str_name);

		str_name = "PARA.SET_RATIO.SLAVE";
		STD__ADD_ANALOG_WITH_OPTION(str_name, "%", 0, 0.0, 100.0, 1, "L", "");
		LINK__VAR_ANALOG_CTRL(aCH__PARA_SET_RATIO_SLAVE, str_name);
	}

	// ...
	{
		str_name = "MON.EXCEPTION.STATE";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__MON_EXCEPTION_STATE, str_name);

		str_name = "MON.CONTROL.MODE";
		STD__ADD_STRING(str_name);
		LINK__VAR_STRING_CTRL(sCH__MON_CONTROL_MODE, str_name);

		//
		str_name = "MON.GET_RATIO.MASTER";
		STD__ADD_STRING_WITH_OPTION(str_name, -1, "L", "");
		LINK__VAR_STRING_CTRL(sCH__MON_GET_RATIO_MASTER, str_name);

		str_name = "MON.GET_FLOW.MASTER";
		STD__ADD_STRING_WITH_OPTION(str_name, -1, "L", "");
		LINK__VAR_STRING_CTRL(sCH__MON_GET_FLOW_MASTER, str_name);

		//
		str_name = "MON.GET_RATIO.SLAVE";
		STD__ADD_STRING_WITH_OPTION(str_name, -1, "L", "");
		LINK__VAR_STRING_CTRL(sCH__MON_GET_RATIO_SLAVE, str_name);

		str_name = "MON.GET_FLOW.SLAVE";
		STD__ADD_STRING_WITH_OPTION(str_name, -1, "L", "");
		LINK__VAR_STRING_CTRL(sCH__MON_GET_FLOW_SLAVE, str_name);
	}

	// Stable Channel ...
	{
		// CFG ...
		{
			str_name = "CFG.WARNING.LIMIT";
			STD__ADD_ANALOG_WITH_X_OPTION(str_name, "%", 1, 0.0, 100.0, "");
			LINK__VAR_ANALOG_CTRL(aCH__CFG_WARNING_LIMIT, str_name);

			str_name = "CFG.FAULT.LIMIT";
			STD__ADD_ANALOG_WITH_X_OPTION(str_name, "%", 1, 0.0, 100.0, "");
			LINK__VAR_ANALOG_CTRL(aCH__CFG_FAULT_LIMIT, str_name);

			//
			str_name = "CFG.CHECKING_TIME.FOR.STABLE";
			STD__ADD_ANALOG_WITH_X_OPTION(str_name, "sec", 1, 0.0, 100.0, "");
			LINK__VAR_ANALOG_CTRL(aCH__CFG_CHECKING_TIME_FOR_STABLE, str_name);

			str_name = "CFG.CHECK_TIMEOUT.FOR.ERROR";
			STD__ADD_ANALOG_WITH_X_OPTION(str_name, "sec", 1, 0.0, 100.0, "");
			LINK__VAR_ANALOG_CTRL(aCH__CFG_CHECK_TIMEOUT_FOR_ERROR, str_name);

			str_name = "CFG.STABLE_TIME.FOR.SETPINT_CHANGE";
			STD__ADD_ANALOG_WITH_X_OPTION(str_name, "sec", 1, 0.0, 100.0, "");
			LINK__VAR_ANALOG_CTRL(aCH__CFG_STABLE_TIME_FOR_SETPINT_CHANGE, str_name);
		}

		// MON.MASTER ...
		{
			str_name = "MON.STABLE.STATE.MASTER";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__MON_STABLE_STATE_MASTER, str_name);

			str_name = "MON.STABLE.TIME.MASTER";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__MON_STABLE_TIME_MASTER, str_name);

			str_name = "MON.FAULT.STATE.MASTER";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__MON_FAULT_STATE_MASTER, str_name);

			str_name = "MON.FAULT.TIME.MASTER";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__MON_FAULT_TIME_MASTER, str_name);

			str_name = "MON.CHANGE.STATE.MASTER";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__MON_CHANGE_STATE_MASTER, str_name);

			str_name = "MON.CHANGE.TIME.MASTER";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__MON_CHANGE_TIME_MASTER, str_name);
		}

		// MON.SLAVE ...
		{
			str_name = "MON.STABLE.STATE.SLAVE";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__MON_STABLE_STATE_SLAVE, str_name);

			str_name = "MON.STABLE.TIME.SLAVE";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__MON_STABLE_TIME_SLAVE, str_name);

			str_name = "MON.FAULT.STATE.SLAVE";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__MON_FAULT_STATE_SLAVE, str_name);

			str_name = "MON.FAULT.TIME.SLAVE";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__MON_FAULT_TIME_SLAVE, str_name);

			str_name = "MON.CHANGE.STATE.SLAVE";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__MON_CHANGE_STATE_SLAVE, str_name);

			str_name = "MON.CHANGE.TIME.SLAVE";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__MON_CHANGE_TIME_SLAVE, str_name);
		}

		// MON.FLAG ...
		{
			str_name = "MON.STABLE.FLAG";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__MON_STABLE_FLAG, str_name);

			str_name = "MON.FAULT.FLAG";
			STD__ADD_STRING(str_name);
			LINK__VAR_STRING_CTRL(sCH__MON_FAULT_FLAG, str_name);
		}
	}

	// ...
	{
		p_variable->Add__MONITORING_PROC(1.0, MON_ID__STATUS);
		p_variable->Add__MONITORING_PROC(1.0, MON_ID__STABLE);
	}
	return 1;
}


// ...
#define  ACT__CLEAR								\
l_act.RemoveAll();								\
l_act.Add(STR__CLEAR);

int CObj__MADEE_IO::__DEFINE__ALARM(p_alarm)
{
	DECLARE__ALARM;

	// ...
	CString title;
	title.Format("%s : ", sObject_Name);

	CString alarm_title;
	CString alarm_msg;
	CStringArray l_act;
	int alarm_id;

	// ...
	{
		alarm_id = ALID__CH1_WARNING;

		alarm_title  = title;
		alarm_title += "CH1 Warning !";

		alarm_msg  = "";
		alarm_msg += "Please, check the ratio of CH1 ! \n";

		ACT__CLEAR;

		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}	
	// ...
	{
		alarm_id = ALID__CH2_WARNING;

		alarm_title  = title;
		alarm_title += "CH2 Warning !";

		alarm_msg  = "";
		alarm_msg += "Please, check the ratio of CH2 ! \n";

		ACT__CLEAR;

		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}	

	// ...
	{
		alarm_id = ALID__CH1_FAULT;

		alarm_title  = title;
		alarm_title += "CH1 Fault !";

		alarm_msg  = "";
		alarm_msg += "Please, check the ratio of CH1 ! \n";

		ACT__CLEAR;

		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}	
	// ...
	{
		alarm_id = ALID__CH2_FAULT;

		alarm_title  = title;
		alarm_title += "CH2 Fault !";

		alarm_msg  = "";
		alarm_msg += "Please, check the ratio of CH2 ! \n";

		ACT__CLEAR;

		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}	

	return 1;
}

//-------------------------------------------------------------------------
int CObj__MADEE_IO::__INITIALIZE__OBJECT(p_variable,p_ext_obj_create)
{	
	// ...
	{
		CString file_name;
		CString log_msg;

		file_name.Format("%s.log", sObject_Name);

		log_msg  = "\n\n";
		log_msg += "//------------------------------------------------------------------------";

		xI_LOG_CTRL->CREATE__SUB_DIRECTORY(sObject_Name);
		xI_LOG_CTRL->SET__PROPERTY(file_name,12*1,60);

		xI_LOG_CTRL->DISABLE__TIME_LOG();
		xI_LOG_CTRL->WRITE__LOG(log_msg);

		xI_LOG_CTRL->ENABLE__TIME_LOG();
		xI_LOG_CTRL->WRITE__LOG("   START   \n");
	}

	// ...
	CString def_name;
	CString ch_name;
	CString obj_name;
	CString var_name;

	// LINK : IO_Chammel
	{
		def_name = "CH__IO_IN_BYTE_HEXA";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, ch_name);
		p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(ch_name, obj_name,var_name);
		LINK__EXT_VAR_STRING_CTRL(sEXT_CH__IO_IN_BYTE_HEXA, obj_name,var_name);

		def_name = "CH__IO_OUT_BYTE_HEXA";
		p_ext_obj_create->Get__DEF_CONST_DATA(def_name, ch_name);
		p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(ch_name, obj_name,var_name);
		LINK__EXT_VAR_STRING_CTRL(sEXT_CH__IO_OUT_BYTE_HEXA, obj_name,var_name);
	}

	// ...
	{
		SCX__SEQ_INFO x_seq_info;

		iActive__SIM_MODE = x_seq_info->Is__SIMULATION_MODE();
	}
	return 1;
}

//-------------------------------------------------------------------------
int CObj__MADEE_IO::__CALL__CONTROL_MODE(mode,p_debug,p_variable,p_alarm)
{
	int flag = -1;

	// ...
	{
		CString log_msg;
		log_msg.Format("Start ... :  [%s]",mode);

		sCH__OBJ_MSG->Set__DATA(log_msg);
		xI_LOG_CTRL->WRITE__LOG(log_msg);
	}

	// ...
	{
		     IF__CTRL_MODE(sMODE__INIT)				flag = Call__INIT(p_variable, p_alarm);
		ELSE_IF__CTRL_MODE(sMODE__ALL_CLOSE)		flag = Call__ALL_CLOSE(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMDDE__CONTROL_CFG)		flag = Call__CONTROL_CFG(p_variable,p_alarm);
		ELSE_IF__CTRL_MODE(sMDDE__CONTROL)			flag = Call__CONTROL(p_variable,p_alarm);

		else									
		{
			CString log_msg;
			log_msg.Format("Invalid Mode: [%s]", mode);
			flag = -1;
		}
	}

	if((flag < 0)||(p_variable->Check__CTRL_ABORT() > 0))
	{
		CString log_msg;
		log_msg.Format("Aborted ... :  [%s]",mode);

		sCH__OBJ_MSG->Set__DATA(log_msg);
		xI_LOG_CTRL->WRITE__LOG(log_msg);		
	}
	else
	{
		CString log_msg;
		log_msg.Format("Completed ... :  [%s]",mode);

		sCH__OBJ_MSG->Set__DATA(log_msg);
		xI_LOG_CTRL->WRITE__LOG(log_msg);
	}

	return flag;
}

int CObj__MADEE_IO::__CALL__MONITORING(id,p_variable,p_alarm)
{
	if(id == MON_ID__STATUS)		Mon__STATUS(p_variable, p_alarm);
	if(id == MON_ID__STABLE)		Mon__STABLE(p_variable, p_alarm);

	return 1;
}
