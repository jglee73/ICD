#include "StdAfx.h"

#include "CObj__DUAL_ARM_STD.h"
#include "CObj__DUAL_ARM_STD__DEF.h"

#include "Macro_Function.h"


// ...
int  CObj__DUAL_ARM_STD
::Call__WAFER_RESYNC_CLEAR_SYS()
{
	// VAC_ROBOT ...
	{
		xCH__VAC_RB__SLOT01_STATUS->Set__DATA(STR__NONE);
		xCH__VAC_RB__SLOT02_STATUS->Set__DATA(STR__NONE);

		xCH__VAC_RB__SLOT01_TITLE->Set__DATA("");
		xCH__VAC_RB__SLOT02_TITLE->Set__DATA("");
	}

	Call__WAFER_RESYNC_CLEAR_PMx();
	Call__WAFER_RESYNC_CLEAR_LLx();
	return 1;
}
int  CObj__DUAL_ARM_STD
::Call__WAFER_RESYNC_CLEAR_PMx()
{
	// PMx ...
	{
		int i_limit = iPMx_SIZE;
		int i;

		for(i=0; i<i_limit; i++)
		{
			xCH__PMx__SLOT_STATUS[i]->Set__DATA(STR__NONE);
			xCH__PMx__SLOT_TITLE[i]->Set__DATA("");
		}
	}

	return 1;
}
int  CObj__DUAL_ARM_STD
::Call__WAFER_RESYNC_CLEAR_LLx()
{
	int ll_limit = iLLx_LIMIT;
	int ll_i;

	for(ll_i=0; ll_i<ll_limit; ll_i++)
	{
		// int i_limit = iLLx_SLOT_MAX[ll_i];
		int i_limit = CFG_LLx__SLOT_MAXx;

		for(int i=0; i<i_limit; i++)
		{
			xEXT_CH__LLx__SLOT_STATUS[ll_i][i]->Set__DATA(STR__NONE);
			xEXT_CH__LLx__SLOT_TITLE[ll_i][i]->Set__DATA("");
		}
	}

	return 1;	
}
