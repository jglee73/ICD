#include "StdAfx.h"
#include "CObj__PMC_STD.h"
#include "CObj__PMC_STD__DEF.h"
#include "CObj__PMC_STD__ALID.h"


// ...
int  CObj__PMC_STD
::Check__PRESSURE_ATM(CII_OBJECT__ALARM* p_alarm,
					  const int pmc_id,
					  const int alarm_id)
{
	CString str_PMx_prss_sts;
	CString str_TM_prss_sts;

LOOP_RETRY:

	// ...
	int pm_index = pmc_id - 1;	

	if(pm_index <  0)				return -1;
	if(pm_index > m_nPM_LIMIT)		return -1;

	// 1. PMx ATM Sensor Check.. 
	dCH__PRESS_STATUS[pm_index]->Get__DATA(str_PMx_prss_sts);

	// 2. TM Chamber Pressure Status...
	dEXT_CH__PHY_TM__PRESS_STS->Get__DATA(str_TM_prss_sts);

	if((str_PMx_prss_sts.CompareNoCase("ATM") != 0) 
	|| (str_TM_prss_sts.CompareNoCase("ATM")  != 0))
	{
		CString alarm_msg;
		CString alarm_bff;
		CString r_act;

		alarm_msg.Format("The pressure of PM%d is [%s] status.\n",
						 pmc_id, str_PMx_prss_sts);

		alarm_bff.Format("The pressure of TM is [%s] status.\n",
						 str_TM_prss_sts);
		alarm_msg += alarm_bff;

		p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,alarm_msg,r_act);

		if(r_act.CompareNoCase("RETRY") == 0)
		{	
			goto LOOP_RETRY;
		}
		return -1;
	}

	return 1;
}

int  CObj__PMC_STD
::Check__PRESSURE_VAC(CII_OBJECT__ALARM* p_alarm,
					  const int pmc_id,
					  const int alarm_id)
{
	CString str_PMx_prss_sts;
	CString str_TM_prss_sts;

LOOP_RETRY:

	// ...
	int pm_index = pmc_id - 1;	

	if(pm_index <  0)				return -1;
	if(pm_index > m_nPM_LIMIT)		return -1;

	// 1. PMx VAC Sensor Check.. 
	dCH__PRESS_STATUS[pm_index]->Get__DATA(str_PMx_prss_sts);

	// 2. TM Chamber Pressure Status...
	dEXT_CH__PHY_TM__PRESS_STS->Get__DATA(str_TM_prss_sts);

	if((str_PMx_prss_sts.CompareNoCase("VAC") != 0) 
	|| (str_TM_prss_sts.CompareNoCase("VAC")  != 0))
	{
		CString alarm_msg;
		CString alarm_bff;
		CString r_act;

		alarm_msg.Format("The pressure of PM%d is [%s] status.\n",
			pmc_id, str_PMx_prss_sts);

		alarm_bff.Format("The pressure of TM is [%s] status.\n",
			str_TM_prss_sts);
		alarm_msg += alarm_bff;

		p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,alarm_msg,r_act);

		if(r_act.CompareNoCase("RETRY") == 0)
		{	
			goto LOOP_RETRY;
		}
		return -1;
	}

	return 1;
}

int  CObj__PMC_STD
::Check__SAME_PRESSURE_FOR_VAC_Or_ATM(CII_OBJECT__ALARM* p_alarm,
									  const int pmc_id,
									  const int alarm_id,
									  const CString& act_msg)
{
	CString str_pmc_prss_sts;
	CString str_tmc_prss_sts;

LOOP_RETRY:

	// ...
	int pm_index = pmc_id - 1;	

	if(pm_index < 0)			return -1;
	if(pm_index > m_nPM_LIMIT)	return -1;

	// ...
	dCH__PRESS_STATUS[pm_index]->Get__DATA(str_pmc_prss_sts);
	dEXT_CH__PHY_TM__PRESS_STS->Get__DATA(str_tmc_prss_sts);

	if((str_pmc_prss_sts.CompareNoCase("VAC") == 0) 
		&& (str_tmc_prss_sts.CompareNoCase("VAC") == 0))
	{
		return 1;
	}
	if((str_pmc_prss_sts.CompareNoCase("ATM") == 0) 
		&& (str_tmc_prss_sts.CompareNoCase("ATM") == 0))
	{
		return 1;
	}

	// ...
	{
		CString alarm_msg;
		CString alarm_bff;
		CString r_act;

		// 
		alarm_msg = act_msg;

		alarm_bff.Format("TMC's pressure and PM%1d's pressure must be same. \n", pmc_id);
		alarm_msg += alarm_bff;

		alarm_bff.Format("The pressure of PM%d is [%s] status.\n",
			pmc_id, str_pmc_prss_sts);
		alarm_msg += alarm_bff;

		alarm_bff.Format("The pressure of TM is [%s] status.\n",
			str_tmc_prss_sts);
		alarm_msg += alarm_bff;

		p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,alarm_msg,r_act);

		if(r_act.CompareNoCase("RETRY") == 0)
		{	
			goto LOOP_RETRY;
		}
		if(r_act.CompareNoCase("IGNORE") == 0)
		{
			return 1;
		}

		return -1;
	}

	return 1;
}


