#include "StdAfx.h"
#include "CObj__DUAL_ARM_STD.h"

#include "Macro_Function.h"


// ...
MODULE__IS_AVAILABLE(CObj__DUAL_ARM_STD, ATM_RB, xCH__ATM_RB__OBJ_STATUS,xCH__ATM_RB__OBJ_CTRL);
MODULE__IS_AVAILABLE(CObj__DUAL_ARM_STD, AL1, xCH__AL1__OBJ_STATUS,xCH__AL1__OBJ_CTRL);

MODULEx__IS_AVAILABLE_WITH_USE_INDEX(CObj__DUAL_ARM_STD, LLx, xEXT_CH__LLx__OBJ_STATUS,xEXT_CH__LLx__OBJ_CTRL,xEXT_CH__SCH_DB_LLx_USE_FLAG);
MODULEx__IS_AVAILABLE(CObj__DUAL_ARM_STD,LPx,xCH__LPx__OBJ_STATUS,xCH__LPx__OBJ_CTRL);

// ...
MODULE__SET_OBJ_STATUS(CObj__DUAL_ARM_STD,ATM_RB,xCH__ATM_RB__OBJ_STATUS);
MODULE__SET_OBJ_STATUS(CObj__DUAL_ARM_STD,AL1, xCH__AL1__OBJ_STATUS);

MODULEx__SET_OBJ_STATUS_WITH_INDEX(CObj__DUAL_ARM_STD, LLx, xEXT_CH__LLx__OBJ_STATUS);
MODULEx__SET_OBJ_STATUS(CObj__DUAL_ARM_STD, LPx, xCH__LPx__OBJ_STATUS);

// ...
MODULEx__IS_ATM_WITH_INDEX(CObj__DUAL_ARM_STD, LLx, xEXT_CH__LLx__PRESSURE_STATUS);
MODULEx__IS_VAC_WITH_INDEX(CObj__DUAL_ARM_STD, LLx, xEXT_CH__LLx__PRESSURE_STATUS);

// ...
MODULEx__IS_READY(CObj__DUAL_ARM_STD, PMx, sCH__PMx__OBJ_VIRTUAL_STATUS, dEXT_CH__CFG_PMx_USE, sCH__PMx__PROC_INFO_PROCESS_STATUS);


//---------------------------------------------------------------------------------------
int  CObj__DUAL_ARM_STD:: 
LPx__Check_Occupied__Slot_Status(const int lp_id,const int slot_id)
{
	//.....
	int lp_i = lp_id - 1;

	if(lp_i <  0)						return -1;
	if(lp_i >= CFG_LP_LIMIT)			return -1;

	//.....
	int slot_i = slot_id - 1;

	if(slot_i <  0)						return -1;
	if(slot_i >= CFG_LP__SLOT_MAX)		return -1;

	//.....
	{
		if(xCH__LPx__PORT_STATUS[lp_i]->Check__DATA("BUSY") < 0)			return -11;
		if(xCH__LPx__CST_STATUS[lp_i]->Check__DATA("EXIST") < 0)			return -12;
		if(xCH__LPx__SLOT_STATUS[lp_i][slot_i]->Check__DATA("NONE") > 0)	return -13;
	}

	return 1;
}
int  CObj__DUAL_ARM_STD:: 
LPx__Check_Empty__Slot_Status(const int lp_id,const int slot_id)
{
	int lp_i = lp_id - 1;

	if(lp_i <  0)						return -1;
	if(lp_i >= CFG_LP_LIMIT)			return -1;

	// ...
	int slot_i = slot_id - 1;

	if(slot_i <  0)						return -1;
	if(slot_i >= CFG_LP__SLOT_MAX)		return -1;

	// ...
	{
		CString str__lp_sts = xCH__LPx__PORT_STATUS[lp_i]->Get__STRING();

		if((str__lp_sts.CompareNoCase("BUSY")   != 0)
			&& (str__lp_sts.CompareNoCase("PAUSED") != 0))
		{
			return -11;
		}

		if(xCH__LPx__CST_STATUS[lp_i]->Check__DATA("EXIST") < 0)				return -21;
		if(xCH__LPx__SLOT_STATUS[lp_i][slot_i]->Check__DATA("NONE") < 0)		return -22;
	}

	return 1;
}
int  CObj__DUAL_ARM_STD::
LPx__Check_Not_Processed_Wafer(const int lp_id)
{
	int lp_index = lp_id - 1;

	if(lp_index <  0)					return -1;
	if(lp_index >= iLPx_SIZE)			return -2;

	if(xCH__LPx__CST_STATUS[lp_index]->Check__DATA(STR__EXIST) < 0)
	{
		return -3;
	}

	// ...
	int slot_max = (int) xCH__LPx__CFG_SLOT_MAX[lp_index]->Get__VALUE();

	for(int i=0; i<slot_max; i++)
	{
		CString str__wfr_sts = xCH__LPx__SLOT_STATUS[lp_index][i]->Get__STRING();

		if(str__wfr_sts.CompareNoCase(STR__EXIST)  == 0)			return 1;
		if(str__wfr_sts.CompareNoCase(STR__MAPPED) == 0)			return 2;
	}

	return -1;
}

int  CObj__DUAL_ARM_STD::
Fnc__GET_DUMMY_PORT()
{
	int i;

	for(i=0;i<iLPx_SIZE;i++)
	{
		if(xCH__LPx__CFG_USE_TYPE[i]->Check__DATA(STR__DUMMY) < 0)
		{
			continue;
		}

		return (i + 1);
	}
	return -1;
}


//---------------------------------------------------------------------------------------
int  CObj__DUAL_ARM_STD::
LLx__Check_Venting(const int ll_index)
{
	CString obj_ctrl;
	CString obj_mode;
	CString obj_sts;

	xEXT_CH__LLx__OBJ_CTRL[ll_index]->Get__DATA(obj_ctrl);
	xEXT_CH__LLx__OBJ_MODE[ll_index]->Get__DATA(obj_mode);
	xEXT_CH__LLx__OBJ_STATUS[ll_index]->Get__DATA(obj_sts);

	if((obj_sts.CompareNoCase("STANDBY")  != 0)
		&& (obj_sts.CompareNoCase("CTCINUSE") != 0))
	{
		return -1;
	}
	if(obj_ctrl.CompareNoCase("RUN") != 0)
	{
		return -1;
	}
	if(obj_mode.CompareNoCase("VENT") != 0)
	{
		return -1;
	}

	return 1;
}

// ...
CString CObj__DUAL_ARM_STD::
Get__LLx_NAME(const int ll_index)
{
	if(ll_index == _LLx_INDEX__LBA)			return MODULE__LBA;
	if(ll_index == _LLx_INDEX__LBB)			return MODULE__LBB;
	if(ll_index == _LLx_INDEX__LBC)			return MODULE__LBC;
	if(ll_index == _LLx_INDEX__LBD)			return MODULE__LBD;

	return MODULE__LBX;
}
int CObj__DUAL_ARM_STD::
Get__LLx_INDEX(const CString& ll_name)
{
	int i_limit = iLLx_SIZE;
	int i;

	for(i=0; i<i_limit; i++)
	{
		CString str_name = Get__LLx_NAME(i);

		if(ll_name.Find(str_name) == 0)
			return i;
	}
	return -1;
}

