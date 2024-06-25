#include "StdAfx.h"
#include "CObj_Phy__PMC_STD.h"
#include "CObj_Phy__PMC_STD__ALID.h"
#include "CObj_Phy__PMC_STD__DEF.h"

#include "Macro_Function.h"


#include "CMacro_FA.h"
extern CMacro_FA mFA_Link;


//--------------------------------------------------------------------------------------
int  CObj_Phy__PMC_STD
::Call__SCH_TEST_CFG(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	double cfg_sec = 10.0;

	aCH__SCH_TEST_CFG_PMC_PROC_PRE_SEC->Set__VALUE(cfg_sec);
	aCH__SCH_TEST_CFG_PMC_PROC_MAIN_SEC->Set__VALUE(cfg_sec);
	aCH__SCH_TEST_CFG_PMC_PROC_POST_SEC->Set__VALUE(cfg_sec);

	aCH__SCH_TEST_CFG_PMC_JOB_START_READY_SEC->Set__VALUE(cfg_sec);
	aCH__SCH_TEST_CFG_PMC_JOB_END_READY_SEC->Set__VALUE(cfg_sec);
	aCH__SCH_TEST_CFG_PMC_IDLE_START_SEC->Set__VALUE(cfg_sec);

	aCH__SCH_TEST_CFG_PMC_LEAK_CHECK_SEC->Set__VALUE(cfg_sec);
	aCH__SCH_TEST_CFG_PMC_AUTO_PM_SEC->Set__VALUE(cfg_sec);

	return 1;
}

int  CObj_Phy__PMC_STD
::Call__CTC_SIM__TIME_INIT(CII_OBJECT__VARIABLE* p_variable)
{
	return 1;
}
int  CObj_Phy__PMC_STD
::Call__CTC_SIM__SYSTEM_INIT(CII_OBJECT__VARIABLE* p_variable)
{
	CString ch_data;

	if(dEXT_CH_CFG__TRANSFER_MODE->Check__DATA(STR__VAC) > 0)
	{
		double cfg__pressure_torr = xEXT_CH_CFG__REF_VAC_PRESSURE->Get__VALUE();

		cfg__pressure_torr = cfg__pressure_torr * 0.9;

		ch_data.Format("%.3f", cfg__pressure_torr);
		xCH__PRESSURE_VALUE->Set__DATA(ch_data);
	}
	else
	{
		double cfg__pressure_torr = xEXT_CH_CFG__REF_ATM_PRESSURE->Get__VALUE();

		cfg__pressure_torr = cfg__pressure_torr * 1.01;

		ch_data.Format("%.3f", cfg__pressure_torr);
		xCH__PRESSURE_VALUE->Set__DATA(ch_data);
	}

	xCH__OBJ_STATUS->Set__DATA(STR__STANDBY);
	return 1;
}
int  CObj_Phy__PMC_STD
::Call__CTC_SIM__AUTO_INIT(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM *p_alarm)
{
	Call__CTC_SIM__TIME_INIT(p_variable);

	Call__CTC_SIM__PUMP(p_variable, p_alarm);

	return Call__CTC_SIM__SYSTEM_INIT(p_variable);
}
int  CObj_Phy__PMC_STD
::Call__CTC_SIM__SYSTEM_MAINT(CII_OBJECT__VARIABLE* p_variable)
{
	xCH__OBJ_STATUS->Set__DATA(STR__MAINTMODE);
	return 1;
}


int  CObj_Phy__PMC_STD
::Call__CTC_SIM__PUMP(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM *p_alarm)
{
	xCH__PRESSURE_VALUE->Set__DATA("0.0");
	xCH__PRESSURE_STATUS->Set__DATA(STR__VAC);
	sCH__VAC_SNS->Set__DATA("ON");
	return 1;
}
int  CObj_Phy__PMC_STD
::Call__CTC_SIM__VENT(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM *p_alarm)
{
	xCH__PRESSURE_VALUE->Set__DATA("800.0");
	xCH__PRESSURE_STATUS->Set__DATA(STR__ATM);
	sCH__VAC_SNS->Set__DATA("OFF");
	return 1;
}


int  CObj_Phy__PMC_STD
::Call__CTC_SIM__SLOT_OPEN(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM *p_alarm)
{
	Sleep(1000);

	dCH__SLIT_VLV_STS->Set__DATA(STR__OPEN);
	return 1;
}
int  CObj_Phy__PMC_STD
::Call__CTC_SIM__SLOT_CLOSE(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM *p_alarm)
{
	Sleep(1000);

	dCH__SLIT_VLV_STS->Set__DATA(STR__CLOSE);
	return 1;
}

int  CObj_Phy__PMC_STD
::Call__CTC_SIM__PIN_UP(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM *p_alarm)
{
	Sleep(1000);

	sCH__LIFT_PIN_STS->Set__DATA(STR__UP);
	return 1;
}
int  CObj_Phy__PMC_STD
::Call__CTC_SIM__PIN_DOWN(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM *p_alarm)
{
	Sleep(1000);

	sCH__LIFT_PIN_STS->Set__DATA(STR__DOWN);
	return 1;
}


int  CObj_Phy__PMC_STD
::Call__CTC_SIM__PICK_READY(CII_OBJECT__VARIABLE* p_variable)
{
	return 1;
}
int  CObj_Phy__PMC_STD
::Call__CTC_SIM__PICK_X_READY(CII_OBJECT__VARIABLE* p_variable)
{
	return 1;
}
int  CObj_Phy__PMC_STD
::Call__CTC_SIM__PICK_COMPLETE(CII_OBJECT__VARIABLE* p_variable)
{
	return 1;
}
int  CObj_Phy__PMC_STD
::Call__CTC_SIM__PICK_X_COMPLETE(CII_OBJECT__VARIABLE* p_variable)
{
	return 1;
}

int  CObj_Phy__PMC_STD
::Call__CTC_SIM__PLACE_READY(CII_OBJECT__VARIABLE* p_variable)
{
	return 1;
}
int  CObj_Phy__PMC_STD
::Call__CTC_SIM__PLACE_X_READY(CII_OBJECT__VARIABLE* p_variable)
{
	return 1;
}
int  CObj_Phy__PMC_STD
::Call__CTC_SIM__PLACE_COMPLETE(CII_OBJECT__VARIABLE* p_variable)
{
	return 1;
}
int  CObj_Phy__PMC_STD
::Call__CTC_SIM__PLACE_X_COMPLETE(CII_OBJECT__VARIABLE* p_variable)
{
	return 1;
}


