#include "StdAfx.h"
#include "CObj__VAC_ROBOT_STD.h"
#include "CObj__VAC_ROBOT_STD__DEF.h"
#include "CObj__VAC_ROBOT_STD__ALID.h"

#include "Macro_Function.h"


//------------------------------------------------------------------------------------
int  CObj__VAC_ROBOT_STD::
Interlock__LLx_SLOT_CHECK(CII_OBJECT__ALARM* p_alarm,
						  const CString& stn_name, 
						  const CString& para_slot)
{
LOOP_RETRY:

	int ll_i = Macro__CHECK_LLx_INDEX(stn_name);
	if(ll_i <  0)				return  1;
	if(ll_i >= iSIZE__LLx)		return -1;

	// ...
	CString ch_data = dEXT_CH__CFG_LLx_SLOT_MAX[ll_i]->Get__STRING();
	int cfg__slot_max = atoi(ch_data);

	// Check ...
	{
		int i_slot = atoi(para_slot);

		// CFG.MAX ...
		if((i_slot > cfg__slot_max)
		|| (i_slot < 1))
		{
			CString log_msg;

			// ...
			{
				CString log_bff;

				log_msg.Format(" * Error : %s's Slot_MAX ... \n", stn_name);
				log_bff.Format(" * para_slot : %1d \n", i_slot);
				log_msg += log_bff;
				log_bff.Format(" * cfg_slot : %1d \n", cfg__slot_max);
				log_msg += log_bff;

				Fnc__APP_LOG(log_msg);
			}

			// ...
			{
				int alm_id = ALID__LLx__SLOT_ID_ERROR;
				CString r_act;

				p_alarm->Popup__ALARM_With_MESSAGE(alm_id, log_msg, r_act);

				if(r_act.CompareNoCase(ACT__RETRY) == 0)	
				{
					goto LOOP_RETRY;
				}
			}
			return -1;
		}

		// CFG.USE ...
		int slot_index = i_slot - 1;

		if(dEXT_CH__CFG_LLx_SLOT_USE_X[ll_i][slot_index]->Check__DATA(STR__YES) < 0)
		{
			CString log_msg;

			// ...
			{
				CString log_bff;

				log_msg.Format(" * Error : %s's Slot_USE ... \n", stn_name);
				log_bff.Format(" * para_slot : %1d \n", i_slot);
				log_msg += log_bff;
				log_bff.Format(" * %s <- %s \n",
								dEXT_CH__CFG_LLx_SLOT_USE_X[ll_i][slot_index]->Get__CHANNEL_NAME(),
								dEXT_CH__CFG_LLx_SLOT_USE_X[ll_i][slot_index]->Get__STRING());
				log_msg += log_bff;

				Fnc__APP_LOG(log_msg);
			}

			// ...
			{
				int alm_id = ALID__LLx__SLOT_ID_ERROR;
				CString r_act;

				p_alarm->Popup__ALARM_With_MESSAGE(alm_id, log_msg, r_act);

				if(r_act.CompareNoCase(ACT__RETRY) == 0)	
				{
					goto LOOP_RETRY;
				}
			}
			return -2;
		}
	}

	return 1;

}

