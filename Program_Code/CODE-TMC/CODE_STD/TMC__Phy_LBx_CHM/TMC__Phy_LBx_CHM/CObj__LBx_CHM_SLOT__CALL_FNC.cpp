#include "StdAfx.h"
#include "CObj__LBx_CHM_SLOT.h"
#include "CObj__LBx_CHM_SLOT__DEF.h"
#include "CObj__LBx_CHM_SLOT__ALID.h"


//------------------------------------------------------------------------------------------------
// INIT ----- 
int  CObj__LBx_CHM_SLOT
::Call__INIT(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	return Fnc__INIT(p_variable,p_alarm);
}
int  CObj__LBx_CHM_SLOT
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

// CL.PUMP_VLV -----
int  CObj__LBx_CHM_SLOT
::Call__CL_PUMP_VLV(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	Fnc__PUMP_ALL_VLV__CLOSE(p_variable, p_alarm);
	return 1;
}

// OP.PUMP_VLV -----
int  CObj__LBx_CHM_SLOT
::Call__OP_PUMP_VLV(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	Fnc__VENT_ALL_VLV__CLOSE(p_alarm);
	Fnc__PUMP_FAST_VLV__OPEN(p_variable, p_alarm);
	return 1;
}

// PUMP ...
int  CObj__LBx_CHM_SLOT
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
		_Update__ACTION_MIN_MAX(db_i,-1, cur_sec);
	}
	return flag;
}
int  CObj__LBx_CHM_SLOT
::Fnc__PUMP(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
LOOP_RETRY:

	// ...
	CString ch_data;

	// ...
	{
		Fnc__LOG("Vent Valve All Close !!");
	
		if(Fnc__VENT_ALL_VLV__CLOSE(p_alarm) < 0)		return -11;
	}

	// ...
	{
		Fnc__LOG("Gas Valve All Close !!");
	
		if(_Fnc__OBJ_GAS__ALL_CLOSE() < 0)				return -12;
	}

	if(sEXT_CH__MON_PUMP_RUN_STS->Check__DATA(STR__ON) < 0)
	{
		// ...
		{
			Fnc__LOG("Pump Valve All Close !!");
		
			if(Fnc__PUMP_ALL_VLV__CLOSE(p_variable, p_alarm) < 0)		return -131;
		}

		// ...
		{
			Fnc__LOG("Dry Pump ON !!");
		
			if(pPUMP__OBJ_CTRL->Call__OBJECT("PUMP_ON") < 0)			return -132;
		}
	}
	else
	{
		bool active__vac_check = true;

		double cur__press = aiEXT_CH__LBx__PRESSURE_TORR->Get__VALUE();
		double cfg__vac_press = aCH__CFG_VAC_PRESSURE_TORR->Get__VALUE();

		if(cur__press > cfg__vac_press)									active__vac_check = false;
		if(diEXT_CH__LLx_ATM_SNS->Check__DATA(sDATA__ATM_OFF) < 0)		active__vac_check = false;
		if(diEXT_CH__LLx_VAC_SNS->Check__DATA(sDATA__VAC_ON) < 0)		active__vac_check = false;

		if(active__vac_check)
		{
			Fnc__PUMP_FAST_VLV__OPEN(p_variable, p_alarm);

			if(bActive__OBJ_VAT)
			{
				Fnc__LOG("APC Valve Open !");
				_Fnc__OBJ_VAT__OPEN();
			}

			// ...
			{
				CString log_msg;
				CString log_bff;

				log_msg = "Already, VAC-State ... (1) \n";

				log_bff.Format(" * Current pressure <- %.3f (torr) \n", cur__press);
				log_msg += log_bff;
				
				log_bff.Format(" * Config vac-pressure  <- %.3f (torr) \n", cfg__vac_press);
				log_msg += log_bff;

				log_bff.Format(" * %s <- %s \n", 
								diEXT_CH__LLx_ATM_SNS->Get__CHANNEL_NAME(),
								diEXT_CH__LLx_ATM_SNS->Get__STRING());
				log_msg += log_bff;

				log_bff.Format(" * %s <- %s \n", 
								diEXT_CH__LLx_VAC_SNS->Get__CHANNEL_NAME(),
								diEXT_CH__LLx_VAC_SNS->Get__STRING());
				log_msg += log_bff;

				Fnc__LOG(log_msg);
			}
			return 1;
		}	
	}

	// ...
	{
		Fnc__LOG("Pump Valve All Close !!");
	
		if(Fnc__PUMP_ALL_VLV__CLOSE(p_variable, p_alarm) < 0)			return -13;
	}
	
	if(bActive__OBJ_VAT) 
	{
		Fnc__LOG("APC Valve Open !!");

		if(_Fnc__OBJ_VAT__OPEN() < 0)			return -15;
	}
	
	// CHECK : EXHAUST PRESSURE ...
	if(bActive__LBx__EXHAUST_PRESSURE)
	{
		SCX__ASYNC_TIMER_CTRL x_timer;

		x_timer->START__COUNT_UP(99999);

		while(1)
		{
			Sleep(10);

			// ...
			double cur__press = aiEXT_CH__LBx__EXHAUST_PRESSURE->Get__VALUE();

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

	// Pressure Check ...
	{
		bool active__vac_check = true;

		double cur__press = aiEXT_CH__LBx__PRESSURE_TORR->Get__VALUE();
		double cfg__vac_press = aCH__CFG_VAC_PRESSURE_TORR->Get__VALUE();

		if(cur__press > cfg__vac_press)									active__vac_check = false;	
		if(diEXT_CH__LLx_ATM_SNS->Check__DATA(sDATA__ATM_OFF) < 0)		active__vac_check = false;
		if(diEXT_CH__LLx_VAC_SNS->Check__DATA(sDATA__VAC_ON)  < 0)		active__vac_check = false;

		if(active__vac_check)
		{
			Fnc__PUMP_FAST_VLV__OPEN(p_variable, p_alarm);

			if(bActive__OBJ_VAT)
			{
				Fnc__LOG("APC Valve Open !");
				_Fnc__OBJ_VAT__OPEN();
			}

			// ...
			{
				CString log_msg;
				CString log_bff;

				log_msg = "Already, VAC-State ... (2) \n";

				log_bff.Format(" * Current pressure <- %.3f (torr) \n", cur__press);
				log_msg += log_bff;

				log_bff.Format(" * Config vac-pressure  <- %.3f (torr) \n", cfg__vac_press);
				log_msg += log_bff;

				log_bff.Format(" * %s <- %s \n", 
								diEXT_CH__LLx_ATM_SNS->Get__CHANNEL_NAME(),
								diEXT_CH__LLx_ATM_SNS->Get__STRING());
				log_msg += log_bff;

				log_bff.Format(" * %s <- %s \n", 
								diEXT_CH__LLx_VAC_SNS->Get__CHANNEL_NAME(),
								diEXT_CH__LLx_VAC_SNS->Get__STRING());
				log_msg += log_bff;

				Fnc__LOG(log_msg);
			}
			return 1;
		}
	}

	if(iActive__SIM_MODE > 0)
	{
		if(!bActive__ATM_SNS_Virtual_Type)		diEXT_CH__LLx_ATM_SNS->Set__DATA(sDATA__ATM_OFF);
		if(!bActive__VAC_SNS_Virtual_Type)		diEXT_CH__LLx_VAC_SNS->Set__DATA(sDATA__VAC_OFF);
	}

	// Check : Current Pressure ...
	{
		double cur__press = aiEXT_CH__LBx__PRESSURE_TORR->Get__VALUE();	

		double cfg__vac_press  = aCH__CFG_VAC_PRESSURE_TORR->Get__VALUE();
		double cfg__fast_press = aCH__CFG_FAST_PUMP_PRESSURE_TORR->Get__VALUE();

		// ...
		{
			CString log_msg;
			CString log_bff;

			log_msg = "Current pressure state ... \n";

			log_bff.Format(" * Current pressure <- %.3f (torr) \n", cur__press);
			log_msg += log_bff;

			log_bff.Format(" * Config fast-pressure  <- %.3f (torr) \n", cfg__fast_press);
			log_msg += log_bff;

			log_bff.Format(" * Config vac-pressure  <- %.3f (torr) \n", cfg__vac_press);
			log_msg += log_bff;

			log_bff.Format(" * %s <- %s \n", 
							diEXT_CH__LLx_ATM_SNS->Get__CHANNEL_NAME(),
							diEXT_CH__LLx_ATM_SNS->Get__STRING());
			log_msg += log_bff;

			log_bff.Format(" * %s <- %s \n", 
							diEXT_CH__LLx_VAC_SNS->Get__CHANNEL_NAME(),
							diEXT_CH__LLx_VAC_SNS->Get__STRING());
			log_msg += log_bff;

			Fnc__LOG(log_msg);
		}

		if(cur__press < cfg__fast_press)
		{
			goto LOOP__FAST_PUMP;
		}
	}

	// Check : Soft Pumping ...
	{
		Fnc__LOG("Soft-Pumping start ...");

		if(dCH__CFG_USE_SOFT_PUMP_VALVE->Check__DATA(STR__NO) > 0)
		{
			Fnc__PUMP_FAST_VLV__OPEN(p_variable, p_alarm);
		}
		else
		{
			Fnc__PUMP_SOFT_VLV__OPEN(p_variable, p_alarm);
		}

		// Time Delay ...
		{
			SCX__TIMER_CTRL x_timer_ctrl;
			x_timer_ctrl->REGISTER__ABORT_OBJECT(sObject_Name);

			double delay_time = aCH__CFG_SOFT_PUMP_DELAY_TIME->Get__VALUE();
			if(x_timer_ctrl->WAIT(delay_time) < 0)
			{
				return -1;
			}
		}

		if(iActive__SIM_MODE > 0)
		{
			SCX__TIMER_CTRL x_sim_timer;
			x_sim_timer->REGISTER__ABORT_OBJECT(sObject_Name);

			double cfg__sim_sec = aEXT_CH__CFG_SIM_LLx_SLOW_PUMP_TIME->Get__VALUE();
			if(x_sim_timer->WAIT(cfg__sim_sec) < 0)
			{
				return -1;
			}

			double cfg__fast_press = aCH__CFG_FAST_PUMP_PRESSURE_TORR->Get__VALUE();
			aiEXT_CH__LBx__PRESSURE_TORR->Set__VALUE(cfg__fast_press - 0.01);
		}

		// ...
		xI_ASYNC_TIMER->START__COUNT_UP(99999);

		while(1)
		{	
			double cur__press = aiEXT_CH__LBx__PRESSURE_TORR->Get__VALUE();
			double cfg__fast_press = aCH__CFG_FAST_PUMP_PRESSURE_TORR->Get__VALUE();

			// Pressure Check ...
			{
				bool active__vac_check = true;

				if(cur__press > cfg__fast_press)								active__vac_check = false;
				if(diEXT_CH__LLx_ATM_SNS->Check__DATA(sDATA__ATM_OFF) < 0)		active__vac_check = false;

				if(active__vac_check)
				{
					double cfg__vac_press = aCH__CFG_VAC_PRESSURE_TORR->Get__VALUE();

					// ...
					{
						CString log_msg;
						CString log_bff;

						log_msg = "Soft-Pumping Complete ! \n";

						log_bff.Format(" * Current pressure <- %.3f (torr) \n", cur__press);
						log_msg += log_bff;

						log_bff.Format(" * Config fast-pressure <- %.3f (torr) \n", cfg__fast_press);
						log_msg += log_bff;

						log_bff.Format(" * Config vac-pressure <- %.3f (torr) \n", cfg__vac_press);
						log_msg += log_bff;

						log_bff.Format(" * %s <- %s \n", 
										diEXT_CH__LLx_ATM_SNS->Get__CHANNEL_NAME(),
										diEXT_CH__LLx_ATM_SNS->Get__STRING());
						log_msg += log_bff;
						
						log_bff.Format(" * %s <- %s \n", 
										diEXT_CH__LLx_VAC_SNS->Get__CHANNEL_NAME(),
										diEXT_CH__LLx_VAC_SNS->Get__STRING());
						log_msg += log_bff;

						Fnc__LOG(log_msg);
					}
					break;
				}
			}

			Sleep(90);

			if(p_variable->Check__CTRL_ABORT() > 0)
			{
				return -11;
			}

			// ...
			double cfg__soft_timeout = aCH__CFG_SOFT_PUMP_TIMEOUT->Get__VALUE();

			if(xI_ASYNC_TIMER->Get__CURRENT_TIME() >= cfg__soft_timeout)
			{
				int alm_id = ALID__SOFT_PUMPING__TIMEOUT;
				CString alm_msg;
				CString alm_bff;
				CString r_act;

				// ...
				{
					alm_msg.Format(" The current pressure is %.3f (torr). \n", cur__press);

					alm_bff.Format(" The config fast-pressure is %.3f (torr) \n", cfg__fast_press);
					alm_msg += alm_bff;

					alm_bff.Format(" The config soft-timeout is %.0f (sec) \n", cfg__soft_timeout);
					alm_msg += alm_bff;

					Fnc__LOG(alm_msg);
				}

				p_alarm->Popup__ALARM_With_MESSAGE(alm_id, alm_msg, r_act);

				if(r_act.CompareNoCase(ACT__RETRY) == 0)
				{
					goto LOOP_RETRY;
				}
				return -12;
			}
		}

		// Stable.Time ...
		{
			double cfg__stable_sec = aCH__CFG_SOFT_PUMP_COMPLETE_STABLE_TIME->Get__VALUE();

			// ...
			{
				CString log_msg;

				log_msg.Format("Soft-Pumping stable-time <- %.0f (sec) !", cfg__stable_sec);
				Fnc__LOG(log_msg);

				log_msg = "Soft-Pumping stable time starting ...";
				Fnc__MSG(log_msg);
			}

			SCX__TIMER_CTRL x_timer_ctrl;
			x_timer_ctrl->REGISTER__ABORT_OBJECT(sObject_Name);

			if(x_timer_ctrl->WAIT(cfg__stable_sec) < 0)
			{
				return -21;
			}
		}
	}


LOOP__FAST_PUMP:

	if(iActive__SIM_MODE > 0)
	{
		if(!bActive__ATM_SNS_Virtual_Type)			diEXT_CH__LLx_ATM_SNS->Set__DATA(sDATA__ATM_OFF);
		if(!bActive__VAC_SNS_Virtual_Type)			diEXT_CH__LLx_VAC_SNS->Set__DATA(sDATA__VAC_ON);
	}

	// SR Valve Close & FR Valve Open
	{
		// ...
		{
			CString log_msg;
		
			log_msg = "SR Valve Close & FR Valve Open !";
			Fnc__LOG(log_msg);
		}

		Fnc__PUMP_SOFT_VLV__CLOSE(p_variable, p_alarm);
		Fnc__PUMP_FAST_VLV__OPEN(p_variable, p_alarm);
	}

	// Check : VAC-Pressure ...
	{
		xI_ASYNC_TIMER->START__COUNT_UP(99999);

		if(iActive__SIM_MODE > 0)
		{
			SCX__TIMER_CTRL x_sim_timer;
			x_sim_timer->REGISTER__ABORT_OBJECT(sObject_Name);
			
			double cfg__sim_sec = aEXT_CH__CFG_SIM_LLx_FAST_PUMP_TIME->Get__VALUE();
			if(x_sim_timer->WAIT(cfg__sim_sec) < 0)
			{
				return -1;
			}

			double cfg__vac_press = aCH__CFG_VAC_PRESSURE_TORR->Get__VALUE();
			aiEXT_CH__LBx__PRESSURE_TORR->Set__VALUE(cfg__vac_press - 0.01);			
		}

		while(1)
		{
			double cur__press = aiEXT_CH__LBx__PRESSURE_TORR->Get__VALUE();
			double cfg__vac_press = aCH__CFG_VAC_PRESSURE_TORR->Get__VALUE();

			// Pressure Check ...
			{
				bool active__vac_check = true;
		
				if(cur__press > cfg__vac_press)									active__vac_check = false;
				if(diEXT_CH__LLx_ATM_SNS->Check__DATA(sDATA__ATM_OFF) < 0)		active__vac_check = false;
				if(diEXT_CH__LLx_VAC_SNS->Check__DATA(sDATA__VAC_ON) < 0)		active__vac_check = false;

				if(active__vac_check)
				{
					// ...
					{
						CString log_msg;
						CString log_bff;

						log_msg = "Fast-Pumping Complete ! \n";

						log_bff.Format(" * Current pressure <- %.3f (torr) \n", cur__press);
						log_msg += log_bff;

						log_bff.Format(" * Config vac-pressure <- %.3f (torr) \n", cfg__vac_press);
						log_msg += log_bff;

						log_bff.Format(" * %s <- %s \n", 
										diEXT_CH__LLx_ATM_SNS->Get__CHANNEL_NAME(),
										diEXT_CH__LLx_ATM_SNS->Get__STRING());
						log_msg += log_bff;

						log_bff.Format(" * %s <- %s \n", 
										diEXT_CH__LLx_VAC_SNS->Get__CHANNEL_NAME(),
										diEXT_CH__LLx_VAC_SNS->Get__STRING());
						log_msg += log_bff;

						Fnc__LOG(log_msg);
					}
					break;
				}
			}

			Sleep(90);

			if(p_variable->Check__CTRL_ABORT() > 0)
			{
				return -31;
			}

			// Timeout Check ...
			double cfg__fast_timeout = aCH__CFG_FAST_PUMP_TIMEOUT->Get__VALUE();

			if(xI_ASYNC_TIMER->Get__CURRENT_TIME() >= cfg__fast_timeout)
			{
				int alm_id = ALID__FAST_PUMPING__TIMEOUT;
				CString alm_msg;
				CString alm_bff;
				CString r_act;

				// ...
				{
					alm_bff.Format(" The config fast-timeout is %.0f (sec). \n", cfg__fast_timeout);
					alm_msg += alm_bff;
					alm_msg += "\n";

					alm_bff.Format(" The current pressure is %.3f (torr). \n", cur__press);
					alm_msg += alm_bff;
		
					alm_bff.Format(" The config vac-pressure is %.3f (torr). \n", cfg__vac_press);
					alm_msg += alm_bff;

					alm_msg += " Sensor State ... \n";

					alm_bff.Format("   * %s <- %s \n", 
									diEXT_CH__LLx_ATM_SNS->Get__CHANNEL_NAME(),
									diEXT_CH__LLx_ATM_SNS->Get__STRING());
					alm_msg += alm_bff;

					alm_bff.Format("   * %s <- %s \n",
									diEXT_CH__LLx_VAC_SNS->Get__CHANNEL_NAME(),
									diEXT_CH__LLx_VAC_SNS->Get__STRING());
					alm_msg += alm_bff;

					Fnc__LOG(alm_msg);
				}

				p_alarm->Popup__ALARM_With_MESSAGE(alm_id, alm_msg, r_act);

				if(r_act.CompareNoCase(ACT__RETRY) == 0)
				{
					goto LOOP_RETRY;
				}
				return -32;
			}
		}
	}

	Update__PUMP_VLV_STS(p_variable, p_alarm);
	return 1;
}

// VENT ...
int  CObj__LBx_CHM_SLOT
::Call__VENT(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	// ...
	{
		double cur__press = aiEXT_CH__LBx__PRESSURE_TORR->Get__VALUE();

		double cfg__press = aCH__CFG_ATM_PRESSURE_TORR->Get__VALUE();
		double cfg__atm_tol = aCH__CFG_ATM_PRESS_STS_TOLERANCE->Get__VALUE();

		if((cur__press > (cfg__press - cfg__atm_tol))
		&& (dCH__PRESSURE_STATUS->Check__DATA(STR__IO_ATM) > 0))
		{
			Fnc__VENT_ALL_VLV__CLOSE(p_alarm);

			// ...
			{
				CString log_msg;
				CString log_bff;
			
				log_msg = "Already ATM State \n";
				log_bff.Format("  * Current pressure : %.3f torr \n", cur__press);
				log_msg += log_bff;
				log_bff.Format("  * Config atm-pressure : %.3f torr \n", cfg__press);
				log_msg += log_bff;
				log_bff.Format("  * Config atm-tolerance : %.1f torr \n", cfg__atm_tol);
				log_msg += log_bff;
				log_bff.Format("  * ATM-Sensor state \n");
				log_msg += log_bff;
				log_bff.Format("   * %s <- %s \n", 
								dCH__PRESSURE_STATUS->Get__CHANNEL_NAME(),
								dCH__PRESSURE_STATUS->Get__STRING());
				log_msg += log_bff;
							
				Fnc__LOG(log_msg);
			}
			return 1;
		}
	}

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
		_Update__ACTION_MIN_MAX(db_i,-1, cur_sec);
	}

	// ...
	{
		Fnc__VENT_ALL_VLV__CLOSE_WITHOUT_EQUAL_VLV(p_alarm);

		if(dCH__CFG_EQUAL_VLV_OPEN_WHEN_ATM->Check__DATA(STR__ENABLE) < 0)
		{
			if(bActive__ATM_EQUAL_VLV)
				doEXT_CH__ATM_EQUAL_VLV__SET->Set__DATA(STR__CLOSE);
		}
	}
	return flag;
}
int  CObj__LBx_CHM_SLOT
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
			return -11;
		}

		Fnc__VENT_ALL_VLV__CLOSE_WITHOUT_EQUAL_VLV(p_alarm);
	}

	// Equalize-Vent Time ...
	if(bActive__ATM_EQUAL_VLV)
	{
		double cfg_sec = aCH__CFG_EQUALIZE_VENT_TIME->Get__VALUE();

		if(x_timer->WAIT(cfg_sec) < 0)
		{
			return -21;
		}
	}

	// Venting 후 압력 Limit 체크 ...
	{
		double cfg__press = aCH__CFG_ATM_HIGH_PRESSURE_TORR->Get__VALUE();

		if(cfg__press > 700)
		{
			double cur__press = aiEXT_CH__LBx__PRESSURE_TORR->Get__VALUE();

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
int  CObj__LBx_CHM_SLOT
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

	// ...
	cur__press = aiEXT_CH__LBx__PRESSURE_TORR->Get__VALUE();
	cfg__press = aCH__CFG_ATM_PRESSURE_TORR->Get__VALUE();

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
	if(Fnc__PUMP_ALL_VLV__CLOSE(p_variable, p_alarm) < 0)
	{
		return -1;
	}

	Fnc__LOG("Vent Valve All Close !!");
	if(Fnc__VENT_ALL_VLV__CLOSE(p_alarm) < 0)
	{
		return -2;
	}

	if(bActive__OBJ_VAT)
	{
		Fnc__LOG("APC Valve Close !!");
		_Fnc__OBJ_VAT__CLOSE();
	}

	// Soft Venting ...
	{
		Fnc__LOG("Soft Vent Valve Open !!");
		Fnc__VENT_SOFT_VLV__OPEN(p_alarm);

		xI_ASYNC_TIMER->START__COUNT_UP(trg_timeout);

		if(iActive__SIM_MODE > 0)
		{
			if(!bActive__ATM_SNS_Virtual_Type)			diEXT_CH__LLx_ATM_SNS->Set__DATA(sDATA__ATM_OFF);
			if(!bActive__VAC_SNS_Virtual_Type)			diEXT_CH__LLx_VAC_SNS->Set__DATA(sDATA__VAC_OFF);
		}

		// ...
		aiEXT_CH__LBx__PRESSURE_TORR->Get__DATA(var_data);
		cur__press = atof(var_data);

		aCH__CFG_FAST_VENT_PRESSURE_TORR->Get__DATA(var_data);
		cfg__press = atof(var_data);

		if(cur__press < cfg__press)
		{
			double cfg_timeout;

			if(iActive__SIM_MODE > 0)
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
										diEXT_CH__LLx_ATM_SNS->Get__CHANNEL_NAME(),
										diEXT_CH__LLx_ATM_SNS->Get__STRING());
						alm_msg += alm_bff;

						alm_bff.Format("  * %s <- %s \n",
										diEXT_CH__LLx_VAC_SNS->Get__CHANNEL_NAME(),
										diEXT_CH__LLx_VAC_SNS->Get__STRING());
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
			if(bActive__ATM_EQUAL_VLV)	
				doEXT_CH__ATM_EQUAL_VLV__SET->Set__DATA(STR__OPEN);
		}

		xI_ASYNC_TIMER->START__COUNT_UP(trg_timeout);

		if(iActive__SIM_MODE > 0)
		{
			if(!bActive__ATM_SNS_Virtual_Type)			diEXT_CH__LLx_ATM_SNS->Set__DATA(sDATA__ATM_ON);
			if(!bActive__VAC_SNS_Virtual_Type)			diEXT_CH__LLx_VAC_SNS->Set__DATA(sDATA__VAC_OFF);
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

			if(iActive__SIM_MODE > 0)
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
										diEXT_CH__LLx_ATM_SNS->Get__CHANNEL_NAME(),
										diEXT_CH__LLx_ATM_SNS->Get__STRING());
						alm_msg += alm_bff;

						alm_bff.Format("  * %s <- %s \n",
										diEXT_CH__LLx_VAC_SNS->Get__CHANNEL_NAME(),
										diEXT_CH__LLx_VAC_SNS->Get__STRING());
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
int  CObj__LBx_CHM_SLOT
::Call__DV_CLOSE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const int slot_id)
{
	SCX__ASYNC_TIMER_CTRL x_timer_ctrl;
	x_timer_ctrl->START__COUNT_UP(9999);

	int r_flag = Fnc__DV_CLOSE(p_variable,p_alarm, slot_id);
	if(r_flag > 0)
	{
		double cur_sec = x_timer_ctrl->Get__CURRENT_TIME();

		int db_i = _ACT_INDEX__DV_CLOSE;
		int slot_i = slot_id - 1;

		_Update__ACTION_MIN_MAX(db_i,slot_i, cur_sec);
	}

	End_IO__DV_CLOSE();
	return r_flag;
}
int  CObj__LBx_CHM_SLOT
::Fnc__DV_CLOSE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const int slot_id)
{
LOOP_RETRY:

	// ...
	CString str_log;

	if(Is__SLOT_DV_CLOSE())
	{
		str_log.Format("Already, the door Valve of %s is closed.", m_sLBx__MODULE_NAME);
		Fnc__LOG(str_log);
		return 1;
	}

	if(Is_DV_CLOSE_CONDITION(p_variable, p_alarm) < 0)
	{
		return -1;
	}

	Set_IO__DV_CLOSE();

	// ...
	CII__VAR_ANALOG_CTRL* pch__cfg_timeout = NULL;

		 if(slot_id == 1)		pch__cfg_timeout = aCH__CFG_DOOR_1_VALVE_CLOSE_TIMEOUT.Get__PTR();
	else if(slot_id == 2)		pch__cfg_timeout = aCH__CFG_DOOR_2_VALVE_CLOSE_TIMEOUT.Get__PTR();

	if(pch__cfg_timeout == NULL)		return -11;

	// ...
	double  trg_timeout = 9999.0;
	double  cfg_timeout = 0.0;

	xI_ASYNC_TIMER->START__COUNT_UP(trg_timeout);

	while(1)
	{
		Sleep(3);

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			return -1;
		}

		if(Is__SLOT_DV_CLOSE())
		{
			str_log.Format("%s DV Close Completed..", m_sLBx__MODULE_NAME);	
			Fnc__LOG(str_log);
			return 1;
		}

		cfg_timeout = pch__cfg_timeout->Get__VALUE();

		if(xI_ASYNC_TIMER->Get__CURRENT_TIME() >= cfg_timeout)
		{
			break;
		}
	}

	// ...
	{
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
	}
	return -1;
}

