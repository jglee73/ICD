#include "StdAfx.h"
#include "CObj__PMC_STD.h"
#include "CObj__PMC_STD__DEF.h"
#include "CObj__PMC_STD__ALID.h"

#include "math.h"


//----------------------------------------------------------------------------------------------------
// CHECK SLIT VLV ALL CLOSE -----
int  CObj__PMC_STD
::Call__CHECK_SV_PM_ALL_CLOSE(CII_OBJECT__VARIABLE* p_variable,
							  CII_OBJECT__ALARM* p_alarm)
{
	return Fnc__CHECK_SV_PM_ALL_CLOSE(p_variable,p_alarm);
}

int  CObj__PMC_STD
::Fnc__CHECK_SV_PM_ALL_CLOSE(CII_OBJECT__VARIABLE* p_variable,
						     CII_OBJECT__ALARM* p_alarm)
{
	DECLARE__EXT_CTRL(p_variable);

	// ...
	CString msg;
	CString bff;

	CStringArray l_err_msg;
	CString err_msg;

	int pmc_id;
	int i;

	for(i=0; i<m_nPM_LIMIT; i++)
	{
		pmc_id = i + 1;

		if(dEXT_CH__CFG_PMx_EXIST_FLAG[i]->Check__DATA("YES") < 0)
		{
			continue;
		}

		if(Is__SV_PM_CLOSE(p_variable, p_alarm, pmc_id) < 0)
		{
			err_msg.Format("PM%1d",pmc_id);
			l_err_msg.Add(err_msg);

			bff.Format("[%s]\n",err_msg);
			msg += bff;
		}
	}

	p_ext_mode_ctrl->Set__RETURN_FNC_PARAMETER(l_err_msg);
	return 1;
}


// INIT -----
int  CObj__PMC_STD
::Call__INIT(CII_OBJECT__VARIABLE* p_variable,
		     CII_OBJECT__ALARM* p_alarm)
{
	return Fnc__INIT(p_variable,p_alarm);
}

int  CObj__PMC_STD
::Fnc__INIT(CII_OBJECT__VARIABLE* p_variable,
		    CII_OBJECT__ALARM* p_alarm)
{
	return 1;
}


