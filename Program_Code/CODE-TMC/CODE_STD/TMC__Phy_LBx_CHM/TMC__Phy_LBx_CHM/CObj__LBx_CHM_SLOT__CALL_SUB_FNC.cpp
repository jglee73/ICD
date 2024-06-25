#include "StdAfx.h"
#include "CObj__LBx_CHM_SLOT.h"
#include "CObj__LBx_CHM_SLOT__DEF.h"
#include "CObj__LBx_CHM_SLOT__ALID.h"

#include "math.h"


//----------------------------------------------------------------------------------------------------
int  CObj__LBx_CHM_SLOT
::Check__PRESSURE_ATM(CII_OBJECT__ALARM* p_alarm,const int alarm_id)
{
LOOP_RETRY:

	// ...
	double  cur_press;
	double  ref_press;
	CString var_data;
	CString var_prs_sts;
	CString str_log;
	CString str_TM_prss_sts;

	aiEXT_CH__LBx__PRESSURE_TORR->Get__DATA(var_data);
	cur_press = atof(var_data);

	aEXT_CH__CFG_REF_ATM_PRESSURE->Get__DATA(var_data);
	ref_press = atof(var_data);

	dCH__PRESSURE_STATUS->Get__DATA(var_prs_sts);

	dEXT_CH__PHY_TM__PRESS_STS->Get__DATA(str_TM_prss_sts);

	// ...
	{
		str_log.Format("Check Prs ATM: Reference Prs:%f, Current Prs:%f, Prs Sts:%s", 
			ref_press, 
			cur_press, 
			var_prs_sts);

		Fnc__LOG(str_log);
	}

	if((cur_press < ref_press)
	|| (var_prs_sts.CompareNoCase(STR__IO_ATM)     != 0)
	|| (str_TM_prss_sts.CompareNoCase(STR__IO_ATM) != 0))
	{
		CString alarm_msg;
		CString alarm_bff;
		CString r_act;

		alarm_msg.Format("1. The pressure of %s is %s Sts.\n",
			m_sLBx__MODULE_NAME, var_prs_sts);

		alarm_bff.Format("2. The pressure of TM is %s\n",
			str_TM_prss_sts);

		alarm_msg += alarm_bff;

		alarm_bff.Format("3. The pressure of %s is %.3f and The config pressure is %.3f.\n",
			m_sLBx__MODULE_NAME, cur_press, ref_press);
		alarm_msg += alarm_bff;

		alarm_bff.Format("The %s's pressure must be more than %.3f and ATM Sts..\n",
			m_sLBx__MODULE_NAME, ref_press);
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


bool CObj__LBx_CHM_SLOT::Is__ALL_DV_OPEN()
{
	bool active__all_open = true;

	for(int i=0; i<iLBx_SLOT_SIZE; i++)
	{
		if(diEXT_CH__LLx__DV_OPEN_X[i]->Check__DATA(STR__ON)   > 0)		continue;
		if(diEXT_CH__LLx__DV_CLOSE_X[i]->Check__DATA(STR__OFF) > 0)		continue;

		active__all_open = false;
		break;
	}

	return active__all_open;
}
bool CObj__LBx_CHM_SLOT::Is__ANY_DV_OPEN()
{
	bool active__any_open = false;

	for(int i=0; i<iLBx_SLOT_SIZE; i++)
	{
		if(diEXT_CH__LLx__DV_OPEN_X[i]->Check__DATA(STR__ON)   < 0)		continue;
		if(diEXT_CH__LLx__DV_CLOSE_X[i]->Check__DATA(STR__OFF) < 0)		continue;

		active__any_open = true;
		break;
	}

	return active__any_open;
}
bool CObj__LBx_CHM_SLOT::Is__SLOT_DV_OPEN()
{
	int para_id = (int)	aCH__PARA_SLOT_ID->Get__VALUE();
	int p_index = para_id - 1;

	if(p_index <  0)					return false;
	if(p_index >= iLBx_SLOT_SIZE)		return false;

	//
	if(diEXT_CH__LLx__DV_OPEN_X[p_index]->Check__DATA(STR__ON)   < 0)		return false;
	if(diEXT_CH__LLx__DV_CLOSE_X[p_index]->Check__DATA(STR__OFF) < 0)		return false;

	return true;
}
bool CObj__LBx_CHM_SLOT::Is__SLOT_DV_CLOSE()
{
	int para_id = (int)	aCH__PARA_SLOT_ID->Get__VALUE();
	int p_index = para_id - 1;

	if(p_index <  0)					return false;
	if(p_index >= iLBx_SLOT_SIZE)		return false;

	//
	if(diEXT_CH__LLx__DV_OPEN_X[p_index]->Check__DATA(STR__OFF) < 0)		return false;
	if(diEXT_CH__LLx__DV_CLOSE_X[p_index]->Check__DATA(STR__ON) < 0)		return false;

	return true;
}

int CObj__LBx_CHM_SLOT::Set_IO__DV_OPEN()
{
	int para_id = (int)	aCH__PARA_SLOT_ID->Get__VALUE();
	int p_index = para_id - 1;

	if(p_index <  0)					return -1;
	if(p_index >= iLBx_SLOT_SIZE)		return -2;

	// ...
	{
		doEXT_CH__LLx__DV_CLOSE_X[p_index]->Set__DATA(STR__OFF);
		doEXT_CH__LLx__DV_OPEN_X[p_index]->Set__DATA(STR__ON);
	}
	return 1;
}
int CObj__LBx_CHM_SLOT::End_IO__DV_OPEN()
{
	int para_id = (int)	aCH__PARA_SLOT_ID->Get__VALUE();
	int p_index = para_id - 1;

	if(p_index <  0)					return -1;
	if(p_index >= iLBx_SLOT_SIZE)		return -2;

	if(dCH__CFG_IO_OFF_MODE->Check__DATA(STR__ENABLE) > 0)
	{
		doEXT_CH__LLx__DV_CLOSE_X[p_index]->Set__DATA(STR__OFF);
		doEXT_CH__LLx__DV_OPEN_X[p_index]->Set__DATA(STR__OFF);
	}
	return 1;
}

int CObj__LBx_CHM_SLOT::Set_IO__DV_CLOSE()
{
	int para_id = (int)	aCH__PARA_SLOT_ID->Get__VALUE();
	int p_index = para_id - 1;

	if(p_index <  0)					return -1;
	if(p_index >= iLBx_SLOT_SIZE)		return -2;

	// ...
	{
		doEXT_CH__LLx__DV_OPEN_X[p_index]->Set__DATA(STR__OFF);
		doEXT_CH__LLx__DV_CLOSE_X[p_index]->Set__DATA(STR__ON);
	}
	return 1;
}
int CObj__LBx_CHM_SLOT::End_IO__DV_CLOSE()
{
	int para_id = (int)	aCH__PARA_SLOT_ID->Get__VALUE();
	int p_index = para_id - 1;

	if(p_index <  0)					return -1;
	if(p_index >= iLBx_SLOT_SIZE)		return -2;

	if(dCH__CFG_IO_OFF_MODE->Check__DATA(STR__ENABLE) > 0)
	{
		doEXT_CH__LLx__DV_OPEN_X[p_index]->Set__DATA(STR__OFF);
		doEXT_CH__LLx__DV_CLOSE_X[p_index]->Set__DATA(STR__OFF);
	}
	return 1;
}

bool CObj__LBx_CHM_SLOT::Is__ALL_SV_OPEN()
{
	bool active__all_open = true;

	for(int i=0; i<iLBx_SLOT_SIZE; i++)
	{
		if((diEXT_CH__LLx__SV_OPEN_X[i]->Check__DATA(STR__ON)   < 0)
		|| (diEXT_CH__LLx__SV_CLOSE_X[i]->Check__DATA(STR__OFF) < 0))
		{
			active__all_open = false;
			break;
		}
	}

	return active__all_open;
}
bool CObj__LBx_CHM_SLOT::Is__ALL_SV_CLOSE()
{
	bool active__all_close = true;

	for(int i=0; i<iLBx_SLOT_SIZE; i++)
	{
		if((diEXT_CH__LLx__SV_OPEN_X[i]->Check__DATA(STR__OFF) < 0)
		|| (diEXT_CH__LLx__SV_CLOSE_X[i]->Check__DATA(STR__ON) < 0))
		{
			active__all_close = false;
			break;
		}

		// ...
		{
			bool active__up_err = false;

			if(bActive_DI__LLx_SV_UP_X[i])
			{
				if(diEXT_CH__LLx_SV_UP_X[i]->Check__DATA(STR__ON) < 0)
					active__up_err = true;
			}
			if(bActive_DI__LLx_SV_DOWN_X[i])
			{
				if(diEXT_CH__LLx_SV_DOWN_X[i]->Check__DATA(STR__OFF) < 0)
					active__up_err = true;
			}

			if(active__up_err)
			{
				active__all_close = false;
				break;
			}
		}
	}

	return active__all_close;
}
bool CObj__LBx_CHM_SLOT::Is__ANY_SV_OPEN()
{
	bool active__any_open = false;

	for(int i=0; i<iLBx_SLOT_SIZE; i++)
	{
		if(diEXT_CH__LLx__SV_OPEN_X[i]->Check__DATA(STR__ON)   < 0)		continue;
		if(diEXT_CH__LLx__SV_CLOSE_X[i]->Check__DATA(STR__OFF) < 0)		continue;

		active__any_open = true;
		break;
	}

	return active__any_open;
}

bool CObj__LBx_CHM_SLOT::Is__SLOT_SV_OPEN()
{
	int para_id = (int)	aCH__PARA_SLOT_ID->Get__VALUE();
	int p_index = para_id - 1;

	return Is__SLOT_SV_OPEN(p_index);
}
bool CObj__LBx_CHM_SLOT::Is__SLOT_SV_OPEN(const int p_index, const bool active__down_check)
{
	if(p_index <  0)					return false;
	if(p_index >= iLBx_SLOT_SIZE)		return false;

	if(diEXT_CH__LLx__SV_OPEN_X[p_index]->Check__DATA(STR__ON)   < 0)		return false;
	if(diEXT_CH__LLx__SV_CLOSE_X[p_index]->Check__DATA(STR__OFF) < 0)		return false;

	if(active__down_check)		
	{
		return Is__SLOT_SV_DOWN(p_index);
	}
	return true;
}
bool CObj__LBx_CHM_SLOT::Is__SLOT_SV_CLOSE()
{
	int para_id = (int)	aCH__PARA_SLOT_ID->Get__VALUE();
	int p_index = para_id - 1;

	return Is__SLOT_SV_CLOSE(p_index);
}
bool CObj__LBx_CHM_SLOT::Is__SLOT_SV_CLOSE(const int p_index, const bool active__down_check)
{
	if(p_index <  0)					return false;
	if(p_index >= iLBx_SLOT_SIZE)		return false;

	if(diEXT_CH__LLx__SV_OPEN_X[p_index]->Check__DATA(STR__OFF)  < 0)		return false;
	if(diEXT_CH__LLx__SV_CLOSE_X[p_index]->Check__DATA(STR__ON) < 0)		return false;

	if(active__down_check)	
	{
		return Is__SLOT_SV_UP(p_index);
	}
	return true;
}

bool CObj__LBx_CHM_SLOT::Is__SLOT_SV_UP()
{
	int para_id = (int)	aCH__PARA_SLOT_ID->Get__VALUE();
	int p_index = para_id - 1;

	return Is__SLOT_SV_UP(p_index);
}
bool CObj__LBx_CHM_SLOT::Is__SLOT_SV_UP(const int p_index)
{
	if(p_index <  0)					return false;
	if(p_index >= iLBx_SLOT_SIZE)		return false;

	if(bActive_DI__LLx_SV_UP_X[p_index])
	{
		if(diEXT_CH__LLx_SV_UP_X[p_index]->Check__DATA(STR__ON) < 0)
			return false;
	}
	if(bActive_DI__LLx_SV_DOWN_X[p_index])
	{
		if(diEXT_CH__LLx_SV_DOWN_X[p_index]->Check__DATA(STR__OFF) < 0)
			return false;
	}

	return true;
}
bool CObj__LBx_CHM_SLOT::Is__SLOT_SV_DOWN()
{
	int para_id = (int)	aCH__PARA_SLOT_ID->Get__VALUE();
	int p_index = para_id - 1;

	return Is__SLOT_SV_DOWN(p_index);
}
bool CObj__LBx_CHM_SLOT::Is__SLOT_SV_DOWN(const int p_index)
{
	if(p_index <  0)					return false;
	if(p_index >= iLBx_SLOT_SIZE)		return false;

	if(bActive_DI__LLx_SV_UP_X[p_index])
	{
		if(diEXT_CH__LLx_SV_UP_X[p_index]->Check__DATA(STR__OFF) < 0)
			return false;
	}
	if(bActive_DI__LLx_SV_DOWN_X[p_index])
	{
		if(diEXT_CH__LLx_SV_DOWN_X[p_index]->Check__DATA(STR__ON) < 0)
			return false;
	}

	return true;
}

int CObj__LBx_CHM_SLOT::Set_IO__SV_OPEN(CII_OBJECT__VARIABLE* p_variable, 
										CII_OBJECT__ALARM* p_alarm,
										const double cfg_timeout)
{
	int para_id = (int)	aCH__PARA_SLOT_ID->Get__VALUE();
	int p_index = para_id - 1;

	if(p_index <  0)					return -1;
	if(p_index >= iLBx_SLOT_SIZE)		return -2;

	// DO : CLOSE -> OPEN 
	{
		doEXT_CH__LLx__SV_CLOSE_X[p_index]->Set__DATA(STR__OFF);
		doEXT_CH__LLx__SV_OPEN_X[p_index]->Set__DATA(STR__ON);
	}

	// DO : UP -> DOWN
	if((bActive_DO__LLx_SV_UP_X[p_index])
	|| (bActive_DO__LLx_SV_DOWN_X[p_index]))
	{
		// DI : OPEN CHECK
		{
			SCX__ASYNC_TIMER_CTRL x_timer_sv;

			x_timer_sv->REGISTER__COUNT_CHANNEL_NAME(aCH_SV_OPEN__TIME_COUNT->Get__CHANNEL_NAME());
			x_timer_sv->START__COUNT_UP(9999);
			
			while(1)
			{
				Sleep(100);

				if(p_variable->Check__CTRL_ABORT() > 0)
				{
					return -11;
				}

				if(Is__SLOT_SV_OPEN(p_index, false) > 0)
				{
					if(iActive__SIM_MODE > 0)
					{
						Sleep(1000);
					}
					break;
				}

				if(x_timer_sv->Get__CURRENT_TIME() >= cfg_timeout)
				{
					int alarm_id = ALID__SLIT_VALVE_OPEN__TIMEOUT;

					CString err_msg;
					CString r_act;

					err_msg.Format("%s's Slit Valve Open TimeOut (%.0f)", m_sLBx__MODULE_NAME, cfg_timeout);
					Fnc__LOG(err_msg);

					p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,err_msg, r_act);

					if(r_act.CompareNoCase(ACT__RETRY) != 0)
					{
						return -12;
					}
					
					x_timer_sv->STOP_ZERO();
					x_timer_sv->START__COUNT_UP(9999);
				}
			}
		}

		if(bActive_DO__LLx_SV_UP_X[p_index])		doEXT_CH__LLx_SV_UP_X[p_index]->Set__DATA(STR__OFF);
		if(bActive_DO__LLx_SV_DOWN_X[p_index])		doEXT_CH__LLx_SV_DOWN_X[p_index]->Set__DATA(STR__ON);
	}
	return 1;
}
int CObj__LBx_CHM_SLOT::End_IO__SV_OPEN()
{
	int para_id = (int)	aCH__PARA_SLOT_ID->Get__VALUE();
	int p_index = para_id - 1;

	if(p_index <  0)					return -1;
	if(p_index >= iLBx_SLOT_SIZE)		return -2;

	if((bActive_DO__LLx_SV_UP_X[p_index])
	|| (bActive_DO__LLx_SV_DOWN_X[p_index]))
	{

	}
	else
	{
		if(dCH__CFG_IO_OFF_MODE->Check__DATA(STR__ENABLE) > 0)
		{
			doEXT_CH__LLx__SV_CLOSE_X[p_index]->Set__DATA(STR__OFF);
			doEXT_CH__LLx__SV_OPEN_X[p_index]->Set__DATA(STR__OFF);
		}
	}
	return 1;
}

int CObj__LBx_CHM_SLOT::Set_IO__SV_CLOSE(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm, const double cfg_timeout)
{
	int para_id = (int)	aCH__PARA_SLOT_ID->Get__VALUE();
	int p_index = para_id - 1;

	if(p_index <  0)					return -1;
	if(p_index >= iLBx_SLOT_SIZE)		return -2;

	// DO : DOWN -> UP
	if((bActive_DO__LLx_SV_UP_X[p_index])
	|| (bActive_DO__LLx_SV_DOWN_X[p_index]))
	{
		if(bActive_DO__LLx_SV_UP_X[p_index])		doEXT_CH__LLx_SV_UP_X[p_index]->Set__DATA(STR__ON);
		if(bActive_DO__LLx_SV_DOWN_X[p_index])		doEXT_CH__LLx_SV_DOWN_X[p_index]->Set__DATA(STR__OFF);

		// DI : UP CHECK
		{
			SCX__ASYNC_TIMER_CTRL x_timer_sv;

			x_timer_sv->REGISTER__COUNT_CHANNEL_NAME(aCH_SV_OPEN__TIME_COUNT->Get__CHANNEL_NAME());
			x_timer_sv->START__COUNT_UP(9999);

			while(1)
			{
				Sleep(100);

				if(p_variable->Check__CTRL_ABORT() > 0)
				{
					return -11;
				}

				if(Is__SLOT_SV_UP(p_index) > 0)
				{
					if(iActive__SIM_MODE > 0)
					{
						Sleep(1000);
					}
					break;
				}

				if(x_timer_sv->Get__CURRENT_TIME() >= cfg_timeout)
				{
					int alarm_id = ALID__SLIT_VALVE_CLOSE__TIMEOUT;

					CString err_msg;
					CString r_act;

					err_msg.Format("%s's Slit Valve Close TimeOut (%.0f)", m_sLBx__MODULE_NAME,cfg_timeout);
					Fnc__LOG(err_msg);

					p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,err_msg, r_act);

					if(r_act.CompareNoCase(ACT__RETRY) != 0)
					{
						return -12;
					}

					x_timer_sv->STOP_ZERO();
					x_timer_sv->START__COUNT_UP(9999);
				}
			}
		}
	}

	// DO : OPEN -> CLOSE
	{
		doEXT_CH__LLx__SV_OPEN_X[p_index]->Set__DATA(STR__OFF);
		doEXT_CH__LLx__SV_CLOSE_X[p_index]->Set__DATA(STR__ON);
	}
	return 1;
}
int CObj__LBx_CHM_SLOT::End_IO__SV_CLOSE()
{
	int para_id = (int)	aCH__PARA_SLOT_ID->Get__VALUE();
	int p_index = para_id - 1;

	if(p_index <  0)					return -1;
	if(p_index >= iLBx_SLOT_SIZE)		return -2;

	if((bActive_DO__LLx_SV_UP_X[p_index])
	|| (bActive_DO__LLx_SV_DOWN_X[p_index]))
	{

	}
	else
	{
		if(dCH__CFG_IO_OFF_MODE->Check__DATA(STR__ENABLE) > 0)
		{
			doEXT_CH__LLx__SV_OPEN_X[p_index]->Set__DATA(STR__OFF);
			doEXT_CH__LLx__SV_CLOSE_X[p_index]->Set__DATA(STR__OFF);
		}
	}
	return 1;
}