// DOOR VLV OPEN -----
int  CObj__LBx_CHM_SLOT
::Call__DV_OPEN(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const int slot_id)
{
	bool active__pressure_check = true;

	if(dEXT_CH__CFG_SETUP_TEST_MODE->Check__DATA(STR__ENABLE) > 0)
	{
		if(dEXT_CH__CFG_SETUP_PRESSURE_CHECK->Check__DATA(STR__DISABLE) > 0)
			active__pressure_check = false;
	}

	if(active__pressure_check)
	{
		if(Check__PRESSURE_ATM_TO_DV_OPEN(p_variable, p_alarm, ALID__DV_OPEN__NOT_ATM_ERROR) < 0)
		{
			return -1;
		}
	}

	// ...
	SCX__ASYNC_TIMER_CTRL x_timer_ctrl;
	x_timer_ctrl->START__COUNT_UP(9999);

	int r_flag = Fnc__DV_OPEN(p_variable,p_alarm, slot_id);
	if(r_flag > 0)
	{
		double cur_sec = x_timer_ctrl->Get__CURRENT_TIME();

		int db_i = _ACT_INDEX__DV_OPEN;
		int slot_i = slot_id - 1;

		_Update__ACTION_MIN_MAX(db_i,slot_i, cur_sec);
	}

	End_IO__DV_OPEN();
	return r_flag;
}
int  CObj__LBx_CHM_SLOT
::Fnc__DV_OPEN(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const int slot_id)
{
LOOP_RETRY:

	// ...
	CString str_log;

	if(Is__SLOT_DV_OPEN())
	{
		str_log.Format("Already... Open sts.. %s Door Valve.", m_sLBx__MODULE_NAME);
		Fnc__LOG(str_log);
		return 1;
	}

	Set_IO__DV_OPEN();

	// ...
	CII__VAR_ANALOG_CTRL* pch__cfg_timeout = NULL;

		 if(slot_id == 1)		pch__cfg_timeout = aCH__CFG_DOOR_1_VALVE_OPEN_TIMEOUT.Get__PTR();
	else if(slot_id == 2)		pch__cfg_timeout = aCH__CFG_DOOR_2_VALVE_OPEN_TIMEOUT.Get__PTR();

	if(pch__cfg_timeout == NULL)		return -11;

	// ...
	double  trg_timeout = 9999.0;
	double  cfg_timeout = 0.0;

	xI_ASYNC_TIMER->START__COUNT_UP(trg_timeout);

	while(1)
	{
		Sleep(3);

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			return -1;
		}

		if(Is__SLOT_DV_OPEN())
		{
			str_log.Format("%s DV Open Completed..", m_sLBx__MODULE_NAME);	
			Fnc__LOG(str_log);
			return 1;
		}

		cfg_timeout = pch__cfg_timeout->Get__VALUE();

		if(xI_ASYNC_TIMER->Get__CURRENT_TIME() >= cfg_timeout)
		{
			break;
		}
	}

	// ...
	{
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
	}
	return -1;
}

