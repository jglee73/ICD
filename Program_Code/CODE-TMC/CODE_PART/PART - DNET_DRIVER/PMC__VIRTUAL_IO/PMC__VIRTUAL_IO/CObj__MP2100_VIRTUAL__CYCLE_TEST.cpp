#include "StdAfx.h"
#include "CObj__MP2100_VIRTUAL.h"


// ...
int  CObj__MP2100_VIRTUAL
::Call__CYCLE_DOWN_UP(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	// ...
	{
		sCH__RESULT_MOVE_DOWN_UP_CYCLE->Set__DATA("__");

		sCH__RESULT_MOVE_DOWN_UP_AVG_UP->Set__DATA("__");
		sCH__RESULT_MOVE_DOWN_UP_MIN_UP->Set__DATA("__");
		sCH__RESULT_MOVE_DOWN_UP_MAX_UP->Set__DATA("__");

		sCH__RESULT_MOVE_DOWN_UP_AVG_DOWN->Set__DATA("__");
		sCH__RESULT_MOVE_DOWN_UP_MIN_DOWN->Set__DATA("__");
		sCH__RESULT_MOVE_DOWN_UP_MAX_DOWN->Set__DATA("__");
	}

	// 0. Initial ...
	{
		sCH__OBJ_SUB_MSG->Set__DATA("CALL__INIT() ...");

		if(Call__INIT(p_variable, p_alarm) < 0)
		{
			return -1;
		}
	}

	// ...
	{
		CString ch_data;
		int i_count = 0;

		while(1)
		{
			int cfg_cycle = (int) aCH__CFG_MOVE_DOWN_UP_CYCLE->Get__VALUE();
			if(i_count >= cfg_cycle)		break;

			// Up ...
			sCH__OBJ_SUB_MSG->Set__DATA("Call__UP() ...");

			if(Call__UP(p_variable,p_alarm) < 0)
			{
				break;
			}
			else
			{
				ch_data = sCH__OBJ_TIME_COUNT->Get__STRING();

				if(i_count < 1)
				{
					sCH__RESULT_MOVE_DOWN_UP_AVG_UP->Set__DATA(ch_data);
					sCH__RESULT_MOVE_DOWN_UP_MIN_UP->Set__DATA(ch_data);
					sCH__RESULT_MOVE_DOWN_UP_MAX_UP->Set__DATA(ch_data);
				}
				else
				{
					double sec_cur = atof(ch_data);

					// MIN
					{
						ch_data = sCH__RESULT_MOVE_DOWN_UP_MIN_UP->Get__STRING();
						double sec_min = atof(ch_data);
						if(sec_cur < sec_min)		
						{
							ch_data.Format("%.1f", sec_cur);
							sCH__RESULT_MOVE_DOWN_UP_MIN_UP->Set__DATA(ch_data);
						}
					}
					// MAX
					{
						ch_data = sCH__RESULT_MOVE_DOWN_UP_MAX_UP->Get__STRING();
						double sec_max = atof(ch_data);
						if(sec_cur < sec_max)		
						{
							ch_data.Format("%.1f", sec_cur);
							sCH__RESULT_MOVE_DOWN_UP_MAX_UP->Set__DATA(ch_data);
						}
					}
					// AVG
					{
						ch_data = sCH__RESULT_MOVE_DOWN_UP_AVG_UP->Get__STRING();
						double sec_avg  = atof(ch_data);
						double sec_rate = 1.0 / (i_count + 1);

						double new_avg = (sec_avg * (i_count * sec_rate)) + (sec_cur * sec_rate);

						ch_data.Format("%.1f", sec_cur);
						sCH__RESULT_MOVE_DOWN_UP_AVG_UP->Set__DATA(ch_data);
					}
				}
			}

			// Down ...
			sCH__OBJ_SUB_MSG->Set__DATA("Call__DOWN() ...");

			if(Call__DOWN(p_variable,p_alarm) < 0)
			{
				break;
			}
			else
			{
				ch_data = sCH__OBJ_TIME_COUNT->Get__STRING();

				if(i_count < 1)
				{
					sCH__RESULT_MOVE_DOWN_UP_AVG_DOWN->Set__DATA(ch_data);
					sCH__RESULT_MOVE_DOWN_UP_MIN_DOWN->Set__DATA(ch_data);
					sCH__RESULT_MOVE_DOWN_UP_MAX_DOWN->Set__DATA(ch_data);
				}
				else
				{
					double sec_cur = atof(ch_data);

					// MIN
					{
						ch_data = sCH__RESULT_MOVE_DOWN_UP_MIN_DOWN->Get__STRING();
						double sec_min = atof(ch_data);
						if(sec_cur < sec_min)		
						{
							ch_data.Format("%.1f", sec_cur);
							sCH__RESULT_MOVE_DOWN_UP_MIN_DOWN->Set__DATA(ch_data);
						}
					}
					// MAX
					{
						ch_data = sCH__RESULT_MOVE_DOWN_UP_MAX_DOWN->Get__STRING();
						double sec_max = atof(ch_data);
						if(sec_cur < sec_max)		
						{
							ch_data.Format("%.1f", sec_cur);
							sCH__RESULT_MOVE_DOWN_UP_MAX_DOWN->Set__DATA(ch_data);
						}
					}
					// AVG
					{
						ch_data = sCH__RESULT_MOVE_DOWN_UP_AVG_DOWN->Get__STRING();
						double sec_avg  = atof(ch_data);
						double sec_rate = 1.0 / (i_count + 1);

						double new_avg = (sec_avg * (i_count * sec_rate)) + (sec_cur * sec_rate);

						ch_data.Format("%.1f", sec_cur);
						sCH__RESULT_MOVE_DOWN_UP_AVG_DOWN->Set__DATA(ch_data);
					}
				}
			}

			i_count++;
			ch_data.Format("%1d", i_count);
			sCH__RESULT_MOVE_DOWN_UP_CYCLE->Set__DATA(ch_data);
		}

	}

	return 1;
}

