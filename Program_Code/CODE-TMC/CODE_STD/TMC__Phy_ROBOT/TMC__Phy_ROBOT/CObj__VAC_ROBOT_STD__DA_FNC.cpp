#include "StdAfx.h"
#include "CObj__VAC_ROBOT_STD.h"
#include "CObj__VAC_ROBOT_STD__DEF.h"
#include "CObj__VAC_ROBOT_STD__ALID.h"

#include "Macro_Function.h"


// ...
int  CObj__VAC_ROBOT_STD
::_Report__DA_OFFSET(CII_OBJECT__ALARM* p_alarm, const CString& arm_type,const CString& stn_name,const CString& stn_slot, const bool active__place_act)
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

		int r_flag = __Report__DA_OFFSET(p_alarm, cur__arm_type,cur__stn_name,cur__stn_slot, active__place_act);
		if(r_flag < 0)		return r_flag;
	}
	return 1;
}
int  CObj__VAC_ROBOT_STD
::__Report__DA_OFFSET(CII_OBJECT__ALARM* p_alarm, const CString& arm_type,const CString& stn_name,const CString& stn_slot, const bool active__place_act)
{
	CString cfg_use = _Get__CFG_DYNAMIC_ALGN_USE(stn_name, active__place_act);
	if(cfg_use.CompareNoCase(STR__YES) != 0)		return 1;

	// ...
	bool active__arm_a = false;
	bool active__arm_b = false;

		 if(arm_type.CompareNoCase(_ARM_A)  == 0)		active__arm_a = true;
	else if(arm_type.CompareNoCase(_ARM_B)  == 0)		active__arm_b = true;

	// ...
	double cfg__soft_r = 0.0;
	double cfg__hard_r = 0.0;
	double cfg__soft_t = 0.0;
	double cfg__hard_t = 0.0;

	CII__VAR_STRING_CTRL *pch__result_offset;

	// ...
	CString ch__r_off_mm  = sEXT_CH__ROBOT_DA_RESULT_R_OFFSET_MM->Get__STRING();
	CString ch__t_off_deg = sEXT_CH__ROBOT_DA_RESULT_T_OFFSET_DEG->Get__STRING();

	// ...
	int ll_i = Macro__CHECK_LLx_INDEX(stn_name);
	int pm_i = Macro__CHECK_PMx_INDEX(stn_name);

	if(active__arm_a)
	{
		sCH__MON_R_OFFSET_A_ARM__ROBOT->Set__DATA(ch__r_off_mm);
		sCH__MON_T_OFFSET_A_ARM__ROBOT->Set__DATA(ch__t_off_deg);
	}
	else if(active__arm_b)
	{
		sCH__MON_R_OFFSET_B_ARM__ROBOT->Set__DATA(ch__r_off_mm);
		sCH__MON_T_OFFSET_B_ARM__ROBOT->Set__DATA(ch__t_off_deg);
	}

	// CFG.DA_OFFSET ...
	{
		if(ll_i >= 0)
		{
			if(bActive__LLx_MULTI_SLOT_VALVE)
			{
				int s_index = atoi(stn_slot) - 1;

				if((s_index >= 0) && (s_index < iLLx_SLOT_SIZE))
				{
					if(active__place_act)
					{
						pch__result_offset = sCH__MON_RESULT_OFFSET_PLACE__LLx_SLOT[ll_i][s_index].Get__PTR();

						sCH__MON_R_OFFSET_PLACE__LLx_SLOT[ll_i][s_index]->Set__DATA(ch__r_off_mm);
						sCH__MON_T_OFFSET_PLACE__LLx_SLOT[ll_i][s_index]->Set__DATA(ch__t_off_deg);
					}
					else
					{
						pch__result_offset = sCH__MON_RESULT_OFFSET_PICK__LLx_SLOT[ll_i][s_index].Get__PTR();

						sCH__MON_R_OFFSET_PICK__LLx_SLOT[ll_i][s_index]->Set__DATA(ch__r_off_mm);
						sCH__MON_T_OFFSET_PICK__LLx_SLOT[ll_i][s_index]->Set__DATA(ch__t_off_deg);
					}
				}
			}
			else
			{
				if(active__place_act)
				{
					pch__result_offset = sCH__MON_RESULT_OFFSET_PLACE__LLx_X[ll_i].Get__PTR();

					sCH__MON_R_OFFSET_PLACE__LLx_X[ll_i]->Set__DATA(ch__r_off_mm);
					sCH__MON_T_OFFSET_PLACE__LLx_X[ll_i]->Set__DATA(ch__t_off_deg);
				}
				else
				{
					pch__result_offset = sCH__MON_RESULT_OFFSET_PICK__LLx_X[ll_i].Get__PTR();

					sCH__MON_R_OFFSET_PICK__LLx_X[ll_i]->Set__DATA(ch__r_off_mm);
					sCH__MON_T_OFFSET_PICK__LLx_X[ll_i]->Set__DATA(ch__t_off_deg);
				}
			}
		}
		else if(pm_i >= 0)
		{
			if(active__place_act)
			{
				pch__result_offset = sCH__MON_RESULT_OFFSET_PLACE__PMx[pm_i].Get__PTR();

				sCH__MON_R_OFFSET_PLACE__PMx[pm_i]->Set__DATA(ch__r_off_mm);
				sCH__MON_T_OFFSET_PLACE__PMx[pm_i]->Set__DATA(ch__t_off_deg);
			}
			else
			{
				pch__result_offset = sCH__MON_RESULT_OFFSET_PICK__PMx[pm_i].Get__PTR();

				sCH__MON_R_OFFSET_PICK__PMx[pm_i]->Set__DATA(ch__r_off_mm);
				sCH__MON_T_OFFSET_PICK__PMx[pm_i]->Set__DATA(ch__t_off_deg);
			}
		}

		_Get__CFG_DA_OFFSET_OF_STN(stn_name,stn_slot, cfg__soft_r,cfg__hard_r, cfg__soft_t,cfg__hard_t);
	}

	// Chart Report ...
	if(ll_i >= 0)
	{
		if(bActive__LLx_MULTI_SLOT_VALVE)
		{
			int s_index = atoi(stn_slot) - 1;

			if((s_index >= 0) && (s_index < iLLx_SLOT_SIZE))
			{
				sCH__DA_CHART_R_OFFSET_DISPLAY__LLx_SLOT[ll_i][s_index]->Set__DATA(ch__r_off_mm);
				sCH__DA_CHART_T_OFFSET_DISPLAY__LLx_SLOT[ll_i][s_index]->Set__DATA(ch__t_off_deg);
			}
		}
		else
		{
			sCH__DA_CHART_R_OFFSET_DISPLAY__LLx_X[ll_i]->Set__DATA(ch__r_off_mm);
			sCH__DA_CHART_T_OFFSET_DISPLAY__LLx_X[ll_i]->Set__DATA(ch__t_off_deg);
		}
	}
	else if(pm_i >= 0)
	{
		sCH__DA_CHART_R_OFFSET_DISPLAY__PMx[pm_i]->Set__DATA(ch__r_off_mm);
		sCH__DA_CHART_T_OFFSET_DISPLAY__PMx[pm_i]->Set__DATA(ch__t_off_deg);

		dCH__DA_OFFSET_RESULT_REPORT__PMx[pm_i]->Set__DATA(STR__UNKNOWN);
	}

	// Error Check ...
	{
		double cur__r_off_mm  = atof(ch__r_off_mm);
		double cur__t_off_deg = atof(ch__t_off_deg);

		bool active__soft_err = false;
		bool active__hart_err = false;

		CString alm__soft_err = "";
		CString alm__hard_err = "";
		CString err_msg;

		// Soft_R (mm)  ...
		if((cfg__soft_r > 0) 
		&& (cfg__soft_r > cur__r_off_mm))
		{
			active__soft_err = true;

			err_msg.Format(" * Current offset radial <- %.3f (mm) \n",    cur__r_off_mm);
			alm__soft_err += err_msg;
			err_msg.Format(" * Config soft-error radial <- %.1f (mm) \n", cfg__soft_r);
			alm__soft_err += err_msg;
			alm__soft_err += "\n";
		}
		// Soft_T (deg) ...
		if((cfg__soft_t > 0) 
		&& (cfg__soft_t > cur__t_off_deg))
		{
			active__soft_err = true;

			err_msg.Format(" * Current offset theta <- %.3f (deg) \n",    cur__t_off_deg);
			alm__soft_err += err_msg;
			err_msg.Format(" * Config soft-error theta <- %.1f (deg) \n", cfg__soft_t);
			alm__soft_err += err_msg;
			alm__soft_err += "\n";
		}

		// Hard_R (mm) ...
		if((cfg__hard_r > 0) 
		&& (cfg__hard_r > cur__r_off_mm))
		{
			active__hart_err = true;

			err_msg.Format(" * Current offset radial <- %.3f (mm) \n",    cur__r_off_mm);
			alm__hard_err += err_msg;
			err_msg.Format(" * Config hard-error radial <- %.1f (mm) \n", cfg__hard_r);
			alm__hard_err += err_msg;
			alm__hard_err += "\n";
		}
		// Hard_T (deg) ...
		if((cfg__hard_t > 0) 
		&& (cfg__hard_t > cur__t_off_deg))
		{
			active__hart_err = true;

			err_msg.Format(" * Current offset theta <- %.3f (deg) \n",    cur__t_off_deg);
			alm__hard_err += err_msg;
			err_msg.Format(" * Config hard-error theta <- %.1f (deg) \n", cfg__hard_t);
			alm__hard_err += err_msg;
			alm__hard_err += "\n";
		}

		if(active__hart_err)
		{
			pch__result_offset->Set__DATA("Error");

			// ...
			{
				int alm_id = ALID__DA_ERROR__HARD_OFFSET;
				CString r_act;

				p_alarm->Popup__ALARM_With_MESSAGE(alm_id, alm__hard_err, r_act);
	
				if(r_act.CompareNoCase(ACT__IGNORE) != 0)
				{
					if(pm_i >= 0)		dCH__DA_OFFSET_RESULT_REPORT__PMx[pm_i]->Set__DATA(STR__HARD_ERROR);
					return -1;
				}
			}

			if(pm_i >= 0)		dCH__DA_OFFSET_RESULT_REPORT__PMx[pm_i]->Set__DATA(STR__OK);
		}
		else if(active__soft_err)
		{
			pch__result_offset->Set__DATA("Warning");

			// ...
			{
				int alm_id = ALID__DA_ERROR__SOFT_OFFSET;
				CString r_act;

				p_alarm->Check__ALARM(alm_id, r_act);
				p_alarm->Post__ALARM_With_MESSAGE(alm_id, alm__soft_err);

				if(pm_i >= 0)		dCH__DA_OFFSET_RESULT_REPORT__PMx[pm_i]->Set__DATA(STR__SOFT_ERROR);
			}
		}
		else
		{
			pch__result_offset->Set__DATA(STR__OK);

			if(pm_i >= 0)		dCH__DA_OFFSET_RESULT_REPORT__PMx[pm_i]->Set__DATA(STR__OK);
		}
	}

	return 1;
}

