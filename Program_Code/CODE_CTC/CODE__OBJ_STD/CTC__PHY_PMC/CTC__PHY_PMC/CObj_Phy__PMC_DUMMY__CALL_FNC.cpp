#include "StdAfx.h"
#include "CObj_Phy__PMC_DUMMY.h"
#include "CObj_Phy__PMC_DUMMY__ALID.h"


// ...
int  CObj_Phy__PMC_DUMMY
::Call__TIME_INIT(CII_OBJECT__VARIABLE* p_variable)
{
	Set__Module_Time();
	return 1;
}
int  CObj_Phy__PMC_DUMMY
::Call__SYSTEM_INIT(CII_OBJECT__VARIABLE* p_variable)
{
	xCH__OBJ_STATUS->Set__DATA(STR__STANDBY);
	return 1;
}
int  CObj_Phy__PMC_DUMMY
::Call__AUTO_INIT(CII_OBJECT__VARIABLE* p_variable)
{
	Call__TIME_INIT(p_variable);
	Call__SYSTEM_INIT(p_variable);
	return 1;
}

int  CObj_Phy__PMC_DUMMY
::Call__SYSTEM_MAINT(CII_OBJECT__VARIABLE* p_variable)
{
	xCH__OBJ_STATUS->Set__DATA(STR__MAINTMODE);
	return 1;
}

int  CObj_Phy__PMC_DUMMY
::Call__PUMP(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM *p_alarm)
{
	xCH__PRESSURE_STATUS->Set__DATA(STR__VAC);
	return 1;
}
int  CObj_Phy__PMC_DUMMY
::Call__VENT(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM *p_alarm)
{
	xCH__PRESSURE_STATUS->Set__DATA(STR__ATM);
	return 1;
}


// ...
void CObj_Phy__PMC_DUMMY::Set__Module_Time()
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

	xCH__MODULE_TIME->Set__DATA(str_time);
}
void CObj_Phy__PMC_DUMMY::Clear__PMC_Info()
{
	// ...
	{
		xCH__LPx_PORT_ID->Set__DATA("");
		xCH__LPx_SLOT_ID->Set__DATA("");
		xCH__LPx_CID->Set__DATA("");
	}

	// ...
	{
		xCH__RECIPE->Set__DATA("");			
		xCH__LOTID->Set__DATA("");			
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
void CObj_Phy__PMC_DUMMY
::Set__Place_Ready_Parameter(CII_OBJECT__VARIABLE *p_variable)
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
}
void CObj_Phy__PMC_DUMMY
::Set__Slot_No_Parameter(CII_OBJECT__VARIABLE *p_variable)
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

void CObj_Phy__PMC_DUMMY
::Set__Slot_Info(CII_OBJECT__VARIABLE *p_variable)
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
			IDS__SCH_MATERIAL_INFO ds_info;
			CString sch_name;

			sch_name.Format("PM%1d-%1d",iPMx_ID,i+1);
			xSCH_MATERIAL_CTRL->Get__MATERIAL_INFO(sch_name,ds_info);

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

void CObj_Phy__PMC_DUMMY
::Fnc__SET_TIME()
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
							
	xCH__MODULE_TIME->Set__DATA(str_time);
}


// ...
#define  DEF__CHECK_DB_EMPTY								\
if(xSCH_MATERIAL_CTRL->Get__MATERIAL_DB_SIZE() < 1)			\
{															\
	return -1;												\
}

#define  DEF__CHECK_SYSTEM_ABORT							\
if(xSCH_MATERIAL_CTRL->Check__SYSTEM_ABORT() > 0)			\
{															\
	return -1;												\
}

