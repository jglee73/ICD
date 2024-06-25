#include "StdAfx.h"
#include "CObj_Phy__PMC_STD.h"
#include "CObj_Phy__PMC_STD__ALID.h"

#include "Macro_Function.h"


#include "CMacro_FA.h"
extern CMacro_FA mFA_Link;


//--------------------------------------------------------------------------------------
int  CObj_Phy__PMC_STD::Call__TIME_INIT(CII_OBJECT__VARIABLE* p_variable)
{
	bActive__Init_Act = true;
	xI_Module_Obj->Disable__ERROR_CHECK__NOT_STANDBY();

	Set__Module_Time();

	return xI_Module_Obj->Connect__Module_Obj("TIME_INIT");
}
int  CObj_Phy__PMC_STD::Call__SYSTEM_INIT(CII_OBJECT__VARIABLE* p_variable)
{
	bActive__Init_Act = true;
	xI_Module_Obj->Disable__ERROR_CHECK__NOT_STANDBY();

	return xI_Module_Obj->Connect__Module_Obj("SYSTEM_INIT");
}
int  CObj_Phy__PMC_STD::Call__AUTO_INIT(CII_OBJECT__VARIABLE* p_variable)
{
	bActive__Init_Act = true;
	xI_Module_Obj->Disable__ERROR_CHECK__NOT_STANDBY();

	Set__Module_Time();

	return xI_Module_Obj->Connect__Module_Obj("AUTO_INIT");
}

int  CObj_Phy__PMC_STD::Call__SYSTEM_MAINT(CII_OBJECT__VARIABLE* p_variable)
{
	return xI_Module_Obj->Connect__Module_Obj("SYSTEM_MAINT");
}


//--------------------------------------------------------------------------------------
int  CObj_Phy__PMC_STD
::Call__JOB_START_READY(CII_OBJECT__VARIABLE* p_variable)
{
	Fnc__SET_TIME();

	// ...
	int r_flag = 1;

	// ...
	bool active__marathon_test = false;

	if((dEXT_CH__ALL_PM_MARATHON_TEST_USE->Check__DATA(STR__ENABLE) > 0)
	&& (dEXT_CH__PM_ID_MARATHON_TEST_USE->Check__DATA(STR__YES) > 0))
	{
		active__marathon_test = true;
	}

	if(active__marathon_test)
	{

	}
	else
	{
		r_flag = xI_Module_Obj->Connect__Module_Obj("JOB_START_READY");
	}

	if(r_flag > 0)
	{
		Save__JOB_START_READY();
	}
	return r_flag;
}
int  CObj_Phy__PMC_STD
::Save__JOB_START_READY()
{
	// ...
	{
		CString log_msg;
		CString log_bff;

		log_msg = "Save__JOB_START_READY() ... \n";
		
		log_bff.Format(" # PRE_INFO ... \n");
		log_msg += log_bff;
		log_bff.Format(" * %s <- %s \n", 
						sCH__PRE_INFO_JOB_START_READY_PORTID->Get__CHANNEL_NAME(),
						sCH__PRE_INFO_JOB_START_READY_PORTID->Get__STRING());
		log_msg += log_bff;
		log_bff.Format(" * %s <- %s \n", 
					sCH__PRE_INFO_JOB_START_READY_JOBID->Get__CHANNEL_NAME(),
					sCH__PRE_INFO_JOB_START_READY_JOBID->Get__STRING());
		log_msg += log_bff;

		log_bff.Format(" # REQ_INFO ... \n");
		log_msg += log_bff;
		log_bff.Format(" * %s <- %s \n", 
						sCH__REQ_INFO_JOB_START_READY_PORTID->Get__CHANNEL_NAME(),
						sCH__REQ_INFO_JOB_START_READY_PORTID->Get__STRING());
		log_msg += log_bff;
		log_bff.Format(" * %s <- %s \n", 
						sCH__REQ_INFO_JOB_START_READY_JOBID->Get__CHANNEL_NAME(),
						sCH__REQ_INFO_JOB_START_READY_JOBID->Get__STRING());
		log_msg += log_bff;
		
		xLOG_CTRL->WRITE__LOG(log_msg);
	}

	CString req_portid = sCH__REQ_INFO_JOB_START_READY_PORTID->Get__STRING();
	CString req_jobid  = sCH__REQ_INFO_JOB_START_READY_JOBID->Get__STRING();

	sCH__PRE_INFO_JOB_START_READY_PORTID->Set__DATA(req_portid);
	sCH__PRE_INFO_JOB_START_READY_JOBID->Set__DATA(req_jobid);

	return 1;
}

