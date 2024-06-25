#include "StdAfx.h"
#include "CObj__LBx_CHM_STD.h"
#include "CObj__LBx_CHM_STD__DEF.h"
#include "CObj__LBx_CHM_STD__ALID.h"


//------------------------------------------------------------------------------------------------
// INIT ----- 
int  CObj__LBx_CHM_STD
::Call__INIT(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	return Fnc__INIT(p_variable,p_alarm);
}

int  CObj__LBx_CHM_STD
::Fnc__INIT(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm)
{
	if(dCH__CFG_PUMP_DOWN_OPTION->Check__DATA("TRUE") > 0)
	{
		if(Call__PUMP(p_variable, p_alarm) < 0)		return OBJ_ABORT;
	}
	else
	{
		if(Call__VENT(p_variable, p_alarm) < 0)		return OBJ_ABORT;
	}

	return 1;
}

// ALL_GAS_VLV_CLOSE ----- 
int  CObj__LBx_CHM_STD
::Call__ALL_GAS_VLV_CLOSE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	Fnc__VENT_ALL_VLV__CLOSE(p_alarm);
	Fnc__PUMP_ALL_VLV__CLOSE(p_alarm);
	return 1;
}

// CL.PUMP_VLV -----
int  CObj__LBx_CHM_STD
::Call__CL_PUMP_VLV(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	Fnc__PUMP_ALL_VLV__CLOSE(p_alarm);
	return 1;
}

// OP.PUMP_VLV -----
int  CObj__LBx_CHM_STD
::Call__OP_PUMP_VLV(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	Fnc__VENT_ALL_VLV__CLOSE(p_alarm);
	Fnc__PUMP_FAST_VLV__OPEN(p_alarm);
	return 1;
}

// PUMP ...
int  CObj__LBx_CHM_STD
::Call__PUMP(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	// ...
	{
		CString act_name;
		act_name.Format("pump %s",m_sLBx__MODULE_NAME);

		if(Check__TRANSFER_VLV__CLOSE(p_alarm,act_name) < 0)
		{
			return -1;
		}
	}

	return Fnc__PUMP(p_variable,p_alarm);
}