int  CObj__LBx_CHM_SLOT
::Check__PRESSURE_ATM_TO_DV_OPEN(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm, const int alarm_id)
{
LOOP_RETRY:

	if(Is__ALL_DV_OPEN())
	{
		CString str_log;

		str_log.Format("Already... Open sts.. %s Door Valve.", m_sLBx__MODULE_NAME);
		Fnc__LOG(str_log);
		return 1;
	}

	// ...
	int count__error_check = 0;

	while(1)
	{
		double cur_press = aiEXT_CH__LBx__PRESSURE_TORR->Get__VALUE();
		double ref_press = aEXT_CH__CFG_REF_ATM_PRESSURE->Get__VALUE();

		CString cur__ll_press_sts = dCH__PRESSURE_STATUS->Get__STRING();

		// ...
		{
			CString log_msg;
			CString log_bff;

			log_msg = "Pressure State ... \n";

			log_bff.Format(" * Current pressure <- %.3f (torr) \n", cur_press);
			log_msg += log_bff;
			log_bff.Format(" * Ref.pressure <- %.3f (torr) \n", ref_press);
			log_msg += log_bff;
			log_bff.Format(" * Pressure.State <- [%s] \n", cur__ll_press_sts);
			log_msg += log_bff;

			Fnc__LOG(log_msg);
		}

		if((cur_press >= ref_press)
		&& (cur__ll_press_sts.CompareNoCase(STR__IO_ATM) == 0))
		{
			return 1;
		}
		else
		{
			count__error_check++;

			if(count__error_check > 30)
			{
				CString alarm_msg;
				CString alarm_bff;
				CString r_act;

				alarm_msg.Format("1. The pressure of %s is %s \n",
								  m_sLBx__MODULE_NAME, 
								  cur__ll_press_sts);

				alarm_bff.Format("2. The pressure of %s is %.3f and the config pressure is %.3f.\n",
								  m_sLBx__MODULE_NAME, 
								  cur_press, 
								  ref_press);
				alarm_msg += alarm_bff;

				alarm_bff.Format("The %s's pressure must be more than %.3f and ATM State \n",
								 m_sLBx__MODULE_NAME, 
								 ref_press);
				alarm_msg += alarm_bff;

				p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,alarm_msg,r_act);

				if(r_act.CompareNoCase("RETRY") == 0)
				{	
					goto LOOP_RETRY;
				}
				return -1;
			}
		}

		Sleep(90);

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			return -11;
		}
	}

	return 11;
}

