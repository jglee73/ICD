#include "StdAfx.h"
#include "CObj_Phy__PMC_STD.h"
#include "CObj_Phy__PMC_STD__ALID.h"

#include "Macro_Function.h"


#include "CMacro_FA.h"
extern CMacro_FA mFA_Link;


//--------------------------------------------------------------------------------------
#define  _DEF__CHECK_DB_EMPTY								\
if(xSCH_MATERIAL_CTRL->Get__MATERIAL_DB_SIZE() < 1)			\
{															\
	return -1;												\
}

#define  _DEF__CHECK_SYSTEM_ABORT							\
if(xSCH_MATERIAL_CTRL->Check__SYSTEM_ABORT() > 0)			\
{															\
	return -1;												\
}


//--------------------------------------------------------------------------------------
int  CObj_Phy__PMC_STD
::Call__PRO_READY(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	Set__Prc_Ready_Parameter(p_variable);

	return Seq__PRO_READY(p_variable,p_alarm);
}
int  CObj_Phy__PMC_STD
::Seq__PRO_READY(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	// Start Time
	{
		CString pmc_name;
		pmc_name.Format("PM%1d",iPMx_ID);

		CString date_time;
		Macro__Get_Date_Time(date_time);

		//
		CString var_data;
		xCH__SLOT_NO->Get__DATA(var_data);

		CString sch_name;
		sch_name.Format("%s-%1d", pmc_name,atoi(var_data));

		//
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

	_DEF__CHECK_DB_EMPTY;
	_DEF__CHECK_SYSTEM_ABORT;

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
		r_flag = xI_Module_Obj->Connect__Module_Obj("PRO_READY");
	}

	return r_flag;
}