// SLIT VLV CLOSE -----
int  CObj__PMC_STD
::Call__SV_PM_CLOSE(CII_OBJECT__VARIABLE* p_variable,
				    CII_OBJECT__ALARM* p_alarm,
				    const int pmc_id)
{
	return Fnc__SV_PM_CLOSE(p_variable, p_alarm, pmc_id);
}
int  CObj__PMC_STD
::Fnc__SV_PM_CLOSE(CII_OBJECT__VARIABLE* p_variable,
				   CII_OBJECT__ALARM* p_alarm,
				   const int pmc_id,
				   const int close_check)
{
	CString str_log;
	int nRet = -1;

LOOP_RETRY:

	// ...
	int pm_index = pmc_id - 1;	

	if(pm_index <  0)				return -1;
	if(pm_index > m_nPM_LIMIT)		return -1;

	if(dEXT_CH__CFG_PMx_EXIST_FLAG[pm_index]->Check__DATA("YES") < 0)
	{
		CString r_act;
		CString str_msg;

		// ...
		{		
			str_msg.Format("Check.. PM%d SV PM CLOSE", pmc_id);
			Fnc_App_Log("PM Not Exist.. Config Value... Skip..");

			return OBJ_AVAILABLE;
		}

		// ...
		{
			int alarm_id = ALID__PM_NOT_EXIST_CONFIG_ALARAM;

			p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,str_msg, r_act);

			if(r_act.CompareNoCase("ABORT") == 0)
			{	
				return OBJ_ABORT;
			}
		}
	}

	if(Is__SV_FULL_OPEN(p_variable,p_alarm,pmc_id) < 0)
	{
		int alarm_id = ALID__TMC_PMC_PRESSURE_IS_NOT_SAME;
		CString act_msg = "\"SV_PM_CLOSE\" can not be executed. \n";

		int lock_flag = Check__SAME_PRESSURE_FOR_VAC_Or_ATM(p_alarm,pmc_id,alarm_id,act_msg);

		if(lock_flag < 0)
		{
			return OBJ_ABORT;
		}
	}

	// ...
	double  cfg_timeout = 0.0;
	CString var_data;

	aCH__CFG_VALVE_CLOSE_TIMEOUT[pm_index]->Get__DATA(var_data);
	cfg_timeout = atof(var_data);

	// 1. Up and PM_CLOSE이면, Close
	if(Is__SV_PM_CLOSE(p_variable, p_alarm, pmc_id) > 0)
	{
		return 1;
	}

	if(close_check > 0)
	{
		if(Is__SV_TM_CLOSE(p_variable, p_alarm, pmc_id) > 0)
		{
			return 1;
		}
	}

	if(Is_CLOSE_CONDITION(p_variable, p_alarm, "PM", pmc_id) < 0)
	{
		return -1;
	}

	// 3. Up ...
	{
		nRet = Sub__SV_UP(p_variable, p_alarm, pmc_id, cfg_timeout);
		// Sleep(300);			// HW 동작 time
	}

	// 4. TM 쪽 ... Open
	{
		if(nRet > 0) Sub__SV_TM_OPEN(p_variable, p_alarm, pmc_id, cfg_timeout);
		// Sleep(300);			// HW 동작 time
	}

	// 5. PM 쪽 ... Close
	{
		if(nRet > 0) Sub__SV_PM_CLOSE(p_variable, p_alarm, pmc_id, cfg_timeout);
		// Sleep(300);			// HW 동작 time

		if(nRet > 0)		return 1;		// FULL CLOSE Complete !!
		else if(nRet < 0)		return -1;		// Abort
		// == 0 : timeout !!
	}

	// ...
	{
		int alarm_id = ALID__SLOT_VALVE_PM_CLOSE__TIMEOUT;

		CString err_msg;
		CString r_act;

		err_msg.Format("PM%1d's Slit Valve Close TimeOut (%.0f)",cfg_timeout);	Fnc_App_Log(err_msg);	
		p_alarm->Popup__ALARM(alarm_id,r_act);

		if(r_act.CompareNoCase(ACT__RETRY) == 0)
		{
			goto LOOP_RETRY;
		}
	}

	return -1;
}

// ...
int  CObj__PMC_STD
::Call__SV_TM_CLOSE(CII_OBJECT__VARIABLE* p_variable,
				    CII_OBJECT__ALARM* p_alarm,
				    const int pmc_id)
{
	return Fnc__SV_TM_CLOSE(p_variable,p_alarm, pmc_id);
}

