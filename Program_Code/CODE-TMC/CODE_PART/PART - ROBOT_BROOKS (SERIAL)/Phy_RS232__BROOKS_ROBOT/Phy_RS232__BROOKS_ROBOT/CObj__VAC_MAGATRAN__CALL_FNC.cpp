#include "StdAfx.h"
#include "CObj__VAC_MAGATRAN.h"
#include "CObj__VAC_MAGATRAN__DEF.h"
#include "CObj__VAC_MAGATRAN__ALID.h"

#include "CCommon_DEF.h"
#include "Macro_Function.h"
#include "math.h"


//-------------------------------------------------------------------------	
// Init -----
int  CObj__VAC_MAGATRAN
::Call__INIT(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	if(iActive_SIM > 0)
	{
		return Fnc__SIM_INIT(p_variable,p_alarm);
	}

	return Fnc__INIT(p_variable,p_alarm);
}

int  CObj__VAC_MAGATRAN
::Fnc__SIM_INIT(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	if(Is__OFFLINE(p_alarm) < 0)
	{
		return -1;
	}

	return 1;
}
int  CObj__VAC_MAGATRAN
::Fnc__INIT(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
LOOP_RETRY:

	// ...
	int state;
	Fnc__APP_LOG("[INIT] Mode --> ComInit Start...");

	if(Is__LOCAL(p_alarm) < 0)
	{
		return -1;
	}

	// after Robot Module doesn't Recieved data, IS__OFFLINE popup
	if(diCH__COMM_STS->Check__DATA(STR__OFFLINE) > 0)
	{
		int nCount =0;
		
		do
		{
			if(p_variable->Check__CTRL_ABORT() > 0)
			{
				return -1;
			}

			state = doCH__COMMAND->Set__DATA("ComInit");
			nCount++;

			if(nCount > 3)
			{
				break;
			}
		}
		while(state);
	}

	if(Is__OFFLINE(p_alarm) < 0)
	{
		return -1;
	}

	if(dCH__CFG_WFR_SNS_CHECK->Check__DATA("TRUE") > 0)
	{
		doCH__ARM_WFR_SNS->Set__DATA("Yes");
		state = doCH__COMMAND->Set__DATA("WfrSnsIntk");
	}
	else
	{
		doCH__ARM_WFR_SNS->Set__DATA("No");
		state = doCH__COMMAND->Set__DATA("WfrSnsIntk");
	}

	if(state == 0)    // Abort
	{
		return -1;
	}

	if(Is__OFFLINE(p_alarm) < 0)
	{
		return -1;
	}

	// ...
	{
		state = Is__ERROR_EXIST(p_alarm);

		if(state != OBJ_AVAILABLE)
		{
			if(state == OBJ_RETRY)			goto LOOP_RETRY;
			else							return -1;
		}

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			return -1;
		}
	}

	// ...
	{
		state = doCH__COMMAND->Set__DATA("ComInit");

		if(state == 0)    // Abort
		{
			return -1;
		}

		if(Is__OFFLINE(p_alarm) < 0)
		{
			return -1;
		}
	}

	// ...
	{
		state = Is__ERROR_EXIST(p_alarm);

		if(state != OBJ_AVAILABLE)
		{
			if(state == OBJ_RETRY)			goto LOOP_RETRY;
			else							return -1;
		}

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			return -1;
		}
	}

	Fnc__APP_LOG("[INIT] Mode --> HOME Start");
	if(Fnc__HOME(p_variable, p_alarm) < 0)
	{
		return -1;
	}

	return 1;
}

// Pick -----
int  CObj__VAC_MAGATRAN
::Call__PICK(CII_OBJECT__VARIABLE* p_variable,
		     CII_OBJECT__ALARM* p_alarm,
		     const CString& stn_name,
		     const CString& arm_type,
		     const CString& stn_slot)
{
	int is_act_handoff;
	CString str_cfg_val;

	// ...
	{
		CString act_name;
		act_name.Format("pick from %s-%s to Arm-%s", stn_name,stn_slot,arm_type);

		Fnc__APP_LOG(act_name);
	}

	if(stn_name.Find("PM") >= 0)	is_act_handoff = ACTIVE_HAND_OFF;
	else							is_act_handoff = NORMAL;

	int r_flag = Fnc__PICK_DYNAMIC_ALIGN_N(p_variable,p_alarm, stn_name,arm_type,stn_slot,is_act_handoff);
	if(r_flag < 0)		return r_flag;

	Fnc__WAFER_CHECK_ON_ARM(arm_type, true, false);
	return 1;
}

int  CObj__VAC_MAGATRAN
::Fnc__SIM_PICK(CII_OBJECT__VARIABLE* p_variable,
			    CII_OBJECT__ALARM* p_alarm,
			    const CString& stn_name,
			    const CString& arm_type,
			    const CString& stn_slot)
{
	SCX__TIMER_CTRL x_timer_ctrl;
	x_timer_ctrl->REGISTER__ABORT_OBJECT(sObject_Name);

	double  cfg__pick_time = 1.0;
	CString var_data;

	if((stn_name.CompareNoCase(STR__LBA) == 0)
	|| (stn_name.CompareNoCase(STR__LBB) == 0))
	{
		aCH__SIM_CFG__LBx_PICK_TIME->Get__DATA(var_data);
	}
	else if(Macro__CHECK_PMx_INDEX(stn_name) >= 0)
	{
		aCH__SIM_CFG__PMx_PICK_TIME->Get__DATA(var_data);
	}

	cfg__pick_time = atof(var_data);
	if(x_timer_ctrl->WAIT(cfg__pick_time) < 0)
	{
		return -1;
	}
	return 1;
}
int  CObj__VAC_MAGATRAN
::Fnc__PICK(CII_OBJECT__VARIABLE* p_variable,
		    CII_OBJECT__ALARM* p_alarm,
		    const CString& stn_name,
		    const CString& arm_type,
		    const CString& stn_slot)
{
	Fnc__ACT_MSG("Pick Start...");

	if(Is__LOCAL(p_alarm)   < 0)		return -1;
	if(Is__OFFLINE(p_alarm) < 0)		return -1;

	// ...
	CString str__trg_stn_num;

	// Set Station
	{
		str__trg_stn_num = Get__CFG_STN_NUM(stn_name);

		     if(arm_type.CompareNoCase(ARM_A) == 0)		doCH__STN_A->Set__DATA(str__trg_stn_num);
		else if(arm_type.CompareNoCase(ARM_B) == 0)		doCH__STN_B->Set__DATA(str__trg_stn_num);
	}

	// Set Slot
	doCH__SLOT->Set__DATA(stn_slot);

	// ...
	int state = -1;

	// Action : [Pick]	
	{
		     if(arm_type.CompareNoCase(ARM_A) == 0)		state = doCH__COMMAND->Set__DATA("APick");
		else if(arm_type.CompareNoCase(ARM_B) == 0)		state = doCH__COMMAND->Set__DATA("BPick");

		if(state == 0)    // Abort
		{
			return -1;
		}

		if(Is__OFFLINE(p_alarm) < 0)
		{
			return -1;
		}
	}

	// ...
	{
		state = Is__ERROR_EXIST(p_alarm);

		if(state != OBJ_AVAILABLE)
		{
			return -1;
		}

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			return -1;
		}
	}

	return 1;
}

int  CObj__VAC_MAGATRAN
::Fnc__PICK_DYNAMIC_ALIGN(CII_OBJECT__VARIABLE* p_variable,
						  CII_OBJECT__ALARM* p_alarm,
						  const CString& stn_name,
						  const CString& arm_type,
						  const CString& stn_slot,
						  const int is_act_handoff)
{
	CString set_ro_offset, set_to_offset;
	CString str_log;

	Fnc__ACT_MSG("Pick DA Start...");

	// Dynamic Alignment
	// Initialize
	// RETRACT... HIGH Sns
	if(arm_type.CompareNoCase(ARM_A) == 0)
	{
		sCH__CPTR_ARM_A_SNS_R_RE_VALUE_HIGH->Set__DATA("UNKNOWN");
		sCH__CPTR_ARM_A_SNS_T_RE_VALUE_HIGH->Set__DATA("UNKNOWN");
		sCH__CPTR_ARM_A_SNS_Z_RE_VALUE_HIGH->Set__DATA("UNKNOWN");

		// RETRACT... LOW Sns
		sCH__CPTR_ARM_A_SNS_R_RE_VALUE_LOW->Set__DATA("UNKNOWN");
		sCH__CPTR_ARM_A_SNS_T_RE_VALUE_LOW->Set__DATA("UNKNOWN");
		sCH__CPTR_ARM_A_SNS_Z_RE_VALUE_LOW->Set__DATA("UNKNOWN");
	}
	else
	{
		sCH__CPTR_ARM_B_SNS_R_RE_VALUE_HIGH->Set__DATA("UNKNOWN");
		sCH__CPTR_ARM_B_SNS_T_RE_VALUE_HIGH->Set__DATA("UNKNOWN");
		sCH__CPTR_ARM_B_SNS_Z_RE_VALUE_HIGH->Set__DATA("UNKNOWN");

		// RETRACT... LOW Sns
		sCH__CPTR_ARM_B_SNS_R_RE_VALUE_LOW->Set__DATA("UNKNOWN");
		sCH__CPTR_ARM_B_SNS_T_RE_VALUE_LOW->Set__DATA("UNKNOWN");
		sCH__CPTR_ARM_B_SNS_Z_RE_VALUE_LOW->Set__DATA("UNKNOWN");
	}
	
	// 1. SET CPTR (OFF and ON)
	int nRet = Set__CPTR_SNS_OFF_ON(p_variable, p_alarm, arm_type, stn_name);
	if(nRet <= 0)	return OBJ_ABORT;

	if(Is__LOCAL(p_alarm)   < 0)		return -1;
	if(Is__OFFLINE(p_alarm) < 0)		return -1;

	// ...
	CString str__trg_stn_num;

	// Set Station
	{
		str__trg_stn_num = Get__CFG_STN_NUM(stn_name);

		     if(arm_type.CompareNoCase(ARM_A) == 0)		doCH__STN_A->Set__DATA(str__trg_stn_num);
		else if(arm_type.CompareNoCase(ARM_B) == 0)		doCH__STN_B->Set__DATA(str__trg_stn_num);
	}

	// Set Slot
	doCH__SLOT->Set__DATA(stn_slot);

	// ...
	int state = -1;

	// Action : [Pick]	
	{
			 if(arm_type.CompareNoCase(ARM_A) == 0)		state = doCH__COMMAND->Set__DATA("APick");
		else if(arm_type.CompareNoCase(ARM_B) == 0)		state = doCH__COMMAND->Set__DATA("BPick");

		if(state == 0)    // Abort
		{
			return -1;
		}

		if(Is__OFFLINE(p_alarm) < 0)
		{
			return -1;
		}
	}

	// ...
	{
		state = Is__ERROR_EXIST(p_alarm);

		if(state != OBJ_AVAILABLE)
		{
			return -1;
		}

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			return -1;
		}
	}

	// 3. Get Sns Value
	{
		nRet = Get__CPTR_SNS(p_variable,p_alarm, arm_type,stn_name,"RETRACT");
		if(nRet <= 0)	return OBJ_ABORT;

		if(iActive_SIM > 0)
		{
			return OBJ_AVAILABLE;
		}
	}

	// 4. PICK 후에 얻어온 R, T, Z 값과 CALIBRATION 된 R, T, Z 값들을 비교 계산한다.
	if(arm_type.CompareNoCase(ARM_A) == 0)
	{
		Fnc__calculation_CPTR_SNS_VAL(stn_name, arm_type, ARM_A_m_set_cptr_tune_val);

		// 5. Rotate 전에 체크한다.
		set_ro_offset.Format("%.f", ARM_A_m_set_cptr_tune_val.ro_tuned_val);
		set_to_offset.Format("%.f", ARM_A_m_set_cptr_tune_val.to_tuned_val);

		str_log.Format("After Calculation, Offset Val --> R:%s, T:%s", set_ro_offset, set_to_offset);
		Fnc__APP_LOG(str_log);
		
		if(Check_Offset_Range(stn_name, p_variable, p_alarm, ARM_A_m_set_cptr_tune_val) < 0)
		{
			return OBJ_ABORT;
		}
	}
	else
	{
		Fnc__calculation_CPTR_SNS_VAL(stn_name, arm_type, ARM_B_m_set_cptr_tune_val);

		// 5. Rotate 전에 체크한다.
		set_ro_offset.Format("%.f", ARM_B_m_set_cptr_tune_val.ro_tuned_val);
		set_to_offset.Format("%.f", ARM_B_m_set_cptr_tune_val.to_tuned_val);

		str_log.Format("After Calculation, Offset Val --> R:%s, T:%s", set_ro_offset, set_to_offset);
		Fnc__APP_LOG(str_log);
		
		if(Check_Offset_Range(stn_name, p_variable, p_alarm, ARM_B_m_set_cptr_tune_val) < 0)
		{
			return OBJ_ABORT;
		}
	}

	return OBJ_AVAILABLE;
}