int  CObj_Phy__PMC_STD
::Call__CTC_SIM__PRO_READY(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM *p_alarm)
{
	Set__Prc_Ready_Parameter(p_variable);

	return Seq__CTC_SIM__PRO_READY(p_variable, p_alarm);
}
int  CObj_Phy__PMC_STD
::Seq__CTC_SIM__PRO_READY(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM *p_alarm)
{
	// Start Time ...
	{
		CString pmc_name;
		pmc_name.Format("PM%1d",iPMx_ID);

		CString date_time;
		Macro__Get_Date_Time(date_time);

		CString var_data;
		xCH__SLOT_NO->Get__DATA(var_data);

		CString sch_name;
		sch_name.Format("%s-%1d", pmc_name,atoi(var_data));

		xI_LCD_DB_CTRL->Set__GLS__START_INFO(sch_name,pmc_name,date_time);
	}

	if(dCH__CFG_SLIT_VALVE_CLOSE_CHECK->Check__DATA(STR__ENABLE) > 0)
	{
LOOP_CHECK:

		int check_flag = -1;
		int check_count = 10;

		while(1)
		{
			if(dCH__SLIT_VLV_STS->Check__DATA("CLOSE") > 0)
			{
				check_flag = 1;
				break;
			}

			check_count--;
			if(check_count < 1)
			{
				break;
			}

			Sleep(100);
		}

		if(check_flag < 0)
		{
			CString r_act;

			p_alarm->Popup__ALARM(ALID__SLIT_VALVE_NOT_CLOSE,r_act);

			if(r_act.CompareNoCase("ABORT") == 0)
			{
				return -1;
			}
			if(r_act.CompareNoCase("RETRY") == 0)
			{
				goto LOOP_CHECK;
			}
		}
	}

	xCH__OBJ_FNC_MSG->Set__DATA("Process Ready");

	Set__Slot_Info(p_variable);

	if(xSCH_MATERIAL_CTRL->Get__MATERIAL_DB_SIZE() < 1)			return -1;	
	if(xSCH_MATERIAL_CTRL->Check__SYSTEM_ABORT()   > 0)			return -1;

	return 1;
}
int  CObj_Phy__PMC_STD
::Call__CTC_SIM__PRO_START(CII_OBJECT__VARIABLE* p_variable, 
						   CII_OBJECT__ALARM *p_alarm,
						   const int fa_report_flag)
{
	if(xSCH_MATERIAL_CTRL->Get__MATERIAL_DB_SIZE() < 1)			return -1;	
	if(xSCH_MATERIAL_CTRL->Check__SYSTEM_ABORT()   > 0)			return -1;

	// ...
	{
		CStringArray l_para;	
		p_variable->Get__FNC_PARAMETER(l_para);

		if(l_para.GetSize() > 0)
			xCH__RECIPE->Set__DATA(l_para[0]);
	}

	return Seq__CTC_SIM__PRO_START(p_variable, p_alarm, fa_report_flag);
}
int  CObj_Phy__PMC_STD
::Seq__CTC_SIM__PRO_START(CII_OBJECT__VARIABLE* p_variable, 
						   CII_OBJECT__ALARM *p_alarm,
						   const int fa_report_flag)
{
	xCH__OBJ_STATUS->Set__DATA(STR__CTCINUSE);

	int r_flag = Fnc__CTC_SIM__PRO_START(p_variable, p_alarm, fa_report_flag);

	if(r_flag > 0)		xCH__OBJ_STATUS->Set__DATA(STR__STANDBY);
	else				xCH__OBJ_STATUS->Set__DATA(STR__MAINTMODE);

	return r_flag;
}
int  CObj_Phy__PMC_STD
::Fnc__CTC_SIM__PRO_START(CII_OBJECT__VARIABLE* p_variable, 
						  CII_OBJECT__ALARM *p_alarm,
						  const int fa_report_flag)
{
	SCX__ASYNC_TIMER_CTRL x_timer_ctrl;
	x_timer_ctrl->START__COUNT_UP(9999);

	// ...
	{
		CString ch_data = Macro__Get_Current_Time();
		sCH__TIME_ACT_START->Set__DATA(ch_data);
	}

	xCH__PROC_INFO__PROCESS_STATUS->Set__DATA("Processing");

	int flag = _Fnc__CTC_SIM__PRO_START(p_variable,p_alarm, fa_report_flag);

	while(1)
	{
		Sleep(10);

		if(p_variable->Check__CTRL_ABORT() > 0)								break;
		if(xCH__PROC_INFO__PROCESS_STATUS->Check__DATA("Alarm") < 0)		break;
	}

	xCH__PROC_INFO__PROCESS_STATUS->Set__DATA("Idle");

	// ...
	{
		CString ch_data;

		ch_data.Format("%.3f", x_timer_ctrl->Get__CURRENT_TIME());
		sCH__TIME_ACT_TACK->Set__DATA(ch_data);

		ch_data = Macro__Get_Current_Time();
		sCH__TIME_ACT_END->Set__DATA(ch_data);
	}

	if(flag > 0)
	{
		sCH__TIME_ACT_RESULT->Set__DATA("OK");
	}
	else
	{
		if(flag == -10001)			sCH__TIME_ACT_RESULT->Set__DATA("ABORT");
		else						sCH__TIME_ACT_RESULT->Set__DATA("ERROR");
	}

	_Process_Report__Wafer_Info();
	return flag;
}
int  CObj_Phy__PMC_STD
::_Fnc__CTC_SIM__PRO_START(CII_OBJECT__VARIABLE* p_variable, 
						   CII_OBJECT__ALARM *p_alarm,
						   const int fa_report_flag)
{
	xCH__APP_FLAG_ABORT_BY_USER->Set__DATA("");
	xCH__OBJ_FNC_MSG->Set__DATA("Processing");

	// PRC START ...
	{
		CString var_data;

		xCH__RECIPE->Get__DATA(var_data);
		xCH__FDC_RECIPE->Set__DATA(var_data);

		xCH__LOTID->Get__DATA(var_data);
		xCH__FDC_LOTID->Set__DATA(var_data);
	}
	// PRE - INFO ...
	{
		CString var_data;
		
		xCH__LOTID->Get__DATA(var_data);
		xCH_PRE__LOTID->Set__DATA(var_data);

		xCH__MATERIAL_ID->Get__DATA(var_data);
		xCH_PRE__MATERIAL_ID->Set__DATA(var_data);

		xCH_PRE__LPx_PORT_ID->Set__DATA("");
		xCH_PRE__LPx_SLOT_ID->Set__DATA("");
		xCH_PRE__LPx_CID->Set__DATA("");
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
	int pmc_slot_id = -1;
	
	int material_port_id;
	int material_slot_id;
	CString material_ppid;
	CString material_recipe;
	CString pm_log_path;

	// ...
	{
		CString log_msg;
		CString log_bff;

		log_msg = "PM SLOT INFO. ----------------------\n";

		//
		IDS__SCH_MATERIAL_INFO db_info;
		CString sch_name;

		CString material_lotid;
		int slot_lot_count = 0;
		int slot_check = -1;

		for(i=0; i<iPMx_SLOT_MAX; i++)
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
					if(xSCH_MATERIAL_CTRL->Get__MATERIAL_INFO(sch_name,db_info) < 0)
					{
						sch_name.Format("PM%1d-%1d",iPMx_ID,1);
						xSCH_MATERIAL_CTRL->Get__MATERIAL_INFO(sch_name,db_info);
					}

					material_port_id = db_info.iSRC__PTN;
					material_slot_id = db_info.iSRC__STN;
				}

				material_lotid  = db_info.sLOT_ID;
				material_ppid   = db_info.sPPID;
				material_recipe = db_info.sCUR_RECIPE;
				
				l_pmc_slot_id.Add(pmc_slot_id);
				l_lotid.Add(material_lotid);
				l_portid.Add(material_port_id);
				l_slotid.Add(material_slot_id);

				// ...
				{
					log_bff.Format("   PM_SlotID(%1d)   \"P%1d:%002d\"   [%s][%s]\n",
								   pmc_slot_id,
								   material_port_id,
								   material_slot_id,
								   material_lotid,
								   material_recipe);
					log_msg += log_bff;
				}

				if(slot_check < 0)
				{
					slot_check = 1;
					
					CString var_data;

					// SLOT NO.
					var_data.Format("%1d",pmc_slot_id);
					xCH__SLOT_NO->Set__DATA(var_data);
	
					// LPx - PORT ID
					var_data.Format("%1d",material_port_id);
					xCH__LPx_PORT_ID->Set__DATA(var_data);
					xCH_PRE__LPx_PORT_ID->Set__DATA(var_data);

					// LPx - SLOT ID
					var_data.Format("%1d",material_slot_id);
					xCH__LPx_SLOT_ID->Set__DATA(var_data);
					xCH_PRE__LPx_SLOT_ID->Set__DATA(var_data);

					// LPx - CID
					xSCH_MATERIAL_CTRL->Get__LPx_CID(material_port_id,var_data);
					xCH__LPx_CID->Set__DATA(var_data);
					xCH_PRE__LPx_CID->Set__DATA(var_data);
				}

				Datalog__Write_Lot_Proc_Slot_Start(i,slot_lot_count++,pmc_slot_id,db_info,pm_log_path);

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
			else
			{

			}
		}
		
		xLOG_CTRL->WRITE__LOG(log_msg);
	}

	iPRC_ALARM_FLAG = -1;

	// ...
	SCX__ASYNC_TIMER_CTRL x_async_timer;

	x_async_timer->REGISTER__COUNT_CHANNEL_NAME(xCH__PRC_TIME_COUNT->Get__CHANNEL_NAME());
	x_async_timer->START__COUNT_UP(99999);

	double cfg_proc_sec = aCH__SCH_TEST_CFG_PMC_PROC_MAIN_SEC->Get__VALUE();
	double elapse_sec   = 0.0;

	if((dEXT_CH__ALL_PM_MARATHON_TEST_USE->Check__DATA(STR__ENABLE) > 0)
	&& (dEXT_CH__PM_ID_MARATHON_TEST_USE->Check__DATA(STR__YES) > 0))
	{
		cfg_proc_sec = aEXT_CH__PM_ID_MARATHON_TEST_WAIT_SEC->Get__VALUE();
	}

	CString ch_data;
	ch_data.Format("%.0f", cfg_proc_sec);
	xCH__TOTAL_PRC_TIME->Set__DATA(ch_data);

	while(1)
	{
		Sleep(100);

		elapse_sec = x_async_timer->Get__CURRENT_TIME();
		if(elapse_sec >= cfg_proc_sec)
		{
			break;			
		}
	
		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			CString log_msg;
			log_msg = "Seq__PRO_START() - Aborted By Upper_Object !!!";
	
			xLOG_CTRL->WRITE__LOG(log_msg);
			return -10001;
		}
	}

	// ...
	int report__proc_result = 0;

	// ...
	{
		int process_flag = -1;
		CString str_slot_sts;

		for(i=0; i<iPMx_SLOT_MAX; i++)
		{
			xCH__SLOT_STATUS[i]->Get__DATA(str_slot_sts);

			if((str_slot_sts.CompareNoCase(STR__NONE)    != 0)
			&& (str_slot_sts.CompareNoCase(STR__ABORTED) != 0))
			{
				CString pmc_sch_name;
				pmc_sch_name.Format("PM%1d-%1d",iPMx_ID,i+1);

				if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(pmc_sch_name) > 0)
					xCH__SLOT_STATUS[i]->Set__DATA(STR__PROCESSED);
				else
					xCH__SLOT_STATUS[i]->Set__DATA(STR__COMPLETED);

				process_flag = 1;
			}
		}

		if(process_flag > 0)
		{
			report__proc_result = 1;
			aCH__ACC_PROCESSED_COUNT->Inc__VALUE();
		}
	}

	// ...
	{
		CString def_name = "FLAG.ABORT_BY_USER";
		CString def_data;

		if(xCH__APP_FLAG_ABORT_BY_USER->Check__DATA(STR__YES) > 0)
		{
			def_data = STR__YES;
		}
		else
		{
			def_data = "";
		}

		xSCH_MATERIAL_CTRL->Set__MATERIAL_USER_INFO(material_port_id,
													material_slot_id,
													def_name, 
													def_data);
	}

	// ...
	{
		CString lotid;
		CString portid;
		int lp_slotid;

		int limit = l_lotid.GetSize();

		for(i=0; i<limit; i++)
		{
			lotid  = l_lotid[i];
			portid.Format("PORT%1d",(int) l_portid[i]);
			lp_slotid = (int) l_slotid[i];
			
			Datalog__Write_Lot_Proc_Slot_End(i+1,lotid,portid,lp_slotid);
		}
	}

	if(fa_report_flag > 0)
	{
		int material_port_id;
		int material_slot_id;
		int pmc_slot_id;

		int limit = l_pmc_slot_id.GetSize();
		int i;

		for(i=0; i<limit; i++)
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

	// LOT LOG ...
	if(report__proc_result != 0)
	{
		CDS__LOT_SLOT_INFO ds_info;

		int i_portid = material_port_id;
		int i_slotid = material_slot_id;

		// ...
		{
			ds_info.sLOTID     = xCH__LOTID->Get__STRING();
			ds_info.sWAFERID   = xCH__MATERIAL_ID->Get__STRING();
			ds_info.sROUTE_RCP = material_ppid;

			ds_info.sPMC_ID.Format("PM%1d", iPMx_ID);
			ds_info.sRCP_NAME = xCH__FDC_RECIPE->Get__STRING();
			ds_info.sRCP_TIME.Format("%.0f", elapse_sec);

			if(report__proc_result > 0)		ds_info.sPROC_RESULT = "OK";
			else							ds_info.sPROC_RESULT = "ERROR";
		}

		xI_LOT_LOG_CTRL->Register__SLOT_INFO(i_portid,i_slotid, ds_info);
	}

	// PRC END ...
	{
		sCH__PMC_SEND_APC_APPLY->Set__DATA("");
		sCH__PMC_SEND_APC_DATA->Set__DATA("");

		xCH__FDC_PRE_RECIPE->Set__DATA("");
		xCH__FDC_CLN_RECIPE->Set__DATA("");
		xCH__FDC_RECIPE->Set__DATA("");
		xCH__FDC_LOTID->Set__DATA("");
	}

	// End Time ...
	{
		CString pmc_name;
		pmc_name.Format("PM%1d",iPMx_ID);

		CString date_time;
		Macro__Get_Date_Time(date_time);

		CString var_data;
		xCH__SLOT_NO->Get__DATA(var_data);

		CString sch_name;
		sch_name.Format("%s-%1d", pmc_name,atoi(var_data));

		xI_LCD_DB_CTRL->Set__GLS__END_INFO(sch_name,pmc_name,date_time);
	}

	return 1;
}
int  CObj_Phy__PMC_STD
::Call__CTC_SIM__AUTO_PRO(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM *p_alarm)
{
	iFLAG__STEP_REPORT = 1;

	if(Call__CTC_SIM__PRO_READY(p_variable,p_alarm)   < 0)		return -1;
	if(Call__CTC_SIM__PRO_START(p_variable,p_alarm,1) < 0)		return -1;

	return 1;
}
int  CObj_Phy__PMC_STD
::Call__CTC_SIM__AUTO_PRO_EX(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM *p_alarm)
{
	iFLAG__STEP_REPORT = 1;

	// ...
	CString log_msg;
	CString log_bff;

	// ...
	{
		CStringArray l_para;
		CString para_data;

		p_variable->Get__FNC_PARAMETER(l_para);

		int limit = l_para.GetSize();
		int i;

		log_msg.Format("Para List : %1d \n", limit);

		for(i=0;i<limit;i++)
		{
			para_data = l_para[i];

			log_bff.Format("  * %1d)  %s \n", i,para_data);
			log_msg += log_bff;

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

		xLOG_CTRL->WRITE__LOG(log_msg);
	}

	// ...
	{
		CString ch_name;
		CString ch_data;

		log_msg = "Channel Info ... \n";

		// 1.
		{
			ch_name = xCH__SLOT_NO->Get__CHANNEL_NAME();
			xCH__SLOT_NO->Get__DATA(ch_data);

			log_bff.Format("  %s <- %s \n", ch_name,ch_data);
			log_msg += log_bff;
		}
		// 2.
		{
			ch_name = xCH__RECIPE->Get__CHANNEL_NAME();
			ch_data = xCH__RECIPE->Get__STRING();

			log_bff.Format("  %s <- %s \n", ch_name,ch_data);
			log_msg += log_bff;
		}
		// 3.
		{
			ch_name = xCH__LOTID->Get__CHANNEL_NAME();
			ch_data = xCH__LOTID->Get__STRING();

			log_bff.Format("  %s <- %s \n", ch_name,ch_data);
			log_msg += log_bff;
		}
		// 4.
		{
			ch_name = xCH__MATERIAL_ID->Get__CHANNEL_NAME();
			ch_data = xCH__MATERIAL_ID->Get__STRING();

			log_bff.Format("  %s <- %s \n", ch_name,ch_data);
			log_msg += log_bff;
		}

		xLOG_CTRL->WRITE__LOG(log_msg);
	}

	if(Seq__CTC_SIM__PRO_READY(p_variable,p_alarm) < 0)
	{
		return -11;
	}

	if(Seq__CTC_SIM__PRO_START(p_variable,p_alarm,1) < 0)
	{
		return -12;
	}

	xCH__X_PICK_FLAG->Set__DATA("YES");
	return 1;
}


int  CObj_Phy__PMC_STD
::Call__CTC_SIM__AUTO_DUMMY_CLN(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM *p_alarm)
{
	CString win_name;
	xEXT_CH_CFG__PMC_MSG_WIN_NAME->Get__DATA(win_name);
	xEXT_CH_CFG__PMC_MSG_WIN_CTRL->Set__DATA(win_name);

	if(Call__CTC_SIM__CLN_READY(p_variable,1) < 0)				return -1;
	if(Call__CTC_SIM__CLN_START(p_variable,p_alarm,1) < 0)		return -1;

	return 1;
}


int  CObj_Phy__PMC_STD
::Call__CTC_SIM__CLN_READY(CII_OBJECT__VARIABLE* p_variable,
						   const int dummy_cln_flag)
{
	Set__Cln_Ready_Parameter(p_variable,dummy_cln_flag);

	return 1;
}
int  CObj_Phy__PMC_STD
::Fnc__CTC_SIM__POST_CLN_READY(CII_OBJECT__VARIABLE* p_variable)
{
	if(iFLAG__WAC_PROC > 0)
	{
		CString var_data;

		// ...
		{
			xCH_PRE__LOTID->Get__DATA(var_data);
			xCH__LOTID->Set__DATA(var_data);

			xCH_PRE__MATERIAL_ID->Get__DATA(var_data);
			xCH__MATERIAL_ID->Set__DATA(var_data);

			xCH_PRE__LPx_PORT_ID->Get__DATA(var_data);
			xCH__LPx_PORT_ID->Set__DATA(var_data);

			xCH_PRE__LPx_SLOT_ID->Get__DATA(var_data);
			xCH__LPx_SLOT_ID->Set__DATA(var_data);

			xCH_PRE__LPx_CID->Get__DATA(var_data);
			xCH__LPx_CID->Set__DATA(var_data);				
		}
	}
	else
	{
		xCH__LOTID->Set__DATA("");
		xCH__MATERIAL_ID->Set__DATA("");
	}

	return 1;
}

int  CObj_Phy__PMC_STD
::Call__CTC_SIM__CLN_START(CII_OBJECT__VARIABLE* p_variable, 
						   CII_OBJECT__ALARM *p_alarm,
						   const int dummy_cln_flag)
{
	if(xCH_CFG__CLEAN_MODE->Check__DATA(STR__CLEAN_MODE__SYSTEM_IDLE) < 0)
	{
		xTIMER__SYS_IDLE->START__COUNT_DOWN(1.0);
	}

	Set__Cln_Ready_Parameter(p_variable,dummy_cln_flag);

	xCH__NEXT_PROCESS->Set__DATA("");
	xCH_CFG__CLEAN_COUNT->Set__DATA("0");
	xCH_CFG__CLEAN_RF_CUR_TIME->Set__DATA("0");

	xSCH_MATERIAL_CTRL->Clean_Start__PMC(iPMx_ID);

	// ...
	int i;

	for(i=0;i<iPMx_SLOT_MAX;i++)
	{
		if(xCH__SLOT_STATUS[i]->Check__DATA(STR__NONE) < 0)
		{
			xCH__SLOT_STATUS[i]->Set__DATA(STR__PROCESSING);
		}
	}

	// ...
	SCX__ASYNC_TIMER_CTRL x_async_timer;

	x_async_timer->REGISTER__COUNT_CHANNEL_NAME(xCH__PRC_TIME_COUNT->Get__CHANNEL_NAME());
	x_async_timer->START__COUNT_UP(99999);

	double cfg_proc_sec = aCH__SCH_TEST_CFG_PMC_PROC_POST_SEC->Get__VALUE();
	double elapse_sec   = 0.0;

	CString ch_data;
	ch_data.Format("%.0f", cfg_proc_sec);
	xCH__TOTAL_PRC_TIME->Set__DATA(ch_data);

	while(1)
	{
		Sleep(100);

		elapse_sec = x_async_timer->Get__CURRENT_TIME();
		if(elapse_sec >= cfg_proc_sec)
		{
			break;			
		}

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			// ...
			{
				CString log_msg;
				log_msg = "Call__CTC_SIM__CLN_START() - Aborted By Upper_Object !!!";

				xLOG_CTRL->WRITE__LOG(log_msg);
			}
			return -10001;
		}
	}

	// ...
	{
		for(i=0; i<iPMx_SLOT_MAX; i++)
		{
			if(xCH__SLOT_STATUS[i]->Check__DATA(STR__NONE) < 0)
			{
				CString pmc_sch_name;
				pmc_sch_name.Format("PM%1d-%1d",iPMx_ID,i+1);

				if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(pmc_sch_name) > 0)
					xCH__SLOT_STATUS[i]->Set__DATA(STR__PROCESSED);
				else
					xCH__SLOT_STATUS[i]->Set__DATA(STR__COMPLETED);
			}
		}
	}

	xSCH_MATERIAL_CTRL->Clean_End__PMC(iPMx_ID);

	// ...
	{
		xCH__RECIPE->Set__DATA("");

		for(i=0; i<CFG__PMx_MAX_SLOT; i++)
		{
			xCH__SLOT_RECIPE[i]->Set__DATA("");
		}
	}

	return 1;
}
int  CObj_Phy__PMC_STD
::Fnc__CTC_SIM__POST_CLN_START(CII_OBJECT__VARIABLE* p_variable,
					           CII_OBJECT__ALARM* p_alarm)
{
	if(iFLAG__WAC_PROC > 0)
	{
		CString var_data;

		// ...
		{
			xCH__RECIPE->Get__DATA(var_data);
			xCH__FDC_PRE_RECIPE->Set__DATA(var_data);
			xCH__FDC_CLN_RECIPE->Set__DATA(var_data);
			xCH__FDC_RECIPE->Set__DATA(var_data);

			xCH_PRE__LOTID->Get__DATA(var_data);
			xCH__LOTID->Set__DATA(var_data);
			xCH__FDC_LOTID->Set__DATA(var_data);

			xCH_PRE__MATERIAL_ID->Get__DATA(var_data);
			xCH__MATERIAL_ID->Set__DATA(var_data);

			xCH_PRE__LPx_PORT_ID->Get__DATA(var_data);
			xCH__LPx_PORT_ID->Set__DATA(var_data);

			xCH_PRE__LPx_SLOT_ID->Get__DATA(var_data);
			xCH__LPx_SLOT_ID->Set__DATA(var_data);

			xCH_PRE__LPx_CID->Get__DATA(var_data);
			xCH__LPx_CID->Set__DATA(var_data);
		}

		// ...
		{
			xCH_PRE__LOTID->Set__DATA("");		
			xCH_PRE__MATERIAL_ID->Set__DATA("");

			xCH_PRE__LPx_PORT_ID->Set__DATA("");
			xCH_PRE__LPx_SLOT_ID->Set__DATA("");
			xCH_PRE__LPx_CID->Set__DATA("");
		}

		// ...
		{
			CString str_lotid;
			CString str_lp_slotid;

			xCH__LOTID->Get__DATA(str_lotid);
			xCH__LPx_SLOT_ID->Get__DATA(str_lp_slotid);

			Datalog__Write_Lot_Clean_Slot_Start(str_lotid,str_lp_slotid);
		}
	}
	else
	{
		xCH__LOTID->Set__DATA("");
		xCH__MATERIAL_ID->Set__DATA("");
	}

	xSCH_MATERIAL_CTRL->Clean_Start__PMC(iPMx_ID);

	// ...
	SCX__ASYNC_TIMER_CTRL x_async_timer;

	x_async_timer->REGISTER__COUNT_CHANNEL_NAME(xCH__PRC_TIME_COUNT->Get__CHANNEL_NAME());
	x_async_timer->START__COUNT_UP(99999);

	double cfg_proc_sec = aCH__SCH_TEST_CFG_PMC_PROC_POST_SEC->Get__VALUE();
	double elapse_sec   = 0.0;

	if((dEXT_CH__ALL_PM_MARATHON_TEST_USE->Check__DATA(STR__ENABLE) > 0)
	&& (dEXT_CH__PM_ID_MARATHON_TEST_USE->Check__DATA(STR__YES) > 0))
	{
		cfg_proc_sec = aEXT_CH__PM_ID_MARATHON_TEST_WAIT_SEC->Get__VALUE();
	}

	CString ch_data;
	ch_data.Format("%.0f", cfg_proc_sec);
	xCH__TOTAL_PRC_TIME->Set__DATA(ch_data);

	while(1)
	{
		Sleep(100);

		elapse_sec = x_async_timer->Get__CURRENT_TIME();
		if(elapse_sec >= cfg_proc_sec)
		{
			break;			
		}

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			// ...
			{
				CString log_msg;
				log_msg = "Fnc__CTC_SIM__POST_CLN_START() - Aborted By Upper_Object !!!";

				xLOG_CTRL->WRITE__LOG(log_msg);
			}
			return -10001;
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

	// ...
	{
		CString str_lotid;
		CString str_lp_slotid;

		xCH__LOTID->Get__DATA(str_lotid);
		xCH__LPx_SLOT_ID->Get__DATA(str_lp_slotid);

		Datalog__Write_Lot_Clean_Slot_End(str_lotid,str_lp_slotid);
	}

	// ...
	{
		xCH__LOTID->Set__DATA("");
		xCH__MATERIAL_ID->Set__DATA("");

		xCH__LPx_PORT_ID->Set__DATA("");
		xCH__LPx_SLOT_ID->Set__DATA("");
		xCH__LPx_CID->Set__DATA("");

		xCH__FDC_PRE_RECIPE->Set__DATA("");
		xCH__FDC_CLN_RECIPE->Set__DATA("");
		xCH__FDC_RECIPE->Set__DATA("");		
		xCH__FDC_LOTID->Set__DATA("");
	}

	return 1;
}

int  CObj_Phy__PMC_STD
::Call__CTC_SIM__AUTO_CLN(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM *p_alarm)
{
	CString win_name;
	xEXT_CH_CFG__PMC_MSG_WIN_NAME->Get__DATA(win_name);
	xEXT_CH_CFG__PMC_MSG_WIN_CTRL->Set__DATA(win_name);

	if(Call__CTC_SIM__CLN_READY(p_variable) < 0)				return -1;
	if(Call__CTC_SIM__CLN_START(p_variable,p_alarm) < 0)		return -1;

	return 1;
}
int  CObj_Phy__PMC_STD
::Call__CTC_SIM__POST_CLN(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM *p_alarm)
{
	xCH__OBJ_STATUS->Set__DATA(STR__CTCINUSE);

	int r_flag = Seq__CTC_SIM__POST_CLN(p_variable, p_alarm);

	if(r_flag > 0)		xCH__OBJ_STATUS->Set__DATA(STR__STANDBY);
	else				xCH__OBJ_STATUS->Set__DATA(STR__MAINTMODE);

	return r_flag;
}
int  CObj_Phy__PMC_STD
::Seq__CTC_SIM__POST_CLN(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM *p_alarm)
{
	CString str__post_cln_rcp;
	xCH__POST_CLEAN_RECIPE->Get__DATA(str__post_cln_rcp);

	// ...
	{
		CString log_string;

		log_string.Format("Post Clean Recipe : [%s]",str__post_cln_rcp);
		xLOG_CTRL->WRITE__LOG(log_string);
	}

	// ...
	{
		xCH__RECIPE->Set__DATA(str__post_cln_rcp);

		if(Fnc__CTC_SIM__POST_CLN_READY(p_variable) < 0)
		{
			return -1;
		}
	}

	// ...
	{
		xCH__RECIPE->Set__DATA(str__post_cln_rcp);

		if(Fnc__CTC_SIM__POST_CLN_START(p_variable,p_alarm) < 0)
		{
			return -1;
		}
	}

	if(xCH__NEXT_PROCESS->Check__DATA(STR__LEAK_CHECK) > 0)
	{
		if(Call__LEAK_CHECK(p_variable) < 0)
		{
			return -11;
		}
	}
	return 1;
}


int  CObj_Phy__PMC_STD
::Call__CTC_SIM__PRE_RCP_READY(CII_OBJECT__VARIABLE* p_variable,
							   CII_OBJECT__ALARM* p_alarm,
							   const int dummy_pre_flag)
{
	Set__Pre_Ready_Parameter(p_variable,dummy_pre_flag);

	return 1;
}
int  CObj_Phy__PMC_STD
::Fnc__CTC_SIM__PRE_RCP_READY(CII_OBJECT__VARIABLE* p_variable)
{
	if(iFLAG__WAC_PROC > 0)
	{
		CString var_data;

		// ...
		{
			xCH_PRE__LOTID->Get__DATA(var_data);
			xCH__LOTID->Set__DATA(var_data);

			xCH_PRE__MATERIAL_ID->Get__DATA(var_data);
			xCH__MATERIAL_ID->Set__DATA(var_data);

			xCH_PRE__LPx_PORT_ID->Get__DATA(var_data);
			xCH__LPx_PORT_ID->Set__DATA(var_data);

			xCH_PRE__LPx_SLOT_ID->Get__DATA(var_data);
			xCH__LPx_SLOT_ID->Set__DATA(var_data);

			xCH_PRE__LPx_CID->Get__DATA(var_data);
			xCH__LPx_CID->Set__DATA(var_data);				
		}
	}
	else
	{
		xCH__LOTID->Set__DATA("");
		xCH__MATERIAL_ID->Set__DATA("");
	}

	return 1;
}
int  CObj_Phy__PMC_STD
::Call__CTC_SIM__PRE_RCP_START(CII_OBJECT__VARIABLE* p_variable,
							   CII_OBJECT__ALARM* p_alarm,
							   const int dummy_pre_flag)
{
	Set__Pre_Ready_Parameter(p_variable,dummy_pre_flag);

	xCH__NEXT_PROCESS->Set__DATA("");
	xCH_CFG__CLEAN_COUNT->Set__DATA("0");
	xCH_CFG__CLEAN_RF_CUR_TIME->Set__DATA("0");

	xSCH_MATERIAL_CTRL->Clean_Start__PMC(iPMx_ID);

	// ...
	int i;

	for(i=0; i<iPMx_SLOT_MAX; i++)
	{
		if(xCH__SLOT_STATUS[i]->Check__DATA(STR__NONE) < 0)
		{
			xCH__SLOT_STATUS[i]->Set__DATA(STR__PROCESSING);
		}
	}

	// ...
	SCX__ASYNC_TIMER_CTRL x_async_timer;

	x_async_timer->REGISTER__COUNT_CHANNEL_NAME(xCH__PRC_TIME_COUNT->Get__CHANNEL_NAME());
	x_async_timer->START__COUNT_UP(99999);

	double cfg_proc_sec = aCH__SCH_TEST_CFG_PMC_PROC_PRE_SEC->Get__VALUE();
	double elapse_sec   = 0.0;

	if((dEXT_CH__ALL_PM_MARATHON_TEST_USE->Check__DATA(STR__ENABLE) > 0)
	&& (dEXT_CH__PM_ID_MARATHON_TEST_USE->Check__DATA(STR__YES) > 0))
	{
		cfg_proc_sec = aEXT_CH__PM_ID_MARATHON_TEST_WAIT_SEC->Get__VALUE();
	}

	CString ch_data;
	ch_data.Format("%.0f", cfg_proc_sec);
	xCH__TOTAL_PRC_TIME->Set__DATA(ch_data);

	while(1)
	{
		Sleep(100);

		elapse_sec = x_async_timer->Get__CURRENT_TIME();
		if(elapse_sec >= cfg_proc_sec)
		{
			break;			
		}

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			// ...
			{
				CString log_msg;
				log_msg = "Call__CTC_SIM__PRE_RCP_START() - Aborted By Upper_Object !!!";

				xLOG_CTRL->WRITE__LOG(log_msg);
			}
			return -10001;
		}
	}

	// ...
	{
		for(i=0; i<iPMx_SLOT_MAX; i++)
		{
			if(xCH__SLOT_STATUS[i]->Check__DATA(STR__NONE) < 0)
			{
				CString pmc_sch_name;
				pmc_sch_name.Format("PM%1d-%1d",iPMx_ID,i+1);

				if(xSCH_MATERIAL_CTRL->Check__NEXT_PROCESS(pmc_sch_name) > 0)
					xCH__SLOT_STATUS[i]->Set__DATA(STR__PROCESSED);
				else
					xCH__SLOT_STATUS[i]->Set__DATA(STR__COMPLETED);
			}
		}
	}

	xSCH_MATERIAL_CTRL->Clean_End__PMC(iPMx_ID);

	// ...
	{
		xCH__RECIPE->Set__DATA("");

		for(i=0; i<CFG__PMx_MAX_SLOT; i++)
		{
			xCH__SLOT_RECIPE[i]->Set__DATA("");
		}
	}
	return 1;
}
int  CObj_Phy__PMC_STD
::Fnc__CTC_SIM__PRE_RCP_START(CII_OBJECT__VARIABLE* p_variable,
							  CII_OBJECT__ALARM* p_alarm)
{
	if(iFLAG__WAC_PROC > 0)
	{
		CString var_data;

		// ...
		{
			xCH__RECIPE->Get__DATA(var_data);
			xCH__FDC_PRE_RECIPE->Set__DATA(var_data);
			xCH__FDC_CLN_RECIPE->Set__DATA(var_data);
			xCH__FDC_RECIPE->Set__DATA(var_data);

			xCH_PRE__LOTID->Get__DATA(var_data);
			xCH__LOTID->Set__DATA(var_data);
			xCH__FDC_LOTID->Set__DATA(var_data);

			xCH_PRE__MATERIAL_ID->Get__DATA(var_data);
			xCH__MATERIAL_ID->Set__DATA(var_data);

			xCH_PRE__LPx_PORT_ID->Get__DATA(var_data);
			xCH__LPx_PORT_ID->Set__DATA(var_data);

			xCH_PRE__LPx_SLOT_ID->Get__DATA(var_data);
			xCH__LPx_SLOT_ID->Set__DATA(var_data);

			xCH_PRE__LPx_CID->Get__DATA(var_data);
			xCH__LPx_CID->Set__DATA(var_data);
		}

		// ...
		{
			xCH_PRE__LOTID->Set__DATA("");		
			xCH_PRE__MATERIAL_ID->Set__DATA("");

			xCH_PRE__LPx_PORT_ID->Set__DATA("");
			xCH_PRE__LPx_SLOT_ID->Set__DATA("");
			xCH_PRE__LPx_CID->Set__DATA("");
		}

		// ...
		{
			CString str_lotid;
			CString str_lp_slotid;

			xCH__LOTID->Get__DATA(str_lotid);
			xCH__LPx_SLOT_ID->Get__DATA(str_lp_slotid);

			// Datalog__Write_Lot_Pre_Rcp_Slot_Start(str_lotid,str_lp_slotid);
		}
	}
	else
	{
		xCH__LOTID->Set__DATA("");
		xCH__MATERIAL_ID->Set__DATA("");
	}

	// xSCH_MATERIAL_CTRL->Pre_Rcp_Start__PMC(iPMx_ID);

	// ...
	SCX__ASYNC_TIMER_CTRL x_async_timer;

	x_async_timer->REGISTER__COUNT_CHANNEL_NAME(xCH__PRC_TIME_COUNT->Get__CHANNEL_NAME());
	x_async_timer->START__COUNT_UP(99999);

	double cfg_proc_sec = aCH__SCH_TEST_CFG_PMC_PROC_PRE_SEC->Get__VALUE();
	double elapse_sec   = 0.0;

	if((dEXT_CH__ALL_PM_MARATHON_TEST_USE->Check__DATA(STR__ENABLE) > 0)
	&& (dEXT_CH__PM_ID_MARATHON_TEST_USE->Check__DATA(STR__YES) > 0))
	{
		cfg_proc_sec = aEXT_CH__PM_ID_MARATHON_TEST_WAIT_SEC->Get__VALUE();
	}

	CString ch_data;
	ch_data.Format("%.0f", cfg_proc_sec);
	xCH__TOTAL_PRC_TIME->Set__DATA(ch_data);

	while(1)
	{
		Sleep(100);

		elapse_sec = x_async_timer->Get__CURRENT_TIME();
		if(elapse_sec >= cfg_proc_sec)
		{
			break;			
		}

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			// ...
			{
				CString log_msg;
				log_msg = "Fnc__CTC_SIM__PRE_RCP_START() - Aborted By Upper_Object !!!";

				xLOG_CTRL->WRITE__LOG(log_msg);
			}
			return -10001;
		}
	}

	// xSCH_MATERIAL_CTRL->Pre_Rcp_End__PMC(iPMx_ID);

	// ...
	{
		xCH__RECIPE->Set__DATA("");

		for(int i=0;i<CFG__PMx_MAX_SLOT;i++)
		{
			xCH__SLOT_RECIPE[i]->Set__DATA("");
		}
	}

	// ...
	{
		CString str_lotid;
		CString str_lp_slotid;

		xCH__LOTID->Get__DATA(str_lotid);
		xCH__LPx_SLOT_ID->Get__DATA(str_lp_slotid);

		// Datalog__Write_Lot_Pre_Rcp_Slot_End(str_lotid,str_lp_slotid);
	}

	// ...
	{
		xCH__LOTID->Set__DATA("");
		xCH__MATERIAL_ID->Set__DATA("");

		xCH__LPx_PORT_ID->Set__DATA("");
		xCH__LPx_SLOT_ID->Set__DATA("");
		xCH__LPx_CID->Set__DATA("");

		xCH__FDC_PRE_RECIPE->Set__DATA("");
		xCH__FDC_CLN_RECIPE->Set__DATA("");
		xCH__FDC_RECIPE->Set__DATA("");		
		xCH__FDC_LOTID->Set__DATA("");
	}

	return 1;
}
int  CObj_Phy__PMC_STD
::Call__CTC_SIM__AUTO_PRE_RCP(CII_OBJECT__VARIABLE* p_variable,
							  CII_OBJECT__ALARM* p_alarm)
{
	CString str__pre_rcp;
	xCH__PRC_PRE_RECIPE->Get__DATA(str__pre_rcp);

	// ...
	{
		CString log_string;

		log_string.Format("Pre-Recipe : [%s]", str__pre_rcp);
		xLOG_CTRL->WRITE__LOG(log_string);
	}

	// ...
	{
		xCH__RECIPE->Set__DATA(str__pre_rcp);

		if(Fnc__CTC_SIM__POST_CLN_READY(p_variable) < 0)
		{
			return -1;
		}
	}

	// ...
	{
		xCH__RECIPE->Set__DATA(str__pre_rcp);

		if(Fnc__CTC_SIM__PRE_RCP_START(p_variable,p_alarm) < 0)
		{
			return -1;
		}
	}

	return 1;
}


