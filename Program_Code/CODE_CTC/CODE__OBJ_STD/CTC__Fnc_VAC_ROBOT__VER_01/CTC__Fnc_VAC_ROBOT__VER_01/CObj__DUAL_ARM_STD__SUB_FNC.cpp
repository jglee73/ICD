#include "StdAfx.h"
#include "CObj__DUAL_ARM_STD.h"

#include "Macro_Function.h"


//---------------------------------------------------------------------------------------
MODULE__IS_AVAILABLE(CObj__DUAL_ARM_STD,VAC_RB,  xCH__VAC_RB__OBJ_STATUS,  xCH__VAC_RB__OBJ_CTRL );
MODULE__IS_AVAILABLE(CObj__DUAL_ARM_STD,TMC_CHM, xCH__TMC_CHM__OBJ_STATUS, xCH__TMC_CHM__OBJ_CTRL);

MODULEx__IS_AVAILABLE_WITH_USE_INDEX(CObj__DUAL_ARM_STD, LLx, xEXT_CH__LLx__OBJ_STATUS,xEXT_CH__LLx__OBJ_CTRL,xEXT_CH__SCH_DB_LLx_USE_FLAG);
MODULEx__IS_IDLE(CObj__DUAL_ARM_STD, LLx, xEXT_CH__LLx__OBJ_STATUS,xEXT_CH__LLx__OBJ_CTRL);
MODULEx__IS_STANDBY_CTCINUSE(CObj__DUAL_ARM_STD, LLx, xEXT_CH__LLx__OBJ_STATUS);

MODULEx__IS_AVAILABLE_EX(CObj__DUAL_ARM_STD,PMx, xCH__PMx__OBJ_STATUS, xCH__PMx__OBJ_CTRL, xEXT_CH__CFG__PMx_USE);
MODULEx__IS_IDLE(CObj__DUAL_ARM_STD,PMx, xCH__PMx__OBJ_STATUS, xCH__PMx__OBJ_CTRL);
MODULEx__IS_READY(CObj__DUAL_ARM_STD,PMx, xCH__PMx__OBJ_STATUS, xEXT_CH__CFG__PMx_USE, xCH__PMx__PROC_INFO__PROCESS_STATUS);

//.....
MODULE__SET_OBJ_STATUS(CObj__DUAL_ARM_STD,VAC_RB,  xCH__VAC_RB__OBJ_STATUS);
MODULE__SET_OBJ_STATUS(CObj__DUAL_ARM_STD,TMC_CHM, xCH__TMC_CHM__OBJ_STATUS);

MODULEx__SET_OBJ_STATUS_WITH_INDEX(CObj__DUAL_ARM_STD, LLx, xEXT_CH__LLx__OBJ_STATUS);
MODULEx__SET_OBJ_STATUS(CObj__DUAL_ARM_STD,PMx, xCH__PMx__OBJ_STATUS);

//.....
MODULE__IS_ATM(CObj__DUAL_ARM_STD,TMC_CHM, xCH__TMC_CHM__PRESSURE_STATUS);
MODULE__IS_VAC(CObj__DUAL_ARM_STD,TMC_CHM, xCH__TMC_CHM__PRESSURE_STATUS);

MODULEx__IS_ATM_WITH_INDEX(CObj__DUAL_ARM_STD, LLx, xEXT_CH__LLx__PRESSURE_STATUS);
MODULEx__IS_VAC_WITH_INDEX(CObj__DUAL_ARM_STD, LLx, xEXT_CH__LLx__PRESSURE_STATUS);

MODULEx__IS_ATM(CObj__DUAL_ARM_STD,PMx, xCH__PMx__PRESSURE_STATUS);
MODULEx__IS_VAC(CObj__DUAL_ARM_STD,PMx, xCH__PMx__PRESSURE_STATUS);


//---------------------------------------------------------------------------------------
CString CObj__DUAL_ARM_STD::
Get__LLx_NAME(const int ll_index)
{
	if(ll_index == 0)			return MODULE__LBA;
	if(ll_index == 1)			return MODULE__LBB;
	if(ll_index == 2)			return MODULE__LBC;
	if(ll_index == 3)			return MODULE__LBD;

	return MODULE__LBX;
}
int CObj__DUAL_ARM_STD::
Get__LLx_INDEX(const CString& ll_name)
{
	CString str_name;

	int i_limit = iLLx_LIMIT;
	int i;

	for(i=0; i<i_limit; i++)
	{
		str_name = Get__LLx_NAME(i);
		if(str_name.CompareNoCase(ll_name) == 0)
			return i;

		str_name.Format("LL%1d", i+1);
		if(str_name.CompareNoCase(ll_name) == 0)
			return i;
	}
	return -1;
}
int CObj__DUAL_ARM_STD::
Check__LLx_NAME(const CStringArray& l_llx)
{
	int i_limit = l_llx.GetSize();
	int i;

	for(i=0; i<i_limit; i++)
	{
		if(Get__LLx_INDEX(l_llx[i]) >= 0)
			return 1;
	}
	return -1;
}

void CObj__DUAL_ARM_STD::
LLx__Update_PRESSURE(const int ll_index)
{
	double ref_atm_press;
	double ref_vac_press;
	double cur_press;
	CString var_data;

	xEXT_CH__CFG__REF_ATM_PRESSURE->Get__DATA(var_data);
	ref_atm_press = atof(var_data);

	xEXT_CH__CFG__REF_VAC_PRESSURE->Get__DATA(var_data);
	ref_vac_press = atof(var_data);

	xEXT_CH__LLx__PRESSURE_VALUE[ll_index]->Get__DATA(var_data);
	cur_press = atof(var_data);

	if(cur_press <= ref_vac_press)
	{
		xEXT_CH__LLx__PRESSURE_STATUS[ll_index]->Set__DATA("VAC");
	}
	else if(cur_press >= ref_atm_press)
	{
		xEXT_CH__LLx__PRESSURE_STATUS[ll_index]->Set__DATA("ATM");
	}
	else
	{
		xEXT_CH__LLx__PRESSURE_STATUS[ll_index]->Set__DATA("BTW");
	}
}

void CObj__DUAL_ARM_STD::PMx__Update_PRESSURE(const int pm_i)
{
	double ref_atm_press;
	double ref_vac_press;
	double cur_press;
	CString var_data;

	xEXT_CH__CFG__REF_ATM_PRESSURE->Get__DATA(var_data);
	ref_atm_press = atof(var_data);

	xEXT_CH__CFG__REF_VAC_PRESSURE->Get__DATA(var_data);
	ref_vac_press = atof(var_data);

	xCH__PMx__PRESSURE_VALUE[pm_i]->Get__DATA(var_data);
	cur_press = atof(var_data);

	if(cur_press <= ref_vac_press)
	{
		xCH__PMx__PRESSURE_STATUS[pm_i]->Set__DATA("VAC");
	}
	else if(cur_press >= ref_atm_press)
	{
		xCH__PMx__PRESSURE_STATUS[pm_i]->Set__DATA("ATM");
	}
	else
	{
		xCH__PMx__PRESSURE_STATUS[pm_i]->Set__DATA("BTW");
	}
}

// LPx ...
int  CObj__DUAL_ARM_STD::
LPx__Get_Process_Type__Normal(const CString& sch_name,
							  int& flag__lp_normal)
{
	IDS__SCH_MATERIAL_STATUS ds_sts;
	xSCH_MATERIAL_CTRL->Get__MATERIAL_STATUS(sch_name,ds_sts);

	int lp_i = ds_sts.iPORT_ID - 1;

	if(lp_i <  0)				return -1;
	if(lp_i >= iLPx_SIZE)		return -1;

	flag__lp_normal = xCH__LPx__CFG_PROCESS_TYPE[lp_i]->Check__DATA(STR__NORMAL);
	return 1;
}
int  CObj__DUAL_ARM_STD::
LPx__Check_Process_Type__Normal(const int lp_i)
{
	if(lp_i <  0)				return -1;
	if(lp_i >= iLPx_SIZE)		return -1;

	if(xCH__LPx__CFG_USE[lp_i]->Check__DATA(STR__DUMMY) > 0)
	{
		return 1;
	}
	return xCH__LPx__CFG_PROCESS_TYPE[lp_i]->Check__DATA(STR__NORMAL);
}