int  CObj__LBx_CHM_STD
::Fnc__PUMP(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
LOOP_RETRY:

	// ...
	double trg_timeout = 9999.0;
	CString var_data;
	CString sLog;

	double  cur__press;
	double  cfg__press;

	double  cfg__vac_press;
	double  cfg__fast_press;

	double  cfg__soft_pump_stable;

	SCX__TIMER_CTRL xTimer;
	xTimer->REGISTER__ABORT_OBJECT(sObject_Name);

	// ...
	Fnc__LOG("Vent Valve All Close !!");
	if(Fnc__VENT_ALL_VLV__CLOSE(p_alarm) < 0)
	{
		return -1;
	}

	Fnc__LOG("Pump Valve All Close !!");
	if(Fnc__PUMP_ALL_VLV__CLOSE(p_alarm) < 0)
	{
		return -2;
	}

	Fnc__LOG("Dry Pump ON !!");
	if(pPUMP__OBJ_CTRL->Call__OBJECT("PUMP_ON") < 0)
	{
		return -3;
	}

	// ...
	aiEXT_CH__LBx__PRESSURE_TORR->Get__DATA(var_data);
	cur__press = atof(var_data);

	aCH__CFG_VAC_PRESSURE_TORR->Get__DATA(var_data);
	cfg__press = atof(var_data);
	cfg__vac_press = cfg__press;

	aCH__CFG_FAST_PUMP_PRESSURE_TORR->Get__DATA(var_data);
	cfg__fast_press = atof(var_data);

	sLog.Format("1. Get.. Pressure : current [%f] / vac config [%f] !!", cur__press, cfg__press);
	Fnc__LOG(sLog);

	// 판단 and 분기 ...
	if((cur__press <= cfg__press)
	&& (dCH__PRESSURE_STATUS->Check__DATA(STR__IO_VAC) > 0))
	{
		Fnc__PUMP_FAST_VLV__OPEN(p_alarm);

		sLog.Format("Already Pump Status.. Current Prs[%f]/[%f], VAC_Sts[%s]", cur__press, cfg__press, "VAC");
		Fnc__LOG(sLog);
		return 1;
	}	

	if((cur__press > cfg__vac_press)
	&& (cur__press < cfg__fast_press))
	{
		xI_ASYNC_TIMER->START__COUNT_UP(trg_timeout);	// Start...

		Fnc__LOG("Start Fast Pumping...");
		goto START_FAST_PUMP;
	}

	// Soft Pumping -----
	Fnc__LOG("Soft Pump Valve Open !!");
	Fnc__PUMP_SOFT_VLV__OPEN(p_alarm);

	xI_ASYNC_TIMER->START__COUNT_UP(trg_timeout);	// Start...

	// Time Delay
	{
		SCX__TIMER_CTRL x_timer_ctrl;
		double delay_time;
		
		aCH__CFG_SOFT_PUMP_DELAY_TIME->Get__DATA(var_data);
		delay_time = atof(var_data);

		if(x_timer_ctrl->WAIT(delay_time) < 0)
		{
			return -1;
		}
	}

	// ...
	aiEXT_CH__LBx__PRESSURE_TORR->Get__DATA(var_data);
	cur__press = atof(var_data);

	aCH__CFG_FAST_PUMP_PRESSURE_TORR->Get__DATA(var_data);
	cfg__press = atof(var_data);

	sLog.Format("2. Get.. Pressure : current [%f] / fast config [%f] !!", cur__press, cfg__press);
	Fnc__LOG(sLog);

	if(cur__press > cfg__press)
	{	
		double cfg_timeout;

		if(iSim_Flag > 0)
		{
			CString str_sim_data;
			SCX__TIMER_CTRL xSim_timer;

			xSim_timer->REGISTER__ABORT_OBJECT(sObject_Name);
			aEXT_CH__CFG_SIM_LLx_SLOW_PUMP_TIME->Get__DATA(str_sim_data);

			if(xSim_timer->WAIT(atof(str_sim_data)) < 0)
			{
				return -1;
			}

			aiEXT_CH__LBx__PRESSURE_TORR->Set__VALUE(cfg__press-0.01);
		}

		while(1)
		{	
			Sleep(90);

			// ...
			aiEXT_CH__LBx__PRESSURE_TORR->Get__DATA(var_data);
			cur__press = atof(var_data);

			aCH__CFG_FAST_PUMP_PRESSURE_TORR->Get__DATA(var_data);
			cfg__press = atof(var_data);

			if((cur__press < cfg__press)
			&& (dCH__PRESSURE_STATUS->Check__DATA(STR__IO_VAC) < 0))		// Vacuum 상태가 아니면...
			{
				sLog.Format("Complete... current [%f] < fast config [%f] and not VAC Sts!!", cur__press, cfg__press);
				Fnc__LOG(sLog);
				break;
			}

			if(p_variable->Check__CTRL_ABORT() > 0)
			{
				return -11;
			}

			aCH__CFG_SOFT_PUMP_TIMEOUT->Get__DATA(var_data);
			cfg_timeout = atof(var_data);

			if(xI_ASYNC_TIMER->Get__CURRENT_TIME() >= cfg_timeout)
			{
				int alarm_id = ALID__SOFT_PUMPING__TIMEOUT;
				CString alarm_msg;
				CString r_act;
				
				alarm_msg.Format("The current pressure is (%.3f), The fast pump config pressure is %.3f.\n",
								 cur__press, cfg__press);
				Fnc__LOG(alarm_msg);

				p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,alarm_msg,r_act);

				if(r_act.CompareNoCase(ACT__RETRY) == 0)
				{
					goto LOOP_RETRY;
				}
				return -12;
			}
		}
	}

	// ...
	aCH__CFG_SOFT_PUMP_COMPLETE_STABLE_TIME->Get__DATA(var_data);
	cfg__soft_pump_stable = atof(var_data);

	var_data.Format("Soft Pump Complete and Stable time:%.f sec !!", cfg__soft_pump_stable);
	Fnc__LOG(var_data);

	Fnc__MSG("Soft Pump stable time Starting...");
	if(xTimer->WAIT(cfg__soft_pump_stable) < 0)
	{
		return OBJ_ABORT;
	}


