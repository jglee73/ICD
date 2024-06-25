#include "StdAfx.h"
#include "CObj__DUAL_ARM_STD.h"

#include "Macro_Function.h"


// ...
void CObj__DUAL_ARM_STD::Mon__PMx_CTRL(CII_OBJECT__VARIABLE *p_variable)
{
	int idle_flag  =  1;
	int abort_flag = -1;

	while(1)
	{
		while(xSCH_MATERIAL_CTRL->Get__MATERIAL_DB_SIZE() < 1)
		{
			if(idle_flag < 0)
			{
				if(xSCH_MATERIAL_CTRL->Check__SYSTEM_ABORT() > 0)
				{
					AUTO_CTRL__PMx_ABORT(p_variable);
				}
				else if(xSCH_MATERIAL_CTRL->Check__SYSTEM_STOP() > 0)
				{

				}
				else
				{
					try
					{
						AUTO_CTRL__LBo_PUMP(p_variable);
					}
					catch(int err_flag)
					{

					}
				}
			}

			Sleep(100);
			idle_flag = 1;
		}

		Sleep(1);

		if(idle_flag > 0)
		{
			idle_flag = -1;
		}

	}
}

void CObj__DUAL_ARM_STD::AUTO_CTRL__PMx_ABORT(CII_OBJECT__VARIABLE *p_variable)
{

	for(int i=0; i<iPMx_SIZE; i++)
	{
		if(xEXT_CH__CFG__PMx_USE[i]->Check__DATA("ENABLE") < 0)
		{
			continue;
		}
		if(xCH__PMx__OBJ_CTRL[i]->Check__DATA("ABORTED") > 0)
		{
			continue;
		}

		xCH__PMx__OBJ_CTRL[i]->Set__DATA("ABORT");
	}
}
void CObj__DUAL_ARM_STD::AUTO_CTRL__PMx_PUMP(CII_OBJECT__VARIABLE *p_variable)
{
	CString log_id = "AUTO_CTRL__PMx_PUMP()";

	for(int i=0; i<iPMx_SIZE; i++)
	{
		if(PMx__Is_Available(i) < 0)
		{
			continue;
		}

		if(xEXT_CH__CFG__TRANSFER_MODE->Check__DATA("VAC") > 0)
		{
			if(PMx__Is_VAC(i) < 0)
			{
				Run__PMx_OBJ(i, CMMD__PUMP);
			}
		}
		else
		{
			if(PMx__Is_ATM(i) < 0)
			{
				Run__PMx_OBJ(i, CMMD__VENT);
			}
		}
	}
}
void CObj__DUAL_ARM_STD::AUTO_CTRL__TMC_PUMP(CII_OBJECT__VARIABLE *p_variable)
{
	if(TMC_CHM__Is_Available() > 0)
	{
		if(xEXT_CH__CFG__TRANSFER_MODE->Check__DATA("VAC") > 0)
		{
			if(TMC_CHM__Is_VAC() < 0)
			{
				Run__TMC_OBJ(CMMD__PUMP);
			}
		}
		else
		{
			if(TMC_CHM__Is_ATM() < 0)
			{
				Run__TMC_OBJ(CMMD__VENT);
			}
		}
	}
}
void CObj__DUAL_ARM_STD::AUTO_CTRL__LBo_PUMP(CII_OBJECT__VARIABLE *p_variable)
{
	if(xEXT_CH__CFG__TRANSFER_MODE->Check__DATA("ATM") > 0)
	{
		return;
	}

	// ...
	int ll_limit = iLLx_LIMIT;
	int ll_i;

	for(ll_i=0; ll_i<ll_limit; ll_i++)
	{
		if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_i]->Check__DATA(LBx_MODE__ONLY_OUTPUT) > 0)
		{
			if((LLx__Is_Available(ll_i)    > 0)
			&& (LLx__Check_All_Empty(ll_i) > 0))
			{
				SCH__LLx_PUMP(ll_i, p_variable);
			}
		}
	}
}