int  CObj__LBx_CHM_SLOT
::Check__PRESSURE_VAC(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm)
{
LOOP_RETRY:

	if(Is__ALL_SV_OPEN())
	{
		CString log_msg;

		log_msg.Format("Already, the SV of %s is open !", m_sLBx__MODULE_NAME);

		Fnc__LOG(log_msg);
		return 1;
	}

	// ...
	int count__error_check = 0;

	while(1)
	{
		double cur_press = aiEXT_CH__LBx__PRESSURE_TORR->Get__VALUE();
		double ref_press = aEXT_CH__CFG_REF_VAC_PRESSURE->Get__VALUE();

		CString cur__ll_press_sts = dCH__PRESSURE_STATUS->Get__STRING();
		CString cur__tm_press_sts = dEXT_CH__PHY_TM__PRESS_STS->Get__STRING();

		// ...
		{
			CString log_msg;
			CString log_bff;

			log_msg = "Pressure Check ... \n";

			log_bff.Format(" * Current Pressure <- %.3f (torr) \n", cur_press);
			log_msg += log_bff;
			log_bff.Format(" * Ref-Pressure <- %.3f (torr) \n", ref_press);
			log_msg += log_bff;
			log_bff.Format(" * LLx Pressure State <- [%s] \n", cur__ll_press_sts);
			log_msg += log_bff;
			log_bff.Format(" * TMx Pressure State <- [%s] \n", cur__tm_press_sts);
			log_msg += log_bff;

			Fnc__LOG(log_msg);
		}

		if((cur_press <= ref_press)
		&& (cur__ll_press_sts.CompareNoCase(STR__IO_VAC) == 0)
		&& (cur__tm_press_sts.CompareNoCase(STR__IO_VAC) == 0))
		{
			return 1;
		}
		else
		{
			count__error_check++;

			if(count__error_check > 30)
			{
				int alarm_id = ALID__SV_OPEN__NOT_VAC_ERROR;

				CString alarm_msg;
				CString alarm_bff;
				CString r_act;

				alarm_msg.Format("1. The pressure of %s is %s \n",
								 m_sLBx__MODULE_NAME, 
								 cur__ll_press_sts);

				alarm_bff.Format("2. The pressure of TM is %s \n",
								 cur__tm_press_sts);
				alarm_msg += alarm_bff;

				alarm_bff.Format("3. The pressure of %s is %.3f and the config pressure is %.3f.\n",
								 m_sLBx__MODULE_NAME, 
								 cur_press, 
								 ref_press);
				alarm_msg += alarm_bff;

				alarm_bff.Format("The %s's pressure must be less than %.3f (torr) and VAC state.. \n",
								 m_sLBx__MODULE_NAME, 
								 ref_press);
				alarm_msg += alarm_bff;
	
				p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,alarm_msg,r_act);

				if(r_act.CompareNoCase("RETRY") == 0)
				{	
					goto LOOP_RETRY;
				}
				return -1;
			}
		}

		Sleep(90);

		if(p_variable->Check__CTRL_ABORT() > 0)	
		{
			return -11;
		}
	}

	return 11;
}