int  CObj_Phy__PMC_STD
::Call__JOB_END_READY(CII_OBJECT__VARIABLE* p_variable)
{
	Fnc__SET_TIME();

	// ...
	int r_flag = 1;

	// ...
	bool active__marathon_test = false;

	if((dEXT_CH__ALL_PM_MARATHON_TEST_USE->Check__DATA(STR__ENABLE) > 0)
	&& (dEXT_CH__PM_ID_MARATHON_TEST_USE->Check__DATA(STR__YES) > 0))
	{
		active__marathon_test = true;
	}

	if(active__marathon_test)
	{

	}
	else
	{
		r_flag = xI_Module_Obj->Connect__Module_Obj("JOB_END_READY");
	}

	return r_flag;
}

int  CObj_Phy__PMC_STD
::Call__IDLE_START(CII_OBJECT__VARIABLE* p_variable)
{
	Fnc__SET_TIME();

	// ...
	int r_flag = 1;

	// ...
	bool active__marathon_test = false;

	if((dEXT_CH__ALL_PM_MARATHON_TEST_USE->Check__DATA(STR__ENABLE) > 0)
	&& (dEXT_CH__PM_ID_MARATHON_TEST_USE->Check__DATA(STR__YES) > 0))
	{
		active__marathon_test = true;
	}

	if(active__marathon_test)
	{

	}
	else
	{
		r_flag = xI_Module_Obj->Connect__Module_Obj("IDLE_START");
	}

	return r_flag;
}


//--------------------------------------------------------------------------------------
void CObj_Phy__PMC_STD::Set__Module_Time()
{
	SYSTEMTIME cur_time;
	CString	   str_time;

	GetLocalTime(&cur_time);
	str_time.Format("%00004d%002d%002d%002d%002d%002d",
				    cur_time.wYear,
					cur_time.wMonth, 
					cur_time.wDay,
					cur_time.wHour,
					cur_time.wMinute,
					cur_time.wSecond);

	sCH__MODULE_TIME->Set__DATA(str_time);
}
void CObj_Phy__PMC_STD::Clear__PMC_Info()
{
	// ...
	{
		// LPx - PORT ID
		xCH__LPx_PORT_ID->Set__DATA("");

		// LPx - SLOT ID
		xCH__LPx_SLOT_ID->Set__DATA("");

		// LPx - CID
		xCH__LPx_CID->Set__DATA("");
	}

	// ...
	{
		// RECIPE
		xCH__RECIPE->Set__DATA("");			

		// LOT_ID
		xCH__LOTID->Set__DATA("");			
	
		// MATERIAL_ID
		xCH__MATERIAL_ID->Set__DATA("");		
	}

	if(xCH_CFG__CLEAN_USE->Check__DATA("ENABLE") > 0)
	{
		if(xCH_CFG__CLEAN_MODE->Check__DATA(STR__CLEAN_MODE__SYSTEM_IDLE) < 0)
		{
			double trg_up_sec = 9999999.0;
		
			xTIMER__SYS_IDLE->START__COUNT_UP(trg_up_sec);
		}
	}
}
void CObj_Phy__PMC_STD::Set__Place_Ready_Parameter(CII_OBJECT__VARIABLE *p_variable)
{
	DECLARE__EXT_CTRL(p_variable);

	xCH__RECIPE->Set__DATA("");

	// ...
	CStringArray l_para;
	CString para_data;

	p_variable->Get__FNC_PARAMETER(l_para);

	int limit = l_para.GetSize();
	int i;

	for(i=0; i<limit; i++)
	{
		para_data = l_para[i];

		switch(i)
		{
			case 0:		// SLOT NO.
				xCH__SLOT_NO->Set__DATA(para_data);
				break;

			case 1:		// LPx - PORT ID
				xCH__LPx_PORT_ID->Set__DATA(para_data);

				// ...
				{
					int lp_index = atoi(para_data) - 1;

					if((lp_index >= 0)&&(lp_index < CFG__LPx_SIZE))
					{
						CString var_data;
						xCH_CFG__LPx_WAFER_COUNT[lp_index]->Get__DATA(var_data);

						var_data.Format("%1d",atoi(var_data)+1);
						xCH__LPx_WAFER_COUNT->Set__DATA(var_data);
						xCH_CFG__LPx_WAFER_COUNT[lp_index]->Set__DATA(var_data);
					}
				}
				break;

			case 2:		// LPx - SLOT ID
				xCH__LPx_SLOT_ID->Set__DATA(para_data);
				break;

			case 3:		// LPx - CID
				xCH__LPx_CID->Set__DATA(para_data);
				break;
		}
	}

	// ...
}
void CObj_Phy__PMC_STD::Set__Slot_No_Parameter(CII_OBJECT__VARIABLE *p_variable)
{
	DECLARE__EXT_CTRL(p_variable);

	CStringArray l_para;
	CString para_data;

	p_variable->Get__FNC_PARAMETER(l_para);

	int limit = l_para.GetSize();
	int i;

	for(i=0; i<limit; i++)
	{
		para_data = l_para[i];

		switch(i)
		{
			case 0:		// SLOT NO.
				xCH__SLOT_NO->Set__DATA(para_data);
				break;
		}
	}
}