int  CObj_Phy__PMC_DUMMY
::Call__PICK_READY(CII_OBJECT__VARIABLE* p_variable)
{
	DEF__CHECK_SYSTEM_ABORT;

	xCH__LPx_WAFER_COUNT->Set__DATA("0");

	Set__Slot_No_Parameter(p_variable);
	Fnc__SET_TIME();

	return 1;
}
int  CObj_Phy__PMC_DUMMY
::Call__PICK_X_READY(CII_OBJECT__VARIABLE* p_variable)
{
	DEF__CHECK_SYSTEM_ABORT;

	Set__Slot_No_Parameter(p_variable);

	return Seq__PICK_X_READY(p_variable);
}
int  CObj_Phy__PMC_DUMMY
::Seq__PICK_X_READY(CII_OBJECT__VARIABLE* p_variable)
{
	DEF__CHECK_SYSTEM_ABORT;

	Fnc__SET_TIME();
	return 1;
}
int  CObj_Phy__PMC_DUMMY
::Call__PICK_COMPLETE(CII_OBJECT__VARIABLE* p_variable)
{
	DEF__CHECK_SYSTEM_ABORT;

	Set__Slot_No_Parameter(p_variable);
	Clear__PMC_Info();
	return 1;
}
int  CObj_Phy__PMC_DUMMY
::Call__PICK_X_COMPLETE(CII_OBJECT__VARIABLE* p_variable)
{
	DEF__CHECK_SYSTEM_ABORT;

	Set__Slot_No_Parameter(p_variable);
	Clear__PMC_Info();
	return 1;
}
int  CObj_Phy__PMC_DUMMY
::Call__PLACE_READY(CII_OBJECT__VARIABLE* p_variable)
{
	DEF__CHECK_SYSTEM_ABORT;

	Set__Place_Ready_Parameter(p_variable);
	Fnc__SET_TIME();
	return 1;
}
int  CObj_Phy__PMC_DUMMY
::Call__PLACE_X_READY(CII_OBJECT__VARIABLE* p_variable)
{
	DEF__CHECK_SYSTEM_ABORT;

	Set__Place_Ready_Parameter(p_variable);
	Fnc__SET_TIME();
	return 1;
}

int  CObj_Phy__PMC_DUMMY
::Call__PLACE_COMPLETE(CII_OBJECT__VARIABLE* p_variable)
{
	DEF__CHECK_SYSTEM_ABORT;

	Set__Slot_No_Parameter(p_variable);
	return Seq__PLACE_COMPLETE(p_variable);
}
int  CObj_Phy__PMC_DUMMY
::Seq__PLACE_COMPLETE(CII_OBJECT__VARIABLE* p_variable)
{
	DEF__CHECK_SYSTEM_ABORT;

	if(xCH_CFG__CLEAN_MODE->Check__DATA(STR__CLEAN_MODE__SYSTEM_IDLE) < 0)
	{
		xTIMER__SYS_IDLE->START__COUNT_DOWN(1.0);
	}
	return 1;
}

int  CObj_Phy__PMC_DUMMY
::Call__PLACE_X_COMPLETE(CII_OBJECT__VARIABLE* p_variable)
{
	DEF__CHECK_SYSTEM_ABORT;

	if(xCH_CFG__CLEAN_MODE->Check__DATA(STR__CLEAN_MODE__SYSTEM_IDLE) < 0)
	{
		xTIMER__SYS_IDLE->START__COUNT_DOWN(1.0);
	}

	Set__Slot_No_Parameter(p_variable);
	return 1;
}


//--------------------------------------------------------------------------------------
void CObj_Phy__PMC_DUMMY
::Set__Prc_Ready_Parameter(CII_OBJECT__VARIABLE *p_variable)
{
	CStringArray l_para;
	CString para_data;

	p_variable->Get__FNC_PARAMETER(l_para);

	int limit = l_para.GetSize();
	int i;

	for(i=0;i<limit;i++)
	{
		para_data = l_para[i];

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
}
void CObj_Phy__PMC_DUMMY
::Set__Cln_Ready_Parameter(CII_OBJECT__VARIABLE *p_variable)
{
	xCH__LOTID->Set__DATA("");
	xCH__MATERIAL_ID->Set__DATA("");

	// ...
	CString cln_rcp;

	if(xCH__NEXT_PROCESS->Check__DATA(STR__CLEAN_RF) > 0)
	{
		xCH_CFG__CLEAN_RF_RECIPE->Get__DATA(cln_rcp);
	}
	else
	{
		xCH_CFG__CLEAN_RECIPE->Get__DATA(cln_rcp);
	}

	xCH__RECIPE->Set__DATA(cln_rcp);
}
void CObj_Phy__PMC_DUMMY
::Set__Layer_Cln_Ready_Parameter(CII_OBJECT__VARIABLE *p_variable)
{
	xCH__LOTID->Set__DATA("");
	xCH__MATERIAL_ID->Set__DATA("");

	CString cln_rcp;
	xCH__LAYER_CLEAN_RECIPE->Get__DATA(cln_rcp);
	xCH__RECIPE->Set__DATA(cln_rcp);
}

void CObj_Phy__PMC_DUMMY
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

		     if(rcp__name.Find("PAS") == 0)     xCH__RCP_PAS__MATERIAL_COUNT->Inc__VALUE();
        else if(rcp__name.Find("NP")  == 0)		xCH__RCP_NP__MATERIAL_COUNT->Inc__VALUE();
		else if(rcp__name.Find("ACT") == 0)		xCH__RCP_ACT__MATERIAL_COUNT->Inc__VALUE();
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
void CObj_Phy__PMC_DUMMY
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
	if(ref_time < 1)
	{
		return;
	}

	// ...
	int cur_time;

	xCH_CFG__CLEAN_RF_CUR_TIME->Get__DATA(var_data);
	cur_time = atoi(var_data);

	if(cur_time < ref_time)
	{
		return;
	}

	if(xCH__NEXT_PROCESS->Check__DATA("") > 0)
	{
		xCH__NEXT_PROCESS->Set__DATA(STR__CLEAN_RF);
	}
}
void CObj_Phy__PMC_DUMMY
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