int  CObj__LBx_CHM_SLOT
::Check__LLx_LID__CLOSE(CII_OBJECT__ALARM* p_alarm,
						const CString& act_name)
{
LOOP_RETRY:

	// ...
	{
		bool active_check = true;

		for(int i=0; i<iData__LID_SIZE; i++)
		{
			if(diEXT_CH__LID_CLOSE_SNS_X[i]->Check__DATA(STR__ON) > 0)
				continue;

			active_check = false;
			break;
		}

		if(active_check)		return 1;
	}

	// ...
	{
		int alm_id = ALID__LLx_LID__NOT_CLOSE;
		CString alm_msg;
		CString r_act;

		alm_msg.Format("Can not %s, because LLx's lid is not closed !\n", act_name);

		p_alarm->Popup__ALARM_With_MESSAGE(alm_id, alm_msg, r_act);

		if(r_act.CompareNoCase(ACT__RETRY) == 0)
		{
			goto LOOP_RETRY;
		}
	}
	return -1;
}
int  CObj__LBx_CHM_SLOT
::Check__TRANSFER_VLV__CLOSE(CII_OBJECT__ALARM* p_alarm, const CString& act_name)
{
	int i;

	for(i=0; i<iLBx_SLOT_SIZE; i++)
	{
		dCH__DOOR_VALVE_STATUS_X[i]->Link__UPPER_OBJECT_ABORT(sObject_Name);
		dCH__DOOR_VALVE_STATUS_X[i]->Enable__UPPER_OBJECT_ABORT_CHECK();

		dCH__SLIT_VALVE_STATUS_X[i]->Link__UPPER_OBJECT_ABORT(sObject_Name);
		dCH__SLIT_VALVE_STATUS_X[i]->Enable__UPPER_OBJECT_ABORT_CHECK();
	}

LOOP_RETRY:

	// ...
	{
		Fnc__LOG("Door and Slit Valve Close Check.. Start");
	}

	if(iActive__SIM_MODE > 0)
	{
		/*
		for(i=0; i<iLBx_SLOT_SIZE; i++)
		{
			diEXT_CH__LLx__SV_OPEN_X[i]->Set__DATA(STR__OFF);
			diEXT_CH__LLx__SV_CLOSE_X[i]->Set__DATA(STR__ON);

			diEXT_CH__LLx__DV_OPEN_X[i]->Set__DATA(STR__OFF);
			diEXT_CH__LLx__DV_CLOSE_X[i]->Set__DATA(STR__ON);
		}
		*/
	}

	// AUTO.CLOSE : SV & DV ...
	if(dCH__CFG_AUTO_SV_DV_CLOSE_WHEN_PUMP_VENT->Check__DATA(STR__YES) > 0)
	{
		bool active__arm_retract = true;

		// VAC_ROBOT ...
		{
			if(bActive__VAC_RB_OBJ_CTRL)
			{
				if(dEXT_CH__VAC_RB__MON__LLx_ARM_STATE->Check__DATA(STR__RETRACT) < 0)
				{
					active__arm_retract = false;
				}
			}

			for(i=0; i<iData__VAC_RB_ARM_RNE; i++)
			{
				if(diEXT_CH__VAC_RB_RNE_X[i]->Check__DATA(sDATA__RNE_ON) < 0)
				{
					continue;
				}

				active__arm_retract = false;
				break;
			}
		}

		// ATM_ROBOT ...
		{
			if(bActive__ATM_RB_OBJ_CTRL)
			{
				if(dEXT_CH__ATM_RB__MON_ACTIVE_ACTION_TO_LL->Check__DATA(STR__ON) > 0)
				{
					active__arm_retract = false;
				}
			}

			for(i=0; i<iData__ATM_RB_ARM_RNE; i++)
			{
				if(diEXT_CH__ATM_RB_RNE_X[i]->Check__DATA(sDATA__RNE_ON) < 0)
				{
					continue;
				}

				active__arm_retract = false;
				break;
			}
		}

		if(active__arm_retract)
		{
			bool active__auto_close = false;

			CString log_msg;
			CString log_bff;

			log_msg = "AUTO.CLOSE (SV & DV) \n";

			for(i=0; i<iLBx_SLOT_SIZE; i++)
			{
				if((diEXT_CH__LLx__SV_OPEN_X[i]->Check__DATA(STR__OFF) < 0)
				|| (diEXT_CH__LLx__SV_CLOSE_X[i]->Check__DATA(STR__ON) < 0))
				{
					active__auto_close = true;

					// ...
					{
						log_bff.Format("SV(%1d).State ... \n", i+1);
						log_msg += log_bff;

						log_bff.Format("  * %s <- %s \n", 
										diEXT_CH__LLx__SV_OPEN_X[i]->Get__CHANNEL_NAME(),
										diEXT_CH__LLx__SV_OPEN_X[i]->Get__STRING());
						log_msg += log_bff;
						
						log_bff.Format("  * %s <- %s \n", 
										diEXT_CH__LLx__SV_CLOSE_X[i]->Get__CHANNEL_NAME(),
										diEXT_CH__LLx__SV_CLOSE_X[i]->Get__STRING());
						log_msg += log_bff;
					}
				}

				if((diEXT_CH__LLx__DV_OPEN_X[i]->Check__DATA(STR__OFF) < 0)
				|| (diEXT_CH__LLx__DV_CLOSE_X[i]->Check__DATA(STR__ON) < 0))
				{
					active__auto_close = true;

					// ...
					{
						log_bff.Format("DV(%1d).State ... \n", i+1);
						log_msg += log_bff;

						log_bff.Format("  * %s <- %s \n", 
										diEXT_CH__LLx__DV_OPEN_X[i]->Get__CHANNEL_NAME(),
										diEXT_CH__LLx__DV_OPEN_X[i]->Get__STRING());
						log_msg += log_bff;

						log_bff.Format("  * %s <- %s \n", 
										diEXT_CH__LLx__DV_CLOSE_X[i]->Get__CHANNEL_NAME(),
										diEXT_CH__LLx__DV_CLOSE_X[i]->Get__STRING());
						log_msg += log_bff;
					}
				}

				// ...
				{
					diEXT_CH__LLx__SV_OPEN_X[i]->Set__DATA(STR__OFF);
					diEXT_CH__LLx__SV_CLOSE_X[i]->Set__DATA(STR__ON);

					diEXT_CH__LLx__DV_OPEN_X[i]->Set__DATA(STR__OFF);
					diEXT_CH__LLx__DV_CLOSE_X[i]->Set__DATA(STR__ON);
				}
			}

			if(active__auto_close)
			{
				Fnc__LOG(log_msg);
			}
		}
	}

	// DV CHECK ...
	{
		Fnc__LOG("Door Valve Close Check..");

		for(i=0; i<iLBx_SLOT_SIZE; i++)
		{
			int r_val = dCH__DOOR_VALVE_STATUS_X[i]->When__DATA(STR__CLOSE, 5.0);	// 0:Aborted, -1:Timeout

			// ...
			{
				CString log_msg;

				log_msg.Format("DOOR VLV STS Ret:%d", r_val);
				Fnc__LOG(log_msg);
			}

			if(r_val < 0)
			{
				int alarm_id = ALID__DOOR_VALVE__NOT_CLOSE;

				CString alarm_msg;
				CString msg_bff;
				CString r_act;

				msg_bff.Format("Can not %s, Cause Door Valve Not Close Sts !!\n", act_name);
				alarm_msg = msg_bff;

				Fnc__LOG(alarm_msg);

				p_alarm->Popup__ALARM_With_MESSAGE(alarm_id, alarm_msg, r_act);

				if(r_act.CompareNoCase(ACT__RETRY) == 0)
				{
					goto LOOP_RETRY;
				}
				else if(r_act.CompareNoCase(ACT__ABORT) == 0)
				{
					return -1;
				}
			}
			else if(r_val == 0)
			{
				return OBJ_ABORT;
			}
		}
	}

	// SV CHECK ...
	{
		Fnc__LOG("Slit Valve Close Check..");

		for(i=0; i<iLBx_SLOT_SIZE; i++)
		{
			int r_val = dCH__SLIT_VALVE_STATUS_X[i]->When__DATA(STR__CLOSE, 5.0);

			// ...
			{
				CString log_msg;

				log_msg.Format("SLIT VLV STS Ret:%d", r_val);
				Fnc__LOG(log_msg);
			}

			if(r_val < 0)
			{
				int alarm_id = ALID__SLIT_VALVE__NOT_CLOSE;

				CString alarm_msg;
				CString r_act;

				alarm_msg.Format("Can not %s !\n", act_name);

				p_alarm->Popup__ALARM_With_MESSAGE(alarm_id, alarm_msg, r_act);

				if(r_act.CompareNoCase(ACT__RETRY) == 0)
				{
					goto LOOP_RETRY;
				}
				else if(r_act.CompareNoCase(ACT__ABORT) == 0)
				{
					return -1;
				}
			}
			else if(r_val == 0)
			{
				return OBJ_ABORT;
			}
		}
	}

	Fnc__LOG("Door and Slit Valve Close Check.. Complete");
	return 1;
}