void CObj_Phy__PMC_STD::Set__Slot_Info(CII_OBJECT__VARIABLE *p_variable)
{
	CString recipe;
	CString lotid;
	CString material_id;
	CString lpx_cid;
	CString lpx_port_id;
	CString lpx_slot_id;
	int i;

	xCH__RECIPE->Get__DATA(recipe);

	for(i=0; i<iPMx_SLOT_MAX; i++)
	{
		if(xCH__SLOT_STATUS[i]->Check__DATA("NONE") > 0)
		{
			recipe = "";
			lotid  = "";
			material_id = "";
			lpx_cid = "";
			lpx_port_id = "";
			lpx_slot_id = "";
		}
		else
		{
			//.....
			IDS__SCH_MATERIAL_INFO ds_info;
			CString sch_name;

			sch_name.Format("PM%1d-%1d",iPMx_ID,i+1);
			xSCH_MATERIAL_CTRL->Get__MATERIAL_INFO(sch_name,ds_info);

			//.....
			int port_id = ds_info.iSRC__PTN;
			int slot_id = ds_info.iSRC__STN;

			xSCH_MATERIAL_CTRL->Get__LPx_CID(port_id,lpx_cid);
			lpx_port_id.Format("%1d",port_id);
			lpx_slot_id.Format("%1d",slot_id);

			lotid = ds_info.sLOT_ID;
			material_id = ds_info.sMATERIAL_ID;	
		}
		
		xCH__SLOT_RECIPE[i]->Set__DATA(recipe);
		xCH__SLOT_LOTID[i]->Set__DATA(lotid);
		xCH__SLOT_MATERIALID[i]->Set__DATA(material_id);
		
		xCH__SLOT_LPx_CID[i]->Set__DATA(lpx_cid);
		xCH__SLOT_LPx_PORT_ID[i]->Set__DATA(lpx_port_id);
		xCH__SLOT_LPx_SLOT_ID[i]->Set__DATA(lpx_slot_id);
	}
}
void CObj_Phy__PMC_STD::Check__Slot_Info(CII_OBJECT__VARIABLE *p_variable)
{
	CString msg;
	CString bff;

	CString recipe;
	int i;

	for(i=0; i<iPMx_SLOT_MAX; i++)
	{
		if(xCH__SLOT_STATUS[i]->Check__DATA("NONE") > 0)
		{
			recipe = "";
		}
		else
		{
			IDS__SCH_MATERIAL_INFO ds_info;
			CString sch_name;

			sch_name.Format("PM%1d-%1d",iPMx_ID,i+1);
			xSCH_MATERIAL_CTRL->Get__MATERIAL_INFO(sch_name,ds_info);

			recipe = ds_info.sCUR_RECIPE;
		}

		bff.Format("%1d) [%s]\n",i+1,recipe);
		msg += bff;
	}

	MessageBox(NULL,msg,"CHECK - Check__Slot_Info()",MB_OK|MB_TOPMOST);
}