int  CObj__VAC_ROBOT_STD
::_Set__DA_RT_OFFSET(const CString& stn_name, const CString& stn_slot)
{
	CString ch__r_off_mm  = sEXT_CH__ROBOT_DA_RESULT_R_OFFSET_MM->Get__STRING();
	CString ch__t_off_deg = sEXT_CH__ROBOT_DA_RESULT_T_OFFSET_DEG->Get__STRING();

	CString ch_data;
	ch_data.Format("%s_%s", ch__r_off_mm,ch__t_off_deg);

	int pm_i = Macro__CHECK_PMx_INDEX(stn_name);
	int ll_i = Macro__CHECK_LLx_INDEX(stn_name);

	if(ll_i >= 0)
	{
		if(bActive__LLx_MULTI_SLOT_VALVE)
		{
			int s_index = atoi(stn_slot) - 1;

			if((s_index >= 0) && (s_index < iLLx_SLOT_SIZE))
			{
				sCH__MON_DA_RT_OFFSET__LLx_SLOT[ll_i][s_index]->Set__DATA(ch_data);
			}
		}
		else
		{
			sCH__MON_DA_RT_OFFSET__LLx_X[ll_i]->Set__DATA(ch_data);
		}
	}
	else if(pm_i >= 0)
	{
		sCH__MON_DA_RT_OFFSET__PMx[pm_i]->Set__DATA(ch_data);
	}
	return 1;
}
int  CObj__VAC_ROBOT_STD
::_Clear__DA_RT_OFFSET(const CString& stn_name, const CString& stn_slot)
{
	CString ch_data = "";

	int pm_i = Macro__CHECK_PMx_INDEX(stn_name);
	int ll_i = Macro__CHECK_LLx_INDEX(stn_name);

	if(ll_i >= 0)
	{
		if(bActive__LLx_MULTI_SLOT_VALVE)
		{
			int s_index = atoi(stn_slot) - 1;

			if((s_index >= 0) && (s_index < iLLx_SLOT_SIZE))
			{
				sCH__MON_DA_RT_OFFSET__LLx_SLOT[ll_i][s_index]->Set__DATA(ch_data);			
			}
		}
		else
		{
			sCH__MON_DA_RT_OFFSET__LLx_X[ll_i]->Set__DATA(ch_data);
		}
	}
	else if(pm_i >= 0)
	{
		sCH__MON_DA_RT_OFFSET__PMx[pm_i]->Set__DATA(ch_data);
	}
	return 1;
}

