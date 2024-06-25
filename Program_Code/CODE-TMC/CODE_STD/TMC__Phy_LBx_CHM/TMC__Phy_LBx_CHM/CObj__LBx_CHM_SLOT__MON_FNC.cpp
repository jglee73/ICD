#include "StdAfx.h"
#include "CObj__LBx_CHM_SLOT.h"
#include "CObj__LBx_CHM_SLOT__DEF.h"
#include "CObj__LBx_CHM_SLOT__ALID.h"


// ...
void CObj__LBx_CHM_SLOT
::Mon__IO_MONITOR(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	CString str__atm_sns;
	CString str__vac_sns;
	CString var__data;

	CString str__open_sns;
	CString str__close_sns;

	if(iActive__SIM_MODE > 0)
	{
		for(int i=0; i<iLBx_SLOT_SIZE; i++)
		{
			// SV ...
			{
				doEXT_CH__LLx__SV_OPEN_X[i]->Set__DATA(STR__OFF);
				doEXT_CH__LLx__SV_CLOSE_X[i]->Set__DATA(STR__ON);

				if(bActive_DO__LLx_SV_UP_X[i])		doEXT_CH__LLx_SV_UP_X[i]->Set__DATA(STR__ON);
				if(bActive_DO__LLx_SV_DOWN_X[i])	doEXT_CH__LLx_SV_DOWN_X[i]->Set__DATA(STR__OFF);
			}

			// DV ...
			{
				doEXT_CH__LLx__DV_OPEN_X[i]->Set__DATA(STR__OFF);
				doEXT_CH__LLx__DV_CLOSE_X[i]->Set__DATA(STR__ON);
			}
		}

		// LIFT_PIN ...
		if(bActive__LIFT_PIN)
		{
			diEXT_CH__LBx__LIFT_PIN_UP->Set__DATA(STR__OFF);
			diEXT_CH__LBx__LIFT_PIN_DOWN->Set__DATA(STR__ON);
		}
	}


	while(1)
	{
		p_variable->Wait__SINGLE_OBJECT(0.01);


		if(iActive__SIM_MODE > 0)
		{
			if(doEXT_CH__SOFT_PUMP_VLV__SET->Check__DATA(STR__CLOSE) > 0)
			{
				if(bActive__DI_SR_VLV_OPEN)			diEXT_CH__DI_SR_VLV_OPEN->Set__DATA(STR__OFF);
				if(bActive__DI_SR_VLV_CLOSE)		diEXT_CH__DI_SR_VLV_CLOSE->Set__DATA(STR__ON);
			}
			else
			{
				if(bActive__DI_SR_VLV_OPEN)			diEXT_CH__DI_SR_VLV_OPEN->Set__DATA(STR__ON);
				if(bActive__DI_SR_VLV_CLOSE)		diEXT_CH__DI_SR_VLV_CLOSE->Set__DATA(STR__OFF);
			}

			if(doEXT_CH__FAST_PUMP_VLV__SET->Check__DATA(STR__CLOSE) > 0)
			{
				if(bActive__DI_FR_VLV_OPEN)			diEXT_CH__DI_FR_VLV_OPEN->Set__DATA(STR__OFF);
				if(bActive__DI_FR_VLV_CLOSE)		diEXT_CH__DI_FR_VLV_CLOSE->Set__DATA(STR__ON);
			}
			else
			{
				if(bActive__DI_FR_VLV_OPEN)			diEXT_CH__DI_FR_VLV_OPEN->Set__DATA(STR__ON);
				if(bActive__DI_FR_VLV_CLOSE)		diEXT_CH__DI_FR_VLV_CLOSE->Set__DATA(STR__OFF);
			}
		}

		// PRESSURE ...
		{
			aiEXT_CH__LBx__PRESSURE_TORR->Get__DATA(var__data);
			aCH__PRESSURE_TORR->Set__DATA(var__data);
		}

		if(bActive__ATM_SNS_Virtual_Type)
		{
			aiEXT_CH__LBx__PRESSURE_TORR->Get__DATA(var__data);
			double cur_press = atof(var__data);

			aCH__CFG_FAST_VENT_PRESSURE_TORR->Get__DATA(var__data);
			double cfg_press = atof(var__data);

			if(cur_press < cfg_press)
			{
				diEXT_CH__LLx_ATM_SNS->Set__DATA(sDATA__ATM_OFF);
			}
			else
			{
				diEXT_CH__LLx_ATM_SNS->Set__DATA(sDATA__ATM_ON);
			}
		}
		if(bActive__VAC_SNS_Virtual_Type)
		{
			aiEXT_CH__LBx__PRESSURE_TORR->Get__DATA(var__data);
			double cur_press = atof(var__data);

			aCH__CFG_FAST_PUMP_PRESSURE_TORR->Get__DATA(var__data);
			double cfg_press = atof(var__data);

			if(cur_press > cfg_press)
			{
				diEXT_CH__LLx_VAC_SNS->Set__DATA(sDATA__VAC_OFF);
			}
			else
			{
				diEXT_CH__LLx_VAC_SNS->Set__DATA(sDATA__VAC_ON);
			}
		}

		Update__PRESSURE_STS(p_variable,  p_alarm);

		// ...
		{
			if(diEXT_CH__LLx_ATM_SNS->Check__DATA(sDATA__ATM_ON) > 0)
			{
				dCH__VAC_SNS->Set__DATA(STR__OFF);
			}
			else
			{
				dCH__VAC_SNS->Set__DATA(STR__ON);
			}
		}

		// PUMP VLV FLAG ...
		Update__PUMP_VLV_STS(p_variable, p_alarm);

		// SV STATUS ...
		Update__SV_STS(p_variable, p_alarm);

		// DV STATUS ...
		Update__DV_STS(p_variable, p_alarm);

		// LIFT_PIN STATUS ...
		Update__LIFT_PIN_STS(p_variable, p_alarm);

		// INTERLOCK CHECK ...
		Fnc__INTERLOCK(p_variable, p_alarm);
	}	
}

