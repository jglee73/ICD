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
	int r_flag = pATM_RB__OBJ_CTRL->Run__OBJ_MODE(obj_mode, l_para);

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

	if((obj_mode.CompareNoCase(ATM_CMMD__PLACE)  == 0)
	|| (obj_mode.CompareNoCase(ATM_CMMD__XPLACE) == 0))
	{
		active_place = true;

		arm_a__wfr_state = xCH__ATM_RB__SLOT01_STATUS->Get__STRING();
		arm_a__wfr_title = xCH__ATM_RB__SLOT01_TITLE->Get__STRING();

		arm_b__wfr_state = xCH__ATM_RB__SLOT02_STATUS->Get__STRING();
		arm_b__wfr_title = xCH__ATM_RB__SLOT02_TITLE->Get__STRING();
	}

	// ...
	CStringArray l_set_para;
	l_set_para.Copy(l_para);

	int r_flag = pATM_RB__OBJ_CTRL->Call__OBJ_MODE(obj_mode, l_para);

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
							xCH__ATM_RB__SLOT01_STATUS->Get__STRING(),
							xCH__ATM_RB__SLOT01_TITLE->Get__STRING());
			log_msg += log_bff;

			log_bff.Format(" * B_ARM <- %s (%s) \n",
							xCH__ATM_RB__SLOT02_STATUS->Get__STRING(),
							xCH__ATM_RB__SLOT02_TITLE->Get__STRING());
			log_msg += log_bff;
		}

		log_bff.Format(" * log_id <- %s \n\n", log_id);
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
