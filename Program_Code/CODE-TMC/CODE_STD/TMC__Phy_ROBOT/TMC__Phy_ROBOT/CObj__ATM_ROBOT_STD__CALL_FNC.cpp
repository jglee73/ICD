#include "StdAfx.h"
#include "CObj__ATM_ROBOT_STD.h"
#include "CObj__ATM_ROBOT_STD__DEF.h"
#include "CObj__ATM_ROBOT_STD__ALID.h"

#include "Macro_Function.h"


//-------------------------------------------------------------------------	
// INIT -----
int  CObj__ATM_ROBOT_STD
::Call__INIT(CII_OBJECT__VARIABLE* p_variable,
			 CII_OBJECT__ALARM* p_alarm)
{
	int flag = pROBOT__OBJ_CTRL->Call__OBJECT(CMMD__INIT);

	if(flag > 0)
	{
		Set_ANI__ROBOT_ALL_RETRACT();
	}
	return flag;
}


// ...
int  CObj__ATM_ROBOT_STD
::Interlock__AL1_SLOT_CHECK(const CString& stn_name,
							const CString& para_slot,
							CString& trg_slot)
{
	if(stn_name.CompareNoCase(STR__AL1) != 0)
	{
		return 1;
	}

	// ...
	CString ch_data = dEXT_CH__CFG_AL1_SLOT_MAX->Get__STRING();
	int cfg_slot = atoi(ch_data);

	if(cfg_slot < 2)
	{
		trg_slot = "1";
		return 1;
	}

	// ...
	{
		int i_slot = atoi(para_slot);

		if(i_slot > cfg_slot)
		{
			CString log_msg;
			CString log_bff;

			log_msg = " * Error : Aligner Slot_MAX ... \n";
			log_bff.Format(" * para_slot : %1d \n", i_slot);
			log_msg += log_bff;
			log_bff.Format(" * cfg_slot : %1d \n", cfg_slot);
			log_msg += log_bff;

			Fnc__APP_LOG(log_msg);
			return -1;
		}
	}

	trg_slot = para_slot;
	return 1;
}
int  CObj__ATM_ROBOT_STD
::Interlock__LLx_SLOT_CHECK(const CString& stn_name,
							const CString& para_slot,
							CString& trg_slot)
{
	int ll_i = Macro__CHECK_LLx_INDEX(stn_name);
	if(ll_i < 0)
	{
		return 1;
	}

	// ...
	CString ch_data = "";
	
	if(ll_i >= 0)
	{
		ch_data = dEXT_CH__CFG_LLx_SLOT_MAX[ll_i]->Get__STRING();
	}
	else
	{
		return -1;
	}

	int cfg_slot = atoi(ch_data);
	if(cfg_slot < 2)
	{
		trg_slot = "1";
		return 1;
	}

	// ...
	{
		int i_slot = atoi(para_slot);

		if(i_slot > cfg_slot)
		{
			CString log_msg;
			CString log_bff;

			log_msg.Format(" * Error : %s's Slot_MAX ... \n", stn_name);
			log_bff.Format(" * para_slot : %1d \n", i_slot);
			log_msg += log_bff;
			log_bff.Format(" * cfg_slot : %1d \n", cfg_slot);
			log_msg += log_bff;

			Fnc__APP_LOG(log_msg);
			return -1;
		}
	}

	trg_slot = para_slot;
	return 1;
}