START_FAST_PUMP:

	// ...
	Fnc__LOG("Soft Pump Valve Close !!");
	Fnc__PUMP_SOFT_VLV__CLOSE(p_alarm);

	// Fast Pumping -----
	Fnc__PUMP_FAST_VLV__OPEN(p_alarm);

	xI_ASYNC_TIMER->START__COUNT_UP(trg_timeout);	// Start...

	// ...
	aiEXT_CH__LBx__PRESSURE_TORR->Get__DATA(var_data);
	cur__press = atof(var_data);

	aCH__CFG_VAC_PRESSURE_TORR->Get__DATA(var_data);
	cfg__press = atof(var_data);

	sLog.Format("3. Get.. Pressure : current [%f] / vac config [%f] !!", cur__press, cfg__press);
	Fnc__LOG(sLog);

	if((cur__press > cfg__press)
	|| (dCH__PRESSURE_STATUS->Check__DATA(STR__IO_VAC) < 0))
	{
		double trg_timeout = 9999.0;	
		double cfg_timeout;

		if(iSim_Flag > 0)
		{
			CString str_sim_data;
			SCX__TIMER_CTRL xSim_timer;
			
			xSim_timer->REGISTER__ABORT_OBJECT(sObject_Name);
			aEXT_CH__CFG_SIM_LLx_FAST_PUMP_TIME->Get__DATA(str_sim_data);

			if(xSim_timer->WAIT(atof(str_sim_data)) < 0)
			{
				return -1;
			}

			aiEXT_CH__LBx__PRESSURE_TORR->Set__VALUE(cfg__press-0.01);			
		}

		while(1)
		{	
			Sleep(90);

			aiEXT_CH__LBx__PRESSURE_TORR->Get__DATA(var_data);
			cur__press = atof(var_data);

			aCH__CFG_VAC_PRESSURE_TORR->Get__DATA(var_data);
			cfg__press = atof(var_data);

			if((cur__press < cfg__press)
			&& (dCH__PRESSURE_STATUS->Check__DATA(STR__IO_VAC) > 0))
			{
				sLog.Format("Complete... current [%f] < vac config [%f] and VAC Sts !!", cur__press,cfg__press);
				Fnc__LOG(sLog);
				break;
			}

			if(p_variable->Check__CTRL_ABORT() > 0)
			{
				return -21;
			}

			// ...
			aCH__CFG_FAST_PUMP_TIMEOUT->Get__DATA(var_data);
			cfg_timeout = atof(var_data);
			
			if(xI_ASYNC_TIMER->Get__CURRENT_TIME() >= cfg_timeout)
			{
				int alarm_id = ALID__FAST_PUMPING__TIMEOUT;
				CString alarm_msg;
				CString str_sns;
				CString r_act;
				
				alarm_msg.Format("The current pressure is (%.3f), The vac config pressure is %.3f.\n",
								 cur__press, cfg__press);
				Fnc__LOG(alarm_msg);

				dCH__PRESSURE_STATUS->Get__DATA(var_data);
				str_sns.Format("VAC Sensor is %s.\n", var_data);
				alarm_msg = alarm_msg+ str_sns;

				p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,alarm_msg,r_act);

				if(r_act.CompareNoCase(ACT__RETRY) == 0)
				{
					goto LOOP_RETRY;
				}
				return -22;
			}
		}
	}

	Update__PUMP_VLV_STS(p_alarm);
	return 1;
}
	
// VENT ...
int  CObj__LBx_CHM_STD
::Call__VENT(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	CString str_cfg_val;
	CString str_cur_val;
	CString str_alarm_msg;
	SCX__TIMER_CTRL xTimer;
	
	double  cur__press;
	double  cfg__press;

	xTimer->REGISTER__ABORT_OBJECT(sObject_Name);

	// ...
	{
		CString act_name;
		act_name.Format("vent %s",m_sLBx__MODULE_NAME);

		if(Check__TRANSFER_VLV__CLOSE(p_alarm,act_name) < 0)
		{
			return -1;
		}
	}

	// ...
	int flag = Fnc__VENT(p_variable,p_alarm);

	if(flag > 0)
	{
		// 압력 평형 상태 유지 시간 ...
		{
			aCH__CFG_OVER_VENT_TIME->Get__DATA(str_cfg_val);

			if(xTimer->WAIT(atof(str_cfg_val)) < 0)
			{
				return OBJ_ABORT;
			}

			Fnc__VENT_ALL_VLV__CLOSE_WITHOUT_EQUAL_VLV(p_alarm);
			doEXT_CH__ATM_EQUAL_VLV__SET->Set__DATA(STR__CLOSE);
		}
			
		// Venting 후 압력 Limit 체크 ...
		{
			aCFG_CH_CONVECT_ATM_HIGH_PRESS->Get__DATA(str_cfg_val); 
			cfg__press = atof(str_cfg_val)*0.001;	// mtorr -> torr

			if(cfg__press > 700)
			{
				aiEXT_CH__LBx__PRESSURE_TORR->Get__DATA(str_cur_val);	
				cur__press = atof(str_cur_val);
				
				if(cur__press > cfg__press)
				{
					int alarm_id = ALID__CONVECTRON_ATM_HIGH_PRESS_ALARM;

					str_alarm_msg.Format("%s Module.. After Venting Complete, Current Press %.3f (torr) > Config %.3f (torr) Status.\n", 
										  m_sLBx__MODULE_NAME, cur__press, cfg__press);

					p_alarm->Post__ALARM_With_MESSAGE(alarm_id, str_alarm_msg);
					return OBJ_ABORT;
				}
			}
		}

		// Equalize-Valve <<- "Open" ...
		{
			doEXT_CH__ATM_EQUAL_VLV__SET->Set__DATA(STR__OPEN);
		}
	}
	else
	{
		Fnc__VENT_ALL_VLV__CLOSE_WITHOUT_EQUAL_VLV(p_alarm);
		doEXT_CH__ATM_EQUAL_VLV__SET->Set__DATA(STR__CLOSE);
	}
	
	return flag;
}

