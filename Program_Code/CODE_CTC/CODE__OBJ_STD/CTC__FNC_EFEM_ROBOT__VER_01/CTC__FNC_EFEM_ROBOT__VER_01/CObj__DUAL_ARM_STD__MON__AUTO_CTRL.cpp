#include "StdAfx.h"
#include "CObj__DUAL_ARM_STD.h"

#include "Macro_Function.h"


// ...
#define STR__ROBOT_NAME			"ATM_RB1"


void CObj__DUAL_ARM_STD
::Mon__AUTO_CTRL(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm)
{
	// ...
	{
		xSCH_MATERIAL_CTRL->Link__SERVICE_DB("");
	
		xSCH_MATERIAL_CTRL->Define__ROBOT_NAME(STR__ROBOT_NAME);
		xSCH_MATERIAL_CTRL->Define__ROBOT_ARM(ARM_A);
		xSCH_MATERIAL_CTRL->Define__ROBOT_ARM(ARM_B);

		xSCH_MATERIAL_CTRL->Register__SYSTEM_NAME(sObject_Name);

		xSCH_MATERIAL_CTRL->JOB_CTRL__Disable_AUTO_PORT_COUNT();
	}

	iJOB_FLAG = -1;

	// ...
	CString ch_data;

	int loop_count = 0;
	int idle_flag = 0;
	int idle_count = -1;

	int ref__start_delay = 5000;
	int count__start_delay = 0;


	while(1)
	{
		while(xSCH_MATERIAL_CTRL->Get__MATERIAL_DB_SIZE() < 1)
		{
			if(idle_flag <= 0)
			{
				if(xSCH_MATERIAL_CTRL->Check__SYSTEM_ABORT() > 0)
				{
					//
				}
				else if(xSCH_MATERIAL_CTRL->Check__SYSTEM_STOP() > 0)
				{
					//
				}
				else
				{
					if(idle_flag < 0)
					{
						printf(" * Idle : ATM_Robot <- Home ! \n");
					
						SEQ__ATM_RB__HOME(1);
					}

					try
					{
						Start__ALL_LBx_To_IDLE_STATE(p_variable);
					}
					catch(int err_flag)
					{

					}
				}

				xSCH_MATERIAL_CTRL->Set__ROBOT_IDLE(STR__ROBOT_NAME);

				Fnc__LPx_SideBuffer_Use_Flag_Clear();
				Set__TMC_CtcInUse_To_Standby();

				//  INIT : ARM.RESERVE ...
				for(int i=0; i<iLLx_SIZE; i++)
				{
					dCH__ATM_RB__INFO_LL_RESERVE_X[i]->Set__DATA(STR__OFF);
				}

				mMNG__PMx_ALIGN.Reset__ALL_INFO();
			}

			// ...
			{
				sCH__SCH_LOOP_ID->Set__DATA("__");

				xEXT_CH__BUFFER1__APP_NEXT_SLOT_TO_PLACE->Set__DATA("1");
				xEXT_CH__BUFFER1__APP_NEXT_SLOT_TO_PICK->Set__DATA("1");
				xEXT_CH__BUFFER1__APP_CUR_LOTID->Set__DATA("");

				xEXT_CH__BUFFER2__APP_NEXT_SLOT_TO_PLACE->Set__DATA("1");
				xEXT_CH__BUFFER2__APP_NEXT_SLOT_TO_PICK->Set__DATA("1");
				xEXT_CH__BUFFER2__APP_CUR_LOTID->Set__DATA("");
			}

			// LLx ..
			{
				int ll_limit = iLLx_SIZE;
				int ll_i;

				for(ll_i=0; ll_i<ll_limit; ll_i++)
				{
					xCH__ATM_RB__SCH_STS_TO_LLx[ll_i]->Set__DATA("");

					dEXT_CH__LLx__ACTIVE_ATM_AREA_CTRL[ll_i]->Set__DATA(STR__OFF);
				}
			}

			iJOB_FLAG = -1;
			SCH__MATERIAL_CHECK();
			SCH__MODULE_CHECK();
			CHECK__ROBOT_MAP(p_variable,p_alarm);

			Sleep(100);
			idle_flag = 1;
			idle_count--;
		}
		iJOB_FLAG = 1;
			
		Sleep(1);

		// ...
		{
			sCH__SCH_LOOP_ID->Set__DATA("1");

			if(CHECK__ROBOT_MAP(p_variable,p_alarm) < 0)			continue;
			if(ATM_RB__Is_Available() < 0)							continue;
			if(xSCH_MATERIAL_CTRL->Get__MATERIAL_DB_SIZE() < 1)		continue;
			if(xCH__ATM_RB_PAUSE_FLAG->Check__DATA(STR__YES) > 0)	continue;
		}

		if(idle_flag > 0)
		{
			loop_count = 0;
			idle_flag = -1;

			xSCH_MATERIAL_CTRL->Set__ROBOT_BUSY(STR__ROBOT_NAME);
			SCH__SYSTEM_INIT(p_variable);

			for(int i=0; i<iLPx_SIZE; i++)
			{
				xCH__LPx__PICK_COUNT[i]->Set__DATA("");
			}

			mMNG__PMx_ALIGN.Reset__ALL_INFO();

			count__start_delay = 0;
		}

		// ...
		{
			loop_count++;
			if(loop_count > 10)			loop_count = 0;
		}

		// ...
		{
			sCH__SCH_LOOP_ID->Set__DATA("10");

			if(ATM_RB__Is_Available() < 0)							continue;
			if(xSCH_MATERIAL_CTRL->Check__SYSTEM_PAUSE() > 0)		continue;
			if(xSCH_MATERIAL_CTRL->Get__MATERIAL_DB_SIZE() < 1)		continue;
			if(xCH__ATM_RB_PAUSE_FLAG->Check__DATA(STR__YES) > 0)	continue;
		}

		try
		{
			if(count__start_delay < ref__start_delay)
			{
				count__start_delay++;
			}
			else
			{
				if(loop_count == 1)
				{
					sCH__SCH_LOOP_ID->Set__DATA("11.1");

					SCH__CHECK_LBi_ATM_PRESSURE(p_variable,p_alarm);
				}
			}
		}
		catch(int err_flag)
		{

		}

		try
		{
			sCH__SCH_LOOP_ID->Set__DATA("11.2");

			SCH__CHECK_DUMMY_PORT(p_variable,p_alarm);
		}
		catch(int err_flag)
		{

		}

		if(xCH__SCH_DB_STx_TRANSFER_MODE->Check__DATA(STR__AFTER_PROCESS) > 0)
		{
			sCH__SCH_LOOP_ID->Set__DATA("20");

			if(ATM_RB__Is_Available() < 0)							continue;
			if(xSCH_MATERIAL_CTRL->Check__SYSTEM_PAUSE() > 0)		continue;
			if(xSCH_MATERIAL_CTRL->Get__MATERIAL_DB_SIZE() < 1)		continue;
			if(xCH__ATM_RB_PAUSE_FLAG->Check__DATA(STR__YES) > 0)	continue;

			//  LPi -> RB
			try
			{
				if(xCH__AL1__ALIGN_RETRY_FLAG->Check__DATA("YES") < 0)
				{
					xSCH_MATERIAL_CTRL->Lock__MATERIAL_DB();
				
					sCH__SCH_LOOP_ID->Set__DATA("21");

					AUTO_CTRL__LPi_RB(p_variable,p_alarm);

					xSCH_MATERIAL_CTRL->Unlock__MATERIAL_DB();
				}
			}
			catch(int err_flag)
			{
				xSCH_MATERIAL_CTRL->Unlock__MATERIAL_DB();

				/*
				// ...
				{
					CString log_msg;

					log_msg.Format("LP1 -> RB : err_flag (%1d) \n", err_flag);
					printf(log_msg);
				}
				*/
			}
		}

		// ...
		{
			sCH__SCH_LOOP_ID->Set__DATA("50");

			if(ATM_RB__Is_Available() < 0)							continue;
			if(xSCH_MATERIAL_CTRL->Check__SYSTEM_PAUSE() > 0)		continue;
			if(xSCH_MATERIAL_CTRL->Get__MATERIAL_DB_SIZE() < 1)		continue;
			if(xCH__ATM_RB_PAUSE_FLAG->Check__DATA(STR__YES) > 0)	continue;
		}

		//  ALx -> LBi
		try
		{
			if(xEXT_CH__SCH_DB_AL1_USE_FLAG->Check__DATA(STR__ENABLE) > 0)
			{
				if(xCH__AL1__ALIGN_RETRY_FLAG->Check__DATA(STR__YES) > 0)
				{
					sCH__SCH_LOOP_ID->Set__DATA("51");

					// ...
					bool active__ll_all_mode = true;

					if(SCH__LBx__CHECK_ONLY_INPUNT() > 0)
					{
						active__ll_all_mode = false;
					}

					AUTO_CTRL__Retry_ALIGN(p_variable,p_alarm, active__ll_all_mode);
				}
				else
				{
					if(SCH__LBx__CHECK_ONLY_INPUNT() > 0)
					{
						sCH__SCH_LOOP_ID->Set__DATA("52");

						AUTO_CTRL__AL_LBi__ONLY_MODE(p_variable,p_alarm);
					}
					else
					{
						sCH__SCH_LOOP_ID->Set__DATA("53");

						AUTO_CTRL__AL_LBi__ALL_MODE(p_variable,p_alarm);
					}
				}
			}
			else
			{
				if(SCH__LBx__CHECK_ONLY_INPUNT() > 0)
				{
					sCH__SCH_LOOP_ID->Set__DATA("54");

					AUTO_CTRL__RB_LBi__ONLY_MODE(p_variable,p_alarm);
				}
				else
				{
					sCH__SCH_LOOP_ID->Set__DATA("55");

					AUTO_CTRL__RB_LBi__ALL_MODE(p_variable,p_alarm);
				}
			}
		}
		catch(int err_flag)
		{
			
		}

		if(xCH__SCH_DB_STx_TRANSFER_MODE->Check__DATA(STR__AFTER_PROCESS) > 0)
		{
			// ...
			{
				sCH__SCH_LOOP_ID->Set__DATA("100");

				if(ATM_RB__Is_Available() < 0)							continue;
				if(xSCH_MATERIAL_CTRL->Check__SYSTEM_PAUSE() > 0)		continue;
				if(xSCH_MATERIAL_CTRL->Get__MATERIAL_DB_SIZE() < 1)		continue;
				if(xCH__ATM_RB_PAUSE_FLAG->Check__DATA(STR__YES) > 0)	continue;
			}

			//  LBo -> RB
			try
			{
				sCH__SCH_LOOP_ID->Set__DATA("101");

				AUTO_CTRL__LBo_RB(p_variable,p_alarm);
			}
			catch(int err_flag)
			{

			}

			// ...
			{
				sCH__SCH_LOOP_ID->Set__DATA("90");

				if(ATM_RB__Is_Available() < 0)							continue;
				if(xSCH_MATERIAL_CTRL->Check__SYSTEM_PAUSE() > 0)		continue;
				if(xSCH_MATERIAL_CTRL->Get__MATERIAL_DB_SIZE() < 1)		continue;
				if(xCH__ATM_RB_PAUSE_FLAG->Check__DATA(STR__YES) > 0)	continue;
			}

			//  RB -> BUFFERx
			try
			{
				sCH__SCH_LOOP_ID->Set__DATA("91");

				AUTO_CTRL__RB_BUFFERx(p_variable,p_alarm);
			}
			catch(int err_flag)
			{

			}

			// ...
			{
				sCH__SCH_LOOP_ID->Set__DATA("110");

				if(ATM_RB__Is_Available() < 0)							continue;
				if(xSCH_MATERIAL_CTRL->Check__SYSTEM_PAUSE() > 0)		continue;
				if(xSCH_MATERIAL_CTRL->Get__MATERIAL_DB_SIZE() < 1)		continue;
				if(xCH__ATM_RB_PAUSE_FLAG->Check__DATA(STR__YES) > 0)	continue;
			}

			//  BUFFERx -> RB
			try
			{
				sCH__SCH_LOOP_ID->Set__DATA("111");

				AUTO_CTRL__BUFFERx_RB(p_variable,p_alarm);
			}
			catch(int err_flag)
			{

			}			

			// ...
			{
				sCH__SCH_LOOP_ID->Set__DATA("120");

				if(ATM_RB__Is_Available() < 0)							continue;
				if(xSCH_MATERIAL_CTRL->Check__SYSTEM_PAUSE() > 0)		continue;
				if(xSCH_MATERIAL_CTRL->Get__MATERIAL_DB_SIZE() < 1)		continue;
				if(xCH__ATM_RB_PAUSE_FLAG->Check__DATA(STR__YES) > 0)	continue;
			}

			//  ATM_RB -> LPo
			try
			{
				sCH__SCH_LOOP_ID->Set__DATA("121");

				AUTO_CTRL__RB_LPo(p_variable,p_alarm);
			}
			catch(int err_flag)
			{

			}
		}

		// ...
		{
			sCH__SCH_LOOP_ID->Set__DATA("140");

			if(ATM_RB__Is_Available() < 0)							continue;
			if(xSCH_MATERIAL_CTRL->Check__SYSTEM_PAUSE() > 0)		continue;
			if(xSCH_MATERIAL_CTRL->Get__MATERIAL_DB_SIZE() < 1)		continue;
			if(xCH__ATM_RB_PAUSE_FLAG->Check__DATA(STR__YES) > 0)	continue;
		}

		//  FRONT LBi
		try
		{
			sCH__SCH_LOOP_ID->Set__DATA("141");

			AUTO_CTRL__FRONT_LBi(p_variable);
		}
		catch(int err_flag)
		{

		}

		// ...
		{
			sCH__SCH_LOOP_ID->Set__DATA("150");

			if(ATM_RB__Is_Available() < 0)							continue;
			if(xSCH_MATERIAL_CTRL->Check__SYSTEM_PAUSE() > 0)		continue;
			if(xSCH_MATERIAL_CTRL->Get__MATERIAL_DB_SIZE() < 1)		continue;
			if(xCH__ATM_RB_PAUSE_FLAG->Check__DATA(STR__YES) > 0)	continue;
		}

		//  FRONT LBo
		try
		{
			sCH__SCH_LOOP_ID->Set__DATA("151");

			AUTO_CTRL__FRONT_LBo(p_variable);
		}
		catch(int err_flag)
		{

		}

		// ...
		{
			sCH__SCH_LOOP_ID->Set__DATA("160");

			if(ATM_RB__Is_Available() < 0)							continue;
			if(xSCH_MATERIAL_CTRL->Check__SYSTEM_PAUSE() > 0)		continue;
			if(xSCH_MATERIAL_CTRL->Get__MATERIAL_DB_SIZE() < 1)		continue;
			if(xCH__ATM_RB_PAUSE_FLAG->Check__DATA(STR__YES) > 0)	continue;
		}

		try
		{
			if(loop_count == 3)
			{
				sCH__SCH_LOOP_ID->Set__DATA("161");

				AUTO_CTRL__Venting_Exception_Proc(p_variable,p_alarm);
			}
		}
		catch(int err_flag)
		{

		}

		// ...
		{
			sCH__SCH_LOOP_ID->Set__DATA("170");

			if(ATM_RB__Is_Available() < 0)							continue;
			if(xSCH_MATERIAL_CTRL->Check__SYSTEM_PAUSE() > 0)		continue;
			if(xSCH_MATERIAL_CTRL->Get__MATERIAL_DB_SIZE() < 1)		continue;
			if(xCH__ATM_RB_PAUSE_FLAG->Check__DATA(STR__YES) > 0)	continue;
		}
		
		try
		{
			if(loop_count == 3)
			{
				sCH__SCH_LOOP_ID->Set__DATA("171");

				AUTO_CTRL__Robot_Abort_Exception(p_variable,p_alarm);
			}
		}
		catch(int err_flag)
		{
			
		}
	
		/*
		// ...
		{
			sCH__SCH_LOOP_ID->Set__DATA("180");

			if(ATM_RB__Is_Available() < 0)							continue;
			if(xSCH_MATERIAL_CTRL->Check__SYSTEM_PAUSE() > 0)		continue;
			if(xSCH_MATERIAL_CTRL->Get__MATERIAL_DB_SIZE() < 1)		continue;
			if(xCH__ATM_RB_PAUSE_FLAG->Check__DATA(STR__YES) > 0)	continue;
		}
		
		try
		{
			if(loop_count == 3)
			{
				sCH__SCH_LOOP_ID->Set__DATA("181");

				AUTO_CTRL__Robot_Exception_Proc(p_variable,p_alarm);
			}
		}
		catch(int err_flag)
		{
			
		}
		*/

		if(loop_count == 5)
		{
			int ll_limit = iLLx_SIZE;
			int ll_i;

			for(ll_i=0; ll_i<ll_limit; ll_i++)
			{
				if(xCH__ATM_RB__SCH_STS_TO_LLx[ll_i]->Check__DATA("") < 0)
				{
					if(xSCH_MATERIAL_CTRL->Check__MATERIAL_OF_LLx_IN(-1, 1, true) < 0)
					{
						xCH__ATM_RB__SCH_STS_TO_LLx[ll_i]->Set__DATA("");
					}
				}
			}
		}

		// ...
	}
}


//------------------------------------------------------------------------------------------------
void CObj__DUAL_ARM_STD
::SCH__SYSTEM_INIT(CII_OBJECT__VARIABLE *p_variable)
{
	// ...
	{
		xCH__ATM_RB__EXCHANGE_LOCK_FLAG->Set__DATA("");
		xCH__ATM_RB__EXCHANGE_LOCK_LBx->Set__DATA("");
		xCH__ATM_RB__EXCHANGE_LOCK_SLOT->Set__DATA("");

		xCH__AL1__ALIGN_RETRY_FLAG->Set__DATA("");
	}

	// LLx ...
	{
		iLBx_FRONT_FLAG = -1;
		iLLx_IN_COUNT = 0;

		int i_limit = iLLx_SIZE;
		int i;

		for(i=0; i<i_limit; i++)
		{
			xEXT_CH__LLx__MOVE_FLAG[i]->Set__DATA(STR__NO);
			dEXT_CH__LLx__ACTIVE_VAC_AREA_CTRL[i]->Set__DATA(STR__OFF);
			dEXT_CH__LLx__ACTIVE_ATM_AREA_CTRL[i]->Set__DATA(STR__OFF);

			xEXT_CH__LLx__IN_OUT_FLAG[i]->Set__DATA(STR__IN);
			xEXT_CH__LLx__IN_COUNT[i]->Set__DATA("0");
			xEXT_CH__LLx__OUT_COUNT[i]->Set__DATA("0");

			xEXT_CH__LLx__PRE_MAINT_FLAG[i]->Set__DATA("");
		}
	}

	try
	{
		AUTO_CTRL__LBi_VENT(p_variable);
	}
	catch(int err_flag)
	{
		
	}

	try
	{
		AUTO_CTRL__LBo_PUMP(p_variable);
	}
	catch(int err_flag)
	{
		
	}
}
void CObj__DUAL_ARM_STD
::SCH__SYSTEM_CHECK(CII_OBJECT__VARIABLE *p_variable,
					  CII_OBJECT__ALARM *p_alarm,
					  const int job_flag)
{	
	CString r_act;
	int maint_flag = -1;
	int i;

	for(i=0;i<iLPx_SIZE;i++)
	{
		if(xCH__LPx__CFG_USE_TYPE[i]->Check__DATA(STR__DISABLE) > 0)
		{
			continue;
		}

		if(LPx__Check_CtcInUse_Or_Standby(i) > 0)
		{
			continue;
		}

		maint_flag = 1;
		break;
	}

	if(ATM_RB__Check_CtcInUse_Or_Standby() < 0)
	{
		if(job_flag > 0)
		{
			p_alarm->Check__ALARM(ALID__JOB_ERROR__ATM_RB__NOT_STANDBY,r_act);
			p_alarm->Post__ALARM(ALID__JOB_ERROR__ATM_RB__NOT_STANDBY);
		}

		maint_flag = 1;
	}

	if(AL1__Check_CtcInUse_Or_Standby() < 0)
	{
		if(job_flag > 0)
		{
			p_alarm->Check__ALARM(ALID__JOB_ERROR__AL1__NOT_STANDBY,r_act);
			p_alarm->Post__ALARM(ALID__JOB_ERROR__AL1__NOT_STANDBY);
		}

		maint_flag = 1;
	}

	// LLx ...
	{
		int ll_limit = iLLx_SIZE;
		int ll_i;

		for(ll_i=0; ll_i<ll_limit; ll_i++)
		{
			if(LLx__Check_CtcInUse_Or_Standby(ll_i) < 0)
			{
				if(job_flag > 0)
				{
					p_alarm->Check__ALARM(ALID__JOB_ERROR__LBA__NOT_STANDBY,r_act);
					p_alarm->Post__ALARM(ALID__JOB_ERROR__LBA__NOT_STANDBY);
				}
	
				maint_flag = 1;
			}
		}
	}

	if(maint_flag > 0)
	{
		xSCH_MATERIAL_CTRL->Set__SYSTEM_STATUS__MAINT(sObject_Name);
	}
	else
	{
		xSCH_MATERIAL_CTRL->Clear__SYSTEM_STATUS__MAINT(sObject_Name);
	}
}
void CObj__DUAL_ARM_STD
::SCH__MATERIAL_CHECK()
{	
	// EFEM-RB ...
	{
		if(ATM_RB__Check_Empty__Arm_Type() > 0)
		{
			xSCH_MATERIAL_CTRL->Clear__SYSTEM_STATUS__MATERIAL_STS(sObject_Name, "ATM_RB-A");		
			xSCH_MATERIAL_CTRL->Clear__SYSTEM_STATUS__MATERIAL_STS(sObject_Name, "ATM_RB-B");		
		}
		else
		{
			CString arm_type;
			CString wfr_pos;

			if(ATM_RB__Get_Occupied__Arm_Type(arm_type) > 0)
			{
				wfr_pos.Format("ATM_RB-%s", arm_type);
				xSCH_MATERIAL_CTRL->Set__SYSTEM_STATUS__MATERIAL_STS(sObject_Name, wfr_pos);		
			}
		}
	}

	if(AL1__Check_Empty__All_Slot() < 0)
	{
		xSCH_MATERIAL_CTRL->Set__SYSTEM_STATUS__MATERIAL_STS(sObject_Name,"AL1");		
	}
	else
	{
		xSCH_MATERIAL_CTRL->Clear__SYSTEM_STATUS__MATERIAL_STS(sObject_Name,"AL1");		
	}

	// ...
	{
		xSCH_MATERIAL_CTRL->Clear__SYSTEM_STATUS__MATERIAL_STS(sObject_Name,"VIS1");		
	}

	// LLx ...
	{
		int ll_limit = iLLx_SIZE;
		int ll_i;

		for(ll_i=0; ll_i<ll_limit; ll_i++)
		{
			CString ll_name = Get__LLx_NAME(ll_i);

			if(LLx__Check_All_Empty(ll_i) < 0)
			{
				xSCH_MATERIAL_CTRL->Set__SYSTEM_STATUS__MATERIAL_STS(sObject_Name, ll_name);		
			}
			else
			{
				xSCH_MATERIAL_CTRL->Clear__SYSTEM_STATUS__MATERIAL_STS(sObject_Name, ll_name);		
			}
		}
	}

	// SIDE_STORAGE1 ...
	{
		CString str_stx = "ST1";

		if(sCH__SCH_DB_ST1_USE_FLAG->Check__DATA(STR__ENABLE) > 0)
		{
			if(Buffer1__Check_Empty__All_Slot() < 0)
				xSCH_MATERIAL_CTRL->Set__SYSTEM_STATUS__MATERIAL_STS(sObject_Name, str_stx);
			else
				xSCH_MATERIAL_CTRL->Clear__SYSTEM_STATUS__MATERIAL_STS(sObject_Name, str_stx);		
		}
		else
		{
			xSCH_MATERIAL_CTRL->Clear__SYSTEM_STATUS__MATERIAL_STS(sObject_Name, str_stx);		
		}
	}
	// SIDE_STORAGE2 ...
	{
		CString str_stx = "ST2";

		if(sCH__SCH_DB_ST2_USE_FLAG->Check__DATA(STR__ENABLE) > 0)
		{
			if(Buffer2__Check_Empty__All_Slot() < 0)
				xSCH_MATERIAL_CTRL->Set__SYSTEM_STATUS__MATERIAL_STS(sObject_Name, str_stx);					
			else
				xSCH_MATERIAL_CTRL->Clear__SYSTEM_STATUS__MATERIAL_STS(sObject_Name, str_stx);		
		}
		else
		{
			xSCH_MATERIAL_CTRL->Clear__SYSTEM_STATUS__MATERIAL_STS(sObject_Name, str_stx);		
		}
	}
}
void CObj__DUAL_ARM_STD
::SCH__MODULE_CHECK()
{	
	if(ATM_RB__Is_Available() > 0)
	{
		xSCH_MATERIAL_CTRL->Set__SYSTEM_STATUS__MODULE_AVAILABLE(sObject_Name,"ATM_RB");		
	}
	else
	{
		xSCH_MATERIAL_CTRL->Set__SYSTEM_STATUS__MODULE_NOT_AVAILABLE(sObject_Name,"ATM_RB");		
	}

	if(AL1__Is_Available() > 0)
	{
		xSCH_MATERIAL_CTRL->Set__SYSTEM_STATUS__MODULE_AVAILABLE(sObject_Name,"AL1");		
	}
	else
	{
		xSCH_MATERIAL_CTRL->Set__SYSTEM_STATUS__MODULE_NOT_AVAILABLE(sObject_Name,"AL1");		
	}

	// ...
	{
		xSCH_MATERIAL_CTRL->Set__SYSTEM_STATUS__MODULE_DISABLE(sObject_Name,"VIS1");		
	}

	// LLX ...
	{
		int ll_limit = iLLx_SIZE;
		int ll_i;

		for(ll_i=0; ll_i<ll_limit; ll_i++)
		{
			CString ll_name = Get__LLx_NAME(ll_i);

			if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_i]->Check__DATA(LBx_MODE__ALL) > 0)
			{
				if(LLx__Is_Available(ll_i) < 0)
				{
					xSCH_MATERIAL_CTRL->Set__SYSTEM_STATUS__MODULE_NOT_AVAILABLE(sObject_Name, ll_name);		
				}
				else
				{
					xSCH_MATERIAL_CTRL->Set__SYSTEM_STATUS__MODULE_AVAILABLE(sObject_Name, ll_name);		
				}
			}
			else
			{
				if(LLx__Is_Available(ll_i) > 0)
				{
					xSCH_MATERIAL_CTRL->Set__SYSTEM_STATUS__MODULE_AVAILABLE(sObject_Name, ll_name);		
				}
				else
				{
					xSCH_MATERIAL_CTRL->Set__SYSTEM_STATUS__MODULE_NOT_AVAILABLE(sObject_Name, ll_name);		
				}
			}
		}
	}

	// ...
}

//------------------------------------------------------------------------------------------------
int  CObj__DUAL_ARM_STD
::CHECK__ROBOT_MAP(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm)
{
	int i;

	if(xEXT_CH__SCH_MAP_DEVICE->Check__DATA(STR__ATM_RB) < 0)
	{
		for(i=0;i<iLPx_SIZE;i++)
		{
			if(xEXT_CH__LPx_MAP_REQ[i]->Check__DATA(STR__YES) > 0)
			{
				xEXT_CH__LPx_MAP_REQ[i]->Set__DATA("");
			}
		}
		return 1;
	}
	else
	{
		for(i=0;i<iLPx_SIZE;i++)
		{
			if((ATM_RB__Check_CtcInUse_Or_Standby() < 0)
			|| (LPx__Check_CtcInUse_Or_Standby(i)   < 0))
			{
				if(xEXT_CH__LPx_MAP_REQ[i]->Check__DATA(STR__YES) > 0)
				{
					xEXT_CH__LPx_MAP_REQ[i]->Set__DATA(STR__ERROR);
					
					// ...
					int alarm_id = ALID__LPx__MAP_ABORTED;
					CString alm_msg;
					CString alm_bff;
					CString r_act;
					CString var_data;

					alm_bff.Format("LP%1d's mapping request is rejected. \n", i+1);
					alm_msg += alm_bff;

					//
					alm_msg += "1. ATM Robot's status must be \"CtcInUse\" or \"Standby\". \n";

					xCH__ATM_RB__OBJ_STATUS->Get__DATA(var_data);
					alm_bff.Format("   Robot's current status is \"%s\". \n", var_data);
					alm_msg += alm_bff;

					//
					alm_bff.Format("2. LP%1d's status must be \"CtcInUse\" or \"Standby\". \n", i+1);
					alm_msg += alm_bff;

					xCH__LPx__OBJ_STATUS[i]->Get__DATA(var_data);
					alm_bff.Format("   LP%1d's current status is \"%s\". \n", i+1,var_data);
					alm_msg += alm_bff;

					//
					p_alarm->Check__ALARM(alarm_id,r_act);
					p_alarm->Post__ALARM_With_MESSAGE(alarm_id,alm_msg);
				}
			}
		}
	}

	if(ATM_RB__Check_Empty__Arm_Type() < 0)
	{
		return 1;
	}
	if(ATM_RB__Is_Available() < 0)
	{
		return -1;
	}

	// ...
	CString lp_name;
	int flag;

	for(i=0; i<iLPx_SIZE; i++)
	{
		if(xEXT_CH__LPx_MAP_REQ[i]->Check__DATA(STR__YES) > 0)
		{
			if(LPx__Check_CtcInUse_Or_Standby(i) < 0)
			{
				xEXT_CH__LPx_MAP_REQ[i]->Set__DATA(STR__ERROR);
				continue;
			}

			if(ATM_RB__Check_Empty__Arm_Type() < 0)
			{
				return 1;
			}
			if(ATM_RB__Is_Available() < 0)
			{
				return -1;
			}

			int wait_flag = -1;

			if(xCH__LPx__PORT_STATUS[i]->Check__DATA("WAITING") > 0)
			{
				wait_flag = 1;
				
				xCH__LPx__PORT_STATUS[i]->Set__DATA("MAPPING ...");
			}

			lp_name.Format("LP%1d",i+1);
			flag = SEQ__MAP_BY_ATM_RB(p_variable,lp_name);

			if(flag > 0)		xEXT_CH__LPx_MAP_REQ[i]->Set__DATA("");
			else				xEXT_CH__LPx_MAP_REQ[i]->Set__DATA(STR__ERROR);

			if(wait_flag > 0)
			{
				xCH__LPx__PORT_STATUS[i]->Set__DATA("WAITING");
			}

			if(flag > 0)
			{
				if(xSCH_MATERIAL_CTRL->Get__MATERIAL_DB_SIZE() < 1)
				{
					SEQ__ATM_RB__HOME(-1);
				}
			}
		}
	}
	return 1;
}

