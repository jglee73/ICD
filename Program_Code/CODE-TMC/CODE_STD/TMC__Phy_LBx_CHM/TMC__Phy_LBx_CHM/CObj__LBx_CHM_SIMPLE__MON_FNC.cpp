#include "StdAfx.h"
#include "CObj__LBx_CHM_SIMPLE.h"
#include "CObj__LBx_CHM_SIMPLE__DEF.h"
#include "CObj__LBx_CHM_SIMPLE__ALID.h"


// ...
void CObj__LBx_CHM_SIMPLE
::Mon__IO_MONITOR(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	CString str__atm_sns;
	CString str__vac_sns;
	CString var__data;

	CString str__open_sns;
	CString str__close_sns;

	if(iSim_Flag > 0)
	{
		if(bActive__LIFT_PIN)
		{
			// SLIT VALVE ...
			{
				diEXT_CH__LBx__SV_OPEN->Set__DATA(STR__OFF);
				diEXT_CH__LBx__SV_CLOSE->Set__DATA(STR__ON);
			}
			// DOOR VALVE ...
			{
				diEXT_CH__LBx__DV_OPEN->Set__DATA(STR__OFF);
				diEXT_CH__LBx__DV_CLOSE->Set__DATA(STR__ON);
			}
			// LIFT_PIN VALVE ...
			{
				diEXT_CH__LBx__LIFT_PIN_UP->Set__DATA(STR__OFF);
				diEXT_CH__LBx__LIFT_PIN_DOWN->Set__DATA(STR__ON);
			}
		}
	}

	while(1)
	{
		p_variable->Wait__SINGLE_OBJECT(0.01);

		// PRESSURE ...
		aiEXT_CH__LBx__PRESSURE_TORR->Get__DATA(var__data);
		aCH__PRESSURE_TORR->Set__DATA(var__data);

		if(bActive__ATM_SNS_Virtual_Type)
		{
			aiEXT_CH__LBx__PRESSURE_TORR->Get__DATA(var__data);
			double cur_press = atof(var__data);

			aCH__CFG_FAST_VENT_PRESSURE_TORR->Get__DATA(var__data);
			double cfg_press = atof(var__data);
			
			if(cur_press < cfg_press)
			{
				diEXT_CH__LBx__ATM_SENSOR->Set__DATA(sDATA__ATM_OFF);
			}
			else
			{
				diEXT_CH__LBx__ATM_SENSOR->Set__DATA(sDATA__ATM_ON);
			}
		}

		// ...
		{
			double cur_press;
			double ref_atm_press;
			double ref_vac_press;
			double ref_vac_upper_tole_press;
			double tolerance_atm_press;

			double atm_range_min;
			double atm_range_max;

			double ref_vac_max;

			// Get Pressure Value
			aCH__PRESSURE_TORR->Get__DATA(var__data);
			cur_press = atof(var__data);

			aCH__CFG_ATM_PRESSURE_TORR->Get__DATA(var__data);
			ref_atm_press = atof(var__data);

			// Get Atm Tolerance...
			aCH__CFG_ATM_PRESS_STS_TOLERANCE->Get__DATA(var__data);
			tolerance_atm_press = atof(var__data);

			atm_range_min = ref_atm_press - tolerance_atm_press;
			atm_range_max = ref_atm_press + tolerance_atm_press;

			// 1. VAC
			aCH__CFG_VAC_PRESSURE_TORR->Get__DATA(var__data);
			ref_vac_press = atof(var__data);

			// 2. VAC
			aCH__CFG_VAC_UPPER_TOLERANCE->Get__DATA(var__data);
			ref_vac_upper_tole_press = atof(var__data)*0.001;	// mtorr
			ref_vac_max = ref_vac_press + ref_vac_upper_tole_press;

			// 3.
			int vac_sns = -1;
			int atm_sns = -1;
			
			if(diEXT_CH__LBx__ATM_SENSOR->Check__DATA(sDATA__ATM_ON) > 0)		atm_sns = 1;
			if(diEXT_CH__LBx__VAC_SENSOR->Check__DATA(sDATA__VAC_ON) > 0)		vac_sns = 1;

			if((atm_range_min <= cur_press)
			&& (atm_sns > 0)
			&& (vac_sns < 0))
			{
				dCH__PRESSURE_STATUS->Set__DATA(STR__ATM);
			}
			else if((cur_press > 0)					// 0.0 이면 Gauge Offline 가능성 큼.. 
				 &&	(cur_press <= ref_vac_max)
				 && (atm_sns < 0)
				 && (vac_sns > 0))
			{
				dCH__PRESSURE_STATUS->Set__DATA(STR__VAC);
			}
			else
			{
				dCH__PRESSURE_STATUS->Set__DATA(STR__BTW);
			}
		}

		// ...
		{
			if(diEXT_CH__LBx__ATM_SENSOR->Check__DATA(sDATA__ATM_ON) > 0)
			{
				dCH__VAC_SNS->Set__DATA(STR__OFF);
			}
			else
			{
				dCH__VAC_SNS->Set__DATA(STR__ON);
			}
		}

		// PUMP VLV FLAG ...
		Update__PUMP_VLV_STS(p_alarm);

		// SV STATUS ...
		Update__SV_STS(p_variable,p_alarm);
		
		// DV STATUS ...
		Update__DV_STS(p_variable,p_alarm);
		
		// LIFT_PIN STATUS ...
		Update__LIFT_PIN_STS(p_variable,p_alarm);

		// INTERLOCK CHECK ...
		Fnc__INTERLOCK(p_variable, p_alarm);
	}	
}

