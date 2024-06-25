#include "StdAfx.h"
#include "CObj_Opr__AUTO_MODE.h"

#include "Macro_Function.h"


#include "CMacro_FA.h"
extern CMacro_FA mFA_Link;


// ...
void CObj_Opr__AUTO_MODE::
Mon__PORT_CTRL(CII_OBJECT__VARIABLE *p_variable,CII_OBJECT__ALARM *p_alarm)
{
	SCX__ASYNC_TIMER_CTRL x_async_timer_ctrl;
	
	x_async_timer_ctrl->REGISTER__COUNT_CHANNEL_NAME(xEXT_CH__DUMMY_CLEAN_SYSTEM_IDLE_TIME_COUNT->Get__CHANNEL_NAME());
	x_async_timer_ctrl->START__COUNT_UP(99999.9);

	// ...
	xCH__JOB_CTRL->Set__DATA(STR__AVAILABLE);
	xCH_PRE__JOB_CTRL->Set__DATA(STR__AVAILABLE);

	xCH__JOB_STATUS->Set__DATA(STR__IDLE);
	xCH_PRE__JOB_STATUS->Set__DATA(STR__IDLE);

	// ...
	int idle_flag = 1;

	xSCH_MATERIAL_CTRL->Register__SYSTEM_NAME(sObject_Name);
	xSCH_MATERIAL_CTRL->Init__MATERIAL_PICK_COUNT();
	xSCH_MATERIAL_CTRL->Init__MATERIAL_PLACE_COUNT();
	// xSCH_MATERIAL_CTRL->Set__SYSTEM_PAUSE();
	xSCH_MATERIAL_CTRL->Set__SYSTEM_START();


	iPICK_COUNT  = 0;
	iPLACE_COUNT = 0;

	// ...
	int sec_flag = -1;
	int time_count = 0;
	int maint_count = 0;


	while(1)
	{
		while(iMAINT_FLAG > 0)	
		{
			p_variable->Wait__SINGLE_OBJECT(0.1);
			maint_count++;

				 if(maint_count == 1)			Update__CFG_DB_TO_SCH_DB();
			else if(maint_count >= 10)			maint_count = 0;

			Seq__CLEAR_ALL_PORT(p_alarm);		
		}

		maint_count = 0;
		p_variable->Wait__SINGLE_OBJECT(0.01);

		// ...
		{
			if(xCH__JOB_STATUS->Check__DATA(STR__IDLE) < 0)
			{
				xCH__LEAK_CHECK_START_FLAG->Set__DATA("");
				xEXT_CH__LEAK_CHECK__IDLE_CUR_TIME->Set__DATA("0");
			}

			Fnc__LEAK_CHECK(p_variable,p_alarm);
			Fnc__AUTO_PM(p_variable,p_alarm);
		}

		time_count++;

		if(time_count >= 50)
		{
			time_count = 0;
			sec_flag = 1;

			if(idle_flag > 0)
			{
				// CFG_DB -> SCH_DB ...
				Update__CFG_DB_TO_SCH_DB();
			}
		}
		else
		{
			sec_flag = -1;
		}

		// ...
		try
		{
			Seq__JOB_CTRL();
		}
		catch(int err_flag)
		{

		}

		try
		{
			Seq__JOB_END(p_alarm);
		}
		catch(int err_flag)
		{

		}

		try
		{
			if(sec_flag > 0)
			{
				Seq__UPDATE_PORT_INFO();
			}
		}
		catch(int err_flag)
		{

		}

		try
		{
			if(sec_flag > 0)
			{
				CString sch_mode = xCH__CFG_SCH_MODE->Get__STRING();

				if(sch_mode.CompareNoCase("SEQUENTIAL") == 0)
				{
					xSCH_MATERIAL_CTRL->Set__SCH_MODE__SEQUENTIAL();
				}
				else if(sch_mode.CompareNoCase("PIPELINE") == 0)
				{	
					xSCH_MATERIAL_CTRL->Set__SCH_MODE__PIPELINE();
				}
				else 
				{
					xSCH_MATERIAL_CTRL->Set__SCH_MODE__CONCURRENT();				
				}

				Seq__UPLOAD_JOB(p_alarm);
				Seq__UPLOAD_RESERVE_PORT(p_alarm);
			}
		}
		catch(int err_flag)
		{

		}

		try
		{
			Fnc__CYCLE_COUNT();
			Seq__CHECK_PORT_COMPLETE(p_alarm);
		}
		catch(int err_flag)
		{

		}

		if(xSCH_MATERIAL_CTRL->Get__MATERIAL_DB_SIZE() < 1)
		{
			if(Check__ALL_PORT_COMPLETE() > 0)
			{
				if(idle_flag < 0)
				{
					Clear__PMx_JOB_START_READY();
					SCH__PMx_JOB_END_READY();

					x_async_timer_ctrl->START__COUNT_UP(99999.9);

					// ...
					int time_count = 0;

					while(time_count < 50)
					{
						if(xSCH_MATERIAL_CTRL->Check__ROBOT_IDLE() > 0)
						{
							time_count = 100;
						}
						else
						{
							Sleep(100);
							time_count++;
						}
					}

					// ...
					xCH__JOB_STATUS->Set__DATA(STR__IDLE);
					xCH__JOB_CTRL->Set__DATA(STR__AVAILABLE);

					xSCH_MATERIAL_CTRL->Clear__SYSTEM_STATUS__PROCESS(sObject_Name);
					xSCH_MATERIAL_CTRL->Set__SYSTEM_PAUSE();
					
					Fnc__CYCLE_END(p_alarm);

					// SCH.MODE.XXX ...
					sEXT_CH__SCH_MODE_LLx_IN->Set__DATA("");
					sEXT_CH__SCH_MODE_LLx_OUT->Set__DATA("");
				}

				idle_flag = 1;
			}
		}
		else
		{
			if(idle_flag > 0)
			{
				if(xCH__JOB_STATUS->Check__DATA(STR__IDLE) > 0)
				{
					if(Fnc__CYCLE_START(p_alarm) > 0)
					{
						x_async_timer_ctrl->START__COUNT_DOWN(0.0);

						//.....
						{
							iPICK_COUNT  = 0;
							iPLACE_COUNT = 0;

							xCH_CFG__CYCLE_COUNT_PICK->Set__DATA("0");
							xCH_CFG__CYCLE_COUNT_PLACE->Set__DATA("0");

							xSCH_MATERIAL_CTRL->Set__SYSTEM_STATUS__PROCESS(sObject_Name);
							xSCH_MATERIAL_CTRL->Init__MATERIAL_PICK_COUNT();
							xSCH_MATERIAL_CTRL->Init__MATERIAL_PLACE_COUNT();
							xSCH_MATERIAL_CTRL->Set__SYSTEM_START();
						}
					}
				}	

				// ...
				{
					xCH__JOB_STATUS->Set__DATA(STR__PROCESS);
					xCH__JOB_CTRL->Set__DATA(STR__RUN);
					xSCH_MATERIAL_CTRL->Set__SYSTEM_START();
				}
			}

			idle_flag = -1;
		}

		// ...
	}
}

int  CObj_Opr__AUTO_MODE::
Fnc__GET_DUMMY_PORT()
{
	int i;

	for(i=0; i<iLPx_UNIT_SIZE; i++)
	{
		if(xCH__PORT_CFG_USE_TYEP[i]->Check__DATA(STR__DUMMY) < 0)
		{
			continue;
		}
		return (i + 1);
	}
	return -1;
}

int  CObj_Opr__AUTO_MODE::
Fnc__GET_DUMMY_SLOT(const int port_id,int& slot_id)
{
	int db_index = port_id - 1;

	if(db_index <  0)					return -1;
	if(db_index >= iLPx_UNIT_SIZE)		return -1;

	// ...
	CString var_data;
	int slot_max;
	int i;

	xCH__PORT_CFG_SLOT_MAX[db_index]->Get__DATA(var_data);
	slot_max = atoi(var_data);
	
	if(slot_max > CFG_LP__SLOT_MAX)
	{
		slot_max = CFG_LP__SLOT_MAX;
	}

	// ...
	xCH__PORT_LAST_RESERVE_SLOT[db_index]->Get__DATA(var_data);
	int s_i = atoi(var_data);

	for(i=s_i; i<slot_max; i++)
	{
		if(xCH_CFG__DUMMY_PORT_SLOT_USE[i]->Check__DATA(STR__DISABLE) > 0)
		{
			continue;
		}

		// ...
		var_data = xCH__PORT_SLOT_STS[db_index][i]->Get__STRING();

		if(var_data.CompareNoCase(STR__EXIST)  != 0)
		{
			continue;
		}

		xCH__PORT_SLOT_STS[db_index][i]->Set__DATA(STR__MAPPED);
		
		slot_id = i + 1;
		return 1;
	}

	for(i=0; i<slot_max; i++)
	{
		if(xCH_CFG__DUMMY_PORT_SLOT_USE[i]->Check__DATA(STR__DISABLE) > 0)
		{
			continue;
		}

		// ...
		var_data = xCH__PORT_SLOT_STS[db_index][i]->Get__STRING();

		if(var_data.CompareNoCase(STR__EXIST)  != 0)
		{
			continue;
		}

		xCH__PORT_SLOT_STS[db_index][i]->Set__DATA(STR__MAPPED);

		slot_id = i + 1;
		return 1;
	}

	return -1;
}
int  CObj_Opr__AUTO_MODE::
Fnc__SET_DUMMY_SLOT_READY(const int port_id)
{
	int db_index = port_id - 1;

	if(db_index <  0)					return -1;
	if(db_index >= iLPx_UNIT_SIZE)		return -1;

	// ...
	CString var_data;
	int slot_max;
	int i;

	xCH__PORT_CFG_SLOT_MAX[db_index]->Get__DATA(var_data);
	slot_max = atoi(var_data);

	if(slot_max > CFG_LP__SLOT_MAX)
	{
		slot_max = CFG_LP__SLOT_MAX;
	}

	for(i=0;i<slot_max;i++)
	{
		xCH__PORT_SLOT_STS[db_index][i]->Get__DATA(var_data);

		if((var_data.CompareNoCase(STR__NONE)   == 0)
		|| (var_data.CompareNoCase(STR__MAPPED) == 0))
		{
			continue;
		}

		xCH__PORT_SLOT_STS[db_index][i]->Set__DATA(STR__EXIST);
	}
	return 1;
}

int  CObj_Opr__AUTO_MODE
::Fnc__GET_PORT_SLOTMAPLIST(const int port_id,CString& str_maplist)
{
	str_maplist = "";

	// ...
	int db_index = port_id - 1;

	if(db_index <  0)					return -1;
	if(db_index >= iLPx_UNIT_SIZE)		return -1;

	// ...
	CString var_data;
	int slot_max;
	int i;

	xCH__PORT_CFG_SLOT_MAX[db_index]->Get__DATA(var_data);
	slot_max = atoi(var_data);

	if(slot_max > CFG_LP__SLOT_MAX)
	{
		slot_max = CFG_LP__SLOT_MAX;
	}

	for(i=0; i<slot_max; i++)
	{
		xCH__PORT_SLOT_STS[db_index][i]->Get__DATA(var_data);

		if(var_data.CompareNoCase(STR__NONE) == 0)			str_maplist += "0";
		else												str_maplist += "1";
	}
	return 1;
}
int  CObj_Opr__AUTO_MODE
::Fnc__GET_PORT_SLOTJOBLIST(const int port_id,CString& str_joblist)
{
	str_joblist = "";

	// ...
	int db_index = port_id - 1;

	if(db_index <  0)					return -1;
	if(db_index >= iLPx_UNIT_SIZE)		return -1;

	// ...
	CString var_data;
	int slot_max;
	int i;

	xCH__PORT_CFG_SLOT_MAX[db_index]->Get__DATA(var_data);
	slot_max = atoi(var_data);

	if(slot_max > CFG_LP__SLOT_MAX)
	{
		slot_max = CFG_LP__SLOT_MAX;
	}

	for(i=0; i<slot_max; i++)
	{
		xCH__PORT_SLOT_STS[db_index][i]->Get__DATA(var_data);

		     if(var_data.CompareNoCase(STR__NONE)   == 0)		str_joblist += "0";
		else if(var_data.CompareNoCase(STR__MAPPED) == 0)		str_joblist += "2";
		else													str_joblist += "1";
	}
	return 1;
}


int  CObj_Opr__AUTO_MODE::
Fnc__DUMMY_PROC_JOB(CII_OBJECT__ALARM* p_alarm,
					const int src_ptn,
					const CStringArray& l_pmc)
{
	int dummy_portid = Fnc__GET_DUMMY_PORT();

	if(dummy_portid < 0)
	{
		int alarm_id = ALID__JOB_START__DUMMY_PORT__NOT_EXIST;
		CString r_act;

		p_alarm->Check__ALARM(alarm_id,r_act);
		p_alarm->Post__ALARM(alarm_id);
		return 1;
	}
	Fnc__SET_DUMMY_SLOT_READY(dummy_portid);

	// ...
	CStringArray l_src_slot;
	xSCH_MATERIAL_CTRL->Get__LPx_SLOT_LIST(src_ptn, l_src_slot);

	if(l_src_slot.GetSize() < 1)			
	{
		return -1;
	}
	int src_slot = atoi(l_src_slot[0]);

	// ...
	int wafer_count = 0;

	CString var_data;
	int i;

	dCH_CFG__DUMMY_PORT_WAFER_PER_PMC->Get__DATA(var_data);
	int pm_wafer_count = atoi(var_data);

	while(pm_wafer_count > 0)
	{
		pm_wafer_count--;

		for(i=0; i<iPMx_UNIT_SIZE; i++)
		{
			CString pmc_name;
			pmc_name.Format("PM%1d", i+1);

			if(Macro__Check_Item(pmc_name,l_pmc) < 0)
			{
				continue;
			}

			// OBJ STATUS -----
			xEXT_CH__PMx_OBJECT_STATUS[i]->Get__DATA(var_data);

			if((var_data.CompareNoCase(STR__CTCINUSE) != 0)
			&& (var_data.CompareNoCase(STR__STANDBY)  != 0))
			{
				continue;
			}

			// CHECK : DUMMY PROC RECIPE -----
			CString rcp_main = xEXT_CH__PMx_CFG_DUMMY_PROC_RCP[i]->Get__STRING();
			CString rcp_pre  = xEXT_CH__PMx_CFG_DUMMY_PRE_RCP[i]->Get__STRING();
			CString rcp_post = xEXT_CH__PMx_CFG_DUMMY_CLN_RCP[i]->Get__STRING();

			if(rcp_main.GetLength() < 1)
			{
				continue;
			}

			// ...
			{
				IDS__SCH_DB_INFO db_info;
				int dummy_slotid;

				if(Fnc__GET_DUMMY_SLOT(dummy_portid,dummy_slotid) < 0)
				{
					int alarm_id = ALID__JOB_START__DUMMY_MATERIAL__NOT_EXIST;
					CString alarm_msg;
					CString r_act;

					alarm_msg.Format("Dummy Port : LP%1d\n",dummy_portid);
				
					p_alarm->Check__ALARM(alarm_id,r_act);
					p_alarm->Post__ALARM_With_MESSAGE(alarm_id,alarm_msg);
					continue;
				}
				
				// ...
				{
					CString para__job_name      = "DUMMY_PROCESS";
					CString para__lotid         = "DUMMY_PROCESS";
					CString para__material_id   = "";
					CString para__ppid          = "DUMMY_PPID";
					CString para__material_type = "D";

					int db_index = wafer_count++;	

					xSCH_MATERIAL_CTRL->Copy__SCH_INFO_At_SCH_DB(db_index, 
						                                         src_ptn,src_slot, 
																 dummy_portid,dummy_slotid, 
																 true,true,true);

					//
					CStringArray l_pre_module;
					CStringArray l_pre_recipe;
					CStringArray l_main_module;
					CStringArray l_main_recipe;
					CStringArray l_post_module;
					CStringArray l_post_recipe;

					l_main_module.Add(pmc_name);
					l_main_recipe.Add(rcp_main);

					if(rcp_pre.GetLength() > 0)		
					{
						l_pre_module.Add(pmc_name);
						l_pre_recipe.Add(rcp_pre);
					}
					if(rcp_post.GetLength() > 0)		
					{
						l_post_module.Add(pmc_name);
						l_post_recipe.Add(rcp_post);
					}

					xSCH_MATERIAL_CTRL->Change__PROC_INFO_OF_SCH_DB(dummy_portid,dummy_slotid, 
																	para__job_name,
																	para__lotid,
																	para__material_id,
																	para__ppid,
																	para__material_type,
																	l_pre_module,
																	l_pre_recipe,
																	l_main_module,
																	l_main_recipe,
																	l_post_module,
																	l_post_recipe);
				}
			}
		}
	}

	if(wafer_count > 0)
	{
		// ...
		{
			printf("===================================================\n");
			printf(" * CObj_Opr__AUTO_MODE::Fnc__DUMMY_PROC_JOB() ... \n");
			printf(" ** port_id [%1d] - Pause !!! \n", src_ptn);
			printf("===================================================\n\n");

			xSCH_MATERIAL_CTRL->Pause__PORT_ID(src_ptn);
		}

		// ...
		int db_index = dummy_portid - 1;

		if((db_index >= 0) && (db_index < CFG_LP_LIMIT))
		{
			CString str_cid = xCH__CSTID[db_index]->Get__STRING();
			xSCH_MATERIAL_CTRL->Set__LPx_CID(dummy_portid, str_cid);

			xSCH_MATERIAL_CTRL->Set__JOB_PORT(dummy_portid);
			xSCH_MATERIAL_CTRL->JOB_CTRL__GoTo_JOB_PORT(dummy_portid);

			// ...
			{
				xCH__PORT_LAST_RESERVE_SLOT[db_index]->Get__DATA(var_data);

				if(wafer_count > 0)
				{
					int cur_slot = atoi(var_data);

					var_data.Format("%1d", (cur_slot + wafer_count));
					xCH__PORT_LAST_RESERVE_SLOT[db_index]->Set__DATA(var_data);
				}
			}

			// ...
			{
				xCH__PORT_LAMP_STATUS[db_index]->Set__DATA("PROCESS");
				xCH__PORT_STATUS[db_index]->Set__DATA(STR__BUSY);		
			}

			// ...
			{
				CString str_jobid = "DUMMY_JOB";

				xCH__PORT_END_REQ_FLAG[db_index]->Set__DATA("");
				xCH__PORT_JOB_POPUP_TYPE[db_index]->Set__DATA("");

				xCH__PORT_JOB_NAME[db_index]->Set__DATA(str_jobid);
				xCH__PORT_CTRL[db_index]->Set__DATA("RUN");

				xCH__PORT_MANUAL_CTRL_RUNNING_FLAG[db_index]->Set__DATA(STR__YES);
			}
		}
	}	

	return 1;
}