void CObj__DUAL_ARM_STD::
LLx__Update_PRESSURE(const int ll_index)
{
	CString var_data;

	xEXT_CH__CFG__REF_ATM_PRESSURE->Get__DATA(var_data);
	double ref_atm_press = atof(var_data);

	xEXT_CH__CFG__REF_VAC_PRESSURE->Get__DATA(var_data);
	double ref_vac_press = atof(var_data);

	xEXT_CH__LLx__PRESSURE_VALUE[ll_index]->Get__DATA(var_data);
	double cur_press = atof(var_data);

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


//---------------------------------------------------------------------------------------
int  CObj__DUAL_ARM_STD::
Set__TMC_CtcInUse_To_Standby()
{
	int i;

	// ...
	{
		ATM_RB__Set_CtcInUse_To_Standby();
		AL1__Set_CtcInUse_To_Standby();
	}

	// LLx ...
	{
		for(i=0; i<iLLx_SIZE; i++)
		{
			LLx__Set_CtcInUse_To_Standby(i);
		}
	}

	// LPx ...
	{
		for(i=0; i<iLPx_SIZE; i++)
		{
			LPx__Set_CtcInUse_To_Standby(i);
		}
	}

	return 1;
}

//---------------------------------------------------------------------------------------
int  CObj__DUAL_ARM_STD::
AL1__Check_Empty__All_Slot()
{
	if(xEXT_CH__SCH_DB_AL1_SLOT01_USE_FLAG->Check__DATA(STR__ENABLE) > 0)
	{
		if(xCH__AL1__SLOT01_STATUS->Check__DATA(SLOT_STS__NONE) < 0)
		{
			return -1;
		}
	}
	if(xEXT_CH__SCH_DB_AL1_SLOT02_USE_FLAG->Check__DATA(STR__ENABLE) > 0)
	{
		if(xCH__AL1__SLOT02_STATUS->Check__DATA(SLOT_STS__NONE) < 0)
		{
			return -1;
		}
	}

	return 1;
}

int  CObj__DUAL_ARM_STD::
AL1__Get_Empty__SlotID(int& slot_id)
{
	if(xEXT_CH__SCH_DB_AL1_SLOT01_USE_FLAG->Check__DATA(STR__ENABLE) > 0)
	{
		if(xCH__AL1__SLOT01_STATUS->Check__DATA(SLOT_STS__NONE) > 0)
		{
			slot_id = 1;
			return 1;
		}
	}
	if(xEXT_CH__SCH_DB_AL1_SLOT02_USE_FLAG->Check__DATA(STR__ENABLE) > 0)
	{
		if(xCH__AL1__SLOT02_STATUS->Check__DATA(SLOT_STS__NONE) > 0)
		{
			slot_id = 2;
			return 1;
		}
	}

	return -1;
}
int  CObj__DUAL_ARM_STD::
AL1__Get_Occupied__SlotID(int& slot_id)
{
	if(xEXT_CH__SCH_DB_AL1_SLOT01_USE_FLAG->Check__DATA(STR__ENABLE) > 0)
	{
		if(xCH__AL1__SLOT01_STATUS->Check__DATA(SLOT_STS__NONE) < 0)
		{
			slot_id = 1;
			return 1;
		}
	}
	if(xEXT_CH__SCH_DB_AL1_SLOT02_USE_FLAG->Check__DATA(STR__ENABLE) > 0)
	{
		if(xCH__AL1__SLOT02_STATUS->Check__DATA(SLOT_STS__NONE) < 0)
		{
			slot_id = 2;
			return 1;
		}
	}

	return -1;
}
int  CObj__DUAL_ARM_STD:: 
AL1__Get_Occupied__Total_Count()
{
	int count = 0;

	if(xEXT_CH__SCH_DB_AL1_SLOT01_USE_FLAG->Check__DATA(STR__ENABLE) > 0)
	{
		if(xCH__AL1__SLOT01_STATUS->Check__DATA(SLOT_STS__NONE) < 0)
		{
			count++;
		}
	}
	if(xEXT_CH__SCH_DB_AL1_SLOT02_USE_FLAG->Check__DATA(STR__ENABLE) > 0)
	{
		if(xCH__AL1__SLOT02_STATUS->Check__DATA(SLOT_STS__NONE) < 0)
		{
			count++;
		}
	}

	return count;
}

int  CObj__DUAL_ARM_STD::
AL1__Check_Occupied__SlotStatus(const int slot_id)
{
	switch(slot_id)
	{
	case 1:
		if(xEXT_CH__SCH_DB_AL1_SLOT01_USE_FLAG->Check__DATA(STR__ENABLE) > 0)
		{
			if(xCH__AL1__SLOT01_STATUS->Check__DATA(SLOT_STS__NONE) < 0)
			{
				return 1;
			}
		}
		return -1;

	case 2:
		if(xEXT_CH__SCH_DB_AL1_SLOT02_USE_FLAG->Check__DATA(STR__ENABLE) > 0)
		{
			if(xCH__AL1__SLOT02_STATUS->Check__DATA(SLOT_STS__NONE) < 0)
			{
				return 1;
			}
		}
		return -1;
	}

	return -1;
}
int  CObj__DUAL_ARM_STD::
AL1__Check_Occupied__SlotStatus(const int slot_id,CString& title)
{
	switch(slot_id)
	{
	case 1:
		if(xEXT_CH__SCH_DB_AL1_SLOT01_USE_FLAG->Check__DATA(STR__ENABLE) > 0)
		{
			if(xCH__AL1__SLOT01_STATUS->Check__DATA(SLOT_STS__NONE) < 0)
			{
				xCH__AL1__SLOT01_TITLE->Get__DATA(title);
				return 1;
			}
		}
		return -1;

	case 2:
		if(xEXT_CH__SCH_DB_AL1_SLOT02_USE_FLAG->Check__DATA(STR__ENABLE) > 0)
		{
			if(xCH__AL1__SLOT02_STATUS->Check__DATA(SLOT_STS__NONE) < 0)
			{
				xCH__AL1__SLOT02_TITLE->Get__DATA(title);
				return 1;
			}
		}
		return -1;
	}

	return -1;
}


//---------------------------------------------------------------------------------------
int  CObj__DUAL_ARM_STD::
ATM_RB__Get_Cfg_Arm_Count()
{
	int arm_count  = 0;

	if(xCH__ATM_RB__CFG_A_ARM_USE_MODE->Check__DATA(STR__ENABLE) > 0)			arm_count++;
	if(xCH__ATM_RB__CFG_B_ARM_USE_MODE->Check__DATA(STR__ENABLE) > 0)			arm_count++;

	return arm_count;
}

int  CObj__DUAL_ARM_STD::
ATM_RB__Get_Empty_Arm_Count()
{
	int arm_count  = 0;

	if(xCH__ATM_RB__CFG_A_ARM_USE_MODE->Check__DATA(STR__ENABLE) > 0)
	{
		if(xCH__ATM_RB__SLOT01_STATUS->Check__DATA(SLOT_STS__NONE) > 0)			arm_count++;
	}
	if(xCH__ATM_RB__CFG_B_ARM_USE_MODE->Check__DATA(STR__ENABLE) > 0)
	{
		if(xCH__ATM_RB__SLOT02_STATUS->Check__DATA(SLOT_STS__NONE) > 0)			arm_count++;
	}

	return arm_count;
}
int  CObj__DUAL_ARM_STD::
ATM_RB__Get_Occupied_Arm_Count()
{
	int arm_count  = 0;

	if(xCH__ATM_RB__CFG_A_ARM_USE_MODE->Check__DATA(STR__ENABLE) > 0)
	{
		if(xCH__ATM_RB__SLOT01_STATUS->Check__DATA(SLOT_STS__NONE) < 0)			arm_count++;
	}
	if(xCH__ATM_RB__CFG_B_ARM_USE_MODE->Check__DATA(STR__ENABLE) > 0)
	{
		if(xCH__ATM_RB__SLOT02_STATUS->Check__DATA(SLOT_STS__NONE) < 0)			arm_count++;
	}

	return arm_count;
}

int  CObj__DUAL_ARM_STD::
ATM_RB__Check_Empty__Arm_Type()
{
	if(xCH__ATM_RB__CFG_A_ARM_USE_MODE->Check__DATA(STR__ENABLE) > 0)
	{
		if(xCH__ATM_RB__SLOT01_STATUS->Check__DATA(SLOT_STS__NONE) > 0)			return 1;
	}
	if(xCH__ATM_RB__CFG_B_ARM_USE_MODE->Check__DATA(STR__ENABLE) > 0)
	{
		if(xCH__ATM_RB__SLOT02_STATUS->Check__DATA(SLOT_STS__NONE) > 0)			return 1;
	}

	return -1;
}

int  CObj__DUAL_ARM_STD::
ATM_RB__Check_Process_Wafer_On_Arm()
{
	int count__prc_count = ATM_RB__Get_Process_Wafer_Count_On_Arm();
	if(count__prc_count > 0)		return 1;

	return -1;
}
int  CObj__DUAL_ARM_STD::
ATM_RB__Get_Process_Wafer_Count_On_Arm()
{
	CStringArray l__arm_type;

	if(ATM_RB__Check_Occupied__A_Arm() > 0)
	{
		l__arm_type.Add(ARM_A);
	}
	if(ATM_RB__Check_Occupied__B_Arm() > 0)
	{
		l__arm_type.Add(ARM_B);
	}

	// ...
	int count__proc_wfr = 0;

	int k_limit = l__arm_type.GetSize();
	for(int k=0; k<k_limit; k++)
	{
		CString arm_type = l__arm_type[k];

		if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(arm_type) > 0)
		{
			count__proc_wfr++;
		}
	}
	return count__proc_wfr;
}