int  CObj__VAC_MAGATRAN
::Fnc__PICK_DYNAMIC_ALIGN_N(CII_OBJECT__VARIABLE* p_variable,
						    CII_OBJECT__ALARM* p_alarm,
						    const CString& stn_name,
						    const CString& arm_type,
						    const CString& stn_slot,
						    const int is_act_handoff)
{

	Fnc__ACT_MSG("[Fnc__PICK_DYNAMIC_ALIGN_N] Start...");

	int nRet;
	CString set_ro_offset, set_to_offset;
	CString str_log;

	Fnc__ACT_MSG("Pick DA Start...");

	// RETRACT... HIGH Sns
	if(arm_type.CompareNoCase(ARM_A) == 0)
	{
		sCH__CPTR_ARM_A_SNS_R_RE_VALUE_HIGH->Set__DATA("UNKNOWN");
		sCH__CPTR_ARM_A_SNS_T_RE_VALUE_HIGH->Set__DATA("UNKNOWN");
		sCH__CPTR_ARM_A_SNS_Z_RE_VALUE_HIGH->Set__DATA("UNKNOWN");

		// RETRACT... LOW Sns
		sCH__CPTR_ARM_A_SNS_R_RE_VALUE_LOW->Set__DATA("UNKNOWN");
		sCH__CPTR_ARM_A_SNS_T_RE_VALUE_LOW->Set__DATA("UNKNOWN");
		sCH__CPTR_ARM_A_SNS_Z_RE_VALUE_LOW->Set__DATA("UNKNOWN");
	}
	else
	{
		sCH__CPTR_ARM_B_SNS_R_RE_VALUE_HIGH->Set__DATA("UNKNOWN");
		sCH__CPTR_ARM_B_SNS_T_RE_VALUE_HIGH->Set__DATA("UNKNOWN");
		sCH__CPTR_ARM_B_SNS_Z_RE_VALUE_HIGH->Set__DATA("UNKNOWN");

		// RETRACT... LOW Sns
		sCH__CPTR_ARM_B_SNS_R_RE_VALUE_LOW->Set__DATA("UNKNOWN");
		sCH__CPTR_ARM_B_SNS_T_RE_VALUE_LOW->Set__DATA("UNKNOWN");
		sCH__CPTR_ARM_B_SNS_Z_RE_VALUE_LOW->Set__DATA("UNKNOWN");
	}	

	// 1. SET CPTR OFF
	{
		Fnc__APP_LOG("Start Sensor Off");
	
		nRet = Set__CPTR_SNS_OFF_N(p_variable, p_alarm, arm_type, stn_name);
		str_log.Format("End Sensor Off (Result:%d)", nRet);

		Fnc__APP_LOG(str_log);

		if(nRet <= 0)	
		{
			return OBJ_ABORT;
		}
	}

	// 2. SET CPTR ON
	{
		Fnc__APP_LOG("Start Sensor On");
	
		nRet = Set__CPTR_SNS_ON_N(p_variable, p_alarm, arm_type, stn_name);
		str_log.Format("End Sensor On (Result:%d)", nRet);

		Fnc__APP_LOG(str_log);

		if(nRet <= 0)
		{
			return OBJ_ABORT;
		}
	}

	if(Is__LOCAL(p_alarm)   < 0)		return -1;
	if(Is__OFFLINE(p_alarm) < 0)		return -1;

	// ...
	CString str__trg_stn_num;

	// Set Station
	str__trg_stn_num = Get__CFG_STN_NUM(stn_name);

	     if(arm_type.CompareNoCase(ARM_A) == 0)		doCH__STN_A->Set__DATA(str__trg_stn_num);
	else if(arm_type.CompareNoCase(ARM_B) == 0)		doCH__STN_B->Set__DATA(str__trg_stn_num);

	// Set Slot
	doCH__SLOT->Set__DATA(stn_slot);

	// ...
	int state = -1;

	// 3. PICK
	if(dCH__CFG_MACRO_ACTION_PICK_USE->Check__DATA(STR__YES) > 0)
	{
		     if(arm_type.CompareNoCase(ARM_A) == 0)		state = doCH__COMMAND->Set__DATA("APick");
		else if(arm_type.CompareNoCase(ARM_B) == 0)		state = doCH__COMMAND->Set__DATA("BPick");

		if(state == 0)    // Abort
		{
			return -1;
		}

		if(Is__OFFLINE(p_alarm) < 0)
		{
			return -1;
		}

		state = Is__ERROR_EXIST(p_alarm);
		if(state != OBJ_AVAILABLE)
		{
			return -1;
		}
	}
	else
	{
		int r_act;

		r_act = Fnc__EXTEND(p_variable, p_alarm, stn_name, arm_type, stn_slot, is_act_handoff);
		if(r_act < 0)
		{
			CString log_msg;
			CString log_bff;

			log_bff.Format("Fnc__EXTEND() - Error(%1d) \n", r_act);
			log_msg += log_bff;

			Fnc__APP_LOG(log_msg);
			return -101;
		}

		r_act = Fnc__GOUP(p_variable, p_alarm, stn_name, arm_type, stn_slot);
		if(r_act < 0)
		{
			CString log_msg;
			CString log_bff;

			log_bff.Format("Fnc__GOUP() - Error(%1d) \n", r_act);
			log_msg += log_bff;

			Fnc__APP_LOG(log_msg);
			return -102;
		}

		r_act = Fnc__RETRACT(p_variable, p_alarm, stn_name, arm_type, stn_slot, is_act_handoff);
		if(r_act < 0)
		{
			CString log_msg;
			CString log_bff;

			log_bff.Format("Fnc__RETRACT() - Error(%1d) \n", r_act);
			log_msg += log_bff;

			Fnc__APP_LOG(log_msg);
			return -103;
		}
	}

	if(p_variable->Check__CTRL_ABORT() > 0)
	{
		return -1;
	}

	// 4. SET CPTR OFF 
	{
		Fnc__APP_LOG("Start Senosr Off");
	
		nRet = Set__CPTR_SNS_OFF_N(p_variable, p_alarm, arm_type, stn_name);
		str_log.Format("End Sensor Off (Result:%d)", nRet);

		Fnc__APP_LOG(str_log);

		if(nRet <= 0)	
		{
			return OBJ_ABORT;
		}
	}

	// 5. Get Sns Value
	{
		nRet = Get__CPTR_SNS_N(p_variable,p_alarm, arm_type,stn_name,"RETRACT");

		if(nRet <= 0)
		{
			return OBJ_ABORT;
		}
	}

	// 5. PICK 후에 얻어온 R, T, Z 값과 CALIBRATION 된 R, T, Z 값들을 비교 계산한다.
	if(arm_type.CompareNoCase(ARM_A) == 0)
	{
		Fnc__calculation_CPTR_SNS_VAL_N(stn_name, arm_type, ARM_A_m_set_cptr_tune_val);

		// 5. Rotate 전에 체크한다.
		set_ro_offset.Format("%f", ARM_A_m_set_cptr_tune_val.ro_tuned_val);
		set_to_offset.Format("%f", ARM_A_m_set_cptr_tune_val.to_tuned_val);

		str_log.Format("After Calculation, Offset Val --> Y:%s, X:%s", set_ro_offset, set_to_offset);
		Fnc__APP_LOG(str_log);
	}
	else
	{
		Fnc__calculation_CPTR_SNS_VAL_N(stn_name, arm_type, ARM_B_m_set_cptr_tune_val);

		// 5. Rotate 전에 체크한다.
		set_ro_offset.Format("%f", ARM_B_m_set_cptr_tune_val.ro_tuned_val);
		set_to_offset.Format("%f", ARM_B_m_set_cptr_tune_val.to_tuned_val);

		str_log.Format("After Calculation, Offset Val --> R:%s, T:%s", set_ro_offset, set_to_offset);
		Fnc__APP_LOG(str_log);
	}

	Fnc__ACT_MSG("[Fnc__PICK_DYNAMIC_ALIGN_N] End...");
	return OBJ_AVAILABLE;
}

int  CObj__VAC_MAGATRAN
::Fnc__PLACE_DYNAMIC_ALIGN(CII_OBJECT__VARIABLE* p_variable,
						   CII_OBJECT__ALARM* p_alarm,
						   const CString& stn_name,
						   const CString& before_stn_name,
						   const CString& arm_type,
						   const CString& stn_slot,
						   const int is_act_handoff)
{
	CString set_ro_offset;
	CString set_to_offset;
	CString str_log;

	Fnc__ACT_MSG("Place DA Start...");

	// 1. SET CPTR (OFF and ON)
	{
		int nRet = Set__CPTR_SNS_OFF_ON(p_variable, p_alarm, arm_type, stn_name);
		
		if(nRet <= 0)
		{
			return OBJ_ABORT;
		}
	}

	if(Is__LOCAL(p_alarm)   < 0)		return -1;
	if(Is__OFFLINE(p_alarm) < 0)		return -1;

	// ...
	CString str__trg_stn_num;

	// Set Station
	{
		str__trg_stn_num = Get__CFG_STN_NUM(stn_name);

		     if(arm_type.CompareNoCase(ARM_A) == 0)		doCH__STN_A->Set__DATA(str__trg_stn_num);
		else if(arm_type.CompareNoCase(ARM_B) == 0)		doCH__STN_B->Set__DATA(str__trg_stn_num);
	}

	// Set Slot
	doCH__SLOT->Set__DATA(stn_slot);

	if(arm_type.CompareNoCase(ARM_A) == 0)
	{
		if(Check_Offset_Range(before_stn_name, p_variable, p_alarm, ARM_A_m_set_cptr_tune_val) < 0)	
		{
			return OBJ_ABORT;
		}

		set_ro_offset.Format("%.f", ARM_A_m_set_cptr_tune_val.ro_tuned_val);
		set_to_offset.Format("%.f", ARM_A_m_set_cptr_tune_val.to_tuned_val);
	}
	else
	{
		if(Check_Offset_Range(before_stn_name, p_variable, p_alarm, ARM_B_m_set_cptr_tune_val) < 0)	
		{
			return OBJ_ABORT;
		}

		set_ro_offset.Format("%.f", ARM_B_m_set_cptr_tune_val.ro_tuned_val);
		set_to_offset.Format("%.f", ARM_B_m_set_cptr_tune_val.to_tuned_val);
	}

	str_log.Format("To Set OFFSET.. RO:%s microns, TO:%s milidegrees", set_ro_offset, set_to_offset);
	Fnc__APP_LOG(str_log);

	// Set Offset
	aoCH_RO_OFFSET->Set__DATA(set_ro_offset);
	aoCH_TO_OFFSET->Set__DATA(set_to_offset);

	// ...
	int state = -1;

	// Action : [Place]	
	{
		     if(arm_type.CompareNoCase(ARM_A) == 0)		state = doCH__COMMAND->Set__DATA("APlace_Offset");
		else if(arm_type.CompareNoCase(ARM_B) == 0)		state = doCH__COMMAND->Set__DATA("BPlace_Offset");

		if(state == 0)    // Abort
		{
			return -1;
		}

		if(Is__OFFLINE(p_alarm) < 0)
		{
			return -1;
		}
	}

	// ...
	{
		state = Is__ERROR_EXIST(p_alarm);

		if(state != OBJ_AVAILABLE)
		{
			return -1;
		}

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			return -1;
		}
	}

	// 3. Get Sns Value
	{
		int nRet = Get__CPTR_SNS(p_variable,p_alarm, arm_type,stn_name,"EXTEND");

		if(nRet <= 0)
		{
			return OBJ_ABORT;	
		}
	}

	return OBJ_AVAILABLE;
}