// ...
int  CObj__PMC_STD
::Is_CLOSE_CONDITION(CII_OBJECT__VARIABLE* p_variable,
				     CII_OBJECT__ALARM* p_alarm,
				     const CString str_sv_type,
				     const int pmc_id)
{
	CString var_data;
	CString str_log;

	diEXT_CH__VAC_RB_RNE->Link__UPPER_OBJECT_ABORT(sObject_Name);

	// ...
	int pm_index = pmc_id - 1;	

	if(pm_index <  0)
	{
		str_log.Format("PM index below zero.. Error [%d] .. Ret -1", pm_index);
		Fnc_App_Log(str_log);
		return -1;
	}
	if(pm_index > m_nPM_LIMIT)
	{
		str_log.Format("PM index max[%d] limit.. Error [%d] .. Ret -1", m_nPM_LIMIT, pm_index);
		Fnc_App_Log(str_log);
		return -1;
	}

LOOP_RETRY:

	// 1. VAC Robot Arm Retract Status. !!
	int nRet = diEXT_CH__VAC_RB_RNE->When__DATA(STR__ON, 2);
	if(nRet == 0)		return OBJ_ABORT;

	diEXT_CH__VAC_RB_RNE->Get__DATA(var_data);

	if(var_data.CompareNoCase(STR__ON) != 0)
	{
		// Popup Alarm Occur !!

		// ...
		{
			int alarm_id = ALID__VAC_RB_NOT_RETRACTED;

			CString err_msg;
			CString r_act;

			err_msg.Format("VAC Robot's Arm [%s] Status", var_data);	
			Fnc_App_Log(err_msg);	

			p_alarm->Popup__ALARM(alarm_id,r_act);

			if(r_act.CompareNoCase(ACT__RETRY) == 0)
			{
				goto LOOP_RETRY;
			}
		}
		return -1;
	}

	return 1;
}

int  CObj__PMC_STD
::Is__SV_PM_CLOSE(CII_OBJECT__VARIABLE* p_variable,
				  CII_OBJECT__ALARM* p_alarm,
				  const int pmc_id)
{
	CString str_log;

	// ...
	int pm_index = pmc_id - 1;	

	if(pm_index <  0)
	{
		str_log.Format("PM index below zero.. Error [%d] .. Ret -1", pm_index);
		Fnc_App_Log(str_log);
		return -1;
	}
	if(pm_index > m_nPM_LIMIT)
	{
		str_log.Format("PM index max[%d] limit.. Error [%d] .. Ret -1", m_nPM_LIMIT, pm_index);
		Fnc_App_Log(str_log);
		return -1;
	}

	// 1. Already UP Sns Check ...
	if((diEXT_CH__PMx__SV_UP[pm_index]->Check__DATA(STR__ON) > 0)
	&& (diEXT_CH__PMx__SV_DOWN[pm_index]->Check__DATA(STR__OFF) > 0))
	{
		// ...
		{
			doEXT_CH__PMx__SV_UP[pm_index]->Set__DATA(STR__OFF);
			doEXT_CH__PMx__SV_DOWN[pm_index]->Set__DATA(STR__OFF);

			str_log.Format("Already... SV UP sts.. PM%d Slot Valve. and..", pmc_id);
			Fnc_App_Log(str_log);
		}

		// 2. Already TM_OPEN Sns Check..
		if((diEXT_CH__PMx__SV_TM_OPEN[pm_index]->Check__DATA(STR__ON) > 0)
		&& (diEXT_CH__PMx__SV_TM_CLOSE[pm_index]->Check__DATA(STR__OFF) > 0))
		{
			// ...
			{
				doEXT_CH__PMx__SV_TM_OPEN[pm_index]->Set__DATA(STR__OFF);
				doEXT_CH__PMx__SV_TM_CLOSE[pm_index]->Set__DATA(STR__OFF);

				str_log.Format("Already... SV [UP and TM_OPEN] sts.. PM%d Slot Valve. and..", pmc_id);
				Fnc_App_Log(str_log);
			}

			// 3. Already PM_CLOSE Sns Check..
			if((diEXT_CH__PMx__SV_PM_OPEN[pm_index]->Check__DATA(STR__OFF) > 0)
			&& (diEXT_CH__PMx__SV_PM_CLOSE[pm_index]->Check__DATA(STR__ON) > 0))
			{
				// ...
				{
					doEXT_CH__PMx__SV_PM_OPEN[pm_index]->Set__DATA(STR__OFF);
					doEXT_CH__PMx__SV_PM_CLOSE[pm_index]->Set__DATA(STR__OFF);

					str_log.Format("OK.. Already... SV [UP and TM_OPEN and PM_CLOSE] sts.. PM%d Slot Valve.", pmc_id);
					Fnc_App_Log(str_log);
				}
				return 1;
			}
		}
	}

	return -1;
}