int  CObj__PMC_STD
::Fnc__SV_TM_CLOSE(CII_OBJECT__VARIABLE* p_variable,
				   CII_OBJECT__ALARM* p_alarm,
				   const int pmc_id)
{
	CString str_log;
	int nRet = -1;

LOOP_RETRY:

	// ...
	int pm_index = pmc_id - 1;	

	if(pm_index <  0)				return -1;
	if(pm_index > m_nPM_LIMIT)		return -1;

	// ...
	double  cfg_timeout = 0.0;
	CString var_data;

	if(dEXT_CH__CFG_PMx_EXIST_FLAG[pm_index]->Check__DATA("YES") < 0)
	{
		CString r_act;
		CString str_msg;

		str_msg.Format("Check.. PM%d SV TM CLOSE", pmc_id);
		Fnc_App_Log("PM Not Exist.. Config Value... Skip..");
		
		return OBJ_AVAILABLE;

		/*
		int alarm_id = ALID__PM_NOT_EXIST_CONFIG_ALARAM;

		p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,str_msg, r_act);

		if(r_act.CompareNoCase("ABORT") == 0)
		{	
			return SEQ_ABORT;
		}
		*/
	}

	if(Is__SV_FULL_OPEN(p_variable,p_alarm,pmc_id) < 0)
	{
		int alarm_id = ALID__TMC_PMC_PRESSURE_IS_NOT_SAME;
		CString act_msg = "\"SV_TM_CLOSE\" can not be executed. \n";

		int lock_flag = Check__SAME_PRESSURE_FOR_VAC_Or_ATM(p_alarm,pmc_id,alarm_id,act_msg);

		if(lock_flag < 0)
		{
			return OBJ_ABORT;
		}
	}

	// ...
	aCH__CFG_VALVE_CLOSE_TIMEOUT[pm_index]->Get__DATA(var_data);
	cfg_timeout = atof(var_data);

	// 1. Up and TM_CLOSE이면, Close
	if(Is__SV_TM_CLOSE(p_variable, p_alarm, pmc_id) > 0)
	{
		return 1;
	}

	if(Is_CLOSE_CONDITION(p_variable, p_alarm, "TM", pmc_id) < 0)
	{
		return -1;
	}

	// 3. Up ...
	{
		nRet = Sub__SV_UP(p_variable, p_alarm, pmc_id, cfg_timeout);
		// Sleep(300);			// HW 동작 time
	}

	// 4. PM 쪽 ... Open
	{
		if(nRet > 0) Sub__SV_PM_OPEN(p_variable, p_alarm, pmc_id, cfg_timeout);
		// Sleep(300);			// HW 동작 time
	}

	// 5. TM 쪽 ... Close
	{
		if(nRet > 0) Sub__SV_TM_CLOSE(p_variable, p_alarm, pmc_id, cfg_timeout);
		// Sleep(300);			// HW 동작 time

		if(nRet > 0)      return 1;		// FULL CLOSE Complete !!
		else if(nRet < 0) return -1;	// Abort
		// == 0 : timeout !!
	}

	// ...
	{
		int alarm_id = ALID__SLOT_VALVE_TM_CLOSE__TIMEOUT;

		CString err_msg;
		CString r_act;

		err_msg.Format("PM%1d's Slit Valve Close TimeOut (%.0f)",cfg_timeout);	Fnc_App_Log(err_msg);	
		p_alarm->Popup__ALARM(alarm_id,r_act);

		if(r_act.CompareNoCase(ACT__RETRY) == 0)
		{
			goto LOOP_RETRY;
		}
	}
	return -1;
}


// SLIT VLV ALL CLOSE -----
int  CObj__PMC_STD
::Call__SV_PM_ALL_CLOSE(CII_OBJECT__VARIABLE* p_variable,
					    CII_OBJECT__ALARM* p_alarm)
{
	int pmc_id;
	int i;

	for(i=0;i<m_nPM_LIMIT;i++)
	{
		pmc_id = i + 1;

		if(dEXT_CH__CFG_PMx_EXIST_FLAG[i]->Check__DATA("YES") < 0)
		{
			continue;
		}

		if(Fnc__SV_PM_CLOSE(p_variable,p_alarm,pmc_id) < 0)
		{
			return -1;
		}
	}
	return 1;
}

int  CObj__PMC_STD
::Call__SV_TM_ALL_CLOSE(CII_OBJECT__VARIABLE* p_variable,
					    CII_OBJECT__ALARM* p_alarm)
{
	int pmc_id;
	int i;

	for(i=0;i<m_nPM_LIMIT;i++)
	{
		pmc_id = i + 1;

		// ...
		if(dEXT_CH__CFG_PMx_EXIST_FLAG[i]->Check__DATA("YES") < 0)
		{
			continue;
		}

		if(Fnc__SV_PM_CLOSE(p_variable,p_alarm,pmc_id) < 0)
		{
			return -1;
		}
	}
	return 1;
}