int  CObj__VAC_ROBOT_STD::
Interlock__CHECK_MATERIAL(CII_OBJECT__ALARM* p_alarm,
						  const bool active_place,
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

		int r_flag = _Interlock__CHECK_MATERIAL(p_alarm, active_place, cur__arm_type,cur__stn_name,cur__stn_slot, act_name);
		if(r_flag < 0)		return r_flag;
	}
	return 1;
}
int  CObj__VAC_ROBOT_STD
::_Interlock__CHECK_MATERIAL(CII_OBJECT__ALARM* p_alarm,
						     const bool active_place,
						     const CString& arm_type,
						     const CString& stn_name,
						     const CString& stn_slot,
						     const CString& act_name)
{
LOOP_RETRY:

	// ...
	bool active__arm_a = false;
	bool active__arm_b = false;

		 if(arm_type.CompareNoCase(_ARM_A) == 0)			active__arm_a = true;
	else if(arm_type.CompareNoCase(_ARM_B) == 0)			active__arm_b = true;

	// ...
	CString wfr_sts;

	if(active__arm_a)
	{
		dCH__OTR_OUT_MON__ARM_A_MATERIAL_STATUS->Get__DATA(wfr_sts);

		if(active_place)
		{
			if(wfr_sts.CompareNoCase(STR__NONE) == 0)
			{
				int alarm_id = ALID__ARM_A__MATERIAL_NONE_ERROR;
				CString err_msg;
				CString r_act;

				err_msg = "Material on Arm-A must exist.\n";

				p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,err_msg,r_act);

				if(r_act.CompareNoCase(ACT__RETRY)  == 0)		goto LOOP_RETRY;
				if(r_act.CompareNoCase(ACT__IGNORE) != 0)		return -1;
			}
		}
		else
		{
			if(wfr_sts.CompareNoCase(STR__NONE) != 0)
			{
				int alarm_id = ALID__ARM_A__MATERIAL_EXIST_ERROR;
				CString err_msg;
				CString r_act;

				err_msg = "No material on Arm-A must exist.\n";

				p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,err_msg,r_act);

				if(r_act.CompareNoCase(ACT__RETRY) == 0)		goto LOOP_RETRY;
				return -2;
			}
		}
	}
	else if(active__arm_b)
	{
		dCH__OTR_OUT_MON__ARM_B_MATERIAL_STATUS->Get__DATA(wfr_sts);

		if(active_place)
		{
			if(wfr_sts.CompareNoCase(STR__NONE) == 0)
			{
				int alarm_id = ALID__ARM_B__MATERIAL_NONE_ERROR;
				CString err_msg;
				CString r_act;

				err_msg = "Material on Arm-B must exist.\n";

				p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,err_msg,r_act);

				if(r_act.CompareNoCase(ACT__RETRY)  == 0)		goto LOOP_RETRY;
				if(r_act.CompareNoCase(ACT__IGNORE) != 0)		return -3;
			}
		}
		else
		{
			if(wfr_sts.CompareNoCase(STR__NONE) != 0)
			{
				int alarm_id = ALID__ARM_B__MATERIAL_EXIST_ERROR;
				CString err_msg;
				CString r_act;

				err_msg = "No material on Arm-B must exist.\n";

				p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,err_msg,r_act);

				if(r_act.CompareNoCase(ACT__RETRY) == 0)		goto LOOP_RETRY;
				return -4;
			}
		}
	}

	// LLx ...
	int ll_i = Macro__CHECK_LLx_INDEX(stn_name);
	if(ll_i >= 0)
	{
		int s_index = atoi(stn_slot) - 1;

		if(s_index <  0)						return -11;
		if(s_index >= CFG_LLx__SLOT_SIZE)		return -12;

		//			
		dEXT_CH__LLx_SLOT_STATUS[ll_i][s_index]->Get__DATA(wfr_sts);

		if(active_place)
		{
			if(wfr_sts.CompareNoCase(STR__NONE) != 0)
			{
				int alarm_id = ALID__LLx__MATERIAL_EXIST_ERROR;
				CString err_msg;
				CString r_act;

				err_msg.Format("No material in %s(%s) must exist.\n", stn_name,stn_slot);

				p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,err_msg,r_act);

				if(r_act.CompareNoCase(ACT__RETRY)  == 0)		goto LOOP_RETRY;
				return -13;
			}
		}
		else
		{
			if(wfr_sts.CompareNoCase(STR__NONE) == 0)
			{
				int alarm_id = ALID__LLx__MATERIAL_NONE_ERROR;
				CString err_msg;
				CString r_act;

				err_msg.Format("Material in %s(%s) must exist.\n",stn_name,stn_slot);

				p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,err_msg,r_act);

				if(r_act.CompareNoCase(ACT__RETRY)  == 0)		goto LOOP_RETRY;
				if(r_act.CompareNoCase(ACT__IGNORE) != 0)		return -14;
			}
		}
	}

	// ...
	int pm_index = Macro__CHECK_PMx_INDEX(stn_name);
	if(pm_index >= 0)
	{
		if(pm_index >= m_nPM_LIMIT)			return -21;

		int slot_index = atoi(stn_slot) - 1;
		if(slot_index != 0)					return -22;

		//
		dEXT_CH__PMx_SLOT01_STATUS[pm_index]->Get__DATA(wfr_sts);

		if(active_place)
		{
			if(wfr_sts.CompareNoCase(STR__NONE) != 0)
			{
				int alarm_id = ALID__PMx__MATERIAL_EXIST_ERROR;
				CString err_msg;
				CString r_act;

				err_msg.Format("No material in %s(%s) must exist.\n", stn_name,stn_slot);

				p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,err_msg,r_act);

				if(r_act.CompareNoCase(ACT__RETRY) == 0)		goto LOOP_RETRY;
				return -23;
			}
		}
		else
		{
			if(wfr_sts.CompareNoCase(STR__NONE) == 0)
			{
				int alarm_id = ALID__PMx__MATERIAL_NONE_ERROR;
				CString err_msg;
				CString r_act;

				err_msg.Format("Material in %s(%s) must exist.\n", stn_name,stn_slot);

				p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,err_msg,r_act);

				if(r_act.CompareNoCase(ACT__RETRY)  == 0)		goto LOOP_RETRY;
				if(r_act.CompareNoCase(ACT__IGNORE) != 0)		return -24;
			}
		}
	}

	return 1;
}