//---------------------------------------------------------------------------------------
int  CObj__DUAL_ARM_STD::Set__TMC_CtcInUse_To_Standby()
{
	VAC_RB__Set_CtcInUse_To_Standby();
	TMC_CHM__Set_CtcInUse_To_Standby();

	// LLx ...
	{
		int ll_limit = iLLx_LIMIT;
		int ll_i;

		for(ll_i=0; ll_i<ll_limit; ll_i++)
		{
			LLx__Set_CtcInUse_To_Standby(ll_i);
		}
	}
	return 1;
}
int  CObj__DUAL_ARM_STD::Set__PMx_CtcInUse_To_Standby()
{
	int i;

	for(i=0;i<iPMx_SIZE;i++)
	{
		PMx__Set_CtcInUse_To_Standby(i);
	}
	return 1;
}

//.....
int  CObj__DUAL_ARM_STD::
Module__Check(const CString& fnc_title,
			  CII_OBJECT__ALARM *p_alarm)
{
	if(VAC_RB__Is_Available() < 0)
	{
		p_alarm->Post__ALARM(ALID__VAC_ROBOT__NOT_STANDBY_ERROR);
		return -1;
	}
	if(TMC_CHM__Is_Available() < 0)
	{
		p_alarm->Post__ALARM(ALID__VAC_TMC__NOT_STANDBY_ERROR);
		return -1;
	}

	return 1;
}


//---------------------------------------------------------------------------------------
int  CObj__DUAL_ARM_STD::VAC_RB__Check_Dual_Arm_Enable()
{
	if((xCH__VAC_RB__CFG_A_ARM_USE_MODE->Check__DATA(STR__ENABLE) < 0)
	|| (xCH__VAC_RB__CFG_B_ARM_USE_MODE->Check__DATA(STR__ENABLE) < 0))
	{
		return -1;
	}
	return 1;
}
int  CObj__DUAL_ARM_STD::VAC_RB__Check_A_Arm_Enable()
{
	if(xCH__VAC_RB__CFG_A_ARM_USE_MODE->Check__DATA(STR__ENABLE) < 0)
	{
		return -1;
	}
	return 1;
}
int  CObj__DUAL_ARM_STD::VAC_RB__Check_B_Arm_Enable()
{
	if(xCH__VAC_RB__CFG_B_ARM_USE_MODE->Check__DATA(STR__ENABLE) < 0)
	{
		return -1;
	}
	return 1;
}

int  CObj__DUAL_ARM_STD::VAC_RB__Check_Empty__Arm_Type(const CString& arm_type)
{
	bool active__arm_a = false;
	bool active__arm_b = false;

		 if(arm_type.CompareNoCase(_ARM__A)  == 0)			active__arm_a = true;
	else if(arm_type.CompareNoCase(_ARM__B)  == 0)			active__arm_b = true;
	else if(arm_type.CompareNoCase(_ARM__AB) == 0)
	{
		active__arm_a = true;
		active__arm_b = true;
	}

	if(active__arm_a)
	{
		if(VAC_RB__Check_A_Arm_Enable() > 0)
		{
			if(xCH__VAC_RB__SLOT01_STATUS->Check__DATA(SLOT_STS__NONE) < 0)		return -1;
		}
	}
	if(active__arm_b)
	{
		if(VAC_RB__Check_B_Arm_Enable() > 0)
		{
			if(xCH__VAC_RB__SLOT02_STATUS->Check__DATA(SLOT_STS__NONE) < 0)		return -1;
		}
	}

	return 1;
}
int  CObj__DUAL_ARM_STD::VAC_RB__Check_Empty__Arm_Type()
{
	if(VAC_RB__Check_A_Arm_Enable() > 0)
	{
		if(xCH__VAC_RB__SLOT01_STATUS->Check__DATA(SLOT_STS__NONE) > 0)		return 1;
	}
	if(VAC_RB__Check_B_Arm_Enable() > 0)
	{
		if(xCH__VAC_RB__SLOT02_STATUS->Check__DATA(SLOT_STS__NONE) > 0)		return 1;
	}

	return -1;
}
int  CObj__DUAL_ARM_STD::VAC_RB__Check_Empty__A_Arm()
{
	if(VAC_RB__Check_A_Arm_Enable() < 0)
	{
		return -1;
	}

	if(xCH__VAC_RB__SLOT01_STATUS->Check__DATA(SLOT_STS__NONE) > 0)		return 1;

	return -1;
}
int  CObj__DUAL_ARM_STD::VAC_RB__Check_Empty__B_Arm()
{
	if(VAC_RB__Check_B_Arm_Enable() < 0)
	{
		return -1;
	}

	if(xCH__VAC_RB__SLOT02_STATUS->Check__DATA(SLOT_STS__NONE) > 0)		return 1;

	return -1;
}

int  CObj__DUAL_ARM_STD::VAC_RB__Check_Occupied__Arm_Type()
{
	CString arm_type = _ARM__AB;

	return VAC_RB__Check_Occupied__Arm_Type(arm_type);
}
int  CObj__DUAL_ARM_STD::VAC_RB__Check_Occupied__Arm_Type(const CString& arm_type)
{
	bool active__arm_a = false;
	bool active__arm_b = false;

		 if(arm_type.CompareNoCase(_ARM__A)  == 0)			active__arm_a = true;
	else if(arm_type.CompareNoCase(_ARM__B)  == 0)			active__arm_b = true;
	else if(arm_type.CompareNoCase(_ARM__AB) == 0)
	{
		active__arm_a = true;
		active__arm_b = true;
	}

	if(active__arm_a)
	{
		if(VAC_RB__Check_Occupied__A_Arm() > 0)			return 1;
	}
	if(active__arm_b)
	{
		if(VAC_RB__Check_Occupied__B_Arm() > 0)			return 1;
	}
	return -1;
}

int  CObj__DUAL_ARM_STD::VAC_RB__Check_Occupied__A_Arm()
{
	if(VAC_RB__Check_A_Arm_Enable() < 0)
	{
		return -1;
	}

	if(xCH__VAC_RB__SLOT01_STATUS->Check__DATA(SLOT_STS__NONE) > 0)
	{
		return -1;
	}

	return 1;
}
int  CObj__DUAL_ARM_STD::VAC_RB__Check_Occupied__A_Arm(CString& title)
{
	if(VAC_RB__Check_A_Arm_Enable() < 0)
	{
		return -1;
	}

	if(xCH__VAC_RB__SLOT01_STATUS->Check__DATA(SLOT_STS__NONE) > 0)
	{
		return -1;
	}

	xCH__VAC_RB__SLOT01_TITLE->Get__DATA(title);
	return 1;
}

int  CObj__DUAL_ARM_STD::VAC_RB__Check_Occupied__B_Arm()
{
	if(VAC_RB__Check_B_Arm_Enable() < 0)
	{
		return -1;
	}

	if(xCH__VAC_RB__SLOT02_STATUS->Check__DATA(SLOT_STS__NONE) > 0)
	{
		return -1;
	}

	return 1;
}
int  CObj__DUAL_ARM_STD::VAC_RB__Check_Occupied__B_Arm(CString& title)
{
	if(VAC_RB__Check_B_Arm_Enable() < 0)
	{
		return -1;
	}

	if(xCH__VAC_RB__SLOT02_STATUS->Check__DATA(SLOT_STS__NONE) > 0)
	{
		return -1;
	}

	xCH__VAC_RB__SLOT02_TITLE->Get__DATA(title);
	return 1;
}