// PICK -----
int  CObj__ATM_ROBOT_STD
::Call__PICK(CII_OBJECT__VARIABLE* p_variable,
		     CII_OBJECT__ALARM* p_alarm,
		     const CString& arm_type,
		     const CString& stn_name,
		     const CString& para_slot)
{
	CString act_name;
	CString stn_slot = para_slot;

	// Slot Check ...
	{
		if(Interlock__AL1_SLOT_CHECK(stn_name, para_slot, stn_slot) < 0)
		{
			return -11;
		}
		if(Interlock__LLx_SLOT_CHECK(stn_name, para_slot, stn_slot) < 0)
		{
			return -12;
		}
	}

	// ...
	{
		act_name.Format("pick from %s-%s to Arm-%s", stn_name,stn_slot,arm_type);
		Fnc__APP_LOG(act_name);

		// Door Check -----
		if(Interlock__CHECK_DOOR_OPEN(p_alarm,stn_name,act_name) < 0)
		{
			Fnc__APP_LOG("Interlock__CHECK_DOOR_OPEN.. ret:-1");
			return -1;
		}

		// Material Check -----
		if(dCH__CFG__WAFER_PRESENT_CHECK->Check__DATA("YES") > 0)	
		{
			if(Interlock__CHECK_MATERIAL(p_alarm,-1,arm_type,stn_name,stn_slot,act_name) < 0)
			{
				Fnc__APP_LOG("Interlock__CHECK_MATERIAL.. ret:-1");
				return -1;
			}
		}
	}

	Set_ANI__ROBOT_ROTATE(arm_type,stn_name);
	Set_ANI__ROBOT_EXTEND(arm_type,stn_name,stn_slot);

	// ...
	int flag;

	// ...
	{
		CStringArray l_para;

		l_para.Add(arm_type);
		l_para.Add(stn_name);
		l_para.Add(stn_slot);

		act_name.Format("Call Lower Object.. pick from %s-%s to Arm-%s", stn_name,stn_slot,arm_type);
		Fnc__APP_LOG(act_name);

		flag = pROBOT__OBJ_CTRL->Call__OBJ_MODE(CMMD__PICK,l_para);
	}

	if(flag > 0)
	{
		Fnc__CHANGE_MATERIAL_INFO(-1,arm_type,stn_name,stn_slot);
		Set_ANI__ROBOT_RETRACT(arm_type,stn_name,stn_slot);
	}

	act_name.Format("End..Call__PICK.. ret:%d", flag);
	Fnc__APP_LOG(act_name);

	return flag;
}


// PLACE -----
int  CObj__ATM_ROBOT_STD
::Call__PLACE(CII_OBJECT__VARIABLE* p_variable,
			  CII_OBJECT__ALARM* p_alarm,
			  const CString& arm_type,
			  const CString& stn_name,
			  const CString& para_slot)
{
	CString act_name;
	CString stn_slot = para_slot;

	// Slot Check ...
	{
		if(Interlock__AL1_SLOT_CHECK(stn_name, para_slot, stn_slot) < 0)
		{
			return -11;
		}
		if(Interlock__LLx_SLOT_CHECK(stn_name, para_slot, stn_slot) < 0)
		{
			return -12;
		}
	}

	// ...
	{
		act_name.Format("place from Arm-%s to %s-%s", arm_type,stn_name,stn_slot);
		Fnc__APP_LOG(act_name);

		// Door Check -----
		if(Interlock__CHECK_DOOR_OPEN(p_alarm,stn_name,act_name) < 0)
		{
			Fnc__APP_LOG("Interlock__CHECK_DOOR_OPEN.. ret:-1");
			return -1;
		}

		// Material Check -----
		if(Interlock__CHECK_MATERIAL(p_alarm,1,arm_type,stn_name,stn_slot,act_name) < 0)
		{
			Fnc__APP_LOG("Interlock__CHECK_MATERIAL.. ret:-1");
			return -1;
		}
	}

	Set_ANI__ROBOT_ROTATE(arm_type,stn_name);
	Set_ANI__ROBOT_EXTEND(arm_type,stn_name,stn_slot);

	// ...
	int flag;

	// ...
	{
		CStringArray l_para;

		l_para.Add(arm_type);
		l_para.Add(stn_name);
		l_para.Add(stn_slot);

		act_name.Format("Call Lower Object.. place from %s-%s to Arm-%s", stn_name,stn_slot,arm_type);
		Fnc__APP_LOG(act_name);

		flag = pROBOT__OBJ_CTRL->Call__OBJ_MODE(CMMD__PLACE,l_para);
	}

	if(flag > 0)
	{
		Fnc__CHANGE_MATERIAL_INFO(1,arm_type,stn_name,stn_slot);
		Set_ANI__ROBOT_RETRACT(arm_type,stn_name,stn_slot);
	}

	act_name.Format("End..Call__PLACE.. ret:%d", flag);
	Fnc__APP_LOG(act_name);
	return flag;
}


