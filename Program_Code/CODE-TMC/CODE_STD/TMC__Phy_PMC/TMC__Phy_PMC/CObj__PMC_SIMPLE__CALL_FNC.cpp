#include "StdAfx.h"
#include "CObj__PMC_SIMPLE.h"
#include "CObj__PMC_SIMPLE__DEF.h"
#include "CObj__PMC_SIMPLE__ALID.h"

#include "math.h"


//----------------------------------------------------------------------------------------------------
// INIT -----
int  CObj__PMC_SIMPLE
::Call__INIT(CII_OBJECT__VARIABLE* p_variable,
			 CII_OBJECT__ALARM* p_alarm)
{
	return 1;
}

int  CObj__PMC_SIMPLE
::Call__INIT_SV_CLOSE(CII_OBJECT__VARIABLE* p_variable,
					  CII_OBJECT__ALARM* p_alarm, 
					  const int pmc_id)
{
	int r_flag = Fnc__SV_CLOSE(p_variable,p_alarm, pmc_id, 1.0);

	End_IO__SV_CLOSE(pmc_id);
	return r_flag;
}


// CHECK SLIT VLV ALL CLOSE -----
int  CObj__PMC_SIMPLE
::Call__CHECK_SV_ALL_CLOSE(CII_OBJECT__VARIABLE* p_variable,
						   CII_OBJECT__ALARM* p_alarm)
{
	DECLARE__EXT_CTRL(p_variable);

	// ...
	CString msg;
	CString bff;

	CStringArray l_err_msg;
	CString err_msg;

	for(int i=0; i<iPM_LIMIT; i++)
	{
		int pmc_id = i + 1;

		if(dEXT_CH__CFG_PMx_EXIST_FLAG[i]->Check__DATA(STR__YES) < 0)
		{
			continue;
		}

		if(Is__SV_CLOSE(p_variable, p_alarm, pmc_id) < 0)
		{
			err_msg.Format("PM%1d", pmc_id);
			l_err_msg.Add(err_msg);

			bff.Format("[%s]\n", err_msg);
			msg += bff;
		}
	}

	p_ext_mode_ctrl->Set__RETURN_FNC_PARAMETER(l_err_msg);
	return 1;
}