//-------------------------------------------------------------------------
void CObj_Opr__AUTO_MODE::Seq__JOB_CTRL()
{
	CString var_data;
	xCH__JOB_CTRL->Get__DATA(var_data);

	if(xCH_PRE__JOB_CTRL->Check__DATA(var_data) > 0)
	{
		return;
	}
	
	if(xCH__JOB_CTRL->Check__DATA(STR__RUN) > 0)
	{
		xSCH_MATERIAL_CTRL->Set__SYSTEM_START();
	}
	else if(xCH__JOB_CTRL->Check__DATA(STR__PAUSE) > 0)
	{
		xSCH_MATERIAL_CTRL->Set__SYSTEM_PAUSE();
	}
	else if(xCH__JOB_CTRL->Check__DATA(STR__STOP) > 0)
	{
		xSCH_MATERIAL_CTRL->Clear__ALL_MATERIAL_INFO();		
		xSCH_MATERIAL_CTRL->Set__SYSTEM_STOP();

		Seq__ABORT_ALL_PORT();
		mFA_Link.CJOB__ALL_ABORT();

		xCH__JOB_STATUS->Set__DATA(STR__IDLE);
		xCH__JOB_CTRL->Set__DATA(STR__AVAILABLE);
	}
	else if(xCH__JOB_CTRL->Check__DATA(STR__ABORT) > 0)
	{
		xSCH_MATERIAL_CTRL->Clear__ALL_MATERIAL_INFO();		
		xSCH_MATERIAL_CTRL->Set__SYSTEM_ABORT();

		Seq__ABORT_ALL_PORT();
		mFA_Link.CJOB__ALL_ABORT();

		xCH__JOB_STATUS->Set__DATA(STR__IDLE);
		xCH__JOB_CTRL->Set__DATA(STR__AVAILABLE);
	}

	xCH_PRE__JOB_CTRL->Set__DATA(var_data);
}
void CObj_Opr__AUTO_MODE::Seq__JOB_END(CII_OBJECT__ALARM* p_alarm)
{
	CString var_data;
	xCH__JOB_STATUS->Get__DATA(var_data);

	if(xCH_PRE__JOB_STATUS->Check__DATA(var_data) > 0)
	{
		return;
	}
	xCH_PRE__JOB_STATUS->Set__DATA(var_data);

	if(xCH__JOB_STATUS->Check__DATA(STR__END) < 0)
	{
		return;
	}
	xSCH_MATERIAL_CTRL->End__ALL_PORT();

	// ...
	int i;

	for(i=0; i<iLPx_UNIT_SIZE; i++)
	{
		if(xCH__PORT_STATUS[i]->Check__DATA(STR__RESERVE) > 0)
		{
			xCH__PORT_CTRL[i]->Set__DATA("ABORTED");
			xCH__PORT_STATUS[i]->Set__DATA("ABORTED");
			xCH__PORT_UPLOAD_FILE_FLAG[i]->Set__DATA("");

			// xCH__PORT_EXCEPTION[i]->Set__DATA("CANCEL");
		}
	}

	// ...
	{
		int alarm_id = ALID__ALL_JOB__END_BY_USER;
		CString r_act;

		p_alarm->Check__ALARM(alarm_id,r_act);
		p_alarm->Post__ALARM(alarm_id);
	}
}

void CObj_Opr__AUTO_MODE::Seq__UPDATE_PORT_INFO()
{
	IDS__SCH_MATERIAL_INFO ds_info;
	CString port_sts;
	int i;

	for(i=0; i<iLPx_UNIT_SIZE; i++)
	{
		xCH__PORT_STATUS[i]->Get__DATA(port_sts);

		if(port_sts.CompareNoCase(STR__BUSY) != 0)
		{
			continue;
		}

		if(xSCH_MATERIAL_CTRL->Get__PORT_FIRST_SLOT_INFO(i+1,ds_info) < 0)
		{
			// xCH__PORT_LOTID[i]->Set__DATA("");
			// xCH__PORT_PPID[i]->Set__DATA("");		
			continue;
		}

		xCH__PORT_LOTID[i]->Set__DATA(ds_info.sLOT_ID);
		xCH__PORT_PPID[i]->Set__DATA(ds_info.sPPID);
	}
}