int  CObj__VAC_ROBOT_STD
::Fnc__CHANGE_MATERIAL_INFO(const int place_flag,
						    const CString& arm_type,
						    const CString& stn_name,
						    const CString& stn_slot)
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

		int r_flag = _Fnc__CHANGE_MATERIAL_INFO(place_flag, cur__arm_type,cur__stn_name,cur__stn_slot);
		if(r_flag < 0)		return r_flag;
	}
	return 1;
}
int  CObj__VAC_ROBOT_STD
::_Fnc__CHANGE_MATERIAL_INFO(const int place_flag,
						     const CString& arm_type,
						     const CString& stn_name,
						     const CString& stn_slot)
{
	bool active__arm_a = false;
	bool active__arm_b = false;

		 if(arm_type.CompareNoCase(_ARM_A) == 0)			active__arm_a = true;
	else if(arm_type.CompareNoCase(_ARM_B) == 0)			active__arm_b = true;

	// ...
	int check_flag = -1;

	CString arm__wfr_sts;
	CString arm__wfr_title;

	if(place_flag > 0)
	{
		if(active__arm_a)
		{
			arm__wfr_sts   = dCH__OTR_OUT_MON__ARM_A_MATERIAL_STATUS->Get__STRING();
			arm__wfr_title = sCH__OTR_OUT_MON__ARM_A_MATERIAL_TITLE->Get__STRING();

			dCH__OTR_OUT_MON__ARM_A_MATERIAL_STATUS->Set__DATA(STR__NONE);
			sCH__OTR_OUT_MON__ARM_A_MATERIAL_TITLE->Set__DATA("");
		}
		else if(active__arm_b)
		{
			arm__wfr_sts   = dCH__OTR_OUT_MON__ARM_B_MATERIAL_STATUS->Get__STRING();
			arm__wfr_title = sCH__OTR_OUT_MON__ARM_B_MATERIAL_TITLE->Get__STRING();

			dCH__OTR_OUT_MON__ARM_B_MATERIAL_STATUS->Set__DATA(STR__NONE);
			sCH__OTR_OUT_MON__ARM_B_MATERIAL_TITLE->Set__DATA("");
		}
	}

	if(check_flag < 0)
	{
		int ll_i = Macro__CHECK_LLx_INDEX(stn_name);
		if(ll_i >= 0)
		{
			if(ll_i >= iSIZE__LLx)					return -11;

			int s_index = atoi(stn_slot) - 1;
			if(s_index <  0)						return -12;
			if(s_index >= CFG_LLx__SLOT_SIZE)		return -13;

			// ...
			check_flag = 1;

			if(place_flag > 0)
			{
				dEXT_CH__LLx_SLOT_STATUS[ll_i][s_index]->Set__DATA(arm__wfr_sts);
				sEXT_CH__LLx_SLOT_TITLE[ll_i][s_index]->Set__DATA(arm__wfr_title);
			}
			else
			{
				arm__wfr_sts   = dEXT_CH__LLx_SLOT_STATUS[ll_i][s_index]->Get__STRING();
				arm__wfr_title = sEXT_CH__LLx_SLOT_TITLE[ll_i][s_index]->Get__STRING();

				dEXT_CH__LLx_SLOT_STATUS[ll_i][s_index]->Set__DATA(STR__NONE);
				sEXT_CH__LLx_SLOT_TITLE[ll_i][s_index]->Set__DATA("");
			}
		}
	}

	if(check_flag < 0)
	{
		int p_index = Macro__CHECK_PMx_INDEX(stn_name);
		if(p_index >= 0)
		{
			if(p_index >= m_nPM_LIMIT)				return -21;

			int s_index = atoi(stn_slot) - 1;
			if(s_index != 0)						return -22;

			// ...
			check_flag = 1;

			if(place_flag > 0)
			{
				dEXT_CH__PMx_SLOT01_STATUS[p_index]->Set__DATA(arm__wfr_sts);
				sEXT_CH__PMx_SLOT01_TITLE[p_index]->Set__DATA(arm__wfr_title);
			}
			else
			{
				arm__wfr_sts   = dEXT_CH__PMx_SLOT01_STATUS[p_index]->Get__STRING();
				arm__wfr_title = sEXT_CH__PMx_SLOT01_TITLE[p_index]->Get__STRING();

				dEXT_CH__PMx_SLOT01_STATUS[p_index]->Set__DATA(STR__NONE);
				sEXT_CH__PMx_SLOT01_TITLE[p_index]->Set__DATA("");
			}
		}
	}

	if(place_flag < 0)
	{
		if(active__arm_a)
		{
			dCH__OTR_OUT_MON__ARM_A_MATERIAL_STATUS->Set__DATA(arm__wfr_sts);
			sCH__OTR_OUT_MON__ARM_A_MATERIAL_TITLE->Set__DATA(arm__wfr_title);
		}
		else if(active__arm_b)
		{
			dCH__OTR_OUT_MON__ARM_B_MATERIAL_STATUS->Set__DATA(arm__wfr_sts);
			sCH__OTR_OUT_MON__ARM_B_MATERIAL_TITLE->Set__DATA(arm__wfr_title);
		}
	}

	return 1;
}

