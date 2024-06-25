#include "StdAfx.h"
#include "CObj__CDA.h"
#include "CObj__CDA__ALID.h"


//-------------------------------------------------------------------------
CObj__CDA::CObj__CDA()
{

}
CObj__CDA::~CObj__CDA()
{

}

//-------------------------------------------------------------------------
int CObj__CDA::__DEFINE__CONTROL_MODE(obj,l_mode)
{
	sObject_Name = obj;

	return 1;
}
int CObj__CDA::__DEFINE__VERSION_HISTORY(version)
{

	return 1;
}


// ...
#define  MON_ID__CDA_PRESSURE				1


int CObj__CDA::__DEFINE__VARIABLE_STD(p_variable)
{
	DECLARE__STD_VARIABLE;

	// ...
	CString str_name;
	int i;

	for(i=0; i<_CFG__CDA_SIZE; i++)
	{
		if(i == _INDEX__CDA_GN2)
		{
			str_name = "CFG.CDA_GN2.LOW_PRESSURE";
			STD__ADD_ANALOG_WITH_X_OPTION(str_name, "MPa", 3, 0.0, 1, "");
		}
		else if(i == _INDEX__CDA_TM_LL)
		{
			str_name = "CFG.CDA_TM_LL.LOW_PRESSURE";
			STD__ADD_ANALOG_WITH_X_OPTION(str_name, "MPa", 3, 0.0, 1, "");
		}
		else if(i == _INDEX__CDA_PCW)
		{
			str_name = "CFG.CDA_PCW.LOW_PRESSURE";
			STD__ADD_ANALOG_WITH_X_OPTION(str_name, "L/Min", 1, 0.0, 250, "");
		}
		else if(i == _INDEX__CDA1_PM)
		{
			str_name = "CFG.CDA1_PM.LOW_PRESSURE";
			STD__ADD_ANALOG_WITH_X_OPTION(str_name, "MPa", 3, 0.0, 1, "");
		}
		else if(i == _INDEX__CDA2_PM)
		{
			str_name = "CFG.CDA2_PM.LOW_PRESSURE";
			STD__ADD_ANALOG_WITH_X_OPTION(str_name, "MPa", 3, 0.0, 1, "");
		}
		else
		{
			continue;
		}

		LINK__VAR_ANALOG_CTRL(aCH__CFG_CDA_LOW_LIMIT[i], str_name);
	}

	// ...
	{
		p_variable->Add__MONITORING_PROC(3.0, MON_ID__CDA_PRESSURE);
	}
	return 1;
}


// ...
#define  REG_ACT__CHECK							\
l_act.RemoveAll();								\
l_act.Add("CHECK");


int CObj__CDA::__DEFINE__ALARM(p_alarm)
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
		alarm_id = ALID__CDA_GN2__LOW_PRESSURE;

		alarm_title  = title;
		alarm_title += "GN2 CDA Low-Pressure !";

		alarm_msg = "Please, check the pressure of cda gn2.\n";

		REG_ACT__CHECK;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}
	// ...
	{
		alarm_id = ALID__CDA_PM1__LOW_PRESSURE;

		alarm_title  = title;
		alarm_title += "PM1 CDA Low-Pressure !";

		alarm_msg = "Please, check the pressure of cda pm1.\n";

		REG_ACT__CHECK;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}
	// ...
	{
		alarm_id = ALID__CDA_PM2__LOW_PRESSURE;

		alarm_title  = title;
		alarm_title += "PM2 CDA Low-Pressure !";

		alarm_msg = "Please, check the pressure of cda pm2.\n";

		REG_ACT__CHECK;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}
	// ...
	{
		alarm_id = ALID__CDA_TM_LL__LOW_PRESSURE;

		alarm_title  = title;
		alarm_title += "TM_LL CDA Low-Pressure !";

		alarm_msg = "Please, check the pressure of cda tm_ll.\n";

		REG_ACT__CHECK;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}
	// ...
	{
		alarm_id = ALID__CDA_PCW__LOW_PRESSURE;

		alarm_title  = title;
		alarm_title += "PCW CDA Low-Pressure !";

		alarm_msg = "Please, check the pressure of cda pcw.\n";

		REG_ACT__CHECK;
		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	return 1;
}

int CObj__CDA::__Define__USER_FUNCTION(CII_DEFINE__FUNCTION *p_fnc_ctrl)
{

	return 1;
}

//-------------------------------------------------------------------------
int CObj__CDA::__INITIALIZE__OBJECT(p_variable,p_ext_obj_create)
{	
	CString obj_name;
	CString var_name;

	// LINK IO ...
	{
		obj_name = "IO";

		for(int i = 0; i<_CFG__CDA_SIZE; i++)
		{
				 if(i == _INDEX__CDA_GN2)			var_name = "ai.GN2.Pressure.Switch";
			else if(i == _INDEX__CDA_TM_LL)			var_name = "ai.CDA2.TM_LL.Pressure.Switch";
			else if(i == _INDEX__CDA_PCW)			var_name = "ai.PCW.Flow.Meter";
			else if(i >= _INDEX__CDA1_PM)			var_name = "ai.CDA1.PM.Pressure.Switch";
			else if(i >= _INDEX__CDA2_PM)			var_name = "ai.CDA2.PM.Pressure.Switch";
			else									continue;

			LINK__EXT_VAR_ANALOG_CTRL(aEXT_CH__CDA_PRESSURE[i], obj_name,var_name);
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
int CObj__CDA::__CALL__CONTROL_MODE(mode,p_debug,p_variable,p_alarm)
{

	return -1;
}

int CObj__CDA::__CALL__MONITORING(id,p_variable,p_alarm)
{
	switch(id)
	{
		case MON_ID__CDA_PRESSURE:
			Mon__CDA_PRESSURE(p_variable, p_alarm);
			break;
	}
	return 1;
}
