#include "StdAfx.h"
#include "CObj__PMC_SIMPLE.h"
#include "CObj__PMC_SIMPLE__DEF.h"
#include "CObj__PMC_SIMPLE__ALID.h"


// ...
int  CObj__PMC_SIMPLE
::Check__PRESSURE_ATM(CII_OBJECT__ALARM* p_alarm,
					  const int pmc_id,
					  const int alarm_id)
{
	CString str_PMx_prss_sts;
	CString str_TM_prss_sts;

LOOP_RETRY:

	// ...
	int pm_index = pmc_id - 1;	

	if(pm_index < 0)				return -1;
	if(pm_index > iPM_LIMIT)		return -1;

	// 1. PMx ATM Sensor Check.. 
	dCH__PRESS_STATUS[pm_index]->Get__DATA(str_PMx_prss_sts);

	// 2. TM Chamber Pressure Status...
	dEXT_CH__PHY_TM__PRESS_STS->Get__DATA(str_TM_prss_sts);

	if((str_PMx_prss_sts.CompareNoCase(STR__ATM) != 0) 
	|| (str_TM_prss_sts.CompareNoCase(STR__ATM)  != 0))
	{
		CString alarm_msg;
		CString alarm_bff;
		CString r_act;

		alarm_msg.Format("The pressure of PM%d is [%s] status.\n", pmc_id, str_PMx_prss_sts);

		alarm_bff.Format("The pressure of TM is [%s] status.\n", str_TM_prss_sts);
		alarm_msg += alarm_bff;

		p_alarm->Popup__ALARM_With_MESSAGE(alarm_id, alarm_msg, r_act);

		if(r_act.CompareNoCase(ACT__RETRY) == 0)
		{	
			goto LOOP_RETRY;
		}
		return -1;
	}

	return 1;
}

int  CObj__PMC_SIMPLE
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
	if(pm_index > iPM_LIMIT)		return -1;

	// 1. PMx VAC Sensor Check.. 
	dCH__PRESS_STATUS[pm_index]->Get__DATA(str_PMx_prss_sts);

	// 2. TM Chamber Pressure Status...
	dEXT_CH__PHY_TM__PRESS_STS->Get__DATA(str_TM_prss_sts);

	if((str_PMx_prss_sts.CompareNoCase(STR__VAC) != 0) 
	|| (str_TM_prss_sts.CompareNoCase(STR__VAC)  != 0))
	{
		CString alarm_msg;
		CString alarm_bff;
		CString r_act;

		alarm_msg.Format("The pressure of PM%d is [%s] status.\n", pmc_id, str_PMx_prss_sts);

		alarm_bff.Format("The pressure of TM is [%s] status.\n", str_TM_prss_sts);
		alarm_msg += alarm_bff;

		p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,alarm_msg,r_act);

		if(r_act.CompareNoCase(ACT__RETRY) == 0)
		{	
			goto LOOP_RETRY;
		}
		return -1;
	}

	return 1;
}

int  CObj__PMC_SIMPLE
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

	if(pm_index < 0)				return -1;
	if(pm_index > iPM_LIMIT)		return -1;

	// ...
	dCH__PRESS_STATUS[pm_index]->Get__DATA(str_pmc_prss_sts);
	dEXT_CH__PHY_TM__PRESS_STS->Get__DATA(str_tmc_prss_sts);

	if((str_pmc_prss_sts.CompareNoCase(STR__VAC) == 0) 
	&& (str_tmc_prss_sts.CompareNoCase(STR__VAC) == 0))
	{
		return 1;
	}
	
	if((str_pmc_prss_sts.CompareNoCase(STR__ATM) == 0) 
	&& (str_tmc_prss_sts.CompareNoCase(STR__ATM) == 0))
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

		alarm_bff.Format("The pressure of PM%d is [%s] status.\n", pmc_id, str_pmc_prss_sts);
		alarm_msg += alarm_bff;

		alarm_bff.Format("The pressure of TM is [%s] status.\n", str_tmc_prss_sts);
		alarm_msg += alarm_bff;

		p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,alarm_msg,r_act);

		if(r_act.CompareNoCase(ACT__RETRY) == 0)
		{	
			goto LOOP_RETRY;
		}
		if(r_act.CompareNoCase(ACT__IGNORE) == 0)
		{
			return 1;
		}

		return -1;
	}

	return 1;
}