int  CObj__DUAL_ARM_STD::VAC_RB__Get_Empty__Arm_Type_From_All(CString& arm_type)
{
	if(VAC_RB__Check_A_Arm_Enable() > 0)
	{
		if(xCH__VAC_RB__SLOT01_STATUS->Check__DATA(SLOT_STS__NONE) > 0)
		{
			arm_type = "A";
			return 1;
		}
	}
	if(VAC_RB__Check_B_Arm_Enable() > 0)
	{
		if(xCH__VAC_RB__SLOT02_STATUS->Check__DATA(SLOT_STS__NONE) > 0)
		{
			arm_type = "B";
			return 1;
		}
	}
	return -1;
}
int  CObj__DUAL_ARM_STD::VAC_RB__Get_Empty__Arm_Type_From_LBx(CString& arm_type)
{
	if(VAC_RB__Check_Dual_Arm_Enable() > 0)
	{
		if(xCH_CFG__PICK_ARM_TYPE_FROM_LBX->Check__DATA("A") > 0)
		{
			if(VAC_RB__Check_Empty__A_Arm() < 0)		return -1;

			arm_type = "A";
			return 1;
		}
		if(xCH_CFG__PICK_ARM_TYPE_FROM_LBX->Check__DATA("B") > 0)
		{
			if(VAC_RB__Check_Empty__B_Arm() < 0)		return -1;

			arm_type = "B";
			return 1;
		}
	}

	return VAC_RB__Get_Empty__Arm_Type_From_All(arm_type);
}
int  CObj__DUAL_ARM_STD::VAC_RB__Get__Arm_Type_From_LBx(CString& arm_type)
{
	if(VAC_RB__Check_A_Arm_Enable() > 0)
	{
		if(xCH_CFG__PICK_ARM_TYPE_FROM_LBX->Check__DATA("A") > 0)
		{
			arm_type = "A";
			return 1;
		}
	}
	if(VAC_RB__Check_B_Arm_Enable() > 0)
	{
		if(xCH_CFG__PICK_ARM_TYPE_FROM_LBX->Check__DATA("B") > 0)
		{
			arm_type = "B";
			return 1;
		}
	}

	return VAC_RB__Get_Empty__Arm_Type_From_All(arm_type);
}
int  CObj__DUAL_ARM_STD::VAC_RB__Get_Empty__Arm_Type_From_PMx(CString& arm_type)
{
	if(VAC_RB__Check_Dual_Arm_Enable() > 0)
	{
		if(xCH_CFG__PICK_ARM_TYPE_FROM_LBX->Check__DATA("A") > 0)
		{
			if(VAC_RB__Check_Empty__B_Arm() < 0)		return -1;

			arm_type = "B";
			return 1;
		}
		else if(xCH_CFG__PICK_ARM_TYPE_FROM_LBX->Check__DATA("B") > 0)
		{
			if(VAC_RB__Check_Empty__A_Arm() < 0)		return -1;

			arm_type = "A";
			return 1;
		}
	}

	return VAC_RB__Get_Empty__Arm_Type_From_All(arm_type);
}

int  CObj__DUAL_ARM_STD::VAC_RB__Get_Occupied__Arm_Count()
{
	int arm_count = 0;

	if(VAC_RB__Check_A_Arm_Enable() > 0)
	{
		if(xCH__VAC_RB__SLOT01_STATUS->Check__DATA(SLOT_STS__NONE) < 0)				arm_count++;
	}
	if(VAC_RB__Check_B_Arm_Enable() > 0)
	{
		if(xCH__VAC_RB__SLOT02_STATUS->Check__DATA(SLOT_STS__NONE) < 0)				arm_count++;
	}

	return arm_count;
}
int  CObj__DUAL_ARM_STD::VAC_RB__Get_Occupied__Arm_Type(CString& arm_type)
{
	if(VAC_RB__Check_A_Arm_Enable() > 0)
	{
		if(xCH__VAC_RB__SLOT01_STATUS->Check__DATA(SLOT_STS__NONE) < 0)
		{
			arm_type = _ARM__A;
			return 1;
		}
	}
	if(VAC_RB__Check_B_Arm_Enable() > 0)
	{
		if(xCH__VAC_RB__SLOT02_STATUS->Check__DATA(SLOT_STS__NONE) < 0)
		{
			arm_type = _ARM__B;
			return 1;
		}
	}

	return -1;
}
int  CObj__DUAL_ARM_STD::VAC_RB__Get_Occupied__Arm_Type(CStringArray& l_arm_type)
{
	l_arm_type.RemoveAll();

	if(VAC_RB__Check_A_Arm_Enable() > 0)
	{
		if(xCH__VAC_RB__SLOT01_STATUS->Check__DATA(SLOT_STS__NONE) < 0)
		{
			l_arm_type.Add(_ARM__A);
		}
	}
	if(VAC_RB__Check_B_Arm_Enable() > 0)
	{
		if(xCH__VAC_RB__SLOT02_STATUS->Check__DATA(SLOT_STS__NONE) < 0)
		{
			l_arm_type.Add(_ARM__B);
		}
	}

	if(l_arm_type.GetSize() > 0)			return 1;
	return -1;
}
int  CObj__DUAL_ARM_STD::VAC_RB__Check_All_Empty()
{
	if(VAC_RB__Check_Dual_Arm_Enable() > 0)
	{
		if(xCH__VAC_RB__SLOT01_STATUS->Check__DATA(SLOT_STS__NONE) < 0)		return -1;
		if(xCH__VAC_RB__SLOT02_STATUS->Check__DATA(SLOT_STS__NONE) < 0)		return -1;

		return 1;
	}

	if(VAC_RB__Check_A_Arm_Enable() > 0)
	{
		if(xCH__VAC_RB__SLOT01_STATUS->Check__DATA(SLOT_STS__NONE) < 0)		return -1;

		return 1;
	}
	if(VAC_RB__Check_B_Arm_Enable() > 0)
	{
		if(xCH__VAC_RB__SLOT02_STATUS->Check__DATA(SLOT_STS__NONE) < 0)		return -1;

		return 1;
	}

	return -1;
}
int  CObj__DUAL_ARM_STD::VAC_RB__Check_Empty__Dual_Arm()
{
	if(VAC_RB__Check_Dual_Arm_Enable() > 0)
	{
		if(xCH__VAC_RB__SLOT01_STATUS->Check__DATA(SLOT_STS__NONE) < 0)		return -1;
		if(xCH__VAC_RB__SLOT02_STATUS->Check__DATA(SLOT_STS__NONE) < 0)		return -1;

		return 1;
	}

	return -1;
}
int  CObj__DUAL_ARM_STD::VAC_RB__Check_Occupied__Dual_Arm()
{
	if(VAC_RB__Check_Dual_Arm_Enable() > 0)
	{
		if(xCH__VAC_RB__SLOT01_STATUS->Check__DATA(SLOT_STS__NONE) > 0)		return -1;
		if(xCH__VAC_RB__SLOT02_STATUS->Check__DATA(SLOT_STS__NONE) > 0)		return -1;
	}
	else
	{
		if(VAC_RB__Check_A_Arm_Enable() > 0)
		{
			if(xCH__VAC_RB__SLOT01_STATUS->Check__DATA(SLOT_STS__NONE) > 0)		return -1;
		}
		if(VAC_RB__Check_B_Arm_Enable() > 0)
		{
			if(xCH__VAC_RB__SLOT02_STATUS->Check__DATA(SLOT_STS__NONE) > 0)		return -1;
		}
	}

	return 1;
}

bool CObj__DUAL_ARM_STD::
VAC_RB__Check_Empty__All_Type_Of_LL_Constraint(const CString& arm_type)
{
	bool active__arm_a = false;
	bool active__arm_b = false;

	     if(arm_type.CompareNoCase(_ARM__A)  == 0)		active__arm_a = true;
	else if(arm_type.CompareNoCase(_ARM__B)  == 0)		active__arm_b = true;
	else if(arm_type.CompareNoCase(_ARM__AB) == 0)
	{
		active__arm_a = true;
		active__arm_b = true;
	}

	if(active__arm_a)
	{
		if(dCH__VAC_RB__CFG_A_ARM_CONSTRAINT_LL->Check__DATA(STR__ALL) > 0)			return true;
	}
	if(active__arm_b)
	{
		if(dCH__VAC_RB__CFG_B_ARM_CONSTRAINT_LL->Check__DATA(STR__ALL) > 0)			return true;
	}
	return false;
}
int  CObj__DUAL_ARM_STD::
VAC_RB__Get_Empty__Arm_Type_With_LL_Constraint(CString& empty_arm, const int ll_slot_id)
{
	if(VAC_RB__Check_Empty__A_Arm() > 0)
	{
		empty_arm = _ARM__A;
		
		//
		CString ch_data = dCH__VAC_RB__CFG_A_ARM_CONSTRAINT_LL->Get__STRING();

		int r_flag = _VAC_RB__Check_LL_Constraint(ch_data, ll_slot_id);
		if(r_flag > 0)		return r_flag;
	}
	if(VAC_RB__Check_Empty__B_Arm() > 0)
	{
		empty_arm = _ARM__B;

		//
		CString ch_data = dCH__VAC_RB__CFG_B_ARM_CONSTRAINT_LL->Get__STRING();

		int r_flag = _VAC_RB__Check_LL_Constraint(ch_data, ll_slot_id);
		if(r_flag > 0)		return r_flag;
	}

	return -1;
}
int  CObj__DUAL_ARM_STD::_VAC_RB__Check_LL_Constraint(const CString& ll_type, const int ll_slot_id)
{
	if(ll_type.CompareNoCase(STR__ALL) == 0)
	{
		return 1;	
	}
	else if(ll_type.CompareNoCase(STR__ODD) == 0)
	{
		if((ll_slot_id % 2) == 1)		return 1;
	}
	else if(ll_type.CompareNoCase(STR__EVEN) == 0)
	{
		if((ll_slot_id % 2) == 0)		return 1;
	}
	
	return -1;
}

