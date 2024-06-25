#include "stdafx.h"

#include "CObj__PART_CLIENT__ALID.h"
#include "CObj__PART_CLIENT.h"


//--------------------------------------------------------------------------------
CObj__PART_CLIENT::CObj__PART_CLIENT()
{

}
CObj__PART_CLIENT::~CObj__PART_CLIENT()
{

}

//--------------------------------------------------------------------------------
int CObj__PART_CLIENT::__DEFINE__CONTROL_MODE(obj,l_mode)
{
	sObject_Name = obj;

	// ...
	{
		ADD__CTRL_VAR(sMODE__INIT, "INIT");

		ADD__CTRL_VAR(sMODE__TRANSFER_START, "TRANSFER.START");
		ADD__CTRL_VAR(sMODE__TRANSFER_END,   "TRANSFER.END");

		ADD__CTRL_VAR(sMODE__PIN_UP,   "PIN.UP");
		ADD__CTRL_VAR(sMODE__PIN_DOWN, "PIN.DOWN");

		ADD__CTRL_VAR(sMODE__CHAMBER_PUMP,  "CHAMBER.PUMP");
		ADD__CTRL_VAR(sMODE__CHAMBER_VENT,  "CHAMBER.VENT");
		ADD__CTRL_VAR(sMODE__CHAMBER_ABORT, "CHAMBER.ABORT");

		ADD__CTRL_VAR(sMODE__PROCESS_START, "PROCESS.START");
		ADD__CTRL_VAR(sMODE__CLEAN_START,   "CLEAN.START");
		ADD__CTRL_VAR(sMODE__PROCESS_ABORT, "PROCESS.ABORT");
	}
	return 1;
}
int CObj__PART_CLIENT::__DEFINE__VERSION_HISTORY(version)
{
	version = "\n";

	return 1;
}


// ...
#define _MON_ID__IO_MONITORING								1