int  CObj__PMC_STD
::Is__SV_TM_CLOSE(CII_OBJECT__VARIABLE* p_variable,
				  CII_OBJECT__ALARM* p_alarm,
				  const int pmc_id)
{
	CString str_log;

	// ...
	int pm_index = pmc_id - 1;	

	if(pm_index <  0)
	{
		str_log.Format("PM index below zero.. Error [%d] .. Ret -1", pm_index);
		Fnc_App_Log(str_log);
		return -1;
	}
	if(pm_index > m_nPM_LIMIT)
	{
		str_log.Format("PM index max[%d] limit.. Error [%d] .. Ret -1", m_nPM_LIMIT, pm_index);
		Fnc_App_Log(str_log);
		return -1;
	}

	// 1. Already UP Sns Check ...
	if((diEXT_CH__PMx__SV_UP[pm_index]->Check__DATA(STR__ON) > 0)
	&& (diEXT_CH__PMx__SV_DOWN[pm_index]->Check__DATA(STR__OFF) > 0))
	{
		// ...
		{
			doEXT_CH__PMx__SV_UP[pm_index]->Set__DATA(STR__OFF);
			doEXT_CH__PMx__SV_DOWN[pm_index]->Set__DATA(STR__OFF);

			str_log.Format("Already... SV UP sts.. PM%d Slot Valve. and..", pmc_id);
			Fnc_App_Log(str_log);
		}

		// 2. Already PM_OPEN Sns Check ...
		if((diEXT_CH__PMx__SV_PM_OPEN[pm_index]->Check__DATA(STR__ON) > 0)
		&& (diEXT_CH__PMx__SV_PM_CLOSE[pm_index]->Check__DATA(STR__OFF) > 0))
		{
			// ...
			{
				doEXT_CH__PMx__SV_PM_OPEN[pm_index]->Set__DATA(STR__OFF);
				doEXT_CH__PMx__SV_PM_CLOSE[pm_index]->Set__DATA(STR__OFF);

				str_log.Format("Already... SV [UP and PM_OPEN] sts.. PM%d Slot Valve. and..", pmc_id);
				Fnc_App_Log(str_log);
			}

			// 3. Already TM_CLOSE Sns Check ...
			if((diEXT_CH__PMx__SV_TM_OPEN[pm_index]->Check__DATA(STR__OFF) > 0)
			&& (diEXT_CH__PMx__SV_TM_CLOSE[pm_index]->Check__DATA(STR__ON) > 0))
			{
				// ...
				{
					doEXT_CH__PMx__SV_TM_OPEN[pm_index]->Set__DATA(STR__OFF);
					doEXT_CH__PMx__SV_TM_CLOSE[pm_index]->Set__DATA(STR__OFF);

					str_log.Format("OK.. Already... SV [UP and PM_OPEN and TM_CLOSE] sts.. PM%d Slot Valve.", pmc_id);
					Fnc_App_Log(str_log);
				}
				return 1;
			}
		}
	}

	return -1;
}

int  CObj__PMC_STD
::Is__SV_FULL_OPEN(CII_OBJECT__VARIABLE* p_variable,
				   CII_OBJECT__ALARM* p_alarm,
				   const int pmc_id)
{
	CString str_log;

	// ...
	int pm_index = pmc_id - 1;	

	if(pm_index <  0)
	{
		str_log.Format("PM index below zero.. Error [%d] .. Ret -1", pm_index);
		Fnc_App_Log(str_log);
		return -1;
	}
	if(pm_index > m_nPM_LIMIT)
	{
		str_log.Format("PM index max[%d] limit.. Error [%d] .. Ret -1", m_nPM_LIMIT, pm_index);
		Fnc_App_Log(str_log);
		return -1;
	}

	// 1. Already DOWN Sns Check..
	if((diEXT_CH__PMx__SV_UP[pm_index]->Check__DATA(STR__OFF) > 0)
	&& (diEXT_CH__PMx__SV_DOWN[pm_index]->Check__DATA(STR__ON) > 0))
	{
		// ...
		{
			doEXT_CH__PMx__SV_UP[pm_index]->Set__DATA(STR__OFF);
			doEXT_CH__PMx__SV_DOWN[pm_index]->Set__DATA(STR__OFF);

			str_log.Format("Already... SV DOWN sts.. PM%d Slot Valve. and..", pmc_id);
			Fnc_App_Log(str_log);
		}

		// 2. Already TM_OPEN Sns Check..
		if((diEXT_CH__PMx__SV_TM_OPEN[pm_index]->Check__DATA(STR__ON) > 0)
		&& (diEXT_CH__PMx__SV_TM_CLOSE[pm_index]->Check__DATA(STR__OFF) > 0))
		{
			// ...
			{
				doEXT_CH__PMx__SV_TM_OPEN[pm_index]->Set__DATA(STR__OFF);
				doEXT_CH__PMx__SV_TM_CLOSE[pm_index]->Set__DATA(STR__OFF);

				str_log.Format("Already... SV [DOWN and TM_OPEN] sts.. PM%d Slot Valve. and..", pmc_id);
				Fnc_App_Log(str_log);
			}

			// 3. Already PM_OPEN Sns Check..
			if((diEXT_CH__PMx__SV_PM_OPEN[pm_index]->Check__DATA(STR__ON) > 0)
			&& (diEXT_CH__PMx__SV_PM_CLOSE[pm_index]->Check__DATA(STR__OFF) > 0))
			{
				// ...
				{
					doEXT_CH__PMx__SV_PM_OPEN[pm_index]->Set__DATA(STR__OFF);
					doEXT_CH__PMx__SV_PM_CLOSE[pm_index]->Set__DATA(STR__OFF);

					str_log.Format("OK.. Already... SV [DOWN and TM_OPEN and PM_OPEN] sts.. PM%d Slot Valve.", pmc_id);
					Fnc_App_Log(str_log);
				}
				return 1;
			}
		}
	}

	return -1;
}