// VLV OPEN -----
int  CObj__PMC_STD
::Call__SV_OPEN(CII_OBJECT__VARIABLE* p_variable,
			    CII_OBJECT__ALARM* p_alarm,
			    const int pmc_id)
{
	CString str_data;
	double dblMax_Diff_Press_mtorr;
	double dblTm_press_mtorr;
	double dblPm_press_mtorr;

LOOP_RETRY:

	// ...
	int pm_index = pmc_id - 1;	

	if(pm_index <  0)			return -1;
	if(pm_index > m_nPM_LIMIT)	return -1;

	if(dEXT_CH__CFG_PMx_EXIST_FLAG[pm_index]->Check__DATA("YES") < 0)
	{
		CString r_act;
		CString str_msg;

		// ...
		{
			str_msg.Format("Check.. PM%d SV OPEN", pmc_id);
			Fnc_App_Log("PM Not Exist.. Config Value... Skip..");

			return OBJ_AVAILABLE;
		}

		/*
		// ...
		{
			int alarm_id = ALID__PM_NOT_EXIST_CONFIG_ALARAM;

			p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,str_msg, r_act);

			if(r_act.CompareNoCase("ABORT") == 0)
			{	
				return OBJ_ABORT;
			}
		}
		*/
	}

	// ...
	int atm_mode = -1;

	if(dEXT_CH__CFG_SETUP_TEST_MODE->Check__DATA(STR__ENABLE) > 0)
	{
		if(dEXT_CH__CFG_SETUP_LLx_DOOR_SLOT_VLV_CONTROL->Check__DATA(STR__DISABLE) > 0)
		{
			if(Is__SV_FULL_OPEN(p_variable, p_alarm, pmc_id) > 0)
			{
				return 1;
			}
		}

		atm_mode = 1;
	}
	if(dEXT_CH__CFG_TRANSFER_MODE->Check__DATA(STR__ATM) > 0)
	{
		atm_mode = 1;
	}

	// 1. Up and TM_OPEN and PM_OPEN이면, Fully Open
	if(Is__SV_FULL_OPEN(p_variable, p_alarm, pmc_id) > 0)
	{
		return 1;
	}

	if(atm_mode > 0)
	{
		if(Check__PRESSURE_ATM(p_alarm, pmc_id, ALID__SV_OPEN__NOT_ATM_ERROR) < 0)
		{
			return -1;
		}
	}
	else
	{
		if(Check__PRESSURE_VAC(p_alarm, pmc_id, ALID__SV_OPEN__NOT_VAC_ERROR) < 0)
		{
			return -1;
		}

		// ---- VACUUM 일 때만, 체크한다.
		aCH__CFG_MAX_DIFF_PRESS[pm_index]->Get__DATA(str_data);
		dblMax_Diff_Press_mtorr = atof(str_data);		// 이미 mtorr..

		aCH__PRESSURE_TORR[pm_index]->Get__DATA(str_data);
		dblPm_press_mtorr = atof(str_data)*1000;		//mtorr

		aEXT_CH__PHY_TM__PRESS_TORR->Get__DATA(str_data);
		dblTm_press_mtorr = atof(str_data)*1000;		//mtorr

		if(dblMax_Diff_Press_mtorr <= fabs(dblPm_press_mtorr-dblTm_press_mtorr))
		{
			CString alarm_msg;
			CString alarm_bff;
			CString r_act;

			int alarm_id = ALID__MAX_DIFFER_PRESS_ALARM;

			alarm_msg.Format("The config pressure differ Max of PM%d is [%.3f] mtorr.\n",
							 pmc_id, dblMax_Diff_Press_mtorr);

			alarm_bff.Format("The pressure difference of TM and PM is [%.3f] mtorr.\n",
							 fabs(dblPm_press_mtorr-dblTm_press_mtorr));
			alarm_msg += alarm_bff;

			p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,alarm_msg,r_act);

			if(r_act.CompareNoCase("RETRY") == 0)
			{	
				goto LOOP_RETRY;
			}
			else if(r_act.CompareNoCase("ABORT") == 0)
			{	
				return OBJ_ABORT;
			}
		}
	}

	return Fnc__SV_OPEN(p_variable,p_alarm,pmc_id);
}