// ...
int  CObj__PMC_SIMPLE
::Is__CLOSE_CONDITION(CII_OBJECT__VARIABLE* p_variable,
					  CII_OBJECT__ALARM* p_alarm,
					  const int pmc_id)
{
	CString var_data;
	CString str_log;

	// ...
	int pm_index = pmc_id - 1;	

	if(pm_index <  0)
	{
		str_log.Format("PM index below zero.. Error [%d] .. Ret -1", pm_index);
		Fnc_App_Log(str_log);
		return -1;
	}
	if(pm_index >= iPM_LIMIT)
	{
		str_log.Format("PM index max[%d] limit.. Error [%d] .. Ret -1", iPM_LIMIT, pm_index);
		Fnc_App_Log(str_log);
		return -1;
	}

	if(bActive__VAC_RB__RNE_PM_X[pm_index])
	{
		diEXT_CH__VAC_RB__RNE_PM_X[pm_index]->Link__UPPER_OBJECT_ABORT(sObject_Name);
	}


LOOP_RETRY:

	if(iActive__SIM_MODE > 0)
	{
		if(bActive__VAC_RB__RNE_PM_X[pm_index])			diEXT_CH__VAC_RB__RNE_PM_X[pm_index]->Set__DATA(sDATA__RNE_ON);
	}

	if(bActive__VAC_RB__RNE_PM_X[pm_index])
	{
		int r_ret = diEXT_CH__VAC_RB__RNE_PM_X[pm_index]->When__DATA(sDATA__RNE_ON, 2);
		if(r_ret == 0)		return -11;

		if(diEXT_CH__VAC_RB__RNE_PM_X[pm_index]->Check__DATA(sDATA__RNE_ON) < 0)
		{
			var_data = diEXT_CH__VAC_RB__RNE_PM_X[pm_index]->Get__STRING();

			// ...
			int alarm_id = ALID__VAC_RB_NOT_RETRACTED;

			CString err_msg;
			CString err_bff;
			CString r_act;

			err_msg.Format("VAC Robot's Arm [%s] Status", var_data);	

			err_bff.Format("  * %s <- %s \n",
						   diEXT_CH__VAC_RB__RNE_PM_X[pm_index]->Get__CHANNEL_NAME(),
						   diEXT_CH__VAC_RB__RNE_PM_X[pm_index]->Get__STRING());
			err_msg += err_bff;

			err_bff.Format("  * REF.State <- %s \n", sDATA__RNE_ON);
			err_msg += err_bff;

			Fnc_App_Log(err_msg);	

			p_alarm->Popup__ALARM_With_MESSAGE(alarm_id, err_msg, r_act);

			if(r_act.CompareNoCase(ACT__RETRY) == 0)
			{
				goto LOOP_RETRY;
			}
			return -1;
		}
	}
	
	// CHECK : WAFER_OUT ...
	{
		bool active__wafer_out = false;

		// Check ...
		{
			int sns_size = iSIZE_PMx__WAFER_OUT_X[pm_index];
			for(int sns_i = 0; sns_i < sns_size; sns_i++)
			{
				if(diEXT_CH__PMx__WAFER_OUT_XY[pm_index][sns_i]->Check__DATA(STR__OFF) > 0)			continue;

				active__wafer_out = true;
				break;
			}
		}

		if(active__wafer_out)
		{
			CString alm_msg;
			CString alm_bff;

			alm_msg  = "Wafer Slide Sensor List";
			alm_msg += "\n";

			int sns_size = iSIZE_PMx__WAFER_OUT_X[pm_index];
			for(int sns_i = 0; sns_i < sns_size; sns_i++)
			{
				alm_bff.Format("  %1d)  %s <- %s \n",
							   sns_i + 1,
							   diEXT_CH__PMx__WAFER_OUT_XY[pm_index][sns_i]->Get__CHANNEL_NAME(),
							   diEXT_CH__PMx__WAFER_OUT_XY[pm_index][sns_i]->Get__STRING());
				alm_msg += alm_bff;
			}

			int alarm_id = ALID__WAFER_SLIDE_SENSOR_DETECTED;
			CString r_act;

			p_alarm->Popup__ALARM_With_MESSAGE(alarm_id, alm_msg, r_act);

			if(r_act.CompareNoCase(ACT__RETRY) == 0)		goto LOOP_RETRY;
			if(r_act.CompareNoCase(ACT__ABORT) == 0)		return -1;
		}
	}

	return 1;
}