int  CObj__DUAL_ARM_STD
::AUTO_CTRL__LPi_RB(CII_OBJECT__VARIABLE *p_variable, CII_OBJECT__ALARM *p_alarm)
{
	if(ATM_RB__Check_Occupied__Arm_Type() > 0)
	{
		NEXT__LOOP_ID(101);
	}

	if(Check__Robot_Abort_Exception() > 0)
	{
		NEXT__LOOP_ID(102);
	}

	// ...
	{
		int port_id;
		int slot_id;

		if(xSCH_MATERIAL_CTRL->Get__MATERIAL_FROM_LPx(port_id,slot_id) > 0)
		{
			if(SCH__LPx_TO_LLx_OF_ROUTE_TYPE(p_variable, port_id,slot_id) < 0)
			{
				// xSCH_MATERIAL_CTRL->JOB_CTRL__GoTo_NEXT_PORT_WITHOUT_PORT(port_id);

				/*
				// ...
				{
					printf(" * CObj__DUAL_ARM_STD::AUTO_CTRL__LPi_RB() ... \n");
					printf(" ** SCH__LPx_TO_LLx_OF_ROUTE_TYPE() < 0 \n");
					printf(" ** port_id <- %1d \n", port_id);
					printf(" ** slot_id <- %1d \n", slot_id);
					printf("\n");
				}
				*/

				NEXT__LOOP_ID(103);
			}

			// ...
			CUIntArray l_ptn;
			xSCH_MATERIAL_CTRL->Get__JOB_PORT_LIST(l_ptn);

			if(l_ptn.GetSize() > 1)
			{
				int active__pick_lp = 1;

				// ...
				{
					int i_limit = l_ptn.GetSize();
					int i;

					for(i=0; i<i_limit; i++)
					{
						int lp_ptn = l_ptn[i];
						
						if(port_id == lp_ptn)			continue;

						if(xSCH_MATERIAL_CTRL->Check__MATERIAL_TO_PROCESS_IN_LPx(lp_ptn) < 0)
						{
							continue;
						}

						active__pick_lp = -1;
						break;
					}
				}

				if(active__pick_lp > 0)
				{
					if((AL1__Is_Available() < 0)
					|| (AL1__Check_Empty__All_Slot() < 0))
					{
						active__pick_lp = -1;
					}
				}

				if(active__pick_lp < 0)
				{
					if(SCH__CAN_PLACE_LBi(1) < 0)
					{
						NEXT__LOOP_ID(111);
					}
				}
			}
			else
			{
				int active__pick_lp = -1;

				if(AL1__Is_Available() > 0)
				{
					if(AL1__Check_Empty__All_Slot() > 0)
					{
						active__pick_lp = 1;
					}
				}

				if(active__pick_lp < 0)
				{
					if(SCH__CAN_PLACE_LBi(1) < 0)
					{
						NEXT__LOOP_ID(112);
					}
				}
			}
		}
		else
		{
			NEXT__LOOP_ID(113);
		}
	}

	// ...
	{
		int check_count = 0;

		while(1)
		{
			if(ATM_RB__Is_Available() < 0)
			{
				check_count++;
				if(check_count > 10)
				{
					NEXT__LOOP_ID(121);
				}

				Sleep(90);
				continue;
			}
			break;
		}
	}

	if(ATM_RB__Check_Empty__Arm_Type() < 0)
	{
		NEXT__LOOP_ID(122);
	}

	if(dCH__ATM_RB__CFG_PICK_WAFER_CONDITION->Check__DATA(STR__ONLY_PROCESSED) > 0)
	{
		// LLx Out - Check ...
		{
			for(int ll_i=0; ll_i < iLLx_SIZE; ll_i++)
			{
				if(LLx__Get_Occupied__Total_OutSlot(ll_i) < 1)			continue;

				if(LLx__Is_Available(ll_i) < 0)			continue;
				if(LLx__Is_ATM(ll_i) < 0)				continue;

				NEXT__LOOP_ID(131);
			}
		}

		// LLx In - Check ...
		{
			bool active__ll_error = true;

			for(int ll_i=0; ll_i < iLLx_SIZE; ll_i++)
			{
				if(LLx__Get_Empty__Total_InSlot(ll_i) < 1)				continue;

				if(LLx__Is_Available(ll_i) < 0)			continue;
				if(LLx__Is_ATM(ll_i) < 0)				continue;

				active__ll_error = false;
				break;;
			}

			if(active__ll_error)		NEXT__LOOP_ID(132);
		}
	}

	// ...
	int lp_pick__count = 0;
	int empty_arm__count = 1;

	if(xCH_CFG__SCH_ARM_MODE->Check__DATA(STR__DUAL) > 0)
	{
		int ll_in_count = LLx__Get_Empty__Total_InSlot();
		if(ll_in_count < 1)			NEXT__LOOP_ID(133);

		empty_arm__count = ATM_RB__Get_Empty_Arm_Count();
		if(empty_arm__count > ll_in_count)		empty_arm__count = ll_in_count;	
	}

	// ...
	bool active__left_right = false;

	if(dEXT_CH__CFG_LLx_CTRL_ONLY_INPUT_OUTPUT_MODE->Check__DATA(STR__DUAL) > 0)
	{
		active__left_right = true;
	}

	// PMx Check ...
	if((active__left_right)	&& (empty_arm__count > 0))
	{
		bool active__ll_left  = false;
		bool active__ll_right = false;

		// ...
		CUIntArray l__job_ptn;
		xSCH_MATERIAL_CTRL->Get__JOB_PORT_LIST(l__job_ptn);

		int k_limit = l__job_ptn.GetSize();
		for(int k=0; k<k_limit; k++)
		{
			int job__port_id = l__job_ptn[k];

			int r_flag = xSCH_MATERIAL_CTRL->Check__MATERIAL_TO_PROCESS_IN_LPx(job__port_id);
			if(r_flag < 0)			continue;

			xSCH_MATERIAL_CTRL->JOB_CTRL__GoTo_JOB_PORT(job__port_id);

			//===================================================
			int db__port_id;
			int db__slot_id;

			if(xSCH_MATERIAL_CTRL->Get__MATERIAL_FROM_LPx(db__port_id, db__slot_id) < 0)
			{
				continue;
			}

			// ...	
			{
				int ptn_id = db__port_id;
				int stn_id = db__slot_id;

				// ...
				int active__wfr_normal = 1;

				// ...
				{
					IDS__SCH_MATERIAL_INFO ds_info;
					xSCH_MATERIAL_CTRL->Get__MATERIAL_INFO(ptn_id,stn_id, ds_info);

					int wfr__port_id = ds_info.iSRC__PTN;

					if((wfr__port_id >  0)
					&& (wfr__port_id <= iLPx_SIZE))
					{
						int wfr__port_index = wfr__port_id - 1;

						if(xCH__LPx__CFG_PROCESS_TYPE[wfr__port_index]->Check__DATA(STR__TEST) > 0)
						{
							active__wfr_normal = -1;
						}
					}
				}

				// ...
				bool active__pm_odd  = false;
				bool active__pm_even = false;

				CStringArray l__pm_name;
				CStringArray l__pm_rcp;

				xSCH_MATERIAL_CTRL->Get__CUR_PROCESS_INFO(ptn_id,stn_id, l__pm_name,l__pm_rcp);

				// ...
				{
					int count__wfr_type = -1;

					int k_limit = l__pm_name.GetSize();
					for(int k=0; k<k_limit; k++)
					{
						CString pmc_name = l__pm_name[k];

						int pm_index = _Get__PM_INDEX(pmc_name);
						if(pm_index < 0)		continue;

						if(active__wfr_normal < 0)
						{
							if(dEXT_CH__PMx_PROCESS_TYPE[pm_index]->Check__DATA(STR__TEST) < 0)
							{
								continue;
							}
						}

						count__wfr_type = 1;
						break;
					}

					if(count__wfr_type < 0)
					{
						active__wfr_normal = 1;
					}
				}

				// ...
				int k_limit = l__pm_name.GetSize();
				int k;
				
				for(k=0; k<k_limit; k++)
				{
					CString pmc_name = l__pm_name[k];

					int pm_index = _Get__PM_INDEX(pmc_name);
					if(pm_index < 0)		continue;

					if(active__wfr_normal > 0)
					{
						if(dEXT_CH__PMx_PROCESS_TYPE[pm_index]->Check__DATA(STR__NORMAL) < 0)
						{
							continue;
						}
					}
					else
					{
						if(dEXT_CH__PMx_PROCESS_TYPE[pm_index]->Check__DATA(STR__TEST) < 0)
						{
							continue;
						}
					}

					if(PMx__Is_Ready(pm_index) < 0)
					{
						continue;
					}

					int pm_id = pm_index + 1;
					int pm_check = pm_id % 2;

					if(pm_check == 1)
					{
						if(LLx__Check_Empty__InSlot_Of_Odd_Type())
							active__pm_odd  = true;
					}
					else if(pm_check == 0)
					{
						if(LLx__Check_Empty__InSlot_Of_Even_Type())
							active__pm_even = true;
					}
				}

				if((active__pm_odd) && (active__pm_even))
				{
					active__ll_left  = true;
					active__ll_right = true;
				}
				else if(active__pm_odd)
				{
					active__ll_left  = true;
				}
				else if(active__pm_even)
				{
					active__ll_right = true;
				}
				else
				{
					for(int ll_i=0; ll_i<iLLx_SIZE; ll_i++)
					{
						if(xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Check__DATA(STR__IN) < 0)
						{
							continue;
						}

						if((LLx__Is_ATM(ll_i) > 0)
						&& (LLx__Is_Available(ll_i) > 0))
						{
							if(SCH_RUN__LLx_PUMP(ll_i, p_variable, "", "") > 0)
							{
								xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Set__DATA(STR__OUT);
							}
						}
					}

					NEXT__LOOP_ID(141);
				}
			}
			//===================================================
		}

		if((active__ll_left) && (active__ll_right))
		{

		}
		else if((active__ll_left) || (active__ll_right))
		{
			empty_arm__count = 1;
		}
	}
	else if(empty_arm__count > 0)
	{
		int db__port_id;
		int db__slot_id;

		if(xSCH_MATERIAL_CTRL->Get__MATERIAL_FROM_LPx(db__port_id, db__slot_id) < 0)
		{
			NEXT__LOOP_ID(142);
		}

		if(sEXT_CH__SCH_MODE_LLx_IN->Check__DATA(_SCH_MODE__ROUTE) > 0)
		{
			CStringArray l_ll_id;
			CStringArray l_ll_mode;
			CStringArray l_ll_slot;

			xSCH_MATERIAL_CTRL->Get__LLx_IN_OF_EDIT_TYPE(db__port_id,db__slot_id, l_ll_id,l_ll_mode,l_ll_slot);

			if(l_ll_id.GetSize() == 1)
			{
				CString str_mode = l_ll_mode[ 0 ];

				if(str_mode.CompareNoCase(STR__ALL) == 0)
				{
					empty_arm__count = 1;
				}
			}
		}
	}

	//=============================================================================
	bool active__dual_arm = false;

	int dual_arm__port_id;
	int dual_arm__slot_id;

	// ...
	{
		int count__job_port = 0;

		CUIntArray l__job_ptn;
		xSCH_MATERIAL_CTRL->Get__JOB_PORT_LIST(l__job_ptn);
		
		int k_limit = l__job_ptn.GetSize();
		for(int k=0; k<k_limit; k++)
		{
			int port_id = l__job_ptn[k];

			int r_flag = xSCH_MATERIAL_CTRL->Check__MATERIAL_TO_PROCESS_IN_LPx(port_id);
			if(r_flag < 0)			continue;

			count__job_port++;
		}

		if(count__job_port > 1)
		{
			active__dual_arm = false;
		}
		else
		{
			if((dCH__ATM_RB__CFG_DUAL_ARM_MOVING_AT_THE_SAME_TIME->Check__DATA(STR__ENABLE) > 0)
			&& (dCH__ATM_RB__CFG_DUAL_ARM_MOVING_AT_LPx->Check__DATA(STR__YES) > 0)
			&& (empty_arm__count > 1))
			{
				CUIntArray l__port_id;
				CUIntArray l__slot_id;

				int wafer__max_size = 2;

				if(xSCH_MATERIAL_CTRL->Get__MATERIAL_FROM_LPx_WITH_MULTI_SLOT_EX(l__port_id,l__slot_id, wafer__max_size) > 0)
				{
					int ptn_id__1st = 0;
					int stn_id__1st = 0;

					int ptn_id__2nd = 0;
					int stn_id__2nd = 0;

					int k_limit = l__port_id.GetSize();
					int k;
						
					for(k=0; k<k_limit; k++)
					{
						if(k == 0)
						{
							ptn_id__1st = l__port_id[k];
							stn_id__1st = l__slot_id[k];
						}
						else if(k == 1)
						{
							ptn_id__2nd = l__port_id[k];
							stn_id__2nd = l__slot_id[k];
						}
					}

					if((ptn_id__1st == ptn_id__2nd)
					&& ((stn_id__1st + 1) == stn_id__2nd))
					{
						active__dual_arm = true;

						dual_arm__port_id = ptn_id__1st;
						dual_arm__slot_id = stn_id__1st;
					}
				}
			}
		}
	}
	//=============================================================================
	
	while(empty_arm__count > 0)
	{
		empty_arm__count--;

		//=========================================================================
		{
			_Update__JOB_CTRL__GoTo_JOB_PORT();
		}
		//=========================================================================

		// ...
		int ptn_id;
		int stn_id;

		if(xSCH_MATERIAL_CTRL->Get__MATERIAL_FROM_LPx(ptn_id, stn_id) > 0)
		{
			int ll_id = SCH__LPx_TO_LLx_OF_ROUTE_TYPE(p_variable, ptn_id,stn_id);
			if(ll_id < 0)			NEXT__LOOP_ID(151);

			if(xEXT_CH__SYSTEM_LEAK_CHECK_REQ->Check__DATA(STR__YES) > 0)
			{
				int db_index = ptn_id - 1;
				
				if((db_index >= 0) && (db_index < iLPx_SIZE))
				{
					CString var_data = xCH__LPx__PICK_COUNT[db_index]->Get__STRING();
					int i_count = atoi(var_data);

					if(i_count < 1)			NEXT__LOOP_ID(152);
				}
			}

			if(LPx__Check_Occupied__Slot_Status(ptn_id, stn_id) < 0)
			{
				int lp_index   = ptn_id - 1;
				int slot_index = stn_id - 1;

				int alarm_id = ALID__LPx_MATERIAL_PICK__JOB_ERROR;
				CString alm_msg;
				CString alm_bff;
				CString r_act;

				alm_bff.Format("Please, check LP(%1d) and slot(%1d)'s status.\n", ptn_id,stn_id);
				alm_msg += alm_bff;

				if((lp_index >= 0) && (lp_index < CFG_LP_LIMIT))
				{
					CString var_data;

					xCH__LPx__PORT_STATUS[lp_index]->Get__DATA(var_data);
					
					alm_bff.Format("   1. Port Status     : \"%s\" \n", var_data);
					alm_msg += alm_bff;
					alm_msg += "       status must be \"BUSY\". \n";

					xCH__LPx__CST_STATUS[lp_index]->Get__DATA(var_data);
					
					alm_bff.Format("   2. Cassette Status : \"%s\" \n", var_data);
					alm_msg += alm_bff;
					alm_msg += "       status must be \"EXIST\". \n";
				}
				if((slot_index >= 0) && (slot_index < CFG_LP__SLOT_MAX))
				{
					CString var_data;

					xCH__LPx__SLOT_STATUS[lp_index][slot_index]->Get__DATA(var_data);
					
					alm_bff.Format("   3. Slot Status     : \"%s\" \n", var_data);
					alm_msg += alm_bff;
					alm_msg += "       status must be \"MAPPED\". \n";
				}

				p_alarm->Check__ALARM(alarm_id,r_act);
				p_alarm->Post__ALARM_With_MESSAGE(alarm_id,alm_msg);

				NEXT__LOOP_ID(153);
			}

			if(xEXT_CH__SCH_DB_AL1_USE_FLAG->Check__DATA(STR__ENABLE) > 0)
			{
				if(AL1__Check_Empty__All_Slot() < 0)
				{
					if(SCH__CHECK_LBo_OCCUPIED() > 0)
					{
						NEXT__LOOP_ID(154);	
					}
				}
			}

			if(active__dual_arm)
			{
				int cur__arm_count = ATM_RB__Get_Empty_Arm_Count();
				if(cur__arm_count < 2)			NEXT__LOOP_ID(155);

				CString robot_arm = ARM_AB;

				if(Fnc__PICK_LPi_WITH_ARM(p_variable,p_alarm, robot_arm, dual_arm__port_id,dual_arm__slot_id, ll_id) < 0)
				{
					NEXT__LOOP_ID(156);
				}

				empty_arm__count--;
			}
			else
			{
				CString robot_arm;

				if(ATM_RB__Get_Empty__Arm_Type(robot_arm) < 0)			NEXT__LOOP;

				if(Fnc__PICK_LPi_WITH_ARM(p_variable,p_alarm, robot_arm, ptn_id,stn_id, ll_id) < 0)
				{
					NEXT__LOOP_ID(157);
				}
			}

			lp_pick__count++;

			xSCH_MATERIAL_CTRL->Unlock__MATERIAL_DB();
			xSCH_MATERIAL_CTRL->Lock__MATERIAL_DB();

			if(ATM_RB__Check_Empty__Arm_Type() < 1)	
			{
				break;
			}
		}
	}

	if(lp_pick__count > 0)
	{
		xSCH_MATERIAL_CTRL->Unlock__MATERIAL_DB();
			
		if(xSCH_MATERIAL_CTRL->Check__SYSTEM_ABORT() > 0)		NEXT__LOOP_ID(161);
		if(xSCH_MATERIAL_CTRL->Get__MATERIAL_DB_SIZE() < 1)		NEXT__LOOP_ID(162);

		if(xSCH_MATERIAL_CTRL->Check__MATERIAL_MORE_THAN_MOVE_COUNT(1) < 0)
		{
			SCH__SYSTEM_INIT(p_variable);
		}
		else
		{
			Seq__LBx_VENT__ALL_MODE(p_variable);
		}
	}
	
	return 1;
}
int  CObj__DUAL_ARM_STD
::_Update__JOB_CTRL__GoTo_JOB_PORT()
{
	// CHECL : Dummy_Port...
	{
		int dummy__port_id = Fnc__GET_DUMMY_PORT();

		if((dummy__port_id > 0)
		&& (xSCH_MATERIAL_CTRL->Check__MATERIAL_TO_PROCESS_IN_LPx(dummy__port_id) > 0))
		{
			xSCH_MATERIAL_CTRL->JOB_CTRL__GoTo_JOB_PORT(dummy__port_id);
			return 1;
		}
	}

	// ...
	CUIntArray l__job_ptn;
	xSCH_MATERIAL_CTRL->Get__JOB_PORT_LIST(l__job_ptn);

	if(l__job_ptn.GetSize() > 1)
	{
		CUIntArray l__wfr_port_id;
		CUIntArray l__wfr_count;

		// Data Init ...
		{
			int k_limit = l__job_ptn.GetSize();

			for(int k=0; k < k_limit; k++)
			{
				int port_id = l__job_ptn[k];
				int count = 0;

				l__wfr_port_id.Add(port_id);
				l__wfr_count.Add(count);
			}
		}

		// ...
		{
			CUIntArray l__port_id;

			// CHECK : WAFER ON ARM
			{
				CStringArray l__arm_type;
				ATM_RB__Get_Occupied__Arm_Type(l__arm_type);

				int k_limit = l__arm_type.GetSize();
				for(int k=0; k<k_limit; k++)
				{
					CString arm_type = l__arm_type[k];

					if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(arm_type) < 0)
					{
						continue;
					}

					IDS__SCH_MATERIAL_STATUS ds_info;
					xSCH_MATERIAL_CTRL->Get__MATERIAL_STATUS(arm_type, ds_info);

					int port_id = ds_info.iPORT_ID;
					if(port_id < 1)			continue;

					l__port_id.Add( port_id );
				}
			}
			// CHECK : LL_IN WAFER
			{
				for(int ll_i=0; ll_i<iLLx_SIZE; ll_i++)
				{
					int ll__slot_id;

					if(LLx__Check_CtcInUse_Or_Standby(ll_i) < 0)				continue;
					if(LLx__Get_Occupied__InSlot(ll_i, ll__slot_id) < 0)		continue;

					// ...
					CString str__ll_name = Get__LLx_NAME(ll_i);

					CString str__sch_name;
					str__sch_name.Format("%s-%1d", str__ll_name, ll__slot_id);

					if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(str__sch_name) < 0)
					{
						continue;
					}

					IDS__SCH_MATERIAL_STATUS ds_info;
					xSCH_MATERIAL_CTRL->Get__MATERIAL_STATUS(str__sch_name, ds_info);

					int port_id = ds_info.iPORT_ID;
					if(port_id < 1)			continue;

					l__port_id.Add( port_id );
				}
			}

			// ...
			{
				int k_limit = l__port_id.GetSize();
				
				for(int k=0; k < k_limit; k++)
				{
					int port_id = l__port_id[k];

					// ...
					bool active__add_check = true;

					int t_limit = l__wfr_port_id.GetSize();
					for(int t=0; t < t_limit; t++)
					{
						if(port_id != l__wfr_port_id[t])		continue;

						l__wfr_count[t] = l__wfr_count[t] + 1;

						active__add_check = false;
						break;
					}

					if(active__add_check)
					{
						l__wfr_port_id.Add(port_id);
						l__wfr_count.Add(1);
					}
				}
			}
		}

		// ...
		{
			int k_limit = l__wfr_port_id.GetSize();

			for(int k=0; k<k_limit; k++)
			{
				int min__wfr_port_id = l__wfr_port_id[k];
				int min__wfr_count   = l__wfr_count[k];

				for(int t=k+1; t<k_limit; t++)
				{
					int cur__wfr_port_id = l__wfr_port_id[t];
					int cur__wfr_count = l__wfr_count[t];

					if(cur__wfr_count >= min__wfr_count)			continue;

					min__wfr_port_id = cur__wfr_port_id;
					min__wfr_count   = cur__wfr_count;

					l__wfr_port_id[t] = l__wfr_port_id[k];
					l__wfr_count[t]   = l__wfr_count[k];

					l__wfr_port_id[k] = min__wfr_port_id;
					l__wfr_count[k]   = min__wfr_count;
				}
			}
		}

		/*
		// ...
		{
			CString log_msg;
			CString log_bff;

			log_msg = "LPi -> RB \n";

			// ...
			{
				log_msg += "  Wafer Port ID ... \n";

				int k_limit = l__wfr_port_id.GetSize();
				for(int k=0; k<k_limit; k++)
				{
					log_bff.Format("    * %1d)  P%1d  (%1d) \n", k, l__wfr_port_id[k], l__wfr_count[k]);
					log_msg += log_bff;
				}
				log_msg += "\n";
			}

			// ...
			{
				log_msg += "  Job Port ID ... \n";

				int k_limit = l__job_ptn.GetSize();
				for(int k=0; k < k_limit; k++)
				{
					log_bff.Format("    * %1d)  P%1d \n", k, l__job_ptn[k]);
					log_msg += log_bff;
				}
				log_msg += "\n";
			}

			printf(log_msg);
		}
		*/

		if(l__wfr_port_id.GetSize() > 0)
		{
			int port_id = l__wfr_port_id[0];

			xSCH_MATERIAL_CTRL->JOB_CTRL__GoTo_JOB_PORT(port_id);
		}
	}

	return 1;
}

int  CObj__DUAL_ARM_STD
::AUTO_CTRL__RB_LBi__ONLY_MODE(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm)
{
	if(SCH__CAN_PLACE_LBi() < 0)					NEXT__LOOP;
	if(SCH__CHECK_LBi_EMPTY() < 0)					NEXT__LOOP;

	// ...
	int arm__count = 1;

	if(xCH_CFG__SCH_ARM_MODE->Check__DATA(STR__DUAL) > 0)
	{
		arm__count = 2;
	}

	// ...
	CString arm_type;
	bool ex_flag = false;
	
	for(int i=0; i<arm__count; i++)
	{
		if(xCH__ATM_RB_PAUSE_FLAG->Check__DATA(STR__YES) > 0)
		{
			NEXT__LOOP;
		}

		if(ATM_RB__Get_Occupied__Arm_Type(arm_type) < 0)
		{
			NEXT__LOOP;
		}

		if(dCH__CFG_PRE_ALIGN_USE->Check__DATA(STR__YES) > 0)
		{
			IDS__SCH_MATERIAL_STATUS ds_sts;
			xSCH_MATERIAL_CTRL->Get__MATERIAL_STATUS(arm_type, ds_sts);

			if(ds_sts.sMATERIAL_STS.CompareNoCase(STR__ALIGNED) != 0)
			{
				continue;
			}
		}

		if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(arm_type) < 0)
		{
			continue;
		}

		if(Fnc__PLACE_LBi__ONLY_MODE(p_variable,p_alarm, ex_flag, arm_type) < 0)
		{
			NEXT__LOOP;
		}
	}

	return 1;
}

int  CObj__DUAL_ARM_STD
::AUTO_CTRL__RB_LBi__ALL_MODE(CII_OBJECT__VARIABLE *p_variable,
							  CII_OBJECT__ALARM *p_alarm)
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
	int k_limit = l__arm_type.GetSize();

	for(int k=0; k<k_limit; k++)
	{
		CString arm_type = l__arm_type[k];

		if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(arm_type) < 0)
		{
			NEXT__LOOP;
		}

		if(dCH__CFG_PRE_ALIGN_USE->Check__DATA(STR__YES) > 0)
		{
			IDS__SCH_MATERIAL_STATUS ds_sts;
			xSCH_MATERIAL_CTRL->Get__MATERIAL_STATUS(arm_type, ds_sts);

			if(ds_sts.sMATERIAL_STS.CompareNoCase(STR__ALIGNED) != 0)
			{
				NEXT__LOOP;
			}
		}

		int r_flag = _Fnc__RB_LBi__ALL_MODE(p_variable,p_alarm, arm_type);
		if(r_flag < 0)		NEXT__LOOP;
	}

	return 1;
}
int  CObj__DUAL_ARM_STD
::_Fnc__RB_LBi__ALL_MODE(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm, const CString& arm_type)
{
	if(SCH__CAN_PLACE_LBi() < 0)
	{
		return -11;
	}

	if(Seq__LBx_VENT__INTO_LBi(p_variable) > 0)
	{
		return -12;
	}

	// ...
	bool ex_flag = false;

	// ...
	int material_flag = -1;

	CString para_module;
	CString para_in_slot;
	CString para_out_slot;
	int slot_id;
	int pick_flag = -1;

	int active__sch_mode = -1;
	int active__sch_ll = -1;

	if(sEXT_CH__SCH_MODE_LLx_IN->Check__DATA(_SCH_MODE__ROUTE) > 0)
	{
		CStringArray l_ll_id;
		CStringArray l_ll_mode;
		CStringArray l_ll_slot;

		xSCH_MATERIAL_CTRL->Get__LLx_IN_OF_EDIT_TYPE(arm_type, l_ll_id,l_ll_mode,l_ll_slot);

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
	}

	// ...
	int llx_out_flag = -1;

	if(active__sch_mode > 0)
	{
		int i_limit = iLLx_SIZE;
		
		for(int i=0; i<i_limit; i++)
		{
			int id = i + 1;
			if(active__sch_ll != id)		continue;
			
			llx_out_flag = id;
			break;
		}
	}
	else
	{
		int llx__count_max = -1;

		int i_limit = iLLx_SIZE;
		int i;
		
		for(i=0; i<i_limit; i++)
		{
			if(LLx__Is_ATM(i) < 0)							continue;
			if(LLx__Is_Available(i) < 0)					continue;
			if(LLx__Get_Empty__InSlot(i, slot_id) < 0)		continue;

			CString ch_data = xEXT_CH__LLx__OUT_COUNT[i]->Get__STRING();
			int llx_count = atoi(ch_data);
			if(llx_count <= llx__count_max)					continue;

			llx_out_flag = i + 1;
			llx__count_max = llx_count;
		}
	}

	if(llx_out_flag > 0)
	{
		int ll_i = llx_out_flag - 1;

		if(xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Check__DATA(STR__IN) > 0)
		{
			if((LLx__Is_ATM(ll_i) > 0)
			&& (LLx__Is_Available(ll_i) > 0))
			{
				if(LLx__Get_Empty__InSlot(ll_i, slot_id) > 0)
				{
					para_module = Get__LLx_NAME(ll_i);
					para_in_slot.Format("%1d", slot_id);

					if(LLx__Get_Occupied__OutSlot(ll_i, slot_id) > 0)
					{
						para_out_slot.Format("%1d", slot_id);

						pick_flag = 1;
						ex_flag = true;
					}

					material_flag = 1;
				}
			}
		}
	}

	if(material_flag < 0)
	{
		return -21;
	}

	if(ex_flag == true)
	{
		CString sch_name;
		sch_name.Format("%s-%s", para_module,para_out_slot);

		if(Get__LPx_SideBuffer_Info_To_Use(sch_name) < 1)
		{
			ex_flag = false;
		}
	}

	if(ex_flag == true)
	{
		if((xCH_CFG__LBx_SWAP_FLAG->Check__DATA(STR__ENABLE) > 0)
		&& (pick_flag > 0))
		{
			CString sch_place_module;
			CString sch_pick_module;

			sch_place_module.Format("%s-%s",para_module,para_in_slot );
			sch_pick_module.Format( "%s-%s",para_module,para_out_slot);

			if(Fnc__SWAP_LBx(p_variable,
							 p_alarm,
							 para_module,
							 arm_type,
							 para_in_slot,
							 arm_type,
							 para_out_slot,
							 sch_place_module,
							 sch_pick_module) < 0)
			{
				xEXT_CH__JOB_CTRL->Set__DATA("Abort");

				// ...
				{
					int alarm_id = ALID__SWAP_ERROR;
					CString alarm_msg;
					CString r_act;

					alarm_msg.Format("Swap Error occurred when ATM-Robot is swapping with %s.\n",para_module);

					p_alarm->Check__ALARM(alarm_id,r_act);
					p_alarm->Post__ALARM_With_MESSAGE(alarm_id,alarm_msg);
				}

				// ...
				{
					Sleep(990);

					xSCH_MATERIAL_CTRL->Clear__ALL_MATERIAL_INFO();
				}

				return 11;
			}
		}
		else
		{
			if(Fnc__PLACE_LBi__ALL_MODE(p_variable,
										p_alarm,
										ex_flag,
										arm_type,
										para_module,
										para_in_slot) < 0)
			{
				NEXT__LOOP;
			}
				
			if(pick_flag > 0)
			{
				if(Fnc__PICK_LBx(p_variable,p_alarm, false,arm_type,para_module,para_out_slot) < 0)
				{
					return -41;
				}
			}
			else
			{
				xCH__ATM_RB__EXCHANGE_LOCK_FLAG->Set__DATA(STR__YES);
				xCH__ATM_RB__EXCHANGE_LOCK_LBx->Set__DATA(para_module);
				xCH__ATM_RB__EXCHANGE_LOCK_SLOT->Set__DATA(para_out_slot);
			}
		}
	}
	else
	{
		if(Fnc__PLACE_LBi__ALL_MODE(p_variable,
									  p_alarm,
									  false,
									  arm_type,
									  para_module,
									  para_in_slot) < 0)
		{
			return -51;
		}
	}

	return 1;
}

