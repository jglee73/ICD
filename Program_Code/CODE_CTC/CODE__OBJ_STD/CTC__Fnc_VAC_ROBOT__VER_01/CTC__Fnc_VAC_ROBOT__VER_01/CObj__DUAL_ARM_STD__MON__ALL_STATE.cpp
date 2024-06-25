#include "StdAfx.h"
#include "CObj__DUAL_ARM_STD.h"

#include "Macro_Function.h"


#define  NEXT__LOOP					throw 1

#define STR__ROBOT_NAME				"VAC_RB1"


// ...
void CObj__DUAL_ARM_STD
::Mon__ALL_STATE(CII_OBJECT__VARIABLE* p_variable,
				 CII_OBJECT__ALARM* p_alarm)
{
	double pmx_front_sec = -1.0;
	double cur_sec;

	CString var_data;

	while(1)
	{
		p_variable->Wait__SINGLE_OBJECT(0.25);

		// ...
		{
			xCH_CFG__PHY_FRONT_TIME->Get__DATA(var_data);
			cur_sec = atof(var_data);

			if(cur_sec != pmx_front_sec)
			{
				pmx_front_sec = cur_sec;

				// ...
				xCH_CFG__PHY_CONCURRENT_MARGIN_TIME->Get__DATA(var_data);
				pmx_front_sec += atof(var_data);

				xSCH_MATERIAL_CTRL->Set__PMx_FRONT_TIME(pmx_front_sec);
			}
		}

		Fnc__CFG_PARAMETER(p_variable);
		Fnc__LBx_CFG(p_variable);
		Fnc__SYSTEM_CHECK(p_variable, p_alarm);

		Fnc__PMx_LEAK_CHECK_CTRL(p_variable,p_alarm);
		Fnc__PMx_AUTO_PM_CTRL(p_variable,p_alarm);
	}
}

void CObj__DUAL_ARM_STD
::Fnc__CFG_PARAMETER(CII_OBJECT__VARIABLE* p_variable)
{
	CUIntArray l_job_lp;
	CString var_data;

	SCH__Get_Job_LP_List(l_job_lp);

	int limit = l_job_lp.GetSize();
	int i;

	for(i=0; i<limit; i++)
	{
		var_data.Format("%1d", l_job_lp[i]);
		xEXT_CH__SCH__LP_JOB_LIST[i]->Set__DATA(var_data);
	}
	for(i=limit; i<CFG_LP_LIMIT; i++)
	{
		xEXT_CH__SCH__LP_JOB_LIST[i]->Set__DATA("");
	}
}
void CObj__DUAL_ARM_STD
::Fnc__LBx_CFG(CII_OBJECT__VARIABLE *p_variable)
{
	int i_limit = CFG_LLx_LIMIT;
	int i;

	for(i=0; i<i_limit; i++)
	{
		CString var_data = xEXT_CH__SCH_DB_LLx_SLOT_MAX[i]->Get__STRING();
		int slot_max = atoi(var_data);
				
		if(slot_max > CFG_LLx__SLOT_MAXx)		
			slot_max = CFG_LLx__SLOT_MAXx;
	
		iLLx_SLOT_MAX[i] = slot_max;
	}
}
void CObj__DUAL_ARM_STD
::Fnc__SYSTEM_CHECK(CII_OBJECT__VARIABLE *p_variable,
					CII_OBJECT__ALARM *p_alarm)
{
	if(xSCH_MATERIAL_CTRL->Get__MATERIAL_DB_SIZE() < 1)
	{
		for(int i=0; i<iPMx_SIZE; i++)
		{
			xCH__PMC_PROC_READY_FLAG[i]->Set__DATA("");
		}
	}
	else
	{
		CString obj_sts;

		for(int i=0; i<iPMx_SIZE; i++)
		{
			xCH__PMx__OBJ_VIRTUAL_STATUS[i]->Get__DATA(obj_sts);
			
			if((obj_sts.CompareNoCase(STR__STANDBY)  == 0)
			|| (obj_sts.CompareNoCase(STR__CTCINUSE) == 0))
			{
				xCH__PMC_PROC_READY_FLAG[i]->Set__DATA(STR__YES);
			}
			else
			{
				xCH__PMC_PROC_READY_FLAG[i]->Set__DATA("");
			}
		}			
	}

	if(xCH__VAC_RB_PAUSE_FLAG->Check__DATA(STR__YES) > 0)
	{
		int alarm_id = ALID__VAC_ROBOT__MOVE_PAUSED;
		CString r_act;

		p_alarm->Check__ALARM(alarm_id,r_act);
		p_alarm->Post__ALARM(alarm_id);
	}

	SCH__SYSTEM_CHECK(p_variable,p_alarm,iJOB_FLAG);
}