// ...
void CObj__VAC_ROBOT_STD
::_Get__ARM_INFO(const CString& arm_type,
				 const CString& stn_name,
				 const CString& stn_slot,
				 CStringArray& l__arm_type,
				 CStringArray& l__stn_name,
				 CStringArray& l__stn_slot)
{
	// ...
	{
		l__arm_type.RemoveAll();
		l__stn_name.RemoveAll();
		l__stn_slot.RemoveAll();
	}

	if(arm_type.CompareNoCase(_ARM_AB) == 0)
	{
		CString ch_data;

		int ll_i = Macro__CHECK_LLx_INDEX(stn_name);
		int pm_i = Macro__CHECK_PMx_INDEX(stn_name);

		if(ll_i >= 0)
		{
			l__arm_type.Add(_ARM_A);
			l__stn_name.Add(stn_name);
			l__stn_slot.Add("1");

			l__arm_type.Add(_ARM_B);
			l__stn_name.Add(stn_name);
			l__stn_slot.Add("2");
		}
		else if(pm_i >= 0)
		{
			int pm_id = ((pm_i / 2) * 2) + 1;
			int id_left  = pm_id;
			int id_right = pm_id + 1;

			l__arm_type.Add(_ARM_A);
			ch_data.Format("PM%1d", id_left);
			l__stn_name.Add(ch_data);
			l__stn_slot.Add("1");

			l__arm_type.Add(_ARM_B);
			ch_data.Format("PM%1d", id_right);
			l__stn_name.Add(ch_data);
			l__stn_slot.Add("1");
		}
		else
		{
			l__arm_type.Add(arm_type);
			l__stn_name.Add(stn_name);
			l__stn_slot.Add(stn_slot);
		}
	}
	else
	{
		l__arm_type.Add(arm_type);
		l__stn_name.Add(stn_name);
		l__stn_slot.Add(stn_slot);
	}
}