int  CObj__DUAL_ARM_STD::
VAC_RB__Get_Empty__Arm_Type_With_PMx_Odd_Even(CString& empty_arm, 
											  const CStringA& pm_name)
{
	if(VAC_RB__Check_Empty__A_Arm() > 0)
	{
		if(_Check__Odd_PMx(pm_name))
		{
			empty_arm = _ARM__A;
			return 1;
		}
	}
	if(VAC_RB__Check_Empty__B_Arm() > 0)
	{
		if(_Check__Even_PMx(pm_name))
		{
			empty_arm = _ARM__B;
			return 1;
		}
	}

	return -1;
}

int  CObj__DUAL_ARM_STD::
VAC_RB__Get_Empty__Arm_Type_With_PMx_Constraint(CString& empty_arm, 
												const CString& pm_name)
{
	CStringArray l_pm_name;
	l_pm_name.Add(pm_name);

	return VAC_RB__Get_Empty__Arm_Type_With_PMx_Constraint(empty_arm, l_pm_name);
}
int  CObj__DUAL_ARM_STD::
VAC_RB__Get_Empty__Arm_Type_With_PMx_Constraint(CString& empty_arm, 
												const CStringArray& l_pm_name)
{
	if(VAC_RB__Check_Empty__A_Arm() > 0)
	{
		CString ch_type = dCH__VAC_RB__CFG_A_ARM_CONSTRAINT_PM->Get__STRING();

		if(ch_type.CompareNoCase(STR__ODD) == 0)
		{
			if(_Check__Odd_PMx(l_pm_name))
			{
				empty_arm = _ARM__A;
				return 1;
			}
		}
		else if(ch_type.CompareNoCase(STR__EVEN) == 0)
		{
			if(_Check__Even_PMx(l_pm_name))
			{
				empty_arm = _ARM__A;
				return 1;
			}
		}
		else
		{
			empty_arm = _ARM__A;
			return 1;
		}
	}

	if(VAC_RB__Check_Empty__B_Arm() > 0)
	{
		CString ch_type = dCH__VAC_RB__CFG_B_ARM_CONSTRAINT_PM->Get__STRING();

		if(ch_type.CompareNoCase(STR__ODD) == 0)
		{
			if(_Check__Odd_PMx(l_pm_name))
			{
				empty_arm = _ARM__B;
				return 1;
			}
		}
		else if(ch_type.CompareNoCase(STR__EVEN) == 0)
		{
			if(_Check__Even_PMx(l_pm_name))
			{
				empty_arm = _ARM__B;
				return 1;
			}
		}
		else
		{
			empty_arm = _ARM__B;
			return 1;
		}
	}

	return -1;
}

bool CObj__DUAL_ARM_STD::
VAC_RB__Check_Empty__Arm_Type_With_PMx_Constraint(const CString& empty_arm, 
												  const CString& pm_name)
{
	if(empty_arm.CompareNoCase(_ARM__A) == 0)
	{
		if(VAC_RB__Check_Empty__A_Arm() > 0)
		{
			CString ch_type = dCH__VAC_RB__CFG_A_ARM_CONSTRAINT_PM->Get__STRING();

			if(ch_type.CompareNoCase(STR__ODD) == 0)
			{
				if(_Check__Odd_PMx(pm_name))			return true;
			}
			else if(ch_type.CompareNoCase(STR__EVEN) == 0)
			{
				if(_Check__Even_PMx(pm_name))			return true;
			}
			else
			{
				return true;
			}
		}
	}

	if(empty_arm.CompareNoCase(_ARM__B) == 0)
	{
		if(VAC_RB__Check_Empty__B_Arm() > 0)
		{
			CString ch_type = dCH__VAC_RB__CFG_B_ARM_CONSTRAINT_PM->Get__STRING();

			if(ch_type.CompareNoCase(STR__ODD) == 0)
			{
				if(_Check__Odd_PMx(pm_name))			return true;
			}
			else if(ch_type.CompareNoCase(STR__EVEN) == 0)
			{
				if(_Check__Even_PMx(pm_name))			return true;
			}
			else
			{
				return true;
			}
		}
	}

	return false;
}
bool CObj__DUAL_ARM_STD::
VAC_RB__Check__Arm_Type_With_PMx_Constraint(const CString& arm_type, 
											const CString& pm_name)
{
	if(arm_type.CompareNoCase(_ARM__A) == 0)
	{
		CString ch_type = dCH__VAC_RB__CFG_A_ARM_CONSTRAINT_PM->Get__STRING();

		if(ch_type.CompareNoCase(STR__ODD) == 0)
		{
			if(_Check__Odd_PMx(pm_name))		return true;
		}
		else if(ch_type.CompareNoCase(STR__EVEN) == 0)
		{
			if(_Check__Even_PMx(pm_name))		return true;
		}
		else
		{
			return true;
		}
	}

	if(arm_type.CompareNoCase(_ARM__B) == 0)
	{
		CString ch_type = dCH__VAC_RB__CFG_B_ARM_CONSTRAINT_PM->Get__STRING();

		if(ch_type.CompareNoCase(STR__ODD) == 0)
		{
			if(_Check__Odd_PMx(pm_name))		return true;
		}
		else if(ch_type.CompareNoCase(STR__EVEN) == 0)
		{
			if(_Check__Even_PMx(pm_name))		return true;
		}
		else
		{
			return true;
		}
	}

	return false;
}

bool CObj__DUAL_ARM_STD::
_Check__Odd_PMx(const CStringArray& l_pm_name)
{
	int i_limit = l_pm_name.GetSize();

	for(int i=0; i<i_limit; i++)
	{
		CString pm_name = l_pm_name[i];

		if(_Check__Odd_PMx(pm_name))			return true;
	}
	return false;
}
bool CObj__DUAL_ARM_STD::
_Check__Odd_PMx(const CString& pm_name)
{
	int k_limit = iPMx_SIZE;

	for(int k=1; k<=k_limit; k+=2)
	{
		CString cmp_name;
		cmp_name.Format("PM%1d", k);

		if(pm_name.CompareNoCase(cmp_name) == 0)		return true;
		if(pm_name.Find(cmp_name) == 0)					return true;
	}
	return false;
}

bool CObj__DUAL_ARM_STD::
_Check__Even_PMx(const CStringArray& l_pm_name)
{
	int i_limit = l_pm_name.GetSize();

	for(int i=0; i<i_limit; i++)
	{
		CString pm_name = l_pm_name[i];

		if(_Check__Even_PMx(pm_name))					return true;
	}
	return false;
}
bool CObj__DUAL_ARM_STD::
_Check__Even_PMx(const CString& pm_name)
{
	int k_limit = iPMx_SIZE;

	for(int k=2; k<=k_limit; k+=2)
	{
		CString cmp_name;
		cmp_name.Format("PM%1d", k);

		if(pm_name.CompareNoCase(cmp_name) == 0)		return true;
		if(pm_name.Find(cmp_name) == 0)					return true;
	}
	return false;
}
bool CObj__DUAL_ARM_STD::
_Check__PM_Name(const CString& pm_name, const CStringArray& l_pm_name)
{
	int i_limit = l_pm_name.GetSize();

	for(int i=0; i<i_limit; i++)
	{
		if(pm_name.CompareNoCase(l_pm_name[i]) != 0)	continue;

		return true;
	}
	return false;
}

//---------------------------------------------------------------------------------------
int	 CObj__DUAL_ARM_STD::
LLx__Get_Size__All_Slot_Status(const int ll_index)
{
	int slot_max = iLLx_SLOT_MAX[ll_index];
	int slot_count = 0;

	for(int i=0; i<slot_max; i++)
	{
		if(dEXT_CH__SCH_DB_LLx_SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__ENABLE) < 0)		continue;
		if(dEXT_CH__CFG_DB_LLx_SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__ENABLE) < 0)		continue;

		if(xEXT_CH__SCH_DB_LLx_SLOT_MODE[ll_index][i]->Check__DATA(SLOT_STS__ALL) < 0)			continue;

		slot_count++;
	}
	return slot_count;
}

int  CObj__DUAL_ARM_STD::
LLx__Check_All_Empty(const int ll_index)
{
	int slot_max = iLLx_SLOT_MAX[ll_index];

	for(int i=0; i<slot_max; i++)
	{
		if(dEXT_CH__SCH_DB_LLx_SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__ENABLE) < 0)		continue;
		if(dEXT_CH__CFG_DB_LLx_SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__ENABLE) < 0)		continue;

		if(xEXT_CH__LLx__SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__NONE) < 0)
		{
			return -1;
		}
	}
	return 1;
}

