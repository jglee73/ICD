#include "StdAfx.h"
#include "CObj__ATM_ROBOT_STD.h"
#include "CObj__ATM_ROBOT_STD__DEF.h"
#include "CObj__ATM_ROBOT_STD__ALID.h"

#include "Macro_Function.h"


//------------------------------------------------------------------------------------
int  CObj__ATM_ROBOT_STD
::Interlock__CHECK_DOOR_OPEN(CII_OBJECT__ALARM* p_alarm,
					         const CString& stn_name,
						     const CString& act_name)
{
	CString str_log;
	int nRet;

LOOP__RETRY:

	int ll_i = Macro__CHECK_LLx_INDEX(stn_name);
	if(ll_i >= 0)
	{
		CII__VAR_DIGITAL_CTRL *p_lb_door;
		int alarm_id;

		// ...
		{
			alarm_id = ALID__LBA__NOT_DOOR_OPEN  + ll_i;
			p_lb_door = dEXT_CH__LLx_DOOR_STATUS[ll_i].Get__PTR();
		}

		nRet = p_lb_door->When__DATA(STR__OPEN, 2.0);

		if((nRet < 0)
		|| (p_lb_door->Check__DATA(STR__OPEN) < 0))
		{
			CString err_msg;
			CString msg_bff;
			CString r_act;

			err_msg.Format("Robot can not %s.\n",act_name);
			msg_bff.Format("%s's door status is not open.\n", stn_name);
			err_msg += msg_bff;
			err_msg += "Please, check if (open sensor <- ON) and (close sensor <- OFF). \n";

			p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,err_msg,r_act);

			if(r_act.CompareNoCase(ACT__RETRY) == 0)
			{
				goto LOOP__RETRY;
			}
			return -1;
		}

		return 1;
	}

	// ...
	int lp_index = Macro__CHECK_LPx_INDEX(stn_name);

	if((lp_index >= 0)
	&& (lp_index <  CFG_LPx__SIZE))
	{
		if(dEXT_CH__LPx_DOOR_STATUS[lp_index]->Check__DATA(STR__OPEN) < 0)
		{
			int alarm_id = ALID__LPx__NOT_DOOR_OPEN;
			CString err_msg;
			CString err_bff;
			CString r_act;

			err_msg.Format("Robot can not %s.\n",act_name);
			err_bff.Format("%s's door is not open.\n",stn_name);
			err_msg += err_bff;

			p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,err_msg,r_act);

			if(r_act.CompareNoCase(ACT__RETRY) == 0)		goto LOOP__RETRY;
			return -1;			
		}

		if(dEXT_CH__LPx_FOUP_EXIST_FLAG[lp_index]->Check__DATA(STR__EXIST) < 0)
		{
			int alarm_id = ALID__LPx__FOUP_NOT_EXIST;
			CString err_msg;
			CString err_bff;
			CString r_act;

			err_msg.Format("Robot can not %s.\n",act_name);
			err_bff.Format("%s's foup does not exist.\n",stn_name);
			err_msg += err_bff;

			p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,err_msg,r_act);

			if(r_act.CompareNoCase(ACT__RETRY) == 0)		goto LOOP__RETRY;
			return -1;			
		}

		if(dEXT_CH__LPx_FOUP_CLAMP_STATUS[lp_index]->Check__DATA(STR__CLAMP) < 0)
		{
			int alarm_id = ALID__LPx__FOUP_NOT_CLAMP;
			CString err_msg;
			CString err_bff;
			CString r_act;
			
			err_msg.Format("Robot can not %s.\n",act_name);
			err_bff.Format("%s's foup is not clamped.\n",stn_name);
			err_msg += err_bff;

			p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,err_msg,r_act);

			if(r_act.CompareNoCase(ACT__RETRY) == 0)		goto LOOP__RETRY;
			return -1;			
		}

		if(dEXT_CH__LPx_FOUP_POS_STATUS[lp_index]->Check__DATA(STR__LOAD) < 0)
		{
			int alarm_id = ALID__LPx__FOUP_NOT_LOAD;
			CString err_msg;
			CString err_bff;
			CString r_act;

			err_msg.Format("Robot can not %s.\n",act_name);
			err_bff.Format("%s's foup position is not load position.\n",stn_name);
			err_msg += err_bff;

			p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,err_msg,r_act);

			if(r_act.CompareNoCase(ACT__RETRY) == 0)		goto LOOP__RETRY;
			return -1;			
		}
	}

	return 1;
}

int  CObj__ATM_ROBOT_STD
::Interlock__CHECK_WAFER_LP_SLIDE(CII_OBJECT__ALARM* p_alarm,
								  const CString& stn_name,
								  const CString& act_name)
{
LOOP_RETRY:

	// ...
	int lp_index = Macro__CHECK_LPx_INDEX(stn_name);

	if((lp_index >= 0) && (lp_index < iLPx_SIZE))
	{
		if(!bActive__LPx_WAFER_SLIDE_OUT_SNS[lp_index])
		{
			return 1;
		}

		// ...
		CString ch_data = dEXT_CH__LPx_WAFER_SLIDE_OUT_SNS[lp_index]->Get__STRING();

		if(ch_data.CompareNoCase(STR__OFF) != 0)
		{
			CString log_msg;

			log_msg.Format("During Robot [%s] Action to [%s] Stn.. WAFER SLIDE OUT SNS [%s] Status.. so Alarm Occur !!", 
						   act_name, 
						   stn_name, 
						   ch_data);

			Fnc__APP_LOG(log_msg);

			// ...
			{
				int alarm_id = ALID__LP1__WFR_SLIDE_SNS_ALARM + lp_index;
				CString r_act;

				p_alarm->Popup__ALARM(alarm_id,r_act);

				if(r_act.CompareNoCase("RETRY") == 0)
				{
					goto LOOP_RETRY;
				}
			}
			return -1;
		}
	}
	else
	{
		CString log_msg;

		log_msg.Format("Unknown %s Station...ret:-1", stn_name);

		Fnc__APP_LOG(log_msg);
		return -1;
	}

	return 1;
}
