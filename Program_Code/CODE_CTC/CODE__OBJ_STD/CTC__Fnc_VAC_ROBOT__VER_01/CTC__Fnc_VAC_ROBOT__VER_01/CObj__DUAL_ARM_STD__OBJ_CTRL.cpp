#include "StdAfx.h"
#include "CObj__DUAL_ARM_STD.h"

#include "Macro_Function.h"


// ...
int CObj__DUAL_ARM_STD::Run__ROBOT_OBJ(const CString& obj_mode, const CString& log_id)
{
	CStringArray l_para;

	return Run__ROBOT_OBJ(obj_mode, l_para, log_id);
}
int CObj__DUAL_ARM_STD::Run__ROBOT_OBJ(const CString& obj_mode, const CStringArray& l_para, const CString& log_id)
{
	int r_flag = pVAC_RB__OBJ_CTRL->Run__OBJ_MODE(obj_mode, l_para);

	// ...
	{
		CString log_msg;
		CString log_bff;

		log_msg = "\n";
		log_bff.Format("ROBOT - Run_Object( %s ) : r_flag(%1d) \n", obj_mode,r_flag);
		log_msg += log_bff;

		int i_limit = l_para.GetSize();
		if(i_limit > 0)		
		{
			log_bff.Format(" * Parameter Size (%1d) ... \n", i_limit);
			log_msg += log_bff;
		}
		for(int i=0; i<i_limit; i++)
		{
			log_bff.Format("  ** %1d) %s \n", i+1,l_para[i]);
			log_msg += log_bff;
		}

		log_bff.Format(" * log_id <- %s \n\n", log_id);
		log_msg += log_bff;

		xAPP_LOG_CTRL->WRITE__LOG(log_msg);
	}
	return r_flag;
}

int CObj__DUAL_ARM_STD::Call__ROBOT_OBJ(const CString& obj_mode, const CString& log_id)
{
	CStringArray l_para;

	return Call__ROBOT_OBJ(obj_mode, l_para, log_id);
}
int CObj__DUAL_ARM_STD::Call__ROBOT_OBJ(const CString& obj_mode, CStringArray& l_para, const CString& log_id)
{
	bool active_place = false;

	CString arm_a__wfr_state;
	CString arm_a__wfr_title;

	CString arm_b__wfr_state;
	CString arm_b__wfr_title;

	if((obj_mode.CompareNoCase(CMMD__PLACE)  == 0)
	|| (obj_mode.CompareNoCase(CMMD__XPLACE) == 0))
	{
		active_place = true;

		arm_a__wfr_state = xCH__VAC_RB__SLOT01_STATUS->Get__STRING();
		arm_a__wfr_title = xCH__VAC_RB__SLOT01_TITLE->Get__STRING();

		arm_b__wfr_state = xCH__VAC_RB__SLOT02_STATUS->Get__STRING();
		arm_b__wfr_title = xCH__VAC_RB__SLOT02_TITLE->Get__STRING();
	}

	// ...
	CStringArray l_set_para;
	l_set_para.Copy(l_para);

	int r_flag = pVAC_RB__OBJ_CTRL->Call__OBJ_MODE(obj_mode, l_para);

	// ...
	{
		CString log_msg;
		CString log_bff;

		log_msg = "\n";
		log_bff.Format("ROBOT - Call_Object( %s ) : r_flag(%1d) \n", obj_mode,r_flag);
		log_msg += log_bff;

		int i_limit = l_set_para.GetSize();
		if(i_limit > 0)		
		{
			log_bff.Format(" * Parameter Size (%1d) ... \n", i_limit);
			log_msg += log_bff;
		}
		for(int i=0; i<i_limit; i++)
		{
			log_bff.Format("  ** %1d) %s \n", i+1,l_set_para[i]);
			log_msg += log_bff;
		}

		if(active_place)
		{
			log_bff.Format(" * A_ARM <- %s (%s) \n", arm_a__wfr_state, arm_a__wfr_title);
			log_msg += log_bff;

			log_bff.Format(" * B_ARM <- %s (%s) \n", arm_b__wfr_state, arm_b__wfr_title);
			log_msg += log_bff;
		}
		else
		{
			log_bff.Format(" * A_ARM <- %s (%s) \n",
						   xCH__VAC_RB__SLOT01_STATUS->Get__STRING(),
						   xCH__VAC_RB__SLOT01_TITLE->Get__STRING());
			log_msg += log_bff;

			log_bff.Format(" * B_ARM <- %s (%s) \n",
						   xCH__VAC_RB__SLOT02_STATUS->Get__STRING(),
						   xCH__VAC_RB__SLOT02_TITLE->Get__STRING());
			log_msg += log_bff;
		}
		
		log_bff.Format(" * log_id <- %s \n\n", log_id);
		log_msg += log_bff;

		xAPP_LOG_CTRL->WRITE__LOG(log_msg);
	}
	return r_flag;
}


