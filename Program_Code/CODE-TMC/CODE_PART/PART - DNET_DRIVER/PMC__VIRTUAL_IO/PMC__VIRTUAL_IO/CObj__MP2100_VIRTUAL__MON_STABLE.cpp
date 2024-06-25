#include "StdAfx.h"
#include "CObj__MP2100_VIRTUAL.h"


// ...
int CObj__MP2100_VIRTUAL
::Mon__STABLE(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm)
{
	SCX__ASYNC_TIMER_CTRL x_timer__loop_stable_delay[CFG__AXIS_SIZE];
	SCX__ASYNC_TIMER_CTRL x_timer__loop_check_delay[CFG__AXIS_SIZE];
	SCX__ASYNC_TIMER_CTRL x_timer__loop_change_delay[CFG__AXIS_SIZE];

	double  pre__ch_set[CFG__AXIS_SIZE];
	CString ch_data;

	// ...
	{
		int i;

		for(i=0; i<CFG__AXIS_SIZE; i++)
		{
			pre__ch_set[i] = 0.0;

			x_timer__loop_stable_delay[i]->REGISTER__COUNT_CHANNEL_NAME(sCH__MON_AXIS_STABLE_TIME[i]->Get__CHANNEL_NAME());
			x_timer__loop_check_delay[i]->REGISTER__COUNT_CHANNEL_NAME(sCH__MON_AXIS_FAULT_TIME[i]->Get__CHANNEL_NAME());
			x_timer__loop_change_delay[i]->REGISTER__COUNT_CHANNEL_NAME(sCH__MON_AXIS_CHANGE_TIME[i]->Get__CHANNEL_NAME());

			x_timer__loop_stable_delay[i]->STOP_ZERO();
			x_timer__loop_check_delay[i]->STOP_ZERO();
			x_timer__loop_change_delay[i]->STOP_ZERO();
		}
	}

	while(1)
	{
		Sleep(100);

		// ...
		SCI__ASYNC_TIMER_CTRL *p_timer__change_delay;
		SCI__ASYNC_TIMER_CTRL *p_timer__stable_delay;
		SCI__ASYNC_TIMER_CTRL *p_timer__check_delay;

		CII__VAR_STRING_CTRL *p_ch__change;
		CII__VAR_STRING_CTRL *p_ch__stable;
		CII__VAR_STRING_CTRL *p_ch__fault;

		double cur__ch_set;
		double *p_pre__ch_set;
		int i;

		for(i=0; i<iAXIS_SIZE; i++)
		{
			// ...
			{
				p_timer__change_delay = x_timer__loop_change_delay[i].Get__PTR();
				p_timer__stable_delay = x_timer__loop_stable_delay[i].Get__PTR();
				p_timer__check_delay  = x_timer__loop_check_delay[i].Get__PTR();

				p_ch__change = sCH__MON_AXIS_CHANGE_STATE[i].Get__PTR();
				p_ch__stable = sCH__MON_AXIS_STABLE_STATE[i].Get__PTR();
				p_ch__fault  = sCH__MON_AXIS_FAULT_STATE[i].Get__PTR();

				ch_data = sCH__MON_IO_APOS_MM_SET->Get__STRING();
				cur__ch_set = atof(ch_data);
				p_pre__ch_set = &pre__ch_set[i];
			}

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

			if(Fnc__STABLE_CHECK(p_alarm, i, -1) < 0)
			{
				if(p_timer__check_delay->Get__CURRENT_TIME() < 0.001)
				{
					p_timer__check_delay->START__COUNT_UP(9999);
				}

				double cfg__check_delay = aCH__CFG_CHECK_TIMEOUT_FOR_ERROR ->Get__VALUE();
				if(cfg__check_delay <= p_timer__check_delay->Get__CURRENT_TIME())
				{
					if(Fnc__STABLE_CHECK(p_alarm, i) < 0)
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

		// ...
		{
			int stable_check = 1;
			int fault_check  = -1;

			for(i=0; i<iAXIS_SIZE; i++)
			{
				if(sCH__MON_AXIS_STABLE_STATE[i]->Check__DATA(STR__YES) < 0)
				{
					stable_check = -1;
				}
				if(sCH__MON_AXIS_FAULT_STATE[i]->Check__DATA(STR__YES) > 0)
				{
					fault_check = 1;
				}
			}

			if(stable_check > 0)
			{
				sCH__MON_STABLE_FLAG->Set__DATA(STR__ON);
			}
			else
			{
				sCH__MON_STABLE_FLAG->Set__DATA("");
			}

			if(fault_check > 0)
			{
				sCH__MON_FAULT_FLAG->Set__DATA(STR__ON);
			}
			else
			{
				sCH__MON_FAULT_FLAG->Set__DATA("");
			}
		}

		// ...
	}

	return 1;
}

int CObj__MP2100_VIRTUAL
::Fnc__STABLE_CHECK(CII_OBJECT__ALARM* p_alarm, const int axis_index, const int alm_report)
{
	int alm_id__warning;
	int alm_id__fault;

	double cur_set = 0.0;
	double cur_get = 0.0;

	CString ch_data;

	// ...
	{
		alm_id__warning = ALID__AXIS_WARNING + axis_index;
		alm_id__fault   = ALID__AXIS_FAULT   + axis_index;

		ch_data = sCH__MON_IO_APOS_MM_SET->Get__STRING();
		cur_set = atof(ch_data);

		ch_data = sCH__MON_IO_APOS_MM_GET->Get__STRING();
		cur_get = atof(ch_data);
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

				alm_bff.Format("target position  <- %.1f (mm) \n", cur_set);
				alm_msg += alm_bff;
				alm_bff.Format("current position <- %.1f (mm) \n", cur_get);
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

				alm_bff.Format("target position  <- %.1f (mm) \n", cur_set);
				alm_msg += alm_bff;
				alm_bff.Format("current position <- %.1f (mm) \n", cur_get);
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