int  CObj__PMC_SIMPLE
::Is__SV_OPEN(CII_OBJECT__VARIABLE* p_variable,
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
	if(pm_index > iPM_LIMIT)
	{
		str_log.Format("PM index max[%d] limit.. Error [%d] .. Ret -1", iPM_LIMIT, pm_index);
		Fnc_App_Log(str_log);
		return -1;
	}

	// 1. SV_OPEN Sns Check..
	if((diEXT_CH__PMx__SV_OPEN[pm_index]->Check__DATA(STR__ON)   > 0)
	&& (diEXT_CH__PMx__SV_CLOSE[pm_index]->Check__DATA(STR__OFF) > 0))
	{
		str_log.Format("Already... PM(%1d)'s SV is open.", pmc_id);
		Fnc_App_Log(str_log);
		return 1;
	}

	return -1;
}
int  CObj__PMC_SIMPLE
::Is__SV_CLOSE(CII_OBJECT__VARIABLE* p_variable,
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
	if(pm_index > iPM_LIMIT)
	{
		str_log.Format("PM index max[%d] limit.. Error [%d] .. Ret -1", iPM_LIMIT, pm_index);
		Fnc_App_Log(str_log);
		return -1;
	}

	// 1. SV_CLOSE Sns Check..
	if((diEXT_CH__PMx__SV_OPEN[pm_index]->Check__DATA(STR__OFF) > 0)
	&& (diEXT_CH__PMx__SV_CLOSE[pm_index]->Check__DATA(STR__ON) > 0))
	{
		str_log.Format("Already... PM(%1d)'s SV is close.", pmc_id);
		Fnc_App_Log(str_log);
		return 1;
	}

	return -1;
}

int CObj__PMC_SIMPLE::Set_IO__SV_OPEN(const int pmc_id)
{
	if(iDATA__PMx_SLOT_VLV_CTRL_TYPE != _DEF__PMx_SLOT_VLV_TYPE__IO)
	{
		return 11;
	}

	// ...
	int pm_index = pmc_id - 1;

	if(pm_index <  0)				return -1;
	if(pm_index >= iPM_LIMIT)		return -2;

	// ...
	{
		if(bActive__PMx__SV_CLOSE_X[pm_index])		doEXT_CH__PMx__SV_CLOSE_X[pm_index]->Set__DATA(STR__OFF);
		if(bActive__PMx__SV_OPEN_X[pm_index])		doEXT_CH__PMx__SV_OPEN_X[pm_index]->Set__DATA(STR__ON);
	}
	return 1;
}
int CObj__PMC_SIMPLE::End_IO__SV_OPEN(const int pmc_id)
{
	if(iDATA__PMx_SLOT_VLV_CTRL_TYPE != _DEF__PMx_SLOT_VLV_TYPE__IO)
	{
		return 11;
	}

	// ...
	int pm_index = pmc_id - 1;

	if(pm_index <  0)				return -1;
	if(pm_index >= iPM_LIMIT)		return -2;

	if(dCH__CFG_IO_OFF_MODE->Check__DATA(STR__ENABLE) > 0)
	{
		if(bActive__PMx__SV_CLOSE_X[pm_index])		doEXT_CH__PMx__SV_CLOSE_X[pm_index]->Set__DATA(STR__OFF);
		if(bActive__PMx__SV_OPEN_X[pm_index])		doEXT_CH__PMx__SV_OPEN_X[pm_index]->Set__DATA(STR__OFF);
	}
	return 1;
}