int  CObj_Phy__PMC_STD
::Call__CTC_SIM__MNT_READY(CII_OBJECT__VARIABLE* p_variable)
{

	return 1;
}
int  CObj_Phy__PMC_STD
::Call__CTC_SIM__MNT_START(CII_OBJECT__VARIABLE* p_variable)
{

	return 1;
}
int  CObj_Phy__PMC_STD
::Call__CTC_SIM__AUTO_MNT(CII_OBJECT__VARIABLE* p_variable)
{
	Set__Prc_Ready_Parameter(p_variable);

	// ...
	SCX__ASYNC_TIMER_CTRL x_async_timer;

	x_async_timer->REGISTER__COUNT_CHANNEL_NAME(xCH__PRC_TIME_COUNT->Get__CHANNEL_NAME());
	x_async_timer->START__COUNT_UP(99999);

	double cfg_sec = 10.0;
	double cur_sec = 0.0;

	CString ch_data;
	ch_data.Format("%.0f", cfg_sec);
	xCH__TOTAL_PRC_TIME->Set__DATA(ch_data);

	while(1)
	{
		Sleep(100);

		cur_sec = x_async_timer->Get__CURRENT_TIME();
		if(cur_sec >= cfg_sec)
		{
			break;			
		}

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			// ...
			{
				CString log_msg;
				log_msg = "Call__CTC_SIM__AUTO_MNT() - Aborted By Upper_Object !!!";

				xLOG_CTRL->WRITE__LOG(log_msg);
			}
			return -10001;
		}
	}

	return 1;
}


