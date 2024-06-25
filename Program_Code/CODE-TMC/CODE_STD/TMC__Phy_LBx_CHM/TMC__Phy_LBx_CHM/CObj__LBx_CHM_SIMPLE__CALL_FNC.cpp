#include "StdAfx.h"
#include "CObj__LBx_CHM_SIMPLE.h"
#include "CObj__LBx_CHM_SIMPLE__DEF.h"
#include "CObj__LBx_CHM_SIMPLE__ALID.h"


//------------------------------------------------------------------------------------------------
// INIT ----- 
int  CObj__LBx_CHM_SIMPLE
::Call__INIT(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	return Fnc__INIT(p_variable,p_alarm);
}
int  CObj__LBx_CHM_SIMPLE
::Fnc__INIT(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm)
{
	if(dCH__CFG_PUMP_DOWN_OPTION->Check__DATA("TRUE") > 0)
	{
		if(Call__PUMP(p_variable, p_alarm) < 0)
		{
			return OBJ_ABORT;
		}
	}
	else
	{
		if(Call__VENT(p_variable, p_alarm) < 0)
		{
			return OBJ_ABORT;
		}
	}

	return 1;
}

// PUMP ...
int  CObj__LBx_CHM_SIMPLE
::Call__PUMP(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	CString act_name;
	act_name.Format("pump %s",m_sLBx__MODULE_NAME);

	if(Check__LLx_LID__CLOSE(p_alarm,act_name) < 0)
	{
		return -11;
	}
	if(Check__TRANSFER_VLV__CLOSE(p_alarm,act_name) < 0)
	{
		return -12;
	}

	// ...
	SCX__ASYNC_TIMER_CTRL x_timer_ctrl;
	x_timer_ctrl->START__COUNT_UP(9999);

	int flag = Fnc__PUMP(p_variable,p_alarm);

	if(flag > 0)
	{
		double cur_sec = x_timer_ctrl->Get__CURRENT_TIME();
		
		int db_i = _ACT_INDEX__PUMP;
		_Update__ACTION_MIN_MAX(db_i,cur_sec);
	}
	return flag;
}
int  CObj__LBx_CHM_SIMPLE
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
		return -11;
	}

	Fnc__LOG("Gas Valve All Close !!");
	if(_Fnc__OBJ_GAS__ALL_CLOSE() < 0)
	{
		return -12;
	}

	Fnc__LOG("Pump Valve All Close !!");
	if(Fnc__PUMP_ALL_VLV__CLOSE(p_alarm) < 0)
	{
		return -13;
	}

	Fnc__LOG("Dry Pump ON !!");
	if(pPUMP__OBJ_CTRL->Call__OBJECT("PUMP_ON") < 0)
	{
		return -14;
	}

	if(bActive__OBJ_VAT) // 220930_When Not Use APC Valve_don't have to show
	{
		Fnc__LOG("APC Valve Open !!");
		if(_Fnc__OBJ_VAT__OPEN() < 0)
		{
			return -15;
		}
	}
	// EXHAUST PRESSURE NOT USE
	/** 
	// CHECK : EXHAUST PRESSURE ...
	{
		SCX__ASYNC_TIMER_CTRL x_timer;
		x_timer->START__COUNT_UP(99999);

		while(1)
		{
			Sleep(10);

			cur__press = aiEXT_CH__LBx__EXHAUST_PRESSURE->Get__VALUE();
			double cfg__press = aCH__CFG_EXHAUST_PUMP_PRESSURE_TORR->Get__VALUE();
			if(cur__press <= cfg__press)		break;
			
			if(p_variable->Check__CTRL_ABORT() > 0)
			{
				return -301;
			}

			double cfg__timeout = aCH__CFG_EXHAUST_PUMP_TIMEOUT->Get__VALUE();
			if(x_timer->Get__CURRENT_TIME() > cfg__timeout)
			{
				// Alarm ...
				{
					int  alm_id = ALID__EXHAUST_PRESSURE_TIMEOUT;
					CString alm_msg;
					CString alm_bff;
					CString r_act;

					alm_bff.Format(" * %s <- %.3f torr \n",
									aiEXT_CH__LBx__EXHAUST_PRESSURE->Get__CHANNEL_NAME(),
									aiEXT_CH__LBx__EXHAUST_PRESSURE->Get__VALUE());
					alm_msg += alm_bff;
					alm_bff.Format(" * %s <- %.3f torr \n",
									aCH__CFG_EXHAUST_PUMP_PRESSURE_TORR->Get__CHANNEL_NAME(),
									aCH__CFG_EXHAUST_PUMP_PRESSURE_TORR->Get__VALUE());
					alm_msg += alm_bff;			
					alm_bff.Format(" * %s <- %.0f sec \n",
									aCH__CFG_EXHAUST_PUMP_TIMEOUT->Get__CHANNEL_NAME(),
									aCH__CFG_EXHAUST_PUMP_TIMEOUT->Get__VALUE());
					alm_msg += alm_bff;

					p_alarm->Check__ALARM(alm_id, r_act);
					p_alarm->Post__ALARM_With_MESSAGE(alm_id, alm_msg);
				}
				return -302;
			}
			
		}
		
	}
	**/

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

	if((cur__press <= cfg__press)
	&& (dCH__PRESSURE_STATUS->Check__DATA(STR__IO_VAC) > 0))
	{

		Fnc__PUMP_FAST_VLV__OPEN(p_alarm);

		Fnc__LOG("APC Valve Open !");
		_Fnc__OBJ_VAT__OPEN();

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

	if(iSim_Flag > 0)
	{
		diEXT_CH__LBx__ATM_SENSOR->Set__DATA(sDATA__ATM_OFF);
		diEXT_CH__LBx__VAC_SENSOR->Set__DATA(sDATA__VAC_OFF);
	}

	// Soft Pumping ...
	Fnc__LOG("Soft Pump Valve Open !!");

	if(dCH__CFG_USE_SOFT_PUMP_VALVE->Check__DATA(STR__NO) > 0)
	{
		Fnc__PUMP_FAST_VLV__OPEN(p_alarm);
	}
	else
	{
		Fnc__PUMP_SOFT_VLV__OPEN(p_alarm);
	}

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

	if(iSim_Flag > 0)
	{
		diEXT_CH__LBx__ATM_SENSOR->Set__DATA(sDATA__ATM_OFF);
		diEXT_CH__LBx__VAC_SENSOR->Set__DATA(sDATA__VAC_ON);
	}

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

				alarm_msg += "Sensor State ... \n";
				str_sns.Format("  * %s <- %s \n", 
							   diEXT_CH__LBx__ATM_SENSOR->Get__CHANNEL_NAME(),
							   diEXT_CH__LBx__ATM_SENSOR->Get__STRING());
				alarm_msg += str_sns;
				str_sns.Format("  * %s <- %s \n",
					           diEXT_CH__LBx__VAC_SENSOR->Get__CHANNEL_NAME(),
							   diEXT_CH__LBx__VAC_SENSOR->Get__STRING());
				alarm_msg += str_sns;

				dCH__PRESSURE_STATUS->Get__DATA(var_data);
				str_sns.Format("Pressure Status is %s.\n", var_data);
				alarm_msg += str_sns;

				Fnc__LOG(alarm_msg);

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
int  CObj__LBx_CHM_SIMPLE
::Call__VENT(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	SCX__TIMER_CTRL xTimer;

	double  cur__press;
	double  cfg__press;

	// ...
	{
		CString act_name;
		act_name.Format("vent %s",m_sLBx__MODULE_NAME);

		if(Check__LLx_LID__CLOSE(p_alarm,act_name) < 0)
		{
			return -11;
		}
		if(Check__TRANSFER_VLV__CLOSE(p_alarm,act_name) < 0)
		{
			return -12;
		}
	}

	// ...
	SCX__ASYNC_TIMER_CTRL x_timer_ctrl;
	x_timer_ctrl->START__COUNT_UP(9999);

	int flag = _Fnc__VENT(p_variable,p_alarm);
	if(flag > 0)
	{
		flag = _Sub__VENT(p_variable, p_alarm);
	}

	if(flag > 0)
	{
		double cur_sec = x_timer_ctrl->Get__CURRENT_TIME();

		int db_i = _ACT_INDEX__VENT;
		_Update__ACTION_MIN_MAX(db_i,cur_sec);
	}

	// ...
	{
		Fnc__VENT_ALL_VLV__CLOSE_WITHOUT_EQUAL_VLV(p_alarm);

		if(dCH__CFG_EQUAL_VLV_OPEN_WHEN_ATM->Check__DATA(STR__ENABLE) < 0)
			doEXT_CH__ATM_EQUAL_VLV__SET->Set__DATA(STR__CLOSE);
	}
	return flag;
}
int  CObj__LBx_CHM_SIMPLE
::_Sub__VENT(CII_OBJECT__VARIABLE* p_variable,
			 CII_OBJECT__ALARM* p_alarm)
{
	SCX__TIMER_CTRL x_timer;
	x_timer->REGISTER__ABORT_OBJECT(sObject_Name);

	// Equalize-Valve <<- "Open" ...
	{
		doEXT_CH__ATM_EQUAL_VLV__SET->Set__DATA(STR__OPEN);
	}

	// Over-Vent Time ...
	{
		double cfg_sec = aCH__CFG_OVER_VENT_TIME->Get__VALUE();

		if(x_timer->WAIT(cfg_sec) < 0)
		{
			return OBJ_ABORT;
		}

		Fnc__VENT_ALL_VLV__CLOSE_WITHOUT_EQUAL_VLV(p_alarm);
	}

	// Equalize-Vent Time ...
	{
		double cfg_sec = aCH__CFG_EQUALIZE_VENT_TIME->Get__VALUE();

		if(x_timer->WAIT(cfg_sec) < 0)
		{
			return OBJ_ABORT;
		}
	}

	// Venting 후 압력 Limit 체크 ...
	{
		double  cur__press;
		double  cfg__press;

		cfg__press = aCFG_CH_CONVECT_ATM_HIGH_PRESS->Get__VALUE();

		if(cfg__press > 700)
		{
			cur__press = aiEXT_CH__LBx__PRESSURE_TORR->Get__VALUE();

			if(cur__press > cfg__press)
			{
				int alm_id = ALID__CONVECTRON_ATM_HIGH_PRESS_ALARM;
				CString alm_msg;
				CString alm_bff;

				alm_msg = "After venting complete, \n";

				alm_bff.Format(" * current pressure <- %.1f (torr) \n", cur__press);
				alm_msg += alm_bff;

				alm_msg.Format(" * config pressure <- %.1f (torr) \n", cfg__press);
				alm_msg += alm_bff;

				p_alarm->Post__ALARM_With_MESSAGE(alm_id, alm_msg);
				return OBJ_ABORT;
			}
		}
	}

	return 1;
}
int  CObj__LBx_CHM_SIMPLE
::_Fnc__VENT(CII_OBJECT__VARIABLE* p_variable,
			 CII_OBJECT__ALARM* p_alarm)
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

	Fnc__LOG("APC Valve Close !!");
	_Fnc__OBJ_VAT__CLOSE();

	// Soft Venting ...
	{
		Fnc__LOG("Soft Vent Valve Open !!");
		Fnc__VENT_SOFT_VLV__OPEN(p_alarm);

		xI_ASYNC_TIMER->START__COUNT_UP(trg_timeout);

		if(iSim_Flag > 0)
		{
			diEXT_CH__LBx__ATM_SENSOR->Set__DATA(sDATA__ATM_OFF);
			diEXT_CH__LBx__VAC_SENSOR->Set__DATA(sDATA__VAC_OFF);
		}

		// ...
		aiEXT_CH__LBx__PRESSURE_TORR->Get__DATA(var_data);
		cur__press = atof(var_data);

		aCH__CFG_FAST_VENT_PRESSURE_TORR->Get__DATA(var_data);
		cfg__press = atof(var_data);

		if(cur__press < cfg__press)
		{
			double cfg_timeout;

			if(iSim_Flag > 0)
			{
				CString str_sim_data;
				SCX__TIMER_CTRL xSim_timer;
				xSim_timer->REGISTER__ABORT_OBJECT(sObject_Name);
				aEXT_CH__CFG_SIM_LLx_SLOW_VENT_TIME->Get__DATA(str_sim_data);

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

				aCH__CFG_FAST_VENT_PRESSURE_TORR->Get__DATA(var_data);
				cfg__press = atof(var_data);

				if(cur__press > cfg__press)
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
				aCH__CFG_SOFT_VENT_TIMEOUT->Get__DATA(var_data);
				cfg_timeout = atof(var_data);

				if(xI_ASYNC_TIMER->Get__CURRENT_TIME() >= cfg_timeout)
				{
					Fnc__VENT_ALL_VLV__CLOSE(p_alarm);

					// ...
					{
						int alm_id = ALID__SOFT_VENTING__TIMEOUT;
						CString alm_msg;
						CString alm_bff;
						CString r_act;

						alm_msg.Format("The current pressure is %.3f (torr). \n", cur__press);

						alm_bff.Format("The soft-vent pressure is %.3f (torr). \n", cfg__press);
						alm_msg += alm_bff;

						alm_bff.Format("The config soft-Vent timeout is %.1f (sec). \n", cfg_timeout);
						alm_msg += alm_bff;

						alm_msg += "Sensor State ... \n";
						alm_bff.Format("  * %s <- %s \n", 
									   diEXT_CH__LBx__ATM_SENSOR->Get__CHANNEL_NAME(),
									   diEXT_CH__LBx__ATM_SENSOR->Get__STRING());
						alm_msg += alm_bff;
						alm_bff.Format("  * %s <- %s \n",
										diEXT_CH__LBx__VAC_SENSOR->Get__CHANNEL_NAME(),
										diEXT_CH__LBx__VAC_SENSOR->Get__STRING());
						alm_msg += alm_bff;

						dCH__PRESSURE_STATUS->Get__DATA(var_data);
						alm_bff.Format("Pressure Status is %s.\n", var_data);
						alm_msg += alm_bff;

						Fnc__LOG(alm_msg);

						p_alarm->Popup__ALARM_With_MESSAGE(alm_id,alm_msg,r_act);

						if(r_act.CompareNoCase(ACT__RETRY) == 0)		goto LOOP_RETRY;
						return -22;
					}
				}

				// ...
			}
		}
	}


	// Fast Venting ...
	{
		// Time Delay
		{
			SCX__TIMER_CTRL x_timer_ctrl;
			double delay_time;

			aCH__CFG_SOFT_VENT_DELAY_TIME->Get__DATA(var_data);
			delay_time = atof(var_data);

			if(x_timer_ctrl->WAIT(delay_time) < 0)		return -1;
		}

		Fnc__LOG("Fast Vent Valve Open !!");
		Fnc__VENT_SOFT_VLV__CLOSE(p_alarm);
		Fnc__VENT_FAST_VLV__OPEN(p_alarm);
		
		// Equalize-Valve <<- "Open" ...
		{
			doEXT_CH__ATM_EQUAL_VLV__SET->Set__DATA(STR__OPEN);
		}

		xI_ASYNC_TIMER->START__COUNT_UP(trg_timeout);

		if(iSim_Flag > 0)
		{
			diEXT_CH__LBx__ATM_SENSOR->Set__DATA(sDATA__ATM_ON);
			diEXT_CH__LBx__VAC_SENSOR->Set__DATA(sDATA__VAC_OFF);
		}

		// ...
		aiEXT_CH__LBx__PRESSURE_TORR->Get__DATA(var_data);
		cur__press = atof(var_data);

		aCH__CFG_ATM_PRESSURE_TORR->Get__DATA(var_data);
		cfg__press = atof(var_data);

		if((cur__press < cfg__press)
		|| (dCH__PRESSURE_STATUS->Check__DATA(STR__IO_ATM) < 0))
		{
			// ...
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
						int alm_id = ALID__FAST_VENTING__TIMEOUT;
						CString alm_msg;
						CString alm_bff;
						CString r_act;

						alm_msg.Format("The current pressure is %.3f (torr).\n", cur__press);

						alm_bff.Format("The atm-config pressure is %.3f (torr).\n", cfg__press);
						alm_msg += alm_bff;

						alm_bff.Format("The config fast-Vent timeout is %.1f (sec). \n", cfg_timeout);
						alm_msg += alm_bff;

						alm_msg += "Sensor State ... \n";
						alm_bff.Format("  * %s <- %s \n", 
									   diEXT_CH__LBx__ATM_SENSOR->Get__CHANNEL_NAME(),
									   diEXT_CH__LBx__ATM_SENSOR->Get__STRING());
						alm_msg += alm_bff;
						alm_bff.Format("  * %s <- %s \n",
									   diEXT_CH__LBx__VAC_SENSOR->Get__CHANNEL_NAME(),
									   diEXT_CH__LBx__VAC_SENSOR->Get__STRING());
						alm_msg += alm_bff;

						dCH__PRESSURE_STATUS->Get__DATA(var_data);
						alm_bff.Format("Pressure Status is %s.\n", var_data);
						alm_msg += alm_bff;

						Fnc__LOG(alm_msg);

						p_alarm->Popup__ALARM_With_MESSAGE(alm_id,alm_msg,r_act);

						if(r_act.CompareNoCase(ACT__RETRY) == 0)		goto LOOP_RETRY;
						return -22;
					}
				}

				// ...
			}
		}
	}

	return 1;
}