int  CObj__PMC_STD
::Call__SV_CLOSE(CII_OBJECT__VARIABLE* p_variable,
			     CII_OBJECT__ALARM* p_alarm,
			     const int pmc_id)
{
	return Fnc__SV_CLOSE(p_variable,p_alarm, pmc_id);
}

// ...
int  CObj__PMC_STD
::Call__INIT_SV_CLOSE(CII_OBJECT__VARIABLE* p_variable,
					  CII_OBJECT__ALARM* p_alarm,
					  const int pmc_id)
{
	return Fnc__SV_CLOSE(p_variable,p_alarm, pmc_id,1);
}

int  CObj__PMC_STD
::Fnc__SV_CLOSE(CII_OBJECT__VARIABLE* p_variable,
			    CII_OBJECT__ALARM* p_alarm,
			    const int pmc_id,
			    const int close_check)
{
	return Fnc__SV_PM_CLOSE(p_variable,p_alarm, pmc_id,close_check);
}
int  CObj__PMC_STD
::Fnc__SV_OPEN(CII_OBJECT__VARIABLE* p_variable,
			   CII_OBJECT__ALARM* p_alarm,
			   const int pmc_id)
{
	CString str_log;
	int nRet = -1;

LOOP_RETRY:

	// ...
	int pm_index = pmc_id - 1;	

	if(pm_index <  0)			return -1;
	if(pm_index > m_nPM_LIMIT)	return -1;

	// ...
	double  cfg_timeout = 0.0;
	CString var_data;

	aCH__CFG_VALVE_OPEN_TIMEOUT[pm_index]->Get__DATA(var_data);
	cfg_timeout = atof(var_data);

	// 1. Up and TM_OPEN and PM_OPEN이면, Fully Open
	if(Is__SV_FULL_OPEN(p_variable, p_alarm, pmc_id) > 0)
	{
		return 1;
	}

	// 2. Open ...
	{
		nRet = Sub__SV_PM_OPEN(p_variable, p_alarm, pmc_id, cfg_timeout);
		// Sleep(300);			// HW 동작 time

		if(nRet > 0)
		{
			Sub__SV_TM_OPEN(p_variable, p_alarm, pmc_id, cfg_timeout);
		}
		// Sleep(300);			// HW 동작 time
	}

	// 3. Down ...
	{
		if(nRet > 0) Sub__SV_DOWN(p_variable, p_alarm, pmc_id, cfg_timeout);
		// Sleep(500);			// HW 동작 time

		if(nRet > 0)      return 1;		// FULL OPEN Complete !!
		else if(nRet < 0) return -1;	// Abort
		// == 0 : timeout !!
	}

	// ...
	{
		int alarm_id = ALID__VALVE_OPEN__TIMEOUT;

		CString err_msg;
		CString r_act;

		err_msg.Format("PM%1d's Slit Valve Open TimeOut (%.0f)",cfg_timeout);
		Fnc_App_Log(err_msg);

		p_alarm->Popup__ALARM(alarm_id,r_act);

		if(r_act.CompareNoCase(ACT__RETRY) == 0)
		{
			goto LOOP_RETRY;
		}
	}
	return -1;
}