int  CObj__PMC_STD
::Check_SV__PM_TM_OPEN_Before_SV_UPDOWN(CII_OBJECT__VARIABLE* p_variable,
										CII_OBJECT__ALARM* p_alarm, 
										const int pmc_id)
{
	CString str_log;

LOOP_RETRY:

	// ...
	int pm_index = pmc_id - 1;	

	if(pm_index <  0)
	{
		str_log.Format("PM index below zero.. Error [%d] .. Ret -1", pm_index);
		Fnc_App_Log(str_log);
		return -1;
	}
	if(pm_index > m_nPM_LIMIT)
	{
		str_log.Format("PM index max[%d] limit.. Error [%d] .. Ret -1", m_nPM_LIMIT, pm_index);
		Fnc_App_Log(str_log);
		return -1;
	}

	if(iSim_Flag > 0)
	{
		diEXT_CH__PMx__SV_PM_OPEN[pm_index]->Set__DATA(STR__ON);
		diEXT_CH__PMx__SV_TM_OPEN[pm_index]->Set__DATA(STR__ON);
	}

	// 1. SV PM OPEN.. Sns Check..
	if((diEXT_CH__PMx__SV_PM_OPEN[pm_index]->Check__DATA(STR__ON) > 0)
	&& (diEXT_CH__PMx__SV_PM_CLOSE[pm_index]->Check__DATA(STR__OFF) > 0))
	{
		// ...
		{
			str_log.Format("Check Complete... PM%d Slot Valve... PM_OPEN sts.. and..", pmc_id);
			Fnc_App_Log(str_log);
		}

		// 2. Sns Check..
		if((diEXT_CH__PMx__SV_TM_OPEN[pm_index]->Check__DATA(STR__ON) > 0)
		&& (diEXT_CH__PMx__SV_TM_CLOSE[pm_index]->Check__DATA(STR__OFF) > 0))
		{
			// ...
			{
				str_log.Format("Check Complete... PM%d Slot Valve... TM_OPEN sts.. ", pmc_id);
				Fnc_App_Log(str_log);
			}
			return 1;
		}
	}

	// ...
	{
		int alarm_id = ALID__VALVE_PM_SV_NOT_UP_ACTION_CONDITION;
		CString r_act;

		p_alarm->Popup__ALARM(alarm_id,r_act);

		if(r_act.CompareNoCase(ACT__RETRY) == 0)
		{
			goto LOOP_RETRY;
		}
	}

	return -1;
}

int  CObj__PMC_STD
::Sub__SV_TM_OPEN(CII_OBJECT__VARIABLE* p_variable,
				  CII_OBJECT__ALARM* p_alarm, 
				  const int pmc_id,
				  double dblTimeout)
{
	double  dblDefault_Timeout = 9999.0;
	int nRet = -1;

	CString str_log;
	CString var_data;

	// ...
	int pm_index = pmc_id - 1;	

	if(pm_index <  0)
	{
		str_log.Format("PM index below zero.. Error [%d] .. Ret -1", pm_index);
		Fnc_App_Log(str_log);
		return -1;
	}
	if(pm_index > m_nPM_LIMIT)
	{
		str_log.Format("PM index max[%d] limit.. Error [%d] .. Ret -1", m_nPM_LIMIT, pm_index);
		Fnc_App_Log(str_log);
		return -1;
	}

	if(iSim_Flag > 0)
	{
		SCX__TIMER_CTRL sim_timer;

		diEXT_CH__PMx__SV_TM_CLOSE[pm_index]->Set__DATA(STR__OFF);
		diEXT_CH__PMx__SV_TM_OPEN[pm_index]->Set__DATA(STR__OFF);

		if(sim_timer->WAIT(0.5) < 0)		return -1;

		diEXT_CH__PMx__SV_TM_CLOSE[pm_index]->Set__DATA(STR__OFF);
		diEXT_CH__PMx__SV_TM_OPEN[pm_index]->Set__DATA(STR__ON);
	}
	else
	{
		// 1. Already Sns Check..
		if((diEXT_CH__PMx__SV_TM_OPEN[pm_index]->Check__DATA(STR__ON) > 0)
		&& (diEXT_CH__PMx__SV_TM_CLOSE[pm_index]->Check__DATA(STR__OFF) > 0))
		{
			// 완료 되면, default OFF
			doEXT_CH__PMx__SV_TM_OPEN[pm_index]->Set__DATA(STR__OFF);

			// ...
			{
				str_log.Format("Already... PM%d Slot Valve... TM_OPEN sts.. ", pmc_id);
				Fnc_App_Log(str_log);
			}
			return 1;
		}

		// ...
		{
			doEXT_CH__PMx__SV_TM_CLOSE[pm_index]->Set__DATA(STR__OFF);
			Sleep(100);
			doEXT_CH__PMx__SV_TM_OPEN[pm_index]->Set__DATA(STR__ON);
		}
	}

	// ...
	SCX__ASYNC_TIMER_CTRL x_async_timer_sv;

	x_async_timer_sv->REGISTER__COUNT_CHANNEL(sObject_Name,aCH_SV_OPEN__TIME_COUNT[pm_index]->Get__VARIABLE_NAME());
	x_async_timer_sv->START__COUNT_UP(dblDefault_Timeout);

	while(1)
	{
		Sleep(1);

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			nRet = -1;
			break;
		}

		// 1. TM_OPEN Sns Check..
		if((diEXT_CH__PMx__SV_TM_OPEN[pm_index]->Check__DATA(STR__ON) > 0)
		&& (diEXT_CH__PMx__SV_TM_CLOSE[pm_index]->Check__DATA(STR__OFF) > 0))
		{
			// Sleep(100);

			str_log.Format("OK... PM%d Slot Valve... TM_OPEN sts.. ", pmc_id);
			Fnc_App_Log(str_log);
			nRet = 1;
			break;
		}

		if(x_async_timer_sv->Get__CURRENT_TIME() >= dblTimeout)
		{
			str_log.Format("TM_OPEN Timeout Occur... PM%d Slot Valve...", pmc_id);
			Fnc_App_Log(str_log);
			nRet = 0; // Timeout Occur !!
			break;
		}
	}

	// 완료 되면.. default OFF
	doEXT_CH__PMx__SV_TM_OPEN[pm_index]->Set__DATA(STR__OFF);
	return nRet;
}