int  CObj_Phy__PMC_STD
::Call__PRO_START(CII_OBJECT__VARIABLE* p_variable,
				  CII_OBJECT__ALARM* p_alarm,
				  const int fa_report_flag)
{
	_DEF__CHECK_DB_EMPTY;
	_DEF__CHECK_SYSTEM_ABORT;

	CStringArray l_para;	
	p_variable->Get__FNC_PARAMETER(l_para);

	if(l_para.GetSize() > 0)
	{
		// RECIPE
		xCH__RECIPE->Set__DATA(l_para[0]);
	}

	return Seq__PRO_START(p_variable,p_alarm,fa_report_flag);
}
int  CObj_Phy__PMC_STD
::Seq__PRO_START(CII_OBJECT__VARIABLE* p_variable,
			     CII_OBJECT__ALARM* p_alarm,
			     const int fa_report_flag)
{
	SCX__ASYNC_TIMER_CTRL x_timer_ctrl;

	x_timer_ctrl->REGISTER__COUNT_CHANNEL_NAME(xCH__PRC_TIME_COUNT->Get__CHANNEL_NAME());
	x_timer_ctrl->START__COUNT_UP(99999);

	// ...
	{
		CString ch_data = Macro__Get_Current_Time();
		sCH__TIME_ACT_START->Set__DATA(ch_data);
	}

	int flag = _Seq__PRO_START(p_variable,p_alarm, fa_report_flag);

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
		if(flag == -10001)		sCH__TIME_ACT_RESULT->Set__DATA("ABORT");
		else					sCH__TIME_ACT_RESULT->Set__DATA("ERROR");
	}

	_Process_Report__Wafer_Info();
	return flag;
}
int  CObj_Phy__PMC_STD
::_Seq__PRO_START(CII_OBJECT__VARIABLE* p_variable,
				  CII_OBJECT__ALARM* p_alarm,
				  const int fa_report_flag)
{
	xCH__APP_FLAG_ABORT_BY_USER->Set__DATA("");

	if(xCH__CFG_2st_SLOT_EMPTY_CHECK->Check__DATA(STR__ENABLE) > 0)
	{
		CString str_msg;

		str_msg = "2st Slot Empty Check ...";
		xCH__OBJ_FNC_MSG->Set__DATA(str_msg);

		if(iPMx_SLOT_MAX > 1)
		{
LOOP_SEQ:

			int flag = xCH__SLOT_STATUS[1]->When__DATA("NONE",5);

			if(flag <= 0)
			{
				int alarm_id = ALID__2st_SLOT_NOT_EMPTY;
				CString r_act;

				p_alarm->Popup__ALARM(alarm_id,r_act);

				if(r_act.CompareNoCase("RETRY") == 0)
				{
					goto LOOP_SEQ;
				}
				else
				{
					str_msg = "2st Slot is not empty error ...";
					xCH__OBJ_FNC_MSG->Set__DATA(str_msg);
					return -1;
				}
			}

			if(p_variable->Check__CTRL_ABORT() > 0)
			{
				str_msg = "Ctrl Channel Aborted ...";
				xCH__OBJ_FNC_MSG->Set__DATA(str_msg);
				return -1;
			}
		}
	}

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
	int flag = 1;
	int retry_count = 0;

	double elapse_sec = 0.0;

	while(1)
	{
		SCX__ASYNC_TIMER_CTRL x_async_timer;
		x_async_timer->START__COUNT_UP(99999);

		// ...
		bool active__marathon_test = false;

		if((dEXT_CH__ALL_PM_MARATHON_TEST_USE->Check__DATA(STR__ENABLE) > 0)
		&& (dEXT_CH__PM_ID_MARATHON_TEST_USE->Check__DATA(STR__YES) > 0))
		{
			active__marathon_test = true;
		}

		if(active__marathon_test)
		{
			CString ch_data;
			
			aEXT_CH__PM_ID_MARATHON_TEST_WAIT_SEC->Get__DATA(ch_data);
			xCH__TOTAL_PRC_TIME->Set__DATA(ch_data);
			
			double cfg__wait_sec = atoi(ch_data);

			while(1)
			{
				Sleep(90);

				if(p_variable->Check__CTRL_ABORT() > 0)
				{
					flag = -1;
					break;
				}

				if(x_async_timer->Get__CURRENT_TIME() >= cfg__wait_sec)
				{
					flag = 1;
					break;
				}
			}
		}
		else
		{
			flag = xI_Module_Obj->Connect__Module_Obj("PRO_START");
			if(flag < 0)		break;
		}

		elapse_sec = x_async_timer->Get__CURRENT_TIME();
		if(elapse_sec > 0.9)
		{
			break;			
		}
	
		Sleep(100);

		if(p_variable->Check__CTRL_ABORT() > 0)
		{
			// ...
			{
				CString log_msg;
				log_msg = "Seq__PRO_START() - Aborted By Upper_Object !!!";
	
				xLOG_CTRL->WRITE__LOG(log_msg);
			}
			return -10001;
		}

		retry_count++;

		// ...
		{
			CString log_msg;
			log_msg.Format("Seq__PRO_START() - Retry Count (%1d)", retry_count);
			
			xLOG_CTRL->WRITE__LOG(log_msg);
		}

		// ...
		{
			int alarm_id = ALID__PMC_PROCESS_START_RETRY;
			CString alm_msg;
			CString r_act;

			alm_msg.Format("Command Elapse Time <- (%.3f) sec \n", elapse_sec);

			p_alarm->Check__ALARM(alarm_id,r_act);
			p_alarm->Post__ALARM_With_MESSAGE(alarm_id,alm_msg);
		}

		// jglee : 2023.07.04
		if(retry_count > 1)
		{
			xCH__OBJ_STATUS->Set__DATA(STR__MAINTMODE);

			flag = -101;
			break;
		}
	}

	// ...
	int report__proc_result = 0;

	if(flag < 0)
	{
		int abort_flag = -1;

		for(i=0; i<iPMx_SLOT_MAX; i++)
		{
			if(xCH__SLOT_STATUS[i]->Check__DATA(STR__NONE) < 0)
			{
				xCH__SLOT_STATUS[i]->Set__DATA(STR__ABORTED);
				abort_flag = 1;
			}
		}

		if(abort_flag > 0)
		{
			report__proc_result = -1;
			aCH__ACC_ABORTED_COUNT->Inc__VALUE();

			xSCH_MATERIAL_CTRL->PMC__Process_Fail(material_port_id,
												  material_slot_id,
												  iPMx_ID,
												  pm_log_path);
		}

		if(xCH__ALARM_LOG_MODE->Check__DATA("MATERIAL_ABORTED") > 0)
		{
			Post__PMx_ALARM(p_alarm);
		}
	}
	else
	{
		int process_flag = -1;
		CString str_slot_sts;

		for(i=0;i<iPMx_SLOT_MAX;i++)
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

		for(i=0;i<limit;i++)
		{
			lotid  = l_lotid[i];
			portid.Format("PORT%1d",(int) l_portid[i]);
			lp_slotid = (int) l_slotid[i];
			
			Datalog__Write_Lot_Proc_Slot_End(i+1,lotid,portid,lp_slotid);
		}
	}

	if((fa_report_flag > 0) && (flag > 0))
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

		CString var_data;
		xCH__SLOT_NO->Get__DATA(var_data);

		CString str__end_time;
		Macro__Get_Date_Time(str__end_time);

		CString sch_name;
		sch_name.Format("%s-%1d", pmc_name,atoi(var_data));

		xI_LCD_DB_CTRL->Set__GLS__END_INFO(sch_name,pmc_name, str__end_time);
	}

	return flag;
}

