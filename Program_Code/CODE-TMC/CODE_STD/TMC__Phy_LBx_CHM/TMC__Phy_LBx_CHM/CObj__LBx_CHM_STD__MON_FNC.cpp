#include "StdAfx.h"
#include "CObj__LBx_CHM_STD.h"
#include "CObj__LBx_CHM_STD__DEF.h"
#include "CObj__LBx_CHM_STD__ALID.h"


// ...
void CObj__LBx_CHM_STD
::Mon__IO_MONITOR(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	CString str__atm_sns;
	CString str__vac_sns;
	CString var__data;

	while(1)
	{
		p_variable->Wait__SINGLE_OBJECT(0.01);

		// PRESSURE ...
		{
			aiEXT_CH__LBx__PRESSURE_TORR->Get__DATA(var__data);
			aCH__PRESSURE_TORR->Set__DATA(var__data);
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

			if(atm_range_min <= cur_press)
			{
				dCH__PRESSURE_STATUS->Set__DATA("ATM");
			}
			else if( (cur_press > 0)					// 0.0 이면 Gauge Offline 가능성 큼.. 
				 &&	 (cur_press <= ref_vac_max) )
			{
				dCH__PRESSURE_STATUS->Set__DATA("VAC");
			}
			else
			{
				dCH__PRESSURE_STATUS->Set__DATA("BTW");
			}
		}

		// PUMP VLV FLAG -----
		Update__PUMP_VLV_STS(p_alarm);
	}	
}

void CObj__LBx_CHM_STD
::Mon__INTERLOCK(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	while(1)
	{
		Sleep(9);

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

		// ...
	}
}

void CObj__LBx_CHM_STD
::Mon__PUMP_INTERLOCK(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	int alarm_id = ALID__PUMP_OFFLINE_OFF_INTERLOCK;

	CString r_act;
	CString var_data;

	while(1)
	{
		Sleep(9);

		p_alarm->Check__ALARM(alarm_id,r_act);
		if(iSim_Flag > 0)	continue;

		if((sEXT_CH__MON_PUMP_COMM_STS->Check__DATA(STR__ONLINE) < 0) 
		|| (sEXT_CH__MON_PUMP_RUN_STS->Check__DATA(STR__ON) < 0))
		{
			if(Check__PUMP_ALL_VLV__CLOSE(p_alarm) < 0)
			{
				Fnc__PUMP_ALL_VLV__CLOSE(p_alarm);

				p_alarm->Check__ALARM(alarm_id,r_act);
				p_alarm->Post__ALARM(alarm_id);
			}
		}

		// ...
	}
}

void CObj__LBx_CHM_STD
::Mon__SV_DV_OP_CL_STS(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	CString str__open_sns;
	CString str__close_sns;

	CString str__up_sns;
	CString str__down_sns;

	if(iSim_Flag > 0)
	{
		// SLIT VALVE ...
		{
			// SV OP/CL ...
			diEXT_CH__LBx__SV_TM_OPEN->Set__DATA(STR__OFF);
			diEXT_CH__LBx__SV_TM_CLOSE->Set__DATA(STR__ON);

			// SV UP/DN ...
			diEXT_CH__LBx__SV_UP->Set__DATA(STR__ON);
			diEXT_CH__LBx__SV_DOWN->Set__DATA(STR__OFF);
		}

		// DOOR VALVE ...
		{
			// DV ...
			diEXT_CH__LBx__DV_OPEN->Set__DATA(STR__OFF);
			diEXT_CH__LBx__DV_CLOSE->Set__DATA(STR__ON);
		}
	}

	while(1)
	{
		Sleep(9);


		// SV STATUS ...
		{
			Update__SV_STS(p_variable,p_alarm);
		}
		// DV STATUS ...
		{
			Update__DV_STS(p_variable,p_alarm);
		}
	}

	// ...
}

void CObj__LBx_CHM_STD
::Update__SV_STS(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	CString str__open_sns;
	CString str__close_sns;

	CString str__up_sns;
	CString str__down_sns;

	// SV OP/CL
	diEXT_CH__LBx__SV_TM_OPEN->Get__DATA(str__open_sns);
	diEXT_CH__LBx__SV_TM_CLOSE->Get__DATA(str__close_sns);

	// SV UP/DN
	diEXT_CH__LBx__SV_UP->Get__DATA(str__up_sns);
	diEXT_CH__LBx__SV_DOWN->Get__DATA(str__down_sns);

	if((str__open_sns.CompareNoCase(STR__ON)   == 0)
	&& (str__close_sns.CompareNoCase(STR__OFF) == 0))
	{
		if((str__up_sns.CompareNoCase(STR__OFF)   == 0)
		&& (str__down_sns.CompareNoCase(STR__ON) == 0))
		{
			dCH__SLIT_VALVE_STATUS->Set__DATA(STR__OPEN);
		}
	}
	else if((str__open_sns.CompareNoCase(STR__OFF) == 0)
	&& (str__close_sns.CompareNoCase(STR__ON) == 0))
	{
		if((str__up_sns.CompareNoCase(STR__ON)   == 0)
		&& (str__down_sns.CompareNoCase(STR__OFF) == 0))
		{
			dCH__SLIT_VALVE_STATUS->Set__DATA(STR__CLOSE);
		}	
	}
	else
	{
		dCH__SLIT_VALVE_STATUS->Set__DATA(STR__UNKNOWN);
	}
}

void CObj__LBx_CHM_STD
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