int  CObj__PMC_STD
::Sub__SV_TM_CLOSE(CII_OBJECT__VARIABLE* p_variable,
				   CII_OBJECT__ALARM* p_alarm, 
				   const int pmc_id,
				   double dblTimeout)
{
	double  dblDefault_Timeout = 9999.0;
	int nRet = -1;

	CString str_log;
	CString var_data;

	// ...
	int pm_index = pmc_id - 1;	

	if(pm_index <  0)
	{
		str_log.Format("PM index below zero.. Error [%d] .. Ret -1", pm_index);
		Fnc_App_Log(str_log);
		return -1;
	}
	if(pm_index > m_nPM_LIMIT)
	{
		str_log.Format("PM index max[%d] limit.. Error [%d] .. Ret -1", m_nPM_LIMIT, pm_index);
		Fnc_App_Log(str_log);
		return -1;
	}

	if(iSim_Flag > 0)
	{
		SCX__TIMER_CTRL sim_timer;

		diEXT_CH__PMx__SV_TM_CLOSE[pm_index]->Set__DATA(STR__OFF);
		diEXT_CH__PMx__SV_TM_OPEN[pm_index]->Set__DATA(STR__OFF);

		if(sim_timer->WAIT(0.5) < 0)		return -1;

		diEXT_CH__PMx__SV_TM_CLOSE[pm_index]->Set__DATA(STR__ON);
		diEXT_CH__PMx__SV_TM_OPEN[pm_index]->Set__DATA(STR__OFF);
	}
	else
	{
		// 1. Already Sns Check..
		if((diEXT_CH__PMx__SV_TM_OPEN[pm_index]->Check__DATA(STR__OFF) > 0)
		&& (diEXT_CH__PMx__SV_TM_CLOSE[pm_index]->Check__DATA(STR__ON) > 0))
		{
			doEXT_CH__PMx__SV_TM_CLOSE[pm_index]->Set__DATA(STR__OFF);

			str_log.Format("Already... PM%d Slot Valve... TM_CLOSE sts.. ", pmc_id);
			Fnc_App_Log(str_log);
			return 1;
		}

		// ...
		{
			doEXT_CH__PMx__SV_TM_OPEN[pm_index]->Set__DATA(STR__OFF);
			Sleep(100);
			doEXT_CH__PMx__SV_TM_CLOSE[pm_index]->Set__DATA(STR__ON);
		}
	}

	// ...
	SCX__ASYNC_TIMER_CTRL x_async_timer_sv;

	x_async_timer_sv->REGISTER__COUNT_CHANNEL(sObject_Name,aCH_SV_CLOSE__TIME_COUNT[pm_index]->Get__VARIABLE_NAME());
	x_async_timer_sv->START__COUNT_UP(dblDefault_Timeout);

	while(1)
	{
		Sleep(1);

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			nRet = -1;
			break;
		}

		// 1. TM_CLOSE Sns Check..
		if((diEXT_CH__PMx__SV_TM_OPEN[pm_index]->Check__DATA(STR__OFF) > 0)
		&& (diEXT_CH__PMx__SV_TM_CLOSE[pm_index]->Check__DATA(STR__ON) > 0))
		{
			// ...
			{
				// Sleep(100);

				str_log.Format("OK... PM%d Slot Valve... TM_CLOSE sts.. ", pmc_id);
				Fnc_App_Log(str_log);
			}

			nRet = 1;
			break;
		}

		if(x_async_timer_sv->Get__CURRENT_TIME() >= dblTimeout)
		{
			// ...
			{
				str_log.Format("TM_CLOSE Timeout Occur... PM%d Slot Valve...", pmc_id);
				Fnc_App_Log(str_log);
			}

			nRet = 0;	// Timeout Occur !!
			break;
		}
	}

	doEXT_CH__PMx__SV_TM_CLOSE[pm_index]->Set__DATA(STR__OFF);
	return nRet;
}