// SLIT VLV CLOSE -----
int  CObj__LBx_CHM_SLOT
::Call__SV_CLOSE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const int slot_id)
{
	SCX__ASYNC_TIMER_CTRL x_timer_ctrl;
	x_timer_ctrl->START__COUNT_UP(9999);

	int r_flag = Fnc__SV_CLOSE(p_variable,p_alarm, slot_id);
	if(r_flag > 0)
	{
		double cur_sec = x_timer_ctrl->Get__CURRENT_TIME();

		int db_i = _ACT_INDEX__SV_CLOSE;
		int slot_i = slot_id - 1;

		_Update__ACTION_MIN_MAX(db_i,slot_i, cur_sec);
	}

	End_IO__SV_CLOSE();
	return r_flag;
}
int  CObj__LBx_CHM_SLOT
::Fnc__SV_CLOSE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const int slot_id)
{
	CString str_log;

	// ...
	{
		str_log = "Fnc__SV_CLOSE() : Started ...";
		Fnc__LOG(str_log);
	}

LOOP_RETRY:

	// ...
	double  trg_timeout = 9999.0;
	double  cfg_timeout = 0.0;
	CString var_data;

	if(Is__ALL_SV_CLOSE() > 0)
	{
		str_log.Format("OK..Already... SV Close sts.. %s Slot Valve.", m_sLBx__MODULE_NAME);
		Fnc__LOG(str_log);
		return 1;
	}

	// ...
	CII__VAR_ANALOG_CTRL* pch__cfg_timeout = NULL;

		 if(slot_id == 1)		pch__cfg_timeout = aCH__CFG_SLOT_1_VALVE_CLOSE_TIMEOUT.Get__PTR();
	else if(slot_id == 2)		pch__cfg_timeout = aCH__CFG_SLOT_2_VALVE_CLOSE_TIMEOUT.Get__PTR();

	if(pch__cfg_timeout == NULL)		return -11;

	cfg_timeout = pch__cfg_timeout->Get__VALUE();

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

			p_alarm->Popup__ALARM_With_MESSAGE(alarm_id, err_msg, r_act);

			if(r_act.CompareNoCase(ACT__RETRY) == 0)
			{
				goto LOOP_RETRY;
			}
		}
	}

	return -1;
}

