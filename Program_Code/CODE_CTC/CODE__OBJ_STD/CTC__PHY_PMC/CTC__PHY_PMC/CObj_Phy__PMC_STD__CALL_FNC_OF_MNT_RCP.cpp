#include "StdAfx.h"
#include "CObj_Phy__PMC_STD.h"
#include "CObj_Phy__PMC_STD__ALID.h"

#include "Macro_Function.h"


#include "CMacro_FA.h"
extern CMacro_FA mFA_Link;


//--------------------------------------------------------------------------------------
int  CObj_Phy__PMC_STD
::Call__MNT_READY(CII_OBJECT__VARIABLE* p_variable)
{
	Set__Prc_Ready_Parameter(p_variable);

	Fnc__SET_TIME();

	return xI_Module_Obj->Connect__Module_Obj("MNT_READY");
}
int  CObj_Phy__PMC_STD
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

	int flag = xI_Module_Obj->Connect__Module_Obj("MNT_START");
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
int  CObj_Phy__PMC_STD
::Call__AUTO_MNT(CII_OBJECT__VARIABLE* p_variable)
{
	if(Call__MNT_READY(p_variable) < 0)		return -1;
	if(Call__MNT_START(p_variable) < 0)		return -1;

	return 1;
}