int CObj__PART_CLIENT::__DEFINE__VARIABLE_STD(p_variable)
{
	DECLARE__STD_VARIABLE;

	CString var_name;
	int i;

	// OBJ ...
	{
		var_name = "OBJ.MSG";
		STD__ADD_STRING(var_name);
		LINK__VAR_STRING_CTRL(sCH__OBJ_MSG, var_name);
	}

	// MON ...
	{
		var_name = "MON.COMM.STATE";
		STD__ADD_STRING(var_name);
		LINK__VAR_STRING_CTRL(sCH__MON_COMM_STATE, var_name);

		//
		var_name = "MON.CONTROL.MODE";
		STD__ADD_STRING(var_name);
		LINK__VAR_STRING_CTRL(sCH__MON_CONTROL_MODE, var_name);

		var_name = "MON.CHM.PRESSURE";
		STD__ADD_STRING(var_name);
		LINK__VAR_STRING_CTRL(sCH__MON_CHM_PRESSURE, var_name);

		var_name = "MON.LIFT_PIN.STATE";
		STD__ADD_STRING(var_name);
		LINK__VAR_STRING_CTRL(sCH__MON_LIFT_PIN_STATE, var_name);

		var_name = "MON.SHUTTER.STATE";
		STD__ADD_STRING(var_name);
		LINK__VAR_STRING_CTRL(sCH__MON_SHUTTER_STATE, var_name);

		var_name = "MON.PROCESS.STATE";
		STD__ADD_STRING(var_name);
		LINK__VAR_STRING_CTRL(sCH__MON_PROCESS_STATE, var_name);

		var_name = "MON.CHAMBER.STATE";
		STD__ADD_STRING(var_name);
		LINK__VAR_STRING_CTRL(sCH__MON_CHAMBER_STATE, var_name);

		//
		var_name = "MON.CHM.TIME.COUNT";
		STD__ADD_STRING(var_name);
		LINK__VAR_STRING_CTRL(sCH__MON_CHM_TIME_COUNT, var_name);

		var_name = "MON.PRC.TIME.COUNT";
		STD__ADD_STRING(var_name);
		LINK__VAR_STRING_CTRL(sCH__MON_PRC_TIME_COUNT, var_name);
	}

	// INFO.DRV ...
	{
		var_name = "INFO.DRV.NET_IP";
		STD__ADD_STRING(var_name);
		LINK__VAR_STRING_CTRL(sCH__INFO_DRV_NET_IP, var_name);

		var_name = "INFO.DRV.NET_PORT";
		STD__ADD_STRING(var_name);
		LINK__VAR_STRING_CTRL(sCH__INFO_DRV_NET_PORT, var_name);

		//
		var_name = "INFO.DRV.ACT.NAME";
		STD__ADD_STRING(var_name);
		LINK__VAR_STRING_CTRL(sCH__INFO_DRV_ACT_NAME, var_name);

		var_name = "INFO.DRV.ACT.RSP";
		STD__ADD_STRING(var_name);
		LINK__VAR_STRING_CTRL(sCH__INFO_DRV_ACT_RSP, var_name);
	}

	// ...
	{
		p_variable->Add__MONITORING_PROC(3, _MON_ID__IO_MONITORING);
	}
	return 1;
}
int CObj__PART_CLIENT::__DEFINE__VARIABLE_IO(p_io_variable)
{
	DECLARE__IO_VARIABLE;
	
	CString str_name;

	// DO ...
	{
		CString str_list;

		str_list += _CMD__ACT_TRANSFER_START;
		str_list += " ";
		str_list += _CMD__ACT_TRANSFER_END;
		str_list += " ";
		str_list += _CMD__ACT_PIN_UP;
		str_list += " ";
		str_list += _CMD__ACT_PIN_DOWN;
		str_list += " ";
		str_list += _CMD__ACT_CHAMBER_PUMP;
		str_list += " ";
		str_list += _CMD__ACT_CHAMBER_VENT;
		str_list += " ";
		str_list += _CMD__ACT_CHAMBER_ABORT;
		str_list += " ";
		str_list += _CMD__ACT_PROCESS_START;
		str_list += " ";
		str_list += _CMD__ACT_CLEAN_START;
		str_list += " ";
		str_list += _CMD__ACT_PROCESS_ABORT;
		str_list += " ";

		str_name = "do.Control.SET";
		IO__ADD_DIGITAL_WRITE(str_name, str_list);
		LINK__IO_VAR_DIGITAL_CTRL(doCH__Control_SET, str_name);
	}

	// SI ...
	{
		str_name = "si.CONTROL.MODE";
		IO__ADD_STRING_READ_WITH_OPTION(str_name, "","","", 0.1, "");
		LINK__IO_VAR_STRING_CTRL(siCH__CONTROL_MODE, str_name);

		str_name = "si.CHM.PRESSURE";
		IO__ADD_STRING_READ_WITH_OPTION(str_name, "","","", 0.1, "");
		LINK__IO_VAR_STRING_CTRL(siCH__CHM_PRESSURE, str_name);

		str_name = "si.PIN.STATE";
		IO__ADD_STRING_READ_WITH_OPTION(str_name, "","","", 0.1, "");
		LINK__IO_VAR_STRING_CTRL(siCH__PIN_STATE, str_name);

		str_name = "si.SHUTTER.STATE";
		IO__ADD_STRING_READ_WITH_OPTION(str_name, "","","", 0.1, "");
		LINK__IO_VAR_STRING_CTRL(siCH__SHUTTER_STATE, str_name);

		str_name = "si.PROCESS.STATE";
		IO__ADD_STRING_READ_WITH_OPTION(str_name, "","","", 0.1, "");
		LINK__IO_VAR_STRING_CTRL(siCH__PROCESS_STATE, str_name);

		str_name = "si.CHAMBER.STATE";
		IO__ADD_STRING_READ_WITH_OPTION(str_name, "","","", 0.1, "");
		LINK__IO_VAR_STRING_CTRL(siCH__CHAMBER_STATE, str_name);
	}
	return 1;
}

int CObj__PART_CLIENT::__DEFINE__ALARM(p_alarm)
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
		alarm_id = ALID__OFFLINE;

		alarm_title  = title;
		alarm_title += "Communication Offline !";

		alarm_msg = "Please, check Communication State. \n";

		l_act.RemoveAll();
		l_act.Add(ACT__CHECK);

		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	// ...
	{
		alarm_id = ALID__SS_FAIL;

		alarm_title  = title;
		alarm_title += "SS Fail !";

		alarm_msg = "Please, check VTY. \n";

		l_act.RemoveAll();
		l_act.Add(ACT__CHECK);

		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}
	// ...
	{
		alarm_id = ALID__NET_BODY_TIMEOUT;

		alarm_title  = title;
		alarm_title += "Body Data Timeout !";

		alarm_msg = "Please, check VTY. \n";

		l_act.RemoveAll();
		l_act.Add(ACT__CHECK);

		ADD__ALARM_EX(alarm_id,alarm_title,alarm_msg,l_act);
	}

	return 1;
}