int CObj__PMC_SIMPLE::Set_IO__SV_CLOSE(const int pmc_id)
{
	if(iDATA__PMx_SLOT_VLV_CTRL_TYPE != _DEF__PMx_SLOT_VLV_TYPE__IO)
	{
		return 11;
	}

	// ...
	int pm_index = pmc_id - 1;

	if(pm_index <  0)				return -1;
	if(pm_index >= iPM_LIMIT)		return -2;

	// ...
	{
		if(bActive__PMx__SV_OPEN_X[pm_index])		doEXT_CH__PMx__SV_OPEN_X[pm_index]->Set__DATA(STR__OFF);
		if(bActive__PMx__SV_CLOSE_X[pm_index])		doEXT_CH__PMx__SV_CLOSE_X[pm_index]->Set__DATA(STR__ON);
	}
	return 1;
}
int CObj__PMC_SIMPLE::End_IO__SV_CLOSE(const int pmc_id)
{
	if(iDATA__PMx_SLOT_VLV_CTRL_TYPE != _DEF__PMx_SLOT_VLV_TYPE__IO)
	{
		return 11;
	}

	// ...
	int pm_index = pmc_id - 1;

	if(pm_index <  0)				return -1;
	if(pm_index >= iPM_LIMIT)		return -2;

	if(dCH__CFG_IO_OFF_MODE->Check__DATA(STR__ENABLE) > 0)
	{
		if(bActive__PMx__SV_CLOSE_X[pm_index])		doEXT_CH__PMx__SV_CLOSE_X[pm_index]->Set__DATA(STR__OFF);
		if(bActive__PMx__SV_OPEN_X[pm_index])		doEXT_CH__PMx__SV_OPEN_X[pm_index]->Set__DATA(STR__OFF);
	}
	return 1;
}