void CObj_Phy__PMC_STD
::_Process_Report__Wafer_Info()
{
	CString file_date;
	CString file_time;

	// ...
	{
		SYSTEMTIME file__sys_time;
		GetLocalTime(&file__sys_time);

		file_date.Format("%04d_%02d_%02d",		
						 file__sys_time.wYear,
						 file__sys_time.wMonth,
						 file__sys_time.wDay);

		file_time.Format("%02d:%02d:%02d",		
						 file__sys_time.wHour,
						 file__sys_time.wMinute,
						 file__sys_time.wSecond);
	}

	for(int i=0; i<iPMx_SLOT_MAX; i++)
	{
		if(xCH__SLOT_STATUS[i]->Check__DATA("NONE") > 0)
		{
			continue;
		}

		// ...
		CString sch_name;

		int slot_id = i + 1;
		sch_name.Format("PM%1d-%1d", iPMx_ID,slot_id);

		IDS__JOB_INFO job_info;
		if(xSCH_MATERIAL_CTRL->Get__JOB_INFO(sch_name, job_info) < 0)
		{
			continue;
		}

		int lp_id   = job_info.iPORT_ID;
		int lp_slot = job_info.iSLOT_ID;

		CString wfr_recipe = xCH__RECIPE->Get__STRING();
		CString wfr_log = xCH__WFR_LOG_INFO->Get__STRING();

		// ...
		CString log_msg;

		// ...
		{
			CString log_bff;

			log_msg  = "\n";
			log_msg += "#ACT.START!";
			log_msg += "\n";

			log_bff.Format("#ACT=%s\n", "PROCESS");
			log_msg += log_bff;

			log_bff.Format("#MD=PM%1d-%1d\n", iPMx_ID,slot_id);
			log_msg += log_bff;

			log_bff.Format("#RT_OFFSET=%s\n", xCH__FDC_DA_RT_OFFSET->Get__STRING());
			log_msg += log_bff;

			log_bff.Format("#RECIPE=%s\n", wfr_recipe);
			log_msg += log_bff;

			log_bff.Format("#LOG=%s\n", wfr_log);
			log_msg += log_bff;

			log_bff.Format("#TIME.ACT=%s\n", sCH__TIME_ACT_TACK->Get__STRING());
			log_msg += log_bff;

			log_bff.Format("#TIME.START=%s\n", sCH__TIME_ACT_START->Get__STRING());
			log_msg += log_bff;

			log_bff.Format("#TIME.END=%s\n", sCH__TIME_ACT_END->Get__STRING());
			log_msg += log_bff;

			log_bff.Format("#RESULT=%s\n", sCH__TIME_ACT_RESULT->Get__STRING());
			log_msg += log_bff;

			log_msg += "#ACT.END!";
			log_msg += "\n";
		}
		
		// ...
		{
			SCX__FILE_CTRL x_file_ctrl;
			x_file_ctrl->DISABLE__TIME_LOG();

			//
			CString ref_root;

			ref_root  = sDir_ROOT;
			ref_root += "\\EQP_LOG";
			x_file_ctrl->Create__LOG_DIR(ref_root);

			ref_root += "\\Wafer_Data";
			x_file_ctrl->Create__LOG_DIR(ref_root);

			//
			CString dir_date_lot;
			xSCH_MATERIAL_CTRL->Get__DIR_JOB_LOG(lp_id, dir_date_lot);

			ref_root += dir_date_lot;

			//
			CString file_path;
			file_path.Format("%s\\P%1d_%1d.wfr", ref_root,lp_id,lp_slot);		

			x_file_ctrl->Write__LOG_MSG(file_path, log_msg);
		}
	}
}