int  CObj__LBx_CHM_SLOT
::Check__VENT_ALL_VLV__CLOSE(CII_OBJECT__ALARM* p_alarm)
{
	if(bActive__SOFT_VENT_VLV)
	{
		if(doEXT_CH__SOFT_VENT_VLV__SET->Check__DATA(STR__IO_Close) < 0)
			return -1;
	}

	if(doEXT_CH__FAST_VENT_VLV__SET->Check__DATA(STR__IO_Close) < 0)
	{
		return -1;
	}

	if(bActive__ATM_EQUAL_VLV)
	{
		if(doEXT_CH__ATM_EQUAL_VLV__SET->Check__DATA(STR__CLOSE) < 0)
			return -1;
	}

	return 1;
}
int  CObj__LBx_CHM_SLOT
::Check__PUMP_ALL_VLV__CLOSE(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm)
{
	if(doEXT_CH__SOFT_PUMP_VLV__SET->Check__DATA(STR__IO_Close) < 0)		return -1;
	if(doEXT_CH__FAST_PUMP_VLV__SET->Check__DATA(STR__IO_Close) < 0)		return -2;

	if(Check__SR_VALVE_CLOSE(p_variable,p_alarm, false) < 0)				return -11;
	if(Check__FR_VALVE_CLOSE(p_variable,p_alarm, false) < 0)				return -21;

	return 1;
}
int  CObj__LBx_CHM_SLOT
::Check__FR_VLV__OPEN(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm)
{
	if(doEXT_CH__FAST_PUMP_VLV__SET->Check__DATA(STR__IO_Open) > 0)			return 1;

	if(Check__FR_VALVE_OPEN(p_variable,p_alarm, false) > 0)					return 11;

	return -1;
}
int  CObj__LBx_CHM_SLOT
::Check__PUMP_VLV__OPEN(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm)
{
	if(doEXT_CH__SOFT_PUMP_VLV__SET->Check__DATA(STR__IO_Open) > 0)			return 1;
	if(doEXT_CH__FAST_PUMP_VLV__SET->Check__DATA(STR__IO_Open) > 0)			return 2;

	if(Check__SR_VALVE_OPEN(p_variable,p_alarm, false) > 0)					return 11;
	if(Check__FR_VALVE_OPEN(p_variable,p_alarm, false) > 0)					return 21;

	return -1;
}

void CObj__LBx_CHM_SLOT
::Update__PUMP_VLV_STS(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm)
{
	if(Check__PUMP_VLV__OPEN(p_variable, p_alarm) > 0)
	{
		sCH__PUMP_VLV_OPEN_FLAG->Set__DATA("YES");
	}
	else
	{
		sCH__PUMP_VLV_OPEN_FLAG->Set__DATA("");
	}
}