int  CObj__VAC_MAGATRAN
::Fnc__PLACE_DYNAMIC_ALIGN_N(CII_OBJECT__VARIABLE* p_variable,
						     CII_OBJECT__ALARM* p_alarm,
						     const CString& stn_name,
						     const CString& before_stn_name,
						     const CString& arm_type,
						     const CString& stn_slot,
						     const int is_act_handoff,
						     const CString& da_usage)
{
	CString str_log;
	CString szCount;

	CString set_ro_offset;
	CString set_to_offset;

	
	Fnc__ACT_MSG("[Fnc__PLACE_DYNAMIC_ALIGN_N] Start...");
	Fnc__ACT_MSG("Place DA Start...");

	// EXTEND... HIGH Sns
	if(arm_type.CompareNoCase(ARM_A) == 0)
	{
		sCH__CPTR_ARM_A_SNS_R_EX_VALUE_HIGH->Set__DATA("UNKNOWN");
		sCH__CPTR_ARM_A_SNS_T_EX_VALUE_HIGH->Set__DATA("UNKNOWN");
		sCH__CPTR_ARM_A_SNS_Z_EX_VALUE_HIGH->Set__DATA("UNKNOWN");

		// RETRACT... LOW Sns
		sCH__CPTR_ARM_A_SNS_R_EX_VALUE_LOW->Set__DATA("UNKNOWN");
		sCH__CPTR_ARM_A_SNS_T_EX_VALUE_LOW->Set__DATA("UNKNOWN");
		sCH__CPTR_ARM_A_SNS_Z_EX_VALUE_LOW->Set__DATA("UNKNOWN");
	}
	else
	{
		sCH__CPTR_ARM_B_SNS_R_EX_VALUE_HIGH->Set__DATA("UNKNOWN");
		sCH__CPTR_ARM_B_SNS_T_EX_VALUE_HIGH->Set__DATA("UNKNOWN");
		sCH__CPTR_ARM_B_SNS_Z_EX_VALUE_HIGH->Set__DATA("UNKNOWN");

		// RETRACT... LOW Sns
		sCH__CPTR_ARM_B_SNS_R_EX_VALUE_LOW->Set__DATA("UNKNOWN");
		sCH__CPTR_ARM_B_SNS_T_EX_VALUE_LOW->Set__DATA("UNKNOWN");
		sCH__CPTR_ARM_B_SNS_Z_EX_VALUE_LOW->Set__DATA("UNKNOWN");
	}

	aDA_DA_RETRY_CHECK_COUNT->Get__DATA(szCount);
	int nCount = atoi(szCount);

LOOP_RETRY:

	// 1. SET CPTR OFF
	{
		Fnc__APP_LOG("Start Sensor Off");

		int nRet = Set__CPTR_SNS_OFF_N(p_variable, p_alarm, arm_type, stn_name);

		str_log.Format("End Sensor Off (Result:%d)", nRet);
		Fnc__APP_LOG(str_log);

		if(nRet <= 0)
		{
			return OBJ_ABORT;
		}
	}

	// 2. SET CPTR ON
	{
		Fnc__APP_LOG("Start Sensor On ");

		int nRet = Set__CPTR_SNS_ON_N(p_variable, p_alarm, arm_type, stn_name);

		str_log.Format("End Sensor On (Result:%d)", nRet);
		Fnc__APP_LOG(str_log);

		if(nRet <= 0)
		{
			return OBJ_ABORT;
		}
	}

	if(Is__LOCAL(p_alarm)   < 0)		return -1;
	if(Is__OFFLINE(p_alarm) < 0)		return -1;

	// ...
	{
		// Set Station
		CString str__trg_stn_num = Get__CFG_STN_NUM(stn_name);

		     if(arm_type.CompareNoCase(ARM_A) == 0)		doCH__STN_A->Set__DATA(str__trg_stn_num);
		else if(arm_type.CompareNoCase(ARM_B) == 0)		doCH__STN_B->Set__DATA(str__trg_stn_num);

		// Set Slot
		doCH__SLOT->Set__DATA(stn_slot);
	}

	// 3. Get Station Information
	{
		int nRet = Fnc__RQ_STN(p_variable,p_alarm, stn_name, arm_type);

		if(nRet <= 0)
		{
			return OBJ_ABORT;
		}
	}

	// ...
	CString strArmR;
	double dbl_Arm_R;

	sCH__RQSTN_SNS_R_VALUE->Get__DATA(strArmR);	
	dbl_Arm_R = atof(strArmR);
	

	// 4. Calculate R & T(angle) Offset
	double dbl_Offset_X;
	double dbl_Offset_Y;

	double dbl_Offset_R;
	double dbl_Offset_T;

	if(arm_type.CompareNoCase(ARM_A) == 0)
	{
		dbl_Offset_X = ARM_A_m_set_cptr_tune_val.to_tuned_val;
		dbl_Offset_Y = ARM_A_m_set_cptr_tune_val.ro_tuned_val;
	}
	else
	{
		dbl_Offset_X = ARM_B_m_set_cptr_tune_val.to_tuned_val;
		dbl_Offset_Y = ARM_B_m_set_cptr_tune_val.ro_tuned_val;
	}

	dbl_Offset_R = (-1.0) * dbl_Offset_Y;		// Offsets mean deviation from reference. So, Arm for place should be corrected as inverse values of offsets
	dbl_Offset_T = (-1.0) * atan(dbl_Offset_X/(dbl_Arm_R+dbl_Offset_Y))*180.0/3.141592*1000.0;

	str_log.Format("Offset X:(%f), Offset Y:(%f), Teaching Arm Length:(%f), Offset R:(%f)micro mm, Offset T:(%f)mili deg", dbl_Offset_X, dbl_Offset_Y, dbl_Arm_R, dbl_Offset_R, dbl_Offset_T);
	Fnc__APP_LOG(str_log);


	// Value Display about Offsets in GUI
	CString strOffset;

	strOffset.Format("%.2f", dbl_Offset_R);
	sCH__Estimated_R_OFFSET->Set__DATA(strOffset);

	strOffset.Format("%.2f", dbl_Offset_T);
	sCH__Estimated_T_OFFSET->Set__DATA(strOffset);


	// 5. Set Offset
	if(da_usage.CompareNoCase(STR__YES) == 0)
	{
		Fnc__APP_LOG("==> DA Offsets are APPLIED");

		if(arm_type.CompareNoCase(ARM_A) == 0)
		{
			ARM_A_m_set_cptr_tune_val.ro_tuned_val = dbl_Offset_R;
			ARM_A_m_set_cptr_tune_val.to_tuned_val = dbl_Offset_T;

			if(Check_Offset_Range(before_stn_name, p_variable, p_alarm, ARM_A_m_set_cptr_tune_val) < 0)		
			{
				if(nCount > 0)
				{
					--nCount;
					str_log.Format("Retray Check:%d", nCount);
					Fnc__APP_LOG(str_log);

					goto LOOP_RETRY;
				}

				return OBJ_ABORT;
			}

			set_ro_offset.Format("%.f", ARM_A_m_set_cptr_tune_val.ro_tuned_val);
			set_to_offset.Format("%.f", ARM_A_m_set_cptr_tune_val.to_tuned_val);
		}
		else
		{
			ARM_B_m_set_cptr_tune_val.ro_tuned_val = dbl_Offset_R;
			ARM_B_m_set_cptr_tune_val.to_tuned_val = dbl_Offset_T;

			if(Check_Offset_Range(before_stn_name, p_variable, p_alarm, ARM_B_m_set_cptr_tune_val) < 0)
			{
				if(nCount > 0)
				{
					--nCount;
					str_log.Format("Retray Check:%d",nCount);
					Fnc__APP_LOG(str_log);

					goto LOOP_RETRY;
				}
				
				return OBJ_ABORT;
			}

			set_ro_offset.Format("%.f", ARM_B_m_set_cptr_tune_val.ro_tuned_val);
			set_to_offset.Format("%.f", ARM_B_m_set_cptr_tune_val.to_tuned_val);
		}
		
		str_log.Format("To Set OFFSET.. RO:%s microns, TO:%s milidegrees", set_ro_offset, set_to_offset);
		Fnc__APP_LOG(str_log);
	}
	else
	{
		Fnc__APP_LOG("==> DA Offsets are NOT APPLIED");
	}


	// 6. Do place action according to Dynamic Alignment Usage
	int state = -1;

	if(da_usage.CompareNoCase(STR__YES) == 0)
	{
		Fnc__APP_LOG("==> OFFSET PLACE is APPLIED");

		// Check Offset range
		if(arm_type.CompareNoCase(ARM_A) == 0)
		{
			if(Check_Offset_Range(stn_name, p_variable, p_alarm, ARM_A_m_set_cptr_tune_val) < 0)
			{
				return OBJ_ABORT;
			}
		}
		else
		{	
			if(Check_Offset_Range(stn_name, p_variable, p_alarm, ARM_B_m_set_cptr_tune_val) < 0)
			{
				return OBJ_ABORT;
			}
		}

		// Set Offset
		aoCH_RO_OFFSET->Set__DATA(set_ro_offset);
		aoCH_TO_OFFSET->Set__DATA(set_to_offset);

		// Action : [Place]	
		     if(arm_type.CompareNoCase(ARM_A) == 0)		state = doCH__COMMAND->Set__DATA("APlace_Offset");
		else if(arm_type.CompareNoCase(ARM_B) == 0)		state = doCH__COMMAND->Set__DATA("BPlace_Offset");
	}
	else
	{
		Fnc__APP_LOG("==> NORMAL PLACE is APPLIED without OFFSET");

		int r_act = _Act__PLACE(p_variable, p_alarm, stn_name, arm_type, stn_slot);
		if(r_act < 0)		return r_act;
	}

	// 7. SET CPTR OFF
	{
		Fnc__APP_LOG("Start Senosr Off");

		int nRet = Set__CPTR_SNS_OFF_N(p_variable, p_alarm, arm_type, stn_name);

		str_log.Format("End Sensor Off (Result:%d)", nRet);
		Fnc__APP_LOG(str_log);

		if(nRet <= 0)
		{
			return OBJ_ABORT;
		}
	}

	// 5. Get Sns Value
	{
		int nRet = Get__CPTR_SNS_N(p_variable,p_alarm, arm_type,stn_name,"EXTEND");

		if(nRet <= 0)
		{
			return OBJ_ABORT;
		}

		Fnc__ACT_MSG("[Fnc__PLACE_DYNAMIC_ALIGN_N] End...");
		
		if(nRet <= 0)
		{
			return OBJ_ABORT;	
		}
	}

	return OBJ_AVAILABLE;
}


// Place -----
int  CObj__VAC_MAGATRAN
::Call__PLACE(CII_OBJECT__VARIABLE* p_variable,
			  CII_OBJECT__ALARM* p_alarm,
			  const CString& stn_name,
			  const CString& arm_type,
			  const CString& stn_slot)
{
	int r_flag = _Call__PLACE(p_variable,p_alarm, stn_name,arm_type,stn_slot);
	if(r_flag < 0)		return r_flag;
	
	Fnc__WAFER_CHECK_ON_ARM(arm_type, false, false);
	return 1;
}
int  CObj__VAC_MAGATRAN
::_Call__PLACE(CII_OBJECT__VARIABLE* p_variable,
			   CII_OBJECT__ALARM* p_alarm,
			   const CString& stn_name,
			   const CString& arm_type,
			   const CString& stn_slot)
{
	int is_act_handoff;
	CString str_cfg_val;
	CString str_data;
	CString str_wfr_sts;
	CString str_before_stn;
	CString act_name;

	// ...
	{
		act_name.Format("place from Arm-%s to %s-%s", arm_type,stn_name,stn_slot);
		Fnc__APP_LOG(act_name);
	}

	if(arm_type.CompareNoCase(ARM_A) == 0)
	{
		sCH__ARM_A_BEFORE_MODE_STS->Get__DATA(str_data);
		dCH__MON_ARM_A_MATERIAL_STATUS->Get__DATA(str_wfr_sts);
		
		str_before_stn = m_sBefore_Stn_Retract_ArmA;
	}
	else
	{
		sCH__ARM_B_BEFORE_MODE_STS->Get__DATA(str_data);
		dCH__MON_ARM_B_MATERIAL_STATUS->Get__DATA(str_wfr_sts);
		
		str_before_stn = m_sBefore_Stn_Retract_ArmB;
	}

	str_cfg_val = Get__CFG_DYNAMIC_ALGN_USE(str_before_stn);

	if((str_data.CompareNoCase("RETRACT_Success") == 0) 
	|| (str_data.CompareNoCase("PICK_Success")    == 0))
	{
		if(str_wfr_sts.CompareNoCase("NONE") != 0)
		{
			if(stn_name.Find("PM") >= 0)	is_act_handoff = ACTIVE_HAND_OFF;
			else							is_act_handoff = NORMAL;

			act_name.Format("Arm-%s, Station From %s to %s", arm_type,str_before_stn, stn_name);
			Fnc__APP_LOG(act_name);

			return Fnc__PLACE_DYNAMIC_ALIGN_N(p_variable,
											  p_alarm,
											  stn_name,
											  str_before_stn,
											  arm_type,
											  stn_slot,
											  is_act_handoff,
											  str_cfg_val);
		}
	}

	return Fnc__PLACE(p_variable,p_alarm, stn_name,arm_type,stn_slot);
}

int  CObj__VAC_MAGATRAN
::Fnc__SIM_PLACE(CII_OBJECT__VARIABLE* p_variable,
			     CII_OBJECT__ALARM* p_alarm,
			     const CString& stn_name,
			     const CString& arm_type,
			     const CString& stn_slot)
{
	SCX__TIMER_CTRL x_timer_ctrl;
	x_timer_ctrl->REGISTER__ABORT_OBJECT(sObject_Name);

	// ...
	double  cfg__place_time = 1.0;
	CString var_data;

	if((stn_name.CompareNoCase(STR__LBA) == 0)
	|| (stn_name.CompareNoCase(STR__LBB) == 0))
	{
		aCH__SIM_CFG__LBx_PLACE_TIME->Get__DATA(var_data);
	}
	else if(Macro__CHECK_PMx_INDEX(stn_name) >= 0)
	{
		aCH__SIM_CFG__PMx_PLACE_TIME->Get__DATA(var_data);
	}

	cfg__place_time = atof(var_data);
	if(x_timer_ctrl->WAIT(cfg__place_time) < 0)
	{
		return -1; 
	}

	return 1;
}
int  CObj__VAC_MAGATRAN
::Fnc__PLACE(CII_OBJECT__VARIABLE* p_variable,
		     CII_OBJECT__ALARM* p_alarm,
		     const CString& stn_name,
		     const CString& arm_type,
		     const CString& stn_slot)
{
	Fnc__ACT_MSG("Place Start...");

	if(Is__LOCAL(p_alarm)   < 0)		return -1;
	if(Is__OFFLINE(p_alarm) < 0)		return -1;

	// Set Station
	{
		CString str__trg_stn_num = Get__CFG_STN_NUM(stn_name);

		     if(arm_type.CompareNoCase(ARM_A) == 0)		doCH__STN_A->Set__DATA(str__trg_stn_num);
		else if(arm_type.CompareNoCase(ARM_B) == 0)		doCH__STN_B->Set__DATA(str__trg_stn_num);
	}

	// Set Slot
	doCH__SLOT->Set__DATA(stn_slot);

	return _Act__PLACE(p_variable, p_alarm, stn_name, arm_type, stn_slot);
}