// DOOR VLV CLOSE -----
int  CObj__LBx_CHM_SIMPLE
::Call__DV_CLOSE(CII_OBJECT__VARIABLE* p_variable,
				 CII_OBJECT__ALARM* p_alarm)
{
	SCX__ASYNC_TIMER_CTRL x_timer_ctrl;
	x_timer_ctrl->START__COUNT_UP(9999);

	int r_flag = Fnc__DV_CLOSE(p_variable,p_alarm);

	if(r_flag > 0)
	{
		double cur_sec = x_timer_ctrl->Get__CURRENT_TIME();

		int db_i = _ACT_INDEX__DV_CLOSE;
		_Update__ACTION_MIN_MAX(db_i,cur_sec);
	}

	if(dCH__CFG_IO_OFF_MODE->Check__DATA(STR__ENABLE) > 0)
	{
		doEXT_CH__LBx__DV_OPEN->Set__DATA(STR__OFF);
		doEXT_CH__LBx__DV_CLOSE->Set__DATA(STR__OFF);
	}
	return r_flag;
}
int  CObj__LBx_CHM_SIMPLE
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
		doEXT_CH__LBx__DV_OPEN->Set__DATA(STR__OFF);
		doEXT_CH__LBx__DV_CLOSE->Set__DATA(STR__ON);

		// ...
		{
			str_log.Format("Already... Close sts.. %s Door Valve.", m_sLBx__MODULE_NAME);
			Fnc__LOG(str_log);
		}
		return 1;
	}

	if(Is_DV_CLOSE_CONDITION(p_variable, p_alarm) < 0)
	{
		return -1;
	}

	xI_ASYNC_TIMER->START__COUNT_UP(trg_timeout);

	if(dCH__CFG_IO_OFF_MODE->Check__DATA(STR__ENABLE) > 0)
	{
		doEXT_CH__LBx__DV_OPEN->Set__DATA(STR__OFF);
		doEXT_CH__LBx__DV_CLOSE->Set__DATA(STR__ON);
	}
	else
	{
		doEXT_CH__LBx__DV_OPEN->Set__DATA(STR__OFF);
		Sleep(100);
		doEXT_CH__LBx__DV_CLOSE->Set__DATA(STR__ON);
	}

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
int  CObj__LBx_CHM_SIMPLE
::Call__DV_OPEN(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	if(Check__PRESSURE_ATM_TO_DV_OPEN(p_alarm,ALID__DV_OPEN__NOT_ATM_ERROR) < 0)
	{
		return -1;
	}

	// ...
	SCX__ASYNC_TIMER_CTRL x_timer_ctrl;
	x_timer_ctrl->START__COUNT_UP(9999);

	int r_flag = Fnc__DV_OPEN(p_variable,p_alarm);

	if(r_flag > 0)
	{
		double cur_sec = x_timer_ctrl->Get__CURRENT_TIME();

		int db_i = _ACT_INDEX__DV_OPEN;
		_Update__ACTION_MIN_MAX(db_i,cur_sec);
	}

	if(dCH__CFG_IO_OFF_MODE->Check__DATA(STR__ENABLE) > 0)
	{
		doEXT_CH__LBx__DV_OPEN->Set__DATA(STR__OFF);
		doEXT_CH__LBx__DV_CLOSE->Set__DATA(STR__OFF);
	}
	return r_flag;
}
int  CObj__LBx_CHM_SIMPLE
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

	if(dCH__CFG_IO_OFF_MODE->Check__DATA(STR__ENABLE) > 0)
	{
		doEXT_CH__LBx__DV_CLOSE->Set__DATA(STR__OFF);
		doEXT_CH__LBx__DV_OPEN->Set__DATA(STR__ON);
	}
	else
	{
		doEXT_CH__LBx__DV_CLOSE->Set__DATA(STR__OFF);
		Sleep(100);
		doEXT_CH__LBx__DV_OPEN->Set__DATA(STR__ON);
	}

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
int  CObj__LBx_CHM_SIMPLE
::Call__SV_CLOSE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	SCX__ASYNC_TIMER_CTRL x_timer_ctrl;
	x_timer_ctrl->START__COUNT_UP(9999);

	int r_flag = Fnc__SV_CLOSE(p_variable,p_alarm);

	if(r_flag > 0)
	{
		double cur_sec = x_timer_ctrl->Get__CURRENT_TIME();

		int db_i = _ACT_INDEX__SV_CLOSE;
		_Update__ACTION_MIN_MAX(db_i,cur_sec);
	}

	if(dCH__CFG_IO_OFF_MODE->Check__DATA(STR__ENABLE) > 0)
	{
		doEXT_CH__LBx__SV_OPEN->Set__DATA(STR__OFF);
		doEXT_CH__LBx__SV_CLOSE->Set__DATA(STR__OFF);
	}
	return r_flag;
}
int  CObj__LBx_CHM_SIMPLE
::Fnc__SV_CLOSE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	// ...
	{
		CString str_log;

		str_log = "Fnc__SV_CLOSE() : Started ...";
		Fnc__LOG(str_log);
	}