int  CObj__DUAL_ARM_STD::
ATM_RB__Check_Occupied__Arm_Type()
{
	if(ATM_RB__Check_Occupied__A_Arm() > 0)			return 1;
	if(ATM_RB__Check_Occupied__B_Arm() > 0)			return 1;

	return -1;
}
int  CObj__DUAL_ARM_STD::
ATM_RB__Check_Occupied__Arm_Type(const CString& arm_type)
{
	if(arm_type.CompareNoCase(ARM_A) == 0)
	{
		return ATM_RB__Check_Occupied__A_Arm();
	}
	if(arm_type.CompareNoCase(ARM_B) == 0)
	{
		return ATM_RB__Check_Occupied__B_Arm();
	}

	return -1;
}
int  CObj__DUAL_ARM_STD::
ATM_RB__Check_Occupied__A_Arm()
{
	if(xCH__ATM_RB__CFG_A_ARM_USE_MODE->Check__DATA(STR__ENABLE) < 0)		return -11;
	if(xCH__ATM_RB__SLOT01_STATUS->Check__DATA(SLOT_STS__NONE)   < 0)		return 1;

	return -1;
}
int  CObj__DUAL_ARM_STD::
ATM_RB__Check_Occupied__B_Arm()
{
	if(xCH__ATM_RB__CFG_B_ARM_USE_MODE->Check__DATA(STR__ENABLE) < 0)		return -11;
	if(xCH__ATM_RB__SLOT02_STATUS->Check__DATA(SLOT_STS__NONE)   < 0)		return 1;

	return -1;
}

int  CObj__DUAL_ARM_STD::
ATM_RB__Check_Empty__Arm_Type(const CString& arm_type)
{
	if(xCH__ATM_RB__CFG_A_ARM_USE_MODE->Check__DATA(STR__ENABLE) > 0)
	{
		if(xCH__ATM_RB__SLOT01_STATUS->Check__DATA(SLOT_STS__NONE) > 0)
		{
			return 1;
		}
	}
	if(xCH__ATM_RB__CFG_B_ARM_USE_MODE->Check__DATA(STR__ENABLE) > 0)
	{
		if(xCH__ATM_RB__SLOT02_STATUS->Check__DATA(SLOT_STS__NONE) > 0)
		{
			return 1;
		}
	}

	return -1;
}
int  CObj__DUAL_ARM_STD::
ATM_RB__Get_Empty__Arm_Type(CString& arm_type)
{
	if(xCH__ATM_RB__CFG_A_ARM_USE_MODE->Check__DATA(STR__ENABLE) > 0)
	{
		if(xCH__ATM_RB__SLOT01_STATUS->Check__DATA(SLOT_STS__NONE) > 0)
		{
			arm_type = "A";
			return 1;
		}
	}
	if(xCH__ATM_RB__CFG_B_ARM_USE_MODE->Check__DATA(STR__ENABLE) > 0)
	{
		if(xCH__ATM_RB__SLOT02_STATUS->Check__DATA(SLOT_STS__NONE) > 0)
		{
			arm_type = "B";
			return 1;
		}
	}

	return -1;
}

int  CObj__DUAL_ARM_STD::
ATM_RB__Get_Occupied__Arm_Type(CStringArray& l_arm_type)
{
	if(xCH__ATM_RB__CFG_A_ARM_USE_MODE->Check__DATA(STR__ENABLE) > 0)
	{
		if(xCH__ATM_RB__SLOT01_STATUS->Check__DATA(SLOT_STS__NONE) < 0)
		{
			l_arm_type.Add(STR__A);
		}
	}
	if(xCH__ATM_RB__CFG_B_ARM_USE_MODE->Check__DATA(STR__ENABLE) > 0)
	{
		if(xCH__ATM_RB__SLOT02_STATUS->Check__DATA(SLOT_STS__NONE) < 0)
		{
			l_arm_type.Add(STR__B);
		}
	}

	return 1;
}
int  CObj__DUAL_ARM_STD::
ATM_RB__Get_Occupied__Arm_Type(CString& arm_type)
{
	if(xCH__ATM_RB__CFG_A_ARM_USE_MODE->Check__DATA(STR__ENABLE) > 0)
	{
		if(xCH__ATM_RB__SLOT01_STATUS->Check__DATA(SLOT_STS__NONE) < 0)
		{
			arm_type = STR__A;
			return 1;
		}
	}
	if(xCH__ATM_RB__CFG_B_ARM_USE_MODE->Check__DATA(STR__ENABLE) > 0)
	{
		if(xCH__ATM_RB__SLOT02_STATUS->Check__DATA(SLOT_STS__NONE) < 0)
		{
			arm_type = STR__B;
			return 1;
		}
	}

	return -1;
}
int  CObj__DUAL_ARM_STD::
ATM_RB__Get_Occupied__Arm_Type(const int arm_index, CString& arm_type)
{
	if(arm_index == 0)
	{
		if(xCH__ATM_RB__CFG_A_ARM_USE_MODE->Check__DATA(STR__ENABLE) > 0)
		{
			if(xCH__ATM_RB__SLOT01_STATUS->Check__DATA(SLOT_STS__NONE) < 0)
			{
				arm_type = STR__A;
				return 1;
			}
		}
	}
	else if(arm_index == 1)
	{
		if(xCH__ATM_RB__CFG_B_ARM_USE_MODE->Check__DATA(STR__ENABLE) > 0)
		{
			if(xCH__ATM_RB__SLOT02_STATUS->Check__DATA(SLOT_STS__NONE) < 0)
			{
				arm_type = STR__B;
				return 1;
			}
		}
	}

	return -1;
}
int  CObj__DUAL_ARM_STD::
ATM_RB__Get_Occupied__Wfr_Title_Of_Arm_Type(const CString& arm_type, CString& wfr_title)
{
	if(arm_type.CompareNoCase(STR__A) == 0)
	{
		if(xCH__ATM_RB__CFG_A_ARM_USE_MODE->Check__DATA(STR__ENABLE) > 0)
		{
			if(xCH__ATM_RB__SLOT01_STATUS->Check__DATA(SLOT_STS__NONE) > 0)		return -1;

			xCH__ATM_RB__SLOT01_TITLE->Get__DATA(wfr_title);
			return 1;
		}
	}
	else if(arm_type.CompareNoCase(STR__B) == 0)
	{
		if(xCH__ATM_RB__CFG_B_ARM_USE_MODE->Check__DATA(STR__ENABLE) > 0)
		{
			if(xCH__ATM_RB__SLOT02_STATUS->Check__DATA(SLOT_STS__NONE) > 0)		return -1;

			xCH__ATM_RB__SLOT02_TITLE->Get__DATA(wfr_title);
			return 1;
		}
	}

	return -1;
}
int  CObj__DUAL_ARM_STD::
ATM_RB__Check_Occupied__Wfr_Title_Of_Arm_Type(CString& wfr_title)
{
	if(xCH__ATM_RB__CFG_A_ARM_USE_MODE->Check__DATA(STR__ENABLE) > 0)
	{
		if(xCH__ATM_RB__SLOT01_STATUS->Check__DATA(SLOT_STS__NONE) > 0)		return -1;

		xCH__ATM_RB__SLOT01_TITLE->Get__DATA(wfr_title);
		return 1;
	}
	if(xCH__ATM_RB__CFG_B_ARM_USE_MODE->Check__DATA(STR__ENABLE) > 0)
	{
		if(xCH__ATM_RB__SLOT02_STATUS->Check__DATA(SLOT_STS__NONE) > 0)		return -1;

		xCH__ATM_RB__SLOT02_TITLE->Get__DATA(wfr_title);
		return 1;
	}

	return -1;
}