int  CObj__VAC_MAGATRAN
::_Act__PLACE(CII_OBJECT__VARIABLE* p_variable,
			  CII_OBJECT__ALARM* p_alarm,
			  const CString& stn_name,
			  const CString& arm_type,
			  const CString& stn_slot)
{
	Fnc__ACT_MSG("_Act__PLACE() Start ...");

	if(dCH__CFG_MACRO_ACTION_PLACE_USE->Check__DATA(STR__YES) > 0)
	{
		int state = -1;

			 if(arm_type.CompareNoCase(ARM_A) == 0)		state = doCH__COMMAND->Set__DATA("APlace");
		else if(arm_type.CompareNoCase(ARM_B) == 0)		state = doCH__COMMAND->Set__DATA("BPlace");

		if(state == 0)    // Abort
		{
			return -1;
		}

		if(Is__OFFLINE(p_alarm) < 0)
		{
			return -1;
		}

		state = Is__ERROR_EXIST(p_alarm);
		if(state != OBJ_AVAILABLE)
		{
			return -1;
		}
	}
	else
	{
		int act_handoff = NORMAL;

		int r_act = Fnc__EXTEND(p_variable, p_alarm, stn_name, arm_type, stn_slot, act_handoff);
		if(r_act < 0)
		{
			CString log_msg;
			CString log_bff;

			log_bff.Format("Fnc__EXTEND() - Error(%1d) \n", r_act);
			log_msg += log_bff;

			Fnc__APP_LOG(log_msg);
			return -101;
		}

		r_act = Fnc__GODOWN(p_variable, p_alarm, stn_name, arm_type, stn_slot);
		if(r_act < 0)
		{
			CString log_msg;
			CString log_bff;

			log_bff.Format("Fnc__GODOWN() - Error(%1d) \n", r_act);
			log_msg += log_bff;

			Fnc__APP_LOG(log_msg);
			return -102;
		}

		r_act = Fnc__RETRACT(p_variable, p_alarm, stn_name, arm_type, stn_slot, act_handoff);
		if(r_act < 0)
		{
			CString log_msg;
			CString log_bff;

			log_bff.Format("Fnc__RETRACT() - Error(%1d) \n", r_act);
			log_msg += log_bff;

			Fnc__APP_LOG(log_msg);
			return -103;
		}
	}

	if(p_variable->Check__CTRL_ABORT() > 0)
	{
		return -1;
	}
	return 1;
}


// Rotate -----
int  CObj__VAC_MAGATRAN
::Call__ROTATE(CII_OBJECT__VARIABLE* p_variable,
			   CII_OBJECT__ALARM* p_alarm,
			   const CString& para__stn_name, 
			   const CString& para__arm_type,
			   const CString& para__stn_slot)
{
	if(iActive_SIM > 0)
	{
		return Fnc__SIM_ROTATE(p_variable,p_alarm, para__stn_name,para__arm_type,para__stn_slot);
	}

	return Fnc__ROTATE(p_variable,p_alarm, para__stn_name,para__arm_type,para__stn_slot);
}

int  CObj__VAC_MAGATRAN
::Fnc__SIM_ROTATE(CII_OBJECT__VARIABLE* p_variable,
				  CII_OBJECT__ALARM* p_alarm,
				  const CString& stn_name, 
				  const CString& arm_type,
				  const CString& stn_slot)
{
	if(dCH__OTR_OUT_MON__TRG_MOVE->Check__DATA(stn_name) > 0)
	{
		return 1;
	}

	// ...
	SCX__TIMER_CTRL x_timer_ctrl;
	x_timer_ctrl->REGISTER__ABORT_OBJECT(sObject_Name);

	double  cfg__rotate_time = 1.0;
	CString var_data;

	aCH__SIM_CFG__ROTATE_TIME->Get__DATA(var_data);
	cfg__rotate_time = atof(var_data);

	if(x_timer_ctrl->WAIT(cfg__rotate_time) < 0)
	{
		return -1;
	}
	return 1;
}
int  CObj__VAC_MAGATRAN
::Fnc__ROTATE(CII_OBJECT__VARIABLE* p_variable,
			  CII_OBJECT__ALARM* p_alarm,
			  const CString& stn_name, 
			  const CString& arm_type,
			  const CString& stn_slot)
{
	Fnc__ACT_MSG("Rotate Start...");

	if(Is__LOCAL(p_alarm)   < 0)		return -1;
	if(Is__OFFLINE(p_alarm) < 0)		return -1;

	// ...
	{
		CString str__trg_stn_num = Get__CFG_STN_NUM(stn_name);

			 if(arm_type.CompareNoCase(ARM_A) == 0)		doCH__STN_A->Set__DATA(str__trg_stn_num);
		else if(arm_type.CompareNoCase(ARM_B) == 0)		doCH__STN_B->Set__DATA(str__trg_stn_num);

		doCH__SLOT->Set__DATA(stn_slot);
	}

	// ...
	{
		CString str__zpos = "Down";

		if(arm_type.CompareNoCase(ARM_A) == 0)
		{
			if(dCH__MON_ARM_A_MATERIAL_STATUS->Check__DATA("NONE") > 0)		str__zpos = "Down";
			else															str__zpos = "Up";
		}
		else if(arm_type.CompareNoCase(ARM_B) == 0)
		{
			if(dCH__MON_ARM_B_MATERIAL_STATUS->Check__DATA("NONE") > 0)		str__zpos = "Down";
			else															str__zpos = "Up";
		}

		doCH__ZPOS->Set__DATA(str__zpos);
		doCH__RPOS->Set__DATA("Retract");		// 반드시 넣을것...
	}

	// ...
	{
		int state = -1;

			if(arm_type.CompareNoCase(ARM_A) == 0)		state = doCH__COMMAND->Set__DATA("AGoto");
		else if(arm_type.CompareNoCase(ARM_B) == 0)		state = doCH__COMMAND->Set__DATA("BGoto");

		if(state == 0)    // Abort
		{
			return -1;
		}
		
		if(Is__OFFLINE(p_alarm) < 0)
		{
			return -1;
		}
	}

	// ...
	{
		int state = Is__ERROR_EXIST(p_alarm);

		if(state != OBJ_AVAILABLE)
		{
			return -1;
		}

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			return -1;
		}
	}

	return 1;
}


// Home -----
int  CObj__VAC_MAGATRAN
::Call__HOME(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	if(iActive_SIM > 0)
	{
		return Fnc__SIM_HOME(p_variable,p_alarm);
	}

	return Fnc__HOME(p_variable,p_alarm);
}

int  CObj__VAC_MAGATRAN
::Fnc__SIM_HOME(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM* p_alarm)
{
	Sleep(1500);

	dCH__OTR_OUT_MON__ARM_A_ACT->Set__DATA(STR__RETRACT);
	dCH__OTR_OUT_MON__ARM_B_ACT->Set__DATA(STR__RETRACT);

	dCH__OTR_OUT_MON__TRG_MOVE->Set__DATA(STR__HOME);
	dCH__OTR_OUT_MON__TRG_ROTATE->Set__DATA(STR__HOME);

	return 1;
}
int  CObj__VAC_MAGATRAN
::Fnc__HOME(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
LOOP_RETRY:

	Fnc__ACT_MSG("Home Start...");

	if(Is__LOCAL(p_alarm)   < 0)		return -1;
	if(Is__OFFLINE(p_alarm) < 0)		return -1;

	// ...
	{
		int state = state = doCH__COMMAND->Set__DATA("HomeAll");

		if(state == 0)    // Abort
		{
			return -1;
		}

		if(Is__OFFLINE(p_alarm) < 0)
		{
			return -1;
		}
	}

	// ...
	{
		int state = Is__ERROR_EXIST(p_alarm);

		if(state != OBJ_AVAILABLE)
		{
			if(state == OBJ_RETRY)		goto LOOP_RETRY;
			else						return -1;
		}
		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			return -1;
		}
	}

	if(dCH__CFG_WFR_SNS_CHECK->Check__DATA("TRUE") > 0)
	{
		doCH__ARM_WFR_SNS->Set__DATA("Yes");
		doCH__COMMAND->Set__DATA("WfrSnsIntk");
	}
	else
	{
		doCH__ARM_WFR_SNS->Set__DATA("No");
		doCH__COMMAND->Set__DATA("WfrSnsIntk");
	}

	return 1;
}


// Map -----
int  CObj__VAC_MAGATRAN
::Call__MAP(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	return 1;		// Lam 장비에선 안쓰기로 함.(From... EDD)

	return Fnc__MAP(p_variable,p_alarm);
}


// Extend -----
int  CObj__VAC_MAGATRAN
::Call__EXTEND(CII_OBJECT__VARIABLE* p_variable,
			   CII_OBJECT__ALARM* p_alarm,
			   const CString& stn_name, 
			   const CString& arm_type,
			   const CString& stn_slot,
			   const int is_active_handoff)
{
	CString str_cfg_val;
	CString str_data;
	CString str_wfr_sts;
	CString str_before_stn;
	CString act_name;

	if(arm_type.CompareNoCase(ARM_A) == 0)
	{
		sCH__ARM_A_BEFORE_MODE_STS->Get__DATA(str_data);
		dCH__MON_ARM_A_MATERIAL_STATUS->Get__DATA(str_wfr_sts);
		str_before_stn = m_sBefore_Stn_Retract_ArmA;
	}
	else
	{
		sCH__ARM_B_BEFORE_MODE_STS->Get__DATA(str_data);
		dCH__MON_ARM_B_MATERIAL_STATUS->Get__DATA(str_wfr_sts);
		str_before_stn = m_sBefore_Stn_Retract_ArmB;
	}

	str_cfg_val = Get__CFG_DYNAMIC_ALGN_USE(stn_name);
	str_cfg_val = Get__CFG_DYNAMIC_ALGN_USE(str_before_stn);

	if((str_data.CompareNoCase("RETRACT_Success") == 0) 
	|| (str_data.CompareNoCase("PICK_Success")    == 0))
	{
		if(str_wfr_sts.CompareNoCase("NONE") != 0)
		{
			act_name.Format("Arm-%s, Station From %s to %s", arm_type,str_before_stn, stn_name);
			Fnc__APP_LOG(act_name);

			return Fnc__EXTEND_DYNAMIC_ALIGN_N(p_variable, 
											   p_alarm, 
											   stn_name, 
											   arm_type, 
											   stn_slot, 
											   is_active_handoff,
											   str_cfg_val);
		}
	}

	return Fnc__EXTEND(p_variable,p_alarm, stn_name, arm_type, stn_slot, is_active_handoff);
}


// Retract -----
int  CObj__VAC_MAGATRAN
::Call__RETRACT(CII_OBJECT__VARIABLE* p_variable,
			    CII_OBJECT__ALARM* p_alarm,
			    const CString& stn_name, 
			    const CString& arm_type,
			    const CString& stn_slot,
			    const int is_active_handoff)
{
	return Fnc__RETRACT_DYNAMIC_ALIGN_N(p_variable,p_alarm, stn_name,arm_type,stn_slot,is_active_handoff);
}


// Goup -----
int  CObj__VAC_MAGATRAN
::Call__GOUP(CII_OBJECT__VARIABLE* p_variable,
		     CII_OBJECT__ALARM* p_alarm,
		     const CString& stn_name, 
		     const CString& arm_type,
		     const CString& stn_slot)
{
	return Fnc__GOUP(p_variable,p_alarm, stn_name, arm_type, stn_slot);
}

// GoDown -----
int  CObj__VAC_MAGATRAN
::Call__GODOWN(CII_OBJECT__VARIABLE* p_variable,
			   CII_OBJECT__ALARM* p_alarm,
		  	   const CString& stn_name, 
			   const CString& arm_type,
			   const CString& stn_slot)
{
	return Fnc__GODOWN(p_variable,p_alarm, stn_name, arm_type, stn_slot);
}


// -----
int  CObj__VAC_MAGATRAN
::Call__RQ_STN(CII_OBJECT__VARIABLE* p_variable,
			   CII_OBJECT__ALARM* p_alarm,
			   const CString& stn_name, 
			   const CString& arm_type)
{
	return Fnc__RQ_STN(p_variable,p_alarm, stn_name, arm_type);
}

int  CObj__VAC_MAGATRAN
::Call__SET_STN(CII_OBJECT__VARIABLE* p_variable,
			    CII_OBJECT__ALARM* p_alarm,
			    const CString& stn_name, 
			    const CString& arm_type)
{
	return Fnc__SET_STN(p_variable,p_alarm, stn_name, arm_type);
}