LOOP_RETRY:

	// ...
	double  trg_timeout = 9999.0;
	double  cfg_timeout = 0.0;
	CString var_data;

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

		// ...
		int r_ret = Sub__SV_CLOSE(p_variable, p_alarm, cfg_timeout);

		if(r_ret > 0)		return  1;			// FULL CLOSE Complete !!
		if(r_ret < 0)		return -1;			// Abort

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
	}

	return -1;
}
int  CObj__LBx_CHM_SIMPLE
::Is__SV_FULL_CLOSE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	CString str_log;

	if((diEXT_CH__LBx__SV_OPEN->Check__DATA(STR__OFF) > 0)
	&& (diEXT_CH__LBx__SV_CLOSE->Check__DATA(STR__ON) > 0))
	{
		str_log.Format("OK..Already... SV Close sts.. %s Slot Valve.", m_sLBx__MODULE_NAME);
		Fnc__LOG(str_log);
		return 1;
	}
	return -1;
}

// SLIT VLV OPEN -----
int  CObj__LBx_CHM_SIMPLE
::Call__SV_OPEN(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	int atm_mode = -1;

	if((dEXT_CH__CFG_SETUP_TEST_MODE->Check__DATA(STR__ENABLE) > 0)
	|| (dEXT_CH__CFG_TRANSFER_MODE->Check__DATA(STR__ATM) > 0))
	{
		atm_mode = 1;
	}

	if(atm_mode > 0)
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

	// ...
	SCX__ASYNC_TIMER_CTRL x_timer_ctrl;
	x_timer_ctrl->START__COUNT_UP(9999);

	int r_flag = Fnc__SV_OPEN(p_variable,p_alarm);

	if(r_flag > 0)
	{
		double cur_sec = x_timer_ctrl->Get__CURRENT_TIME();

		int db_i = _ACT_INDEX__SV_OPEN;
		_Update__ACTION_MIN_MAX(db_i,cur_sec);
	}

	if(dCH__CFG_IO_OFF_MODE->Check__DATA(STR__ENABLE) > 0)
	{
		doEXT_CH__LBx__SV_OPEN->Set__DATA(STR__OFF);
		doEXT_CH__LBx__SV_CLOSE->Set__DATA(STR__OFF);
	}
	return r_flag;
}
int  CObj__LBx_CHM_SIMPLE
::Fnc__SV_OPEN(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	// ...
	{
		CString str_log;

		str_log = "Fnc__SV_OPEN() : Started ...";
		Fnc__LOG(str_log);
	}

	// ...
	CString str_log;
	int alarm_id;

LOOP_RETRY:

	// ...
	double  trg_timeout = 9999.0;
	double  cfg_timeout = 0.0;
	CString var_data;
	
	int r_ret = dEXT_CH__CFG_LLx_SV_EXIST_FLAG->Check__DATA("FALSE");

	if(r_ret > 0)
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
	aEXT_CH__CFG_LLx_LIFT_PIN_UP_TIMEOUT->Get__DATA(var_data);
	cfg_timeout = atof(var_data);

	r_ret = Sub__SV_OPEN(p_variable, p_alarm, cfg_timeout);

	if(r_ret > 0)
	{
		Sleep(500);			// HW 동작 time
		return 1;			// FULL OPEN Complete !!
	}
	if(r_ret < 0)
	{
		return -1;			// Abort
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

int  CObj__LBx_CHM_SIMPLE
::Is__SV_FULL_OPEN(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	CString str_log;

	if((diEXT_CH__LBx__SV_OPEN->Check__DATA(STR__ON)   > 0)
	&& (diEXT_CH__LBx__SV_CLOSE->Check__DATA(STR__OFF) > 0))
	{
		str_log.Format("Already... %s's SV Open sts ...", m_sLBx__MODULE_NAME);
		Fnc__LOG(str_log);
		return 1;
	}

	return -1;
}

// ALL_VLV_CLOSE -----
int  CObj__LBx_CHM_SIMPLE
::Call__ALL_GAS_VLV_CLOSE(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm)
{
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

	return 1;
}

// LIFT_PIN UP -----
int  CObj__LBx_CHM_SIMPLE
::Call__LIFT_PIN_UP(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	return Fnc__LIFT_PIN_UP(p_variable,p_alarm);
}
int  CObj__LBx_CHM_SIMPLE
::Fnc__LIFT_PIN_UP(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	if(!bActive__LIFT_PIN)
	{
		return -11;
	}

	// ...
	{
		CString str_log;

		str_log = "Fnc__LIFT_PIN_UP() : Started ...";
		Fnc__LOG(str_log);
	}

LOOP_RETRY:

	// ...
	int cfg_use = dEXT_CH__CFG_LLx_LIFT_PIN_EXIST_FLAG->Check__DATA("FALSE");

	if(cfg_use > 0)
	{
		int alarm_id = ALID__LIFT_PIN__CFG_NOT_USE;

		CString err_msg;
		CString r_act;

		err_msg.Format("%s's Lift_Pin is not used in config page", m_sLBx__MODULE_NAME);
		Fnc__LOG(err_msg);
		p_alarm->Popup__ALARM(alarm_id,r_act);

			 if(r_act.CompareNoCase(ACT__RETRY) == 0)		goto LOOP_RETRY;
		else if(r_act.CompareNoCase(ACT__ABORT) == 0)		return OBJ_ABORT;
	}

	if(bActive__LIFT_PIN)
	{
		doEXT_CH__LBx__LIFT_PIN_UP->Set__DATA(STR__ON);
		doEXT_CH__LBx__LIFT_PIN_DOWN->Set__DATA(STR__OFF);
	}

	// ...
	CString var_data;

	aEXT_CH__CFG_LLx_LIFT_PIN_UP_TIMEOUT->Get__DATA(var_data);
	double cfg_timeout = atof(var_data);

	SCX__ASYNC_TIMER_CTRL x_timer_ctrl;
	x_timer_ctrl->START__COUNT_DOWN(cfg_timeout + 1.0);

	while(1)
	{
		Sleep(100);

		if(Is__LIFT_PIN_UP(p_variable,p_alarm) > 0)
		{
			double cur_sec = x_timer_ctrl->Get__CURRENT_TIME();

			int db_i = _ACT_INDEX__LIFT_PIN_UP;
			_Update__ACTION_MIN_MAX(db_i,cur_sec);
			return 1;
		}

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			return -1;
		}
		if(x_timer_ctrl->Get__CURRENT_TIME() < 1.0)
		{
			break;
		}
	}

	// ...
	{
		int alarm_id = ALID__LIFT_PIN_UP__TIMEOUT;

		CString err_msg;
		CString r_act;

		err_msg.Format("%s's Lift_Pin Up TimeOut (%.0f)", m_sLBx__MODULE_NAME,cfg_timeout);
		Fnc__LOG(err_msg);
		
		p_alarm->Popup__ALARM(alarm_id,r_act);

		if(r_act.CompareNoCase(ACT__RETRY) == 0)
		{
			goto LOOP_RETRY;
		}
	}
	return -1;
}

int  CObj__LBx_CHM_SIMPLE
::Call__LIFT_PIN_DOWN(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	return Fnc__LIFT_PIN_DOWN(p_variable,p_alarm);
}
int  CObj__LBx_CHM_SIMPLE
::Fnc__LIFT_PIN_DOWN(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	if(!bActive__LIFT_PIN)
	{
		return -11;
	}

	// ...
	{
		CString str_log;

		str_log = "Fnc__LIFT_PIN_DOWN() : Started ...";
		Fnc__LOG(str_log);
	}

LOOP_RETRY:

	// ...
	int cfg_use = dEXT_CH__CFG_LLx_LIFT_PIN_EXIST_FLAG->Check__DATA("FALSE");

	if(cfg_use > 0)
	{
		int alarm_id = ALID__LIFT_PIN__CFG_NOT_USE;

		CString err_msg;
		CString r_act;

		err_msg.Format("%s's Lift_Pin is not used in config page", m_sLBx__MODULE_NAME);
		Fnc__LOG(err_msg);
		p_alarm->Popup__ALARM(alarm_id,r_act);

			 if(r_act.CompareNoCase(ACT__RETRY) == 0)		goto LOOP_RETRY;
		else if(r_act.CompareNoCase(ACT__ABORT) == 0)		return OBJ_ABORT;
	}

	if(bActive__LIFT_PIN)
	{
		doEXT_CH__LBx__LIFT_PIN_UP->Set__DATA(STR__OFF);
		doEXT_CH__LBx__LIFT_PIN_DOWN->Set__DATA(STR__ON);
	}

	// ...
	CString var_data;

	aEXT_CH__CFG_LLx_LIFT_PIN_DOWN_TIMEOUT->Get__DATA(var_data);
	double cfg_timeout = atof(var_data);

	SCX__ASYNC_TIMER_CTRL x_timer_ctrl;
	x_timer_ctrl->START__COUNT_DOWN(cfg_timeout + 1.0);

	while(1)
	{
		Sleep(100);

		if(Is__LIFT_PIN_DOWN(p_variable,p_alarm) > 0)
		{
			double cur_sec = x_timer_ctrl->Get__CURRENT_TIME();

			int db_i = _ACT_INDEX__LIFT_PIN_DOWN;
			_Update__ACTION_MIN_MAX(db_i,cur_sec);
			return 1;
		}

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			return -1;
		}
		if(x_timer_ctrl->Get__CURRENT_TIME() < 1.0)
		{
			break;
		}
	}

	// ...
	{
		int alarm_id = ALID__LIFT_PIN_DOWN__TIMEOUT;

		CString err_msg;
		CString r_act;

		err_msg.Format("%s's Lift_Pin Down TimeOut (%.0f)", m_sLBx__MODULE_NAME,cfg_timeout);
		Fnc__LOG(err_msg);

		p_alarm->Popup__ALARM(alarm_id,r_act);

		if(r_act.CompareNoCase(ACT__RETRY) == 0)
		{
			goto LOOP_RETRY;
		}
	}
	return -1;
}