int  CObj__PMC_SIMPLE
::Fnc__SV_CLOSE(CII_OBJECT__VARIABLE* p_variable,
				CII_OBJECT__ALARM* p_alarm, 
				const int pmc_id,
				const double cfg_timeout)
{
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
	if(pm_index >= iPM_LIMIT)
	{
		str_log.Format("PM index max[%d] limit.. Error [%d] .. Ret -1", iPM_LIMIT, pm_index);
		Fnc_App_Log(str_log);
		return -1;
	}

	if(iActive__SIM_MODE > 0)
	{
		if(dEXT_CH__CFG_PMx_CHM_TYPE[pm_index]->Check__DATA(STR__DUMMY) > 0)
		{
			if(dCH__CFG_USE_SHUTTER[pm_index]->Check__DATA(STR__YES) > 0)
			{
				sCH_SHUTTER_STS[pm_index]->Set__DATA(STR__CLOSE);
			}
			if(dCH__CFG_USE_LIFT_PIN[pm_index]->Check__DATA(STR__YES) > 0)
			{
				sCH_LIFT_PIN_POS_STS[pm_index]->Set__DATA(STR__DOWN);
			}
			if(dCH__CFG_USE_CR_POSITION[pm_index]->Check__DATA(STR__YES) > 0)
			{
				sCH_CR_POSITION_STS[pm_index]->Set__DATA(STR__DOWN);
			}

			if(dEXT_CH__CFG_PMx_SV_USE[pm_index]->Check__DATA(STR__YES) > 0)
			{
				Set_IO__SV_CLOSE(pmc_id);
			}
		}
		else
		{
			if(iDATA__PMx_SLOT_VLV_CTRL_TYPE == _DEF__PMx_SLOT_VLV_TYPE__OBJ)
			{
				aEXT_CH__PARA_PMC_ID->Set__VALUE(pmc_id);
	
				int r_flag = pOBJ_CTRL__PMx_HANDOFF->Call__OBJECT(sMODE__PMx_SLOT_VLV__CLOSE);
				if(r_flag < 0)			return -101;
			
				diEXT_CH__PMx__SV_CLOSE[pm_index]->Set__DATA(STR__ON);
				diEXT_CH__PMx__SV_OPEN[pm_index]->Set__DATA(STR__OFF);
			}
			else
			{
				if((diEXT_CH__PMx__SV_OPEN[pm_index]->Check__DATA(STR__OFF) > 0)
				&& (diEXT_CH__PMx__SV_CLOSE[pm_index]->Check__DATA(STR__ON) > 0))
				{
					Set_IO__SV_CLOSE(pmc_id);

					// ...
					{
						str_log.Format("Already... PM%d Slot Valve... TM_CLOSE sts.. ", pmc_id);
						Fnc_App_Log(str_log);
					}
					return 1;
				}

				Set_IO__SV_CLOSE(pmc_id);
			}
		}
	}
	else
	{
		if(dEXT_CH__CFG_PMx_CHM_TYPE[pm_index]->Check__DATA(STR__DUMMY) > 0)
		{

		}
		else
		{
			if(iDATA__PMx_SLOT_VLV_CTRL_TYPE == _DEF__PMx_SLOT_VLV_TYPE__OBJ)
			{
				aEXT_CH__PARA_PMC_ID->Set__VALUE(pmc_id);

				int r_flag = pOBJ_CTRL__PMx_HANDOFF->Call__OBJECT(sMODE__PMx_SLOT_VLV__CLOSE);
				if(r_flag < 0)			return -101;
			}
			else
			{
				if((diEXT_CH__PMx__SV_OPEN[pm_index]->Check__DATA(STR__OFF) > 0)
				&& (diEXT_CH__PMx__SV_CLOSE[pm_index]->Check__DATA(STR__ON) > 0))
				{
					Set_IO__SV_CLOSE(pmc_id);

					// ...
					{
						str_log.Format("Already... PM%d Slot Valve... TM_CLOSE sts.. ", pmc_id);
						Fnc_App_Log(str_log);
					}
					return 1;
				}

				Set_IO__SV_CLOSE(pmc_id);
			}
		}
	}

	// ...
	SCX__ASYNC_TIMER_CTRL x_async_timer_sv;
	double sec_countup = cfg_timeout + 10;
	int r_ret = -1;

	x_async_timer_sv->REGISTER__COUNT_CHANNEL(sObject_Name,aCH_SV_CLOSE__TIME_COUNT[pm_index]->Get__VARIABLE_NAME());
	x_async_timer_sv->START__COUNT_UP(sec_countup);

	while(1)
	{
		Sleep(1);

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			r_ret = -1;
			break;
		}

		if(dEXT_CH__CFG_PMx_SV_USE[pm_index]->Check__DATA(STR__NO) > 0)
		{
			r_ret = 1;
			break;
		}

		// 1. TM_CLOSE Sns Check..
		if((diEXT_CH__PMx__SV_OPEN[pm_index]->Check__DATA(STR__OFF) > 0)
		&& (diEXT_CH__PMx__SV_CLOSE[pm_index]->Check__DATA(STR__ON) > 0))
		{
			// ...
			{
				str_log.Format("OK... PM%d Slot Valve... TM_CLOSE sts.. ", pmc_id);
				Fnc_App_Log(str_log);
			}

			r_ret = 1;
			break;
		}

		if(x_async_timer_sv->Get__CURRENT_TIME() >= cfg_timeout)
		{
			// ...
			{
				str_log.Format("TM_CLOSE Timeout Occur... PM%d Slot Valve...", pmc_id);
				Fnc_App_Log(str_log);
			}

			r_ret = 0;	// Timeout Occur !!
			break;
		}
	}
	return r_ret;
}