int  CObj__PMC_STD
::Sub__SV_PM_OPEN(CII_OBJECT__VARIABLE* p_variable,
				  CII_OBJECT__ALARM* p_alarm, 
				  const int pmc_id,
				  double dblTimeout)
{
	double  dblDefault_Timeout = 9999.0;
	int nRet = -1;

	CString str_log;
	CString var_data;

	// ...
	int pm_index = pmc_id - 1;	

	if(pm_index <  0)
	{
		str_log.Format("PM index below zero.. Error [%d] .. Ret -1", pm_index);
		Fnc_App_Log(str_log);
		return -1;
	}
	if(pm_index > m_nPM_LIMIT)
	{
		str_log.Format("PM index max[%d] limit.. Error [%d] .. Ret -1", m_nPM_LIMIT, pm_index);
		Fnc_App_Log(str_log);
		return -1;
	}

	if(iSim_Flag > 0)
	{
		SCX__TIMER_CTRL sim_timer;

		diEXT_CH__PMx__SV_PM_CLOSE[pm_index]->Set__DATA(STR__OFF);
		diEXT_CH__PMx__SV_PM_OPEN[pm_index]->Set__DATA(STR__OFF);

		if(sim_timer->WAIT(0.5) < 0)		return -1;

		diEXT_CH__PMx__SV_PM_CLOSE[pm_index]->Set__DATA(STR__OFF);
		diEXT_CH__PMx__SV_PM_OPEN[pm_index]->Set__DATA(STR__ON);
	}
	else
	{
		// 1. Already Sns Check..
		if((diEXT_CH__PMx__SV_PM_OPEN[pm_index]->Check__DATA(STR__ON) > 0)
		&& (diEXT_CH__PMx__SV_PM_CLOSE[pm_index]->Check__DATA(STR__OFF) > 0))
		{
			// 완료 되면.. default OFF
			doEXT_CH__PMx__SV_PM_OPEN[pm_index]->Set__DATA(STR__OFF);

			str_log.Format("Already... PM%d Slot Valve... PM_OPEN sts.. ", pmc_id);
			Fnc_App_Log(str_log);
			return 1;
		}

		// ...
		{
			doEXT_CH__PMx__SV_PM_CLOSE[pm_index]->Set__DATA(STR__OFF);
			Sleep(100);
			doEXT_CH__PMx__SV_PM_OPEN[pm_index]->Set__DATA(STR__ON);
		}
	}

	// ...
	SCX__ASYNC_TIMER_CTRL xI_ASYNC_TIMER_SV;

	xI_ASYNC_TIMER_SV->REGISTER__COUNT_CHANNEL(sObject_Name,aCH_SV_OPEN__TIME_COUNT[pm_index]->Get__VARIABLE_NAME());
	xI_ASYNC_TIMER_SV->START__COUNT_UP(dblDefault_Timeout);

	while(1)
	{
		Sleep(1);

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			nRet = -1;
			break;
		}

		// 1. PM_OPEN Sns Check..
		if((diEXT_CH__PMx__SV_PM_OPEN[pm_index]->Check__DATA(STR__ON) > 0)
			&& (diEXT_CH__PMx__SV_PM_CLOSE[pm_index]->Check__DATA(STR__OFF) > 0))
		{
			// ...
			{
				// Sleep(100);

				str_log.Format("OK... PM%d Slot Valve... PM_OPEN sts.. ", pmc_id);
				Fnc_App_Log(str_log);
			}

			nRet = 1;
			break;
		}

		if(xI_ASYNC_TIMER_SV->Get__CURRENT_TIME() >= dblTimeout)
		{
			// ...
			{
				str_log.Format("PM_OPEN Timeout Occur... PM%d Slot Valve...", pmc_id);
				Fnc_App_Log(str_log);
			}

			nRet = 0; // Timeout Occur !!
			break;
		}
	}

	// 완료 되면, default OFF
	doEXT_CH__PMx__SV_PM_OPEN[pm_index]->Set__DATA(STR__OFF);
	return nRet;
}