int  CObj__DUAL_ARM_STD::
LLx__Check_Empty__SlotStatus(const int ll_index,const int slot_id)
{
	int slot_max = iLLx_SLOT_MAX[ll_index];
	int slot_index = slot_id - 1;

	if(slot_index <  0)					return -1;
	if(slot_index >= slot_max)			return -1;

	if(xEXT_CH__LLx__SLOT_STATUS[ll_index][slot_index]->Check__DATA(SLOT_STS__NONE) < 0)
	{
		return -1;
	}
	return 1;
}

int  CObj__DUAL_ARM_STD::
LLx__Check_Empty__InSlot_With_1_Slot_All_Mode(const int ll_index)
{
	if(xEXT_CH__SCH_DB_LLx_USE_FLAG[ll_index]->Check__DATA(STR__ENABLE) < 0)
	{
		return -11;
	}
	if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_index]->Check__DATA(LBx_MODE__ALL) < 0)
	{
		return -12;
	}

	// ...
	int slot_max = iLLx_SLOT_MAX[ll_index];
	int i;

	for(i=0; i<slot_max; i++)
	{
		if(dEXT_CH__SCH_DB_LLx_SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__ENABLE) < 0)		continue;
		if(dEXT_CH__CFG_DB_LLx_SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__ENABLE) < 0)		continue;

		if(xEXT_CH__SCH_DB_LLx_SLOT_MODE[ll_index][i]->Check__DATA(SLOT_STS__ALL) < 0)			continue;

		if(xEXT_CH__LLx__SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__NONE) > 0)
		{
			return 1;
		}
	}
	return -1;
}

int  CObj__DUAL_ARM_STD
::LLx__Check_Empty__InSlot(const int ll_index)
{
	if(xEXT_CH__SCH_DB_LLx_USE_FLAG[ll_index]->Check__DATA(STR__ENABLE) < 0)
	{
		return -11;
	}
	if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_index]->Check__DATA(LBx_MODE__ONLY_OUTPUT) > 0)	
	{
		return -12;
	}

	// ...
	int slot_count = 0;

	int slot_max = iLLx_SLOT_MAX[ll_index];
	int i;

	for(i=0; i<slot_max; i++)
	{
		if(dEXT_CH__SCH_DB_LLx_SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__ENABLE) < 0)			continue;
		if(dEXT_CH__CFG_DB_LLx_SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__ENABLE) < 0)			continue;

		if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_index]->Check__DATA(LBx_MODE__ALL) > 0)
		{
			CString ch_data = xEXT_CH__SCH_DB_LLx_SLOT_MODE[ll_index][i]->Get__STRING();

			if(ch_data.CompareNoCase(SLOT_STS__INPUT) != 0)
			{
				if(ch_data.CompareNoCase(SLOT_STS__ALL) == 0)
				{
					if(xEXT_CH__LLx__SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__NONE) < 0)
					{
						CString sch_name;
						CString ll_name = Get__LLx_NAME(ll_index);

						sch_name.Format("%s-%1d", ll_name,i+1);

						if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(sch_name) > 0)
						{
							return -1;
						}
					}
				}
				else
				{
					continue;
				}
			}
		}

		if(xEXT_CH__LLx__SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__NONE) < 0)
		{
			return -1;
		}

		slot_count++;
	}

	if(slot_count > 0)
	{
		return 1;
	}
	return -1;
}

int  CObj__DUAL_ARM_STD::
LLx__Check_Occupied__InSlot_With_1_Slot_All_Mode(const int ll_index)
{
	if(xEXT_CH__SCH_DB_LLx_USE_FLAG[ll_index]->Check__DATA(STR__ENABLE) < 0)
	{
		return -11;
	}
	if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_index]->Check__DATA(LBx_MODE__ALL) < 0)	
	{
		return -12;
	}

	// ...
	int slot_max = iLLx_SLOT_MAX[ll_index];
	int i;

	for(i=0; i<slot_max; i++)
	{
		if(dEXT_CH__SCH_DB_LLx_SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__ENABLE) < 0)		continue;
		if(dEXT_CH__CFG_DB_LLx_SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__ENABLE) < 0)		continue;

		if(xEXT_CH__SCH_DB_LLx_SLOT_MODE[ll_index][i]->Check__DATA(SLOT_STS__ALL) < 0)			continue;

		if(xEXT_CH__LLx__SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__NONE) < 0)
		{
			CString sch_name;
			CString ll_name = Get__LLx_NAME(ll_index);

			sch_name.Format("%s-%1d", ll_name,i+1);

			if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(sch_name) < 0)
			{
				continue;
			}
			return 1;
		}
	}
	return -1;
}

int  CObj__DUAL_ARM_STD
::LLx__Check_Occupied__InSlot(const int ll_index)
{
	if(xEXT_CH__SCH_DB_LLx_USE_FLAG[ll_index]->Check__DATA(STR__ENABLE) < 0)
	{
		return -11;
	}
	if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_index]->Check__DATA(LBx_MODE__ONLY_OUTPUT) > 0)	
	{
		return -12;
	}

	// ...
	int slot_max = iLLx_SLOT_MAX[ll_index];
	int i;

	for(i=0; i<slot_max; i++)
	{
		if(dEXT_CH__SCH_DB_LLx_SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__ENABLE) < 0)			continue;
		if(dEXT_CH__CFG_DB_LLx_SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__ENABLE) < 0)			continue;

		if(xEXT_CH__LLx__SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__NONE) > 0)
		{
			continue;
		}

		if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_index]->Check__DATA(LBx_MODE__ALL) > 0)
		{
			CString ch_data = xEXT_CH__SCH_DB_LLx_SLOT_MODE[ll_index][i]->Get__STRING();

			if(ch_data.CompareNoCase(SLOT_STS__INPUT) == 0)
			{
				return 11;
			}

			if(ch_data.CompareNoCase(SLOT_STS__ALL) == 0)
			{
				CString sch_name;
				CString ll_name = Get__LLx_NAME(ll_index);

				sch_name.Format("%s-%1d", ll_name,i+1);

				if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(sch_name) > 0)
				{
					return 12;
				}
			}
		}
		else
		{
			return 13;
		}
	}

	return -1;
}

int  CObj__DUAL_ARM_STD::
LLx__Get_Empty__InSlot_With_1_Slot_All_Mode(const int ll_index, int& slot_id)
{
	if(xEXT_CH__SCH_DB_LLx_USE_FLAG[ll_index]->Check__DATA(STR__ENABLE) < 0)
	{
		return -11;
	}
	if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_index]->Check__DATA(LBx_MODE__ALL) < 0)
	{
		return -12;
	}

	// ...
	int slot_max = iLLx_SLOT_MAX[ll_index];

	for(int i=0; i<slot_max; i++)
	{
		if(dEXT_CH__SCH_DB_LLx_SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__ENABLE) < 0)		continue;
		if(dEXT_CH__CFG_DB_LLx_SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__ENABLE) < 0)		continue;

		if(xEXT_CH__SCH_DB_LLx_SLOT_MODE[ll_index][i]->Check__DATA(SLOT_STS__ALL) < 0)			continue;

		if(xEXT_CH__LLx__SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__NONE) > 0)
		{
			slot_id = i + 1;
			return 1;
		}
	}
	return -1;
}

int  CObj__DUAL_ARM_STD::
LLx__Get_Empty__InSlot(const int ll_index,int& slot_id)
{
	if(xEXT_CH__SCH_DB_LLx_USE_FLAG[ll_index]->Check__DATA(STR__ENABLE) < 0)
	{
		return -11;
	}
	if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_index]->Check__DATA(LBx_MODE__ONLY_OUTPUT) > 0)
	{
		return -12;
	}

	// ...
	int slot_max = iLLx_SLOT_MAX[ll_index];

	for(int i=0; i<slot_max; i++)
	{
		if(dEXT_CH__SCH_DB_LLx_SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__ENABLE) < 0)			continue;
		if(dEXT_CH__CFG_DB_LLx_SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__ENABLE) < 0)			continue;

		if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_index]->Check__DATA(LBx_MODE__ALL) > 0)
		{
			CString ch_data = xEXT_CH__SCH_DB_LLx_SLOT_MODE[ll_index][i]->Get__STRING();

			if((ch_data.CompareNoCase(SLOT_STS__INPUT) != 0)
			&& (ch_data.CompareNoCase(SLOT_STS__ALL)   != 0))
			{
				continue;
			}
		}

		if(xEXT_CH__LLx__SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__NONE) > 0)
		{
			slot_id = i + 1;
			return 1;
		}
	}
	return -1;
}