int  CObj__LBx_CHM_STD
::Fnc__VENT(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
LOOP_RETRY:

	// ...
	double trg_timeout = 9999.0;
	CString var_data;
	CString sLog;

	double  cur__press;
	double  cfg__press;

	aiEXT_CH__LBx__PRESSURE_TORR->Get__DATA(var_data);
	cur__press = atof(var_data);

	aCH__CFG_ATM_PRESSURE_TORR->Get__DATA(var_data);
	cfg__press = atof(var_data);

	sLog.Format("1. Get.. Pressure : current [%f] / vac config [%f] !!", cur__press, cfg__press);
	Fnc__LOG(sLog);

	if((cur__press > cfg__press)
	&& (dCH__PRESSURE_STATUS->Check__DATA(STR__IO_ATM) > 0))
	{
		Fnc__VENT_ALL_VLV__CLOSE(p_alarm);

		sLog.Format("Already Vent Status.. Current Prs[%f]/[%f], ATM_Sts", cur__press, cfg__press);
		Fnc__LOG(sLog);
		return 1;
	}
	
	Fnc__LOG("Pump Valve All Close !!");
	if(Fnc__PUMP_ALL_VLV__CLOSE(p_alarm) < 0)
	{
		return -1;
	}

	Fnc__LOG("Vent Valve All Close !!");
	if(Fnc__VENT_ALL_VLV__CLOSE(p_alarm) < 0)
	{
		return -2;
	}

	// Fast Venting -----
	Fnc__LOG("Soft Vent Valve not Exist!!");
	Fnc__LOG("Fast Vent Valve Open !!");
	Fnc__VENT_FAST_VLV__OPEN(p_alarm);

	xI_ASYNC_TIMER->START__COUNT_UP(trg_timeout);

	// Time Delay
	{
		SCX__TIMER_CTRL x_timer_ctrl;
		double delay_time;
		
		aCH__CFG_SOFT_VENT_DELAY_TIME->Get__DATA(var_data);
		delay_time = atof(var_data);

		if(x_timer_ctrl->WAIT(delay_time) < 0)		return -1;
	}

	// ...
	aiEXT_CH__LBx__PRESSURE_TORR->Get__DATA(var_data);
	cur__press = atof(var_data);

	aCH__CFG_ATM_PRESSURE_TORR->Get__DATA(var_data);
	cfg__press = atof(var_data);

	// Real 센서 없슴..
	if((cur__press < cfg__press)
	|| (dCH__PRESSURE_STATUS->Check__DATA(STR__IO_ATM) < 0))
	{
		double cfg_timeout;
	
		if(iSim_Flag > 0)
		{
			CString str_sim_data;
			SCX__TIMER_CTRL xSim_timer;
			xSim_timer->REGISTER__ABORT_OBJECT(sObject_Name);
			aEXT_CH__CFG_SIM_LLx_FAST_VENT_TIME->Get__DATA(str_sim_data);

			if(xSim_timer->WAIT(atof(str_sim_data)) < 0)
			{
				return -1;
			}

			aiEXT_CH__LBx__PRESSURE_TORR->Set__VALUE(cfg__press+0.01);
		}

		while(1)
		{	
			Sleep(90);

			aiEXT_CH__LBx__PRESSURE_TORR->Get__DATA(var_data);
			cur__press = atof(var_data);

			aCH__CFG_ATM_PRESSURE_TORR->Get__DATA(var_data);
			cfg__press = atof(var_data);

 			if((cur__press > cfg__press)
 			&& (dCH__PRESSURE_STATUS->Check__DATA(STR__IO_ATM) > 0))
			{
				sLog.Format("Completed.. Vent.. Cur Prs:[%.f], Cfg Prs:[%.f]", cur__press, cfg__press);
				Fnc__LOG(sLog);
				break;
			}

			if(p_variable->Check__CTRL_ABORT() > 0)
			{
				Fnc__VENT_ALL_VLV__CLOSE(p_alarm);
				return -21;
			}

			// ...
			aCH__CFG_FAST_VENT_TIMEOUT->Get__DATA(var_data);
			cfg_timeout = atof(var_data);
			
			if(xI_ASYNC_TIMER->Get__CURRENT_TIME() >= cfg_timeout)
			{
				Fnc__VENT_ALL_VLV__CLOSE(p_alarm);

				// ...
				{
					int alarm_id = ALID__FAST_VENTING__TIMEOUT;
					CString alarm_msg;
					CString str_sns;
					CString r_act;
					
					alarm_msg.Format("The current pressure is (%.3f), The atm config pressure is %.3f.\n",
									 cur__press, cfg__press);

					dCH__PRESSURE_STATUS->Get__DATA(var_data);
					str_sns.Format("ATM Sensor is %s.\n", var_data);
					alarm_msg = alarm_msg + str_sns;
				
					p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,alarm_msg,r_act);

					if(r_act.CompareNoCase(ACT__RETRY) == 0)		goto LOOP_RETRY;
					return -22;
				}
			}

			// ...
		}
	}

	return 1;
}
	