int  CObj__PMC_STD
::Sub__SV_PM_CLOSE(CII_OBJECT__VARIABLE* p_variable,
				   CII_OBJECT__ALARM* p_alarm, 
				   const int pmc_id,
				   double dblTimeout)
{
	double  dblDefault_Timeout = 9999.0;
	int nRet = -1;

	CString str_log;
	CString var_data;

	// ...
	int pm_index = pmc_id - 1;	

	if(pm_index <  0)
	{
		str_log.Format("PM index below zero.. Error [%d] .. Ret -1", pm_index);
		Fnc_App_Log(str_log);
		return -1;
	}
	if(pm_index > m_nPM_LIMIT)
	{
		str_log.Format("PM index max[%d] limit.. Error [%d] .. Ret -1", m_nPM_LIMIT, pm_index);
		Fnc_App_Log(str_log);
		return -1;
	}

	if(iSim_Flag > 0)
	{
		SCX__TIMER_CTRL sim_timer;

		diEXT_CH__PMx__SV_PM_CLOSE[pm_index]->Set__DATA(STR__OFF);
		diEXT_CH__PMx__SV_PM_OPEN[pm_index]->Set__DATA(STR__OFF);

		if(sim_timer->WAIT(0.5) < 0)		return -1;

		diEXT_CH__PMx__SV_PM_CLOSE[pm_index]->Set__DATA(STR__ON);
		diEXT_CH__PMx__SV_PM_OPEN[pm_index]->Set__DATA(STR__OFF);
	}
	else
	{
		// 1. Already Sns Check..
		if((diEXT_CH__PMx__SV_PM_OPEN[pm_index]->Check__DATA(STR__OFF) > 0)
		&& (diEXT_CH__PMx__SV_PM_CLOSE[pm_index]->Check__DATA(STR__ON) > 0))
		{
			doEXT_CH__PMx__SV_PM_CLOSE[pm_index]->Set__DATA(STR__OFF);

			// ...
			{
				str_log.Format("Already... PM%d Slot Valve... PM_CLOSE sts.. ", pmc_id);
				Fnc_App_Log(str_log);
			}
			return 1;
		}

		// ...
		{
			doEXT_CH__PMx__SV_PM_OPEN[pm_index]->Set__DATA(STR__OFF);
			Sleep(100);
			doEXT_CH__PMx__SV_PM_CLOSE[pm_index]->Set__DATA(STR__ON);
		}
	}

	// ...
	SCX__ASYNC_TIMER_CTRL xI_ASYNC_TIMER_SV;

	xI_ASYNC_TIMER_SV->REGISTER__COUNT_CHANNEL(sObject_Name,aCH_SV_CLOSE__TIME_COUNT[pm_index]->Get__VARIABLE_NAME());
	xI_ASYNC_TIMER_SV->START__COUNT_UP(dblDefault_Timeout);

	while(1)
	{
		Sleep(1);

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			nRet = -1;
			break;
		}

		// 1. PM_CLOSE Sns Check..
		if((diEXT_CH__PMx__SV_PM_OPEN[pm_index]->Check__DATA(STR__OFF) > 0)
			&& (diEXT_CH__PMx__SV_PM_CLOSE[pm_index]->Check__DATA(STR__ON) > 0))
		{
			// ...
			{
				// Sleep(200);

				str_log.Format("OK... PM%d Slot Valve... PM_CLOSE sts.. ", pmc_id);
				Fnc_App_Log(str_log);
			}

			nRet = 1;
			break;
		}

		if(xI_ASYNC_TIMER_SV->Get__CURRENT_TIME() >= dblTimeout)
		{
			// ...
			{
				str_log.Format("PM_CLOSE Timeout Occur... PM%d Slot Valve...", pmc_id);
				Fnc_App_Log(str_log);
			}

			nRet = 0;		// Timeout Occur !!
			break;
		}
	}

	doEXT_CH__PMx__SV_PM_CLOSE[pm_index]->Set__DATA(STR__OFF);
	return nRet;
}

int  CObj__PMC_STD
::Sub__SV_UP(CII_OBJECT__VARIABLE* p_variable,
			 CII_OBJECT__ALARM* p_alarm, 
			 const int pmc_id,
			 double dblTimeout)
{
	double  dblDefault_Timeout = 9999.0;
	int nRet = -1;

	CString str_log;
	CString var_data;

	// ...
	int pm_index = pmc_id - 1;	

	if(pm_index < 0)
	{
		str_log.Format("PM index below zero.. Error [%d] .. Ret -1", pm_index);
		Fnc_App_Log(str_log);
		return -1;
	}
	if(pm_index > m_nPM_LIMIT)
	{
		str_log.Format("PM index max[%d] limit.. Error [%d] .. Ret -1", m_nPM_LIMIT, pm_index);
		Fnc_App_Log(str_log);
		return -1;
	}

	// 1. Already Sns Check ...
	if((diEXT_CH__PMx__SV_UP[pm_index]->Check__DATA(STR__ON)    > 0)
		&& (diEXT_CH__PMx__SV_DOWN[pm_index]->Check__DATA(STR__OFF) > 0))
	{
		doEXT_CH__PMx__SV_UP[pm_index]->Set__DATA(STR__OFF);

		// ...
		{
			str_log.Format("Already... PM%d Slot Valve... UP sts.. ", pmc_id);
			Fnc_App_Log(str_log);
		}
		return 1;
	}

	// 2. PM and TM Open Check ...
	if(iSim_Flag > 0)
	{
		SCX__TIMER_CTRL sim_timer;

		diEXT_CH__PMx__SV_UP[pm_index]->Set__DATA(STR__OFF);
		diEXT_CH__PMx__SV_DOWN[pm_index]->Set__DATA(STR__OFF);

		if(sim_timer->WAIT(0.5) < 0)
		{
			return -1;
		}

		diEXT_CH__PMx__SV_UP[pm_index]->Set__DATA(STR__ON);
		diEXT_CH__PMx__SV_DOWN[pm_index]->Set__DATA(STR__OFF);
	}
	else
	{
		if(Check_SV__PM_TM_OPEN_Before_SV_UPDOWN(p_variable, p_alarm, pmc_id) < 0)
		{
			return -1;
		}

		// ...
		{
			doEXT_CH__PMx__SV_DOWN[pm_index]->Set__DATA(STR__OFF);
			Sleep(100);
			doEXT_CH__PMx__SV_UP[pm_index]->Set__DATA(STR__ON);
		}
	}

	// ...
	SCX__ASYNC_TIMER_CTRL x_asyc_timer_sv;

	x_asyc_timer_sv->REGISTER__COUNT_CHANNEL(sObject_Name,aCH_SV_UP__TIME_COUNT[pm_index]->Get__VARIABLE_NAME());
	x_asyc_timer_sv->START__COUNT_UP(dblDefault_Timeout);

	while(1)
	{
		Sleep(1);

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			nRet = -1;
			break;
		}

		if((diEXT_CH__PMx__SV_UP[pm_index]->Check__DATA(STR__ON)    > 0)
		&& (diEXT_CH__PMx__SV_DOWN[pm_index]->Check__DATA(STR__OFF) > 0))
		{
			// Sleep(100);

			// ...
			{
				str_log.Format("OK... PM%d Slot Valve... UP Complete.. ", pmc_id);
				Fnc_App_Log(str_log);
			}

			nRet = 1;
			break;
		}

		if(x_asyc_timer_sv->Get__CURRENT_TIME() >= dblTimeout)
		{
			// ...
			{
				str_log.Format("SV UP Timeout Occur... PM%d Slot Valve...", pmc_id);
				Fnc_App_Log(str_log);
			}

			nRet = 0;
			break;
		}
	}

	doEXT_CH__PMx__SV_UP[pm_index]->Set__DATA(STR__OFF);
	return nRet;
}