// SLIT VLV OPEN -----
int  CObj__LBx_CHM_SLOT
::Call__SV_OPEN(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm, const int slot_id)
{
	bool active__pressure_check = true;

	if(dEXT_CH__CFG_SETUP_TEST_MODE->Check__DATA(STR__ENABLE) > 0)
	{
		if(dEXT_CH__CFG_SETUP_PRESSURE_CHECK->Check__DATA(STR__DISABLE) > 0)
			active__pressure_check = false;
	}

	if(active__pressure_check)
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
			if(Check__PRESSURE_VAC(p_variable, p_alarm) < 0)
			{
				return -1;
			}

			if(Check__SV_OPEN_MAX_DIFFERENTIAL_PRESS(p_variable, p_alarm) < 0)
			{
				return OBJ_ABORT;
			}
		}
	}

	// ...
	SCX__ASYNC_TIMER_CTRL x_timer_ctrl;
	x_timer_ctrl->START__COUNT_UP(9999);

	int r_flag = Fnc__SV_OPEN(p_variable,p_alarm, slot_id);
	if(r_flag > 0)
	{
		double cur_sec = x_timer_ctrl->Get__CURRENT_TIME();

		int db_i = _ACT_INDEX__SV_OPEN;
		int slot_i = slot_id - 1;

		_Update__ACTION_MIN_MAX(db_i,slot_i, cur_sec);
	}

	End_IO__SV_OPEN();
	return r_flag;
}
int  CObj__LBx_CHM_SLOT
::Fnc__SV_OPEN(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const int slot_id)
{
	CString str_log;

	// ...
	{
		str_log = "Fnc__SV_OPEN() : Started ...";
		Fnc__LOG(str_log);
	}

	// ...
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

	if(Is__ALL_SV_OPEN() > 0)
	{
		str_log.Format("Already... %s's SV Open sts ...", m_sLBx__MODULE_NAME);
		Fnc__LOG(str_log);
		return 1;
	}

	// ...
	CII__VAR_ANALOG_CTRL* pch__cfg_timeout = NULL;

		 if(slot_id == 1)		pch__cfg_timeout = aCH__CFG_SLOT_1_VALVE_OPEN_TIMEOUT.Get__PTR();
	else if(slot_id == 2)		pch__cfg_timeout = aCH__CFG_SLOT_2_VALVE_OPEN_TIMEOUT.Get__PTR();

	if(pch__cfg_timeout == NULL)		return -10001;

	cfg_timeout = pch__cfg_timeout->Get__VALUE();

	r_ret = Sub__SV_OPEN(p_variable, p_alarm, cfg_timeout);

	if(r_ret > 0)
	{
		// Sleep(500);			// HW 동작 time
		return 1;				// FULL OPEN Complete !!
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

		p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,err_msg, r_act);

		if(r_act.CompareNoCase(ACT__RETRY) == 0)
		{
			goto LOOP_RETRY;
		}
	}
	return -1;
}