int  CObj_Phy__PMC_STD
::Call__CTC_SIM__LEAK_CHECK(CII_OBJECT__VARIABLE* p_variable)
{
	SCX__ASYNC_TIMER_CTRL x_async_timer;

	x_async_timer->REGISTER__COUNT_CHANNEL_NAME(xCH__PRC_TIME_COUNT->Get__CHANNEL_NAME());
	x_async_timer->START__COUNT_UP(99999);

	double cfg_sec = aCH__SCH_TEST_CFG_PMC_LEAK_CHECK_SEC->Get__VALUE();
	double cur_sec = 0.0;

	CString ch_data;
	ch_data.Format("%.0f", cfg_sec);
	xCH__TOTAL_PRC_TIME->Set__DATA(ch_data);

	while(1)
	{
		Sleep(100);

		cur_sec = x_async_timer->Get__CURRENT_TIME();
		if(cur_sec >= cfg_sec)
		{
			break;			
		}

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			// ...
			{
				CString log_msg;
				log_msg = "Call__CTC_SIM__LEAK_CHECK() - Aborted By Upper_Object !!!";

				xLOG_CTRL->WRITE__LOG(log_msg);
			}
			return -10001;
		}
	}

	return 1;
}
int  CObj_Phy__PMC_STD
::Call__CTC_SIM__LEAK_CHECK_EX(CII_OBJECT__VARIABLE* p_variable)
{
	return Call__CTC_SIM__LEAK_CHECK(p_variable);
}