// DOOR VLV CLOSE -----
int  CObj__LBx_CHM_STD
::Call__DV_CLOSE(CII_OBJECT__VARIABLE* p_variable,
			     CII_OBJECT__ALARM* p_alarm)
{
	return Fnc__DV_CLOSE(p_variable,p_alarm);
}

int  CObj__LBx_CHM_STD
::Fnc__DV_CLOSE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	CString str_log;

LOOP_RETRY:

	// ...
	double  trg_timeout = 9999.0;
	double  cfg_timeout = 0.0;
	CString var_data;

	if((diEXT_CH__LBx__DV_OPEN->Check__DATA(STR__OFF) > 0)
	&& (diEXT_CH__LBx__DV_CLOSE->Check__DATA(STR__ON) > 0))
	{
		str_log.Format("Already... Close sts.. %s Door Valve.", m_sLBx__MODULE_NAME);
		Fnc__LOG(str_log);
		return 1;
	}

	if(Is_DV_CLOSE_CONDITION(p_variable, p_alarm) < 0)
	{
		return -1;
	}
	
	xI_ASYNC_TIMER->START__COUNT_UP(trg_timeout);

	doEXT_CH__LBx__DV_OPEN->Set__DATA(STR__OFF);
	Sleep(100);
	doEXT_CH__LBx__DV_CLOSE->Set__DATA(STR__ON);

	// Add Simulation !!
	if(iSim_Flag > 0)
	{
		SCX__TIMER_CTRL sim_timer;
		aEXT_CH__CFG_SIM_DOOR_VLV_CLOSE_TIME->Get__DATA(var_data);

		diEXT_CH__LBx__DV_OPEN->Set__DATA(STR__OFF);
		diEXT_CH__LBx__DV_CLOSE->Set__DATA(STR__OFF);

		if(sim_timer->WAIT(atof(var_data)) < 0)
		{
			return -1;
		}

		diEXT_CH__LBx__DV_OPEN->Set__DATA(STR__OFF);
		diEXT_CH__LBx__DV_CLOSE->Set__DATA(STR__ON);
	}

	while(1)
	{
		Sleep(3);

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			return -1;
		}

		if((diEXT_CH__LBx__DV_OPEN->Check__DATA(STR__OFF) > 0)
		&& (diEXT_CH__LBx__DV_CLOSE->Check__DATA(STR__ON) > 0))
		{
			Sleep(100);
			
			str_log.Format("%s DV Close Completed..", m_sLBx__MODULE_NAME);	Fnc__LOG(str_log);
			return 1;
		}

		aCH__CFG_DOOR_VALVE_CLOSE_TIMEOUT->Get__DATA(var_data);
		cfg_timeout = atof(var_data);

		if(xI_ASYNC_TIMER->Get__CURRENT_TIME() >= cfg_timeout)
		{
			break;
		}
	}

	// ...
	int alarm_id = ALID__DOOR_VALVE_CLOSE__TIMEOUT;

	CString err_msg;
	CString r_act;

	err_msg.Format("%s's Door Valve Close TimeOut (%.0f)", m_sLBx__MODULE_NAME,cfg_timeout);
	Fnc__LOG(err_msg);
	p_alarm->Popup__ALARM(alarm_id,r_act);

	if(r_act.CompareNoCase(ACT__RETRY) == 0)
	{
		goto LOOP_RETRY;
	}
	return -1;
}