int  CObj_Phy__PMC_STD
::Call__AUTO_PRO(CII_OBJECT__VARIABLE* p_variable,
			     CII_OBJECT__ALARM* p_alarm)
{
	iFLAG__STEP_REPORT = 1;

	if(Call__PRO_READY(p_variable,p_alarm) < 0)			return -1;
	if(Call__PRO_START(p_variable,p_alarm,1) < 0)		return -1;

	return 1;
}
int  CObj_Phy__PMC_STD
::Call__AUTO_PRO_EX(CII_OBJECT__VARIABLE* p_variable,
					CII_OBJECT__ALARM* p_alarm)
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

	// Wafer.Check ...
	{
		bool active__wfr_exist = false;
		int i;

		for(i=0; i<iPMx_SLOT_MAX; i++)
		{
			if(xCH__SLOT_STATUS[i]->Check__DATA(STR__NONE) > 0)			continue;

			active__wfr_exist = true;
			break;;
		}

		if(active__wfr_exist)
		{
			if(Seq__PLACE_COMPLETE(p_variable) < 0)
			{
				return -11;
			}
		}
	}

	if(Seq__PRO_READY(p_variable,p_alarm) < 0)
	{
		return -12;
	}
	
	if(Seq__PRO_START(p_variable,p_alarm,1) < 0)
	{
		return -13;
	}

	xCH__X_PICK_FLAG->Set__DATA("YES");
	return 1;
}

int  CObj_Phy__PMC_STD
::Call__NEXT_PROC(CII_OBJECT__VARIABLE* p_variable,
				  CII_OBJECT__ALARM* p_alarm)
{
	// ...
	{
		iFNC_STS = 1;
		
		xCH__SYSTEM_TOTAL_MATERIAL_COUNT->Set__DATA("-1");
		xCH__NEXT_PROCESS_REQ->Set__DATA("");
		xCH__NEXT_PROCESS_END_FLAG->Set__DATA("");
	}
		
	CString next_mode;
	CString obj_msg;
	int flag = -1;
	
	// ...
	{
		xCH__NEXT_PROCESS->Get__DATA(next_mode);
		obj_msg.Format("[PM%1d] - [%s]  Starting ...",iPMx_ID,next_mode);

		xCH__OBJ_MSG->Set__DATA(obj_msg);
	}

	if((xCH__NEXT_PROCESS->Check__DATA(STR__CLEAN_COUNT) > 0)
	|| (xCH__NEXT_PROCESS->Check__DATA(STR__CLEAN_RF)    > 0))
	{	
		flag = Call__AUTO_CLN(p_variable,p_alarm);
	}
	else if(xCH__NEXT_PROCESS->Check__DATA(STR__LEAK_CHECK) > 0)
	{
		flag = Call__LEAK_CHECK(p_variable);
	}
	else if(xCH__NEXT_PROCESS->Check__DATA(STR__GOTO_MAINT) > 0)
	{
		xCH__NEXT_PROCESS->Set__DATA("");
		
		flag = Call__SYSTEM_MAINT(p_variable);
	}

	if(flag > 0)
	{
		obj_msg.Format("[PM%1d] - [%s]  Completed ...",iPMx_ID,next_mode);
	}
	else
	{
		obj_msg.Format("[PM%1d] - [%s]  Aborted ...",iPMx_ID,next_mode);
	}

	xCH__OBJ_MSG->Set__DATA(obj_msg);

	iFNC_STS = -1;

	xCH__NEXT_PROCESS_END_FLAG->Set__DATA("YES");
	xCH__NEXT_PROCESS_REQ->Set__DATA("");
	return flag;
}
