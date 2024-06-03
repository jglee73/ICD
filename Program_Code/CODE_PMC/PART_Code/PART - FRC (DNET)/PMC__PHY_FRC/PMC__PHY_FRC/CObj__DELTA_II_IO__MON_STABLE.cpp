#include "StdAfx.h"
#include "CObj__DELTA_II_IO.h"
#include "CObj__DELTA_II_IO__ALID.h"


#define CH_TYPE__1					1
#define CH_TYPE__2					2

#define STR__YES					"YES"
#define STR__ON						"ON"


// ...
int CObj__DELTA_II_IO
::Mon__STABLE(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm)
{
	SCX__ASYNC_TIMER_CTRL x_timer__ch1_stable_delay;
	SCX__ASYNC_TIMER_CTRL x_timer__ch1_check_delay;
	SCX__ASYNC_TIMER_CTRL x_timer__ch1_change_delay;

	SCX__ASYNC_TIMER_CTRL x_timer__ch2_stable_delay;
	SCX__ASYNC_TIMER_CTRL x_timer__ch2_check_delay;
	SCX__ASYNC_TIMER_CTRL x_timer__ch2_change_delay;

	double pre__ch1_set = -1.0;
	double pre__ch2_set = -1.0;

	CString var_data;

	// ...
	{
		x_timer__ch1_stable_delay->REGISTER__COUNT_CHANNEL_NAME(sCH__MON_CH1_STABLE_TIME->Get__CHANNEL_NAME());
		x_timer__ch1_check_delay->REGISTER__COUNT_CHANNEL_NAME(sCH__MON_CH1_FAULT_TIME->Get__CHANNEL_NAME());
		x_timer__ch1_change_delay->REGISTER__COUNT_CHANNEL_NAME(sCH__MON_CH1_CHANGE_TIME->Get__CHANNEL_NAME());

		x_timer__ch2_stable_delay->REGISTER__COUNT_CHANNEL_NAME(sCH__MON_CH2_STABLE_TIME->Get__CHANNEL_NAME());
		x_timer__ch2_check_delay->REGISTER__COUNT_CHANNEL_NAME(sCH__MON_CH2_FAULT_TIME->Get__CHANNEL_NAME());
		x_timer__ch2_change_delay->REGISTER__COUNT_CHANNEL_NAME(sCH__MON_CH2_CHANGE_TIME->Get__CHANNEL_NAME());
	}

	// ...
	{
		x_timer__ch1_stable_delay->STOP_ZERO();
		x_timer__ch1_check_delay->STOP_ZERO();
		x_timer__ch1_change_delay->STOP_ZERO();

		x_timer__ch2_stable_delay->STOP_ZERO();
		x_timer__ch2_check_delay->STOP_ZERO();
		x_timer__ch2_change_delay->STOP_ZERO();
	}

	while(1)
	{
		Sleep(100);

		// ...
		int ch_type = -1;

		SCI__ASYNC_TIMER_CTRL *p_timer__change_delay;
		SCI__ASYNC_TIMER_CTRL *p_timer__stable_delay;
		SCI__ASYNC_TIMER_CTRL *p_timer__check_delay;

		CII__VAR_STRING_CTRL *p_ch__change;
		CII__VAR_STRING_CTRL *p_ch__stable;
		CII__VAR_STRING_CTRL *p_ch__fault;

		double cur__ch_set;
		double *p_pre__ch_set;

		int i_limit = 2;
		int i;

		for(i=0; i<i_limit; i++)
		{

			if(i == 0)
			{
				ch_type = CH_TYPE__1;

				p_timer__change_delay = x_timer__ch1_change_delay.Get__PTR();
				p_timer__stable_delay = x_timer__ch1_stable_delay.Get__PTR();
				p_timer__check_delay  = x_timer__ch1_check_delay.Get__PTR();

				p_ch__change = sCH__MON_CH1_CHANGE_STATE.Get__PTR();
				p_ch__stable = sCH__MON_CH1_STABLE_STATE.Get__PTR();
				p_ch__fault  = sCH__MON_CH1_FAULT_STATE.Get__PTR();

				cur__ch_set = aCH__PARA_CH1_SET_RATIO->Get__VALUE();
				p_pre__ch_set = &pre__ch1_set;
			}
			else if(i == 1)
			{
				ch_type = CH_TYPE__2;

				p_timer__change_delay = x_timer__ch2_change_delay.Get__PTR();
				p_timer__stable_delay = x_timer__ch2_stable_delay.Get__PTR();
				p_timer__check_delay  = x_timer__ch2_check_delay.Get__PTR();

				p_ch__change = sCH__MON_CH2_CHANGE_STATE.Get__PTR();
				p_ch__stable = sCH__MON_CH2_STABLE_STATE.Get__PTR();
				p_ch__fault  = sCH__MON_CH2_FAULT_STATE.Get__PTR();

				cur__ch_set = aCH__PARA_CH2_SET_RATIO->Get__VALUE();
				p_pre__ch_set = &pre__ch2_set;
			}
			else
			{
				continue;
			}

			// ...
			if(*p_pre__ch_set != cur__ch_set)
			{
				*p_pre__ch_set = cur__ch_set;

				p_timer__change_delay->START__COUNT_UP(9999);
				p_ch__change->Set__DATA("");

				p_timer__stable_delay->STOP_ZERO();
				p_ch__stable->Set__DATA("");

				p_timer__check_delay->STOP_ZERO();
				p_ch__fault->Set__DATA("");
				continue;
			}

			if(p_ch__change->Check__DATA(STR__YES) < 0)
			{
				double cfg__change_delay = aCH__CFG_STABLE_TIME_FOR_SETPINT_CHANGE->Get__VALUE();
				if(cfg__change_delay > p_timer__change_delay->Get__CURRENT_TIME())
				{
					continue;
				}

				p_timer__change_delay->STOP_ZERO();
				p_ch__change->Set__DATA(STR__YES);
			}

			if(Fnc__STABLE_CHECK(p_alarm, ch_type, -1) < 0)
			{
				if(p_timer__check_delay->Get__CURRENT_TIME() < 0.001)
				{
					p_timer__check_delay->START__COUNT_UP(9999);
				}

				double cfg__check_delay = aCH__CFG_CHECK_TIMEOUT_FOR_ERROR ->Get__VALUE();
				if(cfg__check_delay <= p_timer__check_delay->Get__CURRENT_TIME())
				{
					if(Fnc__STABLE_CHECK(p_alarm, ch_type) < 0)
					{
						p_timer__check_delay->STOP_ZERO();
						p_ch__fault->Set__DATA(STR__YES);
					}
				}

				p_timer__stable_delay->STOP_ZERO();
				p_ch__stable->Set__DATA("");
			}
			else
			{
				if(p_timer__stable_delay->Get__CURRENT_TIME() < 0.001)
				{
					p_timer__stable_delay->START__COUNT_UP(9999);
				}

				double cfg__check_stable = aCH__CFG_CHECKING_TIME_FOR_STABLE->Get__VALUE();
				if(cfg__check_stable <= p_timer__stable_delay->Get__CURRENT_TIME())
				{
					p_timer__stable_delay->STOP_ZERO();
					p_ch__stable->Set__DATA(STR__YES);
				}

				p_timer__check_delay->STOP_ZERO();
				p_ch__fault->Set__DATA("");
			}
		}


		if((sCH__MON_CH1_STABLE_STATE->Check__DATA(STR__YES) > 0)
		&& (sCH__MON_CH2_STABLE_STATE->Check__DATA(STR__YES) > 0))
		{
			sCH__MON_STABLE_FLAG->Set__DATA(STR__ON);
		}
		else
		{
			sCH__MON_STABLE_FLAG->Set__DATA("");
		}

		if((sCH__MON_CH1_FAULT_STATE->Check__DATA(STR__YES) > 0)
		|| (sCH__MON_CH2_FAULT_STATE->Check__DATA(STR__YES) > 0))
		{
			sCH__MON_FAULT_FLAG->Set__DATA(STR__ON);
		}
		else
		{
			sCH__MON_FAULT_FLAG->Set__DATA("");
		}

		// ...
	}

	return 1;
}

