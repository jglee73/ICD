#include "StdAfx.h"
#include "CObj__DRV_LINK_ALx.h"
#include "CObj__DRV_LINK_ALx__DEF.h"
#include "CObj__DRV_LINK_ALx__ALID.h"


//------------------------------------------------------------------------------------
void CObj__DRV_LINK_ALx
::Mon__IO_MONITOR(CII_OBJECT__VARIABLE* p_variable,
				  CII_OBJECT__ALARM* p_alarm)
{

	SCX__ASYNC_TIMER_CTRL x_timer_ctrl;
	double sec_wait = 0.0;

	while(1)
	{
		p_variable->Wait__SINGLE_OBJECT(0.01);

		if(iActive__SIM_MODE > 0)
		{
			if(dCH__SIM_CFG__REAL_TEST->Check__DATA(STR__YES) > 0)			iCFG__SIM_MODE = -1;
			else															iCFG__SIM_MODE =  1;
		}
		else
		{
			iCFG__SIM_MODE = -1;
		}

		if(iCFG__SIM_MODE > 0)
		{
			if(sEXT_CH__ALx_CMMD_STS->Check__DATA(STR__RUN) > 0)
			{
				sEXT_CH__ALx_CMMD_STS->Set__DATA(STR__WAIT);

				sec_wait = aCH__SIM_CFG__ALIGN_TIME->Get__VALUE();
				x_timer_ctrl->START__COUNT_UP(sec_wait + 100);
			}
			else if(sEXT_CH__ALx_CMMD_STS->Check__DATA(STR__WAIT) > 0)
			{
				if(x_timer_ctrl->Get__CURRENT_TIME() >= sec_wait)
				{
					x_timer_ctrl->STOP();

					sEXT_CH__ALx_CMMD_STS->Set__DATA(STR__IDLE);
				}
			}
		}

		// ...
	}	
}