void CObj_Phy__PMC_STD::Fnc__SET_TIME()
{
	SYSTEMTIME cur_time;
	CString	   str_time;
							
	GetLocalTime(&cur_time);
	str_time.Format("%00004d%002d%002d%002d%002d%002d",
				    cur_time.wYear,
					cur_time.wMonth, 
					cur_time.wDay,
					cur_time.wHour,
					cur_time.wMinute,
					cur_time.wSecond);
							
	sCH__MODULE_TIME->Set__DATA(str_time);
}


//--------------------------------------------------------------------------------------
void CObj_Phy__PMC_STD
::Set__Prc_Ready_Parameter(CII_OBJECT__VARIABLE *p_variable)
{
	CString log_msg;
	CString log_bff;

	CStringArray l_para;
	CString para_data;

	p_variable->Get__FNC_PARAMETER(l_para);

	int limit = l_para.GetSize();
	int i;

	log_msg.Format("Set__Prc_Ready_Parameter : Size(%1d) ... \n", limit);

	for(i=0; i<limit; i++)
	{
		para_data = l_para[i];

		log_bff.Format("  * %1d) %s \n", i,para_data);
		log_msg += log_bff;

		switch(i)
		{
			case 0:		// RECIPE
				xCH__RECIPE->Set__DATA(para_data);
				break;

			case 1:		// LOT ID
				xCH__LOTID->Set__DATA(para_data);
				break;

			case 2:		// MATERIAL ID
				xCH__MATERIAL_ID->Set__DATA(para_data);
				break;
		}
	}

	xLOG_CTRL->WRITE__LOG(log_msg);
}