//---------------------------------------------------------------------------------------
int  CObj__DUAL_ARM_STD::
LLx__Check_All_Empty(const int ll_index)
{
	int slot_max = iLLx_SLOT_MAX[ll_index];
	int i;

	for(i=0; i<slot_max; i++)
	{
		if((dEXT_CH__SCH_DB_LLx_SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__ENABLE) < 0)
			|| (dEXT_CH__CFG_DB_LLx_SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__ENABLE) < 0))
		{
			continue;
		}

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
LLx__Check__Slot_Enable(const int ll_index,const int slot_id)
{
	int slot_max = iLLx_SLOT_MAX[ll_index];
	int slot_index = slot_id - 1;

	if(slot_index <  0)					return -1;
	if(slot_index >= slot_max)			return -1;

	if((dEXT_CH__SCH_DB_LLx_SLOT_STATUS[ll_index][slot_index]->Check__DATA(SLOT_STS__ENABLE) < 0)
		|| (dEXT_CH__CFG_DB_LLx_SLOT_STATUS[ll_index][slot_index]->Check__DATA(SLOT_STS__ENABLE) < 0))
	{
		return -1;
	}
	return 1;
}

int  CObj__DUAL_ARM_STD::
LLx__Get_Empty__Total_InSlot()
{
	int in_total = 0;

	for(int ll_i=0; ll_i < iLLx_SIZE; ll_i++)
	{
		int in_count = LLx__Get_Empty__Total_InSlot(ll_i);
		if(in_count < 1)			continue;

		in_total += in_count;
	}
	return in_total;
}
int  CObj__DUAL_ARM_STD::
LLx__Get_Empty__Total_InSlot(const int ll_index)
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
	int count = 0;

	int slot_max = iLLx_SLOT_MAX[ll_index];
	int i;

	for(i=0; i<slot_max; i++)
	{
		if((dEXT_CH__SCH_DB_LLx_SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__ENABLE) < 0)
			|| (dEXT_CH__CFG_DB_LLx_SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__ENABLE) < 0))
		{
			continue;
		}

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
			count++;
		}
	}
	return count;
}

int  CObj__DUAL_ARM_STD::
LLx__Get_Empty__InSlot(const int ll_index,int& slot_id)
{
	return _LLx__Get_Empty__InSlot(ll_index, 0, 1, slot_id);
}
bool CObj__DUAL_ARM_STD::
LLx__Check_Empty__InSlot_Of_Odd_Type()
{
	int slot_id;

	for(int ll_i=0; ll_i < iLLx_SIZE; ll_i++)
	{
		if(LLx__Get_Empty__InSlot_Of_Odd_Type(ll_i, slot_id) < 0)
		{
			continue;
		}

		return true;
	}

	return false;
}
int  CObj__DUAL_ARM_STD::
LLx__Get_Empty__InSlot_Of_Odd_Type(const int ll_index, int& slot_id)
{
	int db_start = 0;

	if(dCH__ATM_RB__CFG_LL_CONSTRAINT_1->Check__DATA(STR__EVEN) > 0)
	{
		if(dCH__ATM_RB__CFG_LL_CONSTRAINT_2->Check__DATA(STR__ODD) > 0)
		{
			db_start = 1;
		}
	}

	return _LLx__Get_Empty__InSlot(ll_index, db_start, 2, slot_id);
}
bool CObj__DUAL_ARM_STD::
LLx__Check_Empty__InSlot_Of_Even_Type()
{
	int slot_id;

	for(int ll_i=0; ll_i < iLLx_SIZE; ll_i++)
	{
		if(LLx__Get_Empty__InSlot_Of_Even_Type(ll_i, slot_id) < 0)
		{
			continue;
		}

		return true;
	}

	return false;
}
int  CObj__DUAL_ARM_STD::
LLx__Get_Empty__InSlot_Of_Even_Type(const int ll_index, int& slot_id)
{
	int db_start = 1;

	if(dCH__ATM_RB__CFG_LL_CONSTRAINT_2->Check__DATA(STR__ODD) > 0)
	{
		if(dCH__ATM_RB__CFG_LL_CONSTRAINT_1->Check__DATA(STR__EVEN) > 0)
		{
			db_start = 0;
		}
	}

	return _LLx__Get_Empty__InSlot(ll_index, db_start, 2, slot_id);
}
int  CObj__DUAL_ARM_STD::
_LLx__Get_Empty__InSlot(const int ll_index, const int db_start,const int db_offset, int& slot_id)
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

	for(int i=db_start; i<slot_max; i+=db_offset)
	{
		if((dEXT_CH__SCH_DB_LLx_SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__ENABLE) < 0)
			|| (dEXT_CH__CFG_DB_LLx_SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__ENABLE) < 0))
		{
			continue;
		}

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
LLx__Get_Occupied__InSlot(const int ll_index, CUIntArray& l_slot_id)
{
	l_slot_id.RemoveAll();

	if(xEXT_CH__SCH_DB_LLx_USE_FLAG[ll_index]->Check__DATA(STR__ENABLE) < 0)
	{
		return -11;
	}
	if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_index]->Check__DATA(LBx_MODE__ONLY_OUTPUT) > 0)
	{
		return -12;
	}

	// ...
	int i_limit = iLLx_SLOT_MAX[ll_index];

	for(int i=0; i<i_limit; i++)
	{
		if((dEXT_CH__SCH_DB_LLx_SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__ENABLE) < 0)
			|| (dEXT_CH__CFG_DB_LLx_SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__ENABLE) < 0))
		{
			continue;
		}

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
			int slot_id = i + 1;
			l_slot_id.Add(slot_id);
		}
	}

	return l_slot_id.GetSize();
}

int  CObj__DUAL_ARM_STD::
LLx__Get_Occupied__InSlot(const int ll_index,int& slot_id)
{
	return _LLx__Check_All_Occupied__InSlot(ll_index, 0, 1, slot_id);
}
int  CObj__DUAL_ARM_STD::
LLx__Check_All_Occupied__InSlot_Of_Odd_Type(const int ll_index)
{
	int slot_id;

	int r_flag = _LLx__Get_Empty__InSlot(ll_index, 0, 2, slot_id);
	if(r_flag > 0)		return -1;

	return 1;
}
int  CObj__DUAL_ARM_STD::
LLx__Check_All_Occupied__InSlot_Of_Even_Type(const int ll_index)
{
	int slot_id;

	int r_flag = _LLx__Get_Empty__InSlot(ll_index, 1, 2, slot_id);
	if(r_flag > 0)		return -1;

	return 1;
}
int  CObj__DUAL_ARM_STD::
_LLx__Check_All_Occupied__InSlot(const int ll_index, const int db_start,const int db_offset, int& slot_id)
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

	for(int i=db_start; i<slot_max; i+=db_offset)
	{
		if((dEXT_CH__SCH_DB_LLx_SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__ENABLE) < 0)
			|| (dEXT_CH__CFG_DB_LLx_SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__ENABLE) < 0))
		{
			continue;
		}

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

int  CObj__DUAL_ARM_STD::
LLx__Check_Empty__InSlot(const int ll_index)
{
	CUIntArray l_ll_slot;

	return LLx__Check_Empty__InSlot(ll_index, l_ll_slot);
}
int  CObj__DUAL_ARM_STD::
LLx__Check_Empty__InSlot(const int ll_index, CUIntArray& l_ll_slot)
{
	l_ll_slot.RemoveAll();

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
		if((dEXT_CH__SCH_DB_LLx_SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__ENABLE) < 0)
			|| (dEXT_CH__CFG_DB_LLx_SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__ENABLE) < 0))
		{
			continue;
		}

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
			int slot_id = i + 1;

			l_ll_slot.Add(slot_id);
		}
	}

	if(l_ll_slot.GetSize() > 0)
	{
		return l_ll_slot[0];
	}
	return -1;
}

