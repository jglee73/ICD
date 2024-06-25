#include "StdAfx.h"
#include "CObj__LLx_TwoManiFold.h"
#include "CObj__LLx_TwoManiFold__DEF.h"
#include "CObj__LLx_TwoManiFold__ALID.h"

#include "Macro_Function.h"


int  CObj__LLx_TwoManiFold
::Call__LEFT_PUMP(CII_OBJECT__VARIABLE* p_variable,
			      CII_OBJECT__ALARM* p_alarm)
{
LOOP_RETRY:

	pPRESS_VLV__OBJ_CTRL->Call__OBJECT(CMMD__LBA_PUMP_VLV_OPEN);
	
	if(pLBA_PRESS__OBJ_CTRL->Call__OBJECT(CMMD__PUMP) < 0)
	{
		return -101;
	}

	if(p_variable->Check__CTRL_ABORT() > 0)
	{
		return -102;
	}

	// ...
	CString obj_sts;

	// PRESSURE CHECK -----
	int retry_flag = -1;

	if(Check__SYSTEM_VAC_PRESSURE(p_variable, p_alarm, -1, 1, retry_flag) < 0)
	{
		return -301;
	}

	if(retry_flag > 0)
	{
		goto LOOP_RETRY;
	}

	// PUMP VLV OPEN -----
	// RIGTH ...
	{
		pLBB_PRESS__OBJ_CTRL->Get__OBJECT_STATUS(obj_sts);

		if(Check__OBJECT_BUSY(obj_sts) < 0)
		{
			if(sEXT_CH__LBB__PRESSURE_CTRL_FLAG->Check__DATA("PUMP") > 0)
			{
				if(dEXT_CH__LBB__PRESSURE_STATUS->Check__DATA(STR__VAC) > 0)
				{
					pPRESS_VLV__OBJ_CTRL->Call__OBJECT(CMMD__LBB_VAC_VLV_OPEN);
				}
			}
		}
	}
	return 1;
}

