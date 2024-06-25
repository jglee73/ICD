#include "StdAfx.h"
#include "CObj__CHM_STD.h"
#include "CObj__CHM_STD__DEF.h"
#include "CObj__CHM_STD__ALID.h"


//----------------------------------------------------------------------------------------------------
void CObj__CHM_STD
::Mon__IO_MONITOR(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm)
{
	SCX__TIMER_CTRL cx_timer_ctrl;

	CString str__atm_sns;
	CString str__vac_sns;
	CString var__data;

	if(iActive__SIM_MODE > 0)
	{
		if(!bActive__ATM_SNS_Virtual_Type)			diEXT_CH__ATM_SENSOR->Set__DATA(sDATA__ATM_OFF);
		if(!bActive__VAC_SNS_Virtual_Type)			diEXT_CH__VAC_SENSOR->Set__DATA(sDATA__VAC_ON);

		//
		for(int i=0; i<iSIZE__CHM_GAUGE; i++)
		{
			double cur__gauge_torr  = 0.001;
			double cur__gauge_mtorr = cur__gauge_torr * 1000.0; 

			if(bActive__AI_CHM_GAUGE_TORR_X[i])			aEXT_CH__AI_CHM_GAUGE_TORR_X[i]->Set__VALUE(cur__gauge_torr);
			if(bActive__AI_CHM_GAUGE_mTORR_X[i])		aEXT_CH__AI_CHM_GAUGE_mTORR_X[i]->Set__VALUE(cur__gauge_mtorr);
		}
	}
	
	while(1)
	{
		p_variable->Wait__SINGLE_OBJECT(0.1);


		if(iActive__SIM_MODE > 0)
		{
			if(doEXT_CH__FAST_PUMP_VLV__SET->Check__DATA(STR__OPEN) > 0)
			{
				if(bActive__DI_FR_VLV_OPEN)			diEXT_CH__DI_FR_VLV_OPEN->Set__DATA(STR__ON);
				if(bActive__DI_FR_VLV_CLOSE)		diEXT_CH__DI_FR_VLV_CLOSE->Set__DATA(STR__OFF);
			}
			else
			{
				if(bActive__DI_FR_VLV_OPEN)			diEXT_CH__DI_FR_VLV_OPEN->Set__DATA(STR__OFF);
				if(bActive__DI_FR_VLV_CLOSE)		diEXT_CH__DI_FR_VLV_CLOSE->Set__DATA(STR__ON);
			}

			if(bActive__SOFT_PUMP_VLV__SET)
			{
				if(doEXT_CH__SOFT_PUMP_VLV__SET->Check__DATA(STR__OPEN) > 0)
				{
					if(bActive__DI_SR_VLV_OPEN)		diEXT_CH__DI_SR_VLV_OPEN->Set__DATA(STR__ON);
					if(bActive__DI_SR_VLV_CLOSE)	diEXT_CH__DI_SR_VLV_CLOSE->Set__DATA(STR__OFF);
				}
				else
				{
					if(bActive__DI_SR_VLV_OPEN)		diEXT_CH__DI_SR_VLV_OPEN->Set__DATA(STR__OFF);
					if(bActive__DI_SR_VLV_CLOSE)	diEXT_CH__DI_SR_VLV_CLOSE->Set__DATA(STR__ON);
				}
			}
		}

		// CHM.PRESSURE ...
		{
			double cur__chm_torr = aiEXT_CH__CHM_PRESSURE_TORR->Get__VALUE();
			double cur__press_mtorr = cur__chm_torr * 1000.0;

			for(int i=0; i<iSIZE__CHM_GAUGE; i++)
			{
				if(dEXT_CH__DO_CHM_ISO_VLV_X[i]->Check__DATA(STR__OPEN) < 0)
				{
					continue;
				}

				// ...
				double cfg__max_mtorr   = aCH__CFG_CHAMBER_MANOMETER_MAX_PRESSURE_mTORR_X[i]->Get__VALUE();
				double cur__gauge_mtorr = cfg__max_mtorr;

				if(bActive__AI_CHM_GAUGE_mTORR_X[i])
				{
					cur__gauge_mtorr = aEXT_CH__AI_CHM_GAUGE_mTORR_X[i]->Get__VALUE();
				}
				else if(bActive__AI_CHM_GAUGE_TORR_X[i])
				{
					double cur__gauge_torr = aEXT_CH__AI_CHM_GAUGE_TORR_X[i]->Get__VALUE();
					cur__gauge_mtorr = cur__gauge_torr * 1000.0;
				}

				if(cur__gauge_mtorr < cfg__max_mtorr)
				{
					cur__press_mtorr = cur__gauge_mtorr;
				}
			}

			// ...
			{
				double cur__press_torr = cur__press_mtorr * 0.001;

				var__data.Format("%.3f", cur__press_torr);
				aCH__TMC_CHM_PRESSURE_TORR->Set__DATA(var__data);

				var__data.Format("%.1f", cur__press_mtorr);
				aCH__TMC_CHM_PRESSURE_mTORR->Set__DATA(var__data);
			}

			// ...
			{
				double cur__press_torr  = aCH__TMC_CHM_PRESSURE_TORR->Get__VALUE();
				double cur__press_mtorr = aCH__TMC_CHM_PRESSURE_mTORR->Get__VALUE();

					 if(cur__press_torr <  0.01)		var__data.Format("%1.2f mtorr", cur__press_mtorr);
				else if(cur__press_torr <   0.1)		var__data.Format("%2.1f mtorr", cur__press_mtorr);
				else if(cur__press_torr <   1.0)		var__data.Format("%3.0f mtorr", cur__press_mtorr);
				else if(cur__press_torr <  10.0)		var__data.Format("%.3f torr", cur__press_torr);
				else if(cur__press_torr < 100.0)		var__data.Format("%.2f torr", cur__press_torr);
				else									var__data.Format("%.1f torr", cur__press_torr);

				sCH__MON_CHM_PRESSURE_DISPLAY->Set__DATA(var__data);
			}
		}
		
		// CHM.GAUGE_ISO_VLV ...
		{
			bool active__interlock_pressure = false;

			// ...
			{
				double cur__chm_press = aiEXT_CH__CHM_PRESSURE_TORR->Get__VALUE();
				double cfg__interlock_press = aCH__CFG_CHAMBER_MANOMETER_INTERLOCK_HIGH_LIMIT_PRESSURE->Get__VALUE();

				if(cur__chm_press >= cfg__interlock_press)		active__interlock_pressure = true;
			}

			for(int i=0; i<iSIZE__CHM_GAUGE; i++)
			{
				if(active__interlock_pressure)
				{
					dEXT_CH__DO_CHM_ISO_VLV_X[i]->Set__DATA(STR__CLOSE);
				}
				else
				{
					bool active__vlv_open = false;

					if(diEXT_CH__VAC_SENSOR->Check__DATA(sDATA__VAC_ON) > 0)
					{
						double cfg__limit_mtorr = aCH__CFG_CHAMBER_MANOMETER_LIMIT_PRESSURE_mTORR_X[i]->Get__VALUE();

						if(cfg__limit_mtorr > 0.1)
						{
							if(i > 0)
							{
								double cur__press_mtorr = 0;

								if(bActive__AI_CHM_GAUGE_mTORR_X[i-1])
								{
									cur__press_mtorr = aEXT_CH__AI_CHM_GAUGE_mTORR_X[i-1]->Get__VALUE();
								}
								else if(bActive__AI_CHM_GAUGE_TORR_X[i-1])
								{
									double cur__press_torr = aEXT_CH__AI_CHM_GAUGE_TORR_X[i-1]->Get__VALUE();
									cur__press_mtorr = cur__press_torr * 1000.0;
								}

								if(cur__press_mtorr < cfg__limit_mtorr)			active__vlv_open = true;
							}
							else
							{
								double cur__chm_torr  = aiEXT_CH__CHM_PRESSURE_TORR->Get__VALUE();
								double cur__chm_mtorr = cur__chm_torr * 1000.0;

								if(cur__chm_mtorr < cfg__limit_mtorr)			active__vlv_open = true;
							}
						}
						else
						{
							active__vlv_open = true;
						}
					}

					if(active__vlv_open)		dEXT_CH__DO_CHM_ISO_VLV_X[i]->Set__DATA(STR__OPEN);
					else						dEXT_CH__DO_CHM_ISO_VLV_X[i]->Set__DATA(STR__CLOSE);
				}
			}
		}

		if(bActive__ATM_SNS_Virtual_Type)
		{
			double cur__press_torr = aiEXT_CH__CHM_PRESSURE_TORR->Get__VALUE();
			double cfg__press_atm  = aCH__CFG_FAST_VENT_PRESSURE_TORR->Get__VALUE();

			if(cur__press_torr < cfg__press_atm)
			{
				diEXT_CH__ATM_SENSOR->Set__DATA(sDATA__ATM_OFF);
			}
			else
			{
				diEXT_CH__ATM_SENSOR->Set__DATA(sDATA__ATM_ON);
			}
		}
		if(bActive__VAC_SNS_Virtual_Type)
		{
			double cur__press_torr = aiEXT_CH__CHM_PRESSURE_TORR->Get__VALUE();
			double cfg__press_vac  = aCH__CFG_FAST_PUMP_PRESSURE_TORR->Get__VALUE();

			if(cur__press_torr > cfg__press_vac)
			{
				diEXT_CH__VAC_SENSOR->Set__DATA(sDATA__VAC_OFF);
			}
			else
			{
				diEXT_CH__VAC_SENSOR->Set__DATA(sDATA__VAC_ON);
			}
		}

		// ...
		{
			double cur_press;
			double ref_atm_press;
			double ref_vac_press;
			double atm_range_min, atm_range_max;
			double tolerance_atm_press;

			// Get Pressure Value ...
			aCH__TMC_CHM_PRESSURE_TORR->Get__DATA(var__data);
			cur_press = atof(var__data);

			aEXT_CH__CFG_REF_ATM_PRESSURE->Get__DATA(var__data);
			ref_atm_press = atof(var__data);

			aEXT_CH__CFG_REF_VAC_PRESSURE->Get__DATA(var__data);
			ref_vac_press = atof(var__data);

			// Get ATM Tolerance...
			aCH__CFG_ATM_PRESS_STS_TOLERANCE->Get__DATA(var__data);
			tolerance_atm_press = atof(var__data);

			atm_range_min = ref_atm_press - tolerance_atm_press;
			atm_range_max = ref_atm_press + tolerance_atm_press;

			// Get Sensor Value ...
			diEXT_CH__ATM_SENSOR->Get__DATA(str__atm_sns);
			diEXT_CH__VAC_SENSOR->Get__DATA(str__vac_sns);

			if((cur_press >= atm_range_min) 
			&& (str__atm_sns.CompareNoCase(sDATA__ATM_ON)  == 0) 
			&& (str__vac_sns.CompareNoCase(sDATA__VAC_OFF) == 0))
			{
				dCH__TMC_CHM_PRESSURE_STATUS->Set__DATA(STR__ATM);
			}
			else if((cur_press <= ref_vac_press)
				 && (str__atm_sns.CompareNoCase(sDATA__ATM_OFF) == 0) 
				 && (str__vac_sns.CompareNoCase(sDATA__VAC_ON)  == 0))
			{
				dCH__TMC_CHM_PRESSURE_STATUS->Set__DATA(STR__VAC);
			}
			else
			{
				dCH__TMC_CHM_PRESSURE_STATUS->Set__DATA(STR__BTW);
			}			
		}

		// ...
		{
			if(diEXT_CH__VAC_SENSOR->Check__DATA(sDATA__VAC_ON) > 0)			dCH__TMC_CMH_VAC_SNS->Set__DATA(STR__OFF);
			else																dCH__TMC_CMH_VAC_SNS->Set__DATA(STR__ON);
		}

		//
		Update__PUMP_VLV_STS(p_variable, p_alarm);

		Fnc__INTERLOCK(p_variable,p_alarm);
	}	
}