// CYCLE TEST ...
int  CObj__PMC_STD
::Call__CYCLE_TEST(CII_OBJECT__VARIABLE* p_variable,
				   CII_OBJECT__ALARM* p_alarm)
{
	CString para__pmc_id;
	CString para__sv_mode;
	int para__cycle_count;

	CString var_data;
	int i;

	sCH__CYCLE_TEST_CFG_PMC_ID->Get__DATA(para__pmc_id);
	sCH__CYCLE_TEST_CFG_SV_CLOSE_MODE->Get__DATA(para__sv_mode);

	aCH__CYCLE_TEST_CFG_CYCLE_COUNT->Get__DATA(var_data);
	para__cycle_count = atoi(var_data);

	// ...
	{
		int i__pmc_id = atoi(para__pmc_id);
		if(i__pmc_id < 1)		return -101;
	}

	for(i=0;i<para__cycle_count;i++)
	{	
		int n_count = i + 1;

		var_data.Format("%1d", n_count);
		sCH__CYCLE_TEST_CUR_CYCLE_COUNT->Set__DATA(var_data);

		// Open ...
		{
			var_data.Format("PM%s's SV(%s) Open - Start", para__pmc_id,para__sv_mode);
			sCH__CYCLE_TEST_MSG->Set__DATA(var_data);

			// ...
			{
				SCX__ASYNC_TIMER_CTRL x_async_timer;

				x_async_timer->REGISTER__COUNT_CHANNEL_NAME(sCH__CYCLE_TEST_OP_TIME_CUR->Get__CHANNEL_NAME());
				x_async_timer->START__COUNT_UP(9999);

				// Open Seq ...
				{
					int i__pmc_id = atoi(para__pmc_id);

					if(Call__SV_OPEN(p_variable,p_alarm,i__pmc_id) < 0)
					{
						var_data.Format("PM%s's SV(%s) Open - Abort", para__pmc_id,para__sv_mode);
						sCH__CYCLE_TEST_MSG->Set__DATA(var_data);	
						return -1;
					}
				}

				// ...
				{
					double cur_sec = x_async_timer->Get__CURRENT_TIME();
					x_async_timer->STOP();

					sCH__CYCLE_TEST_OP_TIME_AVG->Get__DATA(var_data);
					double pre_avg = atof(var_data);

					CII__VAR_STRING_CTRL* p_ch_min = sCH__CYCLE_TEST_OP_TIME_MIN.Get__PTR();
					CII__VAR_STRING_CTRL* p_ch_max = sCH__CYCLE_TEST_OP_TIME_MAX.Get__PTR();

					CString cur_avg = Get__AVG_DATA(n_count, cur_sec,pre_avg);
					sCH__CYCLE_TEST_OP_TIME_AVG->Set__DATA(cur_avg);
					Fnc__Check_MIN_MAX_DATA(n_count, p_ch_min,p_ch_max, cur_sec);
				}
			}

			var_data.Format("PM%s's SV(%s) Open - Complete", para__pmc_id,para__sv_mode);
			sCH__CYCLE_TEST_MSG->Set__DATA(var_data);
		}

		// ...
		{
			SCX__TIMER_CTRL x_timer_ctrl;
			double cfg_sec;

			aCH__CYCLE_TEST_CFG_DELAY_SEC->Get__DATA(var_data);
			cfg_sec = atof(var_data);

			x_timer_ctrl->REGISTER__COUNT_CHANNEL(sCH__CYCLE_TEST_CUR_DELAY_SEC->Get__CHANNEL_NAME());
			x_timer_ctrl->REGISTER__ABORT_OBJECT(sObject_Name);

			if(x_timer_ctrl->WAIT(cfg_sec) < 0)
			{
				return -11;
			}
		}

		// Close ...
		{
			var_data.Format("PM%s's SV(%s) Close - Start", para__pmc_id,para__sv_mode);
			sCH__CYCLE_TEST_MSG->Set__DATA(var_data);

			// ...
			{
				SCX__ASYNC_TIMER_CTRL x_async_timer;

				x_async_timer->REGISTER__COUNT_CHANNEL_NAME(sCH__CYCLE_TEST_CL_TIME_CUR->Get__CHANNEL_NAME());
				x_async_timer->START__COUNT_UP(9999);

				// Close Seq ...
				{
					int i__pmc_id = atoi(para__pmc_id);
					int flag = -1;

					if(para__sv_mode.CompareNoCase(STR__PMC) == 0)
					{
						flag = Call__SV_PM_CLOSE(p_variable,p_alarm,i__pmc_id);
					}
					else if(para__sv_mode.CompareNoCase(STR__TMC) == 0)
					{
						flag = Call__SV_TM_CLOSE(p_variable,p_alarm,i__pmc_id);
					}

					if(flag < 0)
					{
						var_data.Format("PM%s's SV(%s) Close - Abort", para__pmc_id,para__sv_mode);
						sCH__CYCLE_TEST_MSG->Set__DATA(var_data);
						return -2;
					}
				}

				// ...
				{
					double cur_sec = x_async_timer->Get__CURRENT_TIME();
					x_async_timer->STOP();

					sCH__CYCLE_TEST_CL_TIME_AVG->Get__DATA(var_data);
					double pre_avg = atof(var_data);

					CII__VAR_STRING_CTRL* p_ch_min = sCH__CYCLE_TEST_CL_TIME_MIN.Get__PTR();
					CII__VAR_STRING_CTRL* p_ch_max = sCH__CYCLE_TEST_CL_TIME_MAX.Get__PTR();

					CString cur_avg = Get__AVG_DATA(n_count, cur_sec,pre_avg);
					sCH__CYCLE_TEST_CL_TIME_AVG->Set__DATA(cur_avg);
					Fnc__Check_MIN_MAX_DATA(n_count, p_ch_min,p_ch_max, cur_sec);
				}
			}

			var_data.Format("PM%s's SV(%s) Close - Complete", para__pmc_id,para__sv_mode);
			sCH__CYCLE_TEST_MSG->Set__DATA(var_data);
		}

		// ...
		{
			SCX__TIMER_CTRL x_timer_ctrl;
			double cfg_sec;

			aCH__CYCLE_TEST_CFG_DELAY_SEC->Get__DATA(var_data);
			cfg_sec = atof(var_data);

			x_timer_ctrl->REGISTER__COUNT_CHANNEL(sCH__CYCLE_TEST_CUR_DELAY_SEC->Get__CHANNEL_NAME());
			x_timer_ctrl->REGISTER__ABORT_OBJECT(sObject_Name);

			if(x_timer_ctrl->WAIT(cfg_sec) < 0)
			{
				return -21;
			}
		}
	}

	return 1;
}