// ALL_VLV_CLOSE -----
int  CObj__LBx_CHM_SLOT
::Call__ALL_GAS_VLV_CLOSE(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm)
{
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

	return 1;
}

// LIFT_PIN UP -----
int  CObj__LBx_CHM_SLOT
::Call__LIFT_PIN_UP(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	return Fnc__LIFT_PIN_UP(p_variable,p_alarm);
}
int  CObj__LBx_CHM_SLOT
::Fnc__LIFT_PIN_UP(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	if(!bActive__LIFT_PIN)
	{
		return -1;
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

	// ...
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

			_Update__ACTION_MIN_MAX(db_i,-1, cur_sec);
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

int  CObj__LBx_CHM_SLOT
::Call__LIFT_PIN_DOWN(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	return Fnc__LIFT_PIN_DOWN(p_variable,p_alarm);
}
int  CObj__LBx_CHM_SLOT
::Fnc__LIFT_PIN_DOWN(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	if(!bActive__LIFT_PIN)
	{
		return -1;
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

	// ...
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

			_Update__ACTION_MIN_MAX(db_i,-1, cur_sec);
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

int  CObj__LBx_CHM_SLOT
::Is__LIFT_PIN_UP(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	if(!bActive__LIFT_PIN)
	{
		return -1;
	}

	// ...
	CString str_log;

	if((diEXT_CH__LBx__LIFT_PIN_UP->Check__DATA(STR__ON)    > 0)
	&& (diEXT_CH__LBx__LIFT_PIN_DOWN->Check__DATA(STR__OFF) > 0))
	{
		str_log.Format("Already... %s's Lift_Pin Up sts ...", m_sLBx__MODULE_NAME);
		Fnc__LOG(str_log);
		return 1;
	}

	return -1;
}
int  CObj__LBx_CHM_SLOT
::Is__LIFT_PIN_DOWN(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	if(!bActive__LIFT_PIN)
	{
		return -1;
	}

	// ...
	CString str_log;

	if((diEXT_CH__LBx__LIFT_PIN_UP->Check__DATA(STR__OFF)  > 0)
	&& (diEXT_CH__LBx__LIFT_PIN_DOWN->Check__DATA(STR__ON) > 0))
	{
		str_log.Format("Already... %s's Lift_Pin Down sts ...", m_sLBx__MODULE_NAME);
		Fnc__LOG(str_log);
		return 1;
	}

	return -1;
}


// SV_TRANSFER -----
int  CObj__LBx_CHM_SLOT
::Call__SV_TRANSFER_OPEN(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const int slot_id)
{
	return Fnc__SV_TRANSFER_OPEN(p_variable,p_alarm, slot_id);
}
int  CObj__LBx_CHM_SLOT
::Fnc__SV_TRANSFER_OPEN(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const int slot_od)
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
	int r_ret =  Call__SV_OPEN(p_variable, p_alarm, slot_od);
	if(r_ret > 0)
	{
		if(cfg_use > 0)	
		{
			r_ret = Call__LIFT_PIN_UP(p_variable, p_alarm);
		}
	}
	return r_ret;
}

int  CObj__LBx_CHM_SLOT
::Call__SV_TRANSFER_CLOSE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const int slot_id)
{
	return Fnc__SV_TRANSFER_CLOSE(p_variable,p_alarm, slot_id);
}
int  CObj__LBx_CHM_SLOT
::Fnc__SV_TRANSFER_CLOSE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const int slot_id)
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
	int r_ret =  Call__SV_CLOSE(p_variable, p_alarm, slot_id);
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
int  CObj__LBx_CHM_SLOT
::Call__DV_TRANSFER_OPEN(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const int slot_id)
{
	return Fnc__DV_TRANSFER_OPEN(p_variable,p_alarm, slot_id);
}
int  CObj__LBx_CHM_SLOT
::Fnc__DV_TRANSFER_OPEN(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const int slot_id)
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
		cfg_use = dEXT_CH__CFG_LLx_LIFT_PIN_EXIST_FLAG->Check__DATA("TRUE");

		if(cfg_use > 0)
		{
			doEXT_CH__LBx__LIFT_PIN_UP->Set__DATA(STR__ON);
			doEXT_CH__LBx__LIFT_PIN_DOWN->Set__DATA(STR__OFF);
		}
	}

	// ...
	int r_ret =  Call__DV_OPEN(p_variable, p_alarm, slot_id);
	if(r_ret > 0)
	{
		if(cfg_use > 0)
		{
			r_ret = Call__LIFT_PIN_UP(p_variable, p_alarm);
		}
	}
	return r_ret;
}