int  CObj__LBx_CHM_SLOT
::Is_SV_CLOSE_CONDITION(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	int i;

	for(i=0; i<iData__VAC_RB_ARM_RNE; i++)
	{
		diEXT_CH__VAC_RB_RNE_X[i]->Link__UPPER_OBJECT_ABORT(sObject_Name);
	}

LOOP_RETRY:

	if(iActive__SIM_MODE > 0)
	{
		for(i=0; i<iData__VAC_RB_ARM_RNE; i++)
		{
			diEXT_CH__VAC_RB_RNE_X[i]->Set__DATA(sDATA__RNE_ON);
		}
	}

	if(bActive__VAC_RB_OBJ_CTRL)
	{
		if(dEXT_CH__VAC_RB__MON__LLx_ARM_STATE->Check__DATA(STR__RETRACT) < 0)
		{
			int alarm_id = ALID__VAC_RB_NOT_RETRACTED;

			CString err_msg;
			CString r_act;

			err_msg.Format(" * %s <- %s \n", 
							dEXT_CH__VAC_RB__MON__LLx_ARM_STATE->Get__CHANNEL_NAME(),
							dEXT_CH__VAC_RB__MON__LLx_ARM_STATE->Get__STRING());
							

			p_alarm->Popup__ALARM_With_MESSAGE(alarm_id, err_msg, r_act);

			if(r_act.CompareNoCase(ACT__RETRY) == 0)		goto LOOP_RETRY;
			return -11;
		}
	}

	for(i=0; i<iData__VAC_RB_ARM_RNE; i++)
	{
		int nRet = diEXT_CH__VAC_RB_RNE_X[i]->When__DATA(sDATA__RNE_ON, 2);
		if(nRet == 0)	return OBJ_ABORT;

		CString var_data;
		diEXT_CH__VAC_RB_RNE_X[i]->Get__DATA(var_data);

		if(var_data.CompareNoCase(sDATA__RNE_ON) != 0)
		{
			int alarm_id = ALID__VAC_RB_NOT_RETRACTED;

			CString err_msg;
			CString r_act;

			err_msg.Format("VAC Robot's Arm [%s] Status", var_data);	
			Fnc__LOG(err_msg);	

			p_alarm->Popup__ALARM(alarm_id,r_act);

			if(r_act.CompareNoCase(ACT__RETRY) == 0)		goto LOOP_RETRY;
			return -12;
		}
	}

	// CHECK : WAFER.SLIDE SENSOR ...
	{
		bool active__wafer_out = false;

		// ...
		{
			for(int sns_i = 0; sns_i < iData__WAFER_SLIDE_OUT; sns_i++)
			{
				if(diEXT_CH__WAFER_SKIDE_OUT_X[sns_i]->Check__DATA(STR__OFF) > 0)			continue;

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

			int sns_size = iData__WAFER_SLIDE_OUT;
			for(int sns_i = 0; sns_i < sns_size; sns_i++)
			{
				alm_bff.Format("  %1d)  %s <- %s \n",
								sns_i + 1,
								diEXT_CH__WAFER_SKIDE_OUT_X[sns_i]->Get__CHANNEL_NAME(),
								diEXT_CH__WAFER_SKIDE_OUT_X[sns_i]->Get__STRING());
				alm_msg += alm_bff;
			}

			int alarm_id = ALID__WAFER_SLIDE_SENSOR_DETECTED;
			CString r_act;

			p_alarm->Popup__ALARM_With_MESSAGE(alarm_id, alm_msg, r_act);

			if(r_act.CompareNoCase(ACT__RETRY) == 0)		goto LOOP_RETRY;
			return -13;
		}
	}

	return 1;
}

int  CObj__LBx_CHM_SLOT
::Is_DV_CLOSE_CONDITION(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	CString var_data;
	CString str_log;
	int i;

	for(i=0; i<iData__ATM_RB_ARM_RNE; i++)
	{
		diEXT_CH__ATM_RB_RNE_X[i]->Link__UPPER_OBJECT_ABORT(sObject_Name);
	}

LOOP_RETRY:

	if(iActive__SIM_MODE > 0)
	{
		for(i=0; i<iData__ATM_RB_ARM_RNE; i++)
			diEXT_CH__ATM_RB_RNE_X[i]->Set__DATA(sDATA__RNE_ON);
	}

	if(bActive__ATM_RB_OBJ_CTRL)
	{
		if(dEXT_CH__ATM_RB__MON_ACTIVE_ACTION_TO_LL->Check__DATA(STR__ON) > 0)
		{
			int alarm_id = ALID__ATM_RB_NOT_RETRACTED;

			CString err_msg;
			CString r_act;

			err_msg.Format(" * %s <- %s \n", 
							dEXT_CH__ATM_RB__MON_ACTIVE_ACTION_TO_LL->Get__CHANNEL_NAME(),
							dEXT_CH__ATM_RB__MON_ACTIVE_ACTION_TO_LL->Get__STRING());

			p_alarm->Popup__ALARM_With_MESSAGE(alarm_id, err_msg, r_act);

			if(r_act.CompareNoCase(ACT__RETRY) == 0)
			{
				goto LOOP_RETRY;
			}
			return -11;
		}
	}

	for(i=0; i<iData__ATM_RB_ARM_RNE; i++)
	{
		int r_act = diEXT_CH__ATM_RB_RNE_X[i]->When__DATA(sDATA__RNE_ON, 2);
		if(r_act == 0)			return -11;

		// ...
		CString cur__arm_sts = diEXT_CH__ATM_RB_RNE_X[i]->Get__STRING();

		str_log.Format("ATM RB ARM[%s], [%s] Status...", 
						diEXT_CH__ATM_RB_RNE_X[i]->Get__VARIABLE_NAME(), 
						cur__arm_sts);
		Fnc__LOG(str_log);

		if(cur__arm_sts.CompareNoCase(sDATA__RNE_ON) != 0)
		{
			int alarm_id = ALID__ATM_RB_NOT_RETRACTED;

			CString err_msg;
			CString err_bff;
			CString r_act;

			err_msg.Format("ATM Robot's Arm [%s] Status \n", cur__arm_sts);
			err_bff.Format(" * Arm.Retract_State <- %s \n", sDATA__RNE_ON);
			err_msg += err_bff;

			Fnc__LOG(err_msg);	
			
			p_alarm->Popup__ALARM_With_MESSAGE(alarm_id, err_msg, r_act);

			if(r_act.CompareNoCase(ACT__RETRY) == 0)
			{
				goto LOOP_RETRY;
			}
			return -12;
		}
	}

	return 1;
}

int  CObj__LBx_CHM_SLOT
::Fnc__VENT_ALL_VLV__CLOSE(CII_OBJECT__ALARM* p_alarm)
{
	int flag_close = 1;

	Fnc__LOG("Set... Soft Vent Vlv [Close], Fast Vent Vlv [Close]");

	// 1.
	if(bActive__SOFT_VENT_VLV)
	{
		if(doEXT_CH__SOFT_VENT_VLV__SET->Check__DATA(STR__IO_Close) < 0)
			flag_close = -1;

		doEXT_CH__SOFT_VENT_VLV__SET->Set__DATA(STR__IO_Close);
	}

	// 2.
	if(doEXT_CH__FAST_VENT_VLV__SET->Check__DATA(STR__IO_Close) < 0)
	{
		flag_close = -1;
	}
	doEXT_CH__FAST_VENT_VLV__SET->Set__DATA(STR__IO_Close);

	// 3.
	if(bActive__ATM_EQUAL_VLV)
	{
		if(doEXT_CH__ATM_EQUAL_VLV__SET->Check__DATA(STR__CLOSE) < 0)
			flag_close = -1;

		doEXT_CH__ATM_EQUAL_VLV__SET->Set__DATA(STR__CLOSE);	
	}

	if(flag_close < 0)
	{
		SCX__TIMER_CTRL x_timer_ctrl;
		double delay_time;
		CString var_data;

		aCH__CFG_VENT_VALVE_CLOSE_DELAY_TIME->Get__DATA(var_data);
		delay_time = atof(var_data);
		if(delay_time < 0.3)	delay_time = 0.3;

		if(x_timer_ctrl->WAIT(delay_time) < 0)
		{
			return -1;
		}
	}

	return 1;
}
int  CObj__LBx_CHM_SLOT
::Fnc__VENT_ALL_VLV__CLOSE_WITHOUT_EQUAL_VLV(CII_OBJECT__ALARM* p_alarm)
{
	int flag_close = 1;

	// 1.
	if(bActive__SOFT_VENT_VLV)
	{
		if(doEXT_CH__SOFT_VENT_VLV__SET->Check__DATA(STR__IO_Close) < 0)
			flag_close = -1;

		doEXT_CH__SOFT_VENT_VLV__SET->Set__DATA(STR__IO_Close);
	}

	// 2.
	if(doEXT_CH__FAST_VENT_VLV__SET->Check__DATA(STR__IO_Close) < 0)
	{
		flag_close = -1;
	}
	doEXT_CH__FAST_VENT_VLV__SET->Set__DATA(STR__IO_Close);

	if(flag_close < 0)
	{
		SCX__TIMER_CTRL x_timer_ctrl;
		double delay_time;
		CString var_data;

		aCH__CFG_VENT_VALVE_CLOSE_DELAY_TIME->Get__DATA(var_data);
		delay_time = atof(var_data);
		if(delay_time < 0.3)	delay_time = 0.3;

		if(x_timer_ctrl->WAIT(delay_time) < 0)
		{
			return -1;
		}
	}
	return 1;
}
int  CObj__LBx_CHM_SLOT
::Fnc__PUMP_ALL_VLV__CLOSE(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm)
{
	int flag_close = 1;

	Fnc__LOG("Set... Soft Pump Vlv [Close], Fast Pump Vlv [Close]");

	// 1.
	if(doEXT_CH__SOFT_PUMP_VLV__SET->Check__DATA(STR__IO_Close) < 0)
	{
		flag_close = -1;
	}
	doEXT_CH__SOFT_PUMP_VLV__SET->Set__DATA(STR__IO_Close);

	// 2.
	if(doEXT_CH__FAST_PUMP_VLV__SET->Check__DATA(STR__IO_Close) < 0)
	{
		flag_close = -1;
	}
	doEXT_CH__FAST_PUMP_VLV__SET->Set__DATA(STR__IO_Close);

	if(flag_close < 0)
	{
		SCX__TIMER_CTRL x_timer_ctrl;
		double delay_time;
		CString var_data;

		aCH__CFG_PUMP_VALVE_CLOSE_DELAY_TIME->Get__DATA(var_data);
		delay_time = atof(var_data);
		if(delay_time < 0.3)	delay_time = 0.3;

		if(x_timer_ctrl->WAIT(delay_time) < 0)
		{
			return -11;
		}
	}

	if(Check__SR_VALVE_CLOSE(p_variable, p_alarm, true) < 0)		return -21;
	if(Check__FR_VALVE_CLOSE(p_variable, p_alarm, true) < 0)		return -31;

	return 1;
}

int  CObj__LBx_CHM_SLOT
::Fnc__PUMP_SOFT_VLV__OPEN(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm)
{
	doEXT_CH__SOFT_PUMP_VLV__SET->Set__DATA(STR__IO_Open);

	if(Check__SR_VALVE_OPEN(p_variable,p_alarm, true) < 0)			return -11;
	return 1;
}

int  CObj__LBx_CHM_SLOT
::Fnc__PUMP_SOFT_VLV__CLOSE(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm)
{
	doEXT_CH__SOFT_PUMP_VLV__SET->Set__DATA(STR__IO_Close);

	if(Check__SR_VALVE_CLOSE(p_variable,p_alarm, true) < 0)			return -11;
	return 1;
}

int  CObj__LBx_CHM_SLOT
::Fnc__PUMP_FAST_VLV__OPEN(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm)
{
	doEXT_CH__FAST_PUMP_VLV__SET->Set__DATA(STR__IO_Open);

	if(Check__FR_VALVE_OPEN(p_variable, p_alarm, true) < 0)			return -11;
	return 1;
}

int  CObj__LBx_CHM_SLOT
::Fnc__VENT_SOFT_VLV__OPEN(CII_OBJECT__ALARM* p_alarm)
{
	if(bActive__SOFT_VENT_VLV)
		doEXT_CH__SOFT_VENT_VLV__SET->Set__DATA(STR__IO_Open);

	return 1;
}
int  CObj__LBx_CHM_SLOT
::Fnc__VENT_SOFT_VLV__CLOSE(CII_OBJECT__ALARM* p_alarm)
{
	if(bActive__SOFT_VENT_VLV)
		doEXT_CH__SOFT_VENT_VLV__SET->Set__DATA(STR__IO_Close);
	
	return 1;
}

int  CObj__LBx_CHM_SLOT
::Fnc__VENT_FAST_VLV__OPEN(CII_OBJECT__ALARM* p_alarm)
{
	doEXT_CH__FAST_VENT_VLV__SET->Set__DATA(STR__IO_Open);
	return 1;
}

int  CObj__LBx_CHM_SLOT
::Sub__SV_OPEN(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const double cfg_timeout)
{
	CString str_log;

	if(Is__SLOT_SV_OPEN())
	{
		str_log.Format("Already... SV TM_OPEN sts.. %s Slot Valve.", m_sLBx__MODULE_NAME);
		Fnc__LOG(str_log);
		return 1;
	}

	int r_flag = Set_IO__SV_OPEN(p_variable, p_alarm, cfg_timeout);
	if(r_flag < 0)		return -11;

	// ...
	SCX__ASYNC_TIMER_CTRL x_timer_sv;

	x_timer_sv->REGISTER__COUNT_CHANNEL_NAME(aCH_SV_OPEN__TIME_COUNT->Get__CHANNEL_NAME());
	x_timer_sv->START__COUNT_UP(9999);

	while(1)
	{
		Sleep(3);

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			return -1;
		}

		if(Is__SLOT_SV_OPEN())
		{
			str_log.Format("%s SV TM_OPEN Completed..", m_sLBx__MODULE_NAME);	
			Fnc__LOG(str_log);
			return 1;
		}

		if(x_timer_sv->Get__CURRENT_TIME() >= cfg_timeout)
		{
			str_log.Format("%s SV TM_OPEN Timeout..", m_sLBx__MODULE_NAME);	
			Fnc__LOG(str_log);
			break;
		}
	}

	return 0;
}