void CObj_Opr__AUTO_MODE::Seq__UPLOAD_RESERVE_PORT(CII_OBJECT__ALARM *p_alarm)
{
	// HOQ CHECK ...
	{
		for(int i=0; i<iLPx_UNIT_SIZE; i++)
		{
			if(sCH__PORT_HOQ_REQ[i]->Check__DATA(STR__YES) < 0)			continue;

			int hoq_id = i + 1;

			// ...
			bool active__hoq_change = false;

			CUIntArray l__ptn_reserve;
			CUIntArray l__ptn_change;
			
			xSCH_MATERIAL_CTRL->Get__PORT_RESERVE_LIST(l__ptn_reserve);

			int t_limit = l__ptn_reserve.GetSize();
			for(int t=0; t<t_limit; t++)
			{
				int check_id = l__ptn_reserve[t];
				if(hoq_id == check_id)
				{
					active__hoq_change = true;
					continue;
				}

				l__ptn_change.Add(check_id);
			}

			if(active__hoq_change)
			{
				l__ptn_change.InsertAt(0, hoq_id);
				
				xSCH_MATERIAL_CTRL->Set__PORT_RESERVE_LIST(l__ptn_change);
			}

			sCH__PORT_HOQ_REQ[i]->Set__DATA("");
		}
	}

	if(xSCH_MATERIAL_CTRL->Can_Upload__NEXT_PORT() < 0)
	{
		return;
	}

	// ...
	bool active__next_job = false;

	CUIntArray l__ptn_next;
	CUIntArray l__ptn_check;

	xSCH_MATERIAL_CTRL->Get__PORT_RESERVE_LIST(l__ptn_check);

	int i_limit = l__ptn_check.GetSize();

	for(int i=0;i<i_limit;i++)
	{
		int ptn_id = ((int) l__ptn_check[i]);
		int ptn_index = ptn_id - 1;

		if(ptn_index <  0)					continue;
		if(ptn_index >= iLPx_UNIT_SIZE)		continue;

		if((xCH__PORT_CTRL[ptn_index]->Check__DATA("RUN") < 0)	
		|| (xCH__PORT_STATUS[ptn_index]->Check__DATA(STR__RESERVE) < 0))
		{
			continue;
		}

		if(active__next_job)
		{
			l__ptn_next.Add(ptn_id);
		}
		else
		{
			if(_Seq__UPLOAD_JOB(p_alarm, ptn_index) > 0)
			{
				active__next_job = true;

				printf(" * Seq__UPLOAD_RESERVE_PORT() ... \n");
				printf("  * JOB_START : ptn_id (%1d) \n", ptn_id);

				Sleep(100);
			}
			else
			{
				l__ptn_next.Add(ptn_id);
			}
		}
	}

	if(active__next_job)
	{
		int k_limit;
		int k;

		// NEXT LIST ...
		{
			k_limit = l__ptn_next.GetSize();
			printf(" * NEXT_LIST (%1d) \n", k_limit);

			for(k=0; k<k_limit; k++)
			{
				printf("  * LP( %1d ) \n", l__ptn_next[k]);
			}
			printf("\n");
		}

		// CHECK LIST ...
		{
			k_limit = l__ptn_check.GetSize();
			printf(" * CHECK_LIST (%1d) \n", k_limit);

			for(k=0; k<k_limit; k++)
			{
				printf("  * LP( %1d ) \n", l__ptn_check[k]);
			}
			printf("\n");
		}
	}

	xSCH_MATERIAL_CTRL->Set__PORT_RESERVE_LIST(l__ptn_next);
}
int  CObj_Opr__AUTO_MODE::
Check__LPx_CID_ERROR(CII_OBJECT__ALARM *p_alarm,
				     const int ptn,const CString& cstid)
{
	int i;

	if(cstid.GetLength() < 1)
	{
		int db_index = ptn - 1;

		if(xCH__PORT_CTRL[db_index]->Check__DATA("RUN") > 0)
		{
			xCH__PORT_CTRL[db_index]->Set__DATA("AVAILABLE");
		}
		if(xCH__PORT_STATUS[db_index]->Check__DATA(STR__BUSY) > 0)
		{
			xCH__PORT_STATUS[db_index]->Set__DATA("WAITING");
		}

		// ...
		{
			CString err_msg;
			CString r_act;

			err_msg.Format("The CID of LP%1d is NULL.\n",ptn);
			err_msg += "Please, Input CID.\n";

			p_alarm->Popup__ALARM_With_MESSAGE(ALID__JOB_START__CID_NULL_ERROR,err_msg,r_act);
		}
		return -1;
	}

	// ...
	{
		int port_id;

		for(i=0; i<iLPx_UNIT_SIZE; i++)
		{
			port_id = i + 1;
			if(port_id == ptn)
			{
				continue;
			}

			if(xSCH_MATERIAL_CTRL->Check__LPx_CID(port_id,cstid) < 0)
			{
				continue;
			}

			// ...
			{
				CString err_msg;
				CString r_act;

				err_msg.Format("The CID of LP%1d is duplicated.\n",ptn);
				err_msg += "Please, check the other CIDs.\n";

				p_alarm->Popup__ALARM_With_MESSAGE(ALID__JOB_START__CID_DUPLICATE_ERROR,err_msg,r_act);
			}
			return -1;
		}
	}
	return 1;
}
int  CObj_Opr__AUTO_MODE::
Get__LLx_ALL_MODE_STATE(bool& active__ll_all_in_out)
{
	return _Check__LLx_ALL_MODE_ERROR(NULL, active__ll_all_in_out);
}
int  CObj_Opr__AUTO_MODE::
Check__LLx_ALL_MODE_ERROR(CII_OBJECT__ALARM *p_alarm)
{
	bool active_ll_all_in_out;

	return _Check__LLx_ALL_MODE_ERROR(p_alarm, active_ll_all_in_out);
}
int  CObj_Opr__AUTO_MODE::
_Check__LLx_ALL_MODE_ERROR(CII_OBJECT__ALARM *p_alarm, 
						   bool& para__active_ll_all_in_out)
{
	bool active__ll_only_input  = false;
	bool active__ll_only_output = false;

	bool active__ll_all_mode    = false;
	bool active__ll_all_single  = false;
	bool active__ll_all_dual    = false;
	bool active__ll_all_in_out = false;

	// LLx CHECK : ONLY_INPUT & ONLY_OUTPUT ...
	{
		// CHECK : ONLY_INPUT
		{
			CStringArray l__in_id;
			CStringArray l__in_mode;
			CStringArray l__in_slot;

			xSCH_MATERIAL_CTRL->Get__LLx_IN_OF_EDIT_TYPE(l__in_id, l__in_mode, l__in_slot);

			int i_limit = l__in_id.GetSize();
			for(int i=0; i<i_limit; i++)
			{
				CString str__in_id   = l__in_id[i];
				CString str__in_mode = l__in_mode[i];
				CString str__in_slot = l__in_slot[i];

				if(str__in_mode.CompareNoCase(STR__ONLY_INPUT) != 0)
				{
					continue;
				}

				active__ll_only_input = true;
				break;
			}
		}
		// CHECK : ONLY_OUTPUT
		{
			CStringArray l__out_id;
			CStringArray l__out_mode;
			CStringArray l__out_slot;

			xSCH_MATERIAL_CTRL->Get__LLx_OUT_OF_EDIT_TYPE(l__out_id, l__out_mode, l__out_slot);

			int k_limit = l__out_id.GetSize();
			for(int k=0; k<k_limit; k++)
			{
				CString str__out_id   = l__out_id[k];
				CString str__out_mode = l__out_mode[k];
				CString str__out_slot = l__out_slot[k];

				if(str__out_mode.CompareNoCase(STR__ONLY_OUTPUT) != 0)
				{
					continue;
				}

				active__ll_only_output = true;
				break;
			}
		}
	}
	// LLx CHECK : ALL MODE ...
	{
		CStringArray l__in_id;
		CStringArray l__in_mode;
		CStringArray l__in_slot;

		xSCH_MATERIAL_CTRL->Get__LLx_IN_OF_EDIT_TYPE(l__in_id, l__in_mode, l__in_slot);

		CStringArray l__out_id;
		CStringArray l__out_mode;
		CStringArray l__out_slot;

		xSCH_MATERIAL_CTRL->Get__LLx_OUT_OF_EDIT_TYPE(l__out_id, l__out_mode, l__out_slot);

		// ...
		int i_limit = l__in_id.GetSize();
		for(int i=0; i<i_limit; i++)
		{
			CString str__in_id   = l__in_id[i];
			CString str__in_mode = l__in_mode[i];
			CString str__in_slot = l__in_slot[i];

			if(str__in_mode.CompareNoCase(STR__ALL) != 0)
			{
				continue;
			}

			active__ll_all_mode = true;

			if(str__in_slot.CompareNoCase("11") == 0)
			{
				active__ll_all_dual= true;
			}

			// ...
			int k_limit = l__out_id.GetSize();

			for(int k=0; k<k_limit; k++)
			{
				CString str__out_id   = l__out_id[k];
				CString str__out_mode = l__out_mode[k];
				CString str__out_slot = l__out_slot[k];

				if(str__out_mode.CompareNoCase(STR__ALL) != 0)
				{
					continue;
				}

				if((str__in_slot.CompareNoCase("11")  != 0)
				|| (str__out_slot.CompareNoCase("11") != 0))
				{
					if(str__in_id == str__out_id)
					{
						if(str__in_slot == str__out_slot)
						{
							active__ll_all_single = true;
						}
						continue;
					}
				}

				if((str__in_slot.CompareNoCase("10") == 0)
				&& (str__out_slot.CompareNoCase("01") == 0))
				{
					active__ll_all_in_out = true;
				}
				else if((str__in_slot.CompareNoCase("01") == 0)
					 && (str__out_slot.CompareNoCase("10") == 0))
				{
					active__ll_all_in_out = true;
				}
			}
		}
	}

	// ...
	{
		para__active_ll_all_in_out = active__ll_all_in_out;
	}

	// CHECK ERROR ...
	if(active__ll_all_mode)
	{
		int err_code = 1;

		CString err_msg;
		CString err_bff;

		if(active__ll_only_input)
		{
			err_code = -11;
			err_msg = "\"All Mode\" and \"Only_Input\" should not be set at the same time.";
		}
		if(active__ll_only_output)
		{
			err_code = -12;
			err_msg = "\"All Mode\" and \"Only_Output\" should not be set at the same time.";
		}

		if(active__ll_all_single)
		{
			if(active__ll_all_dual)
			{
				err_code = -21;
				err_msg = "\"All Single\" and \"All Dual\" should not be set at the same time.";
			}
			if(active__ll_all_in_out)
			{
				err_code = -22;
				err_msg = "\"All Single\" and \"All UIn & Out\" should not be set at the same time.";
			}
		}
		else if(active__ll_all_dual)
		{
			if(active__ll_all_in_out)
			{
				err_code = -23;
				err_msg = "\"All Dual\" and \"All UIn & Out\" should not be set at the same time.";
			}
		}
		else if(active__ll_all_in_out)
		{

		}
		else
		{
			err_code = -31;
			err_msg = "\"All Dual\" or \"All UIn & Out\" must be set.";
		}

		if(err_code < 0)
		{
			if(p_alarm != NULL)
			{
				CString r_act;
				p_alarm->Popup__ALARM_With_MESSAGE(ALID__JOB_START__LLx_ALL_MODE_ERROR, err_msg, r_act);
			}
			return err_code;
		}
	}

	return 1;
}
int  CObj_Opr__AUTO_MODE::
Check__SYSTEM_ERROR(CII_OBJECT__ALARM* p_alarm)
{
	int i;

	// ...
	{
		bool active__err_check = false;
		CString err_msg;

		if(xSCH_MATERIAL_CTRL->Check__LBx_SCH_MODE__STATUS_ERROR(err_msg) > 0)
		{
			active__err_check = true;

			CString r_act;
			p_alarm->Popup__ALARM_With_MESSAGE(ALID__JOB_START__LBx_MODE_ERROR,err_msg,r_act);
		}
		if(Check__LLx_ALL_MODE_ERROR(p_alarm) < 0)
		{
			active__err_check = true;
		}

		if(active__err_check)
		{
			for(i=0; i<iLPx_UNIT_SIZE; i++)
			{
				if(xCH__PORT_CTRL[i]->Check__DATA("RUN") > 0)
				{
					xCH__PORT_CTRL[i]->Set__DATA("AVAILABLE");
				}
				if(xCH__PORT_STATUS[i]->Check__DATA(STR__BUSY) > 0)
				{
					xCH__PORT_STATUS[i]->Set__DATA("WAITING");
				}
			}

			return -11;
		}
	}

	// ...
	{
		CStringArray l_sys_name;
		xSCH_MATERIAL_CTRL->Get__SYSTEM_LIST(l_sys_name);

		CString sys_name;
		CStringArray l_pos;
		CUIntArray   l_sts;

		int err_flag = -1;
		CString err_msg;
		CString bff;

		err_msg = "There are wafers in the following module.\n";

		int limit = l_sys_name.GetSize();
		int i;

		for(i=0; i<limit; i++)
		{
			sys_name = l_sys_name[i];
			if(xSCH_MATERIAL_CTRL->Check__SYSTEM_STATUS__MATERIAL_STS(sys_name,l_pos,l_sts) < 0)
			{
				continue;
			}

			int t_limit = l_pos.GetSize();
			int t;

			for(t=0;t<t_limit;t++)
			{
				if((int) l_sts[t] < 1)
				{
					continue;
				}

				err_flag = 1;
				bff.Format("     %s In the \"%s\"\n",l_pos[t],sys_name);
				err_msg += bff;
			}
		}

		if(err_flag > 0)
		{
			for(i=0; i<iLPx_UNIT_SIZE; i++)
			{
				if(xCH__PORT_CTRL[i]->Check__DATA("RUN") > 0)
				{
					xCH__PORT_CTRL[i]->Set__DATA("AVAILABLE");
				}
				if(xCH__PORT_STATUS[i]->Check__DATA(STR__BUSY) > 0)
				{
					xCH__PORT_STATUS[i]->Set__DATA("WAITING");
				}
			}

			CString r_act;
			p_alarm->Popup__ALARM_With_MESSAGE(ALID__JOB_START__MATERIAL_ERROR,err_msg,r_act);

			return -1;
		}
	}

	// ...
	{
		CStringArray l_sys_name;
		xSCH_MATERIAL_CTRL->Get__SYSTEM_LIST(l_sys_name);

		CString sys_name;
		CStringArray l_pmc_name;
		CUIntArray   l_pmc_sts;

		int err_flag = 1;
		CString err_msg;
		CString bff;

		int limit = l_sys_name.GetSize();
		int i;

		err_msg = "There is no available process module in system.\n";

		if(limit < 1)	
		{
			err_msg += "   System List : [0]\n";
		}

		for(i=0;i<limit;i++)
		{
			sys_name = l_sys_name[i];

			// ...
			{
				bff.Format("   %1d)  System Name : [%s]\n",i+1,sys_name);
				err_msg += bff;
			}

			if(xSCH_MATERIAL_CTRL->Check__SYSTEM_STATUS__PMC_STS(sys_name,l_pmc_name,l_pmc_sts) < 0)
			{
				continue;
			}

			// ...
			int t_limit = l_pmc_name.GetSize();
			int t;

			for(t=0;t<t_limit;t++)
			{
				bff.Format("      [%s] : [%1d]\n",l_pmc_name[t],l_pmc_sts[t]);
				err_msg += bff;

				if((int) l_pmc_sts[t] < 1)
				{
					continue;
				}

				err_flag = -1;
				break;
			}
		}

		if(err_flag > 0)
		{
			for(i=0; i<iLPx_UNIT_SIZE; i++)
			{
				if(xCH__PORT_CTRL[i]->Check__DATA("RUN") > 0)
				{
					xCH__PORT_CTRL[i]->Set__DATA("AVAILABLE");
				}
				if(xCH__PORT_STATUS[i]->Check__DATA(STR__BUSY) > 0)
				{
					xCH__PORT_STATUS[i]->Set__DATA("WAITING");
				}
			}

			CString r_act;
			p_alarm->Popup__ALARM_With_MESSAGE(ALID__JOB_START__PM_ERROR,err_msg,r_act);

			return -1;
		}
	}

	// ...
	{
		CStringArray l_sys_name;
		xSCH_MATERIAL_CTRL->Get__SYSTEM_LIST(l_sys_name);

		CString sys_name;
		CStringArray l_name;
		CStringArray l_err_module;
		CUIntArray   l_sts;

		int err_flag = -1;
		CString err_msg;
		CString bff;

		int limit = l_sys_name.GetSize();
		int i;

		for(i=0;i<limit;i++)
		{
			sys_name = l_sys_name[i];
			if(xSCH_MATERIAL_CTRL->Check__SYSTEM_STATUS__MODULE_STS(sys_name,l_name,l_sts) < 0)
			{
				continue;
			}

			// ...
			int t_limit = l_name.GetSize();
			int t;

			for(t=0;t<t_limit;t++)
			{
				if((int) l_sts[t] != MODULE_STATUS__NOT_AVAILABLE)
				{
					continue;
				}

				CString str_name = l_name[t];
				
				int ll_i = Macro__Get_LL_INDEX(str_name);
				if((ll_i >= 0) && (ll_i < iLLx_SIZE))
				{	
					if(xEXT_CH__CFG_DB_LLx_USE_FLAG[ll_i]->Check__DATA(STR__DISABLE) > 0)
					{
						continue;
					}

					// jglee : 2021.11.11
					if(dCH_CFG__LLx_STATE_CHECK_FOR_JOB_START->Check__DATA(STR__DISABLE) > 0)
					{
						continue;
					}
				}

				l_err_module.Add(str_name);
				err_flag = 1;
			}
		}

		if(err_flag > 0)
		{
			for(i=0; i<iLPx_UNIT_SIZE; i++)
			{
				if(xCH__PORT_CTRL[i]->Check__DATA("RUN") > 0)
				{
					xCH__PORT_CTRL[i]->Set__DATA("AVAILABLE");
				}
				if(xCH__PORT_STATUS[i]->Check__DATA(STR__BUSY) > 0)
				{
					xCH__PORT_STATUS[i]->Set__DATA("WAITING");
				}
			}

			// ...
			{
				int err_count = 0;
				int limit = l_err_module.GetSize();
				int i;

				err_msg += "Please, initialize the following module.\n";

				for(i=0;i<limit;i++)
				{
					if(i < (limit - 1))		bff.Format("  %s,",l_err_module[i]);
					else					bff.Format("  %s", l_err_module[i]);

					err_msg += bff;

					err_count++;
					if(err_count > 3)		
					{
						err_count = 0;
						err_msg += "\n";
					}
				}
			}

			CString r_act;
			p_alarm->Popup__ALARM_With_MESSAGE(ALID__JOB_START__MODULE_ERROR,err_msg,r_act);

			return -1;
		}
	}
	return 1;
}
void CObj_Opr__AUTO_MODE::
Seq__UPLOAD_JOB(CII_OBJECT__ALARM* p_alarm)
{
	if(xCH__JOB_STATUS->Check__DATA(STR__IDLE) > 0)
	{
		int start_flag = -1;
		int ptn = -1;
		int i;

		for(i=0; i<iLPx_UNIT_SIZE; i++)
		{
			if(xCH__PORT_CTRL[i]->Check__DATA("RUN") < 0)				continue;
			if(xCH__PORT_STATUS[i]->Check__DATA(STR__BUSY) > 0)			continue;

			start_flag = 1;
			ptn = i + 1;
			break;
		}

		if(start_flag > 0)
		{
			if(Check__SYSTEM_ERROR(p_alarm) < 0)
			{
				mFA_Link.CJOB__ALL_ABORT();
				return;
			}
		}
	}

	// ...
	CString cstid;
	int ptn;
	int i;

	for(i=0; i<iLPx_UNIT_SIZE; i++)
	{
		if(xCH__PORT_CTRL[i]->Check__DATA("RUN") < 0)				continue;
		if(xCH__PORT_STATUS[i]->Check__DATA(STR__BUSY) > 0)			continue;

		ptn = i + 1;

		if(xCH__PORT_CFG_USE_TYEP[i]->Check__DATA(STR__DUMMY) > 0)
		{
			xCH__PORT_CTRL[i]->Set__DATA("AVAILABLE");

			// ...
			{
				CString alarm_msg;
				CString msg_bff;

				alarm_msg.Format("The new job of LP%1d can not be executed. \n", ptn);
				msg_bff.Format("The status of LP%1d must be \"PROCESS\". \n", ptn);
				alarm_msg += msg_bff;	
				msg_bff.Format("The current status of LP%1d is \"DUMMY\". \n", ptn);
				alarm_msg += msg_bff;

				// ...
				CString r_act;
				int alarm_id = ALID__JOB_START__ERROR_AT_DUMMY_PORT;

				p_alarm->Check__ALARM(alarm_id,r_act);
				p_alarm->Post__ALARM_With_MESSAGE(alarm_id,alarm_msg);
			}
			continue;
		}

		if(xCH__PORT_STATUS[i]->Check__DATA("WAITING") > 0)
		{	
			// ...
			{
				xCH__CSTID[i]->Get__DATA(cstid);
				
				if(Check__LPx_CID_ERROR(p_alarm,ptn,cstid) < 0)
				{
					xCH__PORT_CTRL[i]->Set__DATA("ABORTED");
					xCH__PORT_STATUS[i]->Set__DATA("ABORTED");

					//mFA_Link.CJOB__LPx_ABORT(ptn);
					continue;
				}
			}

			if(xCH__JOB_STATUS->Check__DATA(STR__END) > 0)
			{
				CString alarm_msg;
				alarm_msg.Format("The new job of LP%1d can not be executed, when system mode is \"END\".",ptn);

				CString r_act;
				int alarm_id = ALID__JOB_START__NEW_JOB_ERROR_IN_SYS_END;
				p_alarm->Check__ALARM(alarm_id,r_act);
				p_alarm->Post__ALARM_With_MESSAGE(alarm_id+i,alarm_msg);

				xCH__PORT_STATUS[i]->Set__DATA("ABORTED");
				xCH__PORT_CTRL[i]->Set__DATA("AVAILABLE");
				continue;
			}

			// PPID CHECK & PMx RECIPE CHECK
			{
				// ...
				{
					CString dir_lp;
					CString lp_rcp;
					int lp_id = i + 1;

					dir_lp.Format("LP%1d\\",lp_id);
					xCH__PORT_JOB_NAME[i]->Get__DATA(lp_rcp);

					if(xSCH_JOB_FILE_CTRL->Upload__JOB_FILE(sDIR_PROCESS,dir_lp,lp_rcp) < 0)
					{
						CString alarm_msg;
						alarm_msg.Format("There is no the \"%s\" job.",lp_rcp);

						CString r_act;
						int alarm_id = ALID__JOB_START__JOB_FILE_ERROR;
						p_alarm->Check__ALARM(alarm_id,r_act);
						p_alarm->Post__ALARM_With_MESSAGE(alarm_id,alarm_msg);

						xCH__PORT_STATUS[i]->Set__DATA("WAITING");
						xCH__PORT_CTRL[i]->Set__DATA("ABORTED");
						continue;
					}
				}

				// ...
				{
					CUIntArray   l_slot_err_id;
					CStringArray l_slot_err_msg;

					xSCH_JOB_FILE_CTRL->Get__SLOT_ERROR_INFO(l_slot_err_id,l_slot_err_msg);

					CString alarm_msg;
					CString bff;

					int err_count = 0;
					int slot_id;
					int k_limit = l_slot_err_id.GetSize();
					int k;

					for(k=0;k<k_limit;k++)
					{
						slot_id = k + 1;

						switch(l_slot_err_id[k])
						{
							case CSS_FILE__SLOT_ERROR__PPID_EMPTY:
								err_count++;

								bff.Format("Slot %002d)  PPID EMPTY ERROR\n",slot_id);
								alarm_msg += bff;
								break;

							case CSS_FILE__SLOT_ERROR__ROUTE_NoEXIST:
								err_count++;

								bff.Format("Slot %002d)  [%s] PPID NO EXIST\n",slot_id,l_slot_err_msg[k]);
								alarm_msg += bff;
								break;
						}
					}

					if(err_count > 0)
					{
						CString r_act;
						int alarm_id = ALID__JOB_START__ROUTE_RECIPE_ERROR;

						p_alarm->Check__ALARM(alarm_id,r_act);
						p_alarm->Post__ALARM_With_MESSAGE(alarm_id,alarm_msg);

						xCH__PORT_STATUS[i]->Set__DATA("WAITING");
						xCH__PORT_CTRL[i]->Set__DATA("ABORTED");
						continue;
					}
				}

				if(iSIM_MODE < 0)
				{
					SCX__FILE_RCP_CTRL x_file_rcp_ctrl;
					CStringArray l_pm_name;
					CStringArray l_pm_rcp;

					xSCH_JOB_FILE_CTRL->Get__PRC_RECIPE(l_pm_name,l_pm_rcp);
				
					// ...
					int seq_id;

					CString pmc_name;
					CString rcp_name;

					CString alarm_msg;
					CString alarm_bff;
					int err_count = 0;

					alarm_msg = "PMx Recipe Error\n";

					// ...
					{
						int limit = l_pm_name.GetSize();
						int i;

						for(i=0;i<limit;i++)
						{
							pmc_name = l_pm_name[i];
							rcp_name = l_pm_rcp[i];

							seq_id = Macro__Check_SEQ_ID_From_PMx(pmc_name);
							if(seq_id < 0)		continue;

							if(seq_id > 0)
							{
								int  pm_i = Macro__Get_PM_INDEX(pmc_name);
								if(pm_i >= 0)		
								{
									if(dEXT_CH__SCH_TEST_CFG_PMX_DUMMY_BY_CTC[pm_i]->Check__DATA(STR__YES) > 0)
										continue;

									if(xEXT_CH__CFG__PMx_USE[pm_i]->Check__DATA(STR__DISABLE) > 0)
										continue;
								}
							}
							else if(seq_id == 1)
							{
								int ll_i = Macro__Get_LLx_INDEX_OF_LLx_NAME(pmc_name);

								if((ll_i >= 0) && (ll_i < CFG_LLx_LIMIT))
								{
									if(xEXT_CH__CFG_DB_LLx_USE_FLAG[ll_i]->Check__DATA(STR__DISABLE) > 0)
										continue;
								}
							}

							if(x_file_rcp_ctrl->Check__REMOTE_FILE(seq_id, "", rcp_name, "rcp") < 0)
							{
								alarm_bff.Format("   In \"%s\", [%s] recipe no exist. or %s's status is offline.\n", 
												 pmc_name, rcp_name, pmc_name);
								alarm_msg += alarm_bff;

								alarm_bff.Format("   * EqpID : %1d \n", seq_id);
								alarm_msg += alarm_bff;

								err_count++;
							}
						}
					}

					if(err_count > 0)
					{
						CString r_act;
						int alarm_id = ALID__JOB_START__PM_RECIPE_ERROR;

						p_alarm->Check__ALARM(alarm_id,r_act);
						p_alarm->Post__ALARM_With_MESSAGE(alarm_id,alarm_msg);

						/*
						xCH__PORT_STATUS[i]->Set__DATA("ABORTED");
						xCH__PORT_CTRL[i]->Set__DATA("ABORTED");
						*/
						xCH__PORT_STATUS[i]->Set__DATA("WAITING");
						xCH__PORT_CTRL[i]->Set__DATA("AVAILABLE");
						continue;
					}
				}
			}

			// PROCESS TYPE CHECK ...
			{
				CString str__proc_type = xCH__LPx__CFG_PROCESS_TYPE[i]->Get__STRING();

				// ...
				int flag__pmc_check = -1;

				CStringArray l__pmc_name;
				xSCH_JOB_FILE_CTRL->Get__PRC_MODULE(l__pmc_name);

				int k_limit = l__pmc_name.GetSize();
				if(k_limit > iPMx_UNIT_SIZE)		k_limit = iPMx_UNIT_SIZE;	

				for(int k=0; k < k_limit; k++)
				{
					int pm_index = Macro__Get_PM_INDEX(l__pmc_name[k]);
					
					if(pm_index <  0)						continue;
					if(pm_index >= iPMx_UNIT_SIZE)			continue;

					if(xEXT_CH__PMx_CFG_USE[pm_index]->Check__DATA(STR__ENABLE) < 0)
					{
						continue;
					}
					
					if(xEXT_CH__PMx_PROCESS_TYPE[pm_index]->Check__DATA(str__proc_type) < 0)
					{
						continue;
					}

					// ...
					{
						CString var_data = xEXT_CH__PMx_OBJECT_STATUS[pm_index]->Get__STRING();

						if((var_data.CompareNoCase(STR__CTCINUSE) != 0)
						&& (var_data.CompareNoCase(STR__STANDBY)  != 0))
						{
							continue;
						}
					}

					flag__pmc_check = 1;
					break;
				}

				if(flag__pmc_check < 0)
				{
					CString err_msg;
					CString err_bff;

					// ...
					{
						int port_id = i + 1;

						err_msg.Format("PMx list of LP%1d \n", port_id);

						int k_limit = l__pmc_name.GetSize();
						for(int k=0; k < k_limit; k++)
						{
							err_bff.Format("  %s \n", l__pmc_name[k]);
							err_msg += err_bff;
						}
					}

					if(str__proc_type.CompareNoCase(STR__NORMAL) == 0)
					{
						int alarm_id = ALID__JOB_START__PM_NORMAL_TYPE__NOT_EXIST;
						CString r_act;

						p_alarm->Check__ALARM(alarm_id,r_act);
						p_alarm->Post__ALARM_With_MESSAGE(alarm_id, err_msg);
					}
					else if(str__proc_type.CompareNoCase(STR__TEST) == 0)
					{
						int alarm_id = ALID__JOB_START__PM_TEST_TYPE__NOT_EXIST;
						CString r_act;

						p_alarm->Check__ALARM(alarm_id,r_act);
						p_alarm->Post__ALARM_With_MESSAGE(alarm_id, err_msg);
					}

					xCH__PORT_STATUS[i]->Set__DATA("WAITING");
					xCH__PORT_CTRL[i]->Set__DATA("ABORTED");
					continue;
				}
			}

			xCH__PORT_STATUS[i]->Set__DATA(STR__RESERVE);
			xCH__PORT_UPLOAD_FILE_FLAG[i]->Set__DATA("");

			xSCH_MATERIAL_CTRL->Set__PORT_RESERVE(ptn);

			// ...
			{
				CString str_lotid;
				CString str_ppid;

				xSCH_JOB_FILE_CTRL->Get__LOTID_PPID_OF_FIRST_DB(str_lotid,str_ppid);

				xCH__PORT_LOTID[i]->Set__DATA(str_lotid);
				xCH__PORT_PPID[i]->Set__DATA(str_ppid);
			}

			// SLOT MAP	...
			{
				CStringArray l_slot;
				xSCH_JOB_FILE_CTRL->Get__SLOT_NUM_LIST(l_slot);

				if(pPORT__OBJ_CTRL[i] != NULL)
				{
					pPORT__OBJ_CTRL[i]->Call__OBJ_MODE("SCH_MAP",l_slot);
				}
			}

			if(xCH__PORT_JOB_POPUP_TYPE[i]->Check__DATA("") > 0)
			{
				// CJOB & PJOB CREATE ...
				CString job_name;
				CString cstid;
					
				xCH__PORT_JOB_NAME[i]->Get__DATA(job_name);
				xCH__CSTID[i]->Get__DATA(cstid);

				// ...
				CString pjobid = job_name;
				CString pre_pjob;
				
				CUIntArray l_slotmap;
				CString ppid = "";

				if(Check__CYCLE_MODE() > 0)
				{
					CString cur_pjob;	
					cur_pjob.Format("%s_%1d", pjobid,iCYCLE_LOT_COUNT);

					xCH__PORT_PJOBID[i]->Get__DATA(pre_pjob);
					if(pre_pjob.CompareNoCase(cur_pjob) != 0)
					{
						mFA_Link.CJOB__COMPLETE(pre_pjob);
					}

					pjobid = cur_pjob;
				}
				else
				{
					if(Check__LPx_CYCLE_MODE(i,1) > 0)
					{
						CString var_data;
						int i_lot_count;

						xCH__PORT_CUR_CYCLE_COUNT[i]->Get__DATA(var_data);
						i_lot_count = atoi(var_data);

						CString cur_pjob;	
						cur_pjob.Format("%s_%1d", pjobid,i_lot_count);
						
						xCH__PORT_PJOBID[i]->Get__DATA(pre_pjob);						
						if(pre_pjob.CompareNoCase(cur_pjob) != 0)
						{
							mFA_Link.CJOB__COMPLETE(pre_pjob);
						}
						
						pjobid = cur_pjob;
					}
				}

				xCH__PORT_CJOBID[i]->Set__DATA(pjobid);
				xCH__PORT_PJOBID[i]->Set__DATA(pjobid);

				mFA_Link.Create__Local_PJOBID(pjobid,cstid,ptn,l_slotmap,ppid);
				mFA_Link.Create__Local_CJOBID(pjobid,cstid);

				// ...
				{
					CString log_msg;
					log_msg.Format("LP%1d : Pre-PJob [%s], Cur-PJob [%s], Cycle-Count [%1d].",
								   i+1,
								   pre_pjob,
								   pjobid,
								   iCYCLE_LOT_COUNT);

					xLOG_CTRL->WRITE__LOG(log_msg);
				}
			}
			else
			{
				xCH__PORT_CJOBID[i]->Set__DATA("");
			}
		}
		else if((xCH__PORT_STATUS[i]->Check__DATA("WAITING") < 0)
			 && (xCH__PORT_STATUS[i]->Check__DATA(STR__RESERVE) < 0))
		{					
			xCH__PORT_CTRL[i]->Set__DATA("ABORTED");
			continue;
		}

		// ...
	}
}