int  CObj_Phy__PMC_STD
::Call__CTC_SIM__AUTO_PM(CII_OBJECT__VARIABLE* p_variable)
{
	SCX__ASYNC_TIMER_CTRL x_async_timer;

	x_async_timer->REGISTER__COUNT_CHANNEL_NAME(xCH__PRC_TIME_COUNT->Get__CHANNEL_NAME());
	x_async_timer->START__COUNT_UP(99999);

	double cfg_sec = aCH__SCH_TEST_CFG_PMC_AUTO_PM_SEC->Get__VALUE();
	double cur_sec = 0.0;

	CString ch_data;
	ch_data.Format("%.0f", cfg_sec);
	xCH__TOTAL_PRC_TIME->Set__DATA(ch_data);

	while(1)
	{
		Sleep(100);

		cur_sec = x_async_timer->Get__CURRENT_TIME();
		if(cur_sec >= cfg_sec)
		{
			break;			
		}

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			// ...
			{
				CString log_msg;
				log_msg = "Call__CTC_SIM__AUTO_PM() - Aborted By Upper_Object !!!";

				xLOG_CTRL->WRITE__LOG(log_msg);
			}
			return -10001;
		}
	}

	return 1;
}
int  CObj_Phy__PMC_STD
::Call__CTC_SIM__AUTO_PM_EX(CII_OBJECT__VARIABLE* p_variable)
{
	return Call__CTC_SIM__AUTO_PM(p_variable);
}