int  CObj__LBx_CHM_SLOT
::Sub__SV_CLOSE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const double cfg_timeout)
{
	CString str_log;

	// 1. Already Sns Check..
	if(Is__SLOT_SV_CLOSE())
	{
		str_log.Format("Already... SV TM_CLOSE sts.. %s Slot Valve.", m_sLBx__MODULE_NAME);
		Fnc__LOG(str_log);
		return 1;
	}

	int r_flag = Set_IO__SV_CLOSE(p_variable, p_alarm, cfg_timeout);
	if(r_flag < 0)		return -11;

	// ...
	SCX__ASYNC_TIMER_CTRL x_timer_sv;

	x_timer_sv->REGISTER__COUNT_CHANNEL_NAME(aCH_SV_CLOSE__TIME_COUNT->Get__CHANNEL_NAME());
	x_timer_sv->START__COUNT_UP(9999);

	while(1)
	{
		Sleep(3);

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			return -1;
		}

		if(Is__SLOT_SV_CLOSE())
		{
			str_log.Format("%s SV TM_CLOSE Completed..", m_sLBx__MODULE_NAME);	
			Fnc__LOG(str_log);
			return 1;
		}

		if(x_timer_sv->Get__CURRENT_TIME() >= cfg_timeout)
		{
			str_log.Format("%s SV TM_CLOSE Timeout..", m_sLBx__MODULE_NAME);	
			Fnc__LOG(str_log);
			break;
		}
	}

	return 0;
}

int CObj__LBx_CHM_SLOT
::Check__SV_OPEN_MAX_DIFFERENTIAL_PRESS(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	double dblMax_Diff_Press_mtorr;
	double dblLLx_press_mtorr;
	double dblTm_press_mtorr;

	CString str_data;

LOOP_RETRY:

	// ...
	aCH__CFG_MAX_SV_OPEN_DIFF_PRESS->Get__DATA(str_data);
	dblMax_Diff_Press_mtorr = atof(str_data);				// mtorr..

	aiEXT_CH__LBx__PRESSURE_TORR->Get__DATA(str_data);
	dblLLx_press_mtorr = atof(str_data);					// torr

	aEXT_CH__PHY_TM__PRESS_TORR->Get__DATA(str_data);
	dblTm_press_mtorr = atof(str_data);						// torr

	if(dblMax_Diff_Press_mtorr < (fabs(dblLLx_press_mtorr-dblTm_press_mtorr)*1000)) 
	{
		CString alarm_msg;
		CString alarm_bff;
		CString r_act;

		int alarm_id = ALID__MAX_DIFFER_PRESS_ALARM;

		alarm_msg.Format("The config pressure differ Max of %s is [%.3f] mtorr.\n",
							m_sLBx__MODULE_NAME, dblMax_Diff_Press_mtorr);

		alarm_bff.Format("The pressure difference of TM and %s is [%.3f] mtorr.\n",
							m_sLBx__MODULE_NAME, fabs(dblLLx_press_mtorr-dblTm_press_mtorr)*1000);
		alarm_msg += alarm_bff;

		p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,alarm_msg,r_act);

		if(r_act.CompareNoCase("RETRY") == 0)
		{	
			goto LOOP_RETRY;
		}
		if(r_act.CompareNoCase("ABORT") == 0)
		{	
			return OBJ_ABORT;
		}
	}

	return 1;
}

