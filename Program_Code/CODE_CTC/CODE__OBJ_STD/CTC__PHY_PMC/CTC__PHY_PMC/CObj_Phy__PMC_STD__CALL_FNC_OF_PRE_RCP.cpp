#include "StdAfx.h"
#include "CObj_Phy__PMC_STD.h"
#include "CObj_Phy__PMC_STD__ALID.h"

#include "Macro_Function.h"


#include "CMacro_FA.h"
extern CMacro_FA mFA_Link;


//--------------------------------------------------------------------------------------
int  CObj_Phy__PMC_STD
::Call__PRE_RCP_READY(CII_OBJECT__VARIABLE* p_variable,
					  const int dummy_pre_flag)
{
	Set__Pre_Ready_Parameter(p_variable, dummy_pre_flag);

	Fnc__SET_TIME();

	return xI_Module_Obj->Connect__Module_Obj("PRE_READY");
}

int  CObj_Phy__PMC_STD
::Call__PRE_RCP_START(CII_OBJECT__VARIABLE* p_variable,
					  CII_OBJECT__ALARM* p_alarm,
					  const int dummy_pre_flag)
{
	if(xCH_CFG__CLEAN_MODE->Check__DATA(STR__CLEAN_MODE__SYSTEM_IDLE) < 0)
	{
		xTIMER__SYS_IDLE->START__COUNT_DOWN(1.0);
	}

	Set__Pre_Ready_Parameter(p_variable, dummy_pre_flag);

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

	for(i=0; i<iPMx_SLOT_MAX; i++)
	{
		if(xCH__SLOT_STATUS[i]->Check__DATA(STR__NONE) < 0)
		{
			xCH__SLOT_STATUS[i]->Set__DATA(STR__PROCESSING);
		}
	}

	int flag = xI_Module_Obj->Connect__Module_Obj("PRE_START");
	if(flag < 0)
	{
		for(i=0; i<iPMx_SLOT_MAX; i++)
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

	x_timer_ctrl->STOP();

	// xSCH_MATERIAL_CTRL->Pre_Rcp_End__PMC(iPMx_ID);

	// ...
	{
		xCH__RECIPE->Set__DATA("");

		for(int i=0; i<CFG__PMx_MAX_SLOT; i++)
		{
			xCH__SLOT_RECIPE[i]->Set__DATA("");
		}
	}
	return flag;
}

int  CObj_Phy__PMC_STD
::Call__AUTO_PRE_RCP(CII_OBJECT__VARIABLE* p_variable,
					 CII_OBJECT__ALARM* p_alarm)
{

	if(Call__PRE_RCP_READY(p_variable) < 0)					return -1;
	if(Call__PRE_RCP_START(p_variable,p_alarm) < 0)			return -2;

	return 1;
}


//--------------------------------------------------------------------------------------
void CObj_Phy__PMC_STD
::Set__Pre_Ready_Parameter(CII_OBJECT__VARIABLE *p_variable,
						   const int dummy_pre_flag)
{
	// ...
	{
		xCH__LOTID->Set__DATA("");
		xCH__MATERIAL_ID->Set__DATA("");
	}

	// ...
	CString pre_rcp;

	if(dummy_pre_flag > 0)
	{
		xCH_CFG__DUMMY_PRE_RECIPE->Get__DATA(pre_rcp);
	}
	else
	{
		xCH__PRC_PRE_RECIPE->Get__DATA(pre_rcp);
	}

	xCH__FDC_PRE_RECIPE->Set__DATA(pre_rcp);
	xCH__RECIPE->Set__DATA(pre_rcp);

	// ...
	{
		printf("CObj_Phy__PMC_STD::Set__Pre_Ready_Parameter() ... \n");

		printf(" * dummy_pre_flag >- [%1d] \n", dummy_pre_flag);
		printf(" ** \"%s\" <- \"%s\" \n", 
		       xCH_CFG__DUMMY_PRE_RECIPE->Get__CHANNEL_NAME(),
		       xCH_CFG__DUMMY_PRE_RECIPE->Get__STRING());

		printf(" ** \"%s\" <- \"%s\" \n", 
			   xCH__PRC_PRE_RECIPE->Get__CHANNEL_NAME(),
			   xCH__PRC_PRE_RECIPE->Get__STRING());

		printf(" ** \"%s\" <- \"%s\" \n", 
			   xCH__RECIPE->Get__CHANNEL_NAME(),
			   xCH__RECIPE->Get__STRING());
	}
}
