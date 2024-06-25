#include "StdAfx.h"
#include "CObj__VAC_ROBOT_STD.h"
#include "CObj__VAC_ROBOT_STD__DEF.h"
#include "CObj__VAC_ROBOT_STD__ALID.h"

#include "Macro_Function.h"


//------------------------------------------------------------------------------------
int  CObj__VAC_ROBOT_STD
::Interlock__CHECK_VALVE_OPEN(CII_OBJECT__ALARM* p_alarm,
							  const CString& arm_type,
							  const CString& stn_name,
							  const CString& stn_slot,
							  const CString& act_name)
{
	CStringArray l__arm_type;
	CStringArray l__stn_name;
	CStringArray l__stn_slot;

	_Get__ARM_INFO(arm_type,stn_name,stn_slot, l__arm_type,l__stn_name,l__stn_slot);

	int i_limit = l__arm_type.GetSize();
	for(int i=0; i<i_limit; i++)
	{
		CString cur__arm_type = l__arm_type[i];
		CString cur__stn_name = l__stn_name[i];
		CString cur__stn_slot = l__stn_slot[i];

		int r_flag = _Interlock__CHECK_VALVE_OPEN(p_alarm, cur__arm_type,cur__stn_name,cur__stn_slot, act_name);
		if(r_flag < 0)		return r_flag;
	}
	return 1;
}
int  CObj__VAC_ROBOT_STD
::_Interlock__CHECK_VALVE_OPEN(CII_OBJECT__ALARM* p_alarm,
							   const CString& arm_type,
							   const CString& stn_name,
							   const CString& stn_slot,
							   const CString& act_name)
{
LOOP__RETRY:

	// ...
	bool active__arm_a = false;
	bool active__arm_b = false;

		 if(arm_type.CompareNoCase(_ARM_A) == 0)			active__arm_a = true;
	else if(arm_type.CompareNoCase(_ARM_B) == 0)			active__arm_b = true;

	// ...
	int ll_i = Macro__CHECK_LLx_INDEX(stn_name);
	if(ll_i >= 0)
	{
		int s_index = atoi(stn_slot) - 1;

		if(s_index <  0)						return -11;
		if(s_index >= iLLx_SLOT_SIZE)			return -12;

		// ...
		CII__VAR_DIGITAL_CTRL *p_lb_door;

		if(bActive__LLx_MULTI_SLOT_VALVE)
		{
			p_lb_door = dEXT_CH__LLx_SLIT_VALVE_STATUS_XX[ll_i][s_index].Get__PTR();
		}
		else
		{
			p_lb_door = dEXT_CH__LLx_SLIT_VALVE_STATUS_A[ll_i].Get__PTR();
		}

		if(p_lb_door->When__DATA(STR__OPEN, 2.0) == 0)
		{
			return -13;
		}

		// ...
		bool active__sv_err = true;

		SCX__ASYNC_TIMER_CTRL x_timer_ctrl;
		double ref_sec = 10.0;

		x_timer_ctrl->START__COUNT_UP(9999);

		int check_count = 0;
		int ref_count = 10;

		while(1)
		{
			Sleep(90);

			if(p_lb_door->Check__DATA(STR__OPEN) > 0)		check_count++;
			else											check_count = 0;

			if(check_count >= ref_count)
			{
				active__sv_err = false;
				break;
			}

			if(x_timer_ctrl->Get__CURRENT_TIME() > ref_sec)		
			{
				break;
			}
		}

		if((active__sv_err)
		|| (p_lb_door->Check__DATA(STR__OPEN) < 0))
		{
			int alarm_id = ALID__LLx__NOT_VALVE_OPEN;
			CString err_msg;
			CString err_bff;
			CString r_act;

			err_msg.Format("Robot can not %s.\n", act_name);

			err_bff.Format("Please, check %s(%s) ! \n", stn_name,stn_slot);
			err_msg += err_bff;

			err_bff.Format(" * %s <- %s \n", 
							p_lb_door->Get__CHANNEL_NAME(),
							p_lb_door->Get__STRING());
			err_msg += err_bff;

			err_bff.Format(" * ref_sec <- %.1f (sec) \n", ref_sec);
			err_msg += err_bff;

			err_bff.Format(" * ref_check_count <- %1d \n", ref_count);
			err_msg += err_bff;

			//
			p_alarm->Popup__ALARM_With_MESSAGE(alarm_id, err_msg, r_act);

			if(r_act.CompareNoCase(ACT__RETRY) == 0)
			{
				goto LOOP__RETRY;
			}

			return -14;
		}

		return 1;
	}

	// ...
	int pm_index = Macro__CHECK_PMx_INDEX(stn_name);
	if(pm_index >= 0)
	{
		if(pm_index >= m_nPM_LIMIT)	
		{
			return -21;
		}

		if(dEXT_CH__CFG_PMx_SV_USE[pm_index]->Check__DATA("NO") < 0)
		{
			CII__VAR_DIGITAL_CTRL *p_pm_sv = dEXT_CH__PMx_SLIT_VALVE_STATUS[pm_index].Get__PTR();

			if(p_pm_sv->When__DATA(STR__OPEN, 2.0) == 0)
			{
				return -23;
			}

			// ...
			bool active__sv_err = true;

			SCX__ASYNC_TIMER_CTRL x_timer_ctrl;
			double ref_sec = 10.0;

			x_timer_ctrl->START__COUNT_UP(9999);

			int check_count = 0;
			int ref_count = 10;

			while(1)
			{
				Sleep(90);

				if(p_pm_sv->Check__DATA(STR__OPEN) > 0)			check_count++;
				else											check_count = 0;

				if(check_count >= ref_count)
				{
					active__sv_err = false;
					break;
				}

				if(x_timer_ctrl->Get__CURRENT_TIME() > ref_sec)		
				{
					break;
				}
			}

			if((active__sv_err)
			|| (p_pm_sv->Check__DATA(STR__OPEN) < 0))
			{
				int alarm_id = ALID__PMx__NOT_VALVE_OPEN;
				CString err_msg;
				CString err_bff;
				CString r_act;

				err_msg.Format("Robot can not %s.\n",act_name);
				err_msg += "Please, check the items below. \n";

				err_bff.Format(" -. PM%1d \n", pm_index+1);
				err_msg += err_bff;

				err_bff.Format(" * %s <- %s \n", 
								p_pm_sv->Get__CHANNEL_NAME(),
								p_pm_sv->Get__STRING());
				err_msg += err_bff;

				err_bff.Format(" * ref_sec <- %.1f (sec) \n", ref_sec);
				err_msg += err_bff;

				err_bff.Format(" * ref_check_count <- %1d \n", ref_count);
				err_msg += err_bff;

				//
				p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,err_msg,r_act);

				if(r_act.CompareNoCase(ACT__RETRY) == 0)
				{
					goto LOOP__RETRY;
				}

				return -24;	
			}
		}

		return 1;
	}

	return 1;
}