int  CObj_Opr__AUTO_MODE::
Check__LPx_CYCLE_MODE(const int lp_index,const int count_flag)
{
	if(xCH__PORT_CYCLE_CTRL_FLAG[lp_index]->Check__DATA(STR__RUNNING) < 0)
	{
		return -1;
	}

	if(xCH__PORT_END_REQ_FLAG[lp_index]->Check__DATA(STR__YES) > 0)
	{
		return -1;
	}

	// ...
	int ref_cycle_count = 0;
	int cur_cycle_count = 0;

	CString str_msg;
	CString var_data;

	xCH__PORT_DB_CYCLE_COUNT[lp_index]->Get__DATA(var_data);
	ref_cycle_count = atoi(var_data);

	xCH__PORT_CUR_CYCLE_COUNT[lp_index]->Get__DATA(var_data);
	cur_cycle_count = atoi(var_data);

	if(cur_cycle_count >= ref_cycle_count)
	{
		str_msg.Format("End.");
		xCH__PORT_DB_CYCLE_MSG[lp_index]->Set__DATA(str_msg);

		xCH__PORT_CYCLE_CTRL_FLAG[lp_index]->Set__DATA("");
		return -1;
	}
	
	if(count_flag > 0)
	{
		cur_cycle_count++;
		var_data.Format("%1d", cur_cycle_count);
		xCH__PORT_CUR_CYCLE_COUNT[lp_index]->Set__DATA(var_data);
		
		str_msg.Format("Cycle (%1d)", cur_cycle_count);
		xCH__PORT_DB_CYCLE_MSG[lp_index]->Set__DATA(str_msg);		
		
		xCH__PORT_CYCLE_CTRL_RUNNING_FLAG[lp_index]->Set__DATA(STR__YES);
	}
	return 1;
}


// ...
#define  DEF__NEXT_CHECK			10001

int  CObj_Opr__AUTO_MODE::
_Seq__UPLOAD_JOB(CII_OBJECT__ALARM *p_alarm,const int lp_index)
{
	CString dir_lp;
	CString lp_rcp;

	int lp_id = lp_index+1;

	dir_lp.Format("LP%1d\\",lp_id);
	xCH__PORT_JOB_NAME[lp_index]->Get__DATA(lp_rcp);

	// ...
	int flag = Fnc__UPLOAD_JOB(p_alarm,lp_id,dir_lp,lp_rcp);

	if(flag == DEF__NEXT_CHECK)
	{
		return -1;
	}
	else if(flag > 0)
	{
		CString str_cid = xCH__CSTID[lp_index]->Get__STRING();
		xSCH_MATERIAL_CTRL->Set__LPx_CID(lp_id, str_cid);

		xCH__PORT_LAMP_STATUS[lp_index]->Set__DATA("PROCESS");
		xCH__PORT_STATUS[lp_index]->Set__DATA(STR__BUSY);

		CStringArray l_slot;
		xSCH_MATERIAL_CTRL->Get__LPx_SLOT_LIST(lp_id,l_slot);

		if(pPORT__OBJ_CTRL[lp_index] != NULL)
		{
			pPORT__OBJ_CTRL[lp_index]->Call__OBJ_MODE("SCH_MAP",l_slot);
		}

		// ...
		{
			CStringArray l_proc_slot;
			xSCH_MATERIAL_CTRL->Get__LPx_SLOT_LIST(lp_id,l_proc_slot);

			CString cst_id;
			xCH__CSTID[lp_index]->Get__DATA(cst_id);

			xSCH_MATERIAL_CTRL->Start__JOB(lp_id,l_proc_slot,cst_id);
		}

		// ...
		{
			printf(" * LP%1d : Start !!! \n", lp_id); 

			xSCH_MATERIAL_CTRL->Get__LPx_SLOT_LIST(lp_id, l_slot);

			int i_limit = l_slot.GetSize();
			int i;

			for(i=0; i<i_limit; i++)
			{
				int slot_id = atoi(l_slot[i]);

				// ...
				CString loop_id = "?";
				int loop_count  = -1;

				int r_flag = xSCH_MATERIAL_CTRL->Get__PROC_LOOPx_OF_EDIT_TYPE(lp_id,slot_id, loop_id,loop_count);

				printf(" * %02d)  LP%1d:%02d <- [%s] [%1d] : (r_flag : %1d) \n", 
					   i+1,lp_id,slot_id, loop_id,loop_count, r_flag);

				if(r_flag > 0)
					xSCH_MATERIAL_CTRL->Set__PROC_LOOPx_OF_EDIT_TYPE(lp_id,slot_id, loop_id,loop_count);
			}
		}

		// ...
		{
			CString str_lotid;
			CString str_cstid;

			xCH__PORT_LOTID[lp_index]->Get__DATA(str_lotid);
			xCH__CSTID[lp_index]->Get__DATA(str_cstid);

			// jglee - 2020.07.07
			{
				CString str_portid;
				str_portid.Format("%1d", lp_id);

				CString str_jobid = xCH__PORT_JOB_NAME[lp_index]->Get__STRING();

				CString dir_log;
				dir_log.Format("\\LPx+%s+%s+%s+%s", str_portid,str_lotid,str_jobid,str_cstid);

				SCX__FILE_CTRL x_file_ctrl;
				CString ref_root;

				// ...
				{
					x_file_ctrl->DISABLE__TIME_LOG();

					ref_root  = sDIR_ROOT;
					ref_root += "\\EQP_LOG";
					x_file_ctrl->Create__LOG_DIR(ref_root);

					ref_root += "\\Wafer_Data";
					x_file_ctrl->Create__LOG_DIR(ref_root);
					sDIR_WFR_DATA = ref_root;

					x_file_ctrl->Delete__LOG_DIR(ref_root, 90);
				}

				// ...
				{
					CString dir_date;
					CString dir_lot;
					CString dir_date_lot;

					//
					dir_date  = "\\";
					dir_date += Macro__Get_Current_Date();
					
					dir_date_lot = dir_date;
					ref_root += dir_date;
					x_file_ctrl->Create__LOG_DIR(ref_root);

					//
					dir_lot  = "\\";
					dir_lot += dir_log;
					
					dir_date_lot += dir_lot;
					ref_root += dir_lot;
					x_file_ctrl->Create__LOG_DIR(ref_root);

					//
					xSCH_MATERIAL_CTRL->Set__DIR_JOB_LOG(lp_id, dir_date_lot);

					printf(" ** 1)  LP(%1d) <- [%s] \n", lp_id, dir_date_lot);
				}

				// CJob Info ...
				{
					CString job_info;
					CString bff_info;

					bff_info.Format("#JOBID=%s\n",  str_jobid);
					job_info += bff_info;

					bff_info.Format("#LOTID=%s\n",  str_lotid);
					job_info += bff_info;

					bff_info.Format("#PORTID=%s\n", str_portid);
					job_info += bff_info;

					bff_info.Format("#CSTID=%s\n",  str_cstid);
					job_info += bff_info;

					//
					CString str_slot;
					
					Fnc__GET_PORT_SLOTMAPLIST(lp_id, str_slot);
					bff_info.Format("#SLOTMAP.LIST=%s\n", str_slot);
					job_info += bff_info;

					Fnc__GET_PORT_SLOTJOBLIST(lp_id, str_slot);
					bff_info.Format("#SLOTJOB.LIST=%s\n", str_slot);
					job_info += bff_info;

					//
					CString str_start_time = Macro__Get_Current_Date_Time();
					bff_info.Format("#TIME.START=%s\n", str_start_time);
					job_info += bff_info;

					//
					CString path_job;
					path_job.Format("%s\\%s.job_log", ref_root,str_jobid);		

					x_file_ctrl->Write__LOG_MSG(path_job, job_info);
				}

				// Route Info ...
				{
					CString str_ppid = xCH__PORT_DB_ROUTEID[lp_index]->Get__STRING();

					CString src_path;
					src_path.Format("%s\\ROUTE\\%s.rte", sDIR_PROCESS,str_ppid);

					CString trg_path;
					trg_path.Format("%s\\%s.rte_log", ref_root,str_ppid);
									
					CopyFile(src_path, trg_path, TRUE);
				}
			}

			// ...
			{
				CString str_data;

				str_data.Format("LP%1d - Start", lp_id);
				sEXT_CH__FNC_MSG->Set__DATA(str_data);
			}
		}
		return 1;
	}
	else if(flag == 0)
	{
		mFA_Link.PJOB__DELETE(lp_id);

		xCH__PORT_STATUS[lp_index]->Set__DATA("WAITING");
		xCH__PORT_CTRL[lp_index]->Set__DATA("AVAILABLE");
		return -1;
	}
	else if(flag < 0)
	{
		mFA_Link.PJOB__DELETE(lp_id);

		xCH__PORT_STATUS[lp_index]->Set__DATA("ABORTED");
		xCH__PORT_CTRL[lp_index]->Set__DATA("ABORTED");
		return -1;
	}

	return -1;
}