int  CObj__MP2100_VIRTUAL
::Call__CYCLE_PROC_DOWN_UP(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	// ...
	{
		sCH__RESULT_PROC_DOWN_UP_CYCLE->Set__DATA("__");

		sCH__RESULT_PROC_DOWN_UP_AVG_UP->Set__DATA("__");
		sCH__RESULT_PROC_DOWN_UP_MIN_UP->Set__DATA("__");
		sCH__RESULT_PROC_DOWN_UP_MAX_UP->Set__DATA("__");

		sCH__RESULT_PROC_DOWN_UP_AVG_DOWN->Set__DATA("__");
		sCH__RESULT_PROC_DOWN_UP_MIN_DOWN->Set__DATA("__");
		sCH__RESULT_PROC_DOWN_UP_MAX_DOWN->Set__DATA("__");
	}

	// 0. Initial ...
	{
		sCH__OBJ_SUB_MSG->Set__DATA("Call__INIT() ...");

		if(Call__INIT(p_variable, p_alarm) < 0)
		{
			return -1;
		}
	}

	// ...
	{
		CString ch_data;
		int i_count = 0;

		while(1)
		{
			int cfg_cycle = (int) aCH__CFG_PROC_DOWN_UP_CYCLE->Get__VALUE();
			if(i_count >= cfg_cycle)		break;

			// PROC.UP ...
			sCH__OBJ_SUB_MSG->Set__DATA("Call__PROC_UP() ...");

			if(Call__PROC_UP(p_variable,p_alarm) < 0)
			{
				break;
			}
			else
			{
				ch_data = sCH__OBJ_TIME_COUNT->Get__STRING();

				if(i_count < 1)
				{
					sCH__RESULT_PROC_DOWN_UP_AVG_UP->Set__DATA(ch_data);
					sCH__RESULT_PROC_DOWN_UP_MIN_UP->Set__DATA(ch_data);
					sCH__RESULT_PROC_DOWN_UP_MAX_UP->Set__DATA(ch_data);
				}
				else
				{
					double sec_cur = atof(ch_data);

					// MIN
					{
						ch_data = sCH__RESULT_PROC_DOWN_UP_MIN_UP->Get__STRING();
						double sec_min = atof(ch_data);
						if(sec_cur < sec_min)		
						{
							ch_data.Format("%.1f", sec_cur);
							sCH__RESULT_PROC_DOWN_UP_MIN_UP->Set__DATA(ch_data);
						}
					}
					// MAX
					{
						ch_data = sCH__RESULT_PROC_DOWN_UP_MAX_UP->Get__STRING();
						double sec_max = atof(ch_data);
						if(sec_cur > sec_max)		
						{
							ch_data.Format("%.1f", sec_cur);
							sCH__RESULT_PROC_DOWN_UP_MAX_UP->Set__DATA(ch_data);
						}
					}
					// AVG
					{
						ch_data = sCH__RESULT_PROC_DOWN_UP_AVG_UP->Get__STRING();
						double sec_avg  = atof(ch_data);
						double sec_rate = 1.0 / (i_count + 1);

						double new_avg = (sec_avg * (i_count * sec_rate)) + (sec_cur * sec_rate);

						ch_data.Format("%.1f", sec_cur);
						sCH__RESULT_PROC_DOWN_UP_AVG_UP->Set__DATA(ch_data);
					}
				}
			}

			// PROC.DOWN ...
			sCH__OBJ_SUB_MSG->Set__DATA("Call__PROC_DOWN() ...");

			if(Call__PROC_DOWN(p_variable,p_alarm) < 0)
			{
				break;
			}
			else
			{
				ch_data = sCH__OBJ_TIME_COUNT->Get__STRING();

				if(i_count < 1)
				{
					sCH__RESULT_PROC_DOWN_UP_AVG_DOWN->Set__DATA(ch_data);
					sCH__RESULT_PROC_DOWN_UP_MIN_DOWN->Set__DATA(ch_data);
					sCH__RESULT_PROC_DOWN_UP_MAX_DOWN->Set__DATA(ch_data);
				}
				else
				{
					double sec_cur = atof(ch_data);

					// MIN
					{
						ch_data = sCH__RESULT_PROC_DOWN_UP_MIN_DOWN->Get__STRING();
						double sec_min = atof(ch_data);
						if(sec_cur < sec_min)		
						{
							ch_data.Format("%.1f", sec_cur);
							sCH__RESULT_PROC_DOWN_UP_MIN_DOWN->Set__DATA(ch_data);
						}
					}
					// MAX
					{
						ch_data = sCH__RESULT_PROC_DOWN_UP_MAX_DOWN->Get__STRING();
						double sec_max = atof(ch_data);
						if(sec_cur > sec_max)		
						{
							ch_data.Format("%.1f", sec_cur);
							sCH__RESULT_PROC_DOWN_UP_MAX_DOWN->Set__DATA(ch_data);
						}
					}
					// AVG
					{
						ch_data = sCH__RESULT_PROC_DOWN_UP_AVG_DOWN->Get__STRING();
						double sec_avg  = atof(ch_data);
						double sec_rate = 1.0 / (i_count + 1);

						double new_avg = (sec_avg * (i_count * sec_rate)) + (sec_cur * sec_rate);

						ch_data.Format("%.1f", sec_cur);
						sCH__RESULT_PROC_DOWN_UP_AVG_DOWN->Set__DATA(ch_data);
					}
				}
			}

			i_count++;
			ch_data.Format("%1d", i_count);
			sCH__RESULT_PROC_DOWN_UP_CYCLE->Set__DATA(ch_data);
		}

	}

	return 1;
}

