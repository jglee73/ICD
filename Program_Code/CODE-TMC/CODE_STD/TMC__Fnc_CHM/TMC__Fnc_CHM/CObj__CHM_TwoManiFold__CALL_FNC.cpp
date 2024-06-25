#include "StdAfx.h"
#include "CObj__CHM_TwoManiFold.h"
#include "CObj__CHM_TwoManiFold__DEF.h"
#include "CObj__CHM_TwoManiFold__ALID.h"

#include "Macro_Function.h"


// TMC_CHM ---------------
// INIT -----
int  CObj__CHM_TwoManiFold
::Call__INIT(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm)
{
	if(pPMx__OBJ_CTRL->Call__OBJECT(CMMD__INIT) < 0)
	{
		return -1;
	}

	if(dEXT_CH__CFG_TRANSFER_MODE->Check__DATA(STR__ATM) > 0)
	{
		if(Check__ALL_VLV_CLOSE(p_variable,p_alarm,"Vent TMC-CHM") < 0)
		{
			return -11;
		}

		// TMC_CHM
		if(Call__VENT(p_variable,p_alarm) < 0)
		{
			return -103;
		}
	}
	else 
	{
		if(Check__ALL_VLV_CLOSE(p_variable,p_alarm,"Pump TMC-CHM") < 0)
		{
			return -11;
		}

		// TMC_CHM
		if(Call__PUMP(p_variable,p_alarm) < 0)
		{
			return -103;
		}
	}

	if(pTMC_CHM_PRESS__OBJ_CTRL->Call__OBJECT(CMMD__INIT) < 0)
	{
		return -1;
	}

	return 1;
}

// ISOLATE -----
int  CObj__CHM_TwoManiFold
::Call__ISOLATE(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm)
{
	pPRESS_VLV__OBJ_CTRL->Call__OBJECT("TMC_CHM_VLV_ALL_CLOSE");
	return 1;
}

// PUMP -----
int  CObj__CHM_TwoManiFold
::Call__PUMP(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm)
{
LOOP_RETRY:

	// ...
	dCH__PUMPING_STS_FLAG->Set__DATA("PUMPING");

	// LBA & LBB - PUMP VLV CLOSE
	if(Check__ALL_VLV_CLOSE(p_variable,p_alarm,"Pump TMC-CHM") < 0)
	{
		return -11;
	}

	// TMC CHM 
	pPRESS_VLV__OBJ_CTRL->Call__OBJECT(CMMD__TMC_CHM_PUMP_VLV_OPEN);

	if(pTMC_CHM_PRESS__OBJ_CTRL->Call__OBJECT(CMMD__PUMP) < 0)
	{
		return -101;
	}

	if(p_variable->Check__CTRL_ABORT() > 0)
	{
		return -102;
	}

	return 1;
}

// VENT -----
int  CObj__CHM_TwoManiFold
::Call__VENT(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm)
{
	int flag = Check__ALL_VLV_CLOSE(p_variable,p_alarm,"vent TMC-CHM");

	if(flag < 0)
	{
		return flag;
	}

	// TMC CHM
	if(pTMC_CHM_PRESS__OBJ_CTRL->Call__OBJECT(CMMD__VENT) < 0)
	{
		return -101;
	}

	if(p_variable->Check__CTRL_ABORT() > 0)
	{
		return -102;
	}

	return 1;
}

