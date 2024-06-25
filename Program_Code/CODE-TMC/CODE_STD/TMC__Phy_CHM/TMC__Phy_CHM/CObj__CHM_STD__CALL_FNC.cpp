#include "StdAfx.h"
#include "CObj__CHM_STD.h"
#include "CObj__CHM_STD__DEF.h"
#include "CObj__CHM_STD__ALID.h"


//----------------------------------------------------------------------------------------------------
// INIT -----
int  CObj__CHM_STD
::Call__INIT(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm)
{

	return 1;
}

//
int  CObj__CHM_STD
::Call__CL_ALL_VLV(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm)
{
	Fnc__VENT_ALL_VLV__CLOSE(p_alarm);
	Fnc__PUMP_ALL_VLV__CLOSE(p_variable, p_alarm);
	return 1;
}

int  CObj__CHM_STD
::Call__CL_PUMP_VLV(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm)
{
	Fnc__PUMP_ALL_VLV__CLOSE(p_variable, p_alarm);
	return 1;
}
int  CObj__CHM_STD
::Call__OP_PUMP_VLV(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm)
{
	Fnc__PUMP_FAST_VLV__OPEN(p_variable, p_alarm);
	return 1;
}

// PUMP -----
int  CObj__CHM_STD
::Call__PUMP(CII_OBJECT__VARIABLE* p_variable,
		     CII_OBJECT__ALARM* p_alarm)
{
	if(Check__CHM_LID_CLOSE(p_alarm) < 0)
	{
		return -1;
	}

	return _Fnc__PUMP(p_variable,p_alarm);
}
int  CObj__CHM_STD
::_Fnc__PUMP(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm)
{
	double  cur__press;
	double  cfg__press;

	CString var_data;
	CString str_log;

	Fnc__LOG("Vent Valve All Close !!");
	if(Fnc__VENT_ALL_VLV__CLOSE(p_alarm) < 0)
	{
		return -1;
	}

	Fnc__LOG("Pump Valve All Close !!");
	if(Fnc__PUMP_ALL_VLV__CLOSE(p_variable, p_alarm) < 0)
	{
		return -2;
	}

	if(sEXT_CH__MON_PUMP_RUN_STS->Check__DATA(STR__ON) < 0)	
	{
		Fnc__LOG("Dry Pump ON !!");
		
		if(pPUMP__OBJ_CTRL->Call__OBJECT("PUMP_ON") < 0)
		{
			return -3;
		}
	}

	// CHECK : EXHAUST PRESSURE ...
	if(bActive__TMC_CHM__EXHAUST_PRESSURE)
	{
		SCX__ASYNC_TIMER_CTRL x_timer;

		x_timer->START__COUNT_UP(99999);

		while(1)
		{
			Sleep(10);

			cur__press = aiEXT_CH__TMC_CHM__EXHAUST_PRESSURE->Get__VALUE();
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
									aiEXT_CH__TMC_CHM__EXHAUST_PRESSURE->Get__CHANNEL_NAME(),
									aiEXT_CH__TMC_CHM__EXHAUST_PRESSURE->Get__VALUE());
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

	// Check : Current State ...
	{
		cur__press = aiEXT_CH__CHM_PRESSURE_TORR->Get__VALUE();
		cfg__press = aCH__CFG_FAST_PUMP_PRESSURE_TORR->Get__VALUE();

		str_log.Format("1. Get.. Pressure : current [%f] / config [%f] !!", cur__press,cfg__press);
		Fnc__LOG(str_log);

		if((cur__press <= cfg__press)
		&& (diEXT_CH__VAC_SENSOR->Check__DATA(sDATA__VAC_ON) > 0))
		{
			Fnc__PUMP_FAST_VLV__OPEN(p_variable, p_alarm);
			
			str_log.Format("Already Pump Status.. Current-Pressure [%f]/[%f], VAC_Sns[%s]", cur__press,cfg__press,STR__VAC);
			Fnc__LOG(str_log);
			return 1;
		}
	}

	// Soft Pumping ...
	{
		Fnc__LOG("Soft-Pumping Valve Open !!");
		Fnc__PUMP_SOFT_VLV__OPEN(p_variable, p_alarm);

		cur__press = aiEXT_CH__CHM_PRESSURE_TORR->Get__VALUE();
		cfg__press = aCH__CFG_SOFT_PUMP_PRESSURE_TORR->Get__VALUE();

		str_log.Format("1. Get.. Pressure : current [%f] / config [%f] !!", cur__press,cfg__press);
		Fnc__LOG(str_log);

		if(iActive__SIM_MODE > 0)
		{
			SCX__TIMER_CTRL xSim_timer;

			xSim_timer->REGISTER__ABORT_OBJECT(sObject_Name);
			double trg__timeout = aEXT_CH__CFG_SIM_PUMP_TIME->Get__VALUE();

			if(xSim_timer->WAIT(trg__timeout) < 0)
			{
				return -1;
			}

			aiEXT_CH__CHM_PRESSURE_TORR->Set__VALUE(cfg__press - 0.01);

			if(!bActive__ATM_SNS_Virtual_Type)			diEXT_CH__ATM_SENSOR->Set__DATA(sDATA__ATM_OFF);
			if(!bActive__VAC_SNS_Virtual_Type)			diEXT_CH__VAC_SENSOR->Set__DATA(sDATA__VAC_OFF);
		}

		if(cur__press > cfg__press) // 220930 : "<="부분 이상으로 사용 시 Skip으로 Fast로 바로 넘어가는 문제 있음
		{
			CII__VAR_ANALOG_CTRL *pch__gauge_pressure = aiEXT_CH__CHM_PRESSURE_TORR.Get__PTR();
			CII__VAR_ANALOG_CTRL *pch__cfg_pressyre   = aCH__CFG_SOFT_PUMP_PRESSURE_TORR.Get__PTR();
			CII__VAR_ANALOG_CTRL *pch__cfg_timeout    = aCH__CFG_SOFT_PUMP_TIMEOUT.Get__PTR();

			bool active__atm_sns = TRUE;
			CII__VAR_DIGITAL_CTRL *pch__atm_sns = diEXT_CH__ATM_SENSOR.Get__PTR();
			CString trg__atm_sts = sDATA__ATM_OFF;

			bool active__vac_sns = FALSE;
			CII__VAR_DIGITAL_CTRL *pch__vac_sns = diEXT_CH__VAC_SENSOR.Get__PTR();
			CString trg__vac_sts = sDATA__VAC_ON;
			
			int alm_id__pumping_timeout = ALID__SOFT_PUMPING__TIMEOUT;

			int r_flag = _Wait__PUMPING_PRESSURE(p_variable,
												 p_alarm,
												 pch__gauge_pressure,
												 pch__cfg_pressyre,
												 pch__cfg_timeout,
												 active__atm_sns,
												 pch__atm_sns,
												 trg__atm_sts,
												 active__vac_sns,
												 pch__vac_sns,
												 trg__vac_sts,
												 alm_id__pumping_timeout);

			if(r_flag < 0)			return r_flag;

			// Stable Waiting ...
			{
				SCX__TIMER_CTRL x_timer;
				x_timer->REGISTER__ABORT_OBJECT(sObject_Name);

				double trg__timeout = aCH__CFG_SOFT_PUMP_COMPLETE_STABLE_TIME->Get__VALUE();
				if(x_timer->WAIT(trg__timeout) < 0)
				{
					return -11;
				}
			}
		}
	}

	// Fast Pumping ...
	{
		Fnc__LOG("Fast-Pumping Valve Open !!");
		Fnc__PUMP_FAST_VLV__OPEN(p_variable, p_alarm);

		cur__press = aiEXT_CH__CHM_PRESSURE_TORR->Get__VALUE();
		cfg__press = aCH__CFG_FAST_PUMP_PRESSURE_TORR->Get__VALUE();

		str_log.Format("1. Get.. Pressure : current [%f] / config [%f] !!", cur__press,cfg__press);
		Fnc__LOG(str_log);

		if(iActive__SIM_MODE > 0)
		{
			SCX__TIMER_CTRL xSim_timer;
		
			xSim_timer->REGISTER__ABORT_OBJECT(sObject_Name);
			double cfg__timeout = aEXT_CH__CFG_SIM_PUMP_TIME->Get__VALUE();

			if(xSim_timer->WAIT(cfg__timeout) < 0)
			{
				return -1;
			}

			aiEXT_CH__CHM_PRESSURE_TORR->Set__VALUE(cfg__press - 0.01);
	
			if(!bActive__ATM_SNS_Virtual_Type)			diEXT_CH__ATM_SENSOR->Set__DATA(sDATA__ATM_OFF);
			if(!bActive__VAC_SNS_Virtual_Type)			diEXT_CH__VAC_SENSOR->Set__DATA(sDATA__VAC_ON);
		}

		// ...
		CII__VAR_ANALOG_CTRL *pch__gauge_pressure = aiEXT_CH__CHM_PRESSURE_TORR.Get__PTR();
		CII__VAR_ANALOG_CTRL *pch__cfg_pressyre   = aCH__CFG_FAST_PUMP_PRESSURE_TORR.Get__PTR();
		CII__VAR_ANALOG_CTRL *pch__cfg_timeout    = aCH__CFG_FAST_PUMP_TIMEOUT.Get__PTR();

		bool active__atm_sns = TRUE;
		CII__VAR_DIGITAL_CTRL *pch__atm_sns = diEXT_CH__ATM_SENSOR.Get__PTR();
		CString trg__atm_sts = sDATA__ATM_OFF;

		bool active__vac_sns = TRUE;
		CII__VAR_DIGITAL_CTRL *pch__vac_sns = diEXT_CH__VAC_SENSOR.Get__PTR();
		CString trg__vac_sts = sDATA__VAC_ON;

		int alm_id__pumping_timeout = ALID__FAST_PUMPING__TIMEOUT;

		int r_flag = _Wait__PUMPING_PRESSURE(p_variable,
											 p_alarm,
											 pch__gauge_pressure,
											 pch__cfg_pressyre,
											 pch__cfg_timeout,
											 active__atm_sns,
											 pch__atm_sns,
											 trg__atm_sts,
											 active__vac_sns,
											 pch__vac_sns,
											 trg__vac_sts,
											 alm_id__pumping_timeout);

		if(r_flag < 0)			return r_flag;
	}

	Update__PUMP_VLV_STS(p_variable, p_alarm);
	return 1;
}
int  CObj__CHM_STD
::_Wait__PUMPING_PRESSURE(CII_OBJECT__VARIABLE  *p_variable,
						  CII_OBJECT__ALARM     *p_alarm,
						  CII__VAR_ANALOG_CTRL  *pch__gauge_pressure,
						  CII__VAR_ANALOG_CTRL  *pch__cfg_pressyre,
						  CII__VAR_ANALOG_CTRL  *pch__cfg_timeout,
						  const bool active__atm_sns,
						  CII__VAR_DIGITAL_CTRL *pch__atm_sns,
						  const CString& trg__atm_sts,
						  const bool active__vac_sns,
						  CII__VAR_DIGITAL_CTRL *pch__vac_sns,
						  const CString& trg__vac_sts,
						  const int alm_id__pumping_timeout)
{
LOOP_RETRY:

	// ...
	double cur__press;
	double cfg__press;

	SCX__ASYNC_TIMER_CTRL x_timer;	
	x_timer->REGISTER__COUNT_CHANNEL_NAME(aCH__TIME_COUNT->Get__CHANNEL_NAME());
	x_timer->START__COUNT_UP(99999);

	while(1)
	{	
		Sleep(90);

		// ...
		cur__press = pch__gauge_pressure->Get__VALUE();
		cfg__press = pch__cfg_pressyre->Get__VALUE();

		int check__atm_sns = 1;
		int check__vac_sns = 1;

		if(active__atm_sns)
		{
			if(pch__atm_sns->Check__DATA(trg__atm_sts) < 0)			check__atm_sns = -1;
		}
		if(active__vac_sns)
		{
			if(pch__vac_sns->Check__DATA(trg__vac_sts) < 0)			check__vac_sns = -1;
		}

		if((cur__press < cfg__press)
		&& (check__atm_sns > 0)
		&& (check__vac_sns > 0))
		{
			CString str_log;
			CString str_bff;

			str_log = "Complete... \n";
			
			str_bff.Format(" * Current [%.3f] < Config [%.3f] \n", cur__press,cfg__press);
			str_log += str_bff;

			if(active__atm_sns)
			{
				str_bff.Format(" * %s <- %s \n", 
								pch__atm_sns->Get__CHANNEL_NAME(),
								pch__atm_sns->Get__STRING());
				str_log += str_bff;
			}
			if(active__vac_sns)
			{
				str_bff.Format(" * %s <- %s \n", 
								pch__vac_sns->Get__CHANNEL_NAME(),
								pch__vac_sns->Get__STRING());
				str_log += str_bff;
			}

			Fnc__LOG(str_log);
			return 1;
		}

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			return -21;
		}

		double cfg__timeout = pch__cfg_timeout->Get__VALUE();
		if(x_timer->Get__CURRENT_TIME() >= cfg__timeout)
		{
			int alm_id = alm_id__pumping_timeout;
			CString alm_msg;
			CString alm_bff;
			CString r_act;

			alm_msg.Format("The current pressure is (%.3f), The config pressure is %.3f.\n",
						   cur__press, cfg__press);

			if(pch__vac_sns->Check__DATA(trg__vac_sts) > 0)
			{
				alm_msg += "VAC Sensor is VAC (Meaning vacuum). \n";
			}
			else
			{
				alm_msg += "VAC Sensor is Not VAC (Meaning ATM or BTW). \n";
			}

			alm_msg += "Sensor State ... \n";

			if(active__atm_sns)
			{
				alm_bff.Format("  * %s <- %s \n", 
								pch__atm_sns->Get__CHANNEL_NAME(),
								pch__atm_sns->Get__STRING());
				alm_msg += alm_bff;
			}
			if(active__vac_sns)
			{
				alm_bff.Format("  * %s <- %s \n",
								pch__vac_sns->Get__CHANNEL_NAME(),
								pch__vac_sns->Get__STRING());
				alm_msg += alm_bff;
			}

			CString ch_data = dCH__TMC_CHM_PRESSURE_STATUS->Get__STRING();
			alm_bff.Format("Pressure Status is %s. \n", ch_data);
			alm_msg += alm_bff;

			Fnc__LOG(alm_msg);

			p_alarm->Popup__ALARM_With_MESSAGE(alm_id,alm_msg,r_act);
			if(r_act.CompareNoCase(ACT__RETRY) == 0)
			{
				goto LOOP_RETRY;
			}
			return -22;
		}
	}

	return -23;
}