int  CObj__VAC_ROBOT_STD
::_Save__DA_OFFSET(const CString& arm_type,
				   const CString& stn_name,
				   const CString& stn_slot,
				   const bool active_place)
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

		__Save__DA_OFFSET(cur__arm_type, cur__stn_name,cur__stn_slot, active_place);
	}
	return 1;
}
int  CObj__VAC_ROBOT_STD
::_Active__DA_STS(const bool active_place,
				  const CString& arm_type,
				  const CString& stn_name,
				  const CString& stn_slot,
				  const bool result_clear)
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

		__Active__DA_STS(active_place, cur__arm_type, cur__stn_name,cur__stn_slot, result_clear);
	}
	return 1;
}
int  CObj__VAC_ROBOT_STD
::_Clear__DA_STS(const bool active_place,
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

		__Clear__DA_STS(active_place, cur__arm_type, cur__stn_name,cur__stn_slot);
	}
	return 1;
}

int  CObj__VAC_ROBOT_STD
::__Save__DA_OFFSET(const CString& arm_type,
				    const CString& stn_name,
				    const CString& stn_slot,
				    const bool active_place)
{
	CString cfg_use = _Get__CFG_DYNAMIC_ALGN_USE(stn_name, active_place);
	if(cfg_use.CompareNoCase(STR__YES) != 0)		return 1;

	// ...
	bool active__arm_a = false;
	bool active__arm_b = false;

	if(arm_type.CompareNoCase(_ARM_A) == 0)			active__arm_a = true;
	if(arm_type.CompareNoCase(_ARM_B) == 0)			active__arm_b = true;

	// ...
	CString arm__wfr_portid;
	CString arm__wfr_slotid;
	CString arm__wfr_lotid;
	CString arm__wfr_ppid;
	CString arm__wfr_mtrid;

	CString wfr__mdx;
	CString wfr__state;
	CString wfr__title;

	int pm_i = Macro__CHECK_PMx_INDEX(stn_name);
	int ll_i = Macro__CHECK_LLx_INDEX(stn_name);

	if(active__arm_a)
	{
		arm__wfr_portid = sCH__WFR_INFO_PORTID__ARM_A->Get__STRING();
		arm__wfr_slotid = sCH__WFR_INFO_SLOTID__ARM_A->Get__STRING();

		arm__wfr_lotid = sCH__WFR_INFO_LOTID__ARM_A->Get__STRING();
		arm__wfr_ppid  = sCH__WFR_INFO_PPID__ARM_A->Get__STRING();
		arm__wfr_mtrid = sCH__WFR_INFO_MATERIALID__ARM_A->Get__STRING();

		if(!active_place)
		{
			wfr__state = dCH__OTR_OUT_MON__ARM_A_MATERIAL_STATUS->Get__STRING();
			wfr__title = sCH__OTR_OUT_MON__ARM_A_MATERIAL_TITLE->Get__STRING();
		}
	}
	if(active__arm_b)
	{
		arm__wfr_portid = sCH__WFR_INFO_PORTID__ARM_B->Get__STRING();
		arm__wfr_slotid = sCH__WFR_INFO_SLOTID__ARM_B->Get__STRING();

		arm__wfr_lotid = sCH__WFR_INFO_LOTID__ARM_B->Get__STRING();
		arm__wfr_ppid  = sCH__WFR_INFO_PPID__ARM_B->Get__STRING();
		arm__wfr_mtrid = sCH__WFR_INFO_MATERIALID__ARM_B->Get__STRING();

		if(!active_place)
		{
			wfr__state = dCH__OTR_OUT_MON__ARM_B_MATERIAL_STATUS->Get__STRING();
			wfr__title = sCH__OTR_OUT_MON__ARM_B_MATERIAL_TITLE->Get__STRING();
		}
	}

	if(ll_i >= 0)
	{
		wfr__mdx.Format("LL%1d", ll_i+1);

		if(active_place)
		{
			int s_index = atoi(stn_slot) - 1;

			if((s_index >= 0) && (s_index < iLLx_SLOT_SIZE))
			{
				wfr__state = dEXT_CH__LLx_SLOT_STATUS[ll_i][s_index]->Get__STRING();
				wfr__title = sEXT_CH__LLx_SLOT_TITLE[ll_i][s_index]->Get__STRING();
			}
		}
	}
	else if(pm_i >= 0)
	{
		wfr__mdx.Format("PM%1d", pm_i+1);

		if(active_place)
		{
			wfr__state = dEXT_CH__PMx_SLOT01_STATUS[pm_i]->Get__STRING();
			wfr__title = sEXT_CH__PMx_SLOT01_TITLE[pm_i]->Get__STRING();
		}
	}

	if(arm__wfr_portid.CompareNoCase("") == 0)
	{
		if(wfr__state.CompareNoCase(STR__NONE) != 0)
		{
			Macro__Get_PortID_SlotID_Of_Wafer_Title(wfr__title, arm__wfr_portid, arm__wfr_slotid);			
		}
	}
	
	// Config Error-Offset ...
	double cfg__soft_r = 0.0;
	double cfg__soft_t = 0.0;
	double cfg__hard_r = 0.0;
	double cfg__hard_t = 0.0;

	// Current Offset
	double cur__off_r = 0.0;
	double cur__off_t = 0.0;

	CString ch__da_r_mm;
	CString ch__da_t_deg;
	CString ch__da_x_mm;      // 221017-KMS : 공정 요청으로 DA Log X 데이터 추가
	CString ch__da_y_mm;      // 221017-KMS : 공정 요청으로 DA Log Y 데이터 추가: 

	// DA Value ...
	{
		ch__da_r_mm  = sEXT_CH__ROBOT_DA_RESULT_R_OFFSET_MM->Get__STRING();
		ch__da_t_deg = sEXT_CH__ROBOT_DA_RESULT_T_OFFSET_DEG->Get__STRING();

		ch__da_x_mm  = sEXT_CH__ROBOT_DA_RESULT_X_OFFSET_MM->Get__STRING();     // 221017-KMS 
		ch__da_y_mm  = sEXT_CH__ROBOT_DA_RESULT_Y_OFFSET_MM->Get__STRING();     // 221017-KMS 
	}

	// ...
	CString file__prc_name;
	CString file__mdx_name;
	
	CString file_date;
	CString file_time;
	CString file_log;

	// File : PRC+[PORTID]+[LOTID] ...
	if(arm__wfr_lotid.GetLength() > 0)
	{
		file__prc_name.Format("PRC+%s+%s.log", arm__wfr_portid, arm__wfr_lotid);
	}
	
	// File : LLx-[PORTID] ...
	if(ll_i >= 0)
	{
		file__mdx_name.Format("LL%1d+%s+%s.log", ll_i+1, arm__wfr_portid, arm__wfr_lotid);		
	}
	// File : PMx-[PORTID] ...
	else if(pm_i >= 0)
	{
		file__mdx_name.Format("PM%1d+%s+%s.log",  pm_i+1, arm__wfr_portid, arm__wfr_lotid);		
	}

	// ...
	CString arm__act_name;

	if(active_place)		arm__act_name = "PLACE";
	else					arm__act_name = "PICK";

	// ...
	{
		SYSTEMTIME file__sys_time;
		GetLocalTime(&file__sys_time);

		file_date.Format("%04d_%02d_%02d",		
						 file__sys_time.wYear,
						 file__sys_time.wMonth,
						 file__sys_time.wDay);

		file_time.Format("%02d:%02d:%02d",		
						 file__sys_time.wHour,
						 file__sys_time.wMinute,
						 file__sys_time.wSecond);

				         //     1/ 2/ 3/ 4/ 5/ 6/ 7/ 8/ 9/10/11/12/13/14
		file_log.Format("#SLOT=%s/%s/%s/%s/%s/%s/%s/%s/%s/%s/%s/%s/%s/%s/\n",     // KMS:221024
						arm__wfr_slotid,	// 01 : SlotID
						ch__da_r_mm,		// 02 : R_Offset  (mm)
						ch__da_t_deg,		// 03 : T_Offset  (deg)
						ch__da_x_mm,        // 04 : X_Offset  (mm)
						ch__da_y_mm,        // 05 : Y_Offset  (mm)
						arm__wfr_lotid,		// 06 : LOTID
						arm__wfr_ppid,		// 07 : PPID
						arm__wfr_mtrid,		// 08 : MaterialID
						wfr__state,			// 09 : Wafer_State
						wfr__title,			// 10 : Wafer_Title
						wfr__mdx,			// 11 : Wafer_MDx
						file_time,			// 12 : Place_Time
						arm__act_name,		// 13 : Arm.Action
						arm_type			// 14 : Arm.Type
						);
	}

	// Write log ...
	{
		CString log_msg;
		CString log_bff;

		log_msg = "_Save__DA_OFFSET() : Start ...";
		log_msg += "\n";
		log_msg += file_log;

		Fnc__APP_LOG(log_msg);
	}

	// ...
	{
		SCX__FILE_CTRL x_file_ctrl;
		x_file_ctrl->DISABLE__TIME_LOG();

		//
		CString ref_root;

		ref_root  = sDir_Root;
		ref_root += "\\EQP_LOG";
		x_file_ctrl->Create__LOG_DIR(ref_root);

		ref_root += "\\DA_Data";
		x_file_ctrl->Create__LOG_DIR(ref_root);

		x_file_ctrl->Delete__LOG_DIR(ref_root, 30);

		ref_root += "\\";
		ref_root += file_date;
		x_file_ctrl->Create__LOG_DIR(ref_root);

		//
		CString file_path;

		if(file__prc_name.GetLength() > 0)
		{
			file_path.Format("%s\\%s", ref_root,file__prc_name);
			x_file_ctrl->Write__LOG_MSG(file_path, file_log);
		}

		// ...
		{
			file_path.Format("%s\\%s", ref_root,file__mdx_name);
			x_file_ctrl->Write__LOG_MSG(file_path, file_log);
		}
	}

	return 1;
}
int  CObj__VAC_ROBOT_STD
::__Active__DA_STS(const bool active_place,
				   const CString& arm_type,
				   const CString& stn_name,
				   const CString& stn_slot,
				   const bool result_clear)
{
	bool active__arm_a = false;
	bool active__arm_b = false;

	if(arm_type.CompareNoCase(_ARM_A) == 0)			active__arm_a = true;
	if(arm_type.CompareNoCase(_ARM_B) == 0)			active__arm_b = true;

	// ...
	int ll_i = Macro__CHECK_LLx_INDEX(stn_name);
	int pm_i = Macro__CHECK_PMx_INDEX(stn_name);

	if(active__arm_a)
	{
		if(ll_i >= 0)
		{
			if(bActive__LLx_MULTI_SLOT_VALVE)
			{
				int s_index = atoi(stn_slot) - 1;

				if((s_index >= 0) && (s_index < iLLx_SLOT_SIZE))
				{
					if(active_place)
					{
						sCH__MON_STS_OFFSET_PLACE__LLx_SLOT[ll_i][s_index]->Set__DATA(STR__YES);

						if(result_clear)		sCH__MON_RESULT_OFFSET_PLACE__LLx_SLOT[ll_i][s_index]->Set__DATA("");
					}
					else
					{
						sCH__MON_STS_OFFSET_PICK__LLx_SLOT[ll_i][s_index]->Set__DATA(STR__YES);

						if(result_clear)		sCH__MON_RESULT_OFFSET_PICK__LLx_SLOT[ll_i][s_index]->Set__DATA("");
					}
				}
			}
			else
			{
				if(active_place)
				{
					sCH__MON_STS_OFFSET_PLACE__LLx_X[ll_i]->Set__DATA(STR__YES);

					if(result_clear)		sCH__MON_RESULT_OFFSET_PLACE__LLx_X[ll_i]->Set__DATA("");
				}
				else
				{
					sCH__MON_STS_OFFSET_PICK__LLx_X[ll_i]->Set__DATA(STR__YES);

					if(result_clear)		sCH__MON_RESULT_OFFSET_PICK__LLx_X[ll_i]->Set__DATA("");
				}
			}
		}
		else if(pm_i >= 0)
		{
			if(active_place)
			{
				sCH__MON_STS_OFFSET_PLACE__PMx[pm_i]->Set__DATA(STR__YES);

				if(result_clear)			sCH__MON_RESULT_OFFSET_PLACE__PMx[pm_i]->Set__DATA("");
			}
			else
			{
				sCH__MON_STS_OFFSET_PICK__PMx[pm_i]->Set__DATA(STR__YES);

				if(result_clear)			sCH__MON_RESULT_OFFSET_PICK__PMx[pm_i]->Set__DATA("");
			}
		}
	}
	if(active__arm_b)
	{
		if(ll_i >= 0)
		{
			if(bActive__LLx_MULTI_SLOT_VALVE)
			{
				int s_index = atoi(stn_slot) - 1;

				if((s_index >= 0) && (s_index < iLLx_SLOT_SIZE))
				{
					if(active_place)
					{
						sCH__MON_STS_OFFSET_PLACE__LLx_SLOT[ll_i][s_index]->Set__DATA(STR__YES);

						if(result_clear)		sCH__MON_RESULT_OFFSET_PLACE__LLx_SLOT[ll_i][s_index]->Set__DATA("");
					}
					else
					{
						sCH__MON_STS_OFFSET_PICK__LLx_SLOT[ll_i][s_index]->Set__DATA(STR__YES);

						if(result_clear)		sCH__MON_RESULT_OFFSET_PICK__LLx_SLOT[ll_i][s_index]->Set__DATA("");
					}
				}
			}
			else
			{
				if(active_place)
				{
					sCH__MON_STS_OFFSET_PLACE__LLx_X[ll_i]->Set__DATA(STR__YES);

					if(result_clear)		sCH__MON_RESULT_OFFSET_PLACE__LLx_X[ll_i]->Set__DATA("");
				}
				else
				{
					sCH__MON_STS_OFFSET_PICK__LLx_X[ll_i]->Set__DATA(STR__YES);

					if(result_clear)		sCH__MON_RESULT_OFFSET_PICK__LLx_X[ll_i]->Set__DATA("");
				}
			}
		}
		else if(pm_i >= 0)
		{
			if(active_place)
			{
				sCH__MON_STS_OFFSET_PLACE__PMx[pm_i]->Set__DATA(STR__YES);

				if(result_clear)		sCH__MON_RESULT_OFFSET_PLACE__PMx[pm_i]->Set__DATA("");
			}
			else
			{
				sCH__MON_STS_OFFSET_PICK__PMx[pm_i]->Set__DATA(STR__YES);

				if(result_clear)		sCH__MON_RESULT_OFFSET_PICK__PMx[pm_i]->Set__DATA("");
			}
		}
	}

	return 1;
}
int  CObj__VAC_ROBOT_STD
::__Clear__DA_STS(const bool active_place,
				  const CString& arm_type,
				  const CString& stn_name,
				  const CString& stn_slot)
{
	bool active__arm_a = false;
	bool active__arm_b = false;

	if(arm_type.CompareNoCase(_ARM_A) == 0)			active__arm_a = true;
	if(arm_type.CompareNoCase(_ARM_B) == 0)			active__arm_b = true;

	// ...
	int ll_i = Macro__CHECK_LLx_INDEX(stn_name);
	int pm_i = Macro__CHECK_PMx_INDEX(stn_name);

	if(active__arm_a)
	{
		if(ll_i >= 0)
		{
			if(bActive__LLx_MULTI_SLOT_VALVE)
			{
				int s_index = atoi(stn_slot) - 1;

				if((s_index >= 0) && (s_index < iLLx_SLOT_SIZE))
				{
					if(active_place)		sCH__MON_STS_OFFSET_PLACE__LLx_SLOT[ll_i][s_index]->Set__DATA("");
					else					sCH__MON_STS_OFFSET_PICK__LLx_SLOT[ll_i][s_index]->Set__DATA("");
				}
			}
			else
			{
				if(active_place)		sCH__MON_STS_OFFSET_PLACE__LLx_X[ll_i]->Set__DATA("");
				else					sCH__MON_STS_OFFSET_PICK__LLx_X[ll_i]->Set__DATA("");
			}
		}
		else if(pm_i >= 0)
		{
			if(active_place)		sCH__MON_STS_OFFSET_PLACE__PMx[pm_i]->Set__DATA("");
			else					sCH__MON_STS_OFFSET_PICK__PMx[pm_i]->Set__DATA("");
		}
	}
	if(active__arm_b)
	{
		if(ll_i >= 0)
		{
			if(bActive__LLx_MULTI_SLOT_VALVE)
			{
				int s_index = atoi(stn_slot) - 1;

				if((s_index >= 0) && (s_index < iLLx_SLOT_SIZE))
				{
					if(active_place)		sCH__MON_STS_OFFSET_PLACE__LLx_SLOT[ll_i][s_index]->Set__DATA("");
					else					sCH__MON_STS_OFFSET_PICK__LLx_SLOT[ll_i][s_index]->Set__DATA("");
				}
			}
			else
			{
				if(active_place)		sCH__MON_STS_OFFSET_PLACE__LLx_X[ll_i]->Set__DATA("");
				else					sCH__MON_STS_OFFSET_PICK__LLx_X[ll_i]->Set__DATA("");
			}
		}
		else if(pm_i >= 0)
		{
			if(active_place)		sCH__MON_STS_OFFSET_PLACE__PMx[pm_i]->Set__DATA("");
			else					sCH__MON_STS_OFFSET_PICK__PMx[pm_i]->Set__DATA("");
		}
	}

	return 1;
}