// ROTATE -----
int  CObj__ATM_ROBOT_STD
::Call__ROTATE(CII_OBJECT__VARIABLE* p_variable,
			   CII_OBJECT__ALARM* p_alarm,
			   const CString& arm_type,
			   const CString& stn_name,
			   const CString& stn_slot)
{
	Set_ANI__ROBOT_ROTATE(arm_type,stn_name);

	// ...
	int flag;

	// ...
	{
		CStringArray l_para;

		l_para.Add(arm_type);
		l_para.Add(stn_name);
		l_para.Add(stn_slot);

		flag = pROBOT__OBJ_CTRL->Call__OBJ_MODE(CMMD__ROTATE,l_para);
	}
	return flag;
}


// MAP -----
int  CObj__ATM_ROBOT_STD::
Call__MAP(CII_OBJECT__VARIABLE* p_variable,
		  CII_OBJECT__ALARM* p_alarm)
{
	return pROBOT__OBJ_CTRL->Call__OBJECT(CMMD__MAP);
}

// HOME -----
int  CObj__ATM_ROBOT_STD::
Call__HOME(CII_OBJECT__VARIABLE* p_variable,
		   CII_OBJECT__ALARM* p_alarm,
		   const int req__vac_off)
{
	int flag = -1;

	if(req__vac_off > 0)
	{
		flag = pROBOT__OBJ_CTRL->Call__OBJECT("VAC_OFF.HOME");
	}
	else
	{
		flag = pROBOT__OBJ_CTRL->Call__OBJECT(CMMD__HOME);
	}

	if(flag > 0)
	{
		Set_ANI__ROBOT_ALL_RETRACT();
	}
	return flag;
}

// VACON -----
int  CObj__ATM_ROBOT_STD::
Call__VACON(CII_OBJECT__VARIABLE* p_variable,
			CII_OBJECT__ALARM* p_alarm,
			const CString& arm_type)
{
	int flag;

	// ...
	{
		CStringArray l_para;

		l_para.Add(arm_type);

		flag = pROBOT__OBJ_CTRL->Call__OBJ_MODE(CMMD__VACON,l_para);
	}

	return flag;
}

// VACOFF -----
int  CObj__ATM_ROBOT_STD::
Call__VACOFF(CII_OBJECT__VARIABLE* p_variable,
			 CII_OBJECT__ALARM* p_alarm,
			 const CString& arm_type)
{
	int flag;

	// ...
	{
		CStringArray l_para;

		l_para.Add(arm_type);

		flag = pROBOT__OBJ_CTRL->Call__OBJ_MODE(CMMD__VACOFF,l_para);
	}

	return flag;
}

// RETRACT -----
int  CObj__ATM_ROBOT_STD::
Call__RETRACT(CII_OBJECT__VARIABLE* p_variable,
			  CII_OBJECT__ALARM* p_alarm,
			  const CString& arm_type,
			  const CString& stn_name,
			  const CString& stn_slot)
{
	CString act_name;

	act_name.Format("retract from Arm-%s to %s-%s", arm_type,stn_name,stn_slot);
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

// EXTEND -----
int  CObj__ATM_ROBOT_STD
::Call__EXTEND(CII_OBJECT__VARIABLE* p_variable,
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

		// Door Check -----
		if(Interlock__CHECK_DOOR_OPEN(p_alarm,stn_name,act_name) < 0)
		{
			return -1;
		}

		// Material Check -----
		int place_flag = -1;

		if(arm_type.CompareNoCase(ARM_A) == 0)
		{
			if(dEXT_CH__ROBOT_ARM_A_MATERIAL_STATUS->Check__DATA(STR__NONE) < 0)
			{
				place_flag = 1;
			}
		}
		else if(arm_type.CompareNoCase(ARM_B) == 0)
		{
			if(dEXT_CH__ROBOT_ARM_B_MATERIAL_STATUS->Check__DATA(STR__NONE) < 0)
			{
				place_flag = 1;
			}
		}

		if(place_flag > 0)
		{
			if(Interlock__CHECK_MATERIAL(p_alarm,1,arm_type,stn_name,stn_slot,act_name) < 0)
			{
				return -1;
			}
		}
	}

	Set_ANI__ROBOT_ROTATE(arm_type,stn_name);

	// ...
	int flag;

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
		Set_ANI__ROBOT_EXTEND(arm_type, stn_name, stn_slot);
	}
	return flag;
}