int  CObj__LBx_CHM_SIMPLE
::Is__LIFT_PIN_UP(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	if(!bActive__LIFT_PIN)
	{
		return -11;
	}

	if((diEXT_CH__LBx__LIFT_PIN_UP->Check__DATA(STR__ON)    > 0)
	&& (diEXT_CH__LBx__LIFT_PIN_DOWN->Check__DATA(STR__OFF) > 0))
	{
		CString str_log;

		str_log.Format("Already... %s's Lift_Pin Up sts ...", m_sLBx__MODULE_NAME);
		Fnc__LOG(str_log);
		return 1;
	}

	return -1;
}
int  CObj__LBx_CHM_SIMPLE
::Is__LIFT_PIN_DOWN(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	if(!bActive__LIFT_PIN)
	{
		return -11;
	}

	if((diEXT_CH__LBx__LIFT_PIN_UP->Check__DATA(STR__OFF)  > 0)
	&& (diEXT_CH__LBx__LIFT_PIN_DOWN->Check__DATA(STR__ON) > 0))
	{
		CString str_log;

		str_log.Format("Already... %s's Lift_Pin Down sts ...", m_sLBx__MODULE_NAME);
		Fnc__LOG(str_log);
		return 1;
	}

	return -1;
}


// SV_TRANSFER -----
int  CObj__LBx_CHM_SIMPLE
::Call__SV_TRANSFER_OPEN(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	return Fnc__SV_TRANSFER_OPEN(p_variable,p_alarm);
}
int  CObj__LBx_CHM_SIMPLE
::Fnc__SV_TRANSFER_OPEN(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	// ...
	{
		CString str_log;

		str_log = "Fnc__SV_TRANSFER_OPEN() : Started ...";
		Fnc__LOG(str_log);
	}

	// ...
	int cfg_use = -1;

	if(bActive__LIFT_PIN)
	{
		cfg_use = dEXT_CH__CFG_LLx_LIFT_PIN_EXIST_FLAG->Check__DATA("TRUE");

		if(cfg_use > 0)
		{
			doEXT_CH__LBx__LIFT_PIN_UP->Set__DATA(STR__ON);
			doEXT_CH__LBx__LIFT_PIN_DOWN->Set__DATA(STR__OFF);
		}
	}

	// ...
	int r_ret =  Call__SV_OPEN(p_variable, p_alarm);

	if(r_ret > 0)
	{
		if(cfg_use > 0)	
		{
			r_ret = Call__LIFT_PIN_UP(p_variable, p_alarm);
		}
	}
	return r_ret;
}