void CObj_Opr__AUTO_MODE::Seq__CHECK_PORT_COMPLETE(CII_OBJECT__ALARM *p_alarm)
{
	CString lp_sts;
	int lp_id;
	int i;

	for(i=0; i<iLPx_UNIT_SIZE; i++)
	{
		if(xCH__PORT_STATUS[i]->Check__DATA(STR__BUSY) < 0)
		{
			continue;
		}

		lp_id = i+1;
		
		if(xSCH_MATERIAL_CTRL->Check__LPx_COMPLETE(lp_id) > 0)
		{
			int flag__cycle_mode = -1;

			if(xCH__PORT_END_REQ_FLAG[i]->Check__DATA(STR__YES) < 0)
			{
				if(Check__CYCLE_MODE() > 0)
				{
					flag__cycle_mode = 1;
				}
				else if(Check__LPx_CYCLE_MODE(i,-1) > 0)
				{
					flag__cycle_mode = 2;
				}
			}

			if(flag__cycle_mode > 0)
			{
				if(xCH__JOB_STATUS->Check__DATA(STR__END) > 0)
				{
					xCH__PORT_CTRL[i]->Set__DATA("AVAILABLE");

					// xCH__PORT_STATUS[i]->Set__DATA("COMPLETED");
					xCH__PORT_STATUS[i]->Set__DATA("ABORTED");
				}
				else
				{
					bool active__lp_reload = true;

					if((xCH__LPx_END_REQ_FLAG[i]->Check__DATA(STR__YES)    > 0)
					|| (xCH__LPx_RETURN_REQ_FLAG[i]->Check__DATA(STR__YES) > 0)
					|| (xCH__PORT_LAMP_STATUS[i]->Check__DATA(STR__END) > 0))
					{
						active__lp_reload = false;
					}

					if(active__lp_reload)
					{
						int act_flag = 1;

						if(xEXT_CH__CFG_CYCLE_FOUP_CTRL_MODE->Check__DATA(STR__UNLOAD_RELOAD) > 0)
						{
							xCH__PORT_STATUS[i]->Set__DATA("UNLOAD_RELOAD");

							// ...
							SCX__ASYNC_TIMER_CTRL x_async_timer;
							x_async_timer->START__COUNT_UP(9999);

							Sleep(1000);

							while(1)
							{
								if((xCH__JOB_CTRL->Check__DATA(STR__ABORT) > 0)
								|| (xSCH_MATERIAL_CTRL->Check__SYSTEM_ABORT() > 0))
								{
									act_flag = -1;
									break;
								}

								if(xCH__PORT_STATUS[i]->Check__DATA("WAITING") > 0)
								{
									break;
								}

								if(x_async_timer->Get__CURRENT_TIME() > 120.0)
								{
									act_flag = -1;
									break;
								}

								Sleep(100);
							}
						}

						if(act_flag > 0)
						{
							CString var_data = xCH__PORT_CFG_SLOT_MAX[i]->Get__STRING();
							int slot_max = atoi(var_data);
							
							for(int k=0; k<slot_max; k++)
							{
								xCH__PORT_SLOT_STS[i][k]->Get__DATA(var_data);
								
								if(var_data.CompareNoCase(STR__NONE) != 0)
								{
									xCH__PORT_SLOT_STS[i][k]->Set__DATA(STR__EXIST);
								}
							}
						}

						if(act_flag > 0)
						{
							int job_flag = 1;

							// ...
							{
								SCX__SEQ_INFO x_seq_info;

								CString str_lotid;
								CString str_ppid;
								CString var_data;

								// ...
								{
									int i_hour;
									int i_min;
									int i_sec;
									int i_msec;
								
									x_seq_info->Get__TIME(i_hour,i_min,i_sec,i_msec);
								
									// ...
									{
										xCH__PORT_DB_LOTID[i]->Get__DATA(var_data);
										str_lotid.Format("%s_%002d%002d%002d", var_data,i_hour,i_min,i_sec);
									
										xCH__PORT_DB_ROUTEID[i]->Get__DATA(str_ppid);
									}
								}

								// css ...
								{
									CString str_jobid;
									CString str_job;
									CString str_css;
									CString str_bff;
									CString dir_proc;
									CString dir_path;
								
									int lp_id = i + 1;

									// ...
									xCH__PORT_CFG_SLOT_MAX[i]->Get__DATA(var_data);
									int slot_max = atoi(var_data);
								
									xCH__PORT_DB_CYCLE_START_SLOT[i]->Get__DATA(var_data);
									int s_slot = atoi(var_data);
								
									xCH__PORT_DB_CYCLE_END_SLOT[i]->Get__DATA(var_data);
									int e_slot = atoi(var_data);
								
									int cur_slot;
									int k;
								
									for(k=0;k<slot_max;k++)
									{
										cur_slot = k + 1;
									
										xCH__PORT_SLOT_STS[i][k]->Get__DATA(var_data);
									
										if((var_data.CompareNoCase(STR__NONE) == 0)
										|| (cur_slot < s_slot)
										|| (cur_slot > e_slot))
										{
											str_bff.Format("!SLOT%002d,%s,,,,\n", k+1,str_lotid);
											str_css += str_bff;
										}
										else
										{
											str_bff.Format("#SLOT%002d,%s,,%s,,X,,\n", k+1,str_lotid,str_ppid);
											str_css += str_bff;
										}
									}
								
									// ...
									{
										x_seq_info->Get__DIR_PROCESS(dir_proc);
										dir_path.Format("%s\\LP%1d\\LP%1d_LocalJOB.css", dir_proc,lp_id,lp_id);
									
										// ...
										{
											FILE *out = fopen(dir_path,"w");
										
											if(out != NULL)
											{
												fputs(str_css,out);
												fclose(out);
											}
											else
											{
												// Alarm Report ...
												{
													int alarm_id = ALID__MANUAL_JOB_START__ERROR;
													CString alarm_msg;
													CString alarm_bff;
													CString r_act;
												
													alarm_msg.Format("LP%1d's manual job error ! \n", lp_id);
													alarm_bff.Format("Cassete path is \"%s\". \n", dir_path);
													alarm_msg += alarm_bff;
												
													p_alarm->Check__ALARM(alarm_id,r_act);
													p_alarm->Post__ALARM_With_MESSAGE(alarm_id,alarm_msg);
												}

												// ...
												{
													xCH__PORT_CTRL[i]->Set__DATA("ABORTED");

													// ...
													CString str_msg;
													int i_lot_count;
												
													xCH__PORT_CUR_CYCLE_COUNT[i]->Get__DATA(var_data);
													i_lot_count = atoi(var_data);

													str_msg.Format("(%1d) Error", i_lot_count);
													xCH__PORT_DB_CYCLE_MSG[i]->Set__DATA(str_msg);

													xCH__PORT_CYCLE_CTRL_FLAG[i]->Set__DATA("");
												}

												job_flag = -1;
											}
										}
									}
								}
							}

							if(job_flag > 0)
							{
								xCH__PORT_STATUS[i]->Set__DATA("WAITING");	
								xCH__PORT_CTRL[i]->Set__DATA("RUN");
							}
							else
							{
								xCH__PORT_STATUS[i]->Set__DATA("WAITING");	
								xCH__PORT_CTRL[i]->Set__DATA("ABORTED");
							}
						}
					}
					else
					{
						xCH__PORT_CTRL[i]->Set__DATA("AVAILABLE");
						xCH__PORT_STATUS[i]->Set__DATA("ABORTED");
					}
				}
			}
			else
			{
				CString str_result;

				if((xCH__JOB_STATUS->Check__DATA(STR__END) > 0)
				|| (xCH__LPx_END_REQ_FLAG[i]->Check__DATA(STR__YES)    > 0)
				|| (xCH__LPx_RETURN_REQ_FLAG[i]->Check__DATA(STR__YES) > 0))
				{
					xCH__PORT_CTRL[i]->Set__DATA("AVAILABLE");
					xCH__PORT_STATUS[i]->Set__DATA("ABORTED");
					
					xCH__PORT_UPLOAD_FILE_FLAG[i]->Set__DATA("");

					str_result = "ABORTED";
				}
				else
				{
					xCH__PORT_CTRL[i]->Set__DATA("AVAILABLE");
					xCH__PORT_STATUS[i]->Set__DATA("COMPLETED");

					xCH__PORT_UPLOAD_FILE_FLAG[i]->Set__DATA("");

					str_result = "COMPLETED";
				}

				xSCH_MATERIAL_CTRL->Complete__JOB(lp_id);

				// ...
				{
					CString str_data;

					if(str_result.CompareNoCase("ABORTED") == 0)
						str_data.Format("LP%1d - Aborted", lp_id);
					else
						str_data.Format("LP%1d - Completed", lp_id);

					sEXT_CH__FNC_MSG->Set__DATA(str_data);
				}

				// jglee - 2020.07.07
				{
					SCX__FILE_CTRL x_file_ctrl;
					CString ref_root;
	
					x_file_ctrl->DISABLE__TIME_LOG();
					
					CString dir_date_lot;
					xSCH_MATERIAL_CTRL->Get__DIR_JOB_LOG(lp_id, dir_date_lot);

					printf(" ** 2)  LP(%1d) <- [%s] \n", lp_id, dir_date_lot);

					ref_root  = sDIR_WFR_DATA;
					ref_root += dir_date_lot;

					// CJob Info ...
					CString job_info;
					CString bff_info;

					CString str_end_time = Macro__Get_Current_Date_Time();
					bff_info.Format("#TIME.END=%s\n", str_end_time);
					job_info += bff_info;

					bff_info.Format("#RESULT=%s\n", str_result);
					job_info += bff_info;

					//
					CString str_jobid = xCH__PORT_JOB_NAME[i]->Get__STRING();

					CString path_job;
					path_job.Format("%s\\%s.job_log", ref_root,str_jobid);		

					x_file_ctrl->Write__LOG_MSG(path_job, job_info);
				}
			}
		}

		// ...
	}
}
int  CObj_Opr__AUTO_MODE::Check__ALL_PORT_COMPLETE()
{
	int i;

	for(i=0; i<iLPx_UNIT_SIZE; i++)
	{
		if(xCH__PORT_CTRL[i]->Check__DATA("RUN") > 0)
		{
			return -1;
		}
	}
	return 1;
}

void CObj_Opr__AUTO_MODE::Seq__ABORT_ALL_PORT()
{
	int i;

	for(i=0; i<iLPx_UNIT_SIZE; i++)
	{
		if(xCH__PORT_CTRL[i]->Check__DATA("RUN") > 0)
		{
			xCH__PORT_CTRL[i]->Set__DATA("ABORTED");
		}

		if(xCH__PORT_STATUS[i]->Check__DATA(STR__BUSY) > 0)
		{					
			xCH__PORT_STATUS[i]->Set__DATA("ABORTED");
			xCH__PORT_UPLOAD_FILE_FLAG[i]->Set__DATA("");

			xSCH_MATERIAL_CTRL->Abort__JOB(i+1);
			continue;
		}
		if(xCH__PORT_STATUS[i]->Check__DATA(STR__RESERVE) > 0)
		{					
			xCH__PORT_STATUS[i]->Set__DATA("ABORTED");
			xCH__PORT_UPLOAD_FILE_FLAG[i]->Set__DATA("");
			continue;
		}
	}
}
void CObj_Opr__AUTO_MODE::Seq__CLEAR_ALL_PORT(CII_OBJECT__ALARM *p_alarm,const int alarm_box)
{
	int err_flag = -1;
	int i;

	for(i=0; i<iLPx_UNIT_SIZE; i++)
	{
		if(xCH__PORT_CTRL[i]->Check__DATA("RUN") < 0)
		{
			continue;
		}

		xCH__PORT_CTRL[i]->Set__DATA("ABORTED");

		if((xCH__PORT_STATUS[i]->Check__DATA(STR__BUSY)    > 0)
		|| (xCH__PORT_STATUS[i]->Check__DATA(STR__RESERVE) > 0))
		{					
			xCH__PORT_STATUS[i]->Set__DATA("WAITING");
			xCH__PORT_UPLOAD_FILE_FLAG[i]->Set__DATA("");
		}

		err_flag = 1;
	}

	xSCH_MATERIAL_CTRL->Clear__ALL_MATERIAL_INFO();

	if((err_flag > 0)&&(alarm_box > 0))
	{
		CString r_act;

		p_alarm->Popup__ALARM(ALID__SYSTEM_IN_MAINT_MODE,r_act);
	}
}