int  CObj__LLx_TwoManiFold::
Call__LEFT_LEAK_CHECK(CII_OBJECT__VARIABLE* p_variable,
					  CII_OBJECT__ALARM* p_alarm)
{
	// ...
	{
		int i;

		sCH__LBA_LEAK_CHECK__START_TIME->Set__DATA("");
		sCH__LBA_LEAK_CHECK__END_TIME->Set__DATA("");
		sCH__LBA_LEAK_CHECK__START_PRESSURE_TORR->Set__DATA("");
		sCH__LBA_LEAK_CHECK__END_PRESSURE_TORR->Set__DATA("");
		sCH__LBA_LEAK_CHECK__TOTAL_LEAK_RATE->Set__DATA("");

		for(i=0; i<CFG_LEAK_RATE__STEP_SIZE; i++)
		{
			sCH__LBA_LEAK_CHECK__STEP_PRESSURE_TORR[i]->Set__DATA("");
			sCH__LBA_LEAK_CHECK__STEP_LEAK_RATE[i]->Set__DATA("");
		}
	}

	xCH__OBJ_MSG->Set__DATA("Left-Pumping ...");

	int flag = Call__LEFT_PUMP(p_variable,p_alarm);
	if(flag < 0)		return -1;

	return Fnc__LEFT_LEAK_CHECK(p_variable,p_alarm);
}
int  CObj__LLx_TwoManiFold
::Fnc__LEFT_LEAK_CHECK(CII_OBJECT__VARIABLE* p_variable,
					   CII_OBJECT__ALARM* p_alarm)
{
	CString log_msg;

	SCX__ASYNC_TIMER_CTRL x_timer_ctrl;
	double  delay_time;
	CString var_data;

	x_timer_ctrl->REGISTER__COUNT_CHANNEL(sObject_Name, aCH__LBA_LEAK_CHECK__TIME_COUNT->Get__VARIABLE_NAME());

	// ...
	CString str__unit = "mTorr";
	CString str__over_pump_time;
	CString str__stable_time;
	CString str__check_time;

	// Over-Pumping ...
	{
		log_msg = "Left-LLx's Leak_Check : Over-Pumping ...";

		xCH__OBJ_MSG->Set__DATA(log_msg);
		xLOG_CTRL->WRITE__LOG(log_msg);
	}

	// ...
	{
		aCH__CFG_LBA_LEAK_CHECK__OVER_PUMPING_TIME->Get__DATA(var_data);
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

		pPRESS_VLV__OBJ_CTRL->Call__OBJECT(CMMD__LBA_VAC_VLV_CLOSE);
	}

	// Stable Time -----
	{
		log_msg = "Left-LLx's Leak_Check : Stable Time ...";

		xCH__OBJ_MSG->Set__DATA(log_msg);
		xLOG_CTRL->WRITE__LOG(log_msg);
	}

	// ...
	{
		aCH__CFG_LBA_LEAK_CHECK__STABLE_TIME->Get__DATA(var_data);
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
		log_msg = "Left-LLx's Leak_Checking ...";

		xCH__OBJ_MSG->Set__DATA(log_msg);
		xLOG_CTRL->WRITE__LOG(log_msg);
	}

	// ...
	{
		CString date_time;
		Macro__Get_Date_Time(date_time);

		sCH__LBA_LEAK_CHECK__START_TIME->Set__DATA(date_time);

		aEXT_CH__LBA__PRESSURE_TORR->Get__DATA(var_data);
		sCH__LBA_LEAK_CHECK__START_PRESSURE_TORR->Set__DATA(var_data);

		// LeakCheck Log -----
		{
			sCH__LBA_LEAK_CHECK__START_PRESSURE_TORR->Get__DATA(var_data);
			double start_pres = atof(var_data);

			CString log_msg;
			log_msg.Format("0,Start Pressure,%.3f,%s", start_pres,str__unit);

			LeakCheck__LOG_MakeFile__LLx_Left();
			LeakCheck__LOG_WriteMsg__LLx_Left(log_msg);
		}

		int check_min = atoi(var_data);
		int i;

		aCH__CFG_LBA_LEAK_CHECK__CHECK_TIME_MIN->Get__DATA(var_data);
		str__check_time = var_data;
		check_min = atoi(var_data);

		for(i=0;i<check_min;i++)
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
					sCH__LBA_LEAK_CHECK__START_PRESSURE_TORR->Get__DATA(var_data);
					pre_press = atof(var_data);
				}
				else
				{
					sCH__LBA_LEAK_CHECK__STEP_PRESSURE_TORR[i-1]->Get__DATA(var_data);
					pre_press = atof(var_data);
				}

				aEXT_CH__LBA__PRESSURE_TORR->Get__DATA(var_data);
				sCH__LBA_LEAK_CHECK__STEP_PRESSURE_TORR[i]->Set__DATA(var_data);
				cur_press = atof(var_data);

				var_data.Format("%.3f", (cur_press-pre_press)/(i+1));
				sCH__LBA_LEAK_CHECK__STEP_LEAK_RATE[i]->Set__DATA(var_data);

				// LeakCheck Log -----
				{
					CString str_rate;
					CString str_press;
					sCH__LBA_LEAK_CHECK__STEP_LEAK_RATE[i]->Get__DATA(str_rate);
					sCH__LBA_LEAK_CHECK__STEP_PRESSURE_TORR[i]->Get__DATA(str_press);

					CString log_msg;
					log_msg.Format("%d,%d min,%.3f,%.3f", 
									i+1, 
									i+1,
									atof(str_press), 
									atof(str_rate));

					LeakCheck__LOG_WriteMsg__LLx_Left(log_msg);
				}
			}
		}

		// ...
		{
			aEXT_CH__LBA__PRESSURE_TORR->Get__DATA(var_data);
			sCH__LBA_LEAK_CHECK__END_PRESSURE_TORR->Set__DATA(var_data);

			double pre_press;
			double cur_press;

			sCH__LBA_LEAK_CHECK__START_PRESSURE_TORR->Get__DATA(var_data);
			pre_press = atof(var_data);

			aEXT_CH__LBA__PRESSURE_TORR->Get__DATA(var_data);
			cur_press = atof(var_data);

			if(check_min < 1)		check_min = 1;
			var_data.Format("%.3f", (cur_press-pre_press)/check_min);
			sCH__LBA_LEAK_CHECK__TOTAL_LEAK_RATE->Set__DATA(var_data);
		}

		Macro__Get_Date_Time(date_time);
		sCH__LBA_LEAK_CHECK__END_TIME->Set__DATA(date_time);
	}

	// LeackCheck Log -----
	{
		CString log_msg;
		CString log_bff;

		log_msg += "#COMMENT";
		log_msg += "\n";

		log_msg += "*** LEFT-LLx's LEAK CHECK INFORMATION ***";
		log_msg += "\n";

		log_bff.Format("(1) Over Pumping Time: %.1f sec                (2) Stable Time: %.1f sec             (3) Leak Check Time: %.0f min", 
						atof(str__over_pump_time),
						atof(str__stable_time),
						atof(str__check_time));
		log_msg += log_bff;
		log_msg += "\n";

		// ...
		CString str__start_time;
		CString str__end_time;

		sCH__LBA_LEAK_CHECK__START_TIME->Get__DATA(str__start_time);
		sCH__LBA_LEAK_CHECK__END_TIME->Get__DATA(str__end_time);

		log_bff.Format("(4) Leak Check Start Time: %s        (5) Leak Check End Time: %s", 
						str__start_time,
						str__end_time);
		log_msg += log_bff;
		log_msg += "\n";

		// ...
		sCH__LBA_LEAK_CHECK__START_PRESSURE_TORR->Get__DATA(var_data);

		log_bff.Format("(6) Start Pressure: %.3f %s", 
						atof(var_data),
						str__unit);
		log_msg += log_bff;
		log_msg += "\n";

		// ...
		sCH__LBA_LEAK_CHECK__TOTAL_LEAK_RATE->Get__DATA(var_data);

		log_bff.Format("(7) Leak Rate: %.3f %s", 
						atof(var_data),
						str__unit);
		log_msg += log_bff;
		log_msg += "\n";

		LeakCheck__LOG_WriteMsg__LLx_Left(log_msg);
	}

	// ...
	{
		sCH__LBA_LEAK_CHECK__TOTAL_LEAK_RATE->Get__DATA(var_data);
		double total_leak_rate = atof(var_data) * 1000.0;

		CFG_aCH__LBA_LEAK_CHECK__ALARM_RATE->Get__DATA(var_data);
		double cfg_leak_alarm = atof(var_data);

		if(total_leak_rate >= cfg_leak_alarm)
		{
			int alarm_id = ALID__RIGHT_LLx__LEAK_CHECK_BAD_RESULT_ALARM;
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

int  CObj__LLx_TwoManiFold
::Call__RIGHT_PUMP(CII_OBJECT__VARIABLE* p_variable,
				  CII_OBJECT__ALARM* p_alarm)
{
LOOP_RETRY:

	// RIGHT ...
	pPRESS_VLV__OBJ_CTRL->Call__OBJECT(CMMD__LBB_PUMP_VLV_OPEN);

	if(pLBB_PRESS__OBJ_CTRL->Call__OBJECT(CMMD__PUMP) < 0)
	{
		return -101;
	}

	if(p_variable->Check__CTRL_ABORT() > 0)
	{
		return -102;
	}

	// ...
	CString obj_sts;

	// PRESSURE CHECK -----
	int retry_flag = -1;

	if(Check__SYSTEM_VAC_PRESSURE(p_variable,p_alarm, 1, -1, retry_flag) < 0)
	{
		return -301;
	}

	if(retry_flag > 0)
	{
		goto LOOP_RETRY;
	}

	// PUMP VLV OPEN -----
	// LEFT ...
	{
		pLBA_PRESS__OBJ_CTRL->Get__OBJECT_STATUS(obj_sts);

		if(Check__OBJECT_BUSY(obj_sts) < 0)
		{
			if(sEXT_CH__LBA__PRESSURE_CTRL_FLAG->Check__DATA("PUMP") > 0)
			{
				if(dEXT_CH__LBA__PRESSURE_STATUS->Check__DATA(STR__VAC) > 0)
				{
					pPRESS_VLV__OBJ_CTRL->Call__OBJECT(CMMD__LBA_VAC_VLV_OPEN);
				}
			}
		}
	}

	return 1;
}
int  CObj__LLx_TwoManiFold
::Call__RIGHT_LEAK_CHECK(CII_OBJECT__VARIABLE* p_variable,
						 CII_OBJECT__ALARM* p_alarm)
{
	// ...
	{
		int i;

		sCH__LBB_LEAK_CHECK__START_TIME->Set__DATA("");
		sCH__LBB_LEAK_CHECK__END_TIME->Set__DATA("");
		sCH__LBB_LEAK_CHECK__START_PRESSURE_TORR->Set__DATA("");
		sCH__LBB_LEAK_CHECK__END_PRESSURE_TORR->Set__DATA("");
		sCH__LBB_LEAK_CHECK__TOTAL_LEAK_RATE->Set__DATA("");

		for(i=0;i<CFG_LEAK_RATE__STEP_SIZE;i++)
		{
			sCH__LBB_LEAK_CHECK__STEP_PRESSURE_TORR[i]->Set__DATA("");
			sCH__LBB_LEAK_CHECK__STEP_LEAK_RATE[i]->Set__DATA("");
		}
	}

	xCH__OBJ_MSG->Set__DATA("Right-Pumping ...");

	int flag = Call__RIGHT_PUMP(p_variable,p_alarm);
	if(flag < 0)		return -1;

	return Fnc__RIGHT_LEAK_CHECK(p_variable,p_alarm);
}

int  CObj__LLx_TwoManiFold
::Fnc__RIGHT_LEAK_CHECK(CII_OBJECT__VARIABLE* p_variable,
						CII_OBJECT__ALARM* p_alarm)
{
	CString log_msg;

	// ...
	SCX__ASYNC_TIMER_CTRL x_timer_ctrl;
	double  delay_time;
	CString var_data;

	x_timer_ctrl->REGISTER__COUNT_CHANNEL(sObject_Name, aCH__LBB_LEAK_CHECK__TIME_COUNT->Get__VARIABLE_NAME());

	// ...
	CString str__unit = "mTorr";
	CString str__over_pump_time;
	CString str__stable_time;
	CString str__check_time;

	// Over-Pumping -----
	{
		log_msg = "Right-LLx's Leak_Check : Over-Pumpingng ...";

		xCH__OBJ_MSG->Set__DATA(log_msg);
		xLOG_CTRL->WRITE__LOG(log_msg);
	}

	// ...
	{
		aCH__CFG_LBB_LEAK_CHECK__OVER_PUMPING_TIME->Get__DATA(var_data);
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

		pPRESS_VLV__OBJ_CTRL->Call__OBJECT(CMMD__LBB_VAC_VLV_CLOSE);
	}

	// Stable Time -----
	{
		log_msg = "Right-LLx's Leak_Check : Stable Time ...";

		xCH__OBJ_MSG->Set__DATA(log_msg);
		xLOG_CTRL->WRITE__LOG(log_msg);
	}

	//.....
	{
		aCH__CFG_LBB_LEAK_CHECK__STABLE_TIME->Get__DATA(var_data);
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
		log_msg = "Right-LLx's Leak_Checking ...";

		xCH__OBJ_MSG->Set__DATA(log_msg);
		xLOG_CTRL->WRITE__LOG(log_msg);
	}

	// ...
	{
		CString date_time;
		Macro__Get_Date_Time(date_time);

		sCH__LBB_LEAK_CHECK__START_TIME->Set__DATA(date_time);

		aEXT_CH__LBB__PRESSURE_TORR->Get__DATA(var_data);
		sCH__LBB_LEAK_CHECK__START_PRESSURE_TORR->Set__DATA(var_data);

		// LeakCheck Log -----
		{
			sCH__LBB_LEAK_CHECK__START_PRESSURE_TORR->Get__DATA(var_data);
			double start_press = atof(var_data);

			CString log_msg;
			log_msg.Format("0,Start Pressure,%.3f,%s", start_press,str__unit);

			LeakCheck__LOG_MakeFile__LLx_Right();
			LeakCheck__LOG_WriteMsg__LLx_Right(log_msg);
		}

		// ...
		aCH__CFG_LBB_LEAK_CHECK__CHECK_TIME_MIN->Get__DATA(var_data);
		str__check_time = var_data;
		int check_min = atoi(var_data);

		for(int i=0; i<check_min; i++)
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
					sCH__LBB_LEAK_CHECK__START_PRESSURE_TORR->Get__DATA(var_data);
					pre_press = atof(var_data);
				}
				else
				{
					sCH__LBB_LEAK_CHECK__STEP_PRESSURE_TORR[i-1]->Get__DATA(var_data);
					pre_press = atof(var_data);
				}

				aEXT_CH__LBB__PRESSURE_TORR->Get__DATA(var_data);
				sCH__LBB_LEAK_CHECK__STEP_PRESSURE_TORR[i]->Set__DATA(var_data);
				cur_press = atof(var_data);

				var_data.Format("%.3f", (cur_press-pre_press)/(i+1.0));
				sCH__LBB_LEAK_CHECK__STEP_LEAK_RATE[i]->Set__DATA(var_data);

				// LeakCheck Log -----
				{
					CString str_rate;
					CString str_press;
					sCH__LBB_LEAK_CHECK__STEP_LEAK_RATE[i]->Get__DATA(str_rate);
					sCH__LBB_LEAK_CHECK__STEP_PRESSURE_TORR[i]->Get__DATA(str_press);

					CString log_msg;
					log_msg.Format("%d,%d min,%.3f,%.3f", 
									i+1, 
									i+1,
									atof(str_press), 
									atof(str_rate));

					LeakCheck__LOG_WriteMsg__LLx_Right(log_msg);
				}
			}
		}

		// ...
		{
			aEXT_CH__LBB__PRESSURE_TORR->Get__DATA(var_data);
			sCH__LBB_LEAK_CHECK__END_PRESSURE_TORR->Set__DATA(var_data);

			double pre_press;
			double cur_press;

			sCH__LBB_LEAK_CHECK__START_PRESSURE_TORR->Get__DATA(var_data);
			pre_press = atof(var_data);

			aEXT_CH__LBB__PRESSURE_TORR->Get__DATA(var_data);
			cur_press = atof(var_data);

			if(check_min < 1)		check_min = 1;
			var_data.Format("%.3f", (cur_press-pre_press)/check_min);
			sCH__LBB_LEAK_CHECK__TOTAL_LEAK_RATE->Set__DATA(var_data);
		}

		Macro__Get_Date_Time(date_time);
		sCH__LBB_LEAK_CHECK__END_TIME->Set__DATA(date_time);
	}

	// LeackCheck Log -----
	{
		CString log_msg;
		CString log_bff;

		log_msg += "#COMMENT";
		log_msg += "\n";

		log_msg += "*** RIGHT-LLx's LEAK CHECK INFORMATION ***";
		log_msg += "\n";

		log_bff.Format("(1) Over Pumping Time: %.1f sec                (2) Stable Time: %.1f sec             (3) Leak Check Time: %.0f min", 
						atof(str__over_pump_time),
						atof(str__stable_time),
						atof(str__check_time));
		log_msg += log_bff;
		log_msg += "\n";

		// ...
		CString str__start_time;
		CString str__end_time;

		sCH__LBB_LEAK_CHECK__START_TIME->Get__DATA(str__start_time);
		sCH__LBB_LEAK_CHECK__END_TIME->Get__DATA(str__end_time);

		log_bff.Format("(4) Leak Check Start Time: %s        (5) Leak Check End Time: %s", 
						str__start_time,
						str__end_time);
		log_msg += log_bff;
		log_msg += "\n";

		// ...
		sCH__LBB_LEAK_CHECK__START_PRESSURE_TORR->Get__DATA(var_data);

		log_bff.Format("(6) Start Pressure: %.3f %s", atof(var_data),str__unit);
		log_msg += log_bff;
		log_msg += "\n";

		// ...
		sCH__LBB_LEAK_CHECK__TOTAL_LEAK_RATE->Get__DATA(var_data);

		log_bff.Format("(7) Leak Rate: %.3f %s", atof(var_data),str__unit);
		log_msg += log_bff;
		log_msg += "\n";

		LeakCheck__LOG_WriteMsg__LLx_Right(log_msg);
	}

	// ...
	{
		sCH__LBB_LEAK_CHECK__TOTAL_LEAK_RATE->Get__DATA(var_data);
		double total_leak_rate = atof(var_data) * 1000.0;

		CFG_aCH__LBB_LEAK_CHECK__ALARM_RATE->Get__DATA(var_data);
		double cfg_leak_alarm = atof(var_data);

		if(total_leak_rate >= cfg_leak_alarm)
		{
			int alarm_id = ALID__LEFT_LLx__LEAK_CHECK_BAD_RESULT_ALARM;
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