void CObj__LBx_CHM_SLOT
::Update__PRESSURE_STS(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	double cur_press;
	double ref_atm_press;
	double ref_vac_press;
	double ref_vac_upper_tole_press;
	double tolerance_atm_press;

	double atm_range_min;
	double atm_range_max;

	double ref_vac_max;
	
	CString var__data;

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

	if(diEXT_CH__LLx_ATM_SNS->Check__DATA(sDATA__ATM_ON) > 0)		atm_sns = 1;
	if(diEXT_CH__LLx_VAC_SNS->Check__DATA(sDATA__VAC_ON) > 0)		vac_sns = 1;

	if((atm_range_min <= cur_press)
	&& (atm_sns > 0)
	&& (vac_sns < 0))
	{
		dCH__PRESSURE_STATUS->Set__DATA("ATM");
	}
	else if((cur_press <= ref_vac_max)
		 && (atm_sns < 0)
		 && (vac_sns > 0))
	{
		dCH__PRESSURE_STATUS->Set__DATA("VAC");
	}
	else
	{
		dCH__PRESSURE_STATUS->Set__DATA("BTW");
	}
}

void CObj__LBx_CHM_SLOT
::Fnc__INTERLOCK(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	int k;

	for(k=0; k<CFG_LBx__SLOT_SIZE; k++)
	{
		if(sCH__TAS_RESET_REQ_SLOT[k]->Check__DATA("") < 0)
		{
			sCH__TAS_RESET_REQ_SLOT[k]->Set__DATA("");

			for(int i=0; i<CFG_ACT__SIZE; i++)
			{
				sCH__TAS_ACTION_TIME_NOW_SLOT[i][k]->Set__DATA("");
				sCH__TAS_ACTION_TIME_MIN_SLOT[i][k]->Set__DATA("");
				sCH__TAS_ACTION_TIME_MAX_SLOT[i][k]->Set__DATA("");
				sCH__TAS_ACTION_TIME_AVG_SLOT[i][k]->Set__DATA("");
				sCH__TAS_ACTION_TIME_AVG_F_SLOT[i][k]->Set__DATA("");
				aCH__TAS_ACTION_TIME_CNT_SLOT[i][k]->Set__VALUE(1.0);
			}
		}
	}

	// ...
	{
		bool active__door_open = false;

		for(int i=0; i<iLBx_SLOT_SIZE; i++)
		{
			if(dCH__DOOR_VALVE_STATUS_X[i]->Check__DATA(STR__CLOSE) > 0)
				continue;

			active__door_open = true;
			break;
		}

		if(active__door_open)
		{
			if(Check__PUMP_ALL_VLV__CLOSE(p_variable, p_alarm) < 0)
			{
				Fnc__PUMP_ALL_VLV__CLOSE(p_variable, p_alarm);

				// ...
				int alarm_id = ALID__DOOR_VALVE__PUMP_INTERLOCK;
				CString r_act;

				p_alarm->Check__ALARM(alarm_id,r_act);
				p_alarm->Post__ALARM(alarm_id);
			}
		}
	}

	if(Check__VENT_ALL_VLV__CLOSE(p_alarm) < 0)
	{			
		if(Check__PUMP_ALL_VLV__CLOSE(p_variable, p_alarm) < 0)
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
		if(Check__PUMP_ALL_VLV__CLOSE(p_variable, p_alarm) < 0)
		{
			Fnc__PUMP_ALL_VLV__CLOSE(p_variable, p_alarm);

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

void CObj__LBx_CHM_SLOT
::Update__SV_STS(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	bool active__io_off_mode = false;
	if(dCH__CFG_IO_OFF_MODE->Check__DATA(STR__ENABLE) > 0)			active__io_off_mode = true;

	if(iActive__SIM_MODE > 0)
	{
		CString str__open_sns;
		CString str__close_sns;

		for(int i=0; i<iLBx_SLOT_SIZE; i++)
		{
			str__open_sns  = doEXT_CH__LLx__SV_OPEN_X[i]->Get__STRING();
			str__close_sns = doEXT_CH__LLx__SV_CLOSE_X[i]->Get__STRING();

			if((bActive_DO__LLx_SV_UP_X[i])
			|| (bActive_DO__LLx_SV_DOWN_X[i]))
			{
				CString str__up_sns;
				CString str__down_sns;

				if(bActive_DO__LLx_SV_UP_X[i])		str__up_sns = doEXT_CH__LLx_SV_UP_X[i]->Get__STRING();
				if(bActive_DO__LLx_SV_DOWN_X[i])	str__down_sns = doEXT_CH__LLx_SV_DOWN_X[i]->Get__STRING();

				if(bActive_DI__LLx_SV_UP_X[i])		diEXT_CH__LLx_SV_UP_X[i]->Set__DATA(str__up_sns);
				if(bActive_DO__LLx_SV_DOWN_X[i])	diEXT_CH__LLx_SV_DOWN_X[i]->Set__DATA(str__down_sns);
			}
			else
			{
				if(active__io_off_mode)
				{
					if((str__open_sns.CompareNoCase(STR__OFF)  == 0)
					&& (str__close_sns.CompareNoCase(STR__OFF) == 0))
					{
						continue;
					}
				}
			}

			diEXT_CH__LLx__SV_OPEN_X[i]->Set__DATA(str__open_sns);
			diEXT_CH__LLx__SV_CLOSE_X[i]->Set__DATA(str__close_sns);
		}
	}

	// ...
	{
		CString str__open_sns;
		CString str__close_sns;

		for(int i=0; i<iLBx_SLOT_SIZE; i++)
		{
			str__open_sns = diEXT_CH__LLx__SV_OPEN_X[i]->Get__STRING();
			str__close_sns = diEXT_CH__LLx__SV_CLOSE_X[i]->Get__STRING();

			// ...
			CString str__sv_sts = STR__UNKNOWN;

			if((str__open_sns.CompareNoCase(STR__ON)   == 0)
			&& (str__close_sns.CompareNoCase(STR__OFF) == 0))
			{
				bool active__down_check = true;

				if(bActive_DI__LLx_SV_UP_X[i])
				{
					if(diEXT_CH__LLx_SV_UP_X[i]->Check__DATA(STR__OFF) < 0)
						active__down_check = false;
				}
				if(bActive_DO__LLx_SV_DOWN_X[i])
				{
					if(diEXT_CH__LLx_SV_DOWN_X[i]->Check__DATA(STR__ON) < 0)
						active__down_check = false;
				}

				if(active__down_check)		str__sv_sts = STR__OPEN;
			}
			else if((str__open_sns.CompareNoCase(STR__OFF) == 0)
				 && (str__close_sns.CompareNoCase(STR__ON) == 0))
			{
				bool active__up_check = true;

				if(bActive_DI__LLx_SV_UP_X[i])
				{
					if(diEXT_CH__LLx_SV_UP_X[i]->Check__DATA(STR__ON) < 0)
						active__up_check = false;
				}
				if(bActive_DO__LLx_SV_DOWN_X[i])
				{
					if(diEXT_CH__LLx_SV_DOWN_X[i]->Check__DATA(STR__OFF) < 0)
						active__up_check = false;
				}

				if(active__up_check)		str__sv_sts = STR__CLOSE;
			}

			dCH__SLIT_VALVE_STATUS_X[i]->Set__DATA(str__sv_sts);
		}
	}

	if(iLBx_SLOT_SIZE > 0)
	{
		CString ch_data = dCH__SLIT_VALVE_STATUS_X[0]->Get__STRING();
		dCH__SLIT_VALVE_STATUS->Set__DATA(ch_data);
	}
}
void CObj__LBx_CHM_SLOT
::Update__DV_STS(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm)
{
	bool active__io_off_mode = false;
	if(dCH__CFG_IO_OFF_MODE->Check__DATA(STR__ENABLE) > 0)			active__io_off_mode = true;

	if(iActive__SIM_MODE > 0)
	{
		CString str__open_sns;
		CString str__close_sns;

		for(int i=0; i<iLBx_SLOT_SIZE; i++)
		{
			str__open_sns  = doEXT_CH__LLx__DV_OPEN_X[i]->Get__STRING();
			str__close_sns = doEXT_CH__LLx__DV_CLOSE_X[i]->Get__STRING();

			if(active__io_off_mode)
			{
				if((str__open_sns.CompareNoCase(STR__OFF)  == 0)
				&& (str__close_sns.CompareNoCase(STR__OFF) == 0))
				{
					continue;
				}
			}

			diEXT_CH__LLx__DV_OPEN_X[i]->Set__DATA(str__open_sns);
			diEXT_CH__LLx__DV_CLOSE_X[i]->Set__DATA(str__close_sns);
		}
	}

	// ...
	{
		CString str__open_sns;
		CString str__close_sns;

		for(int i=0; i<iLBx_SLOT_SIZE; i++)
		{
			diEXT_CH__LLx__DV_OPEN_X[i]->Get__DATA(str__open_sns);
			diEXT_CH__LLx__DV_CLOSE_X[i]->Get__DATA(str__close_sns);

			if((str__open_sns.CompareNoCase(STR__ON)   == 0)
			&& (str__close_sns.CompareNoCase(STR__OFF) == 0))
			{
				dCH__DOOR_VALVE_STATUS_X[i]->Set__DATA(STR__OPEN);
			}
			else if((str__open_sns.CompareNoCase(STR__OFF) == 0)
				 && (str__close_sns.CompareNoCase(STR__ON) == 0))
			{
				dCH__DOOR_VALVE_STATUS_X[i]->Set__DATA(STR__CLOSE);
			}
			else
			{
				dCH__DOOR_VALVE_STATUS_X[i]->Set__DATA(STR__UNKNOWN);
			}
		}
	}

	if(iLBx_SLOT_SIZE > 0)
	{
		CString ch_data = dCH__DOOR_VALVE_STATUS_X[0]->Get__STRING();
		dCH__DOOR_VALVE_STATUS->Set__DATA(ch_data);
	}
}
void CObj__LBx_CHM_SLOT
::Update__LIFT_PIN_STS(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm)
{
	if(!bActive__LIFT_PIN)
	{
		return;
	}

	if(iActive__SIM_MODE > 0)
	{
		CString ch_data;

		doEXT_CH__LBx__LIFT_PIN_UP->Get__DATA(ch_data);
		diEXT_CH__LBx__LIFT_PIN_UP->Set__DATA(ch_data);

		doEXT_CH__LBx__LIFT_PIN_DOWN->Get__DATA(ch_data);
		diEXT_CH__LBx__LIFT_PIN_DOWN->Set__DATA(ch_data);
	}

	// ...
	CString str__up_sns;
	CString str__down_sns;

	diEXT_CH__LBx__LIFT_PIN_UP->Get__DATA(str__up_sns);
	diEXT_CH__LBx__LIFT_PIN_DOWN->Get__DATA(str__down_sns);

	if((str__up_sns.CompareNoCase(STR__ON)    == 0)
	&& (str__down_sns.CompareNoCase(STR__OFF) == 0))
	{
		dCH__LIFT_PIN_STATUS->Set__DATA(STR__UP);
	}
	else if((str__up_sns.CompareNoCase(STR__OFF)  == 0)
		 && (str__down_sns.CompareNoCase(STR__ON) == 0))
	{
		dCH__LIFT_PIN_STATUS->Set__DATA(STR__DOWN);
	}
	else
	{
		dCH__LIFT_PIN_STATUS->Set__DATA(STR__UNKNOWN);
	}
}