int CObj__DELTA_II_IO
::Fnc__STABLE_CHECK(CII_OBJECT__ALARM* p_alarm, const int ch_type, const int alm_report)
{
	int alm_id__warning;
	int alm_id__fault;
		
	double cur_set = 0.0;
	double cur_get = 0.0;

	if(ch_type == CH_TYPE__1)
	{
		alm_id__warning = ALID__CH1_WARNING;
		alm_id__fault   = ALID__CH1_FAULT;

		cur_set = aCH__PARA_CH1_SET_RATIO->Get__VALUE();
		cur_get = atof(sCH__MON_CH1_GET_RATIO->Get__STRING());
	}
	else if(ch_type == CH_TYPE__2)
	{
		alm_id__warning = ALID__CH2_WARNING;
		alm_id__fault   = ALID__CH2_FAULT;

		cur_set = aCH__PARA_CH2_SET_RATIO->Get__VALUE();
		cur_get = atof(sCH__MON_CH2_GET_RATIO->Get__STRING());
	}
	else
	{
		return 1;
	}

	double cfg__warning_limit = aCH__CFG_WARNING_LIMIT->Get__VALUE();
	double cfg__fault_limit   = aCH__CFG_FAULT_LIMIT->Get__VALUE();

	double ref__warning_percent = cfg__warning_limit / 100.0;
	double ref__fault_percent   = cfg__fault_limit / 100.0;

	double ref__warning_min = cur_set * (1.0 - ref__warning_percent);
	double ref__warning_max = cur_set * (1.0 + ref__warning_percent);

	double ref__fault_min = cur_set * (1.0 - ref__fault_percent);
	double ref__fault_max = cur_set * (1.0 + ref__fault_percent);

	// ...
	int err_check = -1;
	
	// Warning Check ...
	if(cfg__warning_limit > 0.001)
	{
		if((cur_get < ref__warning_min)
		|| (cur_get > ref__warning_max))
		{
			err_check = 1;

			if(alm_report > 0)
			{
				int alm_id = alm_id__warning;
				CString alm_msg;
				CString alm_bff;
				CString r_act;

				alm_bff.Format("current ratio <- %.1f \n", cur_get);
				alm_msg += alm_bff;
				alm_bff.Format("config warning Min. <- %.1f \n", ref__warning_min);
				alm_msg += alm_bff;
				alm_bff.Format("config warning Max. <- %.1f \n", ref__warning_max);
				alm_msg += alm_bff;

				p_alarm->Check__ALARM(alm_id, r_act);
				p_alarm->Post__ALARM_With_MESSAGE(alm_id, alm_msg);
			}
		}
	}

	// Fault Check ...
	if(cfg__fault_limit > 0.001)
	{
		if((cur_get < ref__fault_min)
		|| (cur_get > ref__fault_max))
		{
			err_check = 1;

			if(alm_report > 0)
			{
				int alm_id = alm_id__fault;
				CString alm_msg;
				CString alm_bff;
				CString r_act;

				alm_bff.Format("current ratio <- %.1f \n", cur_get);
				alm_msg += alm_bff;
				alm_bff.Format("config fault Min. <- %.1f \n", ref__fault_min);
				alm_msg += alm_bff;
				alm_bff.Format("config fault Max. <- %.1f \n", ref__fault_max);
				alm_msg += alm_bff;

				p_alarm->Check__ALARM(alm_id, r_act);
				p_alarm->Post__ALARM_With_MESSAGE(alm_id, alm_msg);
			}		
		}
	}

	if(err_check > 0)
	{
		return -1;
	}
	return 1;
}