int  CObj__LBx_CHM_SIMPLE
::Call__SV_TRANSFER_CLOSE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	return Fnc__SV_TRANSFER_CLOSE(p_variable,p_alarm);
}
int  CObj__LBx_CHM_SIMPLE
::Fnc__SV_TRANSFER_CLOSE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	// ...
	{
		CString str_log;

		str_log = "Fnc__SV_TRANSFER_CLOSE() : Started ...";
		Fnc__LOG(str_log);
	}

	// ...
	int cfg_use = -1;

	if(bActive__LIFT_PIN)
	{
		if((dEXT_CH__CFG_LLx_LIFT_PIN_EXIST_FLAG->Check__DATA("TRUE") > 0)
		&& (dEXT_CH__CFG_LLx_LIFT_PIN_DOWM_MODE_AFTER_DOOR_CLOSE->Check__DATA("ENABLE") > 0))
		{
			cfg_use = 1;
		}

		if(cfg_use > 0)
		{
			doEXT_CH__LBx__LIFT_PIN_UP->Set__DATA(STR__OFF);
			doEXT_CH__LBx__LIFT_PIN_DOWN->Set__DATA(STR__ON);
		}
	}

	// ...
	int r_ret =  Call__SV_CLOSE(p_variable, p_alarm);

	if(r_ret > 0)
	{
		if(cfg_use > 0)
		{
			r_ret = Call__LIFT_PIN_DOWN(p_variable, p_alarm);
		}
	}
	return r_ret;
}