int  CObj__DUAL_ARM_STD
::AUTO_CTRL__AL_LBi__ONLY_MODE(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm)
{
	if(ATM_RB__Get_Occupied_Arm_Count() < 1)
	{
		if(SCH__CAN_PICK_LBo() > 0)
		{
			NEXT__LOOP;
		}
	}
	if(SCH__CAN_PLACE_LBi() < 0)
	{
		NEXT__LOOP;
	}

	if(xCH_CFG__SCH_ARM_MODE->Check__DATA(STR__DUAL) > 0)
	{
		if((AL1__Check_Empty__All_Slot() > 0)
		&& (ATM_RB__Get_Occupied_Arm_Count() < 1))
		{
			NEXT__LOOP;
		}

		// ...
		int arm_count = ATM_RB__Get_Cfg_Arm_Count();

		// AL -> RB
		if(AL1__Check_Empty__All_Slot() < 0)
		{
			if(ATM_RB__Check_Empty__Arm_Type() > 0)
			{
				CString arm_type;

				if(Fnc__PICK_AL(p_variable,p_alarm, arm_type) < 0)
				{
					NEXT__LOOP;
				}
			}
		}

		// RB -> AL -> RB
		if(dCH__CFG_PRE_ALIGN_USE->Check__DATA(STR__YES) > 0)
		{
			int ll_slot_count = 0;

			for(int i=0; i<arm_count; i++)
			{
				CString arm_type;

				if(arm_count == 1)
				{
					if(ATM_RB__Get_Occupied__Arm_Type(arm_type) < 0)			continue;
				}
				else
				{
					if(ATM_RB__Get_Occupied__Arm_Type(i, arm_type) < 0)			continue;
				}

				if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(arm_type) < 0)
				{
					continue;
				}

				// ...
				{
					IDS__SCH_MATERIAL_STATUS ds_info;
					xSCH_MATERIAL_CTRL->Get__MATERIAL_STATUS(arm_type,ds_info);

					if(ds_info.sMATERIAL_STS.CompareNoCase(STR__MAPPED) != 0)
					{
						continue;
					}
				}

				if(SCH__CHECK_LBi_EMPTY() < 1)
				{
					NEXT__LOOP;
				}

				// Align_Retry ...
				{
					bool active__ll_all_mode = false;

					int r_flag = Fnc__ALIGN_RETRY_TO_PLACE_LLx(p_variable,p_alarm, active__ll_all_mode, arm_type, ll_slot_count);
					if(r_flag < 0)
					{
						printf("AUTO_CTRL__AL_LBi__ONLY_MODE() ... \n");
						printf("  * Fnc__ALIGN_RETRY_TO_PLACE_LLx() - ERROR \n");
						printf("  * r_flag <- [%1d] \n", r_flag);

						NEXT__LOOP;
					}

					ll_slot_count++;
				}
			}
		}

		// RB -> LBi
		bool active__wfr_exchange = false;

		if(dEXT_CH__CFG_LLx_CTRL_ONLY_INPUT_OUTPUT_MODE->Check__DATA(STR__DUAL) < 0)
		{
			if(arm_count > 1)
			{
				int count__arm_wfr = ATM_RB__Get_Process_Wafer_Count_On_Arm();
				int count__llx_wfr = LLx__Get_Empty__Total_InSlot();

				if(count__arm_wfr > 1)
				{
					if(count__arm_wfr <= count__llx_wfr)		active__wfr_exchange = true;
				}
			}
		}

		for(int arm_i=0; arm_i < arm_count; arm_i++)
		{
			CString arm_type;

			if(arm_count == 1)
			{
				if(ATM_RB__Get_Occupied__Arm_Type(arm_type) < 0)			continue;
			}
			else
			{
				if(ATM_RB__Get_Occupied__Arm_Type(arm_i, arm_type) < 0)		continue;
			}

			if(xCH__ATM_RB_PAUSE_FLAG->Check__DATA(STR__YES) > 0)
			{
				NEXT__LOOP;
			}

			if(SCH__CHECK_LBi_EMPTY() < 1)
			{
				NEXT__LOOP;
			}

			if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(arm_type) < 0)
			{
				continue;
			}

			if(dCH__CFG_PRE_ALIGN_USE->Check__DATA(STR__YES) > 0)
			{
				IDS__SCH_MATERIAL_STATUS ds_info;
				xSCH_MATERIAL_CTRL->Get__MATERIAL_STATUS(arm_type, ds_info);

				if(ds_info.sMATERIAL_STS.CompareNoCase(STR__ALIGNED) != 0)
				{
					continue;
				}
			}
			else
			{
				CString trg__al_name = "?";
				CString trg__al_slot = "?";

				if(xCH__ATM_RB__TARGET_PMx_MODE->Check__DATA(STR__ENABLE) > 0)
				{
					trg__al_name = mMNG__PMx_ALIGN.Get__NEXT_ALIGN(arm_type);
					trg__al_slot = "1";

					// ...
					{
						CString def_name;
						CString def_data;

						// 1.
						{
							def_name = USER_DEF__TRG_MDx;
							def_data = trg__al_name;

							xSCH_MATERIAL_CTRL->Set__MATERIAL_USER_INFO(arm_type, def_name,def_data);
						}
						// 2.
						{
							def_name = USER_DEF__TRG_SLOT;
							def_data = trg__al_slot;

							xSCH_MATERIAL_CTRL->Set__MATERIAL_USER_INFO(arm_type, def_name,def_data);
						}
					}
				}

				xCH__ATM_RB__TARGET_LLx_NAME_SET_ALL->Set__DATA(trg__al_name);
				xCH__ATM_RB__TARGET_LLx_SLOT_SET_ALL->Set__DATA(trg__al_slot);
			}

			// ...
			bool active__ex_flag = false;

			if(active__wfr_exchange)
			{
				if(arm_i < (arm_count - 1))		active__ex_flag = true;
			}

			if(Fnc__PLACE_LBi__ONLY_MODE(p_variable,p_alarm, active__ex_flag, arm_type) < 0)
			{
				NEXT__LOOP;
			}

			// ...
			{
				CString log_msg;
				CString log_bff;

				log_msg = "Fnc__PLACE_LBi__ONLY_MODE() : Completed ! \n";

				log_bff.Format(" * %s <- %s \n", 
							   xCH_CFG__SCH_ARM_MODE->Get__CHANNEL_NAME(),
							   xCH_CFG__SCH_ARM_MODE->Get__STRING());
				log_msg += log_bff;

				log_bff.Format(" * arm_type <- %s \n", arm_type);
				log_msg += log_bff;

				log_msg += " * Unpper_Function : AUTO_CTRL__AL_LBi__ONLY_MODE() \n";
				log_msg += "\n";

				xAPP_LOG_CTRL->WRITE__LOG(log_msg);
			}
		}
	}
	else
	{
		if(ATM_RB__Check_Empty__Arm_Type() > 0)
		{
			CString arm_type;
			
			if(ATM_RB__Get_Empty__Arm_Type(arm_type) < 0)
			{
				NEXT__LOOP;
			}

			// ...
			bool ex_flag = false;

			if((SCH__CHECK_LBi_EMPTY() > 1)
			&& (AL1__Get_Occupied__Total_Count() > 1))
			{
				ex_flag = true;
			}

			// AL -> ATM_RB
			if(Fnc__PICK_AL(p_variable,p_alarm, arm_type) < 0)
			{
				NEXT__LOOP;
			}

			if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(arm_type) < 0)
			{
				NEXT__LOOP;
			}

			// ATM_RB -> LBi
			if(Fnc__PLACE_LBi__ONLY_MODE(p_variable,p_alarm, ex_flag, arm_type) < 0)
			{
				NEXT__LOOP;
			}
		
			if(ex_flag == true)
			{
				// AL -> ATM_RB
				if(Fnc__PICK_AL(p_variable,p_alarm, arm_type) < 0)
				{
					NEXT__LOOP;
				}

				// ATM_RB -> LBi
				if((SCH__CHECK_LBi_EMPTY() < 2)
				|| (xSCH_MATERIAL_CTRL->Check__MATERIAL_TO_PROCESS() < 0))
				{
					if(Fnc__PLACE_LBi__ONLY_MODE(p_variable,p_alarm, false, arm_type) < 0)
					{
						NEXT__LOOP;
					}
				}
				else
				{
					if(Fnc__PLACE_LBi__ONLY_MODE(p_variable,p_alarm, true, arm_type) < 0)
					{
						NEXT__LOOP;
					}
				}
			}
		}
		else
		{
			CString arm_type;

			if(ATM_RB__Get_Occupied__Arm_Type(arm_type) < 0)
			{
				NEXT__LOOP;
			}

			if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(arm_type) < 0)
			{
				NEXT__LOOP;
			}

			// jglee : 2023.05.22
			if(dCH__CFG_PRE_ALIGN_USE->Check__DATA(STR__YES) > 0)
			{
				IDS__SCH_MATERIAL_STATUS ds_info;
				xSCH_MATERIAL_CTRL->Get__MATERIAL_STATUS(arm_type,ds_info);

				if(ds_info.sMATERIAL_STS.CompareNoCase(STR__ALIGNED) != 0)
				{
					bool active__ll_all_mode = false;
					int ll_slot_count = 0;

					int r_flag = Fnc__ALIGN_RETRY_TO_PLACE_LLx(p_variable,p_alarm, active__ll_all_mode, arm_type, ll_slot_count);
					if(r_flag < 0)
					{
						printf("AUTO_CTRL__AL_LBi__ONLY_MODE() - Single_Arm ... \n");
						printf("  * Fnc__ALIGN_RETRY_TO_PLACE_LLx() - ERROR \n");
						printf("  * r_flag <- [%1d] \n", r_flag);

						NEXT__LOOP;
					}
				}
			}
			else
			{
				CString trg__al_name = "?";
				CString trg__al_slot = "?";

				if(xCH__ATM_RB__TARGET_PMx_MODE->Check__DATA(STR__ENABLE) > 0)
				{
					trg__al_name = mMNG__PMx_ALIGN.Get__NEXT_ALIGN(arm_type);
					trg__al_slot = "1";

					// ...
					{
						CString def_name;
						CString def_data;

						// 1.
						{
							def_name = USER_DEF__TRG_MDx;
							def_data = trg__al_name;

							xSCH_MATERIAL_CTRL->Set__MATERIAL_USER_INFO(arm_type, def_name,def_data);
						}
						// 2.
						{
							def_name = USER_DEF__TRG_SLOT;
							def_data = trg__al_slot;

							xSCH_MATERIAL_CTRL->Set__MATERIAL_USER_INFO(arm_type, def_name,def_data);
						}
					}
				}

				xCH__ATM_RB__TARGET_LLx_NAME_SET_ALL->Set__DATA(trg__al_name);
				xCH__ATM_RB__TARGET_LLx_SLOT_SET_ALL->Set__DATA(trg__al_slot);
			}

			// ...
			bool ex_flag = false;

			if((SCH__CHECK_LBi_EMPTY() > 1)
			&& (AL1__Get_Occupied__Total_Count() > 0))
			{
				ex_flag = true;
			}

			// ATM_RB -> LBi
			if(Fnc__PLACE_LBi__ONLY_MODE(p_variable,p_alarm, ex_flag, arm_type) < 0)
			{
				NEXT__LOOP;
			}

			if(ATM_RB__Get_Empty__Arm_Type(arm_type) < 0)
			{
				NEXT__LOOP;
			}

			// AL -> ATM_RB
			if(Fnc__PICK_AL(p_variable,p_alarm, arm_type) < 0)
			{
				NEXT__LOOP;
			}

			// ATM_RB -> LBi
			if((SCH__CHECK_LBi_EMPTY() < 2)
			|| (xSCH_MATERIAL_CTRL->Check__MATERIAL_TO_PROCESS() < 0))
			{
				if(Fnc__PLACE_LBi__ONLY_MODE(p_variable,p_alarm, false, arm_type) < 0)
				{
					NEXT__LOOP;
				}
			}
			else
			{
				if(Fnc__PLACE_LBi__ONLY_MODE(p_variable,p_alarm, true, arm_type) < 0)
				{
					NEXT__LOOP;
				}
			}
		}
	}

	return 1;
}
int  CObj__DUAL_ARM_STD
::AUTO_CTRL__AL_LBi__ALL_MODE(CII_OBJECT__VARIABLE *p_variable,
							  CII_OBJECT__ALARM *p_alarm)
{
	// CHECK : LLx (ALL)  ...
	{
		for(int k=0; k<iLLx_SIZE; k++)
		{
			if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[k]->Check__DATA(LBx_MODE__ALL) < 0)
			{
				return -1;
			}
		}
	}

	// ...
	int material_flag = -1;

	bool ex_flag = false;
	CString para_module;
	CString para_in_slot;
	CString para_out_slot;
	int slot_id;
	int pick_flag = -1;

	// ...
	int lba_out_flag = -1;
	int lbb_out_flag = -1;

	// ...
	int active__sch_mode = -1;
	int active__sch_ll = -1;

	if(sEXT_CH__SCH_MODE_LLx_IN->Check__DATA(_SCH_MODE__ROUTE) > 0)
	{
		CString sch__ll_name;

		if(ATM_RB__Check_Empty__Arm_Type() > 0)
		{
			CString para_module = MODULE__AL1;
			int al_slot_id;

			if(AL1__Get_Occupied__SlotID(al_slot_id) > 0)
			{
				sch__ll_name.Format("%s-%1d", para_module,al_slot_id);
			}
		}

		if(sch__ll_name.GetLength() < 1)
		{
			if(ATM_RB__Get_Occupied__Arm_Type(sch__ll_name) < 0)
			{
				NEXT__LOOP;
			}
		}

		// ...
		CStringArray l_ll_id;
		CStringArray l_ll_mode;
		CStringArray l_ll_slot;

		xSCH_MATERIAL_CTRL->Get__LLx_IN_OF_EDIT_TYPE(sch__ll_name, l_ll_id,l_ll_mode,l_ll_slot);

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
	}

	// ...
	int ll_a = _LLx_INDEX__LBA;
	int ll_b = _LLx_INDEX__LBB;

	if(active__sch_mode > 0)
	{
		if(active__sch_ll == 1)
		{
			lba_out_flag = 1;
		}
		else if(active__sch_ll == 2)
		{
			lbb_out_flag = 1;
		}
	}
	else
	{
		int lba_out_count;
		int lbb_out_count;
		CString var_data;

		// LBA ...
		{
			xEXT_CH__LLx__OUT_COUNT[ll_a]->Get__DATA(var_data);
			lba_out_count = atoi(var_data);
		}
		// LBB ...
		{
			xEXT_CH__LLx__OUT_COUNT[ll_b]->Get__DATA(var_data);
			lbb_out_count = atoi(var_data);
		}

		if(lba_out_count <= lbb_out_count)
		{
			if(xEXT_CH__LLx__IN_OUT_FLAG[ll_a]->Check__DATA(STR__IN) > 0)
			{
				if((LLx__Is_ATM(ll_a) > 0)
				&& (LLx__Is_Available(ll_a) > 0)
				&& (LLx__Get_Empty__InSlot(ll_a, slot_id) > 0))
				{
					lba_out_flag = 1;
				}
			}

			if((lba_out_flag < 0)
			&& (xEXT_CH__LLx__IN_OUT_FLAG[ll_b]->Check__DATA(STR__IN) > 0))
			{
				if((LLx__Is_ATM(ll_b) > 0)
			    && (LLx__Is_Available(ll_b) > 0)
				&& (LLx__Get_Empty__InSlot(ll_b, slot_id) > 0))
				{
					lbb_out_flag = 1;
				}
			}
		}
		else
		{
			if(xEXT_CH__LLx__IN_OUT_FLAG[ll_b]->Check__DATA(STR__IN) > 0)
			{
				if((LLx__Is_ATM(ll_b) > 0)
				&& (LLx__Is_Available(ll_b) > 0)
				&& (LLx__Get_Empty__InSlot(ll_b, slot_id) > 0))
				{
					lbb_out_flag = 1;
				}
			}

			if((lbb_out_flag < 0)
			&& (xEXT_CH__LLx__IN_OUT_FLAG[ll_a]->Check__DATA(STR__IN) > 0))
			{
				if((LLx__Is_ATM(ll_a) > 0)
			    && (LLx__Is_Available(ll_a) > 0)
				&& (LLx__Get_Empty__InSlot(ll_a, slot_id) > 0))
				{
					lba_out_flag = 1;
				}
			}
		}
	}

	if(lba_out_flag > 0)
	{
		if((LLx__Is_ATM(ll_a) > 0)
		&& (LLx__Is_Available(ll_a) > 0))
		{
			if(LLx__Get_Empty__InSlot(ll_a, slot_id) > 0)
			{
				para_module = MODULE__LBA;
				para_in_slot.Format("%1d", slot_id);

				if(LLx__Get_Occupied__OutSlot(ll_a, slot_id) > 0)
				{
					para_out_slot.Format("%1d", slot_id);

					pick_flag = 1;
					ex_flag = true;
				}

				material_flag = 1;
			}
		}
	}
	if(lbb_out_flag > 0)
	{
		if((LLx__Is_ATM(ll_b) > 0)
		&& (LLx__Is_Available(ll_b) > 0))
		{
			if(LLx__Get_Empty__InSlot(ll_b, slot_id) > 0)
			{
				para_module = MODULE__LBB;
				para_in_slot.Format("%1d", slot_id);

				if(LLx__Get_Occupied__OutSlot(ll_b, slot_id) > 0)
				{
					para_out_slot.Format("%1d",slot_id);

					pick_flag = 1;
					ex_flag = true;
				}

				material_flag = 1;
			}
		}
	}

	if(material_flag < 0)
	{
		NEXT__LOOP;
	}

	if(ex_flag == true)
	{
		CString sch_name;
		sch_name.Format("%s-%s", para_module,para_out_slot);

		if(Get__LPx_SideBuffer_Info_To_Use(sch_name) < 0)
		{
			ex_flag = false;
		}
	}

	// WAFER : MAPPED -> ALIGNED ...
	{
		if((AL1__Check_Empty__All_Slot() > 0)
		&& (ATM_RB__Get_Occupied_Arm_Count() < 1))
		{
			NEXT__LOOP;
		}

		// ...
		int arm_count = ATM_RB__Get_Cfg_Arm_Count();

		// AL -> RB
		if(AL1__Check_Empty__All_Slot() < 0)
		{
			if(ATM_RB__Check_Empty__Arm_Type() > 0)
			{
				CString arm_type;

				if(Fnc__PICK_AL(p_variable,p_alarm, arm_type) < 0)
				{
					NEXT__LOOP;
				}
			}
		}

		// RB -> AL -> RB
		if(dCH__CFG_PRE_ALIGN_USE->Check__DATA(STR__YES) > 0)
		{
			int ll_slot_count = 0;

			for(int i=0; i<arm_count; i++)
			{
				CString arm_type;

				// jglee : 2023.05.22
				if(arm_count == 1)
				{
					if(ATM_RB__Get_Occupied__Arm_Type(arm_type) < 0)
					{
						continue;
					}
				}
				else
				{
					if(ATM_RB__Get_Occupied__Arm_Type(i, arm_type) < 0)
					{
						continue;
					}
				}

				if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(arm_type) < 0)
				{
					continue;
				}

				// ...
				{
					IDS__SCH_MATERIAL_STATUS ds_info;
					xSCH_MATERIAL_CTRL->Get__MATERIAL_STATUS(arm_type,ds_info);

					if(ds_info.sMATERIAL_STS.CompareNoCase(STR__MAPPED) != 0)
					{
						continue;
					}
				}
	
				/*
				if(SCH__CHECK_LBi_EMPTY() < 1)
				{
					NEXT__LOOP;
				}
				*/

				// Align_Retry ...
				{
					bool active__ll_all_mode = true;

					int r_flag = Fnc__ALIGN_RETRY_TO_PLACE_LLx(p_variable,p_alarm, active__ll_all_mode, arm_type, ll_slot_count);
					if(r_flag < 0)
					{
						printf("AUTO_CTRL__AL_LBi__ALL_MODE() ... \n");
						printf("  * Fnc__ALIGN_RETRY_TO_PLACE_LLx() - ERROR \n");
						printf("  * r_flag <- [%1d] \n", r_flag);

						NEXT__LOOP;
					}

					ll_slot_count++;
				}
			}
		}

		// ...
	}

	// ...
	CString pick_arm;
	
	// ...
	{
		bool active__al_wafer = false;

		if(ATM_RB__Check_Empty__Arm_Type() > 0)
		{
			if((AL1__Is_Available() > 0)
			&& (AL1__Check_Empty__All_Slot() < 0))
			{
				active__al_wafer = true;
			}
		}

		if(active__al_wafer)
		{
			try
			{
				Fnc__PICK_AL(p_variable,p_alarm, pick_arm);

				// CHECK : ARM.RESERVE ...
				{
					CString arm__ll_name;

						 if(pick_arm.CompareNoCase(ARM_A) == 0)			arm__ll_name = xCH__ATM_RB__TARGET_LLx_NAME_SET_OF_ARM_A->Get__STRING();
					else if(pick_arm.CompareNoCase(ARM_B) == 0)			arm__ll_name = xCH__ATM_RB__TARGET_LLx_NAME_SET_OF_ARM_B->Get__STRING();

					int ll_i = Get__LLx_INDEX(arm__ll_name);
					if(ll_i >= 0)		dCH__ATM_RB__INFO_LL_RESERVE_X[ll_i]->Set__DATA(STR__ON);
				}
			}
			catch(int err_flag)
			{
				NEXT__LOOP;
			}
		}
		else 
		{
			CString arm_type;

			if(ATM_RB__Get_Occupied__Arm_Type(arm_type) < 0)
			{
				NEXT__LOOP;
			}

			if(dCH__CFG_PRE_ALIGN_USE->Check__DATA(STR__YES) > 0)
			{
				IDS__SCH_MATERIAL_STATUS ds_info;
			
				if(xSCH_MATERIAL_CTRL->Get__MATERIAL_STATUS(arm_type,ds_info) < 0)
				{
					NEXT__LOOP;
				}
				if(ds_info.sMATERIAL_STS.CompareNoCase(STR__ALIGNED) != 0)
				{
					NEXT__LOOP;
				}
			}
		
			pick_arm = arm_type;
		}
	
		if(pick_arm.GetLength() < 1)
		{
			NEXT__LOOP;
		}
		
		if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(pick_arm) < 0)
		{
			NEXT__LOOP;
		}
	}
	
	if(Seq__LBx_VENT__INTO_LBi(p_variable) > 0)
	{
		NEXT__LOOP;
	}

	if(ex_flag == true)
	{
		if((xCH_CFG__LBx_SWAP_FLAG->Check__DATA(STR__ENABLE) > 0)
		&& (pick_flag > 0))
		{
			CString sch_place_module;
			CString sch_pick_module;

			sch_place_module.Format("%s-%s",para_module,para_in_slot );
			sch_pick_module.Format( "%s-%s",para_module,para_out_slot);

			if(Fnc__SWAP_LBx(p_variable,
							 p_alarm,
							 para_module,
							 pick_arm,
							 para_in_slot,
							 pick_arm,
							 para_out_slot,
							 sch_place_module,
							 sch_pick_module) < 0)
			{
				xEXT_CH__JOB_CTRL->Set__DATA("Abort");

				// ...	
				int alarm_id = ALID__SWAP_ERROR;
				CString alarm_msg;
				CString r_act;

				alarm_msg.Format("Swap Error occured when ATM-Robot is swaping with %s.\n",para_module);

				p_alarm->Check__ALARM(alarm_id,r_act);
				p_alarm->Post__ALARM_With_MESSAGE(alarm_id,alarm_msg);

				Sleep(990);
				xSCH_MATERIAL_CTRL->Clear__ALL_MATERIAL_INFO();

				NEXT__LOOP;
			}
		}
		else
		{
			if(Fnc__PLACE_LBi__ALL_MODE(p_variable,
									    p_alarm,
									    ex_flag,
									    pick_arm,
									    para_module,
									    para_in_slot) < 0)
			{
				NEXT__LOOP;
			}
				
			if(pick_flag > 0)
			{
				if(Fnc__PICK_LBx(p_variable,p_alarm, false,pick_arm,para_module,para_out_slot) < 0)
				{
					NEXT__LOOP;
				}
			}
			else
			{
				xCH__ATM_RB__EXCHANGE_LOCK_FLAG->Set__DATA(STR__YES);
				xCH__ATM_RB__EXCHANGE_LOCK_LBx->Set__DATA(para_module);
				xCH__ATM_RB__EXCHANGE_LOCK_SLOT->Set__DATA(para_out_slot);
			}
		}
	}
	else
	{
		if(Fnc__PLACE_LBi__ALL_MODE(p_variable,
								    p_alarm,
								    false,
								    pick_arm,
								    para_module,
								    para_in_slot) < 0)
		{
			NEXT__LOOP;
		}
	}

	return 1;
}
int  CObj__DUAL_ARM_STD
::AUTO_CTRL__Retry_ALIGN(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm, const bool active__ll_all_mode)
{
	if(AL1__Is_Available()    < 0)			NEXT__LOOP;
	if(ATM_RB__Is_Available() < 0)			NEXT__LOOP;

	// ...
	{
		CString arm_type;
		Fnc__PICK_AL(p_variable,p_alarm, arm_type);

		int ll_slot_count = 0;
		Fnc__ALIGN_RETRY_TO_PLACE_LLx(p_variable,p_alarm, active__ll_all_mode, arm_type, ll_slot_count);
	}

	xCH__AL1__ALIGN_RETRY_FLAG->Set__DATA("");
	return 1;
}

int  CObj__DUAL_ARM_STD
::_Check__ARM_LIST_TO_PLACE_LPx()
{
	bool active__dual_arm;
	CStringArray l__arm_type;

	return _Get__ARM_LIST_TO_PLACE_LPx(active__dual_arm, l__arm_type);
}
int  CObj__DUAL_ARM_STD
::_Get__ARM_LIST_TO_PLACE_LPx(CStringArray& l__arm_type)
{
	bool active__dual_arm;

	return _Get__ARM_LIST_TO_PLACE_LPx(active__dual_arm, l__arm_type);
}
int  CObj__DUAL_ARM_STD
::_Get__ARM_LIST_TO_PLACE_LPx(bool& active__dual_arm, CStringArray& l__arm_type)
{
	active__dual_arm = false;
	l__arm_type.RemoveAll();

	// ...
	int arm_count = 1;

	if(xCH_CFG__SCH_ARM_MODE->Check__DATA(STR__DUAL) > 0)
	{
		arm_count = ATM_RB__Get_Cfg_Arm_Count();
	}

	for(int k=0; k<arm_count; k++)
	{
		CString arm_type;

		if(arm_count == 1)
		{
			if(ATM_RB__Get_Occupied__Arm_Type(arm_type) < 0)			continue;
		}
		else
		{
			if(ATM_RB__Get_Occupied__Arm_Type(k, arm_type) < 0)			continue;
		}

		if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(arm_type) > 0)
		{
			continue;
		}

		// ...
		CString arm__wfr_sts;

			 if(arm_type.CompareNoCase(ARM_A) == 0)			arm__wfr_sts = xCH__ATM_RB__SLOT01_STATUS->Get__STRING();
		else if(arm_type.CompareNoCase(ARM_B) == 0)			arm__wfr_sts = xCH__ATM_RB__SLOT02_STATUS->Get__STRING();

		if((arm__wfr_sts.CompareNoCase(STR__EXIST)  == 0) 
		|| (arm__wfr_sts.CompareNoCase(STR__MAPPED) == 0))
		{
			xSCH_MATERIAL_CTRL->Set__MATERIAL_STATUS(arm_type, STR__TO_LP);
		}
		else
		{		
			if(xCH__SCH_DB_STx_APPLY_MODE->Check__DATA(STR__ROUTE) > 0)
			{
				int check_flag = -1;

				if(xSCH_MATERIAL_CTRL->Check__EDIT_TYPE__STATE() > 0)
				{
					CStringArray l_id;
					CStringArray l_mode;
					CStringArray l_slot; 
					CStringArray l_sec;

					if(xSCH_MATERIAL_CTRL->Get__STx_OF_EDIT_TYPE(arm_type, l_id,l_mode,l_slot,l_sec) > 0)
					{
						if(l_id.GetSize() > 0)			check_flag = 1;
					}
				}
				else
				{
					CString bff1_info;
					CString bff2_info;

					xSCH_MATERIAL_CTRL->Get__BUFFERx_INFO(arm_type, bff1_info,bff2_info);

					if((bff1_info.CompareNoCase(STR__YES) == 0)
					|| (bff2_info.CompareNoCase(STR__YES) == 0))
					{
						check_flag = 1;
					}
				}

				if(check_flag > 0)
				{
					IDS__SCH_MATERIAL_STATUS ds_info;

					if(xSCH_MATERIAL_CTRL->Get__MATERIAL_STATUS(arm_type, ds_info) < 0)			continue;;
					if(ds_info.sMATERIAL_STS.CompareNoCase(STR__TO_LP) != 0)					continue;;
				}
			}
			else
			{
				IDS__SCH_MATERIAL_STATUS ds_info;

				if(xSCH_MATERIAL_CTRL->Get__MATERIAL_STATUS(arm_type,ds_info) < 0)			continue;
				if(ds_info.sMATERIAL_STS.CompareNoCase(STR__TO_LP) != 0)					continue;
			}
		}

		l__arm_type.Add(arm_type);
	}

	if((dCH__ATM_RB__CFG_DUAL_ARM_MOVING_AT_THE_SAME_TIME->Check__DATA(STR__ENABLE) > 0)
	&& (dCH__ATM_RB__CFG_DUAL_ARM_MOVING_AT_LPx->Check__DATA(STR__YES) > 0))
	{
		CString arm_type;

		if(l__arm_type.GetSize() > 1)
		{
			int ptn_id__1st = 0;
			int stn_id__1st = 0;

			int ptn_id__2nd = 0;
			int stn_id__2nd = 0;

			// 1st Wafer ...
			{
				arm_type = l__arm_type[0];

				IDS__SCH_MATERIAL_STATUS ds_info;
				xSCH_MATERIAL_CTRL->Get__MATERIAL_STATUS(arm_type, ds_info);

				ptn_id__1st = ds_info.iPORT_ID;
				stn_id__1st = ds_info.iSLOT_ID;
			}
			// 2nd Wafer ...
			{
				arm_type = l__arm_type[1];

				IDS__SCH_MATERIAL_STATUS ds_info;
				xSCH_MATERIAL_CTRL->Get__MATERIAL_STATUS(arm_type, ds_info);

				ptn_id__2nd = ds_info.iPORT_ID;
				stn_id__2nd = ds_info.iSLOT_ID;
			}

			if((ptn_id__1st == ptn_id__2nd)
			&& ((stn_id__1st + 1) == stn_id__2nd))
			{
				active__dual_arm = true;
			}
		}
	}

	if(l__arm_type.GetSize() > 0)			return 1;
	return -1;
}
int  CObj__DUAL_ARM_STD
::AUTO_CTRL__RB_LPo(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm)
{
	CStringArray l__arm_type;
	bool active__dual_arm = false;

	if(_Get__ARM_LIST_TO_PLACE_LPx(active__dual_arm, l__arm_type) < 0)
	{
		NEXT__LOOP;
	}

	if(active__dual_arm)
	{
		CString arm_type = ARM_AB;

		if(Fnc__PLACE_LPx(p_variable,p_alarm, arm_type) < 0)
		{
			NEXT__LOOP;
		}
	}
	else
	{
		int k_limit = l__arm_type.GetSize();
		int k;

		for(k=0; k<k_limit; k++)
		{
			CString arm_type = l__arm_type[k];

			if(Fnc__PLACE_LPx(p_variable,p_alarm, arm_type) < 0)
			{
				NEXT__LOOP;
			}
		}
	}

	return 1;
}

int  CObj__DUAL_ARM_STD
::_Check__SAME_PTN_IN_STx(const CString& stx_name,const int stx_id, const int port_id)
{
	// ST1
	if(stx_name.CompareNoCase(MODULE__BUFFER1) == 0)
	{
		if(sCH__SCH_DB_ST1_USE_FLAG->Check__DATA(STR__ENABLE) < 0)
		{
			return -11;
		}

		// ...
		CUIntArray l__st_slot_id;

		if(Buffer1__Get_Occupied__Slot_To_Process(l__st_slot_id) < 0)
		{
			return -12;
		}

		// ...
		int t_limit = l__st_slot_id.GetSize();

		for(int t=0; t<t_limit; t++)
		{
			int slot_id = l__st_slot_id[t];
			if(slot_id == stx_id)						continue;

			// ...
			CString sch_name;
			sch_name.Format("%s-%1d", stx_name,slot_id);

			IDS__SCH_MATERIAL_INFO ds_info;
			xSCH_MATERIAL_CTRL->Get__MATERIAL_INFO(sch_name, ds_info);

			if(ds_info.iSRC__PTN == port_id)			return 1;
		}

		return -13;
	}

	// ST2
	if(stx_name.CompareNoCase(MODULE__BUFFER2) == 0)
	{
		if(sCH__SCH_DB_ST2_USE_FLAG->Check__DATA(STR__ENABLE) < 0)
		{
			return -21;
		}

		// ...
		CUIntArray l__st_slot_id;

		if(Buffer2__Get_Occupied__Slot_To_Process(l__st_slot_id) < 0)
		{
			return -22;
		}

		// ...
		int t_limit = l__st_slot_id.GetSize();

		for(int t=0; t<t_limit; t++)
		{
			int slot_id = l__st_slot_id[t];
			if(slot_id == stx_id)						continue;

			// ...
			CString sch_name;
			sch_name.Format("%s-%1d", stx_name,slot_id);

			IDS__SCH_MATERIAL_INFO ds_info;
			xSCH_MATERIAL_CTRL->Get__MATERIAL_INFO(sch_name, ds_info);

			if(ds_info.iSRC__PTN == port_id)			return 1;
		}

		return -23;
	}

	return -1;
}