void CObj_Phy__PMC_STD
::Check__Material_Count(CII_OBJECT__VARIABLE *p_variable)
{
	// ...
	{
		CString var_data;
		int count;

		xCH__ACC_MATERIAL_COUNT->Get__DATA(var_data);		
		count = atoi(var_data) + 1;
		if(count > 99999)   count = 0;
		var_data.Format("%1d",count);

		xCH__ACC_MATERIAL_COUNT->Set__DATA(var_data);

		// ...
		CString rcp__name;
		int rcp__db_index;

		xCH__RECIPE->Get__DATA(rcp__name);

		while((rcp__db_index = rcp__name.Find("\\")) >= 0)
		{
			rcp__name.Delete(0,rcp__db_index+1);
		}

		     if(rcp__name.Find("PAS") >= 0)     xCH__RCP_PAS__MATERIAL_COUNT->Inc__VALUE();
        else if(rcp__name.Find("NP")  >= 0)		xCH__RCP_NP__MATERIAL_COUNT->Inc__VALUE();
		else if(rcp__name.Find("ACT") >= 0)		xCH__RCP_ACT__MATERIAL_COUNT->Inc__VALUE();
		else									xCH__RCP_OTHER__MATERIAL_COUNT->Inc__VALUE();

		xCH__RCP_TOTAL__MATERIAL_COUNT->Inc__VALUE();
	}

	if(xCH_CFG__AUTO_MATERIAL_COUNT_FLAG->Check__DATA("ENABLE") > 0)
	{
		CString var_data;
		int count;

		xCH_CFG__CLEAN_COUNT->Get__DATA(var_data);
		
		count = atoi(var_data) + 1;
		var_data.Format("%1d",count);

		xCH_CFG__CLEAN_COUNT->Set__DATA(var_data);
	}
	else
	{
		CString var_data;

		var_data = sCH_CUR__CLEAN_WAFER_COUNT->Get__STRING();
		xCH_CFG__CLEAN_COUNT->Set__DATA(var_data);
	}

	if(xCH_CFG__CLEAN_USE->Check__DATA("ENABLE") < 0)
	{
		if(xCH_CFG__CLEAN_RF_USE->Check__DATA("ENABLE") < 0)
		{
			if((xCH__NEXT_PROCESS->Check__DATA(STR__LEAK_CHECK) < 0)
			&& (xCH__NEXT_PROCESS->Check__DATA(STR__GOTO_MAINT) < 0))
			{
				xCH__NEXT_PROCESS->Set__DATA("");	
			}
		}
		return;
	}

	if((xCH_CFG__CLEAN_MODE->Check__DATA(STR__CLEAN_MODE__FIRST_SECOND) < 0)
	&& (xCH_CFG__CLEAN_MODE->Check__DATA(STR__CLEAN_MODE__SYSTEM_IDLE)  < 0))
	{
		CString var_data;
		int ref_count;

		xCH_CFG__CLEAN_FREQUENCY->Get__DATA(var_data);	
		ref_count = atoi(var_data);
		if(ref_count < 1)
		{
			return;
		}

		// ...
		int cur_count;

		xCH_CFG__CLEAN_COUNT->Get__DATA(var_data);
		cur_count = atoi(var_data);
		if(cur_count < ref_count)
		{
			return;
		}

		if(xCH__NEXT_PROCESS->Check__DATA("") > 0)
		{
			xCH__NEXT_PROCESS->Set__DATA(STR__CLEAN_COUNT);
		}
		return;
	}
}
void CObj_Phy__PMC_STD
::Check__RF_Time(CII_OBJECT__VARIABLE *p_variable)
{
	if(xCH_CFG__CLEAN_RF_USE->Check__DATA("ENABLE") < 0)
	{
		if(xCH_CFG__CLEAN_USE->Check__DATA("ENABLE") < 0)
		{
			if((xCH__NEXT_PROCESS->Check__DATA(STR__LEAK_CHECK) < 0)
			&& (xCH__NEXT_PROCESS->Check__DATA(STR__GOTO_MAINT) < 0))
			{
				xCH__NEXT_PROCESS->Set__DATA("");	
			}
		}
		return;
	}

	// ...
	CString var_data;
	int ref_time;

	xCH_CFG__CLEAN_RF_REF_TIME->Get__DATA(var_data);
	
	ref_time = atoi(var_data);
	if(ref_time < 1)			return;

	// ...
	int cur_time;

	xCH_CFG__CLEAN_RF_CUR_TIME->Get__DATA(var_data);
	cur_time = atoi(var_data);

	if(cur_time < ref_time)		return;

	if(xCH__NEXT_PROCESS->Check__DATA("") > 0)
	{
		xCH__NEXT_PROCESS->Set__DATA(STR__CLEAN_RF);
	}
}
void CObj_Phy__PMC_STD
::Check__Material_Clear()
{
	if(xCH__MATERIAL_CLEAR_REQ->Check__DATA("YES") < 0)	
	{
		return;
	}

	// ...
	CString cur_pos;
	CString pm_slot;
			
	xCH__MATERIAL_CLEAR_SLOT->Get__DATA(pm_slot);
	cur_pos.Format("PM%1d-%s",iPMx_ID,pm_slot);

	xSCH_MATERIAL_CTRL->Clear__MATERIAL_INFO(cur_pos);

	// ...
	int pm_slot_id = atoi(pm_slot);

	if((pm_slot_id > 0)
	&& (pm_slot_id <= iPMx_SLOT_MAX))
	{
		xCH__SLOT_STATUS[pm_slot_id-1]->Set__DATA("NONE");
		xCH__SLOT_TITLE[pm_slot_id-1]->Set__DATA("");
	}

	xCH__MATERIAL_CLEAR_REQ->Set__DATA("");
	xCH__MATERIAL_CLEAR_SLOT->Set__DATA("");
}