// -----
int  CObj__VAC_MAGATRAN
::Call__TEACHED_CPTR_SAVE(CII_OBJECT__VARIABLE* p_variable,
						  CII_OBJECT__ALARM* p_alarm,
					 	  const CString& stn_name, 
						  const CString& arm_type)
{
	CString str_val;
	double r_hi, t_hi, z_hi;
	double r_lo, t_lo, z_lo;

	CString str_data;
	CString str_cfg_val;

	CString str_wfr_sts;

	// 초기화...
	Init__CALED_CPTR_SNS_VAL_CH(stn_name, arm_type);

	Sleep(1000);	// GUI에서 볼 수 있게...

	if(arm_type.CompareNoCase(ARM_A) == 0)
	{
		sCH__ARM_A_BEFORE_MODE_STS->Get__DATA(str_data);
		dCH__MON_ARM_A_MATERIAL_STATUS->Get__DATA(str_wfr_sts);
	}
	else
	{
		sCH__ARM_B_BEFORE_MODE_STS->Get__DATA(str_data);
		dCH__MON_ARM_B_MATERIAL_STATUS->Get__DATA(str_wfr_sts);
	}

	if(arm_type.CompareNoCase(ARM_A) == 0)
	{
		sCH__CPTR_ARM_A_SNS_R_RE_VALUE_HIGH->Get__DATA(str_val);	r_hi = atof(str_val);
		sCH__CPTR_ARM_A_SNS_T_RE_VALUE_HIGH->Get__DATA(str_val);	t_hi = atof(str_val);
		sCH__CPTR_ARM_A_SNS_Z_RE_VALUE_HIGH->Get__DATA(str_val);	z_hi = atof(str_val);

		// RETRACT... LOW Sns
		sCH__CPTR_ARM_A_SNS_R_RE_VALUE_LOW->Get__DATA(str_val);		r_lo = atof(str_val);
		sCH__CPTR_ARM_A_SNS_T_RE_VALUE_LOW->Get__DATA(str_val);		t_lo = atof(str_val);
		sCH__CPTR_ARM_A_SNS_Z_RE_VALUE_LOW->Get__DATA(str_val);		z_lo = atof(str_val);
	}
	else
	{
		sCH__CPTR_ARM_B_SNS_R_RE_VALUE_HIGH->Get__DATA(str_val);	r_hi = atof(str_val);
		sCH__CPTR_ARM_B_SNS_T_RE_VALUE_HIGH->Get__DATA(str_val);	t_hi = atof(str_val);
		sCH__CPTR_ARM_B_SNS_Z_RE_VALUE_HIGH->Get__DATA(str_val);	z_hi = atof(str_val);

		// RETRACT... LOW Sns
		sCH__CPTR_ARM_B_SNS_R_RE_VALUE_LOW->Get__DATA(str_val);		r_lo = atof(str_val);
		sCH__CPTR_ARM_B_SNS_T_RE_VALUE_LOW->Get__DATA(str_val);		t_lo = atof(str_val);
		sCH__CPTR_ARM_B_SNS_Z_RE_VALUE_LOW->Get__DATA(str_val);		z_lo = atof(str_val);
	}

	Save__CALED_CPTR_SNS_VAL(stn_name, arm_type, r_hi,t_hi,z_hi, r_lo,t_lo,z_lo);
	return 1;
}

int  CObj__VAC_MAGATRAN
::Fnc__SIM_MAP(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	diCH__COMM_STS->Set__DATA(STR__ONLINE);
	return 1;
}
int  CObj__VAC_MAGATRAN
::Fnc__MAP(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
LOOP_RETRY:

	// ...
	{
		Fnc__ACT_MSG("Map Start...");

		if(Is__LOCAL(p_alarm)   < 0)		return -1;
		if(Is__OFFLINE(p_alarm) < 0)		return -1;
	}

	// ...
	{
		int state = doCH__COMMAND->Set__DATA("ChkLoadA");
	
		if(state == 0)    // Abort
		{
			return -1;
		}

		if(Is__OFFLINE(p_alarm) < 0)
		{
			return -1;
		}
	}

	// ...
	{
		int state = Is__ERROR_EXIST(p_alarm);

		if(state != OBJ_AVAILABLE)
		{
			if(state == OBJ_RETRY)		goto LOOP_RETRY;
			else						return -1;
		}

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			return -1;
		}
	}

	// ...
	{
		int state = doCH__COMMAND->Set__DATA("ChkLoadB");
		
		if(state == 0)    // Abort
		{
			return -1;
		}

		if(Is__OFFLINE(p_alarm) < 0)
		{
			return -1;
		}
	}

	// ...
	{
		int state = Is__ERROR_EXIST(p_alarm);

		if(state != OBJ_AVAILABLE)
		{
			if(state == OBJ_RETRY)		goto LOOP_RETRY;
			else						return -1;
		}

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			return -1;
		}
	}

	// INF Update -----
	CString str__wfr_sts;
	CString var_data;

	// A ARM ----- 
	{
		diCH__ARM_A_MAT_READ->Get__DATA(var_data);

			 if(var_data.CompareNoCase("Present") == 0)		str__wfr_sts = "EXIST";
		else if(var_data.CompareNoCase("Absent")  == 0)		str__wfr_sts = "NONE";
		else												str__wfr_sts = "UNKNOWN";

		dCH__MON_ARM_A_MATERIAL_STATUS->Get__DATA(var_data);

		if(((str__wfr_sts.CompareNoCase("EXIST") == 0) && (var_data.CompareNoCase("NONE") == 0))	  
		|| ((str__wfr_sts.CompareNoCase("NONE") == 0) && (var_data.CompareNoCase("NONE") != 0)))
		{
			int alarm_id = ALID__VACRB1_MATERIAL_DIFFERENT_INFO;

			CString r_act;
			CString s_msg;

			s_msg.Format("ARM_A Mapping Result Is ==>  \nSensor Status [%s], Material Status [%s]\n", str__wfr_sts, var_data);
			p_alarm->Popup__ALARM_With_MESSAGE(alarm_id, s_msg, r_act);

			if(r_act.CompareNoCase("ABORT") == 0)
			{
				return OBJ_ABORT;
			}

			// Option : Abort, Yes 를 누르면 H/W 센서정보로 Update !!
			dCH__MON_ARM_A_MATERIAL_STATUS->Set__DATA(str__wfr_sts);
		}
	}

	// B ARM ----- 
	{
		diCH__ARM_B_MAT_READ->Get__DATA(var_data);

		     if(var_data.CompareNoCase("Present") == 0)		str__wfr_sts = "EXIST";
		else if(var_data.CompareNoCase("Absent")  == 0)		str__wfr_sts = "NONE";
		else												str__wfr_sts = "UNKNOWN";

		dCH__MON_ARM_B_MATERIAL_STATUS->Get__DATA(var_data);

		if(((str__wfr_sts.CompareNoCase("EXIST") == 0) && (var_data.CompareNoCase("NONE") == 0))
		|| ((str__wfr_sts.CompareNoCase("NONE") == 0) && (var_data.CompareNoCase("NONE") != 0)))
		{
			int alarm_id = ALID__VACRB1_MATERIAL_DIFFERENT_INFO;

			CString r_act;
			CString s_msg;

			s_msg.Format("ARM_B Mapping Result Is ==> Sensor Status [%s], Material Status [%s]\n", str__wfr_sts, var_data);
			p_alarm->Popup__ALARM_With_MESSAGE(alarm_id, s_msg, r_act);

			if(r_act.CompareNoCase("ABORT") == 0)
			{
				return OBJ_ABORT;
			}

			// Option : Abort, Yes 를 누르면 H/W 센서정보로 Update !!
			dCH__MON_ARM_B_MATERIAL_STATUS->Set__DATA(str__wfr_sts);
		}
	}

	return 1;
}

int  CObj__VAC_MAGATRAN
::Fnc__EXTEND(CII_OBJECT__VARIABLE* p_variable,
			  CII_OBJECT__ALARM* p_alarm,
			  const CString& stn_name,
			  const CString& arm_type,
			  const CString& stn_slot,
			  const int is_active_handoff)
{
	SCX__TIMER_CTRL x_timer_ctrl;
	x_timer_ctrl->REGISTER__ABORT_OBJECT(sObject_Name);

	double  cfg__sim_time = 1.0;
	CString var_data;
	CString str_log;

	// ...
	{
		Fnc__ACT_MSG("EXTEND Start...");

		if(Is__LOCAL(p_alarm)   < 0)		return -1;
		if(Is__OFFLINE(p_alarm) < 0)		return -1;
	}

	// ...
	{
		// Set Station 
		CString str__trg_stn_num = Get__CFG_STN_NUM(stn_name);

		     if(arm_type.CompareNoCase(ARM_A) == 0)		doCH__STN_A->Set__DATA(str__trg_stn_num);
		else if(arm_type.CompareNoCase(ARM_B) == 0)		doCH__STN_B->Set__DATA(str__trg_stn_num);

		// Set Slot
		doCH__SLOT->Set__DATA(stn_slot);

		// Extend
		doCH__RPOS->Set__DATA("Extend");
	}

	// ...
	{
		CString str__zpos = "Down";

		if(arm_type.CompareNoCase(ARM_A) == 0)
		{
			if(dCH__MON_ARM_A_MATERIAL_STATUS->Check__DATA("NONE") > 0)
				str__zpos = "Down";
			else
				str__zpos = "Up";
		}
		else if(arm_type.CompareNoCase(ARM_B) == 0)
		{
			if(dCH__MON_ARM_B_MATERIAL_STATUS->Check__DATA("NONE") > 0)
				str__zpos = "Down";
			else
				str__zpos = "Up";
		}

		if(stn_name.Find("PM") >= 0)
		{
			if(is_active_handoff == ACTIVE_HAND_OFF)
			{
				doCH__ZPOS->Set__DATA("Down");
			}
		}
		else
		{
			doCH__ZPOS->Set__DATA(str__zpos);
		}
	}

	if(iActive_SIM > 0)
	{
		aCH__SIM_CFG__EXTEND_TIME->Get__DATA(var_data);
		cfg__sim_time = atof(var_data);
		
		if(x_timer_ctrl->WAIT(cfg__sim_time) < 0)
		{
			return -1;
		}
		return 1;
	}

	// ...
	{
		int state = -1;

		// Action : [AGoto, BGoto]	
		     if(arm_type.CompareNoCase(ARM_A) == 0)		state = doCH__COMMAND->Set__DATA("AGoto");
		else if(arm_type.CompareNoCase(ARM_B) == 0)		state = doCH__COMMAND->Set__DATA("BGoto");

		str_log.Format("Extend.. Command Ret : %d", state);	
		Fnc__ACT_MSG(str_log);
		
		if(state == 0)    // Abort
		{
			return -1;
		}

		if(Is__OFFLINE(p_alarm) < 0)
		{
			return -1;
		}
	}

	// ...
	{
		int state = Is__ERROR_EXIST(p_alarm);

		str_log.Format("Extend..Error Exist Check Ret : %d", state);	
		Fnc__ACT_MSG(str_log);
		
		if(state != OBJ_AVAILABLE)
		{
			return -1;
		}

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			return -1;
		}
	}

	return 1;
}

int  CObj__VAC_MAGATRAN
::Fnc__EXTEND_OFFSET(CII_OBJECT__VARIABLE* p_variable,
				     CII_OBJECT__ALARM* p_alarm,
				     const CString& stn_name,
				     const CString& arm_type,
				     const CString& stn_slot,
				     const int is_active_handoff)
{
	CString set_ro_offset;
	CString set_to_offset;

	double  cfg__sim_time = 1.0;
	CString var_data;
	CString str_log;

	// ...
	SCX__TIMER_CTRL x_timer_ctrl;
	x_timer_ctrl->REGISTER__ABORT_OBJECT(sObject_Name);

	// ...
	{
		Fnc__ACT_MSG("EXTEND_OFFSET Start...");

		if(Is__LOCAL(p_alarm)   < 0)		return -1;
		if(Is__OFFLINE(p_alarm) < 0)		return -1;
	}

	// ...
	{
		// Set Station
		CString str__trg_stn_num = Get__CFG_STN_NUM(stn_name);

		     if(arm_type.CompareNoCase(ARM_A) == 0)		doCH__STN_A->Set__DATA(str__trg_stn_num);
		else if(arm_type.CompareNoCase(ARM_B) == 0)		doCH__STN_B->Set__DATA(str__trg_stn_num);

		// Set Slot
		doCH__SLOT->Set__DATA(stn_slot);

		// Extend
		doCH__RPOS->Set__DATA("Extend");

		if(arm_type.CompareNoCase(ARM_A) == 0)
		{
			if(Check_Offset_Range(stn_name, p_variable, p_alarm, ARM_A_m_set_cptr_tune_val) < 0)
			{
				return OBJ_ABORT;
			}

			set_ro_offset.Format("%.f", ARM_A_m_set_cptr_tune_val.ro_tuned_val);
			set_to_offset.Format("%.f", ARM_A_m_set_cptr_tune_val.to_tuned_val);
		}
		else
		{
			if(Check_Offset_Range(stn_name, p_variable, p_alarm, ARM_B_m_set_cptr_tune_val) < 0)
			{
				return OBJ_ABORT;
			}

			set_ro_offset.Format("%.f", ARM_B_m_set_cptr_tune_val.ro_tuned_val);
			set_to_offset.Format("%.f", ARM_B_m_set_cptr_tune_val.to_tuned_val);
		}
		
		str_log.Format("To Set OFFSET.. RO:%s microns, TO:%s milidegrees", set_ro_offset,set_to_offset);
		Fnc__APP_LOG(str_log);
	
		// Set Offset
		aoCH_RO_OFFSET->Set__DATA(set_ro_offset);
		aoCH_TO_OFFSET->Set__DATA(set_to_offset);
	}

	// ...
	{
		CString str__zpos = "Down";

		if(arm_type.CompareNoCase(ARM_A) == 0)
		{
			if(dCH__MON_ARM_A_MATERIAL_STATUS->Check__DATA("NONE") > 0)
				str__zpos = "Down";
			else
				str__zpos = "Up";
		}
		else if(arm_type.CompareNoCase(ARM_B) == 0)
		{
			if(dCH__MON_ARM_B_MATERIAL_STATUS->Check__DATA("NONE") > 0)
				str__zpos = "Down";
			else
				str__zpos = "Up";
		}

		if(stn_name.Find("PM") >= 0)
		{
			if(is_active_handoff == ACTIVE_HAND_OFF)
			{
				doCH__ZPOS->Set__DATA("Down");
			}
		}
		else
		{
			doCH__ZPOS->Set__DATA(str__zpos);
		}

		if(iActive_SIM > 0)
		{
			aCH__SIM_CFG__EXTEND_TIME->Get__DATA(var_data);
			cfg__sim_time = atof(var_data);

			if(x_timer_ctrl->WAIT(cfg__sim_time) < 0)
			{
				return -1;
			}
			return 1;
		}
	}

	// ...
	{
		int state = -1;

		// Action : [AGoto_Offset, BGoto_Offset]	
			 if(arm_type.CompareNoCase(ARM_A) == 0)		state = doCH__COMMAND->Set__DATA("AGoto_Offset");
		else if(arm_type.CompareNoCase(ARM_B) == 0)		state = doCH__COMMAND->Set__DATA("BGoto_Offset");

		str_log.Format("Extend Offset.. Command Ret : %d", state);	
		Fnc__ACT_MSG(str_log);

		if(state == 0)    // Abort
		{
			return -1;
		}

		if(Is__OFFLINE(p_alarm) < 0)
		{
			return -1;
		}
	}

	// ...
	{
		int state = Is__ERROR_EXIST(p_alarm);

		str_log.Format("Extend Offset..Error Exist Check Ret : %d", state);	
		Fnc__ACT_MSG(str_log);
		
		if(state != OBJ_AVAILABLE)
		{
			return -1;
		}

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			return -1;
		}
	}

	return 1;
}