int  CObj__DUAL_ARM_STD
::_Check__SLOT_COUNT_TO_PICK_STx(const double ref_sec, const int max_check)
{
	int count__slot_check = 0;

	// ST1
	if(sCH__SCH_DB_ST1_USE_FLAG->Check__DATA(STR__ENABLE) > 0)
	{
		CUIntArray l__st_slot_id;

		if(Buffer1__Get_Occupied__Slot_To_Process(l__st_slot_id) < 0)
		{
			return -11;
		}

		// ...
		int t_limit = l__st_slot_id.GetSize();

		for(int t=0; t<t_limit; t++)
		{
			int slot_id = l__st_slot_id[t];

			if(Buffer1__Check_Wait_Sec(slot_id, ref_sec) > 0)			continue;

			count__slot_check++;
			if(count__slot_check >= max_check)			return 11;
		}
	}

	// ST2
	if(sCH__SCH_DB_ST2_USE_FLAG->Check__DATA(STR__ENABLE) > 0)
	{
		CUIntArray l__st_slot_id;

		if(Buffer2__Get_Occupied__Slot_To_Process(l__st_slot_id) < 0)
		{
			return -12;
		}

		// ...
		int t_limit = l__st_slot_id.GetSize();

		for(int t=0; t<t_limit; t++)
		{
			int slot_id = l__st_slot_id[t];

			if(Buffer2__Check_Wait_Sec(slot_id, ref_sec) > 0)			continue;

			count__slot_check++;
			if(count__slot_check >= max_check)			return 12;
		}
	}

	return -1;
}
int  CObj__DUAL_ARM_STD
::_Get__ARM_LIST_TO_PICK_STx(CStringArray& l__stx_name, 
							 CUIntArray& l__stx_slot)
{
	l__stx_name.RemoveAll();
	l__stx_slot.RemoveAll();

	// STx Check ...
	{
		CStringArray l__check_name;
		CUIntArray   l__check_slot;

		// ST1
		if(sCH__SCH_DB_ST1_USE_FLAG->Check__DATA(STR__ENABLE) > 0)
		{
			CString check_name = MODULE__BUFFER1;
	
			// ...
			CUIntArray l__st_slot_id;
			Buffer1__Get_Occupied__Slot_To_Process(l__st_slot_id);

			int t_limit = l__st_slot_id.GetSize();					
			for(int t=0; t<t_limit; t++)
			{
				int slot_id = l__st_slot_id[t];

				if(Buffer1__Check_Wait_Sec(slot_id) > 0)			continue;

				l__check_name.Add(check_name);
				l__check_slot.Add(slot_id);
			}
		}

		// ST2
		if(sCH__SCH_DB_ST2_USE_FLAG->Check__DATA(STR__ENABLE) > 0)
		{
			CString check_name = MODULE__BUFFER2;

			// ...
			CUIntArray l__st_slot_id;
			Buffer2__Get_Occupied__Slot_To_Process(l__st_slot_id);

			int t_limit = l__st_slot_id.GetSize();
			for(int t=0; t<t_limit; t++)
			{
				int slot_id = l__st_slot_id[t];

				if(Buffer2__Check_Wait_Sec(slot_id) > 0)			continue;

				l__check_name.Add(check_name);
				l__check_slot.Add(slot_id);
			}
		}

		int k_limit = l__check_name.GetSize();		
		for(int k=0; k<k_limit; k++)
		{
			CString bff_name = l__check_name[k];
			int slot_id = l__check_slot[k];

			CString bff_slot;
			bff_slot.Format("%1d", slot_id);

			CString sch_module;
			sch_module.Format("%s-%s", bff_name,bff_slot);

			// ...
			{
				CString para__out_mode = "";
				int active__lpx_empty = -1;
				int active__lpx_opt   = -1;
				int active__lpx_time  = -1;

				if(bff_name.CompareNoCase(MODULE__BUFFER1) == 0)
				{
					para__out_mode = dCH__SCH_DB_ST1_OUT_MODE_BUFFER_TO_LPo->Get__STRING();
				}
				else if(bff_name.CompareNoCase(MODULE__BUFFER2) == 0)
				{
					para__out_mode = dCH__SCH_DB_ST2_OUT_MODE_BUFFER_TO_LPo->Get__STRING();
				}
				else
				{
					continue;
				}

				if(para__out_mode.CompareNoCase(STR__LPx_EMPTY) == 0)					active__lpx_empty = 1;
				if(para__out_mode.CompareNoCase(STR__LPx_OPTIMIZATION) == 0)			active__lpx_opt   = 1;
				if(para__out_mode.CompareNoCase(STR__LPx_TIME)  == 0)					active__lpx_time  = 1;

				if((active__lpx_empty > 0)
				|| (active__lpx_opt   > 0))
				{
					IDS__SCH_MATERIAL_INFO ds_info;
					xSCH_MATERIAL_CTRL->Get__MATERIAL_INFO(sch_module, ds_info);

					if(xSCH_MATERIAL_CTRL->Check__MATERIAL_TO_PROCESS_IN_LPx(ds_info.iSRC__PTN) > 0)
					{
						continue;
					}
				}
				else if(active__lpx_time > 0)
				{
					IDS__SCH_MATERIAL_INFO ds_info;
					xSCH_MATERIAL_CTRL->Get__MATERIAL_INFO(sch_module, ds_info);

					if(xSCH_MATERIAL_CTRL->Check__MATERIAL_TO_PROCESS_IN_LPx(ds_info.iSRC__PTN) > 0)
					{
						int check_skip = -1;

						if(bff_name.CompareNoCase(MODULE__BUFFER1) == 0)
						{
							if(Buffer1__Check_Empty__Any_Slot() < 0)		check_skip = 1;
						}
						else if(bff_name.CompareNoCase(MODULE__BUFFER2) == 0)
						{
							if(Buffer2__Check_Empty__Any_Slot() < 0)		check_skip = 1;
						}

						if(check_skip < 0)			continue;
					}
				}
			}

			l__stx_name.Add(bff_name);
			l__stx_slot.Add(slot_id);
		}
	}

	if(l__stx_name.GetSize() > 0)			return 1;
	return -1;
}
int  CObj__DUAL_ARM_STD
::AUTO_CTRL__BUFFERx_RB(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm)
{
	CString log_id = "AUTO_CTRL__BUFFERx_RB()";

	// ...
	if(dCH__ATM_RB__CFG_PICK_WAFER_CONDITION->Check__DATA(STR__ONLY_PROCESSED) > 0)
	{
		if(ATM_RB__Check_Occupied__A_Arm() > 0)
		{
			if(xCH__ATM_RB__SLOT01_STATUS->Check__DATA(STR__MAPPED) > 0)			NEXT__LOOP;
		}
		if(ATM_RB__Check_Occupied__B_Arm() > 0)
		{
			if(xCH__ATM_RB__SLOT02_STATUS->Check__DATA(STR__MAPPED) > 0)			NEXT__LOOP;
		}
	}

	// Arm Check ...
	{
		if(ATM_RB__Check_Occupied__A_Arm() > 0)			NEXT__LOOP;
		if(ATM_RB__Check_Occupied__B_Arm() > 0)			NEXT__LOOP;
	}

	// ...
	CStringArray l__stx_name;
	CUIntArray   l__stx_slot;

	if(_Get__ARM_LIST_TO_PICK_STx(l__stx_name, l__stx_slot) < 0)
	{
		NEXT__LOOP;
	}

	// ...
	int arm_count = 1;

	if(xCH_CFG__SCH_ARM_MODE->Check__DATA(STR__DUAL) > 0)
	{
		arm_count = ATM_RB__Get_Cfg_Arm_Count();
	}

	// ...
	bool active__dual_arm = false;

	if((dCH__ATM_RB__CFG_DUAL_ARM_MOVING_AT_THE_SAME_TIME->Check__DATA(STR__ENABLE) > 0)
	&& (dCH__ATM_RB__CFG_DUAL_ARM_MOVING_AT_STx->Check__DATA(STR__YES) > 0)
	&& (arm_count > 1))
	{
		if(l__stx_name.GetSize() > 1)
		{
			CString ptn_str__1st;
			int stn_id__1st = 0;

			CString ptn_str__2nd;
			int stn_id__2nd = 0;

			// 1st Wafer ...
			{
				ptn_str__1st = l__stx_name[0];
				stn_id__1st  = l__stx_slot[0];
			}
			// 2nd Wafer ...
			{
				ptn_str__2nd = l__stx_name[1];
				stn_id__2nd  = l__stx_slot[1];
			}

			// ...
			bool active__array_reverse = false;

			int dif__stn_id = stn_id__2nd - stn_id__1st;
			if(dif__stn_id < 0)
			{
				dif__stn_id = -dif__stn_id;

				active__array_reverse = true;
			}

			if((ptn_str__1st.CompareNoCase(ptn_str__2nd) == 0)
			&& (dif__stn_id == 1))
			{
				active__dual_arm = true;

				if(active__array_reverse)
				{
					l__stx_name.RemoveAll();
					l__stx_slot.RemoveAll();

					l__stx_name.Add(ptn_str__2nd);
					l__stx_slot.Add(stn_id__2nd);

					l__stx_name.Add(ptn_str__1st);
					l__stx_slot.Add(stn_id__1st);
				}
			}
		}
		else
		{
			if(l__stx_name.GetSize() < 1)			NEXT__LOOP;

			// ...
			{
				CString stx_name = l__stx_name[0];
				int stx_id = l__stx_slot[0];

				CString stx_sch;
				stx_sch.Format("%s-%1d", stx_name,stx_id);

				IDS__SCH_MATERIAL_INFO stx_info;
				xSCH_MATERIAL_CTRL->Get__MATERIAL_INFO(stx_sch, stx_info);
				
				int port_id = stx_info.iSRC__PTN;

				if(_Check__SAME_PTN_IN_STx(stx_name,stx_id, port_id) > 0)
				{
					double ref_sec = 1.0;

					if(_Check__SLOT_COUNT_TO_PICK_STx(ref_sec, 2) > 0)			NEXT__LOOP;
				}
			}
		}
	}

	if(active__dual_arm)
	{
		// ...
		{
			CString arm_type = ARM_AB;

			CString bff_name = l__stx_name[0];
			int slot_id = l__stx_slot[0];

			CString bff_slot;
			bff_slot.Format("%1d", slot_id);

			CString sch_name;
			sch_name.Format("%s-%s", bff_name,bff_slot);

			if(SCH__PICK_MODULE(p_variable,p_alarm, log_id, false, arm_type, bff_name,bff_slot, sch_name) < 0)
			{
				NEXT__LOOP;
			}
		}

		// ...
		int k_limit = l__stx_name.GetSize();
		if(k_limit > 2)			k_limit = 2;

		for(int k=0; k<k_limit; k++)
		{
			CString arm_type;

				 if(k == 0)			arm_type = ARM_A;
			else if(k == 1)			arm_type = ARM_B;
			else					continue;

			CString bff_name = l__stx_name[k];
			int slot_id = l__stx_slot[k];

			CString bff_slot;
			bff_slot.Format("%1d", slot_id);

			CString sch_name;
			sch_name.Format("%s-%s", bff_name,bff_slot);

			if(ATM_RB__Check_Occupied__Arm_Type(arm_type) > 0)
			{
				xSCH_MATERIAL_CTRL->Pick__From_MODULE(sch_name, arm_type);
			}
		}
	}
	else
	{
		int pick_count = 0;

		int k_limit = l__stx_name.GetSize();
		for(int k=0; k<k_limit; k++)
		{
			CString bff_name = l__stx_name[k];
			int slot_id = l__stx_slot[k];

			CString bff_slot;
			bff_slot.Format("%1d", slot_id);

			CString sch_name;
			sch_name.Format("%s-%s", bff_name,bff_slot);

			if(pick_count >= arm_count)
			{
				NEXT__LOOP;
			}

			// ...
			CString arm_type;

			if(ATM_RB__Get_Empty__Arm_Type(arm_type) < 0)
			{
				NEXT__LOOP;
			}

			if(SCH__PICK_MODULE(p_variable,p_alarm, log_id, false, arm_type, bff_name,bff_slot, sch_name) < 0)
			{
				NEXT__LOOP;
			}

			if(ATM_RB__Check_Occupied__Arm_Type(arm_type) > 0)
			{
				xSCH_MATERIAL_CTRL->Pick__From_MODULE(sch_name, arm_type);
			}

			pick_count++;
		}
	}

	return 1;
}

int  CObj__DUAL_ARM_STD
::_Get__ARM_LIST_TO_PLACE_STx(bool& active__dual_arm, 
							  CStringArray& l__arm_type,
							  CStringArray& l__arm_lotid,
							  CStringArray& l__stx_name, 
							  CUIntArray& l__stx_slot)
{
	active__dual_arm = false;

	l__arm_type.RemoveAll();
	l__arm_lotid.RemoveAll();
	l__stx_name.RemoveAll();
	l__stx_slot.RemoveAll();

	// ...
	CString arm_type;
	CString arm_lotid;
	CString arm_wfr_sts;

	for(int i=0; i<2; i++)
	{
		if(i == 0)
		{
			if(ATM_RB__Check_Occupied__A_Arm() < 0)			continue;

			arm_wfr_sts = xCH__ATM_RB__SLOT01_STATUS->Get__STRING();
			arm_type = ARM_A;
		}
		else if(i == 1)
		{
			if(ATM_RB__Check_Occupied__B_Arm() < 0)			continue;

			arm_wfr_sts = xCH__ATM_RB__SLOT02_STATUS->Get__STRING();
			arm_type = ARM_B;
		}
		else
		{
			continue;
		}

		if((arm_wfr_sts.CompareNoCase(STR__EXIST)  == 0) 
		|| (arm_wfr_sts.CompareNoCase(STR__MAPPED) == 0))
		{
			continue;
		}

		if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(arm_type) > 0)
		{
			continue;
		}

		// ...
		{
			IDS__SCH_MATERIAL_STATUS ds_sts;
			xSCH_MATERIAL_CTRL->Get__MATERIAL_STATUS(arm_type, ds_sts);

			if(ds_sts.sMATERIAL_STS.CompareNoCase(STR__TO_LP) == 0)			continue;
		}

		// ...
		{
			IDS__SCH_MATERIAL_INFO ds_info;
			xSCH_MATERIAL_CTRL->Get__MATERIAL_INFO(arm_type,ds_info);

			arm_lotid = ds_info.sLOT_ID;
		}

		if(xCH__SCH_DB_STx_APPLY_MODE->Check__DATA(STR__ROUTE) > 0)
		{
			if(xSCH_MATERIAL_CTRL->Check__EDIT_TYPE__STATE() > 0)
			{
				CStringArray l_id;
				CStringArray l_mode;
				CStringArray l_slot; 
				CStringArray l_sec;

				xSCH_MATERIAL_CTRL->Get__STx_OF_EDIT_TYPE(arm_type, l_id,l_mode,l_slot,l_sec);

				if(l_id.GetSize() < 1)			continue;
			}
			else
			{
				CString bff1_info;
				CString bff2_info;

				xSCH_MATERIAL_CTRL->Get__BUFFERx_INFO(arm_type, bff1_info,bff2_info);

				if((bff1_info.CompareNoCase(STR__YES) != 0)
				&& (bff2_info.CompareNoCase(STR__YES) != 0))
				{
					continue;
				}
			}
		}

		// ...
		int buffer_flag = -1;
		CString bff_module;
		CUIntArray l__bff_slot;

		if(arm_lotid.GetLength() > 0)
		{
			CString bff1_lotid = Buffer1__Get_LotID();
			CString bff2_lotid = Buffer2__Get_LotID();

			if(buffer_flag < 0)
			{
				if(arm_lotid.CompareNoCase(bff1_lotid) == 0)
				{
					if(sCH__SCH_DB_ST1_USE_FLAG->Check__DATA(STR__ENABLE) > 0)
					{
						if(Buffer1__Get_Empty__Slot_To_Process(l__bff_slot) > 0)
						{
							buffer_flag = 1;
							bff_module = MODULE__BUFFER1;
						}
					}
				}
			}
			if(buffer_flag < 0)
			{
				if(arm_lotid.CompareNoCase(bff2_lotid) == 0)
				{
					if(sCH__SCH_DB_ST2_USE_FLAG->Check__DATA(STR__ENABLE) > 0)
					{
						if(Buffer2__Get_Empty__Slot_To_Process(l__bff_slot) > 0)
						{
							buffer_flag = 1;
							bff_module = MODULE__BUFFER2;
						}
					}
				}
			}

			if(buffer_flag < 0)
			{
				if(bff1_lotid.GetLength() < 1)
				{
					if(sCH__SCH_DB_ST1_USE_FLAG->Check__DATA(STR__ENABLE) > 0)
					{
						if(Buffer1__Get_Empty__Slot_To_Process(l__bff_slot) > 0)
						{
							buffer_flag = 1;
							bff_module = MODULE__BUFFER1;
						}
					}
				}
			}
			if(buffer_flag < 0)
			{
				if(bff2_lotid.GetLength() < 1)
				{
					if(sCH__SCH_DB_ST2_USE_FLAG->Check__DATA(STR__ENABLE) > 0)
					{
						if(Buffer2__Get_Empty__Slot_To_Process(l__bff_slot) > 0)
						{
							buffer_flag = 1;
							bff_module = MODULE__BUFFER2;
						}
					}
				}
			}
		}

		if(buffer_flag < 0)
		{
			if(sCH__SCH_DB_ST1_USE_FLAG->Check__DATA(STR__ENABLE) > 0)
			{
				if(Buffer1__Get_Empty__Slot_To_Process(l__bff_slot) > 0)
				{
					buffer_flag = 1;
					bff_module = MODULE__BUFFER1;
				}
			}
		}
		if(buffer_flag < 0)
		{
			if(sCH__SCH_DB_ST2_USE_FLAG->Check__DATA(STR__ENABLE) > 0)
			{
				if(Buffer2__Get_Empty__Slot_To_Process(l__bff_slot) > 0)
				{
					buffer_flag = 1;
					bff_module = MODULE__BUFFER2;
				}
			}
		}

		if(buffer_flag < 0)			continue;

		// ...
		l__arm_type.Add(arm_type);
		l__arm_lotid.Add(arm_lotid);

		if(Macro__Check_Equal__Any_String(bff_module, l__stx_name) > 0)
		{
			bool active__slot_err = true;

			int kk_limit = l__bff_slot.GetSize();
			for(int kk=0; kk<kk_limit; kk++)
			{
				int cmp_data = l__bff_slot[kk];

				if(Macro__Check_Equal__Any_Integer(cmp_data, l__stx_slot) > 0)			continue;

				l__stx_name.Add(bff_module);
				l__stx_slot.Add(cmp_data);

				active__slot_err = false;
				break;
			}

			if(active__slot_err)				return -101;
		}
		else
		{
			l__stx_name.Add(bff_module);

			if(l__bff_slot.GetSize() > 0)		l__stx_slot.Add(l__bff_slot[0]);
			else								return -102;
		}
	}

	if((dCH__ATM_RB__CFG_DUAL_ARM_MOVING_AT_THE_SAME_TIME->Check__DATA(STR__ENABLE) > 0)
	&& (dCH__ATM_RB__CFG_DUAL_ARM_MOVING_AT_STx->Check__DATA(STR__YES) > 0))
	{
		if(l__arm_type.GetSize() > 1)
		{
			CString ptn_str__1st;
			int stn_id__1st = 0;

			CString ptn_str__2nd;
			int stn_id__2nd = 0;

			// ...
			bool active__up_to_bottom = false;

			if(sCH__SCH_DB_ST1_USE_FLAG->Check__DATA(STR__ENABLE) > 0)
			{
				if(xCH__SCH_DB_ST1_WAFER_PICK_MODE->Check__DATA(STR__UP_TO_BOTTOM) > 0)
					active__up_to_bottom = true;
			}
			if(sCH__SCH_DB_ST2_USE_FLAG->Check__DATA(STR__ENABLE) > 0)
			{
				if(xCH__SCH_DB_ST2_WAFER_PICK_MODE->Check__DATA(STR__UP_TO_BOTTOM) > 0)
					active__up_to_bottom = true;
			}

			if(active__up_to_bottom)
			{
				ptn_str__1st = l__stx_name[1];
				stn_id__1st  = l__stx_slot[1];

				ptn_str__2nd = l__stx_name[0];
				stn_id__2nd  = l__stx_slot[0];

				l__stx_name.RemoveAll();
				l__stx_slot.RemoveAll();

				l__stx_name.Add(ptn_str__1st);
				l__stx_slot.Add(stn_id__1st);

				l__stx_name.Add(ptn_str__2nd);
				l__stx_slot.Add(stn_id__2nd);
			}

			// 1st Wafer ...
			{
				ptn_str__1st = l__stx_name[0];
				stn_id__1st  = l__stx_slot[0];
			}
			// 2nd Wafer ...
			{
				ptn_str__2nd = l__stx_name[1];
				stn_id__2nd  = l__stx_slot[1];
			}

			if((ptn_str__1st == ptn_str__2nd)
			&& ((stn_id__1st + 1) == stn_id__2nd))
			{
				active__dual_arm = true;
			}
		}
	}

	if(l__arm_type.GetSize() > 0)			return 1;
	return -1;
}
int  CObj__DUAL_ARM_STD
::AUTO_CTRL__RB_BUFFERx(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm)
{
	CStringArray l__arm_type;
	CStringArray l__arm_lotid;
	CStringArray l__stx_name;
	CUIntArray   l__stx_slot;
	
	bool active__dual_arm = false;

	if(_Get__ARM_LIST_TO_PLACE_STx(active__dual_arm, l__arm_type,l__arm_lotid, l__stx_name,l__stx_slot) < 0)
	{
		NEXT__LOOP;
	}

	if(active__dual_arm)
	{
		CString arm_type  = ARM_AB;

		CString arm_lotid = l__arm_lotid[0];
		CString trg__stx_name = l__stx_name[0];
		int trg__stx_slot = l__stx_slot[0];

		if(Fnc__PLACE_BUFFERx(p_variable,p_alarm, arm_type, trg__stx_name,trg__stx_slot, arm_lotid) < 0)
		{
			NEXT__LOOP;
		}

		// ...
		{
			CString ch_data;
			int next__slot_id;

			if(trg__stx_name.CompareNoCase(MODULE__BUFFER1) == 0)
			{
				if(xCH__SCH_DB_ST1_WAFER_PICK_MODE->Check__DATA(STR__BOTTOM_TO_UP) > 0)
				{
					next__slot_id = trg__stx_slot + 1;
				}
				else
				{
					next__slot_id = trg__stx_slot - 1;
					if(next__slot_id < 0)		next__slot_id = 0;
				}

				ch_data.Format("%1d", next__slot_id);
				xEXT_CH__BUFFER1__APP_NEXT_SLOT_TO_PLACE->Set__DATA(ch_data);
			}
			else if(trg__stx_name.CompareNoCase(MODULE__BUFFER2) == 0)
			{
				if(xCH__SCH_DB_ST2_WAFER_PICK_MODE->Check__DATA(STR__BOTTOM_TO_UP) > 0)
				{
					next__slot_id = trg__stx_slot + 1;
				}
				else
				{
					next__slot_id = trg__stx_slot - 1;
					if(next__slot_id < 0)		next__slot_id = 0;
				}

				ch_data.Format("%1d", next__slot_id);
				xEXT_CH__BUFFER2__APP_NEXT_SLOT_TO_PLACE->Set__DATA(ch_data);
			}
		}
	}
	else
	{
		int k_limit = l__arm_type.GetSize();
		int k;

		for(k=0; k<k_limit; k++)
		{
			CString arm_type = l__arm_type[k];

			CString arm_lotid = l__arm_lotid[k];
			CString trg__stx_name = l__stx_name[k];
			int trg__stx_slot = l__stx_slot[k];

			if(Fnc__PLACE_BUFFERx(p_variable,p_alarm, arm_type, trg__stx_name,trg__stx_slot, arm_lotid) < 0)
			{
				NEXT__LOOP;
			}

			// ...
			{
				CString ch_data;
				int next__slot_id;

				if(trg__stx_name.CompareNoCase(MODULE__BUFFER1) == 0)
				{
					if(xCH__SCH_DB_ST1_WAFER_PICK_MODE->Check__DATA(STR__BOTTOM_TO_UP) > 0)
					{
						next__slot_id = trg__stx_slot + 1;
					}
					else
					{
						next__slot_id = trg__stx_slot - 1;
						if(next__slot_id < 0)		next__slot_id = 0;
					}

					ch_data.Format("%1d", next__slot_id);
					xEXT_CH__BUFFER1__APP_NEXT_SLOT_TO_PLACE->Set__DATA(ch_data);
				}
				else if(trg__stx_name.CompareNoCase(MODULE__BUFFER2) == 0)
				{
					if(xCH__SCH_DB_ST2_WAFER_PICK_MODE->Check__DATA(STR__BOTTOM_TO_UP) > 0)
					{
						next__slot_id = trg__stx_slot + 1;
					}
					else
					{
						next__slot_id = trg__stx_slot - 1;
						if(next__slot_id < 0)		next__slot_id = 0;
					}

					ch_data.Format("%1d", next__slot_id);
					xEXT_CH__BUFFER2__APP_NEXT_SLOT_TO_PLACE->Set__DATA(ch_data);
				}
			}
		}
	}
	
	return 1;
}
int  CObj__DUAL_ARM_STD
::AUTO_CTRL__LBo_RB(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm)
{
	if(SCH__LBx__CHECK_ONLY_OUTPUT() > 0)
	{
		Fnc__LBo_To_RB__ONLY_MODE(p_variable,p_alarm);
	}
	else
	{
		Fnc__LBo_To_RB__ALL_MODE(p_variable,p_alarm);
	}
	
	return 1;
}

void CObj__DUAL_ARM_STD
::AUTO_CTRL__FRONT_LBi(CII_OBJECT__VARIABLE *p_variable)
{
	DECLARE__EXT_CTRL(p_variable);

	// ...
	{
		CString cfg_mode = dCH__CFG_PLACE_FRONT_TO_LLx_DOOR->Get__STRING();

		if(cfg_mode.CompareNoCase(STR__DISABLE) == 0)
		{
			return;
		}

		// ...
		CString cur__ani_trg_rote = xCH__ATM_RB__ANI_TRG_ROTE->Get__STRING();
		CString cur__ani_trg_move = xCH__ATM_RB__ANI_TRG_MOVE->Get__STRING();

		if(cfg_mode.CompareNoCase(STR__ENABLE) == 0)
		{
			if(Get__LLx_INDEX(cur__ani_trg_rote) >= 0)
			{
				return;
			}
			if(Get__LLx_INDEX(cur__ani_trg_move) >= 0)
			{
				return;
			}
		}
		if(cfg_mode.CompareNoCase(STR__ENABLE_LPx) == 0)
		{
			if(SCH__Get_LPx_INDEX(cur__ani_trg_rote) >= 0)
			{
				return;
			}
			if(SCH__Get_LPx_INDEX(cur__ani_trg_move) >= 0)
			{
				return;
			}
		}
		if(cfg_mode.CompareNoCase(STR__ENABLE_ALx) == 0)
		{
			if((cur__ani_trg_rote.CompareNoCase(MODULE__AL1) == 0)
			|| (cur__ani_trg_move.CompareNoCase(MODULE__AL1) == 0))
			{
				return;
			}			
		}
	}
	
	// ...
	{
		if(iLBx_FRONT_FLAG > 0)							return;
		if(ATM_RB__Check_Occupied__Arm_Type() < 0)		return;
	}

	// ...
	CStringArray l__arm_type;

	if(ATM_RB__Check_Occupied__A_Arm() > 0)			l__arm_type.Add(ARM_A);
	if(ATM_RB__Check_Occupied__B_Arm() > 0)			l__arm_type.Add(ARM_B);

	if(l__arm_type.GetSize() != 1)					return;

	CString arm_type = l__arm_type[0];

	// ...
	{
		IDS__SCH_MATERIAL_STATUS ds_info;

		if(xSCH_MATERIAL_CTRL->Get__MATERIAL_STATUS(arm_type,ds_info) < 0)		return;
		if(ds_info.sMATERIAL_STS.CompareNoCase("ALIGNED") != 0)					return;
	}

	// ...
	{
		CString cfg_mode = dCH__CFG_PLACE_FRONT_TO_LLx_DOOR->Get__STRING();

		if(cfg_mode.CompareNoCase(STR__ENABLE_ALx) == 0)
		{
			CStringArray l_para;
				
			l_para.Add(arm_type);
			l_para.Add(MODULE__AL1);
			l_para.Add("1");
				
			Call__ROBOT_OBJ(ATM_CMMD__ROTATE, l_para);

			iLBx_FRONT_FLAG = 1;
			return;
		}
		else if(cfg_mode.CompareNoCase(STR__ENABLE_LPx) == 0)
		{
			CStringArray l_para;

			CString next_module;
			CString next_slot;
			
			dCH__CFG_FRONT_LPx_ID->Get__DATA(next_module);
			aCH__CFG_FRONT_LPx_SLOT->Get__DATA(next_slot);

			l_para.Add(arm_type);
			l_para.Add(next_module);
			l_para.Add(next_slot);
			
			Call__ROBOT_OBJ(ATM_CMMD__ROTATE, l_para);
			
			iLBx_FRONT_FLAG = 1;

			//
			CString log_msg;
			CString log_bff;

			log_msg = "CObj__DUAL_ARM_STD::AUTO_CTRL__FRONT_LBi() \n";
			log_bff.Format(" * arm_type <- %s \n", arm_type);
			log_msg += log_bff;
			log_bff.Format(" * next_module <- %s \n", next_module);
			log_msg += log_bff;
			log_bff.Format(" * next_slot <- %s \n", next_slot);
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
			return;
		}
		else if(cfg_mode.CompareNoCase(STR__ENABLE) == 0)
		{
			int ll_limit = iLLx_SIZE;
			int ll_i;

			for(ll_i=0; ll_i<ll_limit; ll_i++)
			{
				if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_i]->Check__DATA(LBx_MODE__ONLY_OUTPUT) > 0)
				{
					continue;
				}

				if((LLx__Is_Available(ll_i) < 0)
				&& (LLx__Is_ATM(ll_i) < 0))
				{
					int slot_id;

					if(LLx__Get_Empty__InSlot(ll_i, slot_id) > 0)
					{
						CStringArray l_para;
						CString str_data;

						CString ll_name = Get__LLx_NAME(ll_i);
						str_data.Format("%1d",slot_id);

						l_para.Add(arm_type);
						l_para.Add(ll_name);
						l_para.Add(str_data);

						Call__ROBOT_OBJ(ATM_CMMD__ROTATE, l_para);

						iLBx_FRONT_FLAG = 1;
						return;
					}
				}
			}

			// ...
		}
	}

	// ...
}
void CObj__DUAL_ARM_STD
::AUTO_CTRL__FRONT_LBo(CII_OBJECT__VARIABLE *p_variable)
{
	DECLARE__EXT_CTRL(p_variable);

	if(iLBx_FRONT_FLAG > 0)
	{
		return;
	}

	// ...
	CString cfg_mode = dCH__CFG_PICK_FRONT_TO_LLx_DOOR->Get__STRING();

	if(cfg_mode.CompareNoCase(STR__ENABLE) != 0)				return;

	// ...
	if(xSCH_MATERIAL_CTRL->Check__MATERIAL_TO_PROCESS() > 0)	return;
	if(ATM_RB__Check_Occupied__Arm_Type() > 0)					return;
	if(AL1__Check_Empty__All_Slot() < 0)						return;

	// ...
	CString arm_type;

	if(ATM_RB__Get_Empty__Arm_Type(arm_type) < 0)				return;

	// ...
	CString cur_pos = xCH__ATM_RB__ANI_TRG_ROTE->Get__STRING();

	int ll_limit = iLLx_SIZE;
	int ll_i;

	for(ll_i=0; ll_i<ll_limit; ll_i++)
	{
		if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_i]->Check__DATA(LBx_MODE__ONLY_INPUT) > 0)
		{
			continue;
		}

		if((LLx__Check_CtcInUse_Or_Standby(ll_i) > 0)
		&& (LLx__Is_ATM(ll_i) < 0))
		{
			CString ll_name = Get__LLx_NAME(ll_i);

			if(cur_pos.Find(ll_name) == 0)
			{
				continue;
			}

			// ...
			int slot_id;

			if(LLx__Get_Occupied__OutSlot(ll_i, slot_id) > 0)
			{
				CStringArray l_para;

				CString ll_slot;
				ll_slot.Format("%1d", slot_id);

				l_para.Add(arm_type);
				l_para.Add(ll_name);
				l_para.Add(ll_slot);

				Call__ROBOT_OBJ(ATM_CMMD__ROTATE, l_para);
				return;
			}
		}
	}

	// ...
}