void CObj__CHM_STD
::Mon__BALLAST_CTRL(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM *p_alarm)
{
	bool active__ballast_ctrl = false;

	int loop_count = 0;
	int i;
	

	while(1)
	{
		p_variable->Wait__SINGLE_OBJECT(0.1);

		
		loop_count++;
		if(loop_count > 10)			loop_count = 1;

		if(loop_count == 1)
		{
			double cfg_min = aCH__CFG_TM_BALLAST_N2_PRESSURE_MIN->Get__VALUE();
			double cfg_max = aCH__CFG_TM_BALLAST_N2_PRESSURE_MAX->Get__VALUE();

			aCH__CFG_TM_BALLAST_N2_PRESSURE_mTORR->Change__MIN_MAX_DEC(cfg_min, cfg_max, 0);

			if(bActive__AO_BALLAST_PRESSURE_TORR)		aoEXT_CH__AO_BALLAST_PRESSURE_TORR->Change__MIN_MAX_DEC(cfg_min, cfg_max, 0);
			if(bActive__AI_BALLAST_PRESSURE_TORR)		aoEXT_CH__AI_BALLAST_PRESSURE_TORR->Change__MIN_MAX_DEC(cfg_min, cfg_max, 0);
		}

		// ...
		bool active__chm_pumping = true;

		// Check : Chamber Pumping ...
		{
			if(dCH__TMC_CHM_PRESSURE_STATUS->Check__DATA(STR__VAC) < 0)
			{
				active__chm_pumping = false;
			}
			if(sEXT_CH__MON_PUMP_RUN_STS->Check__DATA(STR__ON) < 0)
			{
				active__chm_pumping = false;
			}
			if(sCH__PUMP_VLV_OPEN_FLAG->Check__DATA(STR__YES) < 0)
			{
				active__chm_pumping = false;
			}
			if(dCH__MON_PUMPING_SEQ_ACTIVE->Check__DATA(STR__ON) > 0)
			{
				active__chm_pumping = false;
			}

			if(!active__chm_pumping)
			{
				if(Check__BALLAST_CLOSE() < 0)
				{
					// ...
					{
						int alm_id = ALID__BALLAST_VLV_CLOSE;
						CString alm_bff;
						CString alm_msg;
						CString r_act;

						alm_msg = "Pumping state ... \n";

						if(dCH__TMC_CHM_PRESSURE_STATUS->Check__DATA(STR__VAC) < 0)
						{
							alm_bff.Format(" * %s <- %s \n", 
											dCH__TMC_CHM_PRESSURE_STATUS->Get__CHANNEL_NAME(),
											dCH__TMC_CHM_PRESSURE_STATUS->Get__STRING());
							alm_msg += alm_bff;
						}
						if(sEXT_CH__MON_PUMP_RUN_STS->Check__DATA(STR__ON) < 0)
						{
							alm_bff.Format(" * %s <- %s \n", 
											sEXT_CH__MON_PUMP_RUN_STS->Get__CHANNEL_NAME(),
											sEXT_CH__MON_PUMP_RUN_STS->Get__STRING());
							alm_msg += alm_bff;
						}
						if(sCH__PUMP_VLV_OPEN_FLAG->Check__DATA(STR__YES) < 0)
						{
							alm_bff.Format(" * %s <- %s \n", 
											sCH__PUMP_VLV_OPEN_FLAG->Get__CHANNEL_NAME(),
											sCH__PUMP_VLV_OPEN_FLAG->Get__STRING());
							alm_msg += alm_bff;
						}
						if(dCH__MON_PUMPING_SEQ_ACTIVE->Check__DATA(STR__ON) > 0)
						{
							alm_bff.Format(" * %s <- %s \n", 
											dCH__MON_PUMPING_SEQ_ACTIVE->Get__CHANNEL_NAME(),
											dCH__MON_PUMPING_SEQ_ACTIVE->Get__STRING());
							alm_msg += alm_bff;
						}

						p_alarm->Check__ALARM(alm_id, r_act);
						p_alarm->Post__ALARM_With_MESSAGE(alm_id, alm_msg);
					}

					Fnc__BALLAST_CLOSE();
				}
			}
		}

		if(iActive__SIM_MODE > 0)
		{
			double cur__pressure_torr = 0.0;

			if(bActive__AO_BALLAST_PRESSURE_TORR)		cur__pressure_torr = aoEXT_CH__AO_BALLAST_PRESSURE_TORR->Get__VALUE();
			if(bActive__AI_BALLAST_PRESSURE_TORR)		aoEXT_CH__AI_BALLAST_PRESSURE_TORR->Set__VALUE(cur__pressure_torr);
		}

		if(dCH__CFG_TM_BALLAST_CONTROL->Check__DATA(STR__DISABLE) > 0)
		{
			if(active__ballast_ctrl)
			{
				active__ballast_ctrl = false;

				Fnc__BALLAST_CLOSE();
			}
			continue;
		}
		else
		{
			active__ballast_ctrl = true;
		
		}

		if((active__ballast_ctrl)
		&& (active__chm_pumping))
		{
			bool active__ballast_interlock = false;

			// Check : SV Open ...
			{
				int count__pm_open = 0;
				int count__ll_open = 0;

				for(i=0; i<iSIZE__PMx_SLOT_VLV; i++)
				{
					if(dEXT_CH__PMx_SLOT_VLV_X[i]->Check__DATA(STR__OPEN) > 0)		count__pm_open++;
				}
				for(i=0; i<iSIZE__LLx_SLOT_VLV; i++)
				{
					if(dEXT_CH__LLx_SLOT_VLV_X[i]->Check__DATA(STR__OPEN) > 0)		count__ll_open++;
				}

				if((count__pm_open > 0)
				|| (count__ll_open > 0))
				{
					if(dCH__CFG_TM_BALLAST_TRANSFER_FLOW->Check__DATA(STR__YES) < 0)		active__ballast_interlock = true;
				}
				else
				{
					if(dCH__CFG_TM_BALLAST_IDLE_FLOW->Check__DATA(STR__YES) < 0)			active__ballast_interlock = true;
				}
			}

			// Check : Wafer Transfer ...
			if(dCH__PARA_BALLAST_CTRL_ACTIVE->Check__DATA(STR__ON) > 0)
			{
				if(dCH__CFG_TM_BALLAST_TRANSFER_FLOW->Check__DATA(STR__YES) < 0)			active__ballast_interlock = true;
			}

			if(active__ballast_interlock)
			{
				Fnc__BALLAST_CLOSE();
			}
			else
			{
				CString cfg__ctrl_mode = dCH__CFG_TM_BALLAST_MODE->Get__STRING();

				if(cfg__ctrl_mode.CompareNoCase(STR__VALVE) == 0)
				{
					if(bActive__DO_BALLAST_VALVE_SET)			doEXT_CH__DO_BALLAST_VALVE_SET->Set__DATA(STR__OPEN);	
				}
				else if(cfg__ctrl_mode.CompareNoCase(STR__PRESSURE_SET) == 0)
				{
					double cfg__pressure_mtorr = aCH__CFG_TM_BALLAST_N2_PRESSURE_mTORR->Get__VALUE();

					if(bActive__DO_BALLAST_VALVE_SET)			doEXT_CH__DO_BALLAST_VALVE_SET->Set__DATA(STR__OPEN);
					if(bActive__AO_BALLAST_PRESSURE_TORR)		aoEXT_CH__AO_BALLAST_PRESSURE_TORR->Set__VALUE(cfg__pressure_mtorr);
				}
				else if(cfg__ctrl_mode.CompareNoCase(STR__PRESSURE_PID) == 0)
				{

				}
			}
		}

		// ...
	}
}

