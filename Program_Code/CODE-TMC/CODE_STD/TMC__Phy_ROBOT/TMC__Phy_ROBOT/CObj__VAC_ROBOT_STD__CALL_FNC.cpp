#include "StdAfx.h"
#include "CObj__VAC_ROBOT_STD.h"
#include "CObj__VAC_ROBOT_STD__DEF.h"
#include "CObj__VAC_ROBOT_STD__ALID.h"

#include "Macro_Function.h"


//-------------------------------------------------------------------------	
int  CObj__VAC_ROBOT_STD
::Call__INIT(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	int flag = pROBOT__OBJ_CTRL->Call__OBJECT(CMMD__INIT);

	if(flag > 0)
	{
		Set_ANI__ROBOT_ALL_RETRACT();
	}
	return flag;
}

int  CObj__VAC_ROBOT_STD
::Call__SET_PARA(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	return pROBOT__OBJ_CTRL->Call__OBJECT(CMMD__SET_PARA);
}

// Pick -----
int  CObj__VAC_ROBOT_STD::
Call__PICK(CII_OBJECT__VARIABLE* p_variable,
		   CII_OBJECT__ALARM* p_alarm,
		   const CString& arm_type,
		   const CString& stn_name,
		   const CString& stn_slot,
		   const int handoff_mode)
{
	_Active__DA_STS(false, arm_type, stn_name,stn_slot, false);

	int r_flag = Fnc__PICK(p_variable,p_alarm, arm_type,stn_name,stn_slot,handoff_mode);
	if(r_flag > 0)
	{
		bool active__place_act = false;

		r_flag = _Report__DA_OFFSET(p_alarm, arm_type,stn_name,stn_slot, active__place_act);
		_Save__DA_OFFSET(arm_type,stn_name,stn_slot, active__place_act);
	}

	_Clear__DA_STS(false, arm_type,stn_name,stn_slot);
	_Clear__DA_RT_OFFSET(stn_name,stn_slot);
	return r_flag;
}
int  CObj__VAC_ROBOT_STD::
Fnc__PICK(CII_OBJECT__VARIABLE* p_variable,
		  CII_OBJECT__ALARM* p_alarm,
		  const CString& arm_type,
		  const CString& stn_name,
		  const CString& stn_slot,
		  const int handoff_mode)
{
	// ...
	{
		CString act_name;

		act_name.Format("pick from %s-%s to Arm-%s", stn_name,stn_slot,arm_type);
		Fnc__APP_LOG(act_name);

		// Valve Check ...
		if(Interlock__CHECK_VALVE_OPEN(p_alarm, arm_type,stn_name,stn_slot, act_name) < 0)
		{
			act_name.Format(" * Interlock__CHECK_VALVE_OPEN() - Error !");
			Fnc__APP_LOG(act_name);
			return -11;
		}

		// Material Check ...
		if(Interlock__CHECK_MATERIAL(p_alarm, false, arm_type,stn_name,stn_slot, act_name) < 0)
		{
			act_name.Format(" * Interlock__CHECK_MATERIAL() - Error !");
			Fnc__APP_LOG(act_name);
			return -12;
		}

		// LLx Slot Check ...
		if(Interlock__LLx_SLOT_CHECK(p_alarm, stn_name, stn_slot) < 0)
		{
			return -13;
		}
	}

	Set_ANI__ROBOT_EXTEND(arm_type,stn_name,stn_slot);

	// ...
	int r_flag = Fnc__ACTION(arm_type,stn_name,stn_slot, CMMD__PICK);

	if(r_flag > 0)
	{
		bool active__err_check = false;

		if(arm_type.CompareNoCase(_ARM_A) == 0)			
		{
			if(dEXT_CH__ROBOT_ARM_A_MATERIAL_STATUS->Check__DATA(STR__NONE) > 0)		active__err_check = true;
		}
		else if(arm_type.CompareNoCase(_ARM_B) == 0)			
		{
			if(dEXT_CH__ROBOT_ARM_B_MATERIAL_STATUS->Check__DATA(STR__NONE) > 0)		active__err_check = true;
		}

		if(active__err_check)
		{
			int alm_id = ALID__PICK__MATERIAL_ERROR;
			CString err_msg;
			CString err_bff;
			CString r_act;

			CString act_info;
			act_info.Format("%s <- %s(%s)", arm_type, stn_name,stn_slot);

			err_msg.Format("Pick : %s \n", act_info);

			if(arm_type.CompareNoCase(_ARM_A) == 0)			
			{
				err_bff.Format(" * %s <- %s \n", 
							   dEXT_CH__ROBOT_ARM_A_MATERIAL_STATUS->Get__CHANNEL_NAME(),
							   dEXT_CH__ROBOT_ARM_A_MATERIAL_STATUS->Get__STRING());
				err_msg += err_bff;
			}
			else if(arm_type.CompareNoCase(_ARM_B) == 0)			
			{
				err_bff.Format(" * %s <- %s \n", 
							   dEXT_CH__ROBOT_ARM_B_MATERIAL_STATUS->Get__CHANNEL_NAME(),
							   dEXT_CH__ROBOT_ARM_B_MATERIAL_STATUS->Get__STRING());
				err_msg += err_bff;
			}

			p_alarm->Popup__ALARM_With_MESSAGE(alm_id, err_msg, r_act);
			return -11;
		}

		Fnc__CHANGE_MATERIAL_INFO(-1,arm_type,stn_name,stn_slot);
		Set_ANI__ROBOT_RETRACT(arm_type,stn_name,stn_slot);
	}
	else
	{
		bool active__wfr_exchange = false;

		if(arm_type.CompareNoCase(_ARM_A) == 0)			
		{
			if(dEXT_CH__ROBOT_ARM_A_MATERIAL_STATUS->Check__DATA(STR__NONE) < 0)		active__wfr_exchange = true;
		}
		else if(arm_type.CompareNoCase(_ARM_B) == 0)			
		{
			if(dEXT_CH__ROBOT_ARM_B_MATERIAL_STATUS->Check__DATA(STR__NONE) < 0)		active__wfr_exchange = true;
		}

		if(active__wfr_exchange)
		{
			Fnc__CHANGE_MATERIAL_INFO(-1,arm_type, stn_name,stn_slot);
		}
	}

	return r_flag;
}

