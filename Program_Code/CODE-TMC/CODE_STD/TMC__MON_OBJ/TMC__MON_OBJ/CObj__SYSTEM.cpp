#include "StdAfx.h"
#include "CObj__SYSTEM.h"
#include "CObj__SYSTEM__DEF.h"
#include "CObj__SYSTEM__ALID.h"


//-------------------------------------------------------------------------
CObj__SYSTEM::CObj__SYSTEM()
{

}
CObj__SYSTEM::~CObj__SYSTEM()
{

}

//-------------------------------------------------------------------------
int CObj__SYSTEM::__DEFINE__CONTROL_MODE(obj,l_mode)
{
	sObject_Name = obj;

	return 1;
}
int CObj__SYSTEM::__DEFINE__VERSION_HISTORY(version)
{

	return 1;
}


// ...
#define  MON_ID__IO_MONITOR					1


int CObj__SYSTEM::__DEFINE__VARIABLE_STD(p_variable)
{
	DECLARE__STD_VARIABLE;

	CCommon_Error__DEF_VARIABLE m_err_def_variable;
	p_variable->Link__DEF_VARIABLE__ERROR_FEEDBACK(&m_err_def_variable);

	// ...
	CString str_name;

	// CFG ...
	{
		str_name = "CFG.CHECK.CDA_FLOW";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, "FALSE TRUE", "");
		LINK__VAR_DIGITAL_CTRL(dCH__CFG_CHECK_CDA_FLOW, str_name);

		str_name = "CFG.CHECK.N2_FLOW";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, "FALSE TRUE", "");
		LINK__VAR_DIGITAL_CTRL(dCH__CFG_CHECK_N2_FLOW, str_name);

		str_name = "CFG.CHECK.EFEM_DOOR.CLOSE";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, "FALSE TRUE", "");
		LINK__VAR_DIGITAL_CTRL(dCH__CFG_CHECK_EFEM_DOOR_CLOSE, str_name);

		//
		str_name = "CFG.CHECK.FFU.DIFF_PRESSURE";
		STD__ADD_DIGITAL_WITH_X_OPTION(str_name, "FALSE TRUE", "");
		LINK__VAR_DIGITAL_CTRL(dCH__CFG_CHECK_FFU_DIFF_PRESSURE, str_name);

		str_name = "CFG.FFU.DIFF_PRESSURE.LOW_LIMIT";
		STD__ADD_ANALOG_WITH_X_OPTION(str_name, "torr", 3, 0.0, 1000, "");
		LINK__VAR_ANALOG_CTRL(aCH__CFG_FFU_DIFF_PRESSURE_LOW_LIMIT, str_name);
	}

	// ...
	{
		p_variable->Add__MONITORING_PROC(3.0, MON_ID__IO_MONITOR);
	}
	return 1;
}


// ...
#define  REG_ACT__CHECK							\
l_act.RemoveAll();								\
l_act.Add("CHECK");