// DV_TRANSFER -----
int  CObj__LBx_CHM_SIMPLE
::Call__DV_TRANSFER_OPEN(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	return Fnc__DV_TRANSFER_OPEN(p_variable,p_alarm);
}
int  CObj__LBx_CHM_SIMPLE
::Fnc__DV_TRANSFER_OPEN(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	// ...
	{
		CString str_log;

		str_log = "Fnc__DV_TRANSFER_OPEN() : Started ...";
		Fnc__LOG(str_log);
	}

	// ...
	int cfg_use = -1;

	if(bActive__LIFT_PIN)
	{
		int cfg_use = dEXT_CH__CFG_LLx_LIFT_PIN_EXIST_FLAG->Check__DATA("TRUE");

		if(cfg_use > 0)
		{
			doEXT_CH__LBx__LIFT_PIN_UP->Set__DATA(STR__ON);
			doEXT_CH__LBx__LIFT_PIN_DOWN->Set__DATA(STR__OFF);
		}
	}

	// ...
	int r_ret =  Call__DV_OPEN(p_variable, p_alarm);

	if(r_ret > 0)
	{
		if(cfg_use > 0)
		{
			r_ret = Call__LIFT_PIN_UP(p_variable, p_alarm);
		}
	}
	return r_ret;
}

