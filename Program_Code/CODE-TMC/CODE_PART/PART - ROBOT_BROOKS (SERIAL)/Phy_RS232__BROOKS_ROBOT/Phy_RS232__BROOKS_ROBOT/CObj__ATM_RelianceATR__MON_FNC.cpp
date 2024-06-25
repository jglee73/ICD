#include "StdAfx.h"
#include "CObj__ATM_RelianceATR.h"
#include "CObj__ATM_RelianceATR__DEF.h"
#include "CObj__ATM_RelianceATR__ALID.h"

#include "CCommon_DEF.h"


//------------------------------------------------------------------------------------
void CObj__ATM_RelianceATR
::Mon__IO_MONITOR(CII_OBJECT__VARIABLE* p_variable,
				  CII_OBJECT__ALARM* p_alarm)
{
	CString var_data;

	if(iActive_SIM > 0)
	{
		diCH__COMM_STS->Set__DATA(STR__ONLINE);
		sCH__MON_COMMUNICATION_STATE->Set__DATA(STR__ONLINE);
	}


	while(1)
	{
		p_variable->Wait__SINGLE_OBJECT(0.5);


		if(diCH__COMM_STS->Check__DATA(STR__OFFLINE) > 0)
		{
			sCH__MON_COMMUNICATION_STATE->Set__DATA(STR__OFFLINE);

			// ...
			{
				int alarm_id = ALID__OFFLINE_ALARM_MON;
				CString r_act;

				p_alarm->Check__ALARM(alarm_id,r_act);
				p_alarm->Post__ALARM(alarm_id);
			}
		}
		else
		{
			sCH__MON_COMMUNICATION_STATE->Set__DATA(STR__ONLINE);
		}

		// ...
		{
			diCH__ROBOT_CURR_POS->Get__DATA(var_data);

			diCH__ROBOT_Z_POS->Get__DATA(var_data);
			diCH__ROBOT_AR_POS->Get__DATA(var_data);
			diCH__ROBOT_T_POS->Get__DATA(var_data);
		}
	}	
}

void CObj__ATM_RelianceATR
::Mon__HYPER_TERMINAL_SEND(CII_OBJECT__VARIABLE* p_variable,
						   CII_OBJECT__ALARM* p_alarm)
{
	SCX__TIMER_CTRL cx_timer_ctrl;
	SCX__SEQ_INFO x_seq_info;
	
	CString str_msg;
	CString str_time;
	CString log_msg;
	CString var_data;
	int count = 0;

	cx_timer_ctrl->REGISTER__ABORT_OBJECT(sObject_Name);

	while(1)
	{
		cx_timer_ctrl->WAIT(0.5);

		if(dCH__HYPER_TERMINAL_SEND_REQ->Check__DATA("YES") > 0)
		{
			xCH__LOCK_FLAG->Set__DATA("YES");		// LockÀ» °Ç´Ù.

			// Do Action ...
			sCH__HYPER_TERMINAL_SEND_MSG->Get__DATA(var_data);
			log_msg.Format("COMMAND:%s \n\n", var_data);
			sCH__HYPER_TERMINAL_RECV_MSG->Set__DATA("");
			sCH__HYPER_TERMINAL_RECV_MSG->Set__DATA(log_msg);

			//
			xCH__UPDATE_FLAG->Get__DATA(str_msg);
			var_data.Format("%1d", atoi(str_msg)+1);
			xCH__UPDATE_FLAG->Set__DATA(var_data);
			_sleep(500);

			str_time = x_seq_info->Get__DATE_TIME_STRING();
			log_msg.Format("[%s] : <----- SEND -----> \n", str_time);
			sCH__HYPER_TERMINAL_RECV_MSG->Set__DATA("");
			sCH__HYPER_TERMINAL_RECV_MSG->Set__DATA(log_msg);

			//
			xCH__UPDATE_FLAG->Get__DATA(str_msg);
			var_data.Format("%1d", atoi(str_msg)+1);
			xCH__UPDATE_FLAG->Set__DATA(var_data);
			_sleep(500);

			// command
			sCH__HYPER_TERMINAL_SEND_MSG->Get__DATA(var_data);
			soCH__HYPER_TERMINAL_CMD->Set__DATA(var_data);

			dCH__HYPER_TERMINAL_SEND_REQ->Set__DATA("NO");
		}
	}	
}