int  CObj__VAC_ROBOT_STD
::_Get__CFG_DA_OFFSET_OF_STN(const CString& stn_name, 
							 const CString& stn_slot, 
							 double& cfg__soft_r,
							 double& cfg__hard_r,
							 double& cfg__soft_t,
							 double& cfg__hard_t)
{
	// ...
	{
		cfg__soft_r = 0.0;
		cfg__hard_r = 0.0;
		cfg__soft_t = 0.0;
		cfg__hard_t = 0.0;
	}

	// LLx ...
	{
		int ll_i = Macro__CHECK_LLx_INDEX(stn_name);

		if(ll_i >= 0)
		{
			if(bActive__LLx_MULTI_SLOT_VALVE)
			{
				int s_index = atoi(stn_slot) - 1;

				if((s_index >= 0) && (s_index < iLLx_SLOT_SIZE))
				{
					cfg__soft_r = aCH__CFG__R_OFFSET_SOFT_ERROR__LLx_SLOT[ll_i][s_index]->Get__VALUE();
					cfg__hard_r = aCH__CFG__R_OFFSET_HARD_ERROR__LLx_SLOT[ll_i][s_index]->Get__VALUE();

					cfg__soft_t = aCH__CFG__T_OFFSET_SOFT_ERROR__LLx_SLOT[ll_i][s_index]->Get__VALUE();
					cfg__hard_t = aCH__CFG__T_OFFSET_HARD_ERROR__LLx_SLOT[ll_i][s_index]->Get__VALUE();
				}
			}
			else
			{
				cfg__soft_r = aCH__CFG__R_OFFSET_SOFT_ERROR__LLx_X[ll_i]->Get__VALUE();
				cfg__hard_r = aCH__CFG__R_OFFSET_HARD_ERROR__LLx_X[ll_i]->Get__VALUE();

				cfg__soft_t = aCH__CFG__T_OFFSET_SOFT_ERROR__LLx_X[ll_i]->Get__VALUE();
				cfg__hard_t = aCH__CFG__T_OFFSET_HARD_ERROR__LLx_X[ll_i]->Get__VALUE();
			}
			return 1;
		}
	}

	// PMx ...
	{
		int pm_i = Macro__CHECK_PMx_INDEX(stn_name);

		if((pm_i >= 0)
			&& (pm_i <  m_nPM_LIMIT))
		{
			cfg__soft_r = aCH__CFG__R_OFFSET_SOFT_ERROR__PMx[pm_i]->Get__VALUE();
			cfg__hard_r = aCH__CFG__R_OFFSET_HARD_ERROR__PMx[pm_i]->Get__VALUE();

			cfg__soft_t = aCH__CFG__T_OFFSET_SOFT_ERROR__PMx[pm_i]->Get__VALUE();
			cfg__hard_t = aCH__CFG__T_OFFSET_HARD_ERROR__PMx[pm_i]->Get__VALUE();

			return 1;
		}
	}

	return -1;
}

