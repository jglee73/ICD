#include "StdAfx.h"
#include "CObj__DUAL_ARM_STD.h"

#include "Macro_Function.h"


// ...
void CObj__DUAL_ARM_STD::
Fnc__PMx_LEAK_CHECK_CTRL(CII_OBJECT__VARIABLE *p_variable,
						 CII_OBJECT__ALARM *p_alarm)
{
	int flag_leak_check = -1;

	CStringArray l_pmc;
	int wfr_count;

	CString var_data;
	int i;

	// ...
	{
		wfr_count = xSCH_MATERIAL_CTRL->Check__MATERIAL_MORE_THAN_MOVE_COUNT(1);
		var_data.Format("%1d", wfr_count);
		xEXT_CH__TOTAL_MATERIAL_COUNT_IN_SYSTEM->Set__DATA(var_data);
	}

	for(i=0; i<iPMx_SIZE; i++)
	{
		if(xEXT_CH__CFG__PMx_USE[i]->Check__DATA(STR__ENABLE) < 0)
		{
			if(xEXT_CH__PMx_LEAK_CHECK_FLAG[i]->Check__DATA(STR__YES) > 0)
			{
				xEXT_CH__PMx_LEAK_CHECK_FLAG[i]->Set__DATA("");
				xCH__PMx__NEXT_PROCESS[i]->Set__DATA("");
			}
		}
		else
		{
			if(PMx__Check_CtcInUse_Or_Standby(i) < 0)
			{
				xEXT_CH__PMx_LEAK_CHECK_FLAG[i]->Set__DATA("");
				xCH__PMx__NEXT_PROCESS[i]->Set__DATA("");
			}
			else 
			{
				if(xEXT_CH__PMx_LEAK_CHECK_FLAG[i]->Check__DATA(STR__YES) > 0)
				{
					flag_leak_check = 1;
				}
			}
		}
	}

	/*
	if(xSCH_MATERIAL_CTRL->Get__MATERIAL_DB_SIZE() > 0)
	{
		int flag_leak_check = -1;

		for(i=0; i<iPMx_SIZE; i++)
		{
			if(PMx__Check_CtcInUse_Or_Standby(i) < 0)
			{
				xEXT_CH__PMx_LEAK_CHECK_FLAG[i]->Set__DATA("");
				xCH__PMx__NEXT_PROCESS[i]->Set__DATA("");
			}
			else
			{
				if(xCH__PMx__NEXT_PROCESS[i]->Check__DATA(STR__LEAK_CHECK) > 0)
				{
					flag_leak_check = 1;
					break;
				}
			}
		}

		if(flag_leak_check > 0)
		{
			continue;
		}
	}
	*/

	if(flag_leak_check > 0)
	{
		if(xEXT_CH__SYSTEM_LEAK_CHECK_REQ->Check__DATA(STR__YES) < 0)
		{
			xEXT_CH__SYSTEM_LEAK_CHECK_REQ->Set__DATA(STR__YES);
		}
	}
	else
	{
		if(xEXT_CH__SYSTEM_LEAK_CHECK_REQ->Check__DATA("") < 0)
		{
			xEXT_CH__SYSTEM_LEAK_CHECK_REQ->Set__DATA("");
		}
	}

	if(wfr_count > 0)
	{
		return;
	}
	
	for(i=0;i<iPMx_SIZE;i++)
	{
		if(xEXT_CH__PMx_LEAK_CHECK_FLAG[i]->Check__DATA(STR__YES) < 0)
		{
			continue;
		}

		if(PMx__Is_Available(i) < 0)
		{
			continue;
		}
		if(PMx__Check_CtcInUse_Or_Standby(i) < 0)
		{
			xEXT_CH__PMx_LEAK_CHECK_FLAG[i]->Set__DATA("");
			xCH__PMx__NEXT_PROCESS[i]->Set__DATA("");
			continue;
		}

		if(xCH__PMx__CFG_AUTO_LEAK_CHECK_MODE[i]->Check__DATA(STR__LEAK_CHECK) > 0)
		{
			if(xSCH_MATERIAL_CTRL->Get__MATERIAL_DB_SIZE() > 0)
			{
				xEXT_CH__PMx_LEAK_CHECK_FLAG[i]->Set__DATA("");

				// ...
				int flag__run = -1;

				if(PMx__Check_Empty__SlotStatus(i) > 0)
				{
					if(VAC_RB__Check_All_Empty() > 0)
					{
						flag__run = 1;
					}
					else
					{
						CString arm_type;

						if(VAC_RB__Get_Occupied__Arm_Type(arm_type) > 0)
						{
							CStringArray l_module;
							CStringArray l_recipe;

							xSCH_MATERIAL_CTRL->Get__CUR_PROCESS_INFO(arm_type,l_module,l_recipe);

							flag__run = 1;

							// ...
							CString cmp_module;

							int i_limit = l_module.GetSize();
							int i;

							for(i=0; i<i_limit; i++)
							{
								cmp_module.Format("PM%1d", i+1);

								if(cmp_module.CompareNoCase(l_module[i]) == 0)
								{
									flag__run = -1;
									break;
								}
							}
						}
					}

					// ...
				}

				// ...
				xCH__PMx__NEXT_PROCESS[i]->Set__DATA(STR__LEAK_CHECK);

				if(flag__run > 0)
				{
					if(pPMx__OBJ_CTRL[i] != NULL)
					{
						xEXT_CH__PMx_LEAK_CHECK_FLAG[i]->Set__DATA("");

						Run__PMx_OBJ(i, STR__LEAK_CHECK);
					}
				}
				break;
			}
			else
			{
				if(pPMx__OBJ_CTRL[i] != NULL)
				{
					xEXT_CH__PMx_LEAK_CHECK_FLAG[i]->Set__DATA("");
					
					Run__PMx_OBJ(i, STR__LEAK_CHECK);
				}
			}				
		}
		else
		{
			if(xSCH_MATERIAL_CTRL->Get__MATERIAL_DB_SIZE() > 0)
			{
				continue;
			}

			if(pPMx__OBJ_CTRL[i] != NULL)
			{
				xEXT_CH__PMx_LEAK_CHECK_FLAG[i]->Set__DATA("");

				Run__PMx_OBJ(i, STR__LEAK_CHECK);
			}
		}
	}

	// ...
}