int  CObj__DUAL_ARM_STD::
LLx__Get_Occupied__Total_OutSlot(const int ll_index)
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
	int count = 0;

	int i_limit = iLLx_SLOT_MAX[ll_index];
	int i;

	for(i=0; i<i_limit; i++)
	{
		if((dEXT_CH__SCH_DB_LLx_SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__ENABLE) < 0)
			|| (dEXT_CH__CFG_DB_LLx_SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__ENABLE) < 0))
		{
			continue;
		}

		if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_index]->Check__DATA(LBx_MODE__ALL) > 0)
		{
			CString ch_data = xEXT_CH__SCH_DB_LLx_SLOT_MODE[ll_index][i]->Get__STRING();

			if((ch_data.CompareNoCase(SLOT_STS__OUTPUT) != 0)
				&& (ch_data.CompareNoCase(SLOT_STS__ALL)    != 0))
			{
				continue;
			}
		}

		if(xEXT_CH__LLx__SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__NONE) < 0)
		{
			count++;
		}
	}
	return count;
}

int  CObj__DUAL_ARM_STD::
LLx__Get_Occupied__Total_InSlot()
{
	int ll_total = 0;

	for(int ll_i=0; ll_i < iLLx_SIZE; ll_i++)
	{
		int ll_count = LLx__Get_Occupied__Total_InSlot(ll_i, -1);
		if(ll_count < 1)			continue;;

		ll_total += ll_count;
	}
	return ll_total;
}
int  CObj__DUAL_ARM_STD::
LLx__Get_Occupied__Total_InSlot(const int ll_index,const int prc_check)
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
	int count = 0;

	int i_limit = iLLx_SLOT_MAX[ll_index];
	int i;

	for(i=0; i<i_limit; i++)
	{
		if((dEXT_CH__SCH_DB_LLx_SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__ENABLE) < 0)
			|| (dEXT_CH__CFG_DB_LLx_SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__ENABLE) < 0))
		{
			continue;
		}

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
			if(prc_check > 0)
			{
				IDS__SCH_MATERIAL_STATUS ds_info;

				CString lb_sch_name;
				int lb_in_slotid = i + 1;

				CString ll_name = Get__LLx_NAME(ll_index);
				lb_sch_name.Format("%s-%1d", ll_name,lb_in_slotid);

				if(xSCH_MATERIAL_CTRL->Get__MATERIAL_STATUS(lb_sch_name,ds_info) > 0)
				{
					count++;		
				}
			}
			else
			{
				count++;
			}
		}
	}
	return count;
}

int  CObj__DUAL_ARM_STD::
LLx__Check_Occupied__OutSlot(const int ll_index,const int slot_index)
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
	int slot_size = iLLx_SLOT_MAX[ll_index];

	if(slot_index <  0)					return -1;
	if(slot_index >= slot_size)			return -1;

	if((dEXT_CH__SCH_DB_LLx_SLOT_STATUS[ll_index][slot_index]->Check__DATA(SLOT_STS__ENABLE) < 0)
		|| (dEXT_CH__CFG_DB_LLx_SLOT_STATUS[ll_index][slot_index]->Check__DATA(SLOT_STS__ENABLE) < 0))
	{
		return -1;
	}

	if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_index]->Check__DATA(LBx_MODE__ALL) > 0)
	{
		CString ch_data = xEXT_CH__SCH_DB_LLx_SLOT_MODE[ll_index][slot_index]->Get__STRING();

		if(ch_data.CompareNoCase(SLOT_STS__OUTPUT) != 0)
		{
			if(ch_data.CompareNoCase(SLOT_STS__ALL) == 0)
			{
				CString sch_name;
				CString ll_name = Get__LLx_NAME(ll_index);

				sch_name.Format("%s-%1d", ll_name,slot_index+1);

				if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(sch_name) > 0)
				{
					return -1;
				}
			}
			else
			{
				return -1;
			}
		}
	}

	if(xEXT_CH__LLx__SLOT_STATUS[ll_index][slot_index]->Check__DATA(SLOT_STS__NONE) < 0)
	{
		return  1;
	}
	return -1;
}

int  CObj__DUAL_ARM_STD::
LLx__Check_Occupied__InSlot(const int ll_index)
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
	int i_limit = iLLx_SLOT_MAX[ll_index];
	int i;

	for(i=0; i<i_limit; i++)
	{
		if((dEXT_CH__SCH_DB_LLx_SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__ENABLE) < 0)
			|| (dEXT_CH__CFG_DB_LLx_SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__ENABLE) < 0))
		{
			continue;
		}

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
			return  13;
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
	int i_limit = iLLx_SLOT_MAX[ll_index];
	int i;

	for(i=0; i<i_limit; i++)
	{
		if((dEXT_CH__SCH_DB_LLx_SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__ENABLE) < 0)
			|| (dEXT_CH__CFG_DB_LLx_SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__ENABLE) < 0))
		{
			continue;
		}

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
			return  13;
		}
	}

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
	int i_limit = iLLx_SLOT_MAX[ll_index];
	int i;

	for(i=0; i<i_limit; i++)
	{
		if((dEXT_CH__SCH_DB_LLx_SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__ENABLE) < 0)
			|| (dEXT_CH__CFG_DB_LLx_SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__ENABLE) < 0))
		{
			continue;
		}

		if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_index]->Check__DATA(LBx_MODE__ALL) > 0)
		{
			CString ch_data = xEXT_CH__SCH_DB_LLx_SLOT_MODE[ll_index][i]->Get__STRING();

			if(ch_data.CompareNoCase(SLOT_STS__OUTPUT) != 0)
			{
				if(ch_data.CompareNoCase(SLOT_STS__INPUT) == 0)
				{
					continue;
				}
				else if(ch_data.CompareNoCase(SLOT_STS__ALL) == 0)
				{
					CString sch_name;
					CString ll_name = Get__LLx_NAME(ll_index);

					sch_name.Format("%s-%1d", ll_name,i+1);

					if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(sch_name) > 0)
					{
						continue;
					}
				}
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
int  CObj__DUAL_ARM_STD::
LLx__Get_Occupied__Only_Output(const int ll_index,CUIntArray& l_slot_id)
{
	l_slot_id.RemoveAll();

	if(xEXT_CH__SCH_DB_LLx_USE_FLAG[ll_index]->Check__DATA(STR__ENABLE) < 0)
	{
		return -11;
	}
	if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_index]->Check__DATA(LBx_MODE__ONLY_OUTPUT) < 0)
	{
		return -12;
	}

	// ...
	int i_limit = iLLx_SLOT_MAX[ll_index];

	for(int i=0; i<i_limit; i++)
	{
		if((dEXT_CH__SCH_DB_LLx_SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__ENABLE) < 0)
			|| (dEXT_CH__CFG_DB_LLx_SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__ENABLE) < 0))
		{
			continue;
		}

		if(xEXT_CH__LLx__SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__NONE) < 0)
		{
			int slot_id = i + 1;
			l_slot_id.Add(slot_id);
		}
	}
	return -1;
}

int  CObj__DUAL_ARM_STD::
LLx__Get_Occupied__Slot(const int ll_index, int& slot_id, CString& wfr_title,CString& wfr_state)
{
	int i_limit = iLLx_SLOT_MAX[ll_index];
	int i;

	for(i=0; i<i_limit; i++)
	{
		if(dEXT_CH__SCH_DB_LLx_SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__ENABLE) < 0)		continue;
		if(dEXT_CH__CFG_DB_LLx_SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__ENABLE) < 0)		continue;

		if(xEXT_CH__LLx__SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__NONE) > 0)				continue;

		slot_id = i + 1;

		wfr_title = xEXT_CH__LLx__SLOT_TITLE[ll_index][i]->Get__STRING();
		wfr_state = xEXT_CH__LLx__SLOT_STATUS[ll_index][i]->Get__STRING();
		return 1;
	}
	return -1;
}