int  CObj__DUAL_ARM_STD::
LLx__Get_Occupied__InSlot_With_1_Slot_All_Mode(const int ll_index,int& slot_id)
{
	if(xEXT_CH__SCH_DB_LLx_USE_FLAG[ll_index]->Check__DATA(STR__ENABLE) < 0)
	{
		return -11;
	}
	if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_index]->Check__DATA(LBx_MODE__ALL) < 0)
	{
		return -12;
	}

	// ...
	int slot_max = iLLx_SLOT_MAX[ll_index];

	for(int i=0; i<slot_max; i++)
	{
		if(dEXT_CH__SCH_DB_LLx_SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__ENABLE) < 0)			continue;
		if(dEXT_CH__CFG_DB_LLx_SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__ENABLE) < 0)			continue;

		if(xEXT_CH__SCH_DB_LLx_SLOT_MODE[ll_index][i]->Check__DATA(SLOT_STS__ALL) < 0)
		{
			continue;
		}

		if(xEXT_CH__LLx__SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__NONE) > 0)
		{
			continue;
		}

		// ...
		{
			CString sch_name;
			CString ll_name = Get__LLx_NAME(ll_index);

			sch_name.Format("%s-%1d", ll_name,i+1);

			if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(sch_name) < 0)
			{
				continue;
			}

			slot_id = i + 1;
		}
		return 1;
	}

	return -1;
}

int  CObj__DUAL_ARM_STD::
LLx__Get_Occupied__InSlot(const int ll_index,int& slot_id)
{
	if(xEXT_CH__SCH_DB_LLx_USE_FLAG[ll_index]->Check__DATA(STR__ENABLE) < 0)
	{
		return -11;
	}
	if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_index]->Check__DATA(LBx_MODE__ONLY_OUTPUT) > 0)
	{
		return -12;
	}

	// ...
	int slot_max = iLLx_SLOT_MAX[ll_index];

	for(int i=0; i<slot_max; i++)
	{
		if(dEXT_CH__SCH_DB_LLx_SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__ENABLE) < 0)			continue;
		if(dEXT_CH__CFG_DB_LLx_SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__ENABLE) < 0)			continue;

		if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_index]->Check__DATA(LBx_MODE__ALL) > 0)
		{
			CString ch_data = xEXT_CH__SCH_DB_LLx_SLOT_MODE[ll_index][i]->Get__STRING();

			if((ch_data.CompareNoCase(SLOT_STS__INPUT) != 0)
			&& (ch_data.CompareNoCase(SLOT_STS__ALL)   != 0))
			{
				continue;
			}
		}

		if(xEXT_CH__LLx__SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__NONE) < 0)
		{
			slot_id = i + 1;
			return 1;
		}
	}
	return -1;
}

int  CObj__DUAL_ARM_STD
::LLx__Get_Occupied__InSlot_Count(const int ll_index)
{
	CUIntArray l__ll_slot;

	int r_flag = LLx__Get_Occupied__InSlot(ll_index, l__ll_slot);
	if(r_flag < 0)		return -1;

	return l__ll_slot.GetSize();
}
int  CObj__DUAL_ARM_STD
::LLx__Get_Occupied__InSlot(const int ll_index, const int db_index,int& slot_id)
{
	if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_index]->Check__DATA(LBx_MODE__ONLY_OUTPUT) > 0)
	{
		return -1;
	}

	// ...
	int slot_max = iLLx_SLOT_MAX[ll_index];

	if(db_index <  0)				return -11;
	if(db_index >= slot_max)		return -12;

	if(dEXT_CH__SCH_DB_LLx_SLOT_STATUS[ll_index][db_index]->Check__DATA(SLOT_STS__ENABLE) < 0)		return -201;
	if(dEXT_CH__CFG_DB_LLx_SLOT_STATUS[ll_index][db_index]->Check__DATA(SLOT_STS__ENABLE) < 0)		return -202;

	if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_index]->Check__DATA(LBx_MODE__ALL) > 0)
	{
		CString ch_data = xEXT_CH__SCH_DB_LLx_SLOT_MODE[ll_index][db_index]->Get__STRING();

		if((ch_data.CompareNoCase(SLOT_STS__INPUT) != 0)
		&& (ch_data.CompareNoCase(SLOT_STS__ALL)   != 0))
		{
			return -22;
		}
	}

	if(xEXT_CH__LLx__SLOT_STATUS[ll_index][db_index]->Check__DATA(SLOT_STS__NONE) > 0)
	{
		return -23;
	}

	slot_id = db_index + 1;
	return 1;
}
int  CObj__DUAL_ARM_STD::
LLx__Get_Occupied__InSlot(const int ll_index,CUIntArray& l_slot_id)
{
	l_slot_id.RemoveAll();

	// ...
	int slot_max = iLLx_SLOT_MAX[ll_index];

	for(int i=0; i<slot_max; i++)
	{
		int slot_id;
		
		if(LLx__Get_Occupied__InSlot(ll_index, i,slot_id) < 0)
		{
			continue;
		}

		l_slot_id.Add(slot_id);
	}

	if(l_slot_id.GetSize() < 1)		return -1;
	return 1;
}

bool CObj__DUAL_ARM_STD::LLx__Check_Empty__OutSlot()
{
	for(int i=0; i<iLLx_LIMIT; i++)
	{
		if(LLx__Check_Empty__OutSlot(i) > 0)		return true;
	}
	return false;
}

int  CObj__DUAL_ARM_STD::
LLx__Check_Empty__OutSlot(const int ll_index)
{
	if(xEXT_CH__SCH_DB_LLx_USE_FLAG[ll_index]->Check__DATA(STR__ENABLE) < 0)
	{
		return -11;
	}
	if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_index]->Check__DATA(LBx_MODE__ONLY_INPUT) > 0)
	{
		return -12;
	}

	// ...
	int slot_max = iLLx_SLOT_MAX[ll_index];
	int i;

	for(i=0; i<slot_max; i++)
	{
		if(dEXT_CH__SCH_DB_LLx_SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__ENABLE) < 0)		continue;
		if(dEXT_CH__CFG_DB_LLx_SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__ENABLE) < 0)		continue;

		if(xEXT_CH__LLx__SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__NONE) < 0)
		{
			continue;
		}

		if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_index]->Check__DATA(LBx_MODE__ALL) > 0)
		{
			CString ch_data = xEXT_CH__SCH_DB_LLx_SLOT_MODE[ll_index][i]->Get__STRING();

			if((ch_data.CompareNoCase(SLOT_STS__OUTPUT) == 0)
			|| (ch_data.CompareNoCase(SLOT_STS__ALL)    == 0))
			{
				return 11;
			}
		}
		else
		{
			return 12;
		}
	}

	return -1;
}

int  CObj__DUAL_ARM_STD::
LLx__Check_Occupied__OutSlot(const int ll_index)
{
	if(xEXT_CH__SCH_DB_LLx_USE_FLAG[ll_index]->Check__DATA(STR__ENABLE) < 0)
	{
		return -11;
	}
	if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_index]->Check__DATA(LBx_MODE__ONLY_INPUT) > 0)
	{
		return -12;
	}

	// ...
	int slot_max = iLLx_SLOT_MAX[ll_index];
	int i;

	for(i=0; i<slot_max; i++)
	{
		if(dEXT_CH__SCH_DB_LLx_SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__ENABLE) < 0)			continue;
		if(dEXT_CH__CFG_DB_LLx_SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__ENABLE) < 0)			continue;

		if(xEXT_CH__LLx__SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__NONE) > 0)
		{
			continue;
		}

		if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_index]->Check__DATA(LBx_MODE__ALL) > 0)
		{
			CString ch_data = xEXT_CH__SCH_DB_LLx_SLOT_MODE[ll_index][i]->Get__STRING();

			if(ch_data.CompareNoCase(SLOT_STS__OUTPUT) == 0)
			{
				return 11;
			}

			if(ch_data.CompareNoCase(SLOT_STS__ALL) == 0)
			{
				CString sch_name;
				CString ll_name = Get__LLx_NAME(ll_index);

				sch_name.Format("%s-%1d", ll_name,i+1);

				if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(sch_name) < 0)
				{
					return 12;
				}
			}
		}
		else
		{
			return 13;
		}
	}

	return -1;
}

