#include "StdAfx.h"
#include "CObj__LPx_SYS.h"
#include "CObj__LPx_SYS__DEF.h"


// ...
void CObj__LPx_SYS
::Mon__IO_MONITOR(CII_OBJECT__VARIABLE* p_variable,CII_OBJECT__ALARM* p_alarm)
{
	int loop_count = 0;
	CString ch_data;


	while(1)
	{
		p_variable->Wait__SINGLE_OBJECT(0.01);

		loop_count++;
		if(loop_count > 10)		loop_count = 1;


		// LPx_INFO UPDATE ...
		{
			ch_data = sEXT_CH__LPx__CLAMP_STS->Get__STRING();
			dCH__MON_CLAMP_STS->Set__DATA(ch_data);

			ch_data = sEXT_CH__LPx__FOUP_STS->Get__STRING();
			dCH__MON_FOUP_STS->Set__DATA(ch_data);

			ch_data = sEXT_CH__LPx__FOUP_POS->Get__STRING();
			dCH__MON_FOUP_POS->Set__DATA(ch_data);

			ch_data = sEXT_CH__LPx__DOOR_STS->Get__STRING();
			dCH__MON_DOOR_STS->Set__DATA(ch_data);
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

			if(active__fa_auto)			dCH__LINK_PIO_ACTIVE_FA_AUTO->Set__DATA(STR__ON);	
			else						dCH__LINK_PIO_ACTIVE_FA_AUTO->Set__DATA(STR__OFF);
		}

		// State Update ...
		if(loop_count == 1)
		{
			Update__LOAD_POS_STATE();
		}
	}	
}

int  CObj__LPx_SYS::Update__LOAD_POS_STATE()
{

	if(dCH__CFG_LOAD_POS_STATE_USE->Check__DATA(STR__NO) > 0)
	{
		CString str__load_sts = STR__UNKNOWN;

		if((dCH__MON_DOOR_STS->Check__DATA(STR__OPEN)   > 0)
		&& (dCH__MON_FOUP_STS->Check__DATA(STR__EXIST)  > 0)
		&& (dCH__MON_CLAMP_STS->Check__DATA(STR__CLAMP) > 0))
		{
			str__load_sts = STR__LOAD;
		}
		else if((dCH__MON_DOOR_STS->Check__DATA(STR__CLOSE)  > 0)
			 && (dCH__MON_FOUP_STS->Check__DATA(STR__EXIST) > 0))
		{
			str__load_sts = STR__UNLOAD;
		}

		dCH__MON_FOUP_POS->Set__DATA(str__load_sts);
	}
	return 1;
}