int  CObj__DUAL_ARM_STD::
LLx__Get_Occupied__TotalSlot(const int ll_index)
{
	int count = 0;

	int i_limit = iLLx_SLOT_MAX[ll_index];
	int i;

	for(i=0; i<i_limit; i++)
	{
		if((dEXT_CH__SCH_DB_LLx_SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__ENABLE) < 0)
			|| (dEXT_CH__CFG_DB_LLx_SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__ENABLE) < 0))
		{
			continue;
		}

		if(xEXT_CH__LLx__SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__NONE) < 0)
		{
			count++;
		}
	}

	return count;
}

int  CObj__DUAL_ARM_STD::
LLx__Get_Occupied__Slot_List(const int ll_index,CUIntArray& l_slot_id)
{
	l_slot_id.RemoveAll();

	// ...
	int count = 0;

	int i_limit = iLLx_SLOT_MAX[ll_index];

	for(int i=0; i<i_limit; i++)
	{
		if(xEXT_CH__LLx__SLOT_STATUS[ll_index][i]->Check__DATA(SLOT_STS__NONE) > 0)
		{
			continue;
		}

		// ...
		count++;

		int id = i + 1;
		l_slot_id.Add(id);
	}

	return count;
}


//---------------------------------------------------------------------------------------
int  CObj__DUAL_ARM_STD::
Buffer1__Get_Occupied__Slot(int& slot_id, CString& title)
{
	CString var_data;
	xEXT_CH__BUFFER1__CFG_SLOT_MAX->Get__DATA(var_data);

	int slot_size = atoi(var_data);
	int i;

	if(xCH__SCH_DB_ST1_WAFER_PICK_MODE->Check__DATA(STR__BOTTOM_TO_UP) > 0)
	{
		for(i=0;i<slot_size;i++)
		{
			if(xEXT_CH__BUFFER1__SLOT_STATUS[i]->Check__DATA(STR__NONE) > 0)
			{
				continue;
			}

			// ...
			{
				slot_id = i + 1;
				xEXT_CH__BUFFER1__SLOT_TITLE[i]->Get__DATA(title);
			}
			return 1;
		}
	}
	else
	{
		for(i=slot_size-1;i>=0;i--)
		{
			if(xEXT_CH__BUFFER1__SLOT_STATUS[i]->Check__DATA(STR__NONE) > 0)
			{
				continue;
			}

			// ...
			{
				slot_id = i + 1;
				xEXT_CH__BUFFER1__SLOT_TITLE[i]->Get__DATA(title);
			}
			return 1;
		}
	}
	return -1;
}
int  CObj__DUAL_ARM_STD::
Buffer1__Get_Occupied__Slot_To_Process(int& slot_id)
{
	CUIntArray l__stx_slot_id;

	int r_flag = Buffer1__Get_Occupied__Slot_To_Process(l__stx_slot_id);
	if(r_flag < 0)		return r_flag;

	slot_id = l__stx_slot_id[0];
	return 1;
}
int  CObj__DUAL_ARM_STD::
Buffer1__Get_Occupied__Slot_To_Process(CUIntArray& l__stx_slot_id)
{
	l__stx_slot_id.RemoveAll();

	// ...
	CUIntArray l__stx_wfr_id;

	// ...
	CString var_data = xEXT_CH__BUFFER1__CFG_SLOT_MAX->Get__STRING();
	int slot_size = atoi(var_data);

	for(int i=0; i<slot_size; i++)
	{
		if(xEXT_CH__BUFFER1__SLOT_STATUS[i]->Check__DATA(STR__NONE) > 0)
		{
			continue;
		}

		int slot_id = i + 1;
		l__stx_slot_id.Add(slot_id);	

		CString str__wfr_id = xEXT_CH__BUFFER1__SLOT_TITLE[i]->Get__STRING();
		int wfr_id = Macro__Get_Number_Except_Char(':', str__wfr_id);
		l__stx_wfr_id.Add(wfr_id);
	}

	Macro__Get_Ascending_Order(l__stx_wfr_id, l__stx_slot_id);

	if(l__stx_slot_id.GetSize() > 0)			return 1;
	return -1;
}