void CObj__DUAL_ARM_STD
::AUTO_CTRL__Venting_Exception_Proc(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm)
{
	CString log_id = "AUTO_CTRL__Venting_Exception_Proc()";
	int i;

	if(ATM_RB__Is_Available() > 0)
	{
		int ll_limit = iLLx_SIZE;
		int ll_i;

		for(ll_i=0; ll_i<ll_limit; ll_i++)
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

			if(i_count < 5)
			{
				continue;
			}

			if(LLx__Is_Available(ll_i) > 0)
			{
				if(LLx__Is_ATM(ll_i)  > 0)
				{
					if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_i]->Check__DATA(LBx_MODE__ONLY_OUTPUT) > 0)	
					{
						if(LLx__Check_All_Empty(ll_i) > 0)
						{
							if(xEXT_CH__CFG__TRANSFER_MODE->Check__DATA(STR__VAC) > 0)
							{
								if(xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Check__DATA(STR__IN) < 0)
								{
									if(SCH_RUN__LLx_PUMP(ll_i, p_variable, log_id, "1") > 0)
									{
										xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Set__DATA(STR__IN);
									}
								}
							}
						}
						else
						{
							xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Set__DATA(STR__OUT);
						}
					}
					else if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_i]->Check__DATA(LBx_MODE__ONLY_INPUT) > 0)	
					{
						if(LLx__Check_All_Empty(ll_i) > 0)
						{
							xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Set__DATA(STR__IN);
						}
					}
					else
					{
						if(xEXT_CH__LLx__PRE_MAINT_FLAG[ll_i]->Check__DATA(STR__YES) > 0)
						{
							if(LLx__Check_All_Empty(ll_i) > 0)
							{
								xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Set__DATA(STR__IN);
							}
							else
							{
								int out_count = LLx__Get_Occupied__Total_OutSlot(ll_i);
								int in_count  = LLx__Get_Occupied__Total_InSlot(ll_i, 1);

								if((out_count > 0) && (in_count == 0))
								{
									xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Set__DATA(STR__IN);	
								}
								else if((out_count == 0) && (in_count > 0))
								{
									xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Set__DATA(STR__OUT);	
								}
							}
						}
						else
						{
							int slot_max = iLLx_SLOT_MAX[ll_i];

							if(slot_max > 1)
							{
								int xx_port;
								int xx_slot;
								
								if(xSCH_MATERIAL_CTRL->Get__MATERIAL_FROM_LPx(xx_port, xx_slot) < 1)
								{
									if((LLx__Check_Occupied__InSlot(ll_i)  > 0)
									&& (LLx__Check_Occupied__OutSlot(ll_i) < 1))
									{
										bool active__wfr_out = false;

										CString ll_name = Get__LLx_NAME(ll_i);
										CString ll_slot;
										
										CUIntArray l__in_slot_id;
										LLx__Get_Occupied__InSlot(ll_i, l__in_slot_id);

										int t_limit = l__in_slot_id.GetSize();
										for(int t=0; t<t_limit; t++)
										{
											CString sch__ll_name;
											sch__ll_name.Format("%s-%1d", ll_name, l__in_slot_id[t]);

											if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(sch__ll_name) > 0)		continue;

											active__wfr_out = true;
											break;
										}

										if(active__wfr_out)
										{

										}
										else
										{
											if(xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Check__DATA(STR__OUT) < 0)
											{
												if(SCH_RUN__LLx_PUMP(ll_i, p_variable, log_id, "2") > 0)
												{
													xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Set__DATA(STR__OUT);
												}
											}
										}
									}
								}
							}

							// ...
						}

						xEXT_CH__LLx__PRE_MAINT_FLAG[ll_i]->Set__DATA("");
					}
				}
				else		// Not ATM - Check.Start ...
				{
					if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_i]->Check__DATA(LBx_MODE__ONLY_OUTPUT) > 0)	
					{

					}
					else if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_i]->Check__DATA(LBx_MODE__ONLY_INPUT) > 0)	
					{
						CString str__ll_flag = xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Get__STRING();

						if((str__ll_flag.CompareNoCase(STR__IN)  == 0)
						|| (str__ll_flag.CompareNoCase(STR__OUT) == 0))
						{
							if(LLx__Check_All_Empty(ll_i) > 0)
							{
								SCH_RUN__LLx_VENT(ll_i, p_variable, log_id, "3.1");

								if(str__ll_flag.CompareNoCase(STR__OUT) == 0)
								{
									xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Set__DATA(STR__IN);
								}
							}
						}
					}
					else
					{
						if(xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Check__DATA(STR__IN) > 0)
						{
							int req_vent = 1;

							if(LLx__Check_Occupied__InSlot(ll_i) > 0)
							{
								req_vent = -1;
							}

							if(req_vent > 0)
							{
								SCH_RUN__LLx_VENT(ll_i, p_variable, log_id, "3");
							}
						}
						else if(xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Check__DATA(STR__OUT) > 0)
						{
							int req_pump = -1;

							if((LLx__Check_Occupied__InSlot(ll_i) > 0)			
							&& (LLx__Get_Occupied__Total_OutSlot(ll_i) < 1))
							{
								req_pump = 1;
							}

							if(req_pump > 0)
							{
								if(xEXT_CH__CFG__TRANSFER_MODE->Check__DATA(STR__VAC) > 0)
								{
									SCH_RUN__LLx_PUMP(ll_i, p_variable, log_id, "4");
								}
							}
						}
					}
				}			// Not ATM - Check.End ...

				// ...
			}
		}

		// ....
	}

	// LPx RETURN - CHECK ...
	for(i=0;i<iLPx_SIZE;i++)
	{
		if(xCH__LPx__RETURN_REQ_FLAG[i]->Check__DATA(STR__YES) < 0)
		{
			continue;
		}

		int lp_id = i + 1;

		int ll_limit = iLLx_SIZE;
		int ll_i;

		for(ll_i=0; ll_i<ll_limit; ll_i++)
		{
			if(xEXT_CH__LLx__MOVE_FLAG[ll_i]->Check__DATA(STR__YES) > 0)
			{
				continue;
			}

			if(LLx__Is_ATM(ll_i) > 0)				continue;
			if(LLx__Is_Available(ll_i) < 0)			continue;

			// ...
			{
				int lb_in_slotid;
				
				if(LLx__Get_Occupied__InSlot(ll_i, lb_in_slotid) < 0)
				{
					continue;
				}

				CString ll_name = Get__LLx_NAME(ll_i);
				
				CString lb_sch_name;					
				lb_sch_name.Format("%s-%1d", ll_name,lb_in_slotid);

				IDS__SCH_MATERIAL_STATUS ds_info;			
				if(xSCH_MATERIAL_CTRL->Get__MATERIAL_STATUS(lb_sch_name,ds_info) < 0)
				{
					continue;
				}

				if(ds_info.iPORT_ID != lp_id)			continue;

				if(SCH_RUN__LLx_VENT(ll_i, p_variable, log_id, "1") > 0)
				{
					xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Set__DATA(STR__IN);
				}
			}
		}

		// ...
	}	
}
void CObj__DUAL_ARM_STD
::AUTO_CTRL__Robot_Abort_Exception(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm)
{
	if(ATM_RB__Is_Available() < 0)
	{
		NEXT__LOOP;
	}

	// ...
	bool active__arm_wafer__mapped = false;
	bool active__arm_wafer__proces = false;

	if(dCH__ATM_RB__CFG_PICK_WAFER_CONDITION->Check__DATA(STR__ONLY_PROCESSED) > 0)
	{
		if(ATM_RB__Check_Occupied__A_Arm() > 0)
		{
			if(xCH__ATM_RB__SLOT01_STATUS->Check__DATA(STR__MAPPED) > 0)		active__arm_wafer__mapped = true;
			else																active__arm_wafer__proces = true;
		}
		if(ATM_RB__Check_Occupied__B_Arm() > 0)
		{
			if(xCH__ATM_RB__SLOT02_STATUS->Check__DATA(STR__MAPPED) < 0)		active__arm_wafer__mapped = true;
			else																active__arm_wafer__proces = true;
		}

		if((active__arm_wafer__mapped)
		&& (active__arm_wafer__proces))
		{
			NEXT__LOOP;
		}
	}

	// ...
	int i;

	// ROBOT ARMx ...
	{
		int count__arm_wfr = 0;

		for(i=0; i<iLPx_SIZE; i++)
		{
			if(xCH__LPx__RETURN_REQ_FLAG[i]->Check__DATA(STR__YES) < 0)
			{
				continue;
			}
			
			int lp_id = i + 1;

			// ...
			CString arm_type;

			if(ATM_RB__Get_Occupied__Arm_Type(arm_type) < 0)				continue;
			if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(arm_type) > 0)		continue;

			// ...
			IDS__SCH_MATERIAL_STATUS ds_sts;

			if(xSCH_MATERIAL_CTRL->Get__MATERIAL_STATUS(arm_type,ds_sts) > 0)
			{
				if(ds_sts.iPORT_ID == lp_id)
				{
					CString pmc_name;
					CString pmc_rcp;

					if(xSCH_MATERIAL_CTRL->Get__Last_Processing_Info(arm_type, pmc_name,pmc_rcp) < 0)
					{
						xSCH_MATERIAL_CTRL->Set__MATERIAL_STATUS(arm_type, STR__TO_LP);
					}

					count__arm_wfr++;
				}
			}
		}

		if(count__arm_wfr > 0)			NEXT__LOOP;
	}
	
	// LPx ...
	for(i=0; i<iLPx_SIZE; i++)
	{
		if(xCH__LPx__RETURN_REQ_FLAG[i]->Check__DATA(STR__YES) < 0)
		{
			continue;
		}
		
		// ...
		int lp_id = i + 1;
			
		int ll_limit = iLLx_SIZE;
		int ll_i;

		// Check : LLx_IN ...
		for(ll_i=0; ll_i<ll_limit; ll_i++)
		{
			if(LLx__Is_ATM(ll_i) < 0)				continue;
			if(LLx__Is_Available(ll_i) < 0)			continue;

			if(xEXT_CH__LLx__MOVE_FLAG[ll_i]->Check__DATA(STR__YES) > 0)
			{
				continue;
			}

			// ...
			int wfr__check_count = 0;
			int wfr__pick_count  = 0;

			while(wfr__check_count < 2)
			{
				wfr__check_count++;

				// ...
				int lb_in_slotid;							
			
				if(LLx__Get_Occupied__InSlot(ll_i, lb_in_slotid) < 0)
				{
					break;
				}

				// ...
				IDS__SCH_MATERIAL_STATUS ds_info;
				CString para_module;
				CString para_slot;
				CString lb_sch_name;
				
				para_module = Get__LLx_NAME(ll_i);
				para_slot.Format("%1d",lb_in_slotid);
				lb_sch_name.Format("%s-%s", para_module,para_slot);
					
				if(xSCH_MATERIAL_CTRL->Get__MATERIAL_STATUS(lb_sch_name,ds_info) > 0)
				{
					if(ds_info.iPORT_ID == lp_id)
					{
						// Check : Wafer State
						{
							int ll_slot_index = lb_in_slotid - 1;
							CString str__wfr_state = xEXT_CH__LLx__SLOT_STATUS[ll_i][ll_slot_index]->Get__STRING();

							if(active__arm_wafer__mapped)
							{
								if(str__wfr_state.CompareNoCase(STR__MAPPED) != 0)			continue;
							}
							if(active__arm_wafer__proces)
							{
								if(str__wfr_state.CompareNoCase(STR__MAPPED) == 0)			continue;
							}
						}

						if(ATM_RB__Check_Empty__Arm_Type() < 0)
						{
							// Fnc__RB_TO_AL(p_variable,p_alarm);
							continue;
						}

						int r_flag = Fnc__PICK_LBx_EXCEPTION(p_variable,p_alarm, para_module,para_slot);
						if(r_flag > 0)		wfr__pick_count++;	
					}
				}
			}

			if(wfr__pick_count > 0)			NEXT__LOOP;
		}
			
		// Check : LLx_OUT ...
		for(ll_i=0; ll_i<ll_limit; ll_i++)
		{
			if(LLx__Is_ATM(ll_i) < 0)				continue;
			if(LLx__Is_Available(ll_i) < 0)			continue;

			if(xEXT_CH__LLx__MOVE_FLAG[ll_i]->Check__DATA(STR__YES) > 0)
			{
				continue;
			}

			// ...
			int wfr__check_count = 0;
			int wfr__pick_count  = 0;

			while(wfr__check_count < 2)
			{
				wfr__check_count++;

				// ...
				int lb_out_slotid;

				if(LLx__Get_Occupied__OutSlot(ll_i, lb_out_slotid) < 0)
				{
					break;
				}

				// ...
				IDS__SCH_MATERIAL_STATUS ds_info;
				CString para_module;
				CString para_slot;
				CString lb_sch_name;

				para_module = Get__LLx_NAME(ll_i);
				para_slot.Format("%1d",lb_out_slotid);
				lb_sch_name.Format("%s-%s", para_module,para_slot);

				if(xSCH_MATERIAL_CTRL->Get__MATERIAL_STATUS(lb_sch_name,ds_info) > 0)
				{
					if(ds_info.iPORT_ID == lp_id)
					{
						// Check : Wafer State
						{
							int ll_slot_index = lb_out_slotid - 1;
							CString str__wfr_state = xEXT_CH__LLx__SLOT_STATUS[ll_i][ll_slot_index]->Get__STRING();

							if(active__arm_wafer__mapped)
							{
								if(str__wfr_state.CompareNoCase(STR__MAPPED) != 0)			continue;
							}
							if(active__arm_wafer__proces)
							{
								if(str__wfr_state.CompareNoCase(STR__MAPPED) == 0)			continue;
							}
						}

						if(ATM_RB__Check_Empty__Arm_Type() < 0)
						{
							// Fnc__RB_TO_AL(p_variable,p_alarm);
							continue;
						}

						int r_flag = Fnc__PICK_LBx_EXCEPTION(p_variable,p_alarm, para_module,para_slot);
						if(r_flag > 0)		wfr__pick_count++;
					}
				}
			}

			if(wfr__pick_count > 0)			NEXT__LOOP;
		}
	}	
}
int CObj__DUAL_ARM_STD
::Check__Robot_Abort_Exception()
{
	int i;

	// ROBOT ARMx ...
	for(i=0; i<iLPx_SIZE; i++)
	{
		if(xCH__LPx__RETURN_REQ_FLAG[i]->Check__DATA(STR__YES) < 0)
		{
			continue;
		}

		// ...
		int lp_id = i + 1;

		// ...
		CString arm_type;

		if(ATM_RB__Get_Occupied__Arm_Type(arm_type) > 0)
		{	
			IDS__SCH_MATERIAL_STATUS ds_sts;

			if(xSCH_MATERIAL_CTRL->Get__MATERIAL_STATUS(arm_type,ds_sts) > 0)
			{
				if(ds_sts.iPORT_ID == lp_id)		return 1;
			}
		}
	}

	// LPx ...
	for(i=0; i<iLPx_SIZE; i++)
	{
		if(xCH__LPx__RETURN_REQ_FLAG[i]->Check__DATA(STR__YES) < 0)
		{
			continue;
		}

		// ...
		int lp_id = i + 1;

		int ll_limit = iLLx_SIZE;
		int ll_i;

		// Check : LLx_IN ...
		for(ll_i=0; ll_i<ll_limit; ll_i++)
		{
			if(xEXT_CH__LLx__MOVE_FLAG[ll_i]->Check__DATA(STR__YES) > 0)
			{
				continue;
			}

			// ...
			int lb_in_slotid;							

			if(LLx__Get_Occupied__InSlot(ll_i, lb_in_slotid) < 0)
			{
				continue;
			}

			// ...
			IDS__SCH_MATERIAL_STATUS ds_info;
			CString para_module;
			CString para_slot;
			CString lb_sch_name;

			para_module = Get__LLx_NAME(ll_i);
			para_slot.Format("%1d",lb_in_slotid);
			lb_sch_name.Format("%s-%s", para_module,para_slot);

			if(xSCH_MATERIAL_CTRL->Get__MATERIAL_STATUS(lb_sch_name,ds_info) > 0)
			{
				if(ds_info.iPORT_ID == lp_id)			return 1;
			}
		}
	}	

	return -1;
}

void CObj__DUAL_ARM_STD
::AUTO_CTRL__Robot_Exception_Proc(CII_OBJECT__VARIABLE *p_variable,	CII_OBJECT__ALARM *p_alarm)
{
	if(ATM_RB__Is_Available() < 0)
	{
		NEXT__LOOP;
	}

	// ...
	int ll_limit = iLLx_SIZE;
	int ll_i;

	for(ll_i=0; ll_i<ll_limit; ll_i++)
	{
		if(LLx__Is_Available(ll_i) < 0)			continue;
		if(LLx__Is_ATM(ll_i)  < 0)				continue;

		if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_i]->Check__DATA(LBx_MODE__ONLY_OUTPUT) > 0)	
		{

		}
		else if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_i]->Check__DATA(LBx_MODE__ONLY_INPUT) > 0)	
		{

		}
		else
		{
			if(xEXT_CH__LLx__PRE_MAINT_FLAG[ll_i]->Check__DATA(STR__YES) > 0)
			{

			}
			else
			{

			}
		}
	}

	// ...
}

int  CObj__DUAL_ARM_STD
::Fnc__RB_TO_AL(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	CString log_id = "Fnc__RB_TO_AL()";

	// ...
	CString para_module = MODULE__AL1;
	CString para_slot;
	int al_slot_id;
	
	// ...
	{
		if(AL1__Is_Available() < 0)
		{
			NEXT__LOOP;
		}
		
		if(AL1__Get_Empty__SlotID(al_slot_id) < 0)
		{
			NEXT__LOOP;
		}
		para_slot.Format("%1d",al_slot_id);
	}
	
	// ...
	CString arm_type = ARM_A;

	if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(arm_type) < 0)
	{
		NEXT__LOOP;
	}
	
	// ...
	bool ex_flag = false;
	
	CString sch_module;
	sch_module.Format("%s-%s",para_module,para_slot);
	
	if(SCH__PLACE_MODULE(p_variable,
						 p_alarm,
						 log_id,
						 ex_flag,
						 arm_type,
						 para_module,
						 para_slot,
						 sch_module) < 0)
	{
		NEXT__LOOP;
	}

	// ...
	xSCH_MATERIAL_CTRL->Place__To_MODULE(arm_type,sch_module);
		
	if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(sch_module) > 0)
	{
		xSCH_MATERIAL_CTRL->Set__MATERIAL_STATUS(sch_module, STR__MAPPED);
	}
	else
	{
		xSCH_MATERIAL_CTRL->Set__MATERIAL_STATUS(sch_module, STR__TO_LP);
	}
	return 1;
}

void CObj__DUAL_ARM_STD
::AUTO_CTRL__LBi_VENT(CII_OBJECT__VARIABLE *p_variable)
{
	DECLARE__EXT_CTRL(p_variable);

	CString log_id = "AUTO_CTRL__LBi_VENT()";

	// ...
	int ll_limit = iLLx_SIZE;
	int ll_i;

	for(ll_i=0; ll_i>ll_limit; ll_i++)
	{
		if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_i]->Check__DATA(LBx_MODE__ONLY_OUTPUT) > 0)	
		{
			continue;
		}

		if((LLx__Is_Available(ll_i) > 0)
		&& (LLx__Check_All_Empty(ll_i) > 0)
		&& (LLx__Is_ATM(ll_i) < 0))
		{
			if(xEXT_CH__LLx__MOVE_FLAG[ll_i]->Check__DATA(STR__YES) < 0)
			{
				SCH_RUN__LLx_VENT(ll_i, p_variable, log_id, "1");
			}
		}
	}
}
void CObj__DUAL_ARM_STD
::AUTO_CTRL__LBo_PUMP(CII_OBJECT__VARIABLE *p_variable)
{
	DECLARE__EXT_CTRL(p_variable);

	CString log_id = "AUTO_CTRL__LBo_PUMP()";
	
	// ...
	int cfg__atm_mode = -1;

	if(xEXT_CH__CFG__TRANSFER_MODE->Check__DATA(STR__ATM) > 0)
	{
		cfg__atm_mode = 1;
	}

	// ...
	int ll_limit = iLLx_SIZE;
	int ll_i;

	for(ll_i=0; ll_i<ll_limit; ll_i++)
	{
		if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_i]->Check__DATA(LBx_MODE__ONLY_OUTPUT) < 0)
		{
			continue;
		}

		if((LLx__Is_Available(ll_i)    > 0)
		&& (LLx__Check_All_Empty(ll_i) > 0))
		{
			if(cfg__atm_mode > 0)		SCH_RUN__LLx_VENT(ll_i, p_variable, log_id, "1");
			else						SCH_RUN__LLx_PUMP(ll_i, p_variable, log_id, "1");
		}
	}
}

void CObj__DUAL_ARM_STD
::Start__LBi_VENT(CII_OBJECT__VARIABLE *p_variable)
{
	DECLARE__EXT_CTRL(p_variable);

	CString log_id = "Start__LBi_VENT()";

	// ...
	int ll_limit = iLLx_SIZE;
	int ll_i;

	for(ll_i=0; ll_i<ll_limit; ll_i++)
	{
		if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_i]->Check__DATA(LBx_MODE__ONLY_OUTPUT) < 0)
		{
			if((LLx__Is_ATM(ll_i)       < 0)
			&& (LLx__Is_Available(ll_i) > 0))
			{
				SCH_RUN__LLx_VENT(ll_i, p_variable, log_id, "1");
			}
		}
	}
}
void CObj__DUAL_ARM_STD
::Start__ALL_LBx_VENT(CII_OBJECT__VARIABLE *p_variable)
{
	DECLARE__EXT_CTRL(p_variable);

	CString log_id = "Start__ALL_LBx_VENT()";

	// ...
	int ll_limit = iLLx_SIZE;
	int ll_i;

	for(ll_i=0; ll_i<ll_limit; ll_i++)
	{
		if((LLx__Is_ATM(ll_i)       < 0)
		&& (LLx__Is_Available(ll_i) > 0))
		{
			Sleep(500);

			if(LLx__Is_Available(ll_i) > 0)
			{
				SCH_RUN__LLx_VENT(ll_i, p_variable, log_id, "1");
			}
		}
	}
}
void CObj__DUAL_ARM_STD
::Start__ALL_LBx_PUMP(CII_OBJECT__VARIABLE *p_variable)
{
	DECLARE__EXT_CTRL(p_variable);

	CString log_id = "Start__ALL_LBx_PUMP()";

	// ...
	int ll_limit = iLLx_SIZE;
	int ll_i;

	for(ll_i=0; ll_i<ll_limit; ll_i++)
	{
		if((LLx__Is_VAC(ll_i)       < 0)
		&& (LLx__Is_Available(ll_i) > 0))
		{
			Sleep(500);

			if(LLx__Is_Available(ll_i) > 0)
			{
				SCH_RUN__LLx_PUMP(ll_i, p_variable, log_id, "1");
			}
		}
	}
}
void CObj__DUAL_ARM_STD
::Start__ALL_LBx_To_IDLE_STATE(CII_OBJECT__VARIABLE *p_variable)
{
	DECLARE__EXT_CTRL(p_variable);

	CString log_id = "Start__ALL_LBx_To_IDLE_STATE()";

	// ...
	int ll_limit = iLLx_SIZE;
	int ll_i;

	for(ll_i=0; ll_i<ll_limit; ll_i++)
	{
		if((LLx__Is_Available(ll_i) > 0)
		&& (xEXT_CH__LLx__MOVE_FLAG[ll_i]->Check__DATA("YES") < 0))
		{
			Sleep(500);
	
			if(xEXT_CH__SCH_DB_LLx_IDLE_STATUS[ll_i]->Check__DATA("VAC") > 0)
			{
				SCH_RUN__LLx_PUMP(ll_i, p_variable, log_id, "1");
			}
			else if(xEXT_CH__SCH_DB_LLx_IDLE_STATUS[ll_i]->Check__DATA("ATM") > 0)
			{
				SCH_RUN__LLx_VENT(ll_i, p_variable, log_id, "1");
			}
		}
	}
}

void CObj__DUAL_ARM_STD
::AUTO_CTRL__LBi_PUMP__ONLY_MODE(CII_OBJECT__VARIABLE *p_variable)
{
	CString log_id = "AUTO_CTRL__LBi_PUMP__ONLY_MODE()";

	// ...
	int ll_limit = iLLx_SIZE;
	int ll_i;

	for(ll_i=0; ll_i<ll_limit; ll_i++)
	{
		if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_i]->Check__DATA(LBx_MODE__ONLY_INPUT) > 0)
		{
			if((ATM_RB__Check_Empty__Arm_Type() > 0)
			&& (AL1__Check_Empty__All_Slot() > 0)
			&& (xSCH_MATERIAL_CTRL->Check__MATERIAL_TO_PROCESS() < 0))
			{
				if(xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Check__DATA("IN") > 0)
				{
					if((LLx__Is_Available(ll_i) > 0)
					&& (LLx__Check_All_Empty(ll_i) < 0)
					&& (LLx__Is_ATM(ll_i) > 0))
					{
						bool active__ll_pump = false;

						// ...
						{
							CUIntArray l_slot_id;
							LLx__Get_Occupied__Slot_List(ll_i, l_slot_id);

							int k_limit = l_slot_id.GetSize();
							for(int k=0; k<k_limit; k++)
							{
								CString sch_name;
								sch_name.Format("%s-%1d", Get__LLx_NAME(ll_i), l_slot_id[k]);

								if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(sch_name) < 0)		continue;

								active__ll_pump = true;
								break;
							}
						}

						if(active__ll_pump)
						{
							if(SCH_RUN__LLx_PUMP(ll_i, p_variable, log_id, "1") > 0)
							{
								xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Set__DATA("OUT");
							}
						}
					}
				}

				// ...
			}
		}

		// ...
	}

	//  ...
}
void CObj__DUAL_ARM_STD
::AUTO_CTRL__LBx_PUMP__ALL_MODE(CII_OBJECT__VARIABLE *p_variable)
{
	CString log_id = "AUTO_CTRL__LBx_PUMP__ALL_MODE()";

	if((xEXT_CH__SCH_DB_LLx_MODE_TYPE[0]->Check__DATA(LBx_MODE__ALL) < 0)
	|| (xEXT_CH__SCH_DB_LLx_MODE_TYPE[1]->Check__DATA(LBx_MODE__ALL) < 0))
	{
		NEXT__LOOP;
	}

	if(ATM_RB__Is_Available() < 0)
	{
		NEXT__LOOP;
	}

	if(sEXT_CH__SCH_MODE_LLx_OUT->Check__DATA(_SCH_MODE__ROUTE) > 0)
	{
		if(xSCH_MATERIAL_CTRL->Check__MATERIAL_OF_LLx_OUT(-1,1,true) > 0)
		{
			NEXT__LOOP;
		}
		if(xSCH_MATERIAL_CTRL->Check__MATERIAL_OF_LLx_OUT(-1,2,true) > 0)
		{
			NEXT__LOOP;
		}
	}

	// LBA & LBB : InSlot Check ...
	{
		int k_limit = 2;
		int k;

		for(k=0; k<k_limit; k++)
		{
			int ll_i = -1;

			if(xCH__ATM_RB__SCH_STS_TO_LLx[k]->Check__DATA("") > 0)
			{
				ll_i = _LLx_INDEX__LBA + k;
			}
			else
			{
				continue;
			}

			if(LLx__Is_Available(ll_i) > 0)
			{
				if((LLx__Get_Occupied__Total_OutSlot(ll_i) < 1)
				&& (LLx__Get_Occupied__Total_InSlot(ll_i)  > 0))
				{
					int lb_slotid;

					if(LLx__Get_Occupied__InSlot(ll_i, lb_slotid) > 0)
					{
						CString lb_sch_name;
						CString ll_name = Get__LLx_NAME(ll_i);

						lb_sch_name.Format("%s-%1d", ll_name,lb_slotid);

						if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(lb_sch_name) > 0)
						{
							if(xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Check__DATA("OUT") < 0)
							{
								if(SCH_RUN__LLx_PUMP(ll_i, p_variable, log_id, "1") > 0)
								{
									xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Set__DATA("OUT");	
								}
							}
						}
					}
				}
			}
		}

		// ...
	}

	// ATM Robot ...
	if(ATM_RB__Check_Occupied__Arm_Type() > 0)
	{
		NEXT__LOOP;
	}

	// AL1 ...
	if(AL1__Is_Available() > 0)
	{
		if(AL1__Check_Empty__All_Slot() < 0)
		{
			NEXT__LOOP;
		}
	}

	// VAC Robot : Out Material Count ...
	{
		CString var_data;

		xEXT_CH__VAC_ROBOT_OUT_MATERIAL_COUNT->Get__DATA(var_data);
		int count = atoi(var_data);

		if(count < 1)
		{
			NEXT__LOOP;
		}
	}

	// ...
	DECLARE__EXT_CTRL(p_variable);

	if(xSCH_MATERIAL_CTRL->Check__MATERIAL_TO_PROCESS() < 0)
	{
		if(xSCH_MATERIAL_CTRL->Check__MATERIAL_MORE_THAN_MOVE_COUNT(1) == 1)
		{
			int lbx_count = 0;

			int ll_limit = iLLx_SIZE;
			int ll_i;

			for(ll_i=0; ll_i<ll_limit; ll_i++)
			{
				if(xEXT_CH__SCH_DB_LLx_USE_FLAG[ll_i]->Check__DATA(STR__ENABLE) > 0)
				{
					if((pLLx__OBJ_CTRL[ll_i]->Is__OBJ_BUSY() > 0)
					|| (LLx__Is_VAC(ll_i) > 0))
					{
						lbx_count++;
					}
				}
			}

			if(lbx_count > 0)
			{
				NEXT__LOOP;
			}
		}
	}

	// LLx ...
	{
		int ll_limit = iLLx_SIZE;
		int ll_i;

		for(ll_i=0; ll_i<ll_limit; ll_i++)
		{
			if(xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Check__DATA("OUT") > 0)
			{
				if((LLx__Is_Available(ll_i) > 0)
				&& (LLx__Check_All_Empty(ll_i) > 0)
				&& (LLx__Is_ATM(ll_i) < 0))
				{
					NEXT__LOOP;
				}		
			}
		}
	}

	// ATM_RB ...
	{
		int k_limit = 2;
		int k;

		for(k=0; k<k_limit; k++)
		{
			int ll_i = -1;

			if(xCH__ATM_RB__SCH_STS_TO_LLx[k]->Check__DATA("") > 0)
			{
				ll_i = _LLx_INDEX__LBA + k;
			}
			else
			{
				continue;
			}

			if(xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Check__DATA("IN") > 0)
			{
				if((LLx__Is_Available(ll_i) > 0)
				&& (LLx__Check_All_Empty(ll_i) > 0)
				&& (LLx__Is_ATM(ll_i) > 0))
				{
					if(SCH_RUN__LLx_PUMP(ll_i, p_variable, log_id, "11") > 0)
					{
						xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Set__DATA("OUT");
					}
				}
			}
		}
	}

	// ...
}
int  CObj__DUAL_ARM_STD
::Seq__LBx_VENT__ALL_MODE(CII_OBJECT__VARIABLE *p_variable)
{
	if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[0]->Check__DATA(LBx_MODE__ALL) < 0)		return -1;
	if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[1]->Check__DATA(LBx_MODE__ALL) < 0)		return -1;

	CString log_id = "Seq__LBx_VENT__ALL_MODE()";

	// ...
	DECLARE__EXT_CTRL(p_variable);

	// ...
	int ll_limit = iLLx_SIZE;
	int ll_i;

	for(ll_i=0; ll_i<ll_limit; ll_i++)
	{
		if((LLx__Is_Available(ll_i)    > 0)
		&& (LLx__Check_All_Empty(ll_i) > 0))
		{
			if(LLx__Is_ATM(ll_i) < 0)
			{
				if(xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Check__DATA("OUT") > 0)
				{
					return -1;
				}

				if(SCH_RUN__LLx_VENT(ll_i, p_variable, log_id, "1") > 0)
				{
					xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Set__DATA("IN");
				}
				return 1;
			}
			else
			{
				return -1;
			}
		}
	}

	return -1;
}
int  CObj__DUAL_ARM_STD
::Seq__LBx_VENT__INTO_LBi(CII_OBJECT__VARIABLE *p_variable)
{
	DECLARE__EXT_CTRL(p_variable);

	CString log_id = "Seq__LBx_VENT__INTO_LBi()";

	// ...
	int ll_limit = iLLx_SIZE;
	int ll_i;

	for(ll_i=0; ll_i<ll_limit; ll_i++)
	{
		if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_i]->Check__DATA(LBx_MODE__ALL) < 0)
		{
			return -1;
		}

		if((xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Check__DATA("IN") > 0)
		&& (LLx__Is_Available(ll_i)    < 0)
		&& (LLx__Check_All_Empty(ll_i) > 0))
		{
			return -1;
		}

		if((xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Check__DATA("IN") > 0)
		&& (LLx__Is_Available(ll_i)    > 0)
		&& (LLx__Check_All_Empty(ll_i) > 0)
		&& (LLx__Is_ATM(ll_i) > 0))
		{
			return -1;
		}

		if((xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Check__DATA("OUT") > 0)
		&& (LLx__Is_Available(ll_i)    > 0)
		&& (LLx__Check_All_Empty(ll_i) > 0)
		&& (LLx__Is_VAC(ll_i) > 0))
		{
			if(SCH_RUN__LLx_VENT(ll_i, p_variable, log_id, "1") > 0)
			{
				xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Set__DATA("IN");
			}
			return 1;
		}
	}

	return -1;
}


//------------------------------------------------------------------------------------------------
int  CObj__DUAL_ARM_STD::Fnc__AUTO_CTRL__ERROR()
{

	return -1;
}