int CObj__SYSTEM::__DEFINE__ALARM(p_alarm)
{
	DECLARE__ALARM;

	// ...
	CString title;
	title.Format("%s - ",sObject_Name);

	CString alarm_title;
	CString alarm_msg;
	CString alarm_bff;
	CStringArray l_act;
	int alarm_id;

	// ...
	{
		alarm_id = ALID__CDA_FLOW__DI_ERROR;

		alarm_title  = title;
		alarm_title += "CDA Flow Sensor is not \"ON\".";

		alarm_msg  = "CDA Flow Sensor is not \"ON\".";
		alarm_msg += "Please, check CDA flow sensor state.\n";

		REG_ACT__CHECK;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__N2_FLOW__DI_ERROR;

		alarm_title  = title;
		alarm_title += "N2 Flow Sensor is not \"ON\".";

		alarm_msg  = "N2 Flow Sensor is not \"ON\".";
		alarm_msg += "Please, check n2 flow sensor state.\n";

		REG_ACT__CHECK;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__EFEM_DOOR_CLOSE__DI_ERROR;

		alarm_title  = title;
		alarm_title += "EFEM Door Close Sensor is not \"ON\".";

		alarm_msg  = "EFEM Door Close Sensor is not \"ON\".";
		alarm_msg += "Please, check EFEM Door Close sensor state.\n";

		REG_ACT__CHECK;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__FFU_DIFF_PRESSURE__LOW_LIMIT;

		alarm_title  = title;
		alarm_title += "System FFU Differential Pressure Alarm Occurred !";

		alarm_msg  = "1. Please, Check.. EFEM Side Doors Close Status.\n";
		alarm_msg += "2. Please, Check.. FFU Fan Speed.\n";

		REG_ACT__CHECK;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	return 1;
}

int CObj__SYSTEM::__Define__USER_FUNCTION(CII_DEFINE__FUNCTION *p_fnc_ctrl)
{
	p_fnc_ctrl->Link__ERROR_FEEDBACK(&mERROR__USER_FNC);

	// ...
	{

	}
	return 1;
}

//-------------------------------------------------------------------------
int CObj__SYSTEM::__INITIALIZE__OBJECT(p_variable,p_ext_obj_create)
{	
	CString def_name;
	CString def_data;

	CString obj_name;
	CString var_name;


	// LINK_IO ...
	{
		// DI.CDA_FLOW
		{
			def_name = "LINK_IO.DI_CDA_FLOW";
			p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);

			bool def_check = _Check__DEF_USE(def_data);
			bActive__DI_CDA_FLOW = def_check;

			if(def_check)
			{
				p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name,var_name);
				LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__DI_CDA_FLOW, obj_name,var_name);
			}
		}

		// DI.N2_FLOW
		{
			def_name = "LINK_IO.DI_N2_FLOW";
			p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);

			bool def_check = _Check__DEF_USE(def_data);
			bActive__DI_N2_FLOW = def_check;

			if(def_check)
			{
				p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name,var_name);
				LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__DI_N2_FLOW, obj_name,var_name);
			}
		}

		// DI.EFEM_DOOR_CLOSE
		{
			def_name = "LINK_IO.DI_EFEM_DOOR_CLOSE";
			p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);

			bool def_check = _Check__DEF_USE(def_data);
			bActive__DI_EFEM_DOOR_CLOSE = def_check;

			if(def_check)
			{
				p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name,var_name);
				LINK__EXT_VAR_DIGITAL_CTRL(dEXT_CH__DI_EFEM_DOOR_CLOSE, obj_name,var_name);
			}
		}

		// AI.FFU_DIFF_PRESSURE
		{
			def_name = "LINK_IO.AI_FFU_DIFF_PRESSURE";
			p_ext_obj_create->Get__DEF_CONST_DATA(def_name, def_data);

			bool def_check = _Check__DEF_USE(def_data);
			bActive__AI_FFU_DIFF_PRESSURE = def_check;

			if(def_check)
			{
				p_ext_obj_create->Get__CHANNEL_To_OBJ_VAR(def_data, obj_name,var_name);
				LINK__EXT_VAR_ANALOG_CTRL(aEXT_CH__AI_FFU_DIFF_PRESSURE, obj_name,var_name);
			}
		}
	}

	// ...
	{
		SCX__SEQ_INFO x_seq_info;

		iActive__SIM_MODE = x_seq_info->Is__SIMULATION_MODE();
	}
	return 1;
}

//-------------------------------------------------------------------------
int CObj__SYSTEM::__CALL__CONTROL_MODE(mode,p_debug,p_variable,p_alarm)
{

	return -1;
}

int CObj__SYSTEM::__CALL__MONITORING(id,p_variable,p_alarm)
{
	switch(id)
	{
		case MON_ID__IO_MONITOR:
			Mon__IO_MONITOR(p_variable,p_alarm);
			break;
	}

	return 1;
}