// GOUP -----
int  CObj__ATM_ROBOT_STD
::Call__GOUP(CII_OBJECT__VARIABLE* p_variable,
		     CII_OBJECT__ALARM* p_alarm,
		     const CString& arm_type,
		     const CString& stn_name,
		     const CString& stn_slot)
{
	CString act_name;

	act_name.Format("GoUp from Arm-%s to %s-%s", arm_type,stn_name,stn_slot);
	Fnc__APP_LOG(act_name);

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
int  CObj__ATM_ROBOT_STD
::Call__GODOWN(CII_OBJECT__VARIABLE* p_variable,
			   CII_OBJECT__ALARM* p_alarm,
			   const CString& arm_type,
			   const CString& stn_name,
			   const CString& stn_slot)
{
	CString act_name;

	act_name.Format("GoDown from Arm-%s to %s-%s", arm_type,stn_name,stn_slot);
	Fnc__APP_LOG(act_name);

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

// ALGN -----
int  CObj__ATM_ROBOT_STD
::Call__PLACE_ALIGN_PICK(CII_OBJECT__VARIABLE* p_variable,
					     CII_OBJECT__ALARM* p_alarm,
					     const CString& arm_type,
					     const CString& stn_name,
					     const CString& stn_slot)
{
	CString act_name;

	act_name.Format("Place.. Align.. Pick from Arm-%s to %s-%s", arm_type,stn_name,stn_slot);
	Fnc__APP_LOG(act_name);

	// ...
	int flag;

	// ...
	{
		CStringArray l_para;

		l_para.Add(arm_type);
		l_para.Add(stn_name);
		l_para.Add(stn_slot);

		flag = pROBOT__OBJ_CTRL->Call__OBJ_MODE(CMMD__PLACE_ALGN_PICK,l_para);
	}
	return flag;
}

int  CObj__ATM_ROBOT_STD
::Call__ALGN(CII_OBJECT__VARIABLE* p_variable,
		     CII_OBJECT__ALARM* p_alarm)
{
	return pROBOT__OBJ_CTRL->Call__OBJECT(CMMD__ALGN);
}

// ALGNPICK -----
int  CObj__ATM_ROBOT_STD
::Call__ALGNPICK(CII_OBJECT__VARIABLE* p_variable,
			     CII_OBJECT__ALARM* p_alarm,
			     const CString& arm_type,
			     const CString& stn_name,
			     const CString& stn_slot)
{
	//  ...
	{
		CString act_name;

		act_name.Format("align_pick from %s-%s to Arm-%s", stn_name,stn_slot,arm_type);
		Fnc__APP_LOG(act_name);

		// Door Check -----
		if(Interlock__CHECK_DOOR_OPEN(p_alarm,stn_name,act_name) < 0)
		{
			return -1;
		}

		// Material Check -----
		if(Interlock__CHECK_MATERIAL(p_alarm,-1,arm_type,stn_name,stn_slot,act_name) < 0)
		{
			return -1;
		}
	}

	Set_ANI__ROBOT_ROTATE(arm_type,stn_name);
	Set_ANI__ROBOT_EXTEND(arm_type,stn_name,stn_slot);

	// ...
	int flag;

	// ...
	{
		CStringArray l_para;

		l_para.Add(arm_type);
		l_para.Add(stn_name);
		l_para.Add(stn_slot);

		flag = pROBOT__OBJ_CTRL->Call__OBJ_MODE(CMMD__ALGNPICK,l_para);
	}

	if(flag > 0)
	{
		Fnc__CHANGE_MATERIAL_INFO(-1,arm_type,stn_name,stn_slot);
	}

	Set_ANI__ROBOT_RETRACT(arm_type,stn_name,stn_slot);
	return flag;
}

// ALGNPLACE -----
int  CObj__ATM_ROBOT_STD
::Call__ALGNPLACE(CII_OBJECT__VARIABLE* p_variable,
				  CII_OBJECT__ALARM* p_alarm,
				  const CString& arm_type,
				  const CString& stn_name,
				  const CString& stn_slot)
{
	// ...
	{
		CString act_name;

		act_name.Format("align_place from Arm-%s to %s-%s", arm_type,stn_name,stn_slot);
		Fnc__APP_LOG(act_name);

		// Door Check -----
		if(Interlock__CHECK_DOOR_OPEN(p_alarm,stn_name,act_name) < 0)
		{
			return -1;
		}

		// Material Check -----
		if(Interlock__CHECK_MATERIAL(p_alarm,1,arm_type,stn_name,stn_slot,act_name) < 0)
		{
			return -1;
		}
	}

	Set_ANI__ROBOT_ROTATE(arm_type,stn_name);
	Set_ANI__ROBOT_EXTEND(arm_type,stn_name,stn_slot);

	// ...
	int flag;

	// ...
	{
		CStringArray l_para;

		l_para.Add(arm_type);
		l_para.Add(stn_name);
		l_para.Add(stn_slot);

		flag = pROBOT__OBJ_CTRL->Call__OBJ_MODE(CMMD__ALGNPLACE,l_para);
	}

	if(flag > 0)
	{
		Fnc__CHANGE_MATERIAL_INFO(1,arm_type,stn_name,stn_slot);
	}

	Set_ANI__ROBOT_RETRACT(arm_type,stn_name,stn_slot);
	return flag;
}

// LP_MAP -----
int  CObj__ATM_ROBOT_STD
::Call__LP_MAP(CII_OBJECT__VARIABLE* p_variable,
			   CII_OBJECT__ALARM* p_alarm,
			   const CString& arm_type,
			   const CString& stn_name,
			   const CString& stn_slot)
{
	CString act_name;
	CString var_data;
	CString recovery_act;

	act_name.Format("LpMap to %s", stn_name);
	Fnc__APP_LOG(act_name);

Chk_Retry:

	// LP Side Sensor Check ...
	if(dCH__CFG_MAPPING_ACTION_SLIDE_OUT_CHECK->Check__DATA("YES") > 0)
	{
		if(Interlock__CHECK_WAFER_LP_SLIDE(p_alarm,stn_name,act_name) < 0)
		{
			return -1;
		}
	}

	// ...
	if(arm_type.CompareNoCase("B") == 0)		dEXT_CH__ROBOT_ARM_B_MATERIAL_STATUS->Get__DATA(var_data);
	else										dEXT_CH__ROBOT_ARM_A_MATERIAL_STATUS->Get__DATA(var_data);

	if(var_data.CompareNoCase("NONE") != 0)
	{
		act_name.Format("Arm Wafer %s Sts", var_data);
		Fnc__APP_LOG(act_name);

		// ...
		{
			CString alarm_msg;

			// Alarm Occur !!
			alarm_msg.Format("Before Map On (%s) Arm %s Wafer %s Status\n",stn_name, arm_type, var_data);

			p_alarm->Check__ALARM(ALID__INTLK__LPx_CANNOT_MAP_WAFER_EXIST_ON_ARM, recovery_act);
			p_alarm->Popup__ALARM_With_MESSAGE(ALID__INTLK__LPx_CANNOT_MAP_WAFER_EXIST_ON_ARM, alarm_msg, recovery_act);

				 if(recovery_act.CompareNoCase("RETRY") == 0)		goto Chk_Retry;
			else if(recovery_act.CompareNoCase("ABORT") == 0)		return OBJ_ABORT;
			else													return OBJ_AVAILABLE;
		}
	}

	// ...
	int flag;

	// ...
	{
		CStringArray l_para;

		l_para.Add(arm_type);
		l_para.Add(stn_name);
		l_para.Add(stn_slot);

		flag = pROBOT__OBJ_CTRL->Call__OBJ_MODE(CMMD__LP_MAP,l_para);
	}

	int lp_index = Macro__CHECK_LPx_INDEX(stn_name);
	if(flag > 0)
	{
		Update__MAP_INFO(p_variable, p_alarm, stn_name);
	}
	return flag;
}