int  CObj_Phy__PMC_DUMMY
::Call__PRO_READY(CII_OBJECT__VARIABLE* p_variable)
{
	Set__Prc_Ready_Parameter(p_variable);

	return Seq__PRO_READY(p_variable);
}
int  CObj_Phy__PMC_DUMMY
::Seq__PRO_READY(CII_OBJECT__VARIABLE* p_variable)
{
	Set__Slot_Info(p_variable);

	DEF__CHECK_DB_EMPTY;
	DEF__CHECK_SYSTEM_ABORT;

	Fnc__SET_TIME();
	return 1;
}

int  CObj_Phy__PMC_DUMMY
::Call__PRO_START(CII_OBJECT__VARIABLE* p_variable,
				  CII_OBJECT__ALARM* p_alarm,
				  const int fa_report_flag)
{
	DEF__CHECK_DB_EMPTY;
	DEF__CHECK_SYSTEM_ABORT;

	// ...
	CStringArray l_para;	
	p_variable->Get__FNC_PARAMETER(l_para);

	if(l_para.GetSize() > 0)
	{
		xCH__RECIPE->Set__DATA(l_para[0]);
	}

	return Seq__PRO_START(p_variable,p_alarm,fa_report_flag);
}
int  CObj_Phy__PMC_DUMMY
::Seq__PRO_START(CII_OBJECT__VARIABLE* p_variable,
			     CII_OBJECT__ALARM* p_alarm,
			     const int fa_report_flag)
{
	double prc_time;

	// ...
	{
		CString var_data;

		xCH__CFG_PRC_TIME->Get__DATA(var_data);
		prc_time = atof(var_data);

		var_data.Format("%.1f",prc_time);
		xCH__TOTAL_PRC_TIME->Set__DATA(var_data);
	}

	// PRC START ...
	{
		CString var_data;

		xCH__RECIPE->Get__DATA(var_data);
		xCH__FDC_RECIPE->Set__DATA(var_data);

		xCH__LOTID->Get__DATA(var_data);
		xCH__FDC_LOTID->Set__DATA(var_data);
	}

	Check__Material_Count(p_variable);
	Check__RF_Time(p_variable);

	// ...
	CUIntArray   l_pmc_slot_id;
	CStringArray l_lotid;
	CUIntArray   l_portid;
	CUIntArray   l_slotid;
	int i;

	l_pmc_slot_id.RemoveAll();
	l_lotid.RemoveAll();
	l_portid.RemoveAll();
	l_slotid.RemoveAll();

	// ...
	{
		IDS__SCH_MATERIAL_INFO db_info;
		CString sch_name;

		CString material_lotid;
		int pmc_slot_id;
		int material_port_id;
		int material_slot_id;
		CString material_recipe;
	
		int slot_lot_count = 0;

		for(i=0;i<iPMx_SLOT_MAX;i++)
		{
			if(xCH__SLOT_STATUS[i]->Check__DATA("NONE") < 0)
			{
				pmc_slot_id = i + 1;

				if(xSCH_MATERIAL_CTRL->Get__MATERIAL_INFO__INTO_PMC(iPMx_ID,
																	pmc_slot_id,
																	material_port_id,
																	material_slot_id) > 0)
				{
					xSCH_MATERIAL_CTRL->Get__MATERIAL_INFO(material_port_id,material_slot_id,db_info);	
				}
				else
				{
					sch_name.Format("PM%1d-%1d",iPMx_ID,pmc_slot_id);
					xSCH_MATERIAL_CTRL->Get__MATERIAL_INFO(sch_name,db_info);

					material_port_id = db_info.iSRC__PTN;
					material_slot_id = db_info.iSRC__STN;
				}

				material_lotid  = db_info.sLOT_ID;
				material_recipe = db_info.sCUR_RECIPE;
				
				l_pmc_slot_id.Add(pmc_slot_id);
				l_lotid.Add(material_lotid);
				l_portid.Add(material_port_id);
				l_slotid.Add(material_slot_id);

				if(i == 0)
				{
					CString var_data;

					// SLOT NO.
					var_data.Format("%1d",pmc_slot_id);
					xCH__SLOT_NO->Set__DATA(var_data);
	
					// LPx - PORT ID
					var_data.Format("%1d",material_port_id);
					xCH__LPx_PORT_ID->Set__DATA(var_data);

					// LPx - SLOT ID
					var_data.Format("%1d",material_slot_id);
					xCH__LPx_SLOT_ID->Set__DATA(var_data);

					// LPx - CID
					xSCH_MATERIAL_CTRL->Get__LPx_CID(material_port_id,var_data);
					xCH__LPx_CID->Set__DATA(var_data);
				}

				CString pm_log_path;
				Datalog__Write_Lot_Slot(slot_lot_count++,pmc_slot_id,db_info,pm_log_path);

				if(fa_report_flag > 0)
				{
					xSCH_MATERIAL_CTRL->Process_Start__PMC_WITH_SLOT_EX(material_port_id,
																		material_slot_id,
																		iPMx_ID,
																		pmc_slot_id,
																		material_recipe,
																		pm_log_path);
				}

				xCH__SLOT_STATUS[i]->Set__DATA("PROCESSING");
			}
		}		
	}

	iPRC_ALARM_FLAG = -1;

	// ...
	SCX__TIMER_CTRL x_timer_ctrl;
	int flag = 1;

	x_timer_ctrl->REGISTER__ABORT_OBJECT(sObject_Name);
	flag = x_timer_ctrl->WAIT(prc_time);

	if(flag < 0)
	{
		for(i=0;i<iPMx_SLOT_MAX;i++)
		{
			if(xCH__SLOT_STATUS[i]->Check__DATA("NONE") < 0)
			{
				xCH__SLOT_STATUS[i]->Set__DATA("ABORTED");
			}
		}

		if(xCH__ALARM_LOG_MODE->Check__DATA("MATERIAL_ABORTED") > 0)
		{
			Post__PMx_ALARM(p_alarm);
		}
	}
	else
	{
		for(i=0;i<iPMx_SLOT_MAX;i++)
		{
			if(xCH__SLOT_STATUS[i]->Check__DATA("NONE") < 0)
			{
				xCH__SLOT_STATUS[i]->Set__DATA("PROCESSED");
			}
		}
	}

	// ...
	{
		CString lotid;
		CString portid;
		int     lp_slotid;

		int limit = l_lotid.GetSize();

		for(i=0;i<limit;i++)
		{
			lotid  = l_lotid[i];
			portid.Format("PORT%1d",(int) l_portid[i]);
			lp_slotid = (int) l_slotid[i];
			
			Datalog__Write_Lot_Slot_End(i+1,lotid,portid,lp_slotid);
		}
	}

	if(fa_report_flag > 0)
	{
		int material_port_id;
		int material_slot_id;
		int pmc_slot_id;

		int limit = l_pmc_slot_id.GetSize();
		int i;

		for(i=0;i<limit;i++)
		{
			material_port_id = (int) l_portid[i];
			material_slot_id = (int) l_slotid[i];
			pmc_slot_id = (int) l_pmc_slot_id[i];

			xSCH_MATERIAL_CTRL->Process_End__PMC_WITH_SLOT(material_port_id,
														   material_slot_id,
														   iPMx_ID,
														   pmc_slot_id);
		}
	}

	Check__RF_Time(p_variable);
	Check__Material_Clear();

	// PRC END ...
	{
		xCH__FDC_RECIPE->Set__DATA("");
		xCH__FDC_LOTID->Set__DATA("");
	}
	return flag;
}
int  CObj_Phy__PMC_DUMMY
::Call__AUTO_PRO(CII_OBJECT__VARIABLE* p_variable,
			     CII_OBJECT__ALARM* p_alarm)
{
	iFLAG__STEP_REPORT = 1;

	if(Call__PRO_READY(p_variable) < 0)					return -1;
	if(Call__PRO_START(p_variable,p_alarm,1) < 0)		return -1;

	return 1;
}
int  CObj_Phy__PMC_DUMMY
::Call__AUTO_PRO_EX(CII_OBJECT__VARIABLE* p_variable,
				    CII_OBJECT__ALARM* p_alarm)
{
	iFLAG__STEP_REPORT = 1;

	// ...
	{
		CStringArray l_para;
		CString para_data;

		p_variable->Get__FNC_PARAMETER(l_para);

		int limit = l_para.GetSize();
		int i;

		for(i=0;i<limit;i++)
		{
			para_data = l_para[i];

			switch(i)
			{
				// SLOT_NO
				case 0:		
					xCH__SLOT_NO->Set__DATA(para_data);			
					break;

				// RECIPE
				case 1:    
					xCH__RECIPE->Set__DATA(para_data);			
					break;

				// LOT_ID
				case 2:		
					xCH__LOTID->Set__DATA(para_data);			
					break;

				// MATERIAL_ID
				case 3:		
					xCH__MATERIAL_ID->Set__DATA(para_data);		
					break;
			}
		}
	}

	if(Seq__PLACE_COMPLETE(p_variable) < 0)				return -1;
	if(Seq__PRO_READY(p_variable) < 0)					return -1;
	if(Seq__PRO_START(p_variable,p_alarm,1) < 0)		return -1;

	if(Seq__PICK_X_READY(p_variable) > 0)
	{
		xCH__X_PICK_FLAG->Set__DATA("YES");
	}
	return 1;
}