void CObj__DUAL_ARM_STD::
SCH__SYSTEM_CHECK(CII_OBJECT__VARIABLE *p_variable,
				  CII_OBJECT__ALARM *p_alarm,
				  const int job_flag)
{	
	// ...
	{
		CString r_act;
		int maint_flag = -1;

		if(VAC_RB__Check_CtcInUse_Or_Standby() < 0)
		{
			if(job_flag > 0)
			{
				p_alarm->Check__ALARM(ALID__JOB_ERROR__VAC_RB__NOT_STANDBY,r_act);
				p_alarm->Post__ALARM(ALID__JOB_ERROR__VAC_RB__NOT_STANDBY);
			}

			maint_flag = 1;
		}
		if(TMC_CHM__Check_CtcInUse_Or_Standby() < 0)
		{
			if(job_flag > 0)
			{
				p_alarm->Check__ALARM(ALID__JOB_ERROR__TMC_CHM__NOT_STANDBY,r_act);
				p_alarm->Post__ALARM(ALID__JOB_ERROR__TMC_CHM__NOT_STANDBY);
			}

			maint_flag = 1;
		}

		if(maint_flag > 0)
		{
			xSCH_MATERIAL_CTRL->Set__SYSTEM_STATUS__MAINT(sObject_Name);
		}
		else
		{
			xSCH_MATERIAL_CTRL->Clear__SYSTEM_STATUS__MAINT(sObject_Name);
		}

		if(xCH_CFG__MATERIAL_COUNT_FLAG->Check__DATA("ENABLE") > 0)
		{
			int i;

			for(i=0;i<iPMx_SIZE;i++)
			{
				xCH__PMx__AUTO_MATERIAL_COUNT_FLAG[i]->Set__DATA("ENABLE");
			}
		}
		else
		{
			int i;

			for(i=0;i<iPMx_SIZE;i++)
			{
				xCH__PMx__AUTO_MATERIAL_COUNT_FLAG[i]->Set__DATA("DISABLE");
			}
		}
	}

	// ...
	{
		int robot_material_count = 0;
		int pmc_material_count = 0;
		int material_count = 0;
		
		// RB Check ...
		{
			if(VAC_RB__Check_Occupied__A_Arm() > 0)
			{	
				material_count++;
				robot_material_count++;
			}
			if(VAC_RB__Check_Occupied__B_Arm() > 0)
			{
				material_count++;
				robot_material_count++;
			}
		}

		// LLx Check ...
		{
			for(int ll_i=0; ll_i<iLLx_LIMIT; ll_i++)
			{
				if(LLx__Check_All_Empty(ll_i) < 0)
				{
					material_count++;
				}
			}
		}

		// PMx Check ...
		{
			for(int pm_i=0; pm_i<iPMx_SIZE; pm_i++)
			{
				if(PMx__Check_Occupied__SlotStatus(pm_i) > 0)
				{
					material_count++;
					pmc_material_count++;
				}
			}
		}

		// ...
		{
			CString var_data;

			var_data.Format("%1d", material_count);
			xEXT_CH__VAC_AREA_MATERIAL_COUNT->Set__DATA(var_data);

			//
			var_data.Format("%1d", robot_material_count);
			xEXT_CH__VAC_ROBOT_OUT_MATERIAL_COUNT->Set__DATA(var_data);

			var_data.Format("%1d", pmc_material_count);
			xEXT_CH__PMC_MATERIAL_COUNT->Set__DATA(var_data);
		}
	}
}


