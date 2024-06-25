#include "StdAfx.h"
#include "CObj__ATM_ROBOT_FUSION.h"
#include "CObj__ATM_ROBOT_FUSION__DEF.h"
#include "CObj__ATM_ROBOT_FUSION__ALID.h"

#include "Macro_Function.h"


//------------------------------------------------------------------------------------
int  CObj__ATM_ROBOT_FUSION::
Interlock__CHECK_MATERIAL(CII_OBJECT__ALARM* p_alarm,
						  const int place_flag,
						  const CString& arm_type,
						  const CString& stn_name,
						  const CString& stn_slot,
						  const CString& act_name)
{
LOOP_RETRY:

	// ...
	CUIntArray l__slot_id;

	// ...
	{
		int slot_id = atoi(stn_slot);
		l__slot_id.Add(slot_id);

		if(arm_type.CompareNoCase(_ARM_AB) == 0)
		{
			slot_id = slot_id + 1;
			l__slot_id.Add(slot_id);
		}
	}

	// ...
	CString wfr_sts;

	if((arm_type.CompareNoCase(_ARM_A)  == 0)
	|| (arm_type.CompareNoCase(_ARM_AB) == 0))
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
	
	if((arm_type.CompareNoCase(_ARM_B)  == 0)
	|| (arm_type.CompareNoCase(_ARM_AB) == 0))
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

	if(lp_index >= 0)
	{
		if(lp_index >= iLPx_SIZE)
		{
			return -1001;
		}

		// ...
		CString ch_data = dEXT_CH__LPx_CFG_MAX_SLOT_COUNT[lp_index]->Get__STRING();
		int cfg_max = atoi(ch_data);

		// ...
		int slot_limit = l__slot_id.GetSize();

		for(int k=0; k<slot_limit; k++)
		{
			int slot_id = l__slot_id[k];
			int slot_index = slot_id - 1;

			if((slot_index >= 0) && (slot_index < cfg_max))
			{
				dEXT_CH__LPx_SLOT_STATUS[lp_index][slot_index]->Get__DATA(wfr_sts);

				if(place_flag > 0)
				{
					if(wfr_sts.CompareNoCase(STR__NONE) != 0)
					{
						int alarm_id = ALID__LP1__MATERIAL_EXIST_ERROR + lp_index;
						CString err_msg;
						CString err_bff;
						CString r_act;

						err_msg.Format("No material in %s(%s) must exist.\n", stn_name, stn_slot);

						err_bff.Format(" * %s(%1d) <- %s \n", stn_name, slot_index+1, wfr_sts);
						err_msg += err_bff;

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
						CString err_bff;
						CString r_act;

						err_msg.Format("Material in %s(%s) must exist.\n", stn_name, stn_slot);

						err_bff.Format(" * %s(%1d) <- %s \n", stn_name, slot_index+1, wfr_sts);
						err_msg += err_bff;

						p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,err_msg,r_act);

						if(r_act.CompareNoCase(ACT__RETRY) == 0)		goto LOOP_RETRY;
						return -1;
					}
				}
			}
			else
			{
				return -101;
			}
		}
	}

	if(stn_name.CompareNoCase(STR__AL1) == 0)
	{
		int slot_limit = l__slot_id.GetSize();

		for(int k=0; k<slot_limit; k++)
		{
			int slot_id = l__slot_id[k];
			int slot_index = slot_id - 1;

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

						err_msg.Format("No material in AL1(%1d) must exist.\n", slot_id);

						p_alarm->Popup__ALARM_With_MESSAGE(alarm_id, err_msg, r_act);

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

						err_msg.Format("Material in AL1(%1d) must exist.\n", slot_id);

						p_alarm->Popup__ALARM_With_MESSAGE(alarm_id, err_msg, r_act);

						if(r_act.CompareNoCase(ACT__RETRY) == 0)		goto LOOP_RETRY;
						return -1;
					}				
				}
			}
			else
			{
				return -111;
			}
		}
	}

	// ...
	int ll_i = Macro__CHECK_LLx_INDEX(stn_name);

	if(ll_i >= 0) 
	{
		if(ll_i >= iSIZE_LLx)
		{
			return -1011;
		}

		// ...
		int slot_limit = l__slot_id.GetSize();

		for(int k=0; k<slot_limit; k++)
		{
			int slot_id = l__slot_id[k];
			int slot_index = slot_id - 1;

			if((slot_index >= 0) && (slot_index < iLLx_SLOT_SIZE))
			{
				dEXT_CH__LLx_SLOT_STATUS[ll_i][slot_index]->Get__DATA(wfr_sts);

				if(place_flag > 0)
				{
					if(wfr_sts.CompareNoCase(STR__NONE) != 0)
					{
						int alarm_id = ALID__LBA__MATERIAL_EXIST_ERROR + ll_i;
						CString err_msg;
						CString r_act;

						err_msg.Format("No material in %s(%s) must exist.\n", stn_name, stn_slot);

						p_alarm->Popup__ALARM_With_MESSAGE(alarm_id, err_msg, r_act);

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
						CString r_act;

						err_msg.Format("Material in %s(%s) must exist.\n", stn_name, stn_slot);

						p_alarm->Popup__ALARM_With_MESSAGE(alarm_id, err_msg, r_act);

						if(r_act.CompareNoCase(ACT__RETRY) == 0)		goto LOOP_RETRY;
						return -1;
					}				
				}
			}
			else
			{
				return -121;
			}
		}
	}

	// SIDE STORAGE (1) ...
	if((stn_name.CompareNoCase(STR__BUFF1) == 0)
	|| (stn_name.CompareNoCase(STR__ST1) == 0))
	{
		int cfg_max = (int) aEXT_CH__SIDE_STORAGE1_CFG_SLOT_MAX->Get__VALUE();

		// ...
		int slot_limit = l__slot_id.GetSize();

		for(int k=0; k<slot_limit; k++)
		{
			int slot_id = l__slot_id[k];
			int slot_index = slot_id - 1;

			if(slot_index <  0)					return -101;
			if(slot_index >= cfg_max)			return -102;

			dEXT_CH__SIDE_STORAGE1_SLOT_STATUS[slot_index]->Get__DATA(wfr_sts);

			if(place_flag > 0)
			{
				if(wfr_sts.CompareNoCase(STR__NONE) != 0)
				{
					int alarm_id = ALID__SIDE_STORAGE1__MATERIAL_EXIST_ERROR;
					CString err_msg;
					CString r_act;

					err_msg.Format("No material in Side_Storage(1) must exist. SlotID(%1d)\n", slot_index+1);

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

					err_msg.Format("Material in Side_Storage(1) must exist. SlotID(%1d)\n", slot_index+1);

					p_alarm->Popup__ALARM_With_MESSAGE(alarm_id,err_msg,r_act);

					if(r_act.CompareNoCase(ACT__RETRY) == 0)		goto LOOP_RETRY;
					return -1;
				}
			}
		}
	}
	// SIDE STORAGE (2) ...
	if((stn_name.CompareNoCase(STR__BUFF2) == 0)
	|| (stn_name.CompareNoCase(STR__ST2) == 0))
	{
		int cfg_max = (int) aEXT_CH__SIDE_STORAGE2_CFG_SLOT_MAX->Get__VALUE();

		// ...
		int slot_limit = l__slot_id.GetSize();

		for(int k=0; k<slot_limit; k++)
		{
			int slot_id = l__slot_id[k];
			int slot_index = slot_id - 1;

			if(slot_index <  0)					return -201;
			if(slot_index >= cfg_max)			return -202;

			dEXT_CH__SIDE_STORAGE2_SLOT_STATUS[slot_index]->Get__DATA(wfr_sts);

			if(place_flag > 0)
			{
				if(wfr_sts.CompareNoCase(STR__NONE) != 0)
				{
					int alarm_id = ALID__SIDE_STORAGE2__MATERIAL_EXIST_ERROR;
					CString err_msg;
					CString r_act;

					err_msg.Format("No material in Side_Storage(2) must exist. SlotID(%1d)\n", slot_index+1);

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
	}

	return 1;
}

int  CObj__ATM_ROBOT_FUSION::
Fnc__CHANGE_MATERIAL_INFO(const int place_flag,
						  const CString& arm_type,
						  const CString& stn_name,
						  const CString& stn_slot)
{
	int check_flag = -1;

	// ...
	CUIntArray l__slot_id;

	// ...
	{
		int slot_id = atoi(stn_slot);
		l__slot_id.Add(slot_id);

		if(arm_type.CompareNoCase(_ARM_AB) == 0)
		{
			slot_id = slot_id + 1;
			l__slot_id.Add(slot_id);
		}
	}

	// ...
	CStringArray l__arm_type;
	CStringArray l__wfr_sts;
	CStringArray l__wfr_title;

	if(place_flag > 0)
	{
		CString wfr_sts;
		CString wfr_title;

		if((arm_type.CompareNoCase(_ARM_A)  == 0)
		|| (arm_type.CompareNoCase(_ARM_AB) == 0))
		{
			dCH__OTR_OUT_MON__ARM_A_MATERIAL_STATUS->Get__DATA(wfr_sts);
			sCH__OTR_OUT_MON__ARM_A_MATERIAL_TITLE->Get__DATA(wfr_title);

			dCH__OTR_OUT_MON__ARM_A_MATERIAL_STATUS->Set__DATA(STR__NONE);
			sCH__OTR_OUT_MON__ARM_A_MATERIAL_TITLE->Set__DATA("");

			l__arm_type.Add(_ARM_A);
			l__wfr_sts.Add(wfr_sts);
			l__wfr_title.Add(wfr_title);
		}
		
		if((arm_type.CompareNoCase(_ARM_B)  == 0)
		|| (arm_type.CompareNoCase(_ARM_AB) == 0))
		{
			dCH__OTR_OUT_MON__ARM_B_MATERIAL_STATUS->Get__DATA(wfr_sts);
			sCH__OTR_OUT_MON__ARM_B_MATERIAL_TITLE->Get__DATA(wfr_title);

			dCH__OTR_OUT_MON__ARM_B_MATERIAL_STATUS->Set__DATA(STR__NONE);
			sCH__OTR_OUT_MON__ARM_B_MATERIAL_TITLE->Set__DATA("");

			l__arm_type.Add(_ARM_B);
			l__wfr_sts.Add(wfr_sts);
			l__wfr_title.Add(wfr_title);
		}
	}
	else
	{
		if(arm_type.CompareNoCase(_ARM_AB) == 0)
		{
			l__arm_type.Add(_ARM_A);
			l__wfr_sts.Add(STR__UNKNOWN);
			l__wfr_title.Add("??");

			l__arm_type.Add(_ARM_B);
			l__wfr_sts.Add(STR__UNKNOWN);
			l__wfr_title.Add("??");
		}
		else
		{
			l__arm_type.Add(arm_type);
			l__wfr_sts.Add(STR__UNKNOWN);
			l__wfr_title.Add("??");
		}
	}

	if(check_flag < 0)
	{
		int lp_index = Macro__CHECK_LPx_INDEX(stn_name);

		if((lp_index >= 0) && (lp_index < iLPx_SIZE))
		{
			check_flag = 1;

			CString ch_data = dEXT_CH__LPx_CFG_MAX_SLOT_COUNT[lp_index]->Get__STRING();
			int cfg_max = atoi(ch_data);

			// ...
			int slot_limit = l__slot_id.GetSize();

			for(int k=0; k<slot_limit; k++)
			{
				int slot_id = l__slot_id[k];
				int slot_index = slot_id - 1;

				if((slot_index >= 0) && (slot_index < cfg_max))
				{
					if(place_flag > 0)
					{
						CString wfr_sts   = l__wfr_sts[k];
						CString wfr_title = l__wfr_title[k];

						dEXT_CH__LPx_SLOT_STATUS[lp_index][slot_index]->Set__DATA(wfr_sts);
						sEXT_CH__LPx_SLOT_TITLE[lp_index][slot_index]->Set__DATA(wfr_title);
					}
					else
					{
						CString wfr_sts;
						CString wfr_title;

						dEXT_CH__LPx_SLOT_STATUS[lp_index][slot_index]->Get__DATA(wfr_sts);
						wfr_title.Format("P%1d:%002d",lp_index+1,slot_index+1);

						dEXT_CH__LPx_SLOT_STATUS[lp_index][slot_index]->Set__DATA(STR__NONE);
						sEXT_CH__LPx_SLOT_TITLE[lp_index][slot_index]->Set__DATA("");

						l__wfr_sts[k] = wfr_sts;
						l__wfr_title[k] = wfr_title;
					}
				}
			}
		}
	}

	if(check_flag < 0)
	{
		if(stn_name.CompareNoCase(STR__AL1) == 0)
		{
			int slot_limit = l__slot_id.GetSize();

			for(int k=0; k<slot_limit; k++)
			{
				int slot_id = l__slot_id[k];
				int slot_index = slot_id - 1;

				if((slot_index == 0)
				|| (slot_index == 1))
				{
					check_flag = 1;
				}

				if(place_flag > 0)
				{
					CString wfr_sts   = l__wfr_sts[k];
					CString wfr_title = l__wfr_title[k];

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
						CString wfr_sts;
						CString wfr_title;

						pch_status->Get__DATA(wfr_sts);
						pch_title->Get__DATA(wfr_title);

						pch_status->Set__DATA(STR__NONE);
						pch_title->Set__DATA("");

						l__wfr_sts[k] = wfr_sts;
						l__wfr_title[k] = wfr_title;
					}
				}
			}
		}
	}

	if(check_flag < 0)
	{
		int ll_i = Macro__CHECK_LLx_INDEX(stn_name);

		if(ll_i >= 0)
		{
			if(ll_i >= iSIZE_LLx)
			{
				return -10001;
			}

			check_flag = 1;

			// ...
			int slot_limit = l__slot_id.GetSize();

			for(int k=0; k<slot_limit; k++)
			{
				int slot_id = l__slot_id[k];
				int slot_index = slot_id - 1;

				if((slot_index >= 0) && (slot_index < iLLx_SLOT_SIZE))
				{
					if(place_flag > 0)
					{
						CString wfr_sts   = l__wfr_sts[k];
						CString wfr_title = l__wfr_title[k];

						dEXT_CH__LLx_SLOT_STATUS[ll_i][slot_index]->Set__DATA(wfr_sts);
						sEXT_CH__LLx_SLOT_TITLE[ll_i][slot_index]->Set__DATA(wfr_title);
					}
					else
					{
						CString wfr_sts;
						CString wfr_title;

						dEXT_CH__LLx_SLOT_STATUS[ll_i][slot_index]->Get__DATA(wfr_sts);
						sEXT_CH__LLx_SLOT_TITLE[ll_i][slot_index]->Get__DATA(wfr_title);

						dEXT_CH__LLx_SLOT_STATUS[ll_i][slot_index]->Set__DATA(STR__NONE);
						sEXT_CH__LLx_SLOT_TITLE[ll_i][slot_index]->Set__DATA("");

						l__wfr_sts[k] = wfr_sts;
						l__wfr_title[k] = wfr_title;
					}
				}
			}
		}
	}

	// SIDE STORAGE1 ...
	if(check_flag < 0)
	{
		if((stn_name.CompareNoCase(STR__BUFF1) == 0)
		|| (stn_name.CompareNoCase(STR__ST1) == 0))
		{
			check_flag = 1;

			// ...
			int cfg_max = aEXT_CH__SIDE_STORAGE1_CFG_SLOT_MAX->Get__VALUE();

			// ...
			int slot_limit = l__slot_id.GetSize();

			for(int k=0; k<slot_limit; k++)
			{
				int slot_id = l__slot_id[k];
				int slot_index = slot_id - 1;

				if(slot_index <  0)					return -1;
				if(slot_index >= cfg_max)			return -1;

				if(place_flag > 0)
				{
					CString wfr_sts   = l__wfr_sts[k];
					CString wfr_title = l__wfr_title[k];

					dEXT_CH__SIDE_STORAGE1_SLOT_STATUS[slot_index]->Set__DATA(wfr_sts);
					sEXT_CH__SIDE_STORAGE1_SLOT_TITLE[slot_index]->Set__DATA(wfr_title);
				}
				else
				{
					CString wfr_sts;
					CString wfr_title;

					dEXT_CH__SIDE_STORAGE1_SLOT_STATUS[slot_index]->Get__DATA(wfr_sts);
					sEXT_CH__SIDE_STORAGE1_SLOT_TITLE[slot_index]->Get__DATA(wfr_title);

					dEXT_CH__SIDE_STORAGE1_SLOT_STATUS[slot_index]->Set__DATA(STR__NONE);
					sEXT_CH__SIDE_STORAGE1_SLOT_TITLE[slot_index]->Set__DATA("");

					l__wfr_sts[k] = wfr_sts;
					l__wfr_title[k] = wfr_title;
				}
			}
		}
	}
	// SIDE STORAGE2 ...
	if(check_flag < 0)
	{
		if((stn_name.CompareNoCase(STR__BUFF2) == 0)
		|| (stn_name.CompareNoCase(STR__ST2) == 0))
		{
			check_flag = 1;

			// ...
			int cfg_max = aEXT_CH__SIDE_STORAGE2_CFG_SLOT_MAX->Get__VALUE();

			// ...
			int slot_limit = l__slot_id.GetSize();

			for(int k=0; k<slot_limit; k++)
			{
				int slot_id = l__slot_id[k];
				int slot_index = slot_id - 1;

				if(slot_index <  0)					return -1;
				if(slot_index >= cfg_max)			return -1;

				if(place_flag > 0)
				{
					CString wfr_sts   = l__wfr_sts[k];
					CString wfr_title = l__wfr_title[k];

					dEXT_CH__SIDE_STORAGE2_SLOT_STATUS[slot_index]->Set__DATA(wfr_sts);
					sEXT_CH__SIDE_STORAGE2_SLOT_TITLE[slot_index]->Set__DATA(wfr_title);
				}
				else
				{
					CString wfr_sts;
					CString wfr_title;

					dEXT_CH__SIDE_STORAGE2_SLOT_STATUS[slot_index]->Get__DATA(wfr_sts);
					sEXT_CH__SIDE_STORAGE2_SLOT_TITLE[slot_index]->Get__DATA(wfr_title);

					dEXT_CH__SIDE_STORAGE2_SLOT_STATUS[slot_index]->Set__DATA(STR__NONE);
					sEXT_CH__SIDE_STORAGE2_SLOT_TITLE[slot_index]->Set__DATA("");

					l__wfr_sts[k] = wfr_sts;
					l__wfr_title[k] = wfr_title;
				}
			}
		}
	}

	if(place_flag < 0)
	{
		int slot_limit = l__slot_id.GetSize();

		for(int k=0; k<slot_limit; k++)
		{
			CString wfr_arm   = l__arm_type[k];
			CString wfr_sts   = l__wfr_sts[k];
			CString wfr_title = l__wfr_title[k];
	
			if(wfr_sts.GetLength() > 0)
			{
				if(wfr_arm.CompareNoCase(_ARM_A) == 0)
				{
					dCH__OTR_OUT_MON__ARM_A_MATERIAL_STATUS->Set__DATA(wfr_sts);
					sCH__OTR_OUT_MON__ARM_A_MATERIAL_TITLE->Set__DATA(wfr_title);
				}
				else if(wfr_arm.CompareNoCase(_ARM_B) == 0)
				{
					dCH__OTR_OUT_MON__ARM_B_MATERIAL_STATUS->Set__DATA(wfr_sts);
					sCH__OTR_OUT_MON__ARM_B_MATERIAL_TITLE->Set__DATA(wfr_title);
				}
			}
		}
	}

	return 1;
}