int  CObj_Phy__PMC_DUMMY
::Call__CLN_READY(CII_OBJECT__VARIABLE* p_variable)
{
	Set__Cln_Ready_Parameter(p_variable);

	Fnc__SET_TIME();
	return 1;
}
int  CObj_Phy__PMC_DUMMY
::Call__CLN_START(CII_OBJECT__VARIABLE* p_variable,
				  CII_OBJECT__ALARM* p_alarm)
{
	if(xCH_CFG__CLEAN_MODE->Check__DATA(STR__CLEAN_MODE__SYSTEM_IDLE) < 0)
	{
		xTIMER__SYS_IDLE->START__COUNT_DOWN(1.0);
	}

	Set__Cln_Ready_Parameter(p_variable);

	xCH__NEXT_PROCESS->Set__DATA("");
	xCH_CFG__CLEAN_COUNT->Set__DATA("0");
	xCH_CFG__CLEAN_RF_CUR_TIME->Set__DATA("0");

	xSCH_MATERIAL_CTRL->Clean_Start__PMC(iPMx_ID);

	// ...
	int i;

	for(i=0;i<iPMx_SLOT_MAX;i++)
	{
		if(xCH__SLOT_STATUS[i]->Check__DATA("NONE") < 0)
		{
			xCH__SLOT_STATUS[i]->Set__DATA("PROCESSING");
		}
	}
	
	int flag = 1;

	if(flag < 0)
	{
		for(i=0;i<iPMx_SLOT_MAX;i++)
		{
			if(xCH__SLOT_STATUS[i]->Check__DATA("NONE") < 0)
			{
				xCH__SLOT_STATUS[i]->Set__DATA("ABORTED");
			}
		}

		if(xCH__ALARM_LOG_MODE->Check__DATA("MATERIAL_ABORTED") > 0)
		{
			Post__PMx_ALARM(p_alarm);
		}
	}
	else
	{
		for(i=0;i<iPMx_SLOT_MAX;i++)
		{
			if(xCH__SLOT_STATUS[i]->Check__DATA("NONE") < 0)
			{
				xCH__SLOT_STATUS[i]->Set__DATA("PROCESSED");
			}
		}
	}

	xSCH_MATERIAL_CTRL->Clean_End__PMC(iPMx_ID);

	// ...
	{
		xCH__RECIPE->Set__DATA("");

		for(int i=0;i<CFG__PMx_MAX_SLOT;i++)
		{
			xCH__SLOT_RECIPE[i]->Set__DATA("");
		}
	}
	return flag;
}
int  CObj_Phy__PMC_DUMMY
::Call__AUTO_CLN(CII_OBJECT__VARIABLE* p_variable,
			     CII_OBJECT__ALARM* p_alarm)
{
	CString win_name;

	xEXT_CH_CFG__PMC_MSG_WIN_NAME->Get__DATA(win_name);
	xEXT_CH_CFG__PMC_MSG_WIN_CTRL->Set__DATA(win_name);

	if(Call__CLN_READY(p_variable) < 0)				return -1;
	if(Call__CLN_START(p_variable,p_alarm) < 0)		return -1;

	return 1;
}