// ...
int CObj__DUAL_ARM_STD::Run__TMC_OBJ(const CString& obj_mode)
{
	int r_flag = pTMC_CHM__OBJ_CTRL->Run__OBJECT(obj_mode);

	// ...
	{
		CString log_msg;
		CString log_bff;

		log_msg = "\n";
		log_bff.Format("TMC - Run_Object( %s ) : r_flag(%1d) \n", obj_mode,r_flag);
		log_msg += log_bff;

		xAPP_LOG_CTRL->WRITE__LOG(log_msg);
	}
	return r_flag;
}

int CObj__DUAL_ARM_STD::Call__TMC_OBJ(const CString& obj_mode)
{
	int r_flag = pTMC_CHM__OBJ_CTRL->Call__OBJECT(obj_mode);

	// ...
	{
		CString log_msg;
		CString log_bff;

		log_msg = "\n";
		log_bff.Format("TMC - Call_Object( %s ) : r_flag(%1d) \n", obj_mode,r_flag);
		log_msg += log_bff;

		xAPP_LOG_CTRL->WRITE__LOG(log_msg);
	}
	return r_flag;
}


// ...
int CObj__DUAL_ARM_STD::Run__LLx_OBJ(const int ll_i, const CString& obj_mode)
{
	CStringArray l_para;

	return Run__LLx_OBJ(ll_i, obj_mode, l_para);
}
int CObj__DUAL_ARM_STD::Run__LLx_OBJ(const int ll_i, const CString& obj_mode, const CStringArray& l_para)
{
	Set__COOLING_SEC_OF_LLx_OBJ(ll_i, obj_mode);

	int r_flag = pLLx__OBJ_CTRL[ll_i]->Run__OBJ_MODE(obj_mode, l_para);

	// ...
	{
		CString log_msg;
		CString log_bff;

		int ll_id = ll_i + 1;

		log_msg = "\n";
		log_bff.Format("LL%1d - Run_Object( %s ) : r_flag(%1d) \n", ll_id,obj_mode,r_flag);
		log_msg += log_bff;

		int i_limit = l_para.GetSize();
		if(i_limit > 0)		
		{
			log_bff.Format(" * Parameter Size (%1d) ... \n", i_limit);
			log_msg += log_bff;
		}
		for(int i=0; i<i_limit; i++)
		{
			log_bff.Format("  ** %1d) %s \n", i+1,l_para[i]);
			log_msg += log_bff;
		}

		xAPP_LOG_CTRL->WRITE__LOG(log_msg);
	}
	return r_flag;
}

int CObj__DUAL_ARM_STD::Call__LLx_OBJ(const int ll_i, const CString& obj_mode)
{
	CStringArray l_para;

	return Call__LLx_OBJ(ll_i, obj_mode, l_para);
}
int CObj__DUAL_ARM_STD::Call__LLx_OBJ(const int ll_i, const CString& obj_mode, CStringArray& l_para)
{
	CStringArray l_set_para;
	l_set_para.Copy(l_para);

	// ...
	Set__COOLING_SEC_OF_LLx_OBJ(ll_i, obj_mode);

	int r_flag = pLLx__OBJ_CTRL[ll_i]->Call__OBJ_MODE(obj_mode, l_para);

	// ...
	{
		CString log_msg;
		CString log_bff;

		int ll_id = ll_i + 1;

		log_msg = "\n";
		log_bff.Format("LL%1d - Call_Object( %s ) : r_flag(%1d) \n", ll_id,obj_mode,r_flag);
		log_msg += log_bff;

		int i_limit = l_set_para.GetSize();
		if(i_limit > 0)		
		{
			log_bff.Format(" * Parameter Size (%1d) ... \n", i_limit);
			log_msg += log_bff;
		}
		for(int i=0; i<i_limit; i++)
		{
			log_bff.Format("  ** %1d) %s \n", i+1,l_set_para[i]);
			log_msg += log_bff;
		}

		xAPP_LOG_CTRL->WRITE__LOG(log_msg);
	}
	return r_flag;
}