int  CObj_Phy__PMC_STD
::Call__CTC_SIM__HEADER_RCP(CII_OBJECT__VARIABLE* p_variable)
{

	return 1;
}


int  CObj_Phy__PMC_STD
::Call__CTC_SIM__NEXT_PROC(CII_OBJECT__VARIABLE* p_variable, CII_OBJECT__ALARM *p_alarm)
{
	// ...
	{
		iFNC_STS = 1;

		xCH__SYSTEM_TOTAL_MATERIAL_COUNT->Set__DATA("-1");
		xCH__NEXT_PROCESS_REQ->Set__DATA("");
		xCH__NEXT_PROCESS_END_FLAG->Set__DATA("");
	}

	// ...
	CString next_mode = xCH__NEXT_PROCESS->Get__STRING();

	// ...
	{
		CString obj_msg;
		obj_msg.Format("[PM%1d] - [%s]  Starting ...", iPMx_ID,next_mode);
		xCH__OBJ_MSG->Set__DATA(obj_msg);

		CString log_msg;
		log_msg.Format("%s\n", obj_msg);
		xLOG_CTRL->WRITE__LOG(log_msg);
	}

	if(next_mode.CompareNoCase(STR__GOTO_MAINT) == 0)
	{
		xCH__NEXT_PROCESS->Set__DATA("");

		Call__CTC_SIM__SYSTEM_MAINT(p_variable);
	}
	else
	{	
		SCX__TIMER_CTRL x_timer_ctrl;
		x_timer_ctrl->REGISTER__ABORT_OBJECT(sObject_Name);

		x_timer_ctrl->WAIT(60.0);
	}

	// ...
	{
		CString obj_msg;
		obj_msg.Format("[PM%1d] - [%s]  Completed ...", iPMx_ID,next_mode);
		xCH__OBJ_MSG->Set__DATA(obj_msg);

		CString log_msg;
		log_msg.Format("%s\n", obj_msg);
		xLOG_CTRL->WRITE__LOG(log_msg);
	}

	// ...
	{
		xCH__NEXT_PROCESS_END_FLAG->Set__DATA("YES");
		xCH__NEXT_PROCESS_REQ->Set__DATA("");

		iFNC_STS = -1;
	}
	return 1;
}


