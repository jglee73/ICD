#include "StdAfx.h"
#include "CObj__DUAL_ARM_STD.h"

#include "Macro_Function.h"


#define  NEXT__LOOP					throw 1

#define STR__ROBOT_NAME				"VAC_RB1"


// ...
void CObj__DUAL_ARM_STD::
Mon__AUTO_CTRL(CII_OBJECT__VARIABLE *p_variable,
			   CII_OBJECT__ALARM *p_alarm)
{
	// ...
	{
		xSCH_MATERIAL_CTRL->Link__SERVICE_DB("");

		xSCH_MATERIAL_CTRL->Define__ROBOT_NAME(STR__ROBOT_NAME);
		xSCH_MATERIAL_CTRL->Define__ROBOT_ARM("A");
		xSCH_MATERIAL_CTRL->Define__ROBOT_ARM("B");

		xSCH_MATERIAL_CTRL->Register__SYSTEM_NAME(sObject_Name);
	}

	// ...
	{
		iJOB_FLAG = -1;
		iLBx_OUT_COUNT = 0;
		iActive__FROMT_PMo = -1;
	}

	// ...
	SCX__ASYNC_TIMER_CTRL x_timer__sys_idle;
	x_timer__sys_idle->REGISTER__COUNT_CHANNEL_NAME(sEXT_CH__SYSTEM_IDLE_CUR_TIME_SEC->Get__CHANNEL_NAME());

	// ...
	int loop_count =  0;
	int idle_flag  =  0;
	int abort_flag = -1;

	while(1)
	{
		while(xSCH_MATERIAL_CTRL->Get__MATERIAL_DB_SIZE() < 1)
		{
			if(idle_flag <= 0)
			{
				printf(" * CObj__DUAL_ARM_STD::Mon__AUTO_CTRL) ... \n");
				printf(" ** Idle Start ... \n");

				// ...
				{
					xSCH_MATERIAL_CTRL->Set__ROBOT_IDLE(STR__ROBOT_NAME);

					Set__TMC_CtcInUse_To_Standby();
					Set__PMx_CtcInUse_To_Standby();
				}

				// ...
				{
					xTIMER_CTRL__LLx_PICK_DELAY->STOP();

					xCH_CFG__PICK_DELAY_TIME->Set__DATA("0");
					xCH_CFG__PICK_COUNT_FROM_LBx->Set__DATA("0");
				}

				/*
				// Job End Ready ...
				if(dEXT_CH__CFG_JOB_END_READY_PMx_USE->Check__DATA(STR__ENABLE) > 0)
				{
					SCH__PMx_JOB_END_READY();
				}
				*/

				x_timer__sys_idle->START__COUNT_UP(99999);

				// Robot Home ...
				if(idle_flag < 0)
				{
					if(VAC_RB__Is_Available() > 0)
					{
						CString log_id = "Mon__AUTO_CTRL()";

						Run__ROBOT_OBJ("INIT", log_id);			
					}
				}
			}

			// ...
			{
				SCH__SYSTEM_INIT();

				iJOB_FLAG = -1;

				dCH__INFO_WAC_FRONT_ACTIVE->Set__DATA(STR__OFF);
				iActive__FROMT_PMo = -1;
			
				SCH__MATERIAL_CHECK();
				SCH__PMC_CHECK();
				SCH__MODULE_CHECK();
			}

			// System Idle ...
			{
				CString ch_data;

				ch_data = sEXT_CH__SYSTEM_IDLE_CUR_TIME_SEC->Get__STRING();
				double cur_sys_sec = atof(ch_data);
				double cfg_sys_sec = aEXT_CH__SYSTEM_IDLE_CFG_TIME_SEC->Get__VALUE();

				if(cur_sys_sec >= cfg_sys_sec)
				{
					int i_limit = iPMx_SIZE;
					int i;

					for(i=0; i<i_limit; i++)
					{
						if(dEXT_CH__SYSTEM_IDLE_CFG_PMx_USE[i]->Check__DATA(STR__YES) < 0)
							continue;

						ch_data = xCH__PMx__IDLE_TIME[i]->Get__STRING();
						double cur_pmc_sec = atof(ch_data);
						double cfg_pmc_sec = aEXT_CH__SYSTEM_IDLE_CFG_PMx_SEC[i]->Get__VALUE();

						if(cur_pmc_sec < cfg_pmc_sec)
							continue;
						
						if(PMx__Is_Available(i) < 0)
							continue;

						Run__PMx_OBJ(i, "IDLE_START");
					}
				}
			}

			// ...
			{
				Check_Alarm__Scheduler_Config(p_alarm);
			}

			sCH__SCH_LOOP_ID->Set__DATA("101");

			Sleep(100);
		
			// ...
			{
				idle_flag = 1;
				loop_count = 0;
			}
		}
		iJOB_FLAG = 1;

		
		// ...
		{
			sCH__SCH_LOOP_ID->Set__DATA("102");

			Sleep(1);
	
			if(VAC_RB__Is_Available() < 0)	
			{
				continue;
			}
			if(xSCH_MATERIAL_CTRL->Get__MATERIAL_DB_SIZE() < 1)
			{
				continue;
			}
		}


		if(idle_flag > 0)
		{
			idle_flag = -1;
			loop_count = 0;

			x_timer__sys_idle->STOP_ZERO();

			// ...
			{
				sCH__SCH_LOOP_ID->Set__DATA("111");

				SCH__SYSTEM_INIT();
			}

			// ...
			{
				int i;

				for(i=0; i<iPMx_SIZE; i++)
				{
					xCH__PMx__FIRST_WAFER_FLAG[i]->Set__DATA(STR__YES);
					
					sEXT_CH__PMx__CUR_MAIN_COUNT[i]->Set__DATA("");
				}

				for(i=0; i<iLLx_LIMIT; i++)
				{
					dEXT_CH__LLx__ACTIVE_VAC_AREA_CTRL[i]->Set__DATA(STR__OFF);
				}
			}

			// ...
			{
				xSCH_MATERIAL_CTRL->Set__ROBOT_BUSY(STR__ROBOT_NAME);			
			}

			try
			{
				sCH__SCH_LOOP_ID->Set__DATA("113");

				AUTO_CTRL__TMC_PUMP(p_variable);
			}
			catch(int err_flag)
			{

			}
		}

		// ...
		{
			loop_count++;

			if(loop_count > 10)
			{
				loop_count = 0;

				Check_Alarm__Scheduler_Config(p_alarm);
			}
		}

		if(xSCH_MATERIAL_CTRL->Check__SYSTEM_PAUSE() > 0)		continue;
		if(xSCH_MATERIAL_CTRL->Get__MATERIAL_DB_SIZE() < 1)		continue;
		if(xCH__VAC_RB_PAUSE_FLAG->Check__DATA(STR__YES) > 0)	continue;

		//  LBi -> VAC_RB
		try
		{
			int delay_flag = -1;

			if(xCH_CFG__PICK_DELAY_FLAG->Check__DATA(STR__ENABLE) > 0)
			{
				CString var_data;
				double delay_time;

				xCH_CFG__PICK_DELAY_TIME->Get__DATA(var_data);
				delay_time = atof(var_data);

				if(delay_time >= 1)		delay_flag = 1;
			}

			if(delay_flag < 0)
			{
				if(SCH__LBx__CHECK_ONLY_MODE() > 0)
				{
					sCH__SCH_LOOP_ID->Set__DATA("121");

					AUTO_CTRL__LBi_RB__ONLY_MODE(p_variable,p_alarm);
				}
				else
				{
					if(SCH__LBx__CHECK_1_SLOT_ALL_MODE(-1) > 0)
					{
						sCH__SCH_LOOP_ID->Set__DATA("122");

						AUTO_CTRL__LBi_RB__1_SLOT_ALL_MODE_NEW(p_variable,p_alarm);
					}
					else
					{
						sCH__SCH_LOOP_ID->Set__DATA("123");

						AUTO_CTRL__LBi_To_RB_With_Ready_PMC__ALL_MODE(p_variable,p_alarm);
					}
				}
			}
		}
		catch(int err_flag)
		{

		}

		// ...
		{
			sCH__SCH_LOOP_ID->Set__DATA("141");

			if(xSCH_MATERIAL_CTRL->Check__SYSTEM_PAUSE() > 0)		continue;
			if(xSCH_MATERIAL_CTRL->Get__MATERIAL_DB_SIZE() < 1)		continue;
			if(xCH__VAC_RB_PAUSE_FLAG->Check__DATA(STR__YES) > 0)	continue;
		}

		//  LBi -> PMx
		try
		{
			int delay_flag = -1;

			if(xCH_CFG__PICK_DELAY_FLAG->Check__DATA("ENABLE") > 0)
			{
				CString var_data;
				double delay_time;

				xCH_CFG__PICK_DELAY_TIME->Get__DATA(var_data);
				delay_time = atof(var_data);

				if(delay_time >= 1)		delay_flag = 1;
			}

			if(delay_flag < 0)
			{
				if(SCH__LBx__CHECK_ONLY_MODE() > 0)
				{
					sCH__SCH_LOOP_ID->Set__DATA("142");

					// AUTO_CTRL__LBi_PMx__ONLY_MODE(p_variable,p_alarm);
				}
				else
				{
					if(SCH__LBx__CHECK_1_SLOT_ALL_MODE(-1) > 0)
					{
						sCH__SCH_LOOP_ID->Set__DATA("143");

						AUTO_CTRL__LBi_PMx__1_SLOT_ALL_MODE(p_variable,p_alarm);
					}
					else
					{
						sCH__SCH_LOOP_ID->Set__DATA("144");

						// AUTO_CTRL__LBi_To_Ready_PMx__ALL_MODE(p_variable,p_alarm);
					}
				}
			}
		}
		catch(int err_flag)
		{

		}

		// ...
		{
			sCH__SCH_LOOP_ID->Set__DATA("151");

			if(xSCH_MATERIAL_CTRL->Check__SYSTEM_PAUSE() > 0)		continue;
			if(xSCH_MATERIAL_CTRL->Get__MATERIAL_DB_SIZE() < 1)		continue;
			if(xCH__VAC_RB_PAUSE_FLAG->Check__DATA(STR__YES) > 0)	continue;
		}

		//  VAC_RB -> PMx
		try
		{
			sCH__SCH_LOOP_ID->Set__DATA("152");

			AUTO_CTRL__RB_PMx(p_variable,p_alarm);
		}
		catch(int err_flag)
		{

		}

		// ...
		{
			sCH__SCH_LOOP_ID->Set__DATA("161");

			if(xSCH_MATERIAL_CTRL->Check__SYSTEM_PAUSE() > 0)		continue;
			if(xSCH_MATERIAL_CTRL->Get__MATERIAL_DB_SIZE() < 1)		continue;
			if(xCH__VAC_RB_PAUSE_FLAG->Check__DATA(STR__YES) > 0)	continue;
		}

		//  VAC_RB -> LLx_PROC
		try
		{
			sCH__SCH_LOOP_ID->Set__DATA("153.1");

			AUTO_CTRL__RB_TO_LLx_PROC(p_variable,p_alarm);
		}
		catch(int err_flag)
		{

		}

		//  LLx_PROC -> VAC_RB
		try
		{
			sCH__SCH_LOOP_ID->Set__DATA("153.2");

			AUTO_CTRL__LLx_PROC_TO_RB(p_variable,p_alarm);
		}
		catch(int err_flag)
		{

		}

		// ...
		{
			sCH__SCH_LOOP_ID->Set__DATA("154");

			if(xSCH_MATERIAL_CTRL->Check__SYSTEM_PAUSE() > 0)		continue;
			if(xSCH_MATERIAL_CTRL->Get__MATERIAL_DB_SIZE() < 1)		continue;
			if(xCH__VAC_RB_PAUSE_FLAG->Check__DATA(STR__YES) > 0)	continue;
		}

		//  PMo -> VAC_RB
		try
		{
			sCH__SCH_LOOP_ID->Set__DATA("162");

			AUTO_CTRL__PMo_RB(p_variable,p_alarm);
		}
		catch(int err_flag)
		{

		}

		// ...
		{
			sCH__SCH_LOOP_ID->Set__DATA("171");

			if(xSCH_MATERIAL_CTRL->Check__SYSTEM_PAUSE() > 0)		continue;
			if(xSCH_MATERIAL_CTRL->Get__MATERIAL_DB_SIZE() < 1)		continue;
			if(xCH__VAC_RB_PAUSE_FLAG->Check__DATA(STR__YES) > 0)	continue;
		}

		//  PMx -> VAC_RB
		try
		{
			sCH__SCH_LOOP_ID->Set__DATA("172");

			AUTO_CTRL__PMx_RB(p_variable,p_alarm);
		}
		catch(int err_flag)
		{

		}

		// ...
		{
			sCH__SCH_LOOP_ID->Set__DATA("181");

			if(xSCH_MATERIAL_CTRL->Check__SYSTEM_PAUSE() > 0)		continue;
			if(xSCH_MATERIAL_CTRL->Get__MATERIAL_DB_SIZE() < 1)		continue;
			if(xCH__VAC_RB_PAUSE_FLAG->Check__DATA(STR__YES) > 0)	continue;
		}

		//  VAC_RB -> LBo
		try
		{
			if(SCH__LBx__CHECK_ONLY_MODE() > 0)
			{
				sCH__SCH_LOOP_ID->Set__DATA("191");

				AUTO_CTRL__RB_LBo__ONLY_MODE(p_variable,p_alarm);
			}
			else
			{
				if(SCH__LBx__CHECK_1_SLOT_ALL_MODE(-1) > 0)
				{
					sCH__SCH_LOOP_ID->Set__DATA("192");

					AUTO_CTRL__RB_LBo__1_SLOT_ALL_MODE(p_variable,p_alarm);
				}
				else
				{
					sCH__SCH_LOOP_ID->Set__DATA("193");

					AUTO_CTRL__RB_LBo__ALL_MODE(p_variable,p_alarm,loop_count);
				}
			}
		}
		catch(int err_flag)
		{
			if(err_flag == 101)
			{
				loop_count = 0;
			}
		}

		/*
		// ...
		{
			sCH__SCH_LOOP_ID->Set__DATA("201");

			if(xSCH_MATERIAL_CTRL->Check__SYSTEM_PAUSE() > 0)		continue;
			if(xSCH_MATERIAL_CTRL->Get__MATERIAL_DB_SIZE() < 1)		continue;
			if(xCH__VAC_RB_PAUSE_FLAG->Check__DATA(STR__YES) > 0)	continue;
		}

		//  VAC_RB : WAC Position
		try
		{
			sCH__SCH_LOOP_ID->Set__DATA("202");

			AUTO_CTRL__FRONT_WAC_POS(p_variable);
		}
		catch(int err_flag)
		{

		}
		*/

		/*
		// ...
		{
			sCH__SCH_LOOP_ID->Set__DATA("211");

			if(xSCH_MATERIAL_CTRL->Check__SYSTEM_PAUSE() > 0)		continue;
			if(xSCH_MATERIAL_CTRL->Get__MATERIAL_DB_SIZE() < 1)		continue;
			if(xCH__VAC_RB_PAUSE_FLAG->Check__DATA(STR__YES) > 0)	continue;
		}

		//  VAC_RB : PMo
		try
		{
			sCH__SCH_LOOP_ID->Set__DATA("212");

			AUTO_CTRL__FRONT_PMo(p_variable);
		}
		catch(int err_flag)
		{

		}
		*/

		// ...
		{
			sCH__SCH_LOOP_ID->Set__DATA("221");

			if(xSCH_MATERIAL_CTRL->Check__SYSTEM_PAUSE() > 0)		continue;
			if(xSCH_MATERIAL_CTRL->Get__MATERIAL_DB_SIZE() < 1)		continue;
			if(xCH__VAC_RB_PAUSE_FLAG->Check__DATA(STR__YES) > 0)	continue;
		}

		// LLx (Idle State) : Exception
		try
		{
			if(loop_count == 3)
			{
				sCH__SCH_LOOP_ID->Set__DATA("222");

				AUTO_CTRL__LLx_Exception_In_Idle_State(p_variable, p_alarm);
			}
		}
		catch(int err_flag)
		{
			
		}

		// ...
		{
			sCH__SCH_LOOP_ID->Set__DATA("231");

			if(xSCH_MATERIAL_CTRL->Check__SYSTEM_PAUSE() > 0)		continue;
			if(xSCH_MATERIAL_CTRL->Get__MATERIAL_DB_SIZE() < 1)		continue;
			if(xCH__VAC_RB_PAUSE_FLAG->Check__DATA(STR__YES) > 0)	continue;
		}

		// LLx (Only_Mode) : Exception
		try
		{
			if(loop_count == 3)
			{
				sCH__SCH_LOOP_ID->Set__DATA("232");

				AUTO_CTRL__LLx_Exception_Of_Only_Mode(p_variable, p_alarm);
			}
		}
		catch(int err_flag)
		{
			
		}

		// LLx (All_Mode) : Exception
		try
		{
			if(loop_count == 3)
			{
				sCH__SCH_LOOP_ID->Set__DATA("233");

				AUTO_CTRL__LLx_Exception_Of_All_Mode(p_variable, p_alarm);
			}
		}
		catch(int err_flag)
		{

		}

		// DeadLock Check ...
		try
		{
			if(loop_count == 3)
			{
				AUTO_CTRL__Deadlock_Report(p_variable, p_alarm);
			}
		}
		catch(int err_flag)
		{

		}
	}
}

//-----------------------------------------------------------------------------------------
void CObj__DUAL_ARM_STD::
SCH__SYSTEM_INIT()
{
	int i;

	// ...
	{
		iLBx_OUT_COUNT = 0;
		iPMx_IN_COUNT = 0;
	}
	
	for(i=0;i<CFG_PM_LIMIT;i++)
	{
		xCH__PMx__IN_COUNT[i]->Set__DATA("0");
		dCH__VAC_RB__INFO_PM_RESERVE_X[i]->Set__DATA(STR__OFF);
	}

	for(i=0;i<iPMx_SIZE;i++)
	{
		xCH__PMx__X_PICK_FLAG[i]->Set__DATA("");
		xCH__PMx__X_PLACE_FLAG[i]->Set__DATA("");

		xCH__PMx__NEXT_PROCESS[i]->Set__DATA("");
	}
}

void CObj__DUAL_ARM_STD::SCH__MATERIAL_CHECK()
{
	if(VAC_RB__Check_Occupied__A_Arm() > 0)
	{
		xSCH_MATERIAL_CTRL->Set__SYSTEM_STATUS__MATERIAL_STS(sObject_Name,"VAC_RB-A");		
	}
	else
	{
		xSCH_MATERIAL_CTRL->Clear__SYSTEM_STATUS__MATERIAL_STS(sObject_Name,"VAC_RB-A");
	}
	if(VAC_RB__Check_Occupied__B_Arm() > 0)
	{
		xSCH_MATERIAL_CTRL->Set__SYSTEM_STATUS__MATERIAL_STS(sObject_Name,"VAC_RB-B");		
	}
	else
	{
		xSCH_MATERIAL_CTRL->Clear__SYSTEM_STATUS__MATERIAL_STS(sObject_Name,"VAC_RB-B");
	}

	// ...
	{
		CString pm_name;
		
		for(int i=0; i<iPMx_SIZE; i++)
		{
			pm_name.Format("PM%1d",i+1);

			if(xEXT_CH__CFG__PMx_USE[i]->Check__DATA(STR__ENABLE) < 0)
			{
				if(PMx__Check_Occupied__SlotStatus(i) > 0)
				{
					CString sch_name;
					sch_name.Format("%s-1", pm_name);

					if(xSCH_MATERIAL_CTRL->Check__MATERIAL_DB(sch_name) > 0)
						xSCH_MATERIAL_CTRL->Set__SYSTEM_STATUS__MATERIAL_STS(sObject_Name, pm_name);		
					else
						xSCH_MATERIAL_CTRL->Clear__SYSTEM_STATUS__MATERIAL_STS(sObject_Name, pm_name);		
				}
				else
				{
					xSCH_MATERIAL_CTRL->Clear__SYSTEM_STATUS__MATERIAL_STS(sObject_Name, pm_name);		
				}
				continue;
			}

			if(PMx__Check_Occupied__SlotStatus(i) > 0)
			{
				xSCH_MATERIAL_CTRL->Set__SYSTEM_STATUS__MATERIAL_STS(sObject_Name, pm_name);		
			}
			else
			{
				xSCH_MATERIAL_CTRL->Clear__SYSTEM_STATUS__MATERIAL_STS(sObject_Name, pm_name);		
			}
		}
	}
}
void CObj__DUAL_ARM_STD::SCH__PMC_CHECK()
{
	//.....
	CString pm_name;
	CString obj_sts;
	int i;
		
	for(i=0;i<iPMx_SIZE;i++)
	{
		pm_name.Format("PM%1d",i+1);

		//.....
		xCH__PMx__OBJ_STATUS[i]->Get__DATA(obj_sts);
			
		if(((obj_sts.CompareNoCase("CTCINUSE") == 0)||(obj_sts.CompareNoCase("STANDBY") == 0))
		&& (xEXT_CH__CFG__PMx_USE[i]->Check__DATA("ENABLE") > 0))
		{
			xSCH_MATERIAL_CTRL->Set__SYSTEM_STATUS__PMC_STS(sObject_Name,pm_name);
		}
		else
		{
			xSCH_MATERIAL_CTRL->Clear__SYSTEM_STATUS__PMC_STS(sObject_Name,pm_name);
		}
	}
}
void CObj__DUAL_ARM_STD::SCH__MODULE_CHECK()
{
	if(TMC_CHM__Is_Available() > 0)
	{
		xSCH_MATERIAL_CTRL->Set__SYSTEM_STATUS__MODULE_AVAILABLE(sObject_Name,"TMC_CHM");		
	}
	else
	{
		xSCH_MATERIAL_CTRL->Set__SYSTEM_STATUS__MODULE_NOT_AVAILABLE(sObject_Name,"TMC_CHM");		
	}

	if(VAC_RB__Is_Available() > 0)
	{
		xSCH_MATERIAL_CTRL->Set__SYSTEM_STATUS__MODULE_AVAILABLE(sObject_Name,"VAC_RB");		
	}
	else
	{
		xSCH_MATERIAL_CTRL->Set__SYSTEM_STATUS__MODULE_NOT_AVAILABLE(sObject_Name,"VAC_RB");		
	}
}


//-----------------------------------------------------------------------------------------
int  CObj__DUAL_ARM_STD::
Fnc__Get_PMx_List(const CString& sch_name, CStringArray& l_pm_name)
{
	CStringArray l_pm_rcp;

	return Fnc__Get_PMx_List(sch_name, l_pm_name,l_pm_rcp);
}
int  CObj__DUAL_ARM_STD::
Fnc__Get_PMx_List(const CString& sch_name, CStringArray& l_pm_name,CStringArray& l_pm_rcp)
{
	l_pm_name.RemoveAll();
	l_pm_rcp.RemoveAll();

	// ...
	int active__lp_normal = 1;

	if(LPx__Get_Process_Type__Normal(sch_name, active__lp_normal) < 0)
	{
		return -11;
	}

	// ...
	CStringArray l_pm;
	CStringArray l_rcp;

	xSCH_MATERIAL_CTRL->Get__CUR_PROCESS_INFO(sch_name, l_pm,l_rcp);

	// ...
	CString pm_name;

	int i_limit = l_pm.GetSize();
	for(int i=0; i<i_limit; i++)
	{
		int pm_i = SCH__CHECK_PM_INDEX(l_pm[i]);

		if(pm_i <  0)				continue;
		if(pm_i >= iPMx_SIZE)		continue;

		if(active__lp_normal > 0)
		{
			if(xEXT_CH__PMx_PROCESS_TYPE[pm_i]->Check__DATA(STR__NORMAL) < 0)
			{
				continue;
			}
		}
		else
		{
			if(xEXT_CH__PMx_PROCESS_TYPE[pm_i]->Check__DATA(STR__TEST) < 0)
			{
				continue;
			}
		}

		if(PMx__Is_Available(pm_i) < 0)				continue;
		if(PMx__Check_Empty__SlotStatus(pm_i) < 0)	continue;

		pm_name.Format("PM%1d", pm_i+1);
		l_pm_name.Add(pm_name);
		l_pm_rcp.Add(l_rcp[i]);
	}

	if(l_pm_name.GetSize() < 1)		return -1;
	return 1;
}

CString CObj__DUAL_ARM_STD::
Fnc__Get_PM_Rcp(const CString& pm_name, const CStringArray& l_pm_name,const CStringArray& l_pm_rcp)
{
	int i_limit = l_pm_name.GetSize();

	for(int i=0; i<i_limit; i++)
	{
		if(pm_name.CompareNoCase(l_pm_name[i]) != 0)		continue;

		return l_pm_rcp[i];
	}
	return "";
}
int  CObj__DUAL_ARM_STD::
Fnc__Get_Find_Index(const CString& str_name, const CStringArray& l_name)
{
	int i_limit = l_name.GetSize();

	for(int i=0; i<i_limit; i++)
	{
		if(str_name.CompareNoCase(l_name[i]) == 0)			return i;
	}
	return -1;
}

int  CObj__DUAL_ARM_STD::
AUTO_CTRL__LBi_RB__ONLY_MODE(CII_OBJECT__VARIABLE *p_variable, CII_OBJECT__ALARM *p_alarm)
{
	if(dCH__VAC_RB__CFG_LLx_CTRL_ONLY_INPUT_OUTPUT_MODE->Check__DATA(STR__DUAL) > 0)
	{
		_AUTO_CTRL__LBi_RB__ONLY_MODE_WITH_DUAL_TYPE(p_variable, p_alarm);
	}
	else
	{
		_AUTO_CTRL__LBi_RB__ONLY_MODE(p_variable, p_alarm);
	}
	return 1;
}
int  CObj__DUAL_ARM_STD::
_AUTO_CTRL__LBi_RB__ONLY_MODE_WITH_DUAL_TYPE(CII_OBJECT__VARIABLE *p_variable, CII_OBJECT__ALARM *p_alarm)
{
	DECLARE__EXT_CTRL(p_variable);

	CString log_id = "AUTO_CTRL__LBi_RB__ONLY_MODE_WITH_DUAL_TYPE()";

	if(dCH__VAC_RB__CFG_DUAL_ARM_MOVING_AT_THE_SAME_TIME->Check__DATA(STR__ENABLE) > 0)
	{
		CStringArray l_arm;	
		Get__Arm_List_To_Process(l_arm);

		if(l_arm.GetSize() > 0)			
		{
			NEXT__LOOP;
		}
	}

	if(dCH__VAC_RB__CFG_PICK_WAFER_CONDITION->Check__DATA(STR__ONLY_PROCESSED) > 0)
	{
		if(VAC_RB__Check_Occupied__A_Arm() > 0)
		{
			if(xCH__VAC_RB__SLOT01_STATUS->Check__DATA(STR__MAPPED) < 0)			NEXT__LOOP;
		}
		if(VAC_RB__Check_Occupied__B_Arm() > 0)
		{
			if(xCH__VAC_RB__SLOT02_STATUS->Check__DATA(STR__MAPPED) < 0)			NEXT__LOOP;
		}
	}

	// ...
	bool active__pm_err = true;

	// LL Check ...
	{
		int total_count = 0;
		int ll_i;

		for(ll_i=0; ll_i<iLLx_LIMIT; ll_i++)
		{
			int cur_count = LLx__Get_Occupied__InSlot_Count(ll_i);
			if(cur_count < 0)		continue;

			total_count += cur_count;
		}

		if(total_count < 1)
		{
			NEXT__LOOP;
		}

		for(ll_i=0; ll_i<iLLx_LIMIT; ll_i++)
		{
			CUIntArray l__ll_slot_id;

			if(LLx__Get_Occupied__InSlot(ll_i, l__ll_slot_id) < 0)
			{
				continue;
			}

			// ...
			CString para__ll_name;
			CString para__ll_slot;

			int s_limit = l__ll_slot_id.GetSize();
			if(s_limit < 0)		continue;

			for(int s_i=0; s_i<s_limit; s_i++)
			{
				int ll_s_id = l__ll_slot_id[s_i];

				// ...
				CString empty_arm;

				if(VAC_RB__Get_Empty__Arm_Type_With_LL_Constraint(empty_arm, ll_s_id) < 0)
				{
					continue;
				}

				// ...
				para__ll_name = Get__LLx_NAME(ll_i);
				para__ll_slot.Format("%1d", ll_s_id);

				CString sch_name;
				sch_name.Format("%s-%s", para__ll_name,para__ll_slot);

				// ...
				CStringArray l__pm_name;
				CStringArray l__pm_rcp;

				if(xSCH_MATERIAL_CTRL->Get__CUR_PROCESS_INFO(sch_name, l__pm_name,l__pm_rcp) < 0)
				{
					continue;
				}

				// ...
				int active__lp_normal = 1;

				if(LPx__Get_Process_Type__Normal(sch_name, active__lp_normal) < 0)
				{
					continue;
				}

				// ...				
				int k_limit = l__pm_name.GetSize();
				
				for(int k=0; k<k_limit; k++)
				{
					CString pmc_name  = l__pm_name[k];

					if(!VAC_RB__Check_Empty__Arm_Type_With_PMx_Constraint(empty_arm, pmc_name))
					{
						continue;
					}

					int pm_index = Macro__Get_PMC_INDEX(pmc_name);
					if(pm_index < 0)		continue;

					if(active__lp_normal > 0)
					{
						if(xEXT_CH__PMx_PROCESS_TYPE[pm_index]->Check__DATA(STR__NORMAL) < 0)
						{
							continue;
						}
					}
					else
					{
						if(xEXT_CH__PMx_PROCESS_TYPE[pm_index]->Check__DATA(STR__TEST) < 0)
						{
							continue;
						}
					}

					if(PMx__Is_Ready(pm_index) > 0)	
					{
						// PMx Waiting ...
						active__pm_err = false;
						break;
					}
				}			

				if(!active__pm_err)		break;
			}

			if(!active__pm_err)		break;
		}
	}

	// ...
	int wfr__pick_count = 0;
	int wfr__ll_index = -1;

	int ll_limit = iLLx_LIMIT;
	int ll_i;

	for(ll_i=0; ll_i<ll_limit; ll_i++)
	{
		if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_i]->Check__DATA(LBx_MODE__ONLY_INPUT) < 0)
		{
			continue;
		}

		if(xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Check__DATA(STR__OUT) < 0)
		{
			continue;
		}

		if(xEXT_CH__SCH_DB_LLx_USE_FLAG[ll_i]->Check__DATA(STR__DISABLE) > 0)
		{
			continue;
		}

		if(LLx__Is_Available(ll_i) < 0)	
		{
			continue;
		}

		// ...
		CUIntArray l__ll_slot_id;

		if(LLx__Get_Occupied__InSlot(ll_i, l__ll_slot_id) < 0)
		{
			continue;
		}

		if(SCH__LLx_CHECK_PRESSURE(ll_i, p_variable,p_alarm, 1) < 0)
		{
			continue;
		}

		wfr__ll_index = ll_i;

		// ...
		CString empty_arm;

		int k_limit = l__ll_slot_id.GetSize();
		int k;

		for(k=0; k<k_limit; k++)
		{
			int ll_slot_id = l__ll_slot_id[k];

			if(VAC_RB__Get_Empty__Arm_Type_With_LL_Constraint(empty_arm, ll_slot_id) < 0)
			{
				continue;
			}

			// ...
			CString para__ll_name;
			CString para__ll_slot;

			para__ll_name = Get__LLx_NAME(ll_i);
			para__ll_slot.Format("%1d", ll_slot_id);

			CString sch_name;
			sch_name.Format("%s-%s", para__ll_name,para__ll_slot);

			// ...
			int active__lp_normal = 1;

			if(LPx__Get_Process_Type__Normal(sch_name, active__lp_normal) < 0)
			{
				continue;
			}

			// ...
			CStringArray l__pmc_name;

			// ...
			{
				CStringArray l__pmc_rcp;
				xSCH_MATERIAL_CTRL->Get__CUR_PROCESS_INFO(sch_name, l__pmc_name,l__pmc_rcp);

				int trg__pmc_id = Get__PM_ID_OF_SCH_USER_DEF_TRG_MDx(log_id, sch_name);
				if(trg__pmc_id > 0)
				{
					int pmc_index = trg__pmc_id - 1;

					int r_flag = Macro__Check_PM_Index_With_StringArray(pmc_index, l__pmc_name);
					if(r_flag > 0)		
					{						
						CString str__pmc_name;
						str__pmc_name.Format("PM%1d", trg__pmc_id);

						l__pmc_name.RemoveAll();
						l__pmc_name.Add(str__pmc_name);
					}
				}
			}

			//====================================================
			// ...
			{
				int count__wfr_type = -1;

				int t_limit = l__pmc_name.GetSize();
				for(int t=0; t < t_limit; t++)
				{
					CString pmc_name = l__pmc_name[t];

					int pm_index = Macro__Get_PMC_INDEX(pmc_name);
					if(pm_index < 0)		continue;

					if(active__lp_normal < 0)
					{
						if(xEXT_CH__PMx_PROCESS_TYPE[pm_index]->Check__DATA(STR__TEST) < 0)
						{
							continue;
						}
					}

					count__wfr_type = 1;
					break;
				}

				if(count__wfr_type < 0)
				{
					active__lp_normal = 1;
				}
			}

			//====================================================
			int t_limit = l__pmc_name.GetSize();
			for(int t=0; t < t_limit; t++)
			{
				CString pmc_name = l__pmc_name[t];

				if(t_limit > 1)
				{
					if(!active__pm_err)
					{
						if(!VAC_RB__Check_Empty__Arm_Type_With_PMx_Constraint(empty_arm, pmc_name))
						{
							continue;
						}
					}
				}
				
				int pm_index = Macro__Get_PMC_INDEX(pmc_name);
				if(pm_index < 0)		continue;

				if(dCH__VAC_RB__INFO_PM_RESERVE_X[pm_index]->Check__DATA(STR__ON) > 0)
				{
					continue;
				}

				if(PMx__Is_Ready(pm_index) < 0)
				{
					continue;
				}

				if(PMx__Check_Empty__SlotStatus(pm_index) < 0)
				{
					continue;
				}

				if(active__lp_normal > 0)
				{
					if(xEXT_CH__PMx_PROCESS_TYPE[pm_index]->Check__DATA(STR__NORMAL) < 0)
					{
						continue;
					}
				}
				else
				{
					if(xEXT_CH__PMx_PROCESS_TYPE[pm_index]->Check__DATA(STR__TEST) < 0)
					{
						continue;
					}
				}

				// ...
				{
					bool active__pm_check = false;
					int count__pm_check = -1;

					while(1)
					{
						if(xSCH_MATERIAL_CTRL->Get__MATERIAL_DB_SIZE() < 1)			NEXT__LOOP;
				
						if(PMx__Is_Available(pm_index) > 0)
						{
							active__pm_check = true;
							break;
						}

						if(count__pm_check < 0)
						{
							if(xCH__PMx__PROCESS_STATUS[pm_index]->Check__DATA("WAIT") > 0)
							{
								CString var_data = xCH__PMx__CURRENT_PRC_TIME[pm_index]->Get__STRING();
								double cur__proc_sec = atof(var_data);

								if(cur__proc_sec > 3.0)			break;
								count__pm_check = 30;	// 3.0 sec
							}
							else
							{
								count__pm_check = 5;	// 0.5 sec
							}
						}
						if(count__pm_check < 1)
						{
							break;
						}

						Sleep(90);

						if(xSCH_MATERIAL_CTRL->Check__SYSTEM_PAUSE() > 0)			continue;
						if(xCH__VAC_RB_PAUSE_FLAG->Check__DATA(STR__YES) > 0)		continue;

						count__pm_check--;
					}

					if(!active__pm_check)
					{
						continue;
					}
				}

				// ...
				bool active__ex_pick = false;

				/*
				int slot_count = LLx__Get_Occupied__InSlot_Count(ll_i);
				if(slot_count > 1)		active__ex_pick = true;
				*/
				
				int act_flag = SCH__PICK_MODULE(p_variable,p_alarm, log_id, active__ex_pick, empty_arm, para__ll_name,para__ll_slot, sch_name);

				if(VAC_RB__Check_Occupied__Arm_Type(empty_arm) > 0)
				{
					xSCH_MATERIAL_CTRL->Pick__From_MODULE(sch_name, empty_arm);
				}

				if(act_flag < 0)			NEXT__LOOP;

				dCH__VAC_RB__INFO_PM_RESERVE_X[pm_index]->Set__DATA(STR__ON);

				wfr__pick_count++;
				break;
			}
		}
	}

	if(wfr__pick_count > 0)
	{
		ll_i = wfr__ll_index;

		if(LLx__Check_All_Empty(ll_i) > 0)
		{
			if(SCH_RUN__LLx_VENT(ll_i, log_id, "1") > 0)
			{
				xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Set__DATA(STR__IN);
			}
		}
	}

	NEXT__LOOP;
}
int  CObj__DUAL_ARM_STD::
_AUTO_CTRL__LBi_RB__ONLY_MODE(CII_OBJECT__VARIABLE *p_variable, CII_OBJECT__ALARM *p_alarm)
{
	DECLARE__EXT_CTRL(p_variable);

	CString log_id = "AUTO_CTRL__LBi_RB__ONLY_MODE()";

	// Check : Wafer State ...
	if(dCH__VAC_RB__CFG_PICK_WAFER_CONDITION->Check__DATA(STR__ONLY_PROCESSED) > 0)
	{
		if(VAC_RB__Check_Occupied__A_Arm() > 0)
		{
			if(xCH__VAC_RB__SLOT01_STATUS->Check__DATA(STR__MAPPED) < 0)			NEXT__LOOP;
		}
		if(VAC_RB__Check_Occupied__B_Arm() > 0)
		{
			if(xCH__VAC_RB__SLOT02_STATUS->Check__DATA(STR__MAPPED) < 0)			NEXT__LOOP;
		}
	}

	// ...
	CString para_module;
	CString para_slot;
	int slot_id;

	CUIntArray l__para_slot_id;

	// LLx ...
	{
		CUIntArray   l__ll_cnt;
		CStringArray l__ll_name;
		CStringArray l__ll_slot;

		int ll_limit = iLLx_LIMIT;
		int ll_i;

		for(ll_i=0; ll_i<ll_limit; ll_i++)
		{
			if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_i]->Check__DATA(LBx_MODE__ONLY_INPUT) < 0)
			{
				continue;
			}

			if(xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Check__DATA(STR__OUT) < 0)
			{
				continue;
			}

			if(LLx__Is_Available(ll_i) < 0)							continue;
			if(LLx__Get_Occupied__InSlot(ll_i, slot_id) < 0)		continue;

			if(SCH__LLx_CHECK_PRESSURE(ll_i, p_variable,p_alarm,1) < 0)
			{
				continue;
			}

			// ...
			{
				CString ch_data;

				ch_data = xEXT_CH__LLx__IN_COUNT[ll_i]->Get__STRING();
				int count__ll_in = atoi(ch_data);
				l__ll_cnt.Add(count__ll_in);

				ch_data = Get__LLx_NAME(ll_i);
				l__ll_name.Add(ch_data);

				ch_data.Format("%1d",slot_id);
				l__ll_slot.Add(ch_data);
			}
		}

		if(l__ll_name.GetSize() < 1)
		{
			NEXT__LOOP;
		}

		Change_Order__In_Count_Of_LLx(l__ll_cnt, l__ll_name, l__ll_slot);

		// ...
		{
			para_module = l__ll_name[0];
			para_slot   = l__ll_slot[0];
		}

		// ...
		CString sch_name;
		sch_name.Format("%s-%s", para_module,para_slot);

		if(_CHECK__Ready_PMC_OF_SCH_NAME(sch_name, true) < 0)
		{
			NEXT__LOOP;
		}

		int trg__goto_pm_id = Get__PM_ID_OF_SCH_USER_DEF_TRG_MDx(log_id, sch_name);
		if(trg__goto_pm_id > 0)
		{

		}
		else
		{
			CString def_name = USER_DEF__GOTO_PM;
			CString def_data;

			xSCH_MATERIAL_CTRL->Get__MATERIAL_USER_INFO(sch_name, def_name,def_data);

			int trg__pmc_index = Macro__Get_PMC_INDEX(def_data);
			if(trg__pmc_index >= 0)
			{
				if(PMx__Is_Ready(trg__pmc_index) > 0)
				{
					NEXT__LOOP;
				}
			}

			if(dCH__VAC_RB__CFG_PICK_WAFER_CONDITION->Check__DATA(STR__ONLY_PROCESSED) > 0)
			{
				if(_CHECK__Ready_PMC_OF_SCH_NAME(sch_name) < 0)
				{
					NEXT__LOOP;
				}
			}
		}
	}

	// ...
	bool active__pmc_ready = false;

	// CHECK : PMC READY ...
	{
		CString sch_name;
		sch_name.Format("%s-%s", para_module,para_slot);

		if(VAC_RB__Check_Empty__Dual_Arm() > 0)
		{
			if(_CHECK__Ready_PMC_OF_SCH_NAME(sch_name, true) > 0)
			{
				active__pmc_ready = true;
			}
		}
		else
		{
			if(_CHECK__Ready_PMC_OF_SCH_NAME(sch_name) > 0)
			{
				active__pmc_ready = true;
			}
		}
	}

	/*
	if(xCH_CFG__SCH_LLx_EXCHANGE_WHEN_PMC_EMPTY->Check__DATA(STR__ENABLE) > 0)
	{
		if(active__pmc_ready)
		{
			
		}
		else
		{
			NEXT__LOOP;
		}
	}
	*/

	// CHECK : WAFER ON ARN ...
	if(active__pmc_ready)
	{
		CString arm_type;

		if(VAC_RB__Get_Occupied__Arm_Type(arm_type) > 0)
		{
			if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(arm_type) > 0)
			{
				if(_CHECK__Ready_PMC_OF_SCH_NAME(arm_type) > 0)
				{
					NEXT__LOOP;
				}
			}
		}
	}
	else
	{
		NEXT__LOOP;
	}

	// ...
	CString empty_arm;

	if(VAC_RB__Get_Empty__Arm_Type_From_LBx(empty_arm) < 0)
	{
		NEXT__LOOP;
	}

	// LBi -> RB : ONLY_MODE
	{
		CString sch_module;
		sch_module.Format("%s-%s", para_module,para_slot);

		int act_flag = SCH__PICK_MODULE(p_variable,p_alarm, log_id, false, empty_arm, para_module,para_slot,sch_module);
		int move_flag = -1;

		if(act_flag < 0)			move_flag = SCH__Check_Material_Pick(empty_arm);
		else						move_flag = 1;

		if(move_flag > 0)			xSCH_MATERIAL_CTRL->Pick__From_MODULE(sch_module, empty_arm);

		if(act_flag  < 0)
		{
			NEXT__LOOP;
		}
	}

	// LLo VENT ...
	{
		int ll_i = Get__LLx_INDEX(para_module);
		if(ll_i >= 0)
		{
			if(LLx__Check_All_Empty(ll_i) > 0)
			{
				if(SCH_RUN__LLx_VENT(ll_i, log_id, "1") > 0)
				{
					xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Set__DATA("IN");
				}
			}
		}
	}

	NEXT__LOOP;
}

int  CObj__DUAL_ARM_STD::
AUTO_CTRL__LBi_To_RB_With_Ready_PMC__ALL_MODE(CII_OBJECT__VARIABLE *p_variable,
											  CII_OBJECT__ALARM *p_alarm)
{
	DECLARE__EXT_CTRL(p_variable);

	CString log_id = "AUTO_CTRL__LBi_To_RB_With_Ready_PMC__ALL_MODE() ... \n";

	// ...
	{
		CStringArray l__arm_type;
		VAC_RB__Get_Occupied__Arm_Type(l__arm_type);

		int k_limit = l__arm_type.GetSize();
		int k;

		for(k=0; k<k_limit; k++)
		{
			CString str__arm_type = l__arm_type[k];
			
			if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(str__arm_type) < 0)
			{
				continue;
			}

			NEXT__LOOP;
		}
	}

	// ...
	CStringArray l__sch_name;
	CStringArray l__ll_name;
	CStringArray l__ll_slot;

	int  r_flag = _GET__WAFER_LIST_OF_LLx_IN__ALL_MODE(l__sch_name,l__ll_name,l__ll_slot);
	if(r_flag < 0)			NEXT__LOOP;

	// ...
	{
		int k_limit = l__sch_name.GetSize();

		for(int k=0; k < k_limit; k++)
		{
			CString pick__arm_type;

			if(VAC_RB__Get_Empty__Arm_Type_From_LBx(pick__arm_type) < 0)
			{
				NEXT__LOOP;
			}

			// ...
			CString str__sch_name = l__sch_name[k];
			CString str__ll_name  = l__ll_name[k];
			CString str__ll_slot  = l__ll_slot[k];

			int ll_i = Get__LLx_INDEX(str__ll_name);
			if(ll_i < 0)			continue;

			if(LLx__Is_Available(ll_i) < 0)			continue;
			if(LLx__Is_VAC(ll_i) < 0)				continue;

			if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(str__sch_name) < 0)
			{
				continue;
			}

			// ...
			bool active__ll_pick  = false;
			int  active__pm_index = -1;

			bool active__ready_check = false;
			
			if(dCH__VAC_RB__CFG_PICK_WAFER_CONDITION->Check__DATA(STR__ONLY_PROCESSED) < 0)
			{
				if(VAC_RB__Check_Empty__Dual_Arm() > 0)			active__ready_check = true;				
			}

			int  pm_id = _CHECK__LBi_To_RB_With_Ready_PMC__ALL_MODE(str__ll_name,str__ll_slot, active__ready_check);
			if(pm_id > 0)
			{
				active__ll_pick  = true;
				active__pm_index = pm_id - 1;
			}

			if(active__pm_index < 0)
			{
				int  active__lp_normal;
				int  count__pmc_type_ready;
				CStringArray l__pmc_name;

				_GET__LBi_To_RB_With_Ready_PMC__ALL_MODE(str__ll_name,str__ll_slot, active__lp_normal,count__pmc_type_ready, l__pmc_name);

				if(count__pmc_type_ready < 1)
				{
					int alarm_id = ALID__JOB_ERROR__MATERIAL_AND_PMC_PROC_TYPE_MISMATCH;

					CString alm_msg;
					CString alm_bff;
					CString r_act;

					// ...
					{
						CString str__proc_type;

						if(active__lp_normal > 0)
						{
							str__proc_type = STR__NORMAL;
						}
						else
						{
							str__proc_type = STR__TEST;
						}	

						alm_bff.Format("The process type of material in \"%s\" is \"%s\". \n", str__sch_name, str__proc_type);
						alm_msg  = alm_bff;
						alm_bff.Format("At least, one PMC with \"%s\" process type must exist. \n", str__proc_type);
						alm_msg += alm_bff;

						alm_bff.Format("PMC List To Visit ...\n");
						alm_msg += alm_bff;

						// ....
						{
							int i_limit = l__pmc_name.GetSize();
							int i;

							for(i=0; i<i_limit; i++)
							{
								alm_bff.Format("   %1d) %s \n", i+1, l__pmc_name[i]);
								alm_msg += alm_bff;
							}
						}
					}

					p_alarm->Check__ALARM(alarm_id,r_act);
					p_alarm->Post__ALARM_With_MESSAGE(alarm_id,alm_msg);
				}
			}

			// LLi -> RB
			if(active__ll_pick)
			{
				int act_flag  = -1;
				int move_flag = -1;

				act_flag = SCH__PICK_MODULE(p_variable,p_alarm, log_id, false, pick__arm_type, str__ll_name,str__ll_slot, str__sch_name);

				if(act_flag < 0)
				{
					move_flag = SCH__Check_Material_Pick(pick__arm_type);
				}
				else
				{
					move_flag = 1;
				}

				if(move_flag > 0)
				{
					xSCH_MATERIAL_CTRL->Pick__From_MODULE(str__sch_name, pick__arm_type);

					dCH__VAC_RB__INFO_PM_RESERVE_X[active__pm_index]->Set__DATA(STR__ON);

					SCH__Change_Current_Route_Step(p_variable,p_alarm, pick__arm_type);
				}

				if(act_flag < 0)
				{
					NEXT__LOOP;
				}
			}
			else
			{
				continue;
			}

			// LLx Check ...
			{
				int ll_i = Get__LLx_INDEX(str__ll_name);
				if(ll_i >= 0)
				{
					if(xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Check__DATA(STR__OUT) > 0)
					{
						bool active__vent_req = false;

						if(LLx__Check_Occupied__OutSlot(ll_i) > 0)
						{
							active__vent_req = true;
						}						
						else if(LLx__Check_Empty__InSlot(ll_i) > 0)
						{
							if(xSCH_MATERIAL_CTRL->Check__MATERIAL_TO_PROCESS() > 0)
							{
								active__vent_req = true;
							}
						}

						if(active__vent_req)
						{
							if(SCH_RUN__LLx_VENT(ll_i, log_id, "1") > 0)
							{
								xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Set__DATA(STR__IN);
							}
						}
					}
				}
			}

			NEXT__LOOP;
		}
	}

	return 1;
}
int  CObj__DUAL_ARM_STD::
_CHECK__FIRST_WAFER_OF_LLx_IN__ALL_MODE(const CString& para__ll_name, 
										const CString& para__ll_slot)
{
	CStringArray l__sch_name;
	CStringArray l__ll_name;
	CStringArray l__ll_slot;

	_GET__WAFER_LIST_OF_LLx_IN__ALL_MODE(l__sch_name, l__ll_name,l__ll_slot);

	/*
	// ...
	{
		CString log_msg;
		CString log_bff;

		log_msg = "_CHECK__FIRST_WAFER_OF_LLx_IN__ALL_MODE() .. \n";

		log_bff.Format("  * para__ll_name <- %s \n", para__ll_name);
		log_msg += log_bff;
		log_bff.Format("  * para__ll_slot <- %s \n", para__ll_slot);
		log_msg += log_bff;

		int k_limit = l__ll_name.GetSize();
		for(int k=0; k<k_limit; k++)
		{
			log_bff.Format(" %1d)  %s %s \n", k, l__ll_name[k], l__ll_slot[k]);
			log_msg += log_bff;
		}
		log_msg += "\n";

		printf(log_msg);
	}
	*/

	/*
	int k_limit = l__ll_name.GetSize();
	for(int k=0; k<k_limit; k++)
	{
		if(l__ll_name[k].CompareNoCase(para__ll_name) != 0)			continue;
		if(l__ll_slot[k].CompareNoCase(para__ll_slot) != 0)			continue;

		return 1;
	}
	*/

	if(l__ll_name.GetSize() > 0)
	{
		if(l__ll_name[0].CompareNoCase(para__ll_name) != 0)			return -11;
		if(l__ll_slot[0].CompareNoCase(para__ll_slot) != 0)			return -12;

		return 1;
	}

	return -1;
}
int  CObj__DUAL_ARM_STD::
_GET__WAFER_LIST_OF_LLx_IN__ALL_MODE(CStringArray& l__sch_name,
									 CStringArray& l__ll_name,
									 CStringArray& l__ll_slot,
									 const bool active__log_msg)
{
	// ...
	{
		l__sch_name.RemoveAll();
		l__ll_name.RemoveAll();
		l__ll_slot.RemoveAll();
	}

	// ...
	CUIntArray   l__sch_port_id;
	CUIntArray   l__sch_slot_id;

	// LLx_IN : LIST ...
	{
		int ll_limit = iLLx_LIMIT;
		int ll_i;

		for(ll_i=0; ll_i<ll_limit; ll_i++)
		{
			if(xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Check__DATA(STR__OUT) < 0)
			{
				continue;
			}

			if(LLx__Is_Available(ll_i) < 0)			continue;
			if(LLx__Is_VAC(ll_i) < 0)				continue;

			// ...
			CString str__ll_name = Get__LLx_NAME(ll_i);

			CUIntArray l__slot_id;
			LLx__Get_Occupied__InSlot(ll_i, l__slot_id);

			int k_limit = l__slot_id.GetSize();
			int k;

			for(k=0; k<k_limit; k++)
			{
				int slot_id = l__slot_id[k];

				// ...
				CString sch_name;
				CString str__ll_slot;

				str__ll_slot.Format("%1d", slot_id);
				sch_name.Format("%s-%s", str__ll_name, str__ll_slot);

				if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(sch_name) < 0)
				{
					continue;
				}

				// ...
				IDS__SCH_MATERIAL_INFO ds_info;
				
				if(xSCH_MATERIAL_CTRL->Get__MATERIAL_INFO(sch_name, ds_info) < 0)
				{
					continue;
				}

				l__sch_name.Add(sch_name);
				l__ll_name.Add(str__ll_name);
				l__ll_slot.Add(str__ll_slot);
				l__sch_port_id.Add(ds_info.iSRC__PTN);
				l__sch_slot_id.Add(ds_info.iSRC__STN);
			}
		}
	}

	if(active__log_msg)
	{
		CString log_msg;
		CString log_bff;

		log_msg  = "\n";
		log_msg += "_GET__WAFER_LIST_OF_LLx_IN__ALL_MODE() - ORG ... \n";

		int t_limit = l__ll_name.GetSize();
		for(int t=0; t<t_limit; t++)
		{
			log_bff.Format("  %1d)  \n", t + 1);
			log_msg += log_bff;

			log_bff.Format("    * ll_name <- %s \n", l__ll_name[t]);
			log_msg += log_bff;

			log_bff.Format("    * ll_slot <- %s \n", l__ll_slot[t]);
			log_msg += log_bff;

			log_bff.Format("    * wafer.port_id <- %1d \n", l__sch_port_id[t]);
			log_msg += log_bff;

			log_bff.Format("    * wafer.slot_id <- %1d \n", l__sch_slot_id[t]);
			log_msg += log_bff;
		}

		xAPP_LOG_CTRL->WRITE__LOG(log_msg);
	}

	// Ascending Order ...
	{
		int k_limit = l__sch_slot_id.GetSize();
		int k;

		for(k=0; k<k_limit; k++)
		{
			int ref__slot_id = l__sch_slot_id[k];
			
			for(int t=k+1; t<k_limit; t++)
			{	
				int cur__slot_id = l__sch_slot_id[t];
				
				if(cur__slot_id >= ref__slot_id)		continue;	
				ref__slot_id = cur__slot_id;

				// ...
				int cur__port_id = l__sch_port_id[t];

				CString cur__sch_name = l__sch_name[t];
				CString cur__ll_name  = l__ll_name[t];
				CString cur__ll_slot  = l__ll_slot[t];

				// ...
				{
					l__sch_port_id[t] = l__sch_port_id[k];
					l__sch_slot_id[t] = l__sch_slot_id[k];
					l__sch_name[t] = l__sch_name[k];
					l__ll_name[t]  = l__ll_name[k];
					l__ll_slot[t]  = l__ll_slot[k];

					l__sch_port_id[k] = cur__port_id;
					l__sch_slot_id[k] = cur__slot_id;
					l__sch_name[k] = cur__sch_name;
					l__ll_name[k]  = cur__ll_name;
					l__ll_slot[k]  = cur__ll_slot;
				}
			}
		}

		if(active__log_msg)
		{
			CString log_msg;
			CString log_bff;

			log_msg  = "\n";
			log_msg += "_GET__WAFER_LIST_OF_LLx_IN__ALL_MODE() - Ascending Order ... \n";

			int t_limit = l__ll_name.GetSize();
			for(int t=0; t<t_limit; t++)
			{
				log_bff.Format("  %1d)  \n", t + 1);
				log_msg += log_bff;

				log_bff.Format("    * ll_name <- %s \n", l__ll_name[t]);
				log_msg += log_bff;

				log_bff.Format("    * ll_slot <- %s \n", l__ll_slot[t]);
				log_msg += log_bff;

				log_bff.Format("    * wafer.port_id <- %1d \n", l__sch_port_id[t]);
				log_msg += log_bff;

				log_bff.Format("    * wafer.slot_id <- %1d \n", l__sch_slot_id[t]);
				log_msg += log_bff;
			}

			xAPP_LOG_CTRL->WRITE__LOG(log_msg);
		}
	}
	
	// Job Ordering ...
	{
		CUIntArray l__job_lp;
		SCH__Get_Job_LP_List(l__job_lp);

		if(l__job_lp.GetSize() > 1)
		{
			int ref__lp_id = l__job_lp[0];

			// ...
			CStringArray l_job__sch_name;
			CStringArray l_job__ll_name;
			CStringArray l_job__ll_slot;
			CUIntArray   l_job__sch_port_id;
			CUIntArray   l_job__sch_slot_id;

			int k_limit = l__sch_port_id.GetSize();
			int k;

			for(k=0; k<k_limit; k++)
			{
				int cur__port_id = l__sch_port_id[k];
				if(cur__port_id == ref__lp_id)			continue;

				int cur__slot_id = l__sch_slot_id[k];
				CString cur__sch_name = l__sch_name[k];
				CString cur__ll_name  = l__ll_name[k];
				CString cur__ll_slot  = l__ll_slot[k];

				l_job__sch_name.Add(cur__sch_name);
				l_job__ll_name.Add(cur__ll_name);
				l_job__ll_slot.Add(cur__ll_slot);
				l_job__sch_port_id.Add(cur__port_id);
				l_job__sch_slot_id.Add(cur__slot_id);

				l__sch_name.RemoveAt(k);
				l__ll_name.RemoveAt(k);
				l__ll_slot.RemoveAt(k);
				l__sch_port_id.RemoveAt(k);
				l__sch_slot_id.RemoveAt(k);

				k_limit--;
				k--;
			}

			if(l_job__sch_name.GetSize() > 0)
			{
				l__sch_name.Append(l_job__sch_name);
				l__ll_name.Append(l_job__ll_name);
				l__ll_slot.Append(l_job__ll_slot);
				l__sch_port_id.Append(l_job__sch_port_id);
				l__sch_slot_id.Append(l_job__sch_slot_id);
			}
		}
	}

	if(active__log_msg)
	{
		CString log_msg;
		CString log_bff;

		log_msg  = "\n";
		log_msg += "_GET__WAFER_LIST_OF_LLx_IN__ALL_MODE() - Job Ordering ... \n";

		int t_limit = l__ll_name.GetSize();
		for(int t=0; t<t_limit; t++)
		{
			log_bff.Format("  %1d)  \n", t + 1);
			log_msg += log_bff;

			log_bff.Format("    * ll_name <- %s \n", l__ll_name[t]);
			log_msg += log_bff;

			log_bff.Format("    * ll_slot <- %s \n", l__ll_slot[t]);
			log_msg += log_bff;

			log_bff.Format("    * wafer.port_id <- %1d \n", l__sch_port_id[t]);
			log_msg += log_bff;

			log_bff.Format("    * wafer.slot_id <- %1d \n", l__sch_slot_id[t]);
			log_msg += log_bff;
		}

		// ...
		CUIntArray l__job_lp;
		SCH__Get_Job_LP_List(l__job_lp);

		int k_limit = l__job_lp.GetSize();

		log_bff.Format("Job Size (%1d) \n", k_limit);
		log_msg += log_bff;

		for(int k=0; k<k_limit; k++)
		{
			log_bff.Format("  %1d)  Job_LP(%1d) \n", k + 1, l__job_lp[k]);
			log_msg += log_bff;
		}

		xAPP_LOG_CTRL->WRITE__LOG(log_msg);
	}

	if(l__sch_name.GetSize() > 0)			return 1;
	return -1;
}
int  CObj__DUAL_ARM_STD::
_CHECK__LBi_To_RB_With_Ready_PMC__ALL_MODE(const bool active__pm_ready)
{
	return _CHECK__LBi_To_RB_With_Ready_PMC__ALL_MODE("", "", active__pm_ready);
}
int  CObj__DUAL_ARM_STD::
_CHECK__LBi_To_RB_With_Ready_PMC__ALL_MODE(const CString& str__ll_name, const int para__ll_slot, const bool active__pm_ready)
{
	CString str__ll_slot;
	str__ll_slot.Format("%1d", para__ll_slot);

	return _CHECK__LBi_To_RB_With_Ready_PMC__ALL_MODE(str__ll_name, str__ll_slot, active__pm_ready);
}
int  CObj__DUAL_ARM_STD::
_CHECK__LBi_To_RB_With_Ready_PMC__ALL_MODE(const CString& str__ll_name, const CString& str__ll_slot, const bool active__pm_ready)
{
	int active__lp_normal;
	int count__pmc_type_ready;
	CStringArray l__pmc_name;

	return _GET__LBi_To_RB_With_Ready_PMC__ALL_MODE(str__ll_name,str__ll_slot, active__lp_normal,count__pmc_type_ready,l__pmc_name, active__pm_ready);
}

int  CObj__DUAL_ARM_STD::
_GET__LBi_To_RB_With_Ready_PMC__ALL_MODE(const CString& str__ll_name, 
										 const CString& str__ll_slot, 
										 int& active__lp_normal, 
										 int& count__pmc_type_ready, 
										 CStringArray& l__ll_pmc_name,
										 const bool active__pm_ready)
{
	// ...
	{
		active__lp_normal = 0;
		count__pmc_type_ready = 0;

		l__ll_pmc_name.RemoveAll();
	}

	// ...
	CStringArray l__sch_name;
	CStringArray l__ll_name;
	CStringArray l__ll_slot;

	if(str__ll_name.GetLength() > 0)
	{
		CString str__sch_name;
		str__sch_name.Format("%s-%s", str__ll_name, str__ll_slot);

		l__sch_name.Add(str__sch_name);
		l__ll_name.Add(str__ll_name);
		l__ll_slot.Add(str__ll_slot);
	}
	else
	{
		int  r_flag = _GET__WAFER_LIST_OF_LLx_IN__ALL_MODE(l__sch_name,l__ll_name,l__ll_slot);
		if(r_flag < 0)			return -11;
	}

	// ...
	{
		int k_limit = l__sch_name.GetSize();

		for(int k=0; k < k_limit; k++)
		{
			CString str__sch_name = l__sch_name[k];

			int pmc_id = _GET__Ready_PMC_OF_SCH_NAME(str__sch_name, active__lp_normal,count__pmc_type_ready,l__ll_pmc_name, active__pm_ready);
			if(pmc_id > 0)			return pmc_id;
		}
	}

	return -1;
}

int  CObj__DUAL_ARM_STD::
_CHECK__Ready_PMC_OF_SCH_NAME(const CString& str__sch_name, 
							  const bool active__pm_ready)
{
	CStringArray l__sch_pmc_name;
	int active__lp_normal = 1;
	int count__pmc_type_ready = 0;

	return _GET__Ready_PMC_OF_SCH_NAME(str__sch_name, active__lp_normal,count__pmc_type_ready,l__sch_pmc_name, active__pm_ready);
}
int  CObj__DUAL_ARM_STD::
_GET__Ready_PMC_OF_SCH_NAME(const CString& str__sch_name, 
							int& active__lp_normal, 
							int& count__pmc_type_ready, 
							CStringArray& l__sch_pmc_name,
							const bool active__pm_ready)
{
	// ...
	{
		l__sch_pmc_name.RemoveAll();
		active__lp_normal = 1;
		count__pmc_type_ready = 0;
	}

	// ...
	CStringArray l__ref_pmc_name;
	int ref_job__port_id = -1;

	// ...
	{	
		CUIntArray l_job_lp;
		SCH__Get_Job_LP_List(l_job_lp);

		if(l_job_lp.GetSize() > 0)
		{
			ref_job__port_id = l_job_lp[0];

			if(xSCH_MATERIAL_CTRL->Check__MATERIAL_TO_PROCESS_IN_LPx(ref_job__port_id) > 0)
			{
				xSCH_MATERIAL_CTRL->Get__ALL_PRC_MODULE_In_LPx(ref_job__port_id, l__ref_pmc_name);
			}
		}
	}

	// ...
	{
		CStringArray l__pmc_name;
		CStringArray l__rcp_name;

		xSCH_MATERIAL_CTRL->Get__CUR_PROCESS_INFO(str__sch_name, l__pmc_name,l__rcp_name);

		if(xEXT_CH__SCH_CONCURRENT_ALWAYS_APPLY->Check__DATA(STR__PPID_CHECK) > 0)
		{
			IDS__SCH_MATERIAL_INFO ds_info;
			xSCH_MATERIAL_CTRL->Get__MATERIAL_INFO(str__sch_name, ds_info);

			if(ref_job__port_id != ds_info.iSRC__PTN)
			{
				CStringArray l__pmc_change;
				CStringArray l__rcp_change;

				l__pmc_change.Copy(l__pmc_name);
				l__rcp_change.Copy(l__rcp_name);

				Macro__Delete_Trg_With_Src(l__ref_pmc_name, l__pmc_change,l__rcp_change);

				if(l__pmc_change.GetSize() > 0)
				{
					l__pmc_name.Copy(l__pmc_change);
					l__rcp_name.Copy(l__rcp_change);
				}
			}
		}

		l__sch_pmc_name.Copy(l__pmc_name);

		// ...
		int t_limit = l__pmc_name.GetSize();

		for(int t=0; t < t_limit; t++)
		{
			CString str__pmc_name = l__pmc_name[t];

			int pm_index = Macro__Get_PMC_INDEX(str__pmc_name);
			if(pm_index < 0)			continue;

			int pm_id = pm_index + 1;

			// ...
			{
				if(LPx__Get_Process_Type__Normal(str__sch_name, active__lp_normal) < 0)
				{
					continue;
				}

				if(active__lp_normal > 0)
				{
					if(xEXT_CH__PMx_PROCESS_TYPE[pm_index]->Check__DATA(STR__NORMAL) < 0)
					{
						continue;
					}
				}
				else
				{
					if(xEXT_CH__PMx_PROCESS_TYPE[pm_index]->Check__DATA(STR__TEST) < 0)
					{
						continue;
					}
				}

				count__pmc_type_ready++;
			}

			if(dCH__VAC_RB__INFO_PM_RESERVE_X[pm_index]->Check__DATA(STR__ON) > 0)
			{
				continue;
			}

			if(active__pm_ready)
			{
				if(PMx__Is_Ready(pm_index) < 0)				continue;
			}
			else
			{
				if(PMx__Is_Available(pm_index) < 0)			continue;
			}

			if(PMx__Check_Empty__SlotStatus(pm_index) < 0)
			{
				continue;
			}

			return pm_id;
		}
	}

	return -1;
}

// ...
int  CObj__DUAL_ARM_STD::
Get__First_Wafer_In_LBi(CII_OBJECT__VARIABLE *p_variable,
						CII_OBJECT__ALARM    *p_alarm,						 
						const CString& check_lbx_type,
						int& lba_flag,
						int& lbb_flag)
{
	int ll_a = _LLx_INDEX__LBA;
	int ll_b = _LLx_INDEX__LBB;

	// ...
	{
		CString var_data;
		int lba_count = 0;
		int lbb_count = 0;
		
		if((check_lbx_type.CompareNoCase(STR__ALL) == 0)
		|| (check_lbx_type.CompareNoCase(STR__LBA) == 0))
		{
			if((xEXT_CH__LLx__IN_OUT_FLAG[ll_a]->Check__DATA("OUT") > 0)
			&& (LLx__Is_Available(ll_a) > 0)
			&& (LLx__Check_Occupied__InSlot(ll_a) > 0))
			{
				if(SCH__LLx_CHECK_PRESSURE(ll_a, p_variable,p_alarm,1) < 0)
				{
					NEXT__LOOP;
				}
				
				xEXT_CH__LLx__IN_COUNT[ll_a]->Get__DATA(var_data);
				lba_count = atoi(var_data);
			}
		}
		
		if((check_lbx_type.CompareNoCase(STR__ALL) == 0)
		|| (check_lbx_type.CompareNoCase(STR__LBB) == 0))
		{
			if((xEXT_CH__LLx__IN_OUT_FLAG[ll_b]->Check__DATA("OUT") > 0)
			&& (LLx__Is_Available(ll_b) > 0)
			&& (LLx__Check_Occupied__InSlot(ll_b) > 0))
			{
				if(SCH__LLx_CHECK_PRESSURE(ll_b, p_variable,p_alarm,1) < 0)
				{
					NEXT__LOOP;
				}
				
				xEXT_CH__LLx__IN_COUNT[ll_b]->Get__DATA(var_data);
				lbb_count = atoi(var_data);
			}
		}
		
		CString para_module;
		CString para_slot;
		int slot_id;
		
		if((lba_count > 0)&&(lbb_count > 0))
		{
			if(lba_count <= lbb_count)
			{
				if(LLx__Get_Occupied__InSlot(ll_a, slot_id) < 0)
				{
					NEXT__LOOP;
				}
				
				para_module = MODULE__LBA;
				para_slot.Format("%1d",slot_id);
				
				lba_flag = 1;
			}
			else
			{
				if(LLx__Get_Occupied__InSlot(ll_b, slot_id) < 0)
				{
					NEXT__LOOP;
				}

				para_module = MODULE__LBB;
				para_slot.Format("%1d",slot_id);
				
				lbb_flag = 1;
			}
		}
		else if(lba_count > 0)
		{
			if(LLx__Get_Occupied__InSlot(ll_a, slot_id) < 0)	
			{
				NEXT__LOOP;
			}

			para_module = MODULE__LBA;
			para_slot.Format("%1d",slot_id);
			
			lba_flag = 1;
		}
		else if(lbb_count > 0)
		{
			if(LLx__Get_Occupied__InSlot(ll_b, slot_id) < 0)
			{
				NEXT__LOOP;
			}

			para_module = MODULE__LBB;
			para_slot.Format("%1d",slot_id);
			
			lbb_flag = 1;
		}
		else
		{
			NEXT__LOOP;
		}
	}

	return 1;
}
int  CObj__DUAL_ARM_STD::
Check__First_Wafer_In_LBi(CII_OBJECT__VARIABLE *p_variable,
						  CII_OBJECT__ALARM    *p_alarm,						 
						  int& llx_id)
{
	int llx_count[CFG_LLx_LIMIT];
	int ll_i;

	for(ll_i=0; ll_i<iLLx_LIMIT; ll_i++)
	{
		llx_count[ll_i] = 0;

		if((xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Check__DATA("OUT") > 0)
		&& (LLx__Is_Available(ll_i) > 0)
		&& (LLx__Check_Occupied__InSlot(ll_i) > 0))
		{
			if(SCH__LLx_CHECK_PRESSURE(ll_i, p_variable,p_alarm,1) < 0)
			{
				return -1;
			}

			CString var_data = xEXT_CH__LLx__IN_COUNT[ll_i]->Get__STRING();
			llx_count[ll_i] = atoi(var_data);
		}
	}

	// llx_count : Ascending Order Sort ...
	{
		for(ll_i=0; ll_i<iLLx_LIMIT; ll_i++)
		{
			int cmp_count = llx_count[ll_i];

			for(int ll_c=ll_i+1; ll_c<iLLx_LIMIT; ll_c++)
			{
				int cur_count = llx_count[ll_c];

				if(cur_count < cmp_count)		
				{
					llx_count[ll_i] = cur_count;
					llx_count[ll_c] = cmp_count;

					cmp_count = cur_count;
				}
			}
		}
	}

	// llx_count : Check ( > 0) ...
	{
		int llx_active = -1;

		for(ll_i=0; ll_i<iLLx_LIMIT; ll_i++)
		{
			int cur_count = llx_count[ll_i];

			if(cur_count > 0)
			{
				int slot_id;
				if(LLx__Get_Occupied__InSlot(ll_i, slot_id) < 0)
				{
					return -1;
				}

				llx_active = 1;
				llx_id = ll_i + 1;
			}

			if(llx_active > 0)
			{
				break;
			}
		}

		if(llx_active < 0)
		{
			return -1;
		}
	}

	return 1;
}

int  CObj__DUAL_ARM_STD::
Get__Arm_List_To_Process(CStringArray& l_arm)
{
	return _Get__Arm_List(l_arm, 1);
}
int  CObj__DUAL_ARM_STD::
Get__Arm_List_To_LPx(CStringArray& l_arm)
{
	return _Get__Arm_List(l_arm, -1);
}
int  CObj__DUAL_ARM_STD::
_Get__Arm_List(CStringArray& l_arm, const int proc_check)
{
	l_arm.RemoveAll();

	// ...
	CUIntArray l_portid;
	CUIntArray l_slotid;
	CString arm_type;

	int i_limit = 2;
	int i;

	for(i=0; i<i_limit; i++)
	{
		if(i == 0)
		{
			if(VAC_RB__Check_Occupied__A_Arm() > 0)		arm_type = "A";
			else										continue;
		}
		else if(i == 1)
		{
			if(VAC_RB__Check_Occupied__B_Arm() > 0)		arm_type = "B";
			else										continue;
		}
		else
		{
			continue;
		}

		if(proc_check > 0)
		{
			if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(arm_type) < 0)
				continue;
		}
		else
		{
			if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(arm_type) > 0)
				continue;
		}

		// ...
		IDS__SCH_MATERIAL_INFO db_info;

		if(xSCH_MATERIAL_CTRL->Get__MATERIAL_INFO(arm_type, db_info) > 0)
		{
			l_arm.Add(arm_type);
			l_portid.Add(db_info.iSRC__PTN);
			l_slotid.Add(db_info.iSRC__STN);
		}
	}

	// Change Order ...
	i_limit = l_portid.GetSize();

	for(i=0; i<i_limit; i++)
	{
		int cmp_portid  = l_portid[i];
		int cmp_slotid  = l_slotid[i];
		CString cmp_arm = l_arm[i];

		for(int k=i+1; k<i_limit; k++)
		{
			int cur_portid  = l_portid[k];
			int cur_slotid  = l_slotid[k];
			CString cur_arm = l_arm[k];

			if(cur_portid != cmp_portid)		continue;
			if(cur_slotid >= cmp_slotid)		continue;

			// ...
			{
				l_portid[i] = cur_portid;
				l_slotid[i] = cur_slotid;
				l_arm[i] = cur_arm;

				l_portid[k] = cmp_portid;
				l_slotid[k] = cmp_slotid;
				l_arm[k] = cmp_arm;
			}

			cmp_portid = cur_portid;
			cmp_slotid = cur_slotid;
			cmp_arm = cur_arm;
		}
	}

	return 1;
}

int  CObj__DUAL_ARM_STD::
Change_Order__In_Count_Of_LLx(CUIntArray& l_count,
							  CStringArray& l_name,
							  CStringArray& l_slot)
{
	int i_limit = l_count.GetSize();
	int i;

	for(i=0; i<i_limit; i++)
	{
		int cmp_count = l_count[i];
		
		CString cmp_name = l_name[i];
		CString cmp_slot = l_slot[i];

		for(int k=i+1; k<i_limit; k++)
		{
			int cur_count = l_count[k];

			CString cur_name = l_name[k];
			CString cur_slot = l_slot[k];

			if(cur_count < cmp_count)		
			{
				l_count[i] = cur_count;
				l_name[i]  = cur_name;
				l_slot[i]  = cur_slot;

				l_count[k] = cmp_count;
				l_name[k]  = cmp_name;
				l_slot[k]  = cmp_slot;

				cmp_count = cur_count;
			}
		}
	}

	return 1;
}

int  CObj__DUAL_ARM_STD::
Fnc__LBi_To_RB_With_Ready_PMC__ALL_MODE(CII_OBJECT__VARIABLE *p_variable,
										CII_OBJECT__ALARM *p_alarm,
										const CString& check_lbx_type,
										const CStringArray& l_lba_pmc_name,
										const CStringArray& l_lba_pmc_rcp,
										const CStringArray& l_lbb_pmc_name,
										const CStringArray& l_lbb_pmc_rcp)
{
	DECLARE__EXT_CTRL(p_variable);
	
	
	// ...
	CString log_id = "Fnc__LBi_To_RB_With_Ready_PMC__ALL_MODE()";

	// ...
	int lba_flag = -1;
	int lbb_flag = -1;

	if(Get__First_Wafer_In_LBi(p_variable,p_alarm,check_lbx_type,lba_flag,lbb_flag) < 0)
	{
		NEXT__LOOP;
	}

	// ...
	CString para_module;
	CString para_in_slot;
	CString para_out_slot;
	int slot_id;

	CStringArray l_change_pmc_name;
	CStringArray l_change_pmc_rcp;
	CStringArray l_change_pmc_stage;

	// ...
	{
		int lbx_flag = -1;

		int ll_limit = iLLx_LIMIT;
		int ll_i;

		for(ll_i=0; ll_i<ll_limit; ll_i++)
		{
			if(ll_i == 0)		
			{
				if(lba_flag < 0)		continue;
			}
			else if(ll_i == 1)		
			{
				if(lbb_flag < 0)		continue;
			}
			else
			{
				continue;
			}

			if(xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Check__DATA(STR__OUT) > 0)
			{
				if((SCH__LLx_CHECK_PRESSURE(ll_i, p_variable,p_alarm) > 0)
				&& (LLx__Is_Available(ll_i) > 0))
				{
					para_module = Get__LLx_NAME(ll_i);

					if(LLx__Get_Occupied__InSlot(ll_i, slot_id) > 0)
					{
						para_in_slot.Format("%1d", slot_id);

						// ...
						CString sch_name;
						sch_name.Format("%s-%s", para_module,para_in_slot);

						if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(sch_name) > 0)
						{
							lbx_flag = 1;
						}
					}
					if(LLx__Get_Empty__OutSlot(ll_i, slot_id) > 0)
					{
						para_out_slot.Format("%1d",slot_id);	
					}
				}
			}
		}

		if(lbx_flag < 0)
		{
			NEXT__LOOP;
		}
	}

	// ...
	CString arm_type;
	bool ex_flag = false;

	// if(xCH_CFG__SCH_LLx_EXCHANGE_WHEN_PMC_EMPTY->Check__DATA(STR__ENABLE) > 0)
	{
		if(VAC_RB__Get_Occupied__Arm_Type(arm_type) > 0)
		{
			IDS__SCH_MATERIAL_STATUS ds_sts;
			if(xSCH_MATERIAL_CTRL->Get__MATERIAL_STATUS(arm_type,ds_sts) < 0)
			{
				NEXT__LOOP;
			}

			if(ds_sts.sMATERIAL_STS.CompareNoCase(STR__PROCESSED) == 0)
			{
				if((para_in_slot.GetLength()  > 0)
				&& (para_out_slot.GetLength() > 0))
				{
					if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(arm_type) < 0)
					{
						ex_flag = true;
					}
				}
			}
		}
	}

	// if((xCH_CFG__PICK_ARM_TYPE_FROM_LBX->Check__DATA("ALL") < 0)
	// || (xCH_CFG__PICK_ARM_TYPE_FROM_PMC->Check__DATA("ALL") < 0))
	// ...
	{
		int flag__wafer_ex = -1;

		if(ex_flag == true)
		{
			CString sch_name;
			sch_name.Format("%s-%s",para_module,para_in_slot);

			if(SCH__LBx_CAN_VENT(sch_name) < 0)
			{
				flag__wafer_ex = 1;
			}
		}	

		if(flag__wafer_ex < 0)
		{
			ex_flag = false;
			para_out_slot = "";

			if(xCH_CFG__SCH_LLx_EXCHANGE_WHEN_PMC_EMPTY->Check__DATA(STR__ENABLE) > 0)
			{
				if(VAC_RB__Check_All_Empty() < 0)
				{
					NEXT__LOOP;
				}
			}

			if(para_in_slot.GetLength() < 1)
			{
				NEXT__LOOP;
			}
		}

		// ...
		{
			CString empty_arm;
			
			if(VAC_RB__Get_Empty__Arm_Type_From_LBx(empty_arm) < 0)
			{
				NEXT__LOOP;
			}
			
			// ...
			CStringArray l_pmc_name;
			CStringArray l_pmc_rcp;

			CString sch_name;
			sch_name.Format("%s-%s",para_module,para_in_slot);

			if(xSCH_MATERIAL_CTRL->Get__CUR_PROCESS_INFO(sch_name,l_pmc_name,l_pmc_rcp) > 0)
			{
				if((check_lbx_type.CompareNoCase(STR__LBA) == 0)
				&& (para_module.CompareNoCase(STR__LBA) == 0))	
				{	
					if(Macro__Check__Src_Item_Included(l_lba_pmc_name,l_pmc_name) > 0)
					{
						l_pmc_name.Copy(l_lba_pmc_name);
						l_pmc_rcp.Copy(l_lba_pmc_rcp);

						// ...
						{
							int t_limit = l_pmc_name.GetSize();
							int t;
	
							for(t=0;t<t_limit;t++)
							{
								l_change_pmc_name.Add(l_pmc_name[t]);
								l_change_pmc_rcp.Add(l_pmc_rcp[t]);
								l_change_pmc_stage.Add("");
							}
						}
					}
				}
				else if((check_lbx_type.CompareNoCase(STR__LBB) == 0)
					 && (para_module.CompareNoCase(STR__LBB) == 0))	
				{	
					if(Macro__Check__Src_Item_Included(l_lbb_pmc_name,l_pmc_name) > 0)
					{
						l_pmc_name.Copy(l_lbb_pmc_name);
						l_pmc_rcp.Copy(l_lbb_pmc_rcp);

						// ...
						{
							int t_limit = l_pmc_name.GetSize();
							int t;
							
							for(t=0;t<t_limit;t++)
							{
								l_change_pmc_name.Add(l_pmc_name[t]);
								l_change_pmc_rcp.Add(l_pmc_rcp[t]);
								l_change_pmc_stage.Add("");
							}
						}
					}
				}

				// ...
				int flag__lp_normal = 1;
				
				if(LPx__Get_Process_Type__Normal(sch_name,flag__lp_normal) < 0)
				{
					return -1;
				}

				// ...
				int pm_check = -1;
				int pm_i;

				int limit;
				int i;

				if(l_change_pmc_name.GetSize() > 0)
				{
					limit = l_change_pmc_name.GetSize();
				
					for(i=0;i<limit;i++)
					{
						pm_i = SCH__CHECK_PM_INDEX(l_change_pmc_name[i]);
						
						if(pm_i < 0)				continue;
						if(pm_i >= iPMx_SIZE)		continue;
						
						if(Fnc__PMx_Empty_Ready_Check(flag__lp_normal,pm_i,1,1) < 0)
						{
							continue;
						}
						
						pm_check = 1;
						break;
					}
				}
				else
				{
					limit = l_pmc_name.GetSize();

					for(i=0;i<limit;i++)
					{
						pm_i = SCH__CHECK_PM_INDEX(l_pmc_name[i]);
					
						if(pm_i < 0)				continue;
						if(pm_i >= iPMx_SIZE)		continue;
					
						if(Fnc__PMx_Empty_Ready_Check(flag__lp_normal,pm_i,1,1) < 0)
						{
							continue;
						}
					
						pm_check = 1;
						break;
					}
				}

				if(pm_check < 0)
				{
					// ...
					{
						int pm_check = -1;
						int pm_i;
						
						int limit;
						int i;

						if(l_change_pmc_name.GetSize() > 0)
						{
							limit = l_change_pmc_name.GetSize();

							for(i=0;i<limit;i++)
							{
								pm_i = SCH__CHECK_PM_INDEX(l_change_pmc_name[i]);
								
								if(pm_i < 0)				continue;
								if(pm_i >= iPMx_SIZE)		continue;
								
								if(flag__lp_normal > 0)
								{
									if(xEXT_CH__PMx_PROCESS_TYPE[pm_i]->Check__DATA(STR__NORMAL) < 0)
									{
										continue;
									}
								}
								else
								{
									if(xEXT_CH__PMx_PROCESS_TYPE[pm_i]->Check__DATA(STR__TEST) < 0)
									{
										continue;
									}
								}
								
								pm_check = 1;
								break;
							}
						}
						else
						{
							limit = l_pmc_name.GetSize();
						
							for(i=0;i<limit;i++)
							{
								pm_i = SCH__CHECK_PM_INDEX(l_pmc_name[i]);
							
								if(pm_i < 0)				continue;
								if(pm_i >= iPMx_SIZE)		continue;
							
								if(flag__lp_normal > 0)
								{
									if(xEXT_CH__PMx_PROCESS_TYPE[pm_i]->Check__DATA(STR__NORMAL) < 0)
									{
										continue;
									}
								}
								else
								{
									if(xEXT_CH__PMx_PROCESS_TYPE[pm_i]->Check__DATA(STR__TEST) < 0)
									{
										continue;
									}
								}
							
								pm_check = 1;
								break;
							}
						}

						// ...
						int pmc_count = 0;

						if(l_change_pmc_name.GetSize() > 0)
						{
							pmc_count = l_change_pmc_name.GetSize();
						}
						else
						{
							pmc_count = l_pmc_name.GetSize();
						}

						if((pm_check < 0) && (pmc_count > 0))	
						{
							int alarm_id = ALID__JOB_ERROR__MATERIAL_AND_PMC_PROC_TYPE_MISMATCH;

							CString alm_msg;
							CString alm_bff;
							CString r_act;

							// ...
							{
								CString str__proc_type;

								if(flag__lp_normal > 0)
								{
									str__proc_type = STR__NORMAL;
								}
								else
								{
									str__proc_type = STR__TEST;
								}	

								alm_bff.Format("The process type of material in \"%s\" is \"%s\". \n", sch_name,str__proc_type);
								alm_msg  = alm_bff;
								alm_bff.Format("At least, one PMC with \"%s\" process type must exist. \n", str__proc_type);
								alm_msg += alm_bff;
	
								alm_bff.Format("PMC List To Visit ...\n");
								alm_msg += alm_bff;

								if(l_change_pmc_name.GetSize() > 0)
								{
									int limit = l_change_pmc_name.GetSize();
									int i;
									
									for(i=0;i<limit;i++)
									{
										alm_bff.Format("   %1d) %s \n", i+1, l_change_pmc_name[i]);
										alm_msg += alm_bff;
									}
								}
								else
								{
									int limit = l_pmc_name.GetSize();
									int i;

									for(i=0;i<limit;i++)
									{
										alm_bff.Format("   %1d) %s \n", i+1, l_pmc_name[i]);
										alm_msg += alm_bff;
									}
								}
							}
							
							p_alarm->Check__ALARM(alarm_id,r_act);
							p_alarm->Post__ALARM_With_MESSAGE(alarm_id,alm_msg);
						}
					}

					NEXT__LOOP;
				}
			}
		}
	}

	//=================================================================================
	if(para_in_slot.GetLength() > 0)
	{
		CString sch_name;
		sch_name.Format("%s-%s", para_module, para_in_slot);

		int trg__goto_pm_id = Get__PM_ID_OF_SCH_USER_DEF_TRG_MDx(log_id, sch_name);
		if(trg__goto_pm_id > 0)
		{

		}
		else
		{
			CString def_name = USER_DEF__GOTO_PM;
			CString def_data;

			xSCH_MATERIAL_CTRL->Get__MATERIAL_USER_INFO(sch_name, def_name,def_data);

			int trg__pmc_index = Macro__Get_PMC_INDEX(def_data);
			if(trg__pmc_index >= 0)
			{
				if(PMx__Is_Ready(trg__pmc_index) > 0)
				{
					NEXT__LOOP;
				}
			}

			if(dCH__VAC_RB__CFG_PICK_WAFER_CONDITION->Check__DATA(STR__ONLY_PROCESSED) > 0)
			{
				CStringArray l_pm_name;

				if(Fnc__Get_PMx_List(sch_name, l_pm_name) < 0)
				{
					NEXT__LOOP;
				}
			}
		}
	}
	//=================================================================================

	if(ex_flag == true)
	{
		CString empty_arm;
		
		if(VAC_RB__Get__Arm_Type_From_LBx(empty_arm) < 0)
		{
			NEXT__LOOP;
		}

		// ...
		{
			CString sch_module;

			// ...
			{
				int act_flag  = -1;
				int move_flag = -1;

				// ...
				sch_module.Format("%s-%s",para_module,para_out_slot);

				act_flag = SCH__PLACE_MODULE(p_variable,p_alarm, log_id, ex_flag,arm_type,para_module,para_out_slot,sch_module);

				if(act_flag < 0)
				{
					move_flag = SCH__Check_Material_Place(arm_type);
				}
				else
				{
					move_flag = 1;
				}

				if(move_flag > 0)
				{
					xSCH_MATERIAL_CTRL->Place__To_MODULE(arm_type,sch_module);
				}

				if(act_flag < 0)
				{
					NEXT__LOOP;
				}
			}

			if(xCH_CFG__PICK_ARM_TYPE_FROM_LBX->Check__DATA(STR__ALL) > 0)
			{
				// LLx - All Mode
				if(xCH_CFG__ROBOT_ARM_DIRECTION_TYPE->Check__DATA(STR__BI_DIRECTION) > 0)
				{
					empty_arm = arm_type;
				}
			}
			else
			{
				CString vac__arm_type;

				if(VAC_RB__Get_Empty__Arm_Type_From_LBx(vac__arm_type) > 0)
				{
					empty_arm = vac__arm_type;
				}
				else 
				{
					// LLx - All Mode
					if(xCH_CFG__ROBOT_ARM_DIRECTION_TYPE->Check__DATA(STR__BI_DIRECTION) > 0)
					{
						empty_arm = arm_type;
					}
				}
			}

			// ...
			{
				int act_flag  = -1;
				int move_flag = -1;

				sch_module.Format("%s-%s",para_module,para_in_slot);

				act_flag = SCH__PICK_MODULE(p_variable,p_alarm, log_id, false,empty_arm,para_module,para_in_slot,sch_module);

				if(act_flag < 0)
				{
					move_flag = SCH__Check_Material_Pick(empty_arm);
				}
				else
				{
					move_flag = 1;
				}
				
				if(move_flag > 0)
				{
					xSCH_MATERIAL_CTRL->Pick__From_MODULE(sch_module,empty_arm);

					SCH__Change_Current_Route_Step(p_variable,p_alarm, empty_arm);
				}

				// ...
				{
					CString log_msg;
					CString log_bff;

					log_msg  = "\n";

					log_msg += "Fnc__LBi_To_RB_With_Ready_PMC__ALL_MODE() - CHECK1";
					log_msg += "\n";
				
					log_msg += "Action : SCH__PICK_MODULE()";
					log_msg += "\n";

					// ...
					{
						IDS__SCH_MATERIAL_STATUS ds_sts;
						xSCH_MATERIAL_CTRL->Get__MATERIAL_STATUS(empty_arm,ds_sts);

						log_bff.Format("wafer <- [P%1d:%1d] \n", 
									   ds_sts.iPORT_ID,
									   ds_sts.iSLOT_ID);
						log_msg += log_bff;
					}

					log_bff.Format("arm_type <- [%s] \n", empty_arm);
					log_msg += log_bff;

					log_bff.Format("para_module  <- [%s] \n", para_module);
					log_msg += log_bff;

					log_bff.Format("para_in_slot <- [%s] \n", para_in_slot);
					log_msg += log_bff;

					log_bff.Format("act_flag  <- [%1d] \n", act_flag);
					log_msg += log_bff;

					log_bff.Format("move_flag <- [%1d] \n", move_flag);
					log_msg += log_bff;

					// ...
					{
						log_bff.Format("lbx_type <- [%s] \n", check_lbx_type);
						log_msg += log_bff;

						// ...
						int limit;
						int i;

						// lba info
						{
							log_msg += "lba info \n";

							// 
							limit = l_lba_pmc_name.GetSize();

							for(i=0;i<limit;i++)
							{
								log_bff.Format("   %1d) [%s][%s] \n",
									           i+1,
											   l_lba_pmc_name[i],
											   l_lba_pmc_rcp[i]);
								log_msg += log_bff;
							}
						}
						// lbb info
						{
							log_msg += "lbb info \n";
								
							// 
							limit = l_lbb_pmc_name.GetSize();
							
							for(i=0;i<limit;i++)
							{
								log_bff.Format("   %1d) [%s][%s] \n",
											   i+1,
											   l_lbb_pmc_name[i],
											   l_lbb_pmc_rcp[i]);
								log_msg += log_bff;
							}
						}
					}

					Fnc__LOG_CTRL(log_msg);
				}

				if(act_flag < 0)
				{
					NEXT__LOOP;
				}
			}
		}
	}
	else
	{
		if(para_in_slot.GetLength() > 0)
		{
			// ...
			CString empty_arm;

			if(VAC_RB__Get_Empty__Arm_Type_From_LBx(empty_arm) < 0)
			{
				NEXT__LOOP;
			}

			// ...
			int act_flag  = -1;
			int move_flag = -1;

			// ...
			CString sch_module;
			sch_module.Format("%s-%s",para_module,para_in_slot);

			act_flag = SCH__PICK_MODULE(p_variable,p_alarm, log_id, false,empty_arm,para_module,para_in_slot,sch_module);

			if(act_flag < 0)
			{
				move_flag = SCH__Check_Material_Pick(empty_arm);
			}
			else
			{
				move_flag = 1;
			}

			if(move_flag > 0)
			{
				xSCH_MATERIAL_CTRL->Pick__From_MODULE(sch_module,empty_arm);	

				// ...
				{
					SCH__Change_Current_Route_Step(p_variable,p_alarm, empty_arm);
				}
			}

			// ...
			{
				CString log_msg;
				CString log_bff;
				
				log_msg  = "\n";
				
				log_msg += "Fnc__LBi_To_RB_With_Ready_PMC__ALL_MODE() - CHECK2";
				log_msg += "\n";
				
				log_msg += "Action : SCH__PICK_MODULE()";
				log_msg += "\n";

				log_bff.Format("arm_type <- [%s] \n", empty_arm);
				log_msg += log_bff;
				
				// ...
				{
					IDS__SCH_MATERIAL_STATUS ds_sts;
					xSCH_MATERIAL_CTRL->Get__MATERIAL_STATUS(empty_arm,ds_sts);
					
					log_bff.Format("wafer <- [P%1d:%1d] \n", 
								   ds_sts.iPORT_ID,
								   ds_sts.iSLOT_ID);
					log_msg += log_bff;
				}

				log_bff.Format("para_module  <- [%s] \n", para_module);
				log_msg += log_bff;
				
				log_bff.Format("para_in_slot <- [%s] \n", para_in_slot);
				log_msg += log_bff;
				
				log_bff.Format("act_flag  <- [%1d] \n", act_flag);
				log_msg += log_bff;
				
				log_bff.Format("move_flag <- [%1d] \n", move_flag);
				log_msg += log_bff;
				
				// ...
				{
					log_bff.Format("lbx_type <- [%s] \n", check_lbx_type);
					log_msg += log_bff;
					
					// ...
					int limit;
					int i;
					
					// lba info
					{
						log_msg += "lba info \n";
						
						// 
						limit = l_lba_pmc_name.GetSize();
						
						for(i=0;i<limit;i++)
						{
							log_bff.Format("   %1d) [%s][%s] \n",
										   i+1,
										   l_lba_pmc_name[i],
										   l_lba_pmc_rcp[i]);
							log_msg += log_bff;
						}
					}
					// lbb info
					{
						log_msg += "lbb info \n";
						
						// 
						limit = l_lbb_pmc_name.GetSize();
						
						for(i=0;i<limit;i++)
						{
							log_bff.Format("   %1d) [%s][%s] \n",
										   i+1,
										   l_lbb_pmc_name[i],
										   l_lbb_pmc_rcp[i]);
							log_msg += log_bff;
						}
					}
				}

				Fnc__LOG_CTRL(log_msg);
			}

			if(act_flag < 0)
			{
				NEXT__LOOP;
			}
		}
		else if((para_out_slot.GetLength() > 0)
			 && (arm_type.GetLength() > 0))
		{
			int act_flag  = -1;
			int move_flag = -1;

			// ...
			CString sch_module;
			sch_module.Format("%s-%s",para_module,para_out_slot);

			act_flag = SCH__PLACE_MODULE(p_variable,p_alarm, log_id, ex_flag,arm_type,para_module,para_out_slot,sch_module);

			if(act_flag < 0)
			{
				move_flag = SCH__Check_Material_Place(arm_type);
			}
			else
			{
				move_flag = 1;
			}

			if(move_flag > 0)
			{
				xSCH_MATERIAL_CTRL->Place__To_MODULE(arm_type,sch_module);
			}

			// ...
			{
				CString log_msg;
				CString log_bff;
				
				log_msg  = "\n";
				
				log_msg += "Fnc__LBi_To_RB_With_Ready_PMC__ALL_MODE() - CHECK3";
				log_msg += "\n";
				
				log_msg += "Action : SCH__PLACE_MODULE()";
				log_msg += "\n";
				
				log_bff.Format("arm_type <- [%s] \n", arm_type);
				log_msg += log_bff;
				
				// ...
				{
					IDS__SCH_MATERIAL_STATUS ds_sts;
					xSCH_MATERIAL_CTRL->Get__MATERIAL_STATUS(arm_type,ds_sts);
					
					log_bff.Format("wafer <- [P%1d:%1d] \n", 
								   ds_sts.iPORT_ID,
								   ds_sts.iSLOT_ID);
					log_msg += log_bff;
				}

				log_bff.Format("para_module  <- [%s] \n", para_module);
				log_msg += log_bff;
				
				log_bff.Format("para_out_slot <- [%s] \n", para_out_slot);
				log_msg += log_bff;
				
				log_bff.Format("act_flag  <- [%1d] \n", act_flag);
				log_msg += log_bff;
				
				log_bff.Format("move_flag <- [%1d] \n", move_flag);
				log_msg += log_bff;
				
				// ...
				{
					log_bff.Format("lbx_type <- [%s] \n", check_lbx_type);
					log_msg += log_bff;
					
					// ...
					int limit;
					int i;
					
					// lba info
					{
						log_msg += "lba info \n";
						
						// 
						limit = l_lba_pmc_name.GetSize();
						
						for(i=0;i<limit;i++)
						{
							log_bff.Format("   %1d) [%s][%s] \n",
										   i+1,
										   l_lba_pmc_name[i],
										   l_lba_pmc_rcp[i]);
							log_msg += log_bff;
						}
					}
					// lbb info
					{
						log_msg += "lbb info \n";
						
						// 
						limit = l_lbb_pmc_name.GetSize();
						
						for(i=0;i<limit;i++)
						{
							log_bff.Format("   %1d) [%s][%s] \n",
										   i+1,
										   l_lbb_pmc_name[i],
										   l_lbb_pmc_rcp[i]);
							log_msg += log_bff;
						}
					}
				}

				Fnc__LOG_CTRL(log_msg);
			}

			if(act_flag < 0)
			{
				NEXT__LOOP;
			}
		}
		else
		{
			NEXT__LOOP;
		}
	}	

	// LLx Check ...
	{
		int ll_i = Get__LLx_INDEX(para_module);
		if(ll_i >= 0)
		{
			if(LLx__Check_Empty__InSlot(ll_i) > 0)
			{
				if(SCH_RUN__LLx_VENT(ll_i, log_id, "1") > 0)
				{
					xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Set__DATA(STR__IN);
				}
			}
		}
	}

	NEXT__LOOP;
}

int  CObj__DUAL_ARM_STD::
AUTO_CTRL__LBi_RB__1_SLOT_ALL_MODE_NEW(CII_OBJECT__VARIABLE *p_variable,
								       CII_OBJECT__ALARM *p_alarm)
{
	DECLARE__EXT_CTRL(p_variable);

	// ...
	CString log_id = "AUTO_CTRL__LBi_RB__1_SLOT_ALL_MODE_NEW()";

	// ...
	int ll_a = _LLx_INDEX__LBA;
	int ll_b = _LLx_INDEX__LBB;

	// ...
	int lba_flag = -1;
	int lbb_flag = -1;

	CString para_module;
	CString para_slot;

	// ...
	{
		CString var_data;
		int lba_count = 0;
		int lbb_count = 0;

		if((xEXT_CH__LLx__IN_OUT_FLAG[ll_a]->Check__DATA("OUT") > 0)
		&& (LLx__Is_Available(ll_a) > 0)
		&& (LLx__Check_Occupied__InSlot_With_1_Slot_All_Mode(ll_a) > 0))
		{
			if(SCH__LLx_CHECK_PRESSURE(ll_a, p_variable,p_alarm,1) < 0)
			{
				NEXT__LOOP;
			}

			xEXT_CH__LLx__IN_COUNT[ll_a]->Get__DATA(var_data);
			lba_count = atoi(var_data);
		}

		if((xEXT_CH__LLx__IN_OUT_FLAG[ll_b]->Check__DATA("OUT") > 0)
		&& (LLx__Is_Available(ll_b) > 0)
		&& (LLx__Check_Occupied__InSlot_With_1_Slot_All_Mode(ll_b) > 0))
		{
			if(SCH__LLx_CHECK_PRESSURE(ll_b, p_variable,p_alarm,1) < 0)
			{
				NEXT__LOOP;
			}

			xEXT_CH__LLx__IN_COUNT[ll_b]->Get__DATA(var_data);
			lbb_count = atoi(var_data);
		}

		// ...
		int slot_id;

		if((lba_count > 0)&&(lbb_count > 0))
		{
			if(lba_count <= lbb_count)
			{
				if(LLx__Get_Occupied__InSlot_With_1_Slot_All_Mode(ll_a, slot_id) < 0)
				{
					NEXT__LOOP;
				}

				para_module = MODULE__LBA;
				para_slot.Format("%1d",slot_id);

				lba_flag = 1;
			}
			else
			{
				if(LLx__Get_Occupied__InSlot_With_1_Slot_All_Mode(ll_b, slot_id) < 0)
				{
					NEXT__LOOP;
				}

				para_module = MODULE__LBB;
				para_slot.Format("%1d",slot_id);

				lbb_flag = 1;
			}
		}
		else if(lba_count > 0)
		{
			if(LLx__Get_Occupied__InSlot_With_1_Slot_All_Mode(ll_a, slot_id) < 0)
			{
				NEXT__LOOP;
			}

			para_module = MODULE__LBA;
			para_slot.Format("%1d",slot_id);

			lba_flag = 1;
		}
		else if(lbb_count > 0)
		{
			if(LLx__Get_Occupied__InSlot_With_1_Slot_All_Mode(ll_b, slot_id) < 0)
			{
				NEXT__LOOP;
			}

			para_module = MODULE__LBB;
			para_slot.Format("%1d",slot_id);

			lbb_flag = 1;
		}
		else
		{
			NEXT__LOOP;
		}
	}

	// ...
	CString para_in_slot;
	CString para_out_slot;
	int slot_id;

	if(lba_flag > 0)
	{
		if(xEXT_CH__LLx__IN_OUT_FLAG[ll_a]->Check__DATA("OUT") > 0)
		{
			if((SCH__LLx_CHECK_PRESSURE(ll_a, p_variable,p_alarm) > 0)
			&& (LLx__Is_Available(ll_a) > 0))
			{
				if(LLx__Get_Occupied__InSlot_With_1_Slot_All_Mode(ll_a, slot_id) > 0)
				{
					para_in_slot.Format("%1d",slot_id);
					para_out_slot.Format("%1d",slot_id);
				}
			}
		}
	}
	if(lbb_flag > 0)
	{	
		if(xEXT_CH__LLx__IN_OUT_FLAG[ll_b]->Check__DATA("OUT") > 0)
		{
			if((SCH__LLx_CHECK_PRESSURE(ll_b, p_variable,p_alarm) > 0)
			&& (LLx__Is_Available(ll_b) > 0))
			{
				para_module = MODULE__LBB;

				if(LLx__Get_Occupied__InSlot_With_1_Slot_All_Mode(ll_b, slot_id) > 0)
				{
					para_in_slot.Format("%1d",slot_id);
					para_out_slot.Format("%1d",slot_id);
				}
			}
		}
	}

	// ...
	CString arm_type = "";
	bool ex_flag = false;

	if(VAC_RB__Get_Occupied__Arm_Type(arm_type) > 0)
	{
		if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(arm_type) < 0)
		{
			int active__sch_mode = -1;
			int active__sch_ll   = -1;

			// ...
			CStringArray l_ll_id;
			CStringArray l_ll_mode;
			CStringArray l_ll_slot;

			xSCH_MATERIAL_CTRL->Get__LLx_OUT_OF_EDIT_TYPE(arm_type, l_ll_id,l_ll_mode,l_ll_slot);

			if(l_ll_id.GetSize() == 1)
			{
				int index_ll = 0;
				int ll_id = atoi(l_ll_id[index_ll]);

				CString str_mode = l_ll_mode[index_ll];
				CString str_slot = l_ll_slot[index_ll];

				if(str_mode.CompareNoCase("ALL") == 0)
				{
					if((str_slot.CompareNoCase("11") == 0)
					|| (str_slot.CompareNoCase("10") == 0)
					|| (str_slot.CompareNoCase("01") == 0))
					{
						active__sch_mode = 1;
						active__sch_ll = ll_id;
					}
				}
			}

			if((para_in_slot.GetLength()  > 0)
			&& (para_out_slot.GetLength() > 0))
			{
				CStringArray l_pm;
				CStringArray l_rcp;

				if(xSCH_MATERIAL_CTRL->Get__CUR_PROCESS_INFO(arm_type,l_pm,l_rcp) < 0)
				{
					if(active__sch_mode < 0)
					{
						ex_flag = true;
					}
					else
					{
						// LBA
						if((active__sch_ll == 1)
						&& (para_module.CompareNoCase(MODULE__LBA) == 0))
						{
							ex_flag = true;
						}
						
						// LBB
					    if((active__sch_ll == 2)				
						&& (para_module.CompareNoCase(MODULE__LBB) == 0))
						{
							ex_flag = true;
						}
					}
				}
			}
		}

		// ...
	}

	if(ex_flag == true)
	{
		CString sch_module;

		// ...
		{
			CString empty_arm;

			if(VAC_RB__Get_Empty__Arm_Type_From_LBx(empty_arm) < 0)
			{
				NEXT__LOOP;
			}

			// ...
			int act_flag  = -1;
			int move_flag = -1;

			sch_module.Format("%s-%s",para_module,para_in_slot);

			act_flag = SCH__PICK_MODULE(p_variable,p_alarm, log_id, ex_flag,empty_arm,para_module,para_in_slot,sch_module);

			if(act_flag < 0)
			{
				move_flag = SCH__Check_Material_Pick(empty_arm);
			}
			else
			{
				move_flag = 1;
			}

			if(move_flag > 0)
			{
				xSCH_MATERIAL_CTRL->Pick__From_MODULE(sch_module,empty_arm);
			}

			if(act_flag < 0)
			{
				NEXT__LOOP;
			}
		}

		// ...
		{
			int act_flag  = -1;
			int move_flag = -1;

			sch_module.Format("%s-%s",para_module,para_out_slot);

			act_flag = SCH__PLACE_MODULE(p_variable,p_alarm, log_id, false,arm_type,para_module,para_out_slot,sch_module);

			if(act_flag < 0)
			{
				move_flag = SCH__Check_Material_Place(arm_type);
			}
			else
			{
				move_flag = 1;
			}

			if(move_flag > 0)
			{
				xSCH_MATERIAL_CTRL->Place__To_MODULE(arm_type,sch_module);
			}

			if(act_flag < 0)
			{
				NEXT__LOOP;
			}
		}

		// LLx Check ...
		{
			int ll_i = Get__LLx_INDEX(para_module);
			if(ll_i >= 0)
			{
				if(xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Check__DATA("IN") < 0)
				{
					if(SCH_RUN__LLx_VENT(ll_i, log_id, "1") > 0)
					{
						xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Set__DATA("IN");
					}
				}
			}
		}
	}
	else
	{
		CString empty_arm;

		if(VAC_RB__Get_Empty__Arm_Type_From_LBx(empty_arm) < 0)
		{
			NEXT__LOOP;
		}

		// LBo -> PMx ...
		{
			CString arm_type;

			if(VAC_RB__Get_Occupied__Arm_Type(arm_type) > 0)
			{
				IDS__SCH_MATERIAL_STATUS ds_sts;
				xSCH_MATERIAL_CTRL->Get__MATERIAL_STATUS(arm_type,ds_sts);

				int material_check = -1;

				if(ds_sts.sMATERIAL_STS.CompareNoCase("ALIGNED") == 0)
				{
					material_check = 1;
				}
				else if(ds_sts.sMATERIAL_STS.CompareNoCase("PROCESSED") == 0)
				{
					if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(arm_type) > 0)
					{
						material_check = 1;
					}
				}

				if(material_check > 0)
				{
					CStringArray l_pmx__rb;
					CStringArray l_rcp__rb;

					if(xSCH_MATERIAL_CTRL->Get__CUR_PROCESS_INFO(arm_type, l_pmx__rb,l_rcp__rb) > 0)
					{
						int limit = l_pmx__rb.GetSize();
						int i;

						for(i=0; i<limit; i++)
						{
							int pm_i = SCH__CHECK_PM_INDEX(l_pmx__rb[i]);

							if(pm_i < 0)				continue;
							if(pm_i >= iPMx_SIZE)		continue;

							if(PMx__Is_Available(pm_i) < 0)
							{
								continue;
							}

							NEXT__LOOP;
						}
					}

					// ...
					CStringArray l_pmx__ll;
					CStringArray l_rcp__ll;

					CString sch_module;
					sch_module.Format("%s-%s", para_module,para_slot);

					xSCH_MATERIAL_CTRL->Get__CUR_PROCESS_INFO(sch_module, l_pmx__ll,l_rcp__ll);

					if(Macro__Check_Equal__Any_String(l_pmx__rb, l_pmx__ll) > 0)
					{
						NEXT__LOOP;
					}

					int limit = l_pmx__ll.GetSize();
					int i;

					for(i=0; i<limit; i++)
					{
						int pm_i = SCH__CHECK_PM_INDEX(l_pmx__ll[i]);

						if(pm_i < 0)				continue;
						if(pm_i >= iPMx_SIZE)		continue;

						if(PMx__Is_Available(pm_i) < 0)				continue;
						if(PMx__Check_Empty__SlotStatus(pm_i) < 0)	continue;

						// ...
						CString pm_name = l_pmx__ll[i];
						CString pm_slot = "1";
						CString pm_rcp  = l_rcp__ll[i];

						Seq__LBi_PMx(p_variable,
									 p_alarm,
									 empty_arm,
									 para_module,para_slot,
									 pm_i,pm_name,pm_slot,pm_rcp);

						NEXT__LOOP;
					}

					NEXT__LOOP;
				}
			}
		}
	}

	NEXT__LOOP;
}

int  CObj__DUAL_ARM_STD::
AUTO_CTRL__LBi_PMx__ONLY_MODE(CII_OBJECT__VARIABLE *p_variable, CII_OBJECT__ALARM *p_alarm)
{
	if(dCH__VAC_RB__CFG_LLx_CTRL_ONLY_INPUT_OUTPUT_MODE->Check__DATA(STR__DUAL) > 0)
	{
		NEXT__LOOP;
	}
	else
	{
		// _AUTO_CTRL__LBi_PMx__ONLY_MODE(p_variable, p_alarm);
	}	
}
int  CObj__DUAL_ARM_STD::
_AUTO_CTRL__LBi_PMx__ONLY_MODE(CII_OBJECT__VARIABLE *p_variable, CII_OBJECT__ALARM *p_alarm)
{
	// Check : Wafer State ...
	if(dCH__VAC_RB__CFG_PICK_WAFER_CONDITION->Check__DATA(STR__ONLY_PROCESSED) > 0)
	{
		if(VAC_RB__Check_Occupied__A_Arm() > 0)
		{
			if(xCH__VAC_RB__SLOT01_STATUS->Check__DATA(STR__MAPPED) < 0)			NEXT__LOOP;
		}
		if(VAC_RB__Check_Occupied__B_Arm() > 0)
		{
			if(xCH__VAC_RB__SLOT02_STATUS->Check__DATA(STR__MAPPED) < 0)			NEXT__LOOP;
		}
	}

	// ...
	CString para_module;
	CString para_slot;
	int slot_id;
	int loop = 1;
	int db_count = 0;

	while(loop > 0)
	{
		loop = -1;

		// LLx ...
		{
			int active__only_input = -1;

			int ll_limit = iLLx_LIMIT;
			int ll_i;

			for(ll_i=0; ll_i<ll_limit; ll_i++)
			{
				if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_i]->Check__DATA(LBx_MODE__ONLY_INPUT) > 0)
				{
					active__only_input = 1;

					if(xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Check__DATA("OUT") < 0)
					{
						NEXT__LOOP;
					}

					if(LLx__Is_Available(ll_i)   < 0)
					{
						NEXT__LOOP;
					}
					if(LLx__Get_Occupied__InSlot(ll_i, db_count,slot_id) < 0)
					{
						NEXT__LOOP;
					}

					if(SCH__LLx_CHECK_PRESSURE(ll_i, p_variable,p_alarm,1) < 0)
					{
						NEXT__LOOP;
					}

					db_count++;

					para_module = Get__LLx_NAME(ll_i);
					para_slot.Format("%1d",slot_id);
				}
			}

			if(active__only_input < 0)
			{
				NEXT__LOOP;
			}
		}

		// ...
		CString empty_arm;

		if(VAC_RB__Get_Empty__Arm_Type_From_LBx(empty_arm) < 0)
		{
			NEXT__LOOP;
		}

		// ...
		CString arm_type;

		if(VAC_RB__Get_Occupied__Arm_Type(arm_type) > 0)
		{
			IDS__SCH_MATERIAL_STATUS ds_sts;
			xSCH_MATERIAL_CTRL->Get__MATERIAL_STATUS(arm_type,ds_sts);

			int material_check = -1;

			if(ds_sts.sMATERIAL_STS.CompareNoCase("ALIGNED") == 0)
			{
				material_check = 1;
			}
			else if(ds_sts.sMATERIAL_STS.CompareNoCase("PROCESSED") == 0)
			{
				if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(arm_type) > 0)
				{
					material_check = 1;
				}
			}

			if(material_check > 0)
			{
				// ...
				{
					CStringArray l_pm;
					CStringArray l_rcp;

					if(xSCH_MATERIAL_CTRL->Get__CUR_PROCESS_INFO(arm_type,l_pm,l_rcp) > 0)
					{
						int pm_i;

						int limit = l_pm.GetSize();
						int i;

						for(i=0;i<limit;i++)
						{
							pm_i = SCH__CHECK_PM_INDEX(l_pm[i]);
					
							if(pm_i < 0)				continue;
							if(pm_i >= iPMx_SIZE)		continue;

							if(PMx__Is_Available(pm_i) < 0)		continue;

							NEXT__LOOP;
						}
					}
				}

				// ...
				CString sch_module;
				sch_module.Format("%s-%s",para_module,para_slot);

				CStringArray l_pm;
				CStringArray l_rcp;
				xSCH_MATERIAL_CTRL->Get__CUR_PROCESS_INFO(sch_module,l_pm,l_rcp);

				int pm_i;
				int limit = l_pm.GetSize();
				int i;

				for(i=0;i<limit;i++)
				{
					pm_i = SCH__CHECK_PM_INDEX(l_pm[i]);
					
					if(pm_i < 0)				continue;
					if(pm_i >= iPMx_SIZE)		continue;

					if(PMx__Is_Available(pm_i) < 0)				continue;
					if(PMx__Check_Empty__SlotStatus(pm_i) < 0)	continue;

					// ...
					CString pm_name = l_pm[i];
					CString pm_slot = "1";
					CString pm_rcp  = l_rcp[i];

					Seq__LBi_PMx(p_variable,
								 p_alarm,
								 empty_arm,
								 para_module,para_slot,
								 pm_i,pm_name,pm_slot,pm_rcp);

					NEXT__LOOP;
				}
			}
		}
	}

	NEXT__LOOP;
}
int  CObj__DUAL_ARM_STD::
AUTO_CTRL__LBi_To_Ready_PMx__ALL_MODE(CII_OBJECT__VARIABLE *p_variable,
									  CII_OBJECT__ALARM *p_alarm)
{
	DECLARE__EXT_CTRL(p_variable);

	// Check : Wafer State ...
	if(dCH__VAC_RB__CFG_PICK_WAFER_CONDITION->Check__DATA(STR__ONLY_PROCESSED) > 0)
	{
		if(VAC_RB__Check_Occupied__A_Arm() > 0)
		{
			if(xCH__VAC_RB__SLOT01_STATUS->Check__DATA(STR__MAPPED) < 0)			NEXT__LOOP;
		}
		if(VAC_RB__Check_Occupied__B_Arm() > 0)
		{
			if(xCH__VAC_RB__SLOT02_STATUS->Check__DATA(STR__MAPPED) < 0)			NEXT__LOOP;
		}
	}

	// ...
	{
		CString arm_type;
		
		// Arm : A ...
		if(VAC_RB__Check_Occupied__A_Arm() > 0)
		{
			arm_type = _ARM__A;
			
			if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(arm_type) > 0)
			{
				// LLx - All_Mode
				if(Fnc__PMC_Empty_Ready_Check(arm_type) > 0)
				{
					NEXT__LOOP;
				}
			}
		}
		// Arm : B ...
		if(VAC_RB__Check_Occupied__B_Arm() > 0)
		{
			arm_type = _ARM__B;
			
			if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(arm_type) > 0)
			{
				// LLx - All_Mode
				if(Fnc__PMC_Empty_Ready_Check(arm_type) > 0)
				{
					NEXT__LOOP;
				}
			}
		}
	}

	// ...
	int ll_a = _LLx_INDEX__LBA;
	int ll_b = _LLx_INDEX__LBB;

	int lba_flag = -1;
	int lbb_flag = -1;
	int lb_order =  1;

	CStringArray l_lba_pmc_name;
	CStringArray l_lba_pmc_rcp;
	CStringArray l_lbb_pmc_name;
	CStringArray l_lbb_pmc_rcp;

	// ...
	{
		IDS__SCH_MATERIAL_STATUS ds_sts;
		CString sch_name;	
		int lb_slotid;
		
		int lba_portid = -1;
		int lbb_portid = -1;
		CStringArray l_lba_pmc;
		CStringArray l_lbb_pmc;
		CStringArray l_lba_rcp;
		CStringArray l_lbb_rcp;

		if((xEXT_CH__LLx__IN_OUT_FLAG[ll_a]->Check__DATA(STR__OUT) > 0)
		&& (LLx__Is_Available(ll_a) > 0)
		&& (LLx__Is_VAC(ll_a) > 0))
		{
			if(LLx__Get_Occupied__InSlot(ll_a, lb_slotid) > 0)
			{
				sch_name.Format("%s-%1d", MODULE__LBA,lb_slotid);
				
				if(xSCH_MATERIAL_CTRL->Get__MATERIAL_STATUS(sch_name,ds_sts) > 0)
				{
					lba_portid = ds_sts.iPORT_ID;	
					
					xSCH_MATERIAL_CTRL->Get__CUR_PROCESS_INFO(sch_name,l_lba_pmc,l_lba_rcp);
				}
			}
		}
		
		if((xEXT_CH__LLx__IN_OUT_FLAG[ll_b]->Check__DATA(STR__OUT) > 0)
		&& (LLx__Is_Available(ll_b) > 0)
		&& (LLx__Is_VAC(ll_b) > 0))
		{
			if(LLx__Get_Occupied__InSlot(ll_b, lb_slotid) > 0)
			{
				sch_name.Format("%s-%1d", MODULE__LBB,lb_slotid);
				
				if(xSCH_MATERIAL_CTRL->Get__MATERIAL_STATUS(sch_name,ds_sts) > 0)
				{
					lbb_portid = ds_sts.iPORT_ID;	
					
					xSCH_MATERIAL_CTRL->Get__CUR_PROCESS_INFO(sch_name,l_lbb_pmc,l_lbb_rcp);
				}
			}
		}
		
		// ...
		int dual_check = -1;

		// ...
		{
			l_lba_pmc_name.RemoveAll();
			l_lba_pmc_rcp.RemoveAll();
			l_lbb_pmc_name.RemoveAll();
			l_lbb_pmc_rcp.RemoveAll();
			
			l_lba_pmc_name.Copy(l_lba_pmc);
			l_lba_pmc_rcp.Copy(l_lba_rcp);
			l_lbb_pmc_name.Copy(l_lbb_pmc);
			l_lbb_pmc_rcp.Copy(l_lbb_rcp);
		}

		if((lba_portid > 0) && (lbb_portid > 0))
		{
			// ...
			{
				int lba__lp_i = lba_portid - 1;
				int lbb__lp_i = lbb_portid - 1;
				
				int lba__proc_flag = LPx__Check_Process_Type__Normal(lba__lp_i);
				int lbb__proc_flag = LPx__Check_Process_Type__Normal(lbb__lp_i);
				
				if(lba__proc_flag != lbb__proc_flag)
				{
					dual_check = 1;
				}
			}

			if(dual_check < 0)
			{
				if(Macro__Check_Equal__Any_String(l_lba_pmc,l_lbb_pmc) < 0)
				{
					if(Macro__Check_Equal__All_String(l_lba_pmc,l_lbb_pmc) < 0)
					{
						dual_check = 1;

						// ...
						CUIntArray l_job_lp;
						SCH__Get_Job_LP_List(l_job_lp);

						if(l_job_lp.GetSize() > 1)
						{
							int cur_lp = l_job_lp[0];

								 if(lba_portid == cur_lp)		lb_order = 1;
							else if(lbb_portid == cur_lp)		lb_order = 2;
						}
					}
				}
			}

			if(dual_check < 0)
			{
				if(xEXT_CH__SCH_CONCURRENT_ALWAYS_APPLY->Check__DATA(STR__DISABLE) < 0)
				{
					if(lba_portid != lbb_portid)
					{
						lb_order = -1;

						// ...
						{
							CUIntArray l_job_lp;
							SCH__Get_Job_LP_List(l_job_lp);
						
							if(l_job_lp.GetSize() > 1)
							{
								int cur_lp = l_job_lp[0];
							
									 if(lba_portid == cur_lp)			lb_order = 1;
								else if(lbb_portid == cur_lp)			lb_order = 2;
							}
						}
						
						if(lb_order > 0)
						{
							if(xEXT_CH__SCH_CONCURRENT_ALWAYS_APPLY->Check__DATA(STR__PPID_CHECK) > 0)
							{
								l_lba_pmc_name.RemoveAll();
								l_lba_pmc_rcp.RemoveAll();
								l_lbb_pmc_name.RemoveAll();
								l_lbb_pmc_rcp.RemoveAll();
								
								if(lb_order == 1)
								{
									l_lba_pmc_name.Copy(l_lba_pmc);
									l_lba_pmc_rcp.Copy(l_lba_rcp);
									
									Macro__Delete_Trg_With_Src(l_lba_pmc,
																	  l_lba_rcp,
																	  l_lbb_pmc,
																	  l_lbb_rcp,
																	  l_lbb_pmc_name,
																	  l_lbb_pmc_rcp);
								}
								else if(lb_order == 2)
								{
									l_lbb_pmc_name.Copy(l_lbb_pmc);
									l_lbb_pmc_rcp.Copy(l_lbb_rcp);
									
									Macro__Delete_Trg_With_Src(l_lbb_pmc,
																	  l_lbb_rcp,
																	  l_lba_pmc,
																	  l_lba_rcp,
																	  l_lba_pmc_name,
																	  l_lba_pmc_rcp);
								}
							}
							
							dual_check = 1;
						}

						/*
						// ...
						{
							CString msg;
							CString bff;
							
							int limit = l_job_lp.GetSize();
							int i;
							
							msg = "Job LPx ...\n";
							
							for(i=0;i<limit;i++)
							{
								bff.Format("   %1d) LP%1d \n", i+1,l_job_lp[i]);
								msg += bff;
							}
							
							bff.Format("lba_portid <- [%1d] \n", lba_portid);
							msg += bff;
							
							bff.Format("lbb_portid <- [%1d] \n", lbb_portid);
							msg += bff;
							
							bff.Format("lb_order   <- [%1d] \n", lb_order);
							msg += bff;
							
							bff.Format("dual_check <- [%1d] \n", dual_check);
							msg += bff;
							
							MessageBox(NULL,msg,"CHECK - AUTO_CTRL__LBi_To_PMx",MB_OK|MB_TOPMOST);
						}
						*/
					}	
				}
			}
		}
		
		if(dual_check > 0)
		{
			lba_flag = 1;
			lbb_flag = 1;
		}
		else
		{
			lb_order = 1;

			// ...
			CString check_lbx_type = STR__ALL;

			if(Get__First_Wafer_In_LBi(p_variable,p_alarm,check_lbx_type,lba_flag,lbb_flag) < 0)
			{
				NEXT__LOOP;
			}
		}
	}

	// ...
	CString para_module;
	CString para_slot;
	int slot_id;

	CString var_data;
	int kk;
	int cmp1_kk;
	int cmp2_kk;

	for(kk=0;kk<2;kk++)
	{
		// ...
		{
			cmp1_kk = -1;
			cmp2_kk = -1;

			if(lb_order != 2)
			{
				cmp1_kk == 1;
				cmp2_kk == 0;
			}
			else 
			{
				cmp1_kk == 0;
				cmp2_kk == 1;
			}
		}

		if(kk == cmp1_kk)
		{
			if(lba_flag < 0)
			{
				continue;	
			}

			if((xEXT_CH__LLx__IN_OUT_FLAG[ll_a]->Check__DATA(STR__OUT) > 0)
			&& (LLx__Is_Available(ll_a) > 0)
			&& (LLx__Check_Occupied__InSlot(ll_a) > 0))
			{
				if(SCH__LLx_CHECK_PRESSURE(ll_a, p_variable,p_alarm,1) < 0)
				{
					continue;
				}

				if(LLx__Get_Occupied__InSlot(ll_a, slot_id) < 0)
				{
					continue;
				}

				para_module = MODULE__LBA;
				para_slot.Format("%1d",slot_id);
			}
		}
		else if(kk == cmp2_kk)
		{
			if(lbb_flag < 0)
			{
				continue;
			}

			if((xEXT_CH__LLx__IN_OUT_FLAG[ll_b]->Check__DATA(STR__OUT) > 0)
			&& (LLx__Is_Available(ll_b) > 0)
			&& (LLx__Check_Occupied__InSlot(ll_b) > 0))
			{
				if(SCH__LLx_CHECK_PRESSURE(ll_b, p_variable,p_alarm,1) < 0)
				{
					continue;
				}

				if(LLx__Get_Occupied__InSlot(ll_b, slot_id) < 0)
				{
					continue;
				}

				para_module = MODULE__LBB;
				para_slot.Format("%1d",slot_id);
			}
		}
		else
		{
			continue;
		}

		// ...
		CString empty_arm;

		if(VAC_RB__Get_Empty__Arm_Type_From_LBx(empty_arm) < 0)
		{
			CString cfg__arm_from_lbx;
			CString cfg__arm_from_pmc;

			xCH_CFG__PICK_ARM_TYPE_FROM_LBX->Get__DATA(cfg__arm_from_lbx);
			xCH_CFG__PICK_ARM_TYPE_FROM_PMC->Get__DATA(cfg__arm_from_pmc);

			// Alarm Report ...
			{

			}				

			NEXT__LOOP;
		}

		// ...
		{
			CString sch_module;
			sch_module.Format("%s-%s",para_module,para_slot);

			// ...
			CStringArray l_pm;
			CStringArray l_rcp;

			xSCH_MATERIAL_CTRL->Get__CUR_PROCESS_INFO(sch_module,l_pm,l_rcp);

			// ...
			int flag__lp_normal = 1;
			
			if(LPx__Get_Process_Type__Normal(sch_module,flag__lp_normal) < 0)
			{
				return -1;
			}

			// ...
			int limit = l_pm.GetSize();
			int i;

			for(i=0;i<limit;i++)
			{
				int pm_i = SCH__CHECK_PM_INDEX(l_pm[i]);
					
				if(pm_i < 0)				continue;
				if(pm_i >= iPMx_SIZE)		continue;

				if(Fnc__PMx_Empty_Ready_Check(flag__lp_normal,pm_i,1) < 0)
				{
					continue;
				}

				// ...
				{
					CString pm_name = l_pm[i];
					CString pm_slot = "1";
					CString pm_rcp  = l_rcp[i];

					Seq__LBi_PMx(p_variable,
								 p_alarm,
								 empty_arm,
								 para_module,para_slot,
								 pm_i,pm_name,pm_slot,pm_rcp);
				}

				NEXT__LOOP;
			}
		}
	}

	NEXT__LOOP;
}
int  CObj__DUAL_ARM_STD::
AUTO_CTRL__LBi_PMx__1_SLOT_ALL_MODE(CII_OBJECT__VARIABLE *p_variable,
									CII_OBJECT__ALARM *p_alarm)
{
	DECLARE__EXT_CTRL(p_variable);

	// ...
	CString para_module;
	CString para_slot;
	int slot_id;

	CString var_data;

	int ll_limit = iLLx_LIMIT;
	int ll_i;

	for(ll_i=0; ll_i<ll_limit; ll_i++)
	{
		if((xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Check__DATA(STR__OUT) > 0)
		&& (LLx__Is_Available(ll_i) > 0)
		&& (LLx__Check_Occupied__InSlot_With_1_Slot_All_Mode(ll_i) > 0))
		{
			if(SCH__LLx_CHECK_PRESSURE(ll_i, p_variable,p_alarm,1) < 0)
			{
				continue;
			}

			if(LLx__Get_Occupied__InSlot_With_1_Slot_All_Mode(ll_i, slot_id) < 0)
			{
				continue;
			}

			para_module = Get__LLx_NAME(ll_i);
			para_slot.Format("%1d",slot_id);
		}

		// ...
		CString empty_arm;

		if(VAC_RB__Get_Empty__Arm_Type_From_LBx(empty_arm) < 0)
		{
			NEXT__LOOP;
		}

		// ...
		CString arm_type;

		if(VAC_RB__Get_Occupied__Arm_Type(arm_type) > 0)
		{
			IDS__SCH_MATERIAL_STATUS ds_sts;
			xSCH_MATERIAL_CTRL->Get__MATERIAL_STATUS(arm_type,ds_sts);

			int material_check = -1;

			if(ds_sts.sMATERIAL_STS.CompareNoCase(STR__ALIGNED) == 0)
			{
				material_check = 1;
			}
			else if(ds_sts.sMATERIAL_STS.CompareNoCase(STR__PROCESSED) == 0)
			{
				if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(arm_type) > 0)
				{
					material_check = 1;
				}
			}

			if(material_check > 0)
			{
				CStringArray l_pm;
				CStringArray l_rcp;

				if(xSCH_MATERIAL_CTRL->Get__CUR_PROCESS_INFO(arm_type,l_pm,l_rcp) > 0)
				{
					int limit = l_pm.GetSize();
					int i;

					for(i=0;i<limit;i++)
					{
						int pm_i = SCH__CHECK_PM_INDEX(l_pm[i]);
					
						if(pm_i < 0)				continue;
						if(pm_i >= iPMx_SIZE)		continue;

						if(PMx__Is_Available(pm_i) < 0)	
						{
							continue;
						}

						NEXT__LOOP;
					}
				}
			}
		}

		// ...
		{
			CString sch_module;
			sch_module.Format("%s-%s",para_module,para_slot);

			CStringArray l_pm;
			CStringArray l_rcp;
			xSCH_MATERIAL_CTRL->Get__CUR_PROCESS_INFO(sch_module,l_pm,l_rcp);

			int limit = l_pm.GetSize();
			int i;

			for(i=0;i<limit;i++)
			{
				int pm_i = SCH__CHECK_PM_INDEX(l_pm[i]);
				
				if(pm_i < 0)				continue;
				if(pm_i >= iPMx_SIZE)		continue;

				if(PMx__Is_Available(pm_i) < 0)				continue;
				if(PMx__Check_Empty__SlotStatus(pm_i) < 0)	continue;

				CString pm_name = l_pm[i];
				CString pm_slot = "1";
				CString pm_rcp  = l_rcp[i];

				Seq__LBi_PMx(p_variable,
							 p_alarm,
							 empty_arm,
							 para_module,para_slot,
							 pm_i,
							 pm_name,
							 pm_slot,
							 pm_rcp);

				NEXT__LOOP;
			}
		}
	}

	NEXT__LOOP;
}


int  CObj__DUAL_ARM_STD::
Seq__LBi_PMx(CII_OBJECT__VARIABLE *p_variable,
			 CII_OBJECT__ALARM *p_alarm,
			 const CString& arm_type,
			 const CString& lbx_name,
			 const CString& lbx_slot,
			 const int pm_i,
			 const CString& pm_name,
			 const CString& pm_slot,
			 const CString& pm_rcp)
{
	DECLARE__EXT_CTRL(p_variable);

	CString log_id = "Seq__LBi_PMx()";

	// ...
	{
		int act_flag  = -1;
		int move_flag = -1;

		CString sch_module;
		sch_module.Format("%s-%s",lbx_name,lbx_slot);

		act_flag = SCH__PICK_MODULE(p_variable,p_alarm, log_id, false,arm_type,lbx_name,lbx_slot,sch_module);

		if(act_flag < 0)
		{
			move_flag = SCH__Check_Material_Pick(arm_type);
		}
		else
		{
			move_flag = 1;
		}

		if(move_flag > 0)
		{
			xSCH_MATERIAL_CTRL->Pick__From_MODULE(sch_module,arm_type);	
		}

		if(act_flag < 0)
		{
			NEXT__LOOP;
		}
	}

	// ...
	{
		int active_pump = 1;

		// ...
		{
			int macterial_check = -1;
			CString check_arm;

			if(VAC_RB__Check_Occupied__A_Arm() > 0)
			{
				check_arm = "A";

				if(check_arm.CompareNoCase(arm_type) != 0)
				{
					if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(check_arm) < 0)
					{
						macterial_check = 1;			
					}
				}
			}
			if(VAC_RB__Check_Occupied__B_Arm() > 0)
			{
				check_arm = "B";

				if(check_arm.CompareNoCase(arm_type) != 0)
				{
					if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(check_arm) > 0)
					{
						macterial_check = 1;
					}
				}
			}

			if(macterial_check > 0)
			{
				int active__sch_mode = -1;
				int active__sch_ll   = -1;

				// ...
				CStringArray l_ll_id;
				CStringArray l_ll_mode;
				CStringArray l_ll_slot;

				xSCH_MATERIAL_CTRL->Get__LLx_OUT_OF_EDIT_TYPE(arm_type, l_ll_id,l_ll_mode,l_ll_slot);

				if(l_ll_id.GetSize() == 1)
				{
					int index_ll = 0;
					int ll_id = atoi(l_ll_id[index_ll]);

					CString str_mode = l_ll_mode[index_ll];
					CString str_slot = l_ll_slot[index_ll];

					if(str_mode.CompareNoCase("ALL") == 0)
					{
						int ll_i = ll_id - 1;

						if((ll_i >= 0) && (ll_i < iLLx_LIMIT))						
						{
							if(LLx__Check_Empty__InSlot(ll_i) > 0)
							{
								active_pump = -1;
							}
						}
					}
				}
			}
		}

		if(active_pump > 0)
		{
			int ll_i = Get__LLx_INDEX(lbx_name);
			if(ll_i >= 0)
			{
				if(LLx__Check_Empty__InSlot(ll_i) > 0)
				{
					if(SCH_RUN__LLx_VENT(ll_i, log_id, "1") > 0)
					{
						xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Set__DATA("IN");
					}
				}
			}
		}
	}

	// jglee : 2020.10.20
	if(SCH__PMx_CHECK_PRESSURE(pm_i,p_variable,p_alarm) < 0)
	{
		NEXT__LOOP;		
	}

	if((PMx__Is_Available(pm_i) > 0)
	&& (PMx__Check_Empty__SlotStatus(pm_i) > 0))
	{
		int act_flag  = -1;
		int move_flag = -1;

		CString sch_name;
		sch_name.Format("%s-%s",pm_name,pm_slot);
		
		xSCH_MATERIAL_CTRL->Set__CURRENT_RECIPE(arm_type,pm_rcp);

		act_flag = SCH__PLACE_MODULE(p_variable,p_alarm, log_id, false,arm_type,pm_name,pm_slot,sch_name);

		if(act_flag < 0)
		{
			move_flag = SCH__Check_Material_Place(arm_type);
		}
		else
		{
			move_flag = 1;
		}

		if(move_flag > 0)
		{
			xSCH_MATERIAL_CTRL->Place__To_MODULE(arm_type,sch_name);

			xSCH_MATERIAL_CTRL->Set__MATERIAL_STATUS(sch_name,"PROCESSING");
			xSCH_MATERIAL_CTRL->Goto__NEXT_PROCESS(sch_name);
		}

		if(act_flag < 0)
		{
			NEXT__LOOP;
		}
	}

	return 1;
}

int  CObj__DUAL_ARM_STD::
AUTO_CTRL__RB_PMx(CII_OBJECT__VARIABLE *p_variable,
				  CII_OBJECT__ALARM* p_alarm)
{
	if(dCH__VAC_RB__CFG_LLx_CTRL_ONLY_INPUT_OUTPUT_MODE->Check__DATA(STR__DUAL) > 0)
	{
		_AUTO_CTRL__RB_PMx_WITH_DUAL_TYPE(p_variable, p_alarm);
	}
	else
	{
		CStringArray l_arm;	
		Get__Arm_List_To_Process(l_arm);

		int i_limit = l_arm.GetSize();

		for(int i=0; i<i_limit; i++)
		{
			CString arm_type = l_arm[i];

			_AUTO_CTRL__RB_PMx(p_variable,p_alarm, arm_type);
		}
	}

	return 1;
}
int  CObj__DUAL_ARM_STD::
_AUTO_CTRL__RB_PMx_WITH_DUAL_TYPE(CII_OBJECT__VARIABLE *p_variable,
								  CII_OBJECT__ALARM* p_alarm)
{
	CString log_id = "_AUTO_CTRL__RB_PMx_WITH_DUAL_TYPE()";

	// ...
	bool active__dual_arm = false;

	if(dCH__VAC_RB__CFG_DUAL_ARM_MOVING_AT_THE_SAME_TIME->Check__DATA(STR__ENABLE) > 0)
	{
		active__dual_arm = true;
	}

	if(active__dual_arm)
	{
		CString arm_a = _ARM__A;
		CStringArray l_pmc_a;
		CStringArray l_rcp_a;
		Fnc__Get_PMx_List(arm_a, l_pmc_a,l_rcp_a);

		CString arm_b = _ARM__B;
		CStringArray l_pmc_b;
		CStringArray l_rcp_b;
		Fnc__Get_PMx_List(arm_b, l_pmc_b,l_rcp_b);

		for(int pm_i=0; pm_i < iPMx_SIZE; )
		{
			int id_left  = ++pm_i;
			int id_right = ++pm_i;

			CString pm_left;
			CString pm_right;
			CString pm_name;

			pm_left.Format("PM%1d",  id_left);
			pm_right.Format("PM%1d", id_right);

			pm_name = pm_left;

			// Check : PM-Left & PM2-Right  
			if((_Check__PM_Name(pm_left,  l_pmc_a) > 0)
			&& (_Check__PM_Name(pm_right, l_pmc_b) > 0))
			{
				CString rcp_left  = Fnc__Get_PM_Rcp(pm_left,  l_pmc_a,l_rcp_a);
				CString rcp_right = Fnc__Get_PM_Rcp(pm_right, l_pmc_b,l_rcp_b);

				xSCH_MATERIAL_CTRL->Set__CURRENT_RECIPE(_ARM__A, rcp_left);
				xSCH_MATERIAL_CTRL->Set__CURRENT_RECIPE(_ARM__B, rcp_right);

				//
				CString arm_type = _ARM__AB;
				CString sch_left;
				CString sch_right;

				sch_left.Format("%s-1",  pm_left);
				sch_right.Format("%s-1", pm_right);

				int act_flag = SCH__PLACE_MODULE(p_variable,p_alarm, log_id, false, arm_type, pm_name, "1", sch_left,sch_right);

				if(VAC_RB__Check_Empty__A_Arm() > 0)
				{
					xSCH_MATERIAL_CTRL->Place__To_MODULE(arm_a, sch_left);
					
					xSCH_MATERIAL_CTRL->Set__MATERIAL_STATUS(sch_left, STR__PROCESSING);
					xSCH_MATERIAL_CTRL->Goto__NEXT_PROCESS(sch_left);
				}
				if(VAC_RB__Check_Empty__B_Arm() > 0)
				{
					xSCH_MATERIAL_CTRL->Place__To_MODULE(arm_b, sch_right);
					
					xSCH_MATERIAL_CTRL->Set__MATERIAL_STATUS(sch_right, STR__PROCESSING);
					xSCH_MATERIAL_CTRL->Goto__NEXT_PROCESS(sch_right);
				}

				if(act_flag < 0)
				{
					NEXT__LOOP;
				}

				NEXT__LOOP;
			}
		}
	}
	else
	{
		CStringArray l__arm_type;
		VAC_RB__Get_Occupied__Arm_Type(l__arm_type);
		
		int i_limit = l__arm_type.GetSize();
		for(int i=0; i<i_limit; i++)
		{
			CString arm_type = l__arm_type[i];

			if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(arm_type) < 0)
			{
				continue;
			}

			// ...
			bool active__wafer_constraint = false;

			if(dCH__VAC_RB__CFG_PICK_WAFER_CONDITION->Check__DATA(STR__ONLY_PROCESSED) > 0)
			{
				if(arm_type.CompareNoCase(_ARM__A) == 0)
				{
					if(xCH__VAC_RB__SLOT01_STATUS->Check__DATA(STR__MAPPED) > 0)			active__wafer_constraint = true;
				}
				else if(arm_type.CompareNoCase(_ARM__B) == 0)
				{
					if(xCH__VAC_RB__SLOT02_STATUS->Check__DATA(STR__MAPPED) > 0)			active__wafer_constraint = true;
				}
			}

			// ...
			int active__lp_normal = 1;

			if(LPx__Get_Process_Type__Normal(arm_type, active__lp_normal) < 0)
			{
				continue;
			}

			// ...
			CStringArray l__pm_name;
			CStringArray l__pm_rcp;

			if(xSCH_MATERIAL_CTRL->Get__CUR_PROCESS_INFO(arm_type, l__pm_name,l__pm_rcp) < 0)
			{
				continue;
			}

			int trg__goto_pm_index = Get__PM_INDEX_OF_SCH_USER_DEF_GOTO_PMX(log_id, arm_type, l__pm_name);
			if(trg__goto_pm_index >= 0)
			{
				CString str__pm_name = l__pm_name[trg__goto_pm_index];
				CString str__pm_rcp  = l__pm_rcp[trg__goto_pm_index];

				l__pm_name.RemoveAll();
				l__pm_rcp.RemoveAll();

				l__pm_name.Add(str__pm_name);
				l__pm_rcp.Add(str__pm_rcp);
			}

			// ...
			{
				int count__wfr_type = -1;

				int k_limit = l__pm_name.GetSize();
				for(int k=0; k < k_limit; k++)
				{
					CString pmc_name = l__pm_name[k];

					int pm_index = Macro__Get_PMC_INDEX(pmc_name);
					if(pm_index < 0)		continue;

					if(active__lp_normal < 0)
					{
						if(xEXT_CH__PMx_PROCESS_TYPE[pm_index]->Check__DATA(STR__TEST) < 0)
						{
							continue;
						}
					}
					
					count__wfr_type = 1;
				}

				if(count__wfr_type < 0)
				{
					active__lp_normal = 1;
				}
			}

			// ...
			CUIntArray l__pm_index;
			CString pmc_name;

			// ...
			bool active__pm_check  = false;
			bool active__all_check = false;

			if(!active__pm_check)
			{
				if(dCH__VAC_RB__CFG_PMx_To_PMx_MOVE_PATH->Check__DATA(STR__ODD_EVEN) > 0)
				{
					active__all_check = true;
				}

				// ...
				int k_limit = l__pm_name.GetSize();
			
				for(int k=0; k<k_limit; k++)
				{
					pmc_name = l__pm_name[k];

					if(active__wafer_constraint)
					{
						if(!VAC_RB__Check__Arm_Type_With_PMx_Constraint(arm_type, pmc_name))
						{
							continue;
						}
					}
					else
					{
						/*
						if((dCH__VAC_RB__CFG_PMx_To_PMx_CONSTRAINT->Check__DATA(STR__ALL) > 0)
						|| (dCH__VAC_RB__CFG_PMx_To_PMx_CONSTRAINT->Check__DATA(STR__SAME_PATH) > 0)
						|| (dCH__VAC_RB__CFG_PMx_To_PMx_CONSTRAINT->Check__DATA(STR__SAME_PROCESS) > 0))
						{

						}
						*/

						if(dCH__VAC_RB__CFG_PMx_To_PMx_MOVE_PATH->Check__DATA(STR__ODD_EVEN) > 0)
						{
							CString last__pmc_name;
							CString last__pmc_rcp;

							xSCH_MATERIAL_CTRL->Get__Last_Processing_Info(arm_type, last__pmc_name,last__pmc_rcp);

							bool active__pmc_odd  = _Check__Odd_PMx(pmc_name);
							bool active__last_odd = _Check__Odd_PMx(last__pmc_name);

							if(active__last_odd)
							{
								if(!active__pmc_odd)		continue;
							}
							else
							{
								if(active__pmc_odd)			continue;
							}
						}
					}

					// ...
					int pm_index = Macro__Get_PMC_INDEX(pmc_name);
					if(pm_index < 0)		continue;

					if(xCH__PMx__OBJ_VIRTUAL_STATUS[pm_index]->Check__DATA(STR__DISABLE) > 0)
					{
						continue;
					}

					if(active__lp_normal > 0)
					{
						if(xEXT_CH__PMx_PROCESS_TYPE[pm_index]->Check__DATA(STR__NORMAL) < 0)
						{
							continue;
						}
					}
					else
					{
						if(xEXT_CH__PMx_PROCESS_TYPE[pm_index]->Check__DATA(STR__TEST) < 0)
						{
							continue;
						}
					}

					if(PMx__Check_Empty__SlotStatus(pm_index) < 0)
					{
						continue;
					}
					if(PMx__Is_Available(pm_index) < 0)
					{
						if(PMx__Is_Ready(pm_index) > 0)
						{
							active__all_check = false;							
						}
						continue;
					}

					l__pm_index.Add(pm_index);

					active__pm_check = true;
					// break;
				}
			}

			if((!active__pm_check) && (active__all_check))
			{
				int k_limit = l__pm_name.GetSize();

				for(int k=0; k<k_limit; k++)
				{
					pmc_name = l__pm_name[k];

					int pm_index = Macro__Get_PMC_INDEX(pmc_name);
					if(pm_index < 0)		continue;

					if(active__lp_normal > 0)
					{
						if(xEXT_CH__PMx_PROCESS_TYPE[pm_index]->Check__DATA(STR__NORMAL) < 0)
						{
							continue;
						}
					}
					else
					{
						if(xEXT_CH__PMx_PROCESS_TYPE[pm_index]->Check__DATA(STR__TEST) < 0)
						{
							continue;
						}
					}

					if(PMx__Is_Available(pm_index) < 0)					continue;
					if(PMx__Check_Empty__SlotStatus(pm_index) < 0)		continue;

					l__pm_index.Add(pm_index);

					active__pm_check = true;
					// break;
				}
			}

			if(!active__pm_check)
			{
				continue;
			}

			// ...
			int min__pm_index = _Get__MIN_PMx_INDEX_OF_PMx_IN_COUNT(l__pm_index);
			if(min__pm_index < 0)			continue;

			int min__pm_id = min__pm_index + 1;
			pmc_name.Format("PM%1d", min__pm_id);

			// ...
			CString sch_name;
			sch_name.Format("%s-1", pmc_name);

			CString pmc_rcp = Fnc__Get_PM_Rcp(pmc_name, l__pm_name,l__pm_rcp);
						
			xSCH_MATERIAL_CTRL->Set__CURRENT_RECIPE(arm_type, pmc_rcp);

			int act_flag = SCH__PLACE_MODULE(p_variable,p_alarm, log_id, false, arm_type, pmc_name, "1", sch_name);

			if(VAC_RB__Check_Empty__Arm_Type(arm_type) > 0)
			{
				xSCH_MATERIAL_CTRL->Place__To_MODULE(arm_type, sch_name);

				xSCH_MATERIAL_CTRL->Set__MATERIAL_STATUS(sch_name, "PROCESSING");
				xSCH_MATERIAL_CTRL->Goto__NEXT_PROCESS(sch_name);
			}

			if(act_flag < 0)
			{
				NEXT__LOOP;
			}
		}

		NEXT__LOOP;
	}

	return -11;
}
int  CObj__DUAL_ARM_STD::
_AUTO_CTRL__RB_PMx(CII_OBJECT__VARIABLE *p_variable,
				   CII_OBJECT__ALARM* p_alarm,
				   const CString& arm_type)
{
	DECLARE__EXT_CTRL(p_variable);

	// ...
	CString log_id = "_AUTO_CTRL__RB_PMx()";

	// ...
	CStringArray l_pmc;
	CStringArray l_pmc_rcp;

	CString pm_name;
	CString pm_slot = "1";

	// ...
	{
		SCH__Change_Current_Route_Step(p_variable,p_alarm, arm_type);
	}

	if(xSCH_MATERIAL_CTRL->Get__CUR_PROCESS_INFO(arm_type,l_pmc,l_pmc_rcp) < 0)
	{
		return 1;
	}

	// ...
	int flag__lp_normal = 1;

	if(LPx__Get_Process_Type__Normal(arm_type,flag__lp_normal) < 0)
	{
		return -1;
	}

	// ...
	{
		int pm_count = 0;

		int pmc_check = -1;
		int pm_i;

		int limit = l_pmc.GetSize();
		int i;

		for(i=0;i<limit;i++)
		{
			CString pm_name = l_pmc[i];

			pm_i = SCH__CHECK_PM_INDEX(pm_name);
			if(pm_i < 0)		continue;

			pm_count++;

			if(PMx__Is_Ready(pm_i) < 0)
			{
				continue;
			}

			/*
			if(PMx__Check_Empty__SlotStatus(pm_i) < 0)
			{
				continue;
			}
			*/

			/*
			if(PMx__Check_Empty__SlotStatus(pm_i) > 0)
			{
				if(xCH__PMx__NEXT_PROCESS[pm_i]->Check__DATA(STR__LEAK_CHECK) > 0)
				{
					continue;
				}
			}
			*/

			if(flag__lp_normal > 0)
			{
				if(xEXT_CH__PMx_PROCESS_TYPE[pm_i]->Check__DATA(STR__NORMAL) < 0)
				{
					continue;
				}
			}
			else
			{
				if(xEXT_CH__PMx_PROCESS_TYPE[pm_i]->Check__DATA(STR__TEST) < 0)
				{
					continue;
				}
			}

			pmc_check = 1;
			break;
		}

		if(pm_count < 1)
		{
			return 1;
		}

		if(pmc_check < 0)
		{
			int alarm_id = ALID__PMC__NOT_AVAILABLE;
			CString alarm_msg;
			CString r_act;

			// ...
			{
				if(flag__lp_normal > 0)
				{
					alarm_msg = "There is no PMC with \"NORMAL\" process type. \n";
				}
				else
				{
					alarm_msg = "There is no PMC with \"TEST\" process type. \n";				
				}
				
				alarm_msg += "At least, one PM's status must be CtcInUse or Standby among the following PMs;\n";

				// ...
				int limit = l_pmc.GetSize();
				int i;
			
				for(i=0;i<limit;i++)
				{
					alarm_msg += l_pmc[i];
					alarm_msg += ", ";
				}

				alarm_msg += "\n";
			}

			// ..
			{
				p_alarm->Check__ALARM(alarm_id,r_act);
				p_alarm->Post__ALARM_With_MESSAGE(alarm_id,alarm_msg);
			}
			return 1;
		}
	}

	// ...
	CStringArray l_module;
	CStringArray l_recipe;
	CStringArray l_pmx_incount;

	// ...
	{
		CString var_data;

		int i_limit = l_pmc.GetSize();
		int i;

		for(i=0;i<i_limit;i++)
		{
			int pm_i = SCH__CHECK_PM_INDEX(l_pmc[i]);
			if(pm_i < 0)
			{
				continue;
			}

			if(xEXT_CH__CFG__PMx_USE[pm_i]->Check__DATA(STR__ENABLE) > 0)
			{
				if(PMx__Is_Available(pm_i) < 0)					continue;
			}
			else
			{
				if(PMx__Is_Idle(pm_i) < 0)						continue;
				if(PMx__Check_Empty__SlotStatus(pm_i) > 0)		continue;
			}
			
			if(SCH__PMx_CHECK_PRESSURE(pm_i,p_variable,p_alarm) < 0)
			{
				continue;
			}

			if(flag__lp_normal > 0)
			{
				if(xEXT_CH__PMx_PROCESS_TYPE[pm_i]->Check__DATA(STR__NORMAL) < 0)
				{
					continue;
				}
			}
			else
			{
				if(xEXT_CH__PMx_PROCESS_TYPE[pm_i]->Check__DATA(STR__TEST) < 0)
				{
					continue;
				}
			}

			if(xCH_CFG__SCH_EXCHANGE_PRIORITY->Check__DATA(STR__WAFER) > 0)
			{
				if(PMx__Check_Occupied__SlotStatus(pm_i) > 0)
				{
					xCH__PMx__IN_COUNT[pm_i]->Get__DATA(var_data);
				}
				else
				{
					var_data = "1000000";
				}
			}
			else
			{
				if(PMx__Check_Occupied__SlotStatus(pm_i) > 0)
				{
					int cur_in_count;
					int cur_idle_sec;
					CString cur_data;

					xCH__PMx__IN_COUNT[pm_i]->Get__DATA(cur_data);
					cur_in_count = atoi(cur_data);

					/*
					xCH__PMx__IDLE_TIME[pm_i]->Get__DATA(cur_data);
					cur_idle_sec = atoi(cur_data);
					*/
					cur_idle_sec = 0;

					var_data.Format("%1d", (1000000 + cur_in_count+cur_idle_sec));
				}
				else
				{
					if(xCH__PMx__FIRST_WAFER_FLAG[pm_i]->Check__DATA(STR__YES) > 0)
					{
						int cur_pmc_sec = - (10 - pm_i);
	
						var_data.Format("%1d", cur_pmc_sec);	
					}
					else
					{
						xCH__PMx__IDLE_TIME[pm_i]->Get__DATA(var_data);
						int cur_idle_sec = atoi(var_data);
						int cur_pmc_sec = 1000000 - cur_idle_sec;

						if(cur_pmc_sec < 1)
						{
							cur_pmc_sec = 1;
						}

						var_data.Format("%1d", cur_pmc_sec);	
					}
				}				
			}

			l_pmx_incount.Add(var_data);
			l_module.Add(l_pmc[i]);
			l_recipe.Add(l_pmc_rcp[i]);
		}
	}

	// Order Check ...
	{
		int k_limit = l_pmx_incount.GetSize();
		int k;

		for(k=0; k<k_limit; k++)
		{
			CString incount_cur = l_pmx_incount[k];
			CString moudle_cur  = l_module[k];
			CString recipe_cur  = l_recipe[k];

			// ...
			bool active__data_change = false;

			int count_cur = atoi(incount_cur);	
			for(int t = k+1; t<k_limit; t++)
			{
				int count_xxx = atoi(l_pmx_incount[t]);
				if(count_xxx >= count_cur)			continue;

				count_cur = count_xxx;
				active__data_change = true;

				// ...
				{
					CString incount_xxx = l_pmx_incount[t];
					CString moudle_xxx  = l_module[t];
					CString recipe_xxx  = l_recipe[t];

					l_pmx_incount[t] = incount_cur; 
					l_module[t] = moudle_cur; 
					l_recipe[t] = recipe_cur;

					incount_cur = incount_xxx; 
					moudle_cur  = moudle_xxx; 
					recipe_cur  = recipe_xxx;
				}
			}

			if(active__data_change)
			{
				l_pmx_incount[k] = incount_cur;
				l_module[k] = moudle_cur; 
				l_recipe[k] = recipe_cur;
			}
		}
	}

	// ...
	{
		CStringArray l__pmc_name;
		CStringArray l__pmc_rcp;

		xSCH_MATERIAL_CTRL->Get__CUR_PROCESS_INFO(arm_type, l__pmc_name,l__pmc_rcp);

		int trg__goto_pm_index = Get__PM_INDEX_OF_SCH_USER_DEF_GOTO_PMX(log_id, arm_type, l__pmc_name);
		if(trg__goto_pm_index >= 0)
		{
			int pmc_id = trg__goto_pm_index + 1;

			CString str__pmc_name;
			str__pmc_name.Format("PM%1d", pmc_id);

			int s_index = Fnc__Get_Find_Index(str__pmc_name, l_module);
			if(s_index < 0)
			{
				NEXT__LOOP;
			}

			CString str_module = l_module[s_index];
			CString str_recipe = l_recipe[s_index];
			CString str_count  = l_pmx_incount[s_index];

			l_module.RemoveAll();
			l_recipe.RemoveAll();
			l_pmx_incount.RemoveAll();

			l_module.Add(str_module);
			l_recipe.Add(str_recipe);
			l_pmx_incount.Add(str_count);
		}
	}

	// ...
	IDS__SCH_MATERIAL_STATUS ds_sts;

	CString sch_name;
	CString pm_rcp;

	int i_limit = l_module.GetSize();
	int i;

	for(i=0; i<i_limit; i++)
	{
		int pm_i = SCH__CHECK_PM_INDEX(l_module[i]);
		if(pm_i < 0)		continue;

		// ...
		{
			pm_name.Format("PM%1d",  pm_i+1);
			sch_name.Format("%s-%s", pm_name,pm_slot);
		
			pm_rcp = l_recipe[i];
		}
	
		if(xEXT_CH__CFG__PMx_USE[pm_i]->Check__DATA(STR__ENABLE) > 0)
		{
			if(PMx__Is_Available(pm_i) < 0)			continue;
		}
		else
		{
			if(PMx__Is_Idle(pm_i) < 0)				continue;
		}

		if(flag__lp_normal > 0)
		{
			if(xEXT_CH__PMx_PROCESS_TYPE[pm_i]->Check__DATA(STR__NORMAL) < 0)
			{
				continue;
			}
		}
		else
		{
			if(xEXT_CH__PMx_PROCESS_TYPE[pm_i]->Check__DATA(STR__TEST) < 0)
			{
				continue;
			}
		}

		if(PMx__Check_Empty__SlotStatus(pm_i) > 0)
		{
			int act_flag  = -1;
			int move_flag = -1;

			// ...
			{
				xSCH_MATERIAL_CTRL->Set__CURRENT_RECIPE(arm_type,pm_rcp);

				act_flag = SCH__PLACE_MODULE(p_variable,p_alarm, log_id, false,arm_type,pm_name,pm_slot,sch_name);

				if(act_flag < 0)
				{
					move_flag = SCH__Check_Material_Place(arm_type);
				}
				else
				{
					move_flag = 1;
				}
			}

			if(move_flag > 0)
			{
				xSCH_MATERIAL_CTRL->Place__To_MODULE(arm_type,sch_name);
			
				xSCH_MATERIAL_CTRL->Set__MATERIAL_STATUS(sch_name, STR__PROCESSING);
				xSCH_MATERIAL_CTRL->Goto__NEXT_PROCESS(sch_name);
			}

			if(act_flag < 0)
			{
				NEXT__LOOP;
			}
			return 1;
		}
	}

	// ...
	{
		CString arm_type;
		
		// Arm : A ...
		if(VAC_RB__Check_Occupied__A_Arm() > 0)
		{
			arm_type = _ARM__A;
			
			if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(arm_type) > 0)
			{
				if(xCH_CFG__SCH_EXCHANGE_PRIORITY->Check__DATA(STR__PMC) > 0)
				{
					if(Fnc__PMC_Empty_Ready_Check(arm_type) > 0)
					{
						return 1;
					}
				}
			}
		}
		// Arm : B ...
		if(VAC_RB__Check_Occupied__B_Arm() > 0)
		{
			arm_type = _ARM__B;
			
			if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(arm_type) > 0)
			{
				if(xCH_CFG__SCH_EXCHANGE_PRIORITY->Check__DATA(STR__PMC) > 0)
				{
					if(Fnc__PMC_Empty_Ready_Check(arm_type) > 0)
					{
						return 1;
					}
				}
			}
		}
	}

	// Check : Wafer State ...
	if(dCH__VAC_RB__CFG_PICK_WAFER_CONDITION->Check__DATA(STR__ONLY_PROCESSED) > 0)
	{
		if(VAC_RB__Check_Occupied__A_Arm() > 0)
		{
			if(xCH__VAC_RB__SLOT01_STATUS->Check__DATA(STR__MAPPED) > 0)			NEXT__LOOP;
		}
		if(VAC_RB__Check_Occupied__B_Arm() > 0)
		{
			if(xCH__VAC_RB__SLOT02_STATUS->Check__DATA(STR__MAPPED) > 0)			NEXT__LOOP;
		}
	}

	// ...
	i_limit = l_module.GetSize();

	for(i=0; i<i_limit; i++)
	{
		int pm_i = SCH__CHECK_PM_INDEX(l_module[i]);
		if(pm_i < 0)			continue;

		if(VAC_RB__Check_Empty__Dual_Arm() > 0)
		{

		}
		else
		{
			if(_SCH_CHECK__PMo_RB__ALL_MODE() < 0)
			{
				NEXT__LOOP;
			}
		}

		// ...
		{
			pm_name.Format("PM%1d",pm_i+1);
			sch_name.Format("%s-%s",pm_name,pm_slot);
		
			pm_rcp = l_recipe[i];
		}
		
		if(xEXT_CH__CFG__PMx_USE[pm_i]->Check__DATA(STR__ENABLE) > 0)
		{
			if(PMx__Is_Available(pm_i) < 0)			continue;
		}
		else
		{
			if(PMx__Is_Idle(pm_i) < 0)				continue;
		}
		
		if(flag__lp_normal > 0)
		{
			if(xEXT_CH__PMx_PROCESS_TYPE[pm_i]->Check__DATA(STR__NORMAL) < 0)
			{
				continue;
			}
		}
		else
		{
			if(xEXT_CH__PMx_PROCESS_TYPE[pm_i]->Check__DATA(STR__TEST) < 0)
			{
				continue;
			}
		}

		if(PMx__Check_Empty__SlotStatus(pm_i) > 0)
		{
			return 1;
		}
		else
		{
			if(xSCH_MATERIAL_CTRL->Get__MATERIAL_STATUS(sch_name,ds_sts) < 0)
			{
				continue;
			}
		}
	
		if(VAC_RB__Check_Empty__Arm_Type() < 0)
		{
			continue;
		}
		
		if((PMx__Check_Next_Process(pm_i,p_alarm) < 0)
		&& (xSCH_MATERIAL_CTRL->Check__POST_CLN_In_PMC(sch_name,pm_i+1) < 0)
		&& (xEXT_CH__CFG__PMx_USE[pm_i]->Check__DATA(STR__ENABLE) > 0))
		{
			CString empty_arm;

			if(VAC_RB__Get_Empty__Arm_Type_From_All(empty_arm) < 0)
			{
				return 1;
			}
			
			if(xCH_CFG__PMx_SWAP_FLAG->Check__DATA(STR__ENABLE) > 0)
			{
				xSCH_MATERIAL_CTRL->Set__CURRENT_RECIPE(arm_type,pm_rcp);

				if(SCH__SWAP_PMx(p_variable,p_alarm,empty_arm,arm_type,pm_name,pm_slot,sch_name) < 0)
				{
					xEXT_CH__JOB_CTRL->Set__DATA("Abort");

					// ...
					int alarm_id = ALID__SWAP_ERROR;
					CString alarm_msg;
					CString r_act;

					alarm_msg.Format("Swap Error occured when Vac-Robot is swaping with %s.\n",pm_name);

					p_alarm->Check__ALARM(alarm_id,r_act);
					p_alarm->Post__ALARM_With_MESSAGE(alarm_id,alarm_msg);

					Sleep(990);
					xSCH_MATERIAL_CTRL->Clear__ALL_MATERIAL_INFO();

					NEXT__LOOP;
				}

				// ...
				{
					xSCH_MATERIAL_CTRL->Pick__From_MODULE(sch_name, empty_arm);
					
					xSCH_MATERIAL_CTRL->Set__MATERIAL_STATUS(empty_arm, STR__PROCESSED);
				}

				// ...
				{
					xSCH_MATERIAL_CTRL->Place__To_MODULE(arm_type, sch_name);

					xSCH_MATERIAL_CTRL->Set__MATERIAL_STATUS(sch_name, STR__PROCESSING);
					xSCH_MATERIAL_CTRL->Goto__NEXT_PROCESS(sch_name);
				}
			}
			else
			{
				// PICK ...
				{
					int act_flag  = -1;
					int move_flag = -1;

					act_flag = SCH__PICK_MODULE(p_variable,p_alarm, log_id, true,empty_arm,pm_name,pm_slot,sch_name);

					if(act_flag < 0)
					{
						move_flag = SCH__Check_Material_Pick(empty_arm);
					}
					else
					{
						move_flag = 1;
					}

					if(move_flag > 0)
					{
						xSCH_MATERIAL_CTRL->Pick__From_MODULE(sch_name, empty_arm);

						xSCH_MATERIAL_CTRL->Set__MATERIAL_STATUS(empty_arm, STR__PROCESSED);
					}

					if(act_flag < 0)
					{
						NEXT__LOOP;
					}
				}

				// PLACE ...
				{
					int act_flag  = -1;
					int move_flag = -1;

					xSCH_MATERIAL_CTRL->Set__CURRENT_RECIPE(arm_type,pm_rcp);

					act_flag = SCH__PLACE_MODULE(p_variable,p_alarm, log_id, false,arm_type,pm_name,pm_slot,sch_name);

					if(act_flag < 0)
					{
						move_flag = SCH__Check_Material_Place(arm_type);
					}
					else
					{
						move_flag = 1;
					}

					if(move_flag > 0)
					{
						xSCH_MATERIAL_CTRL->Place__To_MODULE(arm_type,sch_name);
					
						xSCH_MATERIAL_CTRL->Set__MATERIAL_STATUS(sch_name, STR__PROCESSING);
						xSCH_MATERIAL_CTRL->Goto__NEXT_PROCESS(sch_name);
					}

					if(act_flag < 0)
					{
						NEXT__LOOP;
					}
				}
			}
		}
		else
		{
			int act_flag  = -1;
			int move_flag = -1;

			CString empty_arm;

			if(VAC_RB__Get_Empty__Arm_Type_From_All(empty_arm) < 0)
			{
				NEXT__LOOP;
			}

			act_flag = SCH__PICK_MODULE(p_variable,p_alarm, log_id, false,empty_arm,pm_name,pm_slot,sch_name);

			// ...
			{
				CString log_msg;
				CString log_bff;

				log_msg  = "\n";
				log_msg += "# PM Order Check ... \n";

				int k_limmit = l_module.GetSize();
				int k;

				for(k=0; k<k_limmit; k++)
				{
					log_bff.Format(" ** [%s][%s] - [%s] \n", 
								   l_module[k],
								   l_pmx_incount[k],
								   l_recipe[k]);
					log_msg += log_bff;
				}

				xAPP_LOG_CTRL->WRITE__LOG(log_msg);
			}

			if(act_flag < 0)
			{
				move_flag = SCH__Check_Material_Pick(empty_arm);
			}
			else
			{
				move_flag = 1;
			}

			if(move_flag > 0)
			{
				xSCH_MATERIAL_CTRL->Pick__From_MODULE(sch_name,empty_arm);
			
				xSCH_MATERIAL_CTRL->Set__MATERIAL_STATUS(empty_arm, STR__PROCESSED);
			}

			if(act_flag < 0)
			{
				NEXT__LOOP;
			}
		}
	}

	return 1;
}

int  CObj__DUAL_ARM_STD::
AUTO_CTRL__RB_TO_LLx_PROC(CII_OBJECT__VARIABLE *p_variable,
				          CII_OBJECT__ALARM* p_alarm)
{
	if(dCH__VAC_RB__CFG_LLx_CTRL_ONLY_INPUT_OUTPUT_MODE->Check__DATA(STR__DUAL) > 0)
	{
		NEXT__LOOP;
	}
	else
	{
		CStringArray l_arm;	
		Get__Arm_List_To_Process(l_arm);

		int i_limit = l_arm.GetSize();

		for(int i=0; i<i_limit; i++)
		{
			CString arm_type = l_arm[i];

			_AUTO_CTRL__RB_TO_LLx_PROC(p_variable,p_alarm, arm_type);
		}
	}
}
int  CObj__DUAL_ARM_STD::
_AUTO_CTRL__RB_TO_LLx_PROC(CII_OBJECT__VARIABLE *p_variable,
				           CII_OBJECT__ALARM* p_alarm,
				           const CString& arm_type)
{
	DECLARE__EXT_CTRL(p_variable);

	// ...
	CString log_id = "_AUTO_CTRL__RB_TO_LLx_PROC()";

	// ...
	CStringArray l_ll;
	CStringArray l_ll_rcp;

	CString ll_name;
	CString ll_slot = "1";

	// ...
	{
		SCH__Change_Current_Route_Step(p_variable,p_alarm, arm_type);
	}

	if(xSCH_MATERIAL_CTRL->Get__CUR_PROCESS_INFO(arm_type,l_ll,l_ll_rcp) < 0)
	{
		return 1;
	}

	// ...
	int flag__lp_normal = 1;

	if(LPx__Get_Process_Type__Normal(arm_type,flag__lp_normal) < 0)
	{
		return -1;
	}

	// ...
	{
		int ll_check = -1;
		int ll_i;

		int limit = l_ll.GetSize();
		int i;

		for(i=0;i<limit;i++)
		{
			ll_i = Get__LLx_INDEX(l_ll[i]);
			if(ll_i < 0)
			{
				continue;
			}

			if(LLx__Check_CtcInUse_Or_Standby(ll_i) < 0)
			{
				continue;
			}

			/*
			if(LLx__Check_Empty__SlotStatus(ll_i) > 0)
			{
				if(xCH__LLx__NEXT_PROCESS[ll_i]->Check__DATA(STR__LEAK_CHECK) > 0)
				{
					continue;
				}
			}
			*/

			/*
			if(flag__lp_normal > 0)
			{
				if(xEXT_CH__LLx_PROCESS_TYPE[ll_i]->Check__DATA(STR__NORMAL) < 0)
				{
					continue;
				}
			}
			else
			{
				if(xEXT_CH__LLx_PROCESS_TYPE[ll_i]->Check__DATA(STR__TEST) < 0)
				{
					continue;
				}
			}
			*/

			if((xEXT_CH__SCH_DB_LLx_USE_FLAG[ll_i]->Check__DATA(STR__ENABLE) < 0)
			&& (LLx__Check_All_Empty(ll_i) > 0))
			{
				continue;
			}

			ll_check = 1;
			break;
		}

		if(ll_check < 0)
		{
			/*
			int alarm_id = ALID__LLx__NOT_AVAILABLE;
			CString alarm_msg;
			CString r_act;

			// ...
			{
				if(flag__lp_normal > 0)
				{
					alarm_msg = "There is no LLx with \"NORMAL\" process type. \n";
				}
				else
				{
					alarm_msg = "There is no LLx with \"TEST\" process type. \n";				
				}
				
				alarm_msg += "At least, one LL's status must be CtcInUse or Standby among the following LLs;\n";

				// ...
				int limit = l_ll.GetSize();
				int i;
			
				for(i=0; i<limit; i++)
				{
					alarm_msg += l_ll[i];
					alarm_msg += ", ";
				}

				alarm_msg += "\n";
			}

			// ..
			{
				p_alarm->Check__ALARM(alarm_id,r_act);
				p_alarm->Post__ALARM_With_MESSAGE(alarm_id,alarm_msg);
			}
			*/
			return 1;
		}
	}

	// ...
	CStringArray l_module;
	CStringArray l_recipe;
	CStringArray l_ll_incount;

	// ...
	{
		CString var_data;
		int ll_i;

		int limit = l_ll.GetSize();
		int i;

		for(i=0; i<limit; i++)
		{
			ll_i = Get__LLx_INDEX(l_ll[i]);
			if(ll_i < 0)
			{
				continue;
			}

			if(xEXT_CH__SCH_DB_LLx_USE_FLAG[ll_i]->Check__DATA(STR__ENABLE) > 0)
			{
				if(LLx__Is_Available(ll_i) < 0)
				{
					continue;
				}
			}
			else
			{
				if(LLx__Is_Idle(ll_i) < 0)
				{
					continue;
				}
				if(LLx__Check_All_Empty(ll_i) > 0)
				{
					continue;
				}
			}
			
			if(SCH__LLx_CHECK_PRESSURE(ll_i,p_variable,p_alarm) < 0)
			{
				continue;
			}

			/*
			if(flag__lp_normal > 0)
			{
				if(xEXT_CH__LLx_PROCESS_TYPE[ll_i]->Check__DATA(STR__NORMAL) < 0)
				{
					continue;
				}
			}
			else
			{
				if(xEXT_CH__LLx_PROCESS_TYPE[ll_i]->Check__DATA(STR__TEST) < 0)
				{
					continue;
				}
			}
			*/

			/*
			if(xCH_CFG__SCH_EXCHANGE_PRIORITY->Check__DATA(STR__WAFER) > 0)
			{
				if(LLx__Check_Occupied__SlotStatus(ll_i) > 0)
				{
					xCH__LLx__IN_COUNT[ll_i]->Get__DATA(var_data);
				}
				else
				{
					var_data = "1000000";
				}
			}
			else
			{
				if(LLx__Check_Occupied__SlotStatus(ll_i) > 0)
				{
					int cur_in_count;
					int cur_idle_sec;
					CString cur_data;

					xCH__LLx__IN_COUNT[ll_i]->Get__DATA(cur_data);
					cur_in_count = atoi(cur_data);

					cur_idle_sec = 0;
					var_data.Format("%1d", (1000000 + cur_in_count+cur_idle_sec));
				}
				else
				{
					if(xCH__LLx__FIRST_WAFER_FLAG[ll_i]->Check__DATA(STR__YES) > 0)
					{
						int cur_pmc_sec = - (10 - ll_i);
	
						var_data.Format("%1d", cur_pmc_sec);	
					}
					else
					{
						xCH__LLx__IDLE_TIME[ll_i]->Get__DATA(var_data);
						int cur_idle_sec = atoi(var_data);
						int cur_pmc_sec = 1000000 - cur_idle_sec;

						if(cur_pmc_sec < 1)
						{
							cur_pmc_sec = 1;
						}

						var_data.Format("%1d", cur_pmc_sec);	
					}
				}				
			}
			*/

			// ...
			CString ll_name = Get__LLx_NAME(ll_i);

			l_ll_incount.Add(var_data);
			l_module.Add(ll_name);
			l_recipe.Add(l_ll_rcp[i]);
		}
	}

	/*
	// Order Check ...
	{
		int k_limit = l_ll_incount.GetSize();
		int k;

		for(k=0; k<k_limit; k++)
		{
			int count_cur = atoi(l_ll_incount[k]);				

			CString incount_cur = l_ll_incount[k];
			CString moudle_cur  = l_module[k];
			CString recipe_cur  = l_recipe[k];

			for(int t = k+1; t<k_limit; t++)
			{
				int count_chk = atoi(l_ll_incount[t]);

				if(count_chk >= count_cur)
				{
					continue;
				}

				CString incount_chk = l_ll_incount[t];
				CString moudle_chk  = l_module[t];
				CString recipe_chk  = l_recipe[t];

				count_cur = count_chk;

				l_ll_incount[k] = incount_chk; 
				l_module[k] = moudle_chk; 
				l_recipe[k] = recipe_chk;

				l_ll_incount[t] = incount_cur; 
				l_module[t] = moudle_cur; 
				l_recipe[t] = recipe_cur;
			}
		}
	}
	*/

	// ...
	IDS__SCH_MATERIAL_STATUS ds_sts;

	CString sch_name;
	CString ll_rcp;
	int ll_i;

	int limit = l_module.GetSize();
	int i;

	for(i=0; i<limit; i++)
	{
		ll_name = l_module[i];

		ll_i = Get__LLx_INDEX(ll_name);
		if(ll_i < 0)
		{
			continue;
		}

		// ...
		{
			sch_name.Format("%s-%s", ll_name,ll_slot);	
			ll_rcp = l_recipe[i];
		}
	
		if(xEXT_CH__SCH_DB_LLx_USE_FLAG[ll_i]->Check__DATA(STR__ENABLE) > 0)
		{
			if(LLx__Is_Available(ll_i) < 0)	
			{
				continue;
			}
		}
		else
		{
			if(LLx__Is_Idle(ll_i) < 0)
			{
				continue;
			}
		}

		/*
		if(flag__lp_normal > 0)
		{
			if(xEXT_CH__LLx_PROCESS_TYPE[ll_i]->Check__DATA(STR__NORMAL) < 0)
			{
				continue;
			}
		}
		else
		{
			if(xEXT_CH__LLx_PROCESS_TYPE[ll_i]->Check__DATA(STR__TEST) < 0)
			{
				continue;
			}
		}
		*/

		if(LLx__Check_All_Empty(ll_i) > 0)
		{
			int act_flag  = -1;
			int move_flag = -1;

			// ...
			{
				xSCH_MATERIAL_CTRL->Set__CURRENT_RECIPE(arm_type,ll_rcp);

				act_flag = SCH__PLACE_MODULE(p_variable,p_alarm, log_id, false,arm_type,ll_name,ll_slot,sch_name);

				if(act_flag < 0)
				{
					move_flag = SCH__Check_Material_Place(arm_type);
				}
				else
				{
					move_flag = 1;
				}
			}

			if(move_flag > 0)
			{
				xSCH_MATERIAL_CTRL->Place__To_MODULE(arm_type, sch_name);
			
				SCH__LLx_PROC(p_variable,p_alarm, ll_name,sch_name, ll_rcp, _CMMD_LLx__PROC_MAIN);
			}

			/*
			// PROC_LOOPx ...
			if(move_flag > 0)
			{
				CString loop_id;
				int loop_count;

				xSCH_MATERIAL_CTRL->Get__PROC_LOOPx_OF_EDIT_TYPE(sch_name, loop_id,loop_count) ;

			}
			*/

			if(act_flag < 0)
			{
				NEXT__LOOP;
			}
			return 1;
		}
	}

	/*
	// ...
	{
		CString arm_type;
		
		// Arm : A ...
		if(VAC_RB__Check_Occupied__A_Arm() > 0)
		{
			arm_type = "A";
			
			if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(arm_type) > 0)
			{
				if(xCH_CFG__SCH_EXCHANGE_PRIORITY->Check__DATA(STR__PMC) > 0)
				{
					if(Fnc__LLx_Empty_Ready_Check(arm_type) > 0)
					{
						return 1;
					}
				}
			}
		}
		// Arm : B ...
		if(VAC_RB__Check_Occupied__B_Arm() > 0)
		{
			arm_type = "B";
			
			if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(arm_type) > 0)
			{
				if(xCH_CFG__SCH_EXCHANGE_PRIORITY->Check__DATA(STR__PMC) > 0)
				{
					if(Fnc__LLx_Empty_Ready_Check(arm_type) > 0)
					{
						return 1;
					}
				}
			}
		}
	}
	*/

	// ...
	limit = l_module.GetSize();

	for(i=0; i<limit; i++)
	{
		ll_name = l_module[i];

		ll_i = Get__LLx_INDEX(ll_name);
		if(ll_i < 0)
		{
			continue;
		}

		// ...
		{
			sch_name.Format("%s-%s", ll_name,ll_slot);
			ll_rcp = l_recipe[i];
		}
		
		if(xEXT_CH__SCH_DB_LLx_USE_FLAG[ll_i]->Check__DATA(STR__ENABLE) > 0)
		{
			if(LLx__Is_Available(ll_i) < 0)	
			{
				continue;
			}
		}
		else
		{
			if(LLx__Is_Idle(ll_i) < 0)
			{
				continue;
			}
		}
		
		/*
		if(flag__lp_normal > 0)
		{
			if(xEXT_CH__LLx_PROCESS_TYPE[ll_i]->Check__DATA(STR__NORMAL) < 0)
			{
				continue;
			}
		}
		else
		{
			if(xEXT_CH__LLx_PROCESS_TYPE[lli]->Check__DATA(STR__TEST) < 0)
			{
				continue;
			}
		}
		*/

		if(LLx__Check_All_Empty(ll_i) > 0)
		{
			return 1;
		}
		else
		{
			if(xSCH_MATERIAL_CTRL->Get__MATERIAL_STATUS(sch_name, ds_sts) < 0)
			{
				continue;
			}
		}
	
		if(VAC_RB__Check_Empty__Arm_Type() < 0)
		{
			continue;
		}

		// ...
		{
			int act_flag  = -1;
			int move_flag = -1;

			CString empty_arm;
			if(VAC_RB__Get_Empty__Arm_Type_From_All(empty_arm) < 0)
			{
				NEXT__LOOP;
			}

			act_flag = SCH__PICK_MODULE(p_variable,p_alarm, log_id, false,empty_arm,ll_name,ll_slot,sch_name);

			/*
			// ...
			{
				CString log_msg;
				CString log_bff;

				log_msg  = "\n";
				log_msg += "# LLx Order Check ... \n";

				int k_limmit = l_module.GetSize();
				int k;

				for(k=0; k<k_limmit; k++)
				{
					log_bff.Format(" ** [%s][%s] - [%s] \n", 
								   l_module[k],
								   l_ll_incount[k],
								   l_recipe[k]);
					log_msg += log_bff;
				}

				xAPP_LOG_CTRL->WRITE__LOG(log_msg);
			}
			*/

			if(act_flag < 0)
			{
				move_flag = SCH__Check_Material_Pick(empty_arm);
			}
			else
			{
				move_flag = 1;
			}

			if(move_flag > 0)
			{
				xSCH_MATERIAL_CTRL->Pick__From_MODULE(sch_name,empty_arm);
			
				xSCH_MATERIAL_CTRL->Set__MATERIAL_STATUS(empty_arm,"PROCESSED");
			}

			if(act_flag < 0)
			{
				NEXT__LOOP;
			}
		}
	}

	return 1;
}

int  CObj__DUAL_ARM_STD::
AUTO_CTRL__LLx_PROC_TO_RB(CII_OBJECT__VARIABLE *p_variable, CII_OBJECT__ALARM* p_alarm)
{
	if(dCH__VAC_RB__CFG_LLx_CTRL_ONLY_INPUT_OUTPUT_MODE->Check__DATA(STR__DUAL) > 0)
	{
		NEXT__LOOP;
	}
	else
	{
		if(VAC_RB__Is_Available() < 0)				NEXT__LOOP;
		if(VAC_RB__Check_Empty__Dual_Arm() < 0)		NEXT__LOOP;

		_AUTO_CTRL__LLx_PROC_TO_RB(p_variable,p_alarm);
	}
	return 1;
}
int  CObj__DUAL_ARM_STD::
_AUTO_CTRL__LLx_PROC_TO_RB(CII_OBJECT__VARIABLE *p_variable, CII_OBJECT__ALARM* p_alarm)
{
	DECLARE__EXT_CTRL(p_variable);

	// ...
	CString log_id = "_AUTO_CTRL__LLx_PROC_TO_RB()";

	int ll_limit = iLLx_LIMIT;
	int ll_i;

	for(ll_i=0; ll_i<ll_limit; ll_i++)
	{
		if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_i]->Check__DATA(LBx_MODE__ONLY_OUTPUT) < 0)
			continue;

		if(xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Check__DATA("IN") < 0)
			continue;

		if(LLx__Is_Available(ll_i) < 0)
			continue;

		if(LLx__Check_Occupied__OutSlot(ll_i) < 0)
			continue;

		// ...
		CString ll_name;
		CString ll_slot;
		
		ll_name = Get__LLx_NAME(ll_i);
		ll_slot.Format("%1d", LLx__Check_Occupied__OutSlot(ll_i));

		CString sch_name;
		sch_name.Format("%s-%s", ll_name,ll_slot);

		if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(sch_name) < 0)
			continue;

		// ...
		int act_flag  = -1;
		int move_flag = -1;

		CString empty_arm;
		if(VAC_RB__Get_Empty__Arm_Type_From_All(empty_arm) < 0)
		{
			NEXT__LOOP;
		}

		act_flag = SCH__PICK_MODULE(p_variable,p_alarm, log_id, false,empty_arm,ll_name,ll_slot,sch_name);

		if(act_flag < 0)
		{
			move_flag = SCH__Check_Material_Pick(empty_arm);
		}
		else
		{
			move_flag = 1;
		}

		if(move_flag > 0)
		{
			xSCH_MATERIAL_CTRL->Pick__From_MODULE(sch_name,empty_arm);

			xSCH_MATERIAL_CTRL->Set__MATERIAL_STATUS(empty_arm,"PROCESSED");
		}

		NEXT__LOOP;
	}

	return 1;
}

int  CObj__DUAL_ARM_STD::
SCH__Change_Current_Route_Step(CII_OBJECT__VARIABLE* p_variable,
							   CII_OBJECT__ALARM* p_alarm,
							   const CString& arm_type)
{
	IDS__SCH_MATERIAL_STATUS ds_sts;
	CString sch_name;	
	
	int arm_portid = -1;
	CStringArray l_arm_pmc;
	CStringArray l_arm_rcp;
	
	// ...
	if(xSCH_MATERIAL_CTRL->Get__MATERIAL_STATUS(arm_type,ds_sts) > 0)
	{
		arm_portid = ds_sts.iPORT_ID;	
		
		xSCH_MATERIAL_CTRL->Get__CUR_PROCESS_INFO(arm_type,l_arm_pmc,l_arm_rcp);
	}
	
	if(arm_portid > 0)
	{
		CUIntArray l_job_lp;
		SCH__Get_Job_LP_List(l_job_lp);
								
		if(l_job_lp.GetSize() > 1)
		{
			int cur_lp = l_job_lp[0];
			
			if(arm_portid != cur_lp)
			{
				if(xEXT_CH__SCH_CONCURRENT_ALWAYS_APPLY->Check__DATA(STR__PPID_CHECK) > 0)
				{
					CStringArray l_change_pmc_name;
					CStringArray l_change_pmc_rcp;
					CStringArray l_lp_pmc;
					
					if(xSCH_MATERIAL_CTRL->Get__ALL_PRC_MODULE_In_LPx(cur_lp,l_lp_pmc) > 0)
					{
						Macro__Change_Order__Trg_With_Src(l_lp_pmc,
														  l_arm_pmc,
														  l_arm_rcp,
														  l_change_pmc_name,
														  l_change_pmc_rcp);
					}
					
					/*
					// ...
					{
						CString log_msg;
						CString log_bff;
						
						int limit = l_change_pmc_name.GetSize();
						int i;
						
						for(i=0;i<limit;i++)
						{
							log_bff.Format("%1d) [%s][%s] \n", i+1,l_change_pmc_name[i],l_change_pmc_rcp[i]);
							log_msg += log_bff;
						}
						
						MessageBox(NULL,log_msg,"CHECK - SCH__Change_Current_Route_Step()",MB_OK|MB_TOPMOST);
					}
					*/
					
					if(l_change_pmc_name.GetSize() > 0)
					{
						CStringArray l_change_pmc_stage;
						
						l_change_pmc_stage.SetSize(l_change_pmc_name.GetSize());
						
						xSCH_MATERIAL_CTRL->SCH_DB_INFO__Change_Current_Route_Step(arm_type,
																				   l_change_pmc_name,
																				   l_change_pmc_rcp,
																				   l_change_pmc_stage);
					}
				}
			}
		}
	}

	return 1;
}

int  CObj__DUAL_ARM_STD::
Fnc__PMC_Empty_Ready_Check(const CString& sch_name,
						   const int ex_pick_check)
{
	// ...
	CStringArray l_prc_module;
	CStringArray l_prc_recipe;
				
	xSCH_MATERIAL_CTRL->Get__CUR_PROCESS_INFO(sch_name, l_prc_module,l_prc_recipe);
				
	// ...
	int flag__lp_normal = 1;
	
	if(LPx__Get_Process_Type__Normal(sch_name, flag__lp_normal) < 0)
	{
		return -1;
	}

	// ...
	{
		IDS__SCH_MATERIAL_STATUS ds_sts;

		xSCH_MATERIAL_CTRL->Get__MATERIAL_STATUS(sch_name, ds_sts);
		int sch_lp_id = ds_sts.iPORT_ID;

		// ...
		CUIntArray l_job_lp;
		SCH__Get_Job_LP_List(l_job_lp);
								
		if(l_job_lp.GetSize() > 1)
		{
			int cur_lp = l_job_lp[0];

			if(sch_lp_id != cur_lp)
			{
				CStringArray l_lp_pmc;

				if(xSCH_MATERIAL_CTRL->Get__ALL_PRC_MODULE_In_LPx(cur_lp,l_lp_pmc) > 0)
				{
					CStringArray l_sch_pmc;
					CStringArray l_sch_rcp;

					l_sch_pmc.Copy(l_prc_module);
					l_sch_rcp.Copy(l_prc_recipe);

					Macro__Delete_Trg_With_Src(l_lp_pmc, l_sch_pmc,l_sch_rcp);

					if(l_sch_pmc.GetSize() > 0)
					{
						l_prc_module.RemoveAll();
						l_prc_recipe.RemoveAll();

						l_prc_module.Copy(l_sch_pmc);
						l_prc_recipe.Copy(l_sch_rcp);
					}
				}
			}
		}
	}

	// ...
	int limit = l_prc_module.GetSize();
	int i;
				
	for(i=0;i<limit;i++)
	{
		int pm_i = SCH__CHECK_PM_INDEX(l_prc_module[i]);					
		if(pm_i < 0)
		{
			continue;
		}
					
		if(xEXT_CH__CFG__PMx_USE[pm_i]->Check__DATA(STR__ENABLE) < 0)
		{
			continue;
		}

		if(flag__lp_normal > 0)
		{
			if(xEXT_CH__PMx_PROCESS_TYPE[pm_i]->Check__DATA(STR__NORMAL) < 0)
			{
				continue;
			}
		}
		else
		{
			if(xEXT_CH__PMx_PROCESS_TYPE[pm_i]->Check__DATA(STR__TEST) < 0)
			{
				continue;
			}
		}

		if(PMx__Check_Empty__SlotStatus(pm_i) > 0)
		{
			if(PMx__Is_Available(pm_i) > 0)
			{
				return 1;
			}
			else
			{
				if(PMx__Check_CtcInUse_Or_Standby(pm_i) > 0)
				{
					if(ex_pick_check > 0)
					{
						if(xCH_CFG__SCH_LLx_EXCHANGE_WHEN_PMC_EMPTY->Check__DATA(STR__ENABLE) > 0)
						{
							return 1;
						}
					}

					// ...
					{
						CString var_data;
						double  cur_sec;

						CString cur_sts;
						xCH__PMx__PROC_INFO__PROCESS_STATUS[pm_i]->Get__DATA(cur_sts);
					
						if(cur_sts.CompareNoCase("PROCESS.START") == 0)
						{
							if(xCH__PMx__PROCESS_STATUS[pm_i]->Check__DATA("WAIT") > 0)
							{
								return 1;
							}

							xCH__PMx__CURRENT_PRC_TIME[pm_i]->Get__DATA(var_data);
							cur_sec = atof(var_data);

							if(cur_sec < 1)
							{
								return 1;
							}
						}
						if((cur_sts.CompareNoCase("PROCESS.END")  == 0)
						|| (cur_sts.CompareNoCase("PROCESS.COMP") == 0))
						{
							xCH__PMx__IDLE_TIME[pm_i]->Get__DATA(var_data);
							cur_sec = atof(var_data);

							if(cur_sec < 1)
							{
								return 1;
							}
						}
					}
				}
			}
		}
	}

	return -1;
}
int  CObj__DUAL_ARM_STD::
Fnc__PMx_Empty_Ready_Check(const int flag__lp_normal,
						   const int pm_i,
						   const int only_available_flag,
						   const int ex_pick_check)
{
	// ...
	if(xEXT_CH__CFG__PMx_USE[pm_i]->Check__DATA(STR__ENABLE) < 0)
	{
		return -1;
	}

	// ...
	if(flag__lp_normal > 0)
	{
		if(xEXT_CH__PMx_PROCESS_TYPE[pm_i]->Check__DATA(STR__NORMAL) < 0)
		{
			return -1;
		}
	}
	else
	{
		if(xEXT_CH__PMx_PROCESS_TYPE[pm_i]->Check__DATA(STR__TEST) < 0)
		{
			return -1;
		}
	}
					
	// ...
	if(PMx__Check_Empty__SlotStatus(pm_i) > 0)
	{
		if(PMx__Is_Available(pm_i) > 0)
		{
			return 1;
		}
		else
		{
			if(ex_pick_check > 0)
			{
				if(PMx__Check_CtcInUse_Or_Standby(pm_i) > 0)
				{
					if(xCH_CFG__SCH_LLx_EXCHANGE_WHEN_PMC_EMPTY->Check__DATA(STR__ENABLE) > 0)
					{
						return 1;
					}
				}
			}

			if(only_available_flag < 0)
			{
				if(PMx__Check_CtcInUse_Or_Standby(pm_i) > 0)
				{
					CString var_data;
					double  cur_sec;

					CString cur_sts;
					xCH__PMx__PROC_INFO__PROCESS_STATUS[pm_i]->Get__DATA(cur_sts);
					
					if(cur_sts.CompareNoCase("PROCESS.START") == 0)
					{
						// ...
						if(xCH__PMx__PROCESS_STATUS[pm_i]->Check__DATA("WAIT") > 0)
						{
							return 1;
						}

						// ...
						xCH__PMx__CURRENT_PRC_TIME[pm_i]->Get__DATA(var_data);
						cur_sec = atof(var_data);

						if(cur_sec < 1)
						{
							return 1;
						}
					}
					if((cur_sts.CompareNoCase("PROCESS.END")  == 0)
					|| (cur_sts.CompareNoCase("PROCESS.COMP") == 0))
					{
						/*
						// ...
						xCH__PMx__IDLE_TIME[pm_i]->Get__DATA(var_data);
						cur_sec = atof(var_data);

						if(cur_sec < 1)
						{
							return 1;
						}
						*/
						return 1;
					}
				}
			}
			else if(only_available_flag > 0)
			{
				if(PMx__Check_CtcInUse_Or_Standby(pm_i) > 0)
				{
					CString var_data;
					double  cur_sec;
					
					CString cur_sts;
					xCH__PMx__PROC_INFO__PROCESS_STATUS[pm_i]->Get__DATA(cur_sts);
					
					if(cur_sts.CompareNoCase("PROCESS.START") == 0)
					{
						xCH__PMx__CURRENT_PRC_TIME[pm_i]->Get__DATA(var_data);
						cur_sec = atof(var_data);
						
						xCH_CFG__PMC_CLN_PROC_TIME_FOR_LBx_PICK->Get__DATA(var_data);
						double cfg_cln_sec = atof(var_data);

						if((cur_sec > 1) && (cur_sec < cfg_cln_sec))
						{
							return 1;
						}
					}
					if((cur_sts.CompareNoCase("PROCESS.END")  == 0)
					|| (cur_sts.CompareNoCase("PROCESS.COMP") == 0))
					{
						/*
						// ...
						xCH__PMx__IDLE_TIME[pm_i]->Get__DATA(var_data);
						cur_sec = atof(var_data);
						
						if(cur_sec < 1)
						{
							return 1;
						}
						*/
						return 1;
					}
				}
			}
			else if(only_available_flag == 0)
			{
				if(PMx__Check_CtcInUse_Or_Standby(pm_i) > 0)
				{
					return 1;
				}
			}
		}
	}

	return -1;
}

int  CObj__DUAL_ARM_STD::
Get__PM_ID_OF_SCH_USER_DEF_TRG_MDx(const CString& log_id, const CString& sch_name)
{
	int pmc__step_count = xSCH_MATERIAL_CTRL->SCH_DB_INFO__Get_Current_Step_Count(sch_name);
	if(pmc__step_count > 0)			return -1;

	// ...
	int trg__pmc_id = -1;

	CString def_name;
	CString def_data;

	def_name = USER_DEF__TRG_MDx;
	xSCH_MATERIAL_CTRL->Get__MATERIAL_USER_INFO(sch_name, def_name,def_data);
	CString str__trg_name = def_data;

	if(str__trg_name.GetLength() > 0)
	{
		bool active__wfr_check = false;

		if((dCH__VAC_RB__CFG_PICK_WAFER_CONDITION->Check__DATA(STR__ONLY_PROCESSED) > 0)
		|| (xCH_CFG__SCH_EXCHANGE_PRIORITY->Check__DATA(STR__PMC) > 0))
		{
			active__wfr_check = true;
		}

		int r_flag = Fnc__PMC_first_Path_Check(str__trg_name, active__wfr_check);
		if(r_flag > 0)
		{
			trg__pmc_id = Macro__Get_PMC_ID(str__trg_name);
		}

		// ...
		{
			def_name = USER_DEF__GOTO_PM;
			def_data = str__trg_name;

			xSCH_MATERIAL_CTRL->Set__MATERIAL_USER_INFO(sch_name, def_name,def_data);
		}
	}

	/*
	// ...
	{
		CString log_msg;
		CString log_bff;
		
		log_msg = "Get__PM_ID_OF_SCH_USER_DEF_TRG_MDx() ... \n";

		log_bff.Format(" * Log_ID <- %s. \n", log_id);
		log_msg += log_bff;

		log_bff.Format("  * sch_name <- %s \n", sch_name);
		log_msg += log_bff;

		log_bff.Format("  * %s <- %s \n", USER_DEF__TRG_MDx,str__trg_name);
		log_msg += log_bff;

		log_bff.Format("  * trg__pmc_id <- [%1d] \n", trg__pmc_id);
		log_msg += log_bff;

		printf(log_msg);
	}
	*/

	return trg__pmc_id;
}
int  CObj__DUAL_ARM_STD::
Get__PM_INDEX_OF_SCH_USER_DEF_GOTO_PMX(const CString& log_id, const CString& sch_name, const CStringArray& l_pm_name)
{
	int pmc__step_count = xSCH_MATERIAL_CTRL->SCH_DB_INFO__Get_Current_Step_Count(sch_name);
	if(pmc__step_count > 0)			return -11;

	// ...
	int trg__goto_pm_index = -1;

	CString str__trg_name;
	CString str__trg_slot;
	CString str__al_angle;
	CString str__goto_pm;

	// ...
	CString def_name;
	CString def_data;

	def_name = USER_DEF__GOTO_PM;

	int r_flag = xSCH_MATERIAL_CTRL->Get__MATERIAL_USER_INFO(sch_name, def_name,def_data);
	if(r_flag > 0)
	{
		str__goto_pm = def_data;

		// ...
		bool active__wfr_check = false;

		if((dCH__VAC_RB__CFG_PICK_WAFER_CONDITION->Check__DATA(STR__ONLY_PROCESSED) > 0)
		|| (xCH_CFG__SCH_EXCHANGE_PRIORITY->Check__DATA(STR__PMC) > 0))
		{
			active__wfr_check = true;
		}

		if(Fnc__PMC_first_Path_Check(str__goto_pm, active__wfr_check) > 0)
		{
			trg__goto_pm_index = Fnc__Get_Find_Index(str__goto_pm, l_pm_name);
		}
	}

	// 1.
	{
		def_name = USER_DEF__TRG_MDx;
		xSCH_MATERIAL_CTRL->Get__MATERIAL_USER_INFO(sch_name, def_name,def_data);

		str__trg_name = def_data;
	}
	// 2.
	{
		def_name = USER_DEF__TRG_SLOT;
		xSCH_MATERIAL_CTRL->Get__MATERIAL_USER_INFO(sch_name, def_name,def_data);

		str__trg_slot = def_data;
	}
	// 3.
	{
		def_name = USER_DEF__TRG_ANGLE;
		xSCH_MATERIAL_CTRL->Get__MATERIAL_USER_INFO(sch_name, def_name,def_data);

		str__al_angle = def_data;
	}

	if(sch_name.CompareNoCase(_ARM__A) == 0)
	{
		sCH__REPORT_TRG_NAME_OF_ARM_A->Set__DATA(str__trg_name);
		sCH__REPORT_TRG_SLOT_OF_ARM_A->Set__DATA(str__trg_slot);
		sCH__REPORT_TRG_ANGLE_OF_ARM_A->Set__DATA(str__al_angle);

		sCH__REPORT_GOTO_PM_OF_ARM_A->Set__DATA(str__goto_pm);
	}
	else if(sch_name.CompareNoCase(_ARM__B) == 0)
	{
		sCH__REPORT_TRG_NAME_OF_ARM_B->Set__DATA(str__trg_name);
		sCH__REPORT_TRG_SLOT_OF_ARM_B->Set__DATA(str__trg_slot);
		sCH__REPORT_TRG_ANGLE_OF_ARM_B->Set__DATA(str__al_angle);

		sCH__REPORT_GOTO_PM_OF_ARM_B->Set__DATA(str__goto_pm);
	}
	
	return trg__goto_pm_index;
}

int  CObj__DUAL_ARM_STD::
Fnc__PMC_first_Path_Check(const CString& pmc_name, const bool active__wfr_check)
{			
	int pm_index = Macro__Get_PMC_INDEX(pmc_name);
	if(pm_index < 0)		return -1;

	/*
	if(dCH__VAC_RB__INFO_PM_RESERVE_X[pm_index]->Check__DATA(STR__ON) > 0)
	{
		return -11;
	}
	*/

	if(active__wfr_check)
	{
		if(PMx__Check_Empty__SlotStatus(pm_index) < 0)
		{
			return -12;
		}
	}

	if(PMx__Is_Ready(pm_index) < 0)
	{
		return -13;
	}

	return 1;
}

int  CObj__DUAL_ARM_STD::
_SCH_CHECK__PMo_RB_WITH_DUAL_TYPE(const bool active__dual_type, const bool active__pm_constaint)
{
	CString empty_arm;

	if(VAC_RB__Get_Empty__Arm_Type_From_All(empty_arm) < 0)
	{
		return -11;
	}

	if(xTIMER__WAFER_ON_ARM->Get__CURRENT_TIME() > 0.1)
	{
		CString sch_name;
		CString pm_name;
		CString pm_slot = "1";

		for(int i=0; i<iPMx_SIZE; i++)
		{
			pm_name.Format("PM%1d",i+1);
			sch_name.Format("%s-%s", pm_name,pm_slot);

			if(xSCH_MATERIAL_CTRL->Check__MATERIAL_DB(sch_name) < 0)		continue;

			if(PMx__Check_Empty__SlotStatus(i) > 0)							continue;
			if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(sch_name) > 0)		continue;

			if(active__pm_constaint)
			{
				if(!VAC_RB__Check_Empty__Arm_Type_With_PMx_Constraint(empty_arm, pm_name))
				{
					continue;
				}
			}
		
			if((dCH__VAC_RB__CFG_PMx_To_PMx_CONSTRAINT->Check__DATA(STR__SAME_PATH) > 0)
			|| (dCH__VAC_RB__CFG_PMx_To_PMx_CONSTRAINT->Check__DATA(STR__SAME_PROCESS) > 0))
			{
				int pmc__step_count = xSCH_MATERIAL_CTRL->SCH_DB_INFO__Get_Current_Step_Count(sch_name);

				if(VAC_RB__Check_Occupied__A_Arm() > 0)
				{
					CString check_arm = _ARM__A;

					int arm__step_count = xSCH_MATERIAL_CTRL->SCH_DB_INFO__Get_Current_Step_Count(check_arm);
					if(arm__step_count != pmc__step_count)			continue;
				}
				if(VAC_RB__Check_Occupied__B_Arm() > 0)
				{
					CString check_arm = _ARM__B;

					int arm__step_count = xSCH_MATERIAL_CTRL->SCH_DB_INFO__Get_Current_Step_Count(check_arm);
					if(arm__step_count != pmc__step_count)			continue;
				}
			}

			return 1;
		}
	}
	
	return -1;
}

int  CObj__DUAL_ARM_STD::
AUTO_CTRL__PMo_RB(CII_OBJECT__VARIABLE *p_variable,
				  CII_OBJECT__ALARM *p_alarm)
{
	if(dCH__VAC_RB__CFG_LLx_CTRL_ONLY_INPUT_OUTPUT_MODE->Check__DATA(STR__DUAL) > 0)
	{
		_AUTO_CTRL__PMo_RB_WITH_DUAL_TYPE(p_variable, p_alarm);
	}
	else
	{
		bool active__only_mode = false;		

		if(SCH__LBx__CHECK_ONLY_MODE() > 0)			active__only_mode = true;

		_AUTO_CTRL__PMo_RB__CTRL_MODE(p_variable, p_alarm, active__only_mode);
	}

	return 1;
}
int  CObj__DUAL_ARM_STD::
_AUTO_CTRL__PMo_RB_WITH_DUAL_TYPE(CII_OBJECT__VARIABLE *p_variable,
								  CII_OBJECT__ALARM *p_alarm)
{
	bool active__dual_arm_moving = false;

	if(dCH__VAC_RB__CFG_DUAL_ARM_MOVING_AT_THE_SAME_TIME->Check__DATA(STR__ENABLE) > 0)
	{
		active__dual_arm_moving = true;

		if(VAC_RB__Check_Empty__Dual_Arm() < 0)	
		{
			return -2;
		}
	}

	// ...
	if(dCH__VAC_RB__CFG_PICK_WAFER_CONDITION->Check__DATA(STR__ONLY_PROCESSED) > 0)
	{
		if(VAC_RB__Check_Occupied__A_Arm() > 0)
		{
			CString check_arm = _ARM__A;

			if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(check_arm) > 0)				NEXT__LOOP;
			if(xCH__VAC_RB__SLOT01_STATUS->Check__DATA(STR__MAPPED) > 0)			NEXT__LOOP;
		}
		if(VAC_RB__Check_Occupied__B_Arm() > 0)
		{
			CString check_arm = _ARM__B;

			if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(check_arm) > 0)				NEXT__LOOP;
			if(xCH__VAC_RB__SLOT02_STATUS->Check__DATA(STR__MAPPED) > 0)			NEXT__LOOP;
		}
	}

	// ...
	CString log_id = "_AUTO_CTRL__PMo_RB_WITH_DUAL_TYPE()";

	if(active__dual_arm_moving)
	{
		IDS__SCH_MATERIAL_STATUS ds_sts;

		int pm_min_index = -1;
		int pm_min_count = -1;
		int pm_cur_count = -1;

		for(int i=0; i<iPMx_SIZE; i++)
		{
			CString pm_name;
			CString sch_name;

			pm_name.Format("PM%1d",  i+1);
			sch_name.Format("%s-1", pm_name);

			if(xSCH_MATERIAL_CTRL->Check__MATERIAL_DB(sch_name) < 0)
			{
				continue;
			}

			if(PMx__Check_Empty__SlotStatus(i) > 0)
			{
				continue;
			}

			if(xEXT_CH__CFG__PMx_USE[i]->Check__DATA(STR__ENABLE) > 0)
			{
				if(PMx__Is_Available(i) < 0)		continue;
			}
			else
			{
				if(PMx__Is_Idle(i) < 0)				continue;
			}

			if(xSCH_MATERIAL_CTRL->Get__MATERIAL_STATUS(sch_name,ds_sts) < 0)
			{
				continue;
			}

			if(SCH__PMx_CHECK_PRESSURE(i,p_variable,p_alarm) < 0)
			{
				continue;
			}

			CString var_data = xCH__PMx__IN_COUNT[i]->Get__STRING();
			pm_cur_count = atoi(var_data);
			
			if(pm_cur_count < 1)
			{
				continue;
			}

			if((pm_min_count < 1)		
			|| (pm_cur_count < pm_min_count))
			{
				pm_min_count = pm_cur_count;
				pm_min_index = i;
			}
		}

		if((pm_min_index % 2) != 0)
		{
			NEXT__LOOP;
		}

		if(pm_min_index >= 0)
		{
			CString sch_left;
			CString sch_right;

			CString pm_left;
			CString pm_right;

			int id_left  = pm_min_index + 1;
			int id_right = pm_min_index + 2;

			pm_left.Format("PM%1d",  id_left);
			pm_right.Format("PM%1d", id_right);

			sch_left.Format("%s-1",  pm_left);
			sch_right.Format("%s-1", pm_right);

			if(PMx__Check_Empty__SlotStatus(id_left  - 1) > 0)					NEXT__LOOP;
			if(PMx__Check_Empty__SlotStatus(id_right - 1) > 0)					NEXT__LOOP;

			if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(sch_left)  > 0)			NEXT__LOOP;
			if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(sch_right) > 0)			NEXT__LOOP;

			// ...
			{
				CString log_xxx;
				log_xxx.Format("%s - check_pmc_pick[%1d & %1d] \n", log_id, id_left, id_right);

				// ...
				int act_flag  = -1;
				int move_flag = -1;

				CString empty_arm = _ARM__AB;
				CString para_pmc = pm_left;			
				
				CStringArray l_from_sch;
				l_from_sch.Add(sch_left);
				l_from_sch.Add(sch_right);

				act_flag = SCH__PICK_MODULE(p_variable,p_alarm, log_xxx, false, empty_arm, para_pmc,"1", l_from_sch);

				if(act_flag < 0)
				{
					move_flag = SCH__Check_Material_Pick(empty_arm);
				}
				else
				{
					move_flag = 1;
				}

				// SCH DB : Update ...
				{
					if(VAC_RB__Check_Occupied__A_Arm() > 0)
					{
						xSCH_MATERIAL_CTRL->Pick__From_MODULE(sch_left,  _ARM__A);
						xSCH_MATERIAL_CTRL->Set__MATERIAL_STATUS(_ARM__A, "PROCESSED");
					}
					if(VAC_RB__Check_Occupied__B_Arm() > 0)
					{
						xSCH_MATERIAL_CTRL->Pick__From_MODULE(sch_right, _ARM__B);
						xSCH_MATERIAL_CTRL->Set__MATERIAL_STATUS(_ARM__B, "PROCESSED");
					}
				}

				// ...
				{
					CString log_msg;
					CString log_bff;

					log_msg  = "\n";
					log_msg += "Result - Report ... \n";

					log_bff.Format(" * act_flag  <- [%1d] \n", act_flag);
					log_msg += log_bff;
					
					log_bff.Format(" * move_flag <- [%1d] \n", move_flag);
					log_msg += log_bff;

					xAPP_LOG_CTRL->WRITE__LOG(log_msg);
				}

				if(act_flag < 0)
				{
					NEXT__LOOP;
				}
			}
			return 1;
		}
	}
	else
	{
		CUIntArray l__pm_index;

		CString sch_name;
		CString pm_name;

		int i_limit;
		int i;

		for(i=0; i<iPMx_SIZE; i++)
		{
			int pm_id = i + 1;

			pm_name.Format("PM%1d",  pm_id);
			sch_name.Format("%s-1",  pm_name);

			if(xSCH_MATERIAL_CTRL->Check__MATERIAL_DB(sch_name) < 0)
			{
				continue;
			}

			if(PMx__Check_Empty__SlotStatus(i) > 0)
			{
				continue;
			}

			if(xEXT_CH__CFG__PMx_USE[i]->Check__DATA(STR__ENABLE) > 0)
			{
				if(PMx__Is_Available(i) < 0)		continue;
			}
			else
			{
				if(PMx__Is_Idle(i) < 0)				continue;
			}

			if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(sch_name) > 0)
			{
				continue;
			}

			if(SCH__PMx_CHECK_PRESSURE(i,p_variable,p_alarm) < 0)
			{
				continue;
			}

			if((dCH__VAC_RB__CFG_PMx_To_PMx_CONSTRAINT->Check__DATA(STR__SAME_PATH) > 0)
			|| (dCH__VAC_RB__CFG_PMx_To_PMx_CONSTRAINT->Check__DATA(STR__SAME_PROCESS) > 0))
			{
				int pmc__step_count = xSCH_MATERIAL_CTRL->SCH_DB_INFO__Get_Current_Step_Count(sch_name);

				if(VAC_RB__Check_Occupied__A_Arm() > 0)
				{
					CString check_arm = _ARM__A;

					int arm__step_count = xSCH_MATERIAL_CTRL->SCH_DB_INFO__Get_Current_Step_Count(check_arm);
					if(arm__step_count != pmc__step_count)			continue;
				}
				if(VAC_RB__Check_Occupied__B_Arm() > 0)
				{
					CString check_arm = _ARM__B;

					int arm__step_count = xSCH_MATERIAL_CTRL->SCH_DB_INFO__Get_Current_Step_Count(check_arm);
					if(arm__step_count != pmc__step_count)			continue;
				}
			}

			int pm_index = pm_id - 1;
			l__pm_index.Add(pm_index);
		}

		_Get__PM_SORT_WITH_IDLE_TIME(l__pm_index, l__pm_index);

		// ...
		i_limit = l__pm_index.GetSize();

		for(i=0; i<i_limit; i++)
		{
			int pm_index = l__pm_index[i]; 
			int pm_id = pm_index + 1;

			pm_name.Format("PM%1d",  pm_id);
			sch_name.Format("%s-1",  pm_name);

			if(PMx__Check_Empty__SlotStatus(pm_index) > 0)						continue;
			if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(sch_name)  > 0)			continue;

			// ...
			{
				CUIntArray l__ll_index;

				if(LLx__Get_Empty__OutSlot(l__ll_index) < 0)
				{
					NEXT__LOOP;
				}

				// ...
				bool active__ll_error = true;

				int k_limit = l__ll_index.GetSize();
				for(int k=0; k<k_limit; k++)
				{
					int ll_index = l__ll_index[k];

					if(LLx__Is_Available(ll_index) < 0)			continue;

					if(xEXT_CH__CFG__TRANSFER_MODE->Check__DATA("VAC") > 0)
					{
						if(LLx__Is_VAC(ll_index) < 0)			continue;
					}
					else
					{
						if(LLx__Is_ATM(ll_index) < 0)			continue;
					}

					active__ll_error = false;
					break;
				}

				if(active__ll_error)
				{
					NEXT__LOOP;
				}
			}

			// ...
			CString log_xxx;
			log_xxx.Format("%s - check_pmc_pick[%1d] \n", log_id, pm_id);

			// ...
			int act_flag  = -1;
			int move_flag = -1;

			CString empty_arm;

			if(VAC_RB__Get_Empty__Arm_Type_From_All(empty_arm) < 0)
			{
				continue;
			}
			/*
			if(VAC_RB__Get_Empty__Arm_Type_With_PMx_Constraint(empty_arm, pm_name) < 0)
			{
				continue;
			}
			*/

			act_flag = SCH__PICK_MODULE(p_variable,p_alarm, log_xxx, false, empty_arm, pm_name,"1", sch_name);

			if(act_flag < 0)
			{
				move_flag = SCH__Check_Material_Pick(empty_arm);
			}
			else
			{
				move_flag = 1;
			}

			// SCH DB : Update ...
			if(VAC_RB__Check_Occupied__Arm_Type(empty_arm) > 0)
			{
				xSCH_MATERIAL_CTRL->Pick__From_MODULE(sch_name, empty_arm);
				xSCH_MATERIAL_CTRL->Set__MATERIAL_STATUS(empty_arm, "PROCESSED");

				double cfg__limit_sec = aCH__CFG_WAFER_WAITING_LIMIT_TIME_ON_ARM->Get__VALUE();
				xTIMER__WAFER_ON_ARM->REGISTER__COUNT_CHANNEL_NAME(sCH__CUR_WAFER_WAITING_LIMIT_TIME_ON_ARM->Get__CHANNEL_NAME());
				xTIMER__WAFER_ON_ARM->START__COUNT_DOWN(cfg__limit_sec);
			}

			// ...
			{
				CString log_msg;
				CString log_bff;

				log_msg  = "\n";
				log_msg += "Result - Report ... \n";

				log_bff.Format(" * act_flag  <- [%1d] \n", act_flag);
				log_msg += log_bff;

				log_bff.Format(" * move_flag <- [%1d] \n", move_flag);
				log_msg += log_bff;

				xAPP_LOG_CTRL->WRITE__LOG(log_msg);
			}

			if(act_flag < 0)
			{
				NEXT__LOOP;
			}
		}
		return 1;
	}

	NEXT__LOOP;
}

int  CObj__DUAL_ARM_STD::
_AUTO_CTRL__PMo_RB__CTRL_MODE(CII_OBJECT__VARIABLE *p_variable, 
							 CII_OBJECT__ALARM *p_alarm,
							 const bool active__only_mode)
{
	CString log_id = "AUTO_CTRL__PMo_RB__CTRL_MODE()";

	// ...
	{
		CString empty_arm;

		if(VAC_RB__Get_Empty__Arm_Type_From_PMx(empty_arm) < 0)
		{
			NEXT__LOOP;
		}
	}

	// Check : Wafer State ...
	if(dCH__VAC_RB__CFG_PICK_WAFER_CONDITION->Check__DATA(STR__ONLY_PROCESSED) > 0)
	{
		if(VAC_RB__Check_Occupied__A_Arm() > 0)
		{
			if(xCH__VAC_RB__SLOT01_STATUS->Check__DATA(STR__MAPPED) > 0)			NEXT__LOOP;
		}
		if(VAC_RB__Check_Occupied__B_Arm() > 0)
		{
			if(xCH__VAC_RB__SLOT02_STATUS->Check__DATA(STR__MAPPED) > 0)			NEXT__LOOP;
		}
	}

	if(VAC_RB__Check_Empty__Dual_Arm() > 0)
	{
		// CHECK : LLx -> PMx 
		if(_CHECK__LBi_To_RB_With_Ready_PMC__ALL_MODE() > 0)
		{
			NEXT__LOOP;
		}
	}
	else
	{
		// CHECK : LLo
		if(active__only_mode)
		{
			if(_SCH_CHECK__PMo_RB__ONLY_MODE() < 0)			NEXT__LOOP;
		}
		else
		{
			if(_SCH_CHECK__PMo_RB__ALL_MODE() < 0)			NEXT__LOOP;
		}
	}

	// CHECK : Wafer On Arm ...
	{
		CStringArray l__arm_type;
		VAC_RB__Get_Occupied__Arm_Type(l__arm_type);

		int k_limit = l__arm_type.GetSize();
		int k;

		for(k=0; k<k_limit; k++)
		{
			CString arm_type = l__arm_type[k];

			if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(arm_type) < 0)
			{
				NEXT__LOOP;
			}

			if(_CHECK__Ready_PMC_OF_SCH_NAME(arm_type) > 0)
			{
				NEXT__LOOP;
			}
		}
	}

	// ...
	CUIntArray l__pmc_index;
	CUIntArray l__pmc_count;

	for(int i=0; i<iPMx_SIZE; i++)
	{
		int pmc_id = i + 1;

		// ...
		CString sch_name;
		CString pm_name;
		CString pm_slot = "1";

		pm_name.Format("PM%1d",  pmc_id);
		sch_name.Format("%s-%s", pm_name,pm_slot);

		if(xSCH_MATERIAL_CTRL->Check__MATERIAL_DB(sch_name) < 0)
		{
			continue;
		}

		if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(sch_name) > 0)
		{
			if(Fnc__PMC_Empty_Ready_Check(sch_name) > 0)
			{
				NEXT__LOOP;
			}
			continue;
		}

		if(PMx__Check_Empty__SlotStatus(i) > 0)
		{
			continue;
		}

		if(xEXT_CH__CFG__PMx_USE[i]->Check__DATA(STR__ENABLE) > 0)
		{
			if(PMx__Is_Available(i) < 0)
			{
				continue;
			}
		}
		else
		{
			if(PMx__Is_Idle(i) < 0)
			{
				continue;
			}
		}

		if(SCH__PMx_CHECK_PRESSURE(i,p_variable,p_alarm) < 0)
		{
			continue;
		}

		// ...
		{
			CString ch_data = xCH__PMx__IN_COUNT[i]->Get__STRING();
			int in_count = atoi(ch_data);

			if(in_count < 1)		continue;

			l__pmc_index.Add(i);
			l__pmc_count.Add(in_count);
		}
	}

	// Check : Order ...
	{
		int k_limit = l__pmc_index.GetSize();

		for(int k=0; k<k_limit; k++)
		{
			int min_count = l__pmc_count[k];
			int min_index = l__pmc_index[k];

			for(int t = k+1; t < k_limit; t++)
			{
				int cur_count = l__pmc_count[t];
				int cur_index = l__pmc_index[t];
				
				if(cur_count >= min_count)			continue;
				
				l__pmc_index[t] = min_index;
				l__pmc_count[t] = min_count;
				
				l__pmc_index[k] = cur_index;
				l__pmc_count[k] = cur_count;

				min_index = cur_index;
				min_count = cur_count;
			}
		}
	}

	// Check : PMo -> RB ...
	{
		int k_limit = l__pmc_index.GetSize();

		for(int k=0; k<k_limit; k++)
		{
			int pm_index = l__pmc_index[k];
			int pmc_id = pm_index + 1;

			if(PMx__Check_Empty__SlotStatus(pm_index) > 0)
			{
				continue;
			}

			// ...
			CString empty_arm;

			if(VAC_RB__Get_Empty__Arm_Type_From_PMx(empty_arm) < 0)
			{
				NEXT__LOOP;
			}

			// ...
			CString sch_name;
			CString pm_name;
			CString pm_slot = "1";

			pm_name.Format("PM%1d",  pmc_id);
			sch_name.Format("%s-%s", pm_name,pm_slot);

			// ...
			{
				int act_flag  = -1;
				int move_flag = -1;

				act_flag = SCH__PICK_MODULE(p_variable,p_alarm, log_id, false, empty_arm, pm_name,pm_slot,sch_name);
	
				if(act_flag < 0)
				{
					move_flag = SCH__Check_Material_Pick(empty_arm);
				}
				else
				{
					move_flag = 1;
				}

				if(move_flag > 0)
				{
					xSCH_MATERIAL_CTRL->Pick__From_MODULE(sch_name, empty_arm);
			
					xSCH_MATERIAL_CTRL->Set__MATERIAL_STATUS(empty_arm, STR__PROCESSED);
				}

				if(act_flag < 0)
				{
					NEXT__LOOP;
				}
			}

			// Post Clean .....
			{
				CString cln_module;
				CString cln_recipe;

				int flag 
					= xSCH_MATERIAL_CTRL->Get__POST_CLN_In_PMC(pm_name,
															   pmc_id,
															   cln_module,
															   cln_recipe);
			
				if(flag > 0)
				{
	
				}
			}

			if(SCH__LBx__CHECK_EMPTY_OF_ONLY_OUTPUT() < 0)
			{
				NEXT__LOOP;
			}
		}
	}

	return 1;
}
int  CObj__DUAL_ARM_STD::
_SCH_CHECK__PMo_RB__ONLY_MODE()
{
	int ll_i;

	for(ll_i=0; ll_i<iLLx_LIMIT; ll_i++)
	{
		if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_i]->Check__DATA(LBx_MODE__ONLY_OUTPUT) < 0)
		{	
			continue;
		}
		if(xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Check__DATA(STR__IN) < 0)
		{
			continue;
		}

		if(LLx__Is_Available(ll_i) < 0)			continue;
		if(LLx__Is_VAC(ll_i) < 0)				continue;

		if(LLx__Check_Empty__OutSlot(ll_i) < 0)
		{
			continue;
		}

		return 1;
	}

	return -1;
}
int  CObj__DUAL_ARM_STD::
_SCH_CHECK__PMo_RB__ALL_MODE()
{
	int ll_i;
	
	for(ll_i=0; ll_i<iLLx_LIMIT; ll_i++)
	{
		if(xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Check__DATA(STR__OUT) < 0)
		{
			continue;
		}

		if(LLx__Is_Available(ll_i) < 0)			continue;
		if(LLx__Is_VAC(ll_i) < 0)				continue;

		if(LLx__Check_Empty__OutSlot(ll_i) < 0)
		{
			continue;
		}

		return 1;
	}

	return -1;
}

int  CObj__DUAL_ARM_STD::
AUTO_CTRL__PMx_RB(CII_OBJECT__VARIABLE *p_variable, CII_OBJECT__ALARM *p_alarm)
{
	if(dCH__VAC_RB__CFG_LLx_CTRL_ONLY_INPUT_OUTPUT_MODE->Check__DATA(STR__DUAL) > 0)
	{
		_AUTO_CTRL__PMx_RB_WITH_DUAL_TYPE(p_variable, p_alarm);
	}
	else
	{
		_AUTO_CTRL__PMx_RB__ALL_MODE(p_variable, p_alarm);
	}
	return 1;
}
int  CObj__DUAL_ARM_STD::
_AUTO_CTRL__PMx_RB_WITH_DUAL_TYPE(CII_OBJECT__VARIABLE *p_variable, CII_OBJECT__ALARM *p_alarm)
{
	DECLARE__EXT_CTRL(p_variable);

	CString log_id = "_AUTO_CTRL__PMx_RB_WITH_DUAL_TYPE()";

	// ...
	if(dCH__VAC_RB__CFG_PICK_WAFER_CONDITION->Check__DATA(STR__ONLY_PROCESSED) > 0)
	{
		if(VAC_RB__Check_Occupied__A_Arm() > 0)
		{
			if(xCH__VAC_RB__SLOT01_STATUS->Check__DATA(STR__MAPPED) > 0)			NEXT__LOOP;
		}
		if(VAC_RB__Check_Occupied__B_Arm() > 0)
		{
			if(xCH__VAC_RB__SLOT02_STATUS->Check__DATA(STR__MAPPED) > 0)			NEXT__LOOP;
		}
	}

	// ...
	CUIntArray l__pm_check_index;

	_Get__IDLE_PM_INDEX_LIST_WITH_WAFER(l__pm_check_index);
	_Get__PM_SORT_WITH_IDLE_TIME(l__pm_check_index, l__pm_check_index);

	// ...
	int k_size = l__pm_check_index.GetSize();
	for(int k=0; k<k_size; k++)
	{
		int pm_i = l__pm_check_index[k];

		if(PMx__Check_Empty__SlotStatus(pm_i) > 0)	
		{
			continue;
		}

		if(PMx__Is_Idle(pm_i) < 0)
		{
			continue;
		}

		if(SCH__PMx_CHECK_PRESSURE(pm_i, p_variable,p_alarm) < 0)
		{
			continue;
		}

		// ...
		CString arm_type;
		CString sch_name;
		CString pm_name;
		CString pm_slot = "1";

		int pm_id  = pm_i + 1;

		pm_name.Format("PM%1d", pm_id);
		sch_name.Format("%s-%s", pm_name,pm_slot);

		if(xSCH_MATERIAL_CTRL->Check__MATERIAL_DB(sch_name) < 0)
		{
			continue;
		}

		if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(sch_name) < 0)
		{
			continue;
		}

		// ...
		bool active__pm_to_pm_constraint = true;

		if((dCH__VAC_RB__CFG_PMx_To_PMx_CONSTRAINT->Check__DATA(STR__ALL) > 0)
		|| (dCH__VAC_RB__CFG_PMx_To_PMx_CONSTRAINT->Check__DATA(STR__SAME_PROCESS) > 0)
		|| (dCH__VAC_RB__CFG_PMx_To_PMx_CONSTRAINT->Check__DATA(STR__SAME_PATH) > 0))
		{
			bool active__empty_arm = false;
			CString empty_arm;

			if(dCH__VAC_RB__CFG_PMx_To_PMx_PICK_CONDITION->Check__DATA(STR__ODD_EVEN) > 0)
			{
				if(VAC_RB__Get_Empty__Arm_Type_With_PMx_Odd_Even(empty_arm, pm_name) > 0)
				{
					active__empty_arm = true;
				}
			}

			if(active__empty_arm)
			{
				arm_type = empty_arm;
			}
			else
			{
				if(VAC_RB__Get_Empty__Arm_Type_From_All(arm_type) < 0)
				{
					continue;
				}
			}

			active__pm_to_pm_constraint = false;
		}
		else
		{
			if(VAC_RB__Get_Empty__Arm_Type_With_PMx_Constraint(arm_type, pm_name) < 0)
			{
				continue;
			}
		}

		// ...
		bool active__pmc_check = false;
		int  active__pmc_index = -1;

		// ...
		{
			int active__lp_normal = 1;

			if(LPx__Get_Process_Type__Normal(sch_name, active__lp_normal) < 0)
			{
				continue;
			}

			// ...
			CStringArray l_pm_name;
			CStringArray l_pm_rcp;

			xSCH_MATERIAL_CTRL->Get__NEXT_PROCESS_INFO(sch_name, l_pm_name,l_pm_rcp);

			// ...
			{
				int count__wfr_type = -1;

				int k_limit = l_pm_name.GetSize();
				for(int k=0; k < k_limit; k++)
				{
					CString str_nmae = l_pm_name[k];

					int pm_index = Macro__Get_PMC_INDEX(str_nmae);
					if(pm_index < 0)			continue;

					if(active__lp_normal < 0)
					{
						if(xEXT_CH__PMx_PROCESS_TYPE[pm_index]->Check__DATA(STR__TEST) < 0)
						{
							continue;
						}
					}

					count__wfr_type = 1;
				}

				if(count__wfr_type < 0)
				{
					active__lp_normal = 1;
				}
			}

			// Normal Pass ...
			if(active__pm_to_pm_constraint)
			{
				int k_limit = l_pm_name.GetSize();

				for(int k=0; k<k_limit; k++)
				{
					CString pm_xxx = l_pm_name[k];
	
					if(!VAC_RB__Check_Empty__Arm_Type_With_PMx_Constraint(arm_type, pm_xxx))
					{
						continue;
					}

					int pm_index = Macro__Get_PMC_INDEX(pm_xxx);
					if(pm_index < 0)			continue;
	
					if(active__lp_normal > 0)
					{
						if(xEXT_CH__PMx_PROCESS_TYPE[pm_index]->Check__DATA(STR__NORMAL) < 0)
						{
							continue;
						}
					}
					else
					{
						if(xEXT_CH__PMx_PROCESS_TYPE[pm_index]->Check__DATA(STR__TEST) < 0)
						{
							continue;
						}
					}

					if(dCH__VAC_RB__INFO_PM_RESERVE_X[pm_index]->Check__DATA(STR__ON) > 0)
					{
						continue;
					}
					if(PMx__Check_Empty__SlotStatus(pm_index) < 0)
					{
						continue;
					}

					if(xEXT_CH__CFG__PMx_USE[pm_index]->Check__DATA(STR__ENABLE) > 0)
					{
						if(PMx__Is_Available(pm_index) < 0)			continue;
					}
					else
					{
						continue;
					}

					active__pmc_check = true;
					active__pmc_index = pm_index;
					break;
				}
			}

			// Abnormal Pass ...
			bool active__abnormal_pass = false;
			CString pm_abnormal;

			if(!active__pmc_check)
			{
				int t_limit = l_pm_name.GetSize();

				for(int t=0; t<t_limit; t++)
				{
					CString pm_xxx = l_pm_name[t];

					/*
					if(!VAC_RB__Check_Empty__Arm_Type_With_PMx_Constraint(arm_type, pm_xxx))
					{
						continue;
					}
					*/

					int pm_index = Macro__Get_PMC_INDEX(pm_xxx);
					if(pm_index < 0)			continue;

					if(active__lp_normal > 0)
					{
						if(xEXT_CH__PMx_PROCESS_TYPE[pm_index]->Check__DATA(STR__NORMAL) < 0)
						{
							continue;
						}
					}
					else
					{
						if(xEXT_CH__PMx_PROCESS_TYPE[pm_index]->Check__DATA(STR__TEST) < 0)
						{
							continue;
						}
					}

					if(dCH__VAC_RB__INFO_PM_RESERVE_X[pm_index]->Check__DATA(STR__ON) > 0)
					{
						continue;
					}
					if(PMx__Check_Empty__SlotStatus(pm_index) < 0)
					{
						continue;
					}

					if(xEXT_CH__CFG__PMx_USE[pm_index]->Check__DATA(STR__ENABLE) > 0)
					{
						if(PMx__Is_Available(pm_index) < 0)			continue;
					}
					else
					{
						continue;
					}

					if(dCH__VAC_RB__CFG_PMx_To_PMx_CONSTRAINT->Check__DATA(STR__SAME_PATH) > 0)
					{
						int pmc__step_count = xSCH_MATERIAL_CTRL->SCH_DB_INFO__Get_Current_Step_Count(sch_name);

						if(VAC_RB__Check_Occupied__A_Arm() > 0)
						{
							CString check_arm = _ARM__A;

							int arm__step_count = xSCH_MATERIAL_CTRL->SCH_DB_INFO__Get_Current_Step_Count(check_arm);
							if(arm__step_count != pmc__step_count)			continue;
						}
						if(VAC_RB__Check_Occupied__B_Arm() > 0)
						{
							CString check_arm = _ARM__B;

							int arm__step_count = xSCH_MATERIAL_CTRL->SCH_DB_INFO__Get_Current_Step_Count(check_arm);
							if(arm__step_count != pmc__step_count)			continue;
						}
					}
					else if(dCH__VAC_RB__CFG_PMx_To_PMx_CONSTRAINT->Check__DATA(STR__SAME_PROCESS) > 0)
					{
						CString cur__pmc_name;
						CString cur__pmc_rcp;

						xSCH_MATERIAL_CTRL->Get__Last_Processing_Info(sch_name, cur__pmc_name,cur__pmc_rcp);

						if(VAC_RB__Check_Occupied__A_Arm() > 0)
						{
							CString check_arm = _ARM__A;
							CString arm__pmc_name;
							CString arm__pmc_rcp;

							xSCH_MATERIAL_CTRL->Get__Last_Processing_Info(check_arm, arm__pmc_name,arm__pmc_rcp);
							if(cur__pmc_rcp != arm__pmc_rcp)				continue;;
						}
						if(VAC_RB__Check_Occupied__B_Arm() > 0)
						{
							CString check_arm = _ARM__B;
							CString arm__pmc_name;
							CString arm__pmc_rcp;

							xSCH_MATERIAL_CTRL->Get__Last_Processing_Info(check_arm, arm__pmc_name,arm__pmc_rcp);
							if(cur__pmc_rcp != arm__pmc_rcp)				continue;;
						}

						/*
						// ...
						{
							CString log_msg;
							CString log_bff;

							log_msg = "_AUTO_CTRL__PMx_RB_WITH_DUAL_TYPE() ... \n";

							// ...
							{
								log_bff.Format("sch_name <- %s \n", sch_name);
								log_msg += log_bff;
								log_bff.Format(" * pmc_name <- [%s] \n", cur__pmc_name);
								log_msg += log_bff;
								log_bff.Format(" * pmc_rcp  <- [%s] \n", cur__pmc_rcp);
								log_msg += log_bff;
							}

							if(VAC_RB__Check_Occupied__A_Arm() > 0)
							{
								CString check_arm = _ARM__A;
								CString arm__pmc_name;
								CString arm__pmc_rcp;

								xSCH_MATERIAL_CTRL->Get__Last_Processing_Info(check_arm, arm__pmc_name,arm__pmc_rcp);

								log_bff.Format("sch_name <- %s \n", check_arm);
								log_msg += log_bff;
								log_bff.Format(" * pmc_name <- [%s] \n", arm__pmc_name);
								log_msg += log_bff;
								log_bff.Format(" * pmc_rcp  <- [%s] \n", arm__pmc_rcp);
								log_msg += log_bff;
							}
							if(VAC_RB__Check_Occupied__B_Arm() > 0)
							{
								CString check_arm = _ARM__B;
								CString arm__pmc_name;
								CString arm__pmc_rcp;

								xSCH_MATERIAL_CTRL->Get__Last_Processing_Info(check_arm, arm__pmc_name,arm__pmc_rcp);

								log_bff.Format("sch_name <- %s \n", check_arm);
								log_msg += log_bff;
								log_bff.Format(" * pmc_name <- [%s] \n", arm__pmc_name);
								log_msg += log_bff;
								log_bff.Format(" * pmc_rcp  <- [%s] \n", arm__pmc_rcp);
								log_msg += log_bff;
							}

							log_msg += "\n";

							printf(log_msg);
						}
						*/
					}

					// ...
					{
						active__abnormal_pass = true;
						pm_abnormal = pm_xxx;

						active__pmc_check = true;
						active__pmc_index = pm_index;
					}
					break;
				}
			}

			if(active__pm_to_pm_constraint)
			{
				if(!active__pmc_check)
				{
					int alm_id = ALID__VAC_ROBOT__DEADLOCK_CONDITION;
					CString alm_msg;
					CString alm_bff;
					CString r_act;

					alm_msg.Format("The wafer in %s can't move to next pm. \n", pm_name);

					int k_limit = l_pm_name.GetSize();
					for(int k=0; k<k_limit; k++)
					{
						alm_bff.Format("  %1d) %s \n", k+1,l_pm_name[k]);
						alm_msg += alm_bff;
					}

					p_alarm->Check__ALARM(alm_id, r_act);
					p_alarm->Post__ALARM_With_MESSAGE(alm_id, alm_msg);
					continue;
				}

				if(active__abnormal_pass)
				{
					int alm_id = ALID__VAC_ROBOT__ABNORMAL_WAFER_MOVING;
					CString alm_msg;
					CString r_act;

					alm_msg.Format("Abnormal pass : %s -> Arm(%s) -> %s \n", pm_name,arm_type,pm_abnormal);

					p_alarm->Check__ALARM(alm_id, r_act);
					p_alarm->Post__ALARM_With_MESSAGE(alm_id, alm_msg);
				}
			}
		}

		if(!active__pmc_check)
		{
			continue;
		}

		// ...
		int act_flag = SCH__PICK_MODULE(p_variable,p_alarm, log_id, false,arm_type,pm_name,pm_slot,sch_name);

		if(VAC_RB__Check_Occupied__Arm_Type(arm_type) > 0)
		{
			xSCH_MATERIAL_CTRL->Pick__From_MODULE(sch_name, arm_type);

			xSCH_MATERIAL_CTRL->Set__MATERIAL_STATUS(arm_type, STR__PROCESSED);
		}

		if(act_flag < 0)
		{
			NEXT__LOOP;
		}

		if((active__pmc_index >= 0)
		&& (active__pmc_index <  CFG_PM_LIMIT))
		{
			dCH__VAC_RB__INFO_PM_RESERVE_X[active__pmc_index]->Set__DATA(STR__ON);
		}
	}

	NEXT__LOOP;
}
int  CObj__DUAL_ARM_STD::
_AUTO_CTRL__PMx_RB__ALL_MODE(CII_OBJECT__VARIABLE *p_variable, CII_OBJECT__ALARM *p_alarm)
{
	DECLARE__EXT_CTRL(p_variable);

	CString log_id = "_AUTO_CTRL__PMx_RB__ALL_MODE()";

	// Check : Wafer State ...
	if(dCH__VAC_RB__CFG_PICK_WAFER_CONDITION->Check__DATA(STR__ONLY_PROCESSED) > 0)
	{
		if(VAC_RB__Check_Occupied__A_Arm() > 0)
		{
			if(xCH__VAC_RB__SLOT01_STATUS->Check__DATA(STR__MAPPED) > 0)			NEXT__LOOP;
		}
		if(VAC_RB__Check_Occupied__B_Arm() > 0)
		{
			if(xCH__VAC_RB__SLOT02_STATUS->Check__DATA(STR__MAPPED) > 0)			NEXT__LOOP;
		}
	}

	// ...
	CString arm_type;

	if(VAC_RB__Check_Empty__Dual_Arm() < 0)						NEXT__LOOP;
	if(VAC_RB__Get_Empty__Arm_Type_From_PMx(arm_type) < 0)		NEXT__LOOP;

	// ...
	IDS__SCH_MATERIAL_STATUS ds_sts;
	int i;

	for(i=0; i<iPMx_SIZE; i++)
	{
		if(PMx__Check_Empty__SlotStatus(i) > 0)		continue;
		if(PMx__Is_Available(i) < 0)				continue;

		if(SCH__PMx_CHECK_PRESSURE(i,p_variable,p_alarm) < 0)
		{
			continue;
		}

		// ...
		CString sch_name;
		CString pm_name;
		CString pm_slot = "1";

		pm_name.Format("PM%1d", i+1);
		sch_name.Format("%s-%s", pm_name,pm_slot);

		if(xSCH_MATERIAL_CTRL->Check__MATERIAL_DB(sch_name) < 0)
		{
			continue;
		}

		if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(sch_name) < 0)
		{
			continue;
		}

		if(xSCH_MATERIAL_CTRL->Get__MATERIAL_STATUS(sch_name,ds_sts) < 0)
		{
			continue;
		}

		// ...
		int act_flag  = -1;
		int move_flag = -1;

		act_flag = SCH__PICK_MODULE(p_variable,p_alarm, log_id, false,arm_type,pm_name,pm_slot,sch_name);

		if(act_flag < 0)
		{
			move_flag = SCH__Check_Material_Pick(arm_type);
		}
		else
		{
			move_flag = 1;
		}

		if(move_flag > 0)
		{
			xSCH_MATERIAL_CTRL->Pick__From_MODULE(sch_name, arm_type);

			xSCH_MATERIAL_CTRL->Set__MATERIAL_STATUS(arm_type, STR__PROCESSED);
		}

		if(act_flag < 0)
		{
			NEXT__LOOP;
		}

		NEXT__LOOP;
	}

	NEXT__LOOP;
}

int  CObj__DUAL_ARM_STD::
AUTO_CTRL__RB_LBo__ONLY_MODE(CII_OBJECT__VARIABLE *p_variable, CII_OBJECT__ALARM *p_alarm)
{
	if(dCH__VAC_RB__CFG_LLx_CTRL_ONLY_INPUT_OUTPUT_MODE->Check__DATA(STR__DUAL) > 0)
	{
		_AUTO_CTRL__RB_LBo__ONLY_MODE_WITH_DUAL_TYPE(p_variable, p_alarm);
	}
	else
	{
		_AUTO_CTRL__RB_LBo__ONLY_MODE(p_variable, p_alarm);
	}

	NEXT__LOOP;
}
int  CObj__DUAL_ARM_STD::
_AUTO_CTRL__RB_LBo__ONLY_MODE_WITH_DUAL_TYPE(CII_OBJECT__VARIABLE *p_variable, CII_OBJECT__ALARM *p_alarm)
{
	if(VAC_RB__Check_Occupied__Dual_Arm() < 0)
	{
		if(_SCH_CHECK__PMo_RB_WITH_DUAL_TYPE(true, false) > 0)			NEXT__LOOP;
	}

	// ...
	bool active__dual_arm = false;

	if(dCH__VAC_RB__CFG_DUAL_ARM_MOVING_AT_THE_SAME_TIME->Check__DATA(STR__ENABLE) > 0)
	{
		active__dual_arm = true;
	}

	// ...
	CString log_id = "_AUTO_CTRL__RB_LBo__ONLY_MODE_WITH_DUAL_TYPE()";

	// ...
	CStringArray l_arm;	
	Get__Arm_List_To_LPx(l_arm);

	if(l_arm.GetSize() < 1)
	{
		NEXT__LOOP;
	}

	// ...
	int sch_ll_id = -1;
	CUIntArray l__sch_ll_slot;

	// Check : LLx ...
	if(sEXT_CH__SCH_MODE_LLx_OUT->Check__DATA(_SCH_MODE__ROUTE) > 0)
	{
		int i_limit = l_arm.GetSize();
		int i;

		for(i=0; i<i_limit; i++)
		{
			CString arm_type = l_arm[i];

			// ...
			CStringArray l_ll_id;
			CStringArray l_ll_mode;
			CStringArray l_ll_slot;

			xSCH_MATERIAL_CTRL->Get__LLx_OUT_OF_EDIT_TYPE(arm_type, l_ll_id,l_ll_mode,l_ll_slot);

			int k_limit = l_ll_id.GetSize();
			int k;

			for(k=0; k<k_limit; k++)
			{
				int ll_i = atoi(l_ll_id[k]) - 1;

				if(ll_i <  0)				continue;
				if(ll_i >= iLLx_LIMIT)		continue;

				// ...
				CString str_mode = l_ll_mode[k];
				CString str_slot = l_ll_slot[k];

				if(str_mode.CompareNoCase(LBx_MODE__ONLY_OUTPUT) != 0)
				{
					continue;
				}

				if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_i]->Check__DATA(LBx_MODE__ONLY_OUTPUT) < 0)
				{
					continue;
				}

				// ...
				CUIntArray l_slot;
				int slot_id;

				if(xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Check__DATA(STR__IN) < 0)		continue;
				if(LLx__Is_Available(ll_i) < 0)										continue;
				if(LLx__Get_Empty__OutSlot(ll_i, slot_id) < 0)						continue;

				if(str_slot.CompareNoCase("11") == 0)
				{
					slot_id = 1;
					if(LLx__Check_Empty__OutSlot(ll_i, slot_id) > 0)		l_slot.Add(slot_id);

					slot_id = 2;
					if(LLx__Check_Empty__OutSlot(ll_i, slot_id) > 0)		l_slot.Add(slot_id);
				}
				else if(str_slot.CompareNoCase("10") == 0)
				{
					slot_id = 1;
					if(LLx__Check_Empty__OutSlot(ll_i, slot_id) > 0)		l_slot.Add(slot_id);
				}	
				else if(str_slot.CompareNoCase("01") == 0)
				{
					slot_id = 2;
					if(LLx__Check_Empty__OutSlot(ll_i, slot_id) > 0)		l_slot.Add(slot_id);
				}

				if(l_slot.GetSize() < 1)			
				{
					continue;
				}

				sch_ll_id = ll_i + 1;
				l__sch_ll_slot.Copy(l_slot);
				break;
			}

			if(sch_ll_id > 0)
			{
				break;
			}
		}
	}
	else
	{
		int active__only_output = -1;

		int i_limit = iLLx_LIMIT;
		int i;

		for(i=0; i<i_limit; i++)
		{
			if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[i]->Check__DATA(LBx_MODE__ONLY_OUTPUT) < 0)
			{
				continue;
			}

			// ...
			int eixt_flag = -1;
			int slot_id;

			if(xEXT_CH__LLx__IN_OUT_FLAG[i]->Check__DATA(STR__IN) < 0)		eixt_flag = 1;
			if(LLx__Is_Available(i) < 0)									eixt_flag = 2;
			if(LLx__Get_Empty__OutSlot(i, slot_id) < 0)						eixt_flag = 3;

			if(SCH__LLx_CHECK_PRESSURE(i, p_variable,p_alarm,1) < 0)
			{
				eixt_flag = 11;
			}

			if(eixt_flag > 0)
			{
				continue;
			}

			// ...
			{
				sch_ll_id = i + 1;
				l__sch_ll_slot.Add(slot_id);
			}

			active__only_output = 1;
			break;
		}

		if(active__only_output < 0)
		{
			NEXT__LOOP;
		}
	}

	if(sch_ll_id < 0)
	{
		NEXT__LOOP;
	}

	// ...
	int sch_ll_i = sch_ll_id - 1;

	if(active__dual_arm)
	{
		int slot_max = l__sch_ll_slot.GetSize();

		int i_limit = l_arm.GetSize();
		int i;

		if(slot_max < i_limit)
		{
			i_limit = slot_max;
		}

		if(i_limit < 2)
		{
			NEXT__LOOP;
		}

		if(xEXT_CH__LLx__IN_OUT_FLAG[sch_ll_i]->Check__DATA(STR__IN) < 0)
		{
			NEXT__LOOP;
		}

		// ...
		{
			CString arm_type = _ARM__AB;
			int slot_id = l__sch_ll_slot[0];

			// ...
			int id_lerf  = slot_id;
			int id_right = slot_id + 1;

			CString sch_01;
			CString sch_02;
			CString para_module;
			CString para_slot;

			para_module = Get__LLx_NAME(sch_ll_i);
			para_slot.Format("%1d", slot_id);

			sch_01.Format("%s-%1d", para_module,id_lerf);
			sch_02.Format("%s-%1d", para_module,id_right);

			// ...
			int act_flag = SCH__PLACE_MODULE(p_variable,p_alarm, log_id, false,arm_type,para_module,para_slot, sch_01,sch_02);

			if(VAC_RB__Check_Occupied__A_Arm() > 0)
			{
				xSCH_MATERIAL_CTRL->Place__To_MODULE(_ARM__A, sch_01);
			}
			if(VAC_RB__Check_Occupied__B_Arm() > 0)
			{
				xSCH_MATERIAL_CTRL->Place__To_MODULE(_ARM__B, sch_02);
			}

			if(act_flag < 0)
			{
				NEXT__LOOP;
			}
		}
	}
	else
	{
		int i_limit = 2;
		int i;

		for(i=0; i<i_limit; i++)
		{
			CString arm_type;

			if(i == 0)
			{
				if(VAC_RB__Check_Occupied__A_Arm() < 0)			continue;

				arm_type = _ARM__A;
			}
			else if(i == 1)
			{
				if(VAC_RB__Check_Occupied__B_Arm() < 0)			continue;

				arm_type = _ARM__B;
			}
			else
			{
				continue;
			}

			if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(arm_type) > 0)
			{
				continue;
			}

			// ...
			CString para_module;
			CString para_slot;
			int ll_slot_id;

			CString ll_name = Get__LLx_NAME(sch_ll_i);

			if(xEXT_CH__LLx__IN_OUT_FLAG[sch_ll_i]->Check__DATA(STR__IN) < 0)
			{
				NEXT__LOOP;
			}
			
			if(LLx__Get_Empty__OutSlot(sch_ll_i, ll_slot_id) < 0)
			{
				NEXT__LOOP;
			}

			para_module = Get__LLx_NAME(sch_ll_i);
			para_slot.Format("%1d", ll_slot_id);

			CString sch_name;
			sch_name.Format("%s-%s", para_module,para_slot);

			int act_flag = SCH__PLACE_MODULE(p_variable,p_alarm, log_id, false,arm_type,para_module,para_slot, sch_name);
				
			if(VAC_RB__Check_Empty__Arm_Type(arm_type) > 0)
			{
				xSCH_MATERIAL_CTRL->Place__To_MODULE(arm_type, sch_name);
			}

			if(act_flag < 0)
			{
				NEXT__LOOP;
			}
		}
	}

	if(LLx__Is_Available(sch_ll_i) > 0)
	{
		if(LLx__Check_Empty__OutSlot(sch_ll_i) < 0)
		{
			SCH_RUN__LLx_VENT(sch_ll_i, log_id, "1");

			xEXT_CH__LLx__IN_OUT_FLAG[sch_ll_i]->Set__DATA("OUT");
		}
		else
		{
			if(SCH__CHECK_MATERIAL_CLEAR(true) > 0)
			{
				SCH_RUN__LLx_VENT(sch_ll_i, log_id, "2");

				xEXT_CH__LLx__IN_OUT_FLAG[sch_ll_i]->Set__DATA("OUT");
			}
		}
	}
	else
	{
		if(LLx__Check_Empty__OutSlot(sch_ll_i) < 0)
		{
			sEXT_CH__LLx__NEXT_CTRL_REQ[sch_ll_i]->Set__DATA("VENTING");
			xEXT_CH__LLx__IN_OUT_FLAG[sch_ll_i]->Set__DATA("OUT");
		}
	}

	NEXT__LOOP;
}
int  CObj__DUAL_ARM_STD::
_AUTO_CTRL__RB_LBo__ONLY_MODE(CII_OBJECT__VARIABLE *p_variable, CII_OBJECT__ALARM *p_alarm)
{
	CStringArray l_arm;	
	Get__Arm_List_To_LPx(l_arm);

	if(l_arm.GetSize() < 1)
	{
		NEXT__LOOP;
	}

	// ...
	CString log_id = "_AUTO_CTRL__RB_LBo__ONLY_MODE()";

	// ...
	int sch_ll_id = -1;
	CUIntArray l__sch_ll_slot;

	// Check : LLx ...
	if(sEXT_CH__SCH_MODE_LLx_OUT->Check__DATA(_SCH_MODE__ROUTE) > 0)
	{
		int i_limit = l_arm.GetSize();
		int i;

		for(i=0; i<i_limit; i++)
		{
			CString arm_type = l_arm[i];

			// ...
			CStringArray l_ll_id;
			CStringArray l_ll_mode;
			CStringArray l_ll_slot;

			xSCH_MATERIAL_CTRL->Get__LLx_OUT_OF_EDIT_TYPE(arm_type, l_ll_id,l_ll_mode,l_ll_slot);

			int k_limit = l_ll_id.GetSize();
			int k;

			for(k=0; k<k_limit; k++)
			{
				int ll_i = atoi(l_ll_id[k]) - 1;

				if(ll_i <  0)				continue;
				if(ll_i >= iLLx_LIMIT)		continue;

				// ...
				CString str_mode = l_ll_mode[k];
				CString str_slot = l_ll_slot[k];

				if(str_mode.CompareNoCase(LBx_MODE__ONLY_OUTPUT) != 0)
				{
					continue;
				}
				if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_i]->Check__DATA(LBx_MODE__ONLY_OUTPUT) < 0)
				{
					continue;
				}

				// ...
				if(xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Check__DATA(STR__IN) < 0)		continue;

				if(LLx__Is_Available(ll_i) < 0)										continue;
				if(LLx__Is_VAC(ll_i) < 0)											continue;

				// ...
				CUIntArray l_slot;
				int slot_id;

				if(LLx__Get_Empty__OutSlot(ll_i, slot_id) < 0)
				{
					continue;
				}
				
				if(str_slot.CompareNoCase("11") == 0)
				{
					slot_id = 1;
					if(LLx__Check_Empty__OutSlot(ll_i, slot_id) > 0)		l_slot.Add(slot_id);

					slot_id = 2;
					if(LLx__Check_Empty__OutSlot(ll_i, slot_id) > 0)		l_slot.Add(slot_id);
				}
				else if(str_slot.CompareNoCase("10") == 0)
				{
					slot_id = 1;
					if(LLx__Check_Empty__OutSlot(ll_i, slot_id) > 0)		l_slot.Add(slot_id);
				}	
				else if(str_slot.CompareNoCase("01") == 0)
				{
					slot_id = 2;
					if(LLx__Check_Empty__OutSlot(ll_i, slot_id) > 0)		l_slot.Add(slot_id);
				}

				if(l_slot.GetSize() < 1)			
				{
					continue;
				}

				sch_ll_id = ll_i + 1;
				l__sch_ll_slot.Copy(l_slot);
				break;
			}

			if(sch_ll_id > 0)
			{
				break;
			}
		}
	}
	else
	{
		int active__only_output = -1;

		int i_limit = iLLx_LIMIT;
		int i;

		for(i=0; i<i_limit; i++)
		{
			if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[i]->Check__DATA(LBx_MODE__ONLY_OUTPUT) < 0)
			{
				continue;
			}

			// ...
			if(xEXT_CH__LLx__IN_OUT_FLAG[i]->Check__DATA(STR__IN) < 0)		continue;

			if(LLx__Is_Available(i) < 0)									continue;
			if(LLx__Is_VAC(i) < 0)											continue;

			// ...
			int slot_id;

			if(LLx__Get_Empty__OutSlot(i, slot_id) < 0)
			{
				continue;
			}

			if(SCH__LLx_CHECK_PRESSURE(i, p_variable,p_alarm,1) < 0)
			{
				continue;
			}

			// ...
			{
				sch_ll_id = i + 1;
				l__sch_ll_slot.Add(slot_id);
			}

			active__only_output = 1;
			break;
		}

		if(active__only_output < 0)
		{
			NEXT__LOOP;
		}
	}

	if(sch_ll_id < 0)
	{
		NEXT__LOOP;
	}

	// ...
	{
		int sch_ll_i = sch_ll_id - 1;
		int slot_max = l__sch_ll_slot.GetSize();

		int i_limit = l_arm.GetSize();
		int i;

		if(slot_max < i_limit)
		{
			i_limit = slot_max;
		}
		
		for(i=0; i<i_limit; i++)
		{
			CString arm_type = l_arm[i];
			int slot_id = l__sch_ll_slot[i];

			// ...
			CString sch_module;
			CString para_module;
			CString para_slot;

			para_module = Get__LLx_NAME(sch_ll_i);
			para_slot.Format("%1d", slot_id);
			sch_module.Format("%s-%s", para_module,para_slot);

			// ...
			int act_flag  = -1;
			int move_flag = -1;

			if((i+1) == i_limit)
			{
				act_flag = SCH__PLACE_MODULE(p_variable,p_alarm, log_id, false,arm_type,para_module,para_slot,sch_module);
			}
			else
			{
				act_flag = SCH__PLACE_MODULE(p_variable,p_alarm, log_id, true,arm_type,para_module,para_slot,sch_module);
			}

			if(act_flag < 0)
			{
				move_flag = SCH__Check_Material_Place(arm_type);
			}
			else
			{
				move_flag = 1;
			}		

			if(move_flag > 0)
			{
				xSCH_MATERIAL_CTRL->Place__To_MODULE(arm_type,sch_module);
			}

			if(act_flag < 0)
			{
				NEXT__LOOP;
			}
		}

		if(LLx__Is_Available(sch_ll_i) > 0)
		{
			if(LLx__Check_Empty__OutSlot(sch_ll_i) < 0)
			{
				SCH_RUN__LLx_VENT(sch_ll_i, log_id, "1");

				xEXT_CH__LLx__IN_OUT_FLAG[sch_ll_i]->Set__DATA(STR__OUT);
			}
			else
			{
				if(SCH__CHECK_MATERIAL_CLEAR(true) > 0)
				{
					SCH_RUN__LLx_VENT(sch_ll_i, log_id, "2");

					xEXT_CH__LLx__IN_OUT_FLAG[sch_ll_i]->Set__DATA(STR__OUT);
				}
			}
		}
		else
		{
			if(LLx__Check_Empty__OutSlot(sch_ll_i) < 0)
			{
				sEXT_CH__LLx__NEXT_CTRL_REQ[sch_ll_i]->Set__DATA("VENTING");
				xEXT_CH__LLx__IN_OUT_FLAG[sch_ll_i]->Set__DATA(STR__OUT);
			}
		}
	}

	NEXT__LOOP;
}
int  CObj__DUAL_ARM_STD::
AUTO_CTRL__RB_LBo__1_SLOT_ALL_MODE(CII_OBJECT__VARIABLE *p_variable,
								   CII_OBJECT__ALARM *p_alarm)
{
	DECLARE__EXT_CTRL(p_variable);

	CString log_id = "AUTO_CTRL__RB_LBo__1_SLOT_ALL_MODE()";

	// LLx ...
	{
		int active__all_mode = -1;
		
		int ll_limit = iLLx_LIMIT;
		int ll_i;

		for(ll_i=0; ll_i<ll_limit; ll_i++)
		{
			if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_i]->Check__DATA(LBx_MODE__ALL) > 0)
			{
				active__all_mode = 1;
			}
		}

		if(active__all_mode < 0)
		{
			NEXT__LOOP;
		}
	}

	if(VAC_RB__Check_Empty__Dual_Arm() > 0)
	{
		NEXT__LOOP;
	}

	// ...
	CString check_arm = "";

	if(sEXT_CH__SCH_MODE_LLx_OUT->Check__DATA(_SCH_MODE__ROUTE) > 0)
	{
		for(int i=0; i<2; i++)
		{
			if(i == 0)
			{
				if(VAC_RB__Check_Occupied__A_Arm() < 0)			continue;

				check_arm = STR__A;
			}
			else if(i == 1)
			{
				if(VAC_RB__Check_Occupied__B_Arm() < 0)			continue;

				check_arm = STR__B;
			}
			else
			{
				continue;
			}

			if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(check_arm) > 0)
			{
				check_arm = "";
				continue;
			}

			break;
		}

		if(check_arm.GetLength() > 0)
		{
			CStringArray l_ll_id;
			CStringArray l_ll_mode;
			CStringArray l_ll_slot;

			xSCH_MATERIAL_CTRL->Get__LLx_OUT_OF_EDIT_TYPE(check_arm, l_ll_id,l_ll_mode,l_ll_slot);

			// ...
			bool active__llx_abnormal = true;

			// Check : LLx_OUT ...
			{
				int k_limit = l_ll_id.GetSize();

				for(int k=0; k<k_limit; k++)
				{
					int ll_id = atoi(l_ll_id[k]);
					int ll_index = ll_id - 1;

					if(ll_index <  0)				continue;
					if(ll_index >= iLLx_LIMIT)		continue;

					// ...
					if(xEXT_CH__SCH_DB_LLx_USE_FLAG[ll_index]->Check__DATA(STR__ENABLE) < 0)		continue;
					if(xEXT_CH__LLx__IN_OUT_FLAG[ll_index]->Check__DATA(STR__OUT) < 0)				continue;

					CString obj_state = xEXT_CH__LLx__OBJ_STATUS[ll_index]->Get__STRING();
					if((obj_state.CompareNoCase(STR__CTCINUSE) != 0)
					&& (obj_state.CompareNoCase(STR__STANDBY)  != 0))
					{
						continue;
					}

					if(LLx__Is_Available(ll_index) < 0)			continue;
					if(LLx__Is_VAC(ll_index) < 0)				continue;

					if(LLx__Check_Empty__OutSlot(ll_index) < 0)
					{
						if(VAC_RB__Check_Empty__Arm_Type() > 0)
						{
							if(LLx__Check_Occupied__InSlot(ll_index) < 0)
							{
								continue;
							}
						}
						else
						{
							continue;
						}
					}

					active__llx_abnormal = false;
					break;
				}
			}

			// Check : Abnormal ...
			if(active__llx_abnormal)
			{
				int wfr_count = xSCH_MATERIAL_CTRL->Check__MATERIAL_LESS_THAN_MOVE_COUNT(4);

				if(wfr_count < 1)
				{
					int k_limit = l_ll_id.GetSize();

					for(int k=0; k<k_limit; k++)
					{
						int ll_id = atoi(l_ll_id[k]);
						int ll_index = ll_id - 1;

						if(ll_index <  0)				continue;
						if(ll_index >= iLLx_LIMIT)		continue;

						// ...
						if(xEXT_CH__LLx__IN_OUT_FLAG[ll_index]->Check__DATA(STR__IN)  < 0)			continue;
						if(xEXT_CH__ATM_RB__SCH_STS_TO_LLx[ll_index]->Check__DATA("") < 0)			continue;

						if(LLx__Is_Available(ll_index) < 0)			continue;
						if(LLx__Is_VAC(ll_index) < 0)				continue;

						// ...
						CString str_mode = l_ll_mode[k];
						CString str_slot = l_ll_slot[k];

						if(str_mode.CompareNoCase(STR__ALL) != 0)
						{
							continue;;
						}

						if((str_slot.CompareNoCase("11") == 0)
						|| (str_slot.CompareNoCase("10") == 0)
						|| (str_slot.CompareNoCase("01") == 0))
						{
							int slot_id;

							if(LLx__Get_Empty__OutSlot(ll_index, slot_id) < 0)
							{
								continue;
							}

							if(SCH_RUN__LLx_PUMP(ll_index, log_id, "") > 0)
							{
								xEXT_CH__LLx__IN_OUT_FLAG[ll_index]->Set__DATA(STR__OUT);
							}
							NEXT__LOOP;
						}
					}
				}
			}
		}
		else
		{
			NEXT__LOOP;
		}
	}

	// ...
	CString para__ll_name;
	CString para__in_slot;
	CString para__out_slot;

	// ...
	{
		CStringArray l__sch_name;
		CStringArray l__ll_name;
		CStringArray l__ll_slot;

		_GET__WAFER_LIST_OF_LLx_IN__ALL_MODE(l__sch_name, l__ll_name,l__ll_slot);

		int k_limit = l__sch_name.GetSize();

		// LL_IN -> RB ...
		if(k_limit > 0)
		{
			for(int k=0; k < k_limit; k++)
			{
				para__in_slot  = "";
				para__out_slot = "";

				// ...
				CString str__ll_name = l__ll_name[k];
				CString str__ll_slot = l__ll_slot[k];

				int ll_index = Get__LLx_INDEX(str__ll_name);
				if(ll_index < 0)		continue;;

				if(xEXT_CH__LLx__IN_OUT_FLAG[ll_index]->Check__DATA(STR__OUT) < 0)
				{
					continue;
				}

				if(LLx__Is_Available(ll_index) < 0)		continue;
				if(LLx__Is_VAC(ll_index) < 0)			continue;

				// CHECK : LL_OUT ...
				{
					int ll__out_slot;

					int r_flag = LLx__Get_Empty__OutSlot(ll_index, ll__out_slot);
					if(r_flag > 0)
					{
						para__out_slot.Format("%1d", ll__out_slot);
					}
				}

				// CHECK : LL_IN -> PM_IN ...
				{
					bool active__ready_check = false;

					if(dCH__VAC_RB__CFG_PICK_WAFER_CONDITION->Check__DATA(STR__ONLY_PROCESSED) < 0)
					{
						if(VAC_RB__Check_Empty__Arm_Type() > 0)			active__ready_check = true;
					}

					int r_flag = _CHECK__LBi_To_RB_With_Ready_PMC__ALL_MODE(str__ll_name, str__ll_slot, active__ready_check);
					if(r_flag > 0)
					{
						para__in_slot = str__ll_slot;
					}
				}

				if((para__in_slot.GetLength()  < 1)
				&& (para__out_slot.GetLength() < 1))
				{
					continue;
				}

				para__ll_name = str__ll_name;
				break;
			}
		}
		else
		{
			CStringArray l__arm_type;
			VAC_RB__Get_Occupied__Arm_Type(l__arm_type);

			int k_limit = l__arm_type.GetSize();
			for(int k=0; k<k_limit; k++)
			{
				CString arm_type = l__arm_type[k];

				if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(arm_type) > 0)
				{
					continue;
				}

				if(sEXT_CH__SCH_MODE_LLx_OUT->Check__DATA(_SCH_MODE__ROUTE) > 0)
				{
					CStringArray l_ll_id;
					CStringArray l_ll_mode;
					CStringArray l_ll_slot;

					xSCH_MATERIAL_CTRL->Get__LLx_OUT_OF_EDIT_TYPE(arm_type, l_ll_id,l_ll_mode,l_ll_slot);

					int t_limit = l_ll_id.GetSize();
					for(int t=0; t < t_limit; t++)
					{
						int ll_id = atoi(l_ll_id[t]);
						int ll_index = ll_id - 1;

						if(ll_index < 0)							continue;
						if(ll_index >= iLLx_LIMIT)					continue;

						if(xEXT_CH__LLx__IN_OUT_FLAG[ll_index]->Check__DATA(STR__OUT) < 0)
						{
							continue;
						}

						if(LLx__Is_Available(ll_index) < 0)			continue;
						if(LLx__Is_VAC(ll_index) < 0)				continue;

						// ...
						int ll__out_slot;

						int r_flag = LLx__Get_Empty__OutSlot(ll_index, ll__out_slot);
						if(r_flag < 0)			continue;

						para__ll_name = Get__LLx_NAME(ll_index);
						para__out_slot.Format("%1d", ll__out_slot);
						para__in_slot = "";
						break;
					}
				}
				else
				{
					for(int ll_index=0; ll_index < iLLx_LIMIT; ll_index++)
					{
						if(xEXT_CH__LLx__IN_OUT_FLAG[ll_index]->Check__DATA(STR__OUT) < 0)
						{
							continue;
						}

						if(LLx__Is_Available(ll_index) < 0)			continue;
						if(LLx__Is_VAC(ll_index) < 0)				continue;

						// ...
						int ll__out_slot;

						int r_flag = LLx__Get_Empty__OutSlot(ll_index, ll__out_slot);
						if(r_flag < 0)			continue;

						para__ll_name = Get__LLx_NAME(ll_index);
						para__out_slot.Format("%1d", ll__out_slot);
						para__in_slot = "";
						break;
					}
				}

				if(para__ll_name.GetLength() > 0)
				{
					break;
				}
			}
		}

		if(para__ll_name == "")
		{
			NEXT__LOOP;
		}
	}

	// RB -> LL_OUT & LL_IN ...
	{
		CStringArray l__arm_type;
		VAC_RB__Get_Occupied__Arm_Type(l__arm_type);

		int k_limit = l__arm_type.GetSize();
		for(int k=0; k < k_limit; k++)
		{
			CString arm_type = l__arm_type[k];

			if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(arm_type) > 0)
			{
				continue;
			}

			// ...
			bool active__ll_exchange = false;
			CString empty_arm;

			if(para__in_slot.GetLength() > 0)
			{
				if(VAC_RB__Get_Empty__Arm_Type_From_LBx(empty_arm) > 0)
				{
					active__ll_exchange = true;

					if(para__out_slot.GetLength() < 1)		para__out_slot = para__in_slot;
				}
			}

			// LL_IN -> RB ...
			if(active__ll_exchange)
			{
				CString sch_name;
				sch_name.Format("%s-%s", para__ll_name, para__in_slot);	

				int act_flag = SCH__PICK_MODULE(p_variable,p_alarm, log_id,  active__ll_exchange, empty_arm, para__ll_name,para__in_slot, sch_name);
				int move_flag = 1;

				if(act_flag < 0)
				{
					move_flag = SCH__Check_Material_Pick(empty_arm);
				}

				if(move_flag > 0)
				{
					xSCH_MATERIAL_CTRL->Pick__From_MODULE(sch_name, empty_arm);
				}

				if(act_flag < 0)
				{
					NEXT__LOOP;
				}
			}

			// RB -> LL_OUT ...
			{
				CString sch_name;
				sch_name.Format("%s-%s", para__ll_name, para__out_slot);	

				int act_flag = SCH__PLACE_MODULE(p_variable,p_alarm, log_id, false, arm_type, para__ll_name,para__out_slot, sch_name);
				int move_flag = 1;

				if(act_flag < 0)
				{
					move_flag = SCH__Check_Material_Place(arm_type);
				}

				if(move_flag > 0)
				{
					xSCH_MATERIAL_CTRL->Place__To_MODULE(arm_type, sch_name);
				}

				if(act_flag < 0)
				{
					NEXT__LOOP;
				}
			}

			// LLx VENT ...
			{
				int ll_i = Get__LLx_INDEX(para__ll_name);
				if(ll_i >= 0)
				{
					int count__ll_out_empty = 0;
					int ref_ll_i = ll_i; 

					// ...
					{
						for(int ll_i=0; ll_i<iLLx_LIMIT; ll_i++)
						{
							if(ref_ll_i == ll_i)		
							{
								continue;
							}

							if((dEXT_CH__LLx__ACTIVE_ATM_AREA_CTRL[ll_i]->Check__DATA(STR__ON) > 0)
							|| (dEXT_CH__LLx__ACTIVE_VAC_AREA_CTRL[ll_i]->Check__DATA(STR__ON) > 0))
							{
								continue;
							}

							if(xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Check__DATA(STR__OUT) < 0)
							{
								continue;
							}

							if(LLx__Is_Available(ll_i) < 0)					continue;
							if(LLx__Is_VAC(ll_i) < 0)						continue;
							if(LLx__Check_Empty__OutSlot(ll_i) < 0)			continue;

							count__ll_out_empty++;
						}
					}

					if(LLx__Check_Occupied__OutSlot(ll_i) > 0)
					{	
						bool active__ll_vent = true;

						if(count__ll_out_empty > 0)
						{
							int ll_in_slot;

							if(LLx__Get_Occupied__InSlot(ll_i, ll_in_slot) > 0)
							{
								CString ll_name = Get__LLx_NAME(ll_i);

								if(_CHECK__LBi_To_RB_With_Ready_PMC__ALL_MODE(ll_name, ll_in_slot, true) > 0)
								{
									active__ll_vent = false;
								}
							}
						}

						if(active__ll_vent)
						{
							CString ll_name = Get__LLx_NAME(ll_i);

							printf(" * AUTO_CTRL__RB_LBo__ALL_MODE() : %s-Venting !!! \n", ll_name);

							if(SCH_RUN__LLx_VENT(ll_i, log_id, "201") > 0)
							{
								xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Set__DATA(STR__IN);
							}
						}
					}
				}
			}

			NEXT__LOOP;
		}
	}

	NEXT__LOOP;
}

int  CObj__DUAL_ARM_STD::
Fnc__Get_LLx__Empty_Out_Slot(CII_OBJECT__VARIABLE* p_variable,
							 CII_OBJECT__ALARM* p_alarm,
							 const int ok_check,
							 const int ll_i,
							 CString& ll_name,
							 CString& ll_slot)
{
	if(xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Check__DATA("OUT") > 0)
	{
		if((SCH__LLx_CHECK_PRESSURE(ll_i, p_variable,p_alarm) > 0)
		&& (LLx__Is_Available(ll_i) > 0))
		{
			int lbx_ok = -1;
			int slot_id;

			if(ok_check > 0)
			{
				if(LLx__Check_All_Empty(ll_i) > 0)
				{
					lbx_ok = 1;
				}
				else if(VAC_RB__Check_Occupied__Dual_Arm() > 0)		
				{
					lbx_ok = 1;
				}
			}
			else
			{
				lbx_ok = 1;
			}

			if(lbx_ok > 0)
			{
				if(LLx__Get_Empty__OutSlot(ll_i, slot_id) > 0)
				{
					ll_name = Get__LLx_NAME(ll_i);
					ll_slot.Format("%1d",slot_id);
					return 1;
				}
			}
		}
	}
	return -1;
}

int  CObj__DUAL_ARM_STD::
AUTO_CTRL__RB_LBo__ALL_MODE(CII_OBJECT__VARIABLE *p_variable,
							CII_OBJECT__ALARM *p_alarm,
							const int loop_count)
{
	DECLARE__EXT_CTRL(p_variable);

	CString log_id = "AUTO_CTRL__RB_LBo__ALL_MODE()";

	// ...
	{	
		CStringArray l__arm_type;
		VAC_RB__Get_Occupied__Arm_Type(l__arm_type);

		int k_limit = l__arm_type.GetSize();
		for(int k=0; k < k_limit; k++)
		{
			CString arm_type = l__arm_type[k];

			if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(arm_type) < 0)
			{
				continue;
			}

			if(xCH_CFG__SCH_EXCHANGE_PRIORITY->Check__DATA(STR__PMC) > 0)
			{
				if(_CHECK__Ready_PMC_OF_SCH_NAME(arm_type) > 0)
				{
					NEXT__LOOP;
				}
			}
		}
	}

	if(sEXT_CH__SCH_MODE_LLx_OUT->Check__DATA(_SCH_MODE__ROUTE) > 0)
	{
		CString check_arm = "";

		// ...
		{
			CStringArray l__arm_type;
			VAC_RB__Get_Occupied__Arm_Type(l__arm_type);

			int k_limit = l__arm_type.GetSize();
			for(int k=0; k < k_limit; k++)
			{
				CString arm_type = l__arm_type[k];

				if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(arm_type) > 0)
				{
					continue;
				}	

				check_arm = arm_type;
				break;
			}
		}

		if(check_arm.GetLength() > 0)
		{
			CStringArray l_ll_id;
			CStringArray l_ll_mode;
			CStringArray l_ll_slot;

			xSCH_MATERIAL_CTRL->Get__LLx_OUT_OF_EDIT_TYPE(check_arm, l_ll_id,l_ll_mode,l_ll_slot);

			// ...
			bool active__llx_abnormal = true;

			// Check : LLx_OUT ...
			{
				int k_limit = l_ll_id.GetSize();
				
				for(int k=0; k<k_limit; k++)
				{
					int ll_id = atoi(l_ll_id[k]);
					int ll_index = ll_id - 1;

					if(ll_index <  0)				continue;
					if(ll_index >= iLLx_LIMIT)		continue;

					// ...
					if(xEXT_CH__SCH_DB_LLx_USE_FLAG[ll_index]->Check__DATA(STR__ENABLE) < 0)		continue;
					if(xEXT_CH__LLx__IN_OUT_FLAG[ll_index]->Check__DATA(STR__OUT) < 0)				continue;

					CString obj_state = xEXT_CH__LLx__OBJ_STATUS[ll_index]->Get__STRING();
					if((obj_state.CompareNoCase(STR__CTCINUSE) != 0)
					&& (obj_state.CompareNoCase(STR__STANDBY)  != 0))
					{
						continue;
					}

					if(LLx__Is_Available(ll_index) < 0)			continue;
					if(LLx__Is_VAC(ll_index) < 0)				continue;

					if(LLx__Check_Empty__OutSlot(ll_index) < 0)
					{
						continue;
					}

					active__llx_abnormal = false;
					break;
				}
			}

			// Check : Abnormal ...
			if(active__llx_abnormal)
			{
				int wfr_count = xSCH_MATERIAL_CTRL->Check__MATERIAL_LESS_THAN_MOVE_COUNT(4);

				if(wfr_count < 1)
				{
					int k_limit = l_ll_id.GetSize();

					for(int k=0; k<k_limit; k++)
					{
						int ll_id = atoi(l_ll_id[k]);
						int ll_index = ll_id - 1;

						if(ll_index <  0)				continue;
						if(ll_index >= iLLx_LIMIT)		continue;

						// ...
						if(xEXT_CH__LLx__IN_OUT_FLAG[ll_index]->Check__DATA(STR__IN)  < 0)			continue;
						if(xEXT_CH__ATM_RB__SCH_STS_TO_LLx[ll_index]->Check__DATA("") < 0)			continue;

						if(LLx__Is_Available(ll_index) < 0)			continue;
						if(LLx__Is_VAC(ll_index) < 0)				continue;

						// ...
						CString str_mode = l_ll_mode[k];
						CString str_slot = l_ll_slot[k];

						if(str_mode.CompareNoCase(STR__ALL) != 0)
						{
							continue;;
						}

						if((str_slot.CompareNoCase("11") == 0)
						|| (str_slot.CompareNoCase("10") == 0)
						|| (str_slot.CompareNoCase("01") == 0))
						{
							int slot_id;

							if(LLx__Get_Empty__OutSlot(ll_index, slot_id) < 0)
							{
								continue;
							}
	
							if(SCH_RUN__LLx_PUMP(ll_index, log_id, "") > 0)
							{
								xEXT_CH__LLx__IN_OUT_FLAG[ll_index]->Set__DATA(STR__OUT);
							}
							NEXT__LOOP;
						}
					}
				}
			}
		}
		else
		{
			NEXT__LOOP;
		}
	}

	// ...
	CString para__ll_name;
	CString para__in_slot;
	CString para__out_slot;

	// ...
	{
		CStringArray l__sch_name;
		CStringArray l__ll_name;
		CStringArray l__ll_slot;

		_GET__WAFER_LIST_OF_LLx_IN__ALL_MODE(l__sch_name, l__ll_name,l__ll_slot);

		int k_limit = l__sch_name.GetSize();

		// LL_IN -> RB ...
		if(k_limit > 0)
		{
			for(int k=0; k < k_limit; k++)
			{
				CString str__ll_name = l__ll_name[k];
				CString str__ll_slot = l__ll_slot[k];

				int ll_index = Get__LLx_INDEX(str__ll_name);
				if(ll_index < 0)		continue;;

				if(xEXT_CH__LLx__IN_OUT_FLAG[ll_index]->Check__DATA(STR__OUT) < 0)
				{
					continue;
				}

				if(LLx__Is_Available(ll_index) < 0)		continue;
				if(LLx__Is_VAC(ll_index) < 0)			continue;

				// CHECK : LL_OUT ...
				{
					int ll__out_slot;

					int r_flag = LLx__Get_Empty__OutSlot(ll_index, ll__out_slot);
					if(r_flag < 0)			continue;

					para__out_slot.Format("%1d", ll__out_slot);
				}

				// CHECK : LL_IN -> PM_IN ...
				{
					bool active__ready_check = false;

					if(dCH__VAC_RB__CFG_PICK_WAFER_CONDITION->Check__DATA(STR__ONLY_PROCESSED) < 0)
					{
						if(VAC_RB__Check_Empty__Arm_Type() > 0)			active__ready_check = true;
					}

					int r_flag = _CHECK__LBi_To_RB_With_Ready_PMC__ALL_MODE(str__ll_name, str__ll_slot, active__ready_check);
					if(r_flag > 0)
					{
						para__in_slot = str__ll_slot;
					}
				}

				para__ll_name = str__ll_name;
				break;
			}
		}
		else
		{
			CStringArray l__arm_type;
			VAC_RB__Get_Occupied__Arm_Type(l__arm_type);

			int k_limit = l__arm_type.GetSize();
			for(int k=0; k<k_limit; k++)
			{
				CString arm_type = l__arm_type[k];

				if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(arm_type) > 0)
				{
					continue;
				}

				if(sEXT_CH__SCH_MODE_LLx_OUT->Check__DATA(_SCH_MODE__ROUTE) > 0)
				{
					CStringArray l_ll_id;
					CStringArray l_ll_mode;
					CStringArray l_ll_slot;

					xSCH_MATERIAL_CTRL->Get__LLx_OUT_OF_EDIT_TYPE(arm_type, l_ll_id,l_ll_mode,l_ll_slot);
	
					int t_limit = l_ll_id.GetSize();
					for(int t=0; t < t_limit; t++)
					{
						int ll_id = atoi(l_ll_id[t]);
						int ll_index = ll_id - 1;

						if(ll_index < 0)							continue;
						if(ll_index >= iLLx_LIMIT)					continue;

						if(xEXT_CH__LLx__IN_OUT_FLAG[ll_index]->Check__DATA(STR__OUT) < 0)
						{
							continue;
						}

						if(LLx__Is_Available(ll_index) < 0)			continue;
						if(LLx__Is_VAC(ll_index) < 0)				continue;

						// ...
						int ll__out_slot;

						int r_flag = LLx__Get_Empty__OutSlot(ll_index, ll__out_slot);
						if(r_flag < 0)			continue;

						para__ll_name = Get__LLx_NAME(ll_index);
						para__out_slot.Format("%1d", ll__out_slot);
						para__in_slot = "";
						break;
					}
				}
				else
				{
					for(int ll_index=0; ll_index < iLLx_LIMIT; ll_index++)
					{
						if(xEXT_CH__LLx__IN_OUT_FLAG[ll_index]->Check__DATA(STR__OUT) < 0)
						{
							continue;
						}

						if(LLx__Is_Available(ll_index) < 0)			continue;
						if(LLx__Is_VAC(ll_index) < 0)				continue;

						// ...
						int ll__out_slot;

						int r_flag = LLx__Get_Empty__OutSlot(ll_index, ll__out_slot);
						if(r_flag < 0)			continue;

						para__ll_name = Get__LLx_NAME(ll_index);
						para__out_slot.Format("%1d", ll__out_slot);
						para__in_slot = "";
						break;
					}
				}

				if(para__ll_name.GetLength() > 0)
				{
					break;
				}
			}
		}

		if(para__ll_name == "")
		{
			NEXT__LOOP;
		}
	}

	// RB -> LL_OUT & LL_IN ...
	{
		CStringArray l__arm_type;
		VAC_RB__Get_Occupied__Arm_Type(l__arm_type);

		int k_limit = l__arm_type.GetSize();
		for(int k=0; k < k_limit; k++)
		{
			CString arm_type = l__arm_type[k];

			if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(arm_type) > 0)
			{
				continue;
			}

			// ...
			bool active__ll_exchange = false;
			CString empty_arm;

			if(para__in_slot.GetLength() > 0)
			{
				if(VAC_RB__Get_Empty__Arm_Type_From_LBx(empty_arm) > 0)
				{
					active__ll_exchange = true;

					if(SCH__Check_In_Slot_Of_Ohter_LLx(para__ll_name, para__in_slot) > 0)
					{
						active__ll_exchange = false;
					}

					/*
					// Check : Log ...
					{
						CStringArray l__sch_name;
						CStringArray l__ll_name;
						CStringArray l__ll_slot;

						_GET__WAFER_LIST_OF_LLx_IN__ALL_MODE(l__sch_name, l__ll_name,l__ll_slot, true);
					}
					*/
				}
			}

			// RB -> LL_OUT ...
			{
				CString sch_name;
				sch_name.Format("%s-%s", para__ll_name, para__out_slot);	

				int act_flag = SCH__PLACE_MODULE(p_variable,p_alarm, log_id, active__ll_exchange, arm_type, para__ll_name,para__out_slot, sch_name);
				int move_flag = 1;

				if(act_flag < 0)
				{
					move_flag = SCH__Check_Material_Place(arm_type);
				}

				if(move_flag > 0)
				{
					xSCH_MATERIAL_CTRL->Place__To_MODULE(arm_type, sch_name);
				}

				if(act_flag < 0)
				{
					NEXT__LOOP;
				}
			}

			// LL_IN -> RB ...
			if(active__ll_exchange)
			{
				CString sch_name;
				sch_name.Format("%s-%s", para__ll_name, para__in_slot);	

				int act_flag = SCH__PICK_MODULE(p_variable,p_alarm, log_id, false, empty_arm, para__ll_name,para__in_slot, sch_name);
				int move_flag = 1;

				if(act_flag < 0)
				{
					move_flag = SCH__Check_Material_Pick(empty_arm);
				}

				if(move_flag > 0)
				{
					xSCH_MATERIAL_CTRL->Pick__From_MODULE(sch_name, empty_arm);
				}

				if(act_flag < 0)
				{
					NEXT__LOOP;
				}
			}

			// LLx VENT ...
			{
				int ll_i = Get__LLx_INDEX(para__ll_name);
				if(ll_i >= 0)
				{
					bool active__ll_vac_check = true;

					// jglee : 2023.07.11
					if(LLx__Check_Occupied__InSlot(ll_i) > 0)
					{
						active__ll_vac_check = false;
					}

					// ...
					int count__ll_out_empty = 0;
					int ref_ll_i = ll_i; 

					// ...
					{
						for(int ll_i=0; ll_i<iLLx_LIMIT; ll_i++)
						{
							if(ref_ll_i == ll_i)		
							{
								continue;
							}

							if(active__ll_vac_check)
							{
								if(LLx__Is_Standby_CTCInUse(ll_i) < 0)	
								{
									continue;
								}

								if(xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Check__DATA(STR__OUT) < 0)
								{
									continue;
								}

								if(LLx__Is_VAC(ll_i) < 0)					continue;
								if(LLx__Check_Empty__OutSlot(ll_i) < 0)		continue;

								if((dEXT_CH__LLx__ACTIVE_ATM_AREA_CTRL[ll_i]->Check__DATA(STR__ON) > 0)
								|| (dEXT_CH__LLx__ACTIVE_VAC_AREA_CTRL[ll_i]->Check__DATA(STR__ON) > 0))
								{
									continue;
								}
							}
							else
							{
								if(LLx__Is_Standby_CTCInUse(ll_i) < 0)	
								{
									continue;
								}

								if(LLx__Check_Empty__OutSlot(ll_i) > 0)
								{
									NEXT__LOOP;
								}

								if(xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Check__DATA(STR__IN) > 0)
								{
									NEXT__LOOP;
								}

								continue;
							}

							count__ll_out_empty++;
						}
					}

					if(LLx__Check_Occupied__OutSlot(ll_i) > 0)
					{	
						bool active__ll_vent = true;

						if(count__ll_out_empty > 0)
						{
							int ll_in_slot;

							if(LLx__Get_Occupied__InSlot(ll_i, ll_in_slot) > 0)
							{
								CString ll_name = Get__LLx_NAME(ll_i);

								if(_CHECK__LBi_To_RB_With_Ready_PMC__ALL_MODE(ll_name, ll_in_slot, true) > 0)
								{
									active__ll_vent = false;
								}
							}
						}

						if(active__ll_vent)
						{
							CString ll_name = Get__LLx_NAME(ll_i);

							printf(" * AUTO_CTRL__RB_LBo__ALL_MODE() : %s-Venting !!! \n", ll_name);

							if(SCH_RUN__LLx_VENT(ll_i, log_id, "201") > 0)
							{
								xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Set__DATA(STR__IN);
							}
						}
					}
				}
			}

			NEXT__LOOP;
		}
	}

	return 1;
}

void CObj__DUAL_ARM_STD::AUTO_CTRL__FRONT_WAC_POS(CII_OBJECT__VARIABLE *p_variable)
{
	DECLARE__EXT_CTRL(p_variable);

	if(dCH__INFO_WAC_FRONT_ACTIVE->Check__DATA(STR__ON) > 0)
	{
		NEXT__LOOP;
	}

	if(VAC_RB__Check_All_Empty() > 0)
	{
		NEXT__LOOP;
	}

	// ...
	CString arm_type;

	//...
	{
		IDS__SCH_MATERIAL_STATUS ds_info;
		int pmi_flag = -1;
	
		if(pmi_flag < 0)
		{
			if(VAC_RB__Check_Occupied__A_Arm() > 0)
			{
				arm_type = _ARM__A;

				if(xSCH_MATERIAL_CTRL->Get__MATERIAL_STATUS(arm_type,ds_info) > 0)
				{
					if(ds_info.sMATERIAL_STS.CompareNoCase("ALIGNED") == 0)		pmi_flag = 1;
				}
			}
		}
		if(pmi_flag < 0)
		{
			if(VAC_RB__Check_Occupied__B_Arm() > 0)
			{
				arm_type = _ARM__B;
	
				if(xSCH_MATERIAL_CTRL->Get__MATERIAL_STATUS(arm_type,ds_info) > 0)
				{
					if(ds_info.sMATERIAL_STS.CompareNoCase("ALIGNED") == 0)		pmi_flag = 1;
				}	
			}
		}

		if(pmi_flag < 0)
		{
			NEXT__LOOP;
		}
	}

	// ...
	{
		CStringArray l_pmx;
		CStringArray l_rcp;

		if(xSCH_MATERIAL_CTRL->Get__CUR_PROCESS_INFO(arm_type,l_pmx,l_rcp) < 0)
		{
			NEXT__LOOP;
		}

		// ...
		CString cfg__wac_pos;

		int pm_index = -1;
		int material_check = -1;

		int limit = l_pmx.GetSize();
		int i;

		for(i=0; i<limit; i++)
		{
			pm_index = SCH__CHECK_PM_INDEX(l_pmx[i]);
			if(pm_index < 0)
			{
				continue;
			}

			if(dEXT_CH__SCH__WAC_PM_USE_X[pm_index]->Check__DATA(STR__YES) < 0)
			{
				continue;
			}
			if(dEXT_CH__SCH__WAC_PM_PRE_FRONT_X[pm_index]->Check__DATA(STR__YES) < 0)
			{
				continue;
			}

			if(PMx__Check_Empty__SlotStatus(pm_index) < 0)
			{
				if(PMx__Check_CtcInUse_Or_Standby(pm_index) > 0)
				{
					cfg__wac_pos = dEXT_CH__SCH__WAC_PM_POS_ID_X[pm_index]->Get__STRING();

					material_check = 1;
					break;
				}
			}

			if(PMx__Is_Available(pm_index) < 0)
			{
				continue;
			}			
			if(PMx__Check_Empty__SlotStatus(pm_index) < 0)
			{
				continue;
			}

			return;
		}
	
		if(material_check < 0)
		{
			return;
		}

		// ...
		{
			CString cur_arm = xCH__VAC_RB__ROTE_ARM->Get__STRING();
			CString cur_pos = xCH__VAC_RB__TRG_ROTE->Get__STRING();

			if(cur_arm.CompareNoCase(arm_type) == 0)
			{
				if(cfg__wac_pos.CompareNoCase("LLx") == 0)
				{
					if(cur_pos.Find(MODULE__LBA) == 0)			NEXT__LOOP;
					if(cur_pos.Find(MODULE__LBB) == 0)			NEXT__LOOP;
					if(cur_pos.Find(MODULE__LBC) == 0)			NEXT__LOOP;
					if(cur_pos.Find(MODULE__LBD) == 0)			NEXT__LOOP;
				}
				else if(cfg__wac_pos.CompareNoCase("LL1") == 0)
				{
					if(cur_pos.Find(MODULE__LBA) == 0)			NEXT__LOOP;
				}
				else if(cfg__wac_pos.CompareNoCase("LL2") == 0)
				{
					if(cur_pos.Find(MODULE__LBB) == 0)			NEXT__LOOP;
				}
				else if(cfg__wac_pos.CompareNoCase("LL3") == 0)
				{
					if(cur_pos.Find(MODULE__LBC) == 0)			NEXT__LOOP;
				}
				else if(cfg__wac_pos.CompareNoCase("LL4") == 0)
				{
					if(cur_pos.Find(MODULE__LBD) == 0)			NEXT__LOOP;
				}
			}
		}

		// ...
		{
			CString log_id = "AUTO_CTRL__FRONT_WAC_POS()";

			CStringArray l_para;
			CString pos_name;

			if(cfg__wac_pos.CompareNoCase("LLx") == 0)			pos_name = MODULE__LBB;
			else												pos_name = cfg__wac_pos;

			l_para.Add(arm_type);
			l_para.Add(pos_name);
			l_para.Add("1");

			Call__ROBOT_OBJ(CMMD__ROTATE, l_para, log_id);

			//
			CString log_msg;
			CString log_bff;

			log_msg = "AUTO_CTRL__FRONT_WAC_POS() \n";
			log_bff.Format(" * arm_type <- %s \n", arm_type);
			log_msg += log_bff;
			log_bff.Format(" * pos_name <- %s \n", pos_name);
			log_msg += log_bff;

			IDS__SCH_MATERIAL_STATUS ds_info;
			if(xSCH_MATERIAL_CTRL->Get__MATERIAL_STATUS(arm_type,ds_info) > 0)
			{
				log_bff.Format(" * wafer_info. ... \n");
				log_msg += log_bff;
	
				log_bff.Format("  ** P%1d:%1d (%s) \n",
								ds_info.iPORT_ID,
								ds_info.iSLOT_ID,
								ds_info.sMATERIAL_STS);
				log_msg += log_bff;
			}

			printf(log_msg);
		}
	}
}
void CObj__DUAL_ARM_STD::AUTO_CTRL__FRONT_PMo(CII_OBJECT__VARIABLE *p_variable)
{
	DECLARE__EXT_CTRL(p_variable);

	if(iActive__FROMT_PMo > 0)
	{
		NEXT__LOOP;
	}

	if(dCH__INFO_WAC_FRONT_ACTIVE->Check__DATA(STR__ON) > 0)
	{
		NEXT__LOOP;
	}

	if(SCH__LBi__CHECK_InSLOT_OCCUPIED() > 0)
	{
		NEXT__LOOP;
	}
	
	if(VAC_RB__Check_All_Empty() < 0)
	{
		NEXT__LOOP;
	}

	// ...
	CString arm_type;

	if(VAC_RB__Get_Empty__Arm_Type_From_PMx(arm_type) < 0)
	{
		NEXT__LOOP;
	}

	// ...
	{
		CString para_pm;
		int i;

		for(i=0;i<iPMx_SIZE;i++)
		{
			if(xCH__PMx__PROCESS_STATUS[i]->Check__DATA("WAIT") < 0)
			{
				continue;
			}

			para_pm.Format("PM%1d",i+1);
			break;
		}
	
		if(para_pm.GetLength() < 1)
		{
			NEXT__LOOP;
		}
		
		if((xCH__VAC_RB__ROTE_ARM->Check__DATA(arm_type) > 0)
		&& (xCH__VAC_RB__TRG_ROTE->Check__DATA(para_pm)  > 0))
		{
			NEXT__LOOP;
		}

		// ...
		{
			CString log_id = "AUTO_CTRL__FRONT_PMo()";

			//
			CStringArray l_para;

			l_para.Add(arm_type);
			l_para.Add(para_pm);
			l_para.Add("1");

			Call__ROBOT_OBJ(CMMD__ROTATE, l_para, log_id);
			
			iActive__FROMT_PMo = 1;

			//
			CString log_msg;
			CString log_bff;

			log_msg = "AUTO_CTRL__FRONT_PMo() \n";
			log_bff.Format(" * arm_type <- %s \n", arm_type);
			log_msg += log_bff;
			log_bff.Format(" * para_pm <- %s \n", para_pm);
			log_msg += log_bff;

			printf(log_msg);
		}
	}
}

void CObj__DUAL_ARM_STD::
AUTO_CTRL__LLx_Exception_In_Idle_State(CII_OBJECT__VARIABLE *p_variable, CII_OBJECT__ALARM *p_alarm)
{
	DECLARE__EXT_CTRL(p_variable);

	CString log_id = "AUTO_CTRL__LLx_Exception_In_Idle_State()";

	// ...
	{
		int material_count = 0;
		int i;

		// PMx
		for(i=0;i<iPMx_SIZE;i++)
		{
			if((PMx__Check_CtcInUse_Or_Standby(i) > 0)
			&& (PMx__Check_Empty__SlotStatus(i)   < 0))
			{
				material_count++;
			}
		}	
		
		// Arm : A & B
		if(VAC_RB__Check_Occupied__A_Arm() > 0)
		{
			material_count++;
		}
		if(VAC_RB__Check_Occupied__B_Arm() > 0)
		{
			material_count++;
		}
		
		if(material_count < 1)
		{
			int ll_limit = iLLx_LIMIT;
			int ll_i;

			for(ll_i=0; ll_i<ll_limit; ll_i++)
			{
				if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_i]->Check__DATA(LBx_MODE__ONLY_INPUT) > 0)
				{
			
				}
				else
				{
					int i_count = 0;

					if((dEXT_CH__LLx__ACTIVE_ATM_AREA_CTRL[ll_i]->Check__DATA(STR__ON) > 0)
					|| (dEXT_CH__LLx__ACTIVE_VAC_AREA_CTRL[ll_i]->Check__DATA(STR__ON) > 0))
					{
						xEXT_CH__LLx__IDLE_COUNT[ll_i]->Set__DATA("0");
					}
					else
					{
						CString ch_data = xEXT_CH__LLx__IDLE_COUNT[ll_i]->Get__STRING();
						i_count = atoi(ch_data);
					}

					if(i_count >= 10)
					{
						if(xEXT_CH__CFG__TRANSFER_MODE->Check__DATA(STR__ATM) > 0)
						{
							if((LLx__Is_Available(ll_i) > 0)
							&& (LLx__Is_ATM(ll_i) < 0)
							&& (LLx__Check_All_Empty(ll_i) > 0))
							{
								if(xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Check__DATA(STR__IN) < 0)
								{
									if(SCH_RUN__LLx_VENT(ll_i, log_id, "1") > 0)
									{
										xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Set__DATA(STR__IN);
									}
								}
							}
						}
						else
						{
							if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_i]->Check__DATA(LBx_MODE__ONLY_OUTPUT) > 0)
							{
								if((LLx__Is_Available(ll_i) > 0)
								&& (LLx__Is_VAC(ll_i) < 0)
								&& (LLx__Check_All_Empty(ll_i) > 0))
								{
									if(SCH_RUN__LLx_PUMP(ll_i, log_id, "2") > 0)
									{
										xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Set__DATA(STR__IN);
									}
								}
							}
							else
							{
								if((LLx__Is_Available(ll_i) > 0)
								&& (LLx__Is_ATM(ll_i) < 0)
								&& (LLx__Check_All_Empty(ll_i) > 0))
								{
									if(xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Check__DATA(STR__IN) < 0)
									{
										if(SCH_RUN__LLx_VENT(ll_i, log_id, "3") > 0)
										{
											xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Set__DATA(STR__IN);
										}
									}
								}
							}
						}
					}
				}

				// ...
			}
		}
		else if(material_count < 2)
		{
			int lb_count = 0;

			int ll_limit = iLLx_LIMIT;
			int ll_i;

			for(ll_i=0; ll_i<ll_limit; ll_i++)
			{
				if((LLx__Is_Available(ll_i) > 0)
				&& (LLx__Is_ATM(ll_i) < 0)
				&& (LLx__Check_All_Empty(ll_i) > 0))
				{
					lb_count++;
				}
			}

			if(lb_count > 1)
			{
				int lb_check = -1;

				for(ll_i=0; ll_i<ll_limit; ll_i++)
				{
					if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_i]->Check__DATA(LBx_MODE__ONLY_INPUT) > 0)
					{

					}
					else if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_i]->Check__DATA(LBx_MODE__ONLY_OUTPUT) > 0)
					{
						int i_count = 0;

						if((dEXT_CH__LLx__ACTIVE_ATM_AREA_CTRL[ll_i]->Check__DATA(STR__ON) > 0)
						|| (dEXT_CH__LLx__ACTIVE_VAC_AREA_CTRL[ll_i]->Check__DATA(STR__ON) > 0))
						{
							xEXT_CH__LLx__IDLE_COUNT[ll_i]->Set__DATA("0");
						}
						else							
						{
							CString ch_data = xEXT_CH__LLx__IDLE_COUNT[ll_i]->Get__STRING();
							i_count = atoi(ch_data);
						}

						if(i_count >= 10)
						{
							if(xEXT_CH__CFG__TRANSFER_MODE->Check__DATA(STR__ATM) > 0)
							{
								if(lb_check < 0)
								{
									if((LLx__Is_Available(ll_i) > 0)
									&& (LLx__Is_ATM(ll_i) < 0)
									&& (LLx__Check_All_Empty(ll_i) > 0))
									{
										if(SCH_RUN__LLx_VENT(ll_i, log_id, "21") > 0)
										{
											xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Set__DATA(STR__IN);
										}

										lb_check = 1;
									}
								}
							}
							else
							{
								if(lb_check < 0)
								{
									if((LLx__Is_Available(ll_i) > 0)
									&& (LLx__Is_VAC(ll_i) < 0)
									&& (LLx__Check_All_Empty(ll_i) > 0))
									{
										if(SCH_RUN__LLx_PUMP(ll_i, log_id, "22") > 0)
										{
											xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Set__DATA(STR__IN);
										}

										lb_check = 1;
									}
								}
							}
						}
					}
				}

				// ...
			}
		}
	}

	// ...
	{
		int ll_limit = iLLx_LIMIT;
		int ll_i;

		for(ll_i=0; ll_i<ll_limit; ll_i++)
		{
			if(LLx__Is_Available(ll_i) > 0)
			{
				if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_i]->Check__DATA(LBx_MODE__ONLY_INPUT) > 0)
				{

				}
				else if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_i]->Check__DATA(LBx_MODE__ONLY_OUTPUT) > 0)
				{

				}
				else
				{
					CString sch_name;
					int lb_slot;

					if((LLx__Is_ATM(ll_i) < 0)
					&& (LLx__Check_Empty__InSlot(ll_i) > 0)
					&& (LLx__Get_Occupied__OutSlot(ll_i, lb_slot) > 0))
					{
						CString ll_name = Get__LLx_NAME(ll_i);
						sch_name.Format("%s-%1d", ll_name,lb_slot);

						// ...
						IDS__SCH_MATERIAL_STATUS ds_sts;
						
						if(xSCH_MATERIAL_CTRL->Get__MATERIAL_STATUS(sch_name,ds_sts) > 0)
						{
							int port_id = ds_sts.iPORT_ID;
							int pump_req_flag = -1;

							if(xSCH_MATERIAL_CTRL->Check__MATERIAL_TO_PROCESS_WITH_PMC_IN_LPx(port_id) < 0)
							{
								pump_req_flag = 1;
							}
							else
							{
								int lb_in_slot;
								
								if(LLx__Get_Occupied__InSlot(ll_i, lb_in_slot) > 0)
								{
									CString sch_lb_name;
									sch_lb_name.Format("%s-%1d", ll_name,lb_in_slot);

									if(SCH__LBx_CAN_VENT(sch_lb_name) > 0)
									{
										pump_req_flag = 1;
									}
								}
							}

							if(pump_req_flag > 0)
							{
								if(SCH_RUN__LLx_VENT(ll_i, log_id, "41") > 0)
								{
									xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Set__DATA("IN");
								}
							}
						}

						// ...
					}
				}
			}
		}

		// ...
	}

	// ...
}

void CObj__DUAL_ARM_STD::
AUTO_CTRL__LLx_Exception_Of_Only_Mode(CII_OBJECT__VARIABLE *p_variable, CII_OBJECT__ALARM *p_alarm)
{
	DECLARE__EXT_CTRL(p_variable);

	// ...
	CString log_id = "AUTO_CTRL__LLx_Exception_Of_Only_Mode()";
	
	if(VAC_RB__Is_Available() < 0)
	{
		NEXT__LOOP;
	}

	// LLx - Check ...
	{
		int check_flag = -1;

		int ll_limit = iLLx_LIMIT;
		int ll_i;

		for(ll_i=0; ll_i<ll_limit; ll_i++)
		{
			if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_i]->Check__DATA(LBx_MODE__ONLY_OUTPUT) > 0)
			{
				if(LLx__Check_Occupied__OutSlot(ll_i) > 0)
				{
					if(SCH__CHECK_MATERIAL_CLEAR(true) > 0)
					{
						if((LLx__Is_ATM(ll_i) < 0)
						&& (xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Check__DATA("OUT") < 0))
						{
							SCH_RUN__LLx_VENT(ll_i, log_id, "13.1");

							xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Set__DATA("OUT");
							continue;
						}
					}
				}
				
				if(LLx__Check_All_Empty(ll_i) > 0)
				{
					if(xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Check__DATA(STR__IN) < 0)		continue;
					if(LLx__Is_Available(ll_i) < 0)			continue;

					if(xEXT_CH__CFG__TRANSFER_MODE->Check__DATA(STR__VAC) > 0)
					{
						if(LLx__Is_VAC(ll_i) > 0)				continue;

						SCH_RUN__LLx_PUMP(ll_i, log_id, "12.1");
						continue;
					}
					else if(xEXT_CH__CFG__TRANSFER_MODE->Check__DATA(STR__ATM) > 0)
					{
						if(LLx__Is_ATM(ll_i) > 0)				continue;

						SCH_RUN__LLx_VENT(ll_i, log_id, "12.2");
						continue;
					}
				}

				// ...
				CString ll_name = Get__LLx_NAME(ll_i);
				int ll_slot = LLx__Check_Occupied__OutSlot(ll_i);

				CString sch_name;
				sch_name.Format("%s-%1d", ll_name,ll_slot);

				if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(sch_name) > 0)
				{
					continue;
				}

				// ...
				check_flag = 1;

				if(LLx__Is_Available(ll_i) > 0)
				{
					if(LLx__Check_Empty__OutSlot(ll_i) < 0)
					{
						int i_count = 0;
						
						if((dEXT_CH__LLx__ACTIVE_ATM_AREA_CTRL[ll_i]->Check__DATA(STR__ON) > 0)
						|| (dEXT_CH__LLx__ACTIVE_VAC_AREA_CTRL[ll_i]->Check__DATA(STR__ON) > 0))
						{
							xEXT_CH__LLx__IDLE_COUNT[ll_i]->Set__DATA("0");
						}
						else
						{
							CString ch_data = xEXT_CH__LLx__IDLE_COUNT[ll_i]->Get__STRING();
							i_count = atoi(ch_data);
						}

						if(i_count >= 10)
						{
							if(LLx__Is_ATM(ll_i) < 0)
							{
								SCH_RUN__LLx_VENT(ll_i, log_id, "1");

								xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Set__DATA(STR__OUT);
							}
						}
					}
				}
			}
		}

		for(ll_i=0; ll_i<ll_limit; ll_i++)
		{
			if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_i]->Check__DATA(LBx_MODE__ONLY_INPUT) > 0)
			{
				check_flag = 1;

				if((LLx__Check_Empty__InSlot(ll_i) < 0)
				&& (LLx__Is_Available(ll_i) > 0))
				{
					int i_count = 0;

					if((dEXT_CH__LLx__ACTIVE_ATM_AREA_CTRL[ll_i]->Check__DATA(STR__ON) > 0)
					|| (dEXT_CH__LLx__ACTIVE_VAC_AREA_CTRL[ll_i]->Check__DATA(STR__ON) > 0))
					{
						xEXT_CH__LLx__IDLE_COUNT[ll_i]->Set__DATA("0");
					}
					else
					{
						CString ch_data = xEXT_CH__LLx__IDLE_COUNT[ll_i]->Get__STRING();
						i_count = atoi(ch_data);
					}

					if(i_count >= 10)
					{
						if(xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Check__DATA(STR__OUT) > 0)
						{
							if(xEXT_CH__CFG__TRANSFER_MODE->Check__DATA(STR__VAC) > 0)
							{
								if(LLx__Is_VAC(ll_i) < 0)
								{	
									SCH_RUN__LLx_PUMP(ll_i, log_id, "11");
								}
							}
						}
						else
						{
							if(xEXT_CH__CFG__TRANSFER_MODE->Check__DATA(STR__VAC) > 0)
							{
								if(LLx__Is_VAC(ll_i) > 0)		xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Set__DATA(STR__OUT);
							}
						}
					}
				}
			}
		}

		if(check_flag > 0)
		{
			NEXT__LOOP;
		}
	}
	
	// ...
}

void CObj__DUAL_ARM_STD::
AUTO_CTRL__LLx_Exception_Of_All_Mode(CII_OBJECT__VARIABLE *p_variable, CII_OBJECT__ALARM *p_alarm)
{
	DECLARE__EXT_CTRL(p_variable);

	// ...
	CString log_id = "AUTO_CTRL__LLx_Exception_Of_All_Mode()";

	if(VAC_RB__Is_Available() < 0)
	{
		NEXT__LOOP;
	}

	// Check : LLx (All_Mode) ...
	{
		int ll_limit = iLLx_LIMIT;

		for(INT ll_i=0; ll_i<ll_limit; ll_i++)
		{
			if((dEXT_CH__LLx__ACTIVE_ATM_AREA_CTRL[ll_i]->Check__DATA(STR__ON) > 0)
			|| (dEXT_CH__LLx__ACTIVE_VAC_AREA_CTRL[ll_i]->Check__DATA(STR__ON) > 0))
			{
				NEXT__LOOP;
			}
		}

		for(INT ll_i=0; ll_i<ll_limit; ll_i++)
		{
			if(LLx__Is_Available(ll_i) < 0)
			{
				continue;
			}

			if((dEXT_CH__LLx__ACTIVE_ATM_AREA_CTRL[ll_i]->Check__DATA(STR__ON) > 0)
			|| (dEXT_CH__LLx__ACTIVE_VAC_AREA_CTRL[ll_i]->Check__DATA(STR__ON) > 0))
			{
				continue;
			}

			// ...
			CString obj_sts = xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_i]->Get__STRING();

			if(obj_sts.CompareNoCase(LBx_MODE__ONLY_INPUT)  == 0)		continue;
			if(obj_sts.CompareNoCase(LBx_MODE__ONLY_OUTPUT) == 0)		continue;

			if(LLx__Is_ATM(ll_i) > 0)
			{
				if(xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Check__DATA(STR__OUT) > 0)
				{
					if((LLx__Check_Empty__InSlot(ll_i)     < 0)
					&& (LLx__Check_Occupied__OutSlot(ll_i) > 0))
					{
						xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Set__DATA(STR__IN);
					}
				}

				if(xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Check__DATA(STR__OUT) > 0)
				{
					if((LLx__Check_Empty__InSlot(ll_i)  < 0)
					&& (LLx__Check_Empty__OutSlot(ll_i) > 0))
					{
						SCH_RUN__LLx_PUMP(ll_i, log_id, "31");
					}
				}
			}
			else
			{
				if(xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Check__DATA(STR__IN) < 0)
				{
					if((LLx__Check_Empty__InSlot(ll_i)     > 0)
					&& (LLx__Check_Occupied__OutSlot(ll_i) > 0))
					{
						SCH_RUN__LLx_VENT(ll_i, log_id, "32");

						xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Set__DATA(STR__IN);
					}
				}
			}
		}

		// ...
	}

	// Check : LLx (All_Mode) ...
	{
		bool active__ll_out_check = false;

		if(VAC_RB__Is_Available() > 0)
		{
			if(VAC_RB__Check_Occupied__Arm_Type() > 0)
			{
				active__ll_out_check = true;
			}
		}

		if(active__ll_out_check)
		{
			int count__ll_out_empty = 0;

			// ...
			{
				int ll_limit = iLLx_LIMIT;
			
				for(INT ll_i=0; ll_i<ll_limit; ll_i++)
				{
					if((dEXT_CH__LLx__ACTIVE_ATM_AREA_CTRL[ll_i]->Check__DATA(STR__ON) > 0)
					|| (dEXT_CH__LLx__ACTIVE_VAC_AREA_CTRL[ll_i]->Check__DATA(STR__ON) > 0))
					{
						continue;
					}

					if((xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_i]->Check__DATA(LBx_MODE__ONLY_INPUT) > 0)
					|| xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_i]->Check__DATA(LBx_MODE__ONLY_OUTPUT) > 0)
					{
						continue;
					}

					if(LLx__Is_Standby_CTCInUse(ll_i) < 0)
					{
						continue;
					}

					if(xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Check__DATA(STR__IN) > 0)
					{

					}
					else
					{
						if(LLx__Check_Empty__OutSlot(ll_i) < 0)			continue;
					}
		
					count__ll_out_empty++;
				}
			}

			if((count__ll_out_empty == 0)
			&& (xSCH_MATERIAL_CTRL->Check__MATERIAL_TO_PROCESS() < 0))
			{
				int count__ll_out = 0;

				for(INT ll_i=0; ll_i<iLLx_LIMIT; ll_i++)
				{
					if((dEXT_CH__LLx__ACTIVE_ATM_AREA_CTRL[ll_i]->Check__DATA(STR__ON) > 0)
					|| (dEXT_CH__LLx__ACTIVE_VAC_AREA_CTRL[ll_i]->Check__DATA(STR__ON) > 0))
					{
						continue;
					}

					if((xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_i]->Check__DATA(LBx_MODE__ONLY_INPUT)  > 0)
					|| (xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_i]->Check__DATA(LBx_MODE__ONLY_OUTPUT) > 0))
					{
						continue;
					}

					if(xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Check__DATA(STR__OUT) < 0)
					{
						continue;
					}

					count__ll_out++;
				}

				if(count__ll_out < 1)
				{
					for(INT ll_i=0; ll_i<iLLx_LIMIT; ll_i++)
					{
						if(LLx__Is_Available(ll_i) < 0)
						{
							continue;
						}

						if((dEXT_CH__LLx__ACTIVE_ATM_AREA_CTRL[ll_i]->Check__DATA(STR__ON) > 0)
						|| (dEXT_CH__LLx__ACTIVE_VAC_AREA_CTRL[ll_i]->Check__DATA(STR__ON) > 0))
						{
							continue;
						}

						if((xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_i]->Check__DATA(LBx_MODE__ONLY_INPUT)  > 0)
						|| (xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_i]->Check__DATA(LBx_MODE__ONLY_OUTPUT) > 0))
						{
							continue;
						}

						if(xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Check__DATA(STR__OUT) > 0)
						{
							continue;
						}

						if(LLx__Is_ATM(ll_i) < 0)						continue;
						if(LLx__Check_Empty__InSlot(ll_i) < 0)			continue;
						if(LLx__Check_Empty__OutSlot(ll_i) < 0)			continue;

						// ...
						{
							SCH_RUN__LLx_PUMP(ll_i, log_id, "101");

							xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Set__DATA(STR__OUT);
						}

						NEXT__LOOP;
					}
				}
			}
			
			// jglee : 2023.07.11
			if(count__ll_out_empty < 1)
			{
				int ll_limit = iLLx_LIMIT;

				for(INT ll_i=0; ll_i<ll_limit; ll_i++)
				{
					if(LLx__Is_Available(ll_i) < 0)
					{
						continue;
					}

					if((dEXT_CH__LLx__ACTIVE_ATM_AREA_CTRL[ll_i]->Check__DATA(STR__ON) > 0)
					|| (dEXT_CH__LLx__ACTIVE_VAC_AREA_CTRL[ll_i]->Check__DATA(STR__ON) > 0))
					{
						continue;
					}

					if((xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_i]->Check__DATA(LBx_MODE__ONLY_INPUT)  > 0)
					|| (xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_i]->Check__DATA(LBx_MODE__ONLY_OUTPUT) > 0))
					{
						continue;
					}

					if(xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Check__DATA(STR__OUT) < 0)
					{
						continue;
					}

					if(LLx__Is_VAC(ll_i) < 0)						continue;

					if(LLx__Check_Empty__OutSlot(ll_i) > 0)			continue;
					if(LLx__Check_Occupied__OutSlot(ll_i) < 0)		continue;

					// ...
					{
						int ll_slot_id;

						if(LLx__Get_Occupied__OutSlot(ll_i, ll_slot_id) > 0)
						{
							CString str__ll_name;
							CString str__ll_slot;

							str__ll_name = Get__LLx_NAME(ll_i);
							str__ll_slot.Format("%1d", ll_slot_id);

							int r_flag = _CHECK__FIRST_WAFER_OF_LLx_IN__ALL_MODE(str__ll_name, str__ll_slot);
							if(r_flag > 0)			continue;
						}
					}

					// ...
					{
						CString log_msg;
						CString log_bff;

						log_msg  = "\n";
						log_msg += "LLx Info ... \n";

						int ll_limit = iLLx_LIMIT;
						for(INT i=0; i<ll_limit; i++)
						{
							int id = i + 1;

							log_bff.Format("LL%1d \n", id);
							log_msg += log_bff;

							log_bff.Format("  * %s <- %s \n",
											dEXT_CH__LLx__ACTIVE_ATM_AREA_CTRL[i]->Get__CHANNEL_NAME(),
											dEXT_CH__LLx__ACTIVE_ATM_AREA_CTRL[i]->Get__STRING());
							log_msg += log_bff;

							log_bff.Format("  * %s <- %s \n",
											dEXT_CH__LLx__ACTIVE_VAC_AREA_CTRL[i]->Get__CHANNEL_NAME(),
											dEXT_CH__LLx__ACTIVE_VAC_AREA_CTRL[i]->Get__STRING());
							log_msg += log_bff;

							log_bff.Format("  * %s <- %s \n",
											xEXT_CH__SCH_DB_LLx_MODE_TYPE[i]->Get__CHANNEL_NAME(),
											xEXT_CH__SCH_DB_LLx_MODE_TYPE[i]->Get__STRING());
							log_msg += log_bff;

							log_bff.Format("  * LLx__Is_Idle() <- %1d \n",
											LLx__Is_Idle(i));
							log_msg += log_bff;

							log_bff.Format("  * %s <- %s \n",
											xEXT_CH__LLx__IN_OUT_FLAG[i]->Get__CHANNEL_NAME(),
											xEXT_CH__LLx__IN_OUT_FLAG[i]->Get__STRING());
							log_msg += log_bff;

							log_bff.Format("  * LLx__Is_VAC() <- %1d \n",
											LLx__Is_VAC(i));
							log_msg += log_bff;

							log_bff.Format("  * LLx__Check_Empty__OutSlot() <- %1d \n",
											LLx__Check_Empty__OutSlot(i));
							log_msg += log_bff;
						}

						Fnc__LOG_CTRL(log_msg);
					}

					// ...
					{
						SCH_RUN__LLx_VENT(ll_i, log_id, "102");

						xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Set__DATA(STR__IN);
					}

					NEXT__LOOP;
				}
			}

			// ...
		}
	}

	// ...
}

void CObj__DUAL_ARM_STD::AUTO_CTRL__Deadlock_Report(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm)
{
	if(VAC_RB__Is_Available() < 0)
	{
		NEXT__LOOP;
	}

	// PMx ...
	bool active__pm_deadlock = false;

	CString err_msg;
	CString err_bff;

	for(int pm_i=0; pm_i<iPMx_SIZE; pm_i++)
	{
		if(PMx__Is_Available(pm_i) < 0)					continue;
		if(PMx__Check_Empty__SlotStatus(pm_i) > 0)		continue;

		// ...
		CString sch_name;
		CStringArray l__pm_name;
		CStringArray l__pm_rcp;

		sch_name.Format("PM%1d-1", pm_i + 1);
		
		int r_flag = xSCH_MATERIAL_CTRL->Get__NEXT_PROCESS_INFO(sch_name, l__pm_name, l__pm_rcp);
		if(r_flag < 0)			continue;

		// ...
		bool active__pm_error = true;

		int k_limit = l__pm_name.GetSize();
		for(int k=0; k<k_limit; k++)
		{
			int pm_index = Macro__Get_PMC_INDEX(l__pm_name[k]);
			
			if((pm_index < 0)
			|| (pm_index >= iLPx_SIZE)
			|| (pm_i == pm_index))
			{
				continue;
			}

			if(PMx__Check_CtcInUse_Or_Standby(pm_index) < 0)
			{
				continue;
			}
			
			active__pm_error = false;
			break;
		}

		if(active__pm_error)
		{
			active__pm_deadlock = true;

			err_msg.Format("Wafer(%s) in PM%1d. \n", 
						   xCH__PMx__SLOT_TITLE[pm_i]->Get__STRING(),
						   pm_i+1);
			err_msg += "\n";

			err_msg += "Next process module is as follows; \n";

			int k_limit = l__pm_name.GetSize();
			for(int k=0; k<k_limit; k++)
			{
				err_bff.Format(" %1d) %s \n", k+1, l__pm_name[k]);
				err_msg += err_bff;
			}
			break;
		}
	}	

	if(active__pm_deadlock)
	{
		int alarm_id = ALID__PMx_DEADLOCK_CONDITION;
		CString r_act;

		p_alarm->Check__ALARM(alarm_id, r_act);
		p_alarm->Post__ALARM_With_MESSAGE(alarm_id, err_msg);
	}
}

int  CObj__DUAL_ARM_STD::SCH__LBx_CAN_VENT(const CString& sch_lbx_name)
{
	// ...
	CStringArray l_pmc_name;
	CStringArray l_pmc_rcp;
	
	xSCH_MATERIAL_CTRL->Get__CUR_PROCESS_INFO(sch_lbx_name,l_pmc_name,l_pmc_rcp);
	
	// ...
	int flag__lp_normal = 1;
	
	if(LPx__Get_Process_Type__Normal(sch_lbx_name,flag__lp_normal) < 0)
	{
		return -1;
	}

	// ...
	int pm_i;
	
	int limit = l_pmc_name.GetSize();
	int i;
	
	for(i=0;i<limit;i++)
	{
		pm_i = SCH__CHECK_PM_INDEX(l_pmc_name[i]);
		if(pm_i < 0)
		{
			continue;
		}
		
		if(Fnc__PMx_Empty_Ready_Check(flag__lp_normal,pm_i,0,1) > 0)
		{
			return -1;
		}
	}
	return 1;
}

int CObj__DUAL_ARM_STD
::Check_Alarm__Scheduler_Config(CII_OBJECT__ALARM* p_alarm)
{
	// VAC Robot - Arm Check ...
	{
		// A & B Arm ...
		if((xCH__VAC_RB__CFG_A_ARM_USE_MODE->Check__DATA(STR__ENABLE) > 0)
		&& (xCH__VAC_RB__CFG_B_ARM_USE_MODE->Check__DATA(STR__ENABLE) > 0))
		{			
			int err_flag = -1;
			CString ch_data;

			if(err_flag > 0)
			{
				/*
				int alarm_id = ALID__PICK_ARM__CONFIG_ERROR;
				CString alm_msg;
				CString alm_bff;
				CString r_act;
				*/
			}	
		}

		// No Arm ...
		else if((xCH__VAC_RB__CFG_A_ARM_USE_MODE->Check__DATA(STR__ENABLE) < 0)
			 && (xCH__VAC_RB__CFG_B_ARM_USE_MODE->Check__DATA(STR__ENABLE) < 0))
		{
			CString ch_data;

			// ...
			{
				int alarm_id = ALID__NO_ARM__CONFIG_ERROR;
				CString alm_msg;
				CString alm_bff;
				CString r_act;

				//
				alm_msg  = "VAC Robot can not use any arm. \n";
				alm_msg += "Current ctc scheduler config setting \n";
				
				xCH__VAC_RB__CFG_A_ARM_USE_MODE->Get__DATA(ch_data);
				alm_bff.Format("   A Arm Use <- %s \n", ch_data);
				alm_msg += alm_bff;

				xCH__VAC_RB__CFG_B_ARM_USE_MODE->Get__DATA(ch_data);
				alm_bff.Format("   B Arm Use <- %s \n", ch_data);
				alm_msg += alm_bff;

				//
				p_alarm->Check__ALARM(alarm_id,r_act);
				p_alarm->Post__ALARM_With_MESSAGE(alarm_id,alm_msg);
			}	
		}
		
		// A Arm ...
		else if((xCH__VAC_RB__CFG_A_ARM_USE_MODE->Check__DATA(STR__ENABLE) > 0)
			 && (xCH__VAC_RB__CFG_B_ARM_USE_MODE->Check__DATA(STR__ENABLE) < 0))
		{
			int err_flag = -1;
			CString ch_data;

			if(err_flag < 0)
			{	
				xCH_CFG__PICK_ARM_TYPE_FROM_LBX->Get__DATA(ch_data);

				if((ch_data.CompareNoCase(STR__ALL) != 0)
				&& (ch_data.CompareNoCase(STR__A)   != 0))
				{
					err_flag = 1;
				}
			}
			if(err_flag < 0)
			{
				xCH_CFG__PICK_ARM_TYPE_FROM_PMC->Get__DATA(ch_data);

				if((ch_data.CompareNoCase(STR__ALL) != 0)
				&& (ch_data.CompareNoCase(STR__B)   != 0))
				{
					err_flag = 1;
				}
			}

			if(err_flag > 0)
			{
				int alarm_id = ALID__A_ARM__CONFIG_ERROR;
				CString alm_msg;
				CString alm_bff;
				CString r_act;

				//
				alm_msg = "Current VAC Robot's Arm Config state. \n";
							
				xCH__VAC_RB__CFG_A_ARM_USE_MODE->Get__DATA(ch_data);
				alm_bff.Format("   A Arm Use <- %s \n", ch_data);
				alm_msg += alm_bff;

				xCH_CFG__PICK_ARM_TYPE_FROM_LBX->Get__DATA(ch_data);
				alm_bff.Format("   Pick Arm Type From LBx <- %s \n", ch_data);
				alm_msg += alm_bff;

				xCH_CFG__PICK_ARM_TYPE_FROM_PMC->Get__DATA(ch_data);
				alm_bff.Format("   Pick Arm Type From PMC <- %s \n", ch_data);
				alm_msg += alm_bff;

				alm_msg += "If you use only A-Arm, \n";
				alm_msg += "   Pick Arm Type From LBx is \"ALL\" or \"A\". \n";
				alm_msg += "   Pick Arm Type From PMC is \"ALL\" or \"A\". \n";

				//
				p_alarm->Check__ALARM(alarm_id,r_act);
				p_alarm->Post__ALARM_With_MESSAGE(alarm_id,alm_msg);
			}
		}

		// B Arm ...
		else if((xCH__VAC_RB__CFG_A_ARM_USE_MODE->Check__DATA(STR__ENABLE) < 0)
			 && (xCH__VAC_RB__CFG_B_ARM_USE_MODE->Check__DATA(STR__ENABLE) > 0))
		{
			int err_flag = -1;
			CString ch_data;
						
			if(err_flag < 0)
			{	
				xCH_CFG__PICK_ARM_TYPE_FROM_LBX->Get__DATA(ch_data);
							
				if((ch_data.CompareNoCase(STR__ALL) != 0)
				&& (ch_data.CompareNoCase(STR__B)   != 0))
				{
					err_flag = 1;
				}
			}
			if(err_flag < 0)
			{
				xCH_CFG__PICK_ARM_TYPE_FROM_PMC->Get__DATA(ch_data);
							
				if((ch_data.CompareNoCase(STR__ALL) != 0)
				&& (ch_data.CompareNoCase(STR__B)   != 0))
				{
					err_flag = 1;
				}
			}
						
			if(err_flag > 0)
			{
				int alarm_id = ALID__B_ARM__CONFIG_ERROR;
				CString alm_msg;
				CString alm_bff;
				CString r_act;
							
				//
				alm_msg = "Current VAC Robot's Arm Config state. \n";
							
				xCH__VAC_RB__CFG_B_ARM_USE_MODE->Get__DATA(ch_data);
				alm_bff.Format("   B Arm Use <- %s \n", ch_data);
				alm_msg += alm_bff;
							
				xCH_CFG__PICK_ARM_TYPE_FROM_LBX->Get__DATA(ch_data);
				alm_bff.Format("   Pick Arm Type From LBx <- %s \n", ch_data);
				alm_msg += alm_bff;
							
				xCH_CFG__PICK_ARM_TYPE_FROM_PMC->Get__DATA(ch_data);
				alm_bff.Format("   Pick Arm Type From PMC <- %s \n", ch_data);
				alm_msg += alm_bff;
							
				alm_msg += "If you use only B-Arm, \n";
				alm_msg += "   Pick Arm Type From LBx is \"ALL\" or \"B\". \n";
				alm_msg += "   Pick Arm Type From PMC is \"ALL\" or \"B\". \n";
							
				//
				p_alarm->Check__ALARM(alarm_id,r_act);
				p_alarm->Post__ALARM_With_MESSAGE(alarm_id,alm_msg);							
			}
		}
	}	

	return 1;
}