int  CObj__DUAL_ARM_STD::_Get__MIN_PMx_INDEX_OF_PMx_IN_COUNT(const CStringArray& l__pm_name)
{
	CUIntArray l__pm_index;

	int k_limit = l__pm_name.GetSize();
	int k;

	for(k=0; k<k_limit; k++)
	{
		int pm_index = Macro__Get_PMC_INDEX(l__pm_name[k]);
		l__pm_index.Add(pm_index);
	}

	return _Get__MIN_PMx_INDEX_OF_PMx_IN_COUNT(l__pm_index);
}
int  CObj__DUAL_ARM_STD::_Get__MIN_PMx_INDEX_OF_PMx_IN_COUNT(const CUIntArray& l__pm_index)
{
	int cur__pm_index = -1;
	int min__pm_count = -1;

	int k_limit = l__pm_index.GetSize();
	int k;

	for(k=0; k<k_limit; k++)
	{
		int pm_index = l__pm_index[k];

		CString ch_data = xCH__PMx__IN_COUNT[pm_index]->Get__STRING();
		int cur__pm_count = atoi(ch_data);

		if((min__pm_count < 0)
		|| (cur__pm_count < min__pm_count))
		{
			min__pm_count = cur__pm_count;
			cur__pm_index = pm_index;  
		}
	}

	return cur__pm_index;
}

void CObj__DUAL_ARM_STD::_Get__IDLE_PM_INDEX_LIST_WITH_WAFER(CUIntArray& l_pm_index)
{
	l_pm_index.RemoveAll();

	// ...
	int k_limit = iPMx_SIZE;

	for(int k=0; k<k_limit; k++)
	{
		int pm_index = k;

		if(PMx__Check_Empty__SlotStatus(pm_index) > 0)
		{
			continue;
		}

		if(PMx__Is_Idle(pm_index) < 0)
		{
			continue;
		}

		l_pm_index.Add(pm_index);
	}
}
void CObj__DUAL_ARM_STD::_Get__PM_SORT_WITH_IDLE_TIME(const CUIntArray& l__pm_index_in, CUIntArray& l__pm_index_out)
{
	CUIntArray l__order_index;
	CUIntArray l__order_sec;

	// Before ...
	{
		int k_limit = l__pm_index_in.GetSize();
		for(int k=0; k<k_limit; k++)
		{
			int pm_index = l__pm_index_in[k];
			if(pm_index < 0)		continue;

			CString ch_data = xCH__PMx__IDLE_TIME[pm_index]->Get__STRING();
			int cur__idle_sec = atoi(ch_data);

			l__order_sec.Add(cur__idle_sec);
			l__order_index.Add(pm_index);
		}
	}

	// After ...
	{
		int k_limit = l__order_sec.GetSize();
		for(int k=0; k<k_limit; k++)
		{
			int max_sec   = l__order_sec[k];
			int max_index = l__order_index[k];

			for(int t=k+1; t<k_limit; t++)
			{
				int cmp_sec = l__order_sec[t];
				if(cmp_sec <= max_sec)			continue;

				int cmp_index = l__order_index[t];

				l__order_sec[t]   = max_sec;
				l__order_index[t] = max_index;

				max_sec   = cmp_sec;
				max_index = cmp_index;
			}

			l__order_sec[k]   = max_sec;
			l__order_index[k] = max_index;
		}
	}

	l__pm_index_out.Copy(l__order_index);
}
void CObj__DUAL_ARM_STD::_Get__PM_NAME_FROM_PM_ID(const CUIntArray& l_pm_id, CStringArray& l_pm_name)
{
	l_pm_name.RemoveAll();

	int k_limit = l_pm_id.GetSize();
	for(int k=0; k<k_limit; k++)
	{
		CString pm_name;
		pm_name.Format("PM%1d", k+1);

		l_pm_name.Add(pm_name);
	}
}
void CObj__DUAL_ARM_STD::_Get__PM_ID_FROM_PM_NAME(const CStringArray& l_pm_name, CUIntArray& l_pm_id)
{
	l_pm_id.RemoveAll();

	int k_limit = l_pm_name.GetSize();
	for(int k=0; k<k_limit; k++)
	{
		int pm_index = Macro__Get_PMC_INDEX(l_pm_name[k]);
		if(pm_index < 0)			continue;

		int pm_id = pm_index + 1;
		l_pm_id.Add(pm_id);
	}
}