int  CObj_Phy__PMC_DUMMY
::Call__LAYER_CLN_READY(CII_OBJECT__VARIABLE* p_variable)
{
	Set__Layer_Cln_Ready_Parameter(p_variable);

	Fnc__SET_TIME();
	return 1;
}
int  CObj_Phy__PMC_DUMMY
::Call__LAYER_CLN_START(CII_OBJECT__VARIABLE* p_variable,
					    CII_OBJECT__ALARM* p_alarm)
{
	Set__Layer_Cln_Ready_Parameter(p_variable);

	xCH__NEXT_PROCESS->Set__DATA("");
	xCH_CFG__CLEAN_COUNT->Set__DATA("0");
	xCH_CFG__CLEAN_RF_CUR_TIME->Set__DATA("0");

	xSCH_MATERIAL_CTRL->Clean_Start__PMC(iPMx_ID);

	// ...
	int i;

	for(i=0;i<iPMx_SLOT_MAX;i++)
	{
		if(xCH__SLOT_STATUS[i]->Check__DATA("NONE") < 0)
		{
			xCH__SLOT_STATUS[i]->Set__DATA("PROCESSING");
		}
	}
	
	int flag = 1;

	if(flag < 0)
	{
		for(i=0;i<iPMx_SLOT_MAX;i++)
		{
			if(xCH__SLOT_STATUS[i]->Check__DATA("NONE") < 0)
			{
				xCH__SLOT_STATUS[i]->Set__DATA("ABORTED");
			}
		}

		if(xCH__ALARM_LOG_MODE->Check__DATA("MATERIAL_ABORTED") > 0)
		{
			Post__PMx_ALARM(p_alarm);
		}
	}
	else
	{
		for(i=0;i<iPMx_SLOT_MAX;i++)
		{
			if(xCH__SLOT_STATUS[i]->Check__DATA("NONE") < 0)
			{
				xCH__SLOT_STATUS[i]->Set__DATA("PROCESSED");
			}
		}
	}

	xSCH_MATERIAL_CTRL->Clean_End__PMC(iPMx_ID);
	return flag;
}
int  CObj_Phy__PMC_DUMMY
::Call__AUTO_LAYER_CLN(CII_OBJECT__VARIABLE* p_variable,
					   CII_OBJECT__ALARM* p_alarm)
{
	if(Call__LAYER_CLN_READY(p_variable) < 0)				return -1;
	if(Call__LAYER_CLN_START(p_variable,p_alarm) < 0)		return -1;

	return 1;
}