//------------------------------------------------------------------------------------------------
int  CObj__DUAL_ARM_STD
::Fnc__PICK_LPi_WITH_ARM(CII_OBJECT__VARIABLE *p_variable,
					     CII_OBJECT__ALARM *p_alarm,
						 const CString& robot_arm,
						 const int port_id,
						 const int slot_id,
						 const int ll_id)
{
	CString log_id = "Fnc__PICK_LPi_WITH_ARM()";

	// ...
	int lp_i = port_id - 1;

	if(LPx__Is_Available(lp_i) < 0)
	{
		NEXT__LOOP;
	}

	if(AL1__Check_Empty__All_Slot() > 0)
	{
		if(robot_arm.CompareNoCase(ARM_AB) == 0)
		{
			int arm_count = ATM_RB__Get_Empty_Arm_Count();
			if(arm_count < 2)			NEXT__LOOP;
		}
		else
		{
			if(ATM_RB__Check_Empty__Arm_Type(robot_arm) < 0)			NEXT__LOOP;
		}
	}
	else
	{
		if(robot_arm.CompareNoCase(ARM_AB) == 0)
		{
			NEXT__LOOP;
		}
		else
		{
			int al_slot_id;

			if(AL1__Get_Empty__SlotID(al_slot_id) < 0)					NEXT__LOOP;
			if(ATM_RB__Check_Empty__Arm_Type(robot_arm) < 0)			NEXT__LOOP;
		}
	}

	if(xEXT_CH__SCH_ALIGN_DEVICE->Check__DATA(STR__ATM_RB) > 0)
	{
		if(SCH__CAN_PLACE_LBi() < 0)
		{
			NEXT__LOOP;
		}
	}

	// ...
	CString para_module;
	CString para_slot;

	para_module.Format("LP%1d",port_id);
	para_slot.Format("%1d",slot_id);

	CString sch_module;
	sch_module.Format("LP%1d-%1d",port_id,slot_id);

	// LLx ...
	{
		int ll_i = ll_id - 1;

		if((ll_i >= 0) && (ll_i < iLLx_SIZE))
		{
			xCH__ATM_RB__SCH_STS_TO_LLx[ll_i]->Set__DATA(STR__YES);
		}
	}

	if(SCH__PICK_MODULE(p_variable,p_alarm, log_id, false, robot_arm, para_module,para_slot,sch_module) < 0)
	{
		// LLx ...
		{
			int ll_i = ll_id - 1;

			if((ll_i >= 0) && (ll_i < iLLx_SIZE))
			{
				xCH__ATM_RB__SCH_STS_TO_LLx[ll_i]->Set__DATA("");
			}
		}
		return -1;
	}

	// ...
	{
		int db_index = port_id - 1;

		if((db_index >= 0) && (db_index < iLPx_SIZE))
		{
			CString var_data;
			int i_count;

			xCH__LPx__PICK_COUNT[db_index]->Get__DATA(var_data);
			i_count = atoi(var_data) + 1;
			var_data.Format("%1d", i_count);
			xCH__LPx__PICK_COUNT[db_index]->Set__DATA(var_data);
		}
	}

	// ...
	CStringArray l__arm_type;
	CUIntArray   l__slot_id;

	if(robot_arm.CompareNoCase(ARM_AB) == 0)
	{
		l__arm_type.Add(ARM_A);
		l__slot_id.Add(slot_id);

		l__arm_type.Add(ARM_B);
		l__slot_id.Add(slot_id + 1);
	}
	else
	{
		l__arm_type.Add(robot_arm);
		l__slot_id.Add(slot_id);
	}

	int arm_size = l__arm_type.GetSize();
	int arm_i;

	for(arm_i=0; arm_i<arm_size; arm_i++)
	{
		CString arm_type = l__arm_type[arm_i];
		int arm__slot_id = l__slot_id[arm_i];

		xSCH_MATERIAL_CTRL->Pick__From_LPx(port_id, arm__slot_id, arm_type);

		if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(port_id, arm__slot_id) > 0)
		{
			if(xEXT_CH__SCH_DB_AL1_USE_FLAG->Check__DATA(STR__ENABLE) < 0)
			{
				xSCH_MATERIAL_CTRL->Set__MATERIAL_STATUS(port_id, arm__slot_id, STR__ALIGNED);
			}
		}
		else
		{
			xSCH_MATERIAL_CTRL->Set__MATERIAL_STATUS(port_id, arm__slot_id, STR__TO_LP);
		}

		if(Fnc__Check_ARM_WAFER_INFO_MISMATCH(p_variable,p_alarm, arm_type) < 0)
		{
			NEXT__LOOP;
		}
	}

	return 1;
}

int  CObj__DUAL_ARM_STD
::Fnc__Check_ARM_WAFER_INFO_MISMATCH(CII_OBJECT__VARIABLE *p_variable,
									   CII_OBJECT__ALARM *p_alarm,
									   const CString& arm_type)
{
	CString wfr_title; 

	if(ATM_RB__Get_Occupied__Wfr_Title_Of_Arm_Type(arm_type, wfr_title) < 0)
	{
		return -1;
	}
	
	// ...
	{
		int wfr_ptn;
		int wfr_slot;		
		Macro__Analyze_Title_Info(wfr_title, wfr_ptn,wfr_slot);
		
		IDS__SCH_MATERIAL_INFO ds_info;
		xSCH_MATERIAL_CTRL->Get__MATERIAL_INFO(arm_type,ds_info);
		
		if((wfr_ptn  != ds_info.iSRC__PTN)
		|| (wfr_slot != ds_info.iSRC__STN))
		{
			xCH__ATM_RB_PAUSE_FLAG->Set__DATA(STR__YES);
			
			// ...
			{
				CString alarm_msg;
				CString r_act;
				
				alarm_msg.Format("Wafer Info (%1d:%1d) is different from SCH DB(%1d:%1d)\n",
								 wfr_ptn,wfr_slot,
								 ds_info.iSRC__PTN,ds_info.iSRC__STN);
				
				p_alarm->Popup__ALARM_With_MESSAGE(ALID__JOB_ERROR__WAFER_INFO_MISMATCH,alarm_msg,r_act);
			}
			
			return -1;
		}
	}

	return 1;
}

// LPx -> ATM_RB
int  CObj__DUAL_ARM_STD
::Fnc__PICK_LPi_In_BUFFER_MODE(CII_OBJECT__VARIABLE *p_variable,
								 CII_OBJECT__ALARM *p_alarm,
								 const int port_id,
								 const int slot_id,
								 const int ll_id,
								 CString& arm_type)
{
	CString log_id = "Fnc__PICK_LPi_In_BUFFER_MODE()";

	// ...
	int lp_i = port_id - 1;
	
	if(LPx__Is_Available(lp_i) < 0)
	{
		NEXT__LOOP;
	}

	if(ATM_RB__Get_Empty__Arm_Type(arm_type) < 0)
	{
		NEXT__LOOP;
	}

	// ...
	CString para_module;
	CString para_slot;

	para_module.Format("LP%1d",port_id);
	para_slot.Format("%1d",slot_id);

	// ...
	CString sch_module;
	sch_module.Format("LP%1d-%1d",port_id,slot_id);

	if(SCH__PICK_MODULE(p_variable,p_alarm, log_id, false,arm_type,para_module,para_slot,sch_module) < 0)
	{
		return -1;
	}

	// ...
	{
		int db_index = port_id - 1;

		if((db_index >= 0) && (db_index < iLPx_SIZE))
		{
			CString var_data;
			int i_count;

			xCH__LPx__PICK_COUNT[db_index]->Get__DATA(var_data);
			i_count = atoi(var_data) + 1;
			var_data.Format("%1d", i_count);
			xCH__LPx__PICK_COUNT[db_index]->Set__DATA(var_data);
		}
	}

	// LLx ...
	{
		int ll_i = ll_id - 1;

		if((ll_i >= 0) && (ll_i < iLLx_SIZE))
		{
			xCH__ATM_RB__SCH_STS_TO_LLx[ll_i]->Set__DATA("YES");
		}
	}

	// ...
	{
		xSCH_MATERIAL_CTRL->Pick__From_LPx(port_id,slot_id,arm_type);

		if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(port_id,slot_id) > 0)
		{
			if(xEXT_CH__SCH_DB_AL1_USE_FLAG->Check__DATA(STR__ENABLE) < 0)
			{
				xSCH_MATERIAL_CTRL->Set__MATERIAL_STATUS(port_id,slot_id, STR__ALIGNED);
			}
		}
		else
		{
			xSCH_MATERIAL_CTRL->Set__MATERIAL_STATUS(port_id,slot_id, STR__TO_LP);
		}
	}

	if(Fnc__Check_ARM_WAFER_INFO_MISMATCH(p_variable,p_alarm, arm_type) < 0)
	{
		NEXT__LOOP;
	}
	return 1;
}

// ATM_RB -> ALx
int  CObj__DUAL_ARM_STD
::Fnc__PLACE_AL(CII_OBJECT__VARIABLE *p_variable,
				CII_OBJECT__ALARM *p_alarm,
				const CString& log_xx,
				const CString& arm_type,
				const int al_start_flag,
				const int ll_slot_count)
{
	CString log_id = log_xx;
	log_id += " -> ";
	log_id += "Fnc__PLACE_AL()";

	// ...
	CString para_module = MODULE__AL1;
	CString para_in_slot;
	CString para_out_slot;
	int al_slot_id;

	if(AL1__Is_Available() < 0)
	{
		NEXT__LOOP;
	}

	if(AL1__Get_Occupied__SlotID(al_slot_id) > 0)
	{
		para_out_slot.Format("%1d",al_slot_id);
	}

	if(AL1__Get_Empty__SlotID(al_slot_id) < 0)
	{
		NEXT__LOOP;
	}
	
	para_in_slot.Format("%1d", al_slot_id);

	// ...
	bool ex_flag = false;
	
	if(para_out_slot.GetLength() > 0)
	{
		ex_flag = true;
	}
	
	if(xEXT_CH__SCH_ALIGN_DEVICE->Check__DATA(STR__ATM_RB) > 0)
	{
		CString sch_module;
		sch_module.Format("%s-%s", para_module,para_in_slot);

		if(SCH__PLACE_MODULE(p_variable,
							 p_alarm,
							 log_id,
							 ex_flag,
							 arm_type,
							 para_module,
							 para_in_slot,
							 sch_module) < 0)
		{
			return -1;
		}

		// ...
		{
			xSCH_MATERIAL_CTRL->Place__To_MODULE(arm_type, sch_module);
		
			if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(sch_module) > 0)
			{
				xSCH_MATERIAL_CTRL->Set__MATERIAL_STATUS(sch_module, STR__ALIGNED);
			}
			else
			{
				xSCH_MATERIAL_CTRL->Set__MATERIAL_STATUS(sch_module, STR__TO_LP);
			}
		}

		if(dCH__SCH_TEST_ONLY_PLACE_TO_AL->Check__DATA(STR__YES) > 0)
		{
			dCH__SCH_TEST_ONLY_PLACE_TO_AL->Set__DATA(STR__NO);
			return -10001;
		}

		if((xCH__ATM_RB__TARGET_LLx_MODE->Check__DATA(STR__ENABLE) > 0)
		|| (xCH__ATM_RB__TARGET_PMx_MODE->Check__DATA(STR__ENABLE) > 0))
		{
			CString trg__al_name;
			CString trg__al_slot;

			if(xCH__ATM_RB__TARGET_LLx_MODE->Check__DATA(STR__ENABLE) > 0)
			{
				trg__al_name = SCH__Get_LBx_To_PLACE(sch_module, ll_slot_count, trg__al_slot);
			}
			else if(xCH__ATM_RB__TARGET_PMx_MODE->Check__DATA(STR__ENABLE) > 0)
			{
				trg__al_name = mMNG__PMx_ALIGN.Get__NEXT_ALIGN(sch_module);
				trg__al_slot = "1";
			}

			if(trg__al_name.GetLength() < 1)
			{
				return -12345;
			}
				
			xCH__ATM_RB__TARGET_LLx_NAME_SET_ALL->Set__DATA(trg__al_name);
			xCH__ATM_RB__TARGET_LLx_SLOT_SET_ALL->Set__DATA(trg__al_slot);
		}
		else
		{
			xCH__ATM_RB__TARGET_LLx_NAME_SET_ALL->Set__DATA("");
			xCH__ATM_RB__TARGET_LLx_SLOT_SET_ALL->Set__DATA("");
		}

		if(SCH__PICK_MODULE(p_variable,p_alarm, log_id, false,arm_type,para_module,para_in_slot,sch_module) < 0)
		{
			return -1;
		}

		xSCH_MATERIAL_CTRL->Pick__From_MODULE(sch_module,arm_type);

		/*
		// ...
		{
			CString log_msg;
			CString log_bff;

			log_msg = "Fnc__PLACE_AL() ... \n";
			log_msg += " 1) PLACE_AL() -> PICK_AL() \n";

			log_bff.Format(" * %s -> %s \n", arm_type,sch_module);
			log_msg += log_bff;

			log_bff.Format(" * al_start_flag <- %1d \n", al_start_flag);
			log_msg += log_bff;

			log_bff.Format(" * ll_slot_count <- %1d \n", ll_slot_count);
			log_msg += log_bff;

			log_bff.Format(" * %s -> %s \n", 
							xCH__ATM_RB__TARGET_LLx_NAME_SET_ALL->Get__CHANNEL_NAME(),
							xCH__ATM_RB__TARGET_LLx_NAME_SET_ALL->Get__STRING());
			log_msg += log_bff;

			log_bff.Format(" * %s -> %s \n", 
							xCH__ATM_RB__TARGET_LLx_SLOT_SET_ALL->Get__CHANNEL_NAME(),
							xCH__ATM_RB__TARGET_LLx_SLOT_SET_ALL->Get__STRING());
			log_msg += log_bff;

			printf(log_msg);
		}
		*/
	}
	else
	{
		if((xCH__ATM_RB__TARGET_LLx_MODE->Check__DATA(STR__ENABLE) > 0)
		|| (xCH__ATM_RB__TARGET_PMx_MODE->Check__DATA(STR__ENABLE) > 0))
		{
			CString trg__al_name;
			CString trg__al_slot;

			if(xCH__ATM_RB__TARGET_LLx_MODE->Check__DATA(STR__ENABLE) > 0)
			{
				CString sch_name;
				sch_name.Format("%s-%s", para_module,para_in_slot );

				trg__al_name = SCH__Get_LBx_To_PLACE(sch_name, ll_slot_count, trg__al_slot);
			}
			else if(xCH__ATM_RB__TARGET_PMx_MODE->Check__DATA(STR__ENABLE) > 0)
			{
				trg__al_name = mMNG__PMx_ALIGN.Get__NEXT_ALIGN(arm_type);
				trg__al_slot = "1";
			}

			if(trg__al_name.GetLength() < 1)
			{
				NEXT__LOOP;
			}
				
			xCH__ATM_RB__TARGET_LLx_NAME_SET_ALL->Set__DATA(trg__al_name);
			xCH__ATM_RB__TARGET_LLx_SLOT_SET_ALL->Set__DATA(trg__al_slot);
		}
		else
		{
			xCH__ATM_RB__TARGET_LLx_NAME_SET_ALL->Set__DATA("");
			xCH__ATM_RB__TARGET_LLx_SLOT_SET_ALL->Set__DATA("");
		}

		if((ex_flag == true)
		&& (xCH_CFG__ALx_SWAP_FLAG->Check__DATA(STR__ENABLE) > 0)
		&& (al_start_flag > 0))
		{
			CString sch_place_module;
			CString sch_pick_module;

			sch_place_module.Format("%s-%s",para_module,para_in_slot );
			sch_pick_module.Format( "%s-%s",para_module,para_out_slot);

			if(Fnc__SWAP_ALx(p_variable,
							 para_module,
							 arm_type,
							 para_in_slot,
							 arm_type,
							 para_out_slot,
							 sch_place_module,
							 sch_pick_module) < 0)
			{
				xEXT_CH__JOB_CTRL->Set__DATA("Abort");

				// ...
				int alarm_id = ALID__SWAP_ERROR;
				CString alarm_msg;
				CString r_act;

				alarm_msg.Format("Swap Error occured when ATM-Robot is swaping with %s.\n",para_module);

				p_alarm->Check__ALARM(alarm_id,r_act);
				p_alarm->Post__ALARM_With_MESSAGE(alarm_id,alarm_msg);

				Sleep(990);
				xSCH_MATERIAL_CTRL->Clear__ALL_MATERIAL_INFO();

				NEXT__LOOP;			
			}
		}
		else
		{
			CString sch_module;
			sch_module.Format("%s-%s",para_module,para_in_slot);

			if(SCH__PLACE_MODULE(p_variable,
								 p_alarm,
								 log_id,
								 ex_flag,
								 arm_type,
								 para_module,
								 para_in_slot,
								 sch_module) < 0)
			{
				return -1;
			}

			// ...
			{
				xSCH_MATERIAL_CTRL->Place__To_MODULE(arm_type,sch_module);

				if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(sch_module) > 0)
				{
					if(al_start_flag > 0)
					{
						xSCH_MATERIAL_CTRL->Set__MATERIAL_STATUS(sch_module, STR__ALIGNED);
					}
				}
				else
				{
					xSCH_MATERIAL_CTRL->Set__MATERIAL_STATUS(sch_module, STR__TO_LP);
				}
			}
			
			if(ex_flag == false)			return 1;
			if(AL1__Is_Available() < 0)		NEXT__LOOP;

			// ...
			CString empty_arm;

			if(ATM_RB__Get_Empty__Arm_Type(empty_arm) < 0)
			{
				NEXT__LOOP;
			}

			sch_module.Format("%s-%s",para_module,para_out_slot);

			if(SCH__PICK_MODULE(p_variable,p_alarm, log_id, false, empty_arm,para_module,para_out_slot,sch_module) < 0)
			{
				return -1;
			}

			xSCH_MATERIAL_CTRL->Pick__From_MODULE(sch_module, empty_arm);
		}
	
		if(ex_flag == true)
		{
			if(al_start_flag > 0)
			{
				SCH__ALIGN(p_variable,para_in_slot);
			}
		}
	}

	return 1;
}

// ATM_RB -> BUFFERx
int  CObj__DUAL_ARM_STD
::Fnc__PLACE_BUFFERx(CII_OBJECT__VARIABLE *p_variable,
					   CII_OBJECT__ALARM *p_alarm,
					   const CString& robot_arm,
					   const CString& para__stx_name,
					   const int para__stx_slot,
					   const CString& para__stx_lotid)
{
	CString log_id;
	log_id.Format("Fnc__PLACE_BUFFERx() - LotID(%s)", para__stx_lotid);

	// ...
	CStringArray l__arm_type;
	CStringArray l__stx_name;
	CUIntArray   l__stx_slot;

	if(robot_arm.CompareNoCase(ARM_AB) == 0)
	{
		l__arm_type.Add(ARM_A);
		l__stx_name.Add(para__stx_name);
		l__stx_slot.Add(para__stx_slot);

		l__arm_type.Add(ARM_B);
		l__stx_name.Add(para__stx_name);
		l__stx_slot.Add(para__stx_slot + 1);
	}
	else
	{
		l__arm_type.Add(robot_arm);
		l__stx_name.Add(para__stx_name);
		l__stx_slot.Add(para__stx_slot);
	}

	// ...
	int arm_size = l__arm_type.GetSize();
	int arm_i;

	for(arm_i=0; arm_i<arm_size; arm_i++)
	{
		CString arm_type = l__arm_type[arm_i];

		CStringArray l_id;
		CStringArray l_mode;
		CStringArray l_slot; 
		CStringArray l_sec;

		if(xSCH_MATERIAL_CTRL->Get__STx_OF_EDIT_TYPE(arm_type, l_id,l_mode,l_slot,l_sec) > 0)
		{
			int i_limit = l_id.GetSize();

			for(int i=0; i<i_limit; i++)
			{
				int stx_id = atoi(l_id[i]);
				CString str_sec = l_sec[i];

					 if(stx_id == 1)		xCH__SCH_DB_ST1_WAFER_CLEAN_TIME->Set__DATA(str_sec);
				else if(stx_id == 2)		xCH__SCH_DB_ST2_WAFER_CLEAN_TIME->Get__DATA(str_sec);
			}
		}
	}

	// ...
	{
		CString var_data;
		
		xCH__SCH_DB_ST1_WAFER_CLEAN_TIME->Get__DATA(var_data);
		xEXT_CH__BUFFER1__CFG_SLOT_WAIT_SEC->Set__DATA(var_data);

		xCH__SCH_DB_ST2_WAFER_CLEAN_TIME->Get__DATA(var_data);
		xEXT_CH__BUFFER2__CFG_SLOT_WAIT_SEC->Set__DATA(var_data);
	}

	// ...
	CString sch_module;
	CString para_module;
	CString para_slot;

	if(robot_arm.CompareNoCase(ARM_AB) == 0)
	{
		para_module = para__stx_name;
		para_slot.Format("%1d", para__stx_slot);

		sch_module.Format("%s-%s", para_module,para_slot);

		if(SCH__PLACE_MODULE(p_variable,
							 p_alarm,
			                 log_id,
							 false,
							 robot_arm,
							 para_module,
							 para_slot,
							 sch_module) < 0)
		{
			return -1;
		}
	}
	else
	{
		arm_size = l__arm_type.GetSize();

		for(arm_i=0; arm_i<arm_size; arm_i++)
		{
			CString arm_type = l__arm_type[arm_i];

			para_module = l__stx_name[arm_i];
			para_slot.Format("%1d", l__stx_slot[arm_i]);

			sch_module.Format("%s-%s", para_module,para_slot);

			if(SCH__PLACE_MODULE(p_variable,
								 p_alarm,
								 log_id,
								 false,
								 arm_type,
								 para_module,
								 para_slot,
								 sch_module) < 0)
			{
				return -1;
			}
		}
	}

	// ...
	arm_size = l__arm_type.GetSize();

	for(arm_i=0; arm_i<arm_size; arm_i++)
	{
		CString arm_type = l__arm_type[arm_i];
		int trg__slot_id = l__stx_slot[arm_i];

		para_module = l__stx_name[arm_i];
		para_slot.Format("%1d", trg__slot_id);

		sch_module.Format("%s-%s", para_module,para_slot);

		// ...
		{
			xSCH_MATERIAL_CTRL->Place__To_MODULE(arm_type, sch_module);

			xSCH_MATERIAL_CTRL->Set__MATERIAL_STATUS(sch_module, STR__TO_LP);
		}

		// ...
		int slot_index = trg__slot_id - 1;

		if((slot_index >= 0) && (slot_index < CFG_LP__SLOT_MAX))
		{
			CString var_data;
			
			if(para_module.CompareNoCase(MODULE__BUFFER1) == 0)
			{
				xCH__SCH_DB_ST1_WAFER_CLEAN_TIME->Get__DATA(var_data);
				xEXT_CH__BUFFER1__SLOT_WAIT_SEC[slot_index]->Set__DATA(var_data);
			}
			else if(para_module.CompareNoCase(MODULE__BUFFER2) == 0)
			{
				xCH__SCH_DB_ST2_WAFER_CLEAN_TIME->Get__DATA(var_data);
				xEXT_CH__BUFFER2__SLOT_WAIT_SEC[slot_index]->Set__DATA(var_data);
			}
		}
	}

	return 1;
}

// EXCEPTION : LBin -> ATM_RB
int  CObj__DUAL_ARM_STD
::Fnc__PICK_LBx_EXCEPTION(CII_OBJECT__VARIABLE *p_variable,
							CII_OBJECT__ALARM *p_alarm,
							const CString& para_module,
							const CString& para_slot)
{
	CString log_id = "Fnc__PICK_LBx_EXCEPTION()";

	// ...
	{
		if(ATM_RB__Is_Available() < 0)					NEXT__LOOP;
		if(ATM_RB__Check_Empty__Arm_Type() < 0)			NEXT__LOOP;
	}

	// ...
	{
		CString arm_type;
		CString sch_module;

		if(ATM_RB__Get_Empty__Arm_Type(arm_type) < 0)
		{
			NEXT__LOOP;
		}

		// ...
		sch_module.Format("%s-%s", para_module,para_slot);

		// ...
		{
			CString pmc_name;
			CString pmc_rcp;

			if(xSCH_MATERIAL_CTRL->Get__Last_Processing_Info(sch_module, pmc_name,pmc_rcp) > 0)
			{
				if(Get__LPx_SideBuffer_Info_To_Use(sch_module) < 0)
				{
					return -1;
				}
			}	
		}

		if(SCH__PICK_MODULE(p_variable,p_alarm, log_id, false,arm_type,para_module,para_slot,sch_module) < 0)
		{
			NEXT__LOOP;
		}
		
		xSCH_MATERIAL_CTRL->Pick__From_MODULE(sch_module, arm_type);			

		// Check : Wafer (Mapped) ....
		{
			int stx_check = 1;

			if(arm_type.CompareNoCase(ARM_A) == 0)
			{
				if(xCH__ATM_RB__SLOT01_STATUS->Check__DATA(STR__MAPPED) > 0)		stx_check = -1;
			}
			else if(arm_type.CompareNoCase(ARM_B) == 0)
			{
				if(xCH__ATM_RB__SLOT02_STATUS->Check__DATA(STR__MAPPED) > 0)		stx_check = -1;
			}

			if(stx_check < 0)
			{
				xSCH_MATERIAL_CTRL->Set__MATERIAL_STATUS(arm_type, STR__TO_LP);
			}
		}
	}

	return 1;
}
	
// ALx -> ATM_RB
int  CObj__DUAL_ARM_STD
::Fnc__PICK_AL(CII_OBJECT__VARIABLE *p_variable,
			   CII_OBJECT__ALARM *p_alarm,
			   CString& arm_type,
			   const bool active__arm_use)
{
	CString log_id = "Fnc__PICK_AL()";

	if(AL1__Is_Available() < 0)
	{
		printf(" * Fnc__PICK_AL() - Error(1) \n");
		NEXT__LOOP;
	}
	if(AL1__Check_Empty__All_Slot() > 0)
	{
		printf(" * Fnc__PICK_AL() - Error(2) \n");
		NEXT__LOOP;
	}

	if(active__arm_use)
	{
		if(ATM_RB__Check_Empty__Arm_Type(arm_type) < 0)
		{
			printf(" * Fnc__PICK_AL() - Error(3.1) \n");
			NEXT__LOOP;
		}
	}
	else
	{
		if(ATM_RB__Get_Empty__Arm_Type(arm_type) < 0)
		{
			printf(" * Fnc__PICK_AL() - Error(3.2) \n");
			NEXT__LOOP;
		}
	}

	// ...
	CString para_module = MODULE__AL1;
	CString para_slot;
	int al_slot_id;

	if(AL1__Get_Occupied__SlotID(al_slot_id) < 0)
	{
		printf(" * Fnc__PICK_AL() - Error(5) \n");
		NEXT__LOOP;
	}
	para_slot.Format("%1d", al_slot_id);

	// ...
	{
		CString sch_module;
		sch_module.Format("%s-%s", para_module,para_slot);

		if(xEXT_CH__SCH_ALIGN_DEVICE->Check__DATA(STR__ATM_RB) > 0)
		{
			if((xCH__ATM_RB__TARGET_LLx_MODE->Check__DATA(STR__ENABLE) > 0)
			|| (xCH__ATM_RB__TARGET_PMx_MODE->Check__DATA(STR__ENABLE) > 0))
			{
				CString trg__al_name = "?";
				CString trg__al_slot = "?";

				if(xCH__ATM_RB__TARGET_LLx_MODE->Check__DATA(STR__ENABLE) > 0)
				{
					trg__al_name = SCH__Get_LBx_To_PLACE(sch_module, trg__al_slot);
				}
				else if(xCH__ATM_RB__TARGET_PMx_MODE->Check__DATA(STR__ENABLE) > 0)
				{
					trg__al_name = mMNG__PMx_ALIGN.Get__NEXT_ALIGN(sch_module);
					trg__al_slot = "1";
				}

				/*
				// ...
				{
					CString log_msg;
					CString log_bff;

					log_msg = "Fnc__PICK_AL() ... \n";

					log_bff.Format(" * sch_name <- [%s] \n", sch_module);
					log_msg += log_bff;

					log_bff.Format(" * trg__al_name <- [%s] \n", trg__al_name);
					log_msg += log_bff;

					log_bff.Format(" * trg__al_slot <- [%s] \n", trg__al_slot);
					log_msg += log_bff;

					// ...
					{
						int ll_limit = iLLx_SIZE;
						int ll_i;

						for(ll_i=0; ll_i<ll_limit; ll_i++)
						{
							if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_i]->Check__DATA(LBx_MODE__ONLY_OUTPUT) > 0)
							{
								continue;
							}

							log_bff.Format("  ** %s <- %s \n",
											dCH__ATM_RB__INFO_LL_RESERVE_X[ll_i]->Get__CHANNEL_NAME(),
											dCH__ATM_RB__INFO_LL_RESERVE_X[ll_i]->Get__STRING());
							log_msg += log_bff;

							// ...
							CUIntArray l_ll_slot;
							LLx__Check_Empty__InSlot(ll_i, l_ll_slot);

							int t_limit = l_ll_slot.GetSize();
							int t;

							log_bff.Format("  LL%1d - Slot_List (%1d) \n", ll_i + 1, t_limit);
							log_msg += log_bff;

							for(t=0; t<t_limit; t++)
							{
								log_bff.Format("    * %1d) %1d \n", t, l_ll_slot[t]);
								log_msg += log_bff;
							}
						}
					}

					// ...
					{
						CStringArray l__pm_name;
						CStringArray l__pm_rcp;

						xSCH_MATERIAL_CTRL->Get__CUR_PROCESS_INFO(sch_module, l__pm_name,l__pm_rcp);

						// ...
						int k_limit = l__pm_name.GetSize();

						log_bff.Format("  PMC List (%1d) \n", k_limit);
						log_msg += log_bff;

						for(int k=0; k<k_limit; k++)
						{
							CString pmc_name = l__pm_name[k];

							int pm_index = _Get__PM_INDEX(pmc_name);
							if(pm_index < 0)		continue;

							if(PMx__Is_Ready(pm_index) < 0)
							{
								continue;
							}

							int pm_id = pm_index + 1;
							int pm_check = pm_id % 2;

							if(pm_check == 1)
							{
								bool active__check_type = LLx__Check_Empty__InSlot_Of_Odd_Type();

								log_bff.Format("    * Odd_Type : %1d \n", active__check_type);
								log_msg += log_bff;
							}
							else if(pm_check == 0)
							{
								bool active__check_type = LLx__Check_Empty__InSlot_Of_Even_Type();

								log_bff.Format("    * Even_Type : %1d \n", active__check_type);
								log_msg += log_bff;
							}
						}
					}

					printf(log_msg);
				}
				*/

				/*
				if(trg__al_name.GetLength() < 1)
				{
					NEXT__LOOP;
				}
				*/

				xCH__ATM_RB__TARGET_LLx_NAME_SET_ALL->Set__DATA(trg__al_name);
				xCH__ATM_RB__TARGET_LLx_SLOT_SET_ALL->Set__DATA(trg__al_slot);
			}
			else
			{
				xCH__ATM_RB__TARGET_LLx_NAME_SET_ALL->Set__DATA("");
				xCH__ATM_RB__TARGET_LLx_SLOT_SET_ALL->Set__DATA("");
			}
		}

		if(SCH__PICK_MODULE(p_variable,p_alarm, log_id, false,arm_type,para_module,para_slot,sch_module) < 0)
		{
			printf(" * Fnc__PICK_AL() - Error(6) \n");
			NEXT__LOOP;
		}

		xSCH_MATERIAL_CTRL->Pick__From_MODULE(sch_module,arm_type);

		if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(arm_type) > 0)
		{
			xSCH_MATERIAL_CTRL->Set__MATERIAL_STATUS(arm_type, STR__ALIGNED);
		}
		else
		{
			xSCH_MATERIAL_CTRL->Set__MATERIAL_STATUS(arm_type, STR__TO_LP);
		}

		// ...
		{
			CString log_msg;
			CString log_bff;

			log_msg = "Fnc__PICK_AL() ... \n";

			log_bff.Format( " * %s -> %s \n", sch_module, arm_type);
			log_msg += log_bff;

			printf(log_msg);
		}
	}
	return 1;
}
int  CObj__DUAL_ARM_STD
::Fnc__PICK_AL_TO_RB(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm)
{
	CString log_id = "Fnc__PICK_AL_TO_RB()";

	if(AL1__Is_Available() < 0)
	{
		NEXT__LOOP;
	}
	if(AL1__Check_Empty__All_Slot() > 0)
	{
		NEXT__LOOP;
	}

	// ...
	CString arm_type;

	if(ATM_RB__Get_Empty__Arm_Type(arm_type) < 0)
	{
		NEXT__LOOP;
	}

	// ...
	CString para_module = MODULE__AL1;
	CString para_slot;
	int al_slot_id;
	
	if(AL1__Get_Occupied__SlotID(al_slot_id) < 0)
	{
		NEXT__LOOP;
	}
	para_slot.Format("%1d",al_slot_id);

	// ...
	{
		CString sch_module;
		sch_module.Format("%s-%s",para_module,para_slot);

		if((xCH__ATM_RB__TARGET_LLx_MODE->Check__DATA(STR__ENABLE) > 0)
		|| (xCH__ATM_RB__TARGET_PMx_MODE->Check__DATA(STR__ENABLE) > 0))
		{
			CString trg__al_name;
			CString trg__al_slot;

			if(xCH__ATM_RB__TARGET_LLx_MODE->Check__DATA(STR__ENABLE) > 0)
			{
				trg__al_name = SCH__Get_LBx_To_PLACE(sch_module, trg__al_slot);
			}
			else if(xCH__ATM_RB__TARGET_PMx_MODE->Check__DATA(STR__ENABLE) > 0)
			{
				trg__al_name = mMNG__PMx_ALIGN.Get__NEXT_ALIGN(sch_module);
				trg__al_slot = "1";
			}

			if(trg__al_name.GetLength() < 1)
			{
				NEXT__LOOP;
			}

			if(arm_type.CompareNoCase(ARM_A) == 0)
			{
				xCH__ATM_RB__TARGET_LLx_NAME_SET_OF_ARM_A->Set__DATA(trg__al_name);
				xCH__ATM_RB__TARGET_LLx_SLOT_SET_OF_ARM_A->Set__DATA(trg__al_slot);
			}
			else if(arm_type.CompareNoCase(ARM_B) == 0)
			{
				xCH__ATM_RB__TARGET_LLx_NAME_SET_OF_ARM_B->Set__DATA(trg__al_name);
				xCH__ATM_RB__TARGET_LLx_SLOT_SET_OF_ARM_B->Set__DATA(trg__al_slot);
			}
		}
		else
		{
			if(arm_type.CompareNoCase(ARM_A) == 0)
			{
				xCH__ATM_RB__TARGET_LLx_NAME_SET_OF_ARM_A->Set__DATA("");
				xCH__ATM_RB__TARGET_LLx_SLOT_SET_OF_ARM_A->Set__DATA("");
			}
			else if(arm_type.CompareNoCase(ARM_B) == 0)
			{
				xCH__ATM_RB__TARGET_LLx_NAME_SET_OF_ARM_B->Set__DATA("");
				xCH__ATM_RB__TARGET_LLx_SLOT_SET_OF_ARM_B->Set__DATA("");
			}
		}

		if(SCH__PICK_MODULE(p_variable,p_alarm, log_id, false, arm_type,para_module,para_slot,sch_module) < 0)
		{
			NEXT__LOOP;
		}
		
		xSCH_MATERIAL_CTRL->Pick__From_MODULE(sch_module,arm_type);
		
		if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(arm_type) > 0)
		{
			xSCH_MATERIAL_CTRL->Set__MATERIAL_STATUS(arm_type, STR__ALIGNED);
		}
		else
		{
			xSCH_MATERIAL_CTRL->Set__MATERIAL_STATUS(arm_type, STR__TO_LP);
		}
	}
	return 1;
}

