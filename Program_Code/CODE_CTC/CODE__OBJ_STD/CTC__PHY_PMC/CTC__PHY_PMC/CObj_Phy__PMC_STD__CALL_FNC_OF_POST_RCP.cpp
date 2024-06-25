#include "StdAfx.h"
#include "CObj_Phy__PMC_STD.h"
#include "CObj_Phy__PMC_STD__ALID.h"

#include "Macro_Function.h"


#include "CMacro_FA.h"
extern CMacro_FA mFA_Link;


//--------------------------------------------------------------------------------------
int  CObj_Phy__PMC_STD
::Call__CLN_READY(CII_OBJECT__VARIABLE* p_variable,
				  const int dummy_cln_flag)
{
	Set__Cln_Ready_Parameter(p_variable,dummy_cln_flag);

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
		r_flag = xI_Module_Obj->Connect__Module_Obj("CLN_READY");
	}

	return r_flag;
}
int  CObj_Phy__PMC_STD
::Fnc__POST_CLN_READY(CII_OBJECT__VARIABLE* p_variable)
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
		r_flag = xI_Module_Obj->Connect__Module_Obj("CLN_READY");
	}

	return r_flag;
}

int  CObj_Phy__PMC_STD
::Call__CLN_START(CII_OBJECT__VARIABLE* p_variable,
				  CII_OBJECT__ALARM* p_alarm,
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
	SCX__ASYNC_TIMER_CTRL x_timer_ctrl;

	x_timer_ctrl->REGISTER__COUNT_CHANNEL_NAME(xCH__PRC_TIME_COUNT->Get__CHANNEL_NAME());
	x_timer_ctrl->START__COUNT_UP(99999);

	// ...
	int i;

	for(i=0;i<iPMx_SLOT_MAX;i++)
	{
		if(xCH__SLOT_STATUS[i]->Check__DATA(STR__NONE) < 0)
		{
			xCH__SLOT_STATUS[i]->Set__DATA(STR__PROCESSING);
		}
	}

	int flag = xI_Module_Obj->Connect__Module_Obj("CLN_START");
	if(flag < 0)
	{
		for(i=0;i<iPMx_SLOT_MAX;i++)
		{
			if(xCH__SLOT_STATUS[i]->Check__DATA(STR__NONE) < 0)
			{
				xCH__SLOT_STATUS[i]->Set__DATA(STR__ABORTED);
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

	x_timer_ctrl->STOP();

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
int  CObj_Phy__PMC_STD
::Fnc__POST_CLN_START(CII_OBJECT__VARIABLE* p_variable,
					  CII_OBJECT__ALARM* p_alarm)
{
	if(iFLAG__WAC_PROC > 0)
	{
		CString var_data;

		// ...
		{
			xCH__RECIPE->Get__DATA(var_data);
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
	// ...
	SCX__ASYNC_TIMER_CTRL x_timer_ctrl;

	x_timer_ctrl->REGISTER__COUNT_CHANNEL_NAME(xCH__PRC_TIME_COUNT->Get__CHANNEL_NAME());
	x_timer_ctrl->START__COUNT_UP(99999);

	// ...
	int flag = 1;
	int retry_count = 0;

	while(1)
	{
		SCX__ASYNC_TIMER_CTRL x_async_timer;
		x_async_timer->START__COUNT_UP(9999);

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
			flag = xI_Module_Obj->Connect__Module_Obj("CLN_START");
		}
		
		// ...
		double elapse_sec = x_async_timer->Get__CURRENT_TIME();

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
				log_msg = "Fnc__POST_CLN_START() - Aborted By Upper_Object !!!";

				xLOG_CTRL->WRITE__LOG(log_msg);
			}
			return -10001;
		}

		retry_count++;

		// ...
		{
			CString log_msg;
			log_msg.Format("Fnc__POST_CLN_START() - Retry Count (%1d)", retry_count);

			xLOG_CTRL->WRITE__LOG(log_msg);
		}

		// ...
		{
			int alarm_id = ALID__PMC_CLN_PROC_START_RETRY;
			CString alm_msg;
			CString r_act;

			alm_msg.Format("Command Elapse Time <- (%.3f) sec \n", elapse_sec);

			p_alarm->Check__ALARM(alarm_id,r_act);
			p_alarm->Post__ALARM_With_MESSAGE(alarm_id,alm_msg);
		}
	}

	x_timer_ctrl->STOP();

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

	return flag;
}

int  CObj_Phy__PMC_STD
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
int  CObj_Phy__PMC_STD
::Call__POST_CLN(CII_OBJECT__VARIABLE* p_variable,
				 CII_OBJECT__ALARM* p_alarm)
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

		if(Fnc__POST_CLN_READY(p_variable) < 0)
		{
			return -1;
		}
	}

	// ...
	{
		xCH__RECIPE->Set__DATA(str__post_cln_rcp);

		if(Fnc__POST_CLN_START(p_variable,p_alarm) < 0)
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
::Call__AUTO_DUMMY_CLN(CII_OBJECT__VARIABLE* p_variable,
					   CII_OBJECT__ALARM* p_alarm)
{
	CString win_name;
	xEXT_CH_CFG__PMC_MSG_WIN_NAME->Get__DATA(win_name);
	xEXT_CH_CFG__PMC_MSG_WIN_CTRL->Set__DATA(win_name);

	if(Call__CLN_READY(p_variable,1) < 0)			return -1;
	if(Call__CLN_START(p_variable,p_alarm,1) < 0)	return -1;

	return 1;
}

int  CObj_Phy__PMC_STD
::Call__LAYER_CLN_READY(CII_OBJECT__VARIABLE* p_variable)
{
	Set__Layer_Cln_Ready_Parameter(p_variable);

	Fnc__SET_TIME();

	return xI_Module_Obj->Connect__Module_Obj("CLN_READY");
}
int  CObj_Phy__PMC_STD
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

	int flag = xI_Module_Obj->Connect__Module_Obj("CLN_START");
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
int  CObj_Phy__PMC_STD
::Call__AUTO_LAYER_CLN(CII_OBJECT__VARIABLE* p_variable,
					   CII_OBJECT__ALARM* p_alarm)
{
	if(Call__LAYER_CLN_READY(p_variable) < 0)				return -1;
	if(Call__LAYER_CLN_START(p_variable,p_alarm) < 0)		return -1;

	return 1;
}


// ...
void CObj_Phy__PMC_STD
::Set__Cln_Ready_Parameter(CII_OBJECT__VARIABLE *p_variable,
						   const int dummy_cln_flag)
{
	// ...
	{
		// LOT ID
		xCH__LOTID->Set__DATA("");

		// MATERIAL ID
		xCH__MATERIAL_ID->Set__DATA("");
	}

	CString cln_rcp;

	if(dummy_cln_flag > 0)
	{
		xCH_CFG__DUMMY_CLEAN_RECIPE->Get__DATA(cln_rcp);
	}
	else
	{
		if(xCH__NEXT_PROCESS->Check__DATA(STR__CLEAN_RF) > 0)
		{
			xCH_CFG__CLEAN_RF_RECIPE->Get__DATA(cln_rcp);
		}
		else
		{
			xCH_CFG__CLEAN_RECIPE->Get__DATA(cln_rcp);
		}
	}

	xCH__FDC_CLN_RECIPE->Set__DATA(cln_rcp);
	xCH__RECIPE->Set__DATA(cln_rcp);
}
void CObj_Phy__PMC_STD
::Set__Layer_Cln_Ready_Parameter(CII_OBJECT__VARIABLE *p_variable)
{
	// LOT ID
	xCH__LOTID->Set__DATA("");

	// MATERIAL ID
	xCH__MATERIAL_ID->Set__DATA("");

	// RECIPE
	CString cln_rcp;
	xCH__LAYER_CLEAN_RECIPE->Get__DATA(cln_rcp);

	xCH__RECIPE->Set__DATA(cln_rcp);
}