//--------------------------------------------------------------------------------
int CObj__PART_CLIENT
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
int CObj__PART_CLIENT
::__INITIALIZE__IO(p_io_para)
{
	//-------------------------------------------------------
	// Set Driver Parameter
	//------------------------------------------------------

	CString net_ip   = "127.0.0.1";
	CString net_port = "10001";

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

		mX__Net_Client->SET__TERMINAL_InSTRING(str_end, str_len);
		mX__Net_Client->SET__TERMINAL_OutSTRING(str_end, str_len);
		mX__Net_Client->SET__ETHERNET_TYPE__CLIENT();

		mX__Net_Client->INIT__PROPERTY(net_ip, atoi(net_port));

		sCH__INFO_DRV_NET_IP->Set__DATA(net_ip);
		sCH__INFO_DRV_NET_PORT->Set__DATA(net_port);
	}

	// ...
	{
		CString log_msg;
		CString log_bff;

		// ...
		{
			log_msg = "Connect to Ethernet ... \n";

			log_bff.Format("Net_IP : %s \n", net_ip);
			log_msg += log_bff;

			log_bff.Format("Net_Port : %s \n", net_port);
			log_msg += log_bff;

			log_bff.Format("End_String(Out) : %s \n", mX__Net_Client->GET__TERMINAL_InSTRING());
			log_msg += log_bff;

			log_bff.Format("End_String(In ) : %s \n", mX__Net_Client->GET__TERMINAL_OutSTRING());
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
		
			if(mX__Net_Client->CONNECT(&err_msg) < 0)
			{
				log_bff.Format("Fail to do INIT__ETHERNET: %s(%s), ret(-1) \n", net_ip,net_port);
				log_msg += log_bff;

				log_bff.Format(" * Error Message <- %s \n", err_msg);
				log_msg += log_bff;
			}
			else
			{
				log_msg += "Initialization Completed ... \n";
			}
		}

		mX__Log_Ctrl->WRITE__LOG(log_msg);
	}
	return 1;
}


//--------------------------------------------------------------------------------
int CObj__PART_CLIENT
::__CALL__CONTROL_MODE(mode, p_debug, p_variable, p_alarm)
{
	DECLARE__EXT_CTRL(p_variable);

	int flag = -1;

	CString log_msg;
	CString log_bff;

	// ...
	{
		log_msg.Format("Start [%s] ... By %s", mode, p_ext_mode_ctrl->Get__UPPER_OBJECT_NAME());
		sCH__OBJ_MSG->Set__DATA(log_msg);

		log_msg += "\n";
		mX__Log_Ctrl->WRITE__LOG(log_msg);
	}

	IF__CTRL_MODE(sMODE__INIT)
	{
		flag = Call__INIT(p_variable, p_alarm);
	}
	ELSE_IF__CTRL_MODE(sMODE__TRANSFER_START)
	{
		flag = Call__TRANSFER_START(p_variable, p_alarm);
	}
	ELSE_IF__CTRL_MODE(sMODE__TRANSFER_END)
	{
		flag = Call__TRANSFER_END(p_variable, p_alarm);
	}
	ELSE_IF__CTRL_MODE(sMODE__PIN_UP)
	{
		flag = Call__PIN_UP(p_variable, p_alarm);
	}
	ELSE_IF__CTRL_MODE(sMODE__PIN_DOWN)
	{
		flag = Call__PIN_DOWN(p_variable, p_alarm);
	}
	ELSE_IF__CTRL_MODE(sMODE__CHAMBER_PUMP)
	{
		flag = Call__CHAMBER_PUMP(p_variable, p_alarm);
	}
	ELSE_IF__CTRL_MODE(sMODE__CHAMBER_VENT)
	{
		flag = Call__CHAMBER_VENT(p_variable, p_alarm);
	}
	ELSE_IF__CTRL_MODE(sMODE__CHAMBER_ABORT)
	{
		flag = Call__CHAMBER_ABORT(p_variable, p_alarm);
	}
	ELSE_IF__CTRL_MODE(sMODE__PROCESS_START)
	{
		flag = Call__PROCESS_START(p_variable, p_alarm);
	}
	ELSE_IF__CTRL_MODE(sMODE__CLEAN_START)
	{
		flag = Call__CLEAN_START(p_variable, p_alarm);
	}
	ELSE_IF__CTRL_MODE(sMODE__PROCESS_ABORT)
	{
		flag = Call__PROCESS_ABORT(p_variable, p_alarm);
	}

	//
	if((flag < 0)||(p_variable->Check__CTRL_ABORT() > 0))
	{
		log_msg.Format("Aborted ... :  [%s]", mode);
		sCH__OBJ_MSG->Set__DATA(log_msg);

		mX__Log_Ctrl->WRITE__LOG(log_msg);
	}
	else
	{
		log_msg.Format("Completed ... :  [%s]", mode);
		sCH__OBJ_MSG->Set__DATA(log_msg);

		mX__Log_Ctrl->WRITE__LOG(log_msg);
	}
	return flag;
}

int CObj__PART_CLIENT
::__CALL__MONITORING(id, p_variable, p_alarm)
{
	if(id == _MON_ID__IO_MONITORING)			Mon__IO_MONITORING(p_variable, p_alarm);

	return 1;
}
