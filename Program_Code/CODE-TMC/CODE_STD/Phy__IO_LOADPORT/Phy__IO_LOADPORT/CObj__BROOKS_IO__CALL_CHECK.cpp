#include "StdAfx.h"
#include "CObj__BROOKS_IO.h"
#include "CObj__BROOKS_IO__DEF.h"


int  CObj__BROOKS_IO::
_Wait_CHECK(CII_OBJECT__VARIABLE* p_variable,
			CII_OBJECT__ALARM* p_alarm, 
			const CString& cmd_name,
			CII__VAR_DIGITAL_CTRL* p_ch__di_01,
			const CString& str__di_01,
			CII__VAR_DIGITAL_CTRL* p_ch__di_02,
			const CString& str__di_02)
{
_RETRY_LOOP:

	// ...
	double cfg_sec = 10.0;

	SCX__ASYNC_TIMER_CTRL x_timer_ctrl;
	x_timer_ctrl->START__COUNT_DOWN(cfg_sec);

	while(1)
	{
		bool active__check_di = true;

		if(p_ch__di_01 != NULL)
		{
			if(p_ch__di_01->Check__DATA(str__di_01) < 0)		active__check_di = false;
		}
		if(p_ch__di_02 != NULL)
		{
			if(p_ch__di_02->Check__DATA(str__di_02) < 0)		active__check_di = false;
		}

		if(active__check_di)
		{
			return 1;
		}

		Sleep(100);

		if(iSIM_FLAG > 0)
		{
			if(dCH__CFG_SIM_ERROR_ACTIVE->Check__DATA(STR__NO) > 0)
			{
				if(x_timer_ctrl->Get__CURRENT_TIME() < (cfg_sec / 2))
				{
					if(p_ch__di_01 != NULL)			p_ch__di_01->Set__DATA(str__di_01);
					if(p_ch__di_02 != NULL)			p_ch__di_02->Set__DATA(str__di_02);
				}
			}
		}

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			return -11;
		}

		if(x_timer_ctrl->Get__CURRENT_TIME() < 0.01)
		{
			CString str_log;

			str_log.Format(" * TimeOut <- %.0f sec ", cfg_sec);
			Fnc__APP_LOG(str_log);

			//
			int alarm_id = ALID__CMD_TIMEOUT_ALARM;
			CString alarm_msg;
			CString alarm_bff;
			CString r_act;

			alarm_msg.Format("\"%s\" command timeout (%.0f) sec \n", cmd_name, cfg_sec);
			alarm_bff.Format("Please, check I/O Input state \n");
			alarm_msg += alarm_bff;

			alarm_bff.Format("  %s <- %s \n", 
							diEXT_CH__LPx__CLAMP->Get__CHANNEL_NAME(),
							diEXT_CH__LPx__CLAMP->Get__STRING());
			alarm_msg += alarm_bff;

			alarm_bff.Format("  %s <- %s \n", 
							diEXT_CH__LPx__UNCLAMP->Get__CHANNEL_NAME(),
							diEXT_CH__LPx__UNCLAMP->Get__STRING());
			alarm_msg += alarm_bff;

			p_alarm->Popup__ALARM_With_MESSAGE(alarm_id, alarm_msg, r_act);

			if(r_act.CompareNoCase(ACT__RETRY) == 0)
			{
				str_log = "\"CLAMP\"command retry";
				Fnc__APP_LOG(str_log);

				goto _RETRY_LOOP;
			}
			return -12;
		}
	}

	return -21;
}