int  CObj_Phy__PMC_DUMMY
::Call__MNT_READY(CII_OBJECT__VARIABLE* p_variable)
{
	Set__Prc_Ready_Parameter(p_variable);

	Fnc__SET_TIME();
	return 1;
}
int  CObj_Phy__PMC_DUMMY
::Call__MNT_START(CII_OBJECT__VARIABLE* p_variable)
{
	Check__Material_Count(p_variable);
	Check__RF_Time(p_variable);

	// ...
	CStringArray l_para;
	p_variable->Get__FNC_PARAMETER(l_para);

	if(l_para.GetSize() > 0)
	{
		// RECIPE
		xCH__RECIPE->Set__DATA(l_para[0]);
	}

	// ...
	int i;

	for(i=0;i<iPMx_SLOT_MAX;i++)
	{
		if(xCH__SLOT_STATUS[i]->Check__DATA("NONE") < 0)
		{
			xCH__SLOT_STATUS[i]->Set__DATA("PROCESSING");
		}
	}
	
	int flag = 1;

	if(flag < 0)
	{
		for(i=0;i<iPMx_SLOT_MAX;i++)
		{
			if(xCH__SLOT_STATUS[i]->Check__DATA("NONE") < 0)
			{
				xCH__SLOT_STATUS[i]->Set__DATA("ABORTED");
			}
		}
	}
	else
	{
		for(i=0;i<iPMx_SLOT_MAX;i++)
		{
			if(xCH__SLOT_STATUS[i]->Check__DATA("NONE") < 0)
			{
				xCH__SLOT_STATUS[i]->Set__DATA("PROCESSED");
			}
		}
	}

	Check__RF_Time(p_variable);
	Check__Material_Clear();

	return flag;
}
int  CObj_Phy__PMC_DUMMY
::Call__AUTO_MNT(CII_OBJECT__VARIABLE* p_variable)
{
	if(Call__MNT_READY(p_variable) < 0)		return -1;
	if(Call__MNT_START(p_variable) < 0)		return -1;

	return 1;
}