int  CObj__DUAL_ARM_STD::
Buffer1__Check_Empty_Slot(int& slot_id)
{
	CString var_data;
	xEXT_CH__BUFFER1__CFG_SLOT_MAX->Get__DATA(var_data);

	int slot_size = atoi(var_data);
	int i;

	if(xCH__SCH_DB_ST1_WAFER_PICK_MODE->Check__DATA(STR__BOTTOM_TO_UP) > 0)
	{
		for(i=0;i<slot_size;i++)
		{
			if(xEXT_CH__BUFFER1__SLOT_STATUS[i]->Check__DATA(STR__NONE) > 0)
			{
				slot_id = i + 1;
				return 1;
			}
		}
	}
	else
	{
		for(i=slot_size-1;i>=0;i--)
		{
			if(xEXT_CH__BUFFER1__SLOT_STATUS[i]->Check__DATA(STR__NONE) > 0)
			{
				slot_id = i + 1;
				return 1;
			}
		}
	}
	return -1;
}
int  CObj__DUAL_ARM_STD::
Buffer1__Get_Empty__Slot_To_Process(int& slot_id)
{
	CUIntArray l__slot_id;

	int r_flag = Buffer1__Get_Empty__Slot_To_Process(l__slot_id, false);
	if(r_flag < 0)			return r_flag;

	if(l__slot_id.GetSize() > 0)
	{
		slot_id = l__slot_id[0];

		// ...
		{
			CString ch_data;
			ch_data.Format("%1d", slot_id);

			xEXT_CH__BUFFER1__APP_NEXT_SLOT_TO_PLACE->Set__DATA(ch_data);
		}
		return 1;
	}
	return -1;
}
int  CObj__DUAL_ARM_STD::
Buffer1__Get_Empty__Slot_To_Process(CUIntArray& l__slot_id, const bool active_slot_to_place)
{
	l__slot_id.RemoveAll();

	// ...
	CString var_data;

	xEXT_CH__BUFFER1__CFG_SLOT_MAX->Get__DATA(var_data);
	int slot_size = atoi(var_data);

	if(Buffer1__Check_Empty__All_Slot() > 0)
	{
		int cur__slot_id = (int) xEXT_CH__BUFFER1__APP_NEXT_SLOT_TO_PLACE->Get__VALUE();

		if(xCH__SCH_DB_ST1_WAFER_PICK_MODE->Check__DATA(STR__BOTTOM_TO_UP) > 0)
		{
			if(cur__slot_id >= slot_size)
			{
				xEXT_CH__BUFFER1__APP_NEXT_SLOT_TO_PLACE->Set__VALUE(1);
				xEXT_CH__BUFFER1__APP_NEXT_SLOT_TO_PICK->Set__VALUE(1);
			}
		}
		else
		{
			if(cur__slot_id <= 1)
			{
				xEXT_CH__BUFFER1__APP_NEXT_SLOT_TO_PLACE->Set__VALUE(slot_size);
				xEXT_CH__BUFFER1__APP_NEXT_SLOT_TO_PICK->Set__VALUE(slot_size);
			}
		}
	}

	// ...
	int slot_id = -1;
	int i;

	// Start Search ...
	{
		xEXT_CH__BUFFER1__APP_NEXT_SLOT_TO_PLACE->Get__DATA(var_data);
		int s_index = atoi(var_data) - 1;
		if(s_index < 0)		s_index = 0;

		if(xCH__SCH_DB_ST1_WAFER_PICK_MODE->Check__DATA(STR__BOTTOM_TO_UP) > 0)
		{
			for(i=s_index; i<slot_size; i++)
			{
				if(xEXT_CH__BUFFER1__SLOT_STATUS[i]->Check__DATA(STR__NONE) < 0)		continue;

				if(active_slot_to_place)
				{
					if(slot_id < 0)
					{
						slot_id = i + 1;

						var_data.Format("%1d", slot_id);
						xEXT_CH__BUFFER1__APP_NEXT_SLOT_TO_PLACE->Set__DATA(var_data);
					}
				}

				slot_id = i + 1;
				l__slot_id.Add(slot_id);
			}

			if(slot_id > 0)			return 1;
		}
		else
		{
			for(i=s_index; i>=0; i--)
			{
				if(xEXT_CH__BUFFER1__SLOT_STATUS[i]->Check__DATA(STR__NONE) < 0)		continue;

				if(active_slot_to_place)
				{
					if(slot_id < 0)
					{
						slot_id = i + 1;

						var_data.Format("%1d", slot_id);
						xEXT_CH__BUFFER1__APP_NEXT_SLOT_TO_PLACE->Set__DATA(var_data);
					}
				}

				slot_id = i + 1;
				l__slot_id.Add(slot_id);
			}

			if(slot_id > 0)			return 1;
		}
	}

	// All Search ...
	if(xCH__SCH_DB_ST1_WAFER_PICK_MODE->Check__DATA(STR__BOTTOM_TO_UP) > 0)
	{
		for(i=0; i<slot_size; i++)
		{
			if(xEXT_CH__BUFFER1__SLOT_STATUS[i]->Check__DATA(STR__NONE) < 0)		continue;

			if(active_slot_to_place)
			{
				if(slot_id > 0)
				{
					slot_id = i + 1;

					var_data.Format("%1d", slot_id);
					xEXT_CH__BUFFER1__APP_NEXT_SLOT_TO_PLACE->Set__DATA(var_data);
				}
			}

			slot_id = i + 1;
			l__slot_id.Add(slot_id);
		}

		if(slot_id > 0)			return 1;
	}
	else
	{
		for(i=slot_size-1; i>=0; i--)
		{
			if(xEXT_CH__BUFFER1__SLOT_STATUS[i]->Check__DATA(STR__NONE) < 0)		continue;

			if(active_slot_to_place)
			{
				if(slot_id > 0)
				{
					slot_id = i + 1;

					var_data.Format("%1d", slot_id);
					xEXT_CH__BUFFER1__APP_NEXT_SLOT_TO_PLACE->Set__DATA(var_data);
				}
			}

			slot_id = i + 1;
			l__slot_id.Add(slot_id);
		}

		if(slot_id > 0)			return 1;
	}

	return -1;
}
CString CObj__DUAL_ARM_STD::
Buffer1__Get_LotID()
{
	CString str_lotid = "";

	if(Buffer1__Check_Empty__All_Slot() < 0)
	{
		int slot_id;

		if(Buffer1__Get_Occupied__Slot_To_Process(slot_id) > 0)
		{
			CString sch_name;
			sch_name.Format("%s-%1d", MODULE__BUFFER1,slot_id);

			IDS__SCH_MATERIAL_INFO ds_info;
			if(xSCH_MATERIAL_CTRL->Get__MATERIAL_INFO(sch_name,ds_info) > 0)
			{
				str_lotid = ds_info.sLOT_ID;
			}
			else
			{
				str_lotid = "!!!";
			}
		}
		else
		{
			str_lotid = "???";
		}
	}

	xEXT_CH__BUFFER1__APP_CUR_LOTID->Set__DATA(str_lotid);
	return str_lotid;
}
int  CObj__DUAL_ARM_STD::
Buffer1__Check_Empty__All_Slot()
{
	CString var_data;
	xEXT_CH__BUFFER1__CFG_SLOT_MAX->Get__DATA(var_data);

	int slot_size = atoi(var_data);
	int i;

	for(i=0;i<slot_size;i++)
	{
		if(xEXT_CH__BUFFER1__SLOT_STATUS[i]->Check__DATA(STR__NONE) < 0)
		{
			return -1;
		}
	}
	return 1;
}
int  CObj__DUAL_ARM_STD::
Buffer1__Check_Empty__Any_Slot()
{
	CString var_data;
	xEXT_CH__BUFFER1__CFG_SLOT_MAX->Get__DATA(var_data);

	int slot_size = atoi(var_data);
	int i;

	for(i=0;i<slot_size;i++)
	{
		if(xEXT_CH__BUFFER1__SLOT_STATUS[i]->Check__DATA(STR__NONE) > 0)
		{
			return 1;
		}
	}
	return -1;
}
int  CObj__DUAL_ARM_STD::
Buffer1__Check_Wait_Sec(const int slot_id, const double ref_sec)
{		
	int db_index = slot_id - 1;

	if(db_index < 0)						return -1;
	if(db_index >= CFG_LP__SLOT_MAX)		return -1;

	CString var_data;
	xEXT_CH__BUFFER1__SLOT_WAIT_SEC[db_index]->Get__DATA(var_data);

	if(atoi(var_data) > ref_sec)
	{
		return 1;
	}
	return -1;
}


// ...
int  CObj__DUAL_ARM_STD::
Buffer2__Get_Occupied__Slot(int& slot_id, CString& title)
{
	CString var_data;
	xEXT_CH__BUFFER2__CFG_SLOT_MAX->Get__DATA(var_data);

	int slot_size = atoi(var_data);
	int i;

	if(xCH__SCH_DB_ST2_WAFER_PICK_MODE->Check__DATA(STR__BOTTOM_TO_UP) > 0)
	{
		for(i=0;i<slot_size;i++)
		{
			if(xEXT_CH__BUFFER2__SLOT_STATUS[i]->Check__DATA(STR__NONE) > 0)
			{
				continue;
			}

			// ...
			{
				slot_id = i + 1;
				xEXT_CH__BUFFER2__SLOT_TITLE[i]->Get__DATA(title);
			}
			return 1;
		}
	}
	else
	{
		for(i=slot_size-1;i>=0;i--)
		{
			if(xEXT_CH__BUFFER2__SLOT_STATUS[i]->Check__DATA(STR__NONE) > 0)
			{
				continue;
			}

			// ...
			{
				slot_id = i + 1;
				xEXT_CH__BUFFER2__SLOT_TITLE[i]->Get__DATA(title);
			}
			return 1;
		}
	}
	return -1;
}
int  CObj__DUAL_ARM_STD::
Buffer2__Check_Empty_Slot(int& slot_id)
{
	CString var_data;
	xEXT_CH__BUFFER2__CFG_SLOT_MAX->Get__DATA(var_data);

	int slot_size = atoi(var_data);
	int i;

	if(xCH__SCH_DB_ST2_WAFER_PICK_MODE->Check__DATA(STR__BOTTOM_TO_UP) > 0)
	{
		for(i=0;i<slot_size;i++)
		{
			if(xEXT_CH__BUFFER2__SLOT_STATUS[i]->Check__DATA(STR__NONE) > 0)
			{
				slot_id = i + 1;
				return 1;
			}
		}
	}
	else
	{
		for(i=slot_size-1;i>=0;i--)
		{
			if(xEXT_CH__BUFFER2__SLOT_STATUS[i]->Check__DATA(STR__NONE) > 0)
			{
				slot_id = i + 1;
				return 1;
			}
		}
	}
	return -1;
}
int  CObj__DUAL_ARM_STD::
Buffer2__Get_Empty__Slot_To_Process(int& slot_id)
{
	CUIntArray l__slot_id;

	int r_flag = Buffer2__Get_Empty__Slot_To_Process(l__slot_id, false);
	if(r_flag < 0)			return r_flag;

	if(l__slot_id.GetSize() > 0)
	{	
		slot_id = l__slot_id[0];

		// ...
		{
			CString ch_data;
			ch_data.Format("%1d", slot_id);

			xEXT_CH__BUFFER2__APP_NEXT_SLOT_TO_PLACE->Set__DATA(ch_data);
		}
		return 1;
	}
	return -1;
}

