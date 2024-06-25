#include "stdafx.h"

#include "CObj__PART_SERVER.h"


//--------------------------------------------------------------------------------
CObj__PART_SERVER::CObj__PART_SERVER()
{

}
CObj__PART_SERVER::~CObj__PART_SERVER()
{

}

//--------------------------------------------------------------------------------
int CObj__PART_SERVER::__DEFINE__CONTROL_MODE(obj,l_mode)
{
	sObject_Name = obj;

	// ...
	{
		ADD__CTRL_VAR(sMODE__INIT, "INIT");
	}
	return 1;
}
int CObj__PART_SERVER::__DEFINE__VERSION_HISTORY(version)
{
	version = "\n";

	return 1;
}

// ...
#define _MON_ID__DRV_PROC									1


int CObj__PART_SERVER::__DEFINE__VARIABLE_STD(p_variable)
{
	DECLARE__STD_VARIABLE;

	CString var_name;

	// ...
	{
		var_name = "COMM.STATE";
		STD__ADD_STRING(var_name);
		LINK__VAR_STRING_CTRL(sCH__COMM_STATE, var_name);
	}

	// CFG ...
	{
		var_name = "CFG.CHM.PUMP.TIME";
		STD__ADD_ANALOG_WITH_X_OPTION(var_name, "sec", 0, 1.0, 100, "");
		LINK__VAR_ANALOG_CTRL(aCH__CFG_CHM_PUMP_TIME, var_name);

		var_name = "CFG.CHM.VENT.TIME";
		STD__ADD_ANALOG_WITH_X_OPTION(var_name, "sec", 0, 1.0, 100, "");
		LINK__VAR_ANALOG_CTRL(aCH__CFG_CHM_VENT_TIME, var_name);

		var_name = "MON.CHM.TIME.COUNT";
		STD__ADD_STRING(var_name);
		LINK__VAR_STRING_CTRL(sCH__MON_CHM_TIME_COUNT, var_name);

		//
		var_name = "CFG.PROCESS.TIME";
		STD__ADD_ANALOG_WITH_X_OPTION(var_name, "sec", 0, 1.0, 100, "");
		LINK__VAR_ANALOG_CTRL(aCH__CFG_PROCESS_TIME, var_name);

		var_name = "CFG.CLEAN.TIME";
		STD__ADD_ANALOG_WITH_X_OPTION(var_name, "sec", 0, 1.0, 100, "");
		LINK__VAR_ANALOG_CTRL(aCH__CFG_CLEAN_TIME, var_name);

		var_name = "MON.PRC.TIME.COUNT";
		STD__ADD_STRING(var_name);
		LINK__VAR_STRING_CTRL(sCH__MON_PRC_TIME_COUNT, var_name);
	}

	// ...
	{
		p_variable->Add__MONITORING_PROC(1, _MON_ID__DRV_PROC);
	}
	return 1;
}
int CObj__PART_SERVER::__DEFINE__VARIABLE_IO(p_io_variable)
{
	DECLARE__IO_VARIABLE;

	CString str_name;
	CString str_list;

	return 1;
}

int CObj__PART_SERVER::__DEFINE__ALARM(p_alarm)
{
	DECLARE__ALARM;

	CString title;
	title.Format("%s - ",sObject_Name);

	CString alarm_title;
	CString alarm_msg;
	CStringArray l_act;
	int alarm_id;

	// ...
	{

	}

	return 1;
}

//--------------------------------------------------------------------------------
int CObj__PART_SERVER
::__INITIALIZE__OBJECT(p_variable,p_ext_obj_create)
{
	CString def_name;
	CString def_data;
	CString str_name;

	// ...
	{
		CString file_name;
		CString log_msg;

		file_name.Format("%s-APP.log", sObject_Name);

		log_msg  = "\n\n";
		log_msg += "//------------------------------------------------------------------------";

		// ...
		{
			mX__Log_Ctrl->CREATE__SUB_DIRECTORY(sObject_Name);
			mX__Log_Ctrl->SET__PROPERTY(file_name,24*5,60);

			mX__Log_Ctrl->DISABLE__TIME_LOG();
			mX__Log_Ctrl->WRITE__LOG(log_msg);

			mX__Log_Ctrl->ENABLE__TIME_LOG();
			mX__Log_Ctrl->WRITE__LOG("   START   \n");
		}
	}

	// ...
	{
		SCX__SEQ_INFO x_seq_info;

		iActive__SIM_MODE = x_seq_info->Is__SIMULATION_MODE();
	}
	return 1;
}
int CObj__PART_SERVER
::__INITIALIZE__IO(p_io_para)
{
	CString net_ip   = "127.0.0.1";
	CString net_port = "12345";

	// ...
	{
		CString para_cmmd;
		CString para_data;

		// 1. IP
		{
			para_cmmd = "IP";

			if(p_io_para->Get__PARAMETER_DATA(para_cmmd, para_data) > 0)
			{
				net_ip = para_data;

			}
		}
		// 2. Port
		{
			para_cmmd = "PORT";

			if(p_io_para->Get__PARAMETER_DATA(para_cmmd, para_data) > 0)
			{
				net_port = para_data;
			}
		}		
	}

	// ...
	{
		char str_end[10];

		strcpy(str_end, "\r\n");
		int str_len = strlen(str_end);

		mX__Net_Server->SET__TERMINAL_InSTRING(str_end, str_len);
		mX__Net_Server->SET__TERMINAL_OutSTRING(str_end, str_len);
		mX__Net_Server->SET__ETHERNET_TYPE__SERVER();

		mX__Net_Server->INIT__PROPERTY(net_ip, atoi(net_port));
	}

	// ...
	{
		CString log_msg;
		CString log_bff;

		// ...
		{
			log_msg = "Ethernet.Server ... \n";

			log_bff.Format("Net_IP : %s \n", net_ip);
			log_msg += log_bff;

			log_bff.Format("Net_Port : %s \n", net_port);
			log_msg += log_bff;

			log_bff.Format("End_String(Out) : %s \n", mX__Net_Server->GET__TERMINAL_InSTRING());
			log_msg += log_bff;

			log_bff.Format("End_String(In ) : %s \n", mX__Net_Server->GET__TERMINAL_OutSTRING());
			log_msg += log_bff;
		}

		if(iActive__SIM_MODE > 0)
		{
			log_msg += "\n";
			log_msg += "Simulation Mode ! \n";
		}
		else
		{
			CString err_msg;

			if(mX__Net_Server->START__SERVER() < 0)
			{
				log_bff.Format("SERVER.Start(%s %s) - Error !!! \n", net_ip,net_port);
				log_msg += log_bff;
			}
			else
			{
				log_msg += "SERVER.Start() - OK ! \n";
			}
		}

		mX__Log_Ctrl->WRITE__LOG(log_msg);
	}
	return 1;
}


//--------------------------------------------------------------------------------
int CObj__PART_SERVER
::__CALL__CONTROL_MODE(mode, p_debug, p_variable, p_alarm)
{
	int flag = -1;

	IF__CTRL_MODE(sMODE__INIT)
	{
		flag = Call__INIT(p_variable, p_alarm);
	}

	return flag;
}

int CObj__PART_SERVER
::__CALL__MONITORING(id, p_variable, p_alarm)
{
	if(id == _MON_ID__DRV_PROC)					Mon__DRV_PROC(p_variable, p_alarm);

	return 1;
}