int  CObj__CHM_STD::Check__BALLAST_CLOSE()
{
	if(bActive__DO_BALLAST_VALVE_SET)
	{
		if(doEXT_CH__DO_BALLAST_VALVE_SET->Check__DATA(STR__CLOSE) < 0)			return -1;
	}
	if(bActive__AO_BALLAST_PRESSURE_TORR)
	{
		double cur_press = aoEXT_CH__AO_BALLAST_PRESSURE_TORR->Get__VALUE();
		if(cur_press > 0.9)			return -2;
	}

	return 1;
}
void CObj__CHM_STD::Fnc__BALLAST_CLOSE()
{
	if(bActive__DO_BALLAST_VALVE_SET)			doEXT_CH__DO_BALLAST_VALVE_SET->Set__DATA(STR__CLOSE);
	if(bActive__AO_BALLAST_PRESSURE_TORR)		aoEXT_CH__AO_BALLAST_PRESSURE_TORR->Set__VALUE(0.0);
}

void CObj__CHM_STD
::Fnc__INTERLOCK(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm)
{
	if(Check__VENT_ALL_VLV__CLOSE(p_alarm) < 0)
	{			
		if(Check__PUMP_ALL_VLV__CLOSE(p_variable, p_alarm) < 0)
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

	// PUMP INTERLOCK ...
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
	
	// ...
}