int  CObj__DUAL_ARM_STD::
Buffer2__Get_Empty__Slot_To_Process(CUIntArray& l__slot_id, const bool active_slot_to_place)
{
	l__slot_id.RemoveAll();

	// ...
	CString var_data;

	xEXT_CH__BUFFER2__CFG_SLOT_MAX->Get__DATA(var_data);
	int slot_size = atoi(var_data);

	if(Buffer2__Check_Empty__All_Slot() > 0)
	{
		int cur__slot_id = (int) xEXT_CH__BUFFER2__APP_NEXT_SLOT_TO_PLACE->Get__VALUE();

		if(xCH__SCH_DB_ST2_WAFER_PICK_MODE->Check__DATA(STR__BOTTOM_TO_UP) > 0)
		{
			if(cur__slot_id >= slot_size)
			{
				xEXT_CH__BUFFER2__APP_NEXT_SLOT_TO_PLACE->Set__VALUE(1);
				xEXT_CH__BUFFER2__APP_NEXT_SLOT_TO_PICK->Set__VALUE(1);
			}
		}
		else
		{
			if(cur__slot_id <= 1)
			{
				xEXT_CH__BUFFER2__APP_NEXT_SLOT_TO_PLACE->Set__VALUE(slot_size);
				xEXT_CH__BUFFER2__APP_NEXT_SLOT_TO_PICK->Set__VALUE(slot_size);
			}
		}
	}

	// ...
	int slot_id = -1;
	int i;

	// Start Search ...
	{
		xEXT_CH__BUFFER2__APP_NEXT_SLOT_TO_PLACE->Get__DATA(var_data);
		int s_index = atoi(var_data) - 1;
		if(s_index < 0)		s_index = 0;

		if(xCH__SCH_DB_ST2_WAFER_PICK_MODE->Check__DATA(STR__BOTTOM_TO_UP) > 0)
		{
			for(i=s_index; i<slot_size; i++)
			{
				if(xEXT_CH__BUFFER2__SLOT_STATUS[i]->Check__DATA(STR__NONE) < 0)		continue;

				if(active_slot_to_place)
				{
					if(slot_id < 0)
					{
						slot_id = i + 1;

						var_data.Format("%1d", slot_id);
						xEXT_CH__BUFFER2__APP_NEXT_SLOT_TO_PLACE->Set__DATA(var_data);
					}
				}

				slot_id = i + 1;
				l__slot_id.Add(slot_id);
			}

			if(slot_id > 0)			return 1;
		}
		else
		{
			for(i=s_index; i>=0; i--)
			{
				if(xEXT_CH__BUFFER2__SLOT_STATUS[i]->Check__DATA(STR__NONE) < 0)		continue;

				if(active_slot_to_place)
				{
					if(slot_id < 0)
					{
						slot_id = i + 1;

						var_data.Format("%1d", slot_id);
						xEXT_CH__BUFFER2__APP_NEXT_SLOT_TO_PLACE->Set__DATA(var_data);
					}
				}

				slot_id = i + 1;
				l__slot_id.Add(slot_id);
			}

			if(slot_id > 0)			return 1;
		}
	}

	// All Search ...
	if(xCH__SCH_DB_ST2_WAFER_PICK_MODE->Check__DATA(STR__BOTTOM_TO_UP) > 0)
	{
		for(i=0; i<slot_size; i++)
		{
			if(xEXT_CH__BUFFER2__SLOT_STATUS[i]->Check__DATA(STR__NONE) < 0)		continue;

			if(active_slot_to_place)
			{
				if(slot_id > 0)
				{
					slot_id = i + 1;

					var_data.Format("%1d", slot_id);
					xEXT_CH__BUFFER2__APP_NEXT_SLOT_TO_PLACE->Set__DATA(var_data);
				}
			}

			slot_id = i + 1;
			l__slot_id.Add(slot_id);
		}

		if(slot_id > 0)			return 1;
	}
	else
	{
		for(i=slot_size-1; i>=0; i--)
		{
			if(xEXT_CH__BUFFER2__SLOT_STATUS[i]->Check__DATA(STR__NONE) < 0)		continue;

			if(active_slot_to_place)
			{
				if(slot_id > 0)
				{
					slot_id = i + 1;

					var_data.Format("%1d", slot_id);
					xEXT_CH__BUFFER2__APP_NEXT_SLOT_TO_PLACE->Set__DATA(var_data);
				}
			}

			slot_id = i + 1;
			l__slot_id.Add(slot_id);
		}

		if(slot_id > 0)			return 1;
	}

	return -1;
}
int  CObj__DUAL_ARM_STD::
Buffer2__Get_Occupied__Slot_To_Process(int& slot_id)
{
	CUIntArray l__stx_slot_id;

	int r_flag = Buffer2__Get_Occupied__Slot_To_Process(l__stx_slot_id);
	if(r_flag < 0)			return r_flag;

	slot_id = l__stx_slot_id[0];
	return 1;
}
int  CObj__DUAL_ARM_STD::
Buffer2__Get_Occupied__Slot_To_Process(CUIntArray& l__stx_slot_id)
{
	l__stx_slot_id.RemoveAll();

	// ...
	CUIntArray l__stx_wfr_id;

	// ...
	CString var_data = xEXT_CH__BUFFER2__CFG_SLOT_MAX->Get__STRING();
	int slot_size = atoi(var_data);

	for(int i=0; i<slot_size; i++)
	{
		if(xEXT_CH__BUFFER2__SLOT_STATUS[i]->Check__DATA(STR__NONE) > 0)
		{
			continue;
		}

		int slot_id = i + 1;
		l__stx_slot_id.Add(slot_id);

		CString str__wfr_id = xEXT_CH__BUFFER1__SLOT_TITLE[i]->Get__STRING();
		int wfr_id = Macro__Get_Number_Except_Char(':', str__wfr_id);
		l__stx_wfr_id.Add(wfr_id);
	}

	Macro__Get_Ascending_Order(l__stx_wfr_id, l__stx_slot_id);

	if(l__stx_slot_id.GetSize() > 0)			return 1;
	return -1;
}

CString CObj__DUAL_ARM_STD::
Buffer2__Get_LotID()
{
	CString str_lotid = "";

	if(Buffer2__Check_Empty__All_Slot() < 0)
	{
		int slot_id;

		if(Buffer2__Get_Occupied__Slot_To_Process(slot_id) > 0)
		{
			CString sch_name;
			sch_name.Format("%s-%1d", MODULE__BUFFER2,slot_id);

			IDS__SCH_MATERIAL_INFO ds_info;
			if(xSCH_MATERIAL_CTRL->Get__MATERIAL_INFO(sch_name,ds_info) > 0)
			{
				str_lotid = ds_info.sLOT_ID;
			}
			else
			{
				str_lotid = "!!!";
			}
		}
		else
		{
			str_lotid = "???";
		}
	}

	xEXT_CH__BUFFER2__APP_CUR_LOTID->Set__DATA(str_lotid);
	return str_lotid;
}
int  CObj__DUAL_ARM_STD::
Buffer2__Check_Empty__All_Slot()
{
	CString var_data;
	xEXT_CH__BUFFER2__CFG_SLOT_MAX->Get__DATA(var_data);

	int slot_size = atoi(var_data);
	int i;

	for(i=0;i<slot_size;i++)
	{
		if(xEXT_CH__BUFFER2__SLOT_STATUS[i]->Check__DATA(STR__NONE) < 0)
		{
			return -1;
		}
	}
	return 1;
}
int  CObj__DUAL_ARM_STD::
Buffer2__Check_Empty__Any_Slot()
{
	CString var_data;
	xEXT_CH__BUFFER2__CFG_SLOT_MAX->Get__DATA(var_data);

	int slot_size = atoi(var_data);
	int i;

	for(i=0;i<slot_size;i++)
	{
		if(xEXT_CH__BUFFER2__SLOT_STATUS[i]->Check__DATA(STR__NONE) > 0)
		{
			return 1;
		}
	}
	return -1;
}
int  CObj__DUAL_ARM_STD::
Buffer2__Check_Wait_Sec(const int slot_id, const double ref_sec)
{		
	int db_index = slot_id - 1;

	if(db_index < 0)						return -1;
	if(db_index >= CFG_LP__SLOT_MAX)		return -1;

	// ...
	CString var_data;
	xEXT_CH__BUFFER2__SLOT_WAIT_SEC[db_index]->Get__DATA(var_data);

	if(atoi(var_data) > ref_sec)
	{
		return 1;
	}
	return -1;
}

int  CObj__DUAL_ARM_STD::
_Get__PM_INDEX(const CString& pm_name)
{
	CString str_name;

	for(int i=0; i<iPMx_SIZE; i++)
	{
		int pm_id = i + 1;

		str_name.Format("PM%1d", pm_id);
		if(pm_name.CompareNoCase(str_name) != 0)		continue;

		return i;
	}
	return -1;
}