int  CObj__DUAL_ARM_STD::
Fnc__ALIGN_RETRY_TO_PLACE_LLx(CII_OBJECT__VARIABLE *p_variable,
							  CII_OBJECT__ALARM *p_alarm,
							  const bool active__ll_all_mode, 
							  const CString& arm_type, 
							  const int ll_slot_count)
{
	// CHECK : ARM.RESET ...
	if(active__ll_all_mode)
	{

	}
	else
	{
		CString arm__ll_name;

			 if(arm_type.CompareNoCase(ARM_A) == 0)			arm__ll_name = xCH__ATM_RB__TARGET_LLx_NAME_SET_OF_ARM_A->Get__STRING();
		else if(arm_type.CompareNoCase(ARM_B) == 0)			arm__ll_name = xCH__ATM_RB__TARGET_LLx_NAME_SET_OF_ARM_B->Get__STRING();

		int ll_i = Get__LLx_INDEX(arm__ll_name);
		if(ll_i >= 0)		dCH__ATM_RB__INFO_LL_RESERVE_X[ll_i]->Set__DATA(STR__OFF);
	}

	// ...
	CString ch_data = aCH__ATM_RB__CFG_ALIGN_RETRY->Get__STRING();
	sCH__ATM_RB__CUR_ALIGN_RETRY->Set__DATA(ch_data);
	int cur_count = atoi(ch_data);

	while(cur_count > 0)
	{
		cur_count--;

		// RB -> AL
		int r_flag = Fnc__PLACE_AL(p_variable,p_alarm, "", arm_type, 1, ll_slot_count);
		if(r_flag < 0)			return r_flag;

		if(ATM_RB__Check_Occupied__Arm_Type(arm_type) > 0)
		{

		}
		else
		{
			CString empty_arm = arm_type;

			// AL -> RB
			if(Fnc__PICK_AL(p_variable,p_alarm, empty_arm, true) < 0)
			{
				return -12;
			}
		}

		ch_data.Format("%1d", cur_count);
		sCH__ATM_RB__CUR_ALIGN_RETRY->Set__DATA(ch_data);
	}

	// CHECK : ARM.RESERVE ...
	if(active__ll_all_mode)
	{
		CString arm__ll_name;

			 if(arm_type.CompareNoCase(ARM_A) == 0)			arm__ll_name = xCH__ATM_RB__TARGET_LLx_NAME_SET_OF_ARM_A->Get__STRING();
		else if(arm_type.CompareNoCase(ARM_B) == 0)			arm__ll_name = xCH__ATM_RB__TARGET_LLx_NAME_SET_OF_ARM_B->Get__STRING();

		int ll_i = Get__LLx_INDEX(arm__ll_name);
		if(ll_i >= 0)		dCH__ATM_RB__INFO_LL_RESERVE_X[ll_i]->Set__DATA(STR__ON);
	}

	return 1;
}

// ATM_RB -> LBi
int  CObj__DUAL_ARM_STD::
Fnc__PLACE_LBi__ONLY_MODE(CII_OBJECT__VARIABLE *p_variable,
					      CII_OBJECT__ALARM *p_alarm,
					      const bool ex_flag,
					      const CString& para__arm_type)
{
	DECLARE__EXT_CTRL(p_variable);

	CString log_id = "Fnc__PLACE_LBi__ONLY_MODE()";

	// ...
	CString para_module;
	CString para_slot;
	int slot_id;

	// ...
	bool active__pm_odd  = false;
	bool active__pm_even = false;

	bool active__ll_odd  = false;
	bool active__ll_even = false;

	// ...
	bool active__left_right = false;

	if(dEXT_CH__CFG_LLx_CTRL_ONLY_INPUT_OUTPUT_MODE->Check__DATA(STR__DUAL) > 0)
	{
		active__left_right = true;
	}
	else
	{
		active__pm_odd  = true;
		active__pm_even = true;

		active__ll_odd  = true;
		active__ll_even = true;
	}

	// PMx Check ...
	if(active__left_right)
	{
		CStringArray l__pm_name;
		CStringArray l__pm_rcp;

		xSCH_MATERIAL_CTRL->Get__CUR_PROCESS_INFO(para__arm_type, l__pm_name,l__pm_rcp);

		// ...
		int k_limit = l__pm_name.GetSize();

		for(int k=0; k<k_limit; k++)
		{
			CString pmc_name = l__pm_name[k];

			int pm_index = _Get__PM_INDEX(pmc_name);
			if(pm_index < 0)		continue;

			if(PMx__Is_Ready(pm_index) < 0)
			{
				continue;
			}

			int pm_id = pm_index + 1;
			int pm_check = pm_id % 2;
			
			if(pm_check == 1)
			{
				if(LLx__Check_Empty__InSlot_Of_Odd_Type())
				{
					active__pm_odd  = true;
					active__ll_odd  = true;
				}
			}
			else if(pm_check == 0)
			{
				if(LLx__Check_Empty__InSlot_Of_Even_Type())
				{
					active__pm_even = true;
					active__ll_even = true;
				}
			}
		}

		// ...
		{
			CString def_name = USER_DEF__TRG_MDx;
			CString def_data;

			xSCH_MATERIAL_CTRL->Get__MATERIAL_USER_INFO(para__arm_type, def_name,def_data);
			CString trg__goto_pm = def_data;

			int pm_index = _Get__PM_INDEX(trg__goto_pm);
			if(pm_index >= 0)
			{
				if(PMx__Is_Ready(pm_index) > 0)
				{
					int pm_id = pm_index + 1;
					int pm_check = pm_id % 2;

					if(pm_check == 1)
					{
						if(LLx__Check_Empty__InSlot_Of_Odd_Type())
						{
							active__ll_odd  = true;
							active__ll_even = false;
						}
					}
					else if(pm_check == 0)
					{
						if(LLx__Check_Empty__InSlot_Of_Even_Type())
						{
							active__ll_odd  = false;
							active__ll_even = true;
						}
					}
				}
			}
		}
	}

	// LLx ...
	int active__ll_only_input = -1;

	if(sEXT_CH__SCH_MODE_LLx_IN->Check__DATA(_SCH_MODE__ROUTE) > 0)
	{
		CStringArray l_ll_id;
		CStringArray l_ll_mode;
		CStringArray l_ll_slot;

		xSCH_MATERIAL_CTRL->Get__LLx_IN_OF_EDIT_TYPE(para__arm_type, l_ll_id,l_ll_mode,l_ll_slot);

		// ...
		int i_limit = l_ll_id.GetSize();
		int i;

		for(i=0; i<i_limit; i++)
		{
			int ll_i = atoi(l_ll_id[i]) - 1;

			if(ll_i <  0)				continue;
			if(ll_i >= iLLx_SIZE)		continue;

			if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_i]->Check__DATA(LBx_MODE__ONLY_INPUT) < 0)
			{
				continue;
			}

			if(xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Check__DATA(STR__IN) < 0)
			{
				continue;
			}

			if(LLx__Is_Available(ll_i) < 0)
			{
				continue;
			}
			
			if((active__ll_odd) && (active__ll_even))
			{
				if(LLx__Get_Empty__InSlot(ll_i, slot_id) < 0)					continue;
			}
			else if(active__ll_odd)
			{
				if(LLx__Get_Empty__InSlot_Of_Odd_Type(ll_i, slot_id) < 0)		continue;
			}
			else if(active__ll_even)
			{
				if(LLx__Get_Empty__InSlot_Of_Even_Type(ll_i, slot_id) < 0)		continue;
			}
			else
			{
				continue;
			}

			if(LLx__Is_ATM(ll_i) < 0)
			{
				if(xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Check__DATA(STR__OUT) < 0)
				{
					SCH_RUN__LLx_VENT(ll_i, p_variable, log_id, "1");
				}
				continue;
			}

			active__ll_only_input = 1;

			para_module = Get__LLx_NAME(ll_i);
			para_slot.Format("%1d", slot_id);
			break;
		}
	}
	else
	{
		int ll_limit = iLLx_SIZE;
		int ll_i;

		for(ll_i=0; ll_i<ll_limit; ll_i++)
		{
			if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_i]->Check__DATA(LBx_MODE__ONLY_INPUT) < 0)
			{
				continue;
			}

			if(xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Check__DATA(STR__IN) < 0)
			{
				continue;
			}

			if(LLx__Is_Available(ll_i) < 0)						continue;
			if(LLx__Get_Empty__InSlot(ll_i, slot_id) < 0)		continue;

			if(LLx__Is_ATM(ll_i) < 0)
			{
				if(xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Check__DATA(STR__OUT) < 0)
				{
					SCH_RUN__LLx_VENT(ll_i, p_variable, log_id, "1");
				}
				continue;
			}

			active__ll_only_input = 1;

			para_module = Get__LLx_NAME(ll_i);
			para_slot.Format("%1d", slot_id);
			break;
		}
	}

	if(active__ll_only_input < 0)
	{
		if(active__left_right)
		{
			int ll_limit = iLLx_SIZE;
			int ll_i;

			for(ll_i=0; ll_i<ll_limit; ll_i++)
			{
				if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_i]->Check__DATA(LBx_MODE__ONLY_INPUT) < 0)
				{
					continue;
				}
				if(xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Check__DATA(STR__IN) < 0)
				{
					continue;
				}

				if(LLx__Is_Available(ll_i) < 0)						continue;
				if(LLx__Check_Occupied__InSlot(ll_i) < 0)			continue;

				if(SCH_RUN__LLx_PUMP(ll_i, p_variable, log_id, "LL.Abnormal") > 0)
				{
					xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Set__DATA("OUT");
				}
			}
		}

		/*
		// ...
		{
			CString log_msg;
			CString log_bff;

			log_msg.Format(" %s(%s) \n", para_module,para_slot);

			log_bff.Format(" * active__pm_odd  <- [%1d] \n", active__pm_odd);
			log_msg += log_bff;
			log_bff.Format(" * active__pm_even <- [%1d] \n", active__pm_even);
			log_msg += log_bff;

			log_bff.Format(" * InSlot_Of_Odd_Type  <- [%1d] \n", LLx__Check_Empty__InSlot_Of_Odd_Type());
			log_msg += log_bff;
			log_bff.Format(" * InSlot_Of_Even_Type <- [%1d] \n", LLx__Check_Empty__InSlot_Of_Even_Type());
			log_msg += log_bff;

			printf(log_msg);
		}
		*/
		return 1;
	}

	/*
	// ...
	{
		CString log_msg;
		CString log_bff;

		log_msg = "SCH__PLACE_MODULE(() : Start \n";
		log_bff.Format(" * arm_type <- %s \n", para__arm_type);
		log_msg += log_bff;
		log_bff.Format(" * para_module <- %s \n", para_module);
		log_msg += log_bff;
		log_bff.Format(" * para_slot <- %s \n", para_slot);
		log_msg += log_bff;
		log_bff.Format(" * active__ll_odd  <- %1d \n", active__ll_odd);
		log_msg += log_bff;
		log_bff.Format(" * active__ll_even <- %1d \n", active__ll_even);
		log_msg += log_bff;

		// ...
		{
			CString def_name = USER_DEF__TRG_MDx;
			CString def_data;

			xSCH_MATERIAL_CTRL->Get__MATERIAL_USER_INFO(para__arm_type, def_name,def_data);

			log_bff.Format("  ** %s <- %s \n", def_name, def_data);
			log_msg += log_bff;

			//
			int pm_index = _Get__PM_INDEX(def_data);

			log_bff.Format("  ** pm_index <- %1d \n", pm_index);
			log_msg += log_bff;

			if(pm_index >= 0)
			{
				int pm_ready = PMx__Is_Ready(pm_index);
				bool ll_even_type = LLx__Check_Empty__InSlot_Of_Even_Type();

				log_bff.Format("  ** pm_ready <- %1d \n", pm_ready);
				log_msg += log_bff;

				log_bff.Format("  ** ll_even_type <- %1d \n", ll_even_type);
				log_msg += log_bff;
			}
		}

		int ll_index = Get__LLx_INDEX(para_module);
		if(ll_index >= 0)
		{
			log_bff.Format(" * %s <- %s \n",
						   xEXT_CH__LLx__IN_OUT_FLAG[ll_index]->Get__CHANNEL_NAME(),
						   xEXT_CH__LLx__IN_OUT_FLAG[ll_index]->Get__STRING());
			log_msg += log_bff;
		}
		log_msg += "\n";

		xAPP_LOG_CTRL->WRITE__LOG(log_msg);
	}
	*/

	// ...
	{
		CString sch_module;
		sch_module.Format("%s-%s", para_module,para_slot);

		if(SCH__PLACE_MODULE(p_variable,
							 p_alarm,
							 log_id,
							 ex_flag,
							 para__arm_type,
							 para_module,
							 para_slot,
							 sch_module) < 0)
		{
			return -1;
		}

		xSCH_MATERIAL_CTRL->Place__To_MODULE(para__arm_type, sch_module);
	}

	// ...
	{
		int ll_i = Get__LLx_INDEX(para_module);

		if(ll_i >= 0)
		{			
			CString str_data;

			str_data.Format("%1d", ++iLLx_IN_COUNT);
			xEXT_CH__LLx__IN_COUNT[ll_i]->Set__DATA(str_data);
		}
	}

	if(ex_flag == true)
	{
		return 1;
	}

	if((active__pm_odd) && (active__pm_even))
	{
		int count__wfr_ready = 0;

		// ...
		{
			int arm_size = 2;

			for(int arm_i=0; arm_i < arm_size; arm_i++)
			{
				CString arm_type;

				if(arm_i == 0)		
				{
					if(ATM_RB__Check_Occupied__A_Arm() < 0)		continue;
					arm_type = ARM_A;
				}
				else if(arm_i == 1)		
				{
					if(ATM_RB__Check_Occupied__B_Arm() < 0)		continue;
					arm_type = ARM_B;
				}
				else
				{
					continue;
				}

				if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(arm_type) < 0)
				{
					continue;
				}

				if(dCH__CFG_PRE_ALIGN_USE->Check__DATA(STR__YES) > 0)
				{
					IDS__SCH_MATERIAL_STATUS ds_info;
					xSCH_MATERIAL_CTRL->Get__MATERIAL_STATUS(arm_type, ds_info);

					if(ds_info.sMATERIAL_STS.CompareNoCase(STR__ALIGNED) != 0)
					{
						continue;
					}
				}
					
				count__wfr_ready++;
				break;
			}
		}

		if(count__wfr_ready < 1)
		{
			Fnc__PUMP_LBi(p_variable,para_module);
		}
	}
	else if(active__pm_odd)
	{
		int ll_i = Get__LLx_INDEX(para_module);

		if(LLx__Check_All_Occupied__InSlot_Of_Odd_Type(ll_i))
		{
			if(SCH_RUN__LLx_PUMP(ll_i, p_variable, log_id, "Odd") > 0)
			{
				xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Set__DATA("OUT");
			}
		}
	}
	else if(active__pm_even)
	{
		int ll_i = Get__LLx_INDEX(para_module);

		if(LLx__Check_All_Occupied__InSlot_Of_Even_Type(ll_i))
		{
			if(SCH_RUN__LLx_PUMP(ll_i, p_variable, log_id, "Even") > 0)
			{
				xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Set__DATA("OUT");
			}
		}
	}

	// ...
	{
		CString log_msg;
		CString log_bff;

		log_msg = "SCH__PLACE_MODULE(() : End \n";

		log_bff.Format(" * active__pm_odd  <- [%1d] \n", active__pm_odd);
		log_msg += log_bff;
		log_bff.Format(" * active__pm_even <- [%1d] \n", active__pm_even);
		log_msg += log_bff;

		int ll_index = Get__LLx_INDEX(para_module);
		if(ll_index >= 0)
		{
			log_bff.Format(" * %s <- %s \n",
							xEXT_CH__LLx__IN_OUT_FLAG[ll_index]->Get__CHANNEL_NAME(),
							xEXT_CH__LLx__IN_OUT_FLAG[ll_index]->Get__STRING());
			log_msg += log_bff;
		}
		log_msg += "\n";

		xAPP_LOG_CTRL->WRITE__LOG(log_msg);
	}

	return 1;
}
int CObj__DUAL_ARM_STD::
Fnc__PUMP_LBi(CII_OBJECT__VARIABLE *p_variable,
			  const CString& para_module)
{
	CString log_id = "Fnc__PUMP_LBi()";

	// ...
	int ll_i = Get__LLx_INDEX(para_module);

	if(ll_i >= 0)
	{
		if(LLx__Check_Empty__InSlot(ll_i) < 0)
		{
			if(SCH_RUN__LLx_PUMP(ll_i, p_variable, log_id, "1") > 0)
			{
				xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Set__DATA("OUT");
			}
		}
		else
		{
			bool active__arm_wfr = false;

			for(int i=0; i<2; i++)
			{
				CString arm_type;

				if(i == 0)
				{
					if(ATM_RB__Check_Occupied__A_Arm() < 0)		continue;

					arm_type = ARM_A;
				}
				else if(i == 1)
				{
					if(ATM_RB__Check_Occupied__B_Arm() < 0)		continue;

					arm_type = ARM_B;
				}
				else
				{
					continue;
				}

				if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(arm_type) < 0)	
				{
					continue;
				}

				active__arm_wfr = true;
				break;
			}

			if((!active__arm_wfr)
			&& (AL1__Check_Empty__All_Slot() > 0))
			{
				if(xSCH_MATERIAL_CTRL->Check__MATERIAL_TO_PROCESS() < 0)
				{
					if(SCH_RUN__LLx_PUMP(ll_i, p_variable, log_id, "2") > 0)
					{
						xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Set__DATA("OUT");
					}
				}
			}
		}
	}
	
	return 1;
}


int CObj__DUAL_ARM_STD::
Fnc__SWAP_ALx(CII_OBJECT__VARIABLE *p_variable,
			  const CString& para_module,
			  const CString& place_arm,
			  const CString& place_slot,
			  const CString& pick_arm,
			  const CString& pick_slot,
			  const CString& sch_place_module,
			  const CString& sch_pick_module)
{
	// ...
	{
		xSCH_MATERIAL_CTRL->Transfer_Start__To_MODULE(place_arm,sch_place_module);
		xSCH_MATERIAL_CTRL->Transfer_Start__To_MODULE(sch_pick_module,pick_arm);
	}

	if(Fnc__Seq_SWAP_ALx(p_variable,
						 place_arm,
						 pick_arm,
						 para_module,
						 place_slot,
						 pick_slot) < 0)
	{
		return -1;
	}

	// ...
	{
		xSCH_MATERIAL_CTRL->Transfer_End__To_MODULE(place_arm,sch_place_module);
		xSCH_MATERIAL_CTRL->Transfer_End__To_MODULE(sch_pick_module,pick_arm);
	}

	// ...
	{
		xSCH_MATERIAL_CTRL->Place__To_MODULE(place_arm,sch_place_module);

		if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(sch_place_module) > 0)
		{
			xSCH_MATERIAL_CTRL->Set__MATERIAL_STATUS(sch_place_module, STR__ALIGNED);
		}
		else
		{
			xSCH_MATERIAL_CTRL->Set__MATERIAL_STATUS(sch_place_module, STR__TO_LP);
		}
	}

	// ...
	{
		xSCH_MATERIAL_CTRL->Pick__From_MODULE(sch_pick_module,pick_arm);
	}
	return 1;
}
int CObj__DUAL_ARM_STD::
Fnc__Seq_SWAP_ALx(CII_OBJECT__VARIABLE *p_variable,
				  const CString& place_arm,
			      const CString& pick_arm,
				  const CString& para_module,
				  const CString& place_slot,
				  const CString& pick_slot)
{
	DECLARE__EXT_CTRL(p_variable);
	
	// ...
	CStringArray l_para;

	l_para.Add(para_module);
	
	l_para.Add(place_arm);
	l_para.Add(place_slot);

	l_para.Add(pick_arm);
	l_para.Add(pick_slot);

	return Call__ROBOT_OBJ("SWAP_ALx", l_para);
}

int CObj__DUAL_ARM_STD::
Fnc__SWAP_LBx(CII_OBJECT__VARIABLE *p_variable,
			  CII_OBJECT__ALARM    *p_alarm,
			  const CString& para_module,
			  const CString& place_arm,
			  const CString& place_slot,
			  const CString& pick_arm,
			  const CString& pick_slot,
			  const CString& sch_place_module,
			  const CString& sch_pick_module)
{
	// ...
	{
		xSCH_MATERIAL_CTRL->Transfer_Start__To_MODULE(place_arm,sch_place_module);
		xSCH_MATERIAL_CTRL->Transfer_Start__To_MODULE(sch_pick_module,pick_arm);
	}

	if(Fnc__Seq_SWAP_LBx(p_variable,
						 place_arm,
						 pick_arm,
						 para_module,
						 place_slot,
						 pick_slot) < 0)
	{
		return -1;
	}

	// ...
	{
		xSCH_MATERIAL_CTRL->Transfer_End__To_MODULE(place_arm,sch_place_module);
		xSCH_MATERIAL_CTRL->Transfer_End__To_MODULE(sch_pick_module,pick_arm);
	}	

	// ...
	{
		xSCH_MATERIAL_CTRL->Place__To_MODULE(place_arm,sch_place_module);
		xSCH_MATERIAL_CTRL->Pick__From_MODULE(sch_pick_module,pick_arm);
	}

	Fnc__PUMP_LBi(p_variable,para_module);
	return 1;
}
int CObj__DUAL_ARM_STD::
Fnc__Seq_SWAP_LBx(CII_OBJECT__VARIABLE *p_variable,
				  const CString& place_arm,
			      const CString& pick_arm,
				  const CString& para_module,
				  const CString& place_slot,
				  const CString& pick_slot)
{
	DECLARE__EXT_CTRL(p_variable);
	
	// ...
	CStringArray l_para;

	l_para.Add(para_module);
	
	l_para.Add(place_arm);
	l_para.Add(place_slot);

	l_para.Add(pick_arm);
	l_para.Add(pick_slot);

	return Call__ROBOT_OBJ("SWAP_LBx", l_para);
}