//-------------------------------------------------------------------------
int  CObj_Opr__AUTO_MODE::
Fnc__UPLOAD_JOB(CII_OBJECT__ALARM *p_alarm,
				const int ptn,
				const CString& dir_lp,
				const CString& lp_rcp)
{
	int lp_index = ptn - 1;

	if(lp_index <  0)				return -1;
	if(lp_index >= CFG_LP_LIMIT)	return -1;

	if(xCH__PORT_UPLOAD_FILE_FLAG[lp_index]->Check__DATA(STR__YES) < 0)
	{
		if(xLPx_JOB_FILE_CTRL[lp_index]->Upload__JOB_FILE(sDIR_PROCESS,dir_lp,lp_rcp) < 0)
		{
			return -1;
		}

		// ...
		{
			SCI__CTC__JOB_FILE_CTRL *p_job_file = xLPx_JOB_FILE_CTRL[lp_index].Get__PTR();

			if(p_job_file->Check__ROUTE_EDIT_TYPE() < 0)
			{
				int alm_id = ALID__JOB_START__ROUTE_RCP_TYPE_OLD;
				CString alm_msg;
				CString r_act;

				alm_msg.Format("\"%s\" Recipe의 Type을 확인 바랍니다 ! \n", lp_rcp);

				p_alarm->Check__ALARM(alm_id, r_act);
				p_alarm->Post__ALARM_With_MESSAGE(alm_id, alm_msg);

				return 0;
			}
		}

		if(xCH__CFG_SCH_REF->Check__DATA(STR__ROUTE) > 0)
		{
			if(xSCH_MATERIAL_CTRL->Get__MATERIAL_DB_SIZE() > 0)
			{
				SCI__CTC__JOB_FILE_CTRL *p_job_file = xLPx_JOB_FILE_CTRL[lp_index].Get__PTR();
				
				CUIntArray l_slotid;
				if(p_job_file->Get__SLOTID_OF_EDIT_TYPE(l_slotid) > 0)
				{
					int err_check = -1;
					CString err_msg = "";
					CString err_bff = "";

					int i_limit = l_slotid.GetSize();
					int i;

					for(i=0; i<i_limit; i++)
					{
						int slot_id = l_slotid[i];

						if(p_job_file->Check__ROUTE_EDIT_TYPE(slot_id) < 0)
						{
							err_check = 1;

							err_bff.Format("Route is not Edit-Type ! \n");
							err_msg += err_bff;
							continue;
						}

						// ALx ...
						{
							CStringArray l_id;
							CStringArray l_mode;
							CStringArray l_slot;

							p_job_file->Get__ALx_OF_EDIT_TYPE(slot_id, l_id,l_mode,l_slot);	

							int r_flag = Check__ALx_OF_SCH_DB(l_id,l_mode,l_slot);
							if(r_flag < 0)
							{
								err_check = 1;

								err_bff.Format("ALx is Error(%1d) ! \n", r_flag);
								err_msg += err_bff;
							}
						}
						// LLx_IN ...
						{
							CStringArray l_id;
							CStringArray l_mode;
							CStringArray l_slot;
							int r_add = -1;

							p_job_file->Get__LLx_IN_OF_EDIT_TYPE(slot_id, l_id,l_mode,l_slot);

							int r_flag = Check__LLx_IN_OF_SCH_DB(l_id,l_mode,l_slot);							
							if(r_flag < 0)
							{
								r_add = Check__ADD_LLx_IN_OF_SCH_DB(p_job_file, slot_id);
								if(r_add > 0)
								{
									r_flag = 1;
								}
							}
							if(r_flag < 0)
							{
								err_check = 1;

								err_bff.Format("LLx_IN is Error(%1d) ! \n", r_flag);
								err_msg += err_bff;

								err_bff.Format("  * r_add <- (%1d) \n", r_add);
								err_msg += err_bff;
							}
						}
						// LLx_OUT ...
						{
							CStringArray l_id;
							CStringArray l_mode;
							CStringArray l_slot;
							int r_add = -1;

							p_job_file->Get__LLx_OUT_OF_EDIT_TYPE(slot_id, l_id,l_mode,l_slot);

							int r_flag = Check__LLx_OUT_OF_SCH_DB(l_id,l_mode,l_slot);
							if(r_flag < 0)
							{
								r_add = Check__ADD_LLx_OUT_OF_SCH_DB(p_job_file, slot_id);
								if(r_add > 0)
								{
									r_flag = 1;
								}
							}
							if(r_flag < 0)
							{
								err_check = 1;

								err_bff.Format("LLx_OUT is Error(%1d) ! \n", r_flag);
								err_msg += err_bff;

								err_bff.Format("  * r_add <- (%1d) \n", r_add);
								err_msg += err_bff;
							}
						}
						// STx ...
						{
							CStringArray l_id;
							CStringArray l_mode;
							CStringArray l_slot;
							CStringArray l_sec;

							p_job_file->Get__STx_OF_EDIT_TYPE(slot_id, l_id,l_mode,l_slot,l_sec);

							int r_flag = Check__STx_OF_SCH_DB(l_id,l_mode,l_slot,l_sec);
							if(r_flag < 0)
							{
								err_check = 1;

								err_bff.Format("STx is Error(%1d) ! \n", r_flag);
								err_msg += err_bff;
							}
						}

						if(err_check > 0)
						{
							break;
						}
					}	

					if(err_check > 0)
					{
						int alm_id = ALID__JOB_START__ROUTE_RCP_DIFFRENT;
						CString r_act;

						p_alarm->Check__ALARM(alm_id, r_act);
						p_alarm->Post__ALARM_With_MESSAGE(alm_id, err_msg);

						return DEF__NEXT_CHECK;
					}
				}
				else
				{
					int alm_id = ALID__JOB_START__ROUTE_RCP_NO_SLOT_LIST;
					CString r_act;

					CString err_msg = "No Slot List \n";

					p_alarm->Check__ALARM(alm_id, r_act);
					p_alarm->Post__ALARM_With_MESSAGE(alm_id, err_msg);

					return -1;
				}

				// ...
			}
		}

		xCH__PORT_UPLOAD_FILE_FLAG[lp_index]->Set__DATA(STR__YES);
	}

	// ...
	int flag__concurrent_mode = -1;

	if(flag__concurrent_mode < 0)
	{
		if(xCH_CFG__SCH_TEST_PORT_CURRENT_APPLY->Check__DATA(STR__ENABLE) > 0)
		{
			if(xCH__LPx__CFG_PROCESS_TYPE[lp_index]->Check__DATA(STR__TEST) > 0)
			{
				flag__concurrent_mode = 1;
			}
		}
	}
	
	if(flag__concurrent_mode < 0)
	{
		if(xEXT_CH__SCH_CONCURRENT_ALWAYS_APPLY->Check__DATA(STR__ENABLE) > 0)
		{
			flag__concurrent_mode = 1;
		}
		else if(xEXT_CH__SCH_CONCURRENT_ALWAYS_APPLY->Check__DATA(STR__PPID_CHECK) > 0)
		{
			CStringArray l_file_pmx;		

			int path_size = xLPx_JOB_FILE_CTRL[lp_index]->Get__PRC_PATH_SIZE();
			if(path_size > 1)
			{
				xLPx_JOB_FILE_CTRL[lp_index]->Get__PRC_MODULE(l_file_pmx);
			}
			else
			{
				xLPx_JOB_FILE_CTRL[lp_index]->Get__PRC_MODULE_OF_FIRST_PATH(l_file_pmx);
			}

			// ...
			CStringArray l_sch_pmx;
			CUIntArray l_job_ptn;
			int ptn_id;
			
			int limit;
			int i;
			
			xSCH_MATERIAL_CTRL->Get__JOB_PORT_LIST(l_job_ptn);
			limit = l_job_ptn.GetSize();
			
			for(i=0; i<limit; i++)
			{
				ptn_id = l_job_ptn[i];
				
				if(ptn_id < 1)
				{
					continue;
				}
				
				/*
				if(xCH__LPx__CFG_PROCESS_TYPE[ptn_id-1]->Check__DATA(STR__TEST) > 0)
				{
					continue;
				}
				*/
				
				// ...
				{
					CStringArray l_lp_pm;
					xSCH_MATERIAL_CTRL->Get__ALL_PRC_MODULE_In_LPx(ptn_id,l_lp_pm);

					Macro__Load_StringArray(l_sch_pmx,l_lp_pm);
				}
			}

			/*
			// ...
			{
				CString log_msg;
				CString log_bff;

				log_msg = "Fnc__UPLOAD_JOB() ... \n";

				log_bff.Format("  * %s <- %s \n",
								xEXT_CH__SCH_CONCURRENT_ALWAYS_APPLY->Get__CHANNEL_NAME(),
								xEXT_CH__SCH_CONCURRENT_ALWAYS_APPLY->Get__STRING());
				log_msg += log_bff;

				// ...
				{
					int k_limit = l_file_pmx.GetSize();

					log_bff.Format(" * file_pmc list (%1d) \n", k_limit);
					log_msg += log_bff;

					for(int k=0; k<k_limit; k++)
					{
						CString str__pmc_name = l_file_pmx[k];

						log_bff.Format("   %1d) %s \n", k, str__pmc_name);
						log_msg += log_bff;
					}					
				}
				// ...
				{
					int k_limit = l_sch_pmx.GetSize();

					log_bff.Format(" * sch_pmc list (%1d) \n", k_limit);
					log_msg += log_bff;

					for(int k=0; k<k_limit; k++)
					{
						CString str__pmc_name = l_sch_pmx[k];

						log_bff.Format("   %1d) %s \n", k, str__pmc_name);
						log_msg += log_bff;
					}					
				}

				printf(log_msg);
			}
			*/

			if(l_sch_pmx.GetSize() > 0)
			{
				int ppid_check = -1;

				limit = l_file_pmx.GetSize();
			
				for(i=0; i<limit; i++)
				{
					CString file_pmx = l_file_pmx[i];

					if(Macro__Check_Item(file_pmx, l_sch_pmx) > 0)		
					{
						if(path_size > 1)
						{
							// printf("PPID_CHECK - Error (1) \n");
							return DEF__NEXT_CHECK;	
						}
						continue;
					}

					ppid_check = 1;
					break;
				}

				if(ppid_check > 0)
				{
					flag__concurrent_mode = 1;
				}
				else
				{
					// printf("PPID_CHECK - Error (2) \n");
					return DEF__NEXT_CHECK;				
				}
			}
			else
			{
				flag__concurrent_mode = 1;
			}

			/*
			printf("PPID_CHECK - OK \n");
			printf(" * flag__concurrent_mode <- %1d \n", flag__concurrent_mode);
			*/
		}
	}
	
	if(flag__concurrent_mode < 0)
	{
		CString log_msg;
		CString log_bff;

		if(xSCH_MATERIAL_CTRL->Check__SCH_MODE__CONCURRENT() > 0)
		{
			CStringArray l_file_pmx;
			xLPx_JOB_FILE_CTRL[lp_index]->Get__PRC_MODULE(l_file_pmx);

			// ...
			CStringArray l_sch_pmx;
			CUIntArray l_job_ptn;
			
			xSCH_MATERIAL_CTRL->Get__JOB_PORT_LIST(l_job_ptn);

			int limit = l_job_ptn.GetSize();
			for(int i=0; i<limit; i++)
			{
				int ptn_id = l_job_ptn[i];
				int ptn_index = ptn_id - 1;

				if(ptn_id < 1)
				{
					continue;
				}

				if(xCH__LPx__CFG_PROCESS_TYPE[ptn_index]->Check__DATA(STR__TEST) > 0)
				{
					continue;
				}

				// ...
				l_sch_pmx.RemoveAll();
				xSCH_MATERIAL_CTRL->Get__ALL_PRC_MODULE_In_LPx(ptn_id, l_sch_pmx);

				int k_limit = l_file_pmx.GetSize();				
				for(int k=0; k<k_limit; k++)
				{
					CString file_pmx = l_file_pmx[k];

					if(Macro__Check_Item(file_pmx, l_sch_pmx) > 0)		
					{
						return DEF__NEXT_CHECK;
					}
				}
			}
		}
		else if(xSCH_MATERIAL_CTRL->Check__SCH_MODE__PIPELINE() > 0)
		{
			CUIntArray l_job_ptn;
			xSCH_MATERIAL_CTRL->Get__JOB_PORT_LIST(l_job_ptn);

			int limit = l_job_ptn.GetSize();
			for(int i=0; i<limit; i++)
			{
				int ptn_id = l_job_ptn[i];
				int ptn_index = ptn_id - 1;

				if(ptn_id < 1)
				{
					continue;
				}

				if(xCH__LPx__CFG_PROCESS_TYPE[ptn_index]->Check__DATA(STR__TEST) > 0)
				{
					continue;
				}

				if(xSCH_MATERIAL_CTRL->Check__MATERIAL_TO_PROCESS_IN_LPx(ptn_id) > 0)
				{
					return DEF__NEXT_CHECK;
				}
			}
		}
		else if(xSCH_MATERIAL_CTRL->Check__SCH_MODE__SEQUENTIAL() > 0)
		{
			if(xSCH_MATERIAL_CTRL->Check__MATERIAL_TO_PROCESS() > 0)
			{
				return DEF__NEXT_CHECK;
			}
		}

		// ...
	}

	// STx Check ...
	SCI__CTC__JOB_FILE_CTRL *p_job_file = xLPx_JOB_FILE_CTRL[lp_index].Get__PTR();;

	bool active_log = true;
	// bool active_log = false;
	
	if(active_log)
	{
		printf("STx Check ... \n");

		printf(" * CFG_SCH_REF[ROUTE] <- %1d \n", xCH__CFG_SCH_REF->Check__DATA(STR__ROUTE));
		printf(" * JOB_FILE[ROUTE] <- %1d \n", 	p_job_file->Check__ROUTE_EDIT_TYPE());
		printf(" * CFG_STx_APPLY_MODE[STR__ROUTE] <- %1d \n", xEXT_CH__CFG_DB_STx_APPLY_MODE->Check__DATA(STR__ROUTE));
	}

	if((xCH__CFG_SCH_REF->Check__DATA(STR__ROUTE) > 0)
	&& (p_job_file->Check__ROUTE_EDIT_TYPE() > 0)
	&& (xEXT_CH__CFG_DB_STx_APPLY_MODE->Check__DATA(STR__ROUTE) > 0))
	{
		int active__sx_check = 1;

		CString ch_data = xEXT_CH__CUR_STx_SIZE->Get__STRING();
		int cur__st_size = atoi(ch_data);

		if(active_log)		printf(" * EDIT_TYPE !!! \n");
		if(active_log)		printf(" * cur__st_size <- %1d \n", cur__st_size);

		// ...
		CUIntArray l_slotid;
		p_job_file->Get__SLOTID_OF_EDIT_TYPE(l_slotid);

		if(l_slotid.GetSize() > 0)
		{
			CStringArray l_id;
			CStringArray l_mode;
			CStringArray l_slot;
			CStringArray l_sec;

			int slot_id = l_slotid[0];
			p_job_file->Get__STx_OF_EDIT_TYPE(slot_id, l_id,l_mode,l_slot,l_sec);

			if(active_log)
			{
				printf(" * Job_File : STx ... \n");

				int i_limit = l_id.GetSize();
				int i;

				for(i=0; i<i_limit; i++)
				{
					int st_id = atoi(l_id[i]);
					printf(" ** %1d) ST%1d [%s][%s][%s] ... \n", i+1, st_id,l_mode[i],l_slot[i],l_sec[i]);
				}
			}

			if(l_id.GetSize() < 1)
			{
				active__sx_check = -1;
			}

			if(active__sx_check > 0)
			{
				if(Check__STx_To_Use(l_id) < 0)
				{
					int alarm_id = ALID__JOB_START__BUFFER_NOT_USE;
					CString alm_msg;
					CString alm_bff;
					CString r_act;

					alm_msg.Format("LP%1d can not use buffer. \n", ptn);
					alm_bff.Format("So, LP's job will be canceled. \n", ptn);
					alm_msg += alm_bff;

					p_alarm->Check__ALARM(alarm_id,r_act);
					p_alarm->Post__ALARM_With_MESSAGE(alarm_id,alm_msg);
					return 0;				
				}
			}
		}

		if(active__sx_check > 0)
		{
			if(cur__st_size == 1)
			{
				if(dEXT_CH__CFG_DB_STx_LOTID_CHECK->Check__DATA(STR__ENABLE) > 0)
				{
					if(xEXT_CH__CFG_DB_STx_JOB_WAIT_MODULE->Check__DATA(STR__LPx) > 0)
					{
						if(xSCH_MATERIAL_CTRL->Check__MATERIAL_TO_USE_BUFFERx() > 0)
						{
							if(active_log)		printf(" * Next_Check (1) \n");
							return DEF__NEXT_CHECK;				
						}

						if(active_log)		printf(" * Pass (1) \n");
					}
					else
					{
						if(xSCH_MATERIAL_CTRL->Check__MATERIAL_IN_LPx_TO_USE_BUFFERx() > 0)
						{
							if(active_log)		printf(" * Next_Check (2) \n");
							return DEF__NEXT_CHECK;
						}

						if(active_log)		printf(" * Pass (2) \n");
					}
				}
			}
		}
	}
	else
	{
		CString var_data;
		int buffer_flag = -1;

		xEXT_CH__CUR_STx_SIZE->Get__DATA(var_data);
		int cur_bff_size = atoi(var_data);

		if(active_log)		printf(" * CFG_TYPE !!! \n");
		if(active_log)		printf(" * cur__st_size <- %1d \n", cur_bff_size);

		if(xEXT_CH__SCH_DB_STx_APPLY_MODE->Check__DATA(STR__ENABLE) > 0)
		{
			if(cur_bff_size > 1)
			{
				buffer_flag = 2;			
			}
			else if(cur_bff_size > 0)
			{
				buffer_flag = 1;
			}
			else
			{
				buffer_flag = 0;
			}
		}
		else if(xEXT_CH__SCH_DB_STx_APPLY_MODE->Check__DATA(STR__ROUTE) > 0)
		{
			CStringArray l_slot;
			CStringArray l_bff1;
			CStringArray l_bff2;

			xLPx_JOB_FILE_CTRL[lp_index]->Get__ALL_BUFFERx_INFO(l_slot,l_bff1,l_bff2);

			// ...
			int check_flag = -1;

			if(check_flag < 0)
			{
				if(Macro__Check_Item(STR__YES,l_bff1) > 0)
				{
					check_flag = 1;
				}
			}
			if(check_flag < 0)
			{
				if(Macro__Check_Item(STR__YES,l_bff2) > 0)
				{
					check_flag = 1;
				}
			}

			if(check_flag > 0)
			{
				if(cur_bff_size > 1)
				{
					buffer_flag = 2;			
				}
				else if(cur_bff_size > 0) 
				{
					buffer_flag = 1;
				}
				else
				{
					buffer_flag = 0;
				}
			}
		}

		if(buffer_flag <= 0)
		{
			// ...
			{
				int alarm_id = ALID__JOB_START__BUFFER_NOT_USE;
				CString alm_msg;
				CString alm_bff;
				CString r_act;

				alm_msg.Format("LP%1d can not use buffer. \n", ptn);
				alm_bff.Format("So, LP's job will be canceled. \n", ptn);
				alm_msg += alm_bff;

				p_alarm->Check__ALARM(alarm_id,r_act);
				p_alarm->Post__ALARM_With_MESSAGE(alarm_id,alm_msg);
			}
			return 0;				
		}
		else if(buffer_flag == 1)
		{
			if(dEXT_CH__CFG_DB_STx_LOTID_CHECK->Check__DATA(STR__ENABLE) > 0)
			{
				if(xEXT_CH__CFG_DB_STx_JOB_WAIT_MODULE->Check__DATA(STR__LPx) > 0)
				{
					if(xSCH_MATERIAL_CTRL->Check__MATERIAL_TO_USE_BUFFERx() > 0)
					{
						return DEF__NEXT_CHECK;				
					}
				}
				else
				{
					if(xSCH_MATERIAL_CTRL->Check__MATERIAL_IN_LPx_TO_USE_BUFFERx() > 0)
					{
						return DEF__NEXT_CHECK;
					}
				}
			}
		}
	}

	// ...
	int pause_flag = xSCH_MATERIAL_CTRL->Check__SYSTEM_PAUSE();
	xSCH_MATERIAL_CTRL->Set__SYSTEM_PAUSE();
	xSCH_MATERIAL_CTRL->Clear__LPx_MATERIAL_INFO(ptn);

	if(xCH_CFG__LPx_PICK_MODE->Check__DATA("TOP_DOWN") > 0)
	{
		xLPx_JOB_FILE_CTRL[lp_index]->Set__TOP_DOWN();
	}

	// ...
	CStringArray l_pre_job_pmc;
	xSCH_MATERIAL_CTRL->Get__ALL_PRC_MODULE(l_pre_job_pmc);

	// ...
	int active__sch_db = -1;
	int active__job_start_ready = 1;

	if(xSCH_MATERIAL_CTRL->Get__MATERIAL_DB_SIZE() < 1)
	{
		active__sch_db = 1;
	}
	else
	{
		printf("CObj_Opr__AUTO_MODE::Fnc__UPLOAD_JOB() ... \n");
		
		// ...
		{
			printf(" * List New_PMC ... \n");

			CStringArray l_pm_new;
			p_job_file->Get__PRC_MODULE(l_pm_new);

			int k_limit = l_pm_new.GetSize();
			int k;

			for(k=0; k<k_limit; k++)
			{
				CString pm_name = l_pm_new[k];

				printf("  * %1d) [%s] \n", k+1,pm_name);

				if(xSCH_MATERIAL_CTRL->Check__PMC_TO_PROCESS_OF_PRE_DB("", pm_name) < 0)
				{
					continue;
				}

				printf("  **  active__job_start_ready <- [-1] \n");

				active__job_start_ready = -1;
				break;
			}
		}
	}

	// ...
	{
		CString str__cstid;

		xCH__CSTID[lp_index]->Get__DATA(str__cstid);
		p_job_file->Copy__TO_MATERIAL_WITH_CSTID(lp_rcp,str__cstid);
	}

	// SCH_DB Check ...
	{
		CString log_msg;
		CString log_bff;

		CUIntArray l_portid;
		CUIntArray l_slotid;

		xSCH_MATERIAL_CTRL->Get__ALL_LPx_INFO_FROM_DB(l_portid,l_slotid);

		log_msg = "Fnc__UPLOAD_JOB() ... \n";
		log_bff.Format(" * ptn <- %1d \n", ptn);
		log_msg += log_bff;
		log_bff.Format(" * dir_ip <- %s \n", dir_lp);
		log_msg += log_bff;
		log_bff.Format(" * lp_rcp <- %s \n", lp_rcp);
		log_msg += log_bff;

		int i_limit = l_portid.GetSize();
		int i;

		for(i=0; i<i_limit; i++)
		{
			int port_id = (int) l_portid[i];
			int slot_id = (int) l_slotid[i];

			log_bff.Format("%1d)  P%1d:%1d \n", i+1,port_id,slot_id);
			log_msg += log_bff;

			int st_count = xSCH_MATERIAL_CTRL->Get__STEP_SIZE_OF_MATERIAL(port_id,slot_id);
			int st_i;

			for(st_i=0; st_i<st_count; st_i++)
			{
				CStringArray l_pre_module;
				CStringArray l_pre_recipe;
				CStringArray l_main_module;
				CStringArray l_main_recipe;
				CStringArray l_post_module;
				CStringArray l_post_recipe;

				int flag = 
					xSCH_MATERIAL_CTRL->Get__STEPx_PROCESS_INFO_OF_MATERIAL(port_id,
																			slot_id,	
																			st_i,
																			l_pre_module,
																			l_pre_recipe,
																			l_main_module,
																			l_main_recipe,
																			l_post_module,
																			l_post_recipe);

				if(flag < 0)		continue;

				log_bff.Format(" * Step_Index (%1d) ...\n", st_i);
				log_msg += log_bff;

				// Pre_ ...
				{
					log_msg += "  ** Pre_ Info ...\n";

					int k_limit = l_pre_module.GetSize();
					int k;

					for(k=0; k<k_limit; k++)
					{
						log_bff.Format("  %s[%s]", l_pre_module[k],l_pre_recipe[k]);
						log_msg += log_bff;
					}
					log_msg += "\n";
				}
				// Main ...
				{
					log_msg += "  ** Main Info ...\n";

					int k_limit = l_main_module.GetSize();
					int k;

					for(k=0; k<k_limit; k++)
					{
						log_bff.Format("  %s[%s]", l_main_module[k],l_main_recipe[k]);
						log_msg += log_bff;
					}
					log_msg += "\n";
				}
				// Post ...
				{
					log_msg += "  ** Post Info ...\n";

					int k_limit = l_post_module.GetSize();
					int k;

					for(k=0; k<k_limit; k++)
					{
						log_bff.Format("  %s[%s]", l_post_module[k],l_post_recipe[k]);
						log_msg += log_bff;
					}
					log_msg += "\n";
				}
			}

			// PROC_LOOPx ...
			{
				log_msg += " * PROC_LOOPx ... \n";

				//
				CString loop_id;
				int loop_count;

				xSCH_MATERIAL_CTRL->Get__PROC_LOOPx_OF_EDIT_TYPE(port_id,slot_id, loop_id,loop_count) ;
	
				log_bff.Format("  ** Loop_ID    <- %s \n", loop_id);
				log_msg += log_bff;
				log_bff.Format("  ** Loop_Count <- %1d \n", loop_count);
				log_msg += log_bff;
			}
		}

		// jglee
		xLOG_CTRL->WRITE__LOG(log_msg);
	}

	if(active__sch_db > 0)
	{
		int lp_id = lp_index + 1;

		// CFG_DB -> SCH_DB ...
		{
			Update__CFG_DB_TO_SCH_DB();
			Update__ROUTE_DB_TO_SCH_DB(lp_id);
		}

		if(dEXT_CH__CFG_JOB_START_READY_CTRL_MODE->Check__DATA("JOB.CHANGE") > 0)
		{
			// SCH__PMx_JOB_START_READY(lp_id);
		}
		else
		{
			SCH__PMx_JOB_START_READY();
		}
	}
	else
	{
		// SCH_DB : ADD_LLx ...
		SCI__CTC__JOB_FILE_CTRL *p_job_file = xLPx_JOB_FILE_CTRL[lp_index].Get__PTR();;

		CUIntArray l_slotid;
		p_job_file->Get__SLOTID_OF_EDIT_TYPE(l_slotid);

		if(l_slotid.GetSize() > 0)
		{
			int slot_id = l_slotid[0];

			Update__ADD_LLx_DB_TO_SCH_DB(p_job_file, slot_id);
		}

		if(active__job_start_ready > 0)
		{
			int lp_id = lp_index + 1;

			if(dEXT_CH__CFG_JOB_START_READY_CTRL_MODE->Check__DATA("JOB.CHANGE") > 0)
			{
				// SCH__PMx_JOB_START_READY(lp_id);
			}
		}
	}

	// ...
	{
		CStringArray l_slot;
		
		xSCH_MATERIAL_CTRL->Get__LPx_SLOT_LIST(ptn,l_slot);
		pPORT__OBJ_CTRL[lp_index]->Call__OBJ_MODE("CHECK_SLOT",l_slot);

		if(l_slot.GetSize() > 0)
		{
			CString err_msg;
			CString bff;
			
			int limit = l_slot.GetSize();
			int i;

			err_msg.Format("Please, check the following slots in LP%1d.\n",ptn);
			err_msg += "   ";

			for(i=0;i<limit;i++)
			{
				bff.Format("[%s] ",l_slot[i]);
				err_msg += bff;

				if(i % 5 == 4)		err_msg += "\n";
			}

			// ...
			int alm_id = ALID__JOB_START__NO_SLOT_MAP_ERROR;
			CString r_act;

			p_alarm->Check__ALARM(alm_id, r_act);
			p_alarm->Post__ALARM_With_MESSAGE(alm_id, err_msg);

			// ...
			{
				CString cjobid;
				xCH__PORT_CJOBID[lp_index]->Get__DATA(cjobid);

				if(cjobid.GetLength() > 0)
				{
					mFA_Link.CJOB__ABORT(cjobid);
				}
			}

			xSCH_MATERIAL_CTRL->Clear__LPx_MATERIAL_INFO(ptn);
			if(pause_flag < 0)
			{
				xSCH_MATERIAL_CTRL->Set__SYSTEM_START();
			}
			return 0;
		}
	}

	if(xSCH_MATERIAL_CTRL->Check__LPx_COMPLETE(ptn) > 0)
	{
		if(pause_flag < 0)
		{
			xSCH_MATERIAL_CTRL->Set__SYSTEM_START();
		}

		// ...
		{
			int alarm_id = ALID__JOB_START__NO_SLOT_ERROR;

			CString err_msg;
			CString bff;
			CString r_act;

			err_msg  = "#MSG_TITLE\n";
			err_msg += "JOB START ERROR : SLOT ERROR\n";
			err_msg += "##MSG\n";

			bff.Format("There is no slot to be selected for processing in LP%1d port recipe.",ptn);
			err_msg += bff;

			p_alarm->Check__ALARM(alarm_id,r_act);
			p_alarm->Post__ALARM_With_MESSAGE(alarm_id,err_msg);
		}
		return 0;
	}

	if(Check__CYCLE_MODE() > 0)
	{
		IDS__SCH_MATERIAL_INFO ds_info;
		CString lotid;

		xSCH_MATERIAL_CTRL->Get__PORT_FIRST_SLOT_INFO(ptn,ds_info);
		lotid.Format("%s_%1d",ds_info.sLOT_ID,iCYCLE_LOT_COUNT++);

		xSCH_MATERIAL_CTRL->Change__LOTID(ptn,lotid);
	}

	if(xSCH_MATERIAL_CTRL->Check__VIS_INFO__CFG_MODE() > 0)
	{
		CString rcp_name;

		xSCH_MATERIAL_CTRL->Get__VIS_INFO__CFG_RECIPE_NAME(rcp_name);
		xSCH_MATERIAL_CTRL->Change__VISION_RECIPE(ptn,rcp_name);
	}
	if(xSCH_MATERIAL_CTRL->Check__VIS_INFO__CFG_VISIT() > 0)
	{
		CUIntArray l_slot_id;
		CUIntArray l_slot_sts;

		xSCH_MATERIAL_CTRL->Get__VIS_INFO__CFG_VISIT_SLOT(l_slot_id,l_slot_sts);
		xSCH_MATERIAL_CTRL->Change__VISION_VISIT_SLOT(ptn,l_slot_id,l_slot_sts);
	}

	xSCH_MATERIAL_CTRL->Set__JOB_PORT(ptn);
	//xSCH_MATERIAL_CTRL->Check__RCP_DB();

	// ...
	bool active__dummy_port = false;

	if((xCH__LPx__CFG_PROCESS_TYPE[lp_index]->Check__DATA(STR__NORMAL) > 0)
	&& (dCH_CFG__DUMMY_PORT_USE_MODE->Check__DATA(STR__ENABLE) > 0))
	{
		active__dummy_port = true;
	}

	if(active__dummy_port)
	{
		if(dCH_CFG__DUMMY_PORT_CTRL_TYPE->Check__DATA(STR__IDLE) > 0)
		{
			CStringArray l_pmc;
			xSCH_MATERIAL_CTRL->Get__ALL_PRC_MODULE(l_pmc);

			// ...
			CStringArray l_dummy_pmc;

			int limit = l_pmc.GetSize();
			if(limit > iPMx_UNIT_SIZE)
			{
				limit = iPMx_UNIT_SIZE;
			}

			// ...
			bool active__check_pmx = false;

			if(dCH_CFG__DUMMY_PORT_PMC_IDLE_MODE->Check__DATA(STR__PMx) > 0)
			{
				active__check_pmx = true;
			}

			// ...
			CString var_data;

			aCH_CFG__DUMMY_PORT_PMC_IDLE_TIME->Get__DATA(var_data);
			int ref_idle_sec = atoi(var_data);	

			for(int i=0;i<limit;i++)
			{
				CString cur_pmc = l_pmc[i];
				if(Macro__Check_Item(cur_pmc,l_pre_job_pmc) > 0)
				{
					continue;
				}

				int pm_i = Macro__Get_PM_INDEX(cur_pmc);
				if(pm_i < 0)
				{
					continue;
				}

				if(active__check_pmx)
				{
					aCH_CFG__DUMMY_PORT_PMx_IDLE_TIME[pm_i]->Get__DATA(var_data);
					ref_idle_sec = atoi(var_data);	
				}

				xEXT_CH__PMx_CUR_IDLE_TIME[pm_i]->Get__DATA(var_data);
				int cur_idle_sec = atoi(var_data);
				
				if(cur_idle_sec > ref_idle_sec)
				{
					if(Macro__Check_Item(cur_pmc,l_dummy_pmc) < 0)
					{
						l_dummy_pmc.Add(cur_pmc);
					}
				}
			}

			if(l_dummy_pmc.GetSize() > 0)
			{
				Fnc__DUMMY_PROC_JOB(p_alarm, ptn,l_dummy_pmc);
			}
		}
		else
		{
			CStringArray l_pmc_prc;
			xSCH_MATERIAL_CTRL->Get__ALL_PRC_MODULE_In_LPx(ptn, l_pmc_prc);

			printf("===================================================\n");
			printf(" * Dummy_Port - Lot_Control \n");

			int k_limit = l_pmc_prc.GetSize();
			for(int k=0; k<k_limit; k++)
			{
				printf(" ** %1d) [%s] \n", k+1,l_pmc_prc[k]);
			}
			printf("===================================================\n\n");

			Fnc__DUMMY_PROC_JOB(p_alarm, ptn,l_pmc_prc);
		}
	}

	if(pause_flag < 0)
	{
		xSCH_MATERIAL_CTRL->Set__SYSTEM_START();
	}

	mFA_Link.LP__START(ptn);
	return 1;
}