int  CObj__VAC_MAGATRAN
::Fnc__EXTEND_DYNAMIC_ALIGN(CII_OBJECT__VARIABLE* p_variable,
						    CII_OBJECT__ALARM* p_alarm,
						    const CString& para__stn_name, 
						    const CString& para__arm_type,
						    const CString& para__stn_slot,
						    const int is_active_handoff)
{
	Fnc__APP_LOG("Extend Dynamic Align Start..");

	// Driver에서 timeout으로 적용된다.
	{
		sCH__ACTION_STS->Set__DATA("EXTEND_DYNAMIC_ALIGN");

		if(para__arm_type.CompareNoCase(ARM_A) == 0)
		{
			if(dCH__MON_ARM_A_MATERIAL_STATUS->Check__DATA("NONE") < 0)
			{
				int nRet = Set__CPTR_SNS_OFF_ON(p_variable, p_alarm, para__arm_type, para__stn_name);
				if(nRet <= 0)	return OBJ_ABORT;
			}
		}
		else if(para__arm_type.CompareNoCase(ARM_B) == 0)
		{
			if(dCH__MON_ARM_B_MATERIAL_STATUS->Check__DATA("NONE") < 0)
			{
				int nRet = Set__CPTR_SNS_OFF_ON(p_variable, p_alarm, para__arm_type, para__stn_name);
				if(nRet <= 0)	return OBJ_ABORT;
			}
		}
	}

	// 2. EXTEND
	{
		int nRet = Fnc__EXTEND_OFFSET(p_variable,p_alarm, para__stn_name,para__arm_type,para__stn_slot,is_active_handoff);
		if(nRet <= 0)	return OBJ_ABORT;
	}

	if(para__arm_type.CompareNoCase(ARM_A) == 0)
	{
		if(dCH__MON_ARM_A_MATERIAL_STATUS->Check__DATA("NONE") < 0)
		{
			// EXTEND... HIGH Sns
			sCH__CPTR_ARM_A_SNS_R_EX_VALUE_HIGH->Set__DATA("UNKNOWN");
			sCH__CPTR_ARM_A_SNS_T_EX_VALUE_HIGH->Set__DATA("UNKNOWN");
			sCH__CPTR_ARM_A_SNS_Z_EX_VALUE_HIGH->Set__DATA("UNKNOWN");

			// EXTEND... LOW Sns
			sCH__CPTR_ARM_A_SNS_R_EX_VALUE_LOW->Set__DATA("UNKNOWN");
			sCH__CPTR_ARM_A_SNS_T_EX_VALUE_LOW->Set__DATA("UNKNOWN");
			sCH__CPTR_ARM_A_SNS_Z_EX_VALUE_LOW->Set__DATA("UNKNOWN");

			int nRet = Get__CPTR_SNS(p_variable,p_alarm, para__arm_type,para__stn_name,"EXTEND");
			if(nRet <= 0)	return OBJ_ABORT;
		}
	}
	else if(para__arm_type.CompareNoCase(ARM_B) == 0)
	{
		if(dCH__MON_ARM_B_MATERIAL_STATUS->Check__DATA("NONE") < 0)
		{
			// EXTEND... HIGH Sns
			sCH__CPTR_ARM_B_SNS_R_EX_VALUE_HIGH->Set__DATA("UNKNOWN");
			sCH__CPTR_ARM_B_SNS_T_EX_VALUE_HIGH->Set__DATA("UNKNOWN");
			sCH__CPTR_ARM_B_SNS_Z_EX_VALUE_HIGH->Set__DATA("UNKNOWN");

			// EXTEND... LOW Sns
			sCH__CPTR_ARM_B_SNS_R_EX_VALUE_LOW->Set__DATA("UNKNOWN");
			sCH__CPTR_ARM_B_SNS_T_EX_VALUE_LOW->Set__DATA("UNKNOWN");
			sCH__CPTR_ARM_B_SNS_Z_EX_VALUE_LOW->Set__DATA("UNKNOWN");

			int nRet = Get__CPTR_SNS(p_variable,p_alarm, para__arm_type,para__stn_name,"EXTEND");
			if(nRet <= 0)	return OBJ_ABORT;
		}
	}

	Fnc__APP_LOG("Extend Dynamic Align Complete..");
	return OBJ_AVAILABLE;
}

int  CObj__VAC_MAGATRAN
::Fnc__EXTEND_DYNAMIC_ALIGN_N(CII_OBJECT__VARIABLE* p_variable,
							  CII_OBJECT__ALARM* p_alarm,
							  const CString& para__stn_name, 
							  const CString& para__arm_type,
							  const CString& para__stn_slot,
							  const int is_active_handoff,
							  const CString& da_usage)
{
	int nCount = 1;

	CString str_log;
	CString szCount;

	// ...
	{
		aDA_DA_RETRY_CHECK_COUNT->Get__DATA(szCount);
		nCount = atoi(szCount);

		Fnc__APP_LOG("[Fnc__EXTEND_DYNAMIC_ALIGN_N] Start...");

		// Driver에서 timeout으로 적용된다.
		sCH__ACTION_STS->Set__DATA("EXTEND_DYNAMIC_ALIGN");
	}

LOOP_RETRY:

	// 1. SET CPTR OFF
	{
		Fnc__APP_LOG("Start Sensor Off");

		int nRet = Set__CPTR_SNS_OFF_N(p_variable,p_alarm, para__arm_type,para__stn_name);

		str_log.Format("End Sensor Off (Result:%d)", nRet);
		Fnc__APP_LOG(str_log);

		if(nRet <= 0)
		{
			return OBJ_ABORT;
		}
	}

	// 2. SET CPTR ON
	{
		Fnc__APP_LOG("Start Sensor On ");

		int nRet = Set__CPTR_SNS_ON_N(p_variable, p_alarm, para__arm_type, para__stn_name);

		str_log.Format("End Sensor On (Result:%d)", nRet);
		Fnc__APP_LOG(str_log);

		if(nRet <= 0)
		{
			return OBJ_ABORT;
		}
	}

	// 3. Get Station Information
	{
		int nRet = Fnc__RQ_STN(p_variable,p_alarm, para__stn_name, para__arm_type);

		if(nRet <= 0)
		{
			return OBJ_ABORT;
		}
	}

	// ...
	CString strArmR;
	double dbl_Arm_R;
	
	sCH__RQSTN_SNS_R_VALUE->Get__DATA(strArmR);	
	dbl_Arm_R = atof(strArmR);

	// 4. Calculate R & T(angle) Offset
	double dbl_Offset_X;
	double dbl_Offset_Y;

	double dbl_Offset_R;
	double dbl_Offset_T;

	if(para__arm_type.CompareNoCase(ARM_A) == 0)
	{
		dbl_Offset_X = ARM_A_m_set_cptr_tune_val.to_tuned_val;
		dbl_Offset_Y = ARM_A_m_set_cptr_tune_val.ro_tuned_val;
	}
	else
	{
		dbl_Offset_X = ARM_B_m_set_cptr_tune_val.to_tuned_val;
		dbl_Offset_Y = ARM_B_m_set_cptr_tune_val.ro_tuned_val;
	}

	dbl_Offset_R = (-1.0) * dbl_Offset_Y;		// Offsets mean deviation from reference. So, Arm for place should be corrected as inverse values of offsets
	dbl_Offset_T = (-1.0) * atan(dbl_Offset_X/(dbl_Arm_R+dbl_Offset_Y))*180.0/3.141592*1000.0;

	str_log.Format("Offset X:(%f), Offset Y:(%f), Teaching Arm Length:(%f), Offset R:(%f)micro mm, Offset T:(%f)mili deg", 
		           dbl_Offset_X,dbl_Offset_Y,dbl_Arm_R,dbl_Offset_R,dbl_Offset_T);
	Fnc__APP_LOG(str_log);

	// Value Display about Offsets in GUI
	CString strOffset;

	strOffset.Format("%.2f", dbl_Offset_R);
	sCH__Estimated_R_OFFSET->Set__DATA(strOffset);

	strOffset.Format("%.2f", dbl_Offset_T);
	sCH__Estimated_T_OFFSET->Set__DATA(strOffset);

	// 5. Set Offset
	if(da_usage.CompareNoCase(STR__YES) == 0)
	{
		Fnc__APP_LOG("==> DA Offsets are APPLIED");

		if(para__arm_type.CompareNoCase(ARM_A) == 0)
		{
			ARM_A_m_set_cptr_tune_val.ro_tuned_val = dbl_Offset_R;
			ARM_A_m_set_cptr_tune_val.to_tuned_val = dbl_Offset_T;
		}
		else
		{
			ARM_B_m_set_cptr_tune_val.ro_tuned_val = dbl_Offset_R;
			ARM_B_m_set_cptr_tune_val.to_tuned_val = dbl_Offset_T;
		}

		str_log.Format("To Set OFFSET.. RO:%f microns, TO:%f milidegrees", dbl_Offset_R,dbl_Offset_T);
		Fnc__APP_LOG(str_log);
	}
	else
	{
		Fnc__APP_LOG("==> DA Offsets are NOT APPLIED");
	}

	// 6. Do place action according to Dynamic Alignment Usage
	if(da_usage.CompareNoCase(STR__YES) == 0)
	{
		Fnc__APP_LOG("==> OFFSET EXTEND is APPLIED");

		// 2. EXTEND
		int nRet = Fnc__EXTEND_OFFSET(p_variable,p_alarm, para__stn_name,para__arm_type,para__stn_slot,is_active_handoff);

		if(nRet < 0)
		{
			if (nCount > 0)
			{
				--nCount;

				str_log.Format("Retray Check:%d", nCount);
				Fnc__APP_LOG(str_log);
				
				goto LOOP_RETRY;
			}

			return OBJ_ABORT;
		}
	}
	else
	{
		Fnc__APP_LOG("==> NORMAL EXTEND is APPLIED without OFFSET");

		int nRet = Fnc__EXTEND(p_variable,p_alarm, para__stn_name,para__arm_type,para__stn_slot,is_active_handoff);
		if(nRet <= 0)		return OBJ_ABORT;
	}

	if(p_variable->Check__CTRL_ABORT() > 0)
	{
		return OBJ_ABORT;
	}

	// 7. SET CPTR OFF
	{
		Fnc__APP_LOG("Start Senosr Off");

		int nRet = Set__CPTR_SNS_OFF_N(p_variable,p_alarm, para__arm_type,para__stn_name);

		str_log.Format("End Sensor Off (Result:%d)", nRet);
		Fnc__APP_LOG(str_log);

		if(nRet <= 0)		return OBJ_ABORT;
	}

	if(para__arm_type.CompareNoCase(ARM_A) == 0)
	{
		if(dCH__MON_ARM_A_MATERIAL_STATUS->Check__DATA("NONE") < 0)
		{
			// EXTEND... HIGH Sns
			sCH__CPTR_ARM_A_SNS_R_EX_VALUE_HIGH->Set__DATA("UNKNOWN");
			sCH__CPTR_ARM_A_SNS_T_EX_VALUE_HIGH->Set__DATA("UNKNOWN");
			sCH__CPTR_ARM_A_SNS_Z_EX_VALUE_HIGH->Set__DATA("UNKNOWN");

			// EXTEND... LOW Sns
			sCH__CPTR_ARM_A_SNS_R_EX_VALUE_LOW->Set__DATA("UNKNOWN");
			sCH__CPTR_ARM_A_SNS_T_EX_VALUE_LOW->Set__DATA("UNKNOWN");
			sCH__CPTR_ARM_A_SNS_Z_EX_VALUE_LOW->Set__DATA("UNKNOWN");

			int nRet = Get__CPTR_SNS_N(p_variable,p_alarm, para__arm_type,para__stn_name,"EXTEND");
			if(nRet <= 0)		return OBJ_ABORT;
		}
	}
	else if(para__arm_type.CompareNoCase(ARM_B) == 0)
	{
		if(dCH__MON_ARM_B_MATERIAL_STATUS->Check__DATA("NONE") < 0)
		{
			// EXTEND... HIGH Sns
			sCH__CPTR_ARM_B_SNS_R_EX_VALUE_HIGH->Set__DATA("UNKNOWN");
			sCH__CPTR_ARM_B_SNS_T_EX_VALUE_HIGH->Set__DATA("UNKNOWN");
			sCH__CPTR_ARM_B_SNS_Z_EX_VALUE_HIGH->Set__DATA("UNKNOWN");

			// EXTEND... LOW Sns
			sCH__CPTR_ARM_B_SNS_R_EX_VALUE_LOW->Set__DATA("UNKNOWN");
			sCH__CPTR_ARM_B_SNS_T_EX_VALUE_LOW->Set__DATA("UNKNOWN");
			sCH__CPTR_ARM_B_SNS_Z_EX_VALUE_LOW->Set__DATA("UNKNOWN");

			int nRet = Get__CPTR_SNS_N(p_variable,p_alarm, para__arm_type,para__stn_name,"EXTEND");
			if(nRet <= 0)		return OBJ_ABORT;
		}
	}

	Fnc__APP_LOG("[Fnc__EXTEND_DYNAMIC_ALIGN_N] End...");
	return OBJ_AVAILABLE;
}