int CObj__DUAL_ARM_STD::
Fnc__PLACE_LBi__ALL_MODE(CII_OBJECT__VARIABLE *p_variable,
					     CII_OBJECT__ALARM *p_alarm,
					     const bool ex_flag,
					     const CString& arm_type,
					     const CString& para_module,
					     const CString& para_slot)
{
	DECLARE__EXT_CTRL(p_variable);

	CString log_id = "Fnc__PLACE_LBi__ALL_MODE()";

	// ...
	int ll_i = Get__LLx_INDEX(para_module);

	if(ll_i >= 0)
	{
		if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_i]->Check__DATA(LBx_MODE__ALL) < 0)
		{
			NEXT__LOOP;
		}
	}

	// ...
	{
		CString sch_module;
		sch_module.Format("%s-%s",para_module,para_slot);

		if(SCH__PLACE_MODULE(p_variable,
							 p_alarm,
							 log_id,
							 ex_flag,
							 arm_type,
							 para_module,
							 para_slot,
							 sch_module) < 0)
		{
			return -1;
		}

		xSCH_MATERIAL_CTRL->Place__To_MODULE(arm_type,sch_module);
	}

	if(ll_i >= 0)
	{
		CString str_data;

		str_data.Format("%1d", ++iLLx_IN_COUNT);
		xEXT_CH__LLx__IN_COUNT[ll_i]->Set__DATA(str_data);			
	}

	if(ex_flag == true)
	{
		return 1;
	}

	if(ll_i >= 0)
	{
		if((LLx__Check_Occupied__OutSlot(ll_i) < 1)
		&& (LLx__Check_Occupied__InSlot(ll_i)  > 0))
		{
			if(SCH_RUN__LLx_PUMP(ll_i, p_variable, log_id, "1") > 0)
			{
				xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Set__DATA(STR__OUT);
			}
		}
		else
		{
			/*
			if((ATM_RB__Check_Empty__Arm_Type() > 0)
			&& (AL1__Check_Empty__All_Slot()    > 0))
			{
				if(xSCH_MATERIAL_CTRL->Check__MATERIAL_TO_PROCESS() < 0)
				{
					if(SCH_RUN__LLx_PUMP(ll_i, p_variable, log_id, "2") > 0)
					{
						xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Set__DATA(STR__OUT);
					}
				}
			}
			*/
		}
	}

	return 1;
}
int CObj__DUAL_ARM_STD::
Fnc__PICK_LBx(CII_OBJECT__VARIABLE *p_variable,
			  CII_OBJECT__ALARM *p_alarm,
		      const bool ex_flag,
			  const CString& arm_type,
			  const CString& para_module,
			  const CString& para_slot)
{
	DECLARE__EXT_CTRL(p_variable);

	CString log_id = "Fnc__PICK_LBx()";

	// ...
	{
		int active__all_mode = -1;

		int ll_limit = iLLx_SIZE;
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

	// ...
	CString sch_module;
	sch_module.Format("%s-%s",para_module,para_slot);

	if(SCH__PICK_MODULE(p_variable,p_alarm, log_id, ex_flag,arm_type,para_module,para_slot,sch_module) < 0)
	{
		return -1;
	}

	xSCH_MATERIAL_CTRL->Pick__From_MODULE(sch_module,arm_type);

	if(ex_flag == true)
	{
		return 1;
	}

	// LLx ...
	{
		int ll_i = Get__LLx_INDEX(para_module);

		if(ll_i >= 0)
		{
			if(LLx__Check_Empty__InSlot(ll_i) < 0)
			{
				if(SCH_RUN__LLx_PUMP(ll_i, p_variable, log_id, "1") > 0)
				{
					xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Set__DATA("OUT");
				}
			}
			else
			{
				if((ATM_RB__Check_Empty__Arm_Type() > 0)
				&& (AL1__Check_Empty__All_Slot() > 0))
				{
					if(xSCH_MATERIAL_CTRL->Check__MATERIAL_TO_PROCESS() < 0)
					{
						if(SCH_RUN__LLx_PUMP(ll_i, p_variable, log_id, "2") > 0)
						{
							xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Set__DATA("OUT");
						}
					}
				}
			}

			NEXT__LOOP;
		}
	}

	return 1;
}
int CObj__DUAL_ARM_STD::
Fnc__PLACE_LPx(CII_OBJECT__VARIABLE *p_variable,
			   CII_OBJECT__ALARM *p_alarm,
			   const CString& robot_arm)
{
	CString log_id = "Fnc__PLACE_LPx()";

	// ...
	CStringArray l__arm_type;
	CUIntArray   l__port_id;
	CUIntArray   l__slot_id;

	if(robot_arm.CompareNoCase(ARM_AB) == 0)
	{
		l__arm_type.Add(ARM_A);
		l__arm_type.Add(ARM_B);
	}
	else
	{
		l__arm_type.Add(robot_arm);
	}

	// ...
	int arm_size = l__arm_type.GetSize();
	int arm_i;

	for(arm_i=0; arm_i<arm_size; arm_i++)
	{
		CString arm_type = l__arm_type[arm_i];

		// ...
		int trg_port_id;
		int trg_slot_id;

		if(xSCH_MATERIAL_CTRL->Get__TARGET_LPx_INFO(arm_type, trg_port_id,trg_slot_id) < 0)
		{
			NEXT__LOOP;
		}

		if(Fnc__Check_ARM_WAFER_INFO_MISMATCH(p_variable,p_alarm, arm_type) < 0)
		{
			NEXT__LOOP;
		}

		if(LPx__Check_Empty__Slot_Status(trg_port_id, trg_slot_id) < 0)
		{
			int lp_index   = trg_port_id - 1;
			int slot_index = trg_slot_id - 1;
			CString var_data;

			int alarm_id = ALID__LPx_MATERIAL_PLACE__JOB_ERROR;
			CString alm_msg;
			CString alm_bff;
			CString r_act;

			alm_bff.Format("Please, check LP(%1d) and slot(%1d)'s status.\n",
						   trg_port_id, trg_slot_id);
			alm_msg += alm_bff;

			if((lp_index >= 0) && (lp_index < CFG_LP_LIMIT))
			{
				xCH__LPx__PORT_STATUS[lp_index]->Get__DATA(var_data);
					
				alm_bff.Format("   1. Port Status     : \"%s\" \n", var_data);
				alm_msg += alm_bff;
				alm_msg += "       status must be \"BUSY\" or \"PAUSED\". \n";

				// 
				xCH__LPx__CST_STATUS[lp_index]->Get__DATA(var_data);
					
				alm_bff.Format("   2. Cassette Status : \"%s\" \n", var_data);
				alm_msg += alm_bff;
				alm_msg += "       status must be \"EXIST\". \n";
			}
			if((slot_index >= 0) && (slot_index < CFG_LP__SLOT_MAX))
			{
				xCH__LPx__SLOT_STATUS[lp_index][slot_index]->Get__DATA(var_data);
					
				alm_bff.Format("   3. Slot Status     : \"%s\" \n", var_data);
				alm_msg += alm_bff;
				alm_msg += "       status must be \"NONE\". \n";
			}

			p_alarm->Check__ALARM(alarm_id,r_act);
			p_alarm->Post__ALARM_With_MESSAGE(alarm_id,alm_msg);

			NEXT__LOOP;
		}

		l__port_id.Add(trg_port_id);
		l__slot_id.Add(trg_slot_id);
	}

	// ...
	arm_size = l__arm_type.GetSize();

	for(arm_i=0; arm_i<arm_size; arm_i++)
	{
		CString arm_type = l__arm_type[arm_i];
		
		int port_id = l__port_id[arm_i];
		int slot_id = l__slot_id[arm_i];

		// ...
		CString para_module;
		CString para_slot;

		para_module.Format("LP%1d", port_id);
		para_slot.Format("%1d", slot_id);

		CString sch_module;
		sch_module.Format("%s-%s", para_module,para_slot);

		if(robot_arm.CompareNoCase(ARM_AB) == 0)
		{
			if(SCH__PLACE_MODULE(p_variable,
								 p_alarm,
								 log_id,
								 false,
								 robot_arm,
								 para_module,
								 para_slot,
								 sch_module) < 0)
			{
				NEXT__LOOP;
			}

			break;
		}
		else
		{
			if(SCH__PLACE_MODULE(p_variable,
								 p_alarm,
								 log_id,
								 false,
								 arm_type,
								 para_module,
								 para_slot,
								sch_module) < 0)
			{
				NEXT__LOOP;
			}
		}
	}

	// ...
	arm_size = l__arm_type.GetSize();

	for(arm_i=0; arm_i<arm_size; arm_i++)
	{
		CString arm_type = l__arm_type[arm_i];

		int port_id = l__port_id[arm_i];
		int slot_id = l__slot_id[arm_i];

		// ...
		CString para_module;
		CString para_slot;

		para_module.Format("LP%1d", port_id);
		para_slot.Format("%1d", slot_id);

		CString sch_module;
		sch_module.Format("%s-%s", para_module,para_slot);

		// ...
		xSCH_MATERIAL_CTRL->Place__To_LPx(arm_type, sch_module);
		xSCH_MATERIAL_CTRL->Clear__MATERIAL_INFO(sch_module);
		xSCH_MATERIAL_CTRL->Clear__MATERIAL_INFO(port_id, slot_id);

		if(xSCH_MATERIAL_CTRL->Check__MATERIAL_TO_PROCESS_IN_LPx(port_id) < 0)
		{
			int lp_index = port_id - 1;

			if((lp_index >= 0) && (lp_index < CFG_LP_LIMIT))
			{
				xCH__LPx__SIDE_BUFFER_USE_FLAG[lp_index]->Set__DATA("");
			}
		}
	}

	return 1;
}

int CObj__DUAL_ARM_STD::
Fnc__LBo_To_RB(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm)
{
	DECLARE__EXT_CTRL(p_variable);

	CString log_id = "Fnc__LBo_To_RB()";

	if(ATM_RB__Check_Empty__Arm_Type() < 0)
	{
		NEXT__LOOP;
	}

	// ...
	CString para_module;
	CString para_slot;
	int slot_id;

	// LLx ...
	{
		int out_flag = -1;

		int ll_limit = iLLx_SIZE;
		int ll_i;

		for(ll_i=0; ll_i<ll_limit; ll_i++)
		{
			if(out_flag < 0)
			{
				if((xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Check__DATA("IN") > 0)
				&& (LLx__Check_Empty__InSlot(ll_i) < 0))
				{
					if((LLx__Is_ATM(ll_i) > 0)
					&& (LLx__Is_Available(ll_i) > 0))
					{
						if(LLx__Get_Occupied__OutSlot(ll_i, slot_id) > 0)
						{
							para_module = Get__LLx_NAME(ll_i);
							para_slot.Format("%1d", slot_id);

							out_flag = 1;
						}
					}
				}
			}
		}

		if(out_flag < 0)
		{
			NEXT__LOOP;
		}
	}
	
	// ...
	CString arm_type;
	
	if(ATM_RB__Get_Empty__Arm_Type(arm_type) < 0)
	{
		NEXT__LOOP;
	}

	// ...
	CString sch_module;
	sch_module.Format("%s-%s",para_module,para_slot);

	if(Get__LPx_SideBuffer_Info_To_Use(sch_module) < 0)
	{
		NEXT__LOOP;
	}

	if(SCH__PICK_MODULE(p_variable,p_alarm, log_id, false,arm_type,para_module,para_slot,sch_module) < 0)
	{
		NEXT__LOOP;
	}
	xSCH_MATERIAL_CTRL->Pick__From_MODULE(sch_module,arm_type);

	// ...
	{
		int pump_req = -1;

		int ll_i = Get__LLx_INDEX(para_module);
		if(ll_i >= 0)
		{
			if(LLx__Check_Empty__InSlot(ll_i) < 0)
			{
				pump_req = 1;
			}
		}
		
		if(pump_req < 0)
		{
			if(xSCH_MATERIAL_CTRL->Check__MATERIAL_MORE_THAN_MOVE_COUNT(1) > 1)
			{
				pump_req = 1;
			}
		}

		if(pump_req > 0)
		{
			if(SCH__LBx_PUMP(p_variable,para_module, log_id) > 0)
			{
				int ll_i = Get__LLx_INDEX(para_module);
				if(ll_i >= 0)
				{
					xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Set__DATA("OUT");
				}
			}
		}
	}

	NEXT__LOOP;
}


// LBo -> BUFFERx
int CObj__DUAL_ARM_STD
::Fnc__LBo_To_RB__ONLY_MODE(CII_OBJECT__VARIABLE *p_variable, CII_OBJECT__ALARM *p_alarm)
{
	DECLARE__EXT_CTRL(p_variable);

	CString log_id = "Fnc__LBo_To_RB__ONLY_MODE()";

	// ...
	// bool active_log = true;
	bool active_log = false;

	if(active_log)		printf("Fnc__LBo_To_RB__ONLY_MODE) ... \n");

	// CHECK : RB -> LPx ...
	if(_Check__ARM_LIST_TO_PLACE_LPx() > 0)
	{
		NEXT__LOOP;
	}

	// ...
	bool active__arm_wafer__mapped  = false;
	bool active__arm_wafer__process = false;

	if(dCH__ATM_RB__CFG_PICK_WAFER_CONDITION->Check__DATA(STR__ONLY_PROCESSED) > 0)
	{
		if(ATM_RB__Check_Occupied__A_Arm() > 0)
		{
			if(xCH__ATM_RB__SLOT01_STATUS->Check__DATA(STR__MAPPED) > 0)			NEXT__LOOP;
			else																	active__arm_wafer__process = true;
		}
		if(ATM_RB__Check_Occupied__B_Arm() > 0)
		{
			if(xCH__ATM_RB__SLOT02_STATUS->Check__DATA(STR__MAPPED) > 0)			NEXT__LOOP;
			else																	active__arm_wafer__process = true;
		}
	}

	// ...
	int arm_count = 1;

	if(xCH_CFG__SCH_ARM_MODE->Check__DATA(STR__DUAL) > 0)
	{
		arm_count = ATM_RB__Get_Cfg_Arm_Count();
	}

	// ...
	CString para_module;

	// LLx ...
	{
		CString para_slot;
		int slot_id;

		//
		int active__only_output = -1;

		int ll_limit = iLLx_SIZE;
		int ll_i;

		for(ll_i=0; ll_i<ll_limit; ll_i++)
		{
			if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_i]->Check__DATA(LBx_MODE__ONLY_OUTPUT) < 0)
			{
				continue;
			}

			if(LLx__Is_Available(ll_i) < 0)			continue;
			if(LLx__Is_ATM(ll_i) < 0)				continue;

			if(xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Check__DATA(STR__OUT) < 0)
			{
				continue;
			}

			// ...
			int material_flag = -1;

			int i_limit = iLLx_SLOT_MAX[ll_i];
			for(int i=0; i<i_limit; i++)
			{
				if(LLx__Check_Occupied__OutSlot(ll_i, i) < 0)
				{
					continue;
				}

				para_module = Get__LLx_NAME(ll_i);
				para_slot.Format("%1d", i+1);

				material_flag = 1;
				break;
			}

			if(material_flag < 0)
			{
				continue;
			}

			active__only_output = 1;
			break;
		}

		if(active__only_output < 0)
		{
			if(active_log)		printf(" * Next_Loop (1) ... \n");
			NEXT__LOOP;
		}
	}

	// ...
	CStringArray l__ll_lot_id;
	CStringArray l__ll_slot_id;
	CUIntArray   l__st_check;

	// LLx ...
	{
		int ll_i = Get__LLx_INDEX(para_module);

		CUIntArray l_slot_id;
		LLx__Get_Occupied__Only_Output(ll_i, l_slot_id);

		int k_limit = l_slot_id.GetSize();
		int k;

		for(k=0; k<k_limit; k++)
		{
			int slot_id = l_slot_id[k];
			int stx_check = -1;

			//
			CString sch_name;
			CString sch_slot;

			sch_slot.Format("%1d", slot_id);
			sch_name.Format("%s-%s", para_module,sch_slot);
		
			// ...
			IDS__SCH_MATERIAL_INFO ds_info;
			xSCH_MATERIAL_CTRL->Get__MATERIAL_INFO(sch_name,ds_info);
				
			CString sch_lotid = ds_info.sLOT_ID;

			int wfr__port_id = ds_info.iSRC__PTN;
			int wfr__slot_id = ds_info.iSRC__STN;

			if(Fnc__LPx_SideBuffer_Use_Flag_Check(wfr__port_id, wfr__slot_id) > 0)
			{
				stx_check =  1;
			}

			if(stx_check > 0)
			{
				if((dCH__SCH_DB_ST1_OUT_MODE_BUFFER_TO_LPo->Check__DATA(STR__LPx_OPTIMIZATION) > 0)
				|| (dCH__SCH_DB_ST2_OUT_MODE_BUFFER_TO_LPo->Check__DATA(STR__LPx_OPTIMIZATION) > 0))
				{
					if(LPx__Check_Not_Processed_Wafer(wfr__port_id) > 0)
					{

					}
					else
					{
						if(xSCH_MATERIAL_CTRL->Check__MATERIAL_TO_PROCESS_IN_LPx(wfr__port_id) < 0)
						{
							stx_check = -1;
						}
					}
				}
			}

			// Check : LLx Wafer ...
			{
				int ll_slot_index = slot_id - 1;
				CString str__wfr_state = xEXT_CH__LLx__SLOT_STATUS[ll_i][ll_slot_index]->Get__STRING();

				if(active__arm_wafer__mapped)
				{
					if(str__wfr_state.CompareNoCase(STR__MAPPED) != 0)		continue;
				}
				if(active__arm_wafer__process)
				{
					if(str__wfr_state.CompareNoCase(STR__MAPPED) == 0)		continue;					
				}

				if(str__wfr_state.CompareNoCase(STR__MAPPED) == 0)			active__arm_wafer__mapped  = true;	
				else														active__arm_wafer__process = true;
			}

			l__ll_lot_id.Add(sch_lotid);
			l__ll_slot_id.Add(sch_slot);
			l__st_check.Add(stx_check);
		}
	}

	// LLo -> RB
	int t_limit = l__ll_lot_id.GetSize();
	int t;

	for(t=0; t<t_limit; t++)
	{
		// ...
		{
			if(xSCH_MATERIAL_CTRL->Check__SYSTEM_PAUSE() > 0)			NEXT__LOOP;
			if(xSCH_MATERIAL_CTRL->Get__MATERIAL_DB_SIZE() < 1)			NEXT__LOOP;
			if(xCH__ATM_RB_PAUSE_FLAG->Check__DATA(STR__YES) > 0)		NEXT__LOOP;
		}

		// ...
		CString para_slot = l__ll_slot_id[t];

		// ...
		CString bff_lotid;
		CString bff_module;
		int bff_slot_id;

		int stx_check = -1;

		// ...
		{
			CString para_lotid = "";

			if(dCH__CFG_BUFFER_LOTID_CHECK->Check__DATA(STR__ENABLE) > 0)
			{
				stx_check  = l__st_check[t];
				para_lotid = l__ll_lot_id[t];
			}
			else if(l__st_check.GetSize() > 0)
			{
				stx_check  = l__st_check[t];
				
				// para_lotid = l__ll_lot_id[0];
				para_lotid = _Get__STx_LotID();
			}

			if(stx_check > 0)
			{
				if(_Get__LPx_SideBuffer_Info_To_Use(para_lotid, bff_module,bff_slot_id) < 0)
				{
					if(active_log)		printf(" * Next_Loop (2) ... \n");
					NEXT__LOOP;
				}

				bff_lotid = para_lotid;
			}
		}

		// ...
		CString arm_type;

		if(ATM_RB__Get_Empty__Arm_Type(arm_type) > 0)
		{
			bool ex_flag = false;

			if(dEXT_CH__CFG_LLx_CTRL_ONLY_INPUT_OUTPUT_MODE->Check__DATA(STR__DUAL) > 0)
			{

			}
			else
			{
				if(SCH__CHECK_LBo_OCCUPIED() > 1)
				{
					ex_flag = true;
				}
			}

			// ...
			CString sch_module;
			sch_module.Format("%s-%s", para_module,para_slot);

			if(SCH__PICK_MODULE(p_variable,p_alarm, log_id, ex_flag, arm_type, para_module,para_slot,sch_module) < 0)
			{
				if(active_log)		printf(" * Next_Loop (3) ... \n");
				NEXT__LOOP;
			}

			xSCH_MATERIAL_CTRL->Pick__From_MODULE(sch_module, arm_type);

			// Check : Wafer (Mapped) ....
			{
				if(arm_type.CompareNoCase(ARM_A) == 0)
				{
					if(xCH__ATM_RB__SLOT01_STATUS->Check__DATA(STR__MAPPED) > 0)		stx_check = -1;
				}
				else if(arm_type.CompareNoCase(ARM_B) == 0)
				{
					if(xCH__ATM_RB__SLOT02_STATUS->Check__DATA(STR__MAPPED) > 0)		stx_check = -1;
				}

				if(stx_check < 0)
				{
					xSCH_MATERIAL_CTRL->Set__MATERIAL_STATUS(arm_type, STR__TO_LP);
				}
			}

			if(ex_flag == false)
			{
				if(SCH__LBx_PUMP(p_variable,para_module, log_id) > 0)
				{
					int ll_i = Get__LLx_INDEX(para_module);
					if(ll_i >= 0)
					{
						xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Set__DATA("IN");
					}
				}
			}
		}
		else
		{
			if(active_log)		printf(" * Next_Loop (5) ... \n");
			NEXT__LOOP;
		}
	}

	return 1;
}
int CObj__DUAL_ARM_STD
::_Check__LBo_To_RB__ONLY_MODE(CII_OBJECT__VARIABLE *p_variable, CII_OBJECT__ALARM *p_alarm)
{
	CString para_module;

	// LLx ...
	{
		CString para_slot;
		int slot_id;

		//
		int active__only_output = -1;

		int ll_limit = iLLx_SIZE;
		int ll_i;

		for(ll_i=0; ll_i<ll_limit; ll_i++)
		{
			if(xEXT_CH__SCH_DB_LLx_MODE_TYPE[ll_i]->Check__DATA(LBx_MODE__ONLY_OUTPUT) > 0)
			{
				if(LLx__Is_Available(ll_i) < 0)			continue;
				if(LLx__Is_ATM(ll_i) < 0)				continue;

				if(xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Check__DATA("OUT") < 0)
				{
					continue;
				}

				// ...
				int material_flag = -1;

				int i_limit = iLLx_SLOT_MAX[ll_i];

				for(int i=0; i<i_limit; i++)
				{
					if(LLx__Check_Occupied__OutSlot(ll_i, i) < 0)
					{
						continue;
					}

					para_module = Get__LLx_NAME(ll_i);
					para_slot.Format("%1d", i+1);

					material_flag = 1;
					break;
				}

				if(material_flag < 0)
				{
					continue;
				}

				active__only_output = 1;
				break;
			}
		}

		if(active__only_output < 0)
		{
			return -1;
		}
	}

	// ...
	CStringArray l__ll_lot_id;
	CStringArray l__ll_slot_id;
	CUIntArray   l__st_check;

	// LLx ...
	{
		int ll_i = Get__LLx_INDEX(para_module);

		CUIntArray l_slot_id;
		LLx__Get_Occupied__Only_Output(ll_i, l_slot_id);

		int k_limit = l_slot_id.GetSize();
		int k;

		for(k=0; k<k_limit; k++)
		{
			int slot_id = l_slot_id[k];
			int stx_check = -1;

			//
			CString sch_name;
			CString sch_slot;

			sch_slot.Format("%1d", slot_id);
			sch_name.Format("%s-%s", para_module,sch_slot);

			IDS__SCH_MATERIAL_INFO ds_info;
			xSCH_MATERIAL_CTRL->Get__MATERIAL_INFO(sch_name,ds_info);

			CString sch_lotid = ds_info.sLOT_ID;

			int wfr__port_id = ds_info.iSRC__PTN;
			int wfr__slot_id = ds_info.iSRC__STN;

			if(Fnc__LPx_SideBuffer_Use_Flag_Check(wfr__port_id, wfr__slot_id) > 0)
			{
				stx_check =  1;
			}

			if(stx_check > 0)
			{
				if((dCH__SCH_DB_ST1_OUT_MODE_BUFFER_TO_LPo->Check__DATA(STR__LPx_OPTIMIZATION) > 0)
				|| (dCH__SCH_DB_ST2_OUT_MODE_BUFFER_TO_LPo->Check__DATA(STR__LPx_OPTIMIZATION) > 0))
				{
					if(xSCH_MATERIAL_CTRL->Check__MATERIAL_TO_PROCESS_IN_LPx(wfr__port_id) < 0)
					{
						stx_check = -1;
					}
				}
			}

			l__ll_lot_id.Add(sch_lotid);
			l__ll_slot_id.Add(sch_slot);
			l__st_check.Add(stx_check);
		}
	}

	// LLo -> RB
	int t_limit = l__ll_lot_id.GetSize();
	int t;

	for(t=0; t<t_limit; t++)
	{
		CString para_slot = l__ll_slot_id[t];

		// ...
		CString bff_lotid;
		CString bff_module;
		int bff_slot_id;

		int stx_check = -1;

		// ...
		{
			CString para_lotid = "";

			if(dCH__CFG_BUFFER_LOTID_CHECK->Check__DATA(STR__ENABLE) > 0)
			{
				stx_check  = l__st_check[t];
				para_lotid = l__ll_lot_id[t];
			}
			else if(l__st_check.GetSize() > 0)
			{
				stx_check  = l__st_check[0];
				para_lotid = _Get__STx_LotID();
			}

			if(stx_check > 0)
			{
				if(_Get__LPx_SideBuffer_Info_To_Use(para_lotid, bff_module,bff_slot_id) < 0)
				{
					return -1;
				}

				bff_lotid = para_lotid;
			}
		}
	}

	return 1;
}

int CObj__DUAL_ARM_STD::
Fnc__LBo_To_RB__ALL_MODE(CII_OBJECT__VARIABLE *p_variable, CII_OBJECT__ALARM *p_alarm)
{
	DECLARE__EXT_CTRL(p_variable);

	CString log_id = "Fnc__LBo_To_RB__ALL_MODE()";

	// ...
	if(ATM_RB__Check_Empty__Arm_Type() < 0)
	{
		NEXT__LOOP;
	}

	// CHECK : RB -> LPx 
	if(_Check__ARM_LIST_TO_PLACE_LPx() > 0)
	{
		NEXT__LOOP;
	}

	if(AL1__Check_Empty__All_Slot() < 0)
	{
		int lbx_check = -1;
		
		int ll_limit = iLLx_SIZE;
		int ll_i;

		for(ll_i=0; ll_i<ll_limit; ll_i++)
		{
			if(LLx__Is_Available(ll_i) > 0)
			{
				if((LLx__Get_Occupied__Total_OutSlot(ll_i) > 0)
				&& (LLx__Get_Occupied__Total_InSlot(ll_i)  > 0))
				{
					lbx_check = 1;
				}
			}
		}
		
		if(lbx_check < 0)
		{
			NEXT__LOOP;
		}
	}

	// ...
	CString para_module;
	CString para__in_empty;
	CString para__out_slot;
	int slot_id;

	// LLx ...
	{
		int out_flag = -1;

		int ll_limit = iLLx_SIZE;
		int ll_i;

		for(ll_i=0; ll_i<ll_limit; ll_i++)
		{
			if(xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Check__DATA(STR__IN) < 0)
			{
				continue;
			}

			if(LLx__Is_ATM(ll_i) < 0)				continue;
			if(LLx__Is_Available(ll_i) < 0)			continue;

			if(LLx__Get_Occupied__OutSlot(ll_i, slot_id) > 0)
			{
				int ll__in_slot_id;
				if(LLx__Get_Empty__InSlot(ll_i, ll__in_slot_id) > 0)
				{
					para__in_empty.Format("%1d", ll__in_slot_id);
				}

				para_module = Get__LLx_NAME(ll_i);
				para__out_slot.Format("%1d", slot_id);

				out_flag = 1;
				break;
			}
		}

		if(out_flag < 0)
		{
			NEXT__LOOP;
		}
	}

	if(para__in_empty.GetLength() > 0)
	{
		if(xSCH_MATERIAL_CTRL->Check__MATERIAL_TO_PROCESS() > 0)
		{
			CString sch_name;
			sch_name.Format("%s-%s", para_module,para__out_slot);

			IDS__SCH_MATERIAL_INFO ds_info;
			xSCH_MATERIAL_CTRL->Get__MATERIAL_INFO(sch_name, ds_info);

			int ll__port_id = ds_info.iSRC__PTN;

			int r_flag = xSCH_MATERIAL_CTRL->Check__MATERIAL_TO_PROCESS_IN_LPx(ll__port_id);
			if(r_flag > 0)
			{
				NEXT__LOOP;
			}
		}
	}

	// ...
	int buffer_check_flag = -1;

	CString bff_module;
	int bff_slot_id;

	// ...
	{
		CString sch_name;
		sch_name.Format("%s-%s", para_module,para__out_slot);

		int r_flag = Get__LPx_SideBuffer_Info_To_Use(sch_name, bff_module,bff_slot_id);
		
			 if(r_flag >  0)			buffer_check_flag =  1;
		else if(r_flag == 0)			buffer_check_flag = -1;
		else if(r_flag <  0)			NEXT__LOOP;
	}

	// ...
	CString arm_type;
	
	if(ATM_RB__Get_Empty__Arm_Type(arm_type) < 0)
	{
		NEXT__LOOP;
	}

	// ...
	{
		CString sch_module;
		sch_module.Format("%s-%s", para_module,para__out_slot);

		if(SCH__PICK_MODULE(p_variable,p_alarm, log_id, false, arm_type, para_module,para__out_slot, sch_module) < 0)
		{
			NEXT__LOOP;
		}

		xSCH_MATERIAL_CTRL->Pick__From_MODULE(sch_module, arm_type);

		if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(arm_type) < 0)
		{
			CString pmc_name;
			CString pmc_rcp;

			if(xSCH_MATERIAL_CTRL->Get__Last_Processing_Info(arm_type,pmc_name,pmc_rcp) > 0)
			{
				xSCH_MATERIAL_CTRL->Set__MATERIAL_STATUS(arm_type, STR__PROCESSED);
			}
			else
			{
				xSCH_MATERIAL_CTRL->Set__MATERIAL_STATUS(arm_type, STR__TO_LP);
			}
		}
	}

	// LLx : InSlot Check ...
	{
		int ll_i = Get__LLx_INDEX(para_module);
		if(ll_i >= 0)
		{
			if(LLx__Is_Available(ll_i) > 0)
			{
				if((LLx__Get_Occupied__Total_OutSlot(ll_i) < 1)
				&& (LLx__Get_Occupied__Total_InSlot(ll_i)  > 0))
				{
					CString lb_sch_name;
					int lb_in_slot = -1;
					
					LLx__Get_Occupied__InSlot(ll_i, lb_in_slot);
					lb_sch_name.Format("%s-%1d", para_module,lb_in_slot);
					
					if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(lb_sch_name) > 0)
					{
						if(SCH_RUN__LLx_PUMP(ll_i, p_variable, log_id, "101") > 0)
						{
							xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Set__DATA(STR__OUT);
						}
					}
				}
				else
				{
					int count__ll_out = 0;

					for(int k=0; k<iLLx_SIZE; k++)
					{
						if(xEXT_CH__LLx__CFG_USE_FLAG[k]->Check__DATA(STR__ENABLE) < 0)		continue;
						if(xEXT_CH__LLx__IN_OUT_FLAG[k]->Check__DATA(STR__OUT) < 0)			continue;

						count__ll_out++;
					}

					if((ATM_RB__Check_Process_Wafer_On_Arm() < 0)
					&& (AL1__Check_Empty__All_Slot() > 0)
					&& (xSCH_MATERIAL_CTRL->Check__MATERIAL_TO_PROCESS() < 0)
					&& (count__ll_out < 1))
					{
						int count__vac_area = 0;
						CString ch_data;

						ch_data = xEXT_CH__VAC_ROBOT_OUT_MATERIAL_COUNT->Get__STRING();
						count__vac_area += atoi(ch_data);

						ch_data = xEXT_CH__PMC_MATERIAL_COUNT->Get__STRING();
						count__vac_area += atoi(ch_data);

						if(count__vac_area > 0)
						{
							if(SCH_RUN__LLx_PUMP(ll_i, p_variable, log_id, "102") > 0)
							{
								xEXT_CH__LLx__IN_OUT_FLAG[ll_i]->Set__DATA(STR__OUT);
							}
						}
					}
				}
			}

			// ...
		}
	}

	return 1;
}

int  CObj__DUAL_ARM_STD::
Fnc__LPx_SideBuffer_Use_Flag_Check(const int port_id,
								   const int slot_id)
{
	// bool active_log = true;
	bool active_log = false;
	
	if(active_log)		printf("Fnc__LPx_SideBuffer_Use_Flag_Check() ... \n");
	if(active_log)		printf(" * (portid, slotid) <- (%1d, %1d) \n", port_id,slot_id);

	// ...
	int db_index = port_id - 1;
	
	if((db_index >= 0) && (db_index < iLPx_SIZE))
	{
		int check_flag = -1;
		
		if(xCH__SCH_DB_STx_APPLY_MODE->Check__DATA(STR__ENABLE) > 0)
		{
			check_flag = 1;			

			if(active_log)		printf(" * STx_Apply_Mode <- (%s) \n", STR__ENABLE);
		}
		else if(xCH__SCH_DB_STx_APPLY_MODE->Check__DATA(STR__ROUTE) > 0)
		{
			if(active_log)		printf(" * STx_Apply_Mode <- (%s) \n", STR__ROUTE);

			if(xSCH_MATERIAL_CTRL->Check__EDIT_TYPE__STATE() > 0)
			{
				CStringArray l_id;
				CStringArray l_mode;
				CStringArray l_slot; 
				CStringArray l_sec;

				if(xSCH_MATERIAL_CTRL->Get__STx_OF_EDIT_TYPE(port_id,slot_id, l_id,l_mode,l_slot,l_sec) > 0)
				{
					if(l_id.GetSize() > 0)
					{
						check_flag = 1;
					}
				}
			}
			else
			{
				CString bff1_info;
				CString bff2_info;

				if(xSCH_MATERIAL_CTRL->Get__BUFFERx_INFO(port_id,slot_id, bff1_info,bff2_info) > 0)
				{
					if((bff1_info.CompareNoCase(STR__YES) == 0)
					|| (bff2_info.CompareNoCase(STR__YES) == 0))
					{
						check_flag = 1;
					}
				}

				if(active_log)		printf(" * bff1_info <- (%s) \n", bff1_info);
				if(active_log)		printf(" * bff2_info <- (%s) \n", bff2_info);
			}
		}
		
		if(check_flag > 0)
		{
			xCH__LPx__SIDE_BUFFER_USE_FLAG[db_index]->Set__DATA(STR__YES);
			return 1;
		}
		else
		{
			xCH__LPx__SIDE_BUFFER_USE_FLAG[db_index]->Set__DATA("");
			return -1;
		}
	}

	return -1;
}
int  CObj__DUAL_ARM_STD::
Fnc__LPx_SideBuffer_Use_Flag_Clear()
{
	int i;

	for(i=0; i<iLPx_SIZE; i++)
	{
		xCH__LPx__SIDE_BUFFER_USE_FLAG[i]->Set__DATA("");
	}
	return 1;
}

int  CObj__DUAL_ARM_STD::
Get__LPx_SideBuffer_Info_To_Use(const int port_id,
								const int slot_id)
{
	CString buffer_name;
	int buffer_slot;

	return Get__LPx_SideBuffer_Info_To_Use(port_id,slot_id, buffer_name,buffer_slot);
}
int  CObj__DUAL_ARM_STD::
Get__LPx_SideBuffer_Info_To_Use(const int port_id,
								const int slot_id,
								CString& buffer_name,
								int& buffer_slot)
{		
	CString sch_lotid;

	// ...
	{
		IDS__SCH_MATERIAL_INFO ds_info;
	
		if(xSCH_MATERIAL_CTRL->Get__MATERIAL_INFO(port_id,slot_id, ds_info) > 0)
		{
			sch_lotid = ds_info.sLOT_ID;				
		}
		else
		{
			return -1;
		}

		if(Fnc__LPx_SideBuffer_Use_Flag_Check(port_id,slot_id) < 0)
		{
			return 0;
		}
	}

	return _Get__LPx_SideBuffer_Info_To_Use(sch_lotid, buffer_name,buffer_slot);
}
int  CObj__DUAL_ARM_STD::
Get__LPx_SideBuffer_Info_To_Use(const CString& pos_name)
{
	CString buffer_name;
	int buffer_slot;

	return Get__LPx_SideBuffer_Info_To_Use(pos_name, buffer_name,buffer_slot);
}
int  CObj__DUAL_ARM_STD::
Get__LPx_SideBuffer_Info_To_Use(const CString& pos_name,
								CString& buffer_name,
								int& buffer_slot)
{
	CString sch_lotid;
	
	// ...
	{
		IDS__SCH_MATERIAL_INFO ds_info;
		int port_id;
		int slot_id;
		
		if(xSCH_MATERIAL_CTRL->Get__MATERIAL_INFO(pos_name, ds_info) > 0)
		{
			sch_lotid = ds_info.sLOT_ID;				

			port_id = ds_info.iSRC__PTN;
			slot_id = ds_info.iSRC__STN;
		}
		else
		{
			return -1;
		}

		if(Fnc__LPx_SideBuffer_Use_Flag_Check(port_id,slot_id) < 0)
		{
			return 0;
		}
	}

	if(dCH__CFG_BUFFER_LOTID_CHECK->Check__DATA(STR__ENABLE) > 0)
	{

	}
	else
	{
		sch_lotid = _Get__STx_LotID();
	}

	return _Get__LPx_SideBuffer_Info_To_Use(sch_lotid, buffer_name,buffer_slot);
}
int  CObj__DUAL_ARM_STD::
_Get__LPx_SideBuffer_Info_To_Use(const CString& sch_lotid,
								 CString& buffer_name,
								 int& buffer_slot)
{
	CString bff1_lotid = Buffer1__Get_LotID();
	CString bff2_lotid = Buffer2__Get_LotID();

	// LotID Check ...
	{
		if(sCH__SCH_DB_ST1_USE_FLAG->Check__DATA(STR__ENABLE) > 0)
		{
			if((bff1_lotid.GetLength() > 0)
			&& (sch_lotid.CompareNoCase(bff1_lotid) == 0))
			{
				if(Buffer1__Get_Empty__Slot_To_Process(buffer_slot) > 0)
				{
					buffer_name = MODULE__BUFFER1;
					return 11;
				}
			}
		}

		if(sCH__SCH_DB_ST2_USE_FLAG->Check__DATA(STR__ENABLE) > 0)
		{
			if((bff2_lotid.GetLength() > 0)
			&& (sch_lotid.CompareNoCase(bff2_lotid) == 0))
			{
				if(Buffer2__Get_Empty__Slot_To_Process(buffer_slot) > 0)
				{
					buffer_name = MODULE__BUFFER2;
					return 12;
				}
			}
		}
	}

	// Full Empty Side_Storage Check ...
	{
		if(sCH__SCH_DB_ST1_USE_FLAG->Check__DATA(STR__ENABLE) > 0)
		{
			if(bff1_lotid == "")
			{
				if(Buffer1__Get_Empty__Slot_To_Process(buffer_slot) > 0)
				{
					buffer_name = MODULE__BUFFER1;
					return 21;
				}
			}
		}

		if(sCH__SCH_DB_ST2_USE_FLAG->Check__DATA(STR__ENABLE) > 0)
		{
			if(bff2_lotid == "")
			{
				if(Buffer2__Get_Empty__Slot_To_Process(buffer_slot) > 0)
				{
					buffer_name = MODULE__BUFFER2;
					return 22;
				}
			}
		}
	}
				
	return -11;
}
CString CObj__DUAL_ARM_STD::_Get__STx_LotID()
{
	CString bff1_lotid = Buffer1__Get_LotID();
	CString bff2_lotid = Buffer2__Get_LotID();

	if(sCH__SCH_DB_ST1_USE_FLAG->Check__DATA(STR__ENABLE) > 0)
	{
		if(bff1_lotid.GetLength() > 0)		return bff1_lotid;
	}

	if(sCH__SCH_DB_ST2_USE_FLAG->Check__DATA(STR__ENABLE) > 0)
	{
		if(bff2_lotid.GetLength() > 0)		return bff2_lotid;
	}

	return "";
}