int  CObj__LBx_CHM_SIMPLE
::Call__DV_TRANSFER_CLOSE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	return Fnc__DV_TRANSFER_CLOSE(p_variable,p_alarm);
}
int  CObj__LBx_CHM_SIMPLE
::Fnc__DV_TRANSFER_CLOSE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	// ...
	{
		CString str_log;

		str_log = "Fnc__DV_TRANSFER_CLOSE() : Started ...";
		Fnc__LOG(str_log);
	}

	// ...
	int cfg_use = -1;

	if(bActive__LIFT_PIN)
	{
		if((dEXT_CH__CFG_LLx_LIFT_PIN_EXIST_FLAG->Check__DATA("TRUE") > 0)
		&& (dEXT_CH__CFG_LLx_LIFT_PIN_DOWM_MODE_AFTER_DOOR_CLOSE->Check__DATA("ENABLE") > 0))
		{
			cfg_use = 1;
		}

		if(cfg_use > 0)
		{
			doEXT_CH__LBx__LIFT_PIN_UP->Set__DATA(STR__OFF);
			doEXT_CH__LBx__LIFT_PIN_DOWN->Set__DATA(STR__ON);
		}
	}

	// ...
	int r_ret =  Call__DV_CLOSE(p_variable, p_alarm);

	if(r_ret > 0)
	{
		if(cfg_use > 0)
		{
			r_ret = Call__LIFT_PIN_DOWN(p_variable, p_alarm);
		}
	}
	return r_ret;
}


