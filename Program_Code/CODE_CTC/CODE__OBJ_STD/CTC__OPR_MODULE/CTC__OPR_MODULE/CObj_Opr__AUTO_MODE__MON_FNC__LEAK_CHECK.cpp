#include "StdAfx.h"
#include "CObj_Opr__AUTO_MODE.h"

#include "Macro_Function.h"


#include "CMacro_FA.h"
extern CMacro_FA mFA_Link;


// ...
void CObj_Opr__AUTO_MODE
::Fnc__LEAK_CHECK(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm)
{
	if(xSCH_MATERIAL_CTRL->Get__MATERIAL_DB_SIZE() > 0)
	{
		return;
	}

	if(xEXT_CH__LEAK_CHECK__IDLE_USE_FLAG->Check__DATA("ENABLE") > 0)
	{
		if(xCH__LEAK_CHECK__IDLE_PRE_STATUS->Check__DATA("ENABLE") < 0)
		{
			xCH__LEAK_CHECK__IDLE_PRE_STATUS->Set__DATA("ENABLE");
		}

		if(xCH__JOB_STATUS->Check__DATA(STR__IDLE) > 0)
		{
			for(int i=0; i<iPMx_UNIT_SIZE; i++)
			{
				xEXT_CH__PMx_LEAK_CHECK_FLAG[i]->Set__DATA("");
			}

			Fnc__IDLE_LEAK_CHECK(p_variable,p_alarm);
		}
		else
		{
			Fnc__TIME_LEAK_CHECK(p_variable,p_alarm);
		}
	}
	else
	{
		if(xCH__LEAK_CHECK__IDLE_PRE_STATUS->Check__DATA("DISABLE") < 0)
		{
			xCH__LEAK_CHECK__IDLE_PRE_STATUS->Set__DATA("DISABLE");

			xI_LEAK_CHECK_IDLE_TIMER->STOP();
			Sleep(100);
			xCH__LEAK_CHECK_START_FLAG->Set__DATA("");
			xEXT_CH__LEAK_CHECK__IDLE_CUR_TIME->Set__DATA("0");
		}

		Fnc__TIME_LEAK_CHECK(p_variable,p_alarm);
	}
}

int  CObj_Opr__AUTO_MODE
::Fnc__IDLE_LEAK_CHECK(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm)
{
	// ...
	{
		CTime time = CTime::GetCurrentTime();
		CString str_date;

		str_date.Format("%00004d.%002d.%002d",
						time.GetYear(),
						time.GetMonth(),
						time.GetDay());

		if(xCH__LEAK_CHECK__IDLE_PRE_DATE->Check__DATA(str_date) < 0)
		{
			xCH__LEAK_CHECK__IDLE_PRE_DATE->Set__DATA(str_date);

			xCH__LEAK_CHECK_START_FLAG->Set__DATA("");
			xEXT_CH__LEAK_CHECK__IDLE_CUR_COUNT->Set__DATA("0");
			xEXT_CH__LEAK_CHECK__IDLE_CUR_TIME->Set__DATA("0");
		}
	}

	// ...
	CString var_data;
	int ref_count;
	int cur_count;

	xEXT_CH__LEAK_CHECK__IDLE_REF_COUNT->Get__DATA(var_data);
	ref_count = atoi(var_data);

	xEXT_CH__LEAK_CHECK__IDLE_CUR_COUNT->Get__DATA(var_data);
	cur_count = atoi(var_data);

	if(ref_count <= cur_count)
	{
		return 1;
	}

	if(xCH__LEAK_CHECK_START_FLAG->Check__DATA("YES") < 0)
	{
		xCH__LEAK_CHECK_START_FLAG->Set__DATA("YES");

		for(int i=0; i<iPMx_UNIT_SIZE; i++)
		{
			xEXT_CH__PMx_LEAK_CHECK_FLAG[i]->Set__DATA("");
		}

		// ...
		CString var_data;
		double  ref_time;

		xEXT_CH__LEAK_CHECK__IDLE_REF_TIME->Get__DATA(var_data);
		ref_time = atof(var_data);

		xI_LEAK_CHECK_IDLE_TIMER->STOP();
		xI_LEAK_CHECK_IDLE_TIMER->START__COUNT_DOWN(ref_time);

		Sleep(1500);		// SYSTEM 안정화 TIME
	}

	if(xI_LEAK_CHECK_IDLE_TIMER->Get__CURRENT_TIME() > 0)
	{
		return 1;
	}

	// ...
	var_data.Format("%1d",cur_count+1);
	xEXT_CH__LEAK_CHECK__IDLE_CUR_COUNT->Set__DATA(var_data);

	xCH__LEAK_CHECK_START_FLAG->Set__DATA("");
	xCH__LEAK_CHECK__IDLE_PRE_DATE->Get__DATA(var_data);
	xCH__LEAK_CHECK__IDLE_RUN_DATE->Set__DATA(var_data);

	// ...
	{
		int i;

		// START : LEAK_CHECK
		for(i=0; i<iPMx_UNIT_SIZE; i++)
		{
			if(xEXT_CH__PMx_CFG_USE[i]->Check__DATA(STR__ENABLE) < 0)		
			{
				continue;
			}

			xEXT_CH__PMx_LEAK_CHECK_FLAG[i]->Set__DATA("YES");
		}

		// COMPELTE : LEAK_CHECK
		int leak_check_flag = -1;

		do
		{
			leak_check_flag = -1;

			for(i=0; i<iPMx_UNIT_SIZE; i++)
			{
				if(xEXT_CH__PMx_LEAK_CHECK_FLAG[i]->Check__DATA("YES") < 0)
				{
					continue;
				}

				leak_check_flag = 1;
				Sleep(90);
				break;
			}
		}
		while(leak_check_flag > 0);
	}

	return 1;
}
int  CObj_Opr__AUTO_MODE::
Fnc__TIME_LEAK_CHECK(CII_OBJECT__VARIABLE *p_variable,
					 CII_OBJECT__ALARM *p_alarm)
{
	// ...
	{
		int pm_leak_check_flag = -1;
		int i;

		for(i=0; i<iPMx_UNIT_SIZE; i++)
		{
			if(xEXT_CH__PMx_LEAK_CHECK_FLAG[i]->Check__DATA("YES") < 0)
			{
				continue;
			}

			pm_leak_check_flag = 1;
			break;
		}

		if(pm_leak_check_flag < 0)
		{
			return 1;
		}
	}

	Sleep(1500);  // SYSTEM 안정화 TIME
	xCH__JOB_READY_START_FLAG->Set__DATA("");

	Seq__CHECK_PORT_COMPLETE(p_alarm);

	// ...
	int loop_check = 1;
	int i;

	while(loop_check > 0)
	{
		loop_check = -1;

		for(i=0; i<iPMx_UNIT_SIZE; i++)
		{
			if(xEXT_CH__PMx_CFG_USE[i]->Check__DATA(STR__ENABLE) < 0)
			{
				continue;
			}
			if(xEXT_CH__PMx_LEAK_CHECK_FLAG[i]->Check__DATA("YES") < 0)
			{
				continue;
			}

			loop_check = 1;

			Sleep(90);
			break;
		}
	}

	return 1;
}