int  CObj__DUAL_ARM_STD::
LLx__Check_Empty__OutSlot(const int ll_index,const int slot_id)
{
	if(xEXT_CH__SCH_DB_LLx_USE_FLAG[ll_index]->Check__DATA(STR__ENABLE) < 0)
	{
		return -11;
	}
	if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_index]->Check__DATA(LBx_MODE__ONLY_INPUT) > 0)
	{
		return -12;
	}

	// ...
	int slot_max = iLLx_SLOT_MAX[ll_index];
	int index = slot_id - 1;

	if(slot_id > slot_max)			return -1;
	if(index   < 0)					return -1;

	if(dEXT_CH__SCH_DB_LLx_SLOT_STATUS[ll_index][index]->Check__DATA(SLOT_STS__ENABLE) < 0)			return -11;
	if(dEXT_CH__CFG_DB_LLx_SLOT_STATUS[ll_index][index]->Check__DATA(SLOT_STS__ENABLE) < 0)			return -12;

	if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_index]->Check__DATA(LBx_MODE__ALL) > 0)
	{
		CString ch_data = xEXT_CH__SCH_DB_LLx_SLOT_MODE[ll_index][index]->Get__STRING();

		if((ch_data.CompareNoCase(SLOT_STS__OUTPUT) != 0)
		&& (ch_data.CompareNoCase(SLOT_STS__ALL)    != 0))
		{
			return -1;
		}
	}

	if(xEXT_CH__LLx__SLOT_STATUS[ll_index][index]->Check__DATA(SLOT_STS__NONE) > 0)
	{
		return 1;
	}
	return -1;
}

int  CObj__DUAL_ARM_STD::
LLx__Get_Empty__OutSlot_With_1_Slot_All_Mode(const int ll_index,int& slot_id)
{
	if(xEXT_CH__SCH_DB_LLx_USE_FLAG[ll_index]->Check__DATA(STR__ENABLE) < 0)
	{
		return -11;
	}
	if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_index]->Check__DATA(LBx_MODE__ALL) < 0)	
	{
		return -12;
	}

	// ...
	int slot_max = iLLx_SLOT_MAX[ll_index];

	for(int i=0; i<slot_max; i++)
	{
		if(dEXT_CH__SCH_DB_LLx_SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__ENABLE) < 0)		continue;
		if(dEXT_CH__CFG_DB_LLx_SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__ENABLE) < 0)		continue;

		if(xEXT_CH__SCH_DB_LLx_SLOT_MODE[ll_index][i]->Check__DATA(SLOT_STS__ALL) < 0)
		{
			continue;
		}

		if(xEXT_CH__LLx__SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__NONE) > 0)
		{
			slot_id = i + 1;
			return 1;
		}
		else
		{
			CString sch_name;
			CString ll_name = Get__LLx_NAME(ll_index);

			sch_name.Format("%s-%1d", ll_name,i+1);

			if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(sch_name) > 0)
			{
				continue;
			}

			slot_id = i + 1;
			return 1;
		}
	}
	
	return -1;
}

int  CObj__DUAL_ARM_STD::
LLx__Get_Empty__OutSlot(const int ll_index,int& slot_id)
{
	if(xEXT_CH__SCH_DB_LLx_USE_FLAG[ll_index]->Check__DATA(STR__ENABLE) < 0)
	{
		return -11;
	}
	if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_index]->Check__DATA(LBx_MODE__ONLY_INPUT) > 0)
	{
		return -12;
	}

	// ...
	int slot_max = iLLx_SLOT_MAX[ll_index];

	for(int i=0; i<slot_max; i++)
	{
		if(dEXT_CH__SCH_DB_LLx_SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__ENABLE) < 0)			continue;
		if(dEXT_CH__CFG_DB_LLx_SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__ENABLE) < 0)			continue;

		if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_index]->Check__DATA(LBx_MODE__ALL) > 0)
		{
			CString ch_data = xEXT_CH__SCH_DB_LLx_SLOT_MODE[ll_index][i]->Get__STRING();

			if((ch_data.CompareNoCase(SLOT_STS__OUTPUT) != 0)
			&& (ch_data.CompareNoCase(SLOT_STS__ALL)    != 0))
			{
				continue;
			}
		}

		if(xEXT_CH__LLx__SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__NONE) > 0)
		{
			slot_id = i + 1;
			return 1;
		}
	}
	return -1;
}

int  CObj__DUAL_ARM_STD::
LLx__Get_Empty__OutSlot(const int ll_index,CUIntArray& l_slotid)
{
	l_slotid.RemoveAll();

	if(xEXT_CH__SCH_DB_LLx_USE_FLAG[ll_index]->Check__DATA(STR__ENABLE) < 0)
	{
		return -11;
	}
	if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_index]->Check__DATA(LBx_MODE__ONLY_INPUT) > 0)
	{
		return -12;
	}

	// ...
	int slot_max = iLLx_SLOT_MAX[ll_index];

	for(int i=0; i<slot_max; i++)
	{
		if(dEXT_CH__SCH_DB_LLx_SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__ENABLE) < 0)			continue;
		if(dEXT_CH__CFG_DB_LLx_SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__ENABLE) < 0)			continue;

		if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_index]->Check__DATA(LBx_MODE__ALL) > 0)
		{
			CString ch_data = xEXT_CH__SCH_DB_LLx_SLOT_MODE[ll_index][i]->Get__STRING();

			if((ch_data.CompareNoCase(SLOT_STS__OUTPUT) != 0)
			&& (ch_data.CompareNoCase(SLOT_STS__ALL)    != 0))
			{
				continue;
			}
		}

		if(xEXT_CH__LLx__SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__NONE) > 0)
		{
			int slot_id = i + 1;
			l_slotid.Add(slot_id);
		}
	}

	return l_slotid.GetSize();
}
int  CObj__DUAL_ARM_STD::
LLx__Get_Empty__OutSlot(CUIntArray& l_ll_index)
{
	l_ll_index.RemoveAll();

	// ...
	int ll_i;

	for(ll_i = 0; ll_i < iLLx_LIMIT; ll_i++)
	{
		if(xEXT_CH__SCH_DB_LLx_USE_FLAG[ll_i]->Check__DATA(STR__ENABLE) < 0)
		{
			continue;
		}
		if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_i]->Check__DATA(LBx_MODE__ONLY_INPUT) > 0)
		{
			continue;
		}

		// ...
		bool active__ll_out = false;

		int slot_max = iLLx_SLOT_MAX[ll_i];
		for(int i=0; i<slot_max; i++)
		{
			if(dEXT_CH__SCH_DB_LLx_SLOT_STATUS[ll_i][i]->Check__DATA(SLOT_STS__ENABLE) < 0)			continue;
			if(dEXT_CH__CFG_DB_LLx_SLOT_STATUS[ll_i][i]->Check__DATA(SLOT_STS__ENABLE) < 0)			continue;

			if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_i]->Check__DATA(LBx_MODE__ALL) > 0)
			{
				CString ch_data = xEXT_CH__SCH_DB_LLx_SLOT_MODE[ll_i][i]->Get__STRING();
	
				if((ch_data.CompareNoCase(SLOT_STS__OUTPUT) != 0)
				&& (ch_data.CompareNoCase(SLOT_STS__ALL)    != 0))
				{
					continue;
				}

				if(xEXT_CH__LLx__SLOT_STATUS[ll_i][i]->Check__DATA(SLOT_STS__NONE) < 0)
				{
					continue;
				}
			}

			active__ll_out = true;
			break;
		}

		if(active__ll_out)
		{
			l_ll_index.Add(ll_i);
		}
	}

	int ll_size = l_ll_index.GetSize();
	if(ll_size > 0)				return ll_size;

	return -1;
}