// DOOR VLV OPEN -----
int  CObj__LBx_CHM_STD
::Call__DV_OPEN(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	if(Check__PRESSURE_ATM_TO_DV_OPEN(p_alarm,ALID__DV_OPEN__NOT_ATM_ERROR) < 0)
	{
		return -1;
	}

	return Fnc__DV_OPEN(p_variable,p_alarm);
}

int  CObj__LBx_CHM_STD
::Fnc__DV_OPEN(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	CString str_log;

LOOP_RETRY:

	// ...
	double  trg_timeout = 9999.0;
	double  cfg_timeout = 0.0;
	CString var_data;

	if((diEXT_CH__LBx__DV_OPEN->Check__DATA(STR__ON)   > 0)
	&& (diEXT_CH__LBx__DV_CLOSE->Check__DATA(STR__OFF) > 0))
	{
		str_log.Format("Already... Open sts.. %s Door Valve.", m_sLBx__MODULE_NAME);
		Fnc__LOG(str_log);
		return 1;
	}
	
	xI_ASYNC_TIMER->START__COUNT_UP(trg_timeout);

	doEXT_CH__LBx__DV_CLOSE->Set__DATA(STR__OFF);
	Sleep(100);
	doEXT_CH__LBx__DV_OPEN->Set__DATA(STR__ON);
	
	if(iSim_Flag > 0)
	{
		SCX__TIMER_CTRL sim_timer;
		aEXT_CH__CFG_SIM_DOOR_VLV_OPEN_TIME->Get__DATA(var_data);

		diEXT_CH__LBx__DV_OPEN->Set__DATA(STR__OFF);
		diEXT_CH__LBx__DV_CLOSE->Set__DATA(STR__OFF);

		if(sim_timer->WAIT(atof(var_data)) < 0)
		{
			return -1;
		}

		diEXT_CH__LBx__DV_OPEN->Set__DATA(STR__ON);
		diEXT_CH__LBx__DV_CLOSE->Set__DATA(STR__OFF);
	}

	while(1)
	{
		Sleep(3);

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			return -1;
		}

		if((diEXT_CH__LBx__DV_OPEN->Check__DATA(STR__ON)   > 0)
		&& (diEXT_CH__LBx__DV_CLOSE->Check__DATA(STR__OFF) > 0))
		{
			Sleep(100);
			str_log.Format("%s DV Open Completed..", m_sLBx__MODULE_NAME);	Fnc__LOG(str_log);
			return 1;
		}

		aCH__CFG_DOOR_VALVE_OPEN_TIMEOUT->Get__DATA(var_data);
		cfg_timeout = atof(var_data);

		if(xI_ASYNC_TIMER->Get__CURRENT_TIME() >= cfg_timeout)
		{
			break;
		}
	}

	// ...
	int alarm_id = ALID__DOOR_VALVE_OPEN__TIMEOUT;

	CString err_msg;
	CString r_act;

	err_msg.Format("%s's Door Valve Open TimeOut (%.0f)", m_sLBx__MODULE_NAME,cfg_timeout);
	Fnc__LOG(err_msg);
	p_alarm->Popup__ALARM(alarm_id,r_act);

	if(r_act.CompareNoCase(ACT__RETRY) == 0)
	{
		goto LOOP_RETRY;
	}
	return -1;
}

// SLIT VLV CLOSE -----
int  CObj__LBx_CHM_STD
::Call__SV_CLOSE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	CString str_log;

	return Fnc__SV_CLOSE(p_variable,p_alarm);
}