int  CObj__PMC_SIMPLE
::Fnc__SV_OPEN(CII_OBJECT__VARIABLE* p_variable,
			   CII_OBJECT__ALARM* p_alarm, 
			   const int pmc_id,
			   const double cfg_timeout)
{
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
	if(pm_index >= iPM_LIMIT)
	{
		str_log.Format("PM index max[%d] limit.. Error [%d] .. Ret -1", iPM_LIMIT, pm_index);
		Fnc_App_Log(str_log);
		return -1;
	}

	if(iActive__SIM_MODE > 0)
	{
		if(dEXT_CH__CFG_PMx_CHM_TYPE[pm_index]->Check__DATA(STR__DUMMY) > 0)
		{
			if(dCH__CFG_USE_SHUTTER[pm_index]->Check__DATA(STR__YES) > 0)
			{
				sCH_SHUTTER_STS[pm_index]->Set__DATA(STR__OPEN);
			}
			if(dCH__CFG_USE_LIFT_PIN[pm_index]->Check__DATA(STR__YES) > 0)
			{
				sCH_LIFT_PIN_POS_STS[pm_index]->Set__DATA(STR__UP);
			}
			if(dCH__CFG_USE_CR_POSITION[pm_index]->Check__DATA(STR__YES) > 0)
			{
				sCH_CR_POSITION_STS[pm_index]->Set__DATA(STR__UP);
			}
			
			if(dEXT_CH__CFG_PMx_SV_USE[pm_index]->Check__DATA(STR__YES) > 0)
			{
				Set_IO__SV_OPEN(pmc_id);
			}
		}
		else
		{
			if(iDATA__PMx_SLOT_VLV_CTRL_TYPE == _DEF__PMx_SLOT_VLV_TYPE__OBJ)
			{
				aEXT_CH__PARA_PMC_ID->Set__VALUE(pmc_id);

				int r_flag = pOBJ_CTRL__PMx_HANDOFF->Call__OBJECT(sMODE__PMx_SLOT_VLV__OPEN);
				if(r_flag < 0)			return -101;
	
				diEXT_CH__PMx__SV_CLOSE[pm_index]->Set__DATA(STR__OFF);
				diEXT_CH__PMx__SV_OPEN[pm_index]->Set__DATA(STR__ON);
			}
			else
			{
				if((diEXT_CH__PMx__SV_OPEN[pm_index]->Check__DATA(STR__ON)   > 0)
				&& (diEXT_CH__PMx__SV_CLOSE[pm_index]->Check__DATA(STR__OFF) > 0))
				{
					Set_IO__SV_OPEN(pmc_id);

					// ...
					{
						str_log.Format("Already, the slot-valve of PM%d is open. ", pmc_id);
						Fnc_App_Log(str_log);
					}
					return 1;
				}

				Set_IO__SV_OPEN(pmc_id);
			}
		}
	}
	else
	{
		if(dEXT_CH__CFG_PMx_CHM_TYPE[pm_index]->Check__DATA(STR__DUMMY) > 0)
		{

		}
		else
		{
			if(iDATA__PMx_SLOT_VLV_CTRL_TYPE == _DEF__PMx_SLOT_VLV_TYPE__OBJ)
			{
				aEXT_CH__PARA_PMC_ID->Set__VALUE(pmc_id);

				int r_flag = pOBJ_CTRL__PMx_HANDOFF->Call__OBJECT(sMODE__PMx_SLOT_VLV__OPEN);
				if(r_flag < 0)			return -101;
			}
			else
			{
				if((diEXT_CH__PMx__SV_OPEN[pm_index]->Check__DATA(STR__ON)   > 0)
				&& (diEXT_CH__PMx__SV_CLOSE[pm_index]->Check__DATA(STR__OFF) > 0))
				{
					Set_IO__SV_OPEN(pmc_id);

					// ...
					{
						str_log.Format("Already, the slot-valve of PM%d is open. ", pmc_id);
						Fnc_App_Log(str_log);
					}
					return 1;
				}
	
				Set_IO__SV_OPEN(pmc_id);
			}
		}
	}

	// ...
	SCX__ASYNC_TIMER_CTRL xI_ASYNC_TIMER_SV;

	double sec_timecount = cfg_timeout + 10.0;
	int r_ret = -1;

	xI_ASYNC_TIMER_SV->REGISTER__COUNT_CHANNEL(sObject_Name,aCH_SV_OPEN__TIME_COUNT[pm_index]->Get__VARIABLE_NAME());
	xI_ASYNC_TIMER_SV->START__COUNT_UP(sec_timecount);

	while(1)
	{
		Sleep(1);

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			r_ret = -1;
			break;
		}

		if(dEXT_CH__CFG_PMx_SV_USE[pm_index]->Check__DATA(STR__NO) > 0)
		{
			r_ret = 1;
			break;
		}

		if((diEXT_CH__PMx__SV_OPEN[pm_index]->Check__DATA(STR__ON)   > 0)
		&& (diEXT_CH__PMx__SV_CLOSE[pm_index]->Check__DATA(STR__OFF) > 0))
		{
			// ...
			{
				str_log.Format("OK... PM%d Slot Valve... PM_OPEN sts.. ", pmc_id);
				Fnc_App_Log(str_log);
			}

			r_ret = 1;
			break;
		}

		if(xI_ASYNC_TIMER_SV->Get__CURRENT_TIME() >= cfg_timeout)
		{
			// ...
			{
				str_log.Format("PM_OPEN Timeout Occur... PM%d Slot Valve...", pmc_id);
				Fnc_App_Log(str_log);
			}

			r_ret = 0; // Timeout Occur !!
			break;
		}
	}

	return r_ret;
}

