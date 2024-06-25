#include "stdafx.h"

#include "CObj__PART_SERVER__DEF.h"
#include "CObj__PART_SERVER.h"

#include "CObj__PART_CLIENT__DEF.h"


// ...
void CObj__PART_SERVER
::Mon__DRV_PROC(CII_OBJECT__VARIABLE *p_variable, CII_OBJECT__ALARM *p_alarm)
{
	SCX__ASYNC_TIMER_CTRL x_timer__chm_state;
	SCX__ASYNC_TIMER_CTRL x_timer__prc_state;

	x_timer__chm_state->REGISTER__COUNT_CHANNEL_NAME(sCH__MON_CHM_TIME_COUNT->Get__CHANNEL_NAME());
	x_timer__prc_state->REGISTER__COUNT_CHANNEL_NAME(sCH__MON_PRC_TIME_COUNT->Get__CHANNEL_NAME());

	// ...
	sCH__COMM_STATE->Set__DATA(STR__OFFLINE);

	// ...
	CString str__chm_action;
	CString str__chm_pressure = "300.0";

	CString str__pin_state = STR__DOWN;
	CString str__shutter_state = STR__CLOSE;

	// ...
	char r_net_bff[256];
	int  r_net_len = 255;

	while(1)
	{
		int r_len = mX__Net_Server->READ__NETWORK_BUFFER(r_net_bff, r_net_len);

		if(r_len < 0)
		{
			sCH__COMM_STATE->Set__DATA(STR__OFFLINE);

			Sleep(100);
			continue;
		}
		if(r_len == 0)
		{
			Sleep(1);
			continue;
		}

		sCH__COMM_STATE->Set__DATA(STR__ONLINE);

		// ...
		CString r_data;

		r_net_bff[r_len] = 0;
		r_data.Format("%s", r_net_bff);

		// ...
		{
			CString log_msg;

			log_msg.Format("Recv << [%s] ", r_data);

			mX__Log_Ctrl->WRITE__LOG(log_msg);
		}

		// ...
		CString s_data;

		if(r_data.CompareNoCase(_CMD__QUERY_CTRL_MODE) == 0)
		{
			s_data = STR__REMOTE;
		}
		else if(r_data.CompareNoCase(_CMD__QUERY_PRESSURE) == 0)
		{
			s_data = str__chm_pressure;
		}
		else if(r_data.CompareNoCase(_CMD__QUERY_PIN_STATE) == 0)
		{
			s_data = str__pin_state;
		}
		else if(r_data.CompareNoCase(_CMD__QUERY_SHUTTER_STATE) == 0)
		{
			s_data = str__shutter_state;
		}
		else if(r_data.CompareNoCase(_CMD__QUERY_CHAMBER_STATE) == 0)
		{
			if(x_timer__chm_state->Get__CURRENT_TIME() > 0.1)
			{
				s_data = STR__BUSY;
			}
			else
			{
				if(str__chm_action.CompareNoCase(_CMD__ACT_CHAMBER_PUMP) == 0)
					str__chm_pressure = "0.1";
				else if(str__chm_action.CompareNoCase(_CMD__ACT_CHAMBER_VENT) == 0)
					str__chm_pressure = "760.0";

				s_data = STR__IDLE;
			}
		}
		else if(r_data.CompareNoCase(_CMD__QUERY_PROCESS_STATE) == 0)
		{
			if(x_timer__prc_state->Get__CURRENT_TIME() > 0.1)
			{
				s_data = STR__PROCESSING;
			}
			else
			{
				s_data = STR__IDLE;
			}
		}
		else if((r_data.CompareNoCase(_CMD__ACT_TRANSFER_START) == 0)
			 || (r_data.CompareNoCase(_CMD__ACT_TRANSFER_END) == 0)
			 || (r_data.CompareNoCase(_CMD__ACT_PIN_UP) == 0)
			 || (r_data.CompareNoCase(_CMD__ACT_PIN_DOWN) == 0)
			 || (r_data.CompareNoCase(_CMD__ACT_CHAMBER_PUMP) == 0)
			 || (r_data.CompareNoCase(_CMD__ACT_CHAMBER_VENT) == 0)
			 || (r_data.CompareNoCase(_CMD__ACT_CHAMBER_ABORT) == 0)
			 || (r_data.CompareNoCase(_CMD__ACT_PROCESS_START) == 0)
			 || (r_data.CompareNoCase(_CMD__ACT_CLEAN_START) == 0)
			 || (r_data.CompareNoCase(_CMD__ACT_PROCESS_ABORT) == 0))
		{
			if(r_data.CompareNoCase(_CMD__ACT_TRANSFER_START) == 0)
			{
				str__shutter_state = STR__OPEN;
			}
			else if(r_data.CompareNoCase(_CMD__ACT_TRANSFER_END) == 0)
			{
				str__shutter_state = STR__CLOSE;
			}
			else if(r_data.CompareNoCase(_CMD__ACT_PIN_UP) == 0)
			{
				str__pin_state = STR__UP;
			}
			else if(r_data.CompareNoCase(_CMD__ACT_PIN_DOWN) == 0)
			{
				str__pin_state = STR__DOWN;
			}
			else if(r_data.CompareNoCase(_CMD__ACT_CHAMBER_PUMP) == 0)
			{
				double cfg_sec = aCH__CFG_CHM_PUMP_TIME->Get__VALUE();

				x_timer__chm_state->START__COUNT_DOWN(cfg_sec);

				str__chm_pressure = "100.0";
				str__chm_action = r_data;
			}
			else if(r_data.CompareNoCase(_CMD__ACT_CHAMBER_VENT) == 0)
			{
				double cfg_sec = aCH__CFG_CHM_VENT_TIME->Get__VALUE();

				x_timer__chm_state->START__COUNT_DOWN(cfg_sec);

				str__chm_pressure = "500.0";
				str__chm_action = r_data;
			}
			else if(r_data.CompareNoCase(_CMD__ACT_CHAMBER_ABORT) == 0)
			{
				x_timer__chm_state->STOP_ZERO();
			}
			else if(r_data.CompareNoCase(_CMD__ACT_PROCESS_START) == 0)
			{
				double cfg_sec = aCH__CFG_PROCESS_TIME->Get__VALUE();

				x_timer__prc_state->START__COUNT_DOWN(cfg_sec);
			}
			else if(r_data.CompareNoCase(_CMD__ACT_CLEAN_START) == 0)
			{
				double cfg_sec = aCH__CFG_CLEAN_TIME->Get__VALUE();

				x_timer__prc_state->START__COUNT_DOWN(cfg_sec);
			}
			else if(r_data.CompareNoCase(_CMD__ACT_PROCESS_ABORT) == 0)
			{
				x_timer__prc_state->STOP_ZERO();
			}

			s_data = STR__OK;
		}

		if(s_data.GetLength() > 0)
		{
			SEND__COMMAND(s_data);
		}
	}

	// ...
}