// ...
int  CObj__PMC_SIMPLE
::Call__SV_OPEN(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const int pmc_id)
{
	int r_flag = _Call__SV_OPEN(p_variable,p_alarm, pmc_id);

	End_IO__SV_OPEN(pmc_id);
	return r_flag;
}
int  CObj__PMC_SIMPLE
::_Call__SV_OPEN(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm, const int pmc_id)
{
LOOP_RETRY:

	// ...
	int pm_index = pmc_id - 1;	

	if(pm_index < 0)				return -1;
	if(pm_index > iPM_LIMIT)		return -1;

	if(dEXT_CH__CFG_PMx_EXIST_FLAG[pm_index]->Check__DATA(STR__YES) < 0)
	{
		int alarm_id = ALID__PM_NOT_EXIST_CONFIG_ALARAM;

		CString alm_msg;
		CString r_act;

		// ...
		{
			alm_msg.Format("Check.. PM%d SV OPEN", pmc_id);
			Fnc_App_Log("PM Not Exist.. Config Value... Skip..");
		}

		p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,alm_msg, r_act);

		if(r_act.CompareNoCase(ACT__ABORT) == 0)
		{	
			return OBJ_ABORT;
		}
	}

	if(Is__SV_OPEN(p_variable, p_alarm, pmc_id) > 0)
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
		}

		return 1;
	}

	if(dEXT_CH__CFG_TRANSFER_MODE->Check__DATA(STR__ATM) > 0)
	{
		if(Check__PRESSURE_ATM(p_alarm, pmc_id, ALID__SV_OPEN__NOT_ATM_ERROR) < 0)
		{
			return -1;
		}
	}
	else  // VAC Mode ...
	{
		if(Check__PRESSURE_VAC(p_alarm, pmc_id, ALID__SV_OPEN__NOT_VAC_ERROR) < 0)
		{
			return -1;
		}

		// ...
		double dblMax_Diff_Press_mtorr;
		double dblTm_press_mtorr;
		double dblPm_press_mtorr;
		CString str_data;

		// ...
		{
			aCH__CFG_MAX_DIFF_PRESS[pm_index]->Get__DATA(str_data);
			dblMax_Diff_Press_mtorr = atof(str_data);			// mtorr

			aCH__PRESSURE_TORR[pm_index]->Get__DATA(str_data);
			dblPm_press_mtorr = atof(str_data) * 1000;			// mtorr

			aEXT_CH__PHY_TM__PRESS_TORR->Get__DATA(str_data);
			dblTm_press_mtorr = atof(str_data) * 1000;			// mtorr
		}

		if(dblMax_Diff_Press_mtorr <= fabs(dblPm_press_mtorr - dblTm_press_mtorr))
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

			if(r_act.CompareNoCase(ACT__RETRY) == 0)
			{	
				goto LOOP_RETRY;
			}
			if(r_act.CompareNoCase(ACT__ABORT) == 0)
			{	
				return OBJ_ABORT;
			}
		}
	}

	// ...
	{
		double cfg_timeout = aCH__CFG_VALVE_OPEN_TIMEOUT[pm_index]->Get__VALUE();

		// ...
		SCX__ASYNC_TIMER_CTRL x_timer_ctrl;
		x_timer_ctrl->START__COUNT_UP(9999);

		int r_ret = Fnc__SV_OPEN(p_variable, p_alarm, pmc_id, cfg_timeout);

		if(r_ret > 0)
		{
			double cur_sec = x_timer_ctrl->Get__CURRENT_TIME();

			int db_i = _ACT_INDEX__SV_OPEN;
			_Update__ACTION_MIN_MAX(db_i,pm_index, cur_sec);
		}

		if(r_ret > 0)		return  1;
		if(r_ret < 0)		return -1;

		// ...
		{
			int alarm_id = ALID__VALVE_OPEN__TIMEOUT;

			CString err_msg;
			CString r_act;

			err_msg.Format("PM%1d's Slit Valve Open TimeOut (%.0f)", pmc_id, cfg_timeout);
			Fnc_App_Log(err_msg);

			p_alarm->Popup__ALARM_With_MESSAGE(alarm_id, err_msg, r_act);

			if(r_act.CompareNoCase(ACT__RETRY) == 0)
			{
				goto LOOP_RETRY;
			}
		}
	}
	return -1;
}

int  CObj__PMC_SIMPLE
::Call__SV_CLOSE(CII_OBJECT__VARIABLE* p_variable,
				 CII_OBJECT__ALARM* p_alarm,
				 const int para__pmc_id)
{
	if(para__pmc_id <  0)
	{
		for(int i=0; i<iPM_LIMIT; i++)
		{
			int pmc_id = i + 1;
			
			_Call__SV_CLOSE(p_variable,p_alarm, pmc_id, -1);
			
			End_IO__SV_CLOSE(pmc_id);
		}
		return 1;
	}
	
	// ...
	int r_flag = _Call__SV_CLOSE(p_variable,p_alarm, para__pmc_id, 1);

	End_IO__SV_CLOSE(para__pmc_id);
	return r_flag;
}
int  CObj__PMC_SIMPLE
::_Call__SV_CLOSE(CII_OBJECT__VARIABLE* p_variable,
				  CII_OBJECT__ALARM* p_alarm,
				  const int pmc_id,
				  const int alarm_check)
{
LOOP_RETRY:

	// ...
	int pm_index = pmc_id - 1;	

	if(pm_index <  0)				return -1;
	if(pm_index >= iPM_LIMIT)		return -1;

	if(dEXT_CH__CFG_PMx_EXIST_FLAG[pm_index]->Check__DATA(STR__YES) < 0)
	{
		if(alarm_check < 0)
		{
			return 1;
		}

		// ...
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
			CString r_act;

			p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,str_msg, r_act);

			if(r_act.CompareNoCase("ABORT") == 0)
			{	
				return OBJ_ABORT;
			}
		}
	}

	if(Is__CLOSE_CONDITION(p_variable, p_alarm, pmc_id) < 0)
	{
		return -1;
	}

	// ...
	{
		double cfg_timeout = aCH__CFG_VALVE_CLOSE_TIMEOUT[pm_index]->Get__VALUE();

		// ...
		SCX__ASYNC_TIMER_CTRL x_timer_ctrl;
		x_timer_ctrl->START__COUNT_UP(9999);

		int r_ret = Fnc__SV_CLOSE(p_variable, p_alarm, pmc_id, cfg_timeout);

		if(r_ret > 0)
		{
			double cur_sec = x_timer_ctrl->Get__CURRENT_TIME();

			int db_i = _ACT_INDEX__SV_CLOSE;
			_Update__ACTION_MIN_MAX(db_i,pm_index, cur_sec);
		}

			 if(r_ret > 0)		return 1;		// Complete
		else if(r_ret < 0)		return -1;		// Abort

		// Timeout ...
		{
			int alarm_id = ALID__SLOT_VALVE_PM_CLOSE__TIMEOUT;

			CString err_msg;
			CString r_act;

			err_msg.Format("PM%1d's Slit Valve Close TimeOut (%.0f)", pmc_id, cfg_timeout);	
			Fnc_App_Log(err_msg);	

			p_alarm->Popup__ALARM_With_MESSAGE(alarm_id, err_msg, r_act);

			if(r_act.CompareNoCase(ACT__RETRY) == 0)
			{
				goto LOOP_RETRY;
			}
		}
	}

	return -1;
}