// PREPMATER -----
int  CObj__LBx_CHM_SIMPLE
::Call__PREPMATER(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	if(dEXT_CH__CFG_LLx_LIFT_PIN_EXIST_FLAG->Check__DATA("TRUE") < 0)
	{
		return 1;
	}

	return Fnc__LIFT_PIN_UP(p_variable,p_alarm);
}

// COMPMATER -----
int  CObj__LBx_CHM_SIMPLE
::Call__COMPMATER(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	return 1;
}


// IN_PROC -----
int  CObj__LBx_CHM_SIMPLE
::Call__IN_PROC(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	return Fnc__IN_PROC(p_variable,p_alarm);
}

int  CObj__LBx_CHM_SIMPLE
::Fnc__IN_PROC(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{

	return 1;
}

// OUT_PROC -----
int  CObj__LBx_CHM_SIMPLE
::Call__OUT_PROC(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	return Fnc__OUT_PROC(p_variable,p_alarm);
}

int  CObj__LBx_CHM_SIMPLE
::Fnc__OUT_PROC(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	if(dCH__CFG_HEATER_USE->Check__DATA("YES") < 0)
	{
		return 1;
	}

	// ...
	{
		int wafer_count = 0;

		int i_limit = iLBx_SLOT_SIZE;
		int i;

		for(i=0; i<i_limit; i++)
		{
			if(dCH__SLOT_STATUS[0]->Check__DATA("NONE") < 0)
				wafer_count++;
		}

		if(wafer_count < 1)
		{
			return 1;
		}
	}

	if(diEXT_CH__LBx__VAC_SENSOR->Check__DATA(sDATA__VAC_ON) < 0)
	{
		// Alarm Post ...
		{

		}
		return -1;
	}

	if(Fnc__LIFT_PIN_DOWN(p_variable,p_alarm) < 0)
	{
		return -1;
	}

	if(dCH__CFG_HEATER_SOFT_VENT_USE->Check__DATA(STR__YES) > 0)
	{
		doEXT_CH__FAST_VENT_VLV__SET->Set__DATA(STR__CLOSE);
		doEXT_CH__ATM_EQUAL_VLV__SET->Set__DATA(STR__CLOSE);

		doEXT_CH__FAST_PUMP_VLV__SET->Set__DATA(STR__CLOSE);

		doEXT_CH__SOFT_PUMP_VLV__SET->Set__DATA(STR__OPEN);
		doEXT_CH__SOFT_VENT_VLV__SET->Set__DATA(STR__OPEN);
	}

	// ...
	{
		CString ch_data;

		aCH__CFG_HEATING_TIME->Get__DATA(ch_data);
		double cfg_sec = atof(ch_data);

		// ...
		SCX__TIMER_CTRL x_timer_ctrl;

		x_timer_ctrl->REGISTER__ABORT_OBJECT(sObject_Name);
		x_timer_ctrl->REGISTER__COUNT_CHANNEL(sCH__APP_HEATING_TIMECOUNT->Get__CHANNEL_NAME());

		x_timer_ctrl->INIT__COUNT_DOWN();
		x_timer_ctrl->WAIT(cfg_sec);
	}

	if(dCH__CFG_HEATER_SOFT_VENT_USE->Check__DATA(STR__YES) > 0)
	{
		doEXT_CH__SOFT_VENT_VLV__SET->Set__DATA(STR__CLOSE);
	}

	if(dEXT_CH__CFG_LLx_LIFT_PIN_UP_MODE_AFTER_HEATING->Check__DATA(STR__ENABLE) > 0)
	{
		return Fnc__LIFT_PIN_UP(p_variable,p_alarm);
	}
	return 1;
}


// AUTO CTRL ---------------
// PUMP VLV CLOSE -----
int  CObj__LBx_CHM_SIMPLE
::Call__AUTO_PUMP_VLV_CLOSE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	return Fnc__AUTO_PUMP_VLV_CLOSE(p_variable,p_alarm);
}

int  CObj__LBx_CHM_SIMPLE
::Fnc__AUTO_PUMP_VLV_CLOSE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	Fnc__PUMP_ALL_VLV__CLOSE(p_alarm);
	return 1;
}


// TIME TEST ...
int  CObj__LBx_CHM_SIMPLE
::Call__TIME_TEST(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	int i_limit = 2000;
	int i;

	for(i=0; i<i_limit; i++)
	{
		double cur_sec = 5.0;
		if(i > 1000)		cur_sec = 10.0;

		// DOWN ...
		{
			int db_i = _ACT_INDEX__LIFT_PIN_DOWN;
			_Update__ACTION_MIN_MAX(db_i, cur_sec);
		}
		// UP ...
		{
			int db_i = _ACT_INDEX__LIFT_PIN_UP;
			_Update__ACTION_MIN_MAX(db_i, cur_sec);
		}

		// OPEN ...
		{
			int db_i = _ACT_INDEX__SV_OPEN;
			_Update__ACTION_MIN_MAX(db_i, cur_sec);
		}
		// CLOSE ...
		{
			int db_i = _ACT_INDEX__SV_CLOSE;
			_Update__ACTION_MIN_MAX(db_i, cur_sec);
		}
	}
	return 1;
}