// ...
CString CObj__PMC_STD
::Get__AVG_DATA(const int n_count,
			    const double cur_data,
			    const double pre_avg)
{
	double cur_avg = (pre_avg * ((n_count-1) / ((double) n_count))) + (cur_data / n_count); 

	CString str_data;
	str_data.Format("%.3f", cur_avg);

	return str_data;	
}
void CObj__PMC_STD
::Fnc__Check_MIN_MAX_DATA(const int n_count,
						  CII__VAR_STRING_CTRL* p_ch_min,
						  CII__VAR_STRING_CTRL* p_ch_max,
						  const double cur_data)
{
	CString var_data;

	if(n_count == 1)
	{
		var_data.Format("%.2f", cur_data);

		p_ch_min->Set__DATA(var_data);
		p_ch_max->Set__DATA(var_data);
	}
	else
	{
		double min_data;
		double max_data;

		p_ch_min->Get__DATA(var_data);
		min_data = atof(var_data);

		p_ch_max->Get__DATA(var_data);
		max_data = atof(var_data);

		if(cur_data < min_data)
		{
			var_data.Format("%.2f", cur_data);
			p_ch_min->Set__DATA(var_data);
		}
		if(cur_data > max_data)
		{
			var_data.Format("%.2f", cur_data);
			p_ch_max->Set__DATA(var_data);
		}
	}	
}

void CObj__PMC_STD::
Fnc_App_Log(const CString str_log)
{
	xLOG_CTRL->WRITE__LOG(str_log);
}