int  CObj_Phy__PMC_STD
::Call__CTC_SIM__JOB_START_READY(CII_OBJECT__VARIABLE* p_variable)
{
	Save__JOB_START_READY();

	// ...
	SCX__ASYNC_TIMER_CTRL x_async_timer;

	x_async_timer->REGISTER__COUNT_CHANNEL_NAME(xCH__PRC_TIME_COUNT->Get__CHANNEL_NAME());
	x_async_timer->START__COUNT_UP(99999);

	double cfg_sec = aCH__SCH_TEST_CFG_PMC_JOB_START_READY_SEC->Get__VALUE();
	double cur_sec = 0.0;
	
	CString ch_data;
	ch_data.Format("%.0f", cfg_sec);
	xCH__TOTAL_PRC_TIME->Set__DATA(ch_data);

	while(1)
	{
		Sleep(100);

		cur_sec = x_async_timer->Get__CURRENT_TIME();
		if(cur_sec >= cfg_sec)
		{
			break;			
		}

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			CString log_msg;
			log_msg = "Call__CTC_SIM__JOB_START_READY() - Aborted By Upper_Object !!!";

			xLOG_CTRL->WRITE__LOG(log_msg);
			return -10001;
		}
	}

	return 1;
}
int  CObj_Phy__PMC_STD
::Call__CTC_SIM__JOB_END_READY(CII_OBJECT__VARIABLE* p_variable)
{
	SCX__ASYNC_TIMER_CTRL x_async_timer;

	x_async_timer->REGISTER__COUNT_CHANNEL_NAME(xCH__PRC_TIME_COUNT->Get__CHANNEL_NAME());
	x_async_timer->START__COUNT_UP(99999);

	double cfg_sec = aCH__SCH_TEST_CFG_PMC_JOB_END_READY_SEC->Get__VALUE();
	double cur_sec = 0.0;

	CString ch_data;
	ch_data.Format("%.0f", cfg_sec);
	xCH__TOTAL_PRC_TIME->Set__DATA(ch_data);

	while(1)
	{
		Sleep(100);

		cur_sec = x_async_timer->Get__CURRENT_TIME();
		if(cur_sec >= cfg_sec)
		{
			break;			
		}

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			CString log_msg;
			log_msg = "Call__CTC_SIM__JOB_END_READY() - Aborted By Upper_Object !!!";

			xLOG_CTRL->WRITE__LOG(log_msg);
			return -10001;
		}
	}

	return 1;
}


int  CObj_Phy__PMC_STD
::Call__CTC_SIM__IDLE_START(CII_OBJECT__VARIABLE* p_variable)
{
	SCX__ASYNC_TIMER_CTRL x_async_timer;

	x_async_timer->REGISTER__COUNT_CHANNEL_NAME(xCH__PRC_TIME_COUNT->Get__CHANNEL_NAME());
	x_async_timer->START__COUNT_UP(99999);

	double cfg_sec = aCH__SCH_TEST_CFG_PMC_IDLE_START_SEC->Get__VALUE();
	double cur_sec = 0.0;

	CString ch_data;
	ch_data.Format("%.0f", cfg_sec);
	xCH__TOTAL_PRC_TIME->Set__DATA(ch_data);

	while(1)
	{
		Sleep(100);

		cur_sec = x_async_timer->Get__CURRENT_TIME();
		if(cur_sec >= cfg_sec)
		{
			break;			
		}

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			CString log_msg;
			log_msg = "Call__CTC_SIM__IDLE_START() - Aborted By Upper_Object !!!";

			xLOG_CTRL->WRITE__LOG(log_msg);
			return -10001;
		}
	}

	return 1;
}