int  CObj__VAC_MAGATRAN
::Fnc__RETRACT_DYNAMIC_ALIGN(CII_OBJECT__VARIABLE* p_variable,
						     CII_OBJECT__ALARM* p_alarm,
						     const CString& para__stn_name, 
						     const CString& para__arm_type,
						     const CString& para__stn_slot,
						     const int is_active_handoff)
{
	CString set_ro_offset;
	CString set_to_offset;
	CString str_log;

	Fnc__APP_LOG("Retract Dynamic Align Start..");

	// Driver에서 timeout으로 적용된다.
	{
		sCH__ACTION_STS->Set__DATA("RETRACT_DYNAMIC_ALIGN");

		if(para__arm_type.CompareNoCase(ARM_A) == 0)
		{
			if(dCH__MON_ARM_A_MATERIAL_STATUS->Check__DATA("NONE") < 0)
			{
				int nRet = Set__CPTR_SNS_OFF_ON(p_variable, p_alarm, para__arm_type, para__stn_name);
				if(nRet <= 0)		return OBJ_ABORT;
			}
		}
		else if(para__arm_type.CompareNoCase(ARM_B) == 0)
		{
			if(dCH__MON_ARM_B_MATERIAL_STATUS->Check__DATA("NONE") < 0)
			{
				int nRet = Set__CPTR_SNS_OFF_ON(p_variable, p_alarm, para__arm_type, para__stn_name);
				if(nRet <= 0)		return OBJ_ABORT;
			}
		}
	}

	// 2. RETRACT
	{
		int nRet = Fnc__RETRACT(p_variable,p_alarm, para__stn_name,para__arm_type,para__stn_slot,is_active_handoff);
		if(nRet <= 0)		return OBJ_ABORT;
	}

	if(para__arm_type.CompareNoCase(ARM_A) == 0)
	{
		if(dCH__MON_ARM_A_MATERIAL_STATUS->Check__DATA("NONE") < 0)
		{
			// RETRACT... HIGH Sns
			sCH__CPTR_ARM_A_SNS_R_RE_VALUE_HIGH->Set__DATA("UNKNOWN");
			sCH__CPTR_ARM_A_SNS_T_RE_VALUE_HIGH->Set__DATA("UNKNOWN");
			sCH__CPTR_ARM_A_SNS_Z_RE_VALUE_HIGH->Set__DATA("UNKNOWN");

			// RETRACT... LOW Sns
			sCH__CPTR_ARM_A_SNS_R_RE_VALUE_LOW->Set__DATA("UNKNOWN");
			sCH__CPTR_ARM_A_SNS_T_RE_VALUE_LOW->Set__DATA("UNKNOWN");
			sCH__CPTR_ARM_A_SNS_Z_RE_VALUE_LOW->Set__DATA("UNKNOWN");

			int nRet = Get__CPTR_SNS(p_variable,p_alarm, para__arm_type,para__stn_name,"RETRACT");
			if(nRet <= 0)		return OBJ_ABORT;

			// 4. PICK 후에 얻어온 R, T, Z 값과 CALIBRATION 된 R, T, Z 값들을 비교 계산한다.
			Fnc__calculation_CPTR_SNS_VAL(para__stn_name, para__arm_type, ARM_A_m_set_cptr_tune_val);
			
			// 5. Rotate 전에 체크한다.
			set_ro_offset.Format("%.f", ARM_A_m_set_cptr_tune_val.ro_tuned_val);
			set_to_offset.Format("%.f", ARM_A_m_set_cptr_tune_val.to_tuned_val);

			str_log.Format("After Calculation, Offset Val --> R:%s, T:%s", set_ro_offset,set_to_offset);
			Fnc__APP_LOG(str_log);
			
			if(Check_Offset_Range(para__stn_name, p_variable, p_alarm, ARM_A_m_set_cptr_tune_val) < 0)
			{
				return OBJ_ABORT;
			}
		}
	}
	else if(para__arm_type.CompareNoCase(ARM_B) == 0)
	{
		if(dCH__MON_ARM_B_MATERIAL_STATUS->Check__DATA("NONE") < 0)
		{
			// RETRACT... HIGH Sns
			sCH__CPTR_ARM_B_SNS_R_RE_VALUE_HIGH->Set__DATA("UNKNOWN");
			sCH__CPTR_ARM_B_SNS_T_RE_VALUE_HIGH->Set__DATA("UNKNOWN");
			sCH__CPTR_ARM_B_SNS_Z_RE_VALUE_HIGH->Set__DATA("UNKNOWN");

			// RETRACT... LOW Sns
			sCH__CPTR_ARM_B_SNS_R_RE_VALUE_LOW->Set__DATA("UNKNOWN");
			sCH__CPTR_ARM_B_SNS_T_RE_VALUE_LOW->Set__DATA("UNKNOWN");
			sCH__CPTR_ARM_B_SNS_Z_RE_VALUE_LOW->Set__DATA("UNKNOWN");

			int nRet = Get__CPTR_SNS(p_variable,p_alarm, para__arm_type,para__stn_name,"RETRACT");
			if(nRet <= 0)		return OBJ_ABORT;

			// 4. RETRACT 후에 얻어온 R, T, Z 값과 CALIBRATION 된 R, T, Z 값들을 비교 계산한다.
			Fnc__calculation_CPTR_SNS_VAL(para__stn_name, para__arm_type, ARM_B_m_set_cptr_tune_val);
			
			// 5. Rotate 전에 체크한다.
			set_ro_offset.Format("%.f", ARM_B_m_set_cptr_tune_val.ro_tuned_val);
			set_to_offset.Format("%.f", ARM_B_m_set_cptr_tune_val.to_tuned_val);

			str_log.Format("After Calculation, Offset Val --> R:%s, T:%s", set_ro_offset,set_to_offset);
			Fnc__APP_LOG(str_log);
			
			if(Check_Offset_Range(para__stn_name, p_variable, p_alarm, ARM_B_m_set_cptr_tune_val) < 0)
			{
				return OBJ_ABORT;
			}
		}
	}

	Fnc__APP_LOG("Retract Dynamic Align Complete..");
	return OBJ_AVAILABLE;
}

int  CObj__VAC_MAGATRAN
::Fnc__RETRACT_DYNAMIC_ALIGN_N(CII_OBJECT__VARIABLE* p_variable,
							   CII_OBJECT__ALARM* p_alarm,
							   const CString& para__stn_name, 
							   const CString& para__arm_type,
							   const CString& para__stn_slot,
							   const int is_active_handoff)
{
	int nCount = 0;

	CString set_ro_offset;
	CString set_to_offset;
	CString str_log;

	// ...
	{
		CString ch_data;

		aDA_DA_RETRY_CHECK_COUNT->Get__DATA(ch_data);
		nCount = atoi(ch_data);

		Fnc__APP_LOG("[Fnc__RETRACT_DYNAMIC_ALIGN_N] Start...");
	}

	// Driver에서 timeout으로 적용된다.
	{
		sCH__ACTION_STS->Set__DATA("RETRACT_DYNAMIC_ALIGN");
	}

	// 1. SET CPTR OFF
	{
		Fnc__APP_LOG("Start Sensor Off");

		int nRet = Set__CPTR_SNS_OFF_N(p_variable, p_alarm, para__arm_type, para__stn_name);

		str_log.Format("End Sensor Off (Result:%d)", nRet);
		Fnc__APP_LOG(str_log);

		if(nRet <= 0)		return OBJ_ABORT;
	}

	// 2. SET CPTR ON
	{
		Fnc__APP_LOG("Start Sensor On ");

		int nRet = Set__CPTR_SNS_ON_N(p_variable, p_alarm, para__arm_type, para__stn_name);

		str_log.Format("End Sensor On (Result:%d)", nRet);
		Fnc__APP_LOG(str_log);

		if(nRet <= 0)		return OBJ_ABORT;
	}

	// 3. RETRACT
	{
		int nRet = Fnc__RETRACT(p_variable,p_alarm, para__stn_name,para__arm_type,para__stn_slot,is_active_handoff);

		if(nRet <= 0)		return OBJ_ABORT;
	}

	// 4. SET CPTR OFF
	{
		Fnc__APP_LOG("Start Senosr Off");

		int nRet = Set__CPTR_SNS_OFF_N(p_variable, p_alarm, para__arm_type, para__stn_name);

		str_log.Format("End Sensor Off (Result:%d)", nRet);
		Fnc__APP_LOG(str_log);

		if(nRet <= 0)		return OBJ_ABORT;
	}

	Fnc__WAFER_CHECK_ON_ARM(para__arm_type, false, true);

	if(para__arm_type.CompareNoCase(ARM_A) == 0)
	{
		if(dCH__MON_ARM_A_MATERIAL_STATUS->Check__DATA("NONE") < 0)
		{
			// RETRACT... HIGH Sns
			sCH__CPTR_ARM_A_SNS_R_RE_VALUE_HIGH->Set__DATA("UNKNOWN");
			sCH__CPTR_ARM_A_SNS_T_RE_VALUE_HIGH->Set__DATA("UNKNOWN");
			sCH__CPTR_ARM_A_SNS_Z_RE_VALUE_HIGH->Set__DATA("UNKNOWN");

			// RETRACT... LOW Sns
			sCH__CPTR_ARM_A_SNS_R_RE_VALUE_LOW->Set__DATA("UNKNOWN");
			sCH__CPTR_ARM_A_SNS_T_RE_VALUE_LOW->Set__DATA("UNKNOWN");
			sCH__CPTR_ARM_A_SNS_Z_RE_VALUE_LOW->Set__DATA("UNKNOWN");

			int nRet = Get__CPTR_SNS_N(p_variable,p_alarm, para__arm_type,para__stn_name,"RETRACT");
			if(nRet <= 0)		return OBJ_ABORT;

			// 6. PICK 후에 얻어온 R, T, Z 값과 CALIBRATION 된 R, T, Z 값들을 비교 계산한다.		
			Fnc__calculation_CPTR_SNS_VAL_N(para__stn_name, para__arm_type, ARM_A_m_set_cptr_tune_val);
			
			// 5. Rotate 전에 체크한다.
			set_ro_offset.Format("%.f", ARM_A_m_set_cptr_tune_val.ro_tuned_val);
			set_to_offset.Format("%.f", ARM_A_m_set_cptr_tune_val.to_tuned_val);

			str_log.Format("After Calculation, Offset Val --> Y:%s, X:%s", set_ro_offset,set_to_offset);
			Fnc__APP_LOG(str_log);
		}
	}
	else if(para__arm_type.CompareNoCase(ARM_B) == 0)
	{
		if(dCH__MON_ARM_B_MATERIAL_STATUS->Check__DATA("NONE") < 0)
		{
			// RETRACT... HIGH Sns
			sCH__CPTR_ARM_B_SNS_R_RE_VALUE_HIGH->Set__DATA("UNKNOWN");
			sCH__CPTR_ARM_B_SNS_T_RE_VALUE_HIGH->Set__DATA("UNKNOWN");
			sCH__CPTR_ARM_B_SNS_Z_RE_VALUE_HIGH->Set__DATA("UNKNOWN");

			// RETRACT... LOW Sns
			sCH__CPTR_ARM_B_SNS_R_RE_VALUE_LOW->Set__DATA("UNKNOWN");
			sCH__CPTR_ARM_B_SNS_T_RE_VALUE_LOW->Set__DATA("UNKNOWN");
			sCH__CPTR_ARM_B_SNS_Z_RE_VALUE_LOW->Set__DATA("UNKNOWN");

			int nRet = Get__CPTR_SNS_N(p_variable,p_alarm, para__arm_type,para__stn_name,"RETRACT");
			if(nRet <= 0)		return OBJ_ABORT;

			// 6. RETRACT 후에 얻어온 R, T, Z 값과 CALIBRATION 된 R, T, Z 값들을 비교 계산한다.
			Fnc__calculation_CPTR_SNS_VAL_N(para__stn_name, para__arm_type, ARM_B_m_set_cptr_tune_val);

			// 5. Rotate 전에 체크한다.
			set_ro_offset.Format("%.f", ARM_B_m_set_cptr_tune_val.ro_tuned_val);
			set_to_offset.Format("%.f", ARM_B_m_set_cptr_tune_val.to_tuned_val);

			str_log.Format("After Calculation, Offset Val --> Y:%s, X:%s", set_ro_offset,set_to_offset);
			Fnc__APP_LOG(str_log);
		}
	}

	Fnc__APP_LOG("[Fnc__RETRACT_DYNAMIC_ALIGN_N] End...");
	return OBJ_AVAILABLE;
}