int  CObj__VAC_ROBOT_STD::_Update__MATERIAL_INFO()
{
	// ARM_A ...
	{
		CII__VAR_DIGITAL_CTRL* p_ch__phy_wfr = dCH__OTR_OUT_MON__ARM_A_MATERIAL_STATUS.Get__PTR();
		CII__VAR_DIGITAL_CTRL* p_ch__io_wfr  = dEXT_CH__ROBOT_ARM_A_MATERIAL_STATUS.Get__PTR();

		CString pre__wfr_sts = p_ch__phy_wfr->Get__STRING();

		if(p_ch__io_wfr->Check__DATA(STR__NONE) > 0)
		{
			p_ch__phy_wfr->Set__DATA(STR__NONE);
		}
		else
		{
			if(pre__wfr_sts.CompareNoCase(STR__NONE) == 0)			p_ch__phy_wfr->Set__DATA(STR__UNKNOWN);
			else													p_ch__phy_wfr->Set__DATA(pre__wfr_sts);
		}
	}

	// ARM_B ...
	{
		CII__VAR_DIGITAL_CTRL* p_ch__phy_wfr = dCH__OTR_OUT_MON__ARM_B_MATERIAL_STATUS.Get__PTR();
		CII__VAR_DIGITAL_CTRL* p_ch__io_wfr  = dEXT_CH__ROBOT_ARM_B_MATERIAL_STATUS.Get__PTR();

		CString pre__wfr_sts = p_ch__phy_wfr->Get__STRING();

		if(p_ch__io_wfr->Check__DATA(STR__NONE) > 0)
		{
			p_ch__phy_wfr->Set__DATA(STR__NONE);
		}
		else
		{
			if(pre__wfr_sts.CompareNoCase(STR__NONE) == 0)			p_ch__phy_wfr->Set__DATA(STR__UNKNOWN);
			else													p_ch__phy_wfr->Set__DATA(pre__wfr_sts);
		}
	}

	return 1;
}