void CObj__LBx_CHM_SIMPLE
::Fnc__INTERLOCK(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	if(sCH__TAS_RESET_REQ->Check__DATA("") < 0)
	{
		sCH__TAS_RESET_REQ->Set__DATA("");

		// ...
		int i_limit = CFG_ACT__SIZE;
		int i;

		for(i=0; i<i_limit; i++)
		{
			sCH__TAS_ACTION_TIME_NOW[i]->Set__DATA("");
			sCH__TAS_ACTION_TIME_MIN[i]->Set__DATA("");
			sCH__TAS_ACTION_TIME_MAX[i]->Set__DATA("");
			sCH__TAS_ACTION_TIME_AVG[i]->Set__DATA("");
			sCH__TAS_ACTION_TIME_AVG_F[i]->Set__DATA("");
			aCH__TAS_ACTION_TIME_CNT[i]->Set__VALUE(1.0);
		}
	}

	if(dCH__DOOR_VALVE_STATUS->Check__DATA(STR__CLOSE) < 0)
	{
		if(Check__PUMP_ALL_VLV__CLOSE(p_alarm) < 0)
		{
			Fnc__PUMP_ALL_VLV__CLOSE(p_alarm);

			// ...
			{
				int alarm_id = ALID__DOOR_VALVE__PUMP_INTERLOCK;
				CString r_act;

				p_alarm->Check__ALARM(alarm_id,r_act);
				p_alarm->Post__ALARM(alarm_id);
			}
		}
	}
	if(Check__VENT_ALL_VLV__CLOSE(p_alarm) < 0)
	{			
		if(Check__PUMP_ALL_VLV__CLOSE(p_alarm) < 0)
		{
			if(sCH__OUTPROC_ACTIVE_FLAG->Check__DATA(STR__YES) < 0)
			{
				Fnc__VENT_ALL_VLV__CLOSE(p_alarm);

				// ...
				{
					int alarm_id = ALID__VENT_VALVE__PUMP_INTERLOCK;
					CString r_act;

					p_alarm->Check__ALARM(alarm_id,r_act);
					p_alarm->Post__ALARM(alarm_id);
				}
			}
		}
	}		

	if((sEXT_CH__MON_PUMP_COMM_STS->Check__DATA(STR__ONLINE) < 0) 
	|| (sEXT_CH__MON_PUMP_RUN_STS->Check__DATA(STR__ON) < 0))
	{
		if(Check__PUMP_ALL_VLV__CLOSE(p_alarm) < 0)
		{
			Fnc__PUMP_ALL_VLV__CLOSE(p_alarm);

			// ...
			{
				int alarm_id = ALID__PUMP_OFFLINE_OFF_INTERLOCK;
				CString r_act;

				p_alarm->Check__ALARM(alarm_id,r_act);
				p_alarm->Post__ALARM(alarm_id);
			}
		}
	}

	// GAUGE VLV ...
	if(bAtive__GAUGE_VLV)
	{
		double cur_press = aCH__PRESSURE_TORR->Get__VALUE();
		double ref_press = aCH__CFG_GAUGE_LIMIT_PRESSURE->Get__VALUE();

		if(cur_press < ref_press)		doEXT_CH__GAUGE_VLV->Set__DATA(STR__OPEN);
		else							doEXT_CH__GAUGE_VLV->Set__DATA(STR__CLOSE);
	}
}