int  CObj__LBx_CHM_STD
::Fnc__SV_CLOSE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
LOOP_RETRY:

	// ...
	double  trg_timeout = 9999.0;
	double  cfg_timeout = 0.0;
	CString var_data;
	int     nRet;

	if(Is__SV_FULL_CLOSE(p_variable, p_alarm) > 0)
	{
		return 1;
	}

	// ...
	aCH__CFG_SLIT_VALVE_CLOSE_TIMEOUT->Get__DATA(var_data);
	cfg_timeout = atof(var_data);

	// 1. VAC Robot Arm Retract Check...
	{
		if(Is_SV_CLOSE_CONDITION(p_variable, p_alarm) < 0)
		{
			return -1;
		}

		nRet = Sub__SV_TM_OPEN(p_variable, p_alarm, cfg_timeout);
		if(nRet > 0)
		{
			Sleep(300);			// HW 동작 time
			nRet = Sub__SV_UP(p_variable, p_alarm, cfg_timeout);
		}
		if(nRet > 0)
		{
			Sleep(300);			// HW 동작 time
			nRet = Sub__SV_TM_CLOSE(p_variable, p_alarm, cfg_timeout);
		}

		     if(nRet > 0)		return 1;			// FULL CLOSE Complete !!
		else if(nRet < 0)		return -1;			// Abort
	}												// == 0 : timeout !!
	
	// ...
	{
		int alarm_id = ALID__SLIT_VALVE_CLOSE__TIMEOUT;

		CString err_msg;
		CString r_act;

		err_msg.Format("%s's Slit Valve Close TimeOut (%.0f)", m_sLBx__MODULE_NAME,cfg_timeout);
		Fnc__LOG(err_msg);
	
		p_alarm->Popup__ALARM(alarm_id,r_act);

		if(r_act.CompareNoCase(ACT__RETRY) == 0)
		{
			goto LOOP_RETRY;
		}
	}
	return -1;
}

int  CObj__LBx_CHM_STD
::Is__SV_FULL_CLOSE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	CString str_log;

	// 1. Already UP Sns Check..
	if((diEXT_CH__LBx__SV_UP->Check__DATA(STR__ON) > 0)
	&& (diEXT_CH__LBx__SV_DOWN->Check__DATA(STR__OFF) > 0))
	{
		str_log.Format("Already... SV UP sts.. %s Slot Valve. and..", m_sLBx__MODULE_NAME);
		Fnc__LOG(str_log);

		// 2. Already TM_CLOSE Sns Check..
		if((diEXT_CH__LBx__SV_TM_OPEN->Check__DATA(STR__OFF) > 0)
		&& (diEXT_CH__LBx__SV_TM_CLOSE->Check__DATA(STR__ON) > 0))
		{
			str_log.Format("OK..Already... SV [UP and TM_CLOSE] sts.. %s Slot Valve.", m_sLBx__MODULE_NAME);
			Fnc__LOG(str_log);
			return 1;
		}
	}

	return -1;
}

// SLIT VLV OPEN -----
int  CObj__LBx_CHM_STD
::Call__SV_OPEN(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	if(dEXT_CH__CFG_TRANSFER_MODE->Check__DATA(STR__ATM) > 0)
	{
		if(Check__PRESSURE_ATM(p_alarm,ALID__SV_OPEN__NOT_ATM_ERROR) < 0)
		{
			return -1;
		}
	}
	else
	{
		if(Check__PRESSURE_VAC(p_alarm) < 0)
		{
			return -1;
		}

		if(Check__SV_OPEN_MAX_DIFFERENTIAL_PRESS(p_variable, p_alarm) < 0)
		{
			return OBJ_ABORT;
		}
	}

	return Fnc__SV_OPEN(p_variable,p_alarm);
}