int  CObj_Phy__PMC_DUMMY
::Call__LEAK_CHECK_EX(CII_OBJECT__VARIABLE* p_variable)
{
	int flag = Call__LEAK_CHECK(p_variable);

	xEXT_CH__PMC_LEAK_CHECK_FLAG->Set__DATA("");
	return flag;
}
int  CObj_Phy__PMC_DUMMY
::Call__LEAK_CHECK(CII_OBJECT__VARIABLE* p_variable)
{
	if(xCH_CFG__AUTO_LEAK_CHECK_USE->Check__DATA("ENABLE") < 0)
	{
		return 1;
	}

	// ...
	CString win_name;

	xEXT_CH_CFG__PMC_MSG_WIN_NAME->Get__DATA(win_name);
	xEXT_CH_CFG__PMC_MSG_WIN_CTRL->Set__DATA(win_name);

	xCH__NEXT_PROCESS->Set__DATA("");
	xCH__RECIPE->Set__DATA("LEAK CHECK");

	xSCH_MATERIAL_CTRL->LeakCheck_Start__PMC(iPMx_ID);
	xSCH_MATERIAL_CTRL->LeakCheck_End__PMC(iPMx_ID);

	xCH__RECIPE->Set__DATA("");

	return 1;
}

int  CObj_Phy__PMC_DUMMY
::Call__NEXT_PROC(CII_OBJECT__VARIABLE* p_variable,
				  CII_OBJECT__ALARM* p_alarm)
{
	iFNC_STS = 1;

	// ...
	{
		CString obj_msg;
		CString next_mode;

		xCH__NEXT_PROCESS->Get__DATA(next_mode);
		obj_msg.Format("[PM%1d] - [%s]  Starting ...",iPMx_ID,next_mode);

		xCH__OBJ_MSG->Set__DATA(obj_msg);
	}

	if((xCH__NEXT_PROCESS->Check__DATA(STR__CLEAN_COUNT) > 0)
	|| (xCH__NEXT_PROCESS->Check__DATA(STR__CLEAN_RF)    > 0))
	{	
		return Call__AUTO_CLN(p_variable,p_alarm);
	}
	else if(xCH__NEXT_PROCESS->Check__DATA(STR__LEAK_CHECK) > 0)
	{
		return Call__LEAK_CHECK(p_variable);
	}
	else if(xCH__NEXT_PROCESS->Check__DATA(STR__GOTO_MAINT) > 0)
	{
		xCH__NEXT_PROCESS->Set__DATA("");
		
		return Call__SYSTEM_MAINT(p_variable);
	}
	
	iFNC_STS = -1;
	return -1;
}

int  CObj_Phy__PMC_DUMMY
::Call__JOB_START_READY(CII_OBJECT__VARIABLE* p_variable)
{
	Fnc__SET_TIME();
	return 1;
}
int  CObj_Phy__PMC_DUMMY
::Call__JOB_END_READY(CII_OBJECT__VARIABLE* p_variable)
{
	Fnc__SET_TIME();
	return 1;
}
