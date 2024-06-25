#include "StdAfx.h"
#include "CObj__DUAL_ARM_STD.h"
#include "CObj__DUAL_ARM_STD__DEF.h"

#include "Macro_Function.h"


// ...
int  CObj__DUAL_ARM_STD
::Call__WAFER_RESYNC_CLEAR_SYS()
{
	// ATM_ROBOT
	{
		xCH__ATM_RB__SLOT01_STATUS->Set__DATA(STR__NONE);
		xCH__ATM_RB__SLOT01_TITLE->Set__DATA("");

		xCH__ATM_RB__SLOT02_STATUS->Set__DATA(STR__NONE);
		xCH__ATM_RB__SLOT02_TITLE->Set__DATA("");
	}

	// AL1
	{
		xCH__AL1__SLOT01_STATUS->Set__DATA(STR__NONE);
		xCH__AL1__SLOT01_TITLE->Set__DATA("");

		xCH__AL1__SLOT02_STATUS->Set__DATA(STR__NONE);
		xCH__AL1__SLOT02_TITLE->Set__DATA("");
	}

	// LLx
	{
		int ll_limit = iLLx_SIZE;
		int ll_i;

		for(ll_i=0; ll_i<ll_limit; ll_i++)
		{
			int i_limit = iLLx_SLOT_MAX[ll_i];
			int i;

			for(i=0; i<i_limit; i++)
			{
				xEXT_CH__LLx__SLOT_STATUS[ll_i][i]->Set__DATA(STR__NONE);
				xEXT_CH__LLx__SLOT_TITLE[ll_i][i]->Set__DATA("");
			}
		}
	}

	// ST1
	{
		int i_limit = CFG_LP__SLOT_MAX;
		int i;

		for(i=0; i<i_limit; i++)
		{
			xEXT_CH__BUFFER1__SLOT_STATUS[i]->Set__DATA(STR__NONE);
			xEXT_CH__BUFFER1__SLOT_TITLE[i]->Set__DATA("");
			xEXT_CH__BUFFER1__SLOT_WAIT_SEC[i]->Set__DATA("");
		}
	}
	// ST2
	{
		int i_limit = CFG_LP__SLOT_MAX;
		int i;

		for(i=0; i<i_limit; i++)
		{
			xEXT_CH__BUFFER2__SLOT_STATUS[i]->Set__DATA(STR__NONE);
			xEXT_CH__BUFFER2__SLOT_TITLE[i]->Set__DATA("");
			xEXT_CH__BUFFER2__SLOT_WAIT_SEC[i]->Set__DATA("");
		}
	}

	return 1;
}