int  CObj__VAC_ROBOT_STD::
Fnc__ACTION(const CString& arm_type,
			const CString& stn_name,
			const CString& stn_slot,
			const CString& cmmd_act)
{
	// ...
	SCX__ASYNC_TIMER_CTRL x_timer_ctrl;
	x_timer_ctrl->START__COUNT_UP(99999);

	// ...
	CStringArray l_para;

	l_para.Add(arm_type);
	l_para.Add(stn_name);
	l_para.Add(stn_slot);

	int flag = pROBOT__OBJ_CTRL->Call__OBJ_MODE(cmmd_act, l_para);

	if(flag > 0)
	{
		CString str_cur;
		CString ch_data;

		int db_index = -1;

			 if(cmmd_act.CompareNoCase(CMMD__PICK)   == 0)			db_index = _ACT_INDEX__PICK;
		else if(cmmd_act.CompareNoCase(CMMD__PLACE)  == 0)			db_index = _ACT_INDEX__PLACE;
		else if(cmmd_act.CompareNoCase(CMMD__ROTATE) == 0)			db_index = _ACT_INDEX__ROTATE;

		if(db_index >= 0)
		{	
			CII__VAR_STRING_CTRL* p_ch_min;
			CII__VAR_STRING_CTRL* p_ch_max;
			CII__VAR_STRING_CTRL* p_ch_avg;
			CII__VAR_STRING_CTRL* p_ch_avg_f;
			CII__VAR_ANALOG_CTRL* p_ch_cnt;

			double cur_sec = x_timer_ctrl->Get__CURRENT_TIME();
			str_cur.Format("%.1f", cur_sec);

			// Robot ...
			{
				sCH__TAS_ACTION_TIME_NOW__RB[db_index]->Set__DATA(str_cur);

				//
				p_ch_min = sCH__TAS_ACTION_TIME_MIN__RB[db_index].Get__PTR();
				p_ch_max = sCH__TAS_ACTION_TIME_MAX__RB[db_index].Get__PTR();
				p_ch_avg = sCH__TAS_ACTION_TIME_AVG__RB[db_index].Get__PTR();
				p_ch_avg_f = sCH__TAS_ACTION_TIME_AVG_F__RB[db_index].Get__PTR();
				p_ch_cnt = aCH__TAS_ACTION_TIME_CNT__RB[db_index].Get__PTR();

				_Update__ACTION_MIN_MAX(p_ch_min,p_ch_max, p_ch_avg,p_ch_avg_f,p_ch_cnt, cur_sec);
			}
			// LLx ...
			{
				int db_i = Macro__CHECK_LLx_INDEX(stn_name);
				if(db_i >= 0)
				{
					if(bActive__LLx_MULTI_SLOT_VALVE)
					{
						int s_index = atoi(stn_slot) - 1;

						if((s_index >= 0) && (s_index < CFG_LLx__SLOT_SIZE))
						{
							sCH__TAS_ACTION_TIME_NOW__LLx_SLOT[db_index][db_i][s_index]->Set__DATA(str_cur);

							//
							p_ch_min = sCH__TAS_ACTION_TIME_MIN__LLx_SLOT[db_index][db_i][s_index].Get__PTR();
							p_ch_max = sCH__TAS_ACTION_TIME_MAX__LLx_SLOT[db_index][db_i][s_index].Get__PTR();
							p_ch_avg = sCH__TAS_ACTION_TIME_AVG__LLx_SLOT[db_index][db_i][s_index].Get__PTR();
							p_ch_avg_f = sCH__TAS_ACTION_TIME_AVG_F__LLx_SLOT[db_index][db_i][s_index].Get__PTR();
							p_ch_cnt = aCH__TAS_ACTION_TIME_CNT__LLx_SLOT[db_index][db_i][s_index].Get__PTR();

							_Update__ACTION_MIN_MAX(p_ch_min,p_ch_max, p_ch_avg,p_ch_avg_f,p_ch_cnt, cur_sec);
						}
					}
					else
					{
						sCH__TAS_ACTION_TIME_NOW__LLx_X[db_index][db_i]->Set__DATA(str_cur);

						//
						p_ch_min = sCH__TAS_ACTION_TIME_MIN__LLx_X[db_index][db_i].Get__PTR();
						p_ch_max = sCH__TAS_ACTION_TIME_MAX__LLx_X[db_index][db_i].Get__PTR();
						p_ch_avg = sCH__TAS_ACTION_TIME_AVG__LLx_X[db_index][db_i].Get__PTR();
						p_ch_avg_f = sCH__TAS_ACTION_TIME_AVG_F__LLx_X[db_index][db_i].Get__PTR();
						p_ch_cnt = aCH__TAS_ACTION_TIME_CNT__LLx_X[db_index][db_i].Get__PTR();

						_Update__ACTION_MIN_MAX(p_ch_min,p_ch_max, p_ch_avg,p_ch_avg_f,p_ch_cnt, cur_sec);
					}
				}
			}
			// PMx ...
			{
				int db_i = Macro__CHECK_PMx_INDEX(stn_name);
				if(db_i >= 0)
				{
					sCH__TAS_ACTION_TIME_NOW__PMx[db_index][db_i]->Set__DATA(str_cur);

					//
					p_ch_min = sCH__TAS_ACTION_TIME_MIN__PMx[db_index][db_i].Get__PTR();
					p_ch_max = sCH__TAS_ACTION_TIME_MAX__PMx[db_index][db_i].Get__PTR();
					p_ch_avg = sCH__TAS_ACTION_TIME_AVG__PMx[db_index][db_i].Get__PTR();
					p_ch_avg_f = sCH__TAS_ACTION_TIME_AVG_F__PMx[db_index][db_i].Get__PTR();
					p_ch_cnt = aCH__TAS_ACTION_TIME_CNT__PMx[db_index][db_i].Get__PTR();
	
					_Update__ACTION_MIN_MAX(p_ch_min,p_ch_max, p_ch_avg,p_ch_avg_f,p_ch_cnt, cur_sec);
				}
			}
		}
	}

	return flag;
}
int  CObj__VAC_ROBOT_STD::
_Update__ACTION_MIN_MAX(CII__VAR_STRING_CTRL* p_ch_min,
						CII__VAR_STRING_CTRL* p_ch_max,
						CII__VAR_STRING_CTRL* p_ch_avg,
						CII__VAR_STRING_CTRL* p_ch_avg_f,
						CII__VAR_ANALOG_CTRL* p_ch_cnt,
						const double cur_sec)
{
	CString ch_data;
	CString str_cur;

	str_cur.Format("%.1f", cur_sec);

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

// Place -----
int  CObj__VAC_ROBOT_STD::
Call__PLACE(CII_OBJECT__VARIABLE* p_variable,
			CII_OBJECT__ALARM* p_alarm,
			const CString& arm_type,
			const CString& stn_name,
			const CString& stn_slot,
			const int handoff_mode)
{
	_Active__DA_STS(true, arm_type, stn_name,stn_slot, true);

	int r_flag = Fnc__PLACE(p_variable,p_alarm, arm_type,stn_name,stn_slot,handoff_mode);
	if(r_flag > 0)
	{
		bool active__place_act = true;

		r_flag = _Report__DA_OFFSET(p_alarm, arm_type,stn_name,stn_slot, active__place_act);
		_Save__DA_OFFSET(arm_type,stn_name,stn_slot, active__place_act);
	}

	_Clear__DA_STS(true, arm_type,stn_name,stn_slot);
	_Set__DA_RT_OFFSET(stn_name,stn_slot);
	return r_flag;
}
int  CObj__VAC_ROBOT_STD::
Fnc__PLACE(CII_OBJECT__VARIABLE* p_variable,
		   CII_OBJECT__ALARM* p_alarm,
		   const CString& arm_type,
		   const CString& stn_name,
		   const CString& stn_slot,
		   const int handoff_mode)
{
	// ...
	{
		CString act_name;

		act_name.Format("place from Arm-%s to %s-%s", arm_type,stn_name,stn_slot);
		Fnc__APP_LOG(act_name);

		// Valve Check ...
		if(Interlock__CHECK_VALVE_OPEN(p_alarm, arm_type,stn_name,stn_slot, act_name) < 0)
		{
			return -11;
		}

		// Material Check ...
		if(Interlock__CHECK_MATERIAL(p_alarm, true, arm_type,stn_name,stn_slot, act_name) < 0)
		{
			return -12;
		}

		// LLx Slot Check ...
		if(Interlock__LLx_SLOT_CHECK(p_alarm, stn_name, stn_slot) < 0)
		{
			return -13;
		}
	}

	Set_ANI__ROBOT_EXTEND(arm_type,stn_name,stn_slot);

	// ...
	int r_flag = Fnc__ACTION(arm_type,stn_name,stn_slot, CMMD__PLACE);
	
	if(r_flag > 0)
	{
		bool active__err_check = false;

		if(arm_type.CompareNoCase(_ARM_A) == 0)			
		{
			if(dEXT_CH__ROBOT_ARM_A_MATERIAL_STATUS->Check__DATA(STR__NONE) < 0)		active__err_check = true;
		}
		else if(arm_type.CompareNoCase(_ARM_B) == 0)			
		{
			if(dEXT_CH__ROBOT_ARM_B_MATERIAL_STATUS->Check__DATA(STR__NONE) < 0)		active__err_check = true;
		}

		if(active__err_check)
		{
			int alm_id = ALID__PLACE__MATERIAL_ERROR;
			CString err_msg;
			CString err_bff;
			CString r_act;

			CString act_info;
			act_info.Format("%s -> %s(%s)", arm_type, stn_name,stn_slot);

			err_msg.Format("Place : %s \n", act_info);

			if(arm_type.CompareNoCase(_ARM_A) == 0)			
			{
				err_bff.Format(" * %s <- %s \n", 
								dEXT_CH__ROBOT_ARM_A_MATERIAL_STATUS->Get__CHANNEL_NAME(),
								dEXT_CH__ROBOT_ARM_A_MATERIAL_STATUS->Get__STRING());
				err_msg += err_bff;
			}
			else if(arm_type.CompareNoCase(_ARM_B) == 0)			
			{
				err_bff.Format(" * %s <- %s \n", 
								dEXT_CH__ROBOT_ARM_B_MATERIAL_STATUS->Get__CHANNEL_NAME(),
								dEXT_CH__ROBOT_ARM_B_MATERIAL_STATUS->Get__STRING());
				err_msg += err_bff;
			}

			p_alarm->Popup__ALARM_With_MESSAGE(alm_id, err_msg, r_act);
			return -11;
		}

		Fnc__CHANGE_MATERIAL_INFO(1,arm_type,stn_name,stn_slot);
		Set_ANI__ROBOT_RETRACT(arm_type,stn_name,stn_slot);
	}
	else
	{
		bool active__wfr_exchange = false;

		if(arm_type.CompareNoCase(_ARM_A) == 0)			
		{
			if(dEXT_CH__ROBOT_ARM_A_MATERIAL_STATUS->Check__DATA(STR__NONE) > 0)		active__wfr_exchange = true;
		}
		else if(arm_type.CompareNoCase(_ARM_B) == 0)			
		{
			if(dEXT_CH__ROBOT_ARM_B_MATERIAL_STATUS->Check__DATA(STR__NONE) > 0)		active__wfr_exchange = true;
		}

		if(active__wfr_exchange)
		{
			Fnc__CHANGE_MATERIAL_INFO(1,arm_type, stn_name,stn_slot);
		}
	}

	return r_flag;
}

// Swap -----
int  CObj__VAC_ROBOT_STD::
Call__SWAP(CII_OBJECT__VARIABLE* p_variable,
		   CII_OBJECT__ALARM* p_alarm,
		   const CString& arm_type,
		   const CString& stn_name,
		   const CString& stn_slot,
		   const int handoff_mode)
{

	return -1;
}


// Rotate -----
int  CObj__VAC_ROBOT_STD::
Call__ROTATE(CII_OBJECT__VARIABLE* p_variable,
			 CII_OBJECT__ALARM* p_alarm,
			 const CString& arm_type,
			 const CString& stn_name, 
			 const CString& stn_slot)
{
	Set_ANI__ROBOT_ROTATE(arm_type, stn_name,stn_slot);

	return Fnc__ACTION(arm_type,stn_name,stn_slot, CMMD__ROTATE);
}


// Home -----
int  CObj__VAC_ROBOT_STD::
Call__HOME(CII_OBJECT__VARIABLE* p_variable,
		   CII_OBJECT__ALARM* p_alarm)
{
	int flag = pROBOT__OBJ_CTRL->Call__OBJECT(CMMD__HOME);

	if(flag > 0)
	{
		Set_ANI__ROBOT_ALL_RETRACT();
	}
	return flag;
}


// Map -----
int  CObj__VAC_ROBOT_STD::
Call__MAP(CII_OBJECT__VARIABLE* p_variable,
		  CII_OBJECT__ALARM* p_alarm)
{
	return pROBOT__OBJ_CTRL->Call__OBJECT(CMMD__MAP);
}


// EXTEND -----
int  CObj__VAC_ROBOT_STD::
Call__EXTEND(CII_OBJECT__VARIABLE* p_variable,
			 CII_OBJECT__ALARM* p_alarm,
			 const CString& arm_type,
			 const CString& stn_name, 
			 const CString& stn_slot)
{
	bool actove__place_act = false;

	if(arm_type.CompareNoCase(_ARM_A) == 0)
	{
		if(dCH__OTR_OUT_MON__ARM_A_MATERIAL_STATUS->Check__DATA(STR__NONE) < 0)			actove__place_act = true;
	}
	else if(arm_type.CompareNoCase(_ARM_B) == 0)
	{
		if(dCH__OTR_OUT_MON__ARM_B_MATERIAL_STATUS->Check__DATA(STR__NONE) < 0)			actove__place_act = true;
	}

	_Active__DA_STS(actove__place_act, arm_type, stn_name,stn_slot, true);

	return Fnc__EXTEND(p_variable,p_alarm, arm_type,stn_name,stn_slot);
}
int  CObj__VAC_ROBOT_STD::
Fnc__EXTEND(CII_OBJECT__VARIABLE* p_variable,
			 CII_OBJECT__ALARM* p_alarm,
			 const CString& arm_type,
			 const CString& stn_name, 
			 const CString& stn_slot)
{
	// ...
	{
		CString act_name;

		act_name.Format("extend from Arm-%s to %s-%s", arm_type,stn_name,stn_slot);
		Fnc__APP_LOG(act_name);

		// Valve Check ...
		if(Interlock__CHECK_VALVE_OPEN(p_alarm, arm_type,stn_name,stn_slot, act_name) < 0)
		{
			return -1;
		}

		// Material Check ...
		bool active__arm_a = false;
		bool active__arm_b = false;

			 if(arm_type.CompareNoCase(_ARM_A)  == 0)			active__arm_a = true;
		else if(arm_type.CompareNoCase(_ARM_B)  == 0)			active__arm_b = true;
		else if(arm_type.CompareNoCase(_ARM_AB) == 0)
		{
			active__arm_a = true;
			active__arm_b = true;
		}

		// ...
		int place_flag = -1;

		if(active__arm_a)
		{
			if(dEXT_CH__ROBOT_ARM_A_MATERIAL_STATUS->Check__DATA(STR__NONE) < 0)
				place_flag = 1;
		}
		if(active__arm_b)
		{
			if(dEXT_CH__ROBOT_ARM_B_MATERIAL_STATUS->Check__DATA(STR__NONE) < 0)
				place_flag = 1;
		}

		if(place_flag > 0)
		{
			if(Interlock__CHECK_MATERIAL(p_alarm, true, arm_type,stn_name,stn_slot, act_name) < 0)
			{
				return -1;
			}
		}
	}

	// ...
	int flag;

	Set_ANI__ROBOT_ROTATE(arm_type, stn_name,stn_slot);

	// ...
	{
		CStringArray l_para;

		l_para.Add(arm_type);
		l_para.Add(stn_name);
		l_para.Add(stn_slot);

		flag = pROBOT__OBJ_CTRL->Call__OBJ_MODE(CMMD__EXTEND,l_para);
	}

	if(flag > 0)
	{
		Set_ANI__ROBOT_EXTEND(arm_type,stn_name,stn_slot);
	}
	return flag;
}

// RETRACT -----
int  CObj__VAC_ROBOT_STD::
Call__RETRACT(CII_OBJECT__VARIABLE* p_variable,
			  CII_OBJECT__ALARM* p_alarm,
			  const CString& arm_type,
			  const CString& stn_name, 
			  const CString& stn_slot)
{
	bool active__arm_a = false;
	bool active__arm_b = false;

		 if(arm_type.CompareNoCase(_ARM_A)  == 0)		active__arm_a = true;
	else if(arm_type.CompareNoCase(_ARM_B)  == 0)		active__arm_b = true;
	else if(arm_type.CompareNoCase(_ARM_AB) == 0)
	{
		active__arm_a = true;
		active__arm_b = true;
	}

	// ...
	bool active_place = false;

	if(active__arm_a)
	{
		if(dCH__OTR_OUT_MON__ARM_A_MATERIAL_STATUS->Check__DATA(STR__NONE) > 0)
			active_place = true;
	}
	if(active__arm_b)
	{
		if(dCH__OTR_OUT_MON__ARM_B_MATERIAL_STATUS->Check__DATA(STR__NONE) > 0)
			active_place = true;
	}

	// ...
	int r_flag = Fnc__RETRACT(p_variable,p_alarm, arm_type,stn_name,stn_slot);
	if(r_flag > 0)
	{
		r_flag = _Report__DA_OFFSET(p_alarm, arm_type,stn_name,stn_slot, active_place);
		_Save__DA_OFFSET(arm_type,stn_name,stn_slot, active_place);
	}

	_Clear__DA_STS(active_place, arm_type,stn_name,stn_slot);
	_Set__DA_RT_OFFSET(stn_name,stn_slot);

	return r_flag;

}
int  CObj__VAC_ROBOT_STD::
Fnc__RETRACT(CII_OBJECT__VARIABLE* p_variable,
			 CII_OBJECT__ALARM* p_alarm,
			 const CString& arm_type,
			 const CString& stn_name, 
			 const CString& stn_slot)
{
	CString act_name;

	act_name.Format("Retract from Arm-%s to %s-%s", arm_type,stn_name,stn_slot);
	Fnc__APP_LOG(act_name);

	// ...
	int flag;

	// ...
	{
		CStringArray l_para;

		l_para.Add(arm_type);
		l_para.Add(stn_name);
		l_para.Add(stn_slot);

		flag = pROBOT__OBJ_CTRL->Call__OBJ_MODE(CMMD__RETRACT,l_para);
	}

	if(flag > 0)
	{
		Set_ANI__ROBOT_RETRACT(arm_type, stn_name, stn_slot);
	}
	return flag;
}


// GOUP -----
int  CObj__VAC_ROBOT_STD::
Call__GOUP(CII_OBJECT__VARIABLE* p_variable,
		   CII_OBJECT__ALARM* p_alarm,
		   const CString& arm_type,
		   const CString& stn_name, 
		   const CString& stn_slot)
{
	CString act_name;

	act_name.Format("GoUp from Arm-%s to %s-%s", arm_type,stn_name,stn_slot);
	Fnc__APP_LOG(act_name);

	// ...
	int flag;

	// ...
	{
		CStringArray l_para;

		l_para.Add(arm_type);
		l_para.Add(stn_name);
		l_para.Add(stn_slot);

		flag = pROBOT__OBJ_CTRL->Call__OBJ_MODE(CMMD__GOUP,l_para);
	}
	return flag;
}

// GODOWN -----
int  CObj__VAC_ROBOT_STD::
Call__GODOWN(CII_OBJECT__VARIABLE* p_variable,
			 CII_OBJECT__ALARM* p_alarm,
			 const CString& arm_type,
			 const CString& stn_name, 
			 const CString& stn_slot)
{
	CString act_name;

	act_name.Format("GoDown from Arm-%s to %s-%s", arm_type,stn_name,stn_slot);
	Fnc__APP_LOG(act_name);

	// ...
	int flag;

	// ...
	{
		CStringArray l_para;

		l_para.Add(arm_type);
		l_para.Add(stn_name);
		l_para.Add(stn_slot);

		flag = pROBOT__OBJ_CTRL->Call__OBJ_MODE(CMMD__GODOWN,l_para);
	}
	return flag;
}

// -----
int  CObj__VAC_ROBOT_STD::
Call__RQ_STN(CII_OBJECT__VARIABLE* p_variable,
			 CII_OBJECT__ALARM* p_alarm,
			 const CString& arm_type,
			 const CString& stn_name)
{
	CString act_name;

	act_name.Format("RQ STN from Arm-%s to %s", arm_type,stn_name);
	Fnc__APP_LOG(act_name);

	// ...
	int flag;

	// ...
	{
		CStringArray l_para;

		l_para.Add(arm_type);
		l_para.Add(stn_name);

		flag = pROBOT__OBJ_CTRL->Call__OBJ_MODE("RQ_STN",l_para);
	}
	return flag;
}

// -----
int  CObj__VAC_ROBOT_STD::
Call__SET_STN(CII_OBJECT__VARIABLE* p_variable,
			  CII_OBJECT__ALARM* p_alarm,
			  const CString& arm_type,
			  const CString& stn_name)
{
	CString act_name;

	act_name.Format("SET STN from Arm-%s to %s", arm_type,stn_name);
	Fnc__APP_LOG(act_name);

	// ...
	int flag;

	// ...
	{
		CStringArray l_para;

		l_para.Add(arm_type);
		l_para.Add(stn_name);

		flag = pROBOT__OBJ_CTRL->Call__OBJ_MODE("SET_STN",l_para);
	}
	return flag;
}

// -----
int  CObj__VAC_ROBOT_STD::
Call__TEACHED_CPTR_SAVE(CII_OBJECT__VARIABLE* p_variable,
						CII_OBJECT__ALARM* p_alarm,
						const CString& arm_type,
						const CString& stn_name)
{
	CString act_name;

	act_name.Format("TEACHED SNS SAVE from Arm-%s to %s", arm_type,stn_name);
	Fnc__APP_LOG(act_name);

	// ...
	int flag;

	// ...
	{
		CStringArray l_para;

		l_para.Add(arm_type);
		l_para.Add(stn_name);

		flag = pROBOT__OBJ_CTRL->Call__OBJ_MODE("TEACHED_CPTR_SAVE",l_para);
	}
	return flag;
}

int  CObj__VAC_ROBOT_STD::
Call__TIME_TEST(CII_OBJECT__VARIABLE* p_variable,
				CII_OBJECT__ALARM* p_alarm)
{
	int n_limit = 2000;
	int n;

	CString stn_name;
	int db_index;
	int i;
	int k;

	for(n=0; n<n_limit; n++)
	{
		stn_name.Format("LL1");

		double cur_sec = 5.0;
		if(n >= 1000)		cur_sec = 10.0;

		for(i=0; i<CFG_ACT__SIZE; i++)
		{
				 if(i == 0)			db_index = _ACT_INDEX__PICK;
			else if(i == 1)			db_index = _ACT_INDEX__PLACE;
			else if(i == 2)			db_index = _ACT_INDEX__ROTATE;
			else					continue;

			// ...
			{	
				CII__VAR_STRING_CTRL* p_ch_min;
				CII__VAR_STRING_CTRL* p_ch_max;
				CII__VAR_STRING_CTRL* p_ch_avg;
				CII__VAR_STRING_CTRL* p_ch_avg_f;
				CII__VAR_ANALOG_CTRL* p_ch_cnt;
				
				CString str_cur;
				str_cur.Format("%.1f", cur_sec);

				// Robot ...
				{
					sCH__TAS_ACTION_TIME_NOW__RB[db_index]->Set__DATA(str_cur);

					//
					p_ch_min = sCH__TAS_ACTION_TIME_MIN__RB[db_index].Get__PTR();
					p_ch_max = sCH__TAS_ACTION_TIME_MAX__RB[db_index].Get__PTR();
					p_ch_avg = sCH__TAS_ACTION_TIME_AVG__RB[db_index].Get__PTR();
					p_ch_avg_f = sCH__TAS_ACTION_TIME_AVG_F__RB[db_index].Get__PTR();
					p_ch_cnt = aCH__TAS_ACTION_TIME_CNT__RB[db_index].Get__PTR();

					_Update__ACTION_MIN_MAX(p_ch_min,p_ch_max, p_ch_avg,p_ch_avg_f,p_ch_cnt, cur_sec);
				}
				// LLx ...
				{
					int db_i = Macro__CHECK_LLx_INDEX(stn_name);
					if(db_i >= 0)
					{
						if(bActive__LLx_MULTI_SLOT_VALVE)
						{
							for(int t=0; t<CFG_LLx__SLOT_SIZE; t++)
							{
								sCH__TAS_ACTION_TIME_NOW__LLx_SLOT[db_index][db_i][t]->Set__DATA(str_cur);

								//
								p_ch_min = sCH__TAS_ACTION_TIME_MIN__LLx_SLOT[db_index][db_i][t].Get__PTR();
								p_ch_max = sCH__TAS_ACTION_TIME_MAX__LLx_SLOT[db_index][db_i][t].Get__PTR();
								p_ch_avg = sCH__TAS_ACTION_TIME_AVG__LLx_SLOT[db_index][db_i][t].Get__PTR();
								p_ch_avg_f = sCH__TAS_ACTION_TIME_AVG_F__LLx_SLOT[db_index][db_i][t].Get__PTR();
								p_ch_cnt = aCH__TAS_ACTION_TIME_CNT__LLx_SLOT[db_index][db_i][t].Get__PTR();

								_Update__ACTION_MIN_MAX(p_ch_min,p_ch_max, p_ch_avg,p_ch_avg_f,p_ch_cnt, cur_sec);
							}
						}
						else
						{
							sCH__TAS_ACTION_TIME_NOW__LLx_X[db_index][db_i]->Set__DATA(str_cur);

							//
							p_ch_min = sCH__TAS_ACTION_TIME_MIN__LLx_X[db_index][db_i].Get__PTR();
							p_ch_max = sCH__TAS_ACTION_TIME_MAX__LLx_X[db_index][db_i].Get__PTR();
							p_ch_avg = sCH__TAS_ACTION_TIME_AVG__LLx_X[db_index][db_i].Get__PTR();
							p_ch_avg_f = sCH__TAS_ACTION_TIME_AVG_F__LLx_X[db_index][db_i].Get__PTR();
							p_ch_cnt = aCH__TAS_ACTION_TIME_CNT__LLx_X[db_index][db_i].Get__PTR();

							_Update__ACTION_MIN_MAX(p_ch_min,p_ch_max, p_ch_avg,p_ch_avg_f,p_ch_cnt, cur_sec);
						}
					}
				}
				// PMx ...
				{
					int db_i = Macro__CHECK_PMx_INDEX(stn_name);
					if(db_i >= 0)
					{
						sCH__TAS_ACTION_TIME_NOW__PMx[db_index][db_i]->Set__DATA(str_cur);

						//
						p_ch_min = sCH__TAS_ACTION_TIME_MIN__PMx[db_index][db_i].Get__PTR();
						p_ch_max = sCH__TAS_ACTION_TIME_MAX__PMx[db_index][db_i].Get__PTR();
						p_ch_avg = sCH__TAS_ACTION_TIME_AVG__PMx[db_index][db_i].Get__PTR();
						p_ch_avg_f = sCH__TAS_ACTION_TIME_AVG_F__PMx[db_index][db_i].Get__PTR();
						p_ch_cnt = aCH__TAS_ACTION_TIME_CNT__PMx[db_index][db_i].Get__PTR();

						_Update__ACTION_MIN_MAX(p_ch_min,p_ch_max, p_ch_avg,p_ch_avg_f,p_ch_cnt, cur_sec);
					}
				}
			}
		}

		// ...
	}

	return 1;
}


// ...
#define  _DEF__PI						3.141592


int  CObj__VAC_ROBOT_STD::
Call__TEST_RT_TO_XY(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm)
{
	CString ch_data;

	ch_data = sCH__TEST_R_OFFSET_MM->Get__STRING();
	double para_mm = atof(ch_data);

	ch_data = sCH__TEST_T_OFFSET_DEG->Get__STRING();
	double para_deg = atof(ch_data);

	double theta_deg = para_deg * _DEF__PI / 180.0;
	double x_pos = para_mm * cos(theta_deg);
	double y_pos = para_mm * sin(theta_deg);

	ch_data.Format("%.3f", x_pos);	
	sCH__TEST_X_OFFSET_MM->Set__DATA(ch_data);

	ch_data.Format("%.3f", y_pos);	
	sCH__TEST_Y_OFFSET_MM->Set__DATA(ch_data);

	// ...
	CString log_msg;
	CString log_bff;

	log_msg = "Call__TEST_RT_TO_XY() ... \n";

	log_bff.Format(" (%s, %s) -> (%s, %s) \n",
				   sCH__TEST_R_OFFSET_MM->Get__STRING(),
				   sCH__TEST_T_OFFSET_DEG->Get__STRING(),
				   sCH__TEST_X_OFFSET_MM->Get__STRING(),
				   sCH__TEST_Y_OFFSET_MM->Get__STRING());
	log_msg += log_bff;
	log_msg += "\n";

	printf(log_msg);
	return 1;
}
int  CObj__VAC_ROBOT_STD::
Call__TEST_XY_TO_RT(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm)
{
	CString ch_data;

	ch_data = sCH__TEST_X_OFFSET_MM->Get__STRING();
	double x_mm = atof(ch_data);

	ch_data = sCH__TEST_Y_OFFSET_MM->Get__STRING();
	double y_mm = atof(ch_data);

	double r_mm  = sqrt(pow(x_mm,2)+pow(y_mm,2));
	double t_deg = atan2(y_mm,x_mm) * 180 / _DEF__PI;

	ch_data.Format("%.3f", r_mm);	
	sCH__TEST_R_OFFSET_MM->Set__DATA(ch_data);

	ch_data.Format("%.3f", t_deg);	
	sCH__TEST_T_OFFSET_DEG->Set__DATA(ch_data);

	// ...
	CString log_msg;
	CString log_bff;

	log_msg = "Call__TEST_XY_TO_RT() ... \n";

	log_bff.Format(" (%s, %s) -> (%s, %s) \n",
					sCH__TEST_X_OFFSET_MM->Get__STRING(),
					sCH__TEST_Y_OFFSET_MM->Get__STRING(),
					sCH__TEST_R_OFFSET_MM->Get__STRING(),
					sCH__TEST_T_OFFSET_DEG->Get__STRING());
	log_msg += log_bff;
	log_msg += "\n";

	printf(log_msg);
	return 1;
}