int  CObj__DUAL_ARM_STD::
LLx__Get_Occupied__OutSlot(const int ll_index,int& slot_id)
{
	if(xEXT_CH__SCH_DB_LLx_USE_FLAG[ll_index]->Check__DATA(STR__ENABLE) < 0)
	{
		return -11;
	}
	if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_index]->Check__DATA(LBx_MODE__ONLY_INPUT) > 0)
	{
		return -12;
	}

	// ...
	int slot_max = iLLx_SLOT_MAX[ll_index];
	int i;

	for(i=0; i<slot_max; i++)
	{
		if(dEXT_CH__SCH_DB_LLx_SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__ENABLE) < 0)			continue;
		if(dEXT_CH__CFG_DB_LLx_SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__ENABLE) < 0)			continue;

		if(xEXT_CH__LLx__SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__NONE) > 0)
		{
			continue;
		}

		if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_index]->Check__DATA(LBx_MODE__ALL) > 0)
		{
			CString ch_data = xEXT_CH__SCH_DB_LLx_SLOT_MODE[ll_index][i]->Get__STRING();

			if(ch_data.CompareNoCase(SLOT_STS__OUTPUT) == 0)
			{
				return 11;
			}

			if(ch_data.CompareNoCase(SLOT_STS__ALL) == 0)
			{
				CString sch_name;
				CString ll_name = Get__LLx_NAME(ll_index);

				sch_name.Format("%s-%1d", ll_name,i+1);

				if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(sch_name) < 0)
				{
					return 12;
				}
			}
		}
		else
		{
			slot_id = i + 1;
			return 13;
		}
	}

	return -1;
}
int  CObj__DUAL_ARM_STD::
LLx__Get_Occupied__OutSlot(const int ll_index,CUIntArray& l_slot_id)
{
	if(xEXT_CH__SCH_DB_LLx_USE_FLAG[ll_index]->Check__DATA(STR__ENABLE) < 0)
	{
		return -11;
	}
	if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_index]->Check__DATA(LBx_MODE__ONLY_INPUT) > 0)
	{
		return -12;
	}

	l_slot_id.RemoveAll();

	// ...
	int slot_max = iLLx_SLOT_MAX[ll_index];
	int i;

	for(i=0; i<slot_max; i++)
	{
		if(dEXT_CH__SCH_DB_LLx_SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__ENABLE) < 0)			continue;
		if(dEXT_CH__CFG_DB_LLx_SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__ENABLE) < 0)			continue;

		if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_index]->Check__DATA(LBx_MODE__ALL) > 0)
		{
			CString ch_data = xEXT_CH__SCH_DB_LLx_SLOT_MODE[ll_index][i]->Get__STRING();

			if(ch_data.CompareNoCase(SLOT_STS__OUTPUT) != 0)
			{
				if(ch_data.CompareNoCase(SLOT_STS__ALL) == 0)
				{
					CString sch_name;
					CString ll_name = Get__LLx_NAME(ll_index);

					sch_name.Format("%s-%1d", ll_name,i+1);

					if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(sch_name) > 0)
					{
						continue;
					}
				}
				else
				{
					continue;
				}
			}
		}

		if(xEXT_CH__LLx__SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__NONE) < 0)
		{
			int slot_id = i + 1;
			l_slot_id.Add(slot_id);
		}
	}

	return 1;
}


//---------------------------------------------------------------------------------------
int  CObj__DUAL_ARM_STD::PMx__Check_Empty__SlotStatus(const int pm_i)
{
	if(pm_i < 0)			return -1;
	if(pm_i >= iPMx_SIZE)	return -1;

	if(xCH__PMx__SLOT_STATUS[pm_i]->Check__DATA(SLOT_STS__NONE) > 0)
	{
		return 1;
	}
	return -1;
}
int  CObj__DUAL_ARM_STD::PMx__Check_Occupied__SlotStatus(const int pm_i)
{
	if(pm_i < 0)			return -1;
	if(pm_i >= iPMx_SIZE)	return -1;

	if(xCH__PMx__SLOT_STATUS[pm_i]->Check__DATA(SLOT_STS__NONE) > 0)
	{
		return -1;
	}
	return 1;
}

int  CObj__DUAL_ARM_STD::
PMx__Check_Next_Process(const int pm_i,
						CII_OBJECT__ALARM* p_alarm)
{
	if(pm_i < 0)			return -1;
	if(pm_i >= iPMx_SIZE)	return -1;

	if(xCH__PMx__NEXT_PROCESS[pm_i]->Check__DATA("") > 0)
	{
		return -1;
	}

	// ...
	CString next_prc;
	xCH__PMx__NEXT_PROCESS[pm_i]->Get__DATA(next_prc);

	if(next_prc.CompareNoCase("LOCK") == 0)	
	{
		return -1;
	}
	if(next_prc.Find("CLEAN") >= 0)
	{
		if(xCH_CFG__CLEAN_MATERIAL_FLAG->Check__DATA("EXIST") > 0)
		{
			int err_flag = -1;
			int ptn;

			if(err_flag < 0)
			{
				if(xSCH_MATERIAL_CTRL->Get__DUMMY_PORT(ptn) < 0)
				{
					err_flag = 1;
				}
				else
				{
					err_flag = 1;

					int lp_index = ptn - 1;
					if((lp_index >= 0)
					&& (lp_index <  iLPx_SIZE))
					{
						if((xCH__LPx__OBJ_STATUS[lp_index]->Check__DATA("STANDBY")  > 0)
						|| (xCH__LPx__OBJ_STATUS[lp_index]->Check__DATA("CTCINUSE") > 0))
						{
							if(xCH__LPx__CST_STATUS[lp_index]->Check__DATA("EXIST") > 0)
							{
								err_flag = -1;
							}
						}
					}
				}
			}

			// ...
			CUIntArray l_slot_sts;
			int stn;

			if(err_flag < 0)
			{
				xSCH_MATERIAL_CTRL->Get__PORT_SLOT_STS(ptn,l_slot_sts);

				IDS__SCH_MATERIAL_INFO ds_info;

				int limit = l_slot_sts.GetSize();
				int i;

				err_flag = 1;

				for(i=0;i<limit;i++)
				{
					if((int) l_slot_sts[i] < 1)
					{
						continue;
					}

					stn = i + 1;
					if(xSCH_MATERIAL_CTRL->Get__MATERIAL_INFO(ptn,stn,ds_info) > 0)
					{
						continue;
					}

					err_flag = -1;
					break;
				}
			}

			if(err_flag > 0)
			{
				// Alarm Post
				CString r_act;

				p_alarm->Check__ALARM(ALID__CLEAN_ERROR__DUMMY_PORT__NOT_EXIST,r_act);
				p_alarm->Post__ALARM(ALID__CLEAN_ERROR__DUMMY_PORT__NOT_EXIST);
				return -1;
			}

			if(xCH__PMx__NEXT_PROCESS[pm_i]->Check__DATA("LOCK") < 0)
			{
				xCH__PMx__NEXT_PROCESS[pm_i]->Set__DATA("LOCK");

				// Dummy Port -> Scheduler DB 
				IDS__SCH_DB_INFO db_info;

				db_info.iSRC__PTN = ptn;
				db_info.iSRC__STN = stn;
				db_info.iTRG__PTN = ptn;
				db_info.iTRG__STN = stn;

				db_info.sJOB_NAME = "CLEAN";
				db_info.sLOT_ID   = "CLEAN";
				db_info.sMATERIAL_ID.Format("%1d",stn);
				db_info.sPPID     = "CLEAN";
				db_info.sMATERIAL_TYPE = "";

				db_info.iVISION_VISIT = -1;
				db_info.sVISION_RCP = "";

				db_info.sOCR_SCRIBE = "";

				CString pm_name;
				pm_name.Format("PM%1d",pm_i+1);

				db_info.sLIST_PRC_MODULE.Add(pm_name);
				db_info.sLIST_PRC_RECIPE.Add("");

				xSCH_MATERIAL_CTRL->Load__SCH_INFO_At_First_SCH_DB(db_info);
			}
			return -1;
		}
	}

	return 1;
}
CString CObj__DUAL_ARM_STD::
PMx__Get_Next_Process(const int pm_i)
{
	if(pm_i < 0)			return "";
	if(pm_i >= iPMx_SIZE)	return "";

	CString next_prc;
	xCH__PMx__NEXT_PROCESS[pm_i]->Get__DATA(next_prc);

	return next_prc;
}


//---------------------------------------------------------------------------------------
void CObj__DUAL_ARM_STD
::Fnc__LOG_CTRL(const CString& log_msg)
{
	if(iLOG_CTRL_FLAG < 0)
	{
		return;
	}

	xAPP_LOG_CTRL->WRITE__LOG(log_msg);
}


//---------------------------------------------------------------------------------------
void CObj__DUAL_ARM_STD
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

	if(arm_type.CompareNoCase(_ARM__AB) == 0)
	{
		CString ch_data;

		int ll_i = Get__LLx_INDEX(stn_name);
		int pm_i = Macro__Get_PMC_INDEX(stn_name);

		if(ll_i >= 0)
		{
			l__arm_type.Add(_ARM__A);
			l__stn_name.Add(stn_name);
			l__stn_slot.Add("1");

			l__arm_type.Add(_ARM__B);
			l__stn_name.Add(stn_name);
			l__stn_slot.Add("2");
		}
		else if(pm_i >= 0)
		{
			int pm_id = ((pm_i / 2) * 2) + 1;
			int id_left  = pm_id;
			int id_right = pm_id + 1;

			l__arm_type.Add(_ARM__A);
			ch_data.Format("PM%1d", id_left);
			l__stn_name.Add(ch_data);
			l__stn_slot.Add("1");

			l__arm_type.Add(_ARM__B);
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