void CObj__LBx_CHM_SLOT::Fnc__MSG(const CString &sMsg)
{
	sCH__MSG->Set__DATA(sMsg);
}
void CObj__LBx_CHM_SLOT::Fnc__LOG(const CString &sMsg)
{
	xLOG_CTRL->WRITE__LOG(sMsg);
}


int  CObj__LBx_CHM_SLOT
::_Update__ACTION_MIN_MAX(const int db_i,const int slot_i, const double cur_sec)
{
	if((slot_i >= 0) && (slot_i < iLBx_SLOT_SIZE))
	{
		_Fnc__ACTION_MIN_MAX(db_i,slot_i, cur_sec);
		return 1;
	}

	if(slot_i < 0)
	{
		for(int k=0; k<iLBx_SLOT_SIZE; k++)
		{
			_Fnc__ACTION_MIN_MAX(db_i,k, cur_sec);
		}
	}
	return 1;
}
int  CObj__LBx_CHM_SLOT
::_Fnc__ACTION_MIN_MAX(const int db_i,const int slot_i, const double cur_sec)
{
	CII__VAR_STRING_CTRL* p_ch_now = sCH__TAS_ACTION_TIME_NOW_SLOT[db_i][slot_i].Get__PTR();
	CII__VAR_STRING_CTRL* p_ch_min = sCH__TAS_ACTION_TIME_MIN_SLOT[db_i][slot_i].Get__PTR();
	CII__VAR_STRING_CTRL* p_ch_max = sCH__TAS_ACTION_TIME_MAX_SLOT[db_i][slot_i].Get__PTR();

	CII__VAR_STRING_CTRL* p_ch_avg = sCH__TAS_ACTION_TIME_AVG_SLOT[db_i][slot_i].Get__PTR();
	CII__VAR_STRING_CTRL* p_ch_avg_f = sCH__TAS_ACTION_TIME_AVG_F_SLOT[db_i][slot_i].Get__PTR();
	CII__VAR_ANALOG_CTRL* p_ch_cnt = aCH__TAS_ACTION_TIME_CNT_SLOT[db_i][slot_i].Get__PTR();

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


// ...
int  CObj__LBx_CHM_SLOT
::Check__SR_VALVE_CLOSE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const bool active__wait_check)
{
	return _Check__SR_VALVE_STATE(p_variable,p_alarm, active__wait_check, true);
}
int  CObj__LBx_CHM_SLOT
::Check__SR_VALVE_OPEN(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const bool active__wait_check)
{
	return _Check__SR_VALVE_STATE(p_variable,p_alarm, active__wait_check, false);
}
int  CObj__LBx_CHM_SLOT
::_Check__SR_VALVE_STATE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const bool active__wait_check, const bool active__close_check)
{
LOOP_RETRY:

	// SR.VLV : CLOSE CHECK ...
	int ref__error_count = 100;

	while(1)
	{
		bool active__sr_close = true;

		// SR.VLV ...
		{
			if(bActive__DI_SR_VLV_CLOSE)
			{
				if(diEXT_CH__DI_SR_VLV_CLOSE->Check__DATA(STR__ON) < 0)			active__sr_close = false;
			}
			if(bActive__DI_SR_VLV_OPEN)
			{
				if(diEXT_CH__DI_SR_VLV_OPEN->Check__DATA(STR__OFF) < 0)			active__sr_close = false;
			}
		}

		if(active__sr_close)
		{
			return 1;
		}

		if(active__wait_check)
		{
			ref__error_count--;
			if(ref__error_count < 1)
			{
				// Alarm 
				int alm_id;
				CString alm_msg;
				CString alm_bff;
				CString r_act;

				if(active__close_check)			alm_id = ALID__SR_VALVE_NOT_CLOSE;
				else							alm_id = ALID__SR_VALVE_NOT_OPEN;

				alm_msg = "SR.Valve Sensor State  ... \n";

				if(bActive__DI_SR_VLV_CLOSE)
				{
					alm_bff.Format(" * %s <- %s \n",
									diEXT_CH__DI_SR_VLV_CLOSE->Get__CHANNEL_NAME(),
									diEXT_CH__DI_SR_VLV_CLOSE->Get__STRING());
					alm_msg += alm_bff;
				}
				if(bActive__DI_SR_VLV_OPEN)
				{
					alm_bff.Format(" * %s <- %s \n",
									diEXT_CH__DI_SR_VLV_OPEN->Get__CHANNEL_NAME(),
									diEXT_CH__DI_SR_VLV_OPEN->Get__STRING());
					alm_msg += alm_bff;
				}

				p_alarm->Popup__ALARM_With_MESSAGE(alm_id,alm_msg,r_act);

				if(r_act.CompareNoCase(ACT__RETRY) == 0)
				{
					goto LOOP_RETRY;
				}
				return -11;
			}
		}
		else
		{
			return -1;
		}

		Sleep(50);

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			return -21;
		}
	}

	return -31;
}

int  CObj__LBx_CHM_SLOT
::Check__FR_VALVE_CLOSE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const bool active__wait_check)
{
	return _Check__FR_VALVE_STATE(p_variable,p_alarm, active__wait_check, true);
}
int  CObj__LBx_CHM_SLOT
::Check__FR_VALVE_OPEN(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const bool active__wait_check)
{
	return _Check__FR_VALVE_STATE(p_variable,p_alarm, active__wait_check, false);
}
int  CObj__LBx_CHM_SLOT
::_Check__FR_VALVE_STATE(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const bool active__wait_check, const bool active__close_check)
{
LOOP_RETRY:

	// FR.VLV : CLOSE CHECK ...
	int ref__error_count = 100;

	while(1)
	{
		bool active__fr_close = true;
		bool active__fr_open  = true;

		// FR.VLV ...
		{
			if(bActive__DI_FR_VLV_CLOSE)
			{
				if(diEXT_CH__DI_FR_VLV_CLOSE->Check__DATA(STR__ON) < 0)			active__fr_close = false;
				else															active__fr_open  = false;
			}
			if(bActive__DI_FR_VLV_OPEN)
			{
				if(diEXT_CH__DI_FR_VLV_OPEN->Check__DATA(STR__OFF) < 0)			active__fr_close = false;
				else															active__fr_open  = false;
			}
		}

		if(active__close_check)
		{
			if(active__fr_close)			return 1;
		}
		else
		{
			if(active__fr_open)				return 1;
		}

		if(active__wait_check)
		{
			ref__error_count--;
			if(ref__error_count < 1)
			{
				// Alarm 
				int alm_id;
				CString alm_msg;
				CString alm_bff;
				CString r_act;

				if(active__close_check)			alm_id = ALID__FR_VALVE_NOT_CLOSE;
				else							alm_id = ALID__FR_VALVE_NOT_OPEN;

				alm_msg = "FR.Valve Sensor State  ... \n";

				if(bActive__DI_FR_VLV_CLOSE)
				{
					alm_bff.Format(" * %s <- %s \n",
									diEXT_CH__DI_FR_VLV_CLOSE->Get__CHANNEL_NAME(),
									diEXT_CH__DI_FR_VLV_CLOSE->Get__STRING());
					alm_msg += alm_bff;
				}
				if(bActive__DI_FR_VLV_OPEN)
				{
					alm_bff.Format(" * %s <- %s \n",
									diEXT_CH__DI_FR_VLV_OPEN->Get__CHANNEL_NAME(),
									diEXT_CH__DI_FR_VLV_OPEN->Get__STRING());
					alm_msg += alm_bff;
				}

				p_alarm->Popup__ALARM_With_MESSAGE(alm_id,alm_msg,r_act);

				if(r_act.CompareNoCase(ACT__RETRY) == 0)
				{
					goto LOOP_RETRY;
				}
				return -11;
			}
		}
		else
		{
			return -1;
		}

		Sleep(50);

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			return -21;
		}
	}

	return -31;
}
