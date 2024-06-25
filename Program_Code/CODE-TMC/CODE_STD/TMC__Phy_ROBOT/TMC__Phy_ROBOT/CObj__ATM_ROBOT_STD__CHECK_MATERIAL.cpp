#include "StdAfx.h"
#include "CObj__ATM_ROBOT_STD.h"
#include "CObj__ATM_ROBOT_STD__DEF.h"
#include "CObj__ATM_ROBOT_STD__ALID.h"

#include "Macro_Function.h"


//------------------------------------------------------------------------------------
int  CObj__ATM_ROBOT_STD::
Interlock__CHECK_MATERIAL(CII_OBJECT__ALARM* p_alarm,
						  const int place_flag,
						  const CString& arm_type,
						  const CString& stn_name,
						  const CString& stn_slot,
						  const CString& act_name)
{
LOOP_RETRY:

	// ...
	int slot_index = atoi(stn_slot) - 1;
	CString wfr_sts;

	if(arm_type.CompareNoCase(ARM_A) == 0)
	{
		dCH__OTR_OUT_MON__ARM_A_MATERIAL_STATUS->Get__DATA(wfr_sts);

		if(place_flag > 0)
		{
			if(wfr_sts.CompareNoCase(STR__NONE) == 0)
			{
				int alarm_id = ALID__ARM_A__MATERIAL_NONE_ERROR;
				CString err_msg;
				CString r_act;

				err_msg = "Material on Arm-A must exist.\n";

				p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,err_msg,r_act);

				if(r_act.CompareNoCase(ACT__RETRY) == 0)		goto LOOP_RETRY;
				return -1;
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
				return -1;
			}
		}
	}
	else if(arm_type.CompareNoCase(ARM_B) == 0)
	{
		dCH__OTR_OUT_MON__ARM_B_MATERIAL_STATUS->Get__DATA(wfr_sts);

		if(place_flag > 0)
		{
			if(wfr_sts.CompareNoCase(STR__NONE) == 0)
			{
				int alarm_id = ALID__ARM_B__MATERIAL_NONE_ERROR;
				CString err_msg;
				CString r_act;

				err_msg = "Material on Arm-B must exist.\n";

				p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,err_msg,r_act);

				if(r_act.CompareNoCase(ACT__RETRY) == 0)		goto LOOP_RETRY;
				return -1;
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
				return -1;
			}
		}
	}

	// ...
	int lp_index = Macro__CHECK_LPx_INDEX(stn_name);

	if((lp_index >= 0)&&(lp_index < CFG_LPx__SIZE))
	{
		if((slot_index >= 0)&&(slot_index < CFG_LPx__SLOT_SIZE))
		{
			dEXT_CH__LPx_SLOT_STATUS[lp_index][slot_index]->Get__DATA(wfr_sts);

			if(place_flag > 0)
			{
				if(wfr_sts.CompareNoCase(STR__NONE) != 0)
				{
					int alarm_id = ALID__LP1__MATERIAL_EXIST_ERROR + lp_index;
					CString err_msg;
					CString r_act;

					err_msg.Format("No material in %s(%s) must exist.\n",
						stn_name,stn_slot);

					Fnc__APP_LOG(err_msg);

					p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,err_msg,r_act);

					if(r_act.CompareNoCase(ACT__RETRY) == 0)		goto LOOP_RETRY;
					return -1;
				}				
			}
			else
			{
				if(wfr_sts.CompareNoCase(STR__NONE) == 0)
				{
					int alarm_id = ALID__LP1__MATERIAL_NONE_ERROR + lp_index;
					CString err_msg;
					CString r_act;

					err_msg.Format("Material in %s(%s) must exist.\n",
						stn_name,stn_slot);

					p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,err_msg,r_act);

					if(r_act.CompareNoCase(ACT__RETRY) == 0)		goto LOOP_RETRY;
					return -1;
				}
			}
		}
	}

	if(stn_name.CompareNoCase(STR__AL1) == 0)
	{
		if(slot_index == 0)
		{			
			dEXT_CH__AL1_SLOT01_STATUS->Get__DATA(wfr_sts);

			if(place_flag > 0)
			{
				if(wfr_sts.CompareNoCase(STR__NONE) != 0)
				{
					int alarm_id = ALID__AL1__MATERIAL_EXIST_ERROR;
					CString err_msg;
					CString r_act;

					err_msg = "No material in AL1(1) must exist.\n";

					p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,err_msg,r_act);

					if(r_act.CompareNoCase(ACT__RETRY) == 0)		goto LOOP_RETRY;
					return -1;
				}
			}
			else
			{
				if(wfr_sts.CompareNoCase(STR__NONE) == 0)
				{
					int alarm_id = ALID__AL1__MATERIAL_NONE_ERROR;
					CString err_msg;
					CString r_act;

					err_msg = "Material in AL1(1) must exist.\n";

					p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,err_msg,r_act);

					if(r_act.CompareNoCase(ACT__RETRY) == 0)		goto LOOP_RETRY;
					return -1;
				}				
			}
		}
	}

	// ...
	int ll_i = Macro__CHECK_LLx_INDEX(stn_name);
	if(ll_i >= 0)
	{
		if((slot_index >= 0)&&(slot_index < CFG_LLx__SLOT_SIZE))
		{
			dEXT_CH__LLx_SLOT_STATUS[ll_i][slot_index]->Get__DATA(wfr_sts);

			if(place_flag > 0)
			{
				if(wfr_sts.CompareNoCase(STR__NONE) != 0)
				{
					int alarm_id = ALID__LBA__MATERIAL_EXIST_ERROR + ll_i;
					CString err_msg;
					CString r_act;

					err_msg.Format("No material in %s(%s) must exist.\n",stn_name,stn_slot);

					p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,err_msg,r_act);

					if(r_act.CompareNoCase(ACT__RETRY) == 0)		goto LOOP_RETRY;
					return -1;
				}
			}
			else
			{
				if(wfr_sts.CompareNoCase(STR__NONE) == 0)
				{
					int alarm_id = ALID__LBA__MATERIAL_NONE_ERROR + ll_i;
					CString err_msg;
					CString err_bff;
					CString r_act;

					err_msg.Format("Material in %s(%s) must exist.\n",stn_name,stn_slot);

					err_bff.Format(" * ll_i <- [%1d] \n", ll_i);
					err_msg += err_bff;
					err_bff.Format(" * slot_index <- [%1d] \n", slot_index);
					err_msg += err_bff;

					err_bff.Format(" * %s <- [%s] \n", 
									dEXT_CH__LLx_SLOT_STATUS[ll_i][slot_index]->Get__CHANNEL_NAME(),
									dEXT_CH__LLx_SLOT_STATUS[ll_i][slot_index]->Get__STRING());
					err_msg += err_bff;

					p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,err_msg,r_act);

					if(r_act.CompareNoCase(ACT__RETRY) == 0)		goto LOOP_RETRY;
					return -1;
				}				
			}
		}
	}

	// SIDE STORAGE (1) ...
	if(stn_name.CompareNoCase(STR__BUFF1) == 0)
	{
		if(slot_index <  0)							return -1;
		if(slot_index >= CFG_LPx__SLOT_SIZE)		return -1;

		dEXT_CH__SIDE_STORAGE1_SLOT_STATUS[slot_index]->Get__DATA(wfr_sts);

		if(place_flag > 0)
		{
			if(wfr_sts.CompareNoCase(STR__NONE) != 0)
			{
				int alarm_id = ALID__SIDE_STORAGE1__MATERIAL_EXIST_ERROR;
				CString err_msg;
				CString r_act;

				err_msg.Format("No material in Side_Storage(1) must exist. SlotID(%1d)\n",
					slot_index+1);

				p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,err_msg,r_act);

				if(r_act.CompareNoCase(ACT__RETRY) == 0)		goto LOOP_RETRY;
				return -1;
			}
		}
		else
		{
			if(wfr_sts.CompareNoCase(STR__NONE) == 0)
			{
				int alarm_id = ALID__SIDE_STORAGE1__MATERIAL_NONE_ERROR;
				CString err_msg;
				CString r_act;

				err_msg.Format("Material in Side_Storage(1) must exist. SlotID(%1d)\n", 
					slot_index+1);

				p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,err_msg,r_act);

				if(r_act.CompareNoCase(ACT__RETRY) == 0)		goto LOOP_RETRY;
				return -1;
			}
		}
	}
	// SIDE STORAGE (2) ...
	if(stn_name.CompareNoCase(STR__BUFF2) == 0)
	{
		if(slot_index <  0)							return -1;
		if(slot_index >= CFG_LPx__SLOT_SIZE)		return -1;

		dEXT_CH__SIDE_STORAGE2_SLOT_STATUS[slot_index]->Get__DATA(wfr_sts);

		if(place_flag > 0)
		{
			if(wfr_sts.CompareNoCase(STR__NONE) != 0)
			{
				int alarm_id = ALID__SIDE_STORAGE2__MATERIAL_EXIST_ERROR;
				CString err_msg;
				CString r_act;

				err_msg.Format("No material in Side_Storage(2) must exist. SlotID(%1d)\n",
					slot_index+1);

				p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,err_msg,r_act);

				if(r_act.CompareNoCase(ACT__RETRY) == 0)		goto LOOP_RETRY;
				return -1;
			}
		}
		else
		{
			if(wfr_sts.CompareNoCase(STR__NONE) == 0)
			{
				int alarm_id = ALID__SIDE_STORAGE2__MATERIAL_NONE_ERROR;
				CString err_msg;
				CString r_act;

				err_msg.Format("Material in Side_Storage(2) must exist. SlotID(%1d)\n", 
					slot_index+1);

				p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,err_msg,r_act);

				if(r_act.CompareNoCase(ACT__RETRY) == 0)		goto LOOP_RETRY;
				return -1;
			}
		}
	}

	return 1;
}