int  CObj__PMC_STD
::Sub__SV_DOWN(CII_OBJECT__VARIABLE* p_variable,
			   CII_OBJECT__ALARM* p_alarm, 
			   const int pmc_id,
			   double dblTimeout)
{
	double dblDefault_Timeout = 9999.0;
	int nRet = -1;

	CString str_log;
	CString var_data;

	// ...
	int pm_index = pmc_id - 1;	

	if(pm_index <  0)
	{
		str_log.Format("PM index below zero.. Error [%d] .. Ret -1", pm_index);
		Fnc_App_Log(str_log);
		return -1;
	}
	if(pm_index > m_nPM_LIMIT)
	{
		str_log.Format("PM index max[%d] limit.. Error [%d] .. Ret -1", m_nPM_LIMIT, pm_index);
		Fnc_App_Log(str_log);
		return -1;
	}

	if(iSim_Flag > 0)
	{
		SCX__TIMER_CTRL sim_timer;

		diEXT_CH__PMx__SV_UP[pm_index]->Set__DATA(STR__OFF);
		diEXT_CH__PMx__SV_DOWN[pm_index]->Set__DATA(STR__OFF);

		if(sim_timer->WAIT(0.5) < 0)		return -1;

		diEXT_CH__PMx__SV_UP[pm_index]->Set__DATA(STR__OFF);
		diEXT_CH__PMx__SV_DOWN[pm_index]->Set__DATA(STR__ON);
	}
	else
	{
		// 1. Already Sns Check..
		if((diEXT_CH__PMx__SV_UP[pm_index]->Check__DATA(STR__OFF) > 0)
		&& (diEXT_CH__PMx__SV_DOWN[pm_index]->Check__DATA(STR__ON) > 0))
		{
			doEXT_CH__PMx__SV_DOWN[pm_index]->Set__DATA(STR__OFF);

			// ...
			{
				str_log.Format("Already... PM%d Slot Valve... DOWN sts.. ", pmc_id);
				Fnc_App_Log(str_log);
			}
			return 1;
		}

		// 2. PM and TM Open Check..
		if(Check_SV__PM_TM_OPEN_Before_SV_UPDOWN(p_variable, p_alarm, pmc_id) < 0)
		{
			return -1;
		}

		// ...
		{
			doEXT_CH__PMx__SV_UP[pm_index]->Set__DATA(STR__OFF);
			Sleep(100);
			doEXT_CH__PMx__SV_DOWN[pm_index]->Set__DATA(STR__ON);
		}
	}

	// ...
	SCX__ASYNC_TIMER_CTRL x_async_timer_sv;

	x_async_timer_sv->REGISTER__COUNT_CHANNEL(sObject_Name,aCH_SV_DOWN__TIME_COUNT[pm_index]->Get__VARIABLE_NAME());
	x_async_timer_sv->START__COUNT_UP(dblDefault_Timeout);

	while(1)
	{
		Sleep(1);

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			nRet = -1;
			break;
		}

		if((diEXT_CH__PMx__SV_UP[pm_index]->Check__DATA(STR__OFF)  > 0)
		&& (diEXT_CH__PMx__SV_DOWN[pm_index]->Check__DATA(STR__ON) > 0))
		{
			// ...
			{
				str_log.Format("OK... PM%d Slot Valve... DOWN Complete.. ", pmc_id);
				Fnc_App_Log(str_log);
			}

			nRet = 1;
			break;
		}

		if(x_async_timer_sv->Get__CURRENT_TIME() >= dblTimeout)
		{
			// ...
			{
				str_log.Format("SV DOWN Timeout Occur... PM%d Slot Valve...", pmc_id);
				Fnc_App_Log(str_log);
			}

			nRet = 0;// Timeout Occur !!
			break;
		}
	}

	doEXT_CH__PMx__SV_DOWN[pm_index]->Set__DATA(STR__OFF);
	return nRet;
}