int  CObj__VAC_MAGATRAN
::Fnc__RETRACT(CII_OBJECT__VARIABLE* p_variable,
			   CII_OBJECT__ALARM* p_alarm,
			   const CString& stn_name,
			   const CString& arm_type,
			   const CString& stn_slot,
			   const int is_active_handoff)
{	
	double  cfg__sim_time = 1.0;
	CString var_data;
	CString str_log;

	SCX__TIMER_CTRL x_timer_ctrl;
	x_timer_ctrl->REGISTER__ABORT_OBJECT(sObject_Name);

	// ...
	{
		Fnc__ACT_MSG("RETRACT Start...");

		if(Is__LOCAL(p_alarm)   < 0)		return -1;
		if(Is__OFFLINE(p_alarm) < 0)		return -1;
	}

	// ...
	{
		// Set Station
		CString str__trg_stn_num = Get__CFG_STN_NUM(stn_name);

		     if(arm_type.CompareNoCase(ARM_A) == 0)		doCH__STN_A->Set__DATA(str__trg_stn_num);
		else if(arm_type.CompareNoCase(ARM_B) == 0)		doCH__STN_B->Set__DATA(str__trg_stn_num);

		// Set Slot
		doCH__SLOT->Set__DATA(stn_slot);

		// Retract
		doCH__RPOS->Set__DATA("Retract");

		if(stn_name.Find("PM") >= 0)
		{
			if(is_active_handoff == ACTIVE_HAND_OFF)
			{
				doCH__ZPOS->Set__DATA("Down");
			}
		}

		if(iActive_SIM > 0)
		{
			aCH__SIM_CFG__RETRACT_TIME->Get__DATA(var_data);
			cfg__sim_time = atof(var_data);

			if(x_timer_ctrl->WAIT(cfg__sim_time) < 0)
			{
				return -1;
			}
			return 1;
		}
	}

	// ...
	{
		int state = -1;

		// Action : [AGoto, BGoto]	
			 if(arm_type.CompareNoCase(ARM_A) == 0)		state = doCH__COMMAND->Set__DATA("AGoto");
		else if(arm_type.CompareNoCase(ARM_B) == 0)		state = doCH__COMMAND->Set__DATA("BGoto");

		str_log.Format("Retract.. Command Ret : %d", state);
		Fnc__ACT_MSG(str_log);
		
		if(state == 0)    // Abort
		{
			return -1;
		}

		if(Is__OFFLINE(p_alarm) < 0)
		{
			return -1;
		}
	}

	// ...
	{
		int state = Is__ERROR_EXIST(p_alarm);

		str_log.Format("Retract..Error Exist Check Ret : %d", state);	
		Fnc__ACT_MSG(str_log);
		
		if(state != OBJ_AVAILABLE)
		{
			return -1;
		}

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			return -1;
		}
	}

	return 1;
}

int  CObj__VAC_MAGATRAN
::Fnc__GOUP(CII_OBJECT__VARIABLE* p_variable,
		    CII_OBJECT__ALARM* p_alarm,
		    const CString& stn_name,
		    const CString& arm_type,
		    const CString& stn_slot)
{
	Fnc__ACT_MSG("GOUP Start...");
	Fnc__APP_LOG("GOUP Start...");

	if(Is__LOCAL(p_alarm)   < 0)		return -1;
	if(Is__OFFLINE(p_alarm) < 0)		return -1;

	// ...
	{
		// Set Station
		CString str__trg_stn_num = Get__CFG_STN_NUM(stn_name);

		     if(arm_type.CompareNoCase(ARM_A) == 0)		doCH__STN_A->Set__DATA(str__trg_stn_num);
		else if(arm_type.CompareNoCase(ARM_B) == 0)		doCH__STN_B->Set__DATA(str__trg_stn_num);

		// Set Slot
		doCH__SLOT->Set__DATA(stn_slot);

		doCH__ZPOS->Set__DATA("Up");
	}

	// ...
	{
		int state = -1;

		// Action : [AGoto, BGoto]	
			 if(arm_type.CompareNoCase(ARM_A) == 0)		state = doCH__COMMAND->Set__DATA("AGoto");
		else if(arm_type.CompareNoCase(ARM_B) == 0)		state = doCH__COMMAND->Set__DATA("BGoto");

		if(state == 0)    // Abort
		{
			return -1;
		}

		if(Is__OFFLINE(p_alarm) < 0)
		{
			return -1;
		}
	}

	// ...
	{
		int state = Is__ERROR_EXIST(p_alarm);

		if(state != OBJ_AVAILABLE)
		{
			return -1;
		}

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			return -1;
		}
	}

	Fnc__APP_LOG("GOUP Complete...");
	return 1;
}

int  CObj__VAC_MAGATRAN
::Fnc__GODOWN(CII_OBJECT__VARIABLE* p_variable,
			  CII_OBJECT__ALARM* p_alarm,
			  const CString& stn_name,
			  const CString& arm_type,
			  const CString& stn_slot)
{
	Fnc__ACT_MSG("GODOWN Start...");
	Fnc__APP_LOG("GODOWN Start...");

	if(Is__LOCAL(p_alarm)   < 0)		return -1;
	if(Is__OFFLINE(p_alarm) < 0)		return -1;

	// ...
	{
		// Set Station
		CString str__trg_stn_num = Get__CFG_STN_NUM(stn_name);

		     if(arm_type.CompareNoCase(ARM_A) == 0)		doCH__STN_A->Set__DATA(str__trg_stn_num);
		else if(arm_type.CompareNoCase(ARM_B) == 0)		doCH__STN_B->Set__DATA(str__trg_stn_num);

		// Set Slot
		doCH__SLOT->Set__DATA(stn_slot);

		// 
		doCH__ZPOS->Set__DATA("Down");
	}

	// ...
	{
		int state = -1;

		// Action : [AGoto, BGoto]	
		     if(arm_type.CompareNoCase(ARM_A) == 0)		state = doCH__COMMAND->Set__DATA("AGoto");
		else if(arm_type.CompareNoCase(ARM_B) == 0)		state = doCH__COMMAND->Set__DATA("BGoto");

		if(state == 0)    // Abort
		{
			return -1;
		}

		if(Is__OFFLINE(p_alarm) < 0)
		{
			return -1;
		}
	}

	// ...
	{
		int state = Is__ERROR_EXIST(p_alarm);

		if(state != OBJ_AVAILABLE)
		{
			return -1;
		}

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			return -1;
		}
	}

	Fnc__APP_LOG("GODOWN Complete...");
	return 1;
}

// -----
int  CObj__VAC_MAGATRAN
::Fnc__RQ_STN(CII_OBJECT__VARIABLE* p_variable,
		 	  CII_OBJECT__ALARM* p_alarm,
			  const CString& stn_name,
			  const CString& arm_type)
{
	CString str_sns_val;
	CString szLog;

	char *token;
	CString szTokenData;
	char szGetData[512];
	char seps[] = " ";
	int  i = 0;

	// ...
	Fnc__ACT_MSG("RQ STN Start...");

	// 초기화
	{
		sCH__RQSTN_SNS_R_VALUE->Set__DATA("UNKNOWN");
		sCH__RQSTN_SNS_T_VALUE->Set__DATA("UNKNOWN");
		sCH__RQSTN_SNS_Z_VALUE->Set__DATA("UNKNOWN");

		if(Is__LOCAL(p_alarm)   < 0)		return -1;
		if(Is__OFFLINE(p_alarm) < 0)		return -1;
	}

	// ...
	{
		// Set Station
		CString str__trg_stn_num = Get__CFG_STN_NUM(stn_name);

		     if(arm_type.CompareNoCase(ARM_A) == 0)		doCH__STN_A->Set__DATA(str__trg_stn_num);
		else if(arm_type.CompareNoCase(ARM_B) == 0)		doCH__STN_B->Set__DATA(str__trg_stn_num);
	}

	// ...
	{
		int state = -1;

			 if(arm_type.CompareNoCase(ARM_A) == 0)		state = doCH__COMMAND->Set__DATA("AGet_Current_Stn");
		else if(arm_type.CompareNoCase(ARM_B) == 0)		state = doCH__COMMAND->Set__DATA("BGet_Current_Stn");

		if(iActive_SIM > 0)
		{
			state = 1;

			str_sns_val = "STN 0002 A 0936750 150340 019457"; 
		}

		if(state > 0)
		{
			sCH__RQ_STN_FULL_VAL->Get__DATA(str_sns_val);

			szLog.Format("Get Stn Val : %s", str_sns_val);
			Fnc__APP_LOG(szLog);

			strcpy(szGetData, str_sns_val);
			token = strtok(szGetData, seps);

			while(token != NULL)
			{	
				szTokenData = token;
				
				if(i == 3)		sCH__RQSTN_SNS_R_VALUE->Set__DATA(szTokenData);
				if(i == 4)		sCH__RQSTN_SNS_T_VALUE->Set__DATA(szTokenData);
				if(i == 5)		sCH__RQSTN_SNS_Z_VALUE->Set__DATA(szTokenData);

				i++;

				/* Get next token: */
				token = strtok( NULL, seps);	
			}
		}

		if(Is__OFFLINE(p_alarm) < 0)
		{
			return -1;
		}
	}

	// ...
	{
		int state = Is__ERROR_EXIST(p_alarm);

		if(state != OBJ_AVAILABLE)
		{
			return -1;
		}

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			return -1;
		}
	}

	return 1;
}

// -----
int  CObj__VAC_MAGATRAN
::Fnc__SET_STN(CII_OBJECT__VARIABLE* p_variable,
			   CII_OBJECT__ALARM* p_alarm,
			   const CString& stn_name,
			   const CString& arm_type)
{
	CString str_r_val, str_t_val;
	CString szLog;

	// ...
	Fnc__ACT_MSG("SET STN Start...");

	// 초기화
	{
		aCH__SET_STN_R_VALUE->Get__DATA(str_r_val);
		aCH__SET_STN_T_VALUE->Get__DATA(str_t_val);
	}

	if(Is__LOCAL(p_alarm)   < 0)		return -1;
	if(Is__OFFLINE(p_alarm) < 0)		return -1;

	// ...
	{
		// Set Station
		CString str__trg_stn_num = Get__CFG_STN_NUM(stn_name);

		     if(arm_type.CompareNoCase(ARM_A) == 0)		doCH__STN_A->Set__DATA(str__trg_stn_num);
		else if(arm_type.CompareNoCase(ARM_B) == 0)		doCH__STN_B->Set__DATA(str__trg_stn_num);

		// Set Stn Value
		aoCH_STN_R_VAL->Set__DATA(str_r_val);
		aoCH_STN_T_VAL->Set__DATA(str_t_val);
	}

	// ...
	{
		int state = -1;

			 if(arm_type.CompareNoCase(ARM_A) == 0)		state = doCH__COMMAND->Set__DATA("ASet_Stn");
		else if(arm_type.CompareNoCase(ARM_B) == 0)		state = doCH__COMMAND->Set__DATA("BSet_Stn");

		if(state > 0)
		{

		}

		if(Is__OFFLINE(p_alarm) < 0)
		{
			return -1;
		}
	}

	// ...
	{
		int state = Is__ERROR_EXIST(p_alarm);

		if(state != OBJ_AVAILABLE)
		{
			return -1;
		}

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			return -1;
		}
	}

	return 1;
}


// Swap -----
int  CObj__VAC_MAGATRAN
::Call__SWAP(CII_OBJECT__VARIABLE* p_variable,
		     CII_OBJECT__ALARM* p_alarm,
		     const CString& para__stn_name,
		     const CString& para__arm_type,
		     const CString& para__stn_slot)
{
	if(iActive_SIM > 0)
	{
		return Fnc__SIM_SWAP(p_variable,p_alarm, para__stn_name,para__arm_type,para__stn_slot);
	}

	return Fnc__SWAP(p_variable,p_alarm, para__stn_name,para__arm_type,para__stn_slot);
}

int  CObj__VAC_MAGATRAN
::Fnc__SIM_SWAP(CII_OBJECT__VARIABLE* p_variable,
			    CII_OBJECT__ALARM* p_alarm,
			    const CString& para__stn_name,
			    const CString& para__arm_type,
			    const CString& para__stn_slot)
{
	diCH__COMM_STS->Set__DATA(STR__ONLINE);
	return 1;
}
int  CObj__VAC_MAGATRAN
::Fnc__SWAP(CII_OBJECT__VARIABLE* p_variable,
		    CII_OBJECT__ALARM* p_alarm,
		    const CString& stn_name,
		    const CString& arm_type,
		    const CString& stn_slot)
{
	Fnc__ACT_MSG("Swap Start...");

	if(Is__LOCAL(p_alarm)   < 0)		return -1;
	if(Is__OFFLINE(p_alarm) < 0)		return -1;

	// ...
	{
		// Set Station -----
		CString str__trg_stn_num = Get__CFG_STN_NUM(stn_name);

		if(arm_type.CompareNoCase(ARM_A) == 0)		doCH__STN_A->Set__DATA(str__trg_stn_num);
		else if(arm_type.CompareNoCase(ARM_B) == 0)		doCH__STN_B->Set__DATA(str__trg_stn_num);

		// Set Slot -----
		doCH__SLOT->Set__DATA(stn_slot);
	}

	// ...
	{
		int state = -1;

		     if(arm_type.CompareNoCase(ARM_A) == 0)		state = doCH__COMMAND->Set__DATA("ASwap");
		else if(arm_type.CompareNoCase(ARM_B) == 0)		state = doCH__COMMAND->Set__DATA("BSwap");

		if(state == 0)    // Abort
		{
			return -1;
		}
		if(Is__OFFLINE(p_alarm) < 0)
		{
			return -1;
		}
	}

	// ...
	{
		int state = Is__ERROR_EXIST(p_alarm);

		if(state != OBJ_AVAILABLE)
		{
			return -1;
		}

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			return -1;
		}
	}

	return 1;
}