int  CObj__ATM_ROBOT_STD::
Fnc__CHANGE_MATERIAL_INFO(const int place_flag,
						  const CString& arm_type,
						  const CString& stn_name,
						  const CString& stn_slot)
{
	int check_flag = -1;
	int slot_index = atoi(stn_slot) - 1;

	// ...
	CString wfr_sts;
	CString wfr_title;

	if(place_flag > 0)
	{
		if(arm_type.CompareNoCase(ARM_A) == 0)
		{
			dCH__OTR_OUT_MON__ARM_A_MATERIAL_STATUS->Get__DATA(wfr_sts);
			sCH__OTR_OUT_MON__ARM_A_MATERIAL_TITLE->Get__DATA(wfr_title);

			dCH__OTR_OUT_MON__ARM_A_MATERIAL_STATUS->Set__DATA(STR__NONE);
			sCH__OTR_OUT_MON__ARM_A_MATERIAL_TITLE->Set__DATA("");
		}
		else if(arm_type.CompareNoCase(ARM_B) == 0)
		{
			dCH__OTR_OUT_MON__ARM_B_MATERIAL_STATUS->Get__DATA(wfr_sts);
			sCH__OTR_OUT_MON__ARM_B_MATERIAL_TITLE->Get__DATA(wfr_title);

			dCH__OTR_OUT_MON__ARM_B_MATERIAL_STATUS->Set__DATA(STR__NONE);
			sCH__OTR_OUT_MON__ARM_B_MATERIAL_TITLE->Set__DATA("");
		}
	}

	if(check_flag < 0)
	{
		int lp_index = Macro__CHECK_LPx_INDEX(stn_name);

		if((lp_index >= 0)&&(lp_index < CFG_LPx__SIZE))
		{
			check_flag = 1;

			if((slot_index >= 0)&&(slot_index < CFG_LPx__SLOT_SIZE))
			{
				if(place_flag > 0)
				{
					dEXT_CH__LPx_SLOT_STATUS[lp_index][slot_index]->Set__DATA(wfr_sts);
					sEXT_CH__LPx_SLOT_TITLE[lp_index][slot_index]->Set__DATA(wfr_title);
				}
				else
				{
					dEXT_CH__LPx_SLOT_STATUS[lp_index][slot_index]->Get__DATA(wfr_sts);
					wfr_title.Format("P%1d:%002d",lp_index+1,slot_index+1);

					dEXT_CH__LPx_SLOT_STATUS[lp_index][slot_index]->Set__DATA(STR__NONE);
					sEXT_CH__LPx_SLOT_TITLE[lp_index][slot_index]->Set__DATA("");
				}
			}
		}
	}

	if(check_flag < 0)
	{
		if(stn_name.CompareNoCase(STR__AL1) == 0)
		{
			if((slot_index == 0)
			|| (slot_index == 1))
			{
				check_flag = 1;
			}

			if(place_flag > 0)
			{
				if(slot_index == 0)
				{
					dEXT_CH__AL1_SLOT01_STATUS->Set__DATA(wfr_sts);
					sEXT_CH__AL1_SLOT01_TITLE->Set__DATA(wfr_title);
				}
				else if(slot_index == 1)
				{
					dEXT_CH__AL1_SLOT02_STATUS->Set__DATA(wfr_sts);
					sEXT_CH__AL1_SLOT02_TITLE->Set__DATA(wfr_title);
				}
			}
			else
			{
				CII__VAR_DIGITAL_CTRL *pch_status = NULL;
				CII__VAR_STRING_CTRL  *pch_title  = NULL;

				if(slot_index == 0)
				{
					pch_status = dEXT_CH__AL1_SLOT01_STATUS.Get__PTR();
					pch_title  = sEXT_CH__AL1_SLOT01_TITLE.Get__PTR();
				}
				else if(slot_index == 1)
				{
					pch_status = dEXT_CH__AL1_SLOT02_STATUS.Get__PTR();
					pch_title  = sEXT_CH__AL1_SLOT02_TITLE.Get__PTR();
				}

				if(pch_status != NULL)
				{
					pch_status->Get__DATA(wfr_sts);
					pch_title->Get__DATA(wfr_title);

					pch_status->Set__DATA(STR__NONE);
					pch_title->Set__DATA("");
				}
			}
		}
	}

	if(check_flag < 0)
	{
		int ll_i = Macro__CHECK_LLx_INDEX(stn_name);
		if(ll_i >= 0)
		{
			check_flag = 1;

			if((slot_index >= 0)&&(slot_index < CFG_LLx__SLOT_SIZE))
			{
				if(place_flag > 0)
				{
					dEXT_CH__LLx_SLOT_STATUS[ll_i][slot_index]->Set__DATA(wfr_sts);
					sEXT_CH__LLx_SLOT_TITLE[ll_i][slot_index]->Set__DATA(wfr_title);
				}
				else
				{
					dEXT_CH__LLx_SLOT_STATUS[ll_i][slot_index]->Get__DATA(wfr_sts);
					sEXT_CH__LLx_SLOT_TITLE[ll_i][slot_index]->Get__DATA(wfr_title);

					dEXT_CH__LLx_SLOT_STATUS[ll_i][slot_index]->Set__DATA(STR__NONE);
					sEXT_CH__LLx_SLOT_TITLE[ll_i][slot_index]->Set__DATA("");
				}
			}
		}
	}

	// SIDE STORAGE1 ...
	if(check_flag < 0)
	{
		if(stn_name.CompareNoCase(STR__BUFF1) == 0)
		{
			check_flag = 1;

			if(slot_index <  0)						return -1;
			if(slot_index >= CFG_LPx__SLOT_SIZE)	return -1;

			if(place_flag > 0)
			{
				dEXT_CH__SIDE_STORAGE1_SLOT_STATUS[slot_index]->Set__DATA(wfr_sts);
				sEXT_CH__SIDE_STORAGE1_SLOT_TITLE[slot_index]->Set__DATA(wfr_title);
			}
			else
			{
				dEXT_CH__SIDE_STORAGE1_SLOT_STATUS[slot_index]->Get__DATA(wfr_sts);
				sEXT_CH__SIDE_STORAGE1_SLOT_TITLE[slot_index]->Get__DATA(wfr_title);

				dEXT_CH__SIDE_STORAGE1_SLOT_STATUS[slot_index]->Set__DATA(STR__NONE);
				sEXT_CH__SIDE_STORAGE1_SLOT_TITLE[slot_index]->Set__DATA("");
			}
		}
	}
	// SIDE STORAGE2 ...
	if(check_flag < 0)
	{
		if(stn_name.CompareNoCase(STR__BUFF2) == 0)
		{
			check_flag = 1;

			if(slot_index <  0)						return -1;
			if(slot_index >= CFG_LPx__SLOT_SIZE)	return -1;

			if(place_flag > 0)
			{
				dEXT_CH__SIDE_STORAGE2_SLOT_STATUS[slot_index]->Set__DATA(wfr_sts);
				sEXT_CH__SIDE_STORAGE2_SLOT_TITLE[slot_index]->Set__DATA(wfr_title);
			}
			else
			{
				dEXT_CH__SIDE_STORAGE2_SLOT_STATUS[slot_index]->Get__DATA(wfr_sts);
				sEXT_CH__SIDE_STORAGE2_SLOT_TITLE[slot_index]->Get__DATA(wfr_title);

				dEXT_CH__SIDE_STORAGE2_SLOT_STATUS[slot_index]->Set__DATA(STR__NONE);
				sEXT_CH__SIDE_STORAGE2_SLOT_TITLE[slot_index]->Set__DATA("");
			}
		}
	}

	if(place_flag < 0)
	{
		if(wfr_sts.GetLength() > 0)
		{
			if(arm_type.CompareNoCase(ARM_A) == 0)
			{
				dCH__OTR_OUT_MON__ARM_A_MATERIAL_STATUS->Set__DATA(wfr_sts);
				sCH__OTR_OUT_MON__ARM_A_MATERIAL_TITLE->Set__DATA(wfr_title);
			}
			else if(arm_type.CompareNoCase(ARM_B) == 0)
			{
				dCH__OTR_OUT_MON__ARM_B_MATERIAL_STATUS->Set__DATA(wfr_sts);
				sCH__OTR_OUT_MON__ARM_B_MATERIAL_TITLE->Set__DATA(wfr_title);
			}
		}
	}

	return 1;
}