int  CObj__LBx_CHM_STD
::Fnc__SV_OPEN(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	CString str_log;
	int alarm_id;

LOOP_RETRY:

	// ...
	double  trg_timeout = 9999.0;
	double  cfg_timeout = 0.0;
	CString var_data;
	int     nRet;

	if(m_sLBx__MODULE_NAME.CompareNoCase("LBA") == 0)
	{
		nRet = dEXT_CH__CFG_LL1_SV_EXIST_FLAG->Check__DATA("FALSE");
	}
	else
	{
		nRet = dEXT_CH__CFG_LL2_SV_EXIST_FLAG->Check__DATA("FALSE");
	}

	if(nRet > 0)
	{
		alarm_id = ALID__SLIT_CANNOT_OPEN_NOT_EXIST;

		CString err_msg;
		CString r_act;

		err_msg.Format("%s's Slit Valve Cannot Open", m_sLBx__MODULE_NAME);
		Fnc__LOG(err_msg);
		p_alarm->Popup__ALARM(alarm_id,r_act);

			 if(r_act.CompareNoCase(ACT__RETRY) == 0)		goto LOOP_RETRY;
		else if(r_act.CompareNoCase(ACT__ABORT) == 0)		return OBJ_ABORT;
	}
	
	if(Is__SV_FULL_OPEN(p_variable, p_alarm) > 0)
	{
		return 1;
	}

	// ...
	aCH__CFG_SLIT_VALVE_OPEN_TIMEOUT->Get__DATA(var_data);
	cfg_timeout = atof(var_data);

	nRet = Sub__SV_TM_OPEN(p_variable, p_alarm, cfg_timeout);
	if(nRet > 0)
	{
		Sleep(300);			// HW 동작 time
		nRet = Sub__SV_DOWN(p_variable, p_alarm, cfg_timeout);
	}
	
	if(nRet > 0)
	{
		Sleep(500);			// HW 동작 time
		return 1;			// FULL OPEN Complete !!
	}
	else if(nRet < 0)
	{
		return -1;	// Abort
					// == 0 : timeout !!
	}

	// ...
	{
		alarm_id = ALID__SLIT_VALVE_OPEN__TIMEOUT;

		CString err_msg;
		CString r_act;

		err_msg.Format("%s's Slit Valve Open TimeOut (%.0f)", m_sLBx__MODULE_NAME,cfg_timeout);
		Fnc__LOG(err_msg);
		p_alarm->Popup__ALARM(alarm_id,r_act);

		if(r_act.CompareNoCase(ACT__RETRY) == 0)
		{
			goto LOOP_RETRY;
		}
	}
	return -1;
}

int  CObj__LBx_CHM_STD
::Is__SV_FULL_OPEN(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	CString str_log;

	// 1. Already OPEN Sns Check..
	if((diEXT_CH__LBx__SV_TM_OPEN->Check__DATA(STR__ON) > 0)
	&& (diEXT_CH__LBx__SV_TM_CLOSE->Check__DATA(STR__OFF) > 0))
	{
		str_log.Format("Already... SV TM_OPEN sts.. %s Slot Valve. and..", m_sLBx__MODULE_NAME);
		Fnc__LOG(str_log);

		// 2. Already DOWN Sns Check..
		if((diEXT_CH__LBx__SV_UP->Check__DATA(STR__OFF) > 0)
		&& (diEXT_CH__LBx__SV_DOWN->Check__DATA(STR__ON) > 0))	
		{
			str_log.Format("OK..Already... SV [DOWN and TM_OPEN] sts.. %s Slot Valve.", m_sLBx__MODULE_NAME);
			Fnc__LOG(str_log);
			return 1;
		}
	}

	return -1;
}

// PREPMATER -----
int  CObj__LBx_CHM_STD
::Call__PREPMATER(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	return Fnc__PREPMATER(p_variable,p_alarm);	
}

int  CObj__LBx_CHM_STD
::Fnc__PREPMATER(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{

	return 1;
}

// COMPMATER -----
int  CObj__LBx_CHM_STD
::Call__COMPMATER(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	return Fnc__COMPMATER(p_variable,p_alarm);
}

int  CObj__LBx_CHM_STD
::Fnc__COMPMATER(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{

	return 1;
}


// IN_PROC -----
int  CObj__LBx_CHM_STD
::Call__IN_PROC(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	return Fnc__IN_PROC(p_variable,p_alarm);
}

int  CObj__LBx_CHM_STD
::Fnc__IN_PROC(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{

	return 1;
}

// OUT_PROC -----
int  CObj__LBx_CHM_STD
::Call__OUT_PROC(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	return Fnc__OUT_PROC(p_variable,p_alarm);
}

int  CObj__LBx_CHM_STD
::Fnc__OUT_PROC(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	return 1;
}


// AUTO CTRL ---------------
// PUMP VLV CLOSE -----
int  CObj__LBx_CHM_STD
::Call__AUTO_PUMP_VLV_CLOSE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	return Fnc__AUTO_PUMP_VLV_CLOSE(p_variable,p_alarm);
}

int  CObj__LBx_CHM_STD
::Fnc__AUTO_PUMP_VLV_CLOSE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	Fnc__PUMP_ALL_VLV__CLOSE(p_alarm);
	return 1;
}