int CObj__DUAL_ARM_STD::Set__COOLING_SEC_OF_LLx_OBJ(const int ll_i, const CString& obj_mode)
{
	if(obj_mode.CompareNoCase(CMMD__VENT) != 0)			return -1;

	// ...
	int max__cooling_sec = 0;

	// ...
	{
		CString ll_name;
		CUIntArray l_slot_id;

		ll_name = Get__LLx_NAME(ll_i);
		LLx__Get_Occupied__OutSlot(ll_i, l_slot_id);

		int k_limit = l_slot_id.GetSize();
		int k;

		for(k=0; k<k_limit; k++)
		{
			int ll_slot_id = l_slot_id[k];

			CString sch_name;
			sch_name.Format("%s-%1d", ll_name,ll_slot_id);

			int ll_id = ll_i + 1;
			CString ll_cooling_sec;

			int r_flag = xSCH_MATERIAL_CTRL->Get__COOLING_SEC_OF_LLx_OUT(sch_name, ll_id, ll_cooling_sec);
			int cur__cooling_sec = atoi(ll_cooling_sec);

			if(cur__cooling_sec > max__cooling_sec)			max__cooling_sec = cur__cooling_sec;			
		}
	}

	// ...
	CString ch_data;

	ch_data.Format("%1d", max__cooling_sec);
	aEXT_CH__LLx__PARA_VENT_COOLING_SEC[ll_i]->Set__DATA(ch_data);
	return 1;
}

// ...
int CObj__DUAL_ARM_STD::Run__PMx_OBJ(const int pm_i, const CString& obj_mode)
{
	CStringArray l_para;

	return Run__PMx_OBJ(pm_i, obj_mode, l_para);
}
int CObj__DUAL_ARM_STD::Run__PMx_OBJ(const int pm_i, const CString& obj_mode, const CStringArray& l_para)
{
	int r_flag = pPMx__OBJ_CTRL[pm_i]->Run__OBJ_MODE(obj_mode, l_para);

	// ...
	{
		CString log_msg;
		CString log_bff;

		int pm_id = pm_i + 1;

		log_msg = "\n";
		log_bff.Format("PM%1d - Run_Object( %s ) : r_flag(%1d) \n", pm_id,obj_mode,r_flag);
		log_msg += log_bff;

		int i_limit = l_para.GetSize();
		if(i_limit > 0)		
		{
			log_bff.Format(" * Parameter Size (%1d) ... \n", i_limit);
			log_msg += log_bff;
		}
		for(int i=0; i<i_limit; i++)
		{
			log_bff.Format("  ** %1d) %s \n", i+1,l_para[i]);
			log_msg += log_bff;
		}
		
		xAPP_LOG_CTRL->WRITE__LOG(log_msg);
	}
	return r_flag;
}

int CObj__DUAL_ARM_STD::Call__PMx_OBJ(const int pm_i, const CString& obj_mode)
{
	CStringArray l_para;

	return Call__PMx_OBJ(pm_i, obj_mode, l_para);
}
int CObj__DUAL_ARM_STD::Call__PMx_OBJ(const int pm_i, const CString& obj_mode, CStringArray& l_para)
{
	CStringArray l_set_para;
	l_set_para.Copy(l_para);
	
	int r_flag = pPMx__OBJ_CTRL[pm_i]->Call__OBJ_MODE(obj_mode, l_para);

	// ...
	{
		CString log_msg;
		CString log_bff;

		int pm_id = pm_i + 1;

		log_msg = "\n";
		log_bff.Format("PM%1d - Call_Object( %s ) : r_flag(%1d) \n", pm_id,obj_mode,r_flag);
		log_msg += log_bff;

		int i_limit = l_set_para.GetSize();
		if(i_limit > 0)		
		{
			log_bff.Format(" * Parameter Size (%1d) ... \n", i_limit);
			log_msg += log_bff;
		}
		for(int i=0; i<i_limit; i++)
		{
			log_bff.Format("  ** %1d) %s \n", i+1,l_set_para[i]);
			log_msg += log_bff;
		}

		xAPP_LOG_CTRL->WRITE__LOG(log_msg);
	}
	return r_flag;
}

