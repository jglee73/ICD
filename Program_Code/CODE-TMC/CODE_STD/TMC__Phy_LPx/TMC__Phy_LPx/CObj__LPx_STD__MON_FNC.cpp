#include "StdAfx.h"
#include "CObj__LPx_STD.h"
#include "CObj__LPx_STD__DEF.h"


//----------------------------------------------------------------------------------------------------
void CObj__LPx_STD
::Mon__IO_MONITOR(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	CString var_data;


	while(1)
	{
		p_variable->Wait__SINGLE_OBJECT(0.01);


		//
		sEXT_CH__LPx__CLAMP_STS->Get__DATA(var_data);
		sCH__CLAMP_STS->Set__DATA(var_data);

		sEXT_CH__LPx__FOUP_STS->Get__DATA(var_data);
		sCH__FOUP_STS->Set__DATA(var_data);

		sEXT_CH__LPx__FOUP_POS->Get__DATA(var_data);
		sCH__FOUP_POS->Set__DATA(var_data);

		sEXT_CH__LPx__DOOR_STS->Get__DATA(var_data);
		sCH__DOOR_STS->Set__DATA(var_data);

		sEXT_CH__TRANSFER_PIO_STATUS->Get__DATA(var_data);
		sCH__TRANSFER_PIO_STATUS->Set__DATA(var_data);

		// RELOAD FLAG ...
		{
			sCH__CFG_RELOAD_FLAG->Get__DATA(var_data);
			sEXT_CH__RELOAD_FLAG->Set__DATA(var_data);
		}

		// CHECK : ACTIVE FA AUTO ...
		{
			bool active__fa_auto = false;

			CString obj_state = dEXT_CH__OBJ_STATE->Get__STRING();

			if((obj_state.CompareNoCase(STR__STANDBY)  == 0)
			|| (obj_state.CompareNoCase(STR__CTCINUSE) == 0))
			{
				if((sCH__CFG_FA_MODE->Check__DATA(STR__AUTO) > 0)
				&& (sCH__CFG_FA_SERVICE_MODE->Check__DATA(STR__YES) > 0))
				{
					active__fa_auto = true;
				}
			}

			if(active__fa_auto)			sCH__MON_ACTIVE_FA_AUTO->Set__DATA(STR__YES);	
			else						sCH__MON_ACTIVE_FA_AUTO->Set__DATA(STR__NO);

			//
			var_data = sCH__MON_ACTIVE_FA_AUTO->Get__STRING();

			sEXT_CH__LPx__MON_ACTIVE_FA_AUTO->Set__DATA(var_data);
			if(bActive__RFx_OBJ)		sEXT_CH__RFx__MON_ACTIVE_FA_AUTO->Set__DATA(var_data);
		}

		// ...
	}	
}