int CObj_Opr__AUTO_MODE
::Check__STx_To_Use(const CStringArray& l_id)
{
	int i_limit = l_id.GetSize();
	int i;

	for(i=0; i<i_limit; i++)
	{
		int st_i = atoi(l_id[i]) - 1;

		if(st_i <  0)						continue;
		if(st_i >= iBUFF_UNIT_SIZE)			continue;

		if(xEXT_CH__CFG_DB_STx_USE_FLAG[st_i]->Check__DATA(STR__ENABLE) < 0)
			continue;
		
		return 1;
	}		

	return -1;
}


// ...
#define DEF__LPx_X			"LP"

#define DEF__ATM_RB_A		"ATM_RB1-A"
#define DEF__ATM_RB_B		"ATM_RB1-B"

#define DEF__AL1_X			"AL1"

#define DEF__BUFFx_X		"BUFF"

#define DEF__LBA_X			"LBA"
#define DEF__LBB_X			"LBB"

#define DEF__VAC_RB_A		"VAC_RB1-A"
#define DEF__VAC_RB_B		"VAC_RB1-B"

#define DEF__PMx_X			"PM"


void CObj_Opr__AUTO_MODE
::Fnc__Check_Wafer_Duplication_Of_UI(CStringArray& l_ui_pos,
								     CStringArray& l_ui_wfr_status,
								     CStringArray& l_ui_wfr_title, 		
								     CStringArray& l_err_pos,
								     CStringArray& l_err_wfr_status,
								     CStringArray& l_err_wfr_title)
{	
	// ...
	{
		l_err_pos.RemoveAll();
		l_err_wfr_status.RemoveAll();
		l_err_wfr_title.RemoveAll();
	}

	// ...
	int wfr_ptnid;
	int wfr_stnid;

	int cur_ptnid;
	int cur_stnid;

	// ...
	CString wfr_pos;
	CString wfr_status;
	CString wfr_title;
	CString cur_pos;
	CString cur_status;
	CString cur_title;

	int i_limit = l_ui_pos.GetSize();
	int i;
	int k;

	for(i=0; i<i_limit; i++)
	{
		wfr_pos    = l_ui_pos[i];
		wfr_status = l_ui_wfr_status[i];
		wfr_title  = l_ui_wfr_title[i];		

		Macro__Analyze_Title_Info(wfr_title, wfr_ptnid,wfr_stnid);

		if(wfr_status.CompareNoCase(STR__NONE) == 0)
		{
			continue;
		}

		for(k=i+1; k<i_limit; k++)
		{
			cur_pos    = l_ui_pos[k];
			cur_status = l_ui_wfr_status[k];
			cur_title  = l_ui_wfr_title[k];			

			if(cur_status.CompareNoCase(STR__NONE) == 0)
			{
				continue;
			}
			if(cur_title.CompareNoCase(wfr_title) != 0)
			{
				Macro__Analyze_Title_Info(cur_title, cur_ptnid,cur_stnid);

				if(wfr_ptnid != cur_ptnid)		continue;
				if(wfr_stnid != cur_stnid)		continue;
			}

			// ui check ...
			{
				int check_flag = -1;

				int t_limit = l_err_pos.GetSize();
				int t;

				for(t=0; t<t_limit; t++)
				{
					if(wfr_pos.CompareNoCase(l_err_pos[t]) != 0)				continue;
					if(wfr_title.CompareNoCase(l_err_wfr_title[t]) != 0)		continue;

					check_flag = 1;
					break;
				}

				if(check_flag < 0)
				{
					l_err_pos.Add(wfr_pos);
					l_err_wfr_status.Add(wfr_status);
					l_err_wfr_title.Add(wfr_title);
				}
			}
		}
	}

	// ...
}
int  CObj_Opr__AUTO_MODE
::Fnc__Check_UI_Wafer_And_SCH_Wafer(const CStringArray& l_ui_pos,
								    const CStringArray& l_ui_wfr_status,
								    const CStringArray& l_ui_wfr_title, 		
								    const CStringArray& l_sch_pos,
								    const CStringArray& l_sch_wfr_status,
								    const CStringArray& l_sch_wfr_title,
								    CStringArray& l_err_pos,
								    CStringArray& l_err_wfr_status,
								    CStringArray& l_err_wfr_title)
{
	CString sch_pos;
	CString wfr_status;
	CString wfr_title;

	// ...
	{
		l_err_pos.RemoveAll();
		l_err_wfr_status.RemoveAll();
		l_err_wfr_title.RemoveAll();
	}

	// ...
	int i_limit = l_sch_pos.GetSize();
	int i;

	for(i=0; i<i_limit; i++)
	{
		sch_pos    = l_sch_pos[i];
		wfr_status = l_sch_wfr_status[i];
		wfr_title  = l_sch_wfr_title[i];

		// ...
		int err_flag = 1;

		int k_limit = l_ui_pos.GetSize();
		int k;

		for(k=0; k<k_limit; k++)
		{
			if(sch_pos.CompareNoCase(l_ui_pos[k]) != 0)
			{
				continue;
			}

			err_flag = -1;
			break;
		}

		if(err_flag > 0)
		{
			l_err_pos.Add(sch_pos);
			l_err_wfr_status.Add(wfr_status);
			l_err_wfr_title.Add(wfr_title);
		}
	}

	if(l_err_pos.GetSize() > 0)
	{
		return -1;
	}
	return 1;
}