// VENT -----
int  CObj__CHM_STD
::Call__VENT(CII_OBJECT__VARIABLE* p_variable,
		     CII_OBJECT__ALARM* p_alarm)
{
	if(Check__CHM_LID_CLOSE(p_alarm) < 0)
	{
		return -1;
	}

	int r_flag = _Fnc__VENT(p_variable,p_alarm);
	if(r_flag > 0)
	{
		r_flag = _Sub__VENT(p_variable, p_alarm);
	}

	Fnc__VENT_ALL_VLV__CLOSE_WITHOUT_EQUAL_VLV(p_alarm);

	if(dCH__CFG_EQUAL_VLV_OPEN_WHEN_ATM->Check__DATA(STR__ENABLE) < 0)
	{
		if(bActive__ATM_EQUAL_VLV)
			doEXT_CH__ATM_EQUAL_VLV__SET->Set__DATA(STR__CLOSE);
	}
	return r_flag;
}
int  CObj__CHM_STD
::_Fnc__VENT(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm)
{
LOOP_RETRY:

	// ...
	double  cur__press;
	double  cfg__press;

	// Init Valve ...
	{
		Fnc__LOG("Pump Valve All Close !!");
		Fnc__PUMP_ALL_VLV__CLOSE(p_variable, p_alarm);

		Fnc__LOG("Vent Valve All Close !!");
		Fnc__VENT_ALL_VLV__CLOSE(p_alarm);
	}

	// Current Pressure Check ...
	{
		CString str_log;

		cur__press = aiEXT_CH__CHM_PRESSURE_TORR->Get__VALUE();
		cfg__press = aCH__CFG_FAST_VENT_PRESSURE_TORR->Get__VALUE();

		str_log.Format("1. Get.. Pressure : current [%f] / atm config [%f] !!", cur__press, cfg__press);
		Fnc__LOG(str_log);

		if((cur__press >= cfg__press)
		&& (diEXT_CH__ATM_SENSOR->Check__DATA(sDATA__ATM_ON)  > 0)
		&& (diEXT_CH__VAC_SENSOR->Check__DATA(sDATA__VAC_OFF) > 0))
		{
			Fnc__PUMP_ALL_VLV__CLOSE(p_variable, p_alarm);

			str_log.Format("Already Vent Status.. Current Prs[%f]/[%f], ATM_Sns[%s]", cur__press, cfg__press, STR__ON);
			Fnc__LOG(str_log);
			return 1;
		}
	}

	// Soft Venting ...
	{
		Fnc__LOG("Soft-Vent Valve Open !!");
		Fnc__VENT_SOFT_VLV__OPEN(p_alarm);

		if(iActive__SIM_MODE > 0)
		{
			SCX__TIMER_CTRL x_timer;

			x_timer->REGISTER__ABORT_OBJECT(sObject_Name);
			double cfg__timeout = aEXT_CH__CFG_SIM_VENT_TIME->Get__VALUE();

			if(x_timer->WAIT(cfg__timeout) < 0)
			{
				return -1;
			}

			cfg__press = aCH__CFG_SOFT_VENT_PRESSURE_TORR->Get__VALUE();
			aiEXT_CH__CHM_PRESSURE_TORR->Set__VALUE(cfg__press + 0.01);

			if(!bActive__ATM_SNS_Virtual_Type)			diEXT_CH__ATM_SENSOR->Set__DATA(sDATA__ATM_OFF);
			if(!bActive__VAC_SNS_Virtual_Type)			diEXT_CH__VAC_SENSOR->Set__DATA(sDATA__VAC_OFF);
		}

		// ...
		CII__VAR_ANALOG_CTRL *pch__gauge_pressure = aiEXT_CH__CHM_PRESSURE_TORR.Get__PTR();
		CII__VAR_ANALOG_CTRL *pch__cfg_pressyre   = aCH__CFG_SOFT_VENT_PRESSURE_TORR.Get__PTR();
		CII__VAR_ANALOG_CTRL *pch__cfg_timeout    = aCH__CFG_SOFT_VENT_TIMEOUT.Get__PTR();

		bool active__atm_sns = FALSE;
		CII__VAR_DIGITAL_CTRL *pch__atm_sns = diEXT_CH__ATM_SENSOR.Get__PTR();
		CString trg__atm_sts = sDATA__ATM_ON;

		bool active__vac_sns = TRUE;
		CII__VAR_DIGITAL_CTRL *pch__vac_sns = diEXT_CH__VAC_SENSOR.Get__PTR();
		CString trg__vac_sts = sDATA__VAC_OFF;

		int alm_id__venting_timeout = ALID__SOFT_VENTING__TIMEOUT;

		int r_flag = _Wait__VENTING_PRESSURE(p_variable,
											 p_alarm,
											 pch__gauge_pressure,
											 pch__cfg_pressyre,
											 pch__cfg_timeout,
											 active__atm_sns,
											 pch__atm_sns,
											 trg__atm_sts,
											 active__vac_sns,
											 pch__vac_sns,
											 trg__vac_sts,
											 alm_id__venting_timeout);

		if(r_flag < 0)			return r_flag;
	}

	// Fast Venting ...
	{
		Fnc__LOG("Fast-Vent Valve Open !!");
		Fnc__VENT_FAST_VLV__OPEN(p_alarm);

		if(iActive__SIM_MODE > 0)
		{
			SCX__TIMER_CTRL x_timer;
		
			x_timer->REGISTER__ABORT_OBJECT(sObject_Name);
			double cfg__timeout = aEXT_CH__CFG_SIM_VENT_TIME->Get__VALUE();

			if(x_timer->WAIT(cfg__timeout) < 0)
			{
				return -1;
			}

			cfg__press = aCH__CFG_FAST_VENT_PRESSURE_TORR->Get__VALUE();
			aiEXT_CH__CHM_PRESSURE_TORR->Set__VALUE(cfg__press + 0.01);
	
			if(!bActive__ATM_SNS_Virtual_Type)			diEXT_CH__ATM_SENSOR->Set__DATA(sDATA__ATM_ON);
			if(!bActive__VAC_SNS_Virtual_Type)			diEXT_CH__VAC_SENSOR->Set__DATA(sDATA__VAC_OFF);
		}

		// ...
		CII__VAR_ANALOG_CTRL *pch__gauge_pressure = aiEXT_CH__CHM_PRESSURE_TORR.Get__PTR();
		CII__VAR_ANALOG_CTRL *pch__cfg_pressyre   = aCH__CFG_FAST_VENT_PRESSURE_TORR.Get__PTR();
		CII__VAR_ANALOG_CTRL *pch__cfg_timeout    = aCH__CFG_FAST_VENT_TIMEOUT.Get__PTR();

		bool active__atm_sns = TRUE;
		CII__VAR_DIGITAL_CTRL *pch__atm_sns = diEXT_CH__ATM_SENSOR.Get__PTR();
		CString trg__atm_sts = sDATA__ATM_ON;
		
		bool active__vac_sns = TRUE;
		CII__VAR_DIGITAL_CTRL *pch__vac_sns = diEXT_CH__VAC_SENSOR.Get__PTR();
		CString trg__vac_sts = sDATA__VAC_OFF;
		
		int alm_id__venting_timeout = ALID__FAST_VENTING__TIMEOUT;
		
		int r_flag = _Wait__VENTING_PRESSURE(p_variable,
											 p_alarm,
											 pch__gauge_pressure,
											 pch__cfg_pressyre,
											 pch__cfg_timeout,
											 active__atm_sns,
											 pch__atm_sns,
											 trg__atm_sts,
											 active__vac_sns,
											 pch__vac_sns,
											 trg__vac_sts,
											 alm_id__venting_timeout);

		if(r_flag < 0)			return r_flag;
	}

	// Over-Venting ...
	{
		SCX__TIMER_CTRL x_timer;

		double cfg__timeout = aCH__CFG_OVER_VENT_TIME->Get__VALUE();		
		if(x_timer->WAIT(cfg__timeout) < 0)
		{
			return -1;
		}
	}

	Fnc__VENT_ALL_VLV__CLOSE(p_alarm);
	return 1;
}
int  CObj__CHM_STD
::_Wait__VENTING_PRESSURE(CII_OBJECT__VARIABLE* p_variable,
						  CII_OBJECT__ALARM* p_alarm,
						  CII__VAR_ANALOG_CTRL  *pch__gauge_pressure,
						  CII__VAR_ANALOG_CTRL  *pch__cfg_pressyre,
						  CII__VAR_ANALOG_CTRL  *pch__cfg_timeout,
						  const bool active__atm_sns,
						  CII__VAR_DIGITAL_CTRL *pch__atm_sns,
						  const CString& trg__atm_sts,
						  const bool active__vac_sns,
						  CII__VAR_DIGITAL_CTRL *pch__vac_sns,
						  const CString& trg__vac_sts,
						  const int alm_id__venting_timeout)
{
LOOP_RETRY:

	// ...
	double cur__press;
	double cfg__press;

	SCX__ASYNC_TIMER_CTRL x_timer;	
	x_timer->REGISTER__COUNT_CHANNEL_NAME(aCH__TIME_COUNT->Get__CHANNEL_NAME());
	x_timer->START__COUNT_UP(99999);

	while(1)
	{	
		Sleep(90);

		// ...
		cur__press = pch__gauge_pressure->Get__VALUE();
		cfg__press = pch__cfg_pressyre->Get__VALUE();

		int check__atm_sns = 1;
		int check__vac_sns = 1;
		
		if(active__atm_sns)
		{
			if(pch__atm_sns->Check__DATA(trg__atm_sts) < 0)			check__atm_sns = -1;
		}
		if(active__vac_sns)
		{
			if(pch__vac_sns->Check__DATA(trg__vac_sts) < 0)			check__vac_sns = -1;
		}

		if((cur__press >= cfg__press)
		&& (check__atm_sns > 0)
		&& (check__vac_sns > 0))
		{
			CString str_log;
			CString str_bff;

			str_log = "Complete ... \n";

			str_bff.Format(" * current [%f] > atm config [%f] \n", cur__press,cfg__press);
			str_log += str_bff;

			if(active__atm_sns)
			{
				str_bff.Format(" * %s <- %s \n", 
					           pch__atm_sns->Get__CHANNEL_NAME(),
							   pch__atm_sns->Get__STRING());
				str_log += str_bff;
			}
			if(active__vac_sns)
			{
				str_bff.Format(" * %s <- %s \n", 
							   pch__vac_sns->Get__CHANNEL_NAME(),
							   pch__vac_sns->Get__STRING());
				str_log += str_bff;
			}

			Fnc__LOG(str_log);
			return 1;
		}

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			return -21;
		}

		// ...
		double cfg__timeout = pch__cfg_timeout->Get__VALUE();

		if(x_timer->Get__CURRENT_TIME() >= cfg__timeout)
		{
			Fnc__VENT_ALL_VLV__CLOSE(p_alarm);

			// ...
			int alm_id = alm_id__venting_timeout;
			CString alm_msg;
			CString alm_bff;
			CString r_act;

			alm_msg.Format("The current pressure is (%.3f), The atm config pressure is %.3f.\n",
						   cur__press, cfg__press);
			alm_msg += "Sensor State ... \n";

			alm_bff.Format(" * %s <- %s \n", 
							pch__atm_sns->Get__CHANNEL_NAME(),
							pch__atm_sns->Get__STRING());
			alm_msg += alm_bff;

			alm_bff.Format(" * %s <- %s \n",
							pch__vac_sns->Get__CHANNEL_NAME(),
							pch__vac_sns->Get__STRING());
			alm_msg += alm_bff;

			CString ch_data = dCH__TMC_CHM_PRESSURE_STATUS->Get__STRING();
			alm_bff.Format(" * Pressure Status is %s. \n", ch_data);
			alm_msg += alm_bff;

			Fnc__LOG(alm_msg);

			p_alarm->Popup__ALARM_With_MESSAGE(alm_id,alm_msg,r_act);

			if(r_act.CompareNoCase(ACT__RETRY) == 0)
			{
				goto LOOP_RETRY;
			}
			return -22;
		}
	}

	return -23;
}
int  CObj__CHM_STD
::_Sub__VENT(CII_OBJECT__VARIABLE* p_variable,
			 CII_OBJECT__ALARM* p_alarm)
{
	SCX__TIMER_CTRL x_timer;
	x_timer->REGISTER__ABORT_OBJECT(sObject_Name);

	// Equalize-Valve <<- "Open" ...
	if(bActive__ATM_EQUAL_VLV)
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
	if(bActive__ATM_EQUAL_VLV)
	{
		double cfg_sec = aCH__CFG_EQUALIZE_VENT_TIME->Get__VALUE();

		if(x_timer->WAIT(cfg_sec) < 0)
		{
			return OBJ_ABORT;
		}
	}

	// Venting 후 압력 Limit Check ...
	{
		double cfg__press = aCH__CFG_ATM_HIGH_PRESSURE_TORR->Get__VALUE();

		if(cfg__press > 700)
		{
			double cur__press = aiEXT_CH__CHM_PRESSURE_TORR->Get__VALUE();

			if(cur__press > cfg__press)
			{
				int alm_id = ALID__ATM_HIGH_PRESSURE_LIMIT;
				CString alm_msg;
				CString alm_bff;

				alm_msg = "After venting complete, \n";

				alm_bff.Format(" * current pressure <- %.1f (torr) \n", cur__press);
				alm_msg += alm_bff;

				alm_msg.Format(" * config pressure <- %.1f (torr) \n", cfg__press);
				alm_msg += alm_bff;

				p_alarm->Post__ALARM_With_MESSAGE(alm_id, alm_msg);
				return -31;
			}
		}
	}

	return 1;
}


// AUTO CTRL ---------------
// PUMP VLV CLOSE -----
int  CObj__CHM_STD
::Call__AUTO_PUMP_VLV_CLOSE(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm)
{
	if(iActive__SIM_MODE > 0)
	{
		return Fnc__SIM_AUTO_PUMP_VLV_CLOSE(p_variable,p_alarm);
	}

	return Fnc__AUTO_PUMP_VLV_CLOSE(p_variable,p_alarm);
}

int  CObj__CHM_STD
::Fnc__SIM_AUTO_PUMP_VLV_CLOSE(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm)
{
	Fnc__PUMP_ALL_VLV__CLOSE(p_variable, p_alarm);
	return 1;
}
int  CObj__CHM_STD
::Fnc__AUTO_PUMP_VLV_CLOSE(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm)
{
	Fnc__PUMP_ALL_VLV__CLOSE(p_variable, p_alarm);
	return 1;
}