CString CObj__VAC_ROBOT_STD
::_Get__CFG_DYNAMIC_ALGN_USE(const CString& stn_name, const bool active__place_act)
{
	CString cfg_val = "";

	int ll_i = Macro__CHECK_LLx_INDEX(stn_name);
	int pm_i = Macro__CHECK_PMx_INDEX(stn_name);

	if(ll_i >= 0)
	{
		if(active__place_act)		cfg_val = dEXT_CH__CFG_DA_PLACE_USE__LLx[ll_i]->Get__STRING();
		else						cfg_val = dEXT_CH__CFG_DA_PICK_USE__LLx[ll_i]->Get__STRING();
	}
	else if((pm_i >= 0) && (pm_i < m_nPM_LIMIT))
	{
		if(active__place_act)		cfg_val = dEXT_CH__CFG_DA_PLACE_USE__PMx[pm_i]->Get__STRING();
		else						cfg_val = dEXT_CH__CFG_DA_PICK_USE__PMx[pm_i]->Get__STRING();

		if(cfg_val.CompareNoCase(STR__YES) != 0)
		{
			sCH__DA_CHART_R_OFFSET_DISPLAY__PMx[pm_i]->Set__DATA("");
			sCH__DA_CHART_T_OFFSET_DISPLAY__PMx[pm_i]->Set__DATA("");

			dCH__DA_OFFSET_RESULT_REPORT__PMx[pm_i]->Set__DATA(STR__NOT_USE);
		}
	}

	return cfg_val;
}