void CObj_Opr__AUTO_MODE
::Fnc__Get_All_Wafer_Info_Of_UI(CStringArray& l_ui_name,
							    CStringArray& l_ui_wafer_status,
							    CStringArray& l_ui_wafer_title)
{
	CString log_msg;
	CString log_bff;

	CString cur_pos;
	CString wfr_status;
	CString wfr_title;
	CStringArray l_cur_pos;
	CStringArray l_wfr_status;
	CStringArray l_wfr_title;
	CString ch_data;

	int limit = 11;
	int i;
	int k_limit;
	int k;

	// ...
	l_ui_name.RemoveAll();
	l_ui_wafer_status.RemoveAll();
	l_ui_wafer_title.RemoveAll();

	// ...
	log_msg  = "\n";
	log_msg += "Wafer Info ...";
	log_msg += "\n";

	for(i=0; i<limit; i++)
	{
		l_cur_pos.RemoveAll();
		l_wfr_status.RemoveAll();
		l_wfr_title.RemoveAll();

		if(i == 0)			// ATM_RB1-A ...
		{
			cur_pos = DEF__ATM_RB_A;

			dEXT_CH__ATM_RB_A__MATERIAL_STATUS_ARM->Get__DATA(wfr_status);
			sEXT_CH__ATM_RB_A__MATERIAL_TITLE_ARM->Get__DATA(wfr_title);

			l_cur_pos.Add(cur_pos);
			l_wfr_status.Add(wfr_status);
			l_wfr_title.Add(wfr_title);
		}
		else if(i == 1)		// AL1-1 ...
		{
			k_limit = iAL1_SLOT_SIZE;

			for(k=0; k<k_limit; k++)
			{
				cur_pos.Format("%s-%1d", DEF__AL1_X,k+1);

				dEXT_CH__AL1_X__MATERIAL_STATUS_SLOT[k]->Get__DATA(wfr_status);
				sEXT_CH__AL1_X__MATERIAL_TITLE_SLOT[k]->Get__DATA(wfr_title);

				l_cur_pos.Add(cur_pos);
				l_wfr_status.Add(wfr_status);
				l_wfr_title.Add(wfr_title);
			}
		}
		else if(i == 4)		// VAC_RB1-A ...
		{
			cur_pos = DEF__VAC_RB_A;

			dEXT_CH__VAC_RB_A__MATERIAL_STATUS_ARM->Get__DATA(wfr_status);
			sEXT_CH__VAC_RB_A__MATERIAL_TITLE_ARM->Get__DATA(wfr_title);

			l_cur_pos.Add(cur_pos);
			l_wfr_status.Add(wfr_status);
			l_wfr_title.Add(wfr_title);
		}
		else if(i == 5)		// VAC_RB1-B ...
		{
			cur_pos = DEF__VAC_RB_B;

			dEXT_CH__VAC_RB_B__MATERIAL_STATUS_ARM->Get__DATA(wfr_status);
			sEXT_CH__VAC_RB_B__MATERIAL_TITLE_ARM->Get__DATA(wfr_title);

			l_cur_pos.Add(cur_pos);
			l_wfr_status.Add(wfr_status);
			l_wfr_title.Add(wfr_title);
		}
		else if(i == 6)		// LPx ...
		{
			int lp_id;
			int slot_id;
			int t;
			int tt;

			for(t=0; t<iLPx_UNIT_SIZE; t++)
			{
				lp_id = t + 1;

				if(xCH__PORT_CST_STATUS[t]->Check__DATA(STR__NONE) > 0)
				{
					continue;
				}

				ch_data = xCH__PORT_CFG_SLOT_MAX[t]->Get__STRING();
				int slot_max = atoi(ch_data);
				if(slot_max > CFG_LP__SLOT_MAX)			slot_max = CFG_LP__SLOT_MAX;

				for(tt=0; tt<slot_max; tt++)
				{
					slot_id = tt + 1;

					wfr_status = xCH__PORT_SLOT_STS[t][tt]->Get__STRING();
					if(wfr_status.CompareNoCase(STR__NONE) == 0)
					{
						continue;
					}

					cur_pos.Format("LP%1d-%1d",  lp_id,slot_id);
					wfr_title.Format("P%1d:%1d", lp_id,slot_id);

					l_cur_pos.Add(cur_pos);
					l_wfr_status.Add(wfr_status);
					l_wfr_title.Add(wfr_title);
				}
			}
		}
		else if(i == 7)		// BUFFx ...
		{
			int lp_id;
			int slot_id;
			int t;
			int tt;

			for(t=0; t<iBUFF_UNIT_SIZE; t++)
			{
				lp_id = t + 1;

				for(tt=0; tt<iBUFF_SLOT_SIZE; tt++)
				{
					slot_id = tt + 1;

					wfr_status = dEXT_CH__BUFFx_SLOT[t][tt]->Get__STRING();
					if(wfr_status.CompareNoCase(STR__NONE) == 0)
					{
						continue;
					}

					cur_pos.Format("%s%1d-%1d",  DEF__BUFFx_X, lp_id,slot_id);
					wfr_title.Format("B%1d:%1d", lp_id,slot_id);

					l_cur_pos.Add(cur_pos);
					l_wfr_status.Add(wfr_status);
					l_wfr_title.Add(wfr_title);
				}
			}
		}
		else
		{
			continue;
		}

		// ...
		{
			k_limit = l_cur_pos.GetSize();

			for(k=0; k<k_limit; k++)
			{
				cur_pos    = l_cur_pos[k];
				wfr_status = l_wfr_status[k];
				wfr_title  = l_wfr_title[k];

				if(wfr_status.CompareNoCase(STR__NONE) != 0)
				{
					l_ui_name.Add(cur_pos);
					l_ui_wafer_status.Add(wfr_status);
					l_ui_wafer_title.Add(wfr_title);
				}
			}
		}
	}

	// LLx Check ...
	{
		for(int ll_i=0; ll_i<iLLx_SIZE; ll_i++)
		{
			l_cur_pos.RemoveAll();
			l_wfr_status.RemoveAll();
			l_wfr_title.RemoveAll();

			// ...
			{
				CString ll_name = Get__LLx_NAME(ll_i);

				ch_data = xEXT_CH__CFG_DB_LLx_SLOT_MAX[ll_i]->Get__STRING();
				int slot_max = atoi(ch_data);
				if(slot_max > iLLx__SLOT_SIZE)		slot_max = iLLx__SLOT_SIZE;

				k_limit = slot_max;	
				for(k=0; k<k_limit; k++)
				{
					cur_pos.Format("%s-%1d", ll_name,k+1);

					dEXT_CH__LLx_X__MATERIAL_STATUS_SLOT[ll_i][k]->Get__DATA(wfr_status);
					sEXT_CH__LLx_X__MATERIAL_TITLE_SLOT[ll_i][k]->Get__DATA(wfr_title);
	
					l_cur_pos.Add(cur_pos);
					l_wfr_status.Add(wfr_status);
					l_wfr_title.Add(wfr_title);
				}
			}

			// ...
			{
				k_limit = l_cur_pos.GetSize();

				for(k=0; k<k_limit; k++)
				{
					cur_pos    = l_cur_pos[k];
					wfr_status = l_wfr_status[k];
					wfr_title  = l_wfr_title[k];

					if(wfr_status.CompareNoCase(STR__NONE) != 0)
					{
						l_ui_name.Add(cur_pos);
						l_ui_wafer_status.Add(wfr_status);
						l_ui_wafer_title.Add(wfr_title);
					}
				}
			}

			// ...
		}
	}

	// ...
	{
		k_limit = l_ui_name.GetSize();

		for(k=0; k<k_limit; k++)
		{
			cur_pos    = l_ui_name[k];
			wfr_status = l_ui_wafer_status[k];
			wfr_title  = l_ui_wafer_title[k];

			if(wfr_status.CompareNoCase(STR__NONE) != 0)
			{
				log_bff.Format("  %002d)  [%s] : Wafer_Status(%s) & Wafer_Title(%s) \n", 
								k+1, cur_pos,wfr_status,wfr_title);	
				log_msg += log_bff;
			}
		}

		xLOG_CTRL->WRITE__LOG(log_msg);		
	}
}
void CObj_Opr__AUTO_MODE
::Fnc__Get_All_Wafer_Info_Of_SCH(CStringArray& l_sch_pos,
							     CStringArray& l_sch_wfr_status,
							     CStringArray& l_sch_wfr_title)
{
	CStringArray l_ui_name;
	CStringArray l_ui_wafer_status;
	CStringArray l_ui_wafer_title;

	CString wfr_title;

	CString log_msg;
	CString log_bff;

	int limit;
	int i;

	Fnc__Get_All_Wafer_Info_Of_UI(l_ui_name,l_ui_wafer_status,l_ui_wafer_title);

	if(xSCH_MATERIAL_CTRL->Get__MATERIAL_DB_SIZE() > 0)
	{
		log_msg  = "\n";
		log_msg += "Scheduler DB Info ...";
		log_msg += "\n";

		// ...
		IDS__SCH_MATERIAL_INFO   ds_info;
		IDS__SCH_MATERIAL_STATUS ds_status;

		CStringArray l_all_sch_db;
		CString sch_name;

		xSCH_MATERIAL_CTRL->Get__ALL_MATERIAL_INFO_TO_PROCESS(l_all_sch_db);
		limit = l_all_sch_db.GetSize();

		for(i=0; i<limit; i++)
		{
			sch_name = l_all_sch_db[i];

			if(xSCH_MATERIAL_CTRL->Get__MATERIAL_INFO(sch_name, ds_info) < 0)
			{
				continue;
			}
			else
			{
				log_bff.Format("  %002d)  [%s] [P%1d:%1d] \n", 
					i+1,
					l_all_sch_db[i],
					ds_info.iSRC__PTN,
					ds_info.iSRC__STN);
			}

			log_msg += log_bff;
		}
		log_msg += "\n";

		// ...
		int port_id;
		int slot_id;

		limit = l_ui_wafer_title.GetSize();

		for(i=0; i<limit; i++)
		{
			wfr_title = l_ui_wafer_title[i];

			if(Macro__Analyze_Title_Info(wfr_title, port_id,slot_id) < 0)
			{
				// Error ...
				continue;
			}

			if(xSCH_MATERIAL_CTRL->Get__MATERIAL_STATUS(port_id,slot_id, ds_status) < 0)
			{
				// Error ...
				continue;
			}

			log_bff.Format("  %002d)  %s(%1d,%1d) : %s(%s) \n", 
							i+1,
							wfr_title,
							port_id,
							slot_id,
							ds_status.sMATERIAL_POS,
							ds_status.sMATERIAL_STS);
			log_msg += log_bff;			

			log_bff.Format("      ->> %s(%s) \n", 
							l_ui_name[i],
							l_ui_wafer_status[i]);
			log_msg += log_bff;			
		}

		xLOG_CTRL->WRITE__LOG(log_msg);
	}
	else
	{
		// Error Check
		log_msg  = "\n";
		log_msg += "Scheduler DB Info : Error !!! ";
		log_msg += "\n";

		xLOG_CTRL->WRITE__LOG(log_msg);
	}
}

void CObj_Opr__AUTO_MODE
::Fnc__Wafer_Info_Resynch_With_Scheduler_DB()
{
	CStringArray l_module_name;
	CStringArray l_module_wafer_status;
	CStringArray l_module_wafer_title;

	CString wfr_title;

	CString log_msg;
	CString log_bff;

	int limit;
	int i;
	int k_limit;
	int k;

	Fnc__Get_All_Wafer_Info_Of_UI(l_module_name,l_module_wafer_status,l_module_wafer_title);

	// ...
	{
		log_msg  = "\n";
		log_msg += "Module Wafer Info ...";
		log_msg += "\n";

		limit = l_module_name.GetSize();

		for(i=0; i<limit; i++)
		{
			log_bff.Format("%002d.  [%s] : %s(%s) \n", 
				i+1,						   
				l_module_name[i],
				l_module_wafer_title[i],
				l_module_wafer_status[i]);
			log_msg += log_bff;
		}

		xLOG_CTRL->WRITE__LOG(log_msg);
	}

	if(xSCH_MATERIAL_CTRL->Get__MATERIAL_DB_SIZE() > 0)
	{
		log_msg  = "\n";
		log_msg += "Scheduler DB Info ...";
		log_msg += "\n";

		// ...
		IDS__SCH_MATERIAL_STATUS ds_status;
		IDS__SCH_MATERIAL_INFO   ds_info;
		int port_id;
		int slot_id;

		limit = l_module_wafer_title.GetSize();

		for(i=0; i<limit; i++)
		{
			wfr_title = l_module_wafer_title[i];

			if(Macro__Analyze_Title_Info(wfr_title, port_id,slot_id) < 0)
			{
				// Error ...
				continue;
			}

			if(xSCH_MATERIAL_CTRL->Get__MATERIAL_STATUS(port_id,slot_id, ds_status) < 0)
			{
				// Error ...
				continue;
			}

			log_bff.Format("%002d.  %s(%1d,%1d) : %s(%s) \n", 
							i+1,
							wfr_title,
							port_id,
							slot_id,
							ds_status.sMATERIAL_POS,
							ds_status.sMATERIAL_STS);
			log_msg += log_bff;			

			log_bff.Format("  ->> %s(%s) \n", 
							l_module_name[i],
							l_module_wafer_status[i]);
			log_msg += log_bff;			
		}

		xLOG_CTRL->WRITE__LOG(log_msg);
	}
	else
	{
		// Error Check

	}
}

