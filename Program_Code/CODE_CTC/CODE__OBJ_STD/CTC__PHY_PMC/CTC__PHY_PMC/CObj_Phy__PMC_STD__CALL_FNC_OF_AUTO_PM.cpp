#include "StdAfx.h"
#include "CObj_Phy__PMC_STD.h"
#include "CObj_Phy__PMC_STD__ALID.h"

#include "Macro_Function.h"


#include "CMacro_FA.h"
extern CMacro_FA mFA_Link;


//--------------------------------------------------------------------------------------
int  CObj_Phy__PMC_STD
::Call__AUTO_PM_EX(CII_OBJECT__VARIABLE* p_variable)
{
	return Call__AUTO_PM(p_variable);
}
int  CObj_Phy__PMC_STD
::Call__AUTO_PM(CII_OBJECT__VARIABLE* p_variable)
{
	xEXT_CH__PMC_AUTO_PM_FLAG->Set__DATA("CHECKING");

	int flag = Fnc__AUTO_PM(p_variable);

	xEXT_CH__PMC_AUTO_PM_FLAG->Set__DATA("");	
	return flag;
}
int  CObj_Phy__PMC_STD
::Fnc__AUTO_PM(CII_OBJECT__VARIABLE* p_variable)
{
	if(xCH_CFG__AUTO_PM_USE_MODE->Check__DATA("ENABLE") < 0)
	{
		return 1;
	}

	// ...
	{
		SCX__SEQ_INFO x_seq_info;

		CString var_data;
		int i_year;
		int i_mon;
		int i_day;

		x_seq_info->Get__DAY(i_year,i_mon,i_day);
		var_data.Format("%00004d.%002d.%002d", i_year,i_mon,i_day);

		xCH__AUTO_PM_RUN_DATE->Set__DATA(var_data);
		xCH__AUTO_PM_RUN_COMPLETE->Set__DATA(STR__YES);
	}

	// ...
	CString win_name;
	xEXT_CH_CFG__PMC_MSG_WIN_NAME->Get__DATA(win_name);
	xEXT_CH_CFG__PMC_MSG_WIN_CTRL->Set__DATA(win_name);

	// ...
	{
		xCH_CFG__AUTO_PM_CUR_TIME->Set__DATA("0");
		xCH__RECIPE->Set__DATA("AUTO_PM");
	}

	// xSCH_MATERIAL_CTRL->AutoPM_Start__PMC(iPMx_ID);

	// ...
	int flag = xI_Module_Obj->Connect__Module_Obj("AUTO_PM");

	if(flag > 0)
	{
		flag = xI_Module_Obj->Connect__Module_Obj("PUMP");
	}

	// xSCH_MATERIAL_CTRL->AutoPM_End__PMC(iPMx_ID);

	// ...
	{
		xCH__NEXT_PROCESS->Set__DATA("");

		xCH_CFG__AUTO_PM_CUR_TIME->Set__DATA("0");
		xCH__RECIPE->Set__DATA("");
	}
	return flag;
}
