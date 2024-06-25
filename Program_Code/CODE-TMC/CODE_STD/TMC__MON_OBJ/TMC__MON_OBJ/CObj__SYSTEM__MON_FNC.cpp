#include "StdAfx.h"
#include "CObj__SYSTEM.h"
#include "CObj__SYSTEM__DEF.h"
#include "CObj__SYSTEM__ALID.h"


// ...
void CObj__SYSTEM
::Mon__IO_MONITOR(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	int count__cda_flow = 0;
	int count__n2_flow = 0;
	int count__efem_door_close = 0;
	int count__ffu_diff_pressure = 0;


	while(1)
	{
		p_variable->Wait__SINGLE_OBJECT(0.1);


		if(iActive__SIM_MODE > 0)
		{
			if(bActive__DI_CDA_FLOW)				dEXT_CH__DI_CDA_FLOW->Set__DATA(STR__ON);
			if(bActive__DI_N2_FLOW)					dEXT_CH__DI_N2_FLOW->Set__DATA(STR__ON);
			if(bActive__DI_EFEM_DOOR_CLOSE)			dEXT_CH__DI_EFEM_DOOR_CLOSE->Set__DATA(STR__ON);

			if(bActive__AI_FFU_DIFF_PRESSURE)
			{
				double cfg__low_limit = aCH__CFG_FFU_DIFF_PRESSURE_LOW_LIMIT->Get__VALUE();
				cfg__low_limit = cfg__low_limit * 1.50;

				aEXT_CH__AI_FFU_DIFF_PRESSURE->Set__VALUE(cfg__low_limit);
			}
		}

		if(dCH__CFG_CHECK_CDA_FLOW->Check__DATA(STR__TRUE) > 0)
		{
			if(bActive__DI_CDA_FLOW)
			{
				if(dEXT_CH__DI_CDA_FLOW->Check__DATA(STR__ON) < 0)
				{
					count__cda_flow++;

					if(count__cda_flow > 4)
					{
						count__cda_flow = 0;

						// ...
						{
							int alarm_id = ALID__CDA_FLOW__DI_ERROR;
							CString err_msg;
							CString err_bff;
							CString r_act;

							err_bff.Format("  * channel name <- [%s] \n", dEXT_CH__DI_CDA_FLOW->Get__CHANNEL_NAME());
							err_msg += err_bff;

							p_alarm->Check__ALARM(alarm_id, r_act);
							p_alarm->Post__ALARM(alarm_id);
						}
					}
				}
				else
				{
					count__cda_flow = 0;
				}
			}
		}

		if(dCH__CFG_CHECK_N2_FLOW->Check__DATA(STR__TRUE) > 0)
		{
			if(bActive__DI_N2_FLOW)
			{
				if(dEXT_CH__DI_N2_FLOW->Check__DATA(STR__ON) < 0)
				{
					count__n2_flow++;

					if(count__n2_flow > 4)
					{
						count__n2_flow = 0;

						// ...
						{
							int alarm_id = ALID__N2_FLOW__DI_ERROR;
							CString err_msg;
							CString err_bff;
							CString r_act;

							err_bff.Format("  * channel name <- [%s] \n", dEXT_CH__DI_N2_FLOW->Get__CHANNEL_NAME());
							err_msg += err_bff;

							p_alarm->Check__ALARM(alarm_id, r_act);
							p_alarm->Post__ALARM(alarm_id);
						}
					}
				}
				else
				{
					count__n2_flow = 0;
				}
			}
		}

		if(dCH__CFG_CHECK_EFEM_DOOR_CLOSE->Check__DATA(STR__TRUE) > 0)
		{
			if(bActive__DI_EFEM_DOOR_CLOSE)
			{
				if(dEXT_CH__DI_EFEM_DOOR_CLOSE->Check__DATA(STR__ON) < 0)
				{
					count__efem_door_close++;

					if(count__efem_door_close > 4)
					{
						count__efem_door_close = 0;

						// ...
						{
							int alarm_id = ALID__EFEM_DOOR_CLOSE__DI_ERROR;
							CString err_msg;
							CString err_bff;
							CString r_act;

							err_bff.Format("  * channel name <- [%s] \n", dEXT_CH__DI_EFEM_DOOR_CLOSE->Get__CHANNEL_NAME());
							err_msg += err_bff;

							p_alarm->Check__ALARM(alarm_id, r_act);
							p_alarm->Post__ALARM(alarm_id);
						}
					}
				}
				else
				{
					count__efem_door_close = 0;
				}
			}
		}

		if(dCH__CFG_CHECK_FFU_DIFF_PRESSURE->Check__DATA(STR__TRUE) > 0)
		{
			if(bActive__AI_FFU_DIFF_PRESSURE)
			{
				double cur__diff_pressure = aEXT_CH__AI_FFU_DIFF_PRESSURE->Get__VALUE();
				double cfg__diff_pressure = aCH__CFG_FFU_DIFF_PRESSURE_LOW_LIMIT->Get__VALUE();

				if(cur__diff_pressure < cfg__diff_pressure)
				{
					count__ffu_diff_pressure++;

					if(count__ffu_diff_pressure > 4)
					{
						count__ffu_diff_pressure = 0;

						// ...
						{
							int alarm_id = ALID__FFU_DIFF_PRESSURE__LOW_LIMIT;
							CString err_msg;
							CString err_bff;
							CString r_act;

							err_bff.Format("The current diff-pressure of FFU is %.3f . \n", cur__diff_pressure);
							err_msg += err_bff;

							err_bff.Format("The config diff-pressure of FFU is %.3f . \n", cfg__diff_pressure);
							err_msg += err_bff;

							err_msg += "\n";

							err_bff.Format("  * channel name <- [%s] \n", aEXT_CH__AI_FFU_DIFF_PRESSURE->Get__CHANNEL_NAME());
							err_msg += err_bff;

							p_alarm->Check__ALARM(alarm_id, r_act);
							p_alarm->Post__ALARM_With_MESSAGE(alarm_id, err_msg);
						}
					}
				}
				else
				{
					count__ffu_diff_pressure = 0;
				}
			}
		}

		// ...
	}	
}