// CYCLE TEST ...
int  CObj__PMC_SIMPLE
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

	for(i=0; i<para__cycle_count; i++)
	{	
		int n_count = i + 1;

		var_data.Format("%1d", n_count);
		sCH__CYCLE_TEST_CUR_CYCLE_COUNT->Set__DATA(var_data);

		// Open ...
		{
			var_data.Format("PM%s's SV(%s) Open - Start", para__pmc_id, para__sv_mode);
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
						var_data.Format("PM%s's SV(%s) Open - Abort", para__pmc_id, para__sv_mode);
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

			var_data.Format("PM%s's SV(%s) Open - Complete", para__pmc_id, para__sv_mode);
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
			var_data.Format("PM%s's SV(%s) Close - Start", para__pmc_id, para__sv_mode);
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
						flag = Call__SV_CLOSE(p_variable,p_alarm,i__pmc_id);
					}
					else if(para__sv_mode.CompareNoCase(STR__TMC) == 0)
					{
						flag = Call__SV_CLOSE(p_variable,p_alarm,i__pmc_id);
					}

					if(flag < 0)
					{
						var_data.Format("PM%s's SV(%s) Close - Abort", para__pmc_id, para__sv_mode);
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

			var_data.Format("PM%s's SV(%s) Close - Complete", para__pmc_id, para__sv_mode);
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


// TIME TEST ...
int CObj__PMC_SIMPLE
::Call__TIME_TEST(CII_OBJECT__VARIABLE* p_variable,
				  CII_OBJECT__ALARM* p_alarm)
{
	int i_count = 2000;
	int i;

	for(i=0; i<i_count; i++)
	{
		double cur_sec = 5.0;
		if(i > 1000)		cur_sec = 10.0;

		// OPEN ...
		{
			int act_i = _ACT_INDEX__SV_OPEN;

			for(int pm_i=0; pm_i<CFG_PMx__SIZE; pm_i++)
			{
				_Update__ACTION_MIN_MAX(act_i,pm_i, cur_sec);
			}
		}
		// CLOSE ...
		{
			int act_i = _ACT_INDEX__SV_CLOSE;

			for(int pm_i=0; pm_i<CFG_PMx__SIZE; pm_i++)
			{
				_Update__ACTION_MIN_MAX(act_i,pm_i, cur_sec);
			}
		}
	}

	return 1;
}


// ...
CString CObj__PMC_SIMPLE
::Get__AVG_DATA(const int n_count,
			    const double cur_data,
			    const double pre_avg)
{
	double cur_avg = (pre_avg * ((n_count-1) / ((double) n_count))) + (cur_data / n_count); 

	CString str_data;
	str_data.Format("%.3f", cur_avg);

	return str_data;	
}
void CObj__PMC_SIMPLE
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

void CObj__PMC_SIMPLE::
Fnc_App_Log(const CString str_log)
{
	xLOG_CTRL->WRITE__LOG(str_log);
}