// LEAK_CHECK -----
int  CObj__CHM_TwoManiFold
::Call__LEAK_CHECK(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm)
{
	// ...
	{
		int i;

		sCH__TMC_LEAK_CHECK__START_TIME->Set__DATA("");
		sCH__TMC_LEAK_CHECK__END_TIME->Set__DATA("");
		sCH__TMC_LEAK_CHECK__START_PRESSURE_TORR->Set__DATA("");
		sCH__TMC_LEAK_CHECK__END_PRESSURE_TORR->Set__DATA("");
		sCH__TMC_LEAK_CHECK__TOTAL_LEAK_RATE->Set__DATA("");

		for(i=0;i<CFG_LEAK_RATE__STEP_SIZE;i++)
		{
			sCH__TMC_LEAK_CHECK__STEP_PRESSURE_TORR[i]->Set__DATA("");
			sCH__TMC_LEAK_CHECK__STEP_LEAK_RATE[i]->Set__DATA("");
		}
	}

	xCH__OBJ_MSG->Set__DATA("TMC-CHM Pumping ...");

	int flag = Call__PUMP(p_variable,p_alarm);
	if(flag < 0)		return -1;

	return Fnc__LEAK_CHECK(p_variable,p_alarm);
}
int  CObj__CHM_TwoManiFold
::Fnc__LEAK_CHECK(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm)
{
	CString log_msg;

	SCX__ASYNC_TIMER_CTRL x_timer_ctrl;
	double  delay_time;
	CString var_data;

	x_timer_ctrl->REGISTER__COUNT_CHANNEL(sObject_Name, aCH__TMC_LEAK_CHECK__TIME_COUNT->Get__VARIABLE_NAME());

	// ...
	CString str__unit = "mTorr";
	CString str__over_pump_time;
	CString str__stable_time;
	CString str__check_time;

	// Over-Pumping -----
	{
		log_msg = "TMC-CHM Leak_Check : Over-Pumping ...";

		xCH__OBJ_MSG->Set__DATA(log_msg);
		xLOG_CTRL->WRITE__LOG(log_msg);
	}

	// ...
	{
		aCH__CFG_TMC_LEAK_CHECK__OVER_PUMPING_TIME->Get__DATA(var_data);
		str__over_pump_time = var_data;
		delay_time = atof(var_data);

		x_timer_ctrl->START__COUNT_DOWN(delay_time);

		while(1)
		{
			Sleep(3);

			if(p_variable->Check__CTRL_ABORT() > 0)
			{
				return -1;
			}
			if(x_timer_ctrl->Get__CURRENT_TIME() < 0.1)
			{
				break;
			}
		}

		pPRESS_VLV__OBJ_CTRL->Call__OBJECT(CMMD__TMC_CHM_VAC_VLV_CLOSE);
	}

	// Stable Time -----
	{
		log_msg = "TMC-CHM Leak_Check : Stable Time ...";

		xCH__OBJ_MSG->Set__DATA(log_msg);
		xLOG_CTRL->WRITE__LOG(log_msg);
	}

	// ...
	{
		aCH__CFG_TMC_LEAK_CHECK__STABLE_TIME->Get__DATA(var_data);
		str__stable_time = var_data;
		delay_time = atof(var_data);

		x_timer_ctrl->START__COUNT_DOWN(delay_time);

		while(1)
		{
			Sleep(3);

			if(p_variable->Check__CTRL_ABORT() > 0)
			{
				return -1;
			}
			if(x_timer_ctrl->Get__CURRENT_TIME() < 0.1)
			{
				break;
			}
		}
	}

	// Leak Check -----
	{
		log_msg = "TMC-CHM Leak_Checking ...";

		xCH__OBJ_MSG->Set__DATA(log_msg);
		xLOG_CTRL->WRITE__LOG(log_msg);
	}

	// ...
	{
		CString date_time;
		Macro__Get_Date_Time(date_time);

		sCH__TMC_LEAK_CHECK__START_TIME->Set__DATA(date_time);

		aEXT_CH__TMC_CHM__PRESSURE_TORR->Get__DATA(var_data);
		sCH__TMC_LEAK_CHECK__START_PRESSURE_TORR->Set__DATA(var_data);

		// LeakCheck Log ...
		{
			sCH__TMC_LEAK_CHECK__START_PRESSURE_TORR->Get__DATA(var_data);
			double start_press = atof(var_data);

			CString log_msg;
			log_msg.Format("0,Start Pressure,%.3f,%s", start_press,str__unit);

			LeakCheck__LOG_MakeFile(MODULE_NAME__TMC);
			LeakCheck__LOG_WriteMsg(MODULE_NAME__TMC,log_msg);
		}

		// ...
		aCH__CFG_TMC_LEAK_CHECK__CHECK_TIME_MIN->Get__DATA(var_data);
		str__check_time = var_data;
		int check_min = atoi(var_data);

		for(int i=0;i<check_min;i++)
		{
			delay_time = 60.0;
			x_timer_ctrl->START__COUNT_DOWN(delay_time);

			while(1)
			{
				Sleep(3);

				if(p_variable->Check__CTRL_ABORT() > 0)
				{
					return -1;
				}
				if(x_timer_ctrl->Get__CURRENT_TIME() < 0.1)
				{
					break;
				}
			}

			if(i < CFG_LEAK_RATE__STEP_SIZE)
			{
				double pre_press;
				double cur_press;

				if(i < 1)
				{
					sCH__TMC_LEAK_CHECK__START_PRESSURE_TORR->Get__DATA(var_data);
					pre_press = atof(var_data);
				}
				else
				{
					sCH__TMC_LEAK_CHECK__STEP_PRESSURE_TORR[i-1]->Get__DATA(var_data);
					pre_press = atof(var_data);
				}

				aEXT_CH__TMC_CHM__PRESSURE_TORR->Get__DATA(var_data);
				sCH__TMC_LEAK_CHECK__STEP_PRESSURE_TORR[i]->Set__DATA(var_data);
				cur_press = atof(var_data);

				var_data.Format("%.3f", (cur_press-pre_press)/(i+1.0));
				sCH__TMC_LEAK_CHECK__STEP_LEAK_RATE[i]->Set__DATA(var_data);

				// LeakCheck Log ...
				{
					CString str_rate;
					CString str_press;

					sCH__TMC_LEAK_CHECK__STEP_LEAK_RATE[i]->Get__DATA(str_rate);
					sCH__TMC_LEAK_CHECK__STEP_PRESSURE_TORR[i]->Get__DATA(str_press);

					//
					CString log_msg;
					log_msg.Format("%d,%d min,%.3f,%.3f", 
									i+1, 
									i+1,
									atof(str_press), 
									atof(str_rate));

					LeakCheck__LOG_WriteMsg(MODULE_NAME__TMC,log_msg);
				}
			}
		}

		// ...
		{
			aEXT_CH__TMC_CHM__PRESSURE_TORR->Get__DATA(var_data);
			sCH__TMC_LEAK_CHECK__END_PRESSURE_TORR->Set__DATA(var_data);

			sCH__TMC_LEAK_CHECK__START_PRESSURE_TORR->Get__DATA(var_data);
			double pre_press = atof(var_data);

			aEXT_CH__TMC_CHM__PRESSURE_TORR->Get__DATA(var_data);
			double cur_press = atof(var_data);

			if(check_min < 1)		check_min = 1;
			var_data.Format("%.3f", (cur_press-pre_press)/check_min);
			sCH__TMC_LEAK_CHECK__TOTAL_LEAK_RATE->Set__DATA(var_data);
		}

		Macro__Get_Date_Time(date_time);
		sCH__TMC_LEAK_CHECK__END_TIME->Set__DATA(date_time);
	}

	// LeackCheck Log ...
	{
		CString log_msg;
		CString log_bff;

		log_msg += "#COMMENT";
		log_msg += "\n";

		log_msg += "*** TM1 LEAK CHECK INFORMATION ***";
		log_msg += "\n";

		log_bff.Format("(1) Over Pumping Time: %.1f sec                (2) Stable Time: %.1f sec             (3) Leak Check Time: %.0f min", 
						atof(str__over_pump_time),
						atof(str__stable_time),
						atof(str__check_time));
		log_msg += log_bff;
		log_msg += "\n";

		CString str__start_time;
		CString str__end_time;

		sCH__TMC_LEAK_CHECK__START_TIME->Get__DATA(str__start_time);
		sCH__TMC_LEAK_CHECK__END_TIME->Get__DATA(str__end_time);

		log_bff.Format("(4) Leak Check Start Time: %s        (5) Leak Check End Time: %s", 
						str__start_time,
						str__end_time);
		log_msg += log_bff;
		log_msg += "\n";

		sCH__TMC_LEAK_CHECK__START_PRESSURE_TORR->Get__DATA(var_data);

		log_bff.Format("(6) Start Pressure: %.3f %s", 
						atof(var_data),
						str__unit);
		log_msg += log_bff;
		log_msg += "\n";

		sCH__TMC_LEAK_CHECK__TOTAL_LEAK_RATE->Get__DATA(var_data);

		log_bff.Format("(7) Leak Rate: %.3f %s", 
						atof(var_data),
						str__unit);
		log_msg += log_bff;
		log_msg += "\n";

		//
		LeakCheck__LOG_WriteMsg(MODULE_NAME__TMC,log_msg);
	}

	// ...
	{
		sCH__TMC_LEAK_CHECK__TOTAL_LEAK_RATE->Get__DATA(var_data);
		double total_leak_rate = atof(var_data) * 1000.0;

		CFG_aCH__TMC_LEAK_CHECK__ALARM_RATE->Get__DATA(var_data);
		double cfg_leak_alarm = atof(var_data);

		if(total_leak_rate >= cfg_leak_alarm)
		{
			int alarm_id = ALID__TMC_LEAK_CHECK_BAD_RESULT_ALARM;
			CString alm_msg;
			CString alm_bff;
			CString r_act;

			alm_msg.Format(" * Total Leak_Rate  : %.0f (mtorr/min) \n", total_leak_rate);
			alm_bff.Format(" * Config Leak_Rate : %.0f (mtorr/min) \n", cfg_leak_alarm);
			alm_msg += alm_bff;

			p_alarm->Popup__ALARM_With_MESSAGE(alarm_id, alm_msg, r_act);
		}
	}
	return 1;
}