void CObj__LBx_CHM_SIMPLE
::Update__SV_STS(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	CString str__open_sns;
	CString str__close_sns;

	// SV OP/CL
	{
		diEXT_CH__LBx__SV_OPEN->Get__DATA(str__open_sns);
		diEXT_CH__LBx__SV_CLOSE->Get__DATA(str__close_sns);
	}

	if((str__open_sns.CompareNoCase(STR__ON)   == 0)
	&& (str__close_sns.CompareNoCase(STR__OFF) == 0))
	{
		dCH__SLIT_VALVE_STATUS->Set__DATA(STR__OPEN);
	}
	else if((str__open_sns.CompareNoCase(STR__OFF) == 0)
		 && (str__close_sns.CompareNoCase(STR__ON) == 0))
	{
		dCH__SLIT_VALVE_STATUS->Set__DATA(STR__CLOSE);
	}
	else
	{
		dCH__SLIT_VALVE_STATUS->Set__DATA(STR__UNKNOWN);
	}
}
void CObj__LBx_CHM_SIMPLE
::Update__DV_STS(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm)
{
	CString str__open_sns;
	CString str__close_sns;

	// DV 
	diEXT_CH__LBx__DV_OPEN->Get__DATA(str__open_sns);
	diEXT_CH__LBx__DV_CLOSE->Get__DATA(str__close_sns);

	if((str__open_sns.CompareNoCase(STR__ON)   == 0)
	&& (str__close_sns.CompareNoCase(STR__OFF) == 0))
	{
		dCH__DOOR_VALVE_STATUS->Set__DATA(STR__OPEN);
	}
	else if((str__open_sns.CompareNoCase(STR__OFF) == 0)
		 && (str__close_sns.CompareNoCase(STR__ON) == 0))
	{
		dCH__DOOR_VALVE_STATUS->Set__DATA(STR__CLOSE);
	}
	else
	{
		dCH__DOOR_VALVE_STATUS->Set__DATA(STR__UNKNOWN);
	}
}
void CObj__LBx_CHM_SIMPLE
::Update__LIFT_PIN_STS(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm)
{
	if(iSim_Flag > 0)
	{
		if(bActive__LIFT_PIN)
		{
			CString ch_data;

			ch_data = doEXT_CH__LBx__LIFT_PIN_UP->Get__STRING();
			diEXT_CH__LBx__LIFT_PIN_UP->Set__DATA(ch_data);
	
			ch_data = doEXT_CH__LBx__LIFT_PIN_DOWN->Get__STRING();
			diEXT_CH__LBx__LIFT_PIN_DOWN->Set__DATA(ch_data);
		}
	}

	if(bActive__LIFT_PIN)
	{
		CString str__up_sns = diEXT_CH__LBx__LIFT_PIN_UP->Get__STRING();
		CString str__dw_sns = diEXT_CH__LBx__LIFT_PIN_DOWN->Get__STRING();

		if((str__up_sns.CompareNoCase(STR__ON)  == 0)
		&& (str__dw_sns.CompareNoCase(STR__OFF) == 0))
		{
			dCH__LIFT_PIN_STATUS->Set__DATA(STR__UP);
		}
		else if((str__up_sns.CompareNoCase(STR__OFF) == 0)
			 && (str__dw_sns.CompareNoCase(STR__ON)  == 0))
		{
			dCH__LIFT_PIN_STATUS->Set__DATA(STR__DOWN);
		}
		else
		{
			dCH__LIFT_PIN_STATUS->Set__DATA(STR__UNKNOWN);
		}
	}
}