int  CObj__PMC_SIMPLE
::_Update__ACTION_MIN_MAX(const int db_i,
						  const int pm_i,
						  const double cur_sec)
{
	CII__VAR_STRING_CTRL* p_ch_now = sCH__TAS_ACTION_TIME_NOW__PMx[db_i][pm_i].Get__PTR();
	CII__VAR_STRING_CTRL* p_ch_min = sCH__TAS_ACTION_TIME_MIN__PMx[db_i][pm_i].Get__PTR();
	CII__VAR_STRING_CTRL* p_ch_max = sCH__TAS_ACTION_TIME_MAX__PMx[db_i][pm_i].Get__PTR();

	CII__VAR_STRING_CTRL* p_ch_avg = sCH__TAS_ACTION_TIME_AVG__PMx[db_i][pm_i].Get__PTR();
	CII__VAR_STRING_CTRL* p_ch_avg_f = sCH__TAS_ACTION_TIME_AVG_F__PMx[db_i][pm_i].Get__PTR();
	CII__VAR_ANALOG_CTRL* p_ch_cnt = aCH__TAS_ACTION_TIME_CNT__PMx[db_i][pm_i].Get__PTR();

	// ...
	CString ch_data;
	CString str_cur;

	str_cur.Format("%.1f", cur_sec);
	p_ch_now->Set__DATA(str_cur);

	// Avg ...
	{
		ch_data = p_ch_avg_f->Get__STRING();
		double pre_avg = atof(ch_data);

		double n_count = p_ch_cnt->Get__VALUE();
		p_ch_cnt->Inc__VALUE();

		double cur_avg;
		if(n_count > 0)		cur_avg = (pre_avg * (n_count - 1) / n_count) + (cur_sec / n_count);
		else				cur_avg = cur_sec;

		ch_data.Format("%.5f", cur_avg);
		p_ch_avg_f->Set__DATA(ch_data);

		ch_data.Format("%.1f", cur_avg);
		p_ch_avg->Set__DATA(ch_data);
	}
	// Min ...
	{
		ch_data = p_ch_min->Get__STRING();

		if(ch_data.GetLength() < 1)
		{
			p_ch_min->Set__DATA(str_cur);
		}
		else
		{
			double cur_min = atof(ch_data);

			if(cur_sec < cur_min)		p_ch_min->Set__DATA(str_cur);
		}
	}
	// Max ...
	{
		ch_data = p_ch_max->Get__STRING();

		if(ch_data.GetLength() < 1)
		{
			p_ch_max->Set__DATA(str_cur);
		}
		else
		{
			double cur_max = atof(ch_data);

			if(cur_sec > cur_max)		p_ch_max->Set__DATA(str_cur);
		}
	}

	return 1;
}