// CYCLE_PURGE -----
int  CObj__CHM_TwoManiFold
::Call__CYCLE_PURGE(CII_OBJECT__ALARM* p_alarm,CII_OBJECT__VARIABLE* p_variable)
{
	int flag;
	CString str_get_cfg;
	CString str_temp;

	int cfg_count;
	int current_count = 0;

	aCFG_CH__CYCLE_PURGE_COUNT->Get__DATA(str_get_cfg);
	cfg_count = atoi(str_get_cfg);

	do 
	{
		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			return OBJ_ABORT;
		}

		current_count++;
		str_temp.Format("%d", current_count);
		sCFG_CH__CYCLE_PURGE_CURRENT_COUNT->Set__DATA(str_temp);

		sCFG_CH__CYCLE_PURGE_SEQ_MSG->Set__DATA("Pumping Started...");

		flag = Call__PUMP(p_variable,p_alarm);
		if(flag < 0)
		{
			sCFG_CH__CYCLE_PURGE_SEQ_MSG->Set__DATA("Pumping Aborted...");
			return flag;
		}

		sCFG_CH__CYCLE_PURGE_SEQ_MSG->Set__DATA("Venting Started...");

		flag = Call__VENT(p_variable,p_alarm);
		if(flag < 0)
		{
			sCFG_CH__CYCLE_PURGE_SEQ_MSG->Set__DATA("Venting Aborted...");
			return flag;
		}

	} 
	while(cfg_count > current_count);

	sCFG_CH__CYCLE_PURGE_SEQ_MSG->Set__DATA("Pumping Started...");
	flag = Call__PUMP(p_variable,p_alarm);
	if(flag < 0)
	{
		sCFG_CH__CYCLE_PURGE_SEQ_MSG->Set__DATA("Pumping Aborted...");
		return flag;
	}

	sCFG_CH__CYCLE_PURGE_SEQ_MSG->Set__DATA("Completed...");
	return 1;
}