int  CObj__LBx_CHM_SLOT
::Call__DV_TRANSFER_CLOSE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const int slot_id)
{
	return Fnc__DV_TRANSFER_CLOSE(p_variable,p_alarm, slot_id);
}
int  CObj__LBx_CHM_SLOT
::Fnc__DV_TRANSFER_CLOSE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const int slot_id)
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
		&& (dEXT_CH__CFG_LLx_LIFT_PIN_DOWM_MODE_AFTER_DOOR_CLOSE->Check__DATA("ENABLE") < 0))
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
	int r_ret =  Call__DV_CLOSE(p_variable, p_alarm, slot_id);
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
int  CObj__LBx_CHM_SLOT
::Call__PREPMATER(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	if(dEXT_CH__CFG_LLx_LIFT_PIN_EXIST_FLAG->Check__DATA("TRUE") < 0)
	{
		return 1;
	}

	return Fnc__LIFT_PIN_UP(p_variable,p_alarm);
}

// COMPMATER -----
int  CObj__LBx_CHM_SLOT
::Call__COMPMATER(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	return 1;
}


// IN_PROC -----
int  CObj__LBx_CHM_SLOT
::Call__IN_PROC(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	return Fnc__IN_PROC(p_variable,p_alarm);
}

int  CObj__LBx_CHM_SLOT
::Fnc__IN_PROC(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{

	return 1;
}

// OUT_PROC -----
int  CObj__LBx_CHM_SLOT
::Call__OUT_PROC(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	return Fnc__OUT_PROC(p_variable,p_alarm);
}

int  CObj__LBx_CHM_SLOT
::Fnc__OUT_PROC(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	if(dCH__CFG_HEATER_USE->Check__DATA("YES") < 0)
	{
		return 1;
	}

	// ...
	{
		int wafer_count = 0;

		for(int i=0; i<iLBx_SLOT_SIZE; i++)
		{
			if(dCH__SLOT_STATUS[0]->Check__DATA("NONE") < 0)
				wafer_count++;
		}

		if(wafer_count < 1)
		{
			return 1;
		}
	}

	if(diEXT_CH__LLx_VAC_SNS->Check__DATA(sDATA__VAC_ON) < 0)
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
		if(bActive__ATM_EQUAL_VLV)
			doEXT_CH__ATM_EQUAL_VLV__SET->Set__DATA(STR__CLOSE);

		doEXT_CH__FAST_PUMP_VLV__SET->Set__DATA(STR__CLOSE);
		doEXT_CH__SOFT_PUMP_VLV__SET->Set__DATA(STR__OPEN);

		if(bActive__SOFT_VENT_VLV)
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
		if(bActive__SOFT_VENT_VLV)		
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
int  CObj__LBx_CHM_SLOT
::Call__AUTO_PUMP_VLV_CLOSE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	return Fnc__AUTO_PUMP_VLV_CLOSE(p_variable,p_alarm);
}

int  CObj__LBx_CHM_SLOT
::Fnc__AUTO_PUMP_VLV_CLOSE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	Fnc__PUMP_ALL_VLV__CLOSE(p_variable, p_alarm);
	return 1;
}


// TIME TEST ...
int  CObj__LBx_CHM_SLOT
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

			_Update__ACTION_MIN_MAX(db_i,-1, cur_sec);
		}
		// UP ...
		{
			int db_i = _ACT_INDEX__LIFT_PIN_UP;
			_Update__ACTION_MIN_MAX(db_i,-1, cur_sec);
		}

		// OPEN ...
		{
			int db_i = _ACT_INDEX__SV_OPEN;
			_Update__ACTION_MIN_MAX(db_i,-1, cur_sec);
		}
		// CLOSE ...
		{
			int db_i = _ACT_INDEX__SV_CLOSE;
			_Update__ACTION_MIN_MAX(db_i,-1, cur_sec);
		}
	}
	return 1;
}
